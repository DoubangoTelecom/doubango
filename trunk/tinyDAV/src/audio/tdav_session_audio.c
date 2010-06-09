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

/**@file tdav_session_audio.c
 * @brief Audio Session plugin.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinydav/audio/tdav_session_audio.h"

#include "tinymedia/tmedia_consumer.h"

#include "tinyrtp/trtp_manager.h"
#include "tinyrtp/rtp/trtp_rtp_packet.h"

#include "tsk_memory.h"
#include "tsk_debug.h"

extern const tmedia_codec_t* _tmedia_session_match_codec(tmedia_session_t* self, const tsdp_header_M_t* M, char** format);


static int tdav_session_audio_rtp_cb(const void* callback_data, const struct trtp_rtp_packet_s* packet)
{
	tdav_session_audio_t* audio = (tdav_session_audio_t*)callback_data;

	if(!audio || !packet){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(audio->consumer){
		/* decode data--> FIXME:we should lock negociated codec */
		if(TMEDIA_SESSION(audio)->negociated_codec && TMEDIA_SESSION(audio)->negociated_codec->plugin && TMEDIA_SESSION(audio)->negociated_codec->plugin->decode){
			void* out_data = tsk_null;
			tsk_size_t out_size;
			out_size = TMEDIA_SESSION(audio)->negociated_codec->plugin->decode(TMEDIA_SESSION(audio)->negociated_codec, packet->payload.data, packet->payload.size, &out_data);
			if(out_size){
				tmedia_consumer_consume(audio->consumer, &out_data, out_size);
			}
			TSK_FREE(out_data);
		}
	}
	return 0;
}


/* ============ Plugin interface ================= */

int tmedia_session_audio_set(tmedia_session_t* self, va_list *app)
{
	int ret = 0;
	tdav_session_audio_t* audio;
	tmedia_session_param_type_t type;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	TSK_DEBUG_INFO("tmedia_session_audio_set");

	audio = (tdav_session_audio_t*)self;

	while((type = va_arg(*app, tmedia_session_param_type_t)) != tmedia_sptype_null){
		switch(type){
			case tmedia_sptype_remote_ip:
				{	/* (const char*) IP_STR */
					const char* ip = va_arg(*app, const char *);
					/* only if no ip associated to the "m=" line */
					if(ip && !audio->remote_ip){
						audio->remote_ip = tsk_strdup(ip);
					}
					break;
				}
			case tmedia_sptype_local_ip:
				{	/* IP_STR, IPv6_BOOL */
					tsk_strupdate(&audio->local_ip, va_arg(*app, const char *));
					audio->useIPv6 = va_arg(*app, tsk_bool_t);
					break;
				}
			case tmedia_sptype_set_rtcp:
				{/* (tsk_bool_t)ENABLED_BOOL */
					audio->rtcp_enabled = va_arg(*app, tsk_bool_t);
					break;
				}
			default:
				{	/* MUST exit if unknown parameter */
					if((ret = tmedia_session_skip_param(type, app))){
						TSK_DEBUG_ERROR("Unknown parameter");
						goto bail;
					}
					break;
				}
		}
	}

bail:
	return ret;
}

int tdav_session_audio_prepare(tmedia_session_t* self)
{
	tdav_session_audio_t* audio;
	int ret = 0;

	TSK_DEBUG_INFO("tdav_session_audio_prepare");

	audio = (tdav_session_audio_t*)self;

	/* set local port */
	if(!audio->rtp_manager){
		if((audio->rtp_manager = trtp_manager_create(audio->rtcp_enabled, audio->local_ip, audio->useIPv6))){
			
			ret = trtp_manager_set_rtp_callback(audio->rtp_manager, tdav_session_audio_rtp_cb, audio);
			ret = trtp_manager_prepare(audio->rtp_manager);
		}
	}

	/* Consumer will be prepared in tdav_session_audio_start() */
	/* Producer will be prepared in tdav_session_audio_start() */

	return ret;
}

int tdav_session_audio_start(tmedia_session_t* self)
{
	tdav_session_audio_t* audio;

	TSK_DEBUG_INFO("tdav_session_audio_start");

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	audio = (tdav_session_audio_t*)self;

	if(audio->rtp_manager){
		int ret;
		/* RTP/RTCP manager: use latest information. */
		ret = trtp_manager_set_rtp_remote(audio->rtp_manager, audio->remote_ip, audio->remote_port);
		trtp_manager_set_payload_type(audio->rtp_manager, self->negociated_format ? atoi(self->negociated_format) : 255);
		ret = trtp_manager_start(audio->rtp_manager);
	
		/* Consumer */
		if(audio->consumer){
			tmedia_consumer_prepare(audio->consumer, self->negociated_codec);
			tmedia_consumer_start(audio->consumer);
		}

		/* for test */
		//trtp_manager_send_rtp(audio->rtp_manager, "test", 4, tsk_true);
		return ret;
	}
	else{
		TSK_DEBUG_ERROR("Invalid RTP/RTCP manager");
		return -2;
	}
	
	return 0;
}

