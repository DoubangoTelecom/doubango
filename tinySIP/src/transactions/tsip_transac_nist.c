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
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/transactions/tsip_transac_nist.h"

#include "tsk_debug.h"

#define DEBUG_STATE_MACHINE						1

#define TRANSAC_NIST_TIMER_SCHEDULE(TX)			TRANSAC_TIMER_SCHEDULE(nist, TX)
#define TRANSAC_NIST_SET_LAST_RESPONSE(self, response) \
	if(response){ \
		TSK_OBJECT_SAFE_FREE(self->lastResponse); \
		self->lastResponse = tsk_object_ref((void*)response); \
	}

/* ======================== internal functions ======================== */
int tsip_transac_nist_init(tsip_transac_nist_t *self);
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
int tsip_transac_nist_Any_2_Terminated_X_cancel(va_list *app); /* doubango-specific */

/* ======================== conds ======================== */

/* ======================== actions ======================== */
typedef enum _fsm_action_e
{
	_fsm_action_cancel = tsip_atype_cancel,

	_fsm_action_request = 0xFF,
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
	case tsip_transac_incoming_msg: /* From Transport Layer to Transaction Layer */
		{
			if(msg && TSIP_MESSAGE_IS_REQUEST(msg)){
				ret = tsip_transac_fsm_act(TSIP_TRANSAC(self), _fsm_action_request, msg);
			}
			break;
		}

	case tsip_transac_outgoing_msg: /* From TU to Transport Layer */
		{
			if(msg && TSIP_MESSAGE_IS_RESPONSE(msg))
			{
				if(TSIP_RESPONSE_IS_1XX(msg)){
					ret = tsip_transac_fsm_act(TSIP_TRANSAC(self), _fsm_action_send_1xx, msg);
				}
				else if(TSIP_RESPONSE_IS_23456(msg)){
					ret = tsip_transac_fsm_act(TSIP_TRANSAC(self), _fsm_action_send_200_to_699, msg);
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
			ret = tsip_transac_fsm_act(TSIP_TRANSAC(self), _fsm_action_error, msg);
			break;
		}

	case tsip_transac_transport_error:
		{
			ret = tsip_transac_fsm_act(TSIP_TRANSAC(self), _fsm_action_transporterror, msg);
			break;
		}
	}

	return ret;
}

int tsip_transac_nist_timer_callback(const tsip_transac_nist_t* self, tsk_timer_id_t timer_id)
{
	int ret = -1;

	if(self){
		if(timer_id == self->timerJ.id){
			ret = tsip_transac_fsm_act(TSIP_TRANSAC(self), _fsm_action_timerJ, tsk_null);
		}
	}

	return ret;
}

int tsip_transac_nist_init(tsip_transac_nist_t *self)
{
	/* Initialize the state machine.
	*/
	tsk_fsm_set(TSIP_TRANSAC_GET_FSM(self),
			
			/*=======================
			* === Started === 
			*/
			// Started -> (receive request) -> Trying
			TSK_FSM_ADD_ALWAYS(_fsm_state_Started, _fsm_action_request, _fsm_state_Trying, tsip_transac_nist_Started_2_Trying_X_request, "tsip_transac_nist_Started_2_Trying_X_request"),
			// Started -> (Any other) -> Started
			TSK_FSM_ADD_ALWAYS_NOTHING(_fsm_state_Started, "tsip_transac_nist_Started_2_Started_X_any"),

			/*=======================
			* === Trying === 
			*/
			// Trying -> (receive request retransmission) -> Trying
			TSK_FSM_ADD_ALWAYS(_fsm_state_Trying, _fsm_action_request, _fsm_state_Trying, tsk_null, "tsip_transac_nist_Trying_2_Trying_X_request"),
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
			// Any -> (cancel) -> Terminated
			TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, _fsm_action_cancel, _fsm_state_Terminated, tsip_transac_nist_Any_2_Terminated_X_cancel, "tsip_transac_nist_Any_2_Terminated_X_cancel"),
			
			TSK_FSM_ADD_NULL());

	/* Set callback function to call when new messages arrive or errors happen at
	the transport layer.
	*/
	TSIP_TRANSAC(self)->callback = TSIP_TRANSAC_EVENT_CALLBACK_F(tsip_transac_nist_event_callback);

	 return 0;
}

