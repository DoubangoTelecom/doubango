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

/**@file tsip_ssession.c
 * @brief SIP session.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/tsip_ssession.h"

#include "tinysip/tsip_action.h"
#include "tsip.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tinysip/dialogs/tsip_dialog_layer.h"
#include "tinysip/tsip_message.h"

#include "tinymedia/tmedia_defaults.h"

#include "tsk_debug.h"

/**@defgroup tsip_session_group SIP sessions
*/

/* internal function used to create session for server dialogs */
tsip_ssession_handle_t* tsip_ssession_create_2(const tsip_stack_t* stack, const struct tsip_message_s* message)
{
    tsip_ssession_t* ss = tsk_null;

    if(message) {
        char *from = tsk_null, *to = tsk_null;

        /* From: */
        if(message->From && message->From->uri) { /* MUST be not null */
            from = tsip_uri_tostring(message->From->uri, tsk_false, tsk_false);
        }
        /* To: */
        if(message->To && message->To->uri) { /* MUST be not null */
            to = tsip_uri_tostring(message->To->uri, tsk_false, tsk_false);
        }
        /* create the "server-side-session" */
        if((ss = tsip_ssession_create((tsip_stack_handle_t*)stack, TSIP_SSESSION_SET_NULL()))) {
            tsip_ssession_set(ss,
                              /* default values should be in conformance with the swig wrapper */
                              TSIP_SSESSION_SET_FROM_STR(from),
                              TSIP_SSESSION_SET_TO_STR(to),
                              TSIP_SSESSION_SET_NULL());
        }

        /* in all cases */
        TSK_FREE(from);
        TSK_FREE(to);
    }

    /* as the it's a "server-side-session", you are not the owner
    * The end-user should call tsip_ssession_have_ownership() to check whether he has the ownership.
    * The end-user should also call tsip_ssession_take_ownership() to take the ownership. This will avoid the session to be deleted by the stack
    * when the associated dialog ends. */
    if(ss) {
        ss->owner = tsk_false;
    }

    return ss;
}

int __tsip_ssession_set_To(tsip_ssession_t *self, const char* value)
{
    tsip_uri_t* uri;
    if(value && (uri = tsip_uri_parse(value, tsk_strlen(value)))) {
        TSK_OBJECT_SAFE_FREE(self->to);
        self->to = uri;
        return 0;
    }
    else {
        TSK_DEBUG_ERROR("%s is invalid as 'To' header value", value);
        return -1;
    }
}

int __tsip_ssession_set_From(tsip_ssession_t *self, const char* value)
{
    tsip_uri_t* uri;
    if(value && (uri = tsip_uri_parse(value, tsk_strlen(value)))) {
        TSK_OBJECT_SAFE_FREE(self->from);
        self->from = uri;
        return 0;
    }
    else {
        TSK_DEBUG_ERROR("%s is invalid as 'From' header value", value);
        return -1;
    }
}

