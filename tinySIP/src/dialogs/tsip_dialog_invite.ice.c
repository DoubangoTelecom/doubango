/*
 * Copyright (C) 2012 Doubango Telecom <http://www.doubango.org>
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
#include "tinysip/dialogs/tsip_dialog_invite.h"

#include "tinysip/dialogs/tsip_dialog_invite.common.h"

#include "tinysdp/parsers/tsdp_parser_message.h"
#include "tinysdp/tsdp_message.h"
#include "tinysdp/headers/tsdp_header_S.h"
#include "tinysdp/headers/tsdp_header_O.h"

#include "stun/tnet_stun_types.h"
#include "ice/tnet_ice_ctx.h"

#include "tsk_debug.h"

extern int tsip_dialog_invite_msession_start(tsip_dialog_invite_t *self);

static int tsip_dialog_invite_ice_create_ctx(tsip_dialog_invite_t * self, tmedia_type_t media_type);
static int tsip_dialog_invite_ice_audio_callback(const tnet_ice_event_t *e);
static int tsip_dialog_invite_ice_video_callback(const tnet_ice_event_t *e);
int tsip_dialog_invite_ice_set_media_type(tsip_dialog_invite_t * self, tmedia_type_t media_type);
tsk_bool_t tsip_dialog_invite_ice_got_local_candidates(const tsip_dialog_invite_t * self);
int tsip_dialog_invite_ice_process_ro(tsip_dialog_invite_t * self, const tsdp_message_t* sdp_ro, tsk_bool_t is_remote_offer);

#define tsip_dialog_invite_ice_cancel_silent_and_sync_ctx(_self) \
tsip_dialog_invite_ice_set_sync_mode_ctx((_self), tsk_true); \
tsip_dialog_invite_ice_set_silent_mode_ctx((_self), tsk_true); \
tsip_dialog_invite_ice_cancel_ctx((_self));  /* "cancelled" event will not be sent and we're sure that cancel operation will be done when the function exit */ \
tsip_dialog_invite_ice_set_sync_mode_ctx((_self), tsk_false); \
tsip_dialog_invite_ice_set_silent_mode_ctx((_self), tsk_false); \
 
/* ======================== transitions ======================== */
// Use "Current" instead of "Any" to avoid priority reordering
static int x0500_Current_2_Current_X_oINVITE(va_list *app);
static int x0500_Current_2_Current_X_iINVITE(va_list *app);

/* ======================== conds ======================== */
static tsk_bool_t _fsm_cond_get_local_candidates(tsip_dialog_invite_t* self, tsip_message_t* message)
{
    if(self->supported.ice) {
        tsk_bool_t use_ice = tsk_false;
        // "action->media.type" will be defined for locally initiated media update
        tmedia_type_t new_media = TSIP_DIALOG(self)->curr_action ? TSIP_DIALOG(self)->curr_action->media.type : tmedia_none;

        if(message && TSIP_MESSAGE_HAS_CONTENT(message) && tsk_striequals("application/sdp", TSIP_MESSAGE_CONTENT_TYPE(message))) {
            // If this code is called this means that we are the "answerer"
            // only gets the candidates if ICE is enabled and the remote peer supports ICE
            tsdp_message_t* sdp_ro;
            const tsdp_header_M_t* M;
            int index;
            if(!(sdp_ro = tsdp_message_parse(TSIP_MESSAGE_CONTENT_DATA(message), TSIP_MESSAGE_CONTENT_DATA_LENGTH(message)))) {
                TSK_DEBUG_ERROR("Failed to parse remote sdp message");
                return tsk_false;
            }

            index = 0;
            while((M = (const tsdp_header_M_t*)tsdp_message_get_headerAt(sdp_ro, tsdp_htype_M, index++))) {
                if(!tsdp_header_M_findA(M, "candidate")) {
                    use_ice = tsk_false; // do not use ICE if at least on media is ICE-less (e.g. MSRP)
                    break;
                }
                use_ice = tsk_true; // only use ICE if there is a least one media line
            }

            new_media = tmedia_type_from_sdp(sdp_ro);

            TSK_OBJECT_SAFE_FREE(sdp_ro);
        }
        else if(!message) {
            // we are the "offerer" -> use ICE only for audio or video medias (ignore ice for MSRP)
            use_ice = (new_media & tmedia_audio) || (new_media & tmedia_video);
        }

        if(use_ice) {
            if(!self->ice.ctx_audio && !self->ice.ctx_video) { // First time
                return tsk_true;
            }
            else {
                if(self->ice.media_type != new_media) {
                    return tsk_true;
                }
                return !tsip_dialog_invite_ice_got_local_candidates(self);
            }
        }
    }
    return tsk_false;
}