tsip_transac_nist_t* tsip_transac_nist_create(int32_t cseq_value, const char* cseq_method, const char* callid, tsip_transac_dst_t* dst)
{
	tsip_transac_nist_t* transac = tsk_object_new(tsip_transac_nist_def_t);
	
	if(transac){
		// initialize base class
		tsip_transac_init(TSIP_TRANSAC(transac), tsip_transac_type_nist, cseq_value, cseq_method, callid, dst, _fsm_state_Started, _fsm_state_Terminated);

		// init FSM
		TSIP_TRANSAC_GET_FSM(transac)->debug = DEBUG_STATE_MACHINE;
		tsk_fsm_set_callback_terminated(TSIP_TRANSAC_GET_FSM(transac), TSK_FSM_ONTERMINATED_F(tsip_transac_nist_OnTerminated), (const void*)transac);

		// initialize NICT object
		tsip_transac_nist_init(transac);
	}
	return transac;
}

int tsip_transac_nist_start(tsip_transac_nist_t *self, const tsip_request_t* request)
{
	int ret = -1;

	if(self && !TSIP_TRANSAC(self)->running && request){
		TSIP_TRANSAC(self)->running = 1;
		if((ret = tsip_transac_fsm_act(TSIP_TRANSAC(self), _fsm_action_request, request))){
			//
		}
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
	const tsip_request_t *request = va_arg(*app, const tsip_request_t *);

	if(TNET_SOCKET_TYPE_IS_VALID(request->src_net_type)){
		TSIP_TRANSAC(self)->reliable = TNET_SOCKET_TYPE_IS_STREAM(request->src_net_type);
	}

	/* Set Timers */
	self->timerJ.timeout = TSIP_TRANSAC(self)->reliable ? 0 : TSIP_TIMER_GET(J); /* RFC 3261 - 17.2.2*/

	/*	RFC 3261 - 17.2.2
		The state machine is initialized in the "Trying" state and is passed
		a request other than INVITE or ACK when initialized.  This request is
		passed up to the TU.  Once in the "Trying" state, any further request
		retransmissions are discarded.  A request is a retransmission if it
		matches the same server transaction, using the rules specified in
		Section 17.2.3.
	*/
	return tsip_transac_deliver(TSIP_TRANSAC(self), tsip_dialog_i_msg, request);
}

/* Trying --> (1xx) --> Proceeding
*/
int tsip_transac_nist_Trying_2_Proceeding_X_send_1xx(va_list *app)
{
	tsip_transac_nist_t *self = va_arg(*app, tsip_transac_nist_t *);
	const tsip_response_t *response = va_arg(*app, const tsip_response_t *);
	int ret;

	/*	RFC 3261 - 17.2.2
		While in the "Trying" state, if the TU passes a provisional response
		to the server transaction, the server transaction MUST enter the
		"Proceeding" state.  The response MUST be passed to the transport
		layer for transmission.
	*/
	ret = tsip_transac_send(TSIP_TRANSAC(self), TSIP_TRANSAC(self)->branch, TSIP_MESSAGE(response));

	/* Update last response */
	TRANSAC_NIST_SET_LAST_RESPONSE(self, response);

	return ret;
}

/*	Trying --> (200-699) --> Completed
*/
int tsip_transac_nist_Trying_2_Completed_X_send_200_to_699(va_list *app)
{
	tsip_transac_nist_t *self = va_arg(*app, tsip_transac_nist_t *);
	const tsip_response_t *response = va_arg(*app, const tsip_response_t *);
	int ret;

	ret = tsip_transac_send(TSIP_TRANSAC(self), TSIP_TRANSAC(self)->branch, TSIP_MESSAGE(response));

	/*	RFC 3261 - 17.2.2
		When the server transaction enters the "Completed" state, it MUST set
		Timer J to fire in 64*T1 seconds for unreliable transports, and zero
		seconds for reliable transports.
	*/
	TRANSAC_NIST_TIMER_SCHEDULE(J);

	/* Update last response */
	TRANSAC_NIST_SET_LAST_RESPONSE(self, response);

	return ret;
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
	tsip_transac_send(TSIP_TRANSAC(self), TSIP_TRANSAC(self)->branch, TSIP_MESSAGE(response));

	/* Update last response */
	TRANSAC_NIST_SET_LAST_RESPONSE(self, response);

	return 0;
}

/* Proceeding -> (INCOMING REQUEST) -> Proceeding
*/
int tsip_transac_nist_Proceeding_2_Proceeding_X_request(va_list *app)
{
	tsip_transac_nist_t *self = va_arg(*app, tsip_transac_nist_t *);
	/*const tsip_response_t *response = va_arg(*app, const tsip_response_t *);*/

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
	int ret;

	/*	RFC 3261 - 17.2.2
		If the TU passes a final response (status
		codes 200-699) to the server while in the "Proceeding" state, the
		transaction MUST enter the "Completed" state, and the response MUST
		be passed to the transport layer for transmission.
	*/
	ret = tsip_transac_send(TSIP_TRANSAC(self), TSIP_TRANSAC(self)->branch, TSIP_MESSAGE(response));

	/*	RFC 3261 - 17.2.2
		When the server transaction enters the "Completed" state, it MUST set
		Timer J to fire in 64*T1 seconds for unreliable transports, and zero
		seconds for reliable transports.
	*/
	TRANSAC_NIST_TIMER_SCHEDULE(J);

	/* Update last response */
	TRANSAC_NIST_SET_LAST_RESPONSE(self, response);

	return ret;
}

/* Completed --> (INCOMING REQUEST) --> Completed
*/
int tsip_transac_nist_Completed_2_Completed_X_request(va_list *app)
{
	tsip_transac_nist_t *self = va_arg(*app, tsip_transac_nist_t *);
	/*const tsip_response_t *response = va_arg(*app, const tsip_response_t *);*/

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
	/*tsip_transac_nist_t *self = va_arg(*app, tsip_transac_nist_t *);*/
	/*const tsip_response_t *response = va_arg(*app, const tsip_response_t *);*/

	/*	RFC 3261 - 17.2.2
		The server transaction remains in this state (Completed) until Timer J fires, at
	    which point it MUST transition to the "Terminated" state.
	*/

	/*	RFC 3261 - 17.2.2
		THE SERVER TRANSACTION MUST BE DESTROYED THE INSTANT IT ENTERS THE "TERMINATED" STATE.
	*/
	return 0;
}

/* Any -> (Transport Error) -> Terminated
*/
int tsip_transac_nist_Any_2_Terminated_X_transportError(va_list *app)
{
	tsip_transac_nist_t *self = va_arg(*app, tsip_transac_nist_t *);
	//const tsip_message_t *message = va_arg(*app, const tsip_message_t *);

	/* Timers will be canceled by "tsip_transac_nict_OnTerminated" */

	return tsip_transac_deliver(TSIP_TRANSAC(self), tsip_dialog_transport_error, tsk_null);
}

/* Any -> (Error) -> Terminated
*/
int tsip_transac_nist_Any_2_Terminated_X_Error(va_list *app)
{
	tsip_transac_nist_t *self = va_arg(*app, tsip_transac_nist_t *);
	//const tsip_message_t *message = va_arg(*app, const tsip_message_t *);

	/* Timers will be canceled by "tsip_transac_nict_OnTerminated" */

	return tsip_transac_deliver(TSIP_TRANSAC(self), tsip_dialog_error, tsk_null);
}

/* Any -> (cancel) -> Terminated
*/
int tsip_transac_nist_Any_2_Terminated_X_cancel(va_list *app)
{
	/* doubango-specific */
	return 0;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//				== STATE MACHINE END ==
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++





/*== Callback function called when the state machine enter in the "terminated" state.
*/
int tsip_transac_nist_OnTerminated(tsip_transac_nist_t *self)
{
	TSK_DEBUG_INFO("=== NIST terminated ===");
	
	/* Remove (and destroy) the transaction from the layer. */
	return tsip_transac_remove(TSIP_TRANSAC(self));
}





















//========================================================
//	NIST object definition
//
static tsk_object_t* tsip_transac_nist_ctor(tsk_object_t * self, va_list * app)
{
	tsip_transac_nist_t *transac = self;
	if(transac){
	}
	return self;
}

static tsk_object_t* tsip_transac_nist_dtor(tsk_object_t * _self)
{ 
	tsip_transac_nist_t *self = _self;
	if(self){
		/* Cancel timers */
		TRANSAC_TIMER_CANCEL(J);

		TSIP_TRANSAC(self)->running = tsk_false;
		TSK_OBJECT_SAFE_FREE(self->lastResponse);

		/* DeInitialize base class */
		tsip_transac_deinit(TSIP_TRANSAC(self));

		TSK_DEBUG_INFO("*** NIST destroyed ***");
	}
	return _self;
}

static int tsip_transac_nist_cmp(const tsk_object_t *t1, const tsk_object_t *t2)
{
	return tsip_transac_cmp(t1, t2);
}

static const tsk_object_def_t tsip_transac_nist_def_s = 
{
	sizeof(tsip_transac_nist_t),
	tsip_transac_nist_ctor, 
	tsip_transac_nist_dtor,
	tsip_transac_nist_cmp, 
};
const tsk_object_def_t *tsip_transac_nist_def_t = &tsip_transac_nist_def_s;
