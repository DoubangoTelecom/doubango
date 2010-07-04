/*
* Copyright (C) 2009 Mamadou Diop.
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
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinydav/tdav.h"

// Sessions
#include "tinymedia/tmedia_session_ghost.h"
#include "tinydav/audio/tdav_session_audio.h"
#include "tinydav/video/tdav_session_video.h"

// Codecs
#include "tinydav/codecs/dtmf/tdav_codec_dtmf.h"
#include "tinydav/codecs/msrp/tdav_codec_msrp.h"
#include "tinydav/codecs/amr/tdav_codec_amr.h"
#include "tinydav/codecs/g711/tdav_codec_g711.h"
#include "tinydav/codecs/gsm/tdav_codec_gsm.h"
#include "tinydav/codecs/ilbc/tdav_codec_ilbc.h"
#include "tinydav/codecs/h261/tdav_codec_h261.h"
#include "tinydav/codecs/h263/tdav_codec_h263.h"

// Consumers
#include "tinydav/audio/waveapi/tdav_consumer_waveapi.h"
#include "tinydav/audio/directsound/tdav_consumer_dsound.h"
#include "tinydav/audio/coreaudio/tdav_consumer_coreaudio.h"
#if HAVE_TINYDSHOW // DirectShow
#	include "tinydshow/plugin/DSConsumer.h"
#endif

// Producers
#include "tinydav/audio/waveapi/tdav_producer_waveapi.h"
#include "tinydav/audio/directsound/tdav_producer_dsound.h"
#include "tinydav/audio/coreaudio/tdav_producer_coreaudio.h"
#if HAVE_TINYDSHOW // DirectShow
#	include "tinydshow/plugin/DSProducer.h"
#endif

// Audio Denoise (AGC, Noise Suppression, VAD and AEC)
#if HAVE_SPEEX_DSP
#include "tinydav/audio/tdav_speex_denoise.h"
#endif

#if HAVE_FFMPEG
#	include <libavcodec/avcodec.h>
#endif

int tdav_init()
{
	/* === Initialize ffmpeg === */
#if HAVE_FFMPEG
	avcodec_init();
#endif

	/* === Register sessions === */
	tmedia_session_plugin_register(tmedia_session_ghost_plugin_def_t);
	tmedia_session_plugin_register(tdav_session_audio_plugin_def_t);
	tmedia_session_plugin_register(tdav_session_video_plugin_def_t);

	/* === Register codecs === */
#if HAVE_FFMPEG
	avcodec_register_all();
#endif
	
	tmedia_codec_plugin_register(tdav_codec_dtmf_plugin_def_t);
	tmedia_codec_plugin_register(tdav_codec_msrp_plugin_def_t);
	tmedia_codec_plugin_register(tdav_codec_g711a_plugin_def_t);
	tmedia_codec_plugin_register(tdav_codec_g711u_plugin_def_t);
#if HAVE_OPENCORE_AMR
	tmedia_codec_plugin_register(tdav_codec_amrnb_plugin_def_t);
#endif
#if HAVE_LIBGSM
	tmedia_codec_plugin_register(tdav_codec_gsm_plugin_def_t);
#endif
#if HAVE_ILBC
	tmedia_codec_plugin_register(tdav_codec_ilbc_plugin_def_t);
#endif
#if HAVE_FFMPEG
	tmedia_codec_plugin_register(tdav_codec_h261_plugin_def_t);
	//tmedia_codec_plugin_register(tdav_codec_h263_plugin_def_t);
	//tmedia_codec_plugin_register(tdav_codec_h263p_plugin_def_t);
	//tmedia_codec_plugin_register(tdav_codec_h263pp_plugin_def_t);
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

#if HAVE_COREAUDIO
	tmedia_consumer_plugin_register(tdav_consumer_coreaudio_plugin_def_t);
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

	/* === Register Audio Denoise (AGC, VAD, Noise Suppression and AEC) === */
#if HAVE_SPEEX_DSP
	tmedia_denoise_plugin_register(tdav_speex_denoise_plugin_def_t);
#endif

	return 0;
}

int tdav_deinit()
{
	/* === UnRegister sessions === */
	tmedia_session_plugin_unregister(tmedia_session_ghost_plugin_def_t);
	tmedia_session_plugin_unregister(tdav_session_audio_plugin_def_t);
	tmedia_session_plugin_unregister(tdav_session_video_plugin_def_t);

	/* === UnRegister codecs === */
	tmedia_codec_plugin_unregister(tdav_codec_dtmf_plugin_def_t);
	tmedia_codec_plugin_unregister(tdav_codec_msrp_plugin_def_t);
	tmedia_codec_plugin_unregister(tdav_codec_g711a_plugin_def_t);
	tmedia_codec_plugin_unregister(tdav_codec_g711u_plugin_def_t);
#if HAVE_OPENCORE_AMR
	tmedia_codec_plugin_unregister(tdav_codec_amrnb_plugin_def_t);
#endif
#if HAVE_LIBGSM
	tmedia_codec_plugin_unregister(tdav_codec_gsm_plugin_def_t);
#endif
#if HAVE_ILBC
	tmedia_codec_plugin_unregister(tdav_codec_ilbc_plugin_def_t);
#endif
#if HAVE_FFMPEG
	tmedia_codec_plugin_unregister(tdav_codec_h261_plugin_def_t);
	tmedia_codec_plugin_unregister(tdav_codec_h263_plugin_def_t);
	tmedia_codec_plugin_unregister(tdav_codec_h263p_plugin_def_t);
	tmedia_codec_plugin_unregister(tdav_codec_h263pp_plugin_def_t);
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
#if HAVE_COREAUDIO // CoreAudio
	tmedia_consumer_plugin_unregister(tdav_consumer_coreaudio_plugin_def_t);
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
#if HAVE_COREAUDIO // CoreAudio
	tmedia_producer_plugin_unregister(tdav_producer_coreaudio_plugin_def_t);
#endif

#if HAVE_OSS_H
	tmedia_consumer_plugin_unregister(tmedia_consumer_oss_plugin_def_t);
#endif

	/* === UnRegister Audio Denoise (AGC, VAD, Noise Suppression and AEC) === */
#if HAVE_SPEEX_DSP
	tmedia_denoise_plugin_unregister(tdav_speex_denoise_plugin_def_t);
#endif

	return 0;
}