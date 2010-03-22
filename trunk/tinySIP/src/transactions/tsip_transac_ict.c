/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou@yahoo.fr>
*	
* This file is part of Open Source Doubango Framework.
*
* DOUBANGO is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*	
* DOUBANGO is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

/*=============================================================================
* IMPORTANT: The INVITE Client Transaction (ICT) implements corrections defined in 'draft-sparks-sip-invfix-02.txt'.
* which fixes RFC 3261. This will alow us to easily suppoort forking.

                                     |INVITE from TU
                   Timer A fires     |INVITE sent      Timer B fired
                   Reset A,          V                 or Transport Err.
                   INVITE sent +-----------+           inform TU
                     +---------|           |--------------------------+
                     |         |  Calling  |                          |
                     +-------->|           |-----------+              |
    300-699                    +-----------+ 2xx       |              |
    ACK sent                      |  |       2xx to TU |              |
    resp. to TU                   |  |1xx              |              |
    +-----------------------------+  |1xx to TU        |              |
    |                                |                 |              |
    |                1xx             V                 |              |
    |                1xx to TU +-----------+           |              |
    |                +---------|           |           |              |
    |                |         |Proceeding |           |              |
    |                +-------->|           |           |              |
    |                          +-----------+ 2xx       |              |
    |         300-699             |    |     2xx to TU |              |
    |         ACK sent,  +--------+    +---------------+              |
    |         resp. to TU|                             |              |
    |                    |                             |              |
    |                    V                             V              |
    |              +-----------+                   +----------+       |
    +------------->|           |Transport Err.     |          |       |
                   | Completed |Inform TU          | Accepted |       |
                +--|           |-------+           |          |-+     |
        300-699 |  +-----------+       |           +----------+ |     |
        ACK sent|    ^  |              |               |  ^     |     |
                |    |  |              |               |  |     |     |
                +----+  |              |               |  +-----+     |
                        |Timer D fires |  Timer M fires|    2xx       |
                        |-             |             - |    2xx to TU |
                        +--------+     |   +-----------+              |
       NOTE:                     V     V   V                          |
    transitions                 +------------+                        |
    labeled with                |            |                        |
    the event                   | Terminated |<-----------------------+
    over the action             |            |
    to take                     +------------+


                   	draft-sparks-sip-invfix-03.txt - Figure 3: INVITE client transaction

=============================================================================*/

/**@file tsip_transac_ict.c
 * @brief SIP INVITE Client Transaction as per RFC 3261 subclause 17.1.1.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinySIP/transactions/tsip_transac_ict.h"

#include "tsk_debug.h"

#define DEBUG_STATE_MACHINE						1

#define TRANSAC_ICT_TIMER_SCHEDULE(TX)			TRANSAC_TIMER_SCHEDULE(ict, TX)

/* ======================== internal functions ======================== */
int tsip_transac_ict_init(tsip_transac_ict_t *self);
int tsip_transac_ict_send_ack(const tsip_response_t* response);
int tsip_transac_ict_OnTerminated(tsip_transac_ict_t *self);

/* ======================== transitions ======================== */
int tsip_transac_ict_Started_2_Calling_X_send(va_list *app);
int tsip_transac_ict_Calling_2_Calling_X_timerA(va_list *app);
int tsip_transac_ict_Calling_2_Terminated_X_timerB(va_list *app);
int tsip_transac_ict_Calling_2_Completed_X_300_to_699(va_list *app);
int tsip_transac_ict_Calling_2_Proceeding_X_1xx(va_list *app);
int tsip_transac_ict_Calling_2_Accepted_X_2xx(va_list *app);
int tsip_transac_ict_Proceeding_2_Proceeding_X_1xx(va_list *app);
int tsip_transac_ict_Proceeding_2_Completed_X_300_to_699(va_list *app);
int tsip_transac_ict_Proceeding_2_Accepted_X_2xx(va_list *app);
int tsip_transac_ict_Completed_2_Completed_X_300_to_699(va_list *app);
int tsip_transac_ict_Completed_2_Terminated_X_timerD(va_list *app);
int tsip_transac_ict_Accepted_2_Accepted_X_2xx(va_list *app);
int tsip_transac_ict_Accepted_2_Terminated_X_timerM(va_list *app);
int tsip_transac_ict_Any_2_Terminated_X_transportError(va_list *app);
int tsip_transac_ict_Any_2_Terminated_X_Error(va_list *app);

