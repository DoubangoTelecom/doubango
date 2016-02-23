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
#include "tinydav/audio/alsa/tdav_common_alsa.h"

#if HAVE_ALSA_ASOUNDLIB_H

#define ALSA_DEBUG_INFO(FMT, ...) TSK_DEBUG_INFO("[ALSA Common] " FMT, ##__VA_ARGS__)
#define ALSA_DEBUG_WARN(FMT, ...) TSK_DEBUG_WARN("[ALSA Common] " FMT, ##__VA_ARGS__)
#define ALSA_DEBUG_ERROR(FMT, ...) TSK_DEBUG_ERROR("[ALSA Common] " FMT, ##__VA_ARGS__)
#define ALSA_DEBUG_FATAL(FMT, ...) TSK_DEBUG_FATAL("[ALSA Common] " FMT, ##__VA_ARGS__)

#define ALSA_PLAYBACK_PERIODS 6

int tdav_common_alsa_init(tdav_common_alsa_t* p_self)
{
    if (!p_self) {
        ALSA_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if (p_self->b_initialized) {
        ALSA_DEBUG_WARN("Already initialized");
        return 0;
    }
    tsk_safeobj_init(p_self);
    p_self->b_initialized = tsk_true;
    return 0;
}

int tdav_common_alsa_lock(tdav_common_alsa_t* p_self)
{
    if (!p_self) {
        ALSA_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    return tsk_safeobj_lock(p_self);
}

int tdav_common_alsa_unlock(tdav_common_alsa_t* p_self)
{
    if (!p_self) {
        ALSA_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    return tsk_safeobj_unlock(p_self);
}

int tdav_common_alsa_prepare(tdav_common_alsa_t* p_self, tsk_bool_t is_capture, int ptime, int channels, int sample_rate)
{
    int err = 0, val;
    if (!p_self) {
        ALSA_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tdav_common_alsa_lock(p_self);

    if (p_self->b_prepared) {
        ALSA_DEBUG_WARN("Already prepared");
        goto bail;
    }
    if (!p_self->p_device_name) {
        p_self->p_device_name = strdup("default");
    }
    p_self->b_capture = is_capture;

    if ((err = snd_pcm_open(&p_self->p_handle, p_self->p_device_name, is_capture ? SND_PCM_STREAM_CAPTURE : SND_PCM_STREAM_PLAYBACK, /*SND_PCM_NONBLOCK | SND_PCM_ASYNC*/0)) != 0) {
        ALSA_DEBUG_ERROR("Failed to open audio device %s (%s)", p_self->p_device_name, snd_strerror(err));
        goto bail;
    }
    ALSA_DEBUG_INFO("device('%s') opened", p_self->p_device_name);

    if ((err = snd_pcm_hw_params_malloc(&p_self->p_params)) != 0) {
        ALSA_DEBUG_ERROR("Failed to allocate hardware parameter structure(%s)", snd_strerror(err));
        goto bail;
    }

    if ((err = snd_pcm_hw_params_any(p_self->p_handle, p_self->p_params)) < 0) {
        ALSA_DEBUG_ERROR("Failed to initialize hardware parameter structure (device=%s, err=%s)", p_self->p_device_name, snd_strerror(err));
        goto bail;
    }

    if ((err = snd_pcm_hw_params_set_access(p_self->p_handle, p_self->p_params, SND_PCM_ACCESS_RW_INTERLEAVED)) != 0) {
        ALSA_DEBUG_ERROR("Failed to set access type (device=%s, err=%s)", p_self->p_device_name, snd_strerror(err));
        goto bail;
    }

    if ((err = snd_pcm_hw_params_set_format(p_self->p_handle, p_self->p_params, SND_PCM_FORMAT_S16_LE)) != 0) {
        ALSA_DEBUG_ERROR("Failed to set sample format (device=%s, err=%s)", p_self->p_device_name, snd_strerror(err));
        goto bail;
    }

    val = sample_rate;
    if ((err = snd_pcm_hw_params_set_rate_near(p_self->p_handle, p_self->p_params, &val, 0)) != 0) {
        ALSA_DEBUG_ERROR("Failed to set sample rate  (rate=%d, device=%s, err=%s)", p_self->sample_rate, p_self->p_device_name, snd_strerror(err));
        goto bail;
    }
    ALSA_DEBUG_INFO("sample_rate: req=%d, resp=%d", sample_rate, val);
    p_self->sample_rate = val;

    val = channels;
    if ((err = snd_pcm_hw_params_set_channels_near(p_self->p_handle, p_self->p_params, &val)) != 0) {
        ALSA_DEBUG_ERROR("Failed to set channels (channels=%d, device=%s, err=%s)", p_self->channels, p_self->p_device_name, snd_strerror(err));
        goto bail;
    }
    ALSA_DEBUG_INFO("channels: req=%d, resp=%d", channels, val);
    p_self->channels = val;

    if (!is_capture) {
        unsigned int periods = ALSA_PLAYBACK_PERIODS;
        snd_pcm_uframes_t periodSize = (ptime * p_self->sample_rate * p_self->channels) / 1000;
        if ((err = snd_pcm_hw_params_set_periods_near(p_self->p_handle, p_self->p_params, &periods, 0)) != 0) {
            ALSA_DEBUG_ERROR ("Failed to set periods (val=%u, device=%s, err=%s)", periods, p_self->p_device_name, snd_strerror(err));
            goto bail;
        }

        snd_pcm_uframes_t bufferSize = (periodSize * periods);
        if ((err = snd_pcm_hw_params_set_buffer_size(p_self->p_handle, p_self->p_params, bufferSize)) != 0) {
            ALSA_DEBUG_ERROR ("Failed to set buffer size (val=%lu, device=%s, err=%s)", bufferSize, p_self->p_device_name, snd_strerror(err));
            goto bail;
        }
        ALSA_DEBUG_INFO("periods=%u, buffersize=%lu", periods, bufferSize);
    }

    if ((err = snd_pcm_hw_params (p_self->p_handle, p_self->p_params)) != 0) {
        ALSA_DEBUG_ERROR ("Failed to set parameters (channels=%d, rate=%d, device=%s, err=%s)", p_self->channels, p_self->sample_rate, p_self->p_device_name, snd_strerror(err));
        goto bail;
    }
    if ((err = snd_pcm_prepare(p_self->p_handle)) != 0) {
        ALSA_DEBUG_ERROR ("Failed to prepare device (channels=%d, rate=%d, device=%s, err=%s)", p_self->channels, p_self->sample_rate, p_self->p_device_name, snd_strerror(err));
        goto bail;
    }

    /*if (is_capture)*/ {
        p_self->n_buff_size_in_bytes = (ptime * p_self->sample_rate * (2/*SND_PCM_FORMAT_S16_LE*/ * p_self->channels)) / 1000;
        if (!(p_self->p_buff_ptr = tsk_realloc(p_self->p_buff_ptr, p_self->n_buff_size_in_bytes))) {
            ALSA_DEBUG_ERROR("Failed to allocate buffer with size = %u", p_self->n_buff_size_in_bytes);
            err = -4;
            goto bail;
        }
        p_self->n_buff_size_in_samples = (p_self->n_buff_size_in_bytes >> 1/*SND_PCM_FORMAT_S16_LE*/);
        ALSA_DEBUG_INFO("n_buff_size_in_bytes=%u", p_self->n_buff_size_in_bytes);
    }

    ALSA_DEBUG_INFO("device('%s') prepared", p_self->p_device_name);

    // everything is OK
    p_self->b_prepared = tsk_true;
bail:
    if (err) {
        tdav_common_alsa_unprepare(p_self);
    }
    tdav_common_alsa_unlock(p_self);
    return err;

}

int tdav_common_alsa_unprepare(tdav_common_alsa_t* p_self)
{
    int err = 0;
    if (!p_self) {
        ALSA_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tdav_common_alsa_lock(p_self);

    if (p_self->b_started) {
        ALSA_DEBUG_ERROR("Must stop the capture device before unpreparing");
        err = -2;
        goto bail;
    }

    if (p_self->p_params) {
        snd_pcm_hw_params_free(p_self->p_params);
        p_self->p_params = tsk_null;
    }
    if (p_self->p_handle) {
        snd_pcm_close(p_self->p_handle);
        p_self->p_handle = tsk_null;
    }
    p_self->b_prepared = tsk_false;

    ALSA_DEBUG_INFO("device('%s') unprepared", p_self->p_device_name);

bail:
    tdav_common_alsa_unlock(p_self);
    return err;
}

int tdav_common_alsa_start(tdav_common_alsa_t* p_self)
{
    int err = 0;
    if (!p_self) {
        ALSA_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tdav_common_alsa_lock(p_self);

    if (p_self->b_started) {
        ALSA_DEBUG_WARN("Already started");
        err = - 3;
        goto bail;
    }
    if (!p_self->b_prepared) {
        ALSA_DEBUG_ERROR("Not prepared");
        err = -2;
        goto bail;
    }

    if ((err = snd_pcm_start(p_self->p_handle)) != 0) {
        ALSA_DEBUG_ERROR ("Failed to start device (channels=%d, rate=%d, device=%s, err=%s)", p_self->channels, p_self->sample_rate, p_self->p_device_name, snd_strerror(err));
        goto bail;
    }

    p_self->b_started = tsk_true;
    ALSA_DEBUG_INFO("device('%s') started", p_self->p_device_name);
bail:
    tdav_common_alsa_unlock(p_self);
    return err;
}

int tdav_common_alsa_stop(tdav_common_alsa_t* p_self)
{
    int err = 0;
    if (!p_self) {
        ALSA_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tdav_common_alsa_lock(p_self);

    if (p_self->b_started) {
        p_self->b_started = tsk_false;
        //err = snd_pcm_drain(p_self->p_handle);
        ALSA_DEBUG_INFO("device('%s') stopped", p_self->p_device_name);
    }
    if (p_self->b_prepared) {
        tdav_common_alsa_unprepare(p_self);
    }
bail:
    tdav_common_alsa_unlock(p_self);
    return err;
}

int tdav_common_alsa_deinit(tdav_common_alsa_t* p_self)
{
    if (p_self && p_self->b_initialized) {
        tdav_common_alsa_stop(p_self);
        tdav_common_alsa_unprepare(p_self);
        TSK_FREE(p_self->p_device_name);
        TSK_FREE(p_self->p_buff_ptr);
        tsk_safeobj_deinit(p_self);
        p_self->b_initialized = tsk_false;
    }
    return 0;
}

#endif /* HAVE_ALSA_ASOUNDLIB_H */

