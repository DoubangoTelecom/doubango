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

/**@file tsip_dialog_message.c
 * @brief SIP dialog message (Client side).
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/dialogs/tsip_dialog_message.h"
#include "tinysip/parsers/tsip_parser_uri.h"

#include "tinysip/api/tsip_api_message.h"

#include "tinysip/headers/tsip_header_Dummy.h"
#include "tinysip/headers/tsip_header_Min_Expires.h"

#include "tinysip/transactions/tsip_transac_layer.h"

#include "tsk_memory.h"
#include "tsk_debug.h"
#include "tsk_time.h"

#define DEBUG_STATE_MACHINE											1
#define TSIP_DIALOG_MESSAGE_SIGNAL(self, type, code, phrase, message)	\
	tsip_message_event_signal(type, TSIP_DIALOG(self)->ss, code, phrase, message)

/* ======================== internal functions ======================== */
static int send_MESSAGE(tsip_dialog_message_t *self);
static int tsip_dialog_message_OnTerminated(tsip_dialog_message_t *self);

/* ======================== transitions ======================== */
static int tsip_dialog_message_Started_2_Sending_X_sendMESSAGE(va_list *app);
static int tsip_dialog_message_Started_2_Receiving_X_recvMESSAGE(va_list *app);
static int tsip_dialog_message_Sending_2_Sending_X_1xx(va_list *app);
static int tsip_dialog_message_Sending_2_Terminated_X_2xx(va_list *app);
static int tsip_dialog_message_Sending_2_Sending_X_401_407_421_494(va_list *app);
static int tsip_dialog_message_Sending_2_Terminated_X_300_to_699(va_list *app);
static int tsip_dialog_message_Sending_2_Terminated_X_cancel(va_list *app);
static int tsip_dialog_message_Receiving_2_Terminated_X_accept(va_list *app);
static int tsip_dialog_message_Receiving_2_Terminated_X_reject(va_list *app);
static int tsip_dialog_message_Any_2_Terminated_X_transportError(va_list *app);
static int tsip_dialog_message_Any_2_Terminated_X_Error(va_list *app);

/* ======================== conds ======================== */

/* ======================== actions ======================== */
typedef enum _fsm_action_e {
    _fsm_action_sendMESSAGE = tsip_atype_message_send,
    _fsm_action_accept = tsip_atype_accept,
    _fsm_action_reject = tsip_atype_reject,
    _fsm_action_cancel = tsip_atype_cancel,
    _fsm_action_shutdown = tsip_atype_shutdown,
    _fsm_action_transporterror = tsip_atype_transport_error,

    _fsm_action_receiveMESSAGE = 0xFF,
    _fsm_action_1xx,
    _fsm_action_2xx,
    _fsm_action_401_407_421_494,
    _fsm_action_300_to_699,
    _fsm_action_error,
}
_fsm_action_t;

/* ======================== states ======================== */
typedef enum _fsm_state_e {
    _fsm_state_Started,
    _fsm_state_Sending,
    _fsm_state_Receiving,
    _fsm_state_Terminated
}
_fsm_state_t;


static int tsip_dialog_message_event_callback(const tsip_dialog_message_t *self, tsip_dialog_event_type_t type, const tsip_message_t *msg)
{
    int ret = -1;

    switch(type) {
    case tsip_dialog_i_msg: {
        if(msg) {
            if(TSIP_MESSAGE_IS_RESPONSE(msg)) {
                const tsip_action_t* action = tsip_dialog_keep_action(TSIP_DIALOG(self), msg) ? TSIP_DIALOG(self)->curr_action : tsk_null;
                if(TSIP_RESPONSE_IS_1XX(msg)) {
                    ret = tsip_dialog_fsm_act(TSIP_DIALOG(self), _fsm_action_1xx, msg, action);
                }
                else if(TSIP_RESPONSE_IS_2XX(msg)) {
                    ret = tsip_dialog_fsm_act(TSIP_DIALOG(self), _fsm_action_2xx, msg, action);
                }
                else if(TSIP_RESPONSE_CODE(msg) == 401 || TSIP_RESPONSE_CODE(msg) == 407 || TSIP_RESPONSE_CODE(msg) == 421 || TSIP_RESPONSE_CODE(msg) == 494) {
                    ret = tsip_dialog_fsm_act(TSIP_DIALOG(self), _fsm_action_401_407_421_494, msg, action);
                }
                else if(TSIP_RESPONSE_IS_3456(msg)) {
                    ret = tsip_dialog_fsm_act(TSIP_DIALOG(self), _fsm_action_300_to_699, msg, action);
                }
                else { /* Should never happen */
                    ret = tsip_dialog_fsm_act(TSIP_DIALOG(self), _fsm_action_error, msg, action);
                }
            }
            else if (TSIP_REQUEST_IS_MESSAGE(msg)) { /* have been checked by dialog layer...but */
                // REQUEST ==> Incoming MESSAGE
                ret = tsip_dialog_fsm_act(TSIP_DIALOG(self), _fsm_action_receiveMESSAGE, msg, tsk_null);
            }
        }
        break;
    }

    case tsip_dialog_canceled: {
        ret = tsip_dialog_fsm_act(TSIP_DIALOG(self), _fsm_action_cancel, msg, tsk_null);
        break;
    }

    case tsip_dialog_terminated:
    case tsip_dialog_timedout:
    case tsip_dialog_error:
    case tsip_dialog_transport_error: {
        ret = tsip_dialog_fsm_act(TSIP_DIALOG(self), _fsm_action_transporterror, msg, tsk_null);
        break;
    }

    default:
        break;
    }

    return ret;
}

