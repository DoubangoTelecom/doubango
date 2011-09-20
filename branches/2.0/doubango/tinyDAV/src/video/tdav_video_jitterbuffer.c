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

/**@file tdav_video_jitterbuffer.c
 * @brief Video Jitter Buffer
 *
 * @author Mamadou Diop <diopmamadou(at)doubango(DOT)org>
 */
#include "tinydav/video/tdav_video_jitterbuffer.h"

#include "tinyrtp/rtp/trtp_rtp_header.h"

#include "tsk_debug.h"

#define TDAV_VIDEO_JB_TAIL		1000 // in milliseconds

// Internal functions
static tdav_video_jitterbuffer_packet_t* _tdav_video_jitterbuffer_packet_create(void* data, tsk_size_t data_size);



static int tdav_video_jitterbuffer_set(tmedia_jitterbuffer_t *self, const tmedia_param_t* param)
{
	TSK_DEBUG_ERROR("Not implemented");
	return -2;
}

static int tdav_video_jitterbuffer_open(tmedia_jitterbuffer_t* self, uint32_t frame_duration, uint32_t rate)
{
	tdav_video_jitterbuffer_t *jb = (tdav_video_jitterbuffer_t *)self;
	
	if(!jb->packets && !(jb->packets = tsk_list_create())){
		TSK_DEBUG_ERROR("Failed to create list");
		return -2;
	}
	if(!jb->timer && !(jb->timer = tsk_timer_manager_create())){
		TSK_DEBUG_ERROR("Failed to create list");
		return -3;
	}
	
	jb->frame_duration = frame_duration;
	jb->frame_max_count = (jb->tail / jb->frame_duration);

	return 0;
}

static int tdav_video_jitterbuffer_tick(tmedia_jitterbuffer_t* self)
{
	tdav_video_jitterbuffer_t *jb = (tdav_video_jitterbuffer_t *)self;
	
	(jb);
	
	// do nothing
	// only useful for audio

	return 0;
}

