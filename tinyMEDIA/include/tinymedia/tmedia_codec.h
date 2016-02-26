/*
* Copyright (C) 2010-2015 Mamadou DIOP.
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

/**@file tmedia_codec.h
 * @brief Base codec object.
 *
 */
#ifndef TINYMEDIA_CODEC_H
#define TINYMEDIA_CODEC_H

#include "tinymedia_config.h"

#include "tmedia_common.h"

#include "tsk_list.h"


TMEDIA_BEGIN_DECLS

/* =====
* http://www.iana.org/assignments/rtp-parameters
* http://www.networksorcery.com/enp/protocol/rtp.htm
=====*/
/******* Fixed Payload Type *************/
#define TMEDIA_CODEC_FORMAT_G711u						"0"
#define TMEDIA_CODEC_FORMAT_1016						"1"
#define TMEDIA_CODEC_FORMAT_G721						"2"
#define TMEDIA_CODEC_FORMAT_GSM							"3"
#define TMEDIA_CODEC_FORMAT_G723						"4"
#define TMEDIA_CODEC_FORMAT_DVI4_8000					"5"
#define TMEDIA_CODEC_FORMAT_DVI4_16000					"6"
#define TMEDIA_CODEC_FORMAT_LPC							"7"
#define TMEDIA_CODEC_FORMAT_G711a						"8"
#define TMEDIA_CODEC_FORMAT_G722						"9"
#define TMEDIA_CODEC_FORMAT_L16_STEREO					"10"
#define TMEDIA_CODEC_FORMAT_L16							"11"
#define TMEDIA_CODEC_FORMAT_QCELP						"12"
#define TMEDIA_CODEC_FORMAT_CN							"13"
#define TMEDIA_CODEC_FORMAT_MPA							"14"
#define TMEDIA_CODEC_FORMAT_G728						"15"
#define TMEDIA_CODEC_FORMAT_DVI4_11025					"16"
#define TMEDIA_CODEC_FORMAT_DVI4_22050					"17"
#define TMEDIA_CODEC_FORMAT_G729						"18"

#define TMEDIA_CODEC_FORMAT_CELLB						"25"
#define TMEDIA_CODEC_FORMAT_JPEG						"26"
#define TMEDIA_CODEC_FORMAT_NV							"28"

#define TMEDIA_CODEC_FORMAT_H261						"31"
#define TMEDIA_CODEC_FORMAT_MPV							"32"
#define TMEDIA_CODEC_FORMAT_MP2T						"33"
#define TMEDIA_CODEC_FORMAT_H263						"34"

/******* Dynamic Payload Type
Must starts at 96 to be conform to RFC 5761 (rtcp-mux)
**********/

#define TMEDIA_CODEC_FORMAT_ILBC						"96"

#define TMEDIA_CODEC_FORMAT_SPEEX_NB					"97"
#define TMEDIA_CODEC_FORMAT_SPEEX_WB					"98"
#define TMEDIA_CODEC_FORMAT_SPEEX_UWB					"99"
#define TMEDIA_CODEC_FORMAT_VP8							"100" /* Must to ease neg. with chrome and Asterisk */
#define TMEDIA_CODEC_FORMAT_DTMF						"101"

#define TMEDIA_CODEC_FORMAT_H263_2000					"102"
#define TMEDIA_CODEC_FORMAT_H263_1998					"103"
#define TMEDIA_CODEC_FORMAT_H264_BP						"104"
#define TMEDIA_CODEC_FORMAT_H264_MP						"105"
#define TMEDIA_CODEC_FORMAT_H264_HP						"106"

#define TMEDIA_CODEC_FORMAT_AMR_WBP_BE					"107"
#define TMEDIA_CODEC_FORMAT_AMR_WBP_OA					"108"
#define TMEDIA_CODEC_FORMAT_AAC							"109"
#define TMEDIA_CODEC_FORMAT_AACPLUS						"110"

#define TMEDIA_CODEC_FORMAT_OPUS						"111"

#define TMEDIA_CODEC_FORMAT_AMR_NB_BE					"112"
#define TMEDIA_CODEC_FORMAT_AMR_NB_OA					"113"
#define TMEDIA_CODEC_FORMAT_AMR_WB_BE					"114"
#define TMEDIA_CODEC_FORMAT_AMR_WB_OA					"115"

#define TMEDIA_CODEC_FORMAT_BV16						"116"

#define TMEDIA_CODEC_FORMAT_MP4V_ES						"121"