tsip_dialog_message_t* tsip_dialog_message_create(const tsip_ssession_handle_t* ss)
{
    return tsk_object_new(tsip_dialog_message_def_t, ss);
}

int tsip_dialog_message_init(tsip_dialog_message_t *self)
{
    //const tsk_param_t* param;

    /* Initialize the state machine. */
    tsk_fsm_set(TSIP_DIALOG_GET_FSM(self),

                /*=======================
                * === Started ===
                */
                // Started -> (send) -> Sending
                TSK_FSM_ADD_ALWAYS(_fsm_state_Started, _fsm_action_sendMESSAGE, _fsm_state_Sending, tsip_dialog_message_Started_2_Sending_X_sendMESSAGE, "tsip_dialog_message_Started_2_Sending_X_sendMESSAGE"),
                // Started -> (receive) -> Receiving
                TSK_FSM_ADD_ALWAYS(_fsm_state_Started, _fsm_action_receiveMESSAGE, _fsm_state_Receiving, tsip_dialog_message_Started_2_Receiving_X_recvMESSAGE, "tsip_dialog_message_Started_2_Receiving_X_recvMESSAGE"),
                // Started -> (Any) -> Started
                TSK_FSM_ADD_ALWAYS_NOTHING(_fsm_state_Started, "tsip_dialog_message_Started_2_Started_X_any"),


                /*=======================
                * === Sending ===
                */
                // Sending -> (1xx) -> Sending
                TSK_FSM_ADD_ALWAYS(_fsm_state_Sending, _fsm_action_1xx, _fsm_state_Sending, tsip_dialog_message_Sending_2_Sending_X_1xx, "tsip_dialog_message_Sending_2_Sending_X_1xx"),
                // Sending -> (2xx) -> Terminated
                TSK_FSM_ADD_ALWAYS(_fsm_state_Sending, _fsm_action_2xx, _fsm_state_Terminated, tsip_dialog_message_Sending_2_Terminated_X_2xx, "tsip_dialog_message_Sending_2_Terminated_X_2xx"),
                // Sending -> (401/407/421/494) -> Sending
                TSK_FSM_ADD_ALWAYS(_fsm_state_Sending, _fsm_action_401_407_421_494, _fsm_state_Sending, tsip_dialog_message_Sending_2_Sending_X_401_407_421_494, "tsip_dialog_message_Sending_2_Sending_X_401_407_421_494"),
                // Sending -> (300_to_699) -> Terminated
                TSK_FSM_ADD_ALWAYS(_fsm_state_Sending, _fsm_action_300_to_699, _fsm_state_Terminated, tsip_dialog_message_Sending_2_Terminated_X_300_to_699, "tsip_dialog_message_Sending_2_Terminated_X_300_to_699"),
                // Sending -> (cancel) -> Terminated
                TSK_FSM_ADD_ALWAYS(_fsm_state_Sending, _fsm_action_cancel, _fsm_state_Terminated, tsip_dialog_message_Sending_2_Terminated_X_cancel, "tsip_dialog_message_Sending_2_Terminated_X_cancel"),
                // Sending -> (shutdown) -> Terminated
                TSK_FSM_ADD_ALWAYS(_fsm_state_Sending, _fsm_action_shutdown, _fsm_state_Terminated, tsk_null, "tsip_dialog_message_Sending_2_Terminated_X_shutdown"),
                // Sending -> (Any) -> Sending
                TSK_FSM_ADD_ALWAYS_NOTHING(_fsm_state_Sending, "tsip_dialog_message_Sending_2_Sending_X_any"),

                /*=======================
                * === Receiving ===
                */
                // Receiving -> (accept) -> Terminated
                TSK_FSM_ADD_ALWAYS(_fsm_state_Receiving, _fsm_action_accept, _fsm_state_Terminated, tsip_dialog_message_Receiving_2_Terminated_X_accept, "tsip_dialog_message_Receiving_2_Terminated_X_accept"),
                // Receiving -> (rejected) -> Terminated
                TSK_FSM_ADD_ALWAYS(_fsm_state_Receiving, _fsm_action_reject, _fsm_state_Terminated, tsip_dialog_message_Receiving_2_Terminated_X_reject, "tsip_dialog_message_Receiving_2_Terminated_X_reject"),
                // Receiving -> (Any) -> Receiving
                TSK_FSM_ADD_ALWAYS_NOTHING(_fsm_state_Receiving, "tsip_dialog_message_Receiving_2_Receiving_X_any"),

                /*=======================
                * === Any ===
                */
                // Any -> (transport error) -> Terminated
                TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, _fsm_action_transporterror, _fsm_state_Terminated, tsip_dialog_message_Any_2_Terminated_X_transportError, "tsip_dialog_message_Any_2_Terminated_X_transportError"),
                // Any -> (transport error) -> Terminated
                TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, _fsm_action_error, _fsm_state_Terminated, tsip_dialog_message_Any_2_Terminated_X_Error, "tsip_dialog_message_Any_2_Terminated_X_Error"),

                TSK_FSM_ADD_NULL());

    TSIP_DIALOG(self)->callback = TSIP_DIALOG_EVENT_CALLBACK_F(tsip_dialog_message_event_callback);

    return 0;
}