int __tsip_ssession_set(tsip_ssession_t *self, va_list *app)
{
    tsip_ssession_param_type_t sscurr;
    tsip_msession_param_type_t mscurr;
    tmedia_session_mgr_t* mgr = tsk_null;

    int ret = 0;

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    while((sscurr = va_arg(*app, tsip_ssession_param_type_t)) != sstype_null) {
        switch(sscurr) {
            //=======
            //	Sip
            //=======
        case sstype_header:
        case sstype_caps: {
            /* (const char*)NAME_STR, (const char*)VALUE_STR */
            const char* name = va_arg(*app, const char *);
            const char* value = va_arg(*app, const char *);

            if(sscurr == sstype_header) {
                /* whether to SET or UNSET the header */
                if(value == ((const char*)-1)) {
                    tsk_params_remove_param(self->headers, name);
                    break;
                }

                /* From */
                if(value && tsk_striequals(name, "From")) {
                    if((ret = __tsip_ssession_set_From(self, value))) {
                        goto bail;
                    }
                }
                /* To */
                else if(value && tsk_striequals(name, "To")) {
                    if((ret = __tsip_ssession_set_To(self, value))) {
                        goto bail;
                    }
                }
                /* Expires */
                else if(value && tsk_striequals(name, "Expires")) {
                    /* should never happen ==> ...but who know? */
                }
                /* Any other */
                else {
                    tsk_params_add_param(&self->headers, name, value);
                }
            }
            else if(sscurr == sstype_caps) {
                if(value == ((const char*)-1)) { /* UNSET */
                    tsk_params_remove_param(self->caps, name);
                }
                else { /* SET */
                    tsk_params_add_param(&self->caps, name, value);
                }
            }
            break;
        }
        case sstype_userdata: {
            /* (const void*)DATA_PTR */
            self->userdata = va_arg(*app, const void *);
            break;
        }
        case sstype_to_str: {
            /* (const char*)URI_STR */
            if((ret = __tsip_ssession_set_To(self, va_arg(*app, const char *)))) {
                goto bail;
            }
            break;
        }
        case sstype_from_str: {
            /* (const char*)URI_STR*/
            if((ret = __tsip_ssession_set_From(self, va_arg(*app, const char *)))) {
                goto bail;
            }
            break;
        }
        case sstype_to_obj: {
            /* (const tsip_uri_t*)URI_OBJ */
            const tsip_uri_t* URI_OBJ = va_arg(*app, const tsip_uri_t *);
            if(URI_OBJ) {
                TSK_OBJECT_SAFE_FREE(self->to);
                self->to = tsk_object_ref((void*)URI_OBJ);
            }
            break;
        }
        case sstype_from_obj: {
            /* (const char*)URI_OBJ*/
            const tsip_uri_t* URI_OBJ = va_arg(*app, const tsip_uri_t *);
            if(URI_OBJ) {
                TSK_OBJECT_SAFE_FREE(self->from);
                self->from = tsk_object_ref((void*)URI_OBJ);
            }
            break;
        }
        case sstype_nocontact: {
            /* (tsk_bool_t)ENABLED_BOOL */
            self->no_contact = va_arg(*app, tsk_bool_t);
            break;
        }
        case sstype_expires: {
            /* (unsigned)VALUE_UINT */
            self->expires = (((int64_t)va_arg(*app, unsigned)) * 1000) /* milliseconds */;
            break;
        }
        case sstype_silent_hangup: {
            /* sstype_silent_hangup, (tsk_bool_t)ENABLED_BOOL */
            self->silent_hangup = va_arg(*app, tsk_bool_t);
            break;
        }
        case sstype_sigcomp_id: {
            /* (const char*)COMPARTMENT_ID_STR */
            const char* COMPARTMENT_ID_STR = va_arg(*app, const char*);
            if(COMPARTMENT_ID_STR == (const char*)-1) {
                TSK_FREE(self->sigcomp_id);
            }
            else {
                tsk_strupdate(&self->sigcomp_id, COMPARTMENT_ID_STR);
            }
            break;
        }
        case sstype_auth_ha1: {
            /* (const char*)AUTH_HA1_STR */
            const char* AUTH_HA1_STR = va_arg(*app, const char*);
            tsk_strupdate(&self->auth_ha1, AUTH_HA1_STR);
            break;
        }
        case sstype_auth_impi: {
            /* (const char*)AUTH_IMPI_STR */
            const char* AUTH_IMPI_STR = va_arg(*app, const char*);
            tsk_strupdate(&self->auth_impi, AUTH_IMPI_STR);
            break;
        }
        case sstype_parent_id: {
            /* ((tsip_ssession_id_t)PARENT_ID_SSID) */
            self->id_parent = va_arg(*app, tsip_ssession_id_t);
            break;
        }
        case sstype_ws_src: {
            /* (const char*)SRC_HOST_STR, (int32_t)SRC_PORT_INT, (const char*)SRC_PROTO_STR */
            const char* SRC_HOST_STR = va_arg(*app, const char*);
            int32_t SRC_PORT_INT = va_arg(*app, int32_t);
            const char* SRC_PROTO_STR = va_arg(*app, const char*);
            tsk_strupdate(&self->ws.src.host, SRC_HOST_STR);
            tsk_itoa(SRC_PORT_INT, &self->ws.src.port);
            tsk_strupdate(&self->ws.src.proto, SRC_PROTO_STR);
            break;
        }
        case sstype_media: {
            //=========
            //	Media
            //=========
            if (!mgr) {
                mgr = tsip_session_get_mediamgr(self);
            }
            while((mscurr = va_arg(*app, tsip_msession_param_type_t)) != mstype_null) {
                switch(mscurr) {
                case mstype_set_profile:
                    // (tmedia_profile_t)PROFILE_ENUM
                    self->media.profile = va_arg(*app, tmedia_profile_t);
                    break;
                case mstype_set_srtp_mode:
                    // (tmedia_srtp_mode_t)SRTP_MODE_ENUM
                    self->media.srtp_mode = va_arg(*app, tmedia_srtp_mode_t);
                    break;
                case mstype_set_avpf_mode:
                    // (tmedia_mode_t)MEDIA_MODE_ENUM
                    self->media.avpf_mode = va_arg(*app, tmedia_mode_t);
                    break;
                case mstype_set_100rel:
                    self->media.enable_100rel = va_arg(*app, tsk_bool_t);
                    break;
                case mstype_set_ice:
                    self->media.enable_ice = va_arg(*app, tsk_bool_t);
                    break;
                case mstype_set_ice_stun:
                    self->media.enable_icestun = va_arg(*app, tsk_bool_t);
                    break;
                case mstype_set_ice_turn:
                    self->media.enable_iceturn = va_arg(*app, tsk_bool_t);
                    break;
                case mstype_set_rtcp:
                    self->media.enable_rtcp = va_arg(*app, tsk_bool_t);
                    break;
                case mstype_set_rtcpmux:
                    self->media.enable_rtcpmux = va_arg(*app, tsk_bool_t);
                    break;
                case mstype_set_qos: {
                    /* (tmedia_qos_stype_t)TYPE_ENUM, (tmedia_qos_strength_t)STRENGTH_ENUM */
                    self->media.qos.type = va_arg(*app, tmedia_qos_stype_t);
                    self->media.qos.strength = va_arg(*app, tmedia_qos_strength_t);
                    break;
                }
                case mstype_unset_qos: {
                    /*  */
                    self->media.qos.type = tmedia_qos_stype_none;
                    self->media.qos.strength = tmedia_qos_strength_none;
                    break;
                }
                case mstype_set_timers: {
                    /* (unsigned)TIMEOUT_UINT, (const char*)REFRESHER_STR */
                    /* set values */
                    self->media.timers.timeout = va_arg(*app, unsigned);
                    tsk_strupdate(&self->media.timers.refresher, va_arg(*app, const char*));
                    break;
                }
                case mstype_unset_timers: {
                    /*  */
                    /* unset values */
                    self->media.timers.timeout = 0;
                    TSK_FREE(self->media.timers.refresher);
                    break;
                }
                case mstype_set_codecs: {
                    /* (signed)CODECS_INT */
                    self->media.codecs = va_arg(*app, signed);
                    if(mgr) { // apply now
                        tmedia_session_mgr_set_codecs_supported(mgr, self->media.codecs);
                    }
                    break;
                }
                case mstype_set_bypass_encoding: {
                    /* (tsk_bool_t)ENABLED_BOOL */
                    self->media.bypass_encoding = va_arg(*app, tsk_bool_t);
                    break;
                }
                case mstype_set_bypass_decoding: {
                    /* (tsk_bool_t)ENABLED_BOOL */
                    self->media.bypass_decoding = va_arg(*app, tsk_bool_t);
                    break;
                }
                case mstype_set_rtp_ssrc: {
                    /* (tmedia_type_t)MEDIA_ENUM, (uint32_t)SSRC_UINT */
                    tmedia_type_t MEDIA_ENUM = va_arg(*app, tmedia_type_t);
                    uint32_t SSRC_UINT = va_arg(*app, uint32_t);
                    switch(MEDIA_ENUM) {
                    case tmedia_audio:
                        self->media.rtp.ssrc.audio = SSRC_UINT;
                        break;
                    case tmedia_video:
                        self->media.rtp.ssrc.video = SSRC_UINT;
                        break;
                    default:
                        break;
                    }
                    break;
                }
                case mstype_set_msrp_cb: {
                    /* (tmedia_session_msrp_cb_f)TMEDIA_SESSION_MSRP_CB_F */
                    self->media.msrp.callback = va_arg(*app, tmedia_session_msrp_cb_f);
                    break;
                }
                case mstype_set_stun_server: {
                    /* (const char*)HOSTNAME, (uint16_t)PORT */
                    const char* HOSTNAME = va_arg(*app, const char*);
                    uint16_t PORT = tsk_va_arg_u16(*app);
                    tsk_strupdate(&self->media.stun.hostname, HOSTNAME);
                    self->media.stun.port = PORT;
                    break;
                }
                case mstype_set_stun_cred: {
                    /* (const char*)USERNAME, (const char*)PASSWORD */
                    const char* USERNAME = va_arg(*app, const char*);
                    const char* PASSWORD = va_arg(*app, const char*);
                    tsk_strupdate(&self->media.stun.username, USERNAME);
                    tsk_strupdate(&self->media.stun.password, PASSWORD);
                    break;
                }
                case mstype_set_video_fps: {
                    /* (signed)FPS_INT */
                    self->media.video_fps = va_arg(*app, signed);
                    break;
                }
                case mstype_set_video_bw_up: {
                    /* (signed)BW_INT */
                    self->media.video_bw_up = va_arg(*app, signed);
                    break;
                }
                case mstype_set_video_bw_down: {
                    /* (signed)BW_INT */
                    self->media.video_bw_down = va_arg(*app, signed);
                    break;
                }
                case mstype_set_video_prefsize: {
                    /* (tmedia_pref_video_size_t)PREFSIZE_ENUM */
                    self->media.video_pref_size = va_arg(*app, tmedia_pref_video_size_t);
                    break;
                }
                default: {
                    /* va_list will be unsafe => exit */
                    TSK_DEBUG_ERROR("%d NOT a valid MEDIA pname", mscurr);
                    goto bail;
                }
                } /* switch */
            } /* while */

            break;
        } /* case */

        default: {
            /* va_list will be unsafe => exit */
            TSK_DEBUG_ERROR("%d NOT a valid SIP pname", sscurr);
            goto bail;
        }

        } /* switch */
    } /* while */

bail:
    TSK_OBJECT_SAFE_FREE(mgr);

    return ret;
}


