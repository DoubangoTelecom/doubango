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

									   |INVITE
                                       |pass INV to TU
                    INVITE             V send 100 if TU won't in 200ms
                    send response+------------+
                        +--------|            |--------+ 101-199 from TU
                        |        |            |        | send response
                        +------->|            |<-------+
                                 | Proceeding |
                                 |            |--------+ Transport Err.
                                 |            |        | Inform TU
                                 |            |<-------+
                                 +------------+
                    300-699 from TU |    |2xx from TU
                    send response   |    |send response
                     +--------------+    +------------+
                     |                                |
    INVITE           V          Timer G fires         |
    send response +-----------+ send response         |
         +--------|           |--------+              |
         |        |           |        |              |
         +------->| Completed |<-------+      INVITE  |  Transport Err.
                  |           |               -       |  Inform TU
         +--------|           |----+          +-----+ |  +---+
         |        +-----------+    | ACK      |     | v  |   v
         |          ^   |          | -        |  +------------+
         |          |   |          |          |  |            |
         +----------+   |          |          +->|  Accepted  |
         Transport Err. |          |             |            |
         Inform TU      |          V             +------------+
                        |      +-----------+        |  ^     |
                        |      |           |        |  |     |
                        |      | Confirmed |        |  +-----+
                        |      |           |        |  2xx from TU
          Timer H fires |      +-----------+        |  send response
          -             |          |                |
                        |          | Timer I fires  |
                        |          | -              | Timer L fires
                        |          V                | -
                        |        +------------+     |
                        |        |            |<----+
                        +------->| Terminated |
                                 |            |
                                 +------------+



                   draft-sparks-sip-invfix-03 - Figure 5: INVITE server transaction

=============================================================================*/

/**@file tsip_transac_ist.c
 * @brief SIP INVITE Server Transaction as per RFC 3261 subclause 17.2.1.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/transactions/tsip_transac_ist.h"


#include "tsk_debug.h"

#define DEBUG_STATE_MACHINE						1

#define TRANSAC_IST_TIMER_SCHEDULE(TX)			TRANSAC_TIMER_SCHEDULE(ist, TX)
#define TRANSAC_IST_SET_LAST_RESPONSE(self, response) \
	if(response){ \
		TSK_OBJECT_SAFE_FREE(self->lastResponse); \
		self->lastResponse = tsk_object_ref((void*)response); \
	}

/* ======================== internal functions ======================== */
static int tsip_transac_ist_init(tsip_transac_ist_t *self);
static int tsip_transac_ist_OnTerminated(tsip_transac_ist_t *self);

/* ======================== transitions ======================== */
static int tsip_transac_ist_Started_2_Proceeding_X_INVITE(va_list *app);
static int tsip_transac_ist_Proceeding_2_Proceeding_X_INVITE(va_list *app);
static int tsip_transac_ist_Proceeding_2_Proceeding_X_1xx(va_list *app);
static int tsip_transac_ist_Proceeding_2_Completed_X_300_to_699(va_list *app);
static int tsip_transac_ist_Proceeding_2_Accepted_X_2xx(va_list *app);
static int tsip_transac_ist_Completed_2_Completed_INVITE(va_list *app);
static int tsip_transac_ist_Completed_2_Completed_timerG(va_list *app);
static int tsip_transac_ist_Completed_2_Terminated_timerH(va_list *app);
static int tsip_transac_ist_Completed_2_Confirmed_ACK(va_list *app);
static int tsip_transac_ist_Accepted_2_Accepted_INVITE(va_list *app);
static int tsip_transac_ist_Accepted_2_Accepted_2xx(va_list *app);
static int tsip_transac_ist_Accepted_2_Accepted_timerX(va_list *app);  /* doubango-specific */
static int tsip_transac_ist_Accepted_2_Accepted_iACK(va_list *app);  /* doubango-specific */
static int tsip_transac_ist_Accepted_2_Terminated_timerL(va_list *app);
static int tsip_transac_ist_Confirmed_2_Terminated_timerI(va_list *app);
static int tsip_transac_ist_Any_2_Terminated_X_transportError(va_list *app);
static int tsip_transac_ist_Any_2_Terminated_X_Error(va_list *app);
static int tsip_transac_ist_Any_2_Terminated_X_cancel(va_list *app); /* doubango-specific */

/* ======================== conds ======================== */
static tsk_bool_t _fsm_cond_is_resp2INVITE(tsip_transac_ist_t* self, tsip_message_t* message)
{
    return TSIP_RESPONSE_IS_TO_INVITE(message);
}

