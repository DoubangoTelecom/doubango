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

static tsk_bool_t __b_initialized = tsk_false;
static tsk_bool_t __b_ipsec_supported = tsk_false;
static const struct tmedia_codec_plugin_def_s* __codec_plugins_all[0xFF] = { tsk_null }; // list of all codecs BEFORE filtering
static const tsk_size_t __codec_plugins_all_count = sizeof(__codec_plugins_all)/sizeof(__codec_plugins_all[0]);

#if TDAV_UNDER_WINDOWS
#	include "tinydav/tdav_win32.h"
#elif TDAV_UNDER_APPLE
#	include "tinydav/tdav_apple.h"
#endif

// Shared libraries not allowed on WP8 and iOS
#if !TDAV_UNDER_WINDOWS_PHONE && !TDAV_UNDER_IPHONE
#include "tsk_plugin.h"
#	if TDAV_UNDER_WINDOWS
#		define TDAV_HAVE_PLUGIN_EXT_WIN32	1
		static struct tsk_plugin_s* __dll_plugin_wasapi = tsk_null; /* Windows Audio Session API (WASAPI): Windows [Vista - 8] */
		static struct tsk_plugin_s* __dll_plugin_dshow = tsk_null; /* DirectShow: Windows [XP - 8] */
		static struct tsk_plugin_s* __dll_plugin_mf = tsk_null; /* Media Foundation and WASAPI : Windows [7 - 8] */
		static struct tsk_plugin_s* __dll_plugin_cuda = tsk_null; /* Media Foundation and WASAPI : Windows [XP - 8] */
		static struct tsk_plugin_s* __dll_plugin_audio_dsp = tsk_null; /* Audio DSP, Resampler, AEC, NS, AGC...: Windows [Vista - 8] */
		static struct tsk_plugin_s* __dll_plugin_ipsec_wfp = tsk_null; /* IPSec implementation using WFP (Windows Filtering platform): Windows [Vista - 8] */
#	endif /* TDAV_UNDER_WINDOWS */
#endif

// Media Contents, plugins defintion...
#include "tinymedia.h"

// IPSec
#include "tipsec.h"

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
#include "tinydav/bfcp/tdav_session_bfcp.h"
#include "tinydav/t140/tdav_session_t140.h"

// Codecs
#include "tinydav/codecs/dtmf/tdav_codec_dtmf.h"
#include "tinydav/codecs/t140/tdav_codec_t140.h"
#include "tinydav/codecs/fec/tdav_codec_ulpfec.h"
#include "tinydav/codecs/fec/tdav_codec_red.h"
#include "tinydav/codecs/msrp/tdav_codec_msrp.h"
#include "tinydav/codecs/bfcp/tdav_codec_bfcp.h"
#include "tinydav/codecs/amr/tdav_codec_amr.h"
#include "tinydav/codecs/bv/tdav_codec_bv16.h"
#include "tinydav/codecs/g711/tdav_codec_g711.h"
#include "tinydav/codecs/gsm/tdav_codec_gsm.h"
#include "tinydav/codecs/ilbc/tdav_codec_ilbc.h"
#include "tinydav/codecs/g729/tdav_codec_g729.h"
#include "tinydav/codecs/g722/tdav_codec_g722.h"
#include "tinydav/codecs/speex/tdav_codec_speex.h"
#include "tinydav/codecs/opus/tdav_codec_opus.h"
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
#include "tinydav/audio/wasapi/tdav_consumer_wasapi.h"
#include "tinydav/video/winm/tdav_consumer_winm.h"
#include "tinydav/video/mf/tdav_consumer_video_mf.h"
#include "tinydav/t140/tdav_consumer_t140.h"

// Producers
#include "tinydav/audio/waveapi/tdav_producer_waveapi.h"
#include "tinydav/audio/directsound/tdav_producer_dsound.h"
#include "tinydav/video/gdi/tdav_producer_screencast_gdi.h"
#include "tinydav/audio/coreaudio/tdav_producer_audioqueue.h"
#include "tinydav/audio/coreaudio/tdav_producer_audiounit.h"
#include "tinydav/audio/wasapi/tdav_producer_wasapi.h"
#include "tinydav/video/winm/tdav_producer_winm.h"
#include "tinydav/video/mf/tdav_producer_video_mf.h"
#include "tinydav/t140/tdav_producer_t140.h"

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

static inline int _tdav_codec_plugins_collect();
static inline int _tdav_codec_plugins_disperse();
static inline tsk_bool_t _tdav_codec_is_supported(tdav_codec_id_t codec, const tmedia_codec_plugin_def_t* plugin);

