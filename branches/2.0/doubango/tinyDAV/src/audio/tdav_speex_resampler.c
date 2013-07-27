/*
* Copyright (C) 2011 Mamadou Diop.
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

/**@file tdav_speex_resampler.c
 * @brief Speex Audio resampler Plugin
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>

 */
#include "tinydav/audio/tdav_speex_resampler.h"

#if HAVE_SPEEX_DSP && (!defined(HAVE_SPEEX_RESAMPLER) || HAVE_SPEEX_RESAMPLER)

#include <speex/speex_resampler.h>

#include "tsk_memory.h"
#include "tsk_debug.h"

#define TDAV_SPEEX_RESAMPLER_MAX_QUALITY 10

/** Speex resampler*/
typedef struct tdav_speex_resampler_s
{
	TMEDIA_DECLARE_RESAMPLER;

	tsk_size_t in_size;
	tsk_size_t out_size;
	uint32_t in_channels;
	uint32_t out_channels;

	struct{
		spx_int16_t* ptr;
		tsk_size_t size_in_samples;
	} tmp_buffer;

	SpeexResamplerState *state;
}
tdav_speex_resampler_t;

static int tdav_speex_resampler_open(tmedia_resampler_t* self, uint32_t in_freq, uint32_t out_freq, uint32_t frame_duration, uint32_t in_channels, uint32_t out_channels, uint32_t quality)
{
	tdav_speex_resampler_t *resampler = (tdav_speex_resampler_t *)self;
	int ret = 0;
	
	if(in_channels != 1 && in_channels != 2){
		TSK_DEBUG_ERROR("%d not valid as input channel", in_channels);
		return -1;
	}
	if(out_channels != 1 && out_channels != 2){
		TSK_DEBUG_ERROR("%d not valid as output channel", out_channels);
		return -1;
	}

	if(!(resampler->state = speex_resampler_init(in_channels, in_freq, out_freq, quality>10 ? TDAV_SPEEX_RESAMPLER_MAX_QUALITY : quality, &ret))){
		TSK_DEBUG_ERROR("speex_resampler_init() returned %d", ret);
		return -2;
	}

	resampler->in_size  = ((in_freq * frame_duration) / 1000) << (in_channels == 2 ? 1 : 0);
	resampler->out_size = ((out_freq * frame_duration) / 1000) << (out_channels == 2 ? 1 : 0);
	resampler->in_channels = in_channels;
	resampler->out_channels = out_channels;

	if(in_channels != out_channels){
		resampler->tmp_buffer.size_in_samples = ((TSK_MAX(in_freq, out_freq) * frame_duration) / 1000) << (TSK_MAX(in_channels, out_channels) == 2 ? 1 : 0);
		if(!(resampler->tmp_buffer.ptr = (spx_int16_t*)tsk_realloc(resampler->tmp_buffer.ptr, resampler->tmp_buffer.size_in_samples * sizeof(spx_int16_t)))){
			resampler->tmp_buffer.size_in_samples = 0;
			return -2;
		}
	}

	return 0;
}

static tsk_size_t tdav_speex_resampler_process(tmedia_resampler_t* self, const uint16_t* in_data, tsk_size_t in_size, uint16_t* out_data, tsk_size_t out_size)
{
	tdav_speex_resampler_t *resampler = (tdav_speex_resampler_t *)self;
	spx_uint32_t out_len = (spx_uint32_t)out_size;
	int err = RESAMPLER_ERR_SUCCESS;
	if(!resampler->state || !out_data){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}

	if(in_size != resampler->in_size){
		TSK_DEBUG_ERROR("Input data has wrong size");
		return 0;
	}

	if(out_size < resampler->out_size){
		TSK_DEBUG_ERROR("Output data is too short");
		return 0;
	}

	if(resampler->in_channels == resampler->out_channels) {
		err = speex_resampler_process_int(resampler->state, 0,
					    (const spx_int16_t *)in_data, (spx_uint32_t *)&in_size,
					    (spx_int16_t *)out_data, &out_len);
	}
	else {
		if(resampler->in_channels == 1){
			// in_channels = 1, out_channels = 2
			spx_int16_t* pout_data = (spx_int16_t*)(out_data);
			speex_resampler_set_output_stride(resampler->state, 2);
			err = speex_resampler_process_int(resampler->state, 0, 
				(const spx_int16_t *)in_data, (spx_uint32_t *)&in_size, 
				pout_data, &out_len);
			if(err == RESAMPLER_ERR_SUCCESS) {
				spx_uint32_t i;
				// duplicate
				for(i = 0; i < out_len; i += 2){
					pout_data[i + 1] = pout_data[i];
				}
			}
		}
		else {
			// in_channels = 2, out_channels = 1
			spx_uint32_t out_len_chan2 = (out_len << 1);
			err = speex_resampler_process_int(resampler->state, 0,
					    (const spx_int16_t *)in_data, (spx_uint32_t *)&in_size,
					    (spx_int16_t *)resampler->tmp_buffer.ptr, &out_len_chan2);
			if(err == RESAMPLER_ERR_SUCCESS) {
				spx_uint32_t i, j;
				spx_int16_t* pout_data = (spx_int16_t*)(out_data);
				for(i = 0, j = 0; j < out_len_chan2; ++i, j+=2){
					pout_data[i] = resampler->tmp_buffer.ptr[j];
				}
			}
		}
	}
	
	if(err != RESAMPLER_ERR_SUCCESS){
		TSK_DEBUG_ERROR("speex_resampler_process_int() failed with error code %d", err);
		return 0;
	}
	return (tsk_size_t)out_len;
}

static int tdav_speex_resampler_close(tmedia_resampler_t* self)
{
	tdav_speex_resampler_t *resampler = (tdav_speex_resampler_t *)self;

	if(resampler->state){
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
	if(resampler){
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
	if(resampler){
		/* deinit base */
		tmedia_resampler_deinit(TMEDIA_RESAMPLER(resampler));
		/* deinit self */
		if(resampler->state){
			speex_resampler_destroy(resampler->state);
			resampler->state = tsk_null;
		}
		TSK_FREE(resampler->tmp_buffer.ptr);

		TSK_DEBUG_INFO("*** SpeexDSP resampler (plugin) destroyed ***");
	}

	return self;
}
/* object definition */
static const tsk_object_def_t tdav_speex_resampler_def_s = 
{
	sizeof(tdav_speex_resampler_t),
	tdav_speex_resampler_ctor, 
	tdav_speex_resampler_dtor,
	tsk_null, 
};
/* plugin definition*/
static const tmedia_resampler_plugin_def_t tdav_speex_resampler_plugin_def_s = 
{
	&tdav_speex_resampler_def_s,
	
	"Audio Resampler based on Speex",
	
	tdav_speex_resampler_open,
	tdav_speex_resampler_process,
	tdav_speex_resampler_close,
};
const tmedia_resampler_plugin_def_t *tdav_speex_resampler_plugin_def_t = &tdav_speex_resampler_plugin_def_s;


#endif /* HAVE_SPEEX_DSP */