/* ======================== actions ======================== */
typedef enum _fsm_action_e {
    _fsm_action_cancel = tsip_atype_cancel,

    _fsm_action_recv_INVITE = 0xFF,
    _fsm_action_recv_ACK,
    _fsm_action_send_1xx,
    _fsm_action_send_2xx,
    _fsm_action_send_300_to_699,
    _fsm_action_send_non1xx,
    _fsm_action_timerH,
    _fsm_action_timerI,
    _fsm_action_timerG,
    _fsm_action_timerL,
    _fsm_action_timerX,
    _fsm_action_transporterror,
    _fsm_action_error,
}
_fsm_action_t;

/* ======================== states ======================== */
typedef enum _fsm_state_e {
    _fsm_state_Started,
    _fsm_state_Proceeding,
    _fsm_state_Completed,
    _fsm_state_Accepted,
    _fsm_state_Confirmed,
    _fsm_state_Terminated
}
_fsm_state_t;

int tsip_transac_ist_event_callback(const tsip_transac_ist_t *self, tsip_transac_event_type_t type, const tsip_message_t *msg)
{
    int ret = -1;

    switch(type) {
    case tsip_transac_incoming_msg: { /* From Transport Layer to Transaction Layer */
        if(msg && TSIP_MESSAGE_IS_REQUEST(msg)) {
            if(TSIP_REQUEST_IS_INVITE(msg)) {
                ret = tsip_transac_fsm_act(TSIP_TRANSAC(self), _fsm_action_recv_INVITE, msg);
            }
            else if(TSIP_REQUEST_IS_ACK(msg)) {
                ret = tsip_transac_fsm_act(TSIP_TRANSAC(self), _fsm_action_recv_ACK, msg);
            }
        }
        break;
    }

    case tsip_transac_outgoing_msg: { /* From TU to Transport Layer */
        if(msg && TSIP_MESSAGE_IS_RESPONSE(msg)) {
            if(TSIP_RESPONSE_IS_1XX(msg)) {
                ret = tsip_transac_fsm_act(TSIP_TRANSAC(self), _fsm_action_send_1xx, msg);
            }
            else if(TSIP_RESPONSE_IS_2XX(msg)) {
                ret = tsip_transac_fsm_act(TSIP_TRANSAC(self), _fsm_action_send_2xx, msg);
            }
            else if(TSIP_RESPONSE_IS_3456(msg)) {
                ret = tsip_transac_fsm_act(TSIP_TRANSAC(self), _fsm_action_send_300_to_699, msg);
            }
        }
        break;
    }

    case tsip_transac_canceled:
    case tsip_transac_terminated:
    case tsip_transac_timedout:
        break;

    case tsip_transac_error: {
        ret = tsip_transac_fsm_act(TSIP_TRANSAC(self), _fsm_action_error, msg);
        break;
    }

    case tsip_transac_transport_error: {
        ret = tsip_transac_fsm_act(TSIP_TRANSAC(self), _fsm_action_transporterror, msg);
        break;
    }
    }

    return ret;
}

int tsip_transac_ist_timer_callback(const tsip_transac_ist_t* self, tsk_timer_id_t timer_id)
{
    int ret = -1;

    if(self) {
        if(timer_id == self->timerH.id) {
            ret = tsip_transac_fsm_act(TSIP_TRANSAC(self), _fsm_action_timerH, tsk_null);
        }
        else if(timer_id == self->timerI.id) {
            ret = tsip_transac_fsm_act(TSIP_TRANSAC(self), _fsm_action_timerI, tsk_null);
        }
        else if(timer_id == self->timerG.id) {
            ret = tsip_transac_fsm_act(TSIP_TRANSAC(self), _fsm_action_timerG, tsk_null);
        }
        else if(timer_id == self->timerL.id) {
            ret = tsip_transac_fsm_act(TSIP_TRANSAC(self), _fsm_action_timerL, tsk_null);
        }
        else if(timer_id == self->timerX.id) {
            ret = tsip_transac_fsm_act(TSIP_TRANSAC(self), _fsm_action_timerX, tsk_null);
        }
    }

    return ret;
}

