/*
* Copyright (C) 2011-2015 Mamadou DIOP.
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
#include "tinydav/audio/tdav_speex_resampler.h"

#if HAVE_SPEEX_DSP && (!defined(HAVE_SPEEX_RESAMPLER) || HAVE_SPEEX_RESAMPLER)

#include <speex/speex_resampler.h>

#include "tsk_memory.h"
#include "tsk_debug.h"

#define TDAV_SPEEX_RESAMPLER_MAX_QUALITY 10

/** Speex resampler*/
typedef struct tdav_speex_resampler_s {
    TMEDIA_DECLARE_RESAMPLER;

    tsk_size_t in_size;
    tsk_size_t out_size;
    uint32_t in_channels;
    uint32_t out_channels;
    uint32_t bytes_per_sample;

    struct {
        void* ptr;
        tsk_size_t size_in_samples;
    } tmp_buffer;

    SpeexResamplerState *state;
}
tdav_speex_resampler_t;

static int tdav_speex_resampler_open(tmedia_resampler_t* self, uint32_t in_freq, uint32_t out_freq, uint32_t frame_duration, uint32_t in_channels, uint32_t out_channels, uint32_t quality, uint32_t bits_per_sample)
{
    tdav_speex_resampler_t *resampler = (tdav_speex_resampler_t *)self;
    int ret = 0;
    uint32_t bytes_per_sample = (bits_per_sample >> 3);

    if (in_channels != 1 && in_channels != 2) {
        TSK_DEBUG_ERROR("%d not valid as input channel", in_channels);
        return -1;
    }
    if (out_channels != 1 && out_channels != 2) {
        TSK_DEBUG_ERROR("%d not valid as output channel", out_channels);
        return -1;
    }
    if (bytes_per_sample != sizeof(spx_int16_t) && bytes_per_sample != sizeof(float)) {
        TSK_DEBUG_ERROR("%d not valid as bits_per_sample", bits_per_sample);
        return -1;
    }

    if (!(resampler->state = speex_resampler_init(in_channels, in_freq, out_freq, TSK_CLAMP(0, quality, TDAV_SPEEX_RESAMPLER_MAX_QUALITY), &ret))) {
        TSK_DEBUG_ERROR("speex_resampler_init() returned %d", ret);
        return -2;
    }

    resampler->bytes_per_sample = bytes_per_sample;
    resampler->in_size = ((in_freq * frame_duration) / 1000) << (in_channels == 2 ? 1 : 0);
    resampler->out_size = ((out_freq * frame_duration) / 1000) << (out_channels == 2 ? 1 : 0);
    resampler->in_channels = in_channels;
    resampler->out_channels = out_channels;

    if (in_channels != out_channels) {
        resampler->tmp_buffer.size_in_samples = ((TSK_MAX(in_freq, out_freq) * frame_duration) / 1000) << (TSK_MAX(in_channels, out_channels) == 2 ? 1 : 0);
        if (!(resampler->tmp_buffer.ptr = tsk_realloc(resampler->tmp_buffer.ptr, resampler->tmp_buffer.size_in_samples * resampler->bytes_per_sample))) {
            resampler->tmp_buffer.size_in_samples = 0;
            return -2;
        }
    }

    return 0;
}


