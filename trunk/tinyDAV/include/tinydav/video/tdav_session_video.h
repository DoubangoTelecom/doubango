/*
* Copyright (C) 2009-2010 Mamadou Diop.
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
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYDAV_SESSION_VIDEO_H
#define TINYDAV_SESSION_VIDEO_H

#include "tinydav_config.h"

#include "tinydav/video/tdav_converter_video.h"

#include "tinymedia/tmedia_session.h"

#include "tsk_safeobj.h"

TDAV_BEGIN_DECLS

// Forward declaration
struct trtp_manager_s;
struct tdav_consumer_video_s;
struct tdav_converter_video_s;

typedef struct tdav_session_video_s
{
	TMEDIA_DECLARE_SESSION_VIDEO;

	tsk_bool_t useIPv6;

	char* local_ip;

	char* remote_ip;
	uint16_t remote_port;
	
	/* NAT Traversal context */
	tnet_nat_context_handle_t* natt_ctx;

	tsk_bool_t rtcp_enabled;

	struct trtp_manager_s* rtp_manager;

	struct{
		void* buffer;
		tsk_size_t buffer_size;

		void* conv_buffer;
		tsk_size_t conv_buffer_size;
	} encoder;

	struct{
		void* buffer;
		tsk_size_t buffer_size;

		void* conv_buffer;
		tsk_size_t conv_buffer_size;
	} decoder;

	struct tmedia_consumer_s* consumer;
	struct tmedia_producer_s* producer;
	struct {
		tsk_size_t consumerWidth;
		tsk_size_t consumerHeight;
		tsk_size_t xConsumerSize;
		struct tdav_converter_video_s* fromYUV420;
		
		tsk_size_t producerWidth;
		tsk_size_t producerHeight;
		tsk_size_t xProducerSize;
		struct tdav_converter_video_s* toYUV420;
	} conv;

	TSK_DECLARE_SAFEOBJ;
}
tdav_session_video_t;


TINYDAV_GEXTERN const tmedia_session_plugin_def_t *tdav_session_video_plugin_def_t;

TDAV_END_DECLS

#endif /* TINYDAV_SESSION_VIDEO_H */
