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
								  |Request received
                                  |pass to TU
                                  V
                            +-----------+
                            |           |
                            | Trying    |-------------+
                            |           |             |
                            +-----------+             |200-699 from TU
                                  |                   |send response
                                  |1xx from TU        |
                                  |send response      |
                                  |                   |
               Request            V      1xx from TU  |
               send response+-----------+send response|
                   +--------|           |--------+    |
                   |        | Proceeding|        |    |
                   +------->|           |<-------+    |
            +<--------------|           |             |
            |Trnsprt Err    +-----------+             |
            |Inform TU            |                   |
            |                     |                   |
            |                     |200-699 from TU    |
            |                     |send response      |
            |  Request            V                   |
            |  send response+-----------+             |
            |      +--------|           |             |
            |      |        | Completed |<------------+
            |      +------->|           |
            +<--------------|           |
            |Trnsprt Err    +-----------+
            |Inform TU            |
            |                     |Timer J fires
            |                     |-
            |                     |
            |                     V
            |               +-----------+
            |               |           |
            +-------------->| Terminated|
                            |           |
                            +-----------+ 

=============================================================================*/

/**@file tsip_transac_nist.c
 * @brief SIP Non-INVITE Server Transaction as per RFC 3261 subclause 17.2.2.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinySIP/transactions/tsip_transac_nist.h"

#include "tsk_debug.h"

#define DEBUG_STATE_MACHINE						1

#define TRANSAC_NIST_TIMER_SCHEDULE(TX)			TRANSAC_TIMER_SCHEDULE(nist, TX)

/* ======================== internal functions ======================== */
int tsip_transac_nist_init(tsip_transac_nist_t *self);
int tsip_transac_nist_handle_request(const tsip_transac_nist_t *self, const tsip_request_t* request);
int tsip_transac_nist_OnTerminated(tsip_transac_nist_t *self);

/* ======================== transitions ======================== */
int tsip_transac_nist_Started_2_Trying_X_request(va_list *app);
int tsip_transac_nist_Trying_2_Proceeding_X_send_1xx(va_list *app);
int tsip_transac_nist_Trying_2_Completed_X_send_200_to_699(va_list *app);
int tsip_transac_nist_Proceeding_2_Proceeding_X_send_1xx(va_list *app);
int tsip_transac_nist_Proceeding_2_Proceeding_X_request(va_list *app);
int tsip_transac_nist_Proceeding_2_Completed_X_send_200_to_699(va_list *app);
int tsip_transac_nist_Completed_2_Completed_X_request(va_list *app);
int tsip_transac_nist_Completed_2_Terminated_X_tirmerJ(va_list *app);
int tsip_transac_nist_Any_2_Terminated_X_transportError(va_list *app);
int tsip_transac_nist_Any_2_Terminated_X_Error(va_list *app);

/* ======================== conds ======================== */

/* ======================== actions ======================== */
typedef enum _fsm_action_e
{
	_fsm_action_request,
	_fsm_action_send_1xx,
	_fsm_action_send_200_to_699,
	_fsm_action_timerJ,
	_fsm_action_transporterror,
	_fsm_action_error,
}
_fsm_action_t;

/* ======================== states ======================== */
typedef enum _fsm_state_e
{
	_fsm_state_Started,
	_fsm_state_Trying,
	_fsm_state_Proceeding,
	_fsm_state_Completed,
	_fsm_state_Terminated
}
_fsm_state_t;