int tsip_transac_ist_init(tsip_transac_ist_t *self)
{
    /* Initialize the state machine.
    */
    tsk_fsm_set(TSIP_TRANSAC_GET_FSM(self),

                /*=======================
                * === Started ===
                */
                // Started -> (recv INVITE) -> Proceeding
                TSK_FSM_ADD_ALWAYS(_fsm_state_Started, _fsm_action_recv_INVITE, _fsm_state_Proceeding, tsip_transac_ist_Started_2_Proceeding_X_INVITE, "tsip_transac_ist_Started_2_Proceeding_X_INVITE"),
                // Started -> (Any other) -> Started
                TSK_FSM_ADD_ALWAYS_NOTHING(_fsm_state_Started, "tsip_transac_ist_Started_2_Started_X_any"),

                /*=======================
                * === Proceeding ===
                */
                // Proceeding -> (recv INVITE) -> Proceeding
                TSK_FSM_ADD_ALWAYS(_fsm_state_Proceeding, _fsm_action_recv_INVITE, _fsm_state_Proceeding, tsip_transac_ist_Proceeding_2_Proceeding_X_INVITE, "tsip_transac_ist_Proceeding_2_Proceeding_X_INVITE"),
                // Proceeding -> (send 1xx) -> Proceeding
                TSK_FSM_ADD(_fsm_state_Proceeding, _fsm_action_send_1xx, _fsm_cond_is_resp2INVITE, _fsm_state_Proceeding, tsip_transac_ist_Proceeding_2_Proceeding_X_1xx, "tsip_transac_ist_Proceeding_2_Proceeding_X_1xx"),
                // Proceeding -> (send 300to699) -> Completed
                TSK_FSM_ADD(_fsm_state_Proceeding, _fsm_action_send_300_to_699, _fsm_cond_is_resp2INVITE, _fsm_state_Completed, tsip_transac_ist_Proceeding_2_Completed_X_300_to_699, "tsip_transac_ist_Proceeding_2_Completed_X_300_to_699"),
                // Proceeding -> (send 2xx) -> Accepted
                TSK_FSM_ADD(_fsm_state_Proceeding, _fsm_action_send_2xx, _fsm_cond_is_resp2INVITE, _fsm_state_Accepted, tsip_transac_ist_Proceeding_2_Accepted_X_2xx, "tsip_transac_ist_Proceeding_2_Accepted_X_2xx"),

                /*=======================
                * === Completed ===
                */
                // Completed -> (recv INVITE) -> Completed
                TSK_FSM_ADD_ALWAYS(_fsm_state_Completed, _fsm_action_recv_INVITE, _fsm_state_Completed, tsip_transac_ist_Completed_2_Completed_INVITE, "tsip_transac_ist_Completed_2_Completed_INVITE"),
                // Completed -> (timer G) -> Completed
                TSK_FSM_ADD_ALWAYS(_fsm_state_Completed, _fsm_action_timerG, _fsm_state_Completed, tsip_transac_ist_Completed_2_Completed_timerG, "tsip_transac_ist_Completed_2_Completed_timerG"),
                // Completed -> (timerH) -> Terminated
                TSK_FSM_ADD_ALWAYS(_fsm_state_Completed, _fsm_action_timerH, _fsm_state_Terminated, tsip_transac_ist_Completed_2_Terminated_timerH, "tsip_transac_ist_Completed_2_Terminated_timerH"),
                // Completed -> (recv ACK) -> Confirmed
                TSK_FSM_ADD_ALWAYS(_fsm_state_Completed, _fsm_action_recv_ACK, _fsm_state_Confirmed, tsip_transac_ist_Completed_2_Confirmed_ACK, "tsip_transac_ist_Completed_2_Confirmed_ACK"),

                /*=======================
                * === Accepted ===
                */
                // Accepted -> (recv INVITE) -> Accepted
                TSK_FSM_ADD_ALWAYS(_fsm_state_Accepted, _fsm_action_recv_INVITE, _fsm_state_Accepted, tsip_transac_ist_Accepted_2_Accepted_INVITE, "tsip_transac_ist_Accepted_2_Accepted_INVITE"),
                // Accepted -> (send 2xx) -> Accepted
                TSK_FSM_ADD(_fsm_state_Accepted, _fsm_action_send_2xx, _fsm_cond_is_resp2INVITE, _fsm_state_Accepted, tsip_transac_ist_Accepted_2_Accepted_2xx, "tsip_transac_ist_Accepted_2_Accepted_2xx"),
                // Accepted -> (timer X) -> Accepted
                TSK_FSM_ADD_ALWAYS(_fsm_state_Accepted, _fsm_action_timerX, _fsm_state_Accepted, tsip_transac_ist_Accepted_2_Accepted_timerX, "tsip_transac_ist_Accepted_2_Accepted_timerX"),
                // Accepted -> (recv ACK) -> Accepted
                TSK_FSM_ADD_ALWAYS(_fsm_state_Accepted, _fsm_action_recv_ACK, _fsm_state_Accepted, tsip_transac_ist_Accepted_2_Accepted_iACK, "tsip_transac_ist_Accepted_2_Accepted_iACK"),
                // Accepted -> (timerL) -> Terminated
                TSK_FSM_ADD_ALWAYS(_fsm_state_Accepted, _fsm_action_timerL, _fsm_state_Terminated, tsip_transac_ist_Accepted_2_Terminated_timerL, "tsip_transac_ist_Accepted_2_Terminated_timerL"),

                /*=======================
                * === Confirmed ===
                */
                // Confirmed -> (timerI) -> Terminated
                TSK_FSM_ADD_ALWAYS(_fsm_state_Confirmed, _fsm_action_timerI, _fsm_state_Terminated, tsip_transac_ist_Confirmed_2_Terminated_timerI, "tsip_transac_ist_Confirmed_2_Terminated_timerI"),


                /*=======================
                * === Any ===
                */
                // Any -> (transport error) -> Terminated
                TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, _fsm_action_transporterror, _fsm_state_Terminated, tsip_transac_ist_Any_2_Terminated_X_transportError, "tsip_transac_ist_Any_2_Terminated_X_transportError"),
                // Any -> (transport error) -> Terminated
                TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, _fsm_action_error, _fsm_state_Terminated, tsip_transac_ist_Any_2_Terminated_X_Error, "tsip_transac_ist_Any_2_Terminated_X_Error"),
                // Any -> (cancel) -> Terminated
                TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, _fsm_action_cancel, _fsm_state_Terminated, tsip_transac_ist_Any_2_Terminated_X_cancel, "tsip_transac_ist_Any_2_Terminated_X_cancel"),


                TSK_FSM_ADD_NULL());

    /* Set callback function to call when new messages arrive or errors happen at
    the transport layer.
    */
    TSIP_TRANSAC(self)->callback = TSIP_TRANSAC_EVENT_CALLBACK_F(tsip_transac_ist_event_callback);

    /* Set Timers
    	* RFC 3261 17.2.1: For unreliable transports, timer G is set to fire in T1 seconds, and is not set to fire for
    	reliable transports.
    */
    self->timerH.timeout = TSIP_TIMER_GET(H);
    self->timerG.timeout = TSIP_TIMER_GET(G);
    self->timerL.timeout = TSIP_TIMER_GET(L);
    self->timerX.timeout = TSIP_TIMER_GET(G);

    return 0;
}

