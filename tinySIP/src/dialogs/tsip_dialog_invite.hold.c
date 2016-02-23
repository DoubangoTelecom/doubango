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

/**@file tsip_dialog_invite.hold.c
 * @brief Communication Hold (3GPP TS 24.610)
 * The Communication Hold supplementary service enables a user to suspend the reception of media stream(s) of an established IP multimedia session,
 * and resume the media stream(s) at a later time.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/dialogs/tsip_dialog_invite.h"

#include "tinysip/dialogs/tsip_dialog_invite.common.h"

#include "tsk_debug.h"

/* ======================== transitions ======================== */
static int x0100_Connected_2_Holding_X_oHold(va_list *app);
static int x0101_Holding_2_Connected_X_ixxx(va_list *app);
static int x0102_Connected_2_Resuming_X_oResume(va_list *app);
static int x0103_Resuming_2_Connected_X_ixxx(va_list *app);

static int x0150_Any_2_Any_X_i422(va_list *app);

/* ======================== conds ======================== */
static tsk_bool_t _fsm_cond_is_resp2INVITEorUPDATE(tsip_dialog_invite_t* self, tsip_message_t* message)
{
    return (TSIP_RESPONSE_IS_TO_INVITE(message) || TSIP_RESPONSE_IS_TO_UPDATE(message));
}

/* ======================== external functions ======================== */
extern int send_INVITEorUPDATE(tsip_dialog_invite_t *self, tsk_bool_t is_INVITE, tsk_bool_t force_sdp);
extern int tsip_dialog_invite_process_ro(tsip_dialog_invite_t *self, const tsip_message_t* message);
extern int send_ACK(tsip_dialog_invite_t *self, const tsip_response_t* r2xxINVITE);

int tsip_dialog_invite_hold_init(tsip_dialog_invite_t *self)
{
    tsk_fsm_set(TSIP_DIALOG_GET_FSM(self),

                /*=======================
                * === Hold ===
                */
                // Connected -> (send HOLD) -> Holding
                TSK_FSM_ADD_ALWAYS(_fsm_state_Connected, _fsm_action_oHold, _fsm_state_Holding, x0100_Connected_2_Holding_X_oHold, "x0100_Connected_2_Holding_X_oHold"),
                // Holding -> (i2xx) -> Connected
                TSK_FSM_ADD(_fsm_state_Holding, _fsm_action_i2xx, _fsm_cond_is_resp2INVITEorUPDATE, _fsm_state_Connected, x0101_Holding_2_Connected_X_ixxx, "x0101_Holding_2_Connected_X_ixxx"),
                // Holding -> (i300-699) -> Connected
                TSK_FSM_ADD(_fsm_state_Holding, _fsm_action_i300_to_i699, _fsm_cond_is_resp2INVITEorUPDATE, _fsm_state_Connected, x0101_Holding_2_Connected_X_ixxx, "x0101_Holding_2_Connected_X_ixxx"),

                /*=======================
                * === Resume ===
                */
                // Connected -> (send RESUME) -> Resuming
                TSK_FSM_ADD_ALWAYS(_fsm_state_Connected, _fsm_action_oResume, _fsm_state_Resuming, x0102_Connected_2_Resuming_X_oResume, "x0102_Connected_2_Resuming_X_oResume"),
                // Resuming -> (i2xx) -> Connected
                TSK_FSM_ADD(_fsm_state_Resuming, _fsm_action_i2xx, _fsm_cond_is_resp2INVITEorUPDATE, _fsm_state_Connected, x0103_Resuming_2_Connected_X_ixxx, "x0103_Resuming_2_Connected_X_ixxx"),
                // Resuming -> (i300-699) -> Connected
                TSK_FSM_ADD(_fsm_state_Resuming, _fsm_action_i300_to_i699, _fsm_cond_is_resp2INVITEorUPDATE, _fsm_state_Connected, x0103_Resuming_2_Connected_X_ixxx, "x0103_Resuming_2_Connected_X_ixxx"),

                TSK_FSM_ADD_NULL());

    return 0;
}



//--------------------------------------------------------
//				== STATE MACHINE BEGIN ==
//--------------------------------------------------------

// Connected -> (send HOLD) -> Holding
int x0100_Connected_2_Holding_X_oHold(va_list *app)
{
    int ret;
    tsip_dialog_invite_t *self;
    const tsip_action_t* action;

    self = va_arg(*app, tsip_dialog_invite_t *);
    va_arg(*app, const tsip_message_t *);
    action = va_arg(*app, const tsip_action_t *);

    if(!self->msession_mgr) {
        TSK_DEBUG_WARN("Media Session manager is Null");
        return 0;
    }

    /* put on hold */
    ret = tmedia_session_mgr_hold(self->msession_mgr, action->media.type);

    /* send the request */
    if((ret = send_INVITE(self, tsk_false))) {
        // FIXME: signal error without breaking the state machine
    }

    return 0;
}

