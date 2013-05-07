/*
* Copyright (C) 2012 Doubango Telecom <http://www.doubango.org>
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

/**@file tdav_video_frame.h
 * @brief Video Frame
 *
 * @author Mamadou Diop <diopmamadou(at)doubango(DOT)org>
 */
#ifndef TINYDAV_VIDEO_JB_FRAME_H
#define TINYDAV_VIDEO_JB_FRAME_H

#include "tinydav_config.h"

#include "tinyrtp/rtp/trtp_rtp_packet.h"

#include "tsk_safeobj.h"
#include "tsk_list.h"

TDAV_BEGIN_DECLS

#define TDAV_VIDEO_FRAME(self) ((tdav_video_frame_t*)(self))

typedef uint16_t tdav_video_frame_seq_nums[16];
typedef tsk_list_t tdav_video_frames_L_t;

typedef struct tdav_video_frame_s
{
	TSK_DECLARE_OBJECT;

	uint8_t payload_type;
	uint32_t timestamp;
	uint16_t highest_seq_num;
	uint32_t ssrc;
	trtp_rtp_packets_L_t* pkts;
	
	TSK_DECLARE_SAFEOBJ;
}
tdav_video_frame_t;

struct tdav_video_frame_s* tdav_video_frame_create(struct trtp_rtp_packet_s* rtp_pkt);
int tdav_video_frame_put(struct tdav_video_frame_s* self, struct trtp_rtp_packet_s* rtp_pkt);
const struct trtp_rtp_packet_s* tdav_video_frame_find_by_seq_num(const struct tdav_video_frame_s* self, uint16_t seq_num);
tsk_size_t tdav_video_frame_write(struct tdav_video_frame_s* self, void** buffer_ptr, tsk_size_t* buffer_size);
tsk_bool_t tdav_video_frame_is_complete(const struct tdav_video_frame_s* self, int32_t last_seq_num_with_mark, uint16_t* missing_seq_num_start, tsk_size_t* missing_seq_num_count);

TDAV_END_DECLS

#endif /* TINYDAV_VIDEO_JB_FRAME_H */
