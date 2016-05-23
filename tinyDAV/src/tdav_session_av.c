/*
 * Copyright (C) 2012-2013 Doubango Telecom <http://www.doubango.org>
 * Copyright (C) 2012 Diop Mamadou Ibrahima
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

/**@file tdav_session_av.c
 * @brief Audio/Video/T.140 base Session plugin
 */
// http://c-faq.com/ansi/constmismatch.html: to be checked for warnings

#include "tinydav/tdav_session_av.h"
#include "tinydav/codecs/dtmf/tdav_codec_dtmf.h"
#include "tinydav/codecs/fec/tdav_codec_red.h"
#include "tinydav/codecs/fec/tdav_codec_ulpfec.h"

#include "tinysdp/headers/tsdp_header_S.h"
#include "tinysdp/headers/tsdp_header_B.h"

#include "tinyrtp/trtp_manager.h"
#include "tinyrtp/rtp/trtp_rtp_packet.h"
#if HAVE_SRTP
#	include "tinyrtp/trtp_srtp.h"
#endif

#include "ice/tnet_ice_ctx.h"
#include "ice/tnet_ice_candidate.h"

#include "tinymedia/tmedia_consumer.h"
#include "tinymedia/tmedia_producer.h"
#include "tinymedia/tmedia_defaults.h"

#include "tls/tnet_tls.h"
#include "tls/tnet_dtls.h"

#include <math.h> /* log10 */
#include <limits.h> /* INT_MAX */
#include <ctype.h> /* isspace */


#if HAVE_SRTP
static const tsk_bool_t __have_libsrtp = tsk_true;
#else
static const tsk_bool_t __have_libsrtp = tsk_false;
#endif

#define TDAV_IS_DTMF_CODEC(codec) (codec && TMEDIA_CODEC((codec))->plugin == tdav_codec_dtmf_plugin_def_t)
#define TDAV_IS_ULPFEC_CODEC(codec) (codec && TMEDIA_CODEC((codec))->plugin == tdav_codec_ulpfec_plugin_def_t)
#define TDAV_IS_RED_CODEC(codec) (codec && TMEDIA_CODEC((codec))->plugin == tdav_codec_red_plugin_def_t)
#define TDAV_IS_VIDEO_CODEC(codec) (codec && TMEDIA_CODEC((codec))->plugin->type & tmedia_video)

#if !defined(TDAV_DFAULT_FP_HASH)
#define TDAV_DFAULT_FP_HASH		tnet_dtls_hash_type_sha256
#endif /* TDAV_DFAULT_FP_HASH */
#if !defined(TDAV_FIXME_MEDIA_LEVEL_DTLS_ATT)
#define TDAV_FIXME_MEDIA_LEVEL_DTLS_ATT		0
#endif /* TDAV_FIXME_MEDIA_LEVEL_DTLS_ATT */

// rfc5763 - The endpoint MUST NOT use the connection attribute defined in [RFC4145].
#if !defined(TDAV_DTLS_CONNECTION_ATT)
#	define TDAV_DTLS_CONNECTION_ATT		0
#endif

static void* TSK_STDCALL _tdav_session_av_error_async_thread(void* usrdata);
static int _tdav_session_av_raise_error_async(struct tdav_session_av_s* self, tsk_bool_t is_fatal, const char* reason);
#if HAVE_SRTP
static int _tdav_session_av_srtp_dtls_cb(const void* usrdata, enum trtp_srtp_dtls_event_type_e type, const char* reason);
#endif /* HAVE_SRTP */
static int _tdav_session_av_red_cb(const void* usrdata, const struct trtp_rtp_packet_s* packet);
static int _tdav_session_av_dtls_set_remote_setup(struct tdav_session_av_s* self, tnet_dtls_setup_t setup, tsk_bool_t connection_new, tsk_bool_t is_ro_null);

#define SDP_CAPS_COUNT_MAX		0x1F
#define SDP_DECLARE_TAG int32_t tag // [1 - *]
#define SDP_TAG(self) ((self) ? *((int32_t*)(self)) : 0)

typedef enum RTP_PROFILE_E {
    RTP_PROFILE_NONE = 0x00,

    RTP_PROFILE_AVP = (1 << 0),
    RTP_PROFILE_AVPF = (1 << 1),

    RTP_PROFILE_SECURE = (1 << 2),
    RTP_PROFILE_SECURE_SDES = (RTP_PROFILE_SECURE | (1 << 3)),
    RTP_PROFILE_SECURE_DTLS = (RTP_PROFILE_SECURE | (1 << 4)),

    RTP_PROFILE_SAVP = (RTP_PROFILE_AVP | RTP_PROFILE_SECURE_SDES),
    RTP_PROFILE_SAVPF = (RTP_PROFILE_AVPF | RTP_PROFILE_SECURE_SDES),

    RTP_PROFILE_UDP_TLS_RTP_SAVP = (RTP_PROFILE_AVP | RTP_PROFILE_SECURE_DTLS),
    RTP_PROFILE_UDP_TLS_RTP_SAVPF = (RTP_PROFILE_AVPF | RTP_PROFILE_SECURE_DTLS)
}
RTP_PROFILE_T;

typedef struct RTP_PROFILE_XS {
    enum RTP_PROFILE_E type;
    const char* name;
}
RTP_PROFILE_XT;

static const RTP_PROFILE_XT RTP_PROFILES[] = {
    { RTP_PROFILE_AVP, "RTP/AVP" },
    { RTP_PROFILE_AVPF, "RTP/AVPF" },
    { RTP_PROFILE_SAVP, "RTP/SAVP" },
    { RTP_PROFILE_SAVPF, "RTP/SAVPF" },
    { RTP_PROFILE_UDP_TLS_RTP_SAVP, "UDP/TLS/RTP/SAVP" },
    { RTP_PROFILE_UDP_TLS_RTP_SAVPF, "UDP/TLS/RTP/SAVPF" },
};
#define RTP_PROFILES_COUNT (sizeof(RTP_PROFILES) / sizeof(RTP_PROFILES[0]))

typedef struct sdp_acap_xs {
    SDP_DECLARE_TAG;
    unsigned optional:1; // "e.g. [2]"
    unsigned or:1; // "e.g.|2"
    const char* value;
}
sdp_acap_xt;
typedef sdp_acap_xt sdp_acaps_xt[SDP_CAPS_COUNT_MAX];

typedef struct sdp_tcap_xs {
    SDP_DECLARE_TAG;
    RTP_PROFILE_T profile;
}
sdp_tcap_xt;
typedef sdp_tcap_xt sdp_tcaps_xt[SDP_CAPS_COUNT_MAX];

typedef struct sdp_pcfg_xs {
    SDP_DECLARE_TAG;
    sdp_tcap_xt tcap;
    sdp_acaps_xt acaps;
}
sdp_pcfg_xt;
typedef sdp_pcfg_xt sdp_acfg_xt;
typedef sdp_pcfg_xt sdp_pcfgs_xt[SDP_CAPS_COUNT_MAX];
typedef tsk_object_t sdp_headerM_Or_Message; /* tsdp_header_M_t or tsdp_message_t */

#define _sdp_reset(self) if((self)) memset((self), 0, sizeof(*(self)));
#define _sdp_pcfgs_reset(self) _sdp_reset((self))
#define _sdp_acfgs_reset(self) _sdp_reset((self))
#define _sdp_pcfg_reset(self) _sdp_reset((self))
#define _sdp_acfg_reset(self) _sdp_reset((self))
#define _sdp_tcaps_reset(self) _sdp_reset((self))
#define _sdp_acaps_reset(self) _sdp_reset((self))
#define _sdp_integer_length(self) ((self) ? ((int32_t)log10(abs(self)) + 1) : 1)
#define _sdp_str_index_of(str, sub_str) tsk_strindexOf((str), tsk_strlen((str)), sub_str)
#define _sdp_str_starts_with(str, sub_str) (_sdp_str_index_of((str), (sub_str)) == 0)
#define _sdp_str_contains(str, sub_str) (_sdp_str_index_of((str), (sub_str)) != -1)
#define _SDP_DECLARE_INDEX_OF(name) \
static int32_t _sdp_##name##s_indexof(const sdp_##name##_xt (*name##s)[SDP_CAPS_COUNT_MAX], int32_t tag) \
{ \
if(name##s){ \
int32_t i; \
for(i = 0; i < SDP_CAPS_COUNT_MAX; ++i){ \
if((*name##s)[i].tag == tag){ \
return i; \
} \
} \
} \
return -1; \
}

typedef struct tdav_sdp_caps_s {
    TSK_DECLARE_OBJECT;

    sdp_pcfgs_xt local;
    sdp_pcfgs_xt remote;
    sdp_acfg_xt acfg;
}
tdav_sdp_caps_t;

static tdav_sdp_caps_t* tdav_sdp_caps_create();

static const tsdp_header_A_t* _sdp_findA_at(const sdp_headerM_Or_Message* sdp, const char* field, tsk_size_t index);
static int _sdp_add_headerA(sdp_headerM_Or_Message* sdp, const char* field, const char* value);
static RTP_PROFILE_T _sdp_profile_from_string(const char* profile);
static const char* _sdp_profile_to_string(RTP_PROFILE_T profile);
static int32_t _sdp_acaps_indexof(const sdp_acap_xt (*acaps)[SDP_CAPS_COUNT_MAX], int32_t tag);
static const sdp_acap_xt* _sdp_acaps_find_by_field(const sdp_acap_xt (*acaps)[SDP_CAPS_COUNT_MAX], const char* field, int32_t index);
static int _sdp_acaps_from_sdp(const sdp_headerM_Or_Message* sdp, sdp_acap_xt (*acaps)[SDP_CAPS_COUNT_MAX], tsk_bool_t reset);
static int32_t _sdp_tcaps_indexof(const sdp_tcap_xt (*tcaps)[SDP_CAPS_COUNT_MAX], int32_t tag);
static int _sdp_tcaps_from_sdp(const sdp_headerM_Or_Message* sdp, sdp_tcap_xt (*tcaps)[SDP_CAPS_COUNT_MAX], tsk_bool_t reset);
static int _sdp_acfg_to_sdp(sdp_headerM_Or_Message* sdp, const sdp_acfg_xt *acfg);
static int _sdp_pcfgs_from_sdp(const sdp_headerM_Or_Message* sdp, sdp_acap_xt (*acaps)[SDP_CAPS_COUNT_MAX], sdp_tcap_xt (*tcaps)[SDP_CAPS_COUNT_MAX], sdp_pcfg_xt (*pcfgs)[SDP_CAPS_COUNT_MAX], tsk_bool_t reset);
static int _sdp_pcfgs_to_sdp(sdp_headerM_Or_Message* sdp, const sdp_pcfg_xt (*pcfg)[SDP_CAPS_COUNT_MAX]);
static int _sdp_pcfg_ensure(sdp_headerM_Or_Message* sdp, const sdp_pcfg_xt* pcfg);
static int _sdp_pcfgs_cat(const sdp_pcfg_xt (*pcfgs_src)[SDP_CAPS_COUNT_MAX], sdp_pcfg_xt (*pcfgs_dst)[SDP_CAPS_COUNT_MAX]);


int tdav_session_av_init(tdav_session_av_t* self, tmedia_type_t media_type)
{
    uint64_t session_id;
    tmedia_session_t* base = TMEDIA_SESSION(self);

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if (!base->initialized) {
        int ret = tmedia_session_init(base, media_type);
        if (ret != 0) {
            return ret;
        }
    }

    /* base::init(): called by tmedia_session_create() */

    self->media_type = media_type;
    self->media_profile = tmedia_defaults_get_profile();
    self->use_rtcp = tmedia_defaults_get_rtcp_enabled();
    self->use_rtcpmux = tmedia_defaults_get_rtcpmux_enabled();
    self->avpf_mode_set = self->avpf_mode_neg = tmedia_defaults_get_avpf_mode();
    self->fps = -1; // use what is negotiated by the codec unless overrided by the user
    self->pref_size = tmedia_defaults_get_pref_video_size(); // for the encoder
    self->bandwidth_max_upload_kbps = ((media_type & tmedia_video || (media_type & tmedia_bfcp_video) == tmedia_bfcp_video) ? tmedia_defaults_get_bandwidth_video_upload_max() : INT_MAX); // INT_MAX or <=0 means undefined
    self->bandwidth_max_download_kbps = ((media_type & tmedia_video || (media_type & tmedia_bfcp_video) == tmedia_bfcp_video) ? tmedia_defaults_get_bandwidth_video_download_max() : INT_MAX); // INT_MAX or <=0 means undefined
    self->congestion_ctrl_enabled = tmedia_defaults_get_congestion_ctrl_enabled(); // whether to enable draft-alvestrand-rtcweb-congestion-03 and draft-alvestrand-rmcat-remb-01
#if HAVE_SRTP
    // this is the default value and can be updated by the user using "session_set('srtp-mode', mode_e)"
    self->srtp_type = (self->media_profile == tmedia_profile_rtcweb) ? (tsk_strnullORempty(TMEDIA_SESSION(self)->dtls.file_pbk) ? tmedia_srtp_type_sdes : tmedia_srtp_type_dtls) : tmedia_defaults_get_srtp_type();
    self->srtp_mode = (self->media_profile == tmedia_profile_rtcweb) ? tmedia_srtp_mode_mandatory : tmedia_defaults_get_srtp_mode();
    self->use_srtp = (self->srtp_mode == tmedia_srtp_mode_mandatory); // if optional -> negotiate
    // remove DTLS-SRTP option if not supported
    if((self->srtp_type & tmedia_srtp_type_dtls) && !tnet_dtls_is_srtp_supported()) {
        TSK_DEBUG_WARN("DTLS-SRTP enabled but not supported. Please rebuild the code with this option enabled (requires OpenSSL 1.0.1+)");
        if(!(self->srtp_type &= ~tmedia_srtp_type_dtls)) {
            // only DTLS-SRTP was enabled
            self->srtp_mode = tmedia_srtp_mode_none;
            self->use_srtp = tsk_false;
        }
    }
    //!\ DTLS-SRTP requires certificates but do not check right now as it could be defined later
#endif

    tsk_safeobj_init(self);

    // session id
    if (!(session_id = TMEDIA_SESSION(self)->id)) { // set the session id if not already done
        TMEDIA_SESSION(self)->id = session_id = tmedia_session_get_unique_id();
    }
    // consumer
    TSK_OBJECT_SAFE_FREE(self->consumer);
    if (!(self->consumer = tmedia_consumer_create((self->media_type & tmedia_video || (self->media_type & tmedia_bfcp_video) == tmedia_bfcp_video) ? tmedia_video : tmedia_audio, session_id))) { // get an audio (or video) consumer and ignore "bfcp" part
        TSK_DEBUG_ERROR("Failed to create consumer for media type = %d", self->media_type);
    }
    // producer
    TSK_OBJECT_SAFE_FREE(self->producer);
    if (!(self->producer = tmedia_producer_create(self->media_type, session_id))) {
        TSK_DEBUG_ERROR("Failed to create producer for media type = %d", self->media_type);
    }

    // sdp caps
    TSK_OBJECT_SAFE_FREE(self->sdp_caps);
    if (!(self->sdp_caps = tdav_sdp_caps_create())) {
        TSK_DEBUG_ERROR("Failed to create SDP caps");
        return -1;
    }

    // pt mapping (when bypassing is enabled)
    self->pt_map.local = self->pt_map.remote = self->pt_map.neg = -1;

    return 0;
}

tsk_bool_t tdav_session_av_set(tdav_session_av_t* self, const tmedia_param_t* param)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_false;
    }

    // try with base class first
    if(tmedia_session_set_2(TMEDIA_SESSION(self), param)) {
        return tsk_true;
    }

    if(param->plugin_type == tmedia_ppt_consumer && self->consumer) {
        return (tmedia_consumer_set(self->consumer, param) == 0);
    }
    else if(param->plugin_type == tmedia_ppt_producer && self->producer) {
        return (tmedia_producer_set(self->producer, param) == 0);
    }
    else if(param->plugin_type == tmedia_ppt_session) {
        if(param->value_type == tmedia_pvt_pchar) {
            if(tsk_striequals(param->key, "remote-ip")) {
                if(param->value) {
                    tsk_strupdate(&self->remote_ip, (const char*)param->value);
                    return tsk_true;
                }
            }
            else if(tsk_striequals(param->key, "local-ip")) {
                tsk_strupdate(&self->local_ip, (const char*)param->value);
                return tsk_true;
            }
            else if(tsk_striequals(param->key, "use-ipv6")) {
                self->use_ipv6 = tsk_striequals(param->value, "ipv6");
                return tsk_true;
            }
        }
        else if(param->value_type == tmedia_pvt_int32) {
            if(tsk_striequals(param->key, "srtp-mode")) {
#if HAVE_SRTP
                self->srtp_mode = (tmedia_srtp_mode_t)TSK_TO_INT32((uint8_t*)param->value);
                if(self->rtp_manager) {
                    trtp_manager_set_srtp_type_local(self->rtp_manager, self->srtp_type, self->srtp_mode);
                }
#else
                TSK_DEBUG_INFO("'srtp-mode' param ignored beacuse SRTP not enabled. Please rebuild the source code with this option.");
#endif /* HAVE_SRTP */
                return tsk_true;
            }
            else if(tsk_striequals(param->key, "rtp-ssrc")) {
                self->rtp_ssrc = *((uint32_t*)param->value);
                if(self->rtp_manager && self->rtp_ssrc) {
                    self->rtp_manager->rtp.ssrc.local = self->rtp_ssrc;
                }
                return tsk_true;
            }
            else if(tsk_striequals(param->key, "rtcp-enabled")) {
                self->use_rtcp = (TSK_TO_INT32((uint8_t*)param->value) != 0);
                return tsk_true;
            }
            else if(tsk_striequals(param->key, "rtcpmux-enabled")) {
                self->use_rtcpmux = (TSK_TO_INT32((uint8_t*)param->value) != 0);
                return tsk_true;
            }
            else if(tsk_striequals(param->key, "avpf-mode")) {
                self->avpf_mode_set = (tmedia_mode_t)TSK_TO_INT32((uint8_t*)param->value);
                return tsk_true;
            }
            else if(tsk_striequals(param->key, "webrtc2sip-mode-enabled")) {
                self->is_webrtc2sip_mode_enabled = (TSK_TO_INT32((uint8_t*)param->value) != 0);
                return tsk_true;
            }
            else if (tsk_striequals(param->key, "bandwidth-max-upload")) {
                self->bandwidth_max_upload_kbps = TSK_TO_INT32((uint8_t*)param->value);
                return tsk_true;
            }
            else if (tsk_striequals(param->key, "bandwidth-max-download")) {
                self->bandwidth_max_download_kbps = TSK_TO_INT32((uint8_t*)param->value);
                return tsk_true;
            }
            else if (tsk_striequals(param->key, "fps")) {
                self->fps = TSK_TO_INT32((uint8_t*)param->value);
                return tsk_true;
            }
            else if (tsk_striequals(param->key, "pref-size")) {
                self->pref_size = (tmedia_pref_video_size_t)TSK_TO_INT32((uint8_t*)param->value);
                return tsk_true;
            }
        }
        else if(param->value_type == tmedia_pvt_pobject) {
            if(tsk_striequals(param->key, "natt-ctx")) {
                TSK_OBJECT_SAFE_FREE(self->natt_ctx);
                self->natt_ctx = tsk_object_ref(param->value);
                return tsk_true;
            }
            else if(tsk_striequals(param->key, "ice-ctx")) {
                TSK_OBJECT_SAFE_FREE(self->ice_ctx);
                self->ice_ctx = tsk_object_ref(param->value);
                if(self->rtp_manager) {
                    trtp_manager_set_ice_ctx(self->rtp_manager, self->ice_ctx);
                }
                return tsk_true;
            }
            else if(tsk_striequals(param->key, "remote-sdp-message")) {
                TSK_OBJECT_SAFE_FREE(self->remote_sdp);
                self->remote_sdp = (struct tsdp_message_s*)tsk_object_ref(param->value);
                return tsk_true;
            }
            else if(tsk_striequals(param->key, "local-sdp-message")) {
                TSK_OBJECT_SAFE_FREE(self->local_sdp);
                self->local_sdp = (struct tsdp_message_s*)tsk_object_ref(param->value);
                return tsk_true;
            }
        }
    }

    return tsk_false;
}

