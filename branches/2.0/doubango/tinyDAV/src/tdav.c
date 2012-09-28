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
/**@file tdav.h
 * @brief tinyDAV.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#include "tinydav/tdav.h"

#if TDAV_UNDER_WINDOWS
#	include "tinydav/tdav_win32.h"
#elif TDAV_UNDER_APPLE
#	include "tinydav/tdav_apple.h"
#endif

// Media Contents, ...
#include "tinymedia.h"

// Converters
#include "tinymedia/tmedia_converter_video.h"
// Converters
#include "tinymedia/tmedia_converter_video.h"
#include "tinydav/video/tdav_converter_video.h"

// Sessions
#include "tinymedia/tmedia_session_ghost.h"
#include "tinydav/audio/tdav_session_audio.h"
#include "tinydav/video/tdav_session_video.h"
#include "tinydav/msrp/tdav_session_msrp.h"

// Codecs
#include "tinydav/codecs/dtmf/tdav_codec_dtmf.h"
#include "tinydav/codecs/fec/tdav_codec_ulpfec.h"
#include "tinydav/codecs/fec/tdav_codec_red.h"
#include "tinydav/codecs/msrp/tdav_codec_msrp.h"
#include "tinydav/codecs/amr/tdav_codec_amr.h"
#include "tinydav/codecs/bv/tdav_codec_bv16.h"
#include "tinydav/codecs/g711/tdav_codec_g711.h"
#include "tinydav/codecs/gsm/tdav_codec_gsm.h"
#include "tinydav/codecs/ilbc/tdav_codec_ilbc.h"
#include "tinydav/codecs/g729/tdav_codec_g729.h"
#include "tinydav/codecs/g722/tdav_codec_g722.h"
#include "tinydav/codecs/speex/tdav_codec_speex.h"
#include "tinydav/codecs/h261/tdav_codec_h261.h"
#include "tinydav/codecs/h263/tdav_codec_h263.h"
#include "tinydav/codecs/h264/tdav_codec_h264.h"
#include "tinydav/codecs/h264/tdav_codec_h264_cuda.h"
#include "tinydav/codecs/theora/tdav_codec_theora.h"
#include "tinydav/codecs/mp4ves/tdav_codec_mp4ves.h"
#include "tinydav/codecs/vpx/tdav_codec_vp8.h"

// Consumers
#include "tinydav/audio/waveapi/tdav_consumer_waveapi.h"
#include "tinydav/audio/directsound/tdav_consumer_dsound.h"
#include "tinydav/audio/coreaudio/tdav_consumer_audioqueue.h"
#include "tinydav/audio/coreaudio/tdav_consumer_audiounit.h"
#if HAVE_TINYDSHOW // DirectShow
#	include "tinydshow/plugin/DSConsumer.h"
#endif

// Producers
#include "tinydav/audio/waveapi/tdav_producer_waveapi.h"
#include "tinydav/audio/directsound/tdav_producer_dsound.h"
#include "tinydav/audio/coreaudio/tdav_producer_audioqueue.h"
#include "tinydav/audio/coreaudio/tdav_producer_audiounit.h"
#if HAVE_TINYDSHOW // DirectShow
#	include "tinydshow/plugin/DSProducer.h"
#endif

// Audio Denoise (AGC, Noise Suppression, VAD and AEC)
#if HAVE_SPEEX_DSP && (!defined(HAVE_SPEEX_DENOISE) || HAVE_SPEEX_DENOISE)
#	include "tinydav/audio/tdav_speex_denoise.h"
#endif
#if HAVE_WEBRTC && (!defined(HAVE_WEBRTC_DENOISE) || HAVE_WEBRTC_DENOISE)
#	include "tinydav/audio/tdav_webrtc_denoise.h"
#endif

// Audio resampler
#if HAVE_SPEEX_DSP && (!defined(HAVE_SPEEX_RESAMPLER) || HAVE_SPEEX_RESAMPLER)
#	include "tinydav/audio/tdav_speex_resampler.h"
#endif

// Audio/Video JitterBuffer
#if HAVE_SPEEX_DSP && HAVE_SPEEX_JB
#	include "tinydav/audio/tdav_speex_jitterbuffer.h"
#else 
#	include "tinydav/audio/tdav_speakup_jitterbuffer.h"
#endif

#if HAVE_FFMPEG
#	include <libavcodec/avcodec.h>
#endif

static inline tsk_bool_t _tdav_codec_is_supported(tdav_codec_id_t codec, const tmedia_codec_plugin_def_t* plugin);

int tdav_init()
{
	int ret = 0;
	
	/* === OS specific === */
