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

/**@file tsip_dialog_invite.h
 * @brief SIP dialog INVITE.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef TINYSIP_DIALOG_INVITE_H
#define TINYSIP_DIALOG_INVITE_H

#include "tinysip_config.h"
#include "tinysip/dialogs/tsip_dialog.h"

#include "tinymedia/tmedia_session.h"


TSIP_BEGIN_DECLS

#define TSIP_DIALOG_INVITE(self)							((tsip_dialog_invite_t*)(self))

typedef struct tsip_dialog_invite {
    TSIP_DECLARE_DIALOG;

    tsk_bool_t support_update; /**< Indicates whether the remote party support UPDATE */
    tsk_bool_t is_client;
    tsk_bool_t is_transf;
    tsk_bool_t refersub;
    tsk_bool_t use_rtcp;
    tsk_bool_t use_rtcpmux;
    tsk_bool_t is_initial_iack_pending; // we're waiting for the initial incoming ACK (for the 200 OK) to ensure the session
    tsk_bool_t is_cancelling; // whether we're cancelling the outgoing INVITE
    tsk_bool_t is_conditional_ringing_enabled; // whether to ask end-user before sending 18x ringing message
    uint32_t rseq;
    uint32_t cseq_out_media_update; // CSeq for the last media update request (INVITE or UPDATE).
    uint64_t last_out_fastupdate_time;

    tsip_timer_t timershutdown;
    tsip_timer_t timer100rel;

    tsip_response_t* last_o1xxrel;
    tsip_request_t* last_iInvite;
    tsip_request_t* last_oInvite;
    tsip_request_t* last_iRefer;
    tmedia_session_mgr_t* msession_mgr; /**< Media session Manager */

    struct tsip_ssession_s* ss_transf;

    /* ICE */
    struct {
        tmedia_type_t media_type;
        tsk_bool_t is_jingle;
        tsk_bool_t start_smgr;
        struct tnet_ice_ctx_s *ctx_audio;
        struct tnet_ice_ctx_s *ctx_video;
        tsk_fsm_action_id last_action_id;
        tsip_action_t* last_action;
        tsip_message_t* last_message;
        int32_t last_sdp_ro_ver;
    } ice;

    /* Session Timers */
    struct {
        tsip_timer_t timer;
        char* refresher;
        uint64_t minse;
        tsk_bool_t is_refresher;
    } stimers;
    /* QoS (Preconditions) */
    struct {
        tsip_timer_t timer;
        enum tmedia_qos_stype_e type;
        enum tmedia_qos_strength_e strength;
    } qos;
    /* Hold/Resume */
    struct {
        unsigned remote:1;
        unsigned local:1;
    } hold;

    struct {
        unsigned _100rel:1;
        unsigned precondition:1;
        unsigned timer:1;
        unsigned norefersub:1;
        unsigned ice:1;
    } supported;

    struct {
        unsigned _100rel:1;
        unsigned precondition:1;
        unsigned timer:1;
        unsigned norefersub;
        unsigned ice:1;
    } required;
}
tsip_dialog_invite_t;

tsip_dialog_invite_t* tsip_dialog_invite_create(const tsip_ssession_handle_t* ss, const char* call_id);

int tsip_dialog_invite_start(tsip_dialog_invite_t *self);

TINYSIP_GEXTERN const tsk_object_def_t *tsip_dialog_invite_def_t;

TSIP_END_DECLS

#endif /* TINYSIP_DIALOG_INVITE_H */