int tsip_transac_nist_event_callback(const tsip_transac_nist_t *self, tsip_transac_event_type_t type, const tsip_message_t *msg)
{
	int ret = -1;

	switch(type)
	{
	case tsip_transac_incoming_msg:
		{
			if(msg && TSIP_MESSAGE_IS_REQUEST(msg)){
				ret = tsk_fsm_act(self->fsm, _fsm_action_request, self, msg, self, msg);
			}
			break;
		}

	case tsip_transac_outgoing_msg:
		{
			if(msg && TSIP_MESSAGE_IS_RESPONSE(msg))
			{
				if(TSIP_RESPONSE_IS_1XX(msg)){
					ret = tsk_fsm_act(self->fsm, _fsm_action_send_1xx, self, msg, self, msg);
				}
				else if(TSIP_RESPONSE_IS_23456(msg)){
					ret = tsk_fsm_act(self->fsm, _fsm_action_send_200_to_699, self, msg, self, msg);
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

int tsip_transac_nist_timer_callback(const tsip_transac_nist_t* self, tsk_timer_id_t timer_id)
{
	int ret = -1;

	if(self)
	{
		if(timer_id == self->timerJ.id)
		{
			ret = tsk_fsm_act(self->fsm, _fsm_action_timerJ, self, TSK_NULL, self, TSK_NULL);
		}
	}

	return ret;
}

int tsip_transac_nist_init(tsip_transac_nist_t *self)
{
	/* Initialize the state machine.
	*/
	tsk_fsm_set(self->fsm,
			
			/*=======================
			* === Started === 
			*/
			// Started -> (receive request) -> Trying
			TSK_FSM_ADD_ALWAYS(_fsm_state_Started, _fsm_action_request, _fsm_state_Trying, tsip_transac_nist_Started_2_Trying_X_request, "tsip_transac_nist_Started_2_Trying_X_request"),
			// Started -> (Any) -> Started
			TSK_FSM_ADD_ALWAYS_NOTHING(_fsm_state_Started, "tsip_transac_nist_Started_2_Started_X_any"),
			

			/*=======================
			* === Trying === 
			*/
			// Trying -> (send 1xx) -> Proceeding
			TSK_FSM_ADD_ALWAYS(_fsm_state_Trying, _fsm_action_send_1xx, _fsm_state_Proceeding, tsip_transac_nist_Trying_2_Proceeding_X_send_1xx, "tsip_transac_nist_Trying_2_Proceeding_X_send_1xx"),
			// Trying -> (send 200 to 699) -> Completed
			TSK_FSM_ADD_ALWAYS(_fsm_state_Trying, _fsm_action_send_200_to_699, _fsm_state_Completed, tsip_transac_nist_Trying_2_Completed_X_send_200_to_699, "tsip_transac_nist_Trying_2_Completed_X_send_200_to_699"),
			
			
			/*=======================
			* === Proceeding === 
			*/
			// Proceeding -> (send 1xx) -> Proceeding
			TSK_FSM_ADD_ALWAYS(_fsm_state_Proceeding, _fsm_action_send_1xx, _fsm_state_Proceeding, tsip_transac_nist_Proceeding_2_Proceeding_X_send_1xx, "tsip_transac_nist_Proceeding_2_Proceeding_X_send_1xx"),
			// Proceeding -> (send 200 to 699) -> Completed
			TSK_FSM_ADD_ALWAYS(_fsm_state_Proceeding, _fsm_action_send_200_to_699, _fsm_state_Completed, tsip_transac_nist_Proceeding_2_Completed_X_send_200_to_699, "tsip_transac_nist_Proceeding_2_Completed_X_send_200_to_699"),
			// Proceeding -> (receive request) -> Proceeding
			TSK_FSM_ADD_ALWAYS(_fsm_state_Proceeding, _fsm_action_request, _fsm_state_Proceeding, tsip_transac_nist_Proceeding_2_Proceeding_X_request, "tsip_transac_nist_Proceeding_2_Proceeding_X_request"),
			
			
			/*=======================
			* === Completed === 
			*/
			// Completed -> (receive request) -> Completed
			TSK_FSM_ADD_ALWAYS(_fsm_state_Completed, _fsm_action_request, _fsm_state_Completed, tsip_transac_nist_Completed_2_Completed_X_request, "tsip_transac_nist_Completed_2_Completed_X_request"),
			// Completed -> (timer J) -> Terminated
			TSK_FSM_ADD_ALWAYS(_fsm_state_Completed, _fsm_action_timerJ, _fsm_state_Terminated, tsip_transac_nist_Completed_2_Terminated_X_tirmerJ, "tsip_transac_nist_Completed_2_Terminated_X_tirmerJ"),
			

			/*=======================
			* === Any === 
			*/
			// Any -> (transport error) -> Terminated
			TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, _fsm_action_transporterror, _fsm_state_Terminated, tsip_transac_nist_Any_2_Terminated_X_transportError, "tsip_transac_nist_Any_2_Terminated_X_transportError"),
			// Any -> (transport error) -> Terminated
			TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, _fsm_action_error, _fsm_state_Terminated, tsip_transac_nist_Any_2_Terminated_X_Error, "tsip_transac_nist_Any_2_Terminated_X_Error"),

			
			TSK_FSM_ADD_NULL());

	/* Set callback function to call when new messages arrive or errors happen in
		the transport layer.
	*/
	TSIP_TRANSAC(self)->callback = TSIP_TRANSAC_EVENT_CALLBACK(tsip_transac_nist_event_callback);

	 self->timerJ.timeout = TSIP_TRANSAC(self)->reliable ? 0 : TSIP_TIMER_GET(J); /* RFC 3261 - 17.2.2*/

	 return 0;
}

int tsip_transac_nist_start(tsip_transac_nist_t *self, const tsip_request_t* request)
{
	int ret = -1;

	if(self && !TSIP_TRANSAC(self)->running && request)
	{
		TSIP_TRANSAC(self)->running = 1;
		tsip_transac_nist_handle_request(self, request);

		ret = 0;
	}
	return ret;
}

int tsip_transac_nist_handle_request(const tsip_transac_nist_t *self, const tsip_request_t* request)
{
	int ret = -1;
	if(!(ret = tsk_fsm_act(self->fsm, _fsm_action_request, self, TSK_NULL, self, TSK_NULL))){
		/* Alert the dialog */
		ret = TSIP_TRANSAC(self)->dialog->callback(TSIP_TRANSAC(self)->dialog, tsip_dialog_i_msg, request);
	}
	return ret;
}









//--------------------------------------------------------
//				== STATE MACHINE BEGIN ==
//--------------------------------------------------------

/* Started --> (INCOMING REQUEST) --> Trying
*/
int tsip_transac_nist_Started_2_Trying_X_request(va_list *app)
{
	tsip_transac_nist_t *self = va_arg(*app, tsip_transac_nist_t *);
	const tsip_response_t *response = va_arg(*app, const tsip_response_t *);

	/*	RFC 3261 - 17.2.2
		The state machine is initialized in the "Trying" state and is passed
		a request other than INVITE or ACK when initialized.  This request is
		passed up to the TU.  Once in the "Trying" state, any further request
		retransmissions are discarded.  A request is a retransmission if it
		matches the same server transaction, using the rules specified in
		Section 17.2.3.
	*/

	return 0;
}

/* Trying --> (1xx) --> Proceeding
*/
int tsip_transac_nist_Trying_2_Proceeding_X_send_1xx(va_list *app)
{
	tsip_transac_nist_t *self = va_arg(*app, tsip_transac_nist_t *);
	const tsip_response_t *response = va_arg(*app, const tsip_response_t *);

	/*	RFC 3261 - 17.2.2
		While in the "Trying" state, if the TU passes a provisional response
		to the server transaction, the server transaction MUST enter the
		"Proceeding" state.  The response MUST be passed to the transport
		layer for transmission.
	*/
	tsip_transac_send(TSIP_TRANSAC(self), TSIP_TRANSAC(self)->branch, response);

	/* Update last response */
	TSK_OBJECT_SAFE_FREE(self->lastResponse);
	self->lastResponse = tsk_object_ref((void*)response);

	return 0;
}

/*	Trying --> (200-699) --> Completed
*/
int tsip_transac_nist_Trying_2_Completed_X_send_200_to_699(va_list *app)
{
	tsip_transac_nist_t *self = va_arg(*app, tsip_transac_nist_t *);
	const tsip_response_t *response = va_arg(*app, const tsip_response_t *);

	tsip_transac_send(TSIP_TRANSAC(self), TSIP_TRANSAC(self)->branch, response);

	/*	RFC 3261 - 17.2.2
		When the server transaction enters the "Completed" state, it MUST set
		Timer J to fire in 64*T1 seconds for unreliable transports, and zero
		seconds for reliable transports.
	*/
	TRANSAC_NIST_TIMER_SCHEDULE(J);

	/* Update last response */
	TSK_OBJECT_SAFE_FREE(self->lastResponse);
	self->lastResponse = tsk_object_ref((void*)response);

	return 0;
}

/*	Proceeding --> (1xx) --> Proceeding
*/
int tsip_transac_nist_Proceeding_2_Proceeding_X_send_1xx(va_list *app)
{
	tsip_transac_nist_t *self = va_arg(*app, tsip_transac_nist_t *);
	const tsip_response_t *response = va_arg(*app, const tsip_response_t *);

	/*	RFC 3261 - 17.2.2
		Any further provisional responses that are
		received from the TU while in the "Proceeding" state MUST be passed
		to the transport layer for transmission.
	*/
	tsip_transac_send(TSIP_TRANSAC(self), TSIP_TRANSAC(self)->branch, response);

	/* Update last response */
	TSK_OBJECT_SAFE_FREE(self->lastResponse);
	self->lastResponse = tsk_object_ref((void*)response);

	return 0;
}

/* Proceeding -> (INCOMING REQUEST) -> Proceeding
*/
int tsip_transac_nist_Proceeding_2_Proceeding_X_request(va_list *app)
{
	tsip_transac_nist_t *self = va_arg(*app, tsip_transac_nist_t *);
	const tsip_response_t *response = va_arg(*app, const tsip_response_t *);

	/*	RFC 3261 - 17.2.2
		If a retransmission of the request is received while in the "Proceeding" state, the most
		recently sent provisional response MUST be passed to the transport
		layer for retransmission.
	*/
	if(self->lastResponse){
		tsip_transac_send(TSIP_TRANSAC(self), TSIP_TRANSAC(self)->branch, self->lastResponse);
	}

	return 0;
}

/*	Proceeding --> (200-699) --> Completed
*/
int tsip_transac_nist_Proceeding_2_Completed_X_send_200_to_699(va_list *app)
{
	tsip_transac_nist_t *self = va_arg(*app, tsip_transac_nist_t *);
	const tsip_response_t *response = va_arg(*app, const tsip_response_t *);

	/*	RFC 3261 - 17.2.2
		If the TU passes a final response (status
		codes 200-699) to the server while in the "Proceeding" state, the
		transaction MUST enter the "Completed" state, and the response MUST
		be passed to the transport layer for transmission.
	*/
	tsip_transac_send(TSIP_TRANSAC(self), TSIP_TRANSAC(self)->branch, response);

	/*	RFC 3261 - 17.2.2
		When the server transaction enters the "Completed" state, it MUST set
		Timer J to fire in 64*T1 seconds for unreliable transports, and zero
		seconds for reliable transports.
	*/
	TRANSAC_NIST_TIMER_SCHEDULE(J);

	/* Update last response */
	TSK_OBJECT_SAFE_FREE(self->lastResponse);
	self->lastResponse = tsk_object_ref((void*)response);

	return 0;
}

/* Completed --> (INCOMING REQUEST) --> Completed
*/
int tsip_transac_nist_Completed_2_Completed_X_request(va_list *app)
{
	tsip_transac_nist_t *self = va_arg(*app, tsip_transac_nist_t *);
	const tsip_response_t *response = va_arg(*app, const tsip_response_t *);

	/*	RFC 3261 - 17.2.2
		While in the "Completed" state, the server transaction MUST pass the final response to the transport
		layer for retransmission whenever a retransmission of the request is received.
	*/
	if(self->lastResponse){
		tsip_transac_send(TSIP_TRANSAC(self), TSIP_TRANSAC(self)->branch, self->lastResponse);
	}

	return 0;
}

/* Complete --> (Timer J) --> Terminated
*/
int tsip_transac_nist_Completed_2_Terminated_X_tirmerJ(va_list *app)
{
	tsip_transac_nist_t *self = va_arg(*app, tsip_transac_nist_t *);
	const tsip_response_t *response = va_arg(*app, const tsip_response_t *);

	/*	RFC 3261 - 17.2.2
		The server transaction remains in this state (Completed) until Timer J fires, at
	    which point it MUST transition to the "Terminated" state.
	*/

	/*	RFC 3261 - 17.2.2
		THE SERVER TRANSACTION MUST BE DESTROYED THE INSTANT IT ENTERS THE "TERMINATED" STATE.
	*/
	return 0;
}

/* Any --> (Transport error) --> Terminated
*/
int tsip_transac_nist_Any_2_Terminated_X_transportError(va_list *app)
{
	tsip_transac_nist_t *self = va_arg(*app, tsip_transac_nist_t *);
	const tsip_response_t *response = va_arg(*app, const tsip_response_t *);

	return 0;
}

/* Any --> (error) --> Terminated
*/
int tsip_transac_nist_Any_2_Terminated_X_Error(va_list *app)
{
	tsip_transac_nist_t *self = va_arg(*app, tsip_transac_nist_t *);
	const tsip_response_t *response = va_arg(*app, const tsip_response_t *);

	return 0;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//				== STATE MACHINE END ==
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++





/*== Callback function called when the state machine enter in the "terminated" state.
*/
int tsip_transac_nist_OnTerminated(tsip_transac_nist_t *self)
{
	/* Cancel timers */
	TRANSAC_TIMER_CANCEL(J);

	TSIP_TRANSAC(self)->running = 0;

	TSK_DEBUG_INFO("=== NIST terminated ===");
	
	/* Remove (and destroy) the transaction from the layer. */
	return tsip_transac_remove(TSIP_TRANSAC(self));
}





















//========================================================
//	NIST object definition
//
static void* tsip_transac_nist_create(void * self, va_list * app)
{
	tsip_transac_nist_t *transac = self;
	if(transac)
	{
		const tsip_stack_handle_t *stack = va_arg(*app, const tsip_stack_handle_t *);
		unsigned reliable = va_arg(*app, unsigned);
		int32_t cseq_value = va_arg(*app, int32_t);
		const char *cseq_method = va_arg(*app, const char *);
		const char *callid = va_arg(*app, const char *);

		/* create FSM */
		transac->fsm = TSK_FSM_CREATE(_fsm_state_Started, _fsm_state_Terminated);
		transac->fsm->debug = DEBUG_STATE_MACHINE;
		tsk_fsm_set_callback_terminated(transac->fsm, TSK_FSM_ONTERMINATED(tsip_transac_nist_OnTerminated), (const void*)transac);

		/* Initialize base class */
		tsip_transac_init(TSIP_TRANSAC(transac), stack, tsip_nist, reliable, cseq_value, cseq_method, callid);

		/* Initialize NICT object */
		tsip_transac_nist_init(transac);
	}
	return self;
}

static void* tsip_transac_nist_destroy(void * self)
{ 
	tsip_transac_nist_t *transac = self;
	if(transac)
	{
		TSIP_TRANSAC(transac)->running = 0;
		TSK_OBJECT_SAFE_FREE(transac->lastResponse);

		/* DeInitialize base class */
		tsip_transac_deinit(TSIP_TRANSAC(transac));

		/* FSM */
		TSK_OBJECT_SAFE_FREE(transac->fsm);
	}
	return self;
}

static int tsip_transac_nist_cmp(const void *t1, const void *t2)
{
	return tsip_transac_cmp(t1, t2);
}

static const tsk_object_def_t tsip_transac_nist_def_s = 
{
	sizeof(tsip_transac_nist_t),
	tsip_transac_nist_create, 
	tsip_transac_nist_destroy,
	tsip_transac_nist_cmp, 
};
const void *tsip_transac_nist_def_t = &tsip_transac_nist_def_s;
