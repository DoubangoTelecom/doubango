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

/**@file tdav_session_video.c
 * @brief Video Session plugin.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinydav/video/tdav_session_video.h"

#include "tinydav/video/tdav_converter_video.h"

#include "tinymedia/tmedia_consumer.h"
#include "tinymedia/tmedia_producer.h"

#include "tinyrtp/trtp_manager.h"
#include "tinyrtp/rtp/trtp_rtp_packet.h"

#include "tsk_memory.h"
#include "tsk_debug.h"

// RTP/RTCP callback (From the network to the consumer)
static int tdav_session_video_rtp_cb(const void* callback_data, const struct trtp_rtp_packet_s* packet)
{
	tdav_session_video_t* session = (tdav_session_video_t*)callback_data;
	int ret = 0;

	if(!session || !packet){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(session->consumer){
		tmedia_codec_t* codec;
		tsk_istr_t format;
		tsk_size_t out_size = 0;

		// Find the codec to use to decode the RTP payload
		tsk_itoa(packet->header->payload_type, &format);
		if(!(codec = tmedia_codec_find_by_format(TMEDIA_SESSION(session)->neg_codecs, format)) || !codec->plugin || !codec->plugin->decode){
			TSK_DEBUG_ERROR("%s is not a valid payload for this session", format);
			ret = -2;
			goto bail;
		}
		// Open codec if not already done
		if(!TMEDIA_CODEC(codec)->opened){
			tsk_safeobj_lock(session);
			if((ret = tmedia_codec_open(codec))){
				tsk_safeobj_unlock(session);
				TSK_DEBUG_ERROR("Failed to open [%s] codec", codec->plugin->desc);
				goto bail;
			}
			tsk_safeobj_unlock(session);
		}
		// Decode data
		out_size = codec->plugin->decode(codec, packet->payload.data, packet->payload.size, &session->decoder.buffer, &session->decoder.buffer_size, packet->header);
		// check
		if(!out_size || !session->decoder.buffer){
			goto bail;
		}

		// Convert decoded data to the consumer chroma and size
#define CONSUMER_SIZE_CHANGED (session->conv.consumerWidth != session->consumer->video.width) || (session->conv.consumerHeight != session->consumer->video.height) \
		|| (session->conv.xConsumerSize != out_size)
		if((session->consumer->video.chroma != tmedia_yuv420p) || CONSUMER_SIZE_CHANGED){
			tsk_size_t _output_size;
			// Create video converter if not already done
			if(!session->conv.fromYUV420 || CONSUMER_SIZE_CHANGED){
				const tmedia_video_size_t* video_size = tmedia_get_video_size(tmedia_yuv420p, out_size);
				TSK_OBJECT_SAFE_FREE(session->conv.fromYUV420);
				session->conv.consumerWidth = session->consumer->video.width;
				session->conv.consumerHeight = session->consumer->video.height;
				session->conv.xConsumerSize = (tsk_size_t)(((float)(video_size->width * video_size->height)) * 1.5f/*YUV420P*/);
				if(!(session->conv.fromYUV420 = tdav_converter_video_create(video_size->width, video_size->height, session->conv.consumerWidth, session->conv.consumerHeight,
					session->consumer->video.chroma, tsk_false))){
					TSK_DEBUG_ERROR("Failed to create video converter");
					ret = -3;
					goto bail;
				}
			}
			// convert data to the consumer's chroma
			_output_size = tdav_converter_video_convert(session->conv.fromYUV420, session->decoder.buffer, &session->decoder.conv_buffer, &session->decoder.conv_buffer_size);
			if(!_output_size || !session->decoder.conv_buffer){
				TSK_DEBUG_ERROR("Failed to convert YUV420 buffer to consumer's chroma");
				ret = -4;
				goto bail;
			}

			tmedia_consumer_consume(session->consumer, &session->decoder.conv_buffer, _output_size, packet->header);
			if(!session->decoder.conv_buffer){
				/* taken  by the consumer */
				session->decoder.conv_buffer_size = 0;
			}
			
		}
		else{
			tmedia_consumer_consume(session->consumer, &session->decoder.buffer, out_size, packet->header);
			if(!session->decoder.buffer){
				/* taken  by the consumer */
				session->decoder.buffer_size = 0;
			}
		}

