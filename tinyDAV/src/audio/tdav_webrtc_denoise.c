/*
* Copyright (C) 2011-2015 Mamadou DIOP
* Copyright (C) 2011-2015 Doubango Telecom <http://www.doubango.org>
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

/**@file tdav_webrtc_denoise.c
* @brief Google WebRTC Denoiser (Noise suppression, AGC, AEC) Plugin
*/
#include "tinydav/audio/tdav_webrtc_denoise.h"

#if HAVE_WEBRTC && (!defined(HAVE_WEBRTC_DENOISE) || HAVE_WEBRTC_DENOISE)

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#include "tinymedia/tmedia_defaults.h"
#include "tinymedia/tmedia_resampler.h"

#include <string.h>

#if !defined(WEBRTC_AEC_AGGRESSIVE)
#	define WEBRTC_AEC_AGGRESSIVE		0
#endif
#if !defined(WEBRTC_MAX_ECHO_TAIL)
#	define WEBRTC_MAX_ECHO_TAIL		500
#endif
#if !defined(WEBRTC_MIN_ECHO_TAIL)
#	define WEBRTC_MIN_ECHO_TAIL		20 // 0 will cause random crashes
#endif

#if TDAV_UNDER_MOBILE || 1 // FIXME
typedef int16_t sample_t;
#else
typedef float sample_t;
#endif

typedef struct tdav_webrtc_pin_xs {
    uint32_t n_duration;
    uint32_t n_rate;
    uint32_t n_channels;
    uint32_t n_sample_size;
}
tdav_webrtc_pin_xt;

typedef struct tdav_webrtc_resampler_s {
    TSK_DECLARE_OBJECT;

    tmedia_resampler_t* p_resampler;
    void* p_bufftmp_ptr; // used to convert float <->int16
    tsk_size_t n_bufftmp_size_in_bytes;

    struct {
        tdav_webrtc_pin_xt x_pin;
        tsk_size_t n_buff_size_in_bytes;
        tsk_size_t n_buff_size_in_samples;
    } in;
    struct {
        tdav_webrtc_pin_xt x_pin;
        void* p_buff_ptr;
        tsk_size_t n_buff_size_in_bytes;
        tsk_size_t n_buff_size_in_samples;
    } out;
}
tdav_webrtc_resampler_t;

static int _tdav_webrtc_resampler_create(const tdav_webrtc_pin_xt* p_pin_in, const tdav_webrtc_pin_xt* p_pin_out, tdav_webrtc_resampler_t **pp_resampler);
static int _tdav_webrtc_resampler_process(tdav_webrtc_resampler_t* p_self, const void* p_buff_ptr, tsk_size_t n_buff_size_in_bytes);

/** WebRTC denoiser (AEC, NS, AGC...) */
typedef struct tdav_webrtc_denoise_s {
    TMEDIA_DECLARE_DENOISE;

    void *AEC_inst;
#if HAVE_SPEEX_DSP && PREFER_SPEEX_DENOISER
    SpeexPreprocessState *SpeexDenoiser_proc;
#else
    TDAV_NsHandle *NS_inst;
#endif

    uint32_t echo_tail;
    uint32_t echo_skew;

    struct {
        tdav_webrtc_resampler_t* p_rpl_in2den; // input -> denoiser
        tdav_webrtc_resampler_t* p_rpl_den2in; // denoiser -> input
    } record;
    struct {
        tdav_webrtc_resampler_t* p_rpl_in2den; // input -> denoiser
        tdav_webrtc_resampler_t* p_rpl_den2in; // denoiser -> input
    } playback;

    struct {
        uint32_t nb_samples_per_process;
        uint32_t sampling_rate;
        uint32_t channels; // always "1"
    } neg;

    TSK_DECLARE_SAFEOBJ;
}
tdav_webrtc_denoise_t;

static int tdav_webrtc_denoise_set(tmedia_denoise_t* _self, const tmedia_param_t* param)
{
    tdav_webrtc_denoise_t *self = (tdav_webrtc_denoise_t *)_self;
    if (!self || !param) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if (param->value_type == tmedia_pvt_int32) {
        if (tsk_striequals(param->key, "echo-tail")) {
            int32_t echo_tail = *((int32_t*)param->value);
            self->echo_tail = TSK_CLAMP(WEBRTC_MIN_ECHO_TAIL, echo_tail, WEBRTC_MAX_ECHO_TAIL);
            TSK_DEBUG_INFO("set_echo_tail (%d->%d)", echo_tail, self->echo_tail);
            return 0;
        }
    }
    return -1;
}