/* ======================== conds ======================== */

/* ======================== actions ======================== */
typedef enum _fsm_action_e
{
	_fsm_action_send,
	_fsm_action_timerA,
	_fsm_action_timerB,
	_fsm_action_timerD,
	_fsm_action_timerM,
	_fsm_action_1xx,
	_fsm_action_2xx,
	_fsm_action_300_to_699,
	_fsm_action_transporterror,
	_fsm_action_error,
}
_fsm_action_t;

/* ======================== states ======================== */
typedef enum _fsm_state_e
{
	_fsm_state_Started,
	_fsm_state_Calling,
	_fsm_state_Proceeding,
	_fsm_state_Completed,
	_fsm_state_Accepted,
	_fsm_state_Terminated
}
_fsm_state_t;


/**
 * Callback function called by the transport layer to alert the transaction for incoming messages
 *			or errors (e.g. transport error).
 *
 * @param [in,out]	self	A pointer to the IC transaction. 
 * @param	type		The event type. 
 * @param [in,out]	msg	The incoming message.
 *
 * @return	Zero if succeed and no-zero error code otherwise. 
**/
int tsip_transac_ict_event_callback(const tsip_transac_ict_t *self, tsip_transac_event_type_t type, const tsip_message_t *msg)
{	
	int ret = 0;

	/*	draft-sparks-sip-invfix-03 - 7.2. UAC Impacts
		Any response received which does not match an existing client transaction state machine is simply dropped.
	*/
	switch(type)
	{
	case tsip_transac_incoming_msg:
		{
			if(msg && TSIP_MESSAGE_IS_RESPONSE(msg))
			{
				if(TSIP_RESPONSE_IS_1XX(msg)){
					ret = tsk_fsm_act(self->fsm, _fsm_action_1xx, self, msg, self, msg);
				}
				else if(TSIP_RESPONSE_IS_2XX(msg)){
					ret = tsk_fsm_act(self->fsm, _fsm_action_2xx, self, msg, self, msg);
				}
				else if(TSIP_RESPONSE_IS_3456(msg)){
					ret = tsk_fsm_act(self->fsm, _fsm_action_300_to_699, self, msg, self, msg);
				}
				else{
					TSK_DEBUG_WARN("Not supported status code: %d", TSIP_RESPONSE_CODE(msg));
				}
			}
			break;
		}

	case tsip_transac_canceled:
	case tsip_transac_terminated:
	case tsip_transac_timedout:
		break;

	case tsip_transac_error:
		{
			ret = tsk_fsm_act(self->fsm, _fsm_action_error, self, msg, self, msg);
			break;
		}

	case tsip_transac_transport_error:
		{
			ret = tsk_fsm_act(self->fsm, _fsm_action_transporterror, self, msg, self, msg);
			break;
		}
	}

	return ret;
}

int tsip_transac_ict_timer_callback(const tsip_transac_ict_t* self, tsk_timer_id_t timer_id)
{
	int ret = -1;

	if(self)
	{
		if(timer_id == self->timerA.id){
			ret = tsk_fsm_act(self->fsm, _fsm_action_timerA, self, tsk_null, self, tsk_null);
		}
		else if(timer_id == self->timerB.id){
			ret = tsk_fsm_act(self->fsm, _fsm_action_timerB, self, tsk_null, self, tsk_null);
		}
		else if(timer_id == self->timerD.id){
			ret = tsk_fsm_act(self->fsm, _fsm_action_timerD, self, tsk_null, self, tsk_null);
		}
		else if(timer_id == self->timerM.id){
			ret = tsk_fsm_act(self->fsm, _fsm_action_timerM, self, tsk_null, self, tsk_null);
		}
	}

	return ret;
}