bail:
		TSK_OBJECT_SAFE_FREE(codec);
	}
	return ret;
}

// Codec callback (From codec to the network)
static int tdav_session_video_codec_cb(const void* callback_data, const void* buffer, tsk_size_t size, uint32_t duration, tsk_bool_t marker)
{
	tdav_session_video_t* session = (tdav_session_video_t*)callback_data;
	
	if(session->rtp_manager){
		return trtp_manager_send_rtp(session->rtp_manager, buffer, size, duration, marker, marker);
	}

	return 0;
}

// Producer callback (From the producer to the network)
static int tdav_session_video_producer_cb(const void* callback_data, const void* buffer, tsk_size_t size)
{
	tdav_session_video_t* session = (tdav_session_video_t*)callback_data;
	tsk_size_t yuv420p_size = 0;
	int ret = 0;

	if(session && session->rtp_manager){
		/* encode */
		tsk_size_t out_size = 0;
		tmedia_codec_t* codec = tsk_null;

		// Use first codec to encode data
		if((codec = tsk_object_ref(TSK_LIST_FIRST_DATA(TMEDIA_SESSION(session)->neg_codecs)))){
			if(!codec->plugin || !codec->plugin->encode){
				TSK_OBJECT_SAFE_FREE(codec);
				TSK_DEBUG_ERROR("Invalid codec");
				return -2;
			}
			// open the codec if not already done
			if(!TMEDIA_CODEC(codec)->opened){
				tsk_safeobj_lock(session);
				if((ret = tmedia_codec_open(codec))){
					tsk_safeobj_unlock(session);
					TSK_DEBUG_ERROR("Failed to open [%s] codec", codec->plugin->desc);
					TSK_OBJECT_SAFE_FREE(codec);
					return ret;
				}
				tsk_safeobj_unlock(session);
			}
		}
		else{
			TSK_DEBUG_ERROR("Failed to find a valid codec");
			return -4;
		}

	
#define PRODUCER_SIZE_CHANGED (session->conv.producerWidth != session->producer->video.width) || (session->conv.producerHeight != session->producer->video.height) \
|| (session->conv.xProducerSize != size)
		// Video codecs only accept YUV420P buffers ==> do conversion if needed or producer doesn't have the right size
		if((session->producer->video.chroma != tmedia_yuv420p) || PRODUCER_SIZE_CHANGED){
			// Create video converter if not already done or producer size has changed
			if(!session->conv.toYUV420 || PRODUCER_SIZE_CHANGED){
				TSK_OBJECT_SAFE_FREE(session->conv.toYUV420);
				session->conv.producerWidth = session->producer->video.width;
				session->conv.producerHeight = session->producer->video.height;
				session->conv.xProducerSize = size;
				
				if(!(session->conv.toYUV420 = tdav_converter_video_create(session->producer->video.width, session->producer->video.height, TMEDIA_CODEC_VIDEO(codec)->width, TMEDIA_CODEC_VIDEO(codec)->height,
					session->producer->video.chroma, tsk_true))){
					TSK_DEBUG_ERROR("Failed to create video converter");
					ret = -5;
					goto bail;
				}
			}
			// update one-shot parameters
			tdav_converter_video_init(session->conv.toYUV420, session->producer->video.rotation);
			// convert data to yuv420p
			yuv420p_size = tdav_converter_video_convert(session->conv.toYUV420, buffer, &session->encoder.conv_buffer, &session->encoder.conv_buffer_size);
			if(!yuv420p_size || !session->encoder.conv_buffer){
				TSK_DEBUG_ERROR("Failed to convert XXX buffer to YUV42P");
				ret = -6;
				goto bail;
			}
		}

		// Encode data
		if(session->encoder.conv_buffer && yuv420p_size){
			/* producer doesn't support yuv42p */
			out_size = codec->plugin->encode(codec, session->encoder.conv_buffer, yuv420p_size, &session->encoder.buffer, &session->encoder.buffer_size);
		}
		else{
			/* producer supports yuv42p */
			out_size = codec->plugin->encode(codec, buffer, size,  &session->encoder.buffer, &session->encoder.buffer_size);
		}

		if(out_size){
			/* Never called, see tdav_session_video_codec_cb() */
			trtp_manager_send_rtp(session->rtp_manager, session->encoder.buffer, out_size, 6006, tsk_true, tsk_true);
		}
bail:
		TSK_OBJECT_SAFE_FREE(codec);
	}
	else{
		TSK_DEBUG_ERROR("Invalid parameter");
		ret = -1;
	}

	return ret;
}