tsk_bool_t tdav_session_av_get(tdav_session_av_t* self, tmedia_param_t* param)
{
    if(!self || !param) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_false;
    }

    if (param->plugin_type == tmedia_ppt_session) {
        if (param->value_type == tmedia_pvt_int32) {
            if (tsk_striequals(param->key, "codecs-negotiated")) { // negotiated codecs
                tmedia_codecs_L_t* neg_codecs = tsk_object_ref(TMEDIA_SESSION(self)->neg_codecs);
                if (neg_codecs) {
                    const tsk_list_item_t* item;
                    tsk_list_foreach(item, neg_codecs) {
                        ((int32_t*)param->value)[0] |= TMEDIA_CODEC(item->data)->id;
                    }
                    TSK_OBJECT_SAFE_FREE(neg_codecs);
                }
                return tsk_true;
            }
            else if (tsk_striequals(param->key, "srtp-enabled")) {
#if HAVE_SRTP
                if (self->rtp_manager) {
                    ((int8_t*)param->value)[0] = self->use_srtp ? 1 : 0;
                    return tsk_true;
                }
#else
                ((int8_t*)param->value)[0] = 0;
                TSK_DEBUG_INFO("Ignoring parameter 'srtp-enabled' because SRTP not supported. Please rebuild the source code with this option enabled.");
                return tsk_true;
#endif /* HAVE_SRTP */
            }
        }
        else if (param->value_type == tmedia_pvt_pobject) {
            if (tsk_striequals(param->key, "producer")) {
                *((tsk_object_t**)param->value) = tsk_object_ref(self->producer); // up to the caller to release the object
                return tsk_true;
            }
        }
    }

    return tsk_false;
}

int tdav_session_av_init_encoder(tdav_session_av_t* self, struct tmedia_codec_s* encoder)
{
    if (!self || !encoder) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    encoder->bandwidth_max_upload = self->bandwidth_max_upload_kbps;
    encoder->bandwidth_max_download = self->bandwidth_max_download_kbps;
    if ((encoder->type & tmedia_video) || (encoder->type & tmedia_bfcp_video)) {
        tmedia_codec_video_t* video = TMEDIA_CODEC_VIDEO(encoder);
        if (self->fps > 0) {
            video->out.fps = self->fps;
        }
        if (self->pref_size != video->pref_size) {
            unsigned width, height;
            if (tmedia_video_get_size(self->pref_size, &width, &height) == 0) {
                video->pref_size = self->pref_size;
                video->out.width = width;
                video->out.height = height;
            }
        }
        // QoS metrics default values
        TMEDIA_SESSION(self)->qos_metrics.video_out_width = video->out.width;
        TMEDIA_SESSION(self)->qos_metrics.video_out_height = video->out.height;
        TMEDIA_SESSION(self)->qos_metrics.video_in_width = video->in.width;
        TMEDIA_SESSION(self)->qos_metrics.video_in_height = video->in.height;
    }
    return 0;
}

int tdav_session_av_prepare(tdav_session_av_t* self)
{
    int ret = 0;

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    /* SRTPType */
#if HAVE_SRTP
    // Now that SSL certs are defined update SRTPType before creating the RTP manager
    if (self->media_profile == tmedia_profile_rtcweb) {
        self->srtp_type = tsk_strnullORempty(TMEDIA_SESSION(self)->dtls.file_pbk) ? tmedia_srtp_type_sdes : tmedia_srtp_type_dtls;
    }
#endif

    /* set local port */
    if (!self->rtp_manager) {
        self->rtp_manager = self->ice_ctx ? trtp_manager_create_2(self->ice_ctx, self->srtp_type, self->srtp_mode)
                            : trtp_manager_create(self->use_rtcp, self->local_ip, self->use_ipv6, self->srtp_type, self->srtp_mode);
    }
    if (self->rtp_manager) {
        const char *webproxy_type = tsk_null, *webproxy_host = tsk_null, *webproxy_login = tsk_null, *webproxy_password = tsk_null;
        unsigned short webproxy_port = 0;
        // Port range
        if ((ret = trtp_manager_set_port_range(self->rtp_manager, tmedia_defaults_get_rtp_port_range_start(), tmedia_defaults_get_rtp_port_range_stop()))) {
            return ret;
        }
        // WebProxy
        if ((ret = trtp_manager_set_proxy_auto_detect(self->rtp_manager, tmedia_defaults_get_webproxy_auto_detect()))) {
            return ret;
        }
        if ((ret = tmedia_defaults_get_webproxy_info(&webproxy_type, &webproxy_host, &webproxy_port, &webproxy_login, &webproxy_password))) {

        }
        if ((ret = trtp_manager_set_proxy_info(self->rtp_manager, tnet_proxy_type_from_string(webproxy_type), webproxy_host, webproxy_port, webproxy_login, webproxy_password))) {
        }

#if HAVE_SRTP
        if (tsk_strnullORempty(TMEDIA_SESSION(self)->dtls.file_pbk)) {
            // DTLS-SRTP requires certificates
            if(self->srtp_type & tmedia_srtp_type_dtls) {
                TSK_DEBUG_WARN("DTLS-SRTP requested but no SSL certificates provided, disabling this option :(");
                if(!(self->srtp_type &= ~tmedia_srtp_type_dtls)) {
                    // only DTLS-SRTP was enabled
                    self->srtp_mode = tmedia_srtp_mode_none;
                    self->use_srtp = tsk_false;
                    // update rtpmanager
                    ret = trtp_manager_set_srtp_type_local(self->rtp_manager, self->srtp_type, self->srtp_mode);
                }
            }
        }

        if ((self->srtp_type & tmedia_srtp_type_dtls) && (self->srtp_mode == tmedia_srtp_mode_optional || self->srtp_mode == tmedia_srtp_mode_mandatory)) {
            if((ret = trtp_manager_set_dtls_certs(self->rtp_manager, TMEDIA_SESSION(self)->dtls.file_ca, TMEDIA_SESSION(self)->dtls.file_pbk, TMEDIA_SESSION(self)->dtls.file_pvk, TMEDIA_SESSION(self)->dtls.verify))) {
                return ret;
            }
        }
#endif /* HAVE_SRTP */
        if((ret = trtp_manager_prepare(self->rtp_manager))) {
            return ret;
        }
        if(self->natt_ctx) {
            if((ret = trtp_manager_set_natt_ctx(self->rtp_manager, self->natt_ctx))) {
                return ret;
            }
        }
        if(self->rtp_ssrc) {
            self->rtp_manager->rtp.ssrc.local = self->rtp_ssrc;
        }
    }


    /* SRTP */
#if HAVE_SRTP
    {

    }
#endif

    /* Consumer will be prepared in tdav_session_audio_start() */
    /* Producer will be prepared in tdav_session_audio_start() */

    return ret;
}

