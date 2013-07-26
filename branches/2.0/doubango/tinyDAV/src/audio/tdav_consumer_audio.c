/*
* Copyright (C) 2010-2011 Mamadou Diop.
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

/**@file tdav_consumer_audio.c
* @brief Base class for all Audio consumers.
*
* @author Mamadou Diop <diopmamadou(at)doubango.org>
*/
#include "tinydav/audio/tdav_consumer_audio.h"

#include "tinymedia/tmedia_defaults.h"
#include "tinymedia/tmedia_denoise.h"
#include "tinymedia/tmedia_resampler.h"
#include "tinymedia/tmedia_jitterbuffer.h"
#include "tinyrtp/rtp/trtp_rtp_header.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_time.h"
#include "tsk_debug.h"

#if TSK_UNDER_WINDOWS
#	include <Winsock2.h> // timeval
#elif defined(__SYMBIAN32__)
#	include <_timeval.h> 
#else
#	include <sys/time.h>
#endif

#define TDAV_BITS_PER_SAMPLE_DEFAULT	16
#define TDAV_CHANNELS_DEFAULT			2
#define TDAV_RATE_DEFAULT				8000
#define TDAV_PTIME_DEFAULT				20

#define TDAV_AUDIO_GAIN_MAX				15

/** Initialize audio consumer */
int tdav_consumer_audio_init(tdav_consumer_audio_t* self)
{
	int ret;

	TSK_DEBUG_INFO("tdav_consumer_audio_init()");

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	/* base */
	if((ret = tmedia_consumer_init(TMEDIA_CONSUMER(self)))){
		return ret;
	}

	/* self (should be update by prepare() by using the codec's info)*/
	TMEDIA_CONSUMER(self)->audio.bits_per_sample = TDAV_BITS_PER_SAMPLE_DEFAULT;
	TMEDIA_CONSUMER(self)->audio.ptime = TDAV_PTIME_DEFAULT;
	TMEDIA_CONSUMER(self)->audio.in.channels = TDAV_CHANNELS_DEFAULT;
	TMEDIA_CONSUMER(self)->audio.in.rate = TDAV_RATE_DEFAULT;
	TMEDIA_CONSUMER(self)->audio.gain = TSK_MIN(tmedia_defaults_get_audio_consumer_gain(), TDAV_AUDIO_GAIN_MAX);

	tsk_safeobj_init(self);

	return 0;
}

/**
* Generic function to compare two consumers.
* @param consumer1 The first consumer to compare.
* @param consumer2 The second consumer to compare.
* @retval Returns an integral value indicating the relationship between the two consumers:
* <0 : @a consumer1 less than @a consumer2.<br>
* 0  : @a consumer1 identical to @a consumer2.<br>
* >0 : @a consumer1 greater than @a consumer2.<br>
*/
int tdav_consumer_audio_cmp(const tsk_object_t* consumer1, const tsk_object_t* consumer2)
{	
	return (TDAV_CONSUMER_AUDIO(consumer1) - TDAV_CONSUMER_AUDIO(consumer2));
}

int tdav_consumer_audio_set(tdav_consumer_audio_t* self, const tmedia_param_t* param)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(param->plugin_type == tmedia_ppt_consumer){
		if(param->value_type == tmedia_pvt_int32){
			if(tsk_striequals(param->key, "gain")){
				int32_t gain = *((int32_t*)param->value);
				if(gain<TDAV_AUDIO_GAIN_MAX && gain>=0){
					TMEDIA_CONSUMER(self)->audio.gain = (uint8_t)gain;
					TSK_DEBUG_INFO("audio consumer gain=%u", gain);
				}
				else{
					TSK_DEBUG_ERROR("%u is invalid as gain value", gain);
					return -2;
				}
			}
			else if(tsk_striequals(param->key, "volume")){
				TMEDIA_CONSUMER(self)->audio.volume = TSK_TO_INT32((uint8_t*)param->value);
				TMEDIA_CONSUMER(self)->audio.volume = TSK_CLAMP(0, TMEDIA_CONSUMER(self)->audio.volume, 100);
			}
		}
	}

	return 0;
}

