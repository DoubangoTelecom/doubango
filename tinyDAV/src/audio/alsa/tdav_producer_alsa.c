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
#include "tinydav/audio/alsa/tdav_producer_alsa.h"

#if HAVE_ALSA_ASOUNDLIB_H

#include "tinydav/audio/alsa/tdav_common_alsa.h"

#define ALSA_DEBUG_INFO(FMT, ...) TSK_DEBUG_INFO("[ALSA Producer] " FMT, ##__VA_ARGS__)
#define ALSA_DEBUG_WARN(FMT, ...) TSK_DEBUG_WARN("[ALSA Producer] " FMT, ##__VA_ARGS__)
#define ALSA_DEBUG_ERROR(FMT, ...) TSK_DEBUG_ERROR("[ALSA Producer] " FMT, ##__VA_ARGS__)
#define ALSA_DEBUG_FATAL(FMT, ...) TSK_DEBUG_FATAL("[ALSA Producer] " FMT, ##__VA_ARGS__)

typedef struct tdav_producer_alsa_s {
    TDAV_DECLARE_PRODUCER_AUDIO;

    tsk_bool_t b_muted;
    tsk_bool_t b_started;
    tsk_bool_t b_paused;

    tsk_thread_handle_t* tid[1];

    struct tdav_common_alsa_s alsa_common;
}
tdav_producer_alsa_t;

static void* TSK_STDCALL _tdav_producer_alsa_record_thread(void *param)
{
    tdav_producer_alsa_t*  p_alsa = (tdav_producer_alsa_t*)param;
    int err;

    ALSA_DEBUG_INFO("__record_thread -- START");

    tsk_thread_set_priority_2(TSK_THREAD_PRIORITY_TIME_CRITICAL);

    while (p_alsa->b_started) {
        tdav_common_alsa_lock(&p_alsa->alsa_common);
        if ((err = snd_pcm_readi(p_alsa->alsa_common.p_handle, p_alsa->alsa_common.p_buff_ptr, p_alsa->alsa_common.n_buff_size_in_samples)) != p_alsa->alsa_common.n_buff_size_in_samples) {
            ALSA_DEBUG_ERROR ("Failed to read data from audio interface failed (%d->%s)", err, snd_strerror(err));
            tdav_common_alsa_unlock(&p_alsa->alsa_common);
            goto bail;
        }
        if (!p_alsa->b_muted && TMEDIA_PRODUCER(p_alsa)->enc_cb.callback) {
            TMEDIA_PRODUCER(p_alsa)->enc_cb.callback(TMEDIA_PRODUCER(p_alsa)->enc_cb.callback_data, p_alsa->alsa_common.p_buff_ptr, p_alsa->alsa_common.n_buff_size_in_bytes);
        }
        tdav_common_alsa_unlock(&p_alsa->alsa_common);
    }
bail:
    ALSA_DEBUG_INFO("__record_thread -- STOP");
    return tsk_null;
}


/* ============ Media Producer Interface ================= */
static int tdav_producer_alsa_set(tmedia_producer_t* self, const tmedia_param_t* param)
{
    tdav_producer_alsa_t*  p_alsa = (tdav_producer_alsa_t*)self;
    if (param->plugin_type == tmedia_ppt_producer) {
        if (param->value_type == tmedia_pvt_int32) {
            if (tsk_striequals(param->key, "volume")) {
                return 0;
            }
            else if(tsk_striequals(param->key, "mute")) {
                p_alsa->b_muted = (TSK_TO_INT32((uint8_t*)param->value) != 0);
                return 0;
            }
        }
    }
    return tdav_producer_audio_set(TDAV_PRODUCER_AUDIO(self), param);
}