#if TDAV_UNDER_WINDOWS
	if ((ret = tdav_win32_init())) {
		return ret;
	}
#elif TDAV_UNDER_APPLE
	if ((ret = tdav_apple_init())) {
		return ret;
	}
#endif
	
	/* === Initialize ffmpeg === */
#if HAVE_FFMPEG
#   if LIBAVCODEC_VERSION_MAJOR <= 53
	avcodec_init();
#   endif
#endif

	/* === SRTP === */

	/* === Register media contents === */
	tmedia_content_plugin_register("text/html", tmedia_content_dummy_plugin_def_t);
	tmedia_content_plugin_register("text/plain", tmedia_content_dummy_plugin_def_t);
	tmedia_content_plugin_register("application/octet-stream", tmedia_content_dummy_plugin_def_t);
	tmedia_content_plugin_register("message/CPIM", tmedia_content_cpim_plugin_def_t);
	/* To be implemented
	tmedia_content_plugin_register("message/sipfrag", tmedia_content_sipfrag_plugin_def_t);
	tmedia_content_plugin_register("multipart/digest", tmedia_content_multipart_plugin_def_t);
	tmedia_content_plugin_register("multipart/mixed", tmedia_content_multipart_plugin_def_t);
	tmedia_content_plugin_register("multipart/related", tmedia_content_multipart_plugin_def_t);
	tmedia_content_plugin_register("multipart/alternative", tmedia_content_multipart_plugin_def_t);
	tmedia_content_plugin_register("multipart/encrypted", tmedia_content_multipart_plugin_def_t);
	tmedia_content_plugin_register("multipart/parallel", tmedia_content_multipart_plugin_def_t);
	tmedia_content_plugin_register("multipart/signed", tmedia_content_multipart_plugin_def_t);
	*/

	/* === Register sessions === */
	tmedia_session_plugin_register(tmedia_session_ghost_plugin_def_t);
	tmedia_session_plugin_register(tdav_session_audio_plugin_def_t);
	tmedia_session_plugin_register(tdav_session_video_plugin_def_t);
	tmedia_session_plugin_register(tdav_session_msrp_plugin_def_t);	

	/* === Register codecs === */
#if HAVE_FFMPEG
	avcodec_register_all();
#endif
	
	tmedia_codec_plugin_register(tdav_codec_msrp_plugin_def_t);
	tmedia_codec_plugin_register(tdav_codec_g711a_plugin_def_t);
	tmedia_codec_plugin_register(tdav_codec_g711u_plugin_def_t);
	tmedia_codec_plugin_register(tdav_codec_g722_plugin_def_t);
#if HAVE_OPENCORE_AMR
	tmedia_codec_plugin_register(tdav_codec_amrnb_oa_plugin_def_t);
	tmedia_codec_plugin_register(tdav_codec_amrnb_be_plugin_def_t);
#endif
#if HAVE_BV16
	tmedia_codec_plugin_register(tdav_codec_bv16_plugin_def_t);
#endif
#if HAVE_LIBGSM
	tmedia_codec_plugin_register(tdav_codec_gsm_plugin_def_t);
#endif
#if HAVE_ILBC
	tmedia_codec_plugin_register(tdav_codec_ilbc_plugin_def_t);
#endif
#if HAVE_LIB_SPEEX
	tmedia_codec_plugin_register(tdav_codec_speex_nb_plugin_def_t);
	tmedia_codec_plugin_register(tdav_codec_speex_wb_plugin_def_t);
	tmedia_codec_plugin_register(tdav_codec_speex_uwb_plugin_def_t);
#endif
#if HAVE_G729
	tmedia_codec_plugin_register(tdav_codec_g729ab_plugin_def_t);
