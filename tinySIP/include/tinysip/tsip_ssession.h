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

/**@file tsip_ssession.h
 * @brief SIP ssession.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef TSIP_SSESSION_H
#define TSIP_SSESSION_H

#include "tinysip_config.h"

#include "tinymedia/tmedia_common.h"
#include "tinymedia/tmedia_session.h"

#include "tsk_object.h"
#include "tsk_list.h"
#include "tsk_params.h"
#include "tsk_options.h"

TSIP_BEGIN_DECLS

// Forward declarations
struct tsip_message_s;
struct tsip_action_s;

typedef uint64_t tsip_ssession_id_t;
#define TSIP_SSESSION_INVALID_ID				0
#define TSIP_SSESSION_INVALID_HANDLE			tsk_null

#if defined(DEBUG) || defined(_DEBUG)
#	define TSIP_SSESSION_EXPIRES_DEFAULT		3600000 /* miliseconds. */
#else
#	define TSIP_SSESSION_EXPIRES_DEFAULT		600000000
#endif


#define TSIP_SSESSION(self) ((tsip_ssession_t*)(self))

typedef enum tsip_ssession_param_type_e {
    sstype_null = 0,

    sstype_header,
    sstype_caps,
    sstype_userdata,
    sstype_to_str,
    sstype_from_str,
    sstype_to_obj,
    sstype_from_obj,
    sstype_nocontact,
    sstype_expires,
    sstype_silent_hangup,
    sstype_sigcomp_id,
    sstype_auth_ha1,
    sstype_auth_impi,
    sstype_parent_id,
    sstype_ws_src,
    sstype_media
}
tsip_ssession_param_type_t;

#define TSIP_SSESSION_SET_PARENT_ID(PARENT_ID_SSID)										sstype_parent_id, ((tsip_ssession_id_t)PARENT_ID_SSID)
#define TSIP_SSESSION_SET_HEADER(NAME_STR, VALUE_STR)									sstype_header, (const char*)NAME_STR, (const char*)VALUE_STR
#define TSIP_SSESSION_UNSET_HEADER(NAME_STR)											TSIP_SSESSION_SET_HEADER(NAME_STR, (const char*)-1)
#define TSIP_SSESSION_SET_CAPS(NAME_STR, VALUE_STR)										sstype_caps, (const char*)NAME_STR, (const char*)VALUE_STR /* RFC 3840 */
#define TSIP_SSESSION_UNSET_CAPS(NAME_STR)												TSIP_SSESSION_SET_CAPS(NAME_STR, (const char*)-1)
#define TSIP_SSESSION_SET_USERDATA(DATA_PTR)											sstype_userdata, (const void*)DATA_PTR
#define TSIP_SSESSION_SET_TO_STR(URI_STR)												sstype_to_str, (const char*)URI_STR
#define TSIP_SSESSION_SET_FROM_STR(URI_STR)												sstype_from_str, (const char*)URI_STR
#define TSIP_SSESSION_SET_TO(URI_STR)													TSIP_SSESSION_SET_TO_STR(URI_STR)
#define TSIP_SSESSION_SET_FROM(URI_STR)													TSIP_SSESSION_SET_FROM_STR(URI_STR)
#define TSIP_SSESSION_SET_TO_OBJ(URI_OBJ)												sstype_to_obj, (const tsip_uri_t*)URI_OBJ
#define TSIP_SSESSION_SET_FROM_OBJ(URI_OBJ)												sstype_from_obj, (const tsip_uri_t*)URI_OBJ
#define TSIP_SSESSION_SET_NO_CONTACT(ENABLED_BOOL)										sstype_nocontact, (tsk_bool_t)ENABLED_BOOL
#define TSIP_SSESSION_SET_EXPIRES(VALUE_UINT)											sstype_expires, (unsigned)VALUE_UINT
#define TSIP_SSESSION_SET_SILENT_HANGUP(ENABLED_BOOL)									sstype_silent_hangup, (tsk_bool_t)ENABLED_BOOL
#define TSIP_SSESSION_SET_SIGCOMP_COMPARTMENT(COMPARTMENT_ID_STR)						sstype_sigcomp_id, (const char*)COMPARTMENT_ID_STR
#define TSIP_SSESSION_UNSET_SIGCOMP_COMPARTMENT()										TSIP_SSESSION_SET_SIGCOMP_COMPARTMENT((const char*)-1)
#define TSIP_SSESSION_SET_AUTH_HA1(AUTH_HA1_STR)										sstype_auth_ha1, (const char*)AUTH_HA1_STR
#define TSIP_SSESSION_SET_AUTH_IMPI(AUTH_IMPI_STR)										sstype_auth_impi, (const char*)AUTH_IMPI_STR
#define TSIP_SSESSION_SET_WEBSOCKET_SRC(SRC_HOST_STR, SRC_PORT_INT, SRC_PROTO_STR)		sstype_ws_src, (const char*)SRC_HOST_STR, (int32_t)SRC_PORT_INT, (const char*)SRC_PROTO_STR
#define TSIP_SSESSION_SET_MEDIA(...)													sstype_media, ##__VA_ARGS__
#define TSIP_SSESSION_SET_NULL()														sstype_null

