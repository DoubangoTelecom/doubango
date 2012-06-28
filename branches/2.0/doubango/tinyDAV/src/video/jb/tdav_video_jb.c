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

/**@file tdav_video_jb.c
 * @brief Video Jitter Buffer
 *
 * @author Mamadou Diop <diopmamadou(at)doubango(DOT)org>
 */
#include "tinydav/video/jb/tdav_video_jb.h"
#include "tinydav/video/jb/tdav_video_frame.h"

#include "tinyrtp/rtp/trtp_rtp_packet.h"

#import "tsk_time.h"
#include "tsk_memory.h"
#include "tsk_timer.h"
#include "tsk_debug.h"

// default frame rate
// the corret fps will be computed using the RTP timestamps
#define TDAV_VIDEO_JB_FPS		TDAV_VIDEO_JB_FPS_MAX
#define TDAV_VIDEO_JB_FPS_MIN	1
#define TDAV_VIDEO_JB_FPS_MAX	30
// Number of correct consecutive RTP packets to receive before computing the FPS
#define TDAV_VIDEO_JB_FPS_PROB	(TDAV_VIDEO_JB_FPS >> 1)
// Max number of frames to allow in jitter buffer
#define TDAV_VIDEO_JB_TAIL_MAX	(TDAV_VIDEO_JB_FPS << 2)
// Min number of frames required before requesting a full decode
// This is required because of the FEC and NACK functions
// Will be updated using the RTT value from RTCP and probation
#define TDAV_VIDEO_JB_TAIL_MIN_MIN	2
#define TDAV_VIDEO_JB_TAIL_MIN_MAX	4
#define TDAV_VIDEO_JB_TAIL_MIN_PROB	(TDAV_VIDEO_JB_FPS >> 2)

#define TDAV_VIDEO_JB_MAX_DROPOUT		0xFD9B

#define TDAV_VIDEO_JB_DISABLE           0

static const tdav_video_frame_t* _tdav_video_jb_get_frame(struct tdav_video_jb_s* self, uint32_t timestamp, uint8_t pt, tsk_bool_t *pt_matched);
static int _tdav_video_jb_timer_callback(const void* arg, tsk_timer_id_t timer_id);

typedef struct tdav_video_jb_s
{
	TSK_DECLARE_OBJECT;

	tsk_bool_t started;
	int32_t fps;
	int32_t fps_prob;
	int32_t avg_duration;
	uint32_t last_timestamp;
	int32_t conseq_frame_drop;
	int32_t tail_max;
	int32_t tail_min;
	int32_t tail_prob;
	tdav_video_frames_L_t *frames;
	int64_t frames_count;
	tsk_timer_manager_handle_t *h_timer;
	tsk_timer_id_t timer_decode;
	uint16_t seq_nums[0xFF];
	tdav_video_jb_cb_f callback;
	const void* callback_data;

	// to avoid locking use different cb_data
	tdav_video_jb_cb_data_xt cb_data_rtp;
	tdav_video_jb_cb_data_xt cb_data_fdd;
	tdav_video_jb_cb_data_xt cb_data_any;

	struct{
		void* ptr;
		tsk_size_t size;
	} buffer;

	TSK_DECLARE_SAFEOBJ;
}
tdav_video_jb_t;


static tsk_object_t* tdav_video_jb_ctor(tsk_object_t * self, va_list * app)
{
	tdav_video_jb_t *jb = self;
	if(jb){
		if(!(jb->frames = tsk_list_create())){
			TSK_DEBUG_ERROR("Failed to create list");
			return tsk_null;
		}
		if(!(jb->h_timer = tsk_timer_manager_create())){
			TSK_DEBUG_ERROR("Failed to create timer manager");
			return tsk_null;
		}
		jb->cb_data_fdd.type = tdav_video_jb_cb_data_type_fdd;
		jb->cb_data_rtp.type = tdav_video_jb_cb_data_type_rtp;

		tsk_safeobj_init(jb);
	}
	return self;
}
static tsk_object_t* tdav_video_jb_dtor(tsk_object_t * self)
{ 
	tdav_video_jb_t *jb = self;
	if(jb){
		if(jb->started){
			tdav_video_jb_stop(jb);
		}
		TSK_OBJECT_SAFE_FREE(jb->frames);
		if(jb->h_timer){
			tsk_timer_manager_destroy(&jb->h_timer);
		}
		TSK_SAFE_FREE(jb->buffer.ptr);
		tsk_safeobj_deinit(jb);
	}

	return self;
}
static const tsk_object_def_t tdav_video_jb_def_s = 
{
	sizeof(tdav_video_jb_t),
	tdav_video_jb_ctor, 
	tdav_video_jb_dtor,
	tsk_null, 
};

