/*
* Copyright (C) 2011 Mamadou Diop.
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

/**@file tdav_speakup_JitterBuffer.h
 * @brief Speakup audio JitterBuffer Plugin
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 */
#ifndef TINYDAV_SPEAKUP_JITTER_BUFFER_H
#define TINYDAV_SPEAKUP_JITTER_BUFFER_H

#include "tinydav_config.h"

#if !(HAVE_SPEEX_DSP && HAVE_SPEEX_JB)

#include "tinymedia/tmedia_jitterbuffer.h"

#include "tinydav/audio/tdav_jitterbuffer.h"

TDAV_BEGIN_DECLS

/** Speakuo JitterBufferr*/
typedef struct tdav_speakup_jitterBuffer_s
{
	TMEDIA_DECLARE_JITTER_BUFFER;

	jitterbuffer *jbuffer;
	uint8_t jcodec;
	uint64_t ref_timestamp;
	uint32_t frame_duration;
	uint32_t rate;
	uint32_t channels;
	uint32_t _10ms_size_bytes;
}
tdav_speakup_jitterbuffer_t;

TINYDAV_GEXTERN const tmedia_jitterbuffer_plugin_def_t *tdav_speakup_jitterbuffer_plugin_def_t;

TDAV_END_DECLS

#endif /* !(HAVE_SPEEX_DSP && HAVE_SPEEX_JB) */

#endif /* TINYDAV_SPEAKUP_JITTER_BUFFER_H */