int tdav_session_av_start(tdav_session_av_t* self, const tmedia_codec_t* best_codec)
{
    int ret;
    if(!self || !best_codec) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    // RED codec
    TSK_OBJECT_SAFE_FREE(self->red.codec);
    self->red.payload_type = 0;
    if((self->red.codec = tsk_object_ref((tsk_object_t*)tdav_session_av_get_red_codec(self)))) {
        self->red.payload_type = atoi(self->red.codec->neg_format);
        if(!TMEDIA_CODEC(self->red.codec)->opened) {
            if((ret = tmedia_codec_open(self->red.codec))) {
                TSK_DEBUG_ERROR("Failed to open [%s] codec", self->red.codec->plugin->desc);
                return ret;
            }
        }
        // set RED callback (unencapsulated data)
        ret = tdav_codec_red_set_callback((struct tdav_codec_red_s*)self->red.codec, _tdav_session_av_red_cb, self);
    }

    // ULPFEC
    TSK_OBJECT_SAFE_FREE(self->ulpfec.codec);
    self->ulpfec.payload_type = 0;
    if((self->ulpfec.codec = tsk_object_ref((tsk_object_t*)tdav_session_av_get_ulpfec_codec(self)))) {
        self->ulpfec.payload_type = atoi(self->ulpfec.codec->neg_format);
        if(!TMEDIA_CODEC(self->ulpfec.codec)->opened) {
            if((ret = tmedia_codec_open(self->ulpfec.codec))) {
                TSK_DEBUG_ERROR("Failed to open [%s] codec", self->ulpfec.codec->plugin->desc);
                return ret;
            }
        }
    }

    // Check if "RTCP-NACK", "RTC-FIR", "RTCP-GOOG-REMB", "RTCP-DOUBS-JCNG".... are supported by the selected encoder
    self->is_fb_fir_neg = self->is_fb_nack_neg = self->is_fb_googremb_neg = self->is_fb_doubsjcng_neg = tsk_false;
    if (TMEDIA_SESSION(self)->M.ro) {
        // a=rtcp-fb:* ccm fir
        // a=rtcp-fb:* nack
        // a=rtcp-fb:* goog-remb
        // a=rtcp-fb:* doubs-jcng
        char attr_fir[256], attr_nack[256], attr_goog_remb[256], attr_doubs_jcng[256];
        int index = 0;
        const tsdp_header_A_t* A;

        sprintf(attr_fir, "%s ccm fir", best_codec->neg_format);
        sprintf(attr_nack, "%s nack", best_codec->neg_format);
        sprintf(attr_goog_remb, "%s goog-remb", best_codec->neg_format);
        sprintf(attr_doubs_jcng, "%s doubs-jcng", best_codec->neg_format);

        while ((A = tsdp_header_M_findA_at(TMEDIA_SESSION(self)->M.ro, "rtcp-fb", index++))) {
            if (!self->is_fb_fir_neg) {
                self->is_fb_fir_neg = (tsk_striequals(A->value, "* ccm fir") || tsk_striequals(A->value, attr_fir));
            }
            if (!self->is_fb_nack_neg) {
                self->is_fb_nack_neg = (tsk_striequals(A->value, "* nack") || tsk_striequals(A->value, attr_nack));
            }
            if (!self->is_fb_googremb_neg) {
                self->is_fb_googremb_neg = (tsk_striequals(A->value, "* goog-remb") || tsk_striequals(A->value, attr_goog_remb));
            }
            if (!self->is_fb_doubsjcng_neg) {
                self->is_fb_doubsjcng_neg = (tsk_striequals(A->value, "* doubs-jcng") || tsk_striequals(A->value, attr_doubs_jcng));
            }
        }
    }

    if (self->rtp_manager) {
        int ret;
        tmedia_param_t* media_param = tsk_null;
        static const int32_t __ByPassIsYes = 1;
        static const int32_t __ByPassIsNo = 0;
        /* RTP/RTCP manager: use latest information. */

        // set callbacks
#if HAVE_SRTP
        ret = trtp_manager_set_dtls_callback(self->rtp_manager, self, _tdav_session_av_srtp_dtls_cb);
#endif /* HAVE_SRTP */

        // network information will be updated when the RTP manager starts if ICE is enabled
        ret = trtp_manager_set_rtp_remote(self->rtp_manager, self->remote_ip, self->remote_port);
        self->rtp_manager->use_rtcpmux = self->use_rtcpmux;
        ret = trtp_manager_set_payload_type(self->rtp_manager, best_codec->neg_format ? atoi(best_codec->neg_format) : atoi(best_codec->format));
        {
            int32_t bandwidth_max_upload_kbps = self->bandwidth_max_upload_kbps;
            int32_t bandwidth_max_download_kbps = self->bandwidth_max_download_kbps;
            if ((self->media_type & tmedia_video || (self->media_type & tmedia_bfcp_video) == tmedia_bfcp_video)) {
                if (self->congestion_ctrl_enabled) {
                    const tmedia_codec_t* best_codec = tdav_session_av_get_best_neg_codec(self); // use for encoding for sure and probably for decoding
                    if (TDAV_IS_VIDEO_CODEC(best_codec)) {
                        // the up bandwidth will be updated once the decode the first frame as the current values (width, height, fps) are not really correct and based on the SDP negotiation
                        bandwidth_max_download_kbps = TSK_MIN(
                                                          tmedia_get_video_bandwidth_kbps_2(TMEDIA_CODEC_VIDEO(best_codec)->in.width, TMEDIA_CODEC_VIDEO(best_codec)->in.height, TMEDIA_CODEC_VIDEO(best_codec)->in.fps),
                                                          bandwidth_max_download_kbps);
                        bandwidth_max_upload_kbps = TSK_MIN(
                                                        tmedia_get_video_bandwidth_kbps_2(TMEDIA_CODEC_VIDEO(best_codec)->out.width, TMEDIA_CODEC_VIDEO(best_codec)->out.height, TMEDIA_CODEC_VIDEO(best_codec)->out.fps),
                                                        bandwidth_max_upload_kbps);
                    }
                    else if ((self->media_type & tmedia_video || (self->media_type & tmedia_bfcp_video) == tmedia_bfcp_video)) {
                        bandwidth_max_download_kbps = TSK_MIN(tmedia_get_video_bandwidth_kbps_3(), bandwidth_max_download_kbps);
                        bandwidth_max_upload_kbps = TSK_MIN(tmedia_get_video_bandwidth_kbps_3(), bandwidth_max_upload_kbps);
                    }
                }
                else {
                    TSK_DEBUG_WARN("Video congestion control not enabled");
                }
            }

            TSK_DEBUG_INFO("max_bw_up=%d kpbs, max_bw_down=%d kpbs, congestion_ctrl_enabled=%d, media_type=%d", bandwidth_max_upload_kbps, bandwidth_max_download_kbps, self->congestion_ctrl_enabled, self->media_type);
            // forward up/down bandwidth info to rctp session (used in RTCP-REMB)
            ret = trtp_manager_set_app_bw_and_jcng(self->rtp_manager, bandwidth_max_upload_kbps, bandwidth_max_download_kbps, 1.f /*jcng_q*/);
        }

        // because of AudioUnit under iOS => prepare both consumer and producer then start() at the same time
        /* prepare consumer and producer */
        // Producer could output encoded frames:
        //	- On WP8 with built-in H.264 encoder
        //	- When Intel Quick Sync is used for encoding and added on the same Topology as the producer (camera MFMediaSource)
        if (self->producer) {
            if((ret = tmedia_producer_prepare(self->producer, best_codec)) == 0) {
                media_param = tmedia_param_create(tmedia_pat_set,
                                                  best_codec->type,
                                                  tmedia_ppt_codec,
                                                  tmedia_pvt_int32,
                                                  "bypass-encoding",
                                                  (void*)(self->producer->encoder.codec_id == best_codec->id ? &__ByPassIsYes : &__ByPassIsNo));
                if(media_param) {
                    tmedia_codec_set(TMEDIA_CODEC(best_codec), media_param);
                    TSK_OBJECT_SAFE_FREE(media_param);
                }
            }
        }
        // Consumer could accept encoded frames as input:
        //	- On WP8 with built-in H.264 decoder
        //	- When IMFTransform decoder is used for decoding and added on the same Topology as the consumer (EVR)
        if (self->consumer) {
            if ((ret = tmedia_consumer_prepare(self->consumer, best_codec)) == 0) {
                media_param = tmedia_param_create(tmedia_pat_set,
                                                  best_codec->type,
                                                  tmedia_ppt_codec,
                                                  tmedia_pvt_int32,
                                                  "bypass-decoding",
                                                  (void*)(self->consumer->decoder.codec_id == best_codec->id ? &__ByPassIsYes : &__ByPassIsNo));
                if(media_param) {
                    tmedia_codec_set(TMEDIA_CODEC(best_codec), media_param);
                    TSK_OBJECT_SAFE_FREE(media_param);
                }
            }
        }

        // Start RTP manager
        ret = trtp_manager_start(self->rtp_manager);

#if HAVE_SRTP
        self->use_srtp = trtp_manager_is_srtp_activated(self->rtp_manager);

        /* start consumer and producer */
        if (trtp_manager_is_dtls_activated(self->rtp_manager) && !trtp_manager_is_dtls_started(self->rtp_manager)) {
            // delay starting util DTLS-SRTP negotiation terminates (handshaking succeed)
            TSK_DEBUG_INFO("Delaying consumer/producer starting until DTLS-SRTP negotiation complete");
        }
        else {
#endif /* HAVE_SRTP */
            tsk_safeobj_lock(self);
            if (self->consumer && !self->consumer->is_started) {
                ret = tmedia_consumer_start(self->consumer);
            }
            if (self->producer && !self->producer->is_started) {
                ret = tmedia_producer_start(self->producer);
            }
            tsk_safeobj_unlock(self);
#if HAVE_SRTP
        }
#endif /* HAVE_SRTP */

        return ret;
    }
    else {
        TSK_DEBUG_ERROR("Invalid RTP/RTCP manager");
        return -3;
    }

    return 0;
}

int tdav_session_av_stop(tdav_session_av_t* self)
{
    tmedia_codec_t* codec;
    tsk_list_item_t* item;
    int ret = 0;

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    /* stop Producer */
    if (self->producer) {
        ret = tmedia_producer_stop(self->producer);
    }

    /* stop RTP/RTCP manager */
    if (self->rtp_manager) {
        ret = trtp_manager_stop(self->rtp_manager);
    }

    /* stop Consumer (after RTP manager to silently discard in coming packets) */
    if (self->consumer) {
        ret = tmedia_consumer_stop(self->consumer);
    }

    /* close codecs to force open() for next start (e.g SIP UPDATE with SDP) */
    if(TMEDIA_SESSION(self)->neg_codecs) {
        tsk_list_foreach(item, TMEDIA_SESSION(self)->neg_codecs) {
            if(!(codec = TMEDIA_CODEC(item->data))) {
                continue;
            }
            ret = tmedia_codec_close(codec);
        }
    }

    self->bytes_in.count_last_time = self->bytes_out.count_last_time = 0;
    self->bytes_in.count = self->bytes_out.count = 0;

    return ret;
}

int tdav_session_av_pause(tdav_session_av_t* self)
{
    int ret = 0;

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    /* Consumer */
    if(self->consumer) {
        ret = tmedia_consumer_pause(self->consumer);
    }
    /* Producer */
    if(self->producer) {
        ret = tmedia_producer_pause(self->producer);
    }

    return ret;
}

