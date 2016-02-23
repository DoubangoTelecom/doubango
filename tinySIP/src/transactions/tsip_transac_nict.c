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

                                   |Request from TU
                                   |send request
               Timer E             V
               send request  +-----------+
                   +---------|           |-------------------+
                   |         |  Trying   |  Timer F          |
                   +-------->|           |  or Transport Err.|
                             +-----------+  inform TU        |
                200-699         |  |                         |
                resp. to TU     |  |1xx                      |
                +---------------+  |resp. to TU              |
                |                  |                         |
                |   Timer E        V       Timer F           |
                |   send req +-----------+ or Transport Err. |
                |  +---------|           | inform TU         |
                |  |         |Proceeding |------------------>|
                |  +-------->|           |-----+             |
                |            +-----------+     |1xx          |
                |              |      ^        |resp to TU   |
                | 200-699      |      +--------+             |
                | resp. to TU  |                             |
                |              |                             |
                |              V                             |
                |            +-----------+                   |
                |            |           |                   |
                |            | Completed |                   |
                |            |           |                   |
                |            +-----------+                   |
                |              ^   |                         |
                |              |   | Timer K                 |
                +--------------+   | -                       |
                                   |                         |
                                   V                         |
             NOTE:           +-----------+                   |
                             |           |                   |
         transitions         | Terminated|<------------------+
         labeled with        |           |
         the event           +-----------+
         over the action

=============================================================================*/

/**@file tsip_transac_nict.c
 * @brief SIP Non-INVITE Client Transaction as per RFC 3261 subcaluse 17.1.2.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *
 */
#include "tinysip/transactions/tsip_transac_nict.h"

#include "tsk_debug.h"

#define DEBUG_STATE_MACHINE						1

#define TRANSAC_NICT_TIMER_SCHEDULE(TX)			TRANSAC_TIMER_SCHEDULE(nict, TX)

/* ======================== internal functions ======================== */
static int tsip_transac_nict_init(tsip_transac_nict_t *self);
static int tsip_transac_nict_OnTerminated(tsip_transac_nict_t *self);

/* ======================== transitions ======================== */
static int tsip_transac_nict_Started_2_Trying_X_send(va_list *app);
static int tsip_transac_nict_Trying_2_Trying_X_timerE(va_list *app);
static int tsip_transac_nict_Trying_2_Terminated_X_timerF(va_list *app);
static int tsip_transac_nict_Trying_2_Terminated_X_transportError(va_list *app);
static int tsip_transac_nict_Trying_2_Proceedding_X_1xx(va_list *app);
static int tsip_transac_nict_Trying_2_Completed_X_200_to_699(va_list *app);
static int tsip_transac_nict_Proceeding_2_Proceeding_X_timerE(va_list *app);
static int tsip_transac_nict_Proceeding_2_Terminated_X_timerF(va_list *app);
static int tsip_transac_nict_Proceeding_2_Terminated_X_transportError(va_list *app);
static int tsip_transac_nict_Proceeding_2_Proceeding_X_1xx(va_list *app);
static int tsip_transac_nict_Proceeding_2_Completed_X_200_to_699(va_list *app);
static int tsip_transac_nict_Completed_2_Terminated_X_timerK(va_list *app);
static int tsip_transac_nict_Any_2_Terminated_X_transportError(va_list *app);
static int tsip_transac_nict_Any_2_Terminated_X_Error(va_list *app);
static int tsip_transac_nict_Any_2_Terminated_X_cancel(va_list *app); /* doubango-specific */

/* ======================== conds ======================== */

/* ======================== actions ======================== */
typedef enum _fsm_action_e {
    _fsm_action_cancel = tsip_atype_cancel,

    _fsm_action_send = 0xFF,
    _fsm_action_timerE,
    _fsm_action_timerF,
    _fsm_action_timerK,
    _fsm_action_1xx,
    _fsm_action_200_to_699,
    _fsm_action_transporterror,
    _fsm_action_error,
}
_fsm_action_t;

/* ======================== states ======================== */
typedef enum _fsm_state_e {
    _fsm_state_Started,
    _fsm_state_Trying,
    _fsm_state_Proceeding,
    _fsm_state_Completed,
    _fsm_state_Terminated
}
_fsm_state_t;


