/*
* Copyright (C) 2010-2011 Mamadou Diop.
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

/**@file tdav_speex_denoise.c
* @brief Speex Denoiser (Noise suppression, AGC, AEC) Plugin
*/
#include "tinydav/audio/tdav_speex_denoise.h"

#if HAVE_SPEEX_DSP && (!defined(HAVE_SPEEX_DENOISE) || HAVE_SPEEX_DENOISE)

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#include "tinymedia/tmedia_defaults.h"

#include <string.h>

#include <speex/speex_preprocess.h>
#include <speex/speex_echo.h>

/** Speex denoiser*/
typedef struct tdav_speex_denoise_s {
    TMEDIA_DECLARE_DENOISE;

    SpeexPreprocessState *preprocess_state_record;
    SpeexPreprocessState *preprocess_state_playback;
    SpeexEchoState *echo_state;

    spx_int16_t* echo_output_frame;
    uint32_t record_frame_size_samples, record_frame_size_bytes;
    uint32_t playback_frame_size_samples, playback_frame_size_bytes;
}
tdav_speex_denoise_t;

static int tdav_speex_denoise_set(tmedia_denoise_t* _self, const tmedia_param_t* param)
{
    tdav_speex_denoise_t *self = (tdav_speex_denoise_t *)_self;
    if(!self || !param) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if(param->value_type == tmedia_pvt_int32) {
        if(tsk_striequals(param->key, "echo-tail")) {
            int32_t echo_tail = *((int32_t*)param->value);
            TSK_DEBUG_INFO("speex_set_echo_tail(%d) ignore", echo_tail); // because Speex AEC just do not work (use WebRTC)
            return 0;
        }
    }
    return -1;
}