#define TMEDIA_CODEC_FORMAT_ULPFEC						"122"
#define TMEDIA_CODEC_FORMAT_RED							"123"
#define TMEDIA_CODEC_FORMAT_T140						"124"

#define TMEDIA_CODEC_FORMAT_THEORA						"125"


#define TMEDIA_CODEC_FORMAT_MSRP						"*"
#define TMEDIA_CODEC_FORMAT_BFCP						"*"



// @tinyWRAP
typedef enum tmedia_codec_id_e {
    tmedia_codec_id_none = 0x00000000,

    tmedia_codec_id_amr_nb_oa = 0x00000001<<0,
    tmedia_codec_id_amr_nb_be = 0x00000001<<1,
    tmedia_codec_id_amr_wb_oa = 0x00000001<<2,
    tmedia_codec_id_amr_wb_be = 0x00000001<<3,
    tmedia_codec_id_gsm = 0x00000001<<4,
    tmedia_codec_id_pcma = 0x00000001<<5,
    tmedia_codec_id_pcmu = 0x00000001<<6,
    tmedia_codec_id_ilbc = 0x00000001<<7,
    tmedia_codec_id_speex_nb = 0x00000001<<8,
    tmedia_codec_id_speex_wb = 0x00000001<<9,
    tmedia_codec_id_speex_uwb = 0x00000001<<10,
    tmedia_codec_id_bv16 = 0x00000001<<11,
    tmedia_codec_id_bv32 = 0x00000001<<12,
    tmedia_codec_id_opus = 0x00000001<<13,
    tmedia_codec_id_g729ab = 0x00000001<<14,
    tmedia_codec_id_g722 = 0x00000001<<15,

    /* room for new Audio codecs */

    tmedia_codec_id_h261 = 0x00010000<<0,
    tmedia_codec_id_h263 = 0x00010000<<1,
    tmedia_codec_id_h263p = 0x00010000<<2,
    tmedia_codec_id_h263pp = 0x00010000<<3,
    tmedia_codec_id_h264_bp = 0x00010000<<4,
    tmedia_codec_id_h264_mp = 0x00010000<<5,
    tmedia_codec_id_h264_hp = 0x00010000<<6,
    tmedia_codec_id_h264_bp10 = tmedia_codec_id_h264_bp, // @deprecated
    tmedia_codec_id_h264_bp20 = tmedia_codec_id_h264_bp, // @deprecated
    tmedia_codec_id_h264_bp30 = tmedia_codec_id_h264_bp, // @deprecated
    tmedia_codec_id_h264_svc = 0x00010000<<7,
    tmedia_codec_id_theora = 0x00010000<<8,
    tmedia_codec_id_mp4ves_es = 0x00010000<<9,
    tmedia_codec_id_vp8 = 0x00010000<<10,

    /* room for new Video codecs */

    tmedia_codec_id_t140 = 0x00010000<<14,
    tmedia_codec_id_red = 0x00010000<<15,


    tmedia_codec_id_all = 0xffffffff,
}
tmedia_codec_id_t;



/**Max number of plugins (codec types) we can create */
#if !defined(TMED_CODEC_MAX_PLUGINS)
#   define TMED_CODEC_MAX_PLUGINS			0xFF
#endif

/** cast any pointer to @ref tmedia_codec_t* object */
#define TMEDIA_CODEC(self)		((tmedia_codec_t*)(self))


#define TMEDIA_CODEC_RATE_DECODING(self)			(TMEDIA_CODEC((self))->in.rate)
#define TMEDIA_CODEC_RATE_ENCODING(self)			(TMEDIA_CODEC((self))->out.rate)

#define TMEDIA_CODEC_PTIME_AUDIO_DECODING(self)			(TMEDIA_CODEC_AUDIO((self))->in.ptime)
#define TMEDIA_CODEC_PTIME_AUDIO_ENCODING(self)			(TMEDIA_CODEC_AUDIO((self))->out.ptime)

#define TMEDIA_CODEC_CHANNELS_AUDIO_DECODING(self)			(TMEDIA_CODEC_AUDIO((self))->in.channels)
#define TMEDIA_CODEC_CHANNELS_AUDIO_ENCODING(self)			(TMEDIA_CODEC_AUDIO((self))->out.channels)

#define TMEDIA_CODEC_PCM_FRAME_SIZE_AUDIO_DECODING(self) ((TMEDIA_CODEC_PTIME_AUDIO_DECODING((self)) * TMEDIA_CODEC_RATE_DECODING((self)))/1000)
#define TMEDIA_CODEC_PCM_FRAME_SIZE_AUDIO_ENCODING(self) ((TMEDIA_CODEC_PTIME_AUDIO_ENCODING((self)) * TMEDIA_CODEC_RATE_ENCODING((self)))/1000)

