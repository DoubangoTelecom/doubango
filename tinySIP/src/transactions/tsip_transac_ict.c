/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/transactions/tsip_transac_ict.h"

#include "tsk_debug.h"

#define DEBUG_STATE_MACHINE						0

#define TRANSAC_ICT_TIMER_SCHEDULE(TX)			TRANSAC_TIMER_SCHEDULE(ict, TX)

/* ======================== internal functions ======================== */
int tsip_transac_ict_init(tsip_transac_ict_t *self);
int tsip_transac_ict_send_ACK(tsip_transac_ict_t *self, const tsip_response_t* response); // ACK
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
int tsip_transac_ict_Any_2_Terminated_X_cancel(va_list *app); /* doubango-specific */

/* ======================== conds ======================== */

/* ======================== actions ======================== */
typedef enum _fsm_action_e
{
	_fsm_action_cancel = tsip_atype_cancel,

	_fsm_action_send = 0xFF,
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
	/*	draft-sparks-sip-invfix-03 - 7.2. UAC Impacts
		Any response received which does not match an existing client transaction state machine is simply dropped.
	*/
	switch(type){

		case tsip_transac_incoming_msg:
			{
				if(msg)
				{
					if(TSIP_MESSAGE_IS_RESPONSE(msg)){
						if(TSIP_RESPONSE_IS_1XX(msg)){
							return tsip_transac_fsm_act(TSIP_TRANSAC(self), _fsm_action_1xx, msg);
						}
						else if(TSIP_RESPONSE_IS_2XX(msg)){
							return tsip_transac_fsm_act(TSIP_TRANSAC(self), _fsm_action_2xx, msg);
						}
						else if(TSIP_RESPONSE_IS_3456(msg)){
							return tsip_transac_fsm_act(TSIP_TRANSAC(self), _fsm_action_300_to_699, msg);
						}
						else{
							TSK_DEBUG_WARN("Not supported status code: %d", TSIP_RESPONSE_CODE(msg));
							return 0;
						}
					}
					// any other response have to be delivered if dst_type is 'network'
					if(TSIP_TRANSAC(self)->dst->type == tsip_transac_dst_type_net){
						return tsip_transac_deliver(TSIP_TRANSAC(self), tsip_transac_incoming_msg, msg);
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
				return tsip_transac_fsm_act(TSIP_TRANSAC(self), _fsm_action_error, msg);
			}

		case tsip_transac_transport_error:
			{
				return tsip_transac_fsm_act(TSIP_TRANSAC(self), _fsm_action_transporterror, msg);
			}
            
        default: break;
	}

	return 0;
}

int tsip_transac_ict_timer_callback(const tsip_transac_ict_t* self, tsk_timer_id_t timer_id)
{
	int ret = -1;

	if(self && TSIP_TRANSAC(self))
	{
		if(timer_id == self->timerA.id){
			ret = tsip_transac_fsm_act(TSIP_TRANSAC(self), _fsm_action_timerA, tsk_null);
		}
		else if(timer_id == self->timerB.id){
			ret = tsip_transac_fsm_act(TSIP_TRANSAC(self), _fsm_action_timerB, tsk_null);
		}
		else if(timer_id == self->timerD.id){
			ret = tsip_transac_fsm_act(TSIP_TRANSAC(self), _fsm_action_timerD, tsk_null);
		}
		else if(timer_id == self->timerM.id){
			ret = tsip_transac_fsm_act(TSIP_TRANSAC(self), _fsm_action_timerM, tsk_null);
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
	tsk_fsm_set(TSIP_TRANSAC_GET_FSM(self),
			
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
			// Any -> (cancel) -> Terminated
			TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, _fsm_action_cancel, _fsm_state_Terminated, tsip_transac_ict_Any_2_Terminated_X_cancel, "tsip_transac_ict_Any_2_Terminated_X_cancel"),
			
			TSK_FSM_ADD_NULL());


	/* Set callback function to call when new messages arrive or errors happen in
	the transport layer.
	*/
	TSIP_TRANSAC(self)->callback = TSIP_TRANSAC_EVENT_CALLBACK_F(tsip_transac_ict_event_callback);

	/* Timers */
	self->timerA.id = TSK_INVALID_TIMER_ID;
	self->timerB.id = TSK_INVALID_TIMER_ID;
	self->timerD.id = TSK_INVALID_TIMER_ID;
	self->timerM.id = TSK_INVALID_TIMER_ID;

	self->timerA.timeout = TSIP_TIMER_GET(A);
	self->timerB.timeout = TSIP_TIMER_GET(B);
	self->timerM.timeout = TSIP_TIMER_GET(M);

	return 0;
}

tsip_transac_ict_t* tsip_transac_ict_create(int32_t cseq_value, const char* callid, tsip_transac_dst_t* dst)
{
	tsip_transac_ict_t* transac = tsk_object_new(tsip_transac_ict_def_t);
	if(transac){
		// initialize base class
		tsip_transac_init(TSIP_TRANSAC(transac), tsip_transac_type_ict, cseq_value, "INVITE", callid, dst, _fsm_state_Started, _fsm_state_Terminated);

		// init FSM
		TSIP_TRANSAC_GET_FSM(transac)->debug = DEBUG_STATE_MACHINE;
		tsk_fsm_set_callback_terminated(TSIP_TRANSAC_GET_FSM(transac), TSK_FSM_ONTERMINATED_F(tsip_transac_ict_OnTerminated), (const void*)transac);

		// initialize ICT object
		tsip_transac_ict_init(transac);
	}
	return transac;
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
	if(self && request && !TSIP_TRANSAC(self)->running){
		/* Add branch to the new client transaction
		* - Transac will use request branch if exit (e.g. when request received over websocket)
		*/
		if((request->firstVia && !tsk_strnullORempty(request->firstVia->branch))){
			tsk_strupdate(&TSIP_TRANSAC(self)->branch, (request->firstVia ? request->firstVia->branch : "doubango"));
		}
		else if((TSIP_TRANSAC(self)->branch = tsk_strdup(TSIP_TRANSAC_MAGIC_COOKIE))){
			tsk_istr_t branch;
			tsk_strrandom(&branch);
			tsk_strcat_2(&(TSIP_TRANSAC(self)->branch), "-%s", branch);
		}

		TSIP_TRANSAC(self)->running = 1;
		self->request = tsk_object_ref((void*)request);

		ret = tsip_transac_fsm_act(TSIP_TRANSAC(self), _fsm_action_send, tsk_null);
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

	// Now that the first request is sent using the best transport mean we know if it's reliable or not
	if(TNET_SOCKET_TYPE_IS_VALID(self->request->dst_net_type)){
		TSIP_TRANSAC(self)->reliable = TNET_SOCKET_TYPE_IS_STREAM(self->request->dst_net_type);
		self->timerD.timeout = TSIP_TRANSAC(self)->reliable ? 0 : TSIP_TIMER_GET(D);
	}

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
	tsip_transac_deliver(TSIP_TRANSAC(self), tsip_dialog_timedout, tsk_null);
	
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
	if((ret = tsip_transac_ict_send_ACK(self, response))){
		return ret;
	}

	/* Pass the response to the dialog. */
	return tsip_transac_deliver(TSIP_TRANSAC(self), tsip_dialog_i_msg, response);
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
	tsip_transac_deliver(TSIP_TRANSAC(self), tsip_dialog_i_msg, response);

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
	
	/* Cancel timers A and B */
	if(!TSIP_TRANSAC(self)->reliable){
		TRANSAC_ICT_TIMER_SCHEDULE(A);
	}
	TRANSAC_ICT_TIMER_SCHEDULE(B);

	/* pass the response to the TU (dialog) */
	return tsip_transac_deliver(TSIP_TRANSAC(self), tsip_dialog_i_msg, response);
}

/* Proceeding -> (1xx) -> Proceeding
*/
int tsip_transac_ict_Proceeding_2_Proceeding_X_1xx(va_list *app)
{
	tsip_transac_ict_t *self = va_arg(*app, tsip_transac_ict_t *);
	const tsip_response_t *response = va_arg(*app, const tsip_response_t *);

	/* pass the response to the TU (dialog) */
	return tsip_transac_deliver(TSIP_TRANSAC(self), tsip_dialog_i_msg, response);
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
	if((ret = tsip_transac_ict_send_ACK(self, response))){
		return ret;
	}

	/* Pass the response to the dialog. */
	ret = tsip_transac_deliver(TSIP_TRANSAC(self), tsip_dialog_i_msg, response);

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

	/* Cancel timers A and B */
	if(!TSIP_TRANSAC(self)->reliable){
		TRANSAC_ICT_TIMER_SCHEDULE(A);
	}
	TRANSAC_ICT_TIMER_SCHEDULE(B);

	/* pass the response to the TU (dialog) */
	return tsip_transac_deliver(TSIP_TRANSAC(self), tsip_dialog_i_msg, response);
}

/* Completed -> (300-699) -> Completed
*/
int tsip_transac_ict_Completed_2_Completed_X_300_to_699(va_list *app)
{
	tsip_transac_ict_t *self = va_arg(*app, tsip_transac_ict_t *);
	const tsip_response_t *response = va_arg(*app, const tsip_response_t *);

	/*	draft-sparks-sip-invfix-03 - 8.4.  Pages 126 through 128
		Any retransmissions of a response with status code 300-699 that
		are received while in the "Completed" state MUST cause the ACK to
		be re-passed to the transport layer for retransmission, but the
		newly received response MUST NOT be passed up to the TU.
	*/
	
	return tsip_transac_ict_send_ACK(self, response);
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
	tsip_transac_deliver(TSIP_TRANSAC(self), tsip_dialog_i_msg, response);
	
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

	/* Timers will be canceled by "tsip_transac_ict_OnTerminated" */

	return tsip_transac_deliver(TSIP_TRANSAC(self), tsip_dialog_transport_error, tsk_null);
}

/* Any -> (Error) -> Terminated
*/
int tsip_transac_ict_Any_2_Terminated_X_Error(va_list *app)
{
	tsip_transac_ict_t *self = va_arg(*app, tsip_transac_ict_t *);
	//const tsip_message_t *message = va_arg(*app, const tsip_message_t *);

	/* Timers will be canceled by "tsip_transac_ict_OnTerminated" */

	return tsip_transac_deliver(TSIP_TRANSAC(self), tsip_dialog_error, tsk_null);
}

/* Any -> (cancel) -> Terminated
*/
int tsip_transac_ict_Any_2_Terminated_X_cancel(va_list *app)
{
	/* doubango-specific */
	return 0;
}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//				== STATE MACHINE END ==
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/* Send ACK message
*/
int tsip_transac_ict_send_ACK(tsip_transac_ict_t *self, const tsip_response_t* response)
{
	int ret = -1;
	tsip_request_t *request = tsk_null;
	const tsk_list_item_t* item;

	if(!self || !self->request || !response){
		goto bail;
	}

	// check lastINVITE
	if(	!self->request->firstVia ||
		!self->request->From || 
		!self->request->line.request.uri || 
		!self->request->Call_ID || 
		!self->request->CSeq)
	{
		ret = -2;
		goto bail;
	}

	// check response
	if(!response->To){
		ret = -3;
		goto bail;
	}

	/*	RFC 3261 - 17.1.1.3 Construction of the ACK Request
		
		The ACK request constructed by the client transaction MUST contain
		values for the Call-ID, From, and Request-URI that are equal to the
		values of those header fields in the request passed to the transport
		by the client transaction (call this the "original request").  The To
		header field in the ACK MUST equal the To header field in the
		response being acknowledged, and therefore will usually differ from
		the To header field in the original request by the addition of the
		tag parameter.  The ACK MUST contain a single Via header field, and
		this MUST be equal to the top Via header field of the original
		request.  The CSeq header field in the ACK MUST contain the same
		value for the sequence number as was present in the original request,
		but the method parameter MUST be equal to "ACK".

		If the INVITE request whose response is being acknowledged had Route
		header fields, those header fields MUST appear in the ACK.  This is
		to ensure that the ACK can be routed properly through any downstream
		stateless proxies.

		Although any request MAY contain a body, a body in an ACK is special
		since the request cannot be rejected if the body is not understood.
		Therefore, placement of bodies in ACK for non-2xx is NOT RECOMMENDED,
		but if done, the body types are restricted to any that appeared in
		the INVITE, assuming that the response to the INVITE was not 415.  If
		it was, the body in the ACK MAY be any type listed in the Accept
		header field in the 415.
	*/
	if((request = tsip_request_new("ACK", self->request->line.request.uri,  self->request->From->uri, response->To->uri, self->request->Call_ID->value, self->request->CSeq->seq))){
		// Via
		request->firstVia = tsk_object_ref((void*)self->request->firstVia);
		// tags
		if(request->From){
			request->From->tag = tsk_strdup(self->request->From->tag);
		}
		if(request->To){
			request->To->tag = tsk_strdup(response->To->tag);
		}
		// Routes
		tsk_list_foreach(item, self->request->headers){
			const tsip_header_t* curr = item->data;
			if(curr->type == tsip_htype_Route){
				tsip_message_add_header(request, curr);
			}
		}

		// SigComp
		if(TSIP_TRANSAC_GET_SESSION(self) && TSIP_TRANSAC_GET_SESSION(self)->sigcomp_id){
			request->sigcomp_id = tsk_strdup(TSIP_TRANSAC_GET_SESSION(self)->sigcomp_id);
		}

		// send the request
		ret = tsip_transac_send(TSIP_TRANSAC(self), request->firstVia->branch, request);
		TSK_OBJECT_SAFE_FREE(request);
	}

bail:
	return ret;
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
static tsk_object_t* tsip_transac_ict_ctor(tsk_object_t * self, va_list * app)
{
	tsip_transac_ict_t *transac = self;
	if(transac){
	}
	return self;
}

static tsk_object_t* tsip_transac_ict_dtor(tsk_object_t * _self)
{ 
	tsip_transac_ict_t *self = _self;
	if(self){
		/* Cancel timers */
		if(!TSIP_TRANSAC(self)->reliable){
			TRANSAC_TIMER_CANCEL(A);
		}
		TRANSAC_TIMER_CANCEL(B);
		TRANSAC_TIMER_CANCEL(D);
		TRANSAC_TIMER_CANCEL(M);

		TSIP_TRANSAC(self)->running = tsk_false;
		TSK_OBJECT_SAFE_FREE(self->request);

		/* DeInitialize base class */
		tsip_transac_deinit(TSIP_TRANSAC(self));

		TSK_DEBUG_INFO("*** ICT destroyed ***");
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
	tsip_transac_ict_ctor, 
	tsip_transac_ict_dtor,
	tsip_transac_ict_cmp, 
};
const tsk_object_def_t *tsip_transac_ict_def_t = &tsip_transac_ict_def_s;