const tsdp_header_M_t* tdav_session_av_get_lo(tdav_session_av_t* self, tsk_bool_t *updated)
{
    tmedia_session_t* base = TMEDIA_SESSION(self);
#if HAVE_SRTP
    static const tsk_bool_t have_lib_srtp = tsk_true;
#else
    static const tsk_bool_t have_lib_srtp = tsk_false;
#endif
    const tsk_bool_t have_lib_srtp_dtls = tnet_dtls_is_srtp_supported();
    tsk_bool_t is_srtp_enable = (self->srtp_type != tmedia_srtp_type_none) && (self->srtp_mode == tmedia_srtp_mode_optional || self->srtp_mode == tmedia_srtp_mode_mandatory);
    tsk_bool_t is_srtp_dtls_enabled = is_srtp_enable && !!(self->srtp_type & tmedia_srtp_type_dtls);
    tsk_bool_t is_srtp_sdes_enabled = is_srtp_enable && !!(self->srtp_type & tmedia_srtp_type_sdes);
    tsk_bool_t is_srtp_local_mandatory = is_srtp_enable && (self->srtp_mode == tmedia_srtp_mode_mandatory);
    tsk_bool_t is_bfcp_session = ((base->type & tmedia_bfcp) == tmedia_bfcp) ? tsk_true : tsk_false;
    tsk_bool_t is_first_media;

    if(!base || !base->plugin || !updated) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }

    *updated = tsk_false;

    if(!self->rtp_manager || (!self->ice_ctx && !self->rtp_manager->transport)) {
        if(self->rtp_manager && (!self->ice_ctx && !self->rtp_manager->transport)) { // reINVITE or UPDATE (manager was destroyed when stoppped)
            if(trtp_manager_prepare(self->rtp_manager)) {
                TSK_DEBUG_ERROR("Failed to prepare transport");
                return tsk_null;
            }
        }
        else {
            TSK_DEBUG_ERROR("RTP/RTCP manager in invalid");
            return tsk_null;
        }
    }

    // only first media will add session-level attributes (e.g. DTLS setup and fingerprint)
    if((is_first_media = !!self->local_sdp)) {
        const tsdp_header_M_t* firstM = (const tsdp_header_M_t*)tsdp_message_get_headerAt(self->local_sdp, tsdp_htype_M, 0);
        if(!(is_first_media = !firstM)) {
            is_first_media = tsk_striequals(TMEDIA_SESSION(self)->plugin->media, firstM->media);
        }
    }

    if(base->ro_changed && base->M.lo) {
        static const char* __fields[] = {
            /* Codecs */
            "fmtp", "rtpmap", "imageattr",
            /* QoS */
            "curr", "des", "conf",
            /* SRTP */
            "crypto",
            /* DTLS */
            "setup", "fingerprint",
            /* ICE */
            "candidate", "ice-ufrag", "ice-pwd",
            /* SDPCapNeg */
            "tcap", "acap", "pcfg",
            /* Others */
            "mid", "rtcp-mux", "ssrc"
        };
        // remove media-level attributes
        tsdp_header_A_removeAll_by_fields(base->M.lo->Attributes, __fields, sizeof(__fields)/sizeof(__fields[0]));
        tsk_list_clear_items(base->M.lo->FMTs);
        // remove session-level attributes
        if(is_first_media) {
            // headers: contains all kind of headers but this is a smart function :)
            tsdp_header_A_removeAll_by_fields((tsdp_headers_A_L_t*)self->local_sdp->headers, __fields, sizeof(__fields)/sizeof(__fields[0]));
        }
    }

    *updated = (base->ro_changed || !base->M.lo);

    if(!base->M.lo) {
        if((base->M.lo = tsdp_header_M_create(base->plugin->media, self->rtp_manager->rtp.public_addr.port, "RTP/AVP"))) {
            /* If NATT is active, do not rely on the global IP address Connection line */
            if(self->natt_ctx) {
                tsdp_header_M_add_headers(base->M.lo,
					TSDP_HEADER_C_VA_ARGS("IN", TNET_SOCKET_TYPE_IS_IPV6(self->rtp_manager->rtp.public_addr.type) ? "IP6" : "IP4", self->rtp_manager->rtp.public_addr.ip),
                                          tsk_null);
            }
            /* 3GPP TS 24.229 - 6.1.1 General
             In order to support accurate bandwidth calculations, the UE may include the "a=ptime" attribute for all "audio" media
             lines as described in RFC 4566 [39]. If a UE receives an "audio" media line with "a=ptime" specified, the UE should
             transmit at the specified packetization rate. If a UE receives an "audio" media line which does not have "a=ptime"
             specified or the UE does not support the "a=ptime" attribute, the UE should transmit at the default codec packetization
             rate as defined in RFC 3551 [55A]. The UE will transmit consistent with the resources available from the network.

             For "video" and "audio" media types that utilize the RTP/RTCP, the UE shall specify the proposed bandwidth for each
             media stream utilizing the "b=" media descriptor and the "AS" bandwidth modifier in the SDP.

             The UE shall include the MIME subtype "telephone-event" in the "m=" media descriptor in the SDP for audio media
             flows that support both audio codec and DTMF payloads in RTP packets as described in RFC 4733 [23].
             */
            if(self->media_type & tmedia_audio) {
                tsk_istr_t ptime;
                tsk_itoa(tmedia_defaults_get_audio_ptime(), &ptime);
                tsdp_header_M_add_headers(base->M.lo,
                                          /* rfc3551 section 4.5 says the default ptime is 20 */
                                          TSDP_HEADER_A_VA_ARGS("ptime", ptime),
                                          TSDP_HEADER_A_VA_ARGS("minptime", "1"),
                                          TSDP_HEADER_A_VA_ARGS("maxptime", "255"),
                                          TSDP_HEADER_A_VA_ARGS("silenceSupp", "off - - - -"),
                                          tsk_null);
                // the "telephone-event" fmt/rtpmap is added below
            }
            else if((self->media_type & tmedia_video || (self->media_type & tmedia_bfcp_video) == tmedia_bfcp_video)) {
                tsk_istr_t session_id;
                // https://code.google.com/p/webrtc2sip/issues/detail?id=81
                // goog-remb: https://groups.google.com/group/discuss-webrtc/browse_thread/thread/c61ad3487e2acd52
                // rfc5104 - 7.1.  Extension of the rtcp-fb Attribute
                tsdp_header_M_add_headers(base->M.lo,
                                          TSDP_HEADER_A_VA_ARGS("rtcp-fb", "* ccm fir"),
                                          TSDP_HEADER_A_VA_ARGS("rtcp-fb", "* nack"),
                                          TSDP_HEADER_A_VA_ARGS("rtcp-fb", "* goog-remb"),
                                          TSDP_HEADER_A_VA_ARGS("rtcp-fb", "* doubs-jcng"),
                                          tsk_null);
                // https://tools.ietf.org/html/rfc4574
                // http://tools.ietf.org/html/rfc4796
                tsk_itoa(base->id, &session_id);
                tsdp_header_M_add_headers(base->M.lo,
                                          TSDP_HEADER_A_VA_ARGS("label", session_id),
                                          TSDP_HEADER_A_VA_ARGS("content", (self->media_type & tmedia_bfcp) ? "slides" : "main"),
                                          tsk_null);
                // http://tools.ietf.org/html/rfc3556
                // https://tools.ietf.org/html/rfc3890
                if(self->bandwidth_max_download_kbps > 0 && self->bandwidth_max_download_kbps != INT_MAX) { // INT_MAX or <=0 means undefined
                    tsdp_header_M_add_headers(base->M.lo,
                                              TSDP_HEADER_B_VA_ARGS("AS", self->bandwidth_max_download_kbps),
                                              TSDP_HEADER_B_VA_ARGS("TIAS", (self->bandwidth_max_download_kbps << 10)),
                                              tsk_null);
                }
            }
        }
        else {
            TSK_DEBUG_ERROR("Failed to create lo");
            return tsk_null;
        }
    }

    if(*updated) {
        tmedia_codecs_L_t* neg_codecs = tsk_null;

        if(base->M.ro) {
            TSK_OBJECT_SAFE_FREE(base->neg_codecs);
            /* update negociated codecs */
            if((neg_codecs = tmedia_session_match_codec(base, base->M.ro))) {
                base->neg_codecs = neg_codecs;
            }
            /* from codecs to sdp */
            if(TSK_LIST_IS_EMPTY(base->neg_codecs) || ((base->neg_codecs->tail == base->neg_codecs->head) && TDAV_IS_DTMF_CODEC(TSK_LIST_FIRST_DATA(base->neg_codecs)))) {
                base->M.lo->port = 0; /* Keep the RTP transport and reuse it when we receive a reINVITE or UPDATE request */
                // To reject an offered stream, the port number in the corresponding stream in the answer
                // MUST be set to zero.  Any media formats listed are ignored.  AT LEAST ONE MUST BE PRESENT, AS SPECIFIED BY SDP.
                tsk_strupdate(&base->M.lo->proto, base->M.ro->proto);
                if(base->M.ro->FMTs) {
                    tsk_list_pushback_list(base->M.lo->FMTs, base->M.ro->FMTs);
                }
                TSK_DEBUG_INFO("No codec matching for media type = %d", (int32_t)self->media_type);
                goto DONE;
            }
            else {
                tmedia_codec_to_sdp(base->neg_codecs, base->M.lo);
            }
        }
        else {
            /* from codecs to sdp */
            tmedia_codec_to_sdp(base->codecs, base->M.lo);
        }

        /* SRTP */
#if HAVE_SRTP
        {
            //start-of-HAVE_SRTP

            /* DTLS-SRTP default values */
            if(is_srtp_dtls_enabled) {
                /* "setup" and "connection" */
                if (self->dtls.local.setup == tnet_dtls_setup_none || self->dtls.local.setup == tnet_dtls_setup_actpass) { // if setup already negotiated then, use the same
                    // rfc5763: the caller is server by default
                    self->dtls.remote.setup = (!base->M.ro) ? tnet_dtls_setup_active : tnet_dtls_setup_passive;
                    _tdav_session_av_dtls_set_remote_setup(self, self->dtls.remote.setup, self->dtls.remote.connection_new, (!base->M.ro));
                }
                if (self->rtp_manager) {
                    trtp_manager_set_dtls_local_setup(self->rtp_manager, self->dtls.local.setup, self->dtls.local.connection_new);
                }
            }

            if(!base->M.ro) {
                // === RO IS NULL ===
                const trtp_srtp_ctx_xt *srtp_ctxs[SRTP_CRYPTO_TYPES_MAX] = { tsk_null };
                tsk_size_t ctx_count = 0, ctx_idx, acap_tag = 1;
                tsk_size_t acap_tag_fp_sha1 = 0, acap_tag_fp_sha256 = 0, acap_tag_setup = 0, acap_tag_connection = 0, acap_tag_crypro_start = 0;
                char* str = tsk_null;
                tsdp_header_A_t* cryptoA = tsk_null;
                tsk_bool_t negotiate_srtp = (self->srtp_mode == tmedia_srtp_mode_optional);
                tsk_bool_t negotiate_avpf = (self->avpf_mode_set == tmedia_mode_optional);
                tsk_bool_t is_srtp_remote_mandatory = (base->M.ro && _sdp_str_contains(base->M.ro->proto, "SAVP"));
                tsk_size_t profiles_index = 0;
                RTP_PROFILE_T profiles[RTP_PROFILES_COUNT] = { RTP_PROFILE_NONE };

                // get local SRTP context
                if(is_srtp_sdes_enabled) {
                    ctx_count = trtp_srtp_get_local_contexts(self->rtp_manager, (const trtp_srtp_ctx_xt **)&srtp_ctxs, sizeof(srtp_ctxs)/sizeof(srtp_ctxs[0]));
                }

                // a=tcap:
                if((negotiate_srtp || negotiate_avpf)) {
                    static const int32_t __tcap_tag = 1;
                    char* tcap = tsk_null;
                    const char* fp_sha1 = tsk_null;
                    const char* fp_sha256 = tsk_null;
#define _first_media_add_header()
#define _first_media_strcat(ppstr, format, ...) if(is_first_media) tsk_strcat_2((ppstr), (format), ##__VA_ARGS__)
#define _first_media_sprintf(ppstr, format, ...) if(is_first_media) tsk_sprintf((ppstr), (format), ##__VA_ARGS__)
#define _first_media_add_headers(sdp, ...) if(is_first_media) tsdp_message_add_headers((sdp), ##__VA_ARGS__)

                    _first_media_strcat(&tcap, "%d", __tcap_tag);

                    if(is_srtp_dtls_enabled) {
                        if(!tsk_strnullORempty(TMEDIA_SESSION(self)->dtls.file_pbk)) {
                            fp_sha1 = trtp_manager_get_dtls_local_fingerprint(self->rtp_manager, tnet_dtls_hash_type_sha1);
                            fp_sha256 = trtp_manager_get_dtls_local_fingerprint(self->rtp_manager, tnet_dtls_hash_type_sha256);
                        }
                        _first_media_strcat(&tcap, negotiate_avpf ? " UDP/TLS/RTP/SAVPF UDP/TLS/RTP/SAVP" : " UDP/TLS/RTP/SAVP");
                        if(negotiate_avpf) {
                            profiles[profiles_index++] = RTP_PROFILE_UDP_TLS_RTP_SAVPF;
                        }
                        profiles[profiles_index++] = RTP_PROFILE_UDP_TLS_RTP_SAVP;
                    }
                    if(is_srtp_sdes_enabled) {
                        _first_media_strcat(&tcap, negotiate_avpf ? " RTP/SAVPF RTP/SAVP" : " RTP/SAVP");
                        if(negotiate_avpf) {
                            profiles[profiles_index++] = RTP_PROFILE_SAVPF;
                        }
                        profiles[profiles_index++] = RTP_PROFILE_SAVP;
                    }

                    if(!is_srtp_local_mandatory) {
                        _first_media_strcat(&tcap, " RTP/AVPF");
                        profiles[profiles_index++] = RTP_PROFILE_AVPF;
                    }

                    // DTLS "setup" and "fringerprint"s
                    if((fp_sha1 || fp_sha256) && negotiate_srtp) {
                        char* acap_fp = tsk_null;
                        acap_tag_setup = 1, acap_tag_connection = 2;
                        _first_media_sprintf(&str, "%d setup:%s", acap_tag_setup, TNET_DTLS_SETUP_NAMES[self->dtls.local.setup]);
                        _first_media_add_headers(self->local_sdp, TSDP_HEADER_A_VA_ARGS("acap", str), tsk_null);
#if TDAV_DTLS_CONNECTION_ATT
                        _first_media_sprintf(&str, "%d connection:%s", acap_tag_connection, self->dtls.local.connection_new ? "new" : "existing");
#endif
                        _first_media_add_headers(self->local_sdp, TSDP_HEADER_A_VA_ARGS("acap", str), tsk_null);
                        // New Firefox Nightly repspond with SHA-256 when offered SHA-1 -> It's a bug in FF
                        // Just use SHA-256 as first choice
                        if(fp_sha256) {
                            _first_media_sprintf(&acap_fp, "3 fingerprint:%s %s", TNET_DTLS_HASH_NAMES[tnet_dtls_hash_type_sha256], fp_sha256);
                            acap_tag_fp_sha256 = 3;
                            _first_media_add_headers(self->local_sdp, TSDP_HEADER_A_VA_ARGS("acap", acap_fp), tsk_null);
                        }
                        if(fp_sha1) {
                            _first_media_sprintf(&acap_fp, "%d fingerprint:%s %s", fp_sha256 ? 4 : 3, TNET_DTLS_HASH_NAMES[tnet_dtls_hash_type_sha1], fp_sha1);
                            acap_tag_fp_sha1 = (fp_sha256 ? 4 : 3);
                            _first_media_add_headers(self->local_sdp, TSDP_HEADER_A_VA_ARGS("acap", acap_fp), tsk_null);
                        }

                        TSK_FREE(acap_fp);
                    }

                    // "tcap"
                    _first_media_add_headers(self->local_sdp, TSDP_HEADER_A_VA_ARGS("tcap", tcap), tsk_null);
                    TSK_FREE(tcap);

                    acap_tag = TSK_MAX(acap_tag, acap_tag_connection);
                    acap_tag = TSK_MAX(acap_tag, acap_tag_fp_sha1);
                    acap_tag = TSK_MAX(acap_tag, acap_tag_fp_sha256);
                }
                if(is_first_media && !negotiate_srtp && is_srtp_dtls_enabled) {
                    // add DTLS-SRTP fingerprint and setup at session-level
                    const char* fp_str = trtp_manager_get_dtls_local_fingerprint(self->rtp_manager, TDAV_DFAULT_FP_HASH);
                    if(fp_str) {
                        tsk_sprintf(&str, "%s %s", TNET_DTLS_HASH_NAMES[TDAV_DFAULT_FP_HASH], fp_str);
                        //!\ From RFC 5763 (DTLS-SRTP Framework) \A75: The endpoint MUST NOT use the connection attribute defined in [RFC4145].
#if TDAV_FIXME_MEDIA_LEVEL_DTLS_ATT
                        tsdp_header_M_add_headers(base->M.lo, TSDP_HEADER_A_VA_ARGS("fingerprint", str), tsk_null);
                        tsdp_header_M_add_headers(base->M.lo, TSDP_HEADER_A_VA_ARGS("setup", TNET_DTLS_SETUP_NAMES[self->dtls.local.setup]), tsk_null);
                        // tsdp_header_M_add_headers(base->M.lo, TSDP_HEADER_A_VA_ARGS("connection", self->dtls.local.connection_new ? "new" : "existing"), tsk_null);
#else
                        _first_media_add_headers(self->local_sdp, TSDP_HEADER_A_VA_ARGS("fingerprint", str), tsk_null);
                        _first_media_add_headers(self->local_sdp, TSDP_HEADER_A_VA_ARGS("setup", TNET_DTLS_SETUP_NAMES[self->dtls.local.setup]), tsk_null);
                        // _first_media_add_headers(self->local_sdp, TSDP_HEADER_A_VA_ARGS("connection", self->dtls.local.connection_new ? "new" : "existing"), tsk_null);
#endif
                    }
                }

                // "a=acap:crypto" or "a=crypto"
                for(ctx_idx = 0; ctx_idx < ctx_count; ++ctx_idx) {
                    if(acap_tag_crypro_start == 0) {
                        acap_tag_crypro_start = (acap_tag == 1 ? acap_tag : ++acap_tag);
                    }
                    if(negotiate_srtp) {
                        tsk_sprintf(&str, "%d crypto:%d %s inline:%s", acap_tag++, srtp_ctxs[ctx_idx]->rtp.tag, trtp_srtp_crypto_type_strings[srtp_ctxs[ctx_idx]->rtp.crypto_type], srtp_ctxs[ctx_idx]->rtp.key_str);
                        cryptoA = tsdp_header_A_create("acap", str);
                    }
                    else {
                        tsk_sprintf(&str, "%d %s inline:%s", srtp_ctxs[ctx_idx]->rtp.tag, trtp_srtp_crypto_type_strings[srtp_ctxs[ctx_idx]->rtp.crypto_type], srtp_ctxs[ctx_idx]->rtp.key_str);
                        cryptoA = tsdp_header_A_create("crypto", str);
                    }

                    tsdp_header_M_add(base->M.lo, (const tsdp_header_t*)cryptoA);

                    TSK_OBJECT_SAFE_FREE(cryptoA);
                }

                // a=pcfg:
                if(negotiate_srtp || negotiate_avpf) {
                    tsk_size_t pcfg_tag, pcfg_idx;
                    char *acap_crypto = tsk_null, *acap_dtls = tsk_null;

                    // crypto
                    for(ctx_idx = 0; ctx_idx < ctx_count; ++ctx_idx) {
                        tsk_strcat_2(&acap_crypto, "%s%d",
                                     acap_crypto ? "," : "",
                                     (acap_tag_crypro_start + ctx_idx)
                                    );
                    }

                    // dtls setup and fingerprints
                    if(acap_tag_setup > 0 && acap_tag_connection > 0 && (acap_tag_fp_sha1 > 0 || acap_tag_fp_sha256 > 0)) {
                        tsk_sprintf(&acap_dtls, "%d,%d", acap_tag_setup, acap_tag_connection);
                        if(acap_tag_fp_sha1 > 0) {
                            tsk_strcat_2(&acap_dtls, ",%d", acap_tag_fp_sha1);
                        }
                        if(acap_tag_fp_sha256 > 0) {
                            tsk_strcat_2(&acap_dtls, "%s%d", acap_tag_fp_sha1 > 0 ? "|" : ",", acap_tag_fp_sha256);
                        }
                    }

                    for(pcfg_tag = 1, pcfg_idx = 0; pcfg_idx < profiles_index; ++pcfg_tag, ++pcfg_idx) {
                        if(((profiles[pcfg_idx] & RTP_PROFILE_SECURE_SDES) == RTP_PROFILE_SECURE_SDES) && acap_crypto) {
                            tsk_sprintf(&str, "%d t=%d a=%s", pcfg_tag, pcfg_tag, acap_crypto);
                        }
                        else if(((profiles[pcfg_idx] & RTP_PROFILE_SECURE_DTLS) == RTP_PROFILE_SECURE_DTLS) && acap_dtls) {
                            tsk_sprintf(&str, "%d t=%d a=%s", pcfg_tag, pcfg_tag, acap_dtls);
                        }
                        else {
                            tsk_sprintf(&str, "%d t=%d", pcfg_tag, pcfg_tag);
                        }
                        tsdp_header_M_add_headers(base->M.lo, TSDP_HEADER_A_VA_ARGS("pcfg", str), tsk_null);
                    }

                    TSK_FREE(acap_crypto);
                    TSK_FREE(acap_dtls);
                }

                if(is_srtp_remote_mandatory || is_srtp_local_mandatory || trtp_srtp_is_initialized(self->rtp_manager)) {
                    self->use_srtp = tsk_true;
                }

                /* RFC 5939 - Session Description Protocol (SDP) Capability Negotiation */
                {
                    sdp_acaps_xt acaps;
                    sdp_tcaps_xt tcaps;

                    _sdp_acaps_reset(&acaps);
                    _sdp_tcaps_reset(&tcaps);
                    _sdp_pcfgs_reset(&self->sdp_caps->local);
                    // session-level attributes
                    if(self->local_sdp) {
                        _sdp_pcfgs_from_sdp(self->local_sdp, &acaps, &tcaps, &self->sdp_caps->local, tsk_false);
                    }
                    // media-level attributes
                    _sdp_pcfgs_from_sdp(base->M.lo, &acaps, &tcaps, &self->sdp_caps->local, tsk_false);
                }

                TSK_FREE(str);
            } //end-of-if(!base->M.ro)
            else {
                // === RO IS NOT NULL ===
                // the ro validity has been checked in "set_ro()"
                RTP_PROFILE_T profile_remote = (self->sdp_caps->acfg.tag > 0 && self->sdp_caps->acfg.tcap.tag > 0)
                                               ? self->sdp_caps->acfg.tcap.profile
                                               : _sdp_profile_from_string(base->M.ro->proto);
                tsk_bool_t is_srtp_sdes_activated = tsk_false, is_srtp_dtls_activated = tsk_false;

                // intersect remote and local SRTP options
                if (self->avpf_mode_neg == tmedia_mode_optional && ((profile_remote & RTP_PROFILE_AVPF) == RTP_PROFILE_AVPF)) {
                    self->avpf_mode_neg = tmedia_mode_mandatory;
                }
                is_srtp_sdes_enabled &= ((profile_remote & RTP_PROFILE_SECURE_SDES) == RTP_PROFILE_SECURE_SDES);
                is_srtp_dtls_enabled &= ((profile_remote & RTP_PROFILE_SECURE_DTLS) == RTP_PROFILE_SECURE_DTLS);


                // SDES-SRTP
                if(is_srtp_sdes_enabled) {
                    const trtp_srtp_ctx_xt *srtp_ctxs[SRTP_CRYPTO_TYPES_MAX] = { tsk_null };
                    tsk_size_t ctx_count = 0, ctx_idx;
                    // get local SRTP context
                    if((ctx_count = trtp_srtp_get_local_contexts(self->rtp_manager, (const trtp_srtp_ctx_xt **)&srtp_ctxs, sizeof(srtp_ctxs)/sizeof(srtp_ctxs[0]))) > 0) {
                        char* str = tsk_null;
                        for(ctx_idx = 0; ctx_idx < ctx_count; ++ctx_idx) {
                            is_srtp_sdes_activated = tsk_true;
                            tsk_sprintf(&str, "%d %s inline:%s", srtp_ctxs[ctx_idx]->rtp.tag, trtp_srtp_crypto_type_strings[srtp_ctxs[ctx_idx]->rtp.crypto_type], srtp_ctxs[ctx_idx]->rtp.key_str);
                            tsdp_header_M_add_headers(base->M.lo, TSDP_HEADER_A_VA_ARGS("crypto", str), tsk_null);
                        }
                        TSK_FREE(str);
                    }
                }

                // DTLS-SRTP
                if(!is_srtp_sdes_activated && is_srtp_dtls_enabled) {
                    // get "fingerprint", "setup" and "connection" attributes
                    if(!tsk_strnullORempty(TMEDIA_SESSION(self)->dtls.file_pbk)) {
                        tnet_dtls_hash_type_t fp_hash_remote;
                        char* str = tsk_null;
                        if((fp_hash_remote = trtp_manager_get_dtls_remote_fingerprint_hash(self->rtp_manager)) == tnet_dtls_hash_type_none) {
                            fp_hash_remote = TDAV_DFAULT_FP_HASH;
                        }
                        tsk_sprintf(&str, "%s %s", TNET_DTLS_HASH_NAMES[fp_hash_remote], trtp_manager_get_dtls_local_fingerprint(self->rtp_manager, fp_hash_remote));
#if TDAV_FIXME_MEDIA_LEVEL_DTLS_ATT
                        tsdp_header_M_add_headers(base->M.lo, TSDP_HEADER_A_VA_ARGS("fingerprint", str), tsk_null);
#else
                        _first_media_add_headers(self->local_sdp, TSDP_HEADER_A_VA_ARGS("fingerprint", str), tsk_null);
#endif /* TDAV_FIXME_MEDIA_LEVEL_DTLS_ATT */
                        TSK_FREE(str);
                    }
#if TDAV_FIXME_MEDIA_LEVEL_DTLS_ATT
                    tsdp_header_M_add_headers(base->M.lo, TSDP_HEADER_A_VA_ARGS("setup", TNET_DTLS_SETUP_NAMES[self->dtls.local.setup]), tsk_null);
#if TDAV_DTLS_CONNECTION_ATT
                    tsdp_header_M_add_headers(base->M.lo, TSDP_HEADER_A_VA_ARGS("connection", self->dtls.local.connection_new ? "new" : "existing"), tsk_null);
#endif /* TDAV_DTLS_CONNECTION_ATT */
#else
                    _first_media_add_headers(self->local_sdp, TSDP_HEADER_A_VA_ARGS("setup", TNET_DTLS_SETUP_NAMES[self->dtls.local.setup]), tsk_null);
#if TDAV_FIXME_MEDIA_LEVEL_DTLS_ATT
                    _first_media_add_headers(self->local_sdp, TSDP_HEADER_A_VA_ARGS("connection", self->dtls.local.connection_new ? "new" : "existing"), tsk_null);
#endif /* TDAV_FIXME_MEDIA_LEVEL_DTLS_ATT */
#endif /* TDAV_FIXME_MEDIA_LEVEL_DTLS_ATT */

                    is_srtp_dtls_activated = tsk_true;
                }

                // activate the right SRTP type and disable others
                trtp_manager_set_srtp_type_remote(self->rtp_manager,
                                                  is_srtp_sdes_activated ? tmedia_srtp_type_sdes : (is_srtp_dtls_activated ? tmedia_srtp_type_dtls : tmedia_srtp_type_none));

                self->use_srtp |= (is_srtp_sdes_activated || is_srtp_dtls_activated);
                is_srtp_sdes_enabled &= is_srtp_sdes_activated;
                is_srtp_dtls_enabled &= is_srtp_dtls_activated;

            }//end-of-else
        }//end-of-HAVE_SRTP
#endif /* HAVE_SRTP */


        /* RFC 5939: acfg */
        if(self->sdp_caps->acfg.tag > 0) {
            _sdp_acfg_to_sdp(base->M.lo, &self->sdp_caps->acfg);
        }

        /* Hold/Resume */
#if 0
        // BFCP sessions send media but not expected to receive any data.
        // TODO: Radvision ignores "sendonly" and use the bfcp session as receiver for the mixed stream
        tsdp_header_M_set_holdresume_att(base->M.lo, (base->lo_held | is_bfcp_session), base->ro_held);
#else
        tsdp_header_M_set_holdresume_att(base->M.lo, base->lo_held, base->ro_held);
#endif

        /* Update Proto*/
        tsk_strupdate(&base->M.lo->proto,
                      self->use_srtp
                      ? ((self->avpf_mode_neg == tmedia_mode_mandatory) ? (is_srtp_dtls_enabled ? "UDP/TLS/RTP/SAVPF" : "RTP/SAVPF") : (is_srtp_dtls_enabled ? "UDP/TLS/RTP/SAVP" : "RTP/SAVP"))
                          : ((self->avpf_mode_neg == tmedia_mode_mandatory) ? "RTP/AVPF" : "RTP/AVP")
                         );

        // RFC 5761: RTCP/RTP muxing
        if(self->use_rtcpmux) {
            tsdp_header_M_add_headers(base->M.lo, TSDP_HEADER_A_VA_ARGS("rtcp-mux", tsk_null), tsk_null);
        }

        // draft-lennox-mmusic-sdp-source-attributes-01
        if((self->media_type & tmedia_audio) || (self->media_type & tmedia_video)  || ((self->media_type & tmedia_bfcp_video) == tmedia_bfcp_video)) {
            char* str = tsk_null;
            tsk_sprintf(&str, "%u cname:%s", self->rtp_manager->rtp.ssrc.local, self->rtp_manager->rtcp.cname); // also defined in RTCP session
            tsdp_header_M_add_headers(base->M.lo, TSDP_HEADER_A_VA_ARGS("ssrc", str), tsk_null);
            tsk_sprintf(&str, "%u mslabel:%s", self->rtp_manager->rtp.ssrc.local, "6994f7d1-6ce9-4fbd-acfd-84e5131ca2e2");
            tsdp_header_M_add_headers(base->M.lo, TSDP_HEADER_A_VA_ARGS("ssrc", str), tsk_null);
            tsk_sprintf(&str, "%u label:%s", self->rtp_manager->rtp.ssrc.local, (self->media_type & tmedia_audio) ? ((base->type & tmedia_bfcp) ? "doubango@bfcpaudio" : "doubango@audio") : ((base->type & tmedia_bfcp) ? "doubango@bfcpvideo" : "doubango@video")); /* https://groups.google.com/group/discuss-webrtc/browse_thread/thread/6c44106c8ce7d6dc */
            tsdp_header_M_add_headers(base->M.lo, TSDP_HEADER_A_VA_ARGS("ssrc", str), tsk_null);
            TSK_FREE(str);
        }

        /* ICE */
        if(self->ice_ctx) {
            tsk_size_t index = 0;
            const tnet_ice_candidate_t* candidate;
            tsk_bool_t remote_use_rtcpmux = (base->M.ro && (tsdp_header_M_findA(base->M.ro, "rtcp-mux") != tsk_null));

            // FIXME: for RTCP, use "RFC 3605"in addition to "rtcp-mux"

            // "a=ice-mismatch" if "C=" line is not included in the candidates
            if ((candidate = tnet_ice_ctx_get_local_candidate_first(self->ice_ctx))) { // at least one candidate
                base->M.lo->port = candidate->socket->port;

                tsdp_header_M_remove(base->M.lo, tsdp_htype_C);
                tsdp_header_M_add_headers(base->M.lo,
                                          TSDP_HEADER_C_VA_ARGS("IN", TNET_SOCKET_TYPE_IS_IPV6(candidate->socket->type) ? "IP6" : "IP4", candidate->socket->ip),
                                          tsk_null);
                tsdp_header_M_add_headers(base->M.lo,
                                          TSDP_HEADER_A_VA_ARGS("ice-ufrag", candidate->ufrag),
                                          TSDP_HEADER_A_VA_ARGS("ice-pwd", candidate->pwd),
                                          tsk_null);
                // RTCWeb
                // "mid:" must not added without BUNDLE
                // tsdp_header_M_add_headers(base->M.lo,
                //	TSDP_HEADER_A_VA_ARGS("mid", self->media_type & tmedia_audio ? "audio" : "video"),
                //		tsk_null);

                while ((candidate = tnet_ice_ctx_get_local_candidate_at(self->ice_ctx, index++))) {
                    if (self->use_rtcpmux && remote_use_rtcpmux && candidate->comp_id == TNET_ICE_CANDIDATE_COMPID_RTCP) {
                        continue; // do not add RTCP candidates if RTCP-MUX is activated (local + remote)
                    }
#if 0 //TURN:FORCE
                    if (candidate->type_e != tnet_ice_cand_type_relay) {
                        continue;
                    }
#endif
                    tsdp_header_M_add_headers(base->M.lo,
                                              TSDP_HEADER_A_VA_ARGS("candidate", tnet_ice_candidate_tostring((tnet_ice_candidate_t*)candidate)),
                                              tsk_null);
                }
            }
        }
        else {
            if(base->M.lo->C) {
                tsk_strupdate(&base->M.lo->C->addr, self->rtp_manager->rtp.public_addr.ip);
                tsk_strupdate(&base->M.lo->C->addrtype, (TNET_SOCKET_TYPE_IS_IPV6(self->rtp_manager->rtp.public_addr.type) ? "IP6" : "IP4"));
            }
            base->M.lo->port = self->rtp_manager->rtp.public_addr.port;
        }

        if(self->media_type & tmedia_audio) {
            ///* 3GPP TS 24.229 - 6.1.1 General
            //	The UE shall include the MIME subtype "telephone-event" in the "m=" media descriptor in the SDP for audio media
            //	flows that support both audio codec and DTMF payloads in RTP packets as described in RFC 4733 [23].
            //*/
            //tsdp_header_M_add_fmt(base->M.lo, TMEDIA_CODEC_FORMAT_DTMF);
            //tsdp_header_M_add_headers(base->M.lo,
            //			TSDP_HEADER_A_VA_ARGS("fmtp", TMEDIA_CODEC_FORMAT_DTMF" 0-15"),
            //		tsk_null);
            //tsdp_header_M_add_headers(base->M.lo,
            //			TSDP_HEADER_A_VA_ARGS("rtpmap", TMEDIA_CODEC_FORMAT_DTMF" telephone-event/8000"),
            //		tsk_null);
        }

        /* QoS */
        if(base->qos) {
            tmedia_qos_tline_t* ro_tline;
            if(base->M.ro && (ro_tline = tmedia_qos_tline_from_sdp(base->M.ro))) {
                tmedia_qos_tline_set_ro(base->qos, ro_tline);
                TSK_OBJECT_SAFE_FREE(ro_tline);
            }
            tmedia_qos_tline_to_sdp(base->qos, base->M.lo);
        }
DONE:
        ;
    } // end-of-if(*updated)

    return base->M.lo;
}