int tsip_dialog_invite_ice_init(tsip_dialog_invite_t *self)
{
    tsk_fsm_set(TSIP_DIALOG_GET_FSM(self),
                // Current -> (oINVITE) -> Current
                TSK_FSM_ADD(tsk_fsm_state_current, _fsm_action_oINVITE, _fsm_cond_get_local_candidates, tsk_fsm_state_current, x0500_Current_2_Current_X_oINVITE, "x0500_Current_2_Current_X_oINVITE"),
                // Current -> (iINVITE) -> Current
                TSK_FSM_ADD(tsk_fsm_state_current, _fsm_action_iINVITE, _fsm_cond_get_local_candidates, tsk_fsm_state_current, x0500_Current_2_Current_X_iINVITE, "x0500_Current_2_Current_X_iINVITE")
               );

    return 0;
}

int tsip_dialog_invite_ice_timers_set(tsip_dialog_invite_t *self, int64_t timeout)
{
    if(/*tnet_ice_ctx_is_active*/(self->ice.ctx_audio)) {
        tnet_ice_ctx_set_concheck_timeout(self->ice.ctx_audio, timeout);
    }
    if(/*tnet_ice_ctx_is_active*/(self->ice.ctx_video)) {
        tnet_ice_ctx_set_concheck_timeout(self->ice.ctx_video, timeout);
    }
    return 0;
}