#define TMEDIA_CODEC_FRAME_DURATION_AUDIO_ENCODING(self) (int32_t)((float)TMEDIA_CODEC_PCM_FRAME_SIZE_AUDIO_ENCODING(self) * (float)TMEDIA_CODEC_AUDIO((self))->out.timestamp_multiplier)

/** callbacks for video codecs */
typedef int (*tmedia_codec_video_enc_cb_f)(const tmedia_video_encode_result_xt* result);
typedef int (*tmedia_codec_video_dec_cb_f)(const tmedia_video_decode_result_xt* result);


struct tmedia_param_s;
struct tsdp_header_M_s;

typedef enum tmedia_codec_action_e {
    tmedia_codec_action_encode_idr,
    tmedia_codec_action_bw_down,
    tmedia_codec_action_bw_up,
}
tmedia_codec_action_t;

/** Base object for all Codecs */
typedef struct tmedia_codec_s {
    TSK_DECLARE_OBJECT;

    //! the type of the codec
    tmedia_type_t type;
    //! the codec identifier
    tmedia_codec_id_t id;
    //! whether the codec is opened
    tsk_bool_t opened;
    //! whether the pay. type is dyn. or not
    tsk_bool_t dyn;
    //! the name of the codec. e.g. "G.711U" or "G.711A" etc used in the sdp
    char* name;
    //! full description
    char* desc;
    //! the format. e.g. "0" for PCMU or "8" for PCMA or "*" for MSRP.
    char* format;
    //! bandwidth level
    tmedia_bandwidth_level_t bl; // @deprecated
    //! maximum bandwidth to use for outgoing RTP (INT_MAX or <=0 means undefined)
    int32_t bandwidth_max_upload;
    //! maximum bandwidth to use for incoming RTP (INT_MAX or <=0 means undefined)
    int32_t bandwidth_max_download;
    //! the negociated format (only useful for codecs with dyn. payload type)
    char* neg_format;
    //! whether this is a passthrough codec
    tsk_bool_t passthrough;

    struct {
        // !negotiated decoding rate (for codecs with dynamic rate, e.g. opus)
        uint32_t rate;
    } in; //decoding direction
    struct {
        // !negotiated encoding rate (for codecs with dynamic rate, e.g. opus)
        uint32_t rate;
    } out; //encoding direction

    //! plugin used to create the codec
    const struct tmedia_codec_plugin_def_s* plugin;
}
tmedia_codec_t;
#define TMEDIA_CODEC(self)		((tmedia_codec_t*)(self))

/** Virtual table used to define a codec plugin */
typedef struct tmedia_codec_plugin_def_s {
    //! object definition used to create an instance of the codec
    const tsk_object_def_t* objdef;

    //! the type of the codec
    tmedia_type_t type;
    //! the codec identifier
    tmedia_codec_id_t codec_id;
    //! the name of the codec. e.g. "G.711U" or "G.711A" etc using in the sdp.
    const char* name;
    //! full description
    const char* desc;
    //! the format. e.g. "0" for PCMU or "8" for PCMA or "*" for MSRP.
    const char* format;
    //! whether the pay. type is dyn. or not
    tsk_bool_t dyn;
    uint32_t rate;

    /* default values could be updated at any time */
    struct {
        int8_t channels;
        uint8_t ptime;
        /* ...to be continued */
    } audio;

    /* default values could be updated at any time */
    struct {
        unsigned width;
        unsigned height;
        unsigned fps;
        /* ...to be continued */
    } video;

    //! set parameters
    int (*set) (tmedia_codec_t* , const struct tmedia_param_s*);
    //! open the codec
    int (*open) (tmedia_codec_t*);
    //! close the codec
    int (*close) (tmedia_codec_t*);
    //! encode data
    tsk_size_t (*encode) (tmedia_codec_t*, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size);
    //! decode data
    tsk_size_t (*decode) (tmedia_codec_t*, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size, const tsk_object_t* proto_hdr);
    //! whether the codec can handle this sdp attribute
    tsk_bool_t (* sdp_att_match) (const tmedia_codec_t*, const char* att_name, const char* att_value);
    //! gets sdp attribute value. e.g. "mode-set=0,2,5,7; mode-change-period=2; mode-change-neighbor=1"
    char* (* sdp_att_get) (const tmedia_codec_t*, const char* att_name);
}
tmedia_codec_plugin_def_t;