typedef enum tsip_msession_param_type_e {
    mstype_null = 0,

    mstype_set_profile,
    mstype_set_srtp_mode,
    mstype_set_avpf_mode,

    mstype_set_100rel,
    mstype_set_rtcp,
    mstype_set_rtcpmux,
    mstype_set_ice,
    mstype_set_ice_stun,
    mstype_set_ice_turn,
    mstype_set_stun_server,
    mstype_set_stun_cred,

    mstype_set_video_fps,
    mstype_set_video_bw_up,
    mstype_set_video_bw_down,
    mstype_set_video_prefsize,

    mstype_set_qos,
    mstype_unset_qos,

    mstype_set_timers,
    mstype_unset_timers,

    mstype_set_codecs,

    mstype_set_bypass_encoding,
    mstype_set_bypass_decoding,

    mstype_set_rtp_ssrc,

    mstype_set_msrp_cb
}
tsip_msession_param_type_t;

#define TSIP_MSESSION_SET_SRTP_MODE(SRTP_MODE_ENUM)											mstype_set_srtp_mode, (tmedia_srtp_mode_t)SRTP_MODE_ENUM
#define TSIP_MSESSION_SET_AVPF_MODE(MEDIA_MODE_ENUM)										mstype_set_avpf_mode, (tmedia_mode_t)MEDIA_MODE_ENUM
#define TSIP_MSESSION_SET_PROFILE(PROFILE_ENUM)												mstype_set_srtp_mode, (tmedia_profile_t)PROFILE_ENUM
#define TSIP_MSESSION_SET_100rel(ENABLED_BOOL)												mstype_set_100rel, (tsk_bool_t)ENABLED_BOOL
#define TSIP_MSESSION_SET_RTCP(ENABLED_BOOL)												mstype_set_rtcp, (tsk_bool_t)ENABLED_BOOL
#define TSIP_MSESSION_SET_RTCPMUX(ENABLED_BOOL)												mstype_set_rtcpmux, (tsk_bool_t)ENABLED_BOOL
#define TSIP_MSESSION_SET_ICE(ENABLED_BOOL)													mstype_set_ice, (tsk_bool_t)ENABLED_BOOL
#define TSIP_MSESSION_SET_ICE_STUN(ENABLED_BOOL)											mstype_set_ice_stun, (tsk_bool_t)ENABLED_BOOL
#define TSIP_MSESSION_SET_ICE_TURN(ENABLED_BOOL)											mstype_set_ice_turn, (tsk_bool_t)ENABLED_BOOL
#define TSIP_MSESSION_SET_STUN_SERVER(HOSTNAME, PORT)										mstype_set_stun_server, (const char*)HOSTNAME, (uint16_t)PORT
#define TSIP_MSESSION_SET_STUN_CRED(USERNAME, PASSWORD)										mstype_set_stun_cred, (const char*)USERNAME, (const char*)PASSWORD
#define TSIP_MSESSION_SET_QOS(TYPE_ENUM, STRENGTH_ENUM)										mstype_set_qos, (tmedia_qos_stype_t)TYPE_ENUM, (tmedia_qos_strength_t)STRENGTH_ENUM
#define TSIP_MSESSION_UNSET_QOS()															mstype_unset_qos
#define TSIP_MSESSION_SET_VIDEO_FPS(FPS_INT)												mstype_set_video_fps, (int32_t)FPS_INT
#define TSIP_MSESSION_SET_VIDEO_BW_UP(BW_INT)												mstype_set_video_bw_up, (int32_t)(BW_INT)
#define TSIP_MSESSION_SET_VIDEO_BW_DOWN(BW_INT)												mstype_set_video_bw_down, (int32_t)(BW_INT)
#define TSIP_MSESSION_SET_VIDEO_PREFSIZE(PREFSIZE_ENUM)										mstype_set_video_prefsize, (int32_t)(PREFSIZE_ENUM)
#define TSIP_MSESSION_SET_TIMERS(TIMEOUT_UINT, REFRESHER_STR)								mstype_set_timers, (unsigned)TIMEOUT_UINT, (const char*)REFRESHER_STR
#define TSIP_MSESSION_UNSET_TIMERS()														mstype_unset_timers
#define TSIP_MSESSION_SET_CODECS(CODECS_INT)												mstype_set_codecs, (signed)CODECS_INT
#define TSIP_MSESSION_SET_BYPASS_ENCODING(ENABLED_BOOL)										mstype_set_bypass_encoding, (tsk_bool_t)ENABLED_BOOL
#define TSIP_MSESSION_UNSET_BYPASS_ENCODING()												TSIP_MSESSION_SET_BYPASS_ENCODING(tsk_false)
#define TSIP_MSESSION_SET_BYPASS_DECODING(ENABLED_BOOL)										mstype_set_bypass_decoding, (tsk_bool_t)ENABLED_BOOL
#define TSIP_MSESSION_SET_RTP_SSRC(MEDIA_ENUM, SSRC_UINT)									mstype_set_rtp_ssrc, (tmedia_type_t)MEDIA_ENUM, (uint32_t)SSRC_UINT
#define TSIP_MSESSION_SET_RTP_SSRC_AUDIO(SSRC_UINT)											TSIP_MSESSION_SET_RTP_SSRC(tmedia_audio, (SSRC_UINT))
#define TSIP_MSESSION_SET_RTP_SSRC_VIDEO(SSRC_UINT)											TSIP_MSESSION_SET_RTP_SSRC(tmedia_video, (SSRC_UINT))
#define TSIP_MSESSION_UNSET_BYPASS_DECODING()												TSIP_MSESSION_SET_BYPASS_DECODING(tsk_false)
#define TSIP_MSESSION_SET_MSRP_CB(TMEDIA_SESSION_MSRP_CB_F)									mstype_set_msrp_cb, (tmedia_session_msrp_cb_f)TMEDIA_SESSION_MSRP_CB_F

