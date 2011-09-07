/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango.org>
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
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
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

/******* Dynamic Payload Type **********/

#define TMEDIA_CODEC_FORMAT_ILBC						"96"

#define TMEDIA_CODEC_FORMAT_SPEEX_NB					"97"
#define TMEDIA_CODEC_FORMAT_SPEEX_WB					"98"
#define TMEDIA_CODEC_FORMAT_SPEEX_UWB					"99"

#define TMEDIA_CODEC_FORMAT_DTMF						"101"

#define TMEDIA_CODEC_FORMAT_H263_2000					"102"
#define TMEDIA_CODEC_FORMAT_H263_1998					"103" // for stupid clients, should be equal to "103"
#define TMEDIA_CODEC_FORMAT_H264_BP10					"104"
#define TMEDIA_CODEC_FORMAT_H264_BP20					"105"
#define TMEDIA_CODEC_FORMAT_H264_BP30					"106"


#define TMEDIA_CODEC_FORMAT_AMR_WBP_BE					"107"
#define TMEDIA_CODEC_FORMAT_AMR_WBP_OA					"108"
#define TMEDIA_CODEC_FORMAT_AAC							"109"
#define TMEDIA_CODEC_FORMAT_AACPLUS						"110"

#define TMEDIA_CODEC_FORMAT_AMR_NB_BE					"111"
#define TMEDIA_CODEC_FORMAT_AMR_NB_OA					"112"
#define TMEDIA_CODEC_FORMAT_AMR_WB_BE					"113"
#define TMEDIA_CODEC_FORMAT_AMR_WB_OA					"114"

#define TMEDIA_CODEC_FORMAT_BV16						"115"

#define TMEDIA_CODEC_FORMAT_MP4V_ES						"121"
#define TMEDIA_CODEC_FORMAT_FFV1						"122"
#define TMEDIA_CODEC_FORMAT_FFVHUFF						"123"
#define TMEDIA_CODEC_FORMAT_HUFFYUV						"124"

#define TMEDIA_CODEC_FORMAT_VP8							"125"
#define TMEDIA_CODEC_FORMAT_THEORA						"126"


#define TMEDIA_CODEC_FORMAT_MSRP						"*"

/**Max number of plugins (codec types) we can create */
#define TMED_CODEC_MAX_PLUGINS			0xFF

/** cast any pointer to @ref tmedia_codec_t* object */
#define TMEDIA_CODEC(self)		((tmedia_codec_t*)(self))

#define TMEDIA_CODEC_PCM_FRAME_SIZE(self) ((TMEDIA_CODEC((self))->plugin->audio.ptime * TMEDIA_CODEC((self))->plugin->rate)/1000)
#define TMEDIA_CODEC_RATE(self)			(TMEDIA_CODEC((self))->plugin->rate)
//#define TMEDIA_CODEC_FRAMES_COUNT(buff_size)	(((buff_size))/TMEDIA_CODEC_FRAME_SIZE(self))

/** callback for video codecs */
typedef int (*tmedia_codec_video_rtpcb_f)(const void* callback_data, const void* buffer, tsk_size_t size, uint32_t duration, tsk_bool_t marker);

/** Base object for all Codecs */
typedef struct tmedia_codec_s
{
	TSK_DECLARE_OBJECT;

	//! the type of the codec
	tmedia_type_t type;
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
	tmedia_bandwidth_level_t bl;
	//! the negociated format (only useful for codecs with dyn. payload type)
	char* neg_format;
	
	//! plugin used to create the codec
	const struct tmedia_codec_plugin_def_s* plugin;
}
tmedia_codec_t;

