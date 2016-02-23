/* Copyright (C) 2014 Mamadou DIOP.
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
*/
#include "tinydav/audio/oss/tdav_consumer_oss.h"

#if HAVE_LINUX_SOUNDCARD_H

#include "tsk_string.h"
#include "tsk_thread.h"
#include "tsk_memory.h"
#include "tsk_safeobj.h"
#include "tsk_debug.h"

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/soundcard.h>

#define OSS_DEBUG_INFO(FMT, ...) TSK_DEBUG_INFO("[OSS Consumer] " FMT, ##__VA_ARGS__)
#define OSS_DEBUG_WARN(FMT, ...) TSK_DEBUG_WARN("[OSS Consumer] " FMT, ##__VA_ARGS__)
#define OSS_DEBUG_ERROR(FMT, ...) TSK_DEBUG_ERROR("[OSS Consumer] " FMT, ##__VA_ARGS__)
#define OSS_DEBUG_FATAL(FMT, ...) TSK_DEBUG_FATAL("[OSS Consumer] " FMT, ##__VA_ARGS__)

typedef struct tdav_consumer_oss_s {
    TDAV_DECLARE_CONSUMER_AUDIO;

    tsk_bool_t b_started;
    tsk_bool_t b_prepared;
    tsk_bool_t b_muted;
    int n_bits_per_sample;

    int fd;
    tsk_thread_handle_t* tid[1];

    tsk_size_t n_buff_size_in_bytes;
    tsk_size_t n_buff_size_in_samples;
    uint8_t* p_buff_ptr;

    tsk_size_t n_buff16_size_in_bytes;
    tsk_size_t n_buff16_size_in_samples;
    uint16_t* p_buff16_ptr;

    TSK_DECLARE_SAFEOBJ;
}
tdav_consumer_oss_t;

static int __oss_from_16bits_to_8bits(const void* p_src, void* p_dst, tsk_size_t n_samples)
{
    tsk_size_t i;
    uint16_t *_p_src = (uint16_t*)p_src;
    uint8_t *_p_dst = (uint8_t*)p_dst;

    if (!p_src || !p_dst || !n_samples) {
        OSS_DEBUG_ERROR("invalid parameter");
        return -1;
    }
    for (i = 0; i < n_samples; ++i) {
        _p_dst[i] = _p_src[i];
    }
    return 0;
}

static void* TSK_STDCALL _tdav_consumer_oss_playback_thread(void *param)
{
    tdav_consumer_oss_t*  p_oss = (tdav_consumer_oss_t*)param;
    int err;
    void* p_buffer = ((p_oss->n_bits_per_sample == 8) ? (void*)p_oss->p_buff16_ptr: (void*)p_oss->p_buff_ptr);
    tsk_size_t n_buffer_in_bytes = (p_oss->n_bits_per_sample == 8) ?  p_oss->n_buff16_size_in_bytes :  p_oss->n_buff_size_in_bytes;
    tsk_size_t n_buffer_in_samples = p_oss->n_buff_size_in_samples;

    const void* _p_buffer;
    tsk_size_t _n_buffer_in_bytes;

    OSS_DEBUG_INFO("__playback_thread -- START");

    tsk_thread_set_priority_2(TSK_THREAD_PRIORITY_TIME_CRITICAL);

    while (p_oss->b_started) {
        tsk_safeobj_lock(p_oss);
        err = tdav_consumer_audio_get(TDAV_CONSUMER_AUDIO(p_oss), p_buffer, n_buffer_in_bytes); // requires 16bits, thread-safe
        if (err >= 0) {
            _p_buffer = p_buffer;
            _n_buffer_in_bytes = n_buffer_in_bytes;
            if (err < n_buffer_in_bytes) {
                memset(((uint8_t*)p_buffer) + err, 0, (n_buffer_in_bytes - err));
            }
            if (p_oss->n_bits_per_sample == 8) {
                __oss_from_16bits_to_8bits(p_buffer, p_oss->p_buff_ptr, n_buffer_in_samples);
                _p_buffer = p_oss->p_buff_ptr;
                _n_buffer_in_bytes >>= 1;
            }
            if ((err = write(p_oss->fd, _p_buffer, _n_buffer_in_bytes)) != _n_buffer_in_bytes) {
                OSS_DEBUG_ERROR ("Failed to read data from audio interface failed (%d -> %s)", err , strerror(errno));
                tsk_safeobj_unlock(p_oss);
                goto bail;
            }
        }
        tdav_consumer_audio_tick(TDAV_CONSUMER_AUDIO(p_oss));

        tsk_safeobj_unlock(p_oss);
    }
bail:
    OSS_DEBUG_INFO("__playback_thread -- STOP");
    return tsk_null;
}

/* ============ Media Consumer Interface ================= */
static int tdav_consumer_oss_set(tmedia_consumer_t* self, const tmedia_param_t* param)
{
    tdav_consumer_oss_t* p_oss = (tdav_consumer_oss_t*)self;
    int ret = 0;

    ret = tdav_consumer_audio_set(TDAV_CONSUMER_AUDIO(self), param);

    return ret;
}

