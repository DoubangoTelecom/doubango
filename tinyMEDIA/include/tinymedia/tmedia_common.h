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

/**@file tmedia_common.h
 * @brief Common functions and definitions.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef TINYMEDIA_COMMON_H
#define TINYMEDIA_COMMON_H

#include "tinymedia_config.h"

#include "tsk_plugin.h"
#include "tsk_object.h"

TMEDIA_BEGIN_DECLS

struct tsdp_message_s;
struct tsdp_header_M_s;

/** List of all supported media types */
// @tinyWRAP
typedef enum tmedia_type_e {
    tmedia_none = 0x00,
    tmedia_ghost = (0x01 << 0),

    tmedia_audio = (0x01 << 1),
    tmedia_video = (0x01 << 2),
    tmedia_chat = (0x01 << 3),
    tmedia_file = (0x01 << 4),
    tmedia_t38 = (0x01 << 5),
    tmedia_t140 = (0x01 << 6),
    tmedia_bfcp = (0x01 << 7),
    tmedia_bfcp_audio = (0x01 << 8)/*must*/ | tmedia_bfcp, /* do not add "| audio". Otherwise it will be impossible to start an "video+bfcp-audio" session. */
    tmedia_bfcp_video = (0x01 << 9)/*must*/ | tmedia_bfcp, /* do not add "| video". Otherwise it will be impossible to start an "audio+bfcp-video" session. */

    tmedia_msrp = (tmedia_chat | tmedia_file),
    tmedia_audiovideo = (tmedia_audio | tmedia_video),

    tmedia_all = 0xff
}
tmedia_type_t;

// @tinyWRAP
typedef enum tmedia_mode_e {
    tmedia_mode_none,
    tmedia_mode_optional,
    tmedia_mode_mandatory
}
tmedia_mode_t;

// @tinyWRAP
typedef enum tmedia_srtp_mode_e {
    tmedia_srtp_mode_none,
    tmedia_srtp_mode_optional,
    tmedia_srtp_mode_mandatory
}
tmedia_srtp_mode_t;

// @tinyWRAP
typedef enum tmedia_srtp_type_e {
    tmedia_srtp_type_none = 0x00,
    tmedia_srtp_type_sdes = 0x01,
    tmedia_srtp_type_dtls = 0x02,
    tmedia_srtp_type_sdes_dtls = (tmedia_srtp_type_sdes | tmedia_srtp_type_dtls)
}
tmedia_srtp_type_t;

// @tinyWRAP
typedef enum tmedia_t140_data_type_e {
    tmedia_t140_data_type_utf8,
    tmedia_t140_data_type_zero_width_no_break_space = 0xefbbbf,
    tmedia_t140_data_type_backspace = 0x08,
    tmedia_t140_data_type_esc = 0x1b,
    tmedia_t140_data_type_cr = 0x0d,
    tmedia_t140_data_type_lf = 0x0a,
    tmedia_t140_data_type_cr_lf = 0x0d0a,
    tmedia_t140_data_type_bell = 0x07,
    tmedia_t140_data_type_sos = 0x98,
    tmedia_t140_data_type_string_term = 0x9c,
    tmedia_t140_data_type_graphic_start = 0x9b,
    tmedia_t140_data_type_graphic_end = 0x6d,
    tmedia_t140_data_type_loss_char_char = 0xfffd,
    tmedia_t140_data_type_loss_utf8 = 0xefbfbd,
}
tmedia_t140_data_type_t;

// @tinyWRAP
typedef enum tmedia_rtcp_event_type_e {
    tmedia_rtcp_event_type_fir, // Full Intra Refresh
}
tmedia_rtcp_event_type_t;

// @tinyWRAP
typedef enum tmedia_profile_e {
    tmedia_profile_default,
    // Enable all RTCWeb specifications:
    // ICE, DTLS-SRTP, RTP/AVPF, FEC, RED, SDPCapNeg, RTCP-MUX, imageattr...
    tmedia_profile_rtcweb
}
tmedia_profile_t;