static int tdav_producer_alsa_prepare(tmedia_producer_t* self, const tmedia_codec_t* codec)
{
    tdav_producer_alsa_t*  p_alsa = (tdav_producer_alsa_t*)self;
    int err = 0;
    ALSA_DEBUG_INFO("******* tdav_producer_alsa_prepare ******");

    if (! p_alsa || !codec && codec->plugin) {
        ALSA_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tdav_common_alsa_lock(&p_alsa->alsa_common);

    // Set using requested
    TMEDIA_PRODUCER( p_alsa)->audio.channels = TMEDIA_CODEC_CHANNELS_AUDIO_ENCODING(codec);
    TMEDIA_PRODUCER( p_alsa)->audio.rate = TMEDIA_CODEC_RATE_ENCODING(codec);
    TMEDIA_PRODUCER( p_alsa)->audio.ptime = TMEDIA_CODEC_PTIME_AUDIO_ENCODING(codec);

    // Prepare
    err = tdav_common_alsa_prepare(&p_alsa->alsa_common, tsk_true/*is_capture*/, TMEDIA_PRODUCER( p_alsa)->audio.ptime, TMEDIA_PRODUCER( p_alsa)->audio.channels, TMEDIA_PRODUCER( p_alsa)->audio.rate);
    if (err) {
        goto bail;
    }

    ALSA_DEBUG_INFO("prepared: req_channels=%d; req_rate=%d, resp_channels=%d; resp_rate=%d",
                    TMEDIA_PRODUCER(p_alsa)->audio.channels, TMEDIA_PRODUCER(p_alsa)->audio.rate,
                    p_alsa->alsa_common.channels, p_alsa->alsa_common.sample_rate);

    // Set using supported (up to the resampler to convert to requested)
    TMEDIA_PRODUCER(p_alsa)->audio.channels = p_alsa->alsa_common.channels;
    TMEDIA_PRODUCER(p_alsa)->audio.rate = p_alsa->alsa_common.sample_rate;

bail:
    tdav_common_alsa_unlock(&p_alsa->alsa_common);
    return err;
}

static int tdav_producer_alsa_start(tmedia_producer_t* self)
{
    tdav_producer_alsa_t* p_alsa = (tdav_producer_alsa_t*)self;
    int err = 0;

    ALSA_DEBUG_INFO("******* tdav_producer_alsa_start ******");

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
    tsk_thread_create(&p_alsa->tid[0], _tdav_producer_alsa_record_thread,  p_alsa);

    ALSA_DEBUG_INFO("started");

bail:
    tdav_common_alsa_unlock(&p_alsa->alsa_common);
    return err;
}

static int tdav_producer_alsa_pause(tmedia_producer_t* self)
{
    tdav_producer_alsa_t*  p_alsa = (tdav_producer_alsa_t*)self;

    if (!p_alsa) {
        ALSA_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    ALSA_DEBUG_INFO("paused");

    return 0;
}

static int tdav_producer_alsa_stop(tmedia_producer_t* self)
{
    tdav_producer_alsa_t*  p_alsa = (tdav_producer_alsa_t*)self;
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
//	ALSA producer object definition
//
/* constructor */
static tsk_object_t* tdav_producer_alsa_ctor(tsk_object_t * self, va_list * app)
{
    tdav_producer_alsa_t *p_alsa = (tdav_producer_alsa_t*)self;
    if (p_alsa) {
        ALSA_DEBUG_INFO("create");
        /* init base */
        tdav_producer_audio_init(TDAV_PRODUCER_AUDIO(p_alsa));
        /* init self */
        tdav_common_alsa_init(&p_alsa->alsa_common);
    }
    return self;
}
/* destructor */
static tsk_object_t* tdav_producer_alsa_dtor(tsk_object_t * self)
{
    tdav_producer_alsa_t *p_alsa = (tdav_producer_alsa_t *)self;
    if (p_alsa) {
        /* stop */
        if (p_alsa->b_started) {
            tdav_producer_alsa_stop((tmedia_producer_t*)p_alsa);
        }

        /* deinit base */
        tdav_producer_audio_deinit(TDAV_PRODUCER_AUDIO(p_alsa));
        /* deinit self */
        tdav_common_alsa_deinit(&p_alsa->alsa_common);

        ALSA_DEBUG_INFO("*** destroyed ***");
    }

    return self;
}
/* object definition */
static const tsk_object_def_t tdav_producer_alsa_def_s = {
    sizeof(tdav_producer_alsa_t),
    tdav_producer_alsa_ctor,
    tdav_producer_alsa_dtor,
    tdav_producer_audio_cmp,
};
/* plugin definition*/
static const tmedia_producer_plugin_def_t tdav_producer_alsa_plugin_def_s = {
    &tdav_producer_alsa_def_s,

    tmedia_audio,
    "Linux ALSA producer",

    tdav_producer_alsa_set,
    tdav_producer_alsa_prepare,
    tdav_producer_alsa_start,
    tdav_producer_alsa_pause,
    tdav_producer_alsa_stop
};
const tmedia_producer_plugin_def_t *tdav_producer_alsa_plugin_def_t = &tdav_producer_alsa_plugin_def_s;

#endif /* #if HAVE_ALSA_ASOUNDLIB_H */
