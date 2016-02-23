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
#include "tinydav/audio/alsa/tdav_consumer_alsa.h"

#if HAVE_ALSA_ASOUNDLIB_H

#include "tinydav/audio/alsa/tdav_common_alsa.h"

#define ALSA_DEBUG_INFO(FMT, ...) TSK_DEBUG_INFO("[ALSA Consumer] " FMT, ##__VA_ARGS__)
#define ALSA_DEBUG_WARN(FMT, ...) TSK_DEBUG_WARN("[ALSA Consumer] " FMT, ##__VA_ARGS__)
#define ALSA_DEBUG_ERROR(FMT, ...) TSK_DEBUG_ERROR("[ALSA Consumer] " FMT, ##__VA_ARGS__)
#define ALSA_DEBUG_FATAL(FMT, ...) TSK_DEBUG_FATAL("[ALSA Consumer] " FMT, ##__VA_ARGS__)

typedef struct tdav_consumer_alsa_s {
    TDAV_DECLARE_CONSUMER_AUDIO;

    tsk_bool_t b_muted;
    tsk_bool_t b_started;
    tsk_bool_t b_paused;

    tsk_thread_handle_t* tid[1];

    struct tdav_common_alsa_s alsa_common;
}
tdav_consumer_alsa_t;

static void* TSK_STDCALL _tdav_producer_alsa_playback_thread(void *param)
{
    tdav_consumer_alsa_t*  p_alsa = (tdav_consumer_alsa_t*)param;
    int err;

    ALSA_DEBUG_INFO("__playback_thread -- START");

    tsk_thread_set_priority_2(TSK_THREAD_PRIORITY_TIME_CRITICAL);

    while (p_alsa->b_started) {
        tdav_common_alsa_lock(&p_alsa->alsa_common);
        //snd_pcm_wait(p_alsa->alsa_common.p_handle, 20);
        //ALSA_DEBUG_INFO ("get (%d)", p_alsa->alsa_common.n_buff_size_in_bytes);
        err = tdav_consumer_audio_get(TDAV_CONSUMER_AUDIO(p_alsa), p_alsa->alsa_common.p_buff_ptr, p_alsa->alsa_common.n_buff_size_in_bytes); // requires 16bits, thread-safe
        //ALSA_DEBUG_INFO ("get returned %d", err);
        if (err < p_alsa->alsa_common.n_buff_size_in_bytes) {
            memset(((uint8_t*)p_alsa->alsa_common.p_buff_ptr) + err, 0, (p_alsa->alsa_common.n_buff_size_in_bytes - err));

        }
        if ((err = snd_pcm_writei(p_alsa->alsa_common.p_handle, p_alsa->alsa_common.p_buff_ptr, p_alsa->alsa_common.n_buff_size_in_samples)) != p_alsa->alsa_common.n_buff_size_in_samples) {
            if (err == -EPIPE) { // pipe broken
                err = snd_pcm_recover(p_alsa->alsa_common.p_handle, err, 0);
                if (err == 0) {
                    ALSA_DEBUG_INFO ("recovered");
                    goto next;
                }
            }
            ALSA_DEBUG_ERROR ("Failed to read data from audio interface failed (%d->%s)", err, snd_strerror(err));
            tdav_common_alsa_unlock(&p_alsa->alsa_common);
            goto bail;
        }
        tdav_consumer_audio_tick(TDAV_CONSUMER_AUDIO(p_alsa));
next:
        tdav_common_alsa_unlock(&p_alsa->alsa_common);
    }
bail:
    ALSA_DEBUG_INFO("__playback_thread -- STOP");
    return tsk_null;
}


/* ============ Media Consumer Interface ================= */
static int tdav_consumer_alsa_set(tmedia_consumer_t* self, const tmedia_param_t* param)
{
    tdav_consumer_alsa_t* p_alsa = (tdav_consumer_alsa_t*)self;
    int ret = 0;

    ret = tdav_consumer_audio_set(TDAV_CONSUMER_AUDIO(self), param);

    return ret;
}