// @tinyWRAP
typedef enum tmedia_pref_video_size_s {
    /* must be sorted like this */
    tmedia_pref_video_size_sqcif, // 128 x 98
    tmedia_pref_video_size_qcif, // 176 x 144
    tmedia_pref_video_size_qvga, // 320 x 240
    tmedia_pref_video_size_cif, // 352 x 288
    tmedia_pref_video_size_hvga, // 480 x 320
    tmedia_pref_video_size_vga, // 640 x 480
    tmedia_pref_video_size_4cif, // 704 x 576
    tmedia_pref_video_size_wvga, // 800 x 480
    tmedia_pref_video_size_svga, // 800 x 600
    tmedia_pref_video_size_480p, // 852 x 480
    tmedia_pref_video_size_xga, // 1024 x 768
    tmedia_pref_video_size_720p, // 1280 x 720
    tmedia_pref_video_size_16cif, // 1408 x 1152
    tmedia_pref_video_size_1080p, // 1920 x 1080
    tmedia_pref_video_size_2160p, // 3840 x 2160
}
tmedia_pref_video_size_t;

typedef enum tmedia_rtcweb_type_e {
    tmedia_rtcweb_type_none,
    tmedia_rtcweb_type_firefox,
    tmedia_rtcweb_type_chrome,
    tmedia_rtcweb_type_ie,
    tmedia_rtcweb_type_safari,
    tmedia_rtcweb_type_opera,
    tmedia_rtcweb_type_ericsson,
    tmedia_rtcweb_type_doubango
}
tmedia_rtcweb_type_t;

typedef enum tmedia_video_encode_result_type_e {
    tmedia_video_encode_result_type_none = 0x00,
    tmedia_video_encode_result_type_params = (0x01 << 0), // e.g. SPS or PPS, DCT coeff., Quant params....
    tmedia_video_encode_result_type_intra = (0x01 << 1),
    tmedia_video_encode_result_type_key = tmedia_video_encode_result_type_intra,
    tmedia_video_encode_result_type_gold = tmedia_video_encode_result_type_intra,
    tmedia_video_encode_result_type_predicted = (0x01 << 2),
    tmedia_video_encode_result_type_bipredicted = (0x01 << 3)
}
tmedia_video_encode_result_type_t;

typedef struct tmedia_video_encode_result_xs {
    tmedia_video_encode_result_type_t type;
    const void* usr_data;
    struct {
        const void* ptr;
        tsk_size_t size;
    } buffer;
    uint32_t duration;
    tsk_bool_t last_chunck;
    const tsk_object_t* proto_hdr;
}
tmedia_video_encode_result_xt;

#define tmedia_video_encode_result_reset(result) \
	(result)->type = tmedia_video_encode_result_type_none; \
	(result)->usr_data = tsk_null; \
	(result)->proto_hdr = tsk_null; \
	(result)->buffer.ptr = tsk_null; \
	(result)->buffer.size = 0; \
	(result)->duration = 0; \
	(result)->last_chunck = tsk_false; \
	(result)->proto_hdr = tsk_null; \
 
typedef enum tmedia_video_decode_result_type_e {
    tmedia_video_decode_result_type_none,

    tmedia_video_decode_result_type_error,
    tmedia_video_decode_result_type_idr,
}
tmedia_video_decode_result_type_t;

typedef struct tmedia_video_decode_result_xs {
    tmedia_video_decode_result_type_t type;
    const void* usr_data;
    const tsk_object_t* proto_hdr; // RTP, RTSP....
}
tmedia_video_decode_result_xt;

#define tmedia_video_decode_result_reset(result) \
	(result)->type = tmedia_video_decode_result_type_none; \
	(result)->usr_data = tsk_null; \
	(result)->proto_hdr = tsk_null; \
 