static int tsip_dialog_invite_ice_create_ctx(tsip_dialog_invite_t * self, tmedia_type_t media_type)
{
    int32_t transport_idx;
    int ret = 0;
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    transport_idx = TSIP_DIALOG_GET_STACK(self)->network.transport_idx_default;
    if (!self->ice.ctx_audio && (media_type & tmedia_audio)) {
        self->ice.ctx_audio = tnet_ice_ctx_create(self->ice.is_jingle, TNET_SOCKET_TYPE_IS_IPV6(TSIP_DIALOG_GET_STACK(self)->network.proxy_cscf_type[transport_idx]),
                              self->use_rtcp, tsk_false, tsip_dialog_invite_ice_audio_callback, self);
        if (!self->ice.ctx_audio) {
            TSK_DEBUG_ERROR("Failed to create ICE audio context");
            return -2;
        }
#if 0 // @deprecated
        ret = tnet_ice_ctx_set_stun(self->ice.ctx_audio, TSIP_DIALOG_GET_SS(self)->media.stun.hostname, TSIP_DIALOG_GET_SS(self)->media.stun.port, kStunSoftware, TSIP_DIALOG_GET_SS(self)->media.stun.username, TSIP_DIALOG_GET_SS(self)->media.stun.password);
#else
        ret = tnet_ice_ctx_add_server(
                  self->ice.ctx_audio,
                  "udp", // "tcp", "tls", "ws", "wss"...
                  TSIP_DIALOG_GET_SS(self)->media.stun.hostname,
                  TSIP_DIALOG_GET_SS(self)->media.stun.port,
                  TSIP_DIALOG_GET_SS(self)->media.enable_iceturn,
                  TSIP_DIALOG_GET_SS(self)->media.enable_icestun,
                  TSIP_DIALOG_GET_SS(self)->media.stun.username,
                  TSIP_DIALOG_GET_SS(self)->media.stun.password);
#endif
        ret = tnet_ice_ctx_set_turn_enabled(self->ice.ctx_audio, TSIP_DIALOG_GET_SS(self)->media.enable_iceturn);
        ret = tnet_ice_ctx_set_stun_enabled(self->ice.ctx_audio, TSIP_DIALOG_GET_SS(self)->media.enable_icestun);
        ret = tnet_ice_ctx_set_rtcpmux(self->ice.ctx_audio, self->use_rtcpmux);
    }
    if (!self->ice.ctx_video && (media_type & tmedia_video)) {
        self->ice.ctx_video = tnet_ice_ctx_create(self->ice.is_jingle, TNET_SOCKET_TYPE_IS_IPV6(TSIP_DIALOG_GET_STACK(self)->network.proxy_cscf_type[transport_idx]),
                              self->use_rtcp, tsk_true, tsip_dialog_invite_ice_video_callback, self);
        if (!self->ice.ctx_video) {
            TSK_DEBUG_ERROR("Failed to create ICE video context");
            return -2;
        }
#if 0 // @deprecated
        ret = tnet_ice_ctx_set_stun(self->ice.ctx_video, TSIP_DIALOG_GET_SS(self)->media.stun.hostname, TSIP_DIALOG_GET_SS(self)->media.stun.port, kStunSoftware, TSIP_DIALOG_GET_SS(self)->media.stun.username, TSIP_DIALOG_GET_SS(self)->media.stun.password);
#else
        ret = tnet_ice_ctx_add_server(
                  self->ice.ctx_video,
                  "udp", // "tcp", "tls", "ws", "wss"...
                  TSIP_DIALOG_GET_SS(self)->media.stun.hostname,
                  TSIP_DIALOG_GET_SS(self)->media.stun.port,
                  TSIP_DIALOG_GET_SS(self)->media.enable_iceturn,
                  TSIP_DIALOG_GET_SS(self)->media.enable_icestun,
                  TSIP_DIALOG_GET_SS(self)->media.stun.username,
                  TSIP_DIALOG_GET_SS(self)->media.stun.password);
#endif
        ret = tnet_ice_ctx_set_turn_enabled(self->ice.ctx_video, TSIP_DIALOG_GET_SS(self)->media.enable_iceturn);
        ret = tnet_ice_ctx_set_stun_enabled(self->ice.ctx_video, TSIP_DIALOG_GET_SS(self)->media.enable_icestun);
        ret = tnet_ice_ctx_set_rtcpmux(self->ice.ctx_video, self->use_rtcpmux);
    }

    // set media type
    ret = tsip_dialog_invite_ice_set_media_type(self, media_type);

    // update session manager with the right ICE contexts
    if (self->msession_mgr) {
        ret = tmedia_session_mgr_set_ice_ctx(self->msession_mgr, self->ice.ctx_audio, self->ice.ctx_video);
    }

    return ret;
}

int tsip_dialog_invite_ice_set_media_type(tsip_dialog_invite_t * self, tmedia_type_t _media_type)
{
    if(self) {
        tmedia_type_t av_media_type = (_media_type & tmedia_audiovideo); // filter to keep audio and video only
        // "none" comparison is used to exclude the "first call"
        if(self->ice.media_type != tmedia_none && self->ice.media_type != av_media_type) {
            // cancels contexts associated to old medias
            if(self->ice.ctx_audio && !(av_media_type & tmedia_audio)) {
                tnet_ice_ctx_cancel(self->ice.ctx_audio);
            }
            if(self->ice.ctx_video && !(av_media_type & tmedia_video)) {
                tnet_ice_ctx_cancel(self->ice.ctx_video);
            }
            // cancels contexts associated to new medias (e.g. session "remove" then "add")
            // cancel() on newly created contexts don't have any effect
            if(self->ice.ctx_audio && (!(av_media_type & tmedia_audio) && (self->ice.media_type & tmedia_audio))) {
                //tnet_ice_ctx_cancel(self->ice.ctx_audio);
            }
            if(self->ice.ctx_video && (!(av_media_type & tmedia_video) && (self->ice.media_type & tmedia_video))) {
                //tnet_ice_ctx_cancel(self->ice.ctx_video);
            }
        }
        self->ice.media_type = av_media_type;
    }
    return 0;
}