//--------------------------------------------------------
//				== STATE MACHINE BEGIN ==
//--------------------------------------------------------


/* Started -> (sendMESSAGE) -> Sending
*/
int tsip_dialog_message_Started_2_Sending_X_sendMESSAGE(va_list *app)
{
    tsip_dialog_message_t *self;

    self = va_arg(*app, tsip_dialog_message_t *);

    TSIP_DIALOG(self)->running = tsk_true;

    return send_MESSAGE(self);
}

/* Started -> (recvMESSAGE) -> Receiving
*/
int tsip_dialog_message_Started_2_Receiving_X_recvMESSAGE(va_list *app)
{
    tsip_dialog_message_t *self = va_arg(*app, tsip_dialog_message_t *);
    const tsip_request_t *request = va_arg(*app, const tsip_request_t *);

    /* Alert the user. */
    TSIP_DIALOG_MESSAGE_SIGNAL(self, tsip_i_message,
                               tsip_event_code_dialog_request_incoming, "Incoming Request.", request);

    /* Update last incoming MESSAGE */
    TSK_OBJECT_SAFE_FREE(self->request);
    self->request = tsk_object_ref((void*)request);

    return 0;
}

/*	Sending -> (1xx) -> Sending
*/
int tsip_dialog_message_Sending_2_Sending_X_1xx(va_list *app)
{
    /*tsip_dialog_message_t *self = va_arg(*app, tsip_dialog_message_t *);*/
    /*const tsip_response_t *response = va_arg(*app, const tsip_response_t *);*/

    return 0;
}

/*	Sending -> (2xx) -> Sending
*/
int tsip_dialog_message_Sending_2_Terminated_X_2xx(va_list *app)
{
    tsip_dialog_message_t *self = va_arg(*app, tsip_dialog_message_t *);
    const tsip_response_t *response = va_arg(*app, const tsip_response_t *);

    /* Alert the user. */
    TSIP_DIALOG_MESSAGE_SIGNAL(self, tsip_ao_message,
                               TSIP_RESPONSE_CODE(response), TSIP_RESPONSE_PHRASE(response), response);

    /* Reset curr action */
    tsip_dialog_set_curr_action(TSIP_DIALOG(self), tsk_null);

    return 0;
}