#define TSIP_MSESSION_SET_NULL()			mstype_null

typedef struct tsip_ssession_s {
    TSK_DECLARE_OBJECT;

    tsip_ssession_id_t id;
    tsip_ssession_id_t id_parent; //for call transfer
    unsigned owner:1;

    const struct tsip_stack_s* stack;
    const void* userdata;

    //=======
    // SIP
    //=======
    tsk_params_L_t *caps;
    tsk_params_L_t *headers;

    unsigned no_contact:1;
    struct tsip_uri_s* from;
    struct tsip_uri_s* to;
    int64_t expires;
    tsk_bool_t silent_hangup;
    char* sigcomp_id;
    char* auth_ha1;
    char* auth_impi;

    //=======
    // WebSocket
    //=======
    struct {
        // used when the stack is running in webrtc2sip mode to store the 'origin' (a.k.a 'src')
        struct {
            char* host;
            tsk_istr_t port;
            char* proto;
        } src;
    } ws;

    //=======
    // Media
    //=======
    struct {
        tmedia_type_t type;
        tmedia_profile_t profile;
        tmedia_srtp_mode_t srtp_mode;
        tmedia_mode_t avpf_mode;
        tmedia_codec_id_t codecs;
        tsk_bool_t bypass_encoding;
        tsk_bool_t bypass_decoding;

        /* Video */
        int32_t video_fps;
        int32_t video_bw_up;
        int32_t video_bw_down;
        tmedia_pref_video_size_t video_pref_size;

        /* RTP*/
        struct {
            struct {
                uint32_t audio;
                uint32_t video;
            } ssrc;
        } rtp;

        /* Session timers */
        struct {
            char* refresher;
            unsigned timeout;
        } timers;
        /* QoS */
        struct {
            tmedia_qos_stype_t type;
            tmedia_qos_strength_t strength;
        } qos;
        /* MSRP */
        struct {
            tmedia_session_msrp_cb_f callback;
        } msrp;
        /* STUN */
        struct {
            char* username;
            char* password;
            char* hostname;
            uint16_t port;
        } stun;

        /* Features */
        unsigned enable_conditional_ringing:1;
        unsigned enable_100rel:1;
        unsigned enable_ice:1;
        unsigned enable_icestun:1;
        unsigned enable_iceturn:1;
        unsigned enable_rtcp:1;
        unsigned enable_rtcpmux:1;
    } media;
}
tsip_ssession_t;