static int tdav_consumer_alsa_prepare(tmedia_consumer_t* self, const tmedia_codec_t* codec)
{
    tdav_consumer_alsa_t*  p_alsa = (tdav_consumer_alsa_t*)self;
    int err = 0;
    ALSA_DEBUG_INFO("******* tdav_consumer_alsa_prepare ******");

    if (! p_alsa || !codec && codec->plugin) {
        ALSA_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tdav_common_alsa_lock(&p_alsa->alsa_common);

    // Set using requested
    TMEDIA_CONSUMER(p_alsa)->audio.ptime = TMEDIA_CODEC_PTIME_AUDIO_DECODING(codec);
    TMEDIA_CONSUMER(p_alsa)->audio.in.channels = TMEDIA_CODEC_CHANNELS_AUDIO_DECODING(codec);
    TMEDIA_CONSUMER(p_alsa)->audio.in.rate = TMEDIA_CODEC_RATE_DECODING(codec);

    // Prepare
    err = tdav_common_alsa_prepare(&p_alsa->alsa_common, tsk_false/*is_record*/, TMEDIA_CONSUMER( p_alsa)->audio.ptime, TMEDIA_CONSUMER( p_alsa)->audio.in.channels, TMEDIA_CONSUMER( p_alsa)->audio.in.rate);
    if (err) {
        goto bail;
    }

    ALSA_DEBUG_INFO("prepared: req_channels=%d; req_rate=%d, resp_channels=%d; resp_rate=%d",
                    TMEDIA_CONSUMER(p_alsa)->audio.in.channels, TMEDIA_CONSUMER(p_alsa)->audio.in.rate,
                    p_alsa->alsa_common.channels, p_alsa->alsa_common.sample_rate);

    // Set using supported (up to the resampler to convert to requested)
    TMEDIA_CONSUMER(p_alsa)->audio.out.channels = p_alsa->alsa_common.channels;
    TMEDIA_CONSUMER(p_alsa)->audio.out.rate = p_alsa->alsa_common.sample_rate;

bail:
    tdav_common_alsa_unlock(&p_alsa->alsa_common);
    return err;
}

static int tdav_consumer_alsa_start(tmedia_consumer_t* self)
{
    tdav_consumer_alsa_t* p_alsa = (tdav_consumer_alsa_t*)self;
    int err = 0;

    ALSA_DEBUG_INFO("******* tdav_consumer_alsa_start ******");

    if (!p_alsa) {
        ALSA_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tdav_common_alsa_lock(&p_alsa->alsa_common);

    if (p_alsa->b_started) {
        ALSA_DEBUG_WARN("Already started");
        goto bail;
    }

    /* start device */
    err = tdav_common_alsa_start(&p_alsa->alsa_common);
    if (err) {
        goto bail;
    }

    /* start thread */
    p_alsa->b_started = tsk_true;
    tsk_thread_create(&p_alsa->tid[0], _tdav_producer_alsa_playback_thread,  p_alsa);

    ALSA_DEBUG_INFO("started");

bail:
    tdav_common_alsa_unlock(&p_alsa->alsa_common);
    return err;
}

static int tdav_consumer_alsa_consume(tmedia_consumer_t* self, const void* buffer, tsk_size_t size, const tsk_object_t* proto_hdr)
{
    int err = 0;
    tdav_consumer_alsa_t* p_alsa = (tdav_consumer_alsa_t*)self;

    if (!p_alsa || !buffer || !size) {
        ALSA_DEBUG_ERROR("Invalid paramter");
        return -1;
    }

    //tdav_common_alsa_lock(&p_alsa->alsa_common);

    if (!p_alsa->b_started) {
        ALSA_DEBUG_WARN("Not started");
        err = -2;
        goto bail;
    }

    if ((err = tdav_consumer_audio_put(TDAV_CONSUMER_AUDIO(p_alsa), buffer, size, proto_hdr))) {//thread-safe
        ALSA_DEBUG_WARN("Failed to put audio data to the jitter buffer");
        goto bail;
    }

bail:
    //tdav_common_alsa_unlock(&p_alsa->alsa_common);
    return err;
}

static int tdav_consumer_alsa_pause(tmedia_consumer_t* self)
{
    return 0;
}

static int tdav_consumer_alsa_stop(tmedia_consumer_t* self)
{
    tdav_consumer_alsa_t*  p_alsa = (tdav_consumer_alsa_t*)self;
    int err;

    if (!p_alsa) {
        ALSA_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    /* should be done here */
    p_alsa->b_started = tsk_false;

    err = tdav_common_alsa_stop(&p_alsa->alsa_common);

    /* stop thread */
    if (p_alsa->tid[0]) {
        tsk_thread_join(&(p_alsa->tid[0]));
    }

    ALSA_DEBUG_INFO("stopped");

    return 0;
}


//
//	ALSA consumer object definition
//
/* constructor */
static tsk_object_t* tdav_consumer_alsa_ctor(tsk_object_t * self, va_list * app)
{
    tdav_consumer_alsa_t *p_alsa = self;
    if (p_alsa) {
        ALSA_DEBUG_INFO("create");
        /* init base */
        tdav_consumer_audio_init(TDAV_CONSUMER_AUDIO(p_alsa));
        /* init self */
        tdav_common_alsa_init(&p_alsa->alsa_common);
    }
    return self;
}
/* destructor */
static tsk_object_t* tdav_consumer_alsa_dtor(tsk_object_t * self)
{
    tdav_consumer_alsa_t *p_alsa = self;
    if (p_alsa) {
        /* stop */
        if (p_alsa->b_started) {
            tdav_consumer_alsa_stop((tmedia_consumer_t*)p_alsa);
        }

        /* deinit base */
        tdav_consumer_audio_deinit(TDAV_CONSUMER_AUDIO(p_alsa));
        /* deinit self */
        tdav_common_alsa_deinit(&p_alsa->alsa_common);

        ALSA_DEBUG_INFO("*** destroyed ***");
    }

    return self;
}
/* object definition */
static const tsk_object_def_t tdav_consumer_alsa_def_s = {
    sizeof(tdav_consumer_alsa_t),
    tdav_consumer_alsa_ctor,
    tdav_consumer_alsa_dtor,
    tdav_consumer_audio_cmp,
};
/* plugin definition*/
static const tmedia_consumer_plugin_def_t tdav_consumer_alsa_plugin_def_s = {
    &tdav_consumer_alsa_def_s,

    tmedia_audio,
    "Linux ALSA consumer",

    tdav_consumer_alsa_set,
    tdav_consumer_alsa_prepare,
    tdav_consumer_alsa_start,
    tdav_consumer_alsa_consume,
    tdav_consumer_alsa_pause,
    tdav_consumer_alsa_stop
};
const tmedia_consumer_plugin_def_t *tdav_consumer_alsa_plugin_def_t = &tdav_consumer_alsa_plugin_def_s;

#endif /* #if HAVE_ALSA_ASOUNDLIB_H */
