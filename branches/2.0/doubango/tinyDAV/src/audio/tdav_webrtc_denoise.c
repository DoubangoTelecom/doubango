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

#include "tsk_memory.h"
#include "tsk_debug.h"

#include "tinymedia/tmedia_defaults.h"

#include <string.h>

#define WEBRTC_MAX_ECHO_TAIL		500

static int tdav_webrtc_denoise_set(tmedia_denoise_t* self, const tmedia_param_t* param)
{
	/* tdav_webrtc_denoise_t *denoiser = (tdav_webrtc_denoise_t *)self; */
	return tmedia_denoise_set(self, param);
}

static int tdav_webrtc_denoise_open(tmedia_denoise_t* self, uint32_t frame_size, uint32_t sampling_rate)
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
	
	denoiser->sound_card_buffer_len = TSK_MIN(TMEDIA_DENOISE(denoiser)->echo_tail, WEBRTC_MAX_ECHO_TAIL);
	denoiser->echo_skew = TMEDIA_DENOISE(denoiser)->echo_skew;
	denoiser->frame_size = frame_size;
	denoiser->sampling_rate = sampling_rate;
	
	//
	//	AEC instance
	//
	if((ret = TDAV_WebRtcAec_Create(&denoiser->AEC_inst))){
		TSK_DEBUG_ERROR("WebRtcAec_Create failed with error code = %d", ret);
		return ret;
	}
	if((ret = TDAV_WebRtcAec_Init(denoiser->AEC_inst, denoiser->sampling_rate, denoiser->sampling_rate))){
		TSK_DEBUG_ERROR("WebRtcAec_Init failed with error code = %d", ret);
		return ret;
	}
	
	//
	//	Noise Suppression instance
	//
	if(TMEDIA_DENOISE(denoiser)->noise_supp_enabled){
#if HAVE_SPEEX_DSP && PREFER_SPEEX_DENOISER
		if((denoiser->SpeexDenoiser_proc = speex_preprocess_state_init(denoiser->frame_size, denoiser->sampling_rate))){
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
	if(!(denoiser->temp_rec_out = tsk_realloc(denoiser->temp_rec_out, denoiser->frame_size * sizeof(WebRtc_Word16)))){
		TSK_DEBUG_ERROR("Failed to allocate new buffer");
		return -3;
	}

	TSK_DEBUG_INFO("WebRTC denoiser opened");

	return ret;
}

static int tdav_webrtc_denoise_echo_playback(tmedia_denoise_t* self, const void* echo_frame)
{
	tdav_webrtc_denoise_t *denoiser = (tdav_webrtc_denoise_t *)self;
	if(denoiser->AEC_inst){
		int ret;
		const WebRtc_Word16 *pEchoFrame = (const WebRtc_Word16 *)echo_frame;
		switch(denoiser->sampling_rate){
			case 8000:
				{
					if((ret = TDAV_WebRtcAec_BufferFarend(denoiser->AEC_inst, pEchoFrame, denoiser->frame_size))){
						TSK_DEBUG_ERROR("WebRtcAec_BufferFarend failed with error code = %d", ret);
						return ret;
					}
					break;
				}
				// 32Hz and 16Khz work but produce very ugly results
			case 16000:
			case 32000:
				{
					uint32_t i, k = (denoiser->sampling_rate == 16000 ? 1 : 2);
					for(i = 0; i<denoiser->frame_size; i+=(denoiser->frame_size>>k)){
						if((ret = TDAV_WebRtcAec_BufferFarend(denoiser->AEC_inst, &pEchoFrame[i], (denoiser->frame_size>>k)))){
							TSK_DEBUG_ERROR("WebRtcAec_BufferFarend failed with error code = %d", ret);
							return ret;
						}
					}
					break;
				}
			default:
				{
					TSK_DEBUG_ERROR("%d Hz not supported by WebRTC AEC", denoiser->sampling_rate);
					return -2;
				}
		}
	}
	return 0;
}



static int tdav_webrtc_denoise_process_record(tmedia_denoise_t* self, void* audio_frame, tsk_bool_t* silence_or_noise)
{
	tdav_webrtc_denoise_t *denoiser = (tdav_webrtc_denoise_t *)self;
	
	*silence_or_noise = tsk_false;

	if(denoiser->AEC_inst){
		int ret;
		WebRtc_Word16 *pAudioFrame = audio_frame;
		
		// Noise suppression
#if HAVE_SPEEX_DSP && PREFER_SPEEX_DENOISER
		if(denoiser->SpeexDenoiser_proc){
			speex_preprocess_run(denoiser->SpeexDenoiser_proc, denoiser->temp_rec_out);
		}
		memcpy(denoiser->temp_rec_out, pAudioFrame, denoiser->frame_size * sizeof(spx_int16_t));
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
				return ret;
			}
		}
#endif

		// AEC
		switch(denoiser->sampling_rate){
			case 8000:
				{
					if((ret = TDAV_WebRtcAec_Process(denoiser->AEC_inst, denoiser->temp_rec_out, tsk_null, pAudioFrame, tsk_null, denoiser->frame_size, denoiser->sound_card_buffer_len, denoiser->echo_skew))){
						TSK_DEBUG_ERROR("WebRtcAec_Process with error code = %d", ret);
						return ret;
					}
					break;
				}
				// 32Hz and 16Khz work but produce very ugly results
			case 16000:
			case 3200:
				{
					uint32_t i, k = (denoiser->sampling_rate == 16000 ? 1 : 2);
					for(i = 0; i<denoiser->frame_size; i+=(denoiser->frame_size>>k)){
						if((ret = TDAV_WebRtcAec_Process(denoiser->AEC_inst, &denoiser->temp_rec_out[i], tsk_null, &pAudioFrame[i], tsk_null, (denoiser->frame_size>>k), denoiser->sound_card_buffer_len, denoiser->echo_skew))){
							TSK_DEBUG_ERROR("WebRtcAec_Process with error code = %d", ret);
							return ret;
						}

					}
					break;
				}
			default:
				{
					TSK_DEBUG_ERROR("%d Hz not supported by WebRTC AEC", denoiser->sampling_rate);
					return -2;
				}
		}
	}

	return 0;
}