static int tdav_video_jitterbuffer_put(tmedia_jitterbuffer_t* self, void* data, tsk_size_t data_size, const tsk_object_t* proto_hdr)
{
	tdav_video_jitterbuffer_t *jb = (tdav_video_jitterbuffer_t *)self;
	const trtp_rtp_header_t* rtp_hdr;
	register uint32_t i = 0;
	tsk_bool_t item_found = tsk_false;
	const tsk_list_item_t *item;
	tdav_video_jitterbuffer_packet_t *jb_packet = tsk_null;
	int ret = 0;
	
	if(!data || !data_size || !proto_hdr){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	
	rtp_hdr = TRTP_RTP_HEADER(proto_hdr);

	tsk_safeobj_lock(jb);

	tsk_list_foreach(item, jb->packets){
		if(i++ == jb->frame_curr_index){
			if((jb_packet = (tdav_video_jitterbuffer_packet_t*)item->data)){
				if((ret = tsk_buffer_copy(jb_packet->data, 0, data, data_size))){
					TSK_DEBUG_ERROR("Failed to copy buffer");
					goto done;
				}
			}
			else{
				TSK_DEBUG_ERROR("Item contains null data");
				goto done;
			}
			// update values
			jb_packet->taken = tsk_false;
			jb_packet->seq_num = rtp_hdr->seq_num;
			item_found = tsk_true;
		}
	}

	if(!item_found && !jb_packet){
		if((jb_packet = _tdav_video_jitterbuffer_packet_create(data, data_size))){
			// update values (use constructor)
			jb_packet->seq_num = rtp_hdr->seq_num;
			
			tsk_list_push_ascending_data(jb->packets, (void**)&jb_packet);
			TSK_OBJECT_SAFE_FREE(jb_packet);
		}
		else{
			TSK_DEBUG_ERROR("Failed to create jb packet");
			ret = -2;
			goto done;
		}
	}

done:
	if(ret == 0){
		jb->frame_curr_index = (++jb->frame_curr_index % jb->frame_max_count);
	}
	tsk_safeobj_unlock(jb);
	
	return ret;
}

static tsk_size_t tdav_video_jitterbuffer_get(tmedia_jitterbuffer_t* self, void* out_data, tsk_size_t out_size)
{
	tdav_video_jitterbuffer_t *jb = (tdav_video_jitterbuffer_t *)self;
	const tsk_list_item_t *item;
	tdav_video_jitterbuffer_packet_t *jb_packet = tsk_null;
	tsk_size_t ret_size = 0;

	if(!out_data || !out_size){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}

	tsk_safeobj_lock(jb);

	tsk_list_foreach(item, jb->packets){
		if((jb_packet = (tdav_video_jitterbuffer_packet_t*)item->data) && !jb_packet->taken){
			ret_size = TSK_MIN(TSK_BUFFER_SIZE(jb_packet->data), out_size);
			memcpy(out_data, TSK_BUFFER_DATA(jb_packet->data), ret_size);
			jb_packet->taken = tsk_true;
			break;
		}
	}

	tsk_safeobj_unlock(jb);

	return ret_size;
}

static int tdav_video_jitterbuffer_reset(tmedia_jitterbuffer_t* self)
{
	tdav_video_jitterbuffer_t *jb = (tdav_video_jitterbuffer_t *)self;
	
	(jb);

	return 0;
}

static int tdav_video_jitterbuffer_close(tmedia_jitterbuffer_t* self)
{
	tdav_video_jitterbuffer_t *jb = (tdav_video_jitterbuffer_t *)self;
	
	(jb);

	return 0;
}


//
//	Speex jitterbuffer Packet Object definition
//
static tdav_video_jitterbuffer_packet_t* _tdav_video_jitterbuffer_packet_create(void* data, tsk_size_t data_size)
{
	tdav_video_jitterbuffer_packet_t *jb_packet;
	if((jb_packet = tsk_object_new(tdav_video_jitterbuffer_packet_def_t))){
		jb_packet->data = tsk_buffer_create(data, data_size);
	}
	return jb_packet;
}

/* constructor */
static tsk_object_t* tdav_video_jitterbuffer_packet_ctor(tsk_object_t * self, va_list * app)
{
	tdav_video_jitterbuffer_packet_t *jb_packet = self;
	if(jb_packet){
		TSK_OBJECT_SAFE_FREE(jb_packet->data);
		tsk_safeobj_init(jb_packet);
	}
	return self;
}

/* destructor */
static tsk_object_t* tdav_video_jitterbuffer_packet_dtor(tsk_object_t * self)
{ 
	tdav_video_jitterbuffer_packet_t *jb_packet = self;
	if(jb_packet){
		tsk_safeobj_deinit(jb_packet);
	}

	return self;
}
/* comparator */
static int tdav_video_jitterbuffer_packet_cmp(const tsk_object_t *_p1, const tsk_object_t *_p2)
{
	const tdav_video_jitterbuffer_packet_t *p1 = _p1;
	const tdav_video_jitterbuffer_packet_t *p2 = _p2;

	if(p1 && p2){
		return (int)(p1->seq_num - p2->seq_num);
	}
	else if(!p1 && !p2) return 0;
	else return -1;
}
/* object definition */
static const tsk_object_def_t tdav_video_jitterbuffer_packet_def_s = 
{
	sizeof(tdav_video_jitterbuffer_packet_t),
	tdav_video_jitterbuffer_packet_ctor, 
	tdav_video_jitterbuffer_packet_dtor,
	tdav_video_jitterbuffer_packet_cmp, 
};
const tsk_object_def_t *tdav_video_jitterbuffer_packet_def_t = &tdav_video_jitterbuffer_packet_def_s;

//
//	Speex jitterbuffer Plugin definition
//

/* constructor */
static tsk_object_t* tdav_video_jitterbuffer_ctor(tsk_object_t * self, va_list * app)
{
	tdav_video_jitterbuffer_t *jb = self;
	if(jb){
		/* init base */
		tmedia_jitterbuffer_init(TMEDIA_JITTER_BUFFER(jb));
		/* init self */
		tsk_safeobj_init(jb);
		jb->tail = TDAV_VIDEO_JB_TAIL;
	}
	return self;
}
/* destructor */
static tsk_object_t* tdav_video_jitterbuffer_dtor(tsk_object_t * self)
{ 
	tdav_video_jitterbuffer_t *jb = self;
	if(jb){
		/* deinit base */
		tmedia_jitterbuffer_deinit(TMEDIA_JITTER_BUFFER(jb));
		/* deinit self */
		TSK_OBJECT_SAFE_FREE(jb->packets);
		if(jb->timer){
			tsk_timer_manager_destroy(&jb->timer);
		}
		tsk_safeobj_deinit(jb);
	}

	return self;
}
/* object definition */
static const tsk_object_def_t tdav_video_jitterbuffer_def_s = 
{
	sizeof(tdav_video_jitterbuffer_t),
	tdav_video_jitterbuffer_ctor, 
	tdav_video_jitterbuffer_dtor,
	tsk_null, 
};
/* plugin definition*/
static const tmedia_jitterbuffer_plugin_def_t tdav_video_jitterbuffer_plugin_def_s = 
{
	&tdav_video_jitterbuffer_def_s,
	tmedia_video,
	"Native Video JitterBuffer",
	
	tdav_video_jitterbuffer_set,
	tdav_video_jitterbuffer_open,
	tdav_video_jitterbuffer_tick,
	tdav_video_jitterbuffer_put,
	tdav_video_jitterbuffer_get,
	tdav_video_jitterbuffer_reset,
	tdav_video_jitterbuffer_close,
};
const tmedia_jitterbuffer_plugin_def_t *tdav_video_jitterbuffer_plugin_def_t = &tdav_video_jitterbuffer_plugin_def_s;