int tdav_session_av_set_ro(tdav_session_av_t* self, const struct tsdp_header_M_s* m, tsk_bool_t *updated)
{
    tmedia_codecs_L_t* neg_codecs;
    tsk_bool_t srtp_sdes_neg_ok = tsk_false, srtp_dtls_neg_ok = tsk_false;
    tsk_bool_t is_srtp_remote_mandatory, is_srtp_dtls_remote_mandatory, is_srtp_sdes_remote_mandatory;
    tsk_bool_t is_srtp_local_mandatory, is_srtp_dtls_local_enabled, is_srtp_sdes_local_enabled;
    tmedia_session_t* base = TMEDIA_SESSION(self);
    RTP_PROFILE_T profile_remote;
    int32_t acfg_idx = -1;

    if(!base || !m || !updated) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if(!self->rtp_manager) {
        TSK_DEBUG_ERROR("RTP manager is null. Did you forget to prepare the session?");
        return -1;
    }

    /* update remote offer */
    TSK_OBJECT_SAFE_FREE(base->M.ro);
    base->M.ro = tsk_object_ref((void*)m);

    *updated = tsk_false;

    // check if the RTP profile from remote party is supported or not
    if((profile_remote = _sdp_profile_from_string(m->proto)) == RTP_PROFILE_NONE) {
        TSK_DEBUG_ERROR("%s not supported as RTP profile", m->proto);
        return -2;
    }
    // check that all options in the profile are supported
    if((is_srtp_remote_mandatory = (profile_remote & RTP_PROFILE_SECURE))  && (self->srtp_mode == tmedia_srtp_mode_none)) {
        TSK_DEBUG_ERROR("Remote party requesting secure transport (%s) but this option is not enabled", m->proto);
        return -2;
    }
    if((is_srtp_sdes_remote_mandatory = (profile_remote & RTP_PROFILE_SECURE_SDES) == RTP_PROFILE_SECURE_SDES) && !(self->srtp_type & tmedia_srtp_type_sdes)) {
        TSK_DEBUG_ERROR("Remote party requesting SRTP-SDES (%s) but this option is not enabled", m->proto);
        return -2;
    }
    if((is_srtp_dtls_remote_mandatory = (profile_remote & RTP_PROFILE_SECURE_DTLS) == RTP_PROFILE_SECURE_DTLS) && !(self->srtp_type & tmedia_srtp_type_dtls)) {
        TSK_DEBUG_ERROR("Remote party requesting DTLS-DTLS (%s) but this option is not enabled", m->proto);
        return -2;
    }
    is_srtp_local_mandatory = (self->srtp_mode == tmedia_srtp_mode_mandatory) && (self->srtp_type != tmedia_srtp_type_none);
    is_srtp_dtls_local_enabled = (self->srtp_mode != tmedia_srtp_mode_none) && (self->srtp_type & tmedia_srtp_type_dtls);
    is_srtp_sdes_local_enabled = (self->srtp_mode != tmedia_srtp_mode_none) && (self->srtp_type & tmedia_srtp_type_sdes);

    if (base->M.lo) {
        if ((neg_codecs = tmedia_session_match_codec(base, m))) {
            /* update negociated codecs */
            TSK_OBJECT_SAFE_FREE(base->neg_codecs);
            base->neg_codecs = neg_codecs;
            *updated = tsk_true;
        }
        else {
            TSK_DEBUG_ERROR("Codecs mismatch");
            return -1;
        }
        /* QoS */
        if (base->qos) {
            tmedia_qos_tline_t* ro_tline;
            if (base->M.ro && (ro_tline = tmedia_qos_tline_from_sdp(base->M.ro))) {
                tmedia_qos_tline_set_ro(base->qos, ro_tline);
                TSK_OBJECT_SAFE_FREE(ro_tline);
            }
        }
    }

    /* AVPF */
    if(self->avpf_mode_set == tmedia_mode_optional && self->avpf_mode_neg != tmedia_mode_mandatory) {
        self->avpf_mode_neg = _sdp_str_contains(base->M.ro->proto, "AVPF") ? tmedia_mode_mandatory : tmedia_mode_none;
    }

    /* RFC 5939 - Session Description Protocol (SDP) Capability Negotiation */
    {
        sdp_acaps_xt acaps;
        sdp_tcaps_xt tcaps;

        _sdp_acfg_reset(&self->sdp_caps->acfg);

        _sdp_acaps_reset(&acaps);
        _sdp_tcaps_reset(&tcaps);
        _sdp_pcfgs_reset(&self->sdp_caps->remote);

        // session-level attributes
        if(self->remote_sdp) {
            _sdp_pcfgs_from_sdp(self->remote_sdp, &acaps, &tcaps, &self->sdp_caps->remote, tsk_false);
        }
        // media-level attributes
        _sdp_pcfgs_from_sdp(base->M.ro, &acaps, &tcaps, &self->sdp_caps->remote, tsk_false);
    }

    /* get connection associated to this media line
     * If the connnection is global, then the manager will call tdav_session_audio_set() */
    if(m->C && m->C->addr) {
        tsk_strupdate(&self->remote_ip, m->C->addr);
        self->use_ipv6 = tsk_striequals(m->C->addrtype, "IP6");
    }
    /* set remote port */
    self->remote_port = m->port;

    /* RTCP-MUX */
    self->use_rtcpmux &= (tsdp_header_M_findA(m, "rtcp-mux") != tsk_null);
    if(self->ice_ctx) {
        tnet_ice_ctx_set_rtcpmux(self->ice_ctx, self->use_rtcpmux);
    }

    // BANDWIDTH:
    // http://tools.ietf.org/html/rfc3556
    // https://tools.ietf.org/html/rfc3890
    {
        if (!TSK_LIST_IS_EMPTY(m->Bandwidths)) {
            const tsk_list_item_t* itemB;
            const tsdp_header_B_t* B;
            int32_t unit_div;
            tsk_list_foreach(itemB, m->Bandwidths) {
                if(!(B = (const tsdp_header_B_t*)itemB->data)) {
                    continue;
                }
                TSK_DEBUG_INFO("Remote party requested bandwidth limitation at %u using 'b=%s' SDP attribute", B->bandwidth, B->bwtype);
                unit_div = tsk_striequals(B->bwtype, "AS") ? 1 : (tsk_striequals(B->bwtype, "TIAS") ? 1024 : 0);
                if (unit_div) {
                    TSK_DEBUG_INFO("Setting bandwidth_max_upload_kbps=%u according to remote party request", B->bandwidth);
                    self->bandwidth_max_upload_kbps = (B->bandwidth/unit_div);
                    break;
                }
            }
        }
    }

    /* Remote SSRC */
    {
        // will be also updated based on received RTP packets
        const tsdp_header_A_t* ssrcA = tsdp_header_M_findA(m, "ssrc");
        if(ssrcA && ssrcA->value) {
            if(sscanf(ssrcA->value, "%u %*s", &self->rtp_manager->rtp.ssrc.remote) != EOF) {
                TSK_DEBUG_INFO("Remote SSRC = %u", self->rtp_manager->rtp.ssrc.remote);
            }
        }
    }

    /* RTCWeb Type */
    if(self->remote_sdp) {
        const tsdp_header_S_t* S = (const tsdp_header_S_t*)tsdp_message_get_header(self->remote_sdp, tsdp_htype_S);
        if(S && !tsk_strnullORempty(S->value)) {
            struct rtcweb_type {
                const char* name;
                tmedia_rtcweb_type_t type;
            };
            static const struct rtcweb_type rtcweb_types[] = {
                { "firefox", tmedia_rtcweb_type_firefox },
                { "chrome", tmedia_rtcweb_type_chrome },
                { "bowser", tmedia_rtcweb_type_ericsson },
                { "doubango", tmedia_rtcweb_type_doubango },
            };
            static const int32_t rtcweb_types_count = sizeof(rtcweb_types)/sizeof(rtcweb_types[0]);
            int32_t i;
            for(i = 0; i < rtcweb_types_count; ++i) {
                if(_sdp_str_contains(S->value, rtcweb_types[i].name)) {
                    trtp_manager_set_rtcweb_type_remote(self->rtp_manager, rtcweb_types[i].type);
                    break;
                }
            }
        }
    }

    /* SRTP */
#if HAVE_SRTP
    // this is SRTP negotiation -> do not trust the remote profile
    if(is_srtp_dtls_local_enabled || is_srtp_sdes_local_enabled) {
        int32_t i, j;
        const sdp_acap_xt *acap;
        int ret;
        if(is_srtp_sdes_local_enabled) {
            const tsdp_header_A_t* A;
            const char* cryptos[2] = { tsk_null };

            /* 1. check crypto lines from the SDP */
            i = 0;
            while((A = tsdp_header_M_findA_at(m, "crypto", i))) {
                cryptos[i++] = A->value;
                if(i >= (sizeof(cryptos)/sizeof(cryptos[0]))) {
                    break;
                }
            }

            /* 2. check crypto lines from the caps (RFC 5939)*/
            if(!cryptos[0]) {
                tsk_size_t k = 0;
                for(i = 0; (i < SDP_CAPS_COUNT_MAX && self->sdp_caps->remote[i].tag > 0); ++i) {
                    j = 0;
                    while((acap = _sdp_acaps_find_by_field(&self->sdp_caps->remote[i].acaps, "crypto", j++))) {
                        if(k < (sizeof(cryptos)/sizeof(cryptos[0]))) {
                            // remove "crypto: "
                            if((cryptos[k] = strstr(acap->value, ":")) && ++cryptos[k]) {
                                while(isspace(*cryptos[k])) {
                                    ++cryptos[k];
                                }
                                ++k;
                            }
                        }
                    }
                    if(k != 0) { // do not mix crypto lines from different pcfgs
                        acfg_idx = i;
                        break;
                    }
                }
            }

            /* 3. match cryptos */
            for(i = 0; i< sizeof(cryptos)/sizeof(cryptos[0]); ++i) {
                if(!cryptos[i]) {
                    break;
                }
                if((ret = trtp_srtp_set_crypto_remote(self->rtp_manager, cryptos[i])) == 0) {
                    srtp_sdes_neg_ok = tsk_true;
                    break;
                }
            }
        } // end-of-sdes

        if(!srtp_sdes_neg_ok && is_srtp_dtls_local_enabled) {
            int ret;
            const tsdp_header_A_t *setupA = tsk_null, *fpA = tsk_null, *connectionA = tsk_null;
            const char* fingerprints[4] = { tsk_null };
            const char* setups[4] = { tsk_null };
            const char* connections[4] = { tsk_null };
            const char* connection = tsk_null;
            const char* setup = tsk_null;

            /* 1. check DTLS attributes from the SDP */

            if(self->remote_sdp) {
                setupA = tsdp_message_get_headerA(self->remote_sdp, "setup");
                fpA = tsdp_message_get_headerA(self->remote_sdp, "fingerprint");
                connectionA = tsdp_message_get_headerA(self->remote_sdp, "connection");
            }
            if(!setupA) {
                setupA = tsdp_header_M_findA(m, "setup");
            }
            if(!fpA) {
                fpA = tsdp_header_M_findA(m, "fingerprint");
            }
            if(!connectionA) {
                connectionA = tsdp_header_M_findA(m, "connection");
            }

            if(setupA) {
                setups[0] = setupA->value;
            }
            if(fpA) {
                fingerprints[0] = fpA->value;
            }
            if(connectionA) {
                connections[0] = connectionA->value;
            }

            /* 2. check DTLS attributes from from the caps (RFC 5939) */
            if(!srtp_dtls_neg_ok && !fingerprints[0]) {
                tsk_size_t k_fp = 0, k_st = 0, k_conn = 0;
                for(i = 0; (i < SDP_CAPS_COUNT_MAX && self->sdp_caps->remote[i].tag > 0); ++i) {
                    // "fingerprint"
                    j = 0;
                    while((acap = _sdp_acaps_find_by_field(&self->sdp_caps->remote[i].acaps, "fingerprint", j++))) {
                        if(k_fp < (sizeof(fingerprints)/sizeof(fingerprints[0]))) {
                            if((fingerprints[k_fp] = strstr(acap->value, ":")) && ++fingerprints[k_fp]) {
                                while(isspace(*fingerprints[k_fp])) {
                                    ++fingerprints[k_fp];
                                }
                                ++k_fp;
                            }
                        }
                    }
                    // "setup"
                    j = 0;
                    while((acap = _sdp_acaps_find_by_field(&self->sdp_caps->remote[i].acaps, "setup", j++))) {
                        if(k_st < (sizeof(setups)/sizeof(setups[0]))) {
                            if((setups[k_st] = strstr(acap->value, ":")) && ++setups[k_st]) {
                                while(isspace(*setups[k_st])) {
                                    ++setups[k_st];
                                }
                                ++k_st;
                            }
                        }
                    }
                    // "connection"
                    j = 0;
                    while((acap = _sdp_acaps_find_by_field(&self->sdp_caps->remote[i].acaps, "connection", j++))) {
                        if(k_conn < (sizeof(connections)/sizeof(connections[0]))) {
                            if((connections[k_conn] = strstr(acap->value, ":")) && ++connections[k_conn]) {
                                while(isspace(*connections[k_conn])) {
                                    ++connections[k_conn];
                                }
                                ++k_conn;
                            }
                        }
                    }

                    if(k_fp || k_st || k_conn) { // do not mix crypto lines from different pcfgs
                        acfg_idx = i;
                        break;
                    }
                }
            }

            /* 3. setup DTLS connection using negotiated attributes */

            if(!srtp_dtls_neg_ok && fingerprints[0]) {
                tnet_fingerprint_t fingerprint;
                char hash[16];
                for(i = 0; i < sizeof(fingerprints)/sizeof(fingerprints[0]) && !srtp_dtls_neg_ok; ++i) {
                    if(!fingerprints[i]) {
                        break;
                    }
                    if(sscanf(fingerprints[i], "%15s %255s", hash, fingerprint) >= 2) {
                        if((ret = trtp_manager_set_dtls_remote_fingerprint(self->rtp_manager, &fingerprint, hash)) == 0) {
                            acfg_idx = i;
                            srtp_dtls_neg_ok = tsk_true;
                            break;
                        }
                    }
                }
            }

            // only accept sdp without fingerprints if certificate verification is OFF
            if(!srtp_dtls_neg_ok && !fingerprints[0] && !TMEDIA_SESSION(self)->dtls.verify) {
                for(i = 0; (i < SDP_CAPS_COUNT_MAX && self->sdp_caps->remote[i].tag > 0); ++i) {
                    if(self->sdp_caps->remote[i].tcap.tag > 0 && (self->sdp_caps->remote[i].tcap.profile & RTP_PROFILE_SECURE_DTLS) == RTP_PROFILE_SECURE_DTLS) {
                        acfg_idx = i;
                        break;
                    }
                }
            }

            // defaults
            if(!connection) {
                connection = connections[acfg_idx != -1 ? acfg_idx : 0];
            }
            if(!setup) {
                setup = setups[acfg_idx != -1 ? acfg_idx : 0];
            }

            if((!connection || !setup) && (is_srtp_dtls_remote_mandatory || is_srtp_local_mandatory || (acfg_idx != -1 && (self->sdp_caps->remote[acfg_idx].tcap.profile & RTP_PROFILE_SECURE_DTLS) == RTP_PROFILE_SECURE_DTLS))) {
                if(!connection) {
                    connection = (self->dtls.local.connection_new ? "existing" : "new");
                }
                if(!setup) setup = (self->dtls.local.setup == tnet_dtls_setup_active
                                        ? "passive"
                                        : (self->dtls.local.setup == tnet_dtls_setup_passive ? "active" : (base->M.lo ? "passive" : "active")));
            }

            if (connection && setup) {
                // update local setup according to remote setup
                // do not update if local setup already negotiated
                if (tnet_dtls_get_setup_from_string(setup) != tnet_dtls_setup_actpass || (self->dtls.local.setup == tnet_dtls_setup_none || self->dtls.local.setup == tnet_dtls_setup_actpass)) {
                    ret = _tdav_session_av_dtls_set_remote_setup(self,
                            tnet_dtls_get_setup_from_string(setup),
                            !tsk_striequals(connection, "existing"),
                            (!base->M.ro)
                                                                );
                }
                if (ret == 0) {
                    // pass new local values to the RTP manager
                    ret = trtp_manager_set_dtls_local_setup(self->rtp_manager, self->dtls.local.setup, self->dtls.local.connection_new);
                    srtp_dtls_neg_ok = (ret == 0);
                }
            }

        }// end-of-dtls
    }//end-of-if(srtp=optional|mandatory)

    self->use_srtp = trtp_srtp_is_initialized(self->rtp_manager);

    // activate the right SRTP type and disable others
    trtp_manager_set_srtp_type_remote(self->rtp_manager,
                                      srtp_sdes_neg_ok ? tmedia_srtp_type_sdes : (srtp_dtls_neg_ok ? tmedia_srtp_type_dtls : tmedia_srtp_type_none));

#endif

    // set actual config
    if(acfg_idx == -1) {
        // none matched (means SRTP negotiation failed or not enabled -> try to negotiate AVP(F))
        int32_t i;
        for(i = 0; (i < SDP_CAPS_COUNT_MAX && self->sdp_caps->remote[i].tag > 0); ++i) {
            if(self->sdp_caps->remote[i].tcap.tag > 0) {
                if((self->sdp_caps->remote[i].tcap.profile & RTP_PROFILE_AVPF) == RTP_PROFILE_AVPF) {
                    acfg_idx = i;
                    break;
                }
            }
        }
    }
    if(acfg_idx != -1) {
        self->sdp_caps->acfg = self->sdp_caps->remote[acfg_idx];
        if (self->avpf_mode_set == tmedia_mode_optional && self->avpf_mode_neg != tmedia_mode_mandatory) {
            self->avpf_mode_neg = ((self->sdp_caps->acfg.tcap.profile & RTP_PROFILE_AVPF) == RTP_PROFILE_AVPF) ? tmedia_mode_mandatory : tmedia_mode_none;
        }
    }

    if(!srtp_sdes_neg_ok && !srtp_dtls_neg_ok && (is_srtp_remote_mandatory || is_srtp_local_mandatory)) {
        TSK_DEBUG_ERROR("SRTP negotiation failed");
        return -4;
    }

    return 0;
}