// Holding -> (ixxx) -> Connected
int x0101_Holding_2_Connected_X_ixxx(va_list *app)
{
    int ret;

    tsip_dialog_invite_t* self = va_arg(*app, tsip_dialog_invite_t *);
    const tsip_response_t* response = va_arg(*app, const tsip_response_t *);

    /* reset current action */
    tsip_dialog_set_curr_action(TSIP_DIALOG(self), tsk_null);

    /* Process remote offer */
    if((ret = tsip_dialog_invite_process_ro(self, response))) {
        /* Send error */
        return ret;
    }
    else if(TSIP_RESPONSE_IS_TO_INVITE(response)) {
        /* send ACK */
        ret = send_ACK(self, response);
    }

    /* alert the user */
    if(TSIP_RESPONSE_IS_2XX(response)) {
        TSIP_DIALOG_INVITE_SIGNAL(self, tsip_m_local_hold_ok,
                                  TSIP_RESPONSE_CODE(response), TSIP_RESPONSE_PHRASE(response), response);
        self->hold.local = tsk_true;
    }
    else {
        TSIP_DIALOG_INVITE_SIGNAL(self, tsip_m_local_hold_nok,
                                  TSIP_RESPONSE_CODE(response), TSIP_RESPONSE_PHRASE(response), response);
        self->hold.local = tsk_false;
    }

    return ret;
}

// Connected -> (send RESUME) -> Resuming
int x0102_Connected_2_Resuming_X_oResume(va_list *app)
{
    int ret;
    tsip_dialog_invite_t *self;
    const tsip_action_t* action;

    self = va_arg(*app, tsip_dialog_invite_t *);
    va_arg(*app, const tsip_message_t *);
    action = va_arg(*app, const tsip_action_t *);

    if(!self->msession_mgr) {
        TSK_DEBUG_WARN("Media Session manager is Null");
        return 0;
    }

    /* Resume both */
    ret = tmedia_session_mgr_resume(self->msession_mgr, action->media.type, tsk_true);
    ret = tmedia_session_mgr_resume(self->msession_mgr, action->media.type, tsk_false);

    /* send the request */
    if((ret = send_INVITE(self, tsk_false))) {
        // FIXME: signal error without breaking the state machine
    }

    return 0;
}

// Resuming -> (ixxx) -> Connected
int x0103_Resuming_2_Connected_X_ixxx(va_list *app)
{
    int ret;

    tsip_dialog_invite_t* self = va_arg(*app, tsip_dialog_invite_t *);
    const tsip_response_t* response = va_arg(*app, const tsip_response_t *);

    /* reset current action */
    tsip_dialog_set_curr_action(TSIP_DIALOG(self), tsk_null);

    /* Process remote offer */
    if((ret = tsip_dialog_invite_process_ro(self, response))) {
        /* Send error */
        return ret;
    }
    else if(TSIP_RESPONSE_IS_TO_INVITE(response)) {
        /* send ACK */
        ret = send_ACK(self, response);
    }

    /* alert the user */
    if(TSIP_RESPONSE_IS_2XX(response)) {
        TSIP_DIALOG_INVITE_SIGNAL(self, tsip_m_local_resume_ok,
                                  TSIP_RESPONSE_CODE(response), TSIP_RESPONSE_PHRASE(response), response);
        self->hold.local = tsk_false;
    }
    else {
        TSIP_DIALOG_INVITE_SIGNAL(self, tsip_m_local_resume_nok,
                                  TSIP_RESPONSE_CODE(response), TSIP_RESPONSE_PHRASE(response), response);
        self->hold.local = tsk_true;
    }

    return ret;
}

/* handle requests/responses (MUST be called after set_ro()) */
int tsip_dialog_invite_hold_handle(tsip_dialog_invite_t* self, const tsip_request_t* rINVITEorUPDATE)
{
    tsk_bool_t remote_hold, bodiless_invite;
    int ret = 0;

    if(!self || !rINVITEorUPDATE || !self->msession_mgr) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    remote_hold = tmedia_session_mgr_is_held(self->msession_mgr, self->msession_mgr->type, tsk_false);

    // resume the call if we receive bodiless INVITE
    bodiless_invite = !TSIP_MESSAGE_HAS_CONTENT(rINVITEorUPDATE) && TSIP_REQUEST_IS_INVITE(rINVITEorUPDATE);
    if(bodiless_invite && remote_hold) {
        // resume remote
        if((ret = tmedia_session_mgr_resume(self->msession_mgr, self->msession_mgr->type, tsk_false)) == 0) {
            remote_hold = tsk_false;
        }
    }

    if(ret == 0 && (remote_hold != self->hold.remote)) {
        self->hold.remote = remote_hold;
        TSIP_DIALOG_INVITE_SIGNAL(self, self->hold.remote ? tsip_m_remote_hold : tsip_m_remote_resume,
                                  tsip_event_code_dialog_request_incoming, "Hold/Resume state changed", rINVITEorUPDATE);
    }

    return ret;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//				== STATE MACHINE END ==
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