/* ============ Plugin interface ================= */

int tmedia_session_video_set(tmedia_session_t* self, const tmedia_param_t* param)
{
	int ret = 0;
	tdav_session_video_t* video;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	video = (tdav_session_video_t*)self;

	if(param->plugin_type == tmedia_ppt_consumer){
		if(!video->consumer){
			TSK_DEBUG_ERROR("No consumer associated to this session");
			return -1;
		}
		ret = tmedia_consumer_set(video->consumer, param);
	}
	else if(param->plugin_type == tmedia_ppt_producer){
		if(!video->producer){
			TSK_DEBUG_ERROR("No producer associated to this session");
			return -1;
		}
		ret = tmedia_producer_set(video->producer, param);
	}
	else{
		if(param->value_type == tmedia_pvt_pchar){
			if(tsk_striequals(param->key, "remote-ip")){
				/* only if no ip associated to the "m=" line */
				if(param->value && !video->remote_ip){
					video->remote_ip = tsk_strdup(param->value);
				}
			}
			else if(tsk_striequals(param->key, "local-ip")){
				tsk_strupdate(&video->local_ip, param->value);
			}
			else if(tsk_striequals(param->key, "local-ipver")){
				video->useIPv6 = tsk_striequals(param->value, "ipv6");
			}
		}
		else if(param->value_type == tmedia_pvt_int32){
			if(tsk_striequals(param->key, "bandwidth-level")){
				tsk_list_item_t* item;
				self->bl = (tmedia_bandwidth_level_t) TSK_TO_UINT32((uint8_t*)param->value);
				self->codecs = tsk_object_ref(self->codecs);
				tsk_list_foreach(item, self->codecs){
					((tmedia_codec_t*)item->data)->bl = self->bl;
				}
				tsk_object_unref(self->codecs);
			}
		}
		else if(param->value_type == tmedia_pvt_pobject){
			if(tsk_striequals(param->key, "natt-ctx")){
				TSK_OBJECT_SAFE_FREE(video->natt_ctx);
				video->natt_ctx = tsk_object_ref(param->value);
			}
		}
	}

	return ret;
}

int tdav_session_video_prepare(tmedia_session_t* self)
{
	tdav_session_video_t* video;
	int ret = 0;

	video = (tdav_session_video_t*)self;

	/* set local port */
	if(!video->rtp_manager){
		if((video->rtp_manager = trtp_manager_create(video->rtcp_enabled, video->local_ip, video->useIPv6))){
			
			ret = trtp_manager_set_rtp_callback(video->rtp_manager, tdav_session_video_rtp_cb, video);
			ret = trtp_manager_prepare(video->rtp_manager);
			if(video->natt_ctx){
				ret = trtp_manager_set_natt_ctx(video->rtp_manager, video->natt_ctx);
			}
		}
	}

	/* Consumer will be prepared in tdav_session_video_start() */
	/* Producer will be prepared in tdav_session_video_start() */

	return ret;
}

int tdav_session_video_start(tmedia_session_t* self)
{
	tdav_session_video_t* video;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	video = (tdav_session_video_t*)self;

	if(TSK_LIST_IS_EMPTY(self->neg_codecs)){
		TSK_DEBUG_ERROR("No codec matched");
		return -2;
	}

	if(video->rtp_manager){
		int ret;
		const tmedia_codec_t* codec = (const tmedia_codec_t*)TSK_LIST_FIRST_DATA(self->neg_codecs);
		/* RTP/RTCP manager: use latest information. */
		ret = trtp_manager_set_rtp_remote(video->rtp_manager, video->remote_ip, video->remote_port);
		trtp_manager_set_payload_type(video->rtp_manager, codec->neg_format ? atoi(codec->neg_format) : atoi(codec->format));
		ret = trtp_manager_start(video->rtp_manager);
	
		/* Consumer */
		if(video->consumer){
			tmedia_consumer_prepare(video->consumer, codec);
			tmedia_consumer_start(video->consumer);
		}
		/* Producer */
		if(video->producer){
			tmedia_producer_prepare(video->producer, codec);
			tmedia_producer_start(video->producer);
		}

		/* for test */
		//trtp_manager_send_rtp(video->rtp_manager, "test", 4, tsk_true);
		return ret;
	}
	else{
		TSK_DEBUG_ERROR("Invalid RTP/RTCP manager or neg_codecs");
		return -3;
	}
}