/** List of @ref tmedia_codec_t elements */
typedef tsk_list_t tmedia_codecs_L_t;

/**< Declare base class as codec */
#define TMEDIA_DECLARE_CODEC tmedia_codec_t __codec__

TINYMEDIA_API int tmedia_codec_init(tmedia_codec_t* self, tmedia_type_t type, const char* name, const char* desc, const char* format);
TINYMEDIA_API int tmedia_codec_set(tmedia_codec_t* self, const struct tmedia_param_s* param);
TINYMEDIA_API int tmedia_codec_open(tmedia_codec_t* self);
TINYMEDIA_API int tmedia_codec_close(tmedia_codec_t* self);
TINYMEDIA_API int tmedia_codec_cmp(const tsk_object_t* codec1, const tsk_object_t* codec2);
TINYMEDIA_API int tmedia_codec_plugin_register(const tmedia_codec_plugin_def_t* plugin);
TINYMEDIA_API int tmedia_codec_plugin_register_2(const tmedia_codec_plugin_def_t* plugin, int prio);
TINYMEDIA_API tsk_bool_t tmedia_codec_plugin_is_registered(const tmedia_codec_plugin_def_t* plugin);
TINYMEDIA_API tsk_bool_t tmedia_codec_plugin_is_registered_2(tmedia_codec_id_t codec_id);
TINYMEDIA_API int tmedia_codec_plugin_registered_get_all(const struct tmedia_codec_plugin_def_s*(** plugins)[TMED_CODEC_MAX_PLUGINS], tsk_size_t* count);
TINYMEDIA_API const struct tmedia_codec_plugin_def_s* tmedia_codec_plugin_registered_get_const(tmedia_codec_id_t codec_id);
TINYMEDIA_API tsk_size_t tmedia_codec_plugin_registered_count(const struct tmedia_codec_plugin_def_s** plugins, tsk_size_t count);
TINYMEDIA_API int tmedia_codec_plugin_unregister(const tmedia_codec_plugin_def_t* plugin);
TINYMEDIA_API int tmedia_codec_plugin_unregister_all();
TINYMEDIA_API tmedia_codec_t* tmedia_codec_create(const char* format);
TINYMEDIA_API char* tmedia_codec_get_rtpmap(const tmedia_codec_t* self);
TINYMEDIA_API tsk_bool_t tmedia_codec_sdp_att_match(const tmedia_codec_t* self, const char* att_name, const char* att_value);
TINYMEDIA_API char* tmedia_codec_sdp_att_get(const tmedia_codec_t* self, const char* att_name);
TINYMEDIA_API int tmedia_codec_removeAll_exceptThese(tmedia_codecs_L_t* codecs, const tmedia_codecs_L_t * codecs2keep);
TINYMEDIA_API int tmedia_codec_to_sdp(const tmedia_codecs_L_t* codecs, struct tsdp_header_M_s* m);
TINYMEDIA_API tmedia_codec_t* tmedia_codec_find_by_format(tmedia_codecs_L_t* codecs, const char* format);
TINYMEDIA_API int tmedia_codec_parse_fmtp(const char* fmtp, unsigned* maxbr, unsigned* fps, unsigned *width, unsigned *height);
TINYMEDIA_API int tmedia_codec_deinit(tmedia_codec_t* self);

/** Audio codec */
typedef struct tmedia_codec_audio_s {
    TMEDIA_DECLARE_CODEC;

    struct {
        // !negotiated decoding ptime
        uint8_t ptime;
        // !negotiated decoding channels
        int8_t channels;
        // ! timestamp multiplier
        float timestamp_multiplier;
    } in; //decoding direction
    struct {
        // !negotiated decoding ptime
        uint8_t ptime;
        // !negotiated encoding channels
        int8_t channels;
        // ! timestamp multiplier
        float timestamp_multiplier;
    } out; //encoding direction
}
tmedia_codec_audio_t;

/**@def TMEDIA_DECLARE_CODEC_AUDIO
* Declares base class as audio codec.
*/
/**@def TMEDIA_CODEC_AUDIO
* Cast any pointer as @ref tmedia_codec_audio_t* object.
*/
/**@def tmedia_codec_audio_init
* Initialize a audio codec.
*/
/**@def tmedia_codec_audio_deinit
* DeInitialize a audio codec.
*/
#define TMEDIA_DECLARE_CODEC_AUDIO tmedia_codec_audio_t __audio__
#define TMEDIA_CODEC_AUDIO(self)		((tmedia_codec_audio_t*)(self))
#define tmedia_codec_audio_init(self, name, desc, format) tmedia_codec_init(TMEDIA_CODEC(self), tmedia_audio, name, desc, format)
#define tmedia_codec_audio_deinit(self) tmedia_codec_deinit(TMEDIA_CODEC(self))
TINYMEDIA_API float tmedia_codec_audio_get_timestamp_multiplier(tmedia_codec_id_t id, uint32_t sample_rate);