tdav_video_jb_t* tdav_video_jb_create()
{
	tdav_video_jb_t* jb;

	if((jb = tsk_object_new(&tdav_video_jb_def_s))){
		jb->fps = TDAV_VIDEO_JB_FPS;
		jb->fps_prob = TDAV_VIDEO_JB_FPS_PROB;
		jb->tail_max = TDAV_VIDEO_JB_TAIL_MAX;
		jb->tail_min = TDAV_VIDEO_JB_TAIL_MIN_MIN;
		jb->tail_prob = TDAV_VIDEO_JB_TAIL_MIN_PROB;
	}
	return jb;
}

#define tdav_video_jb_reset_fps_prob(self) {\
	(self)->fps_prob = TDAV_VIDEO_JB_FPS_PROB; \
	(self)->last_timestamp = 0; \
	(self)->avg_duration = 0; \
}
#define tdav_video_jb_reset_tail_min_prob(self) {\
	(self)->tail_prob = TDAV_VIDEO_JB_TAIL_MIN_PROB; \
	(self)->tail_min = TDAV_VIDEO_JB_TAIL_MIN_MAX; \
}

int tdav_video_jb_set_callback(tdav_video_jb_t* self, tdav_video_jb_cb_f callback, const void* usr_data)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	self->callback = callback;
	self->cb_data_any.usr_data = usr_data;
	self->cb_data_fdd.usr_data = usr_data;
	self->cb_data_rtp.usr_data = usr_data;
	return 0;
}

int tdav_video_jb_start(tdav_video_jb_t* self)
{
	int ret;
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	if(self->started){
		return 0;
	}
	
	if((ret = tsk_timer_manager_start(self->h_timer)) == 0){
		self->timer_decode = tsk_timer_manager_schedule(self->h_timer, (1000 / self->fps), _tdav_video_jb_timer_callback, self);
		self->started = tsk_true;
	}
	return ret;
}