static int tdav_webrtc_denoise_process_playback(tmedia_denoise_t* self, void* audio_frame)
{
	tdav_webrtc_denoise_t *denoiser = (tdav_webrtc_denoise_t *)self;
	
	(denoiser);

	// Not mandatory but we could denoise the audio data sent from an encoder without denoiser
	// All Doubango clients support noise suppression
	return 0;
}

static int tdav_webrtc_denoise_close(tmedia_denoise_t* self)
{
	tdav_webrtc_denoise_t *denoiser = (tdav_webrtc_denoise_t *)self;

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

	return 0;
}


//
//	Speex denoiser Plugin definition
//

/* constructor */
static tsk_object_t* tdav_webrtc_denoise_ctor(tsk_object_t * self, va_list * app)
{
	tdav_webrtc_denoise_t *denoise = self;
	if(denoise){
		/* init base */
		tmedia_denoise_init(TMEDIA_DENOISE(denoise));
		/* init self */
	}
	return self;
}
/* destructor */
static tsk_object_t* tdav_webrtc_denoise_dtor(tsk_object_t * self)
{ 
	tdav_webrtc_denoise_t *denoise = self;
	if(denoise){
		/* deinit base */
		tmedia_denoise_deinit(TMEDIA_DENOISE(denoise));
		/* deinit self */
		if(denoise->AEC_inst){
			TDAV_WebRtcAec_Free(denoise->AEC_inst);
			denoise->AEC_inst = tsk_null;
		}
		
#if HAVE_SPEEX_DSP && PREFER_SPEEX_DENOISER
		if(denoise->SpeexDenoiser_proc){
			speex_preprocess_state_destroy(denoise->SpeexDenoiser_proc);
			denoise->SpeexDenoiser_proc = tsk_null;
		}
#else
		if(denoise->NS_inst){
			TDAV_WebRtcNs_Free(denoise->NS_inst);
			denoise->NS_inst = tsk_null;
		}
#endif
		TSK_FREE(denoise->temp_rec_out);
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