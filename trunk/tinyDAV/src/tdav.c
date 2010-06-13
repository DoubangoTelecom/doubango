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

// Codecs
#include "tinydav/codecs/g711/tdav_codec_g711.h"

// Consumers
#include "tinydav/audio/waveapi/tdav_consumer_waveapi.h"
#include "tinydav/audio/directsound/tdav_consumer_dsound.h"

// Producers
#include "tinydav/audio/waveapi/tdav_producer_waveapi.h"
#include "tinydav/audio/directsound/tdav_producer_dsound.h"

#if 0
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#endif

int tdav_init()
{
#if 0
	AVCodecContext * context_encode;
	AVCodec* codec;

	/* === Initialize ffmpeg === */
	avcodec_init();
#endif

	/* === Register sessions === */
	tmedia_session_plugin_register(tmedia_session_ghost_plugin_def_t);
	tmedia_session_plugin_register(tdav_session_audio_plugin_def_t);

	/* === Register codecs === */
#if 0
	avcodec_register_all();
#endif

	tmedia_codec_plugin_register(tdav_codec_g711a_plugin_def_t);
	//--tmedia_codec_plugin_register(tdav_codec_g711u_plugin_def_t);

	/* === Register consumers === */
#if HAVE_DSOUND_H
	tmedia_consumer_plugin_register(tdav_consumer_dsound_plugin_def_t);
#elif HAVE_WAVE_API
	tmedia_consumer_plugin_register(tdav_consumer_waveapi_plugin_def_t);
#endif

#if HAVE_OSS_H
	tmedia_consumer_plugin_register(tmedia_consumer_oss_plugin_def_t);
#endif

	/* === Register producers === */
#if HAVE_DSOUND_H
	tmedia_producer_plugin_register(tdav_producer_dsound_plugin_def_t);
#elif HAVE_WAVE_API
	tmedia_producer_plugin_register(tdav_producer_waveapi_plugin_def_t);
#endif

	//if((context_encode = avcodec_alloc_context())){
	//	printf("avcodec_alloc_context()");
	//}

	//if((codec = avcodec_find_encoder(CODEC_ID_H263))){
	//	printf("avcodec_find_encoder(H.263)");
	//}

	//if((codec = avcodec_find_encoder(CODEC_ID_H264))){
	//	printf("avcodec_find_encoder(H.264)");
	//}

	return 0;
}

int tdav_deinit()
{
	/* === UnRegister sessions === */
	tmedia_session_plugin_register(tmedia_session_ghost_plugin_def_t);
	tmedia_session_plugin_unregister(tdav_session_audio_plugin_def_t);

	/* === UnRegister codecs === */
	tmedia_codec_plugin_unregister(tdav_codec_g711a_plugin_def_t);
	tmedia_codec_plugin_unregister(tdav_codec_g711u_plugin_def_t);

	/* === unRegister consumers === */
#if HAVE_DSOUND_H
	tmedia_consumer_plugin_unregister(tdav_consumer_dsound_plugin_def_t);
#elif HAVE_WAVE_API
	tmedia_consumer_plugin_unregister(tdav_consumer_waveapi_plugin_def_t);
#endif

	/* === UnRegister producers === */
#if HAVE_DSOUND_H
	tmedia_producer_plugin_unregister(tdav_producer_dsound_plugin_def_t);
#elif HAVE_WAVE_API
	tmedia_producer_plugin_unregister(tdav_producer_waveapi_plugin_def_t);
#endif

#if HAVE_OSS_H
	tmedia_consumer_plugin_unregister(tmedia_consumer_oss_plugin_def_t);
#endif

	return 0;
}