static int tdav_webrtc_denoise_open(tmedia_denoise_t* self, uint32_t record_frame_size_samples, uint32_t record_sampling_rate, uint32_t record_channels, uint32_t playback_frame_size_samples, uint32_t playback_sampling_rate, uint32_t playback_channels)
{
    tdav_webrtc_denoise_t *denoiser = (tdav_webrtc_denoise_t *)self;
    int ret;
    tdav_webrtc_pin_xt pin_record_in = { 0 }, pin_record_den = { 0 }, pin_playback_in = { 0 }, pin_playback_den = { 0 };

    if (!denoiser) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if (denoiser->AEC_inst ||
#if HAVE_SPEEX_DSP && PREFER_SPEEX_DENOISER
            denoiser->SpeexDenoiser_proc
#else
            denoiser->NS_inst
#endif
       ) {
        TSK_DEBUG_ERROR("Denoiser already initialized");
        return -2;
    }

    denoiser->echo_tail = TSK_CLAMP(WEBRTC_MIN_ECHO_TAIL, TMEDIA_DENOISE(denoiser)->echo_tail, WEBRTC_MAX_ECHO_TAIL);
    denoiser->echo_skew = TMEDIA_DENOISE(denoiser)->echo_skew;
    TSK_DEBUG_INFO("echo_tail=%d, echo_skew=%d, echo_supp_enabled=%d, noise_supp_enabled=%d", denoiser->echo_tail, denoiser->echo_skew, self->echo_supp_enabled, self->noise_supp_enabled);

    //
    //	DENOISER
    //
#if TDAV_UNDER_MOBILE // AECM= [8-16]k, AEC=[8-32]k
    denoiser->neg.sampling_rate = TSK_MIN(TSK_MAX(record_sampling_rate, playback_sampling_rate), 16000);
#else
    denoiser->neg.sampling_rate = TSK_MIN(TSK_MAX(record_sampling_rate, playback_sampling_rate), 16000); // FIXME: 32000 accepted by echo_process fails
#endif
    denoiser->neg.nb_samples_per_process = /*TSK_CLAMP(80,*/ ((denoiser->neg.sampling_rate * 10) / 1000)/*, 160)*/; // Supported by the module: "80"(10ms) and "160"(20ms)
    denoiser->neg.channels = 1;

    //
    //	RECORD
    //
    TSK_OBJECT_SAFE_FREE(denoiser->record.p_rpl_den2in);
    TSK_OBJECT_SAFE_FREE(denoiser->record.p_rpl_in2den);
    pin_record_in.n_sample_size = sizeof(int16_t);
    pin_record_in.n_rate = record_sampling_rate;
    pin_record_in.n_channels = record_channels;
    pin_record_in.n_duration = (((record_frame_size_samples * 1000) / record_sampling_rate)) / record_channels;
    pin_record_den.n_sample_size = sizeof(sample_t);
    pin_record_den.n_rate = denoiser->neg.sampling_rate;

    pin_record_den.n_channels = 1;
    pin_record_den.n_duration = pin_record_in.n_duration;
    if (pin_record_in.n_sample_size != pin_record_den.n_sample_size || pin_record_in.n_rate != pin_record_den.n_rate || pin_record_in.n_channels != pin_record_den.n_channels) {
        if ((ret = _tdav_webrtc_resampler_create(&pin_record_in, &pin_record_den, &denoiser->record.p_rpl_in2den))) {
            return ret;
        }
        if ((ret = _tdav_webrtc_resampler_create(&pin_record_den, &pin_record_in, &denoiser->record.p_rpl_den2in))) {
            return ret;
        }
    }
    //
    //	PLAYBACK
    //
    TSK_OBJECT_SAFE_FREE(denoiser->playback.p_rpl_den2in);
    TSK_OBJECT_SAFE_FREE(denoiser->playback.p_rpl_in2den);
    pin_playback_in.n_sample_size = sizeof(int16_t);
    pin_playback_in.n_rate = playback_sampling_rate;
    pin_playback_in.n_channels = playback_channels;
    pin_playback_in.n_duration = (((playback_frame_size_samples * 1000) / playback_sampling_rate)) / playback_channels;
    pin_playback_den.n_sample_size = sizeof(sample_t);
    pin_playback_den.n_rate = denoiser->neg.sampling_rate;
    pin_playback_den.n_channels = 1;
    pin_playback_den.n_duration = pin_playback_in.n_duration;
    if (pin_playback_in.n_sample_size != pin_playback_den.n_sample_size || pin_playback_in.n_rate != pin_playback_den.n_rate || pin_playback_in.n_channels != pin_playback_den.n_channels) {
        if ((ret = _tdav_webrtc_resampler_create(&pin_playback_in, &pin_playback_den, &denoiser->playback.p_rpl_in2den))) {
            return ret;
        }
        if ((ret = _tdav_webrtc_resampler_create(&pin_playback_den, &pin_playback_in, &denoiser->playback.p_rpl_den2in))) {
            return ret;
        }
    }

    //
    //	AEC instance
    //
    if ((ret = TDAV_WebRtcAec_Create(&denoiser->AEC_inst))) {
        TSK_DEBUG_ERROR("WebRtcAec_Create failed with error code = %d", ret);
        return ret;
    }
    if ((ret = TDAV_WebRtcAec_Init(denoiser->AEC_inst, denoiser->neg.sampling_rate, denoiser->neg.sampling_rate))) {
        TSK_DEBUG_ERROR("WebRtcAec_Init failed with error code = %d", ret);
        return ret;
    }

#if TDAV_UNDER_MOBILE
#else
    {
        AecConfig aecConfig;
#if WEBRTC_AEC_AGGRESSIVE
        aecConfig.nlpMode = kAecNlpAggressive;
#else
        aecConfig.nlpMode = kAecNlpModerate;
#endif
        aecConfig.skewMode = kAecFalse;
        aecConfig.metricsMode = kAecTrue;
        aecConfig.delay_logging = kAecFalse;
        if ((ret = WebRtcAec_set_config(denoiser->AEC_inst, aecConfig))) {
            TSK_DEBUG_ERROR("WebRtcAec_set_config failed with error code = %d", ret);
        }
    }
#endif


    //
    //	Noise Suppression instance
    //
    if (TMEDIA_DENOISE(denoiser)->noise_supp_enabled) {
#if HAVE_SPEEX_DSP && PREFER_SPEEX_DENOISER
        if ((denoiser->SpeexDenoiser_proc = speex_preprocess_state_init((pin_record_den.n_rate / 1000) * pin_record_den.n_duration, pin_record_den.n_rate))) {
            int i = 1;
            speex_preprocess_ctl(denoiser->SpeexDenoiser_proc, SPEEX_PREPROCESS_SET_DENOISE, &i);
            i = TMEDIA_DENOISE(denoiser)->noise_supp_level;
            speex_preprocess_ctl(denoiser->SpeexDenoiser_proc, SPEEX_PREPROCESS_SET_NOISE_SUPPRESS, &i);
        }
#else
        if ((ret = TDAV_WebRtcNs_Create(&denoiser->NS_inst))) {
            TSK_DEBUG_ERROR("WebRtcNs_Create failed with error code = %d", ret);
            return ret;
        }
        if ((ret = TDAV_WebRtcNs_Init(denoiser->NS_inst, 80))) {
            TSK_DEBUG_ERROR("WebRtcNs_Init failed with error code = %d", ret);
            return ret;
        }
#endif
    }

    TSK_DEBUG_INFO("WebRTC denoiser opened: record:%uHz,%uchannels // playback:%uHz,%uchannels // neg:%uHz,%uchannels",
                   record_sampling_rate, record_channels,
                   playback_sampling_rate, playback_channels,
                   denoiser->neg.sampling_rate, denoiser->neg.channels);

    return ret;
}

