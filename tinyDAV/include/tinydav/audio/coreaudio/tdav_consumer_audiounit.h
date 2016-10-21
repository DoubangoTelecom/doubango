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
#ifndef TINYDAV_CONSUMER_COREAUDIO_AUDIO_UNIT_H
#define TINYDAV_CONSUMER_COREAUDIO_AUDIO_UNIT_H

#include "tinydav_config.h"

#if HAVE_COREAUDIO_AUDIO_UNIT

#include <AudioToolbox/AudioToolbox.h>
#include <speex/speex_buffer.h>
#include "tinydav/audio/coreaudio/tdav_audiounit.h"
#include "tinydav/audio/tdav_consumer_audio.h"
#import "tinymedia/tmedia_resampler.h"

#include "tsk_mutex.h"

TDAV_BEGIN_DECLS

typedef struct tdav_consumer_audiounit_s {
    TDAV_DECLARE_CONSUMER_AUDIO;

    tdav_audiounit_handle_t* audioUnitHandle;
    unsigned started:1;
    unsigned paused:1;
    unsigned ready:1;

    struct {
        struct {
            void* buffer;
            tsk_size_t size;
        } chunck;
        tsk_ssize_t leftBytes;
        SpeexBuffer* buffer;
        tsk_size_t size;
        tsk_mutex_handle_t* mutex;
    } ring;

    tmedia_resampler_t *resampler;
}
tdav_consumer_audiounit_t;

TINYDAV_GEXTERN const tmedia_consumer_plugin_def_t *tdav_consumer_audiounit_plugin_def_t;

TDAV_END_DECLS

#endif /* HAVE_COREAUDIO_AUDIO_UNIT */

#endif /* TINYDAV_CONSUMER_COREAUDIO_AUDIO_UNIT_H */
