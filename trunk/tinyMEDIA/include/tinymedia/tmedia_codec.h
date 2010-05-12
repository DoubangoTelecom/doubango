/*
* Copyright (C) 2009 Mamadou Diop.
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
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
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

#define TMEDIA_CODEC_FORMAT_ILBC						" 96"

#define TMEDIA_CODEC_FORMAT_SPEEX						" 97"
#define TMEDIA_CODEC_FORMAT_SPEEX_WIDEBAND				" 98"
#define TMEDIA_CODEC_FORMAT_SPEEX_ULTRAWIDEBAND			" 99"

#define TMEDIA_CODEC_FORMAT_DTMF						"101" // Reserved for DTMF over RTP

#define TMEDIA_CODEC_FORMAT_AMR_NARROWBAND_BE			"102"
#define TMEDIA_CODEC_FORMAT_AMR_NARROWBAND_OA			"103"
#define TMEDIA_CODEC_FORMAT_AMR_WIDEBAND_BE				"104"
#define TMEDIA_CODEC_FORMAT_AMR_WIDEBAND_OA				"105"

#define TMEDIA_CODEC_FORMAT_AMR_WIDEBANDPLUS_BE			"106" // Reserved for future use
#define TMEDIA_CODEC_FORMAT_AMR_WIDEBANDPLUS_OA			"107" // Reserved for future use
#define TMEDIA_CODEC_FORMAT_AAC							"108" // Reserved for future use
#define TMEDIA_CODEC_FORMAT_AACPLUS						"109" // Reserved for future use

#define TMEDIA_CODEC_FORMAT_H263_1998					"111"
#define TMEDIA_CODEC_FORMAT_H263_2000					"112"
#define TMEDIA_CODEC_FORMAT_H264_BP10					"113"
#define TMEDIA_CODEC_FORMAT_H264_BP20					"114"
#define TMEDIA_CODEC_FORMAT_H264_BP30					"115"

#define TMEDIA_CODEC_FORMAT_RAW							"121"
#define TMEDIA_CODEC_FORMAT_THEORA						"122"
#define TMEDIA_CODEC_FORMAT_FFV1						"123"
#define TMEDIA_CODEC_FORMAT_FFVHUFF						"124"
#define TMEDIA_CODEC_FORMAT_HUFFYUV						"125"


#define TMEDIA_CODEC_FORMAT_MSRP						"*"

/**Max number of plugins (codec types) we can create */
#define TMED_CODEC_MAX_PLUGINS			0xFF

/** cast any pointer to @ref tmedia_codec_t* object */
#define TMEDIA_CODEC(self)		((tmedia_codec_t*)(self))

/** Base object for all Codecs */
typedef struct tmedia_codec_s
{
	TSK_DECLARE_OBJECT;

	//! the type of the codec
	tmedia_type_t type;
	//! whether the pay. type is dyn. or not
	tsk_bool_t dyn;
	//! the name of the codec. e.g. "G.711U" or "G.711A" etc used in the sdp
	char* name;
	//! full description
	char* desc;
	//! the format. e.g. "0" for PCMU or "8" for PCMA or "*" for MSRP.
	char* format;
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
	int32_t rate;

	struct{
		int8_t channels;
		/* ...to be continued */
	} audio;

	struct{
		unsigned width;
		unsigned height;
		/* ...to be continued */
	} video;
	
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
TINYMEDIA_API int tmedia_codec_cmp(const tsk_object_t* codec1, const tsk_object_t* codec2);
TINYMEDIA_API int tmedia_codec_plugin_register(const tmedia_codec_plugin_def_t* plugin);
TINYMEDIA_API int tmedia_codec_plugin_unregister(const tmedia_codec_plugin_def_t* plugin);
TINYMEDIA_API tmedia_codec_t* tmedia_codec_create(const char* format);
TINYMEDIA_API char* tmedia_codec_get_rtpmap(const tmedia_codec_t* self);
TINYMEDIA_API char* tmedia_codec_get_fmtp(const tmedia_codec_t* self);
TINYMEDIA_API tsk_bool_t tmedia_codec_match_fmtp(const tmedia_codec_t* self, const char* fmtp);
TINYMEDIA_API int tmedia_codec_set_remote_fmtp(tmedia_codec_t* self, const char* fmtp);
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