static int tdav_webrtc_denoise_echo_playback(tmedia_denoise_t* self, const void* echo_frame, uint32_t echo_frame_size_bytes)
{
    tdav_webrtc_denoise_t *p_self = (tdav_webrtc_denoise_t *)self;
    int ret = 0;

    tsk_safeobj_lock(p_self);
    if (p_self->AEC_inst && echo_frame && echo_frame_size_bytes) {
        const sample_t* _echo_frame = (const sample_t*)echo_frame;
        tsk_size_t _echo_frame_size_bytes = echo_frame_size_bytes;
        tsk_size_t _echo_frame_size_samples = (_echo_frame_size_bytes / sizeof(int16_t));
        // IN -> DEN
        if (p_self->playback.p_rpl_in2den) {
            if ((ret = _tdav_webrtc_resampler_process(p_self->playback.p_rpl_in2den, _echo_frame, _echo_frame_size_bytes))) {
                goto bail;
            }
            _echo_frame = p_self->playback.p_rpl_in2den->out.p_buff_ptr;
            _echo_frame_size_bytes = p_self->playback.p_rpl_in2den->out.n_buff_size_in_bytes;
            _echo_frame_size_samples = p_self->playback.p_rpl_in2den->out.n_buff_size_in_samples;
        }
        // PROCESS
        if (_echo_frame_size_samples && _echo_frame) {
            uint32_t _samples;
            for (_samples = 0; _samples < _echo_frame_size_samples; _samples += p_self->neg.nb_samples_per_process) {
                if ((ret = TDAV_WebRtcAec_BufferFarend(p_self->AEC_inst, &_echo_frame[_samples], p_self->neg.nb_samples_per_process))) {
                    TSK_DEBUG_ERROR("WebRtcAec_BufferFarend failed with error code = %d, nb_samples_per_process=%u", ret, p_self->neg.nb_samples_per_process);
                    goto bail;
                }
            }
        }
    }
bail:
    tsk_safeobj_unlock(p_self);
    return ret;
}