/*	Sending -> (401/407/421/494) -> Sending
*/
int tsip_dialog_message_Sending_2_Sending_X_401_407_421_494(va_list *app)
{
    tsip_dialog_message_t *self = va_arg(*app, tsip_dialog_message_t *);
    const tsip_response_t *response = va_arg(*app, const tsip_response_t *);
    int ret;

    if((ret = tsip_dialog_update(TSIP_DIALOG(self), response))) {
        // Alert the user
        TSIP_DIALOG_MESSAGE_SIGNAL(self, tsip_ao_message,
                                   TSIP_RESPONSE_CODE(response), TSIP_RESPONSE_PHRASE(response), response);

        return ret;
    }

    return send_MESSAGE(self);
}

/*	Sending -> (300 to 699) -> Terminated
*/
int tsip_dialog_message_Sending_2_Terminated_X_300_to_699(va_list *app)
{
    tsip_dialog_message_t *self = va_arg(*app, tsip_dialog_message_t *);
    const tsip_response_t *response = va_arg(*app, const tsip_response_t *);

    /* set last error (or info) */
    tsip_dialog_set_lasterror(TSIP_DIALOG(self), TSIP_RESPONSE_PHRASE(response), TSIP_RESPONSE_CODE(response));

    /* Alert the user. */
    TSIP_DIALOG_MESSAGE_SIGNAL(self, tsip_ao_message,
                               TSIP_RESPONSE_CODE(response), TSIP_RESPONSE_PHRASE(response), response);

    return 0;
}

/*	Sending -> (cancel) -> Terminated
*/
int tsip_dialog_message_Sending_2_Terminated_X_cancel(va_list *app)
{
    tsip_dialog_message_t *self = va_arg(*app, tsip_dialog_message_t *);
    /* const tsip_message_t *message = va_arg(*app, const tsip_message_t *); */

    /* RFC 3261 - 9.1 Client Behavior
       A CANCEL request SHOULD NOT be sent to cancel a request other than INVITE.
    */

    /* Cancel all transactions associated to this dialog (will also be done when the dialog is destroyed (worth nothing)) */
    tsip_transac_layer_cancel_by_dialog(TSIP_DIALOG_GET_STACK(self)->layer_transac, TSIP_DIALOG(self));

    /* Alert the user */
    TSIP_DIALOG_SIGNAL(self, tsip_event_code_dialog_request_cancelled, "MESSAGE cancelled");

    return 0;
}

/*	Receiving -> (accept) -> Terminated
*/
int tsip_dialog_message_Receiving_2_Terminated_X_accept(va_list *app)
{
    tsip_dialog_message_t *self;
    const tsip_action_t* action;

    self = va_arg(*app, tsip_dialog_message_t *);
    va_arg(*app, tsip_message_t *);
    action = va_arg(*app, const tsip_action_t *);

    if(!self->request) {
        TSK_DEBUG_ERROR("There is non MESSAGE to accept()");
        /* Not an error ...but do not update current action */
    }
    else {
        tsip_response_t *response;
        int ret;

        /* curr_action is only used for outgoing requests */
        /* tsip_dialog_set_curr_action(TSIP_DIALOG(self), action); */

        /* send 200 OK */
        if((response = tsip_dialog_response_new(TSIP_DIALOG(self), 200, "OK", self->request))) {
            tsip_dialog_apply_action(response, action); /* apply action params to "this" response */
            if((ret = tsip_dialog_response_send(TSIP_DIALOG(self), response))) {
                TSK_DEBUG_ERROR("Failed to send SIP response.");
                TSK_OBJECT_SAFE_FREE(response);
                return ret;
            }
            TSK_OBJECT_SAFE_FREE(response);
        }
        else {
            TSK_DEBUG_ERROR("Failed to create SIP response.");
            return -1;
        }
    }

    return 0;
}

/*	Receiving -> (reject) -> Terminated
*/
int tsip_dialog_message_Receiving_2_Terminated_X_reject(va_list *app)
{
    tsip_dialog_message_t *self;
    const tsip_action_t* action;

    self = va_arg(*app, tsip_dialog_message_t *);
    va_arg(*app, tsip_message_t *);
    action = va_arg(*app, const tsip_action_t *);

    if(!self->request) {
        TSK_DEBUG_ERROR("There is non MESSAGE to reject()");
        /* Not an error ...but do not update current action */
    }
    else {
        tsip_response_t *response;
        int ret;

        /* curr_action is only used for outgoing requests */
        /* tsip_dialog_set_curr_action(TSIP_DIALOG(self), action); */

        /* send 486 Rejected */
        if((response = tsip_dialog_response_new(TSIP_DIALOG(self), 486, "Rejected", self->request))) {
            tsip_dialog_apply_action(response, action); /* apply action params to "this" response */
            if((ret = tsip_dialog_response_send(TSIP_DIALOG(self), response))) {
                TSK_DEBUG_ERROR("Failed to send SIP response.");
                TSK_OBJECT_SAFE_FREE(response);
                return ret;
            }
            TSK_OBJECT_SAFE_FREE(response);
        }
        else {
            TSK_DEBUG_ERROR("Failed to create SIP response.");
            return -1;
        }
    }

    return 0;
}