static int tsip_dialog_invite_ice_start_ctx(tsip_dialog_invite_t * self)
{
    int ret = 0;
    if(self) {
        if((self->ice.media_type & tmedia_audio)) {
            if(self->ice.ctx_audio && (ret = tnet_ice_ctx_start(self->ice.ctx_audio)) != 0) {
                return ret;
            }
        }
        if((self->ice.media_type & tmedia_video)) {
            if(self->ice.ctx_video && (ret = tnet_ice_ctx_start(self->ice.ctx_video)) != 0) {
                return ret;
            }
        }
    }
    return 0;
}

static int tsip_dialog_invite_ice_cancel_ctx(tsip_dialog_invite_t * self)
{
    int ret = 0;
    if(self) {
        if((self->ice.media_type & tmedia_audio)) {
            if(self->ice.ctx_audio && (ret = tnet_ice_ctx_cancel(self->ice.ctx_audio)) != 0) {
                return ret;
            }
        }
        if((self->ice.media_type & tmedia_video)) {
            if(self->ice.ctx_video && (ret = tnet_ice_ctx_cancel(self->ice.ctx_video)) != 0) {
                return ret;
            }
        }
    }
    return 0;
}

static int tsip_dialog_invite_ice_set_sync_mode_ctx(tsip_dialog_invite_t * self, tsk_bool_t sync_mode)
{
    int ret = 0;
    if(self) {
        if((self->ice.media_type & tmedia_audio)) {
            if(self->ice.ctx_audio && (ret = tnet_ice_ctx_set_sync_mode(self->ice.ctx_audio, sync_mode)) != 0) {
                return ret;
            }
        }
        if((self->ice.media_type & tmedia_video)) {
            if(self->ice.ctx_video && (ret = tnet_ice_ctx_set_sync_mode(self->ice.ctx_video, sync_mode)) != 0) {
                return ret;
            }
        }
    }
    return 0;
}

static int tsip_dialog_invite_ice_set_silent_mode_ctx(tsip_dialog_invite_t * self, tsk_bool_t silent_mode)
{
    int ret = 0;
    if(self) {
        if((self->ice.media_type & tmedia_audio)) {
            if(self->ice.ctx_audio && (ret = tnet_ice_ctx_set_silent_mode(self->ice.ctx_audio, silent_mode)) != 0) {
                return ret;
            }
        }
        if((self->ice.media_type & tmedia_video)) {
            if(self->ice.ctx_video && (ret = tnet_ice_ctx_set_silent_mode(self->ice.ctx_video, silent_mode)) != 0) {
                return ret;
            }
        }
    }
    return 0;
}

tsk_bool_t tsip_dialog_invite_ice_is_enabled(const tsip_dialog_invite_t * self)
{
    if(self) {
        return (self->supported.ice && (tnet_ice_ctx_is_active(self->ice.ctx_audio) || tnet_ice_ctx_is_active(self->ice.ctx_video)));
    }
    return tsk_false;
}

tsk_bool_t tsip_dialog_invite_ice_is_connected(const tsip_dialog_invite_t * self)
{
    if(self) {
        return (!tnet_ice_ctx_is_active(self->ice.ctx_audio) || tnet_ice_ctx_is_connected(self->ice.ctx_audio))
               && (!tnet_ice_ctx_is_active(self->ice.ctx_video) || tnet_ice_ctx_is_connected(self->ice.ctx_video));
    }
    return tsk_false;
}