static int tdav_webrtc_denoise_process_record(tmedia_denoise_t* self, void* audio_frame, uint32_t audio_frame_size_bytes, tsk_bool_t* silence_or_noise)
{
    tdav_webrtc_denoise_t *p_self = (tdav_webrtc_denoise_t *)self;
    int ret = 0;

    *silence_or_noise = tsk_false;

    tsk_safeobj_lock(p_self);

    if (p_self->AEC_inst && audio_frame && audio_frame_size_bytes) {
        tsk_size_t _samples;
        const sample_t* _audio_frame = (const sample_t*)audio_frame;
        tsk_size_t _audio_frame_size_bytes = audio_frame_size_bytes;
        tsk_size_t _audio_frame_size_samples = (_audio_frame_size_bytes / sizeof(int16_t));
        // IN -> DEN
        if (p_self->record.p_rpl_in2den) {
            if ((ret = _tdav_webrtc_resampler_process(p_self->record.p_rpl_in2den, _audio_frame, _audio_frame_size_bytes))) {
                goto bail;
            }
            _audio_frame = p_self->record.p_rpl_in2den->out.p_buff_ptr;
            _audio_frame_size_bytes = p_self->record.p_rpl_in2den->out.n_buff_size_in_bytes;
            _audio_frame_size_samples = p_self->record.p_rpl_in2den->out.n_buff_size_in_samples;
        }
        // NOISE SUPPRESSION
#if HAVE_SPEEX_DSP && PREFER_SPEEX_DENOISER
        if (p_self->SpeexDenoiser_proc) {
            speex_preprocess_run(p_self->SpeexDenoiser_proc, (spx_int16_t*)_audio_frame);
        }
#else
        // WebRTC NoiseSupp only accept 10ms frames
        // Our encoder will always output 20ms frames ==> execute 2x noise_supp
        if (p_self->NS_inst) {
            for (_samples = 0; _samples < _audio_frame_size_samples; _samples+= p_self->neg.nb_samples_per_process) {
                if ((ret = TDAV_WebRtcNs_Process(p_self->NS_inst, &_audio_frame[_samples], tsk_null, _audio_frame, tsk_null))) {
                    TSK_DEBUG_ERROR("WebRtcNs_Process with error code = %d", ret);
                    goto bail;
                }
            }
        }
#endif
        // PROCESS
        if (_audio_frame_size_samples && _audio_frame) {
            for (_samples = 0; _samples < _audio_frame_size_samples; _samples += p_self->neg.nb_samples_per_process) {
                if ((ret = TDAV_WebRtcAec_Process(p_self->AEC_inst, &_audio_frame[_samples], tsk_null, (sample_t*)&_audio_frame[_samples], tsk_null, p_self->neg.nb_samples_per_process, p_self->echo_tail, p_self->echo_skew))) {
                    TSK_DEBUG_ERROR("WebRtcAec_Process with error code = %d, nb_samples_per_process=%u", ret, p_self->neg.nb_samples_per_process);
                    goto bail;
                }
            }
        }
        // DEN -> IN
        if (p_self->record.p_rpl_den2in) {
            if ((ret = _tdav_webrtc_resampler_process(p_self->record.p_rpl_den2in, _audio_frame, _audio_frame_size_bytes))) {
                goto bail;
            }
            _audio_frame = p_self->record.p_rpl_den2in->out.p_buff_ptr;
            _audio_frame_size_bytes = p_self->record.p_rpl_den2in->out.n_buff_size_in_bytes;
            _audio_frame_size_samples = p_self->record.p_rpl_den2in->out.n_buff_size_in_samples;
        }
        // Sanity check
        if (_audio_frame_size_bytes != audio_frame_size_bytes) {
            TSK_DEBUG_ERROR("Size mismatch: %u <> %u", _audio_frame_size_bytes, audio_frame_size_bytes);
            ret = -3;
            goto bail;
        }
        if (audio_frame != (const void*)_audio_frame) {
            memcpy(audio_frame, _audio_frame, _audio_frame_size_bytes);
        }
    }

bail:
    tsk_safeobj_unlock(p_self);
    return ret;
}

