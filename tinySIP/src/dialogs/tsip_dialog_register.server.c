/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
*
* This file is part of Open Source Doubango Framework.
*
* DOUBANGO is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as publishd by
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
#include "tinysip/dialogs/tsip_dialog_register.h"
#include "tinysip/dialogs/tsip_dialog_register.common.h"


/* ======================== external functions ======================== */
extern int tsip_dialog_register_send_RESPONSE(tsip_dialog_register_t *self, const tsip_request_t* request, short code, const char* phrase);

/* ======================== transitions ======================== */
static int s0000_Started_2_Terminated_X_iREGISTER(va_list *app);
static int s0000_Started_2_Incoming_X_iREGISTER(va_list *app);
static int s0000_Incoming_2_Connected_X_Accept(va_list *app);
static int s0000_Incoming_2_Terminated_X_Terminates(va_list *app);
static int s0000_Connected_2_Connected_X_iREGISTER(va_list *app);
static int s0000_Connected_2_Terminated_X_iREGISTER(va_list *app);


/* ======================== conds ======================== */
static tsk_bool_t _fsm_cond_not_served_here(tsip_dialog_register_t* dialog, tsip_message_t* message)
{
#if 0 // FIXME: Have to disabled only when in proxy mode
    if(message && TSIP_REQUEST_IS_REGISTER(message)) {
        if(tsk_object_cmp(TSIP_DIALOG_GET_STACK(dialog)->network.realm, message->line.request.uri) != 0) {
            tsip_dialog_register_send_RESPONSE(dialog, TSIP_MESSAGE_AS_REQUEST(message), 404, "Domain not served here");
            return tsk_true;
        }
    }
#endif
    return tsk_false;
}
static tsk_bool_t _fsm_cond_server_unregistering(tsip_dialog_register_t* dialog, tsip_message_t* message)
{
    if(message && dialog->is_server) {
        int64_t expires = tsip_message_getExpires(message);
        dialog->unregistering = (expires == 0);
        return dialog->unregistering;
    }
    return tsk_false;
}
static tsk_bool_t _fsm_cond_server_registering(tsip_dialog_register_t* dialog, tsip_message_t* message)
{
    return !_fsm_cond_server_unregistering(dialog, message);
}


int tsip_dialog_register_server_init(tsip_dialog_register_t *self)
{
    return tsk_fsm_set(TSIP_DIALOG_GET_FSM(self),

                       /*=======================
                       * === Started ===
                       */
                       // Started -> (Domain Not Served here) -> Terminated
                       TSK_FSM_ADD(_fsm_state_Started, _fsm_action_iREGISTER, _fsm_cond_not_served_here, _fsm_state_Terminated, s0000_Started_2_Terminated_X_iREGISTER, "s0000_Started_2_Terminated_X_iREGISTER"),
                       // Started -> (All is OK and we are not unRegistering) -> Trying
                       TSK_FSM_ADD(_fsm_state_Started, _fsm_action_iREGISTER, _fsm_cond_server_registering, _fsm_state_Incoming, s0000_Started_2_Incoming_X_iREGISTER, "s0000_Started_2_Incoming_X_iREGISTER"),

                       /*=======================
                       * === Incoming ===
                       */
                       // Incoming -> (Accept) -> Connected
                       TSK_FSM_ADD_ALWAYS(_fsm_state_Incoming, _fsm_action_accept, _fsm_state_Connected, s0000_Incoming_2_Connected_X_Accept, "s0000_Incoming_2_Connected_X_Accept"),
                       // Incoming -> (iRegister) -> Incoming
                       TSK_FSM_ADD(_fsm_state_Incoming, _fsm_action_iREGISTER, _fsm_cond_server_registering, _fsm_state_Incoming, tsk_null, "s0000_Incoming_2_Incoming_X_iREGISTER"),
                       // Incoming -> (iRegister, expires=0) -> Terminated
                       TSK_FSM_ADD(_fsm_state_Incoming, _fsm_action_iREGISTER, _fsm_cond_server_unregistering, _fsm_state_Terminated, tsk_null, "s0000_Incoming_2_Terminated_X_iREGISTER"),
                       // Incoming -> (Reject) -> Terminated
                       TSK_FSM_ADD_ALWAYS(_fsm_state_Incoming, _fsm_action_reject, _fsm_state_Terminated, s0000_Incoming_2_Terminated_X_Terminates, "s0000_Incoming_2_Terminated_X_Terminates"),
                       // Incoming -> (Hangup) -> Terminated
                       TSK_FSM_ADD_ALWAYS(_fsm_state_Incoming, _fsm_action_hangup, _fsm_state_Terminated, s0000_Incoming_2_Terminated_X_Terminates, "s0000_Incoming_2_Terminated_X_Terminates"),

                       /*=======================
                       * === Connected ===
                       */
                       // Connected -> (Register) -> Connected
                       TSK_FSM_ADD(_fsm_state_Connected, _fsm_action_iREGISTER, _fsm_cond_server_registering, _fsm_state_Connected, s0000_Connected_2_Connected_X_iREGISTER, "s0000_Connected_2_Connected_X_iREGISTER"),
                       // Connected -> (UnRegister) -> Terminated
                       TSK_FSM_ADD(_fsm_state_Connected, _fsm_action_iREGISTER, _fsm_cond_server_unregistering, _fsm_state_Terminated, s0000_Connected_2_Terminated_X_iREGISTER, "s0000_Connected_2_Terminated_X_iREGISTER"),
                       // Connected -> (TimedOut) -> Terminated
                       // Connected -> (Refresh OK) -> Connected
                       // Connected -> (Refresh NOK) -> Terminated

                       TSK_FSM_ADD_NULL());
}