int tdav_init()
{
	int ret = 0;

	if(__b_initialized){
		TSK_DEBUG_INFO("TINYDAV already initialized");
		return 0;
	}
	
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

		/* === stand-alone plugins === */
#if TDAV_HAVE_PLUGIN_EXT_WIN32
	{
		tsk_size_t plugins_count = 0;
		char* full_path = tsk_null; // Loading plugins from ActiveX fails when using relative path.
		/* WASAPI (Audio consumer, Audio producer) */
#if 0 // disable WASAPI by default (AEC issue because of code#consumer rate mismatch)
		if(tdav_win32_is_winvista_or_later()){
			tsk_sprintf(&full_path, "%s/pluginWASAPI.dll", tdav_get_current_directory_const());
			if(tsk_plugin_file_exist(full_path) && (__dll_plugin_wasapi = tsk_plugin_create(full_path))){
				plugins_count += tmedia_plugin_register(__dll_plugin_wasapi, tsk_plugin_def_type_all, tsk_plugin_def_media_type_all);
			}
		}
#endif
		/* CUDA (H.264 codec) */
#if 1 // Enable CUDA by default
		tsk_sprintf(&full_path, "%s/pluginCUDA.dll", tdav_get_current_directory_const()); // CUDA works on all Windows versions
		if(tsk_plugin_file_exist(full_path) && (__dll_plugin_cuda = tsk_plugin_create(full_path))){
			plugins_count += tmedia_plugin_register(__dll_plugin_cuda, tsk_plugin_def_type_all, tsk_plugin_def_media_type_all);
		}
#endif
		/* Media Foundation (Video converter, Video consumer, Video producer, Microsoft H.264 codec, Intel Quick Sync H.264 codec) */
		if(tdav_win32_is_win7_or_later()){
			tsk_sprintf(&full_path, "%s/pluginWinMF.dll", tdav_get_current_directory_const());
			if(tsk_plugin_file_exist(full_path) && (__dll_plugin_mf = tsk_plugin_create(full_path))){
				plugins_count += tmedia_plugin_register(__dll_plugin_mf, tsk_plugin_def_type_all, tsk_plugin_def_media_type_all);
			}
		}
		/* DirectShow (Video consumer, Video producer) */
		if(tdav_win32_is_winxp_or_later()){
			tsk_sprintf(&full_path, "%s/pluginDirectShow.dll", tdav_get_current_directory_const());
			if(tsk_plugin_file_exist(full_path) && (__dll_plugin_dshow = tsk_plugin_create(full_path))){
				plugins_count += tmedia_plugin_register(__dll_plugin_dshow, tsk_plugin_def_type_all, tsk_plugin_def_media_type_all);
			}
		}
		/* Audio DSP (Resampler, AEC, NS, AGC...) */
		if(tdav_win32_is_winvista_or_later()){
			tsk_sprintf(&full_path, "%s/pluginWinAudioDSP.dll", tdav_get_current_directory_const());
			if(tsk_plugin_file_exist(full_path) && (__dll_plugin_audio_dsp = tsk_plugin_create(full_path))){
				plugins_count += tmedia_plugin_register(__dll_plugin_audio_dsp, tsk_plugin_def_type_all, tsk_plugin_def_media_type_all);
			}
		}
		/* IPSec implementation using Windows Filtering Platform (WFP) */
		if(tdav_win32_is_winvista_or_later()){
			tsk_sprintf(&full_path, "%s/pluginWinIPSecVista.dll", tdav_get_current_directory_const());
			if(tsk_plugin_file_exist(full_path) && (tipsec_plugin_register_file(full_path, &__dll_plugin_ipsec_wfp) == 0)){
				plugins_count += 1; // at least one
				__b_ipsec_supported = tsk_true;
			}
		}

		TSK_FREE(full_path);
		TSK_DEBUG_INFO("Windows stand-alone plugins loaded = %u", plugins_count);
	}
#endif

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
	tmedia_session_plugin_register(tdav_session_t140_plugin_def_t);
	tmedia_session_plugin_register(tdav_session_bfcp_plugin_def_t);
	tmedia_session_plugin_register(tdav_session_bfcpaudio_plugin_def_t);
	tmedia_session_plugin_register(tdav_session_bfcpvideo_plugin_def_t);

	/* === Register codecs === */
#if HAVE_FFMPEG
	avcodec_register_all();
#endif
	
	tmedia_codec_plugin_register(tdav_codec_msrp_plugin_def_t);
	tmedia_codec_plugin_register(tdav_codec_t140_plugin_def_t);
	tmedia_codec_plugin_register(tdav_codec_bfcp_plugin_def_t);
	tmedia_codec_plugin_register(tdav_codec_red_plugin_def_t);
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
#if HAVE_LIBOPUS
	tmedia_codec_plugin_register(tdav_codec_opus_plugin_def_t);
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
	#error "Support for H.264 Cuda is deprecated"
	if(tdav_codec_h264_cuda_is_supported()){
		tmedia_codec_plugin_register(tdav_codec_h264_cuda_bp10_plugin_def_t);
		tmedia_codec_plugin_register(tdav_codec_h264_cuda_bp20_plugin_def_t);
		tmedia_codec_plugin_register(tdav_codec_h264_cuda_bp30_plugin_def_t);
	}
#endif
#if HAVE_FFMPEG
	if(tdav_codec_ffmpeg_mp4ves_is_supported()){
		tmedia_codec_plugin_register(tdav_codec_mp4ves_plugin_def_t);
	}
	if(tdav_codec_ffmpeg_h264_is_supported()){
		if(!tmedia_codec_plugin_is_registered_2(tmedia_codec_id_h264_bp)) { // could be already registered by stand alone plugins (e.g. pluginWinMF.DLL)
			tmedia_codec_plugin_register(tdav_codec_h264_base_plugin_def_t);
		}
		if(!tmedia_codec_plugin_is_registered_2(tmedia_codec_id_h264_mp)) { // could be already registered by stand alone plugins (e.g. pluginWinMF.DLL)
			tmedia_codec_plugin_register(tdav_codec_h264_main_plugin_def_t);
		}
	}
	tmedia_codec_plugin_register(tdav_codec_h263p_plugin_def_t);
	tmedia_codec_plugin_register(tdav_codec_h263pp_plugin_def_t);
	if(tdav_codec_ffmpeg_theora_is_supported()){
		tmedia_codec_plugin_register(tdav_codec_theora_plugin_def_t);
	}
	tmedia_codec_plugin_register(tdav_codec_h263_plugin_def_t);
	tmedia_codec_plugin_register(tdav_codec_h261_plugin_def_t);
#elif HAVE_H264_PASSTHROUGH
	tmedia_codec_plugin_register(tdav_codec_h264_base_plugin_def_t);
	tmedia_codec_plugin_register(tdav_codec_h264_main_plugin_def_t);
#endif

	
	/* === Register converters === */
	// register several convertors and try them all (e.g. LIBYUV only support to/from I420)
#if HAVE_LIBYUV
	tmedia_converter_video_plugin_register(tdav_converter_video_libyuv_plugin_def_t);
#endif
#if HAVE_FFMPEG || HAVE_SWSSCALE
	tmedia_converter_video_plugin_register(tdav_converter_video_ffmpeg_plugin_def_t);
#endif

	/* === Register consumers === */
	tmedia_consumer_plugin_register(tdav_consumer_t140_plugin_def_t); /* T140 */
#if HAVE_DSOUND_H
	tmedia_consumer_plugin_register(tdav_consumer_dsound_plugin_def_t);
#elif HAVE_WAVE_API
	tmedia_consumer_plugin_register(tdav_consumer_waveapi_plugin_def_t);
#elif HAVE_WASAPI
	tmedia_consumer_plugin_register(tdav_consumer_wasapi_plugin_def_t);
#endif
#if HAVE_WINM // Windows Media (WP8)
	tmedia_consumer_plugin_register(tdav_consumer_winm_plugin_def_t);
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
	tmedia_producer_plugin_register(tdav_producer_t140_plugin_def_t); /* T140 */
#if HAVE_DSOUND_H // DirectSound
	tmedia_producer_plugin_register(tdav_producer_dsound_plugin_def_t);
#elif HAVE_WAVE_API // WaveAPI
	tmedia_producer_plugin_register(tdav_producer_waveapi_plugin_def_t);
#elif HAVE_WASAPI // WASAPI
	tmedia_producer_plugin_register(tdav_producer_wasapi_plugin_def_t);
#endif
#if TDAV_UNDER_WINDOWS && !TDAV_UNDER_WINDOWS_RT // Windows GDI
	tmedia_producer_plugin_register(tdav_producer_screencast_gdi_plugin_def_t);
#endif
#if HAVE_WINM // Windows Media (WP8)
	tmedia_producer_plugin_register(tdav_producer_winm_plugin_def_t);
#endif
	
#if HAVE_COREAUDIO_AUDIO_UNIT // CoreAudio based on AudioUnit
	tmedia_producer_plugin_register(tdav_producer_audiounit_plugin_def_t);
#elif HAVE_COREAUDIO_AUDIO_QUEUE // CoreAudio based on AudioQueue
	tmedia_producer_plugin_register(tdav_producer_audioqueue_plugin_def_t);
#endif

	/* === Register Audio Denoise (AGC, VAD, Noise Suppression and AEC) === */
#if HAVE_WEBRTC && (!defined(HAVE_WEBRTC_DENOISE) || HAVE_WEBRTC_DENOISE)
	tmedia_denoise_plugin_register(tdav_webrtc_denoise_plugin_def_t);
#endif
#if HAVE_SPEEX_DSP && (!defined(HAVE_SPEEX_DENOISE) || HAVE_SPEEX_DENOISE)
	tmedia_denoise_plugin_register(tdav_speex_denoise_plugin_def_t);
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

	// collect all codecs before filtering
	_tdav_codec_plugins_collect();

	__b_initialized = tsk_true;

	return ret;
}