static int tdav_webrtc_denoise_process_playback(tmedia_denoise_t* self, void* audio_frame, uint32_t audio_frame_size_bytes)
{
    tdav_webrtc_denoise_t *denoiser = (tdav_webrtc_denoise_t *)self;

    (void)(denoiser);

    // Not mandatory to denoise audio before playback.
    // All Doubango clients support noise suppression.
    return 0;
}

static int tdav_webrtc_denoise_close(tmedia_denoise_t* self)
{
    tdav_webrtc_denoise_t *denoiser = (tdav_webrtc_denoise_t *)self;

    tsk_safeobj_lock(denoiser);
    if (denoiser->AEC_inst) {
        TDAV_WebRtcAec_Free(denoiser->AEC_inst);
        denoiser->AEC_inst = tsk_null;
    }
#if HAVE_SPEEX_DSP && PREFER_SPEEX_DENOISER
    if (denoiser->SpeexDenoiser_proc) {
        speex_preprocess_state_destroy(denoiser->SpeexDenoiser_proc);
        denoiser->SpeexDenoiser_proc = tsk_null;
    }
#else
    if (denoiser->NS_inst) {
        TDAV_WebRtcNs_Free(denoiser->NS_inst);
        denoiser->NS_inst = tsk_null;
    }
#endif
    tsk_safeobj_unlock(denoiser);

    return 0;
}