int tdav_session_video_stop(tmedia_session_t* self)
{
	tdav_session_video_t* video;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	video = (tdav_session_video_t*)self;

	/* RTP/RTCP manager */
	if(video->rtp_manager){
		trtp_manager_stop(video->rtp_manager);
	}

	/* Consumer */
	if(video->consumer){
		tmedia_consumer_stop(video->consumer);
	}
	/* Producer */
	if(video->producer){
		tmedia_producer_stop(video->producer);
	}

	return 0;
}

int tdav_session_video_pause(tmedia_session_t* self)
{
	tdav_session_video_t* video;

	video = (tdav_session_video_t*)self;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	/* Consumer */
	if(video->consumer){
		tmedia_consumer_pause(video->consumer);
	}
	/* Producer */
	if(video->producer){
		tmedia_producer_pause(video->producer);
	}

	return 0;
}

const tsdp_header_M_t* tdav_session_video_get_lo(tmedia_session_t* self)
{
	tdav_session_video_t* video;
	tsk_bool_t changed = tsk_false;

	if(!self || !self->plugin){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}

	video = (tdav_session_video_t*)self;

	if(!video->rtp_manager || !video->rtp_manager->transport){
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
		if((self->M.lo = tsdp_header_M_create(self->plugin->media, video->rtp_manager->rtp.public_port, "RTP/AVP"))){
			/* If NATT is active, do not rely on the global IP address Connection line */
			if(video->natt_ctx){
				tsdp_header_M_add_headers(self->M.lo,
					TSDP_HEADER_C_VA_ARGS("IN", video->useIPv6 ? "IP6" : "IP4", video->rtp_manager->rtp.public_ip),
					tsk_null);
			}
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
			TSK_OBJECT_SAFE_FREE(self->neg_codecs);
			/* update negociated codecs */
			if((neg_codecs = tmedia_session_match_codec(self, self->M.ro))){
				self->neg_codecs = neg_codecs;
				// set video codec callback
				if(!TSK_LIST_IS_EMPTY(self->neg_codecs)){
					tmedia_codec_video_set_callback((tmedia_codec_video_t*)TSK_LIST_FIRST_DATA(self->neg_codecs), tdav_session_video_codec_cb, self);
				}
			}
			/* from codecs to sdp */
			if(self->neg_codecs){
				tmedia_codec_to_sdp(self->neg_codecs, self->M.lo);
			}
			else{
				self->M.lo->port = 0; /* Keep the RTP transport and reuse it when we receive a reINVITE or UPDATE request */
				goto DONE;
			}
		}
		else{
			/* from codecs to sdp */
			tmedia_codec_to_sdp(self->codecs, self->M.lo);
		}
		
		/* Hold/Resume */
		if(self->M.ro){
			if(tsdp_header_M_is_held(self->M.ro, tsk_false)){
				tsdp_header_M_hold(self->M.lo, tsk_false);
			}
			else{
				tsdp_header_M_resume(self->M.lo, tsk_false);
			}
		}
		
		/* QoS */
		if(self->qos){
			tmedia_qos_tline_t* ro_tline;
			if(self->M.ro && (ro_tline = tmedia_qos_tline_from_sdp(self->M.ro))){
				tmedia_qos_tline_set_ro(self->qos, ro_tline);
				TSK_OBJECT_SAFE_FREE(ro_tline);
			}
			tmedia_qos_tline_to_sdp(self->qos, self->M.lo);
		}
DONE:;
	}

	return self->M.lo;
}