tsk_bool_t tsip_dialog_invite_ice_got_local_candidates(const tsip_dialog_invite_t * self)
{
    if(self) {
        return (!tnet_ice_ctx_is_active(self->ice.ctx_audio) || tnet_ice_ctx_got_local_candidates(self->ice.ctx_audio))
               && (!tnet_ice_ctx_is_active(self->ice.ctx_video) || tnet_ice_ctx_got_local_candidates(self->ice.ctx_video));
    }
    return tsk_false;
}

int tsip_dialog_invite_ice_save_action(tsip_dialog_invite_t * self, tsk_fsm_action_id action_id, const tsip_action_t* action, const tsip_message_t* message)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    // There are good reasons to ref() the action and message before safe_free()
    // /!\ do not change

    self->ice.last_action_id = action_id;
    action = tsk_object_ref((tsk_object_t*)action);
    TSK_OBJECT_SAFE_FREE(self->ice.last_action);
    self->ice.last_action = (tsip_action_t*)action;

    message = tsk_object_ref((tsk_object_t*)message);
    TSK_OBJECT_SAFE_FREE(self->ice.last_message);
    self->ice.last_message = (tsip_message_t*)message;
    return 0;
}

int tsip_dialog_invite_ice_process_lo(tsip_dialog_invite_t * self, const tsdp_message_t* sdp_lo)
{
    const tsdp_header_M_t* M;
    const tsdp_header_A_t *A;
    int ret = 0, i;

    if(!self || !sdp_lo) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    // cancels all ICE contexts without candidates
    // this happens if codecs negotiations mismatch for one media out of two or three
    for(i = 0; i < 2; ++i) {
        struct tnet_ice_ctx_s *ctx = i == 0 ? self->ice.ctx_audio : self->ice.ctx_video;
        const char* media = i == 0 ? "audio" : "video";
        if(tnet_ice_ctx_is_active(ctx)) {
            tsk_bool_t cancel = tsk_true;
            if((M = tsdp_message_find_media(sdp_lo, media))) {
                if((A = tsdp_header_M_findA(M, "candidate"))) {
                    cancel = tsk_false;
                }
            }
            if(cancel) {
                ret = tnet_ice_ctx_cancel(ctx);
            }
        }
    }

    return ret;
}

int tsip_dialog_invite_ice_process_ro(tsip_dialog_invite_t * self, const tsdp_message_t* sdp_ro, tsk_bool_t is_remote_offer)
{
    char* ice_remote_candidates;
    const tsdp_header_M_t* M;
    tsk_size_t index;
    const tsdp_header_A_t *A;
    const tsdp_header_O_t *O;
    const char* sess_ufrag = tsk_null;
    const char* sess_pwd = tsk_null;
    int ret = 0, i;
    struct tnet_ice_ctx_s *ctx;

    if(!self || !sdp_ro) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if(!self->ice.ctx_audio && !self->ice.ctx_video) {
        return 0;
    }

    // make sure this is different SDP
    if((O = (const tsdp_header_O_t*)tsdp_message_get_header(sdp_ro, tsdp_htype_O))) {
        if(self->ice.last_sdp_ro_ver == (int32_t)O->sess_version) {
            TSK_DEBUG_INFO("ICE: ignore processing SDP RO because version haven't changed");
            return 0;
        }
        self->ice.last_sdp_ro_ver = (int32_t)O->sess_version;
    }

    // session level attributes

    if((A = tsdp_message_get_headerA(sdp_ro, "ice-ufrag"))) {
        sess_ufrag = A->value;
    }
    if((A = tsdp_message_get_headerA(sdp_ro, "ice-pwd"))) {
        sess_pwd = A->value;
    }

#if 0 // Use RTCWeb Profile (tmedia_profile_rtcweb)
    {
        const tsdp_header_S_t *S;
        if((S = (const tsdp_header_S_t *)tsdp_message_get_header(sdp_ro, tsdp_htype_S)) && S->value) {
            self->ice.is_jingle = tsk_strcontains(S->value, tsk_strlen(S->value), "webrtc");
        }
    }
#endif

    for(i = 0; i < 2; ++i) {
        if((M = tsdp_message_find_media(sdp_ro, i==0 ? "audio": "video"))) {
            const char *ufrag = sess_ufrag, *pwd = sess_pwd;
            tsk_bool_t remote_use_rtcpmux = (tsdp_header_M_findA(M, "rtcp-mux") != tsk_null);
            ctx = (i==0 ? self->ice.ctx_audio : self->ice.ctx_video);
            ice_remote_candidates = tsk_null;
            index = 0;
            if((A = tsdp_header_M_findA(M, "ice-ufrag"))) {
                ufrag = A->value;
            }
            if((A = tsdp_header_M_findA(M, "ice-pwd"))) {
                pwd = A->value;
            }

            while((A = tsdp_header_M_findA_at(M, "candidate", index++))) {
                tsk_strcat_2(&ice_remote_candidates, "%s\r\n", A->value);
            }
            // ICE processing will be automatically stopped if the remote candidates are not valid
            // ICE-CONTROLLING role if we are the offerer
            ret = tnet_ice_ctx_set_remote_candidates_2(ctx, ice_remote_candidates, ufrag, pwd, !is_remote_offer, self->ice.is_jingle, (self->use_rtcpmux && remote_use_rtcpmux));
            TSK_SAFE_FREE(ice_remote_candidates);
        }
    }

    return ret;
}