tsip_transac_ist_t* tsip_transac_ist_create(int32_t cseq_value, const char* callid, tsip_transac_dst_t* dst)
{
    tsip_transac_ist_t* transac = tsk_object_new(tsip_transac_ist_def_t);
    if(transac) {
        // initialize base class
        tsip_transac_init(TSIP_TRANSAC(transac), tsip_transac_type_ist, cseq_value, "INVITE", callid, dst, _fsm_state_Started, _fsm_state_Terminated);

        // init FSM
        TSIP_TRANSAC_GET_FSM(transac)->debug = DEBUG_STATE_MACHINE;
        tsk_fsm_set_callback_terminated(TSIP_TRANSAC_GET_FSM(transac), TSK_FSM_ONTERMINATED_F(tsip_transac_ist_OnTerminated), (const void*)transac);

        // initialize IST object
        tsip_transac_ist_init(transac);
    }
    return transac;
}

int tsip_transac_ist_start(tsip_transac_ist_t *self, const tsip_request_t* request)
{
    int ret = -1;

    if(self && !TSIP_TRANSAC(self)->running && request) {
        TSIP_TRANSAC(self)->running = 1;
        if((ret = tsip_transac_fsm_act(TSIP_TRANSAC(self), _fsm_action_recv_INVITE, request))) {
            //
        }
    }
    return ret;
}





//--------------------------------------------------------
//				== STATE MACHINE BEGIN ==
//--------------------------------------------------------