const tmedia_codec_t* tdav_session_av_get_best_neg_codec(const tdav_session_av_t* self)
{
    const tsk_list_item_t* item;
    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }

    tsk_list_foreach(item, TMEDIA_SESSION(self)->neg_codecs) {
        // exclude DTMF, RED and ULPFEC
        if (!TDAV_IS_DTMF_CODEC(item->data) && !TDAV_IS_ULPFEC_CODEC(item->data) && !TDAV_IS_RED_CODEC(item->data)
                && TMEDIA_CODEC(item->data)->plugin && TMEDIA_CODEC(item->data)->plugin->encode && TMEDIA_CODEC(item->data)->plugin->decode) {
            return TMEDIA_CODEC(item->data);
        }
    }
    return tsk_null;
}

const tmedia_codec_t* tdav_session_av_get_red_codec(const tdav_session_av_t* self)
{
    const tsk_list_item_t* item;
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }

    tsk_list_foreach(item, TMEDIA_SESSION(self)->neg_codecs) {
        const tmedia_codec_t* codec = (const tmedia_codec_t*)item->data;
        if(TDAV_IS_RED_CODEC(codec)) {
            return TMEDIA_CODEC(item->data);
        }
    }
    return tsk_null;
}

static void* TSK_STDCALL _tdav_session_av_error_async_thread(void* usrdata)
{
    if(usrdata) {
        tdav_session_av_t* self = (tdav_session_av_t*)usrdata;
        tsk_safeobj_lock(self);
        if(TMEDIA_SESSION(self)->onerror_cb.fun) {
            TMEDIA_SESSION(self)->onerror_cb.fun(TMEDIA_SESSION(self)->onerror_cb.usrdata, TMEDIA_SESSION(self), self->last_error.reason, self->last_error.is_fatal);
        }
        tsk_safeobj_unlock(self);
        tsk_object_unref(self); // see _tdav_session_av_raise_error_async()
    }
    return tsk_null;
}