/** Initializes the transaction.
 *
 * @author	Mamadou
 * @date	12/24/2009
 *
 * @param [in,out]	self	The transaction to initialize.
**/
int tsip_transac_ict_init(tsip_transac_ict_t *self)
{
	/* Initialize the state machine. */
	tsk_fsm_set(self->fsm,
			
			/*=======================
			* === Started === 
			*/
			// Started -> (Send) -> Calling
			TSK_FSM_ADD_ALWAYS(_fsm_state_Started, _fsm_action_send, _fsm_state_Calling, tsip_transac_ict_Started_2_Calling_X_send, "tsip_transac_ict_Started_2_Calling_X_send"),
			// Started -> (Any) -> Started
			TSK_FSM_ADD_ALWAYS_NOTHING(_fsm_state_Started, "tsip_transac_ict_Started_2_Started_X_any"),
			
			/*=======================
			* === Calling === 
			*/
			// Calling -> (timerA) -> Calling
			TSK_FSM_ADD_ALWAYS(_fsm_state_Calling, _fsm_action_timerA, _fsm_state_Calling, tsip_transac_ict_Calling_2_Calling_X_timerA, "tsip_transac_ict_Calling_2_Calling_X_timerA"),
			// Calling -> (timerB) -> Terminated
			TSK_FSM_ADD_ALWAYS(_fsm_state_Calling, _fsm_action_timerB, _fsm_state_Terminated, tsip_transac_ict_Calling_2_Terminated_X_timerB, "tsip_transac_ict_Calling_2_Terminated_X_timerB"),
			// Calling -> (300-699) -> Completed
			TSK_FSM_ADD_ALWAYS(_fsm_state_Calling, _fsm_action_300_to_699, _fsm_state_Completed, tsip_transac_ict_Calling_2_Completed_X_300_to_699, "tsip_transac_ict_Calling_2_Completed_X_300_to_699"),
			// Calling  -> (1xx) -> Proceeding
			TSK_FSM_ADD_ALWAYS(_fsm_state_Calling, _fsm_action_1xx, _fsm_state_Proceeding, tsip_transac_ict_Calling_2_Proceeding_X_1xx, "tsip_transac_ict_Calling_2_Proceeding_X_1xx"),
			// Calling  -> (2xx) -> Accepted
			TSK_FSM_ADD_ALWAYS(_fsm_state_Calling, _fsm_action_2xx, _fsm_state_Accepted, tsip_transac_ict_Calling_2_Accepted_X_2xx, "tsip_transac_ict_Calling_2_Accepted_X_2xx"),
			
			/*=======================
			* === Proceeding === 
			*/
			// Proceeding -> (1xx) -> Proceeding
			TSK_FSM_ADD_ALWAYS(_fsm_state_Proceeding, _fsm_action_1xx, _fsm_state_Proceeding, tsip_transac_ict_Proceeding_2_Proceeding_X_1xx, "tsip_transac_ict_Proceeding_2_Proceeding_X_1xx"),
			// Proceeding -> (300-699) -> Completed
			TSK_FSM_ADD_ALWAYS(_fsm_state_Proceeding, _fsm_action_300_to_699, _fsm_state_Completed, tsip_transac_ict_Proceeding_2_Completed_X_300_to_699, "tsip_transac_ict_Proceeding_2_Completed_X_300_to_699"),
			// Proceeding -> (2xx) -> Accepted
			TSK_FSM_ADD_ALWAYS(_fsm_state_Proceeding, _fsm_action_2xx, _fsm_state_Accepted, tsip_transac_ict_Proceeding_2_Accepted_X_2xx, "tsip_transac_ict_Proceeding_2_Accepted_X_2xx"),
			
			/*=======================
			* === Completed === 
			*/
			// Completed -> (300-699) -> Completed
			TSK_FSM_ADD_ALWAYS(_fsm_state_Completed, _fsm_action_300_to_699, _fsm_state_Completed, tsip_transac_ict_Completed_2_Completed_X_300_to_699, "tsip_transac_ict_Completed_2_Completed_X_300_to_699"),
			// Completed -> (timerD) -> Terminated
			TSK_FSM_ADD_ALWAYS(_fsm_state_Completed, _fsm_action_timerD, _fsm_state_Terminated, tsip_transac_ict_Completed_2_Terminated_X_timerD, "tsip_transac_ict_Completed_2_Terminated_X_timerD"),
				
			/*=======================
			* === Accepted === 
			*/
			// Accepted -> (2xx) -> Accepted
			TSK_FSM_ADD_ALWAYS(_fsm_state_Accepted, _fsm_action_2xx, _fsm_state_Accepted, tsip_transac_ict_Accepted_2_Accepted_X_2xx, "tsip_transac_ict_Accepted_2_Accepted_X_2xx"),
			// Accepted -> (timerM) -> Terminated
			TSK_FSM_ADD_ALWAYS(_fsm_state_Accepted, _fsm_action_timerM, _fsm_state_Terminated, tsip_transac_ict_Accepted_2_Terminated_X_timerM, "tsip_transac_ict_Accepted_2_Terminated_X_timerM"),
		
			/*=======================
			* === Any === 
			*/
			// Any -> (transport error) -> Terminated
			TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, _fsm_action_transporterror, _fsm_state_Terminated, tsip_transac_ict_Any_2_Terminated_X_transportError, "tsip_transac_ict_Any_2_Terminated_X_transportError"),
			// Any -> (transport error) -> Terminated
			TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, _fsm_action_error, _fsm_state_Terminated, tsip_transac_ict_Any_2_Terminated_X_Error, "tsip_transac_ict_Any_2_Terminated_X_Error"),

			
			TSK_FSM_ADD_NULL());


	/* Set callback function to call when new messages arrive or errors happen in
	the transport layer.
	*/
	TSIP_TRANSAC(self)->callback = TSIP_TRANSAC_EVENT_CALLBACK(tsip_transac_ict_event_callback);

	/* Timers */
	self->timerA.id = TSK_INVALID_TIMER_ID;
	self->timerB.id = TSK_INVALID_TIMER_ID;
	self->timerD.id = TSK_INVALID_TIMER_ID;
	self->timerM.id = TSK_INVALID_TIMER_ID;

	self->timerA.timeout = TSIP_TIMER_GET(A);
	self->timerB.timeout = TSIP_TIMER_GET(B);
	self->timerD.timeout = TSIP_TRANSAC(self)->reliable ? 0 : TSIP_TIMER_GET(D);
	self->timerM.timeout = TSIP_TIMER_GET(M);

	return 0;
}