/** A pointer to a SIP Session */
typedef void tsip_ssession_handle_t;
/** A pointer to a LTE/IMS stack */
typedef void tsip_stack_handle_t;

TINYSIP_API tsip_ssession_handle_t* tsip_ssession_create(tsip_stack_handle_t *stack, ...);
TINYSIP_API int tsip_ssession_set(tsip_ssession_handle_t *self, ...);
TINYSIP_API tsip_ssession_id_t tsip_ssession_get_id(const tsip_ssession_handle_t *self);
TINYSIP_API tsip_ssession_id_t tsip_ssession_get_id_parent(const tsip_ssession_handle_t *self);
TINYSIP_API int tsip_ssession_take_ownership(tsip_ssession_handle_t *self);
TINYSIP_API tsk_bool_t tsip_ssession_have_ownership(const tsip_ssession_handle_t *self);
TINYSIP_API int tsip_ssession_respond(const tsip_ssession_handle_t *self, short status, const char* phrase, const void* payload, tsk_size_t size, const struct tsip_message_s* request, ...);
TINYSIP_API const void* tsip_ssession_get_userdata(const tsip_ssession_handle_t *self);
TINYSIP_API tmedia_type_t tsip_ssession_get_mediatype(const tsip_ssession_handle_t *self);
TINYSIP_API tmedia_session_mgr_t* tsip_session_get_mediamgr(const tsip_ssession_handle_t *self);
TINYSIP_API const tsip_stack_handle_t* tsip_ssession_get_stack(const tsip_ssession_handle_t *self);
TINYSIP_API tmedia_codec_id_t tsip_ssession_get_codecs_neg(tsip_ssession_handle_t *self);

int tsip_ssession_handle(const tsip_ssession_t *self, const struct tsip_action_s* action);

typedef tsk_list_t tsip_ssessions_L_t; /**< List of @ref tsip_ssession_handle_t elements. */
TINYSIP_GEXTERN const tsk_object_def_t *tsip_ssession_def_t;

TSIP_END_DECLS

#endif /* TSIP_SSESSION_H */

