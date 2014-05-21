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

/**@file tdav_session_video.h
 * @brief Video Session plugin.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#ifndef TINYDAV_SESSION_VIDEO_H
#define TINYDAV_SESSION_VIDEO_H

#include "tinydav_config.h"
#include "tinydav/tdav_session_av.h"

TDAV_BEGIN_DECLS

typedef enum tdav_session_video_pkt_loss_level_e
{
	tdav_session_video_pkt_loss_level_low,
	tdav_session_video_pkt_loss_level_medium,
	tdav_session_video_pkt_loss_level_high,
}
tdav_session_video_pkt_loss_level_t;

typedef struct tdav_session_video_s
{
	TDAV_DECLARE_SESSION_AV;

	struct tdav_video_jb_s* jb;
	tsk_bool_t jb_enabled;
	tsk_bool_t zero_artifacts;
	tsk_bool_t fps_changed;
	tsk_bool_t started;

	struct{
		const void* context;
		tmedia_session_rtcp_onevent_cb_f func;
	} cb_rtcpevent;

	struct{
		void* buffer;
		tsk_size_t buffer_size;

		int rotation;
        tsk_bool_t scale_rotated_frames;

		void* conv_buffer;
		tsk_size_t conv_buffer_size;

		tdav_session_video_pkt_loss_level_t pkt_loss_level;
		int32_t pkt_loss_fact;
		int32_t pkt_loss_prob_good;
		int32_t pkt_loss_prob_bad;

		uint64_t last_frame_time;

		uint8_t payload_type;
		struct tmedia_codec_s* codec;
		tsk_mutex_handle_t* h_mutex;
	} encoder;

	struct{
		void* buffer;
		tsk_size_t buffer_size;

		void* conv_buffer;
		tsk_size_t conv_buffer_size;

		// latest decoded RTP seqnum
		uint16_t last_seqnum;
		// stream is corrupted if packets are lost
		tsk_bool_t stream_corrupted;
		uint64_t stream_corrupted_since;
		uint32_t last_corrupted_timestamp;

		uint8_t codec_payload_type;
		struct tmedia_codec_s* codec;
		uint64_t codec_decoded_frames_count;
	} decoder;
	
	struct {
		tsk_size_t consumerLastWidth;
		tsk_size_t consumerLastHeight;
		struct tmedia_converter_video_s* fromYUV420;
		
		tsk_size_t producerWidth;
		tsk_size_t producerHeight;
		tsk_size_t xProducerSize;
		struct tmedia_converter_video_s* toYUV420;
	} conv;

	struct{
		tsk_list_t* packets;
		tsk_size_t count;
		tsk_size_t max;
		uint64_t last_fir_time;
		uint64_t last_pli_time;
	} avpf;
}
tdav_session_video_t;

#define TDAV_SESSION_VIDEO(self) ((tdav_session_video_t*)(self))

TINYDAV_GEXTERN const tmedia_session_plugin_def_t *tdav_session_video_plugin_def_t;
TINYDAV_GEXTERN const tmedia_session_plugin_def_t *tdav_session_bfcpvideo_plugin_def_t;

TDAV_END_DECLS

#endif /* TINYDAV_SESSION_VIDEO_H */
