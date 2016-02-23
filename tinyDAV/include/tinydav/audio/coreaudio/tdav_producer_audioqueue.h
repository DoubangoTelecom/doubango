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
/**@file tdav_producer_audioqueue.h
 * @brief Audio Producer for MacOSX and iOS platforms using AudioQueue.
 *
 * @author Laurent Etiemble <laurent.etiemble(at)gmail.com>
 * @author Mamadou Diop <diopmamadou(at)doubango(dot)org>
 *
 */
#ifndef TINYDAV_PRODUCER_COREAUDIO_AUDIO_QUEUE_H
#define TINYDAV_PRODUCER_COREAUDIO_AUDIO_QUEUE_H

#include "tinydav_config.h"

#if HAVE_COREAUDIO_AUDIO_QUEUE

#include <AudioToolbox/AudioToolbox.h>
#include "tinydav/audio/tdav_producer_audio.h"

TDAV_BEGIN_DECLS

#define CoreAudioRecordBuffers 3

typedef struct tdav_producer_audioqueue_s {
    TDAV_DECLARE_PRODUCER_AUDIO;

    tsk_bool_t started;

    AudioStreamBasicDescription description;
    AudioQueueRef queue;
    AudioQueueBufferRef buffers[CoreAudioRecordBuffers];

    tsk_size_t buffer_size;
}
tdav_producer_audioqueue_t;

TINYDAV_GEXTERN const tmedia_producer_plugin_def_t *tdav_producer_audioqueue_plugin_def_t;

TDAV_END_DECLS

#endif /* HAVE_COREAUDIO_AUDIO_QUEUE */

#endif /* TINYDAV_PRODUCER_COREAUDIO_AUDIO_QUEUE_H */