int tdav_session_audio_stop(tmedia_session_t* self)
{
	tdav_session_audio_t* audio;

	TSK_DEBUG_INFO("tdav_session_audio_stop");

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	audio = (tdav_session_audio_t*)self;

	/* RTP/RTCP manager */
	if(audio->rtp_manager){
		trtp_manager_stop(audio->rtp_manager);
	}

	/* Consumer */
	if(audio->consumer){
		tmedia_consumer_stop(audio->consumer);
	}

	/* very important */
	//audio->local_port = 0;

	return 0;
}

int tdav_session_audio_pause(tmedia_session_t* self)
{
	tdav_session_audio_t* audio;

	TSK_DEBUG_INFO("tdav_session_audio_pause");

	audio = (tdav_session_audio_t*)self;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	/* Consumer */
	if(audio->consumer){
		tmedia_consumer_pause(audio->consumer);
	}

	return 0;
}

const tsdp_header_M_t* tdav_session_audio_get_lo(tmedia_session_t* self)
{
	tdav_session_audio_t* audio;

	TSK_DEBUG_INFO("tdav_session_audio_get_lo");

	if(!self || !self->plugin){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}

	audio = (tdav_session_audio_t*)self;

	if(!audio->rtp_manager || !audio->rtp_manager->rtp.local_socket){
		TSK_DEBUG_ERROR("RTP/RTCP manager in invalid");
		return tsk_null;
	}

	if(self->ro_changed && self->M.lo){
		TSK_OBJECT_SAFE_FREE(self->M.lo);
	}

	if(self->M.lo){
		return self->M.lo;
	}
	else if(!(self->M.lo = tsdp_header_M_create(self->plugin->media, audio->rtp_manager->rtp.local_socket->port, "RTP/AVP"))){
		TSK_DEBUG_ERROR("Failed to create lo");
		return tsk_null;
	}

	/* codec to sdp */
	if(self->negociated_codec){
		tmedia_codec_to_sdp_2(self->codecs->head->data, self->M.lo, self->negociated_format);
	}
	else{
		tmedia_codec_to_sdp(self->codecs, self->M.lo);
	}

	return self->M.lo;
}

int tdav_session_audio_set_ro(tmedia_session_t* self, const tsdp_header_M_t* m)
{
	const tmedia_codec_t* codec;
	tdav_session_audio_t* audio;

	TSK_DEBUG_INFO("tdav_session_audio_set_ro");

	if(!self || !m){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	audio = (tdav_session_audio_t*)self;

	if((codec = tmedia_session_match_codec(self, m, &self->negociated_format))){
		/* update negociated codec */
		TSK_OBJECT_SAFE_FREE(self->negociated_codec);
		self->negociated_codec = tsk_object_ref((void*)codec);
		/* filter codecs */
		tmedia_codec_removeAll_exceptThis(self->codecs, self->negociated_codec);
		/* update remote offer */
		TSK_OBJECT_SAFE_FREE(self->M.ro);
		self->M.ro = tsk_object_ref((void*)m);
	
		/* get connection associated to this media line
		* If the connnection is at session-level, then the manager will call tmedia_session_audio_set() */
		if(m->C && m->C->addr){
			tsk_strupdate(&audio->remote_ip, m->C->addr);
			audio->useIPv6 = tsk_striequals(m->C->addrtype, "IP6");
		}
		/* set remote port */
		audio->remote_port = m->port;

		return 0;
	}

	return -1;
}





//=================================================================================================
//	Session Audio Plugin object definition
//
/* constructor */
static tsk_object_t* tdav_session_audio_ctor(tsk_object_t * self, va_list * app)
{
	tdav_session_audio_t *session = self;
	if(session){
		/* init base: called by tmedia_session_create() */
		/* init self */
		if(!(session->consumer = tmedia_consumer_create(tdav_session_audio_plugin_def_t->type))){
			TSK_DEBUG_ERROR("Failed to create Audio consumer");
		}
	}
	return self;
}
/* destructor */
static tsk_object_t* tdav_session_audio_dtor(tsk_object_t * self)
{ 
	tdav_session_audio_t *session = self;
	if(session){
		/* deinit base */
		tmedia_session_deinit(self);
		/* deinit self */
		TSK_OBJECT_SAFE_FREE(session->rtp_manager);
		TSK_OBJECT_SAFE_FREE(session->consumer);
		TSK_FREE(session->remote_ip);
		TSK_FREE(session->local_ip);
	}

	return self;
}
/* object definition */
static const tsk_object_def_t tdav_session_audio_def_s = 
{
	sizeof(tdav_session_audio_t),
	tdav_session_audio_ctor, 
	tdav_session_audio_dtor,
	tmedia_session_cmp, 
};
/* plugin definition*/
static const tmedia_session_plugin_def_t tdav_session_audio_plugin_def_s = 
{
	&tdav_session_audio_def_s,
	
	tmedia_audio,
	"audio",
	
	tmedia_session_audio_set,
	tdav_session_audio_prepare,
	tdav_session_audio_start,
	tdav_session_audio_pause,
	tdav_session_audio_stop,
	

	tdav_session_audio_get_lo,
	tdav_session_audio_set_ro
};
const tmedia_session_plugin_def_t *tdav_session_audio_plugin_def_t = &tdav_session_audio_plugin_def_s;