/** Virtual table used to define a codec plugin */
typedef struct tmedia_codec_plugin_def_s
{
	//! object definition used to create an instance of the codec
	const tsk_object_def_t* objdef;

	//! the type of the codec
	tmedia_type_t type;
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
	struct{
		int8_t channels;
		uint8_t ptime;
		/* ...to be continued */
	} audio;

	/* default values could be updated at any time */
	struct{
		unsigned width;
		unsigned height;
		unsigned fps;
		/* ...to be continued */
	} video;

	//! open the codec
	int (*open) (tmedia_codec_t*);
	//! close the codec
	int (*close) (tmedia_codec_t*);
	//! encode data
	tsk_size_t (*encode) (tmedia_codec_t*, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size);
	//! decode data
	tsk_size_t (*decode) (tmedia_codec_t*, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size, const tsk_object_t* proto_hdr);
	//! whether the codec can handle the fmtp
	tsk_bool_t (* fmtp_match) (const tmedia_codec_t*, const char* );
	//! gets fmtp value. e.g. "mode-set=0,2,5,7; mode-change-period=2; mode-change-neighbor=1"
	char* (* fmtp_get) (const tmedia_codec_t* );
	//! sets fmtp received from the remote party
	int (* fmtp_set) (tmedia_codec_t*, const char* );
}
tmedia_codec_plugin_def_t;

/** List of @ref tmedia_codec_t elements */
typedef tsk_list_t tmedia_codecs_L_t;

/**< Declare base class as codec */
#define TMEDIA_DECLARE_CODEC tmedia_codec_t __codec__

TINYMEDIA_API int tmedia_codec_init(tmedia_codec_t* self, tmedia_type_t type, const char* name, const char* desc, const char* format);
TINYMEDIA_API int tmedia_codec_open(tmedia_codec_t* self);
TINYMEDIA_API int tmedia_codec_close(tmedia_codec_t* self);
TINYMEDIA_API int tmedia_codec_cmp(const tsk_object_t* codec1, const tsk_object_t* codec2);
TINYMEDIA_API int tmedia_codec_plugin_register(const tmedia_codec_plugin_def_t* plugin);
TINYMEDIA_API tsk_bool_t tmedia_codec_plugin_is_registered(const tmedia_codec_plugin_def_t* plugin);
TINYMEDIA_API int tmedia_codec_plugin_unregister(const tmedia_codec_plugin_def_t* plugin);
TINYMEDIA_API tmedia_codec_t* tmedia_codec_create(const char* format);
TINYMEDIA_API char* tmedia_codec_get_rtpmap(const tmedia_codec_t* self);
TINYMEDIA_API char* tmedia_codec_get_fmtp(const tmedia_codec_t* self);
TINYMEDIA_API tsk_bool_t tmedia_codec_match_fmtp(const tmedia_codec_t* self, const char* fmtp);
TINYMEDIA_API int tmedia_codec_set_remote_fmtp(tmedia_codec_t* self, const char* fmtp);
TINYMEDIA_API int tmedia_codec_removeAll_exceptThese(tmedia_codecs_L_t* codecs, const tmedia_codecs_L_t * codecs2keep);
TINYMEDIA_API int tmedia_codec_to_sdp(const tmedia_codecs_L_t* codecs, tsdp_header_M_t* m);
TINYMEDIA_API tmedia_codec_t* tmedia_codec_find_by_format(tmedia_codecs_L_t* codecs, const char* format);
TINYMEDIA_API int tmedia_codec_parse_fmtp(const char* fmtp, unsigned* maxbr, unsigned* fps, unsigned *width, unsigned *height);
TINYMEDIA_API int tmedia_codec_deinit(tmedia_codec_t* self);

/** Audio codec */
typedef struct tmedia_codec_audio_s
{
	TMEDIA_DECLARE_CODEC;
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

/** Video codec */
typedef struct tmedia_codec_video_s
{
	TMEDIA_DECLARE_CODEC;

	struct{
		unsigned width;
		unsigned height;
		unsigned fps;
		unsigned max_br;
		unsigned max_mbps;
		tmedia_chroma_t chroma;
		tsk_bool_t flip;
	}in;// decoded
	struct{
		unsigned width;
		unsigned height;
		unsigned fps;
		unsigned max_br;
		unsigned max_mbps;
		tmedia_chroma_t chroma;
		tsk_bool_t flip;
	}out;// encoded


	tmedia_codec_video_rtpcb_f callback;
	const void* callback_data;
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
TINYMEDIA_API int tmedia_codec_video_set_callback(tmedia_codec_video_t *self, tmedia_codec_video_rtpcb_f callback, const void* callback_data);
#define tmedia_codec_video_deinit(self) tmedia_codec_deinit(TMEDIA_CODEC(self))

/** MSRP codec */
typedef struct tmedia_codec_msrp_s
{
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

TMEDIA_END_DECLS

#endif /* TINYMEDIA_CODEC_H */