/*	Started --> (recv INVITE) --> Proceeding
*/
int tsip_transac_ist_Started_2_Proceeding_X_INVITE(va_list *app)
{
    tsip_transac_ist_t *self = va_arg(*app, tsip_transac_ist_t *);
    const tsip_request_t *request = va_arg(*app, const tsip_request_t *);
    int ret = -1;

    if(TNET_SOCKET_TYPE_IS_VALID(request->src_net_type)) {
        TSIP_TRANSAC(self)->reliable = TNET_SOCKET_TYPE_IS_STREAM(request->src_net_type);
    }

    /* Set Timers */
    self->timerI.timeout =  TSIP_TRANSAC(self)->reliable ? 0 : TSIP_TIMER_GET(I);

    /*	RFC 3261 - 17.2.1 INVITE Server Transaction
    	When a server transaction is constructed for a request, it enters the
    	"Proceeding" state.  The server transaction MUST generate a 100
    	(Trying) response unless it knows that the TU will generate a
    	provisional or final response within 200 ms, in which case it MAY
    	generate a 100 (Trying) response.

    	RFC 3262 - 3. UAS Behavior
    	A UAS MUST NOT attempt to send a 100 (Trying) response reliably.
    */
    if(request) {
        tsip_response_t* response;
        if((response = tsip_response_new(100, "Trying (sent from the Transaction Layer)", request))) {
            ret = tsip_transac_send(TSIP_TRANSAC(self), TSIP_TRANSAC(self)->branch, response);
            TRANSAC_IST_SET_LAST_RESPONSE(self, response); // Update last response
            TSK_OBJECT_SAFE_FREE(response);
        }
    }
    if(!ret) { /* Send "100 Trying" is OK ==> alert dialog for the incoming INVITE */
        ret = tsip_transac_deliver(TSIP_TRANSAC(self), tsip_dialog_i_msg, request);
    }
    return ret;
}

/*	Proceeding --> (recv INVITE) --> Proceeding
*/
int tsip_transac_ist_Proceeding_2_Proceeding_X_INVITE(va_list *app)
{
    tsip_transac_ist_t *self = va_arg(*app, tsip_transac_ist_t *);
    //const tsip_request_t *request = va_arg(*app, const tsip_request_t *);
    int ret = -1;

    /*	RFC 3261 - 17.2.1 INVITE Server Transaction
    	If a request retransmission is received while in the "Proceeding" state, the most
    	recent provisional response that was received from the TU MUST be
    	passed to the transport layer for retransmission.
    */
    if(self->lastResponse) {
        ret = tsip_transac_send(TSIP_TRANSAC(self), TSIP_TRANSAC(self)->branch, self->lastResponse);
    }

    return ret;
}

/*	Proceeding --> (send 1xx) --> Proceeding
*/
int tsip_transac_ist_Proceeding_2_Proceeding_X_1xx(va_list *app)
{
    tsip_transac_ist_t *self = va_arg(*app, tsip_transac_ist_t *);
    const tsip_response_t *response = va_arg(*app, const tsip_response_t *);
    int ret;

    /* Send to the transport layer */
    ret = tsip_transac_send(TSIP_TRANSAC(self), TSIP_TRANSAC(self)->branch, TSIP_MESSAGE(response));

    /* Update last response */
    TRANSAC_IST_SET_LAST_RESPONSE(self, response);

    return ret;
}

/*	Proceeding --> (send 300-699) --> Completed
*/
int tsip_transac_ist_Proceeding_2_Completed_X_300_to_699(va_list *app)
{
    tsip_transac_ist_t *self = va_arg(*app, tsip_transac_ist_t *);
    const tsip_response_t *response = va_arg(*app, const tsip_response_t *);
    int ret;

    /*	RFC 3264 17.2.1 INVITE Server Transaction
    	While in the "Proceeding" state, if the TU passes a response with
    	status code from 300 to 699 to the server transaction, the response
    	MUST be passed to the transport layer for transmission, and the state
    	machine MUST enter the "Completed" state. For unreliable transports, timer G is set to fire in T1 seconds,
    	and is not set to fire for reliable transports.
    */
    if(!TSIP_TRANSAC(self)->reliable) {
        TRANSAC_IST_TIMER_SCHEDULE(G);
    }

    /* Send to the transport layer */
    ret = tsip_transac_send(TSIP_TRANSAC(self), TSIP_TRANSAC(self)->branch, TSIP_MESSAGE(response));

    /* Update last response */
    TRANSAC_IST_SET_LAST_RESPONSE(self, response);

    /* RFC 3261 - 17.2.1 INVITE Server Transaction
    	When the "Completed" state is entered, timer H MUST be set to fire in
    	64*T1 seconds for all transports.
    */
    TRANSAC_IST_TIMER_SCHEDULE(H);

    return ret;
}