static int _tdav_webrtc_resampler_create(const tdav_webrtc_pin_xt* p_pin_in, const tdav_webrtc_pin_xt* p_pin_out, tdav_webrtc_resampler_t **pp_resampler)
{
    extern const tsk_object_def_t *tdav_webrtc_resampler_def_t;
    int ret = 0;
    if (!p_pin_in || !p_pin_out || !pp_resampler || *pp_resampler) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if (!(*pp_resampler = tsk_object_new(tdav_webrtc_resampler_def_t))) {
        TSK_DEBUG_ERROR("Failed to create resampler object");
        ret = -3;
        goto bail;
    }
    if (!((*pp_resampler)->p_resampler = tmedia_resampler_create())) {
        ret = -3;
        goto bail;
    }
    ret = tmedia_resampler_open((*pp_resampler)->p_resampler,
                                p_pin_in->n_rate, p_pin_out->n_rate,
                                p_pin_in->n_duration,
                                p_pin_in->n_channels, p_pin_out->n_channels,
                                TMEDIA_RESAMPLER_QUALITY,
                                (p_pin_out->n_sample_size << 3));
    if (ret) {
        TSK_DEBUG_ERROR("Failed to open resampler: in_rate=%u,in_duration=%u,in_channels=%u /// out_rate=%u,out_duration=%u,out_channels=%u",
                        p_pin_in->n_rate, p_pin_in->n_duration, p_pin_in->n_channels,
                        p_pin_out->n_rate, p_pin_out->n_duration, p_pin_out->n_channels);
        goto bail;
    }

    (*pp_resampler)->out.n_buff_size_in_bytes = ((((p_pin_out->n_rate * p_pin_out->n_duration) / 1000)) * p_pin_out->n_channels) * p_pin_out->n_sample_size;
    (*pp_resampler)->out.p_buff_ptr = tsk_malloc((*pp_resampler)->out.n_buff_size_in_bytes);
    if (!(*pp_resampler)->out.p_buff_ptr) {
        TSK_DEBUG_ERROR("Failed to allocate buffer with size=%u", (*pp_resampler)->out.n_buff_size_in_bytes);
        ret = -3;
        goto bail;
    }
    (*pp_resampler)->out.n_buff_size_in_samples = (*pp_resampler)->out.n_buff_size_in_bytes / p_pin_out->n_sample_size;
    (*pp_resampler)->in.n_buff_size_in_bytes = ((((p_pin_in->n_rate * p_pin_in->n_duration) / 1000)) * p_pin_in->n_channels) * p_pin_in->n_sample_size;
    (*pp_resampler)->in.n_buff_size_in_samples = (*pp_resampler)->in.n_buff_size_in_bytes / p_pin_in->n_sample_size;

    (*pp_resampler)->n_bufftmp_size_in_bytes = (((48000 * TSK_MAX(p_pin_in->n_duration, p_pin_out->n_duration)) / 1000) * 2/*channels*/) * sizeof(float); // Max
    (*pp_resampler)->p_bufftmp_ptr = tsk_malloc((*pp_resampler)->n_bufftmp_size_in_bytes);
    if (!(*pp_resampler)->p_bufftmp_ptr) {
        TSK_DEBUG_ERROR("Failed to allocate buffer with size:%u", (*pp_resampler)->n_bufftmp_size_in_bytes);
        ret = -3;
        goto bail;
    }

    memcpy(&(*pp_resampler)->in.x_pin, p_pin_in, sizeof(tdav_webrtc_pin_xt));
    memcpy(&(*pp_resampler)->out.x_pin, p_pin_out, sizeof(tdav_webrtc_pin_xt));
bail:
    if (ret) {
        TSK_OBJECT_SAFE_FREE((*pp_resampler));
    }
    return ret;
}

static int _tdav_webrtc_resampler_process(tdav_webrtc_resampler_t *p_self, const void* p_buff_ptr, tsk_size_t n_buff_size_in_bytes)
{
    tsk_size_t n_out_size;
    const void* _p_buff_ptr = p_buff_ptr;
    tsk_size_t _n_buff_size_in_bytes = n_buff_size_in_bytes;
    tsk_size_t _n_buff_size_in_samples;

    if (!p_self || !p_buff_ptr || !n_buff_size_in_bytes) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if (p_self->in.n_buff_size_in_bytes != n_buff_size_in_bytes) {
        TSK_DEBUG_ERROR("Invalid input size: %u <> %u", p_self->in.n_buff_size_in_bytes, n_buff_size_in_bytes);
        return -2;
    }
    _n_buff_size_in_samples = p_self->in.n_buff_size_in_samples;
    if (p_self->in.x_pin.n_sample_size != p_self->out.x_pin.n_sample_size) {
        tsk_size_t index;
        if (p_self->in.x_pin.n_sample_size == sizeof(int16_t)) {
            // int16_t -> float
            const int16_t* p_src = (const int16_t*)p_buff_ptr;
            float* p_dst = (float*)p_self->p_bufftmp_ptr;
            for (index = 0; index < _n_buff_size_in_samples; ++index) {
                p_dst[index] = (float)p_src[index];
            }
        }
        else {
            // float -> int16_t
            const float* p_src = (const float*)p_buff_ptr;
            int16_t* p_dst = (int16_t*)p_self->p_bufftmp_ptr;
            for (index = 0; index < _n_buff_size_in_samples; ++index) {
                p_dst[index] = (int16_t)p_src[index];
            }
        }
        _p_buff_ptr = p_self->p_bufftmp_ptr;
        _n_buff_size_in_bytes = p_self->in.n_buff_size_in_bytes;
    }
    n_out_size = tmedia_resampler_process(p_self->p_resampler, _p_buff_ptr, _n_buff_size_in_samples, (int16_t*)p_self->out.p_buff_ptr, p_self->out.n_buff_size_in_samples);
    if (n_out_size != p_self->out.n_buff_size_in_samples) {
        TSK_DEBUG_ERROR("Invalid output size: %u <> %u", n_out_size, p_self->out.n_buff_size_in_bytes);
        return -4;
    }
    return 0;
}