static tsk_size_t tdav_speex_resampler_process(tmedia_resampler_t* self, const void* in_data, tsk_size_t in_size_in_sample, void* out_data, tsk_size_t out_size_in_sample)
{
    tdav_speex_resampler_t *resampler = (tdav_speex_resampler_t *)self;
    int err = RESAMPLER_ERR_SUCCESS;
    spx_uint32_t _out_size_in_sample = (spx_uint32_t)out_size_in_sample;
    if (!resampler->state || !out_data) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return 0;
    }

    if (in_size_in_sample != resampler->in_size) {
        TSK_DEBUG_ERROR("Input data has wrong size");
        return 0;
    }

    if (out_size_in_sample < resampler->out_size) {
        TSK_DEBUG_ERROR("Output data is too short");
        return 0;
    }

    if (resampler->in_channels == resampler->out_channels) {
        if (resampler->bytes_per_sample == sizeof(spx_int16_t)) {
            err = speex_resampler_process_int(resampler->state, 0,
                                              (const spx_int16_t *)in_data, (spx_uint32_t *)&in_size_in_sample,
                                              (spx_int16_t *)out_data, &_out_size_in_sample);
        }
        else {
            err = speex_resampler_process_float(resampler->state, 0,
                                                (const float *)in_data, (spx_uint32_t *)&in_size_in_sample,
                                                (float *)out_data, &_out_size_in_sample);
        }
    }
    else {
        spx_uint32_t i, j;
        // in_channels = 1, out_channels = 2
        if (resampler->in_channels == 1) {
            if (resampler->bytes_per_sample == sizeof(spx_int16_t)) {
                err = speex_resampler_process_int(resampler->state, 0, (const spx_int16_t *)in_data, (spx_uint32_t *)&in_size_in_sample, resampler->tmp_buffer.ptr, &_out_size_in_sample);
                if (err == RESAMPLER_ERR_SUCCESS) {
                    spx_int16_t* pout_data = (spx_int16_t*)(out_data);
                    for (i = 0, j = 0; i < _out_size_in_sample; ++i, j += 2) {
                        pout_data[j] = pout_data[j + 1] = *(((const spx_int16_t*)resampler->tmp_buffer.ptr) + i);
                    }
                }
            }
            else {
                err = speex_resampler_process_float(resampler->state, 0, (const float *)in_data, (spx_uint32_t *)&in_size_in_sample, resampler->tmp_buffer.ptr, &_out_size_in_sample);
                if (err == RESAMPLER_ERR_SUCCESS) {
                    float* pout_data = (float*)(out_data);
                    for (i = 0, j = 0; i < _out_size_in_sample; ++i, j += 2) {
                        pout_data[j] = pout_data[j + 1] = *(((const float*)resampler->tmp_buffer.ptr) + i);
                    }
                }
            }

        }
        else {
            // in_channels = 2, out_channels = 1
            spx_uint32_t _out_size2_in_sample = (_out_size_in_sample << 1);
            if (resampler->bytes_per_sample == sizeof(spx_int16_t)) {
                err = speex_resampler_process_int(resampler->state, 0,
                                                  (const spx_int16_t *)in_data, (spx_uint32_t *)&in_size_in_sample,
                                                  (spx_int16_t *)resampler->tmp_buffer.ptr, &_out_size2_in_sample);
                if (err == RESAMPLER_ERR_SUCCESS) {
                    spx_int16_t* pout_data = (spx_int16_t*)(out_data);
                    _out_size_in_sample = (spx_uint32_t)resampler->out_size;
                    for (i = 0, j = 0; j < _out_size2_in_sample; ++i, j += 2) {
                        pout_data[i] = *(((const spx_int16_t*)resampler->tmp_buffer.ptr) + j);
                    }
                }
            }
            else {
                err = speex_resampler_process_float(resampler->state, 0,
                                                    (const float *)in_data, (spx_uint32_t *)&in_size_in_sample,
                                                    (float *)resampler->tmp_buffer.ptr, &_out_size2_in_sample);
                if (err == RESAMPLER_ERR_SUCCESS) {
                    float* pout_data = (float*)(out_data);
                    for (i = 0, j = 0; j < _out_size2_in_sample; ++i, j += 2) {
                        pout_data[i] = *(((const float*)resampler->tmp_buffer.ptr) + j);
                    }
                }
            }
        }
    }

    if (err != RESAMPLER_ERR_SUCCESS) {
        TSK_DEBUG_ERROR("speex_resampler_process_int() failed with error code %d", err);
        return 0;
    }
    return (tsk_size_t)_out_size_in_sample;
}

static int tdav_speex_resampler_close(tmedia_resampler_t* self)
{
    tdav_speex_resampler_t *resampler = (tdav_speex_resampler_t *)self;

    if (resampler->state) {
        speex_resampler_destroy(resampler->state);
        resampler->state = tsk_null;
    }
    return 0;
}



//
//	Speex resamplerr Plugin definition
//

/* constructor */
static tsk_object_t* tdav_speex_resampler_ctor(tsk_object_t * self, va_list * app)
{
    tdav_speex_resampler_t *resampler = (tdav_speex_resampler_t *)self;
    if (resampler) {
        /* init base */
        tmedia_resampler_init(TMEDIA_RESAMPLER(resampler));
        /* init self */
    }
    return self;
}
/* destructor */
static tsk_object_t* tdav_speex_resampler_dtor(tsk_object_t * self)
{
    tdav_speex_resampler_t *resampler = (tdav_speex_resampler_t *)self;
    if (resampler) {
        /* deinit base */
        tmedia_resampler_deinit(TMEDIA_RESAMPLER(resampler));
        /* deinit self */
        if (resampler->state) {
            speex_resampler_destroy(resampler->state);
            resampler->state = tsk_null;
        }
        TSK_FREE(resampler->tmp_buffer.ptr);

        TSK_DEBUG_INFO("*** SpeexDSP resampler (plugin) destroyed ***");
    }

    return self;
}
/* object definition */
static const tsk_object_def_t tdav_speex_resampler_def_s = {
    sizeof(tdav_speex_resampler_t),
    tdav_speex_resampler_ctor,
    tdav_speex_resampler_dtor,
    tsk_null,
};
/* plugin definition*/
static const tmedia_resampler_plugin_def_t tdav_speex_resampler_plugin_def_s = {
    &tdav_speex_resampler_def_s,

    "Audio Resampler based on Speex",

    tdav_speex_resampler_open,
    tdav_speex_resampler_process,
    tdav_speex_resampler_close,
};
const tmedia_resampler_plugin_def_t *tdav_speex_resampler_plugin_def_t = &tdav_speex_resampler_plugin_def_s;


#endif /* HAVE_SPEEX_DSP */