static int _tdav_session_av_raise_error_async(struct tdav_session_av_s* self, tsk_bool_t is_fatal, const char* reason)
{
    int ret;
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tsk_safeobj_lock(self);

    tsk_object_ref(self); // do not unref(), see _tdav_session_av_error_async_thread()

    if (self->last_error.tid[0]) {
        tsk_thread_join(self->last_error.tid);
    }

    self->last_error.is_fatal = is_fatal;
    tsk_strupdate(&self->last_error.reason, reason);
    if ((ret = tsk_thread_create(self->last_error.tid, _tdav_session_av_error_async_thread, self)) != 0) {
        tsk_object_unref(self);
        goto bail;
    }

bail:
    tsk_safeobj_unlock(self);

    return ret;
}

#if HAVE_SRTP
static int _tdav_session_av_srtp_dtls_cb(const void* usrdata, enum trtp_srtp_dtls_event_type_e type, const char* reason)
{
    tdav_session_av_t* self = tsk_object_ref((tdav_session_av_t*)usrdata);

    tsk_safeobj_lock(self);
    switch(type) {
    case trtp_srtp_dtls_event_type_handshake_failed:
    case trtp_srtp_dtls_event_type_fatal_error: {
        if(TMEDIA_SESSION(self)->onerror_cb.fun) {
            static const tsk_bool_t __is_fatal = tsk_true;
            _tdav_session_av_raise_error_async(self, __is_fatal, reason);
        }
        break;
    }
    case trtp_srtp_dtls_event_type_handshake_succeed: {
        break;
    }
    case trtp_srtp_dtls_event_type_started: {
        // start producer and consumer
        if (self->rtp_manager && self->rtp_manager->is_started) {
            if (self->consumer && !self->consumer->is_started) {
                tmedia_consumer_start(self->consumer);
            }
            if (self->producer && !self->producer->is_started) {
                tmedia_producer_start(self->producer);
            }
        }
        break;
    }
    }
    tsk_safeobj_unlock(self);
    tsk_object_unref(self);

    return 0;
}
#endif /* HAVE_SRTP */

static int _tdav_session_av_red_cb(const void* usrdata, const struct trtp_rtp_packet_s* packet)
{
    tdav_session_av_t* self = (tdav_session_av_t*)usrdata;
    if(self->rtp_manager && self->rtp_manager->rtp.cb.fun) {
        return self->rtp_manager->rtp.cb.fun(self->rtp_manager->rtp.cb.usrdata, packet);
    }
    return 0;
}

int _tdav_session_av_dtls_set_remote_setup(struct tdav_session_av_s* self, tnet_dtls_setup_t setup, tsk_bool_t connection_new, tsk_bool_t is_ro_null)
{
    if(self) {
        TSK_DEBUG_INFO("dtls.remote.setup=%s", TNET_DTLS_SETUP_NAMES[(int)setup]);
        self->dtls.remote.setup = setup;
        self->dtls.remote.connection_new = connection_new;
        switch(self->dtls.remote.setup) {
        case tnet_dtls_setup_none:
        default:
            self->dtls.local.setup = tnet_dtls_setup_actpass;
            self->dtls.local.connection_new = tsk_true; // RTP transport always unprepared for reINVITE/UPDATE -> new connection
            break;
        case tnet_dtls_setup_active:
            self->dtls.local.setup = is_ro_null ? tnet_dtls_setup_actpass : tnet_dtls_setup_passive;
            self->dtls.local.connection_new = tsk_true;
            break;
        case tnet_dtls_setup_passive:
            self->dtls.local.setup = is_ro_null ? tnet_dtls_setup_actpass : tnet_dtls_setup_active;
            self->dtls.local.connection_new = tsk_true;
            break;
        case tnet_dtls_setup_actpass:
            if (self->dtls.local.setup == tnet_dtls_setup_actpass || self->dtls.local.setup == tnet_dtls_setup_none) { // change local setup only if actpass or none
                self->dtls.local.setup = (self->dtls.local.setup == tnet_dtls_setup_actpass || self->dtls.local.setup == tnet_dtls_setup_active)
                                         ? tnet_dtls_setup_active
                                         : tnet_dtls_setup_passive;
                self->dtls.local.connection_new = tsk_true;
            }
            break;
        }
    }
    return 0;
}

const tmedia_codec_t* tdav_session_av_get_ulpfec_codec(const tdav_session_av_t* self)
{
    const tsk_list_item_t* item;
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }

    tsk_list_foreach(item, TMEDIA_SESSION(self)->neg_codecs) {
        if(TDAV_IS_ULPFEC_CODEC(item->data)) {
            return TMEDIA_CODEC(item->data);
        }
    }
    return tsk_null;
}

int tdav_session_av_deinit(tdav_session_av_t* self)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    /* deinit self (rtp manager should be destroyed after the producer) */
    TSK_OBJECT_SAFE_FREE(self->consumer);
    TSK_OBJECT_SAFE_FREE(self->producer);
    TSK_OBJECT_SAFE_FREE(self->rtp_manager);
    TSK_OBJECT_SAFE_FREE(self->sdp_caps);
    TSK_OBJECT_SAFE_FREE(self->remote_sdp);
    TSK_OBJECT_SAFE_FREE(self->local_sdp);
    TSK_FREE(self->remote_ip);
    TSK_FREE(self->local_ip);

    /* RED and ULPFEC codecs */
    TSK_OBJECT_SAFE_FREE(self->red.codec);
    TSK_OBJECT_SAFE_FREE(self->ulpfec.codec);

    /* NAT Traversal context */
    TSK_OBJECT_SAFE_FREE(self->natt_ctx);
    TSK_OBJECT_SAFE_FREE(self->ice_ctx);

    /* Last error */
    if(self->last_error.tid[0]) {
        tsk_thread_join(self->last_error.tid);
    }
    TSK_FREE(self->last_error.reason);

    tsk_safeobj_deinit(self);

    /* deinit base */
    tmedia_session_deinit(TMEDIA_SESSION(self));

    return 0;
}









static const tsdp_header_A_t* _sdp_findA_at(const sdp_headerM_Or_Message* sdp, const char* field, tsk_size_t index)
{
    if(sdp) {
        if(TSK_OBJECT_HEADER(sdp)->__def__ == tsdp_message_def_t) {
            return tsdp_message_get_headerA_at((const tsdp_message_t*)sdp, field, index);
        }
        else if(TSK_OBJECT_HEADER(sdp)->__def__ == tsdp_header_M_def_t) {
            return tsdp_header_M_findA_at((const tsdp_header_M_t*)sdp, field, index);
        }
    }

    TSK_DEBUG_ERROR("Invalid parameter");
    return tsk_null;
}

static int _sdp_add_headerA(sdp_headerM_Or_Message* sdp, const char* field, const char* value)
{
    if(sdp && field) {
        if(TSK_OBJECT_HEADER(sdp)->__def__ == tsdp_message_def_t) {
            return tsdp_message_add_headers((tsdp_message_t*)sdp,
                                            TSDP_HEADER_A_VA_ARGS(field, value),
                                            tsk_null);
        }
        else if(TSK_OBJECT_HEADER(sdp)->__def__ == tsdp_header_M_def_t) {
            return tsdp_header_M_add_headers((tsdp_header_M_t*)sdp,
                                             TSDP_HEADER_A_VA_ARGS(field, value),
                                             tsk_null);
        }
    }

    TSK_DEBUG_ERROR("Invalid parameter");
    return -1;
}

static RTP_PROFILE_T _sdp_profile_from_string(const char* profile)
{
    int32_t i;
    for(i = 0; i < RTP_PROFILES_COUNT; ++i) {
        if(tsk_striequals(RTP_PROFILES[i].name, profile)) {
            return RTP_PROFILES[i].type;
        }
    }
    return RTP_PROFILE_NONE;
}

static const char* _sdp_profile_to_string(RTP_PROFILE_T profile)
{
    int32_t i;
    for(i = 0; i < RTP_PROFILES_COUNT; ++i) {
        if(RTP_PROFILES[i].type == profile) {
            return RTP_PROFILES[i].name;
        }
    }
    return tsk_null;
}

_SDP_DECLARE_INDEX_OF(acap);

static const sdp_acap_xt* _sdp_acaps_find_by_field(const sdp_acap_xt (*acaps)[SDP_CAPS_COUNT_MAX], const char* field, int32_t index)
{
    int32_t i, j, k, size;

    if(!acaps || !field) {
        TSK_DEBUG_ERROR("Invalid paramter");
        return tsk_null;
    }

    i = 0, j = 0;
    size = (int32_t)tsk_strlen(field);
    while((*acaps)[j].tag && j < SDP_CAPS_COUNT_MAX) {
        k = _sdp_str_index_of((*acaps)[j].value, field);
        if(k == 0 && (*acaps)[j].value[size] == ':') {
            if(i == index) {
                return &(*acaps)[j];
            }
            ++i;
        }
        ++j;
    }
    return tsk_null;
}

static int _sdp_acaps_from_sdp(const sdp_headerM_Or_Message* sdp, sdp_acap_xt (*acaps)[SDP_CAPS_COUNT_MAX], tsk_bool_t reset)
{
    tsk_size_t acaps_count, acaps_idx;
    const tsdp_header_A_t* A;
    int32_t tag, index, size;

    if(!sdp || !acaps) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }


    if(reset) {
        _sdp_acaps_reset(acaps);
        acaps_count = 0;
    }
    else {
        if((acaps_count = _sdp_acaps_indexof(acaps, 0)) == -1) {
            TSK_DEBUG_ERROR("No room to append items");
            return -1;
        }
    }

    acaps_idx = 0;
    while((A = _sdp_findA_at(sdp, "acap", acaps_idx++))) {
        if (!(size = (int32_t)tsk_strlen(A->value))) {
            goto next;
        }
        if(sscanf(A->value, "%d", &tag) == EOF) {
            TSK_DEBUG_ERROR("sscanf(%s) failed", A->value);
            break;
        }
        if(tag <= 0 || (tag + 1) > SDP_CAPS_COUNT_MAX) {
            TSK_DEBUG_WARN("Ignoring tag with value = %d", tag);
            goto next;
        }

        index = _sdp_integer_length(tag) + 1;/*SPACE*/
        if(index >= size) {
            TSK_DEBUG_WARN("a=%s is empty", A->value);
            goto next;
        }

        (*acaps)[acaps_count].tag = tag;
        (*acaps)[acaps_count].value = &A->value[index];
next:
        if(++acaps_count >= SDP_CAPS_COUNT_MAX) {
            break;
        }
    }

    return 0;
}

