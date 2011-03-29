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

/**@file tdav_consumer_coreaudio.h
 * @brief Audio Consumer for MacOSX and iOS platforms.
 *
 * @author Laurent Etiemble <laurent.etiemble(at)gmail.com>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 letiemble
 */
#ifndef TINYDAV_CONSUMER_COREAUDIO_H
#define TINYDAV_CONSUMER_COREAUDIO_H

#include "tinydav_config.h"

#if HAVE_COREAUDIO

#include <AudioToolbox/AudioToolbox.h>
#include "tinydav/audio/tdav_consumer_audio.h"

TDAV_BEGIN_DECLS

#define CoreAudioPlayBuffers 3

typedef struct tdav_consumer_coreaudio_s
{
	TDAV_DECLARE_CONSUMER_AUDIO;
    
	tsk_bool_t started;
    
    AudioStreamBasicDescription description;
    AudioQueueRef queue;
    AudioQueueBufferRef buffers[CoreAudioPlayBuffers];
    
    tsk_size_t buffer_size;
}
tdav_consumer_coreaudio_t;

TINYDAV_GEXTERN const tmedia_consumer_plugin_def_t *tdav_consumer_coreaudio_plugin_def_t;

TDAV_END_DECLS

#endif /* HAVE_COREAUDIO */

#endif /* TINYDAV_CONSUMER_COREAUDIO_H */