// @tinyWRAP
typedef enum tmedia_chroma_e {
    tmedia_chroma_none=0,
    tmedia_chroma_rgb24,		// will be stored as bgr24 on x86 (little endians) machines; e.g. WindowsPhone7
    tmedia_chroma_bgr24,		// used by windows consumer (DirectShow) -
    tmedia_chroma_rgb32,       // used by iOS4 consumer (iPhone and iPod touch)
    tmedia_chroma_rgb565le,	// (used by both android and wince consumers)
    tmedia_chroma_rgb565be,
    tmedia_chroma_nv12, // used by iOS4 producer (iPhone and iPod Touch 3GS and 4)
    tmedia_chroma_nv21, // Yuv420 SP (used by android producer)
    tmedia_chroma_yuv422p,
    tmedia_chroma_uyvy422, // used by iOS4 producer (iPhone and iPod Touch 3G) - Microsoft: MFVideoFormat_YUY2
    tmedia_chroma_yuv420p, // Default
    tmedia_chroma_mjpeg, // VirtualBox default camera mode (Windows as host and Linux as guest)
    tmedia_chroma_yuyv422, // YUYV422 (V4L2 preferred format)
}
tmedia_chroma_t;

// @tinyWRAP
// @deprecated
// keep order (low->unrestricted)
typedef enum tmedia_bandwidth_level_e {
    tmedia_bl_low,
    tmedia_bl_medium,
    tmedia_bl_hight,
    tmedia_bl_unrestricted
}
tmedia_bandwidth_level_t;

typedef enum tmedia_ro_type_e {
    tmedia_ro_type_none = 0x00,
    tmedia_ro_type_offer = (0x01 << 0),
    tmedia_ro_type_answer = (0x01 << 1),
    tmedia_ro_type_provisional = tmedia_ro_type_answer | (0x01 << 2),
}
tmedia_ro_type_t;

TINYMEDIA_API tsk_size_t tmedia_plugin_register(struct tsk_plugin_s* plugin, enum tsk_plugin_def_type_e type, enum tsk_plugin_def_media_type_e media);
TINYMEDIA_API tsk_size_t tmedia_plugin_unregister(struct tsk_plugin_s* plugin, enum tsk_plugin_def_type_e type, enum tsk_plugin_def_media_type_e media);
TINYMEDIA_API tmedia_type_t tmedia_type_from_sdp(const struct tsdp_message_s* sdp);
TINYMEDIA_API tmedia_type_t tmedia_type_from_sdp_headerM(const struct tsdp_header_M_s* M);
TINYMEDIA_API int tmedia_parse_rtpmap(const char* rtpmap, char** name, int32_t* rate, int32_t* channels);
TINYMEDIA_API int tmedia_video_get_size(tmedia_pref_video_size_t pref_vs, unsigned *width, unsigned *height);
TINYMEDIA_API int tmedia_video_get_closest_cif_size(tmedia_pref_video_size_t pref_vs, tmedia_pref_video_size_t *cif_vs);
TINYMEDIA_API int tmedia_video_get_closest_pref_size(unsigned width, unsigned height, tmedia_pref_video_size_t *pref_vs);
TINYMEDIA_API int tmedia_parse_video_fmtp(const char* fmtp, tmedia_pref_video_size_t pref_vs, unsigned* width, unsigned* height, unsigned* fps);
TINYMEDIA_API int tmedia_parse_video_imageattr(const char* imageattr, tmedia_pref_video_size_t pref_vs, unsigned* in_width, unsigned* in_height, unsigned* out_width, unsigned* out_height);
TINYMEDIA_API char* tmedia_get_video_fmtp(tmedia_pref_video_size_t pref_vs);
TINYMEDIA_API char* tmedia_get_video_imageattr(tmedia_pref_video_size_t pref_vs, unsigned in_width, unsigned in_height, unsigned out_width, unsigned out_height);
TINYMEDIA_API int tmedia_get_video_quality(tmedia_bandwidth_level_t bl);
TINYMEDIA_API int32_t tmedia_get_video_bandwidth_kbps(unsigned width, unsigned height, unsigned fps, unsigned motion_rank);
TINYMEDIA_API int32_t tmedia_get_video_bandwidth_kbps_2(unsigned width, unsigned height, unsigned fps);
TINYMEDIA_API int32_t tmedia_get_video_bandwidth_kbps_3();
TINYMEDIA_API int32_t tmedia_get_video_motion_rank(unsigned width, unsigned height, unsigned fps, int32_t bw_kbps);
#define tmedia_get_video_qscale tmedia_get_video_quality

TMEDIA_END_DECLS

#endif /* TINYMEDIA_COMMON_H */
