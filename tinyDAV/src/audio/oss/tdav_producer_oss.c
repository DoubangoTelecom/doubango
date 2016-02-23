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
#include "tinydav/audio/oss/tdav_producer_oss.h"

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

#define OSS_DEBUG_INFO(FMT, ...) TSK_DEBUG_INFO("[OSS Producer] " FMT, ##__VA_ARGS__)
#define OSS_DEBUG_WARN(FMT, ...) TSK_DEBUG_WARN("[OSS Producer] " FMT, ##__VA_ARGS__)
#define OSS_DEBUG_ERROR(FMT, ...) TSK_DEBUG_ERROR("[OSS Producer] " FMT, ##__VA_ARGS__)
#define OSS_DEBUG_FATAL(FMT, ...) TSK_DEBUG_FATAL("[OSS Producer] " FMT, ##__VA_ARGS__)

typedef struct tdav_producer_oss_s {
    TDAV_DECLARE_PRODUCER_AUDIO;

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
tdav_producer_oss_t;

static int __oss_from_8bits_to_16bits(const void* p_src, void* p_dst, tsk_size_t n_samples)
{
    tsk_size_t i;
    const uint8_t *_p_src = (const uint8_t*)p_src;
    uint16_t *_p_dst = (uint16_t*)p_dst;

    if (!p_src || !p_dst || !n_samples) {
        OSS_DEBUG_ERROR("invalid parameter");
        return -1;
    }
    for (i = 0; i < n_samples; ++i) {
        _p_dst[i] = _p_src[i];
    }
    return 0;
}

static void* TSK_STDCALL _tdav_producer_oss_record_thread(void *param)
{
    tdav_producer_oss_t*  p_oss = (tdav_producer_oss_t*)param;
    int err;
    const void* p_buffer = ((p_oss->n_bits_per_sample == 8) ? (const void*)p_oss->p_buff16_ptr: (const void*)p_oss->p_buff_ptr);
    tsk_size_t n_buffer_in_bytes = (p_oss->n_bits_per_sample == 8) ?  p_oss->n_buff16_size_in_bytes :  p_oss->n_buff_size_in_bytes;

    OSS_DEBUG_INFO("__record_thread -- START");

    tsk_thread_set_priority_2(TSK_THREAD_PRIORITY_TIME_CRITICAL);

    while (p_oss->b_started) {
        tsk_safeobj_lock(p_oss);
        if ((err = read(p_oss->fd, p_oss->p_buff_ptr, p_oss->n_buff_size_in_bytes)) != p_oss->n_buff_size_in_bytes) {
            OSS_DEBUG_ERROR ("Failed to read data from audio interface failed (%d -> %s)", err , strerror(errno));
            tsk_safeobj_unlock(p_oss);
            goto bail;
        }
        if (p_oss->n_bits_per_sample == 8) {
            if ((err = __oss_from_8bits_to_16bits(p_oss->p_buff_ptr, p_oss->p_buff16_ptr, p_oss->n_buff_size_in_samples))) {
                tsk_safeobj_unlock(p_oss);
                goto bail;
            }
        }
        if (!p_oss->b_muted && TMEDIA_PRODUCER(p_oss)->enc_cb.callback) {
            TMEDIA_PRODUCER(p_oss)->enc_cb.callback(TMEDIA_PRODUCER(p_oss)->enc_cb.callback_data, p_buffer, n_buffer_in_bytes);
        }
        tsk_safeobj_unlock(p_oss);
    }
bail:
    OSS_DEBUG_INFO("__record_thread -- STOP");
    return tsk_null;
}


/* ============ Media Producer Interface ================= */
static int tdav_producer_oss_set(tmedia_producer_t* self, const tmedia_param_t* param)
{
    tdav_producer_oss_t*  p_oss = (tdav_producer_oss_t*)self;
    if (param->plugin_type == tmedia_ppt_producer) {
        if (param->value_type == tmedia_pvt_int32) {
            if (tsk_striequals(param->key, "volume")) {
                return 0;
            }
            else if(tsk_striequals(param->key, "mute")) {
                p_oss->b_muted = (TSK_TO_INT32((uint8_t*)param->value) != 0);
                return 0;
            }
        }
    }
    return tdav_producer_audio_set(TDAV_PRODUCER_AUDIO(self), param);
}

static int tdav_producer_oss_prepare(tmedia_producer_t* self, const tmedia_codec_t* codec)
{
    tdav_producer_oss_t*  p_oss = (tdav_producer_oss_t*)self;
    int err = 0, channels, sample_rate, bits_per_sample;

    if (!p_oss || !codec && codec->plugin) {
        OSS_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tsk_safeobj_lock(p_oss);

    if (p_oss->fd == -1) {
        if ((p_oss->fd = open("/dev/dsp", O_RDONLY)) < 0) {
            OSS_DEBUG_ERROR("open('/dev/dsp') failed: %s", strerror(errno));
            err = -2;
            goto bail;
        }
    }

    // Set using requested
    channels = TMEDIA_CODEC_CHANNELS_AUDIO_ENCODING(codec);
    sample_rate = TMEDIA_CODEC_RATE_ENCODING(codec);
    bits_per_sample = TMEDIA_PRODUCER(p_oss)->audio.bits_per_sample; // 16

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

    p_oss->n_buff_size_in_bytes = (TMEDIA_PRODUCER(p_oss)->audio.ptime * sample_rate * ((bits_per_sample >> 3) * channels)) / 1000;
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
                   TMEDIA_PRODUCER(p_oss)->audio.bits_per_sample, TMEDIA_PRODUCER(p_oss)->audio.channels, TMEDIA_PRODUCER(p_oss)->audio.rate,
                   bits_per_sample, channels, sample_rate,
                   p_oss->n_buff_size_in_samples, p_oss->n_buff_size_in_bytes);

    // Set using supported (up to the resampler to convert to requested)
    TMEDIA_PRODUCER(p_oss)->audio.ptime = TMEDIA_CODEC_PTIME_AUDIO_ENCODING(codec);
    TMEDIA_PRODUCER(p_oss)->audio.channels = channels;
    TMEDIA_PRODUCER(p_oss)->audio.rate = sample_rate;
    // TMEDIA_PRODUCER(p_oss)->audio.bits_per_sample = bits_per_sample;

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

static int tdav_producer_oss_start(tmedia_producer_t* self)
{
    tdav_producer_oss_t* p_oss = (tdav_producer_oss_t*)self;
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
    tsk_thread_create(&p_oss->tid[0], _tdav_producer_oss_record_thread,  p_oss);

    OSS_DEBUG_INFO("started");

bail:
    tsk_safeobj_unlock(p_oss);
    return err;
}

static int tdav_producer_oss_pause(tmedia_producer_t* self)
{
    tdav_producer_oss_t*  p_oss = (tdav_producer_oss_t*)self;

    if (!p_oss) {
        OSS_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    OSS_DEBUG_INFO("paused");

    return 0;
}

static int tdav_producer_oss_stop(tmedia_producer_t* self)
{
    tdav_producer_oss_t*  p_oss = (tdav_producer_oss_t*)self;
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
//	Linux OSS producer object definition
//
/* constructor */
static tsk_object_t* tdav_producer_oss_ctor(tsk_object_t * self, va_list * app)
{
    tdav_producer_oss_t *p_oss = (tdav_producer_oss_t*)self;
    if (p_oss) {
        /* init base */
        tdav_producer_audio_init(TDAV_PRODUCER_AUDIO(p_oss));
        /* init self */
        p_oss->fd = -1;
        tsk_safeobj_init(p_oss);
    }
    return self;
}
/* destructor */
static tsk_object_t* tdav_producer_oss_dtor(tsk_object_t * self)
{
    tdav_producer_oss_t *p_oss = (tdav_producer_oss_t *)self;
    if (p_oss) {
        /* stop */
        if (p_oss->b_started) {
            tdav_producer_oss_stop((tmedia_producer_t*)p_oss);
        }

        /* deinit base */
        tdav_producer_audio_deinit(TDAV_PRODUCER_AUDIO(p_oss));
        /* deinit self */
        if (p_oss->fd != -1) {
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
static const tsk_object_def_t tdav_producer_oss_def_s = {
    sizeof(tdav_producer_oss_t),
    tdav_producer_oss_ctor,
    tdav_producer_oss_dtor,
    tdav_producer_audio_cmp,
};
/* plugin definition*/
static const tmedia_producer_plugin_def_t tdav_producer_oss_plugin_def_s = {
    &tdav_producer_oss_def_s,

    tmedia_audio,
    "Linux OSS producer",

    tdav_producer_oss_set,
    tdav_producer_oss_prepare,
    tdav_producer_oss_start,
    tdav_producer_oss_pause,
    tdav_producer_oss_stop
};
const tmedia_producer_plugin_def_t *tdav_producer_oss_plugin_def_t = &tdav_producer_oss_plugin_def_s;

#endif /* HAVE_LINUX_SOUNDCARD_H */
