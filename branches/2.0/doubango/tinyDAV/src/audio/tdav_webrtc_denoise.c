/*
* Copyright (C) 2011 Doubango Telecom <http://www.doubango.org>
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

/**@file tdav_webrtc_denoise.c
* @brief Google WebRTC Denoiser (Noise suppression, AGC, AEC) Plugin
*
* @author Mamadou Diop <diopmamadou(at)doubango.org>
*

*/
#include "tinydav/audio/tdav_webrtc_denoise.h"

#if HAVE_WEBRTC && (!defined(HAVE_WEBRTC_DENOISE) || HAVE_WEBRTC_DENOISE)

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#include "tinymedia/tmedia_defaults.h"

#include <string.h>

#if !defined(WEBRTC_AEC_AGGRESSIVE)
#	define WEBRTC_AEC_AGGRESSIVE		0
#endif
#if !defined(WEBRTC_MAX_ECHO_TAIL)
#	define WEBRTC_MAX_ECHO_TAIL		500
#endif
#if !defined(WEBRTC_MIN_ECHO_TAIL)
#	define WEBRTC_MIN_ECHO_TAIL		20 // 0 will cause random crashes
static const int32_t kSizeOfWord16 = sizeof(int16_t);
#endif

static int tdav_webrtc_denoise_set(tmedia_denoise_t* _self, const tmedia_param_t* param)
{
	tdav_webrtc_denoise_t *self = (tdav_webrtc_denoise_t *)_self;
	if(!self || !param){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	
	if(param->value_type == tmedia_pvt_int32){
		if(tsk_striequals(param->key, "echo-tail")){
			int32_t echo_tail = *((int32_t*)param->value);
			self->echo_tail = TSK_CLAMP(WEBRTC_MIN_ECHO_TAIL, echo_tail, WEBRTC_MAX_ECHO_TAIL);
			TSK_DEBUG_INFO("set_echo_tail (%d->%d)", echo_tail, self->echo_tail);
			return 0;
		}
	}
	return -1;
}

static int tdav_webrtc_denoise_open(tmedia_denoise_t* self, uint32_t record_frame_size_samples, uint32_t record_sampling_rate, uint32_t playback_frame_size_samples, uint32_t playback_sampling_rate)
{
	tdav_webrtc_denoise_t *denoiser = (tdav_webrtc_denoise_t *)self;
	int ret;

	if(!denoiser){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(denoiser->AEC_inst || 
#if HAVE_SPEEX_DSP && PREFER_SPEEX_DENOISER
		denoiser->SpeexDenoiser_proc
#else
		denoiser->NS_inst
#endif
		){
		TSK_DEBUG_ERROR("Denoiser already initialized");
		return -2;
	}
	
	denoiser->echo_tail = TSK_CLAMP(WEBRTC_MIN_ECHO_TAIL, TMEDIA_DENOISE(denoiser)->echo_tail, WEBRTC_MAX_ECHO_TAIL);
	TSK_DEBUG_INFO("echo_tail=%d", denoiser->echo_tail);
	denoiser->echo_skew = TMEDIA_DENOISE(denoiser)->echo_skew;
	denoiser->record_frame_size_samples = record_frame_size_samples;
	denoiser->record_sampling_rate = record_sampling_rate;
	denoiser->playback_frame_size_samples = playback_frame_size_samples;
	denoiser->playback_sampling_rate = playback_sampling_rate;
	
	//
	//	AEC instance
	//
	if((ret = TDAV_WebRtcAec_Create(&denoiser->AEC_inst))){
		TSK_DEBUG_ERROR("WebRtcAec_Create failed with error code = %d", ret);
		return ret;
	}
	if((ret = TDAV_WebRtcAec_Init(denoiser->AEC_inst, denoiser->record_sampling_rate, denoiser->record_sampling_rate))){
		TSK_DEBUG_ERROR("WebRtcAec_Init failed with error code = %d", ret);
		return ret;
	}

#if WEBRTC_AEC_AGGRESSIVE
	{	
		AecConfig aecConfig;
		aecConfig.nlpMode = kAecNlpAggressive;
		aecConfig.skewMode = kAecTrue;
		aecConfig.metricsMode = kAecFalse;
		aecConfig.delay_logging = kAecFalse;

		if((ret = WebRtcAec_set_config(denoiser->AEC_inst, aecConfig))){
			TSK_DEBUG_ERROR("WebRtcAec_set_config failed with error code = %d", ret);
		}
	}
#endif
	
	//
	//	Noise Suppression instance
	//
	if(TMEDIA_DENOISE(denoiser)->noise_supp_enabled){
#if HAVE_SPEEX_DSP && PREFER_SPEEX_DENOISER
		if((denoiser->SpeexDenoiser_proc = speex_preprocess_state_init(denoiser->record_frame_size_samples, denoiser->record_sampling_rate))){
			int i = 1;
			speex_preprocess_ctl(denoiser->SpeexDenoiser_proc, SPEEX_PREPROCESS_SET_DENOISE, &i);
			i = TMEDIA_DENOISE(denoiser)->noise_supp_level;
			speex_preprocess_ctl(denoiser->SpeexDenoiser_proc, SPEEX_PREPROCESS_SET_NOISE_SUPPRESS, &i);
		}
#else
		if((ret = TDAV_WebRtcNs_Create(&denoiser->NS_inst))){
			TSK_DEBUG_ERROR("WebRtcNs_Create failed with error code = %d", ret);
			return ret;
		}
		if((ret = TDAV_WebRtcNs_Init(denoiser->NS_inst, denoiser->sampling_rate))){
			TSK_DEBUG_ERROR("WebRtcNs_Init failed with error code = %d", ret);
			return ret;
		}
#endif
	}

	// allocate temp buffer for record processing
	if(!(denoiser->temp_rec_out = tsk_realloc(denoiser->temp_rec_out, denoiser->record_frame_size_samples * kSizeOfWord16))){
		TSK_DEBUG_ERROR("Failed to allocate new buffer");
		return -3;
	}

	TSK_DEBUG_INFO("WebRTC denoiser opened");

	return ret;
}

static int tdav_webrtc_denoise_echo_playback(tmedia_denoise_t* self, const void* echo_frame, uint32_t echo_frame_size_bytes)
{
	tdav_webrtc_denoise_t *denoiser = (tdav_webrtc_denoise_t *)self;
	if(denoiser->AEC_inst){
		int ret;
		const int16_t *pEchoFrame = (const int16_t *)echo_frame;
		switch(denoiser->record_sampling_rate){
			case 8000:
				{
					if((ret = TDAV_WebRtcAec_BufferFarend(denoiser->AEC_inst, pEchoFrame, denoiser->record_frame_size_samples))){
						TSK_DEBUG_ERROR("WebRtcAec_BufferFarend failed with error code = %d", ret);
						return ret;
					}
					break;
				}
			case 16000:
			case 32000:
				{
					// Split in several 160 samples
					uint32_t i, k = (denoiser->record_sampling_rate == 16000 ? 1 : 2);
					for(i = 0; i<denoiser->record_frame_size_samples; i+=(denoiser->record_frame_size_samples>>k)){
						if((ret = TDAV_WebRtcAec_BufferFarend(denoiser->AEC_inst, &pEchoFrame[i], (denoiser->record_frame_size_samples>>k)))){
							TSK_DEBUG_ERROR("WebRtcAec_BufferFarend failed with error code = %d", ret);
							return ret;
						}
					}
					break;
				}
			default:
				{
					TSK_DEBUG_ERROR("%d Hz not supported by WebRTC AEC", denoiser->record_sampling_rate);
					return -2;
				}
		}
	}
	return 0;
}



static int tdav_webrtc_denoise_process_record(tmedia_denoise_t* self, void* audio_frame, uint32_t audio_frame_size_bytes, tsk_bool_t* silence_or_noise)
{
	tdav_webrtc_denoise_t *denoiser = (tdav_webrtc_denoise_t *)self;
	int ret = 0;
	
	*silence_or_noise = tsk_false;

	tsk_safeobj_lock(denoiser);

	if(denoiser->AEC_inst){
		int16_t *pAudioFrame = audio_frame;
		
		// Noise suppression
#if HAVE_SPEEX_DSP && PREFER_SPEEX_DENOISER
		if(denoiser->SpeexDenoiser_proc){
			speex_preprocess_run(denoiser->SpeexDenoiser_proc, pAudioFrame);
		}
		memcpy(denoiser->temp_rec_out, pAudioFrame, denoiser->record_frame_size_samples * sizeof(spx_int16_t));
#else

		// WebRTC NoiseSupp only accept 10ms frames
		// Our encoder will always output 20ms frames ==> execute 2x noise_supp
		if(denoiser->NS_inst){
			if(
					(ret = TDAV_WebRtcNs_Process(denoiser->NS_inst, pAudioFrame, tsk_null, denoiser->temp_rec_out, tsk_null)) ||
					(ret = TDAV_WebRtcNs_Process(denoiser->NS_inst, &pAudioFrame[(denoiser->frame_size>>1)], tsk_null, &denoiser->temp_rec_out[(denoiser->frame_size>>1)], tsk_null))
				)
			{
				TSK_DEBUG_ERROR("WebRtcNs_Process with error code = %d", ret);
				goto bail;
			}
		}
		else{
			memcpy(denoiser->temp_rec_out, pAudioFrame, (denoiser->frame_size * kSizeOfWord16));
		}
#endif

		// AEC
		switch(denoiser->record_sampling_rate){
			case 8000:
				{
					if((ret = TDAV_WebRtcAec_Process(denoiser->AEC_inst, denoiser->temp_rec_out, tsk_null, pAudioFrame, tsk_null, denoiser->record_frame_size_samples, denoiser->echo_tail, denoiser->echo_skew))){
						TSK_DEBUG_ERROR("WebRtcAec_Process with error code = %d", ret);
						goto bail;
					}
					break;
				}
			case 16000:
			case 32000:
				{
					// Split in several 160 samples
					uint32_t i, k = (denoiser->record_sampling_rate == 16000 ? 1 : 2);
					for(i = 0; i<denoiser->record_frame_size_samples; i+=(denoiser->record_frame_size_samples>>k)){
						if((ret = TDAV_WebRtcAec_Process(denoiser->AEC_inst, &denoiser->temp_rec_out[i], tsk_null, &pAudioFrame[i], tsk_null, (denoiser->record_frame_size_samples>>k), denoiser->echo_tail, denoiser->echo_skew))){
							TSK_DEBUG_ERROR("WebRtcAec_Process with error code = %d", ret);
							goto bail;
						}
					}
					break;
				}
			default:
				{
					TSK_DEBUG_ERROR("%d Hz not supported by WebRTC AEC", denoiser->record_sampling_rate);
					ret = -2;
					goto bail;
				}
		}
	}

bail:
	tsk_safeobj_unlock(denoiser);
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
	if(denoiser->AEC_inst){
		TDAV_WebRtcAec_Free(denoiser->AEC_inst);
		denoiser->AEC_inst = tsk_null;
	}
#if HAVE_SPEEX_DSP && PREFER_SPEEX_DENOISER
	if(denoiser->SpeexDenoiser_proc){
		speex_preprocess_state_destroy(denoiser->SpeexDenoiser_proc);
		denoiser->SpeexDenoiser_proc = tsk_null;
	}
#else
	if(denoiser->NS_inst){
		TDAV_WebRtcNs_Free(denoiser->NS_inst);
		denoiser->NS_inst = tsk_null;
	}
#endif
	TSK_FREE(denoiser->temp_rec_out);
	tsk_safeobj_unlock(denoiser);

	return 0;
}


//
//	Speex denoiser Plugin definition
//

/* constructor */
static tsk_object_t* tdav_webrtc_denoise_ctor(tsk_object_t * _self, va_list * app)
{
	tdav_webrtc_denoise_t *self = _self;
	if(self){
		/* init base */
		tmedia_denoise_init(TMEDIA_DENOISE(self));
		/* init self */
		tsk_safeobj_init(self);
	}
	return self;
}
/* destructor */
static tsk_object_t* tdav_webrtc_denoise_dtor(tsk_object_t * _self)
{ 
	tdav_webrtc_denoise_t *self = _self;
	if(self){
		/* deinit base (will close the denoise if not done yet) */
		tmedia_denoise_deinit(TMEDIA_DENOISE(self));
		/* deinit self */
		tsk_safeobj_deinit(self);
	}

	return self;
}
/* object definition */
static const tsk_object_def_t tdav_webrtc_denoise_def_s = 
{
	sizeof(tdav_webrtc_denoise_t),
	tdav_webrtc_denoise_ctor, 
	tdav_webrtc_denoise_dtor,
	tsk_null, 
};
/* plugin definition*/
static const tmedia_denoise_plugin_def_t tdav_webrtc_denoise_plugin_def_s = 
{
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