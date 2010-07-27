/*
* Copyright (C) 2009-2010 Mamadou Diop.
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

/**@file tmedia_common.h
 * @brief Common functions and definitions.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYMEDIA_COMMON_H
#define TINYMEDIA_COMMON_H

#include "tinymedia_config.h"

#include "tinysdp/tsdp_message.h"

TMEDIA_BEGIN_DECLS

/** List of all supported media types */
typedef enum tmedia_type_e
{
	tmedia_none = 0x00,
	tmedia_ghost = (0x01 << 0),

	tmedia_audio = (0x01 << 1),
	tmedia_video = (0x01 << 2),
	tmedia_msrp = (0x01 << 3),
	tmedia_t38 = (0x01 << 4),

	tmedia_audiovideo = (tmedia_audio | tmedia_video),

	tmedia_all = (tmedia_audio | tmedia_video | tmedia_msrp | tmedia_t38)
}
tmedia_type_t;

typedef enum tmedia_chroma_e
{
	tmedia_rgb24,
	tmedia_rgb565le,	// (used by android consumer)
	tmedia_rgb565be,
	tmedia_nv21, // Yuv420 SP (used by android producer)
	tmedia_yuv420p, // Default
}
tmedia_chroma_t;

TINYMEDIA_API tmedia_type_t tmedia_type_from_sdp(const tsdp_message_t* sdp);
int tmedia_parse_rtpmap(const char* rtpmap, char** name, int32_t* rate, int32_t* channels);

TMEDIA_END_DECLS

#endif /* TINYMEDIA_COMMON_H */