int tdav_video_jb_put(tdav_video_jb_t* self, trtp_rtp_packet_t* rtp_pkt)
{
#if TDAV_VIDEO_JB_DISABLE
    self->cb_data_rtp.rtp.pkt = rtp_pkt;
    self->callback(&self->cb_data_rtp);
#else
	const tdav_video_frame_t* old_frame;
	tsk_bool_t pt_matched = tsk_false, is_frame_late_or_dup = tsk_false, is_restarted = tsk_false;
	uint16_t* seq_num;

	if(!self || !rtp_pkt || !rtp_pkt->header){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	seq_num = &self->seq_nums[rtp_pkt->header->payload_type];

	tsk_safeobj_lock(self);

	old_frame = _tdav_video_jb_get_frame(self, rtp_pkt->header->timestamp, rtp_pkt->header->payload_type, &pt_matched);	

	if((*seq_num && *seq_num != 0xFFFF) && (*seq_num + 1) != rtp_pkt->header->seq_num){ // FIXME: check if seq_num wrapped
		int32_t diff = (rtp_pkt->header->seq_num - *seq_num);
		tsk_bool_t is_frame_loss = (diff > 0);
		is_restarted = (TSK_ABS(diff) > TDAV_VIDEO_JB_MAX_DROPOUT);
		is_frame_late_or_dup = !is_frame_loss;
		tdav_video_jb_reset_fps_prob(self);
		tdav_video_jb_reset_tail_min_prob(self);
		TSK_DEBUG_INFO("Packet %s (from JB) [%u - %u]", is_frame_loss ? "loss" : "late/duplicated/nack", *seq_num, rtp_pkt->header->seq_num);
		
		if(is_frame_loss && !is_restarted){
			if(self->callback){			
				self->cb_data_any.type = tdav_video_jb_cb_data_type_fl;
				self->cb_data_any.ssrc = rtp_pkt->header->ssrc;
				self->cb_data_any.fl.seq_num = (*seq_num + 1);
				self->cb_data_any.fl.count = diff - 1;
				self->callback(&self->cb_data_any);
			}
		}
	}
	else{
		--self->tail_prob;
	}
	self->tail_min = self->tail_prob <= 0 ? TDAV_VIDEO_JB_TAIL_MIN_MIN : TDAV_VIDEO_JB_TAIL_MIN_MAX;

	if(!old_frame){
		tdav_video_frame_t* new_frame;
		if(pt_matched){
			// if we have a frame with the same payload type but without this timestamp this means that we moved to a new frame
			// this happens if the frame is waiting to be decoded or the marker is lost
		}
		if((new_frame = tdav_video_frame_create(rtp_pkt))){
			// compute avg frame duration
			if(self->last_timestamp && self->last_timestamp < rtp_pkt->header->timestamp){
				uint32_t duration = (rtp_pkt->header->timestamp - self->last_timestamp);
				self->avg_duration = self->avg_duration ? ((self->avg_duration + duration) >> 1) : duration;
				--self->fps_prob;
			}
			self->last_timestamp = rtp_pkt->header->timestamp;
			
			tsk_list_lock(self->frames);
			if(self->frames_count >= self->tail_max){
				if(++self->conseq_frame_drop >= self->fps){
					TSK_DEBUG_INFO("Too many frames dropped and fps=%d", self->fps);
					tsk_list_clear_items(self->frames);
					self->conseq_frame_drop = 0;
					self->frames_count = 1;
					if(self->callback){
						self->cb_data_any.type = tdav_video_jb_cb_data_type_tmfr;
						self->cb_data_any.ssrc = rtp_pkt->header->ssrc;
						self->callback(&self->cb_data_any);
					}
				}
				else{
					tsk_list_remove_first_item(self->frames);
					// self->frames_count += 0;
				}
				tdav_video_jb_reset_fps_prob(self);
			}
			else{
				++self->frames_count;
			}
			tsk_list_push_ascending_data(self->frames, (void**)&new_frame);
			tsk_list_unlock(self->frames);
		}
		if(self->fps_prob <= 0 && self->avg_duration){
			// compute FPS
			self->fps = TSK_CLAMP(TDAV_VIDEO_JB_FPS_MIN, ((3003 * 30) / self->avg_duration), TDAV_VIDEO_JB_FPS_MAX);
			//self->fps = ((3003 * 30) / self->avg_duration);
			self->tail_max = (self->fps >> 1); // maximum delay = half second
			tdav_video_jb_reset_fps_prob(self);
		}
	}
	else{
		tdav_video_frame_put((tdav_video_frame_t*)old_frame, rtp_pkt);
	}

	tsk_safeobj_unlock(self);

	if(!is_frame_late_or_dup || is_restarted){
		*seq_num = rtp_pkt->header->seq_num;
	}
#endif

	return 0;
}

int tdav_video_jb_stop(tdav_video_jb_t* self)
{
	int ret;
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	if(!self->started){
		return 0;
	}
	
	if((ret = tsk_timer_manager_stop(self->h_timer)) == 0){
		self->started = tsk_false;
	}
	return ret;
}

static const tdav_video_frame_t* _tdav_video_jb_get_frame(tdav_video_jb_t* self, uint32_t timestamp, uint8_t pt, tsk_bool_t *pt_matched)
{
	const tdav_video_frame_t* ret = tsk_null;
	const tsk_list_item_t *item;
	
	*pt_matched =tsk_false;

	tsk_list_lock(self->frames);
	tsk_list_foreach(item, self->frames){
		if(TDAV_VIDEO_FRAME(item->data)->payload_type == pt){
			if(!(*pt_matched)) *pt_matched = tsk_true;
			if(TDAV_VIDEO_FRAME(item->data)->timestamp == timestamp){
				ret = item->data;
				break;
			}
		}
		
	}
	tsk_list_unlock(self->frames);

	return ret;
}

static int _tdav_video_jb_timer_callback(const void* arg, tsk_timer_id_t timer_id)
{
#if !TDAV_VIDEO_JB_DISABLE
    tdav_video_jb_t* jb = (tdav_video_jb_t*)arg;
    
	if(!jb->started){
		return 0;
	}
	
	if(jb->timer_decode == timer_id){
        uint64_t next_timeout = (1000 / jb->fps);
        
		if(jb->frames_count >= jb->tail_min){
			tsk_list_item_t* item;
            uint64_t decode_start = tsk_time_now(), decode_duration;
            
			tsk_safeobj_lock(jb); // against get_frame()
			tsk_list_lock(jb->frames);
			item = tsk_list_pop_first_item(jb->frames);
			--jb->frames_count;				
			tsk_list_unlock(jb->frames);
			tsk_safeobj_unlock(jb);
            
			if(jb->callback){
				trtp_rtp_packet_t* pkt;
				const tsk_list_item_t* _item = item; // save memory address as "tsk_list_foreach() will change it for each loop"
				const tdav_video_frame_t* frame = _item->data;
				int32_t last_seq_num = -1; // guard against duplicated packets
				tsk_list_foreach(_item, frame->pkts){
					if(!(pkt = _item->data) || !pkt->payload.size || !pkt->header || pkt->header->seq_num == last_seq_num){
						continue;
					}
					// pkt->header->marker = (_item == frame->pkts->tail); // break the accumulator
					jb->cb_data_rtp.rtp.pkt = pkt;
					jb->callback(&jb->cb_data_rtp);
				}
			}
            
			TSK_OBJECT_SAFE_FREE(item);
            decode_duration = (tsk_time_now() - decode_start);
            next_timeout = (decode_duration > next_timeout) ? 0 : (next_timeout - decode_duration);
            //if(!next_timeout)TSK_DEBUG_INFO("next_timeout=%llu", next_timeout);
		}
        else{
            //TSK_DEBUG_INFO("Not enought frames");
            //next_timeout >>= 1;
        }
        
        jb->timer_decode = tsk_timer_manager_schedule(jb->h_timer, next_timeout, _tdav_video_jb_timer_callback, jb);
	}
#endif
	
	return 0;
}