static int tdav_consumer_oss_prepare(tmedia_consumer_t* self, const tmedia_codec_t* codec)
{
    tdav_consumer_oss_t*  p_oss = (tdav_consumer_oss_t*)self;
    int err = 0, channels, sample_rate, bits_per_sample;

    if (!p_oss || !codec && codec->plugin) {
        OSS_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tsk_safeobj_lock(p_oss);

    if (p_oss->fd == -1) {
        if ((p_oss->fd = open("/dev/dsp", O_WRONLY)) < 0) {
            OSS_DEBUG_ERROR("open('/dev/dsp') failed: %s", strerror(errno));
            err = -2;
            goto bail;
        }
    }

    TMEDIA_CONSUMER(p_oss)->audio.ptime = TMEDIA_CODEC_PTIME_AUDIO_DECODING(codec);
    TMEDIA_CONSUMER(p_oss)->audio.in.channels = TMEDIA_CODEC_CHANNELS_AUDIO_DECODING(codec);
    TMEDIA_CONSUMER(p_oss)->audio.in.rate = TMEDIA_CODEC_RATE_DECODING(codec);

    // Set using requested
    channels = TMEDIA_CONSUMER(p_oss)->audio.in.channels;
    sample_rate = TMEDIA_CONSUMER(p_oss)->audio.in.rate;
    bits_per_sample = TMEDIA_CONSUMER(p_oss)->audio.bits_per_sample; // 16

    // Prepare
    if ((err = ioctl(p_oss->fd, SOUND_PCM_WRITE_BITS, &bits_per_sample)) != 0) {
        OSS_DEBUG_ERROR("ioctl(SOUND_PCM_WRITE_BITS, %d) failed: %d->%s", bits_per_sample, err, strerror(errno));
        goto bail;
    }
    if (bits_per_sample != 16 && bits_per_sample != 8) {
        OSS_DEBUG_ERROR("bits_per_sample=%d not supported", bits_per_sample);
        err = -3;
        goto bail;
    }
    if ((err = ioctl(p_oss->fd, SOUND_PCM_WRITE_CHANNELS, &channels)) != 0) {
        OSS_DEBUG_ERROR("ioctl(SOUND_PCM_WRITE_CHANNELS, %d) failed: %d->%s", channels, err, strerror(errno));
        goto bail;
    }
    if ((err = ioctl(p_oss->fd, SOUND_PCM_WRITE_RATE, &sample_rate)) != 0) {
        OSS_DEBUG_ERROR("ioctl(SOUND_PCM_WRITE_RATE, %d) failed: %d->%s", sample_rate, err, strerror(errno));
        goto bail;
    }

    p_oss->n_buff_size_in_bytes = (TMEDIA_CONSUMER(p_oss)->audio.ptime * sample_rate * ((bits_per_sample >> 3) * channels)) / 1000;
    if (!(p_oss->p_buff_ptr = tsk_realloc(p_oss->p_buff_ptr, p_oss->n_buff_size_in_bytes))) {
        OSS_DEBUG_ERROR("Failed to allocate buffer with size = %u", p_oss->n_buff_size_in_bytes);
        err = -4;
        goto bail;
    }
    p_oss->n_buff_size_in_samples = (p_oss->n_buff_size_in_bytes / (bits_per_sample >> 3));
    if (bits_per_sample == 8) {
        p_oss->n_buff16_size_in_bytes = p_oss->n_buff_size_in_bytes << 1;
        if (!(p_oss->p_buff16_ptr = tsk_realloc(p_oss->p_buff16_ptr, p_oss->n_buff16_size_in_bytes))) {
            OSS_DEBUG_ERROR("Failed to allocate buffer with size = %u", p_oss->n_buff_size_in_bytes);
            err = -5;
            goto bail;
        }
        p_oss->n_buff16_size_in_samples = p_oss->n_buff_size_in_samples;
    }

    OSS_DEBUG_INFO("prepared: req_bits_per_sample=%d; req_channels=%d; req_rate=%d, resp_bits_per_sample=%d; resp_channels=%d; resp_rate=%d /// n_buff_size_in_samples=%u;n_buff_size_in_bytes=%u",
                   TMEDIA_CONSUMER(p_oss)->audio.bits_per_sample, TMEDIA_CONSUMER(p_oss)->audio.in.channels, TMEDIA_CONSUMER(p_oss)->audio.in.rate,
                   bits_per_sample, channels, sample_rate,
                   p_oss->n_buff_size_in_samples, p_oss->n_buff_size_in_bytes);

    // Set using supported (up to the resampler to convert to requested)
    TMEDIA_CONSUMER(p_oss)->audio.out.channels = channels;
    TMEDIA_CONSUMER(p_oss)->audio.out.rate = sample_rate;
    // TMEDIA_CONSUMER(p_oss)->audio.bits_per_sample = bits_per_sample;

    p_oss->n_bits_per_sample = bits_per_sample;
    p_oss->b_prepared = tsk_true;

bail:
    if (err) {
        if (p_oss->fd != -1) {
            close(p_oss->fd);
            p_oss->fd = -1;
        }
    }
    tsk_safeobj_unlock(p_oss);

    return err;
}

static int tdav_consumer_oss_start(tmedia_consumer_t* self)
{
    tdav_consumer_oss_t* p_oss = (tdav_consumer_oss_t*)self;
    int err = 0;

    if (! p_oss) {
        OSS_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tsk_safeobj_lock(p_oss);

    if (!p_oss->b_prepared) {
        OSS_DEBUG_WARN("Not prepared");
        err = -2;
        goto bail;
    }

    if (p_oss->b_started) {
        OSS_DEBUG_WARN("Already started");
        goto bail;
    }

    /* start thread */
    p_oss->b_started = tsk_true;
    tsk_thread_create(&p_oss->tid[0], _tdav_consumer_oss_playback_thread,  p_oss);

    OSS_DEBUG_INFO("started");

bail:
    tsk_safeobj_unlock(p_oss);
    return err;
}

static int tdav_consumer_oss_consume(tmedia_consumer_t* self, const void* buffer, tsk_size_t size, const tsk_object_t* proto_hdr)
{
    int err = 0;
    tdav_consumer_oss_t* p_oss = (tdav_consumer_oss_t*)self;

    if (!p_oss || !buffer || !size) {
        OSS_DEBUG_ERROR("Invalid paramter");
        return -1;
    }

    //tsk_safeobj_lock(p_oss);

    if (!p_oss->b_started) {
        OSS_DEBUG_WARN("Not started");
        err = -2;
        goto bail;
    }
    if ((err = tdav_consumer_audio_put(TDAV_CONSUMER_AUDIO(p_oss), buffer, size, proto_hdr))/*thread-safe*/) {
        OSS_DEBUG_WARN("Failed to put audio data to the jitter buffer");
        goto bail;
    }

bail:
    //tsk_safeobj_unlock(p_oss);
    return err;
}

static int tdav_consumer_oss_pause(tmedia_consumer_t* self)
{
    return 0;
}

static int tdav_consumer_oss_stop(tmedia_consumer_t* self)
{
    tdav_consumer_oss_t*  p_oss = (tdav_consumer_oss_t*)self;
    int err;

    if (!p_oss) {
        OSS_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tsk_safeobj_lock(p_oss);

    /* should be done here */
    p_oss->b_started = tsk_false;

    /* stop thread */
    if (p_oss->tid[0]) {
        tsk_thread_join(&(p_oss->tid[0]));
    }
    if (p_oss->fd != -1) {
        close(p_oss->fd);
        p_oss->fd = -1;
    }
    p_oss->b_prepared = tsk_false;

    OSS_DEBUG_INFO("stopped");

    tsk_safeobj_unlock(p_oss);

    return 0;
}


//
//	Linux OSS consumer object definition
//
/* constructor */
static tsk_object_t* tdav_consumer_oss_ctor(tsk_object_t * self, va_list * app)
{
    tdav_consumer_oss_t *p_oss = self;
    if (p_oss) {
        /* init base */
        tdav_consumer_audio_init(TDAV_CONSUMER_AUDIO(p_oss));
        /* init self */

        p_oss->fd = -1;
        tsk_safeobj_init(p_oss);

        OSS_DEBUG_INFO("created");
    }
    return self;
}
/* destructor */
static tsk_object_t* tdav_consumer_oss_dtor(tsk_object_t * self)
{
    tdav_consumer_oss_t *p_oss = self;
    if (p_oss) {

        /* stop */
        if (p_oss->b_started) {
            tdav_consumer_oss_stop(self);
        }

        /* deinit base */
        tdav_consumer_audio_deinit(TDAV_CONSUMER_AUDIO(p_oss));
        /* deinit self */
        if (p_oss->fd > 0) {
            close(p_oss->fd);
            p_oss->fd = -1;
        }
        TSK_FREE(p_oss->p_buff_ptr);
        TSK_FREE(p_oss->p_buff16_ptr);
        tsk_safeobj_deinit(p_oss);

        OSS_DEBUG_INFO("*** destroyed ***");
    }

    return self;
}
/* object definition */
static const tsk_object_def_t tdav_consumer_oss_def_s = {
    sizeof(tdav_consumer_oss_t),
    tdav_consumer_oss_ctor,
    tdav_consumer_oss_dtor,
    tdav_consumer_audio_cmp,
};
/* plugin definition*/
static const tmedia_consumer_plugin_def_t tdav_consumer_oss_plugin_def_s = {
    &tdav_consumer_oss_def_s,

    tmedia_audio,
    "Linux OSS consumer",

    tdav_consumer_oss_set,
    tdav_consumer_oss_prepare,
    tdav_consumer_oss_start,
    tdav_consumer_oss_consume,
    tdav_consumer_oss_pause,
    tdav_consumer_oss_stop
};
const tmedia_consumer_plugin_def_t *tdav_consumer_oss_plugin_def_t = &tdav_consumer_oss_plugin_def_s;

#endif /* HAVE_LINUX_SOUNDCARD_H */
