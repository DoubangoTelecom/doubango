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

#include "tsk_memory.h"
#include "tsk_debug.h"

int tdav_speex_resampler_open(tmedia_resampler_t* self, uint32_t in_freq, uint32_t out_freq, tsk_size_t frame_duration, int8_t _channels, uint32_t quality)
{
	tdav_speex_resampler_t *resampler = (tdav_speex_resampler_t *)self;
	int ret = 0;

	if(!(resampler->state = speex_resampler_init(_channels, in_freq, out_freq, quality>10 ? TMEDIA_RESAMPLER_QUALITY : quality, &ret))){
		TSK_DEBUG_ERROR("speex_resampler_init() returned %d", ret);
		return -2;
	}

	resampler->in_size  = (in_freq * frame_duration)/1000;
	resampler->out_size = (out_freq * frame_duration) / 1000;
	resampler->channels = _channels;

	return 0;
}

tsk_size_t tdav_speex_resampler_process(tmedia_resampler_t* self, const uint16_t* in_data, tsk_size_t in_size, uint16_t* out_data, tsk_size_t out_size)
{
	tdav_speex_resampler_t *resampler = (tdav_speex_resampler_t *)self;
	spx_uint32_t out_len = (spx_uint32_t)out_size;
	int err;
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

	err = speex_resampler_process_int(resampler->state, 0, (const spx_int16_t *)in_data, (spx_uint32_t *)&in_size, (spx_int16_t *)out_data, &out_len);
	if(err != RESAMPLER_ERR_SUCCESS){
		TSK_DEBUG_ERROR("speex_resampler_process_int() failed with error code %d", err);
		return 0;
	}
	return (tsk_size_t)out_len;
}

int tdav_speex_resampler_close(tmedia_resampler_t* self)
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
	tdav_speex_resampler_t *resampler = self;
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
	tdav_speex_resampler_t *resampler = self;
	if(resampler){
		/* deinit base */
		tmedia_resampler_deinit(TMEDIA_RESAMPLER(resampler));
		/* deinit self */
		if(resampler->state){
			speex_resampler_destroy(resampler->state);
			resampler->state = tsk_null;
		}
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