static int tdav_speex_denoise_open(tmedia_denoise_t* self, uint32_t record_frame_size_samples, uint32_t record_sampling_rate, uint32_t record_channels, uint32_t playback_frame_size_samples, uint32_t playback_sampling_rate, uint32_t playback_channels)
{
    tdav_speex_denoise_t *denoiser = (tdav_speex_denoise_t *)self;
    float f;
    int i;

    if (!denoiser->echo_state && TMEDIA_DENOISE(denoiser)->echo_supp_enabled) {
        TSK_DEBUG_INFO("Init Aec frame_size[%u] filter_length[%u] SampleRate[%u]",
                       (uint32_t)(record_frame_size_samples),TMEDIA_DENOISE(denoiser)->echo_tail*record_frame_size_samples, record_sampling_rate);
        if((denoiser->echo_state = speex_echo_state_init(record_frame_size_samples, TMEDIA_DENOISE(denoiser)->echo_tail))) {
            speex_echo_ctl(denoiser->echo_state, SPEEX_ECHO_SET_SAMPLING_RATE, &record_sampling_rate);
        }
    }

    if (!denoiser->preprocess_state_record && !denoiser->preprocess_state_playback) {
        denoiser->record_frame_size_samples = record_frame_size_samples;
        denoiser->record_frame_size_bytes = (record_frame_size_samples << 1);
        denoiser->playback_frame_size_samples = playback_frame_size_samples;
        denoiser->playback_frame_size_bytes = (playback_frame_size_samples << 1);

        if((denoiser->preprocess_state_record = speex_preprocess_state_init(record_frame_size_samples, record_sampling_rate))
                && (denoiser->preprocess_state_playback = speex_preprocess_state_init(playback_frame_size_samples, playback_sampling_rate))
          ) {

            // Echo suppression
            if(denoiser->echo_state) {
                int echo_supp , echo_supp_active = 0;

                speex_preprocess_ctl(denoiser->preprocess_state_record, SPEEX_PREPROCESS_SET_ECHO_STATE, denoiser->echo_state);

                TSK_FREE(denoiser->echo_output_frame);
                denoiser->echo_output_frame = tsk_calloc(denoiser->record_frame_size_samples, sizeof(spx_int16_t));

                speex_preprocess_ctl(denoiser->preprocess_state_record, SPEEX_PREPROCESS_GET_ECHO_SUPPRESS , &echo_supp );
                speex_preprocess_ctl(denoiser->preprocess_state_record, SPEEX_PREPROCESS_GET_ECHO_SUPPRESS_ACTIVE , &echo_supp_active );
                TSK_DEBUG_INFO("AEC echo_supp level [%d] echo_supp_active level[%d] ", echo_supp , echo_supp_active);
                echo_supp = -60 ;
                echo_supp_active = -60 ;
                speex_preprocess_ctl(denoiser->preprocess_state_record, SPEEX_PREPROCESS_SET_ECHO_SUPPRESS , &echo_supp );
                speex_preprocess_ctl(denoiser->preprocess_state_record, SPEEX_PREPROCESS_SET_ECHO_SUPPRESS_ACTIVE , &echo_supp_active );
                // TRACES
                speex_preprocess_ctl(denoiser->preprocess_state_record, SPEEX_PREPROCESS_GET_ECHO_SUPPRESS , &echo_supp );
                speex_preprocess_ctl(denoiser->preprocess_state_record, SPEEX_PREPROCESS_GET_ECHO_SUPPRESS_ACTIVE , &echo_supp_active );
                TSK_DEBUG_INFO("New aec echo_supp level [%d] echo_supp_active level[%d] ", echo_supp , echo_supp_active);
            }

            // Noise suppression
            if(TMEDIA_DENOISE(denoiser)->noise_supp_enabled) {
                TSK_DEBUG_INFO("SpeexDSP: Noise supp enabled");
                i = 1;
                speex_preprocess_ctl(denoiser->preprocess_state_record, SPEEX_PREPROCESS_SET_DENOISE, &i);
                speex_preprocess_ctl(denoiser->preprocess_state_playback, SPEEX_PREPROCESS_SET_DENOISE, &i);
                i = TMEDIA_DENOISE(denoiser)->noise_supp_level;
                speex_preprocess_ctl(denoiser->preprocess_state_record, SPEEX_PREPROCESS_SET_NOISE_SUPPRESS, &i);
                speex_preprocess_ctl(denoiser->preprocess_state_playback, SPEEX_PREPROCESS_SET_NOISE_SUPPRESS, &i);
            }
            else {
                i = 0;
                speex_preprocess_ctl(denoiser->preprocess_state_record, SPEEX_PREPROCESS_SET_DENOISE, &i);
                speex_preprocess_ctl(denoiser->preprocess_state_playback, SPEEX_PREPROCESS_SET_DENOISE, &i);
            }

            // Automatic gain control
            if(TMEDIA_DENOISE(denoiser)->agc_enabled) {
                float agc_level = TMEDIA_DENOISE(denoiser)->agc_level;
                TSK_DEBUG_INFO("SpeexDSP: AGC enabled");

                i = 1;
                speex_preprocess_ctl(denoiser->preprocess_state_record, SPEEX_PREPROCESS_SET_AGC, &i);
                speex_preprocess_ctl(denoiser->preprocess_state_record, SPEEX_PREPROCESS_SET_AGC_LEVEL, &agc_level);
            }
            else {
                i = 0, f = 8000.0f;
                speex_preprocess_ctl(denoiser->preprocess_state_record, SPEEX_PREPROCESS_SET_AGC, &i);
                speex_preprocess_ctl(denoiser->preprocess_state_record, SPEEX_PREPROCESS_SET_AGC_LEVEL, &f);
            }

            // Voice Activity detection
            i = TMEDIA_DENOISE(denoiser)->vad_enabled ? 1 : 0;
            speex_preprocess_ctl(denoiser->preprocess_state_record, SPEEX_PREPROCESS_SET_VAD, &i);

            return 0;
        }
        else {
            TSK_DEBUG_ERROR("Failed to create Speex preprocessor state");
            return -2;
        }
    }

    return 0;
}

static int tdav_speex_denoise_echo_playback(tmedia_denoise_t* self, const void* echo_frame, uint32_t echo_frame_size_bytes)
{
    tdav_speex_denoise_t *denoiser = (tdav_speex_denoise_t *)self;

    if(denoiser->record_frame_size_bytes != echo_frame_size_bytes) {
        TSK_DEBUG_ERROR("Size mismatch: %u<>%u", denoiser->record_frame_size_bytes, echo_frame_size_bytes);
        return -1;
    }

    if(denoiser->echo_state) {
        speex_echo_playback(denoiser->echo_state, echo_frame);
    }
    return 0;
}