int tdav_codec_set_priority(tdav_codec_id_t codec_id, int priority)
{
	tsk_size_t i;
	
	if(priority < 0){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	for(i = 0; i < __codec_plugins_all_count && __codec_plugins_all[i]; ++i){
		if(__codec_plugins_all[i]->codec_id == codec_id){
			const struct tmedia_codec_plugin_def_s *codec_decl_1, *codec_decl_2;
			priority = TSK_MIN(priority, (int)__codec_plugins_all_count-1);
			codec_decl_1 = __codec_plugins_all[priority];
			codec_decl_2 = __codec_plugins_all[i];
			
			__codec_plugins_all[i] = codec_decl_1;
			__codec_plugins_all[priority] = codec_decl_2;

			// change priority if already registered and supported
			if(_tdav_codec_is_supported((tdav_codec_id_t)codec_decl_2->codec_id, codec_decl_2) && tmedia_codec_plugin_is_registered(codec_decl_2)){
				return tmedia_codec_plugin_register_2(codec_decl_2, priority);
			}
			return 0;
		}
	}
	
	TSK_DEBUG_INFO("Cannot find codec with id=%d for priority setting", codec_id);
	return 0;
}

int tdav_set_codecs(tdav_codec_id_t codecs)
{
	tsk_size_t i, prio;

	// unregister all codecs
	tmedia_codec_plugin_unregister_all();
	// register "selected" and "fake" codecs. "fake" codecs have "none" as id (e.g. MSRP or DTMF)
	for(i=0,prio=0; i<__codec_plugins_all_count && __codec_plugins_all[i]; ++i){
		if((codecs & __codec_plugins_all[i]->codec_id) || __codec_plugins_all[i]->codec_id == tmedia_codec_id_none){
			if(_tdav_codec_is_supported((tdav_codec_id_t)__codec_plugins_all[i]->codec_id, __codec_plugins_all[i])){
				tmedia_codec_plugin_register_2(__codec_plugins_all[i], prio++);
			}
		}
	}
	return 0;
}

static inline int _tdav_codec_plugins_collect()
{
	const struct tmedia_codec_plugin_def_s** plugins = tsk_null;
	tsk_size_t i, count;
	int ret;
	static const tsk_size_t __codec_plugins_all_count = sizeof(__codec_plugins_all)/sizeof(__codec_plugins_all[0]);

	ret = _tdav_codec_plugins_disperse();
	if((ret = tmedia_codec_plugin_registered_get_all(&plugins, &count)) == 0) {
		for(i = 0; i < count && i < __codec_plugins_all_count; ++i) {
			__codec_plugins_all[i] = plugins[i];
		}
	}
	return 0;
}

static inline int _tdav_codec_plugins_disperse()
{
	memset((void*)__codec_plugins_all, 0, sizeof(__codec_plugins_all));
	return 0;
}


/*
 Must be called after tdav_init()
*/
static inline tsk_bool_t _tdav_codec_is_supported(tdav_codec_id_t codec, const tmedia_codec_plugin_def_t* plugin)
{
	tsk_size_t i;
	for(i = 0; i < __codec_plugins_all_count && __codec_plugins_all[i]; ++i) {
		if((plugin && __codec_plugins_all[i] == plugin) || __codec_plugins_all[i]->codec_id == codec) {
			return tsk_true;
		}
	}
	return tsk_false;
}

/**
* Checks whether a codec is supported. Being supported doesn't mean it's enabled and ready for use.
* @return @ref tsk_true if supported and @tsk_false otherwise.
* @sa @ref tdav_codec_is_enabled()
*/
tsk_bool_t tdav_codec_is_supported(tdav_codec_id_t codec)
{
	return _tdav_codec_is_supported(codec, tsk_null);
}

/**
* Checks whether a codec is enabled.
* @return @ref tsk_true if enabled and @tsk_false otherwise.
* @sa @ref tdav_codec_is_supported()
*/
tsk_bool_t tdav_codec_is_enabled(tdav_codec_id_t codec)
{
	return tmedia_codec_plugin_is_registered_2((tmedia_codec_id_t)codec);
}

/**
* Checks whether a IPSec is supported.
* @return @ref tsk_true if supported and @tsk_false otherwise.
*/
tsk_bool_t tdav_ipsec_is_supported()
{
	return __b_ipsec_supported;
}

int tdav_deinit()
{
	int ret = 0;

	if(!__b_initialized){
		TSK_DEBUG_INFO("TINYDAV not initialized");
		return 0;
	}
	
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
	tmedia_session_plugin_unregister(tdav_session_t140_plugin_def_t);
	tmedia_session_plugin_unregister(tdav_session_bfcp_plugin_def_t);
	tmedia_session_plugin_unregister(tdav_session_bfcpaudio_plugin_def_t);
	tmedia_session_plugin_unregister(tdav_session_bfcpvideo_plugin_def_t);

	/* === UnRegister codecs === */
	tmedia_codec_plugin_unregister_all();


	/* === unRegister converters === */
#if HAVE_LIBYUV
	tmedia_converter_video_plugin_unregister(tdav_converter_video_libyuv_plugin_def_t);
#endif
#if HAVE_FFMPEG || HAVE_SWSSCALE
	tmedia_converter_video_plugin_unregister(tdav_converter_video_ffmpeg_plugin_def_t);
#endif

	/* === unRegister consumers === */
	tmedia_consumer_plugin_unregister(tdav_consumer_t140_plugin_def_t); /* T140 */
#if HAVE_DSOUND_H
	tmedia_consumer_plugin_unregister(tdav_consumer_dsound_plugin_def_t);
#endif
#if HAVE_WAVE_API
	tmedia_consumer_plugin_unregister(tdav_consumer_waveapi_plugin_def_t);
#endif
#if HAVE_WASAPI
	tmedia_consumer_plugin_unregister(tdav_consumer_wasapi_plugin_def_t);
#endif
#if HAVE_WINM // Windows Media (WP8)
	tmedia_consumer_plugin_unregister(tdav_consumer_winm_plugin_def_t);
#endif
#if HAVE_COREAUDIO_AUDIO_UNIT // CoreAudio based on AudioUnit
	tmedia_consumer_plugin_unregister(tdav_consumer_audiounit_plugin_def_t);
#endif
#if HAVE_COREAUDIO_AUDIO_QUEUE // CoreAudio based on AudioQueue
	tmedia_consumer_plugin_unregister(tdav_consumer_audioqueue_plugin_def_t);
#endif

	/* === UnRegister producers === */
	tmedia_producer_plugin_unregister(tdav_producer_t140_plugin_def_t); /* T140 */
#if HAVE_DSOUND_H // DirectSound
	tmedia_producer_plugin_unregister(tdav_producer_dsound_plugin_def_t);
#endif
#if HAVE_WAVE_API // WaveAPI
	tmedia_producer_plugin_unregister(tdav_producer_waveapi_plugin_def_t);
#endif
#if HAVE_WASAPI // WASAPI
	tmedia_producer_plugin_unregister(tdav_producer_wasapi_plugin_def_t);
#endif
#if TDAV_UNDER_WINDOWS && !TDAV_UNDER_WINDOWS_RT // Windows GDI
	tmedia_producer_plugin_unregister(tdav_producer_screencast_gdi_plugin_def_t);
#endif
#if HAVE_WINM // Windows Media (WP8)
	tmedia_producer_plugin_unregister(tdav_producer_winm_plugin_def_t);
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

	/* === stand-alone plugins === */
#if TDAV_HAVE_PLUGIN_EXT_WIN32
	{
		TSK_OBJECT_SAFE_FREE(__dll_plugin_cuda);
		TSK_OBJECT_SAFE_FREE(__dll_plugin_wasapi);
		TSK_OBJECT_SAFE_FREE(__dll_plugin_mf);
		TSK_OBJECT_SAFE_FREE(__dll_plugin_dshow);
		TSK_OBJECT_SAFE_FREE(__dll_plugin_audio_dsp);
		TSK_OBJECT_SAFE_FREE(__dll_plugin_ipsec_wfp);
	}
#endif

	// disperse all collected codecs
	_tdav_codec_plugins_disperse();

	__b_initialized = tsk_false;

	return ret;
}