#endif
	// last: dtmf, ULPFEC and RED
	tmedia_codec_plugin_register(tdav_codec_dtmf_plugin_def_t);
	// tmedia_codec_plugin_register(tdav_codec_ulpfec_plugin_def_t);
	// tmedia_codec_plugin_register(tdav_codec_red_plugin_def_t);

#if HAVE_LIBVPX
	tmedia_codec_plugin_register(tdav_codec_vp8_plugin_def_t);
#endif
#if HAVE_CUDA
	if(tdav_codec_h264_cuda_is_supported()){
		tmedia_codec_plugin_register(tdav_codec_h264_cuda_bp10_plugin_def_t);
		tmedia_codec_plugin_register(tdav_codec_h264_cuda_bp20_plugin_def_t);
		tmedia_codec_plugin_register(tdav_codec_h264_cuda_bp30_plugin_def_t);
	}
#endif
#if HAVE_FFMPEG
	tmedia_codec_plugin_register(tdav_codec_mp4ves_plugin_def_t);
#	if !defined(HAVE_H264) || HAVE_H264
	tmedia_codec_plugin_register(tdav_codec_h264_base_plugin_def_t);
	tmedia_codec_plugin_register(tdav_codec_h264_main_plugin_def_t);
#	endif
	tmedia_codec_plugin_register(tdav_codec_h263p_plugin_def_t);
	tmedia_codec_plugin_register(tdav_codec_h263pp_plugin_def_t);
#	if !defined(HAVE_THEORA) || HAVE_THEORA
	tmedia_codec_plugin_register(tdav_codec_theora_plugin_def_t);
#	endif
	tmedia_codec_plugin_register(tdav_codec_h263_plugin_def_t);
	tmedia_codec_plugin_register(tdav_codec_h261_plugin_def_t);
#endif
	
	/* === Register converters === */
#if HAVE_LIBYUV
	tmedia_converter_video_plugin_register(tdav_converter_video_libyuv_plugin_def_t);
#elif HAVE_FFMPEG || HAVE_SWSSCALE
	tmedia_converter_video_plugin_register(tdav_converter_video_ffmpeg_plugin_def_t);
#endif

	/* === Register consumers === */
#if HAVE_DSOUND_H
	tmedia_consumer_plugin_register(tdav_consumer_dsound_plugin_def_t);
#elif HAVE_WAVE_API
	tmedia_consumer_plugin_register(tdav_consumer_waveapi_plugin_def_t);
#endif
#if HAVE_TINYDSHOW // DirectShow
	tmedia_consumer_plugin_register(tdshow_consumer_plugin_def_t);
#endif

#if HAVE_COREAUDIO_AUDIO_UNIT // CoreAudio based on AudioUnit
	tmedia_consumer_plugin_register(tdav_consumer_audiounit_plugin_def_t);
#elif HAVE_COREAUDIO_AUDIO_QUEUE // CoreAudio based on AudioQueue
	tmedia_consumer_plugin_register(tdav_consumer_audioqueue_plugin_def_t);
#endif

#if HAVE_OSS_H
	tmedia_consumer_plugin_register(tmedia_consumer_oss_plugin_def_t);
#endif

	/* === Register producers === */
#if HAVE_DSOUND_H // DirectSound
	tmedia_producer_plugin_register(tdav_producer_dsound_plugin_def_t);
#elif HAVE_WAVE_API // WaveAPI
	tmedia_producer_plugin_register(tdav_producer_waveapi_plugin_def_t);
#endif
#if HAVE_TINYDSHOW // DirectShow
	tmedia_producer_plugin_register(tdshow_producer_plugin_def_t);
#endif
	
#if HAVE_COREAUDIO_AUDIO_UNIT // CoreAudio based on AudioUnit
	tmedia_producer_plugin_register(tdav_producer_audiounit_plugin_def_t);
#elif HAVE_COREAUDIO_AUDIO_QUEUE // CoreAudio based on AudioQueue
	tmedia_producer_plugin_register(tdav_producer_audioqueue_plugin_def_t);
#endif

	/* === Register Audio Denoise (AGC, VAD, Noise Suppression and AEC) === */
#if HAVE_SPEEX_DSP && (!defined(HAVE_SPEEX_DENOISE) || HAVE_SPEEX_DENOISE)
	tmedia_denoise_plugin_register(tdav_speex_denoise_plugin_def_t);