static int tdav_speex_denoise_process_record(tmedia_denoise_t* self, void* audio_frame, uint32_t audio_frame_size_bytes, tsk_bool_t* silence_or_noise)
{
    tdav_speex_denoise_t *denoiser = (tdav_speex_denoise_t *)self;
    int vad;

    if(denoiser->record_frame_size_bytes != audio_frame_size_bytes) {
        TSK_DEBUG_ERROR("Size mismatch: %u<>%u", denoiser->record_frame_size_bytes, audio_frame_size_bytes);
        return -1;
    }

    if(denoiser->preprocess_state_record) {
        if(denoiser->echo_state && denoiser->echo_output_frame) {
            speex_echo_capture(denoiser->echo_state, audio_frame, denoiser->echo_output_frame);
            memcpy(audio_frame, denoiser->echo_output_frame, denoiser->record_frame_size_bytes);
        }
        vad = speex_preprocess_run(denoiser->preprocess_state_record, audio_frame);
        if(!vad && TMEDIA_DENOISE(denoiser)->vad_enabled) {
            *silence_or_noise = tsk_true;
        }
    }

    return 0;
}

static int tdav_speex_denoise_process_playback(tmedia_denoise_t* self, void* audio_frame, uint32_t audio_frame_size_bytes)
{
    tdav_speex_denoise_t *denoiser = (tdav_speex_denoise_t *)self;

    if(denoiser->playback_frame_size_bytes != audio_frame_size_bytes) {
        TSK_DEBUG_ERROR("Size mismatch: %u<>%u", denoiser->playback_frame_size_bytes, audio_frame_size_bytes);
        return -1;
    }

    if(denoiser->preprocess_state_playback) {
        speex_preprocess_run(denoiser->preprocess_state_playback, audio_frame);
    }
    return 0;
}

static int tdav_speex_denoise_close(tmedia_denoise_t* self)
{
    tdav_speex_denoise_t *denoiser = (tdav_speex_denoise_t *)self;

    if(denoiser->preprocess_state_record) {
        speex_preprocess_state_destroy(denoiser->preprocess_state_record);
        denoiser->preprocess_state_record = tsk_null;
    }
    if(denoiser->preprocess_state_playback) {
        speex_preprocess_state_destroy(denoiser->preprocess_state_playback);
        denoiser->preprocess_state_playback = tsk_null;
    }
    if(denoiser->echo_state) {
        speex_echo_state_destroy(denoiser->echo_state);
        denoiser->echo_state = tsk_null;
    }
    TSK_FREE(denoiser->echo_output_frame);

    return 0;
}



//
//	Speex denoiser Plugin definition
//

/* constructor */
static tsk_object_t* tdav_speex_denoise_ctor(tsk_object_t * self, va_list * app)
{
    tdav_speex_denoise_t *denoise = self;
    if(denoise) {
        /* init base */
        tmedia_denoise_init(TMEDIA_DENOISE(denoise));
        /* init self */

        TSK_DEBUG_INFO("Create SpeexDSP denoiser");
    }
    return self;
}
/* destructor */
static tsk_object_t* tdav_speex_denoise_dtor(tsk_object_t * self)
{
    tdav_speex_denoise_t *denoise = self;
    if(denoise) {
        /* deinit base */
        tmedia_denoise_deinit(TMEDIA_DENOISE(denoise));
        /* deinit self */
        if(denoise->preprocess_state_record) {
            speex_preprocess_state_destroy(denoise->preprocess_state_record);
            denoise->preprocess_state_record = tsk_null;
        }
        if(denoise->preprocess_state_playback) {
            speex_preprocess_state_destroy(denoise->preprocess_state_playback);
            denoise->preprocess_state_playback = tsk_null;
        }
        if(denoise->echo_state) {
            speex_echo_state_destroy(denoise->echo_state);
            denoise->echo_state = tsk_null;
        }
        TSK_FREE(denoise->echo_output_frame);

        TSK_DEBUG_INFO("*** SpeexDSP denoiser destroyed ***");
    }

    return self;
}
/* object definition */
static const tsk_object_def_t tdav_speex_denoise_def_s = {
    sizeof(tdav_speex_denoise_t),
    tdav_speex_denoise_ctor,
    tdav_speex_denoise_dtor,
    tsk_null,
};
/* plugin definition*/
static const tmedia_denoise_plugin_def_t tdav_speex_denoise_plugin_def_s = {
    &tdav_speex_denoise_def_s,

    "Audio Denoiser based on SpeexDSP",

    tdav_speex_denoise_set,
    tdav_speex_denoise_open,
    tdav_speex_denoise_echo_playback,
    tdav_speex_denoise_process_record,
    tdav_speex_denoise_process_playback,
    tdav_speex_denoise_close,
};
const tmedia_denoise_plugin_def_t *tdav_speex_denoise_plugin_def_t = &tdav_speex_denoise_plugin_def_s;


#endif /* HAVE_SPEEX_DSP */