/** Video codec */
typedef struct tmedia_codec_video_s {
    TMEDIA_DECLARE_CODEC;

    struct {
        unsigned width;
        unsigned height;
        unsigned fps;
        unsigned max_br;
        unsigned max_mbps;
        tmedia_chroma_t chroma;
        tsk_bool_t flip;

        tmedia_codec_video_dec_cb_f callback;
        tmedia_video_decode_result_xt result;
    } in; // decoded
    struct {
        unsigned width;
        unsigned height;
        unsigned fps;
        unsigned max_br;
        unsigned max_mbps;
        tmedia_chroma_t chroma;
        tsk_bool_t flip;

        tmedia_codec_video_enc_cb_f callback;
        tmedia_video_encode_result_xt result;
    } out; // encoded

    //! preferred video size
    tmedia_pref_video_size_t pref_size;
}
tmedia_codec_video_t;

/**@def TMEDIA_DECLARE_CODEC_VIDEO
* Declares base class as video codec.
*/
/**@def TMEDIA_CODEC_VIDEO
* Cast any pointer as @ref tmedia_codec_video_t* object.
*/
/**@def tmedia_codec_video_init
* Initialize a video codec.
*/
/**@def tmedia_codec_video_deinit
* DeInitialize a video codec.
*/
#define TMEDIA_DECLARE_CODEC_VIDEO tmedia_codec_video_t __video__
#define TMEDIA_CODEC_VIDEO(self)		((tmedia_codec_video_t*)(self))
#define tmedia_codec_video_init(self, name, desc, format) tmedia_codec_init(TMEDIA_CODEC(self), tmedia_video, name, desc, format)
TINYMEDIA_API int tmedia_codec_video_set_enc_callback(tmedia_codec_video_t *self, tmedia_codec_video_enc_cb_f callback, const void* callback_data);
TINYMEDIA_API int tmedia_codec_video_set_dec_callback(tmedia_codec_video_t *self, tmedia_codec_video_dec_cb_f callback, const void* callback_data);
TINYMEDIA_API int tmedia_codec_video_clamp_out_size_to_range_max(tmedia_codec_video_t *self);
#define tmedia_codec_video_deinit(self) tmedia_codec_deinit(TMEDIA_CODEC(self))


/** MSRP codec */
typedef struct tmedia_codec_msrp_s {
    TMEDIA_DECLARE_CODEC;
}
tmedia_codec_msrp_t;

/**@def TMEDIA_DECLARE_CODEC_MSRP
* Declares base class as msrp codec.
*/
/**@def TMEDIA_CODEC_MSRP
* Cast any pointer as @ref tmedia_codec_msrp_t* object.
*/
/**@def tmedia_codec_msrp_init
* Initialize a msrp codec.
*/
/**@def tmedia_codec_msrp_deinit
* DeInitialize a msrp codec.
*/
#define TMEDIA_DECLARE_CODEC_MSRP tmedia_codec_msrp_t __msrp__
#define TMEDIA_CODEC_MSRP(self)		((tmedia_codec_msrp_t*)(self))
#define tmedia_codec_msrp_init(self, name, desc) tmedia_codec_init(TMEDIA_CODEC(self), tmedia_msrp, name, desc, "*")
#define tmedia_codec_msrp_deinit(self) tmedia_codec_deinit(TMEDIA_CODEC(self))


/** BFCP codec */
typedef struct tmedia_codec_bfcp_s {
    TMEDIA_DECLARE_CODEC;
}
tmedia_codec_bfcp_t;
#define TMEDIA_DECLARE_CODEC_BFCP tmedia_codec_bfcp_t __bfcp__
#define TMEDIA_CODEC_BFCP(self)		((tmedia_codec_bfcp_t*)(self))
#define tmedia_codec_bfcp_init(self, name, desc) tmedia_codec_init(TMEDIA_CODEC(self), tmedia_bfcp, name, desc, "*")
#define tmedia_codec_bfcp_deinit(self) tmedia_codec_deinit(TMEDIA_CODEC(self))

TMEDIA_END_DECLS

#endif /* TINYMEDIA_CODEC_H */
