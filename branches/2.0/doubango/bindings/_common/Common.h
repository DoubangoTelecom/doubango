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
#ifndef TINYWRAP_COMMON_H
#define TINYWRAP_COMMON_H

#include "tinyWRAP_config.h"

#if ANDROID
#	define dyn_cast	static_cast
#	define __JNIENV JNIEnv
#else
#	define dyn_cast	dynamic_cast
#	define __JNIENV void
#endif

typedef enum twrap_media_type_e
{
	// because of Java don't use OR
	twrap_media_none = 0x00,

	twrap_media_audio = 0x01,
	twrap_media_video = 0x02,
	twrap_media_msrp = 0x04,
	twrap_media_t140 = 0x08,

	twrap_media_audio_t140 = 0x09,
	twrap_media_video_t140 = 0x0a,
	twrap_media_audiovideo = 0x03, /* @deprecated */
	twrap_media_audio_video = twrap_media_audiovideo,
	twrap_media_audio_video_t140 = 0x0b
}
twrap_media_type_t;

#if !defined(SWIG)
#include "tinymedia/tmedia_common.h"
static tmedia_type_t twrap_get_media_type(twrap_media_type_t type)
{
	int media_type = tmedia_none; // Use int because | operator not defined for enumerators
	switch(type){
		case twrap_media_msrp:
			media_type |= tmedia_msrp;
			break;
		case twrap_media_audio:
			media_type |= tmedia_audio;
			break;
		case twrap_media_video:
			media_type |= tmedia_video;
			break;
		case twrap_media_audio_video:
			media_type |= (tmedia_audio | tmedia_video);
			break;
		case twrap_media_t140:
			media_type |= tmedia_t140;
			break;
		case twrap_media_audio_t140:
			media_type |= (tmedia_audio | tmedia_t140);
			break;
		case twrap_media_video_t140:
			media_type |= (tmedia_video | tmedia_t140);
			break;
		case twrap_media_audio_video_t140:
			media_type |= (tmedia_audio | tmedia_video | tmedia_t140);
			break;
        default:
            break;
	}
	return (tmedia_type_t)media_type;
}
#endif

#endif /* TINYWRAP_COMMON_H */

