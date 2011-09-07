/*
* Copyright (C) 2011 Doubango Telecom <http://www.doubango.org>
*
* Contact: Mamadou Diop <diopmamadou(at)doubango(DOT)org>
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

/**@file tdav_video_jitterbuffer.h
 * @brief Video Jitter Buffer
 *
 * @author Mamadou Diop <diopmamadou(at)doubango(DOT)org>
 */
#ifndef TINYDAV_VIDEO_JITTERBUFFER_H
#define TINYDAV_VIDEO_JITTERBUFFER_H

#include "tinydav_config.h"

#include "tinymedia/tmedia_jitterbuffer.h"

#include "tsk_buffer.h"
#include "tsk_timer.h"
#include "tsk_list.h"
#include "tsk_safeobj.h"

TDAV_BEGIN_DECLS

/** Video JitterBuffer packet */
typedef struct tdav_video_jitterbuffer_packet_s
{
	TSK_DECLARE_OBJECT;
	TSK_DECLARE_SAFEOBJ;

	tsk_bool_t taken;
	tsk_buffer_t *data;
	int64_t seq_num;
}
tdav_video_jitterbuffer_packet_t;
TINYDAV_GEXTERN const tsk_object_def_t *tdav_video_jitterbuffer_packet_def_t;
typedef tsk_list_t tdav_video_jitterbuffer_packets_L_t;

/** Video JitterBuffer */
typedef struct tdav_video_jitterbuffer_s
{
	TMEDIA_DECLARE_JITTER_BUFFER;
	TSK_DECLARE_SAFEOBJ;
	
	uint32_t fps;
	uint32_t frame_duration;
	uint32_t frame_max_count;
	uint32_t frame_curr_index;
	uint32_t tail; // in milliseconds

	tdav_video_jitterbuffer_packets_L_t * packets;
	tsk_timer_manager_handle_t *timer;
}
tdav_video_jitterbuffer_t;

const tmedia_jitterbuffer_plugin_def_t *tdav_video_jitterbuffer_plugin_def_t;

TDAV_END_DECLS

#endif /* TINYDAV_VIDEO_JITTERBUFFER_H */