int tdav_session_video_set_ro(tmedia_session_t* self, const tsdp_header_M_t* m)
{
	tdav_session_video_t* video;
	tmedia_codecs_L_t* neg_codecs;

	if(!self || !m){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	video = (tdav_session_video_t*)self;

	/* update remote offer */
	TSK_OBJECT_SAFE_FREE(self->M.ro);
	self->M.ro = tsk_object_ref((void*)m);

	if(self->M.lo){
		if((neg_codecs = tmedia_session_match_codec(self, m))){
			/* update negociated codecs */
			TSK_OBJECT_SAFE_FREE(self->neg_codecs);
			self->neg_codecs = neg_codecs;
			// set codec callback
			if(!TSK_LIST_IS_EMPTY(self->neg_codecs)){
				tmedia_codec_video_set_callback((tmedia_codec_video_t*)TSK_LIST_FIRST_DATA(self->neg_codecs), tdav_session_video_codec_cb, self);
			}
		}
		else{
			return -1;
		}

		/* QoS */
		if(self->qos){
			tmedia_qos_tline_t* ro_tline;
			if(self->M.ro && (ro_tline = tmedia_qos_tline_from_sdp(self->M.ro))){
				tmedia_qos_tline_set_ro(self->qos, ro_tline);
				TSK_OBJECT_SAFE_FREE(ro_tline);
			}
		}
	}

	/* get connection associated to this media line
	* If the connnection is global, then the manager will call tmedia_session_video_set() */
	if(m->C && m->C->addr){
		tsk_strupdate(&video->remote_ip, m->C->addr);
		video->useIPv6 = tsk_striequals(m->C->addrtype, "IP6");
	}
	/* set remote port */
	video->remote_port = m->port;
	

	return 0;
}





//=================================================================================================
//	Session Video Plugin object definition
//
/* constructor */
static tsk_object_t* tdav_session_video_ctor(tsk_object_t * self, va_list * app)
{
	tdav_session_video_t *session = self;
	if(session){
		/* init base: called by tmedia_session_create() */
		/* init self */
		tsk_safeobj_init(session);
		if(!(session->consumer = tmedia_consumer_create(tdav_session_video_plugin_def_t->type))){
			TSK_DEBUG_ERROR("Failed to create Video consumer");
		}
		if((session->producer = tmedia_producer_create(tdav_session_video_plugin_def_t->type))){
			tmedia_producer_set_callback(session->producer, tdav_session_video_producer_cb, self);
		}
		else{
			TSK_DEBUG_ERROR("Failed to create Video producer");
		}
	}
	return self;
}
/* destructor */
static tsk_object_t* tdav_session_video_dtor(tsk_object_t * self)
{ 
	tdav_session_video_t *session = self;
	if(session){

		// Do it in this order (deinit self first)
		
		/* deinit self (rtp manager should be destroyed after the producer) */
		TSK_OBJECT_SAFE_FREE(session->consumer);
		TSK_OBJECT_SAFE_FREE(session->producer);
		TSK_OBJECT_SAFE_FREE(session->conv.toYUV420);
		TSK_OBJECT_SAFE_FREE(session->conv.fromYUV420);
		TSK_OBJECT_SAFE_FREE(session->rtp_manager);
		TSK_FREE(session->remote_ip);
		TSK_FREE(session->local_ip);

		TSK_FREE(session->encoder.buffer);
		TSK_FREE(session->encoder.conv_buffer);
		TSK_FREE(session->decoder.buffer);
		TSK_FREE(session->decoder.conv_buffer);

		/* NAT Traversal context */
		TSK_OBJECT_SAFE_FREE(session->natt_ctx);

		tsk_safeobj_deinit(session);

		/* deinit base */
		tmedia_session_deinit(self);
	}

	return self;
}
/* object definition */
static const tsk_object_def_t tdav_session_video_def_s = 
{
	sizeof(tdav_session_video_t),
	tdav_session_video_ctor, 
	tdav_session_video_dtor,
	tmedia_session_cmp, 
};
/* plugin definition*/
static const tmedia_session_plugin_def_t tdav_session_video_plugin_def_s = 
{
	&tdav_session_video_def_s,
	
	tmedia_video,
	"video",
	
	tmedia_session_video_set,
	tdav_session_video_prepare,
	tdav_session_video_start,
	tdav_session_video_pause,
	tdav_session_video_stop,
	
	/* Audio part */
	{ tsk_null },

	tdav_session_video_get_lo,
	tdav_session_video_set_ro
};
const tmedia_session_plugin_def_t *tdav_session_video_plugin_def_t = &tdav_session_video_plugin_def_s;