#endif
#if HAVE_WEBRTC && (!defined(HAVE_WEBRTC_DENOISE) || HAVE_WEBRTC_DENOISE)
	tmedia_denoise_plugin_register(tdav_webrtc_denoise_plugin_def_t);
#endif

	/* === Register Audio Resampler === */
#if HAVE_SPEEX_DSP && (!defined(HAVE_SPEEX_RESAMPLER) || HAVE_SPEEX_RESAMPLER)
	tmedia_resampler_plugin_register(tdav_speex_resampler_plugin_def_t);
#endif

	/* === Register Audio/video JitterBuffer === */
#if HAVE_SPEEX_DSP && HAVE_SPEEX_JB
	tmedia_jitterbuffer_plugin_register(tdav_speex_jitterbuffer_plugin_def_t);
#else
	tmedia_jitterbuffer_plugin_register(tdav_speakup_jitterbuffer_plugin_def_t);
#endif

	return ret;
}

typedef struct tdav_codec_decl_s{
	tdav_codec_id_t id;
	const tmedia_codec_plugin_def_t** plugin;
} tdav_codec_decl_t;

static tdav_codec_decl_t __codecs[] = {
#if HAVE_OPENCORE_AMR
	{ tdav_codec_id_amr_nb_oa, &tdav_codec_amrnb_oa_plugin_def_t },
	{ tdav_codec_id_amr_nb_be, &tdav_codec_amrnb_be_plugin_def_t },
#endif
#if HAVE_BV16
	{ tdav_codec_id_bv16, &tdav_codec_bv16_plugin_def_t },
#endif
#if HAVE_LIBGSM
	{ tdav_codec_id_gsm, &tdav_codec_gsm_plugin_def_t },
#endif
	{ tdav_codec_id_pcma, &tdav_codec_g711a_plugin_def_t },
	{ tdav_codec_id_pcmu, &tdav_codec_g711u_plugin_def_t },
	{ tdav_codec_id_g722, &tdav_codec_g722_plugin_def_t },
#if HAVE_ILBC
	{ tdav_codec_id_ilbc, &tdav_codec_ilbc_plugin_def_t },
#endif
#if HAVE_LIB_SPEEX
	{ tdav_codec_id_speex_nb, &tdav_codec_speex_nb_plugin_def_t },
	{ tdav_codec_id_speex_wb, &tdav_codec_speex_wb_plugin_def_t },
	{ tdav_codec_id_speex_uwb, &tdav_codec_speex_uwb_plugin_def_t },
#endif
#if HAVE_G729
	{ tdav_codec_id_g729ab, &tdav_codec_g729ab_plugin_def_t },
#endif
	
#if HAVE_LIBVPX
	{ tdav_codec_id_vp8, &tdav_codec_vp8_plugin_def_t },
#endif

#if HAVE_CUDA
	// tdav_codec_h264_cuda_is_supported() will be used to check availability at runtime
	{ tdav_codec_id_h264_bp30, &tdav_codec_h264_cuda_bp30_plugin_def_t },
	{ tdav_codec_id_h264_bp20, &tdav_codec_h264_cuda_bp20_plugin_def_t },
	{ tdav_codec_id_h264_bp10, &tdav_codec_h264_cuda_bp10_plugin_def_t },
#endif
#if HAVE_FFMPEG
#	if (!defined(HAVE_H264) || HAVE_H264) || HAVE_CUDA
	{ tdav_codec_id_h264_bp, &tdav_codec_h264_base_plugin_def_t },
	{ tdav_codec_id_h264_mp, &tdav_codec_h264_main_plugin_def_t },	
#	endif
	{ tdav_codec_id_mp4ves_es, &tdav_codec_mp4ves_plugin_def_t },
	{ tdav_codec_id_h263p, &tdav_codec_h263p_plugin_def_t },
	{ tdav_codec_id_h263pp, &tdav_codec_h263pp_plugin_def_t },
#	if !defined(HAVE_THEORA) || HAVE_THEORA
	{ tdav_codec_id_theora, &tdav_codec_theora_plugin_def_t },
#	endif
	{ tdav_codec_id_h263, &tdav_codec_h263_plugin_def_t },
	{ tdav_codec_id_h261, &tdav_codec_h261_plugin_def_t },
#endif
};