/**
 * Starts the client transaction.
 *
 * @param [in,out]	self	The client transaction to start. 
 * @param [in,out]	request	The SIP/IMS INVITE request to send. 
 *
 * @return	Zero if succeed and non-zero error code otherwise. 
**/
int tsip_transac_ict_start(tsip_transac_ict_t *self, const tsip_request_t* request)
{
	int ret = -1;
	if(self && request && !TSIP_TRANSAC(self)->running)
	{
		/* Add branch to the new client transaction. */
		if((TSIP_TRANSAC(self)->branch = tsk_strdup(TSIP_TRANSAC_MAGIC_COOKIE)))
		{
			tsk_istr_t branch;
			tsk_strrandom(&branch);
			tsk_strcat(&(TSIP_TRANSAC(self)->branch), branch);
		}

		TSIP_TRANSAC(self)->running = 1;
		self->request = tsk_object_ref((void*)request);

		ret = tsk_fsm_act(self->fsm, _fsm_action_send, self, tsk_null, self, tsk_null);
	}
	return ret;
}





//--------------------------------------------------------
//				== STATE MACHINE BEGIN ==
//--------------------------------------------------------

/* Started -> (send) -> Calling
*/
int tsip_transac_ict_Started_2_Calling_X_send(va_list *app)
{
	tsip_transac_ict_t *self = va_arg(*app, tsip_transac_ict_t *);
	//const tsip_message_t *message = va_arg(*app, const tsip_message_t *);

	//== Send the request
	tsip_transac_send(TSIP_TRANSAC(self), TSIP_TRANSAC(self)->branch, TSIP_MESSAGE(self->request));

	/* RFC 3261 - 17.1.1.2 Formal Description
		If an unreliable transport is being used, the client transaction MUST 
		start timer A with a value of T1.
		If a reliable transport is being used, the client transaction SHOULD
		NOT start timer A (Timer A controls request retransmissions).  For
		any transport, the client transaction MUST start timer B with a value
		of 64*T1 seconds (Timer B controls transaction timeouts).
	*/
	if(!TSIP_TRANSAC(self)->reliable){
		TRANSAC_ICT_TIMER_SCHEDULE(A);
	}
	TRANSAC_ICT_TIMER_SCHEDULE(B);

	return 0;
}