//--------------------------------------------------------
//				== STATE MACHINE BEGIN ==
//--------------------------------------------------------

/* Started -> (Failure) -> Terminated
*/
int s0000_Started_2_Terminated_X_iREGISTER(va_list *app)
{
    return 0;
    /*tsip_dialog_register_t *self;
    const tsip_action_t* action;

    self = va_arg(*app, tsip_dialog_register_t *);
    va_arg(*app, const tsip_message_t *);
    action = va_arg(*app, const tsip_action_t *);

    TSIP_DIALOG(self)->running = tsk_true;
    tsip_dialog_set_curr_action(TSIP_DIALOG(self), action);

    // alert the user
    TSIP_DIALOG_SIGNAL(self, tsip_event_code_dialog_connecting, "Dialog connecting");

    return send_REGISTER(self, tsk_true);*/
}

/* Started -> (All is OK and we are Registering) -> Incoming
*/
int s0000_Started_2_Incoming_X_iREGISTER(va_list *app)
{
    tsip_dialog_register_t *self = va_arg(*app, tsip_dialog_register_t *);
    tsip_request_t *request = va_arg(*app, tsip_request_t *);

    // set as server side dialog
    TSIP_DIALOG_REGISTER(self)->is_server = tsk_true;

    /* update last REGISTER */
    TSK_OBJECT_SAFE_FREE(self->last_iRegister);
    self->last_iRegister = tsk_object_ref(request);

    /* alert the user (session) */
    TSIP_DIALOG_REGISTER_SIGNAL(self, tsip_i_newreg,
                                tsip_event_code_dialog_request_incoming, "Incoming New Register", request);

    return 0;
}

/* Incoming -> (Accept) -> Connected
*/
int s0000_Incoming_2_Connected_X_Accept(va_list *app)
{
    int ret;

    tsip_dialog_register_t *self;

    self = va_arg(*app, tsip_dialog_register_t *);

    /* send 2xx OK */
    if((ret = tsip_dialog_register_send_RESPONSE(self, self->last_iRegister, 200, "OK"))) {
        return ret;
    }

    /* alert the user (dialog) */
    TSIP_DIALOG_SIGNAL(self, tsip_event_code_dialog_connected, "Dialog connected");

    return ret;
}

/* Incoming -> (Reject) -> Terminated
*/
int s0000_Incoming_2_Terminated_X_Terminates(va_list *app)
{
    int ret;
    short code;
    const char* phrase;
    char* reason = tsk_null;

    tsip_dialog_register_t *self;
    const tsip_action_t* action;

    self = va_arg(*app, tsip_dialog_register_t *);
    va_arg(*app, const tsip_message_t *);
    action = va_arg(*app, const tsip_action_t *);

    /* Send Reject */
    code = action->line_resp.code>=300 ? action->line_resp.code : 600;
    phrase = action->line_resp.phrase ? action->line_resp.phrase : "Not Supported";
    tsk_sprintf(&reason, "SIP; cause=%hi; text=\"%s\"", code, phrase);
    ret = tsip_dialog_register_send_RESPONSE(self, self->last_iRegister, code, phrase);
    TSK_FREE(reason);

    return ret;
}

/* Connected -> (register) -> Connected
*/
static int s0000_Connected_2_Connected_X_iREGISTER(va_list *app)
{
    tsip_dialog_register_t *self = va_arg(*app, tsip_dialog_register_t *);
    const tsip_request_t *request = va_arg(*app, const tsip_request_t *);

    TSK_OBJECT_SAFE_FREE(self->last_iRegister);
    self->last_iRegister = tsk_object_ref((tsk_object_t*)request);

    /* send 2xx OK */
    return tsip_dialog_register_send_RESPONSE(self, self->last_iRegister, 200, "OK");
}

/* Connected -> (Unregister) -> Terminated
*/
int s0000_Connected_2_Terminated_X_iREGISTER(va_list *app)
{
    int ret;
    tsip_dialog_register_t *self = va_arg(*app, tsip_dialog_register_t *);
    tsip_request_t *request = va_arg(*app, tsip_request_t *);

    /* update last REGISTER */
    TSK_OBJECT_SAFE_FREE(self->last_iRegister);
    self->last_iRegister = tsk_object_ref(request);

    /* send 2xx OK */
    if((ret = tsip_dialog_register_send_RESPONSE(self, self->last_iRegister, 200, "OK"))) {
        return ret;
    }

    /* alert the user (session) */
    TSIP_DIALOG_REGISTER_SIGNAL(self, tsip_i_unregister,
                                tsip_event_code_dialog_request_incoming, "Incoming Request", request);

    return 0;
}