int tdav_codec_set_priority(tdav_codec_id_t codec_id, int priority)
{
	static int count = sizeof(__codecs)/sizeof(tdav_codec_decl_t);
	int i;
	
	if(priority < 0){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	for(i = 0; i<count; ++i){
		if(__codecs[i].id == codec_id){
			tdav_codec_decl_t codec_decl_1, codec_decl_2;
			priority = TSK_MIN(priority, count-1);
			codec_decl_1 = __codecs[priority];
			codec_decl_2 = __codecs[i];
			
			__codecs[i] = codec_decl_1;
			__codecs[priority] = codec_decl_2;

			// change priority if already registered and supported
			if(_tdav_codec_is_supported(codec_decl_2.id, *codec_decl_2.plugin) && tmedia_codec_plugin_is_registered(*codec_decl_2.plugin)){
				return tmedia_codec_plugin_register_2(*codec_decl_2.plugin, priority);
			}
			return 0;
		}
	}
	
	TSK_DEBUG_ERROR("cannot find codec with id=%d", codec_id);
	return -2;
}

void tdav_set_codecs(tdav_codec_id_t codecs)
{
	int i;
	int prio;

	for(i=0,prio=0; i<sizeof(__codecs)/sizeof(tdav_codec_decl_t); ++i){
		if((codecs & __codecs[i].id)){
			if(_tdav_codec_is_supported(__codecs[i].id, *(__codecs[i].plugin))){
				tmedia_codec_plugin_register_2(*(__codecs[i].plugin), prio++);
			}
		}
		else{
			tmedia_codec_plugin_unregister(*(__codecs[i].plugin));
		}
	}
}

tsk_bool_t _tdav_codec_is_supported(tdav_codec_id_t codec, const tmedia_codec_plugin_def_t* plugin)
{
	switch(codec){

		case tdav_codec_id_amr_nb_oa:
		case tdav_codec_id_amr_nb_be:
#if HAVE_OPENCORE_AMR
			return tsk_true;
#else
			return tsk_false;
#endif
		
		case tdav_codec_id_gsm:
#if HAVE_LIBGSM
			return tsk_true;
#else
			return tsk_false;
#endif

		case tdav_codec_id_pcma:
		case tdav_codec_id_pcmu:
		case tdav_codec_id_g722:
			return tsk_true;

		case tdav_codec_id_ilbc:
#if HAVE_ILBC
			return tsk_true;
#else
			return tsk_false;
#endif

		case tdav_codec_id_speex_nb:
		case tdav_codec_id_speex_wb:
		case tdav_codec_id_speex_uwb:
#if HAVE_LIB_SPEEX
			return tsk_true;
#else
			return tsk_false;
#endif
		
		case tdav_codec_id_bv16:
#if HAVE_BV16
			return tsk_true;
#else
			return tsk_false;
#endif
		
		case tdav_codec_id_g729ab:
#if HAVE_G729
			return tsk_true;
#else
			return tsk_false;
#endif
		
		case tdav_codec_id_vp8:
#if HAVE_LIBVPX
			return tsk_true;
#else
			return tsk_false;
#endif

		case tdav_codec_id_h261:
		case tdav_codec_id_h263:
		case tdav_codec_id_h263p:
		case tdav_codec_id_h263pp:
		case tdav_codec_id_mp4ves_es:
#if HAVE_FFMPEG
			return tsk_true;
#else
			return tsk_false;
#endif
		
		case tdav_codec_id_theora:
#if HAVE_FFMPEG && (!defined(HAVE_THEORA) || HAVE_THEORA)
			return tsk_true;
#else
			return tsk_false;
#endif

		case tdav_codec_id_h264_bp:
		case tdav_codec_id_h264_mp:
			{
				if(plugin){
#if HAVE_CUDA
					if(tdav_codec_h264_is_cuda_plugin(plugin) && tdav_codec_h264_cuda_is_supported()) return tsk_true;
#endif
#if HAVE_FFMPEG && (!defined(HAVE_H264) || HAVE_H264)
					if(tdav_codec_h264_is_ffmpeg_plugin(plugin)) return tsk_true;
#endif
				}
				else{
#if HAVE_CUDA
				if(tdav_codec_h264_cuda_is_supported()) return tsk_true;
#endif
#if HAVE_FFMPEG && (!defined(HAVE_H264) || HAVE_H264)
					return tsk_true;
#endif
				}
				return tsk_false;
			}

		case tdav_codec_id_amr_wb_oa:
		case tdav_codec_id_amr_wb_be:
		case tdav_codec_id_bv32:
		case tdav_codec_id_evrc:
		default:
			return tsk_false;
	}
}

tsk_bool_t tdav_codec_is_supported(tdav_codec_id_t codec)
{
	return _tdav_codec_is_supported(codec, tsk_null);
}

int tdav_deinit()
{
	int ret = 0;
	
	/* === OS specific === */
#if TDAV_UNDER_WINDOWS
	if ((ret = tdav_win32_deinit())) {
		return ret;
	}
#elif TDAV_UNDER_APPLE
	if ((ret = tdav_apple_deinit())) {
		return ret;
	}
#endif
	
	/* === UnRegister media contents === */
	tmedia_content_plugin_unregister_all();

	/* === UnRegister sessions === */
	tmedia_session_plugin_unregister(tmedia_session_ghost_plugin_def_t);
	tmedia_session_plugin_unregister(tdav_session_audio_plugin_def_t);
	tmedia_session_plugin_unregister(tdav_session_video_plugin_def_t);
	tmedia_session_plugin_unregister(tdav_session_msrp_plugin_def_t);

	/* === UnRegister codecs === */
	tmedia_codec_plugin_unregister(tdav_codec_dtmf_plugin_def_t);
	tmedia_codec_plugin_unregister(tdav_codec_ulpfec_plugin_def_t);
	tmedia_codec_plugin_unregister(tdav_codec_red_plugin_def_t);
	tmedia_codec_plugin_unregister(tdav_codec_msrp_plugin_def_t);
	tmedia_codec_plugin_unregister(tdav_codec_g711a_plugin_def_t);
	tmedia_codec_plugin_unregister(tdav_codec_g711u_plugin_def_t);
	tmedia_codec_plugin_unregister(tdav_codec_g722_plugin_def_t);
#if HAVE_OPENCORE_AMR
	tmedia_codec_plugin_unregister(tdav_codec_amrnb_oa_plugin_def_t);
	tmedia_codec_plugin_unregister(tdav_codec_amrnb_be_plugin_def_t);
#endif
#if HAVE_BV16
	tmedia_codec_plugin_unregister(tdav_codec_bv16_plugin_def_t);
#endif
#if HAVE_LIBGSM
	tmedia_codec_plugin_unregister(tdav_codec_gsm_plugin_def_t);
#endif
#if HAVE_ILBC
	tmedia_codec_plugin_unregister(tdav_codec_ilbc_plugin_def_t);
#endif
#if HAVE_LIB_SPEEX
	tmedia_codec_plugin_unregister(tdav_codec_speex_nb_plugin_def_t);
	tmedia_codec_plugin_unregister(tdav_codec_speex_wb_plugin_def_t);
	tmedia_codec_plugin_unregister(tdav_codec_speex_uwb_plugin_def_t);
#endif
#if HAVE_G729
	tmedia_codec_plugin_unregister(tdav_codec_g729ab_plugin_def_t);
#endif

#if HAVE_LIBVPX
	tmedia_codec_plugin_unregister(tdav_codec_vp8_plugin_def_t);
#endif
#if HAVE_CUDA
	if(tdav_codec_h264_cuda_is_supported()){
		tmedia_codec_plugin_unregister(tdav_codec_h264_cuda_bp10_plugin_def_t);
		tmedia_codec_plugin_unregister(tdav_codec_h264_cuda_bp20_plugin_def_t);
		tmedia_codec_plugin_unregister(tdav_codec_h264_cuda_bp30_plugin_def_t);
	}
#endif
#if HAVE_FFMPEG
	tmedia_codec_plugin_unregister(tdav_codec_mp4ves_plugin_def_t);
	tmedia_codec_plugin_unregister(tdav_codec_h261_plugin_def_t);
	tmedia_codec_plugin_unregister(tdav_codec_h263_plugin_def_t);
	tmedia_codec_plugin_unregister(tdav_codec_h263p_plugin_def_t);
	tmedia_codec_plugin_unregister(tdav_codec_h263pp_plugin_def_t);
#	if  !defined(HAVE_H264) || HAVE_H264
	tmedia_codec_plugin_unregister(tdav_codec_h264_base_plugin_def_t);
	tmedia_codec_plugin_unregister(tdav_codec_h264_main_plugin_def_t);
#	endif
#	if !defined(HAVE_THEORA) || HAVE_THEORA
	tmedia_codec_plugin_unregister(tdav_codec_theora_plugin_def_t);
#	endif

#endif

	/* === unRegister converters === */
#if HAVE_LIBYUV
	tmedia_converter_video_plugin_unregister(tdav_converter_video_libyuv_plugin_def_t);
#elif HAVE_FFMPEG || HAVE_SWSSCALE
	tmedia_converter_video_plugin_unregister(tdav_converter_video_ffmpeg_plugin_def_t);
#endif

	/* === unRegister consumers === */
#if HAVE_DSOUND_H
	tmedia_consumer_plugin_unregister(tdav_consumer_dsound_plugin_def_t);
#elif HAVE_WAVE_API
	tmedia_consumer_plugin_unregister(tdav_consumer_waveapi_plugin_def_t);
#endif
#if HAVE_TINYDSHOW // DirectShow
	tmedia_consumer_plugin_unregister(tdshow_consumer_plugin_def_t);
#endif
#if HAVE_COREAUDIO_AUDIO_UNIT // CoreAudio based on AudioUnit
	tmedia_consumer_plugin_unregister(tdav_consumer_audiounit_plugin_def_t);
#elif HAVE_COREAUDIO_AUDIO_QUEUE // CoreAudio based on AudioQueue
	tmedia_consumer_plugin_unregister(tdav_consumer_audioqueue_plugin_def_t);
#endif

	/* === UnRegister producers === */
#if HAVE_DSOUND_H // DirectSound
	tmedia_producer_plugin_unregister(tdav_producer_dsound_plugin_def_t);
#elif HAVE_WAVE_API // WaveAPI
	tmedia_producer_plugin_unregister(tdav_producer_waveapi_plugin_def_t);
#endif
#if HAVE_TINYDSHOW // DirectShow
	tmedia_producer_plugin_unregister(tdshow_producer_plugin_def_t);
#endif

#if HAVE_COREAUDIO_AUDIO_UNIT // CoreAudio based on AudioUnit
	tmedia_producer_plugin_unregister(tdav_producer_audiounit_plugin_def_t);
#elif HAVE_COREAUDIO_AUDIO_QUEUE // CoreAudio based on AudioQueue
	tmedia_producer_plugin_unregister(tdav_producer_audioqueue_plugin_def_t);
#endif

#if HAVE_OSS_H
	tmedia_consumer_plugin_unregister(tmedia_consumer_oss_plugin_def_t);
#endif

	/* === UnRegister Audio Denoise (AGC, VAD, Noise Suppression and AEC) === */
#if HAVE_SPEEX_DSP && (!defined(HAVE_SPEEX_DENOISE) || HAVE_SPEEX_DENOISE)
	tmedia_denoise_plugin_unregister(tdav_speex_denoise_plugin_def_t);
#endif
#if HAVE_WEBRTC && (!defined(HAVE_WEBRTC_DENOISE) || HAVE_WEBRTC_DENOISE)
	tmedia_denoise_plugin_unregister(tdav_webrtc_denoise_plugin_def_t);
#endif

	/* === UnRegister Audio Resampler === */
#if HAVE_SPEEX_DSP && (!defined(HAVE_SPEEX_RESAMPLER) || HAVE_SPEEX_RESAMPLER)
	tmedia_resampler_plugin_unregister(tdav_speex_resampler_plugin_def_t);
#endif

	/* === UnRegister Audio/video JitterBuffer === */
#if HAVE_SPEEX_DSP && HAVE_SPEEX_JB
	tmedia_jitterbuffer_plugin_unregister(tdav_speex_jitterbuffer_plugin_def_t);
#else
	tmedia_jitterbuffer_plugin_unregister(tdav_speakup_jitterbuffer_plugin_def_t);
#endif

	return ret;
}