/* Calling -> (timerA) -> Calling
*/
int tsip_transac_ict_Calling_2_Calling_X_timerA(va_list *app)
{
	tsip_transac_ict_t *self = va_arg(*app, tsip_transac_ict_t *);
	//const tsip_message_t *message = va_arg(*app, const tsip_message_t *);

	/*	RFC 3261 - 17.1.1.2 Formal Description
		When timer A fires, the client transaction MUST retransmit the
		request by passing it to the transport layer, and MUST reset the
		timer with a value of 2*T1.  The formal definition of retransmit

		within the context of the transaction layer is to take the message
		previously sent to the transport layer and pass it to the transport
		layer once more.

		When timer A fires 2*T1 seconds later, the request MUST be
		retransmitted again (assuming the client transaction is still in this
		state).  This process MUST continue so that the request is
		retransmitted with intervals that double after each transmission.
		These retransmissions SHOULD only be done while the client
		transaction is in the "calling" state.
	*/

	//== Send the request
	tsip_transac_send(TSIP_TRANSAC(self), TSIP_TRANSAC(self)->branch, self->request);

	self->timerA.timeout *= 2; /* Will not raise indefinitely ==> see timer B */
	TRANSAC_ICT_TIMER_SCHEDULE(A);

	return 0;
}

/* Calling -> (timerB) -> Terminated
*/
int tsip_transac_ict_Calling_2_Terminated_X_timerB(va_list *app)
{
	tsip_transac_ict_t *self = va_arg(*app, tsip_transac_ict_t *);
	//const tsip_message_t *message = va_arg(*app, const tsip_message_t *);

	/*	RFC 3261 - 17.1.1.2 Formal Description
		If the client transaction is still in the "Calling" state when timer
		B fires, the client transaction SHOULD inform the TU that a timeout
		has occurred.  The client transaction MUST NOT generate an ACK.  The
		value of 64*T1 is equal to the amount of time required to send seven
		requests in the case of an unreliable transport.
	*/
	TSIP_TRANSAC(self)->dialog->callback(TSIP_TRANSAC(self)->dialog, tsip_dialog_timedout, tsk_null);
	
	return 0;
}

/* Calling -> (300-699) -> Completed
*/
int tsip_transac_ict_Calling_2_Completed_X_300_to_699(va_list *app)
{
	tsip_transac_ict_t *self = va_arg(*app, tsip_transac_ict_t *);
	const tsip_response_t *response = va_arg(*app, const tsip_response_t *);
	int ret;

	/*	draft-sparks-sip-invfix-03 - 8.4.  Pages 126 through 128
		When in either the "Calling" or "Proceeding" states, reception of
		a response with status code from 300-699 MUST cause the client
		transaction to transition to "Completed".  The client transaction
		MUST pass the received response up to the TU, and the client
		transaction MUST generate an ACK request, even if the transport is
		reliable (guidelines for constructing the ACK from the response
		are given in Section 17.1.1.3) and then pass the ACK to the
		transport layer for transmission.  The ACK MUST be sent to the
		same address, port, and transport to which the original request
		was sent.
	*/
	/* Do not retransmit */
	if(!TSIP_TRANSAC(self)->reliable){
		TRANSAC_TIMER_CANCEL(A);
	}
	TRANSAC_TIMER_CANCEL(B); /* Now it's up to the UAS to update the FSM. */

	/*	draft-sparks-sip-invfix-03 - 8.4.  Pages 126 through 128
		The client transaction MUST start timer D when it enters the
		"Completed" state for any reason, with a value of at least 32
		seconds for unreliable transports, and a value of zero seconds for
		reliable transports.  Timer D reflects the amount of time that the
		server transaction can remain in the "Completed" state when
		unreliable transports are used.
   */
	TRANSAC_ICT_TIMER_SCHEDULE(D); /* timerD already have the right value (0 if reliable and non-zero otherwise) */

	/* Send ACK */
	if((ret = tsip_transac_ict_send_ack(response))){
		return ret;
	}

	/* Pass the response to the dialog. */
	return TSIP_TRANSAC(self)->dialog->callback(TSIP_TRANSAC(self)->dialog, tsip_dialog_i_msg, response);
}