/*	Any -> (transport error) -> Terminated
*/
int tsip_dialog_message_Any_2_Terminated_X_transportError(va_list *app)
{
    /*tsip_dialog_message_t *self = va_arg(*app, tsip_dialog_message_t *);*/
    /*const tsip_message_t *message = va_arg(*app, const tsip_message_t *);*/

    return 0;
}

/*	Any -> (error) -> Terminated
*/
int tsip_dialog_message_Any_2_Terminated_X_Error(va_list *app)
{
    /*tsip_dialog_message_t *self = va_arg(*app, tsip_dialog_message_t *);*/
    /*const tsip_message_t *message = va_arg(*app, const tsip_message_t *);*/

    return 0;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//				== STATE MACHINE END ==
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int send_MESSAGE(tsip_dialog_message_t *self)
{
    tsip_request_t* request = tsk_null;
    int ret = -1;

    if(!self) {
        return -1;
    }

    if(!(request = tsip_dialog_request_new(TSIP_DIALOG(self), "MESSAGE"))) {
        return -2;
    }

    /* apply action params to the request */
    if(TSIP_DIALOG(self)->curr_action) {
        tsip_dialog_apply_action(request, TSIP_DIALOG(self)->curr_action);
    }

    ret = tsip_dialog_request_send(TSIP_DIALOG(self), request);
    TSK_OBJECT_SAFE_FREE(request);

    return ret;
}


int tsip_dialog_message_OnTerminated(tsip_dialog_message_t *self)
{
    TSK_DEBUG_INFO("=== MESSAGE Dialog terminated ===");

    /* Alert the user */
    TSIP_DIALOG_SIGNAL(self, tsip_event_code_dialog_terminated,
                       TSIP_DIALOG(self)->last_error.phrase ? TSIP_DIALOG(self)->last_error.phrase : "Dialog terminated");

    /* Remove from the dialog layer. */
    return tsip_dialog_remove(TSIP_DIALOG(self));
}






















//========================================================
//	SIP dialog MESSAGE object definition
//
static tsk_object_t* tsip_dialog_message_ctor(tsk_object_t * self, va_list * app)
{
    tsip_dialog_message_t *dialog = self;
    if(dialog) {
        tsip_ssession_handle_t *ss = va_arg(*app, tsip_ssession_handle_t *);

        /* Initialize base class */
        tsip_dialog_init(TSIP_DIALOG(self), tsip_dialog_MESSAGE, tsk_null, ss, _fsm_state_Started, _fsm_state_Terminated);

        /* FSM */
        TSIP_DIALOG_GET_FSM(self)->debug = DEBUG_STATE_MACHINE;
        tsk_fsm_set_callback_terminated(TSIP_DIALOG_GET_FSM(self), TSK_FSM_ONTERMINATED_F(tsip_dialog_message_OnTerminated), (const void*)dialog);

        /* Initialize the class itself */
        tsip_dialog_message_init(self);
    }
    return self;
}

static tsk_object_t* tsip_dialog_message_dtor(tsk_object_t * self)
{
    tsip_dialog_message_t *dialog = self;
    if(dialog) {
        /* DeInitialize base class (will cancel all transactions) */
        tsip_dialog_deinit(TSIP_DIALOG(self));

        /* DeInitialize self */
        TSK_OBJECT_SAFE_FREE(dialog->request);

        TSK_DEBUG_INFO("*** MESSAGE Dialog destroyed ***");
    }
    return self;
}

static int tsip_dialog_message_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
    return tsip_dialog_cmp(obj1, obj2);
}

static const tsk_object_def_t tsip_dialog_message_def_s = {
    sizeof(tsip_dialog_message_t),
    tsip_dialog_message_ctor,
    tsip_dialog_message_dtor,
    tsip_dialog_message_cmp,
};
const tsk_object_def_t *tsip_dialog_message_def_t = &tsip_dialog_message_def_s;
