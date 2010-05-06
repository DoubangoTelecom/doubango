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

/**@file tmedia_codec.c
 * @brief Base codec object.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinymedia/tmedia_codec.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

/**@defgroup tmedia_codec_group Codecs
*/

/**@ingroup tmedia_codec_group
* Initialize a Codec 
* @param codec The codec to initialize. Could be any type of codec (e.g. @ref tmedia_codec_audio_t or @ref tmedia_codec_video_t).
* @param type
* @param name the name of the codec. e.g. "G.711u" or "G.711a" etc using in the sdp.
* @param desc full description.
* @param format the format. e.g. "0" for G.711.u or "8" for G.711a or "*" for MSRP.
* @retval Zero if succeed and non-zero error code otherwise.
*/
int tmedia_codec_init(tmedia_codec_t* codec, tmedia_codec_type_t type, const char* name, const char* desc, const char* format)
{
	if(!codec || tsk_strnullORempty(name)){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	codec->type = type;
	tsk_strupdate(&codec->name, name);
	tsk_strupdate(&codec->desc,desc);
	tsk_strupdate(&codec->format, format);

	return 0;
}

/**@ingroup tmedia_codec_group
* Generic function to compare two codecs.
* @param codec1 The first codec to compare.
* @param codec2 The second codec to compare.
* @retval Returns an integral value indicating the relationship between the two codecs:
* <0 : @a codec1 less than @a codec2.<br>
* 0  : @a codec1 identical to @a codec2.<br>
* >0 : @a codec1 greater than @a codec2.<br>
*/
int tmedia_codec_cmp(const tmedia_codec_t* codec1, const tmedia_codec_t* codec2)
{
	if((codec1 && codec2) && (codec1->type == codec2->type)){
		/* Do not compare names. For example, H264 base profile 1.0 will have the 
		* same name than H264 base profile 3.0. */
		return tsk_stricmp(codec1->format, codec2->format);
	}
	else{
		return -1;
	}
}

/**@ingroup tmedia_codec_group
* DeInitialize a Codec 
* @param codec The codec to deinitialize. Could be any type of codec (e.g. @ref tmedia_codec_audio_t or @ref tmedia_codec_video_t).
* @retval Zero if succeed and non-zero error code otherwise.
*/
int tmedia_codec_deinit(tmedia_codec_t* codec)
{
	if(!codec){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	TSK_FREE(codec->name);
	TSK_FREE(codec->desc);
	TSK_FREE(codec->format);

	return 0;
}