/*	Proceeding --> (send 2xx) --> Accepted
*/
int tsip_transac_ist_Proceeding_2_Accepted_X_2xx(va_list *app)
{
    tsip_transac_ist_t *self = va_arg(*app, tsip_transac_ist_t *);
    const tsip_response_t *response = va_arg(*app, const tsip_response_t *);
    int ret = -1;

    /*	draft-sparks-sip-invfix-03 - 8.5. Pages 134 to 135
    	If, while in the "Proceeding" state, the TU passes a 2xx response
    	to the server transaction, the server transaction MUST pass this
    	response to the transport layer for transmission.  It is not
    	retransmitted by the server transaction; retransmissions of 2xx
    	responses are handled by the TU.  The server transaction MUST then
    	transition to the "Accepted" state.
    */
    ret = tsip_transac_send(TSIP_TRANSAC(self), TSIP_TRANSAC(self)->branch, TSIP_MESSAGE(response));

    /* Update last response */
    TRANSAC_IST_SET_LAST_RESPONSE(self, response);

    /* RFC 3261 - 13.3.1.4 The INVITE is Accepted
    	Since 2xx is retransmitted end-to-end, there may be hops between
    	UAS and UAC that are UDP.  To ensure reliable delivery across
    	these hops, the response is retransmitted periodically even if the
    	transport at the UAS is reliable.
    */
    TRANSAC_IST_TIMER_SCHEDULE(X);
    self->timerX.timeout <<= 1;

    /*	draft-sparks-sip-invfix-03 - 8.7. Page 137
    	When the INVITE server transaction enters the "Accepted" state,
    	Timer L MUST be set to fire in 64*T1 for all transports.  This
    	value matches both Timer B in the next upstream client state
    	machine (the amount of time the previous hop will wait for a
    	response when no provisionals have been sent) and the amount of
    	time this (or any downstream) UAS core might be retransmitting the
    	2xx while waiting for an ACK.
    */
    TRANSAC_IST_TIMER_SCHEDULE(L);

    return ret;
}

/*	Completed --> (recv INVITE) --> Completed
*/
int tsip_transac_ist_Completed_2_Completed_INVITE(va_list *app)
{
    tsip_transac_ist_t *self = va_arg(*app, tsip_transac_ist_t *);
    //const tsip_message_t *message = va_arg(*app, const tsip_message_t *);
    int ret = -1;

    /*	RFC 3261 - 17.2.1 INVITE Server Transaction
    	Furthermore, while in the "Completed" state, if a request retransmission is
    	received, the server SHOULD pass the response to the transport for
    	retransmission.
    */
    if(self->lastResponse) {
        ret = tsip_transac_send(TSIP_TRANSAC(self), TSIP_TRANSAC(self)->branch, self->lastResponse);
    }

    return ret;
}

/*	Completed --> (timerG) --> Completed
*/
int tsip_transac_ist_Completed_2_Completed_timerG(va_list *app)
{
    tsip_transac_ist_t *self = va_arg(*app, tsip_transac_ist_t *);
    //const tsip_message_t *message = va_arg(*app, const tsip_message_t *);
    int ret = -1;

    /*	RFC 3261 - 17.2.1 INVITE Server Transaction
    	If timer G fires, the response is passed to the transport layer once
    	more for retransmission, and timer G is set to fire in MIN(2*T1, T2) seconds.
    	From then on, when timer G fires, the response is passed to the transport again for
    	transmission, and timer G is reset with a value that doubles, unless
    	that value exceeds T2, in which case it is reset with the value of T2.
    */
    if(self->lastResponse) {
        ret = tsip_transac_send(TSIP_TRANSAC(self), TSIP_TRANSAC(self)->branch, self->lastResponse);
    }
    self->timerG.timeout = TSK_MIN(self->timerG.timeout*2, TSIP_TIMER_GET(T2));
    TRANSAC_IST_TIMER_SCHEDULE(G);

    return ret;
}

/*	Completed --> (timerH) --> Terminated
*/
int tsip_transac_ist_Completed_2_Terminated_timerH(va_list *app)
{
    tsip_transac_ist_t *self = va_arg(*app, tsip_transac_ist_t *);
    //const tsip_message_t *message = va_arg(*app, const tsip_message_t *);

    /*	RFC 3261 - 17.2.1 INVITE Server Transaction
    	If timer H fires while in the "Completed" state, it implies that the
    	ACK was never received.  In this case, the server transaction MUST
    	transition to the "Terminated" state, and MUST indicate to the TU
    	that a transaction failure has occurred.
    */
    return tsip_transac_deliver(TSIP_TRANSAC(self), tsip_dialog_transport_error, tsk_null);
}