/**
 * Callback function called by the transport layer to alert the transaction for incoming messages
 *			or errors (e.g. transport error).
 *
 * @param [in,out]	self	A pointer to the NIC transaction.
 * @param	type		The event type.
 * @param [in,out]	msg	The incoming message.
 *
 * @return	Zero if succeed and no-zero error code otherwise.
**/
int tsip_transac_nict_event_callback(const tsip_transac_nict_t *self, tsip_transac_event_type_t type, const tsip_message_t *msg)
{
    int ret = 0;

    switch(type) {
    case tsip_transac_incoming_msg: {
        if(msg && TSIP_MESSAGE_IS_RESPONSE(msg)) {
            if(TSIP_RESPONSE_IS_1XX(msg)) {
                ret = tsip_transac_fsm_act(TSIP_TRANSAC(self), _fsm_action_1xx, msg);
            }
            else if(TSIP_RESPONSE_IS_23456(msg)) {
                ret = tsip_transac_fsm_act(TSIP_TRANSAC(self), _fsm_action_200_to_699, msg);
            }
            else {
                TSK_DEBUG_WARN("Not supported status code: %d", TSIP_RESPONSE_CODE(msg));
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

    default:
        break;
    }

    return ret;
}

int tsip_transac_nict_timer_callback(const tsip_transac_nict_t* self, tsk_timer_id_t timer_id)
{
    int ret = -1;

    if(self) {
        if(timer_id == self->timerE.id) {
            ret = tsip_transac_fsm_act(TSIP_TRANSAC(self), _fsm_action_timerE, tsk_null);
        }
        else if(timer_id == self->timerF.id) {
            ret = tsip_transac_fsm_act(TSIP_TRANSAC(self), _fsm_action_timerF, tsk_null);
        }
        else if(timer_id == self->timerK.id) {
            ret = tsip_transac_fsm_act(TSIP_TRANSAC(self), _fsm_action_timerK, tsk_null);
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
int tsip_transac_nict_init(tsip_transac_nict_t *self)
{
    /* Initialize the state machine. */
    tsk_fsm_set(TSIP_TRANSAC_GET_FSM(self),

                /*=======================
                * === Started ===
                */
                // Started -> (Send) -> Trying
                TSK_FSM_ADD_ALWAYS(_fsm_state_Started, _fsm_action_send, _fsm_state_Trying, tsip_transac_nict_Started_2_Trying_X_send, "tsip_transac_nict_Started_2_Trying_X_send"),
                // Started -> (Any) -> Started
                TSK_FSM_ADD_ALWAYS_NOTHING(_fsm_state_Started, "tsip_transac_nict_Started_2_Started_X_any"),

                /*=======================
                * === Trying ===
                */
                // Trying -> (timerE) -> Trying
                TSK_FSM_ADD_ALWAYS(_fsm_state_Trying, _fsm_action_timerE, _fsm_state_Trying, tsip_transac_nict_Trying_2_Trying_X_timerE, "tsip_transac_nict_Trying_2_Trying_X_timerE"),
                // Trying -> (timerF) -> Terminated
                TSK_FSM_ADD_ALWAYS(_fsm_state_Trying, _fsm_action_timerF, _fsm_state_Terminated, tsip_transac_nict_Trying_2_Terminated_X_timerF, "tsip_transac_nict_Trying_2_Terminated_X_timerF"),
                // Trying -> (transport error) -> Terminated
                TSK_FSM_ADD_ALWAYS(_fsm_state_Trying, _fsm_action_transporterror, _fsm_state_Terminated, tsip_transac_nict_Trying_2_Terminated_X_transportError, "tsip_transac_nict_Trying_2_Terminated_X_transportError"),
                // Trying  -> (1xx) -> Proceeding
                TSK_FSM_ADD_ALWAYS(_fsm_state_Trying, _fsm_action_1xx, _fsm_state_Proceeding, tsip_transac_nict_Trying_2_Proceedding_X_1xx, "tsip_transac_nict_Trying_2_Proceedding_X_1xx"),
                // Trying  -> (200 to 699) -> Completed
                TSK_FSM_ADD_ALWAYS(_fsm_state_Trying, _fsm_action_200_to_699, _fsm_state_Completed, tsip_transac_nict_Trying_2_Completed_X_200_to_699, "tsip_transac_nict_Trying_2_Completed_X_200_to_699"),

                /*=======================
                * === Proceeding ===
                */
                // Proceeding -> (timerE) -> Proceeding
                TSK_FSM_ADD_ALWAYS(_fsm_state_Proceeding, _fsm_action_timerE, _fsm_state_Proceeding, tsip_transac_nict_Proceeding_2_Proceeding_X_timerE, "tsip_transac_nict_Proceeding_2_Proceeding_X_timerE"),
                // Proceeding -> (timerF) -> Terminated
                TSK_FSM_ADD_ALWAYS(_fsm_state_Proceeding, _fsm_action_timerF, _fsm_state_Terminated, tsip_transac_nict_Proceeding_2_Terminated_X_timerF, "tsip_transac_nict_Proceeding_2_Terminated_X_timerF"),
                // Proceeding -> (transport error) -> Terminated
                TSK_FSM_ADD_ALWAYS(_fsm_state_Proceeding, _fsm_action_transporterror, _fsm_state_Terminated, tsip_transac_nict_Proceeding_2_Terminated_X_transportError, "tsip_transac_nict_Proceeding_2_Terminated_X_transportError"),
                // Proceeding -> (1xx) -> Proceeding
                TSK_FSM_ADD_ALWAYS(_fsm_state_Proceeding, _fsm_action_1xx, _fsm_state_Proceeding, tsip_transac_nict_Proceeding_2_Proceeding_X_1xx, "tsip_transac_nict_Proceeding_2_Proceeding_X_1xx"),
                // Proceeding -> (200 to 699) -> Completed
                TSK_FSM_ADD_ALWAYS(_fsm_state_Proceeding, _fsm_action_200_to_699, _fsm_state_Completed, tsip_transac_nict_Proceeding_2_Completed_X_200_to_699, "tsip_transac_nict_Proceeding_2_Completed_X_200_to_699"),

                /*=======================
                * === Completed ===
                */
                // Completed -> (timer K) -> Terminated
                TSK_FSM_ADD_ALWAYS(_fsm_state_Completed, _fsm_action_timerK, _fsm_state_Terminated, tsip_transac_nict_Completed_2_Terminated_X_timerK, "tsip_transac_nict_Completed_2_Terminated_X_timerK"),

                /*=======================
                * === Any ===
                */
                // Any -> (transport error) -> Terminated
                TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, _fsm_action_transporterror, _fsm_state_Terminated, tsip_transac_nict_Any_2_Terminated_X_transportError, "tsip_transac_nict_Any_2_Terminated_X_transportError"),
                // Any -> (error) -> Terminated
                TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, _fsm_action_error, _fsm_state_Terminated, tsip_transac_nict_Any_2_Terminated_X_Error, "tsip_transac_nict_Any_2_Terminated_X_Error"),
                // Any -> (cancel) -> Terminated
                TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, _fsm_action_cancel, _fsm_state_Terminated, tsip_transac_nict_Any_2_Terminated_X_cancel, "tsip_transac_nict_Any_2_Terminated_X_cancel"),

                TSK_FSM_ADD_NULL());

    /* Set callback function to call when new messages arrive or errors happen in
    the transport layer.
    */
    TSIP_TRANSAC(self)->callback = TSIP_TRANSAC_EVENT_CALLBACK_F(tsip_transac_nict_event_callback);

    /* Timers */
    self->timerE.id = TSK_INVALID_TIMER_ID;
    self->timerF.id = TSK_INVALID_TIMER_ID;
    self->timerK.id = TSK_INVALID_TIMER_ID;
    self->timerE.timeout = TSIP_TIMER_GET(E);
    self->timerF.timeout = TSIP_TIMER_GET(F);

    return 0;
}

tsip_transac_nict_t* tsip_transac_nict_create(int32_t cseq_value, const char* cseq_method, const char* callid, tsip_transac_dst_t* dst)
{
    tsip_transac_nict_t* transac = tsk_object_new(tsip_transac_nict_def_t);
    if(transac) {
        // initialize base class
        tsip_transac_init(TSIP_TRANSAC(transac), tsip_transac_type_nict, cseq_value, cseq_method, callid, dst, _fsm_state_Started, _fsm_state_Terminated);

        // init FSM
        TSIP_TRANSAC_GET_FSM(transac)->debug = DEBUG_STATE_MACHINE;
        tsk_fsm_set_callback_terminated(TSIP_TRANSAC_GET_FSM(transac), TSK_FSM_ONTERMINATED_F(tsip_transac_nict_OnTerminated), (const void*)transac);

        // initialize NICT object
        tsip_transac_nict_init(transac);
    }
    return transac;
}

/**
 * Starts the client transaction.
 *
 * @param [in,out]	self	The client transaction to start.
 * @param [in,out]	request	The SIP/IMS request to send.
 *
 * @return	Zero if succeed and non-zero error code otherwise.
**/
int tsip_transac_nict_start(tsip_transac_nict_t *self, const tsip_request_t* request)
{
    int ret = -1;
    if(self && request && !TSIP_TRANSAC(self)->running) {
        /* Add branch to the new client transaction
        * - CANCEL will have the same Via and Contact headers as the request it cancel
        * - Transac will use request branch if exit (e.g. when request received over websocket)
        */
        if((request->firstVia && !tsk_strnullORempty(request->firstVia->branch))) {
            tsk_strupdate(&TSIP_TRANSAC(self)->branch, (request->firstVia ? request->firstVia->branch : "doubango"));
        }
        else if((TSIP_TRANSAC(self)->branch = tsk_strdup(TSIP_TRANSAC_MAGIC_COOKIE))) {
            tsk_istr_t branch;
            tsk_strrandom(&branch);
            tsk_strcat_2(&(TSIP_TRANSAC(self)->branch), "-%s", branch);
        }

        TSIP_TRANSAC(self)->running = tsk_true;
        self->request = tsk_object_ref((void*)request);

        ret = tsip_transac_fsm_act(TSIP_TRANSAC(self), _fsm_action_send, tsk_null);
    }
    return ret;
}










//--------------------------------------------------------
//				== STATE MACHINE BEGIN ==
//--------------------------------------------------------
/* Started -> (send) -> Trying
*/
int tsip_transac_nict_Started_2_Trying_X_send(va_list *app)
{
    tsip_transac_nict_t *self = va_arg(*app, tsip_transac_nict_t *);
    //const tsip_message_t *message = va_arg(*app, const tsip_message_t *);

    //== Send the request
    tsip_transac_send(TSIP_TRANSAC(self), TSIP_TRANSAC(self)->branch, TSIP_MESSAGE(self->request));

    // Now that the first request is sent using the best transport mean we know if it's reliable or not
    if(TNET_SOCKET_TYPE_IS_VALID(self->request->dst_net_type)) {
        TSIP_TRANSAC(self)->reliable = TNET_SOCKET_TYPE_IS_STREAM(self->request->dst_net_type);
        self->timerK.timeout = TSIP_TRANSAC(self)->reliable ? 0 : TSIP_TIMER_GET(K); /* RFC 3261 - 17.1.2.2*/
    }

    /*	RFC 3261 - 17.1.2.2
    	The "Trying" state is entered when the TU initiates a new client
    	transaction with a request.  When entering this state, the client
    	transaction SHOULD set timer F to fire in 64*T1 seconds.
    */
    TRANSAC_NICT_TIMER_SCHEDULE(F);

    /*	RFC 3261 - 17.1.2.2
    	If an  unreliable transport is in use, the client transaction MUST set timer
    	E to fire in T1 seconds.
    */
    if(!TSIP_TRANSAC(self)->reliable) {
        TRANSAC_NICT_TIMER_SCHEDULE(E);
    }

    return 0;
}

/* Trying -> (Timer E) -> Trying
*/
int tsip_transac_nict_Trying_2_Trying_X_timerE(va_list *app)
{
    tsip_transac_nict_t *self = va_arg(*app, tsip_transac_nict_t *);
    /*const tsip_message_t *message = va_arg(*app, const tsip_message_t *);*/

    //== Send the request
    tsip_transac_send(TSIP_TRANSAC(self), TSIP_TRANSAC(self)->branch, self->request);

    /*	RFC 3261 - 17.1.2.2
    	If timer E fires while still in this (Trying) state, the timer is reset, but this time with a value of MIN(2*T1, T2).
    	When the timer fires again, it is reset to a MIN(4*T1, T2).  This process continues so that retransmissions occur with an exponentially
        increasing interval that caps at T2.  The default value of T2 is 4s, and it represents the amount of time a non-INVITE server transaction
        will take to respond to a request, if it does not respond immediately.  For the default values of T1 and T2, this results in
        intervals of 500 ms, 1 s, 2 s, 4 s, 4 s, 4 s, etc.
    */
    self->timerE.timeout = TSK_MIN((self->timerE.timeout<<1), TSIP_TIMER_GET(T2));
    TRANSAC_NICT_TIMER_SCHEDULE(E);

    return 0;
}

/* Trying -> (Timer F) -> Terminated
*/
int tsip_transac_nict_Trying_2_Terminated_X_timerF(va_list *app)
{
    tsip_transac_nict_t *self = va_arg(*app, tsip_transac_nict_t *);
    /*const tsip_message_t *message = va_arg(*app, const tsip_message_t *);*/

    /*	RFC 3261 - 17.1.2.2
    	If Timer F fires while the client transaction is still in the
    	"Trying" state, the client transaction SHOULD inform the TU about the
    	timeout, and then it SHOULD enter the "Terminated" state.
    */

    /* Timers will be canceled by "tsip_transac_nict_OnTerminated" */

    tsip_transac_deliver(TSIP_TRANSAC(self), tsip_dialog_timedout, self->request);

    return 0;
}

/* Trying -> (Transport Error) -> Terminated
*/
int tsip_transac_nict_Trying_2_Terminated_X_transportError(va_list *app)
{
    tsip_transac_nict_t *self = va_arg(*app, tsip_transac_nict_t *);
    /*const tsip_message_t *message = va_arg(*app, const tsip_message_t *);*/

    /* Timers will be canceled by "tsip_transac_nict_OnTerminated" */

    tsip_transac_deliver(TSIP_TRANSAC(self), tsip_dialog_transport_error, self->request);

    return 0;
}

/* Trying -> (1xx) -> Proceeding
*/
int tsip_transac_nict_Trying_2_Proceedding_X_1xx(va_list *app)
{
    tsip_transac_nict_t *self = va_arg(*app, tsip_transac_nict_t *);
    const tsip_message_t *message = va_arg(*app, const tsip_message_t *);

    /*	RFC 3261 - 17.1.2.2
    	If a provisional response is received while in the "Trying" state, the
    	response MUST be passed to the TU, and then the client transaction
    	SHOULD move to the "Proceeding" state.
    */

    /* Cancel timers */
    if(!TSIP_TRANSAC(self)->reliable) {
        TRANSAC_TIMER_CANCEL(E);
    }
    TRANSAC_TIMER_CANCEL(F); /* Now it's up to the UAS to update the FSM. */

    /* Pass the provisional response to the dialog. */
    tsip_transac_deliver(TSIP_TRANSAC(self), tsip_dialog_i_msg, message);

    return 0;
}

/* Trying -> (200-699) -> Completed
*/
int tsip_transac_nict_Trying_2_Completed_X_200_to_699(va_list *app)
{
    tsip_transac_nict_t *self = va_arg(*app, tsip_transac_nict_t *);
    const tsip_message_t *message = va_arg(*app, const tsip_message_t *);

    /*	RFC 3261 - 17.1.2.2
    	If a final response (status codes 200-699) is received while in the "Trying" state, the response
    	MUST be passed to the TU, and the client transaction MUST transition
    	to the "Completed" state.

    	If Timer K fires while in this state (Completed), the client transaction MUST transition to the "Terminated" state.
    */

    if(!TSIP_TRANSAC(self)->reliable) {
        TRANSAC_TIMER_CANCEL(E);
    }
    TRANSAC_TIMER_CANCEL(F);

    tsip_transac_deliver(TSIP_TRANSAC(self), tsip_dialog_i_msg, message);

    /* SCHEDULE timer K */
    TRANSAC_NICT_TIMER_SCHEDULE(K);

    return 0;
}

/* Proceeding -> (TimerE) -> Proceeding
*/
int tsip_transac_nict_Proceeding_2_Proceeding_X_timerE(va_list *app)
{
    tsip_transac_nict_t *self = va_arg(*app, tsip_transac_nict_t *);
    //const tsip_message_t *message = va_arg(*app, const tsip_message_t *);

    //== Send the request
    tsip_transac_send(TSIP_TRANSAC(self), TSIP_TRANSAC(self)->branch, self->request);

    /*	RFC 3261 - 17.1.2.2
    	If Timer E fires while in the "Proceeding" state, the request MUST be
    	passed to the transport layer for retransmission, and Timer E MUST be
    	reset with a value of T2 seconds.
    */
    self->timerE.timeout = TSK_MIN(self->timerE.timeout*2, TSIP_TIMER_GET(T2));
    TRANSAC_NICT_TIMER_SCHEDULE(E);

    return 0;
}

/* Proceeding -> (Timer F) -> Proceeding
*/
int tsip_transac_nict_Proceeding_2_Terminated_X_timerF(va_list *app)
{
    tsip_transac_nict_t *self = va_arg(*app, tsip_transac_nict_t *);
    //const tsip_message_t *message = va_arg(*app, const tsip_message_t *);

    /*	RFC 3261 - 17.1.2.2
    	If timer F fires while in the "Proceeding" state, the TU MUST be informed of a timeout, and the
    	client transaction MUST transition to the terminated state.
    */

    /* Timers will be canceled by "tsip_transac_nict_OnTerminated" */

    tsip_transac_deliver(TSIP_TRANSAC(self), tsip_dialog_transport_error, tsk_null);

    return 0;
}

/* Proceeding -> (Transport error) -> Terminated
*/
int tsip_transac_nict_Proceeding_2_Terminated_X_transportError(va_list *app)
{
    tsip_transac_nict_t *self = va_arg(*app, tsip_transac_nict_t *);
    /*const tsip_message_t *message = va_arg(*app, const tsip_message_t *);*/

    /* Timers will be canceles by On */
    tsip_transac_deliver(TSIP_TRANSAC(self), tsip_dialog_transport_error, 0);

    return 0;
}

/* Proceeding -> (1xx) -> Proceeding
*/
int tsip_transac_nict_Proceeding_2_Proceeding_X_1xx(va_list *app)
{
    tsip_transac_nict_t *self = va_arg(*app, tsip_transac_nict_t *);
    const tsip_message_t *message = va_arg(*app, const tsip_message_t *);

    if(!TSIP_TRANSAC(self)->reliable) {
        TRANSAC_TIMER_CANCEL(E);
    }
    tsip_transac_deliver(TSIP_TRANSAC(self), tsip_dialog_i_msg, message);

    return 0;
}

/* Proceeding -> (200-699) -> Completed
*/
int tsip_transac_nict_Proceeding_2_Completed_X_200_to_699(va_list *app)
{
    tsip_transac_nict_t *self = va_arg(*app, tsip_transac_nict_t *);
    const tsip_message_t *message = va_arg(*app, const tsip_message_t *);

    /*	RFC 3261 - 17.1.2.2
    	If a final response (status codes 200-699) is received while in the
    	"Proceeding" state, the response MUST be passed to the TU, and the
    	client transaction MUST transition to the "Completed" state.
    */

    /*	RFC 3261 - 17.1.2.2
    	Once the client transaction enters the "Completed" state, it MUST set
    	Timer K to fire in T4 seconds for unreliable transports, and zero
    	seconds for reliable transports.  The "Completed" state exists to
    	buffer any additional response retransmissions that may be received
    	(which is why the client transaction remains there only for

    	unreliable transports).  T4 represents the amount of time the network
    	will take to clear messages between client and server transactions.
    	The default value of T4 is 5s.
    */

    if(!TSIP_TRANSAC(self)->reliable) {
        TRANSAC_TIMER_CANCEL(E);
    }

    tsip_transac_deliver(TSIP_TRANSAC(self), tsip_dialog_i_msg, message);

    /* SCHEDULE timer K */
    TRANSAC_NICT_TIMER_SCHEDULE(K);

    return 0;
}

/* Completed -> (Timer K) -> Terminated
*/
int tsip_transac_nict_Completed_2_Terminated_X_timerK(va_list *app)
{
    //tsip_transac_nict_t *self = va_arg(*app, tsip_transac_nict_t *);
    //const tsip_message_t *message = va_arg(*app, const tsip_message_t *);

    /*	RFC 3261 - 17.1.2.2
    	If Timer K fires while in this state (Completed), the client transaction
    	MUST transition to the "Terminated" state.
    */

    /*	RFC 3261 - 17.1.2.2
    	ONCE THE TRANSACTION IS IN THE TERMINATED STATE, IT MUST BE DESTROYED IMMEDIATELY.
    */

    /* Timers will be canceled by "tsip_transac_nict_OnTerminated" */

    //tsip_transac_deliver(TSIP_TRANSAC(self), tsip_dialog_transac_ok, 0);

    return 0;
}

/* Any -> (Transport Error) -> Terminated
*/
int tsip_transac_nict_Any_2_Terminated_X_transportError(va_list *app)
{
    tsip_transac_nict_t *self = va_arg(*app, tsip_transac_nict_t *);
    //const tsip_message_t *message = va_arg(*app, const tsip_message_t *);

    /* Timers will be canceled by "tsip_transac_nict_OnTerminated" */

    return tsip_transac_deliver(TSIP_TRANSAC(self), tsip_dialog_transport_error, tsk_null);
}

/* Any -> (Error) -> Terminated
*/
int tsip_transac_nict_Any_2_Terminated_X_Error(va_list *app)
{
    tsip_transac_nict_t *self = va_arg(*app, tsip_transac_nict_t *);
    //const tsip_message_t *message = va_arg(*app, const tsip_message_t *);

    /* Timers will be canceled by "tsip_transac_nict_OnTerminated" */

    return tsip_transac_deliver(TSIP_TRANSAC(self), tsip_dialog_error, tsk_null);
}

/* Any -> (cancel) -> Terminated
*/
int tsip_transac_nict_Any_2_Terminated_X_cancel(va_list *app)
{
    /* doubango-specific */
    return 0;
}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//				== STATE MACHINE END ==
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++




/*== TERMINATED
*/
int tsip_transac_nict_OnTerminated(tsip_transac_nict_t *self)
{
    TSK_DEBUG_INFO("=== NICT terminated ===");

    /* Remove (and destroy) the transaction from the layer. */
    return tsip_transac_remove(TSIP_TRANSAC(self));
}
















//========================================================
//	NICT object definition
//
static tsk_object_t* tsip_transac_nict_ctor(tsk_object_t * self, va_list * app)
{
    tsip_transac_nict_t *transac = self;
    if(transac) {
    }
    return self;
}

static tsk_object_t* tsip_transac_nict_dtor(tsk_object_t * _self)
{
    tsip_transac_nict_t *self = _self;
    if(self) {
        /* Cancel timers */
        if(!TSIP_TRANSAC(self)->reliable) {
            TRANSAC_TIMER_CANCEL(E);
        }
        TRANSAC_TIMER_CANCEL(F);
        TRANSAC_TIMER_CANCEL(K);

        TSIP_TRANSAC(self)->running = tsk_false;
        TSK_OBJECT_SAFE_FREE(self->request);

        /* DeInitialize base class */
        tsip_transac_deinit(TSIP_TRANSAC(self));

        TSK_DEBUG_INFO("*** NICT destroyed ***");
    }
    return _self;
}

static int tsip_transac_nict_cmp(const tsk_object_t *t1, const tsk_object_t *t2)
{
    return tsip_transac_cmp(t1, t2);
}

static const tsk_object_def_t tsip_transac_nict_def_s = {
    sizeof(tsip_transac_nict_t),
    tsip_transac_nict_ctor,
    tsip_transac_nict_dtor,
    tsip_transac_nict_cmp,
};
const tsk_object_def_t *tsip_transac_nict_def_t = &tsip_transac_nict_def_s;