//--------------------------------------------------------
//				== STATE MACHINE BEGIN ==
//--------------------------------------------------------


// Current -> (oINVITE) -> Current
static int x0500_Current_2_Current_X_oINVITE(va_list *app)
{
    int ret;
    tsip_dialog_invite_t *self;
    const tsip_action_t* action;
    const tsip_message_t *message;
    tmedia_type_t media_type;

    self = va_arg(*app, tsip_dialog_invite_t *);
    message = va_arg(*app, const tsip_message_t *);
    action = va_arg(*app, const tsip_action_t *);

    media_type = (action && action->media.type != tmedia_none) ? action->media.type : TSIP_DIALOG_GET_SS(self)->media.type;
    self->is_client = tsk_true;
    tsip_dialog_invite_ice_save_action(self, _fsm_action_oINVITE, action, message);

    // Cancel without notifying ("silent mode") and perform the operation right now ("sync mode")
    tsip_dialog_invite_ice_cancel_silent_and_sync_ctx(self);

    // create ICE context
    if((ret = tsip_dialog_invite_ice_create_ctx(self, media_type))) {
        TSK_DEBUG_ERROR("tsip_dialog_invite_ice_create_ctx() failed");
        return ret;
    }

    // For now disable ICE timers until we receive the 2xx
    ret = tsip_dialog_invite_ice_timers_set(self, -1);

    // Start ICE
    ret = tsip_dialog_invite_ice_start_ctx(self);

    // alert the user only if we are in initial state which means that it's not media update
    if(TSIP_DIALOG(self)->state == tsip_initial) {
        TSIP_DIALOG_SIGNAL(self, tsip_event_code_dialog_connecting, "Dialog connecting");
    }

    return ret;
}