/* Calling -> (1xx) -> Proceeding
*/
int tsip_transac_ict_Calling_2_Proceeding_X_1xx(va_list *app)
{
	tsip_transac_ict_t *self = va_arg(*app, tsip_transac_ict_t *);
	const tsip_response_t *response = va_arg(*app, const tsip_response_t *);
	
	/*	RFC 3261 - 17.1.1.2 Formal Description
		If the client transaction receives a provisional response while in
		the "Calling" state, it transitions to the "Proceeding" state. In the
		"Proceeding" state, the client transaction SHOULD NOT retransmit the
		request any longer. Furthermore, the provisional response MUST be
		passed to the TU.  Any further provisional responses MUST be passed
		up to the TU while in the "Proceeding" state.
	*/
	
	/* Do not retransmit */
	if(!TSIP_TRANSAC(self)->reliable){
		TRANSAC_TIMER_CANCEL(A);
	}
	TRANSAC_TIMER_CANCEL(B); /* Now it's up to the UAS to update the FSM. */
	
	/* Pass the provisional response to the dialog. */
	TSIP_TRANSAC(self)->dialog->callback(TSIP_TRANSAC(self)->dialog, tsip_dialog_i_msg, response);

	return 0;
}

/* Calling -> (2xx) -> Accepted
*/
int tsip_transac_ict_Calling_2_Accepted_X_2xx(va_list *app)
{
	tsip_transac_ict_t *self = va_arg(*app, tsip_transac_ict_t *);
	const tsip_response_t *response = va_arg(*app, const tsip_response_t *);
	
	/*	draft-sparks-sip-invfix-03 - 8.4.  Pages 126 through 128
		When a 2xx response is received while in either the "Calling" or
		"Proceeding" states, the client transaction MUST transition to the
		"Accepted" state, and Timer M MUST be started with a value of
		64*T1.  The 2xx response MUST be passed up to the TU.  The client
		transaction MUST NOT generate an ACK to the 2xx response - its
		handling is delegated to the TU.
	*/
	
	/* Schedule timer M */
	TRANSAC_ICT_TIMER_SCHEDULE(M);

	/* pass the response to the TU (dialog) */
	return TSIP_TRANSAC(self)->dialog->callback(TSIP_TRANSAC(self)->dialog, tsip_dialog_i_msg, response);
}

/* Proceeding -> (1xx) -> Proceeding
*/
int tsip_transac_ict_Proceeding_2_Proceeding_X_1xx(va_list *app)
{
	return 0;
}

/* Proceeding -> (300-699) -> Completed
*/
int tsip_transac_ict_Proceeding_2_Completed_X_300_to_699(va_list *app)
{
	tsip_transac_ict_t *self = va_arg(*app, tsip_transac_ict_t *);
	const tsip_response_t *response = va_arg(*app, const tsip_response_t *);
	int ret;

	/*	draft-sparks-sip-invfix-03 - 8.4.  Pages 126 through 128
		When in either the "Calling" or "Proceeding" states, reception of
		a response with status code from 300-699 MUST cause the client
		transaction to transition to "Completed".  The client transaction
		MUST pass the received response up to the TU, and the client
		transaction MUST generate an ACK request, even if the transport is
		reliable (guidelines for constructing the ACK from the response
		are given in Section 17.1.1.3) and then pass the ACK to the
		transport layer for transmission.  The ACK MUST be sent to the
		same address, port, and transport to which the original request
		was sent.
	*/
	/* Do not retransmit */
	if(!TSIP_TRANSAC(self)->reliable){
		TRANSAC_TIMER_CANCEL(A);
	}
	TRANSAC_TIMER_CANCEL(B); /* Now it's up to the UAS to update the FSM. */

	/*	draft-sparks-sip-invfix-03 - 8.4.  Pages 126 through 128
		The client transaction MUST start timer D when it enters the
		"Completed" state for any reason, with a value of at least 32
		seconds for unreliable transports, and a value of zero seconds for
		reliable transports.  Timer D reflects the amount of time that the
		server transaction can remain in the "Completed" state when
		unreliable transports are used.
   */
	TRANSAC_ICT_TIMER_SCHEDULE(D); /* timerD already have the right value (0 if reliable and non-zero otherwise) */

	/* Send ACK */
	if((ret = tsip_transac_ict_send_ack(response))){
		return ret;
	}

	/* Pass the response to the dialog. */
	return TSIP_TRANSAC(self)->dialog->callback(TSIP_TRANSAC(self)->dialog, tsip_dialog_i_msg, response);

	return ret;
}