_SDP_DECLARE_INDEX_OF(tcap);

static int _sdp_tcaps_from_sdp(const sdp_headerM_Or_Message* sdp, sdp_tcap_xt (*tcaps)[SDP_CAPS_COUNT_MAX], tsk_bool_t reset)
{
    int32_t tcaps_count, tcaps_idx, profiles_count;
    const tsdp_header_A_t* A;
    int32_t tag, index, size, tag_fake;
    char tcap[256];

    if(!sdp || !tcaps) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if(reset) {
        _sdp_tcaps_reset(tcaps);
        tcaps_count = 0;
    }
    else {
        if((tcaps_count = _sdp_tcaps_indexof(tcaps, 0)) == -1) {
            TSK_DEBUG_ERROR("No room to append items");
            return -1;
        }
    }

    profiles_count = 0;
    index = 0;
    tcaps_idx = 0;
    while((A = _sdp_findA_at(sdp, "tcap", tcaps_idx++))) {
        if (!(size = (int32_t)tsk_strlen(A->value))) {
            goto next;
        }
        if(sscanf(&A->value[index], "%d", &tag) == EOF || (_sdp_integer_length(tag) + 1 >= size)) {
            TSK_DEBUG_ERROR("sscanf(%s) failed", A->value);
            break;
        }
        if(tag <= 0 || (tag + 1) > SDP_CAPS_COUNT_MAX) {
            TSK_DEBUG_WARN("Ignoring tag with value = %d", tag);
            goto next;
        }

        index += _sdp_integer_length(tag) + 1/*SPACE*/;

        profiles_count = 0;
        tag_fake = tag;
        while(sscanf(&A->value[index], "%255s", &tcap) != EOF) {
            if(tag_fake < SDP_CAPS_COUNT_MAX) {
                (*tcaps)[tcaps_count + profiles_count].tag = tag_fake;
                (*tcaps)[tcaps_count + profiles_count].profile = _sdp_profile_from_string(tcap); // split profiles
            }
            if ((index += (int32_t)tsk_strlen(tcap) + 1/*SPACE*/) >= size) {
                break;
            }
            ++tag_fake;
            ++profiles_count;
        }
next:
        if(++tcaps_count >= SDP_CAPS_COUNT_MAX) {
            break;
        }
    }

    return 0;
}

static int _sdp_acfg_to_sdp(sdp_headerM_Or_Message* sdp, const sdp_acfg_xt *acfg)
{
    int32_t i_a_caps;
    char *acfg_str = tsk_null;

    if(!sdp || !acfg || acfg->tag <= 0) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    // acfg: tag
    tsk_strcat_2(&acfg_str, "%d", acfg->tag);
    // acfg: t=
    if(acfg_str && acfg->tcap.tag > 0) {
        tsk_strcat_2(&acfg_str, " t=%d", acfg->tcap.tag);
    }
    // acfg: a=
    for (i_a_caps = 0; acfg_str && i_a_caps < SDP_CAPS_COUNT_MAX; ++i_a_caps) {
        if(acfg->acaps[i_a_caps].tag <= 0) {
            break;
        }
        if(i_a_caps == 0) {
            tsk_strcat_2(&acfg_str, " a=%d", acfg->acaps[i_a_caps].tag);
        }
        else {
            tsk_strcat_2(&acfg_str, "%s%s%d%s", // e.g. |2 or ,6 or ,[2]
                         acfg->acaps[i_a_caps].or ? "|" : ",",
                         acfg->acaps[i_a_caps].optional ? "[" : "",
                         acfg->acaps[i_a_caps].tag,
                         acfg->acaps[i_a_caps].optional ? "]" : ""
                        );
        }
    }

    // a=acfg:
    if(acfg_str) {
        _sdp_add_headerA(sdp, "acfg", acfg_str);
        TSK_FREE(acfg_str);
    }

    return 0;
}

_SDP_DECLARE_INDEX_OF(pcfg);

static int _sdp_pcfgs_from_sdp(const sdp_headerM_Or_Message* sdp, sdp_acap_xt (*acaps)[SDP_CAPS_COUNT_MAX], sdp_tcap_xt (*tcaps)[SDP_CAPS_COUNT_MAX], sdp_pcfg_xt (*pcfgs)[SDP_CAPS_COUNT_MAX], tsk_bool_t reset)
{
    tsk_size_t pcfgs_count, pcfgs_idx;
    const tsdp_header_A_t* A;
    int32_t tag, index = 0, size, t, a_tag, indexof;
    sdp_tcap_xt* tcap_curr;
    int ret;
    char pcfg[256], a[256];

    if(!sdp || !acaps || !tcaps || !pcfgs) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if((ret = _sdp_tcaps_from_sdp(sdp, tcaps, reset))) {
        return ret;
    }
    if((ret = _sdp_acaps_from_sdp(sdp, acaps, reset))) {
        return ret;
    }

    if(reset) {
        _sdp_pcfgs_reset(pcfgs);
        pcfgs_count = 0;
    }
    else {
        if((pcfgs_count = _sdp_pcfgs_indexof(pcfgs, 0)) == -1) {
            TSK_DEBUG_ERROR("No room to append items");
            return -1;
        }
    }

    pcfgs_idx = 0;
    tcap_curr = tsk_null;
    while((A = _sdp_findA_at(sdp, "pcfg", pcfgs_idx++))) {
        if (!(size = (int32_t)tsk_strlen(A->value))) {
            goto next_A;
        }
        if(sscanf(A->value, "%d", &tag) == EOF || (_sdp_integer_length(tag) + 1 >= size)) {
            TSK_DEBUG_ERROR("sscanf(%s) failed", A->value);
            break;
        }
        if(tag <= 0 || (tag + 1) > SDP_CAPS_COUNT_MAX) {
            TSK_DEBUG_WARN("Ignoring tag with value = %d", tag);
            goto next_A;
        }

        (*pcfgs)[pcfgs_count].tag = tag;

        index = _sdp_integer_length(tag) + 1/*SPACE*/;

        while(sscanf(&A->value[index], "%255s", &pcfg) != EOF) {
            if(_sdp_str_starts_with(&A->value[index], "t=") && sscanf(pcfg, "t=%d", &t) != EOF) {
                if(t <= 0 || t + 1 >= SDP_CAPS_COUNT_MAX) {
                    TSK_DEBUG_ERROR("t = %d ignored", t);
                    goto next_pcfg;
                }
                // tcap is something like a=tcap:1 RTP/SAVPF RTP/SAVP RTP/AVPF
                // tcap [2] is "RTP/SAVP" -> not indexed by tag
                tcap_curr = &(*pcfgs)[pcfgs_count].tcap;
                if((indexof = _sdp_tcaps_indexof(tcaps, t)) == -1) {
                    TSK_DEBUG_ERROR("Failed to find 'tcap' with tag=%d", t);
                    goto next_pcfg;
                }
                *tcap_curr = (*tcaps)[indexof];
            }
            else {
                if(_sdp_str_starts_with(&A->value[index], "a=") && sscanf(pcfg, "a=%255s", a) != EOF) {
                    char a_copy[sizeof(a)], *pch, *saveptr;
                    tsk_size_t pcfg_acfgs_count = 0;
                    sdp_acap_xt* acap;
                    memcpy(a_copy, a, sizeof(a));

                    pch = tsk_strtok_r (a, ",[]|", &saveptr);
                    while(pch) {
                        a_tag = atoi(pch);
                        if(a_tag <= 0 || a_tag + 1 >= SDP_CAPS_COUNT_MAX) {
                            TSK_DEBUG_ERROR("a = %d ignored", a_tag);
                            goto next_a;
                        }
                        if((indexof = _sdp_acaps_indexof(acaps, a_tag)) == -1) {
                            TSK_DEBUG_ERROR("Failed to find 'acap' with tag=%d", a_tag);
                            goto next_a;
                        }
                        acap = &(*pcfgs)[pcfgs_count].acaps[pcfg_acfgs_count++];
                        *acap = (*acaps)[indexof];
                        acap->optional = (pch != a && a_copy[pch - a - 1] == '[') ? 1 : 0;
                        acap->or = (pch != a && a_copy[pch - a - 1] == '|') ? 1 : 0;
next_a:
                        pch = tsk_strtok_r(tsk_null, ",[]|", &saveptr);
                    }
                }
                tcap_curr = tsk_null;
            }
next_pcfg:
            if ((index += (int32_t)tsk_strlen(pcfg) + 1/*SPACE*/) >= size) {
                break;
            }
        }
next_A:
        if(++pcfgs_count >= SDP_CAPS_COUNT_MAX) {
            break;
        }
    }

    return ret;
}

static int _sdp_pcfgs_to_sdp(sdp_headerM_Or_Message* sdp, const sdp_pcfg_xt (*pcfgs)[SDP_CAPS_COUNT_MAX])
{
    int32_t i_pcfgs, i_a_caps, i_serialized_acaps;
    char *pcfg = tsk_null, *acap = tsk_null, *tcap = tsk_null;
    sdp_acaps_xt serialized_acaps; /* to avoid duplication */

    if(!sdp || !pcfgs) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    // compact(a=tcap:)
    // tcap:1 RTP/AVP
    // tcap:2 RTP/SAVP
    // tcap:3 RTP/SAVPF
    // will be compacted as
    // tcap:1 RTP/AVP RTP/SAVP RTP/SAVPF
    for (i_pcfgs = 0; i_pcfgs < SDP_CAPS_COUNT_MAX; ++i_pcfgs) {
        if((*pcfgs)[i_pcfgs].tag <= 0 || (*pcfgs)[i_pcfgs].tcap.tag <= 0) {
            break;
        }
        if(!tcap) {
            tsk_sprintf(&tcap, "1 %s", _sdp_profile_to_string((*pcfgs)[i_pcfgs].tcap.profile));
        }
        else {
            tsk_strcat_2(&tcap, " %s", _sdp_profile_to_string((*pcfgs)[i_pcfgs].tcap.profile));
        }
    }
    if(tcap) {
        _sdp_add_headerA(sdp, "tcap", tcap);
        TSK_FREE(tcap);
    }

    _sdp_acaps_reset(&serialized_acaps);
    i_serialized_acaps = 0;

    for (i_pcfgs = 0; i_pcfgs < SDP_CAPS_COUNT_MAX; ++i_pcfgs) {
        if((*pcfgs)[i_pcfgs].tag <= 0) {
            break;
        }
        // pcfg: tag
        tsk_strcat_2(&pcfg, "%d", (*pcfgs)[i_pcfgs].tag);
        // pcfg: t=
        if((*pcfgs)[i_pcfgs].tcap.tag > 0) {
            tsk_strcat_2(&pcfg, " t=%d", (*pcfgs)[i_pcfgs].tcap.tag);
        }

        // pcfg: a=
        for (i_a_caps = 0; i_a_caps < SDP_CAPS_COUNT_MAX; ++i_a_caps) {
            if((*pcfgs)[i_pcfgs].acaps[i_a_caps].tag <= 0) {
                break;
            }
            if(i_a_caps == 0) {
                tsk_strcat_2(&pcfg, " a=%d", (*pcfgs)[i_pcfgs].acaps[i_a_caps].tag);
            }
            else {
                tsk_strcat_2(&pcfg, "%s%s%d%s", // e.g. |2 or ,6 or ,[2]
                             (*pcfgs)[i_pcfgs].acaps[i_a_caps].or ? "|" : ",",
                             (*pcfgs)[i_pcfgs].acaps[i_a_caps].optional ? "[" : "",
                             (*pcfgs)[i_pcfgs].acaps[i_a_caps].tag,
                             (*pcfgs)[i_pcfgs].acaps[i_a_caps].optional ? "]" : ""
                            );
            }
            // a=acap:
            if(_sdp_acaps_indexof(&serialized_acaps, (*pcfgs)[i_pcfgs].acaps[i_a_caps].tag) == -1) {
                tsk_sprintf(&acap, "%d %s", (*pcfgs)[i_pcfgs].acaps[i_a_caps].tag, (*pcfgs)[i_pcfgs].acaps[i_a_caps].value);
                if(acap) {
                    _sdp_add_headerA(sdp, "acap", acap);
                    TSK_FREE(acap);
                    serialized_acaps[i_serialized_acaps++].tag = (*pcfgs)[i_pcfgs].acaps[i_a_caps].tag;
                }
            }
        }

        // a=pcfg:
        if(pcfg) {
            _sdp_add_headerA(sdp, "pcfg", pcfg);
            TSK_FREE(pcfg);
        }
    }
    return 0;
}

static int _sdp_pcfg_ensure(sdp_headerM_Or_Message* sdp, const sdp_pcfg_xt* pcfg)
{
    int32_t i, n;
    char field[256];

    if(!sdp || !pcfg || pcfg->tag <=0) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if(TSK_OBJECT_HEADER(sdp)->__def__ == tsdp_header_M_def_t && pcfg->tcap.profile != RTP_PROFILE_NONE) {
        tsk_strupdate(&((tsdp_header_M_t*)sdp)->proto, _sdp_profile_to_string(pcfg->tcap.profile));
    }

    for(i = 0; i < SDP_CAPS_COUNT_MAX && pcfg->acaps[i].tag > 0; ++i) {
        if (sscanf(pcfg->acaps[i].value, "%255s%*s", field) != EOF && (n = (int32_t)tsk_strlen(field)) > 2) {
            field[n - 2] = '\0';
            _sdp_add_headerA(sdp, field, &pcfg->acaps[i].value[n + 1/*SPACE*/]);
        }
    }

    return 0;
}

static int _sdp_pcfgs_cat(const sdp_pcfg_xt (*pcfgs_src)[SDP_CAPS_COUNT_MAX], sdp_pcfg_xt (*pcfgs_dst)[SDP_CAPS_COUNT_MAX])
{
    int32_t i, j;
    if(!pcfgs_src || !pcfgs_dst) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    for(i = 0; i < SDP_CAPS_COUNT_MAX && (*pcfgs_dst)[i].tag > 0; ++i);

    j = 0;
    while (i < SDP_CAPS_COUNT_MAX && j < SDP_CAPS_COUNT_MAX) {
        if((*pcfgs_src)[j].tag > 0) {
            (*pcfgs_dst)[i++] = (*pcfgs_src)[j++];
        }
        else {
            break;
        }
    }

    return 0;
}





static tsk_object_t* tdav_sdp_caps_ctor(tsk_object_t * self, va_list * app)
{
    tdav_sdp_caps_t *caps = self;
    if(caps) {
    }
    return self;
}
static tsk_object_t* tdav_sdp_caps_dtor(tsk_object_t * self)
{
    tdav_sdp_caps_t *caps = self;
    if(caps) {
    }
    return self;
}
static const tsk_object_def_t tdav_sdp_caps_def_s = {
    sizeof(tdav_sdp_caps_t),
    tdav_sdp_caps_ctor,
    tdav_sdp_caps_dtor,
    tsk_null,
};

static tdav_sdp_caps_t* tdav_sdp_caps_create()
{
    return tsk_object_new(&tdav_sdp_caps_def_s);
}
