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

#include "tsk_list.h"

TMEDIA_BEGIN_DECLS

/* === http://www.iana.org/assignments/rtp-parameters ===*/
#define TMEDIA_CODEC_FORMAT_G711u	"0"
#define TMEDIA_CODEC_FORMAT_GSM		"3"
#define TMEDIA_CODEC_FORMAT_G711a	"8"


/** cast any pointer to @ref tmedia_codec_t* object */
#define TMEDIA_CODEC(self)		((tmedia_codec_t*)(self))

/** list of all supported code types */
typedef enum tmedia_codec_type_e
{
	tmed_codec_type_audio,
	tmed_codec_type_video,
	tmed_codec_type_msrp
}
tmedia_codec_type_t;

/** Base object for all Codecs */
typedef struct tmedia_codec_s
{
	TSK_DECLARE_OBJECT;

	//! the type of the codec
	tmedia_codec_type_t type;

	//! the name of the codec. e.g. "G.711U" or "G.711A" etc using in the sdp.
	char* name;
	//! full description
	char* desc;
	//! the format. e.g. "0" for PCMU or "8" for PCMA or "*" for MSRP.
	char* format;
}
tmedia_codec_t;

/** List of @ref tmedia_codec_t elements */
typedef tsk_list_t tmedia_codecs_L_t;

/**< Declare base class as codec */
#define TMEDIA_DECLARE_CODEC tmedia_codec_t __codec__

int tmedia_codec_init(tmedia_codec_t* codec, tmedia_codec_type_t type, const char* name, const char* desc, const char* format);
int tmedia_codec_cmp(const tmedia_codec_t* codec1, const tmedia_codec_t* codec2);
int tmedia_codec_deinit(tmedia_codec_t* codec);

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
#define tmedia_codec_audio_init(self, name, desc, format) tmedia_codec_init(TMEDIA_CODEC(self), tmed_codec_type_audio, name, desc, format)
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
#define tmedia_codec_video_init(self, name, desc, format) tmedia_codec_init(TMEDIA_CODEC(self), tmed_codec_type_video, name, desc, format)
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
#define tmedia_codec_msrp_init(self, name, desc) tmedia_codec_init(TMEDIA_CODEC(self), tmed_codec_type_msrp, name, desc, "*")
#define tmedia_codec_msrp_deinit(self) tmedia_codec_deinit(TMEDIA_CODEC(self))

TMEDIA_END_DECLS

#endif /* TINYMEDIA_CODEC_H */