/* put data (bytes not shorts) into the jitter buffer (consumers always have ptime of 20ms) */
int tdav_consumer_audio_put(tdav_consumer_audio_t* self, const void* data, tsk_size_t data_size, const tsk_object_t* proto_hdr)
{
	int ret;

	if(!self || !data || !self->jitterbuffer){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	tsk_safeobj_lock(self);

	if(!TMEDIA_JITTER_BUFFER(self->jitterbuffer)->opened){
		uint32_t rate = TMEDIA_CONSUMER(self)->audio.out.rate ? TMEDIA_CONSUMER(self)->audio.out.rate : TMEDIA_CONSUMER(self)->audio.in.rate;
		uint32_t channels = TMEDIA_CONSUMER(self)->audio.out.channels ? TMEDIA_CONSUMER(self)->audio.out.channels : tmedia_defaults_get_audio_channels_playback();
		if((ret = tmedia_jitterbuffer_open(self->jitterbuffer, TMEDIA_CONSUMER(self)->audio.ptime, rate, channels))){
			TSK_DEBUG_ERROR("Failed to open jitterbuffer (%d)", ret);
			tsk_safeobj_unlock(self);
			return ret;
		}
	}
	
	ret = tmedia_jitterbuffer_put(self->jitterbuffer, (void*)data, data_size, proto_hdr);

	tsk_safeobj_unlock(self);

	return ret;
}

/* get data from the jitter buffer (consumers should always have ptime of 20ms) */
tsk_size_t tdav_consumer_audio_get(tdav_consumer_audio_t* self, void* out_data, tsk_size_t out_size)
{
	tsk_size_t ret_size = 0;
	if(!self || !self->jitterbuffer){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}

	tsk_safeobj_lock(self);

	if(!TMEDIA_JITTER_BUFFER(self->jitterbuffer)->opened){
		int ret;
		uint32_t frame_duration = TMEDIA_CONSUMER(self)->audio.ptime;
		uint32_t rate = TMEDIA_CONSUMER(self)->audio.out.rate ? TMEDIA_CONSUMER(self)->audio.out.rate : TMEDIA_CONSUMER(self)->audio.in.rate;
		uint32_t channels = TMEDIA_CONSUMER(self)->audio.out.channels ? TMEDIA_CONSUMER(self)->audio.out.channels : tmedia_defaults_get_audio_channels_playback();
		if((ret = tmedia_jitterbuffer_open(TMEDIA_JITTER_BUFFER(self->jitterbuffer), frame_duration, rate, channels))){
			TSK_DEBUG_ERROR("Failed to open jitterbuffer (%d)", ret);
			tsk_safeobj_unlock(self);
			return 0;
		}
	}
	ret_size = tmedia_jitterbuffer_get(TMEDIA_JITTER_BUFFER(self->jitterbuffer), out_data, out_size);

	tsk_safeobj_unlock(self);	

	// denoiser
	if(self->denoise && self->denoise->opened && (self->denoise->echo_supp_enabled || self->denoise->noise_supp_enabled)){
		if(self->denoise->echo_supp_enabled){
			// Echo process last frame 
			if(self->denoise->last_frame && self->denoise->last_frame->size){
				tmedia_denoise_echo_playback(self->denoise, self->denoise->last_frame->data, self->denoise->last_frame->size);
			}
			if(ret_size){
				// save
				tsk_buffer_copy(self->denoise->last_frame, 0, out_data, ret_size);
			}
		}

#if 1 // suppress noise if not supported by remote party's encoder
		// suppress noise
		if(self->denoise->noise_supp_enabled){
			tmedia_denoise_process_playback(self->denoise, out_data, out_size);
		}
#endif
	}

	return ret_size;
}

int tdav_consumer_audio_tick(tdav_consumer_audio_t* self)
{
	if(!self || !self->jitterbuffer){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}
	return tmedia_jitterbuffer_tick(TMEDIA_JITTER_BUFFER(self->jitterbuffer));
}

/* set denioiser */
void tdav_consumer_audio_set_denoise(tdav_consumer_audio_t* self, struct tmedia_denoise_s* denoise)
{
	tsk_safeobj_lock(self);
	TSK_OBJECT_SAFE_FREE(self->denoise);
	self->denoise = (struct tmedia_denoise_s*)tsk_object_ref(denoise);
	tsk_safeobj_unlock(self);
}

void tdav_consumer_audio_set_jitterbuffer(tdav_consumer_audio_t* self, struct tmedia_jitterbuffer_s* jitterbuffer)
{
	tsk_safeobj_lock(self);
	TSK_OBJECT_SAFE_FREE(self->jitterbuffer);
	self->jitterbuffer = (struct tmedia_jitterbuffer_s*)tsk_object_ref(jitterbuffer);
	tsk_safeobj_unlock(self);
}

/** Reset jitterbuffer */
int tdav_consumer_audio_reset(tdav_consumer_audio_t* self){
	int ret;
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	tsk_safeobj_lock(self);
	ret = tmedia_jitterbuffer_reset(TMEDIA_JITTER_BUFFER(self->jitterbuffer));
	tsk_safeobj_unlock(self);

	return ret;
}

/* tsk_safeobj_lock(self); */
/* tsk_safeobj_unlock(self); */

/** DeInitialize audio consumer */
int tdav_consumer_audio_deinit(tdav_consumer_audio_t* self)
{
	int ret;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	/* base */
	if((ret = tmedia_consumer_deinit(TMEDIA_CONSUMER(self)))){
		/* return ret; */
	}

	/* self */
	TSK_OBJECT_SAFE_FREE(self->denoise);
	TSK_OBJECT_SAFE_FREE(self->resampler);
	TSK_OBJECT_SAFE_FREE(self->jitterbuffer);

	tsk_safeobj_deinit(self);

	return 0;
}