// Current -> (iINVITE) -> Current
static int x0500_Current_2_Current_X_iINVITE(va_list *app)
{
    int ret;
    tsip_dialog_invite_t *self;
    const tsip_action_t* action;
    const tsip_message_t *message;

    self = va_arg(*app, tsip_dialog_invite_t *);
    message = va_arg(*app, const tsip_message_t *);
    action = va_arg(*app, const tsip_action_t *);

    self->is_client = tsk_false;
    ret = tsip_dialog_invite_ice_save_action(self, _fsm_action_iINVITE, action, message);

    // Cancel without notifying ("silent mode") and perform the operation right now ("sync mode")
    tsip_dialog_invite_ice_cancel_silent_and_sync_ctx(self);

    // set remote candidates
    if(TSIP_MESSAGE_HAS_CONTENT(message)) {
        if(tsk_striequals("application/sdp", TSIP_MESSAGE_CONTENT_TYPE(message))) {
            tsdp_message_t* sdp_ro;
            if(!(sdp_ro = tsdp_message_parse(TSIP_MESSAGE_CONTENT_DATA(message), TSIP_MESSAGE_CONTENT_DATA_LENGTH(message)))) {
                TSK_DEBUG_ERROR("Failed to parse remote sdp message");
                return -2;
            }
            // create ICE context
            if((ret = tsip_dialog_invite_ice_create_ctx(self, tmedia_type_from_sdp(sdp_ro)))) {
                TSK_DEBUG_ERROR("tsip_dialog_invite_ice_create_ctx() failed");
                return ret;
            }
            ret = tsip_dialog_invite_ice_process_ro(self, sdp_ro, tsk_true);
            TSK_OBJECT_SAFE_FREE(sdp_ro);
        }
        else {
            TSK_DEBUG_ERROR("[%s] content-type is not supportted", TSIP_MESSAGE_CONTENT_TYPE(message));
            return -3;
        }
    }

    // For now disable ICE timers until we send the 2xx and receive the ACK
    ret = tsip_dialog_invite_ice_timers_set(self, -1);

    // Start ICE
    ret = tsip_dialog_invite_ice_start_ctx(self);

    return ret;
}




//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//				== STATE MACHINE END ==
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++

static int tsip_dialog_invite_ice_callback(const tnet_ice_event_t *e)
{
    int ret = 0;
    tsip_dialog_invite_t *dialog;

    TSK_DEBUG_INFO("ICE callback: %s", e->phrase);

    dialog = tsk_object_ref(TSK_OBJECT(e->userdata));

    // Do not lock: caller is thread safe

    switch(e->type) {
    case tnet_ice_event_type_gathering_completed:
    case tnet_ice_event_type_conncheck_succeed:
    case tnet_ice_event_type_conncheck_failed:
    case tnet_ice_event_type_cancelled: {
        if(dialog->ice.last_action_id != tsk_fsm_state_none) {
            if(tsip_dialog_invite_ice_got_local_candidates(dialog)) {
                ret = tsip_dialog_fsm_act(TSIP_DIALOG(dialog), dialog->ice.last_action_id, dialog->ice.last_message, dialog->ice.last_action);
                dialog->ice.last_action_id = tsk_fsm_state_none;
            }
        }
        if(dialog->ice.start_smgr) {
            ret = tsip_dialog_invite_msession_start(dialog);
        }
        break;
    }
    // fatal errors which discard ICE process
    case tnet_ice_event_type_gathering_host_candidates_failed:
    case tnet_ice_event_type_gathering_reflexive_candidates_failed:
    case tnet_ice_event_type_gathering_relay_candidates_failed: {
        if (dialog->ice.last_action_id != tsk_fsm_state_none) {
            ret = tsip_dialog_fsm_act(TSIP_DIALOG(dialog), dialog->ice.last_action_id, dialog->ice.last_message, dialog->ice.last_action);
            dialog->ice.last_action_id = tsk_fsm_state_none;
        }
        break;
    }
    // TURN session disconnected while we're in call
    case tnet_ice_event_type_turn_connection_broken: {
        ret = tsip_dialog_fsm_act_2(TSIP_DIALOG(dialog), _fsm_action_oBYE);
        break;
    }
    default:
        break;
    }

    TSK_OBJECT_SAFE_FREE(dialog);

    return ret;
}

static int tsip_dialog_invite_ice_audio_callback(const tnet_ice_event_t *e)
{
    return tsip_dialog_invite_ice_callback(e);
}

static int tsip_dialog_invite_ice_video_callback(const tnet_ice_event_t *e)
{
    return tsip_dialog_invite_ice_callback(e);
}