/* Proceeding -> (2xx) -> Accepted
*/
int tsip_transac_ict_Proceeding_2_Accepted_X_2xx(va_list *app)
{
	tsip_transac_ict_t *self = va_arg(*app, tsip_transac_ict_t *);
	const tsip_response_t *response = va_arg(*app, const tsip_response_t *);

	/*	draft-sparks-sip-invfix-03 - 8.4.  Pages 126 through 128
		When a 2xx response is received while in either the "Calling" or
		"Proceeding" states, the client transaction MUST transition to the
		"Accepted" state, and Timer M MUST be started with a value of
		64*T1.  The 2xx response MUST be passed up to the TU.  The client
		transaction MUST NOT generate an ACK to the 2xx response - its
		handling is delegated to the TU.
	*/

	/* Schedule timer M */
	TRANSAC_ICT_TIMER_SCHEDULE(M);

	/* pass the response to the TU (dialog) */
	return TSIP_TRANSAC(self)->dialog->callback(TSIP_TRANSAC(self)->dialog, tsip_dialog_i_msg, response);
}

/* Completed -> (300-699) -> Completed
*/
int tsip_transac_ict_Completed_2_Completed_X_300_to_699(va_list *app)
{
	//tsip_transac_ict_t *self = va_arg(*app, tsip_transac_ict_t *);
	const tsip_response_t *response = va_arg(*app, const tsip_response_t *);

	/*	draft-sparks-sip-invfix-03 - 8.4.  Pages 126 through 128
		Any retransmissions of a response with status code 300-699 that
		are received while in the "Completed" state MUST cause the ACK to
		be re-passed to the transport layer for retransmission, but the
		newly received response MUST NOT be passed up to the TU.
	*/
	
	return tsip_transac_ict_send_ack(response);
}

/* Completed -> (timerD) -> Terminated
*/
int tsip_transac_ict_Completed_2_Terminated_X_timerD(va_list *app)
{
	/*	draft-sparks-sip-invfix-03 - 8.4.  Pages 126 through 128
		If timer D fires while the client transaction is in the
		"Completed" state, the client transaction MUST move to the
		"Terminated" state.
	*/
	
	/* Timers will be canceled by "tsip_transac_ict_OnTerminated" */
	return 0;
}

/* Accepted -> (2xx) -> Accepted
*/
int tsip_transac_ict_Accepted_2_Accepted_X_2xx(va_list *app)
{
	tsip_transac_ict_t *self = va_arg(*app, tsip_transac_ict_t *);
	const tsip_response_t *response = va_arg(*app, const tsip_response_t *);

	/*	draft-sparks-sip-invfix-03 - 7.2. UAC Impacts
		A 2xx response received while in the "Accepted" state MUST be passed to the TU and
		the machine remains in the "Accepted" state.  The client transaction
		MUST NOT generate an ACK to any 2xx response on its own.  The TU
		responsible for the transaction will generate the ACK.
	*/
	
	/* Pass the response to the TU. */
	TSIP_TRANSAC(self)->dialog->callback(TSIP_TRANSAC(self)->dialog, tsip_dialog_i_msg, response);
	
	return 0;
}

/* Accepted -> (timerM) -> Terminated
*/
int tsip_transac_ict_Accepted_2_Terminated_X_timerM(va_list *app)
{
	/*	draft-sparks-sip-invfix-03 - 8.4.  Pages 126 through 128
		If timer M fires while the client transaction is in the "Accepted"
		state, the client transaction MUST move to the "Terminated" state.
	*/
	return 0;
}

