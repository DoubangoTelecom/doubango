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
#include "tinymedia/tmedia_producer.h"

#include "tinyrtp/trtp_manager.h"
#include "tinyrtp/rtp/trtp_rtp_packet.h"

#include "tsk_memory.h"
#include "tsk_debug.h"

// RTP/RTCP callback (From the network to the consumer)
static int tdav_session_audio_rtp_cb(const void* callback_data, const struct trtp_rtp_packet_s* packet)
{
	tdav_session_audio_t* audio = (tdav_session_audio_t*)callback_data;

	if(!audio || !packet){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(audio->consumer){
		void* out_data = tsk_null;
		tsk_size_t out_size = 0;
		tmedia_codec_t* codec;
		tsk_istr_t format;

		// Find the codec to use to decode the RTP payload
		tsk_itoa(packet->header->payload_type, &format);
		if(!(codec = tmedia_codec_find_by_format(TMEDIA_SESSION(audio)->neg_codecs, format)) || !codec->plugin || !codec->plugin->decode){
			TSK_DEBUG_ERROR("%s is not a valid payload for this session", format);
			TSK_OBJECT_SAFE_FREE(codec);
			return -2;
		}
		// Decode data
		out_size = codec->plugin->decode(codec, packet->payload.data, packet->payload.size, &out_data);
		if(out_size){
			tmedia_consumer_consume(audio->consumer, &out_data, out_size, packet->header);
		}
		TSK_FREE(out_data);
		TSK_OBJECT_SAFE_FREE(codec);
	}
	return 0;
}

// Producer callback (From the producer to the network)
static int tdav_session_audio_producer_cb(const void* callback_data, const void* buffer, tsk_size_t size)
{
	tdav_session_audio_t* audio = (tdav_session_audio_t*)callback_data;

	if(audio->rtp_manager){
		/* encode */
		void* out_data = tsk_null;
		tsk_size_t out_size = 0;
		tmedia_codec_t* codec = tsk_null;
		
		// Use first codec to encode data
		if((codec = tsk_object_ref(TSK_LIST_FIRST_DATA(TMEDIA_SESSION(audio)->neg_codecs)))){
			if(!codec->plugin || !codec->plugin->encode){
				TSK_OBJECT_SAFE_FREE(codec);
				TSK_DEBUG_ERROR("Invalid codec");
				return -2;
			}
		}
		else{
			TSK_DEBUG_ERROR("Failed to find a valid codec");
			return -3;
		}

		// Encode data
		out_size = codec->plugin->encode(codec, buffer, size, &out_data);
		if(out_size){
			trtp_manager_send_rtp(audio->rtp_manager, out_data, out_size, 160/*FIXME*/, tsk_false);
		}
		TSK_FREE(out_data);
		TSK_OBJECT_SAFE_FREE(codec);
	}

	return 0;
}


/* ============ Plugin interface ================= */

int tmedia_session_audio_set(tmedia_session_t* self, const tmedia_param_t* param)
{
	int ret = 0;
	tdav_session_audio_t* audio;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	TSK_DEBUG_INFO("tmedia_session_audio_set");

	audio = (tdav_session_audio_t*)self;

	if(param->value_type == tmedia_pvt_pchar){
		if(tsk_striequals(param->key, "remote-ip")){
			/* only if no ip associated to the "m=" line */
			if(param->value && !audio->remote_ip){
				audio->remote_ip = tsk_strdup(param->value);
			}
		}
		else if(tsk_striequals(param->key, "local-ip")){
			tsk_strupdate(&audio->local_ip, param->value);
		}
		else if(tsk_striequals(param->key, "local-ipver")){
			audio->useIPv6 = tsk_striequals(param->value, "ipv6");
		}
	}

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

	if(audio->rtp_manager && !TSK_LIST_IS_EMPTY(self->neg_codecs)){
		int ret;
		const tmedia_codec_t* codec = (const tmedia_codec_t*)TSK_LIST_FIRST_DATA(self->neg_codecs);
		/* RTP/RTCP manager: use latest information. */
		ret = trtp_manager_set_rtp_remote(audio->rtp_manager, audio->remote_ip, audio->remote_port);
		trtp_manager_set_payload_type(audio->rtp_manager, codec->neg_format ? atoi(codec->neg_format) : atoi(codec->format));
		ret = trtp_manager_start(audio->rtp_manager);
	
		/* Consumer */
		if(audio->consumer){
			tmedia_consumer_prepare(audio->consumer, codec);
			tmedia_consumer_start(audio->consumer);
		}
		/* Producer */
		if(audio->producer){
			tmedia_producer_prepare(audio->producer, codec);
			tmedia_producer_start(audio->producer);
		}

		/* for test */
		//trtp_manager_send_rtp(audio->rtp_manager, "test", 4, tsk_true);
		return ret;
	}
	else{
		TSK_DEBUG_ERROR("Invalid RTP/RTCP manager or neg_codecs");
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
	/* Producer */
	if(audio->producer){
		tmedia_producer_stop(audio->producer);
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
	/* Producer */
	if(audio->producer){
		tmedia_producer_pause(audio->producer);
	}

	return 0;
}

const tsdp_header_M_t* tdav_session_audio_get_lo(tmedia_session_t* self)
{
	tdav_session_audio_t* audio;
	tsk_bool_t changed = tsk_false;

	TSK_DEBUG_INFO("tdav_session_audio_get_lo");

	if(!self || !self->plugin){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}

	audio = (tdav_session_audio_t*)self;

	if(!audio->rtp_manager || !audio->rtp_manager->transport){
		TSK_DEBUG_ERROR("RTP/RTCP manager in invalid");
		return tsk_null;
	}

	if(self->ro_changed && self->M.lo){
		/* Codecs */
		tsdp_header_A_removeAll_by_field(self->M.lo->Attributes, "fmtp");
		tsdp_header_A_removeAll_by_field(self->M.lo->Attributes, "rtpmap");
		tsk_list_clear_items(self->M.lo->FMTs);
		
		/* QoS */
		tsdp_header_A_removeAll_by_field(self->M.lo->Attributes, "curr");
		tsdp_header_A_removeAll_by_field(self->M.lo->Attributes, "des");
		tsdp_header_A_removeAll_by_field(self->M.lo->Attributes, "conf");
	}

	changed = (self->ro_changed || !self->M.lo);

	if(!self->M.lo){
		if((self->M.lo = tsdp_header_M_create(self->plugin->media, audio->rtp_manager->transport->master->port, "RTP/AVP"))){
			/* 3GPP TS 24.229 - 6.1.1 General
				In order to support accurate bandwidth calculations, the UE may include the "a=ptime" attribute for all "audio" media
				lines as described in RFC 4566 [39]. If a UE receives an "audio" media line with "a=ptime" specified, the UE should
				transmit at the specified packetization rate. If a UE receives an "audio" media line which does not have "a=ptime"
				specified or the UE does not support the "a=ptime" attribute, the UE should transmit at the default codec packetization
				rate as defined in RFC 3551 [55A]. The UE will transmit consistent with the resources available from the network.

				For "video" and "audio" media types that utilize the RTP/RTCP, the UE shall specify the proposed bandwidth for each
				media stream utilizing the "b=" media descriptor and the "AS" bandwidth modifier in the SDP.

				The UE shall include the MIME subtype "telephone-event" in the "m=" media descriptor in the SDP for audio media
				flows that support both audio codec and DTMF payloads in RTP packets as described in RFC 4733 [23].
			*/
			tsdp_header_M_add_headers(self->M.lo,
				TSDP_HEADER_A_VA_ARGS("ptime", "20"),
				tsk_null);
			// the "telephone-event" fmt/rtpmap is added below
		}
		else{
			TSK_DEBUG_ERROR("Failed to create lo");
			return tsk_null;
		}
	}

	/* from codecs to sdp */
	if(changed){
		tmedia_codecs_L_t* neg_codecs = tsk_null;
		
		if(self->M.ro){
			/* update negociated codecs */
			if((neg_codecs = tmedia_session_match_codec(self, self->M.ro))){
				TSK_OBJECT_SAFE_FREE(self->neg_codecs);
				self->neg_codecs = neg_codecs;
			}
		}

		/* from codecs to sdp */
		tmedia_codec_to_sdp(self->neg_codecs ? self->neg_codecs : self->codecs, self->M.lo);
		/* Hold/Resume */
		if(self->M.ro){
			if(tsdp_header_M_is_held(self->M.ro, tsk_false)){
				tsdp_header_M_hold(self->M.lo, tsk_false);
			}
			else{
				tsdp_header_M_resume(self->M.lo, tsk_false);
			}
		}
		/* 3GPP TS 24.229 - 6.1.1 General
			The UE shall include the MIME subtype "telephone-event" in the "m=" media descriptor in the SDP for audio media
			flows that support both audio codec and DTMF payloads in RTP packets as described in RFC 4733 [23].
		*/
		tsdp_header_M_add_fmt(self->M.lo, TMEDIA_CODEC_FORMAT_DTMF);
		tsdp_header_M_add_headers(self->M.lo,
					TSDP_HEADER_A_VA_ARGS("fmtp", TMEDIA_CODEC_FORMAT_DTMF" 0-15"),
				tsk_null);
		tsdp_header_M_add_headers(self->M.lo,
					TSDP_HEADER_A_VA_ARGS("rtpmap", TMEDIA_CODEC_FORMAT_DTMF" telephone-event/8000"),
				tsk_null);
		/* QoS */
		if(self->qos){
			tmedia_qos_tline_t* ro_tline;
			if(self->M.ro && (ro_tline = tmedia_qos_tline_from_sdp(self->M.ro))){
				tmedia_qos_tline_set_ro(self->qos, ro_tline);
				TSK_OBJECT_SAFE_FREE(ro_tline);
			}
			tmedia_qos_tline_to_sdp(self->qos, self->M.lo);
		}
	}

	return self->M.lo;
}

int tdav_session_audio_set_ro(tmedia_session_t* self, const tsdp_header_M_t* m)
{
	tdav_session_audio_t* audio;
	tmedia_codecs_L_t* neg_codecs;

	TSK_DEBUG_INFO("tdav_session_audio_set_ro");

	if(!self || !m){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	audio = (tdav_session_audio_t*)self;

	/* update remote offer */
	TSK_OBJECT_SAFE_FREE(self->M.ro);
	self->M.ro = tsk_object_ref((void*)m);

	if(self->M.lo){
		if((neg_codecs = tmedia_session_match_codec(self, m))){
			/* update negociated codecs */
			TSK_OBJECT_SAFE_FREE(self->neg_codecs);
			self->neg_codecs = neg_codecs;
		}
		else{
			return -1;
		}
	}

	/* get connection associated to this media line
	* If the connnection is global, then the manager will call tmedia_session_audio_set() */
	if(m->C && m->C->addr){
		tsk_strupdate(&audio->remote_ip, m->C->addr);
		audio->useIPv6 = tsk_striequals(m->C->addrtype, "IP6");
	}
	/* set remote port */
	audio->remote_port = m->port;
	

	return 0;
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
		if((session->producer = tmedia_producer_create(tdav_session_audio_plugin_def_t->type))){
			tmedia_producer_set_callback(session->producer, tdav_session_audio_producer_cb, self);
		}
		else{
			TSK_DEBUG_ERROR("Failed to create Audio producer");
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
		TSK_OBJECT_SAFE_FREE(session->producer);
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