//
//	WEBRTC resampler object definition
//
static tsk_object_t* tdav_webrtc_resampler_ctor(tsk_object_t * self, va_list * app)
{
    tdav_webrtc_resampler_t *p_resampler = (tdav_webrtc_resampler_t*)self;
    if (p_resampler) {

    }
    return self;
}
static tsk_object_t* tdav_webrtc_resampler_dtor(tsk_object_t * self)
{
    tdav_webrtc_resampler_t *p_resampler = (tdav_webrtc_resampler_t*)self;
    if (p_resampler) {
        TSK_OBJECT_SAFE_FREE(p_resampler->p_resampler);
        TSK_FREE(p_resampler->out.p_buff_ptr);
        TSK_FREE(p_resampler->p_bufftmp_ptr);
    }
    return self;
}
static const tsk_object_def_t tdav_webrtc_resampler_def_s = {
    sizeof(tdav_webrtc_resampler_t),
    tdav_webrtc_resampler_ctor,
    tdav_webrtc_resampler_dtor,
    tsk_object_cmp,
};
const tsk_object_def_t *tdav_webrtc_resampler_def_t = &tdav_webrtc_resampler_def_s;


//
//	WEBRTC denoiser Plugin definition
//

/* constructor */
static tsk_object_t* tdav_webrtc_denoise_ctor(tsk_object_t * _self, va_list * app)
{
    tdav_webrtc_denoise_t *self = _self;
    if (self) {
        /* init base */
        tmedia_denoise_init(TMEDIA_DENOISE(self));
        /* init self */
        tsk_safeobj_init(self);
        self->neg.channels = 1;

        TSK_DEBUG_INFO("Create WebRTC denoiser");
    }
    return self;
}
/* destructor */
static tsk_object_t* tdav_webrtc_denoise_dtor(tsk_object_t * _self)
{
    tdav_webrtc_denoise_t *self = _self;
    if (self) {
        /* deinit base (will close the denoise if not done yet) */
        tmedia_denoise_deinit(TMEDIA_DENOISE(self));
        /* deinit self */
        tdav_webrtc_denoise_close(TMEDIA_DENOISE(self));
        TSK_OBJECT_SAFE_FREE(self->record.p_rpl_in2den);
        TSK_OBJECT_SAFE_FREE(self->record.p_rpl_den2in);
        TSK_OBJECT_SAFE_FREE(self->playback.p_rpl_in2den);
        TSK_OBJECT_SAFE_FREE(self->playback.p_rpl_den2in);
        tsk_safeobj_deinit(self);

        TSK_DEBUG_INFO("*** Destroy WebRTC denoiser ***");
    }

    return self;
}
/* object definition */
static const tsk_object_def_t tdav_webrtc_denoise_def_s = {
    sizeof(tdav_webrtc_denoise_t),
    tdav_webrtc_denoise_ctor,
    tdav_webrtc_denoise_dtor,
    tsk_null,
};
/* plugin definition*/
static const tmedia_denoise_plugin_def_t tdav_webrtc_denoise_plugin_def_s = {
    &tdav_webrtc_denoise_def_s,

    "Audio Denoiser based on Google WebRTC",

    tdav_webrtc_denoise_set,
    tdav_webrtc_denoise_open,
    tdav_webrtc_denoise_echo_playback,
    tdav_webrtc_denoise_process_record,
    tdav_webrtc_denoise_process_playback,
    tdav_webrtc_denoise_close,
};
const tmedia_denoise_plugin_def_t *tdav_webrtc_denoise_plugin_def_t = &tdav_webrtc_denoise_plugin_def_s;


#endif /* HAVE_WEBRTC */