/*	Completed --> (recv ACK) --> Confirmed
*/
int tsip_transac_ist_Completed_2_Confirmed_ACK(va_list *app)
{
    tsip_transac_ist_t *self = va_arg(*app, tsip_transac_ist_t *);
    //const tsip_message_t *message = va_arg(*app, const tsip_message_t *);

    /*	RFC 3261 - 17.2.1 INVITE Server Transaction
    	If an ACK is received while the server transaction is in the
    	"Completed" state, the server transaction MUST transition to the
    	"Confirmed" state.  As Timer G is ignored in this state, any
    	retransmissions of the response will cease
    */
    TRANSAC_TIMER_CANCEL(G);/* To avoid warnings from FSM manager. */

    /*	RFC 3261 - 17.2.1 INVITE Server Transaction
    	The purpose of the "Confirmed" state is to absorb any additional ACK
    	messages that arrive, triggered from retransmissions of the final
    	response.  When this state is entered, timer I is set to fire in T4
    	seconds for unreliable transports, and zero seconds for reliable
    	transports.
    */
    TRANSAC_IST_TIMER_SCHEDULE(I); /* Has the right value (zero of reliable and ...) */


    return 0;
}

/*	Accepted --> (recv INVITE) --> Accepted
*/
int tsip_transac_ist_Accepted_2_Accepted_INVITE(va_list *app)
{
    tsip_transac_ist_t *self = va_arg(*app, tsip_transac_ist_t *);

    /*	draft-sparks-sip-invfix-03 - 8.7. Page 137
    	The purpose of the "Accepted" state is to absorb retransmissions
    	of an accepted INVITE request.  Any such retransmissions are
    	absorbed entirely within the server transaction.  They are not
    	passed up to the TU since any downstream UAS cores that accepted
    	the request have taken responsibility for reliability and will
    	already retransmit their 2xx responses if neccessary.
    */

    /*	Do not pass to the TU (see above)
    	VERY IMPORTANT: INVITE dialog is responsible for reliability of the 2xx response.
    */
    if(self->lastResponse) {
        return tsip_transac_send(TSIP_TRANSAC(self), TSIP_TRANSAC(self)->branch, self->lastResponse);
    }
    return 0;
}

/*	Accepted --> (send 2xx) --> Accepted
*/
int tsip_transac_ist_Accepted_2_Accepted_2xx(va_list *app)
{
    tsip_transac_ist_t *self = va_arg(*app, tsip_transac_ist_t *);
    const tsip_response_t *response = va_arg(*app, const tsip_response_t *);
    int ret;
    /*	draft-sparks-sip-invfix-03 - 8.7. Page 137
    	While in the "Accepted" state, if the TU passes a 2xx response,
    	the server transaction MUST pass the response to the transport
    	layer for transmission.
    */
    ret = tsip_transac_send(TSIP_TRANSAC(self), TSIP_TRANSAC(self)->branch, TSIP_MESSAGE(response));

    /* Update last response */
    TRANSAC_IST_SET_LAST_RESPONSE(self, response);

    return ret;
}

/*	Accepted --> (timer X) --> Accepted
* Doubango specific
*/
static int tsip_transac_ist_Accepted_2_Accepted_timerX(va_list *app)
{
    tsip_transac_ist_t *self = va_arg(*app, tsip_transac_ist_t *);
    if(self->lastResponse) {
        int ret;
        ret = tsip_transac_send(TSIP_TRANSAC(self), TSIP_TRANSAC(self)->branch, self->lastResponse);
        self->timerX.timeout <<= 1;
        TRANSAC_IST_TIMER_SCHEDULE(X);
    }
    return 0;
}

/*	Accepted --> (Recv ACK) --> Accepted
* Doubango specific
*/
int tsip_transac_ist_Accepted_2_Accepted_iACK(va_list *app)
{
    tsip_transac_ist_t *self = va_arg(*app, tsip_transac_ist_t *);
    const tsip_request_t *request = va_arg(*app, const tsip_request_t *);
    self->acked = tsk_true;
    TRANSAC_TIMER_CANCEL(X);
    return tsip_transac_deliver(TSIP_TRANSAC(self), tsip_dialog_i_msg, request);
}

