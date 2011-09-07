/*
* Copyright (C) 2011 Mamadou Diop.
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

/**@file tdav_speex_jitterbuffer.c
 * @brief Speex Audio jitterbuffer Plugin
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>

 */
#include "tinydav/audio/tdav_speex_jitterbuffer.h"
#include "tinyrtp/rtp/trtp_rtp_header.h"

#if HAVE_SPEEX_DSP && HAVE_SPEEX_JB

#include "tsk_memory.h"
#include "tsk_debug.h"

static int tdav_speex_jitterbuffer_set(tmedia_jitterbuffer_t *self, const tmedia_param_t* param)
{
	TSK_DEBUG_ERROR("Not implemented");
	return -2;
}

static int tdav_speex_jitterbuffer_open(tmedia_jitterbuffer_t* self, uint32_t frame_duration, uint32_t rate)
{
	tdav_speex_jitterbuffer_t *jitterbuffer = (tdav_speex_jitterbuffer_t *)self;
	if(!(jitterbuffer->state = jitter_buffer_init((int)frame_duration))){
		TSK_DEBUG_ERROR("jitter_buffer_init() failed");
		return -2;
	}
	jitterbuffer->rate = rate;
	jitterbuffer->frame_duration = frame_duration;

	return 0;
}

static int tdav_speex_jitterbuffer_tick(tmedia_jitterbuffer_t* self)
{
	tdav_speex_jitterbuffer_t *jitterbuffer = (tdav_speex_jitterbuffer_t *)self;
	if(!jitterbuffer->state){
		TSK_DEBUG_ERROR("Invalid state");
		return -1;
	}
	jitter_buffer_tick(jitterbuffer->state);
	return 0;
}

static int tdav_speex_jitterbuffer_put(tmedia_jitterbuffer_t* self, void* data, tsk_size_t data_size, const tsk_object_t* proto_hdr)
{
	tdav_speex_jitterbuffer_t *jitterbuffer = (tdav_speex_jitterbuffer_t *)self;
	const trtp_rtp_header_t* rtp_hdr;
	JitterBufferPacket jb_packet;
	if(!data || !data_size || !proto_hdr){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	
	if(!jitterbuffer->state){
		TSK_DEBUG_ERROR("Invalid state");
		return -2;
	}
	
	rtp_hdr = TRTP_RTP_HEADER(proto_hdr);
	
	jb_packet.data = data;
	jb_packet.len = data_size;
	jb_packet.span = (data_size*500)/jitterbuffer->rate;
	jb_packet.timestamp = (rtp_hdr->seq_num * jb_packet.span);
	
	jb_packet.sequence = rtp_hdr->seq_num;
	jb_packet.user_data = 0;
	jitter_buffer_put(jitterbuffer->state, &jb_packet);
	
	return 0;
}

static tsk_size_t tdav_speex_jitterbuffer_get(tmedia_jitterbuffer_t* self, void* out_data, tsk_size_t out_size)
{
	tdav_speex_jitterbuffer_t *jitterbuffer = (tdav_speex_jitterbuffer_t *)self;
	JitterBufferPacket jb_packet;
	int ret;

	if(!out_data || !out_size){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}
	if(!jitterbuffer->state){
		TSK_DEBUG_ERROR("Invalid state");
		return 0;
	}

	jb_packet.data = out_data;
	jb_packet.len = out_size;

	if ((ret = jitter_buffer_get(jitterbuffer->state, &jb_packet, (out_size*500)/jitterbuffer->rate, tsk_null)) != JITTER_BUFFER_OK) {
		switch(ret){
			case JITTER_BUFFER_MISSING: /*TSK_DEBUG_INFO("JITTER_BUFFER_MISSING - %d", ret);*/ break;
			case JITTER_BUFFER_INSERTION: /*TSK_DEBUG_INFO("JITTER_BUFFER_INSERTION - %d", ret);*/ break;
			default: TSK_DEBUG_INFO("jitter_buffer_get() failed - %d", ret);
		}
		// jitter_buffer_update_delay(jitterbuffer->state, &jb_packet, NULL);
		return 0;
	}
	jitter_buffer_update_delay(jitterbuffer->state, &jb_packet, NULL);

	return out_size;
}

static int tdav_speex_jitterbuffer_reset(tmedia_jitterbuffer_t* self)
{
	tdav_speex_jitterbuffer_t *jitterbuffer = (tdav_speex_jitterbuffer_t *)self;
	if(jitterbuffer->state){
		jitter_buffer_reset(jitterbuffer->state);
	}
	return 0;
}

static int tdav_speex_jitterbuffer_close(tmedia_jitterbuffer_t* self)
{
	tdav_speex_jitterbuffer_t *jitterbuffer = (tdav_speex_jitterbuffer_t *)self;
	if(jitterbuffer->state){
		jitter_buffer_destroy(jitterbuffer->state);
		jitterbuffer->state = tsk_null;
	}
	return 0;
}



//
//	Speex jitterbufferr Plugin definition
//

/* constructor */
static tsk_object_t* tdav_speex_jitterbuffer_ctor(tsk_object_t * self, va_list * app)
{
	tdav_speex_jitterbuffer_t *jitterbuffer = self;
	if(jitterbuffer){
		/* init base */
		tmedia_jitterbuffer_init(TMEDIA_JITTER_BUFFER(jitterbuffer));
		/* init self */
	}
	return self;
}
/* destructor */
static tsk_object_t* tdav_speex_jitterbuffer_dtor(tsk_object_t * self)
{ 
	tdav_speex_jitterbuffer_t *jitterbuffer = self;
	if(jitterbuffer){
		/* deinit base */
		tmedia_jitterbuffer_deinit(TMEDIA_JITTER_BUFFER(jitterbuffer));
		/* deinit self */
		if(jitterbuffer->state){
			jitter_buffer_destroy(jitterbuffer->state);
			jitterbuffer->state = tsk_null;
		}
	}

	return self;
}
/* object definition */
static const tsk_object_def_t tdav_speex_jitterbuffer_def_s = 
{
	sizeof(tdav_speex_jitterbuffer_t),
	tdav_speex_jitterbuffer_ctor, 
	tdav_speex_jitterbuffer_dtor,
	tsk_null, 
};
/* plugin definition*/
static const tmedia_jitterbuffer_plugin_def_t tdav_speex_jitterbuffer_plugin_def_s = 
{
	&tdav_speex_jitterbuffer_def_s,
	tmedia_audio,
	"Audio JitterBuffer based on Speex",
	
	tdav_speex_jitterbuffer_set,
	tdav_speex_jitterbuffer_open,
	tdav_speex_jitterbuffer_tick,
	tdav_speex_jitterbuffer_put,
	tdav_speex_jitterbuffer_get,
	tdav_speex_jitterbuffer_reset,
	tdav_speex_jitterbuffer_close,
};
const tmedia_jitterbuffer_plugin_def_t *tdav_speex_jitterbuffer_plugin_def_t = &tdav_speex_jitterbuffer_plugin_def_s;


#endif /* HAVE_SPEEX_DSP */