tsip_ssession_handle_t* tsip_ssession_create(tsip_stack_handle_t *stack, ...)
{
    tsip_ssession_t* ss = tsk_null;
    va_list ap;
    tsip_stack_t* _stack = stack;

    if(!_stack) {
        TSK_DEBUG_ERROR("Invalid Parameter.");
        goto bail;
    }

    if(!(ss = tsk_object_new(tsip_ssession_def_t, stack))) {
        TSK_DEBUG_ERROR("Failed to create new SIP Session.");
        return tsk_null;
    }

    va_start(ap, stack);
    if(__tsip_ssession_set(ss, &ap)) {
        TSK_DEBUG_ERROR("Failed to set user's parameters.");
        TSK_OBJECT_SAFE_FREE(ss);
        va_end(ap);
        goto bail;
    }
    va_end(ap);

    /* from */
    if(!ss->from && _stack->identity.impu) {
        ss->from = tsip_uri_clone(_stack->identity.impu, tsk_false, tsk_false);
    }
    /* to */
    /* To value will be set by the dialog (whether to use as Request-URI). */

bail:
    return ss;
}

int tsip_ssession_set(tsip_ssession_handle_t *self, ...)
{
    if(self) {
        int ret;
        va_list ap;

        tsip_ssession_t *ss = self;

        if(ss->id == TSIP_SSESSION_INVALID_ID) {
            return -2;
        }

        va_start(ap, self);
        ret = __tsip_ssession_set(ss, &ap);
        va_end(ap);
        return ret;
    }
    else {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
}

tsip_ssession_id_t tsip_ssession_get_id(const tsip_ssession_handle_t *self)
{
    if(self) {
        const tsip_ssession_t *ss = self;
        return ss->id;
    }
    return TSIP_SSESSION_INVALID_ID;
}

tsip_ssession_id_t tsip_ssession_get_id_parent(const tsip_ssession_handle_t *self)
{
    if(self) {
        const tsip_ssession_t *ss = self;
        return ss->id_parent;
    }
    return TSIP_SSESSION_INVALID_ID;
}

int tsip_ssession_take_ownership(tsip_ssession_handle_t *self)
{
    if(self) {
        tsip_ssession_t *ss = self;
        if(!ss->owner) {
            ss->owner = tsk_true;
            /* before: only the dialog had a reference to the SIP session */
            ss = tsk_object_ref(ss);
            /* after: both the end-user and the dialog have their references */
            return 0;
        }
        return -2;
    }
    else {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
}

tsk_bool_t tsip_ssession_have_ownership(const tsip_ssession_handle_t *self)
{
    if(self) {
        const tsip_ssession_t *ss = self;
        return ss->owner;
    }
    return tsk_false;
}

int tsip_ssession_respond(const tsip_ssession_handle_t *self, short status, const char* phrase, const void* payload, tsk_size_t size, const struct tsip_message_s* request, ...)
{
    tsip_response_t *response = tsk_null;
    tsip_dialog_t* dialog = tsk_null;
    const tsip_ssession_t *ss = self;
    int ret = -1;

    if(!ss || !request) {
        goto bail;
    }

    if(!(dialog = tsip_dialog_layer_find_by_ss(ss->stack->layer_dialog, ss))) {
        goto bail;
    }

    if(!(response = tsip_dialog_response_new(TSIP_DIALOG(self), status, phrase, request))) {
        goto bail;
    }

    if(payload && size) {
        if((ret = tsip_message_add_content(response, tsk_null, payload, size))) {
            goto bail;
        }
    }
    ret = tsip_dialog_response_send(TSIP_DIALOG(self), response);

bail:
    TSK_OBJECT_SAFE_FREE(response);
    TSK_OBJECT_SAFE_FREE(dialog);

    return ret;
}

const void* tsip_ssession_get_userdata(const tsip_ssession_handle_t *self)
{
    const tsip_ssession_t* ss = (const tsip_ssession_t*)self;
    if(ss) {
        return ss->userdata;
    }
    else {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }
}

tmedia_type_t tsip_ssession_get_mediatype(const tsip_ssession_handle_t *self)
{
    if(self) {
        return ((const tsip_ssession_t*)self)->media.type;
    }
    else {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tmedia_none;
    }
}

#include "tinysip/dialogs/tsip_dialog_invite.h"
tmedia_session_mgr_t* tsip_session_get_mediamgr(const tsip_ssession_handle_t *self)
{
    tmedia_session_mgr_t* mgr = tsk_null;

    if(self) {
        const tsip_ssession_t *ss = self;
        tsip_dialog_t* dialog;

        if((dialog = tsip_dialog_layer_find_by_ss(ss->stack->layer_dialog, self))) {
            if(dialog->type == tsip_dialog_INVITE) {
                mgr = tsk_object_ref(TSIP_DIALOG_INVITE(dialog)->msession_mgr);
            }
            tsk_object_unref(dialog);
        }
    }
    else {
        TSK_DEBUG_ERROR("Invalid parameter");
    }

    return mgr;
}

const tsip_stack_handle_t* tsip_ssession_get_stack(const tsip_ssession_handle_t *self)
{
    if(self) {
        return ((const tsip_ssession_t*)self)->stack;
    }
    else {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }
}

tmedia_codec_id_t tsip_ssession_get_codecs_neg(tsip_ssession_handle_t *self)
{
    int32_t codecs_neg = (int32_t)tmedia_codec_id_none;
    if(self) {
        tmedia_session_mgr_t* mgr = tsip_session_get_mediamgr(self);
        if(mgr) {
            (tmedia_session_mgr_get(mgr,
                                    TMEDIA_SESSION_GET_INT32(mgr->type, "codecs-negotiated", &codecs_neg),
                                    TMEDIA_SESSION_GET_NULL()));
            TSK_OBJECT_SAFE_FREE(mgr);
        }
    }
    return (tmedia_codec_id_t)codecs_neg;
}

int tsip_ssession_handle(const tsip_ssession_t *self, const struct tsip_action_s* action)
{
    int ret = -1;

    if(self && self->stack && action) {
        tsip_dialog_t *dialog;

        if((dialog = tsip_dialog_layer_find_by_ss(self->stack->layer_dialog, self))) {
            switch(action->type) {
            case tsip_atype_hangup: {
                /* hang-up is an special case (==> hangup/cancel/nothing) */
                ret = tsip_dialog_hangup(dialog, action);
                break;
            }
            default: {
                /* All other cases */
                ret = tsip_dialog_fsm_act(dialog, action->type, tsk_null, action);
                break;
            }
            }
            /* unref */
            tsk_object_unref(dialog);
        }
        else {
            TSK_DEBUG_ERROR("Failed to find dialog with this opid [%lld]", self->id);
        }
    }
    else {
        TSK_DEBUG_ERROR("Invalid parameter");
    }

    return ret;
}





//========================================================
//	SIP Session object definition
//
static tsk_object_t* tsip_ssession_ctor(tsk_object_t * self, va_list * app)
{
    tsip_ssession_t *ss = self;
    static tsip_ssession_id_t unique_id = 0;
    if(ss) {
        ss->stack = va_arg(*app, const tsip_stack_t*);
        ss->caps = tsk_list_create();
        ss->headers = tsk_list_create();

        /* unique identifier */
        ss->id = ++unique_id;
        // default: you are the owner
        ss->owner = tsk_true;
        // default expires value
        ss->expires = TSIP_SSESSION_EXPIRES_DEFAULT;
        // default parentid: not parent -> no pending transfer
        ss->id_parent = TSIP_SSESSION_INVALID_ID;
        // default SigComp compId (will be updated by session_set())
        if (ss->stack->sigcomp.handle) {
            ss->sigcomp_id = tsk_strdup(tsip_sigcomp_handler_fixme_getcompid(ss->stack->sigcomp.handle));
        }
        // default media values
        ss->media.profile = tmedia_defaults_get_profile();
        ss->media.srtp_mode = tmedia_defaults_get_srtp_mode();
        ss->media.avpf_mode = tmedia_defaults_get_avpf_mode();
        ss->media.enable_conditional_ringing = tmedia_defaults_get_conditional_ringing_enabled();
        ss->media.enable_100rel = tmedia_defaults_get_100rel_enabled();
        ss->media.enable_ice = tmedia_defaults_get_ice_enabled();
        ss->media.enable_icestun = tmedia_defaults_get_icestun_enabled();
        ss->media.enable_iceturn = tmedia_defaults_get_iceturn_enabled();
        ss->media.enable_rtcp = tmedia_defaults_get_rtcp_enabled();
        ss->media.enable_rtcpmux = tmedia_defaults_get_rtcpmux_enabled();
        ss->media.type = tmedia_none;
        ss->media.qos.type = tmedia_qos_stype_none;
        ss->media.qos.strength = tmedia_qos_strength_none;
        ss->media.timers.refresher = tsk_strdup(tmedia_defaults_get_inv_session_refresher());
        ss->media.timers.timeout = tmedia_defaults_get_inv_session_expires();
        ss->media.codecs = tmedia_codec_id_all;
        ss->media.bypass_encoding = tmedia_defaults_get_bypass_encoding();
        ss->media.bypass_decoding = tmedia_defaults_get_bypass_decoding();
        ss->media.video_fps = tmedia_defaults_get_video_fps();
        ss->media.video_bw_down = tmedia_defaults_get_bandwidth_video_download_max();
        ss->media.video_bw_up = tmedia_defaults_get_bandwidth_video_upload_max();
        ss->media.video_pref_size = tmedia_defaults_get_pref_video_size();
        {
            const char *stun_hostname, *stun_username, *stun_password;
            uint16_t stun_port;
            if(tmedia_defaults_get_stun_server(&stun_hostname, &stun_port) == 0) {
                ss->media.stun.hostname = tsk_strdup(stun_hostname);
                ss->media.stun.port = stun_port;
            }
            if(tmedia_defaults_get_stun_cred(&stun_username, &stun_password) == 0) {
                ss->media.stun.username = tsk_strdup(stun_username);
                ss->media.stun.password = tsk_strdup(stun_password);
            }
        }

        /* add the session to the stack */
        if(ss->stack) {
            tsk_list_push_back_data(ss->stack->ssessions, (void**)&ss);
        }
    }

    return self;
}

static tsk_object_t* tsip_ssession_dtor(tsk_object_t * self)
{
    tsip_ssession_t *ss = self;
    if(ss) {
        /* remove from the stack */
        if(ss->stack) {
            tsk_list_remove_item_by_data(ss->stack->ssessions, ss);
        }

        //=======
        // SIP
        //=======
        TSK_OBJECT_SAFE_FREE(ss->caps);
        TSK_OBJECT_SAFE_FREE(ss->headers);

        TSK_OBJECT_SAFE_FREE(ss->from);
        TSK_OBJECT_SAFE_FREE(ss->to);

        TSK_FREE(ss->sigcomp_id);
        TSK_FREE(ss->auth_ha1);
        TSK_FREE(ss->auth_impi);

        //=======
        // Media
        //=======
        TSK_FREE(ss->media.timers.refresher);
        TSK_FREE(ss->media.stun.username);
        TSK_FREE(ss->media.stun.password);
        TSK_FREE(ss->media.stun.hostname);

        //=======
        // WebSocket
        //=======
        TSK_FREE(ss->ws.src.host);
        TSK_FREE(ss->ws.src.proto);

        TSK_DEBUG_INFO("*** SIP Session destroyed ***");
    }
    return self;
}

static int tsip_ssession_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
    const tsip_ssession_t *ss1 = obj1;
    const tsip_ssession_t *ss2 = obj2;

    if(ss1 && ss2) {
        return (int)(ss1->id-ss2->id);
    }
    return -1;
}

static const tsk_object_def_t tsip_ssession_def_s = {
    sizeof(tsip_ssession_t),
    tsip_ssession_ctor,
    tsip_ssession_dtor,
    tsip_ssession_cmp,
};
const tsk_object_def_t *tsip_ssession_def_t = &tsip_ssession_def_s;