/*	Accepted --> (timerL) --> Terminated
*/
static int tsip_transac_ist_Accepted_2_Terminated_timerL(va_list *app)
{
    tsip_transac_ist_t *self = va_arg(*app, tsip_transac_ist_t *);
    //const tsip_message_t *message = va_arg(*app, const tsip_message_t *);

    /*	draft-sparks-sip-invfix-03 - 8.7. Page 137
    	If Timer L fires while the INVITE server transaction is in the "Accepted" state, the transaction
    	MUST transition to the "Terminated" state. Once the transaction is in the "Terminated" state, it MUST be
    	destroyed immediately.
    */
    if(!self->acked) {
        TSK_DEBUG_ERROR("ACK not received");
        return tsip_transac_deliver(TSIP_TRANSAC(self), tsip_dialog_transport_error, tsk_null);
    }
    return 0;
}

/*	Confirmed --> (timerI) --> Terminated
*/
static int tsip_transac_ist_Confirmed_2_Terminated_timerI(va_list *app)
{
    /*	RFC 3261 - 17.2.1 INVITE Server Transaction
    	Once timer I fires, the server MUST transition to the
    	"Terminated" state.

    	Once the transaction is in the "Terminated" state, it MUST be
    	destroyed immediately.  As with client transactions, this is needed
    	to ensure reliability of the 2xx responses to INVITE.
    */
    return 0;
}

/* Any -> (Transport Error) -> Terminated
*/
static int tsip_transac_ist_Any_2_Terminated_X_transportError(va_list *app)
{
    tsip_transac_ist_t *self = va_arg(*app, tsip_transac_ist_t *);
    //const tsip_message_t *message = va_arg(*app, const tsip_message_t *);

    /* Timers will be canceled by "tsip_transac_nict_OnTerminated" */

    return tsip_transac_deliver(TSIP_TRANSAC(self), tsip_dialog_transport_error, tsk_null);
}

/* Any -> (Error) -> Terminated
*/
static int tsip_transac_ist_Any_2_Terminated_X_Error(va_list *app)
{
    tsip_transac_ist_t *self = va_arg(*app, tsip_transac_ist_t *);
    //const tsip_message_t *message = va_arg(*app, const tsip_message_t *);

    /* Timers will be canceled by "tsip_transac_nict_OnTerminated" */

    return tsip_transac_deliver(TSIP_TRANSAC(self), tsip_dialog_error, tsk_null);
}

/* Any -> (cancel) -> Terminated
*/
static int tsip_transac_ist_Any_2_Terminated_X_cancel(va_list *app)
{
    /* doubango-specific */
    return 0;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//				== STATE MACHINE END ==
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++



/*== Callback function called when the state machine enter in the "terminated" state.
*/
static int tsip_transac_ist_OnTerminated(tsip_transac_ist_t *self)
{
    TSK_DEBUG_INFO("=== IST terminated ===");

    /* Remove (and destroy) the transaction from the layer. */
    return tsip_transac_remove(TSIP_TRANSAC(self));
}













//========================================================
//	IST object definition
//
static tsk_object_t* tsip_transac_ist_ctor(tsk_object_t * self, va_list * app)
{
    tsip_transac_ist_t *transac = self;
    if(transac) {
    }
    return self;
}

static tsk_object_t* tsip_transac_ist_dtor(tsk_object_t * _self)
{
    tsip_transac_ist_t *self = _self;
    if(self) {
        /* Cancel timers */
        TRANSAC_TIMER_CANCEL(H);
        TRANSAC_TIMER_CANCEL(I);
        if(!TSIP_TRANSAC(self)->reliable) {
            TRANSAC_TIMER_CANCEL(G);
        }
        TRANSAC_TIMER_CANCEL(L);
        TRANSAC_TIMER_CANCEL(X);

        TSIP_TRANSAC(self)->running = tsk_false;
        TSK_OBJECT_SAFE_FREE(self->lastResponse);

        /* DeInitialize base class */
        tsip_transac_deinit(TSIP_TRANSAC(self));

        TSK_DEBUG_INFO("*** IST destroyed ***");
    }
    return _self;
}

static int tsip_transac_ist_cmp(const tsk_object_t *t1, const tsk_object_t *t2)
{
    return tsip_transac_cmp(t1, t2);
}

static const tsk_object_def_t tsip_transac_ist_def_s = {
    sizeof(tsip_transac_ist_t),
    tsip_transac_ist_ctor,
    tsip_transac_ist_dtor,
    tsip_transac_ist_cmp,
};
const tsk_object_def_t *tsip_transac_ist_def_t = &tsip_transac_ist_def_s;
