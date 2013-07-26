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

// rfc3551 - 4.5 Audio Encodings: all frames length are multiple of 10ms

#include "tinymedia/tmedia_defaults.h"

#include "tsk_memory.h"
#include "tsk_debug.h"

static int tdav_speex_jitterbuffer_set(tmedia_jitterbuffer_t *self, const tmedia_param_t* param)
{
	TSK_DEBUG_ERROR("Not implemented");
	return -2;
}

static int tdav_speex_jitterbuffer_open(tmedia_jitterbuffer_t* self, uint32_t frame_duration, uint32_t rate, uint32_t channels)
{
	tdav_speex_jitterbuffer_t *jitterbuffer = (tdav_speex_jitterbuffer_t *)self;
	spx_int32_t tmp;

	TSK_DEBUG_INFO("Open speex jb (ptime=%u, rate=%u)", frame_duration, rate);

	if(!(jitterbuffer->state = jitter_buffer_init((int)frame_duration))){
		TSK_DEBUG_ERROR("jitter_buffer_init() failed");
		return -2;
	}
	jitterbuffer->rate = rate;
	jitterbuffer->frame_duration = frame_duration;
	jitterbuffer->channels = channels;
	jitterbuffer->x_data_size = ((frame_duration * jitterbuffer->rate) / 500) << (channels == 2 ? 1 : 0);

	jitter_buffer_ctl(jitterbuffer->state, JITTER_BUFFER_GET_MARGIN, &tmp);
	TSK_DEBUG_INFO("Default Jitter buffer margin=%d", tmp);
	jitter_buffer_ctl(jitterbuffer->state, JITTER_BUFFER_GET_MAX_LATE_RATE, &tmp);
	TSK_DEBUG_INFO("Default Jitter max late rate=%d", tmp);
	
	if((tmp = tmedia_defaults_get_jb_margin()) >= 0){
		jitter_buffer_ctl(jitterbuffer->state, JITTER_BUFFER_SET_MARGIN, &tmp);
		TSK_DEBUG_INFO("New Jitter buffer margin=%d", tmp);
	}
	if((tmp = tmedia_defaults_get_jb_max_late_rate()) >= 0){
		jitter_buffer_ctl(jitterbuffer->state, JITTER_BUFFER_SET_MAX_LATE_RATE, &tmp);
		TSK_DEBUG_INFO("New Jitter buffer max late rate=%d", tmp);
	}

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
	tdav_speex_jitterbuffer_t *jb = (tdav_speex_jitterbuffer_t *)self;
    const trtp_rtp_header_t* rtp_hdr;
    JitterBufferPacket jb_packet;
	static uint16_t seq_num = 0;

    if(!data || !data_size || !proto_hdr){
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    
    if(!jb->state){
        TSK_DEBUG_ERROR("Invalid state");
        return -2;
    }
    
    rtp_hdr = TRTP_RTP_HEADER(proto_hdr);

	jb_packet.user_data = 0;
	jb_packet.span = jb->frame_duration;
	jb_packet.len = jb->x_data_size;
    
	if(jb->x_data_size == data_size){ /* ptime match */
		jb_packet.data = data;
		jb_packet.sequence = rtp_hdr->seq_num;
		jb_packet.timestamp = (rtp_hdr->seq_num * jb_packet.span);
		jitter_buffer_put(jb->state, &jb_packet);
	}
	else{ /* ptime mismatch */
		tsk_size_t i;
		jb_packet.sequence = 0; // Ignore
		if((jb->buff.index + data_size) > jb->buff.size){
			if(!(jb->buff.ptr = tsk_realloc(jb->buff.ptr, (jb->buff.index + data_size)))){
				jb->buff.size = 0;
				jb->buff.index = 0;
				return 0;
			}
			jb->buff.size = (jb->buff.index + data_size);
		}

		memcpy(&jb->buff.ptr[jb->buff.index], data, data_size);
		jb->buff.index += data_size;

		if(jb->buff.index >= jb->x_data_size){
			tsk_size_t copied = 0;
			for(i = 0; (i + jb->x_data_size) <= jb->buff.index; i += jb->x_data_size){
				jb_packet.data = (char*)&jb->buff.ptr[i];
				jb_packet.timestamp = (++jb->fake_seqnum * jb_packet.span);// reassembled pkt will have fake seqnum
				jitter_buffer_put(jb->state, &jb_packet);
				copied += jb->x_data_size;
			}
			if(copied == jb->buff.index){
				// all copied
				jb->buff.index = 0;
			}
			else{
				memmove(&jb->buff.ptr[0], &jb->buff.ptr[copied], (jb->buff.index - copied));
				jb->buff.index -= copied;
			}
		}
	}
    
    return 0;
}

static tsk_size_t tdav_speex_jitterbuffer_get(tmedia_jitterbuffer_t* self, void* out_data, tsk_size_t out_size)
{
	tdav_speex_jitterbuffer_t *jb = (tdav_speex_jitterbuffer_t *)self;
    JitterBufferPacket jb_packet;
    int ret;

    if(!out_data || !out_size){
        TSK_DEBUG_ERROR("Invalid parameter");
        return 0;
    }
    if(!jb->state){
        TSK_DEBUG_ERROR("Invalid state");
        return 0;
    }
	if(jb->x_data_size != out_size){ // consumer must request PTIME data
		TSK_DEBUG_WARN("%d not expected as frame size. %u<>%u", out_size, jb->frame_duration, (out_size * 500)/jb->rate);
		return 0;
	}

    jb_packet.data = out_data;
    jb_packet.len = out_size;

	if ((ret = jitter_buffer_get(jb->state, &jb_packet, jb->frame_duration/*(out_size * 500)/jb->rate*/, tsk_null)) != JITTER_BUFFER_OK) {
        switch(ret){
            case JITTER_BUFFER_MISSING: /*TSK_DEBUG_INFO("JITTER_BUFFER_MISSING - %d", ret);*/ break;
            case JITTER_BUFFER_INSERTION: /*TSK_DEBUG_INFO("JITTER_BUFFER_INSERTION - %d", ret);*/ break;
            default: TSK_DEBUG_INFO("jitter_buffer_get() failed - %d", ret); break;
        }
        jitter_buffer_update_delay(jb->state, &jb_packet, NULL);
        return 0;
    }
    // jitter_buffer_update_delay(jitterbuffer->state, &jb_packet, NULL);

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
	TSK_DEBUG_INFO("Create SpeexDSP jitter buffer");
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
	tdav_speex_jitterbuffer_t *jb = self;
	if(jb){
		/* deinit base */
		tmedia_jitterbuffer_deinit(TMEDIA_JITTER_BUFFER(jb));
		/* deinit self */
		if(jb->state){
			jitter_buffer_destroy(jb->state);
			jb->state = tsk_null;
		}
		TSK_FREE(jb->buff.ptr);

		TSK_DEBUG_INFO("*** SpeexDSP jb destroyed ***");
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