/* Any -> (Transport Error) -> Terminated
*/
int tsip_transac_ict_Any_2_Terminated_X_transportError(va_list *app)
{
	tsip_transac_ict_t *self = va_arg(*app, tsip_transac_ict_t *);
	//const tsip_message_t *message = va_arg(*app, const tsip_message_t *);

	/* Timers will be canceled by "tsip_transac_nict_OnTerminated" */

	return TSIP_TRANSAC(self)->dialog->callback(TSIP_TRANSAC(self)->dialog, tsip_dialog_transport_error, tsk_null);
}

/* Any -> (Error) -> Terminated
*/
int tsip_transac_ict_Any_2_Terminated_X_Error(va_list *app)
{
	tsip_transac_ict_t *self = va_arg(*app, tsip_transac_ict_t *);
	//const tsip_message_t *message = va_arg(*app, const tsip_message_t *);

	/* Timers will be canceled by "tsip_transac_nict_OnTerminated" */

	return TSIP_TRANSAC(self)->dialog->callback(TSIP_TRANSAC(self)->dialog, tsip_dialog_error, tsk_null);
}



//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//				== STATE MACHINE END ==
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/* Send ACK message*/
int tsip_transac_ict_send_ack(const tsip_response_t* response)
{
	return -1;
}

/*== TERMINATED
*/
int tsip_transac_ict_OnTerminated(tsip_transac_ict_t *self)
{
	/*	draft-sparks-sip-invfix-03 - 8.4.  Pages 126 through 128
		The client transaction MUST be destroyed the instant it enters the "Terminated" state.
	*/	
	TSK_DEBUG_INFO("=== ICT terminated ===");
	
	/* Remove (and destroy) the transaction from the layer. */
	return tsip_transac_remove(TSIP_TRANSAC(self));
}



























//========================================================
//	ICT object definition
//
static void* tsip_transac_ict_create(void * self, va_list * app)
{
	tsip_transac_ict_t *transac = self;
	if(transac)
	{
		const tsip_stack_handle_t *stack = va_arg(*app, const tsip_stack_handle_t *);
		tsk_bool_t reliable = va_arg(*app, tsk_bool_t);
		int32_t cseq_value = va_arg(*app, int32_t);
		const char *cseq_method = "INVITE";
		const char *callid = va_arg(*app, const char *);

		/* create FSM */
		transac->fsm = TSK_FSM_CREATE(_fsm_state_Started, _fsm_state_Terminated);
		transac->fsm->debug = DEBUG_STATE_MACHINE;
		tsk_fsm_set_callback_terminated(transac->fsm, TSK_FSM_ONTERMINATED(tsip_transac_ict_OnTerminated), (const void*)transac);

		/* Initialize base class */
		tsip_transac_init(TSIP_TRANSAC(transac), stack, tsip_ict, reliable, cseq_value, cseq_method, callid);

		/* Initialize ICT object */
		tsip_transac_ict_init(transac);
	}
	return self;
}

static void* tsip_transac_ict_destroy(void * _self)
{ 
	tsip_transac_ict_t *self = _self;
	if(self)
	{
		/* Cancel timers */
		if(!TSIP_TRANSAC(self)->reliable){
			TRANSAC_TIMER_CANCEL(A);
		}
		TRANSAC_TIMER_CANCEL(B);
		TRANSAC_TIMER_CANCEL(D);
		TRANSAC_TIMER_CANCEL(M);

		TSIP_TRANSAC(self)->running = 0;
		TSK_OBJECT_SAFE_FREE(self->request);

		/* DeInitialize base class */
		tsip_transac_deinit(TSIP_TRANSAC(self));

		/* FSM */
		TSK_OBJECT_SAFE_FREE(self->fsm);
	}
	return _self;
}

static int tsip_transac_ict_cmp(const tsk_object_t *t1, const tsk_object_t *t2)
{
	return tsip_transac_cmp(t1, t2);
}

static const tsk_object_def_t tsip_transac_ict_def_s = 
{
	sizeof(tsip_transac_ict_t),
	tsip_transac_ict_create, 
	tsip_transac_ict_destroy,
	tsip_transac_ict_cmp, 
};
const void *tsip_transac_ict_def_t = &tsip_transac_ict_def_s;