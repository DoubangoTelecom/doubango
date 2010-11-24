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

/**@file tdav_session_audio.c
 * @brief Audio Session plugin.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinydav/audio/tdav_session_audio.h"

#include "tinydav/codecs/dtmf/tdav_codec_dtmf.h"

#include "tinymedia/tmedia_denoise.h"
#include "tinymedia/tmedia_consumer.h"
#include "tinymedia/tmedia_producer.h"

#include "tinyrtp/trtp_manager.h"
#include "tinyrtp/rtp/trtp_rtp_packet.h"

#include "tsk_timer.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#define IS_DTMF_CODEC(codec) (TMEDIA_CODEC((codec))->plugin == tdav_codec_dtmf_plugin_def_t)

static int _tdav_session_audio_dtmfe_timercb(const void* arg, tsk_timer_id_t timer_id);
static struct tdav_session_audio_dtmfe_s* _tdav_session_audio_dtmfe_create(const tdav_session_audio_t* session, uint8_t event, uint16_t duration, uint32_t seq, uint32_t timestamp, uint8_t format, tsk_bool_t M, tsk_bool_t E);
static tsk_bool_t _tdav_has_at_least_one_codec(const tdav_session_audio_t* session);


/* DTMF event object */
typedef struct tdav_session_audio_dtmfe_s
{
	TSK_DECLARE_OBJECT;
	
	tsk_timer_id_t timer_id;
	trtp_rtp_packet_t* packet;

	const tdav_session_audio_t* session;
}
tdav_session_audio_dtmfe_t;
extern const tsk_object_def_t *tdav_session_audio_dtmfe_def_t;

// RTP/RTCP callback (From the network to the consumer)
static int tdav_session_audio_rtp_cb(const void* callback_data, const struct trtp_rtp_packet_s* packet)
{
	tdav_session_audio_t* audio = (tdav_session_audio_t*)callback_data;

	if(!audio || !packet){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(audio->consumer){
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
		// Open codec if not already done
		if(!TMEDIA_CODEC(codec)->opened){
			int ret;
			tsk_safeobj_lock(audio);
			if((ret = tmedia_codec_open(codec))){
				tsk_safeobj_unlock(audio);
				TSK_OBJECT_SAFE_FREE(codec);
				TSK_DEBUG_ERROR("Failed to open [%s] codec", codec->plugin->desc);
				return ret;
			}
			tsk_safeobj_unlock(audio);
		}
		// Decode data
		out_size = codec->plugin->decode(codec, packet->payload.data, packet->payload.size, &audio->decoder.buffer, &audio->decoder.buffer_size, packet->header);
		if(out_size){
			// Denoise (VAD, AGC, Noise suppression, ...)
			if(audio->denoise && TMEDIA_DENOISE(audio->denoise)->opened){
				tmedia_denoise_echo_playback(TMEDIA_DENOISE(audio->denoise), audio->decoder.buffer);
			}
			tmedia_consumer_consume(audio->consumer, &audio->decoder.buffer, out_size, packet->header);
			if(!audio->decoder.buffer){
				/* taken by the consumer */
				audio->decoder.buffer_size = 0;
			}
		}
		TSK_OBJECT_SAFE_FREE(codec);
	}
	return 0;
}

// Producer callback (From the producer to the network)
static int tdav_session_audio_producer_cb(const void* callback_data, const void* buffer, tsk_size_t size)
{
	int ret;

	tdav_session_audio_t* audio = (tdav_session_audio_t*)callback_data;

	if(audio->rtp_manager){
		/* encode */
		tsk_size_t out_size = 0;
		
		ret = 0;

		//
		// Find Encoder (call one time)
		//
		if(!audio->encoder.codec){
			tsk_list_item_t* item;
			tsk_list_foreach(item, TMEDIA_SESSION(audio)->neg_codecs){
				if(!tsk_striequals(TMEDIA_CODEC(item->data)->neg_format, TMEDIA_CODEC_FORMAT_DTMF) && 
					!tsk_striequals(TMEDIA_CODEC(item->data)->format, TMEDIA_CODEC_FORMAT_DTMF)){
						audio->encoder.codec = tsk_object_ref(item->data);
						trtp_manager_set_payload_type(audio->rtp_manager, audio->encoder.codec->neg_format ? atoi(audio->encoder.codec->neg_format) : atoi(audio->encoder.codec->format));
						/* Denoise */
						if(audio->denoise && !audio->denoise->opened){
							ret = tmedia_denoise_open(audio->denoise, TMEDIA_CODEC_PCM_FRAME_SIZE(audio->encoder.codec), TMEDIA_CODEC_RATE(audio->encoder.codec), tsk_true, 8000.0f, tsk_true, tsk_true);
						}
						break;
				}
			}
		}		
		if(!audio->encoder.codec){
			TSK_DEBUG_ERROR("Failed to find a valid codec");
			return -3;
		}

		// Open codec if not already done
		if(!audio->encoder.codec->opened){
			tsk_safeobj_lock(audio);
			if((ret = tmedia_codec_open(audio->encoder.codec))){
				tsk_safeobj_unlock(audio);
				TSK_DEBUG_ERROR("Failed to open [%s] codec", audio->encoder.codec->plugin->desc);
				return -4;
			}
			tsk_safeobj_unlock(audio);
		}
		// Denoise (VAD, AGC, Noise suppression, ...)
		if(audio->denoise){
			tsk_bool_t silence_or_noise = tsk_false;
			ret = tmedia_denoise_process(TMEDIA_DENOISE(audio->denoise), (void*)buffer, &silence_or_noise);
			if(silence_or_noise && (ret == 0)){
				//TSK_DEBUG_INFO("Silence or Noise buffer");
				//FIXME:
				//return 0;
			}
		}

		// Encode data
		if((audio->encoder.codec = tsk_object_ref(audio->encoder.codec))){ /* Thread safeness (SIP reINVITE or UPDATE could update the encoder) */
			out_size = audio->encoder.codec->plugin->encode(audio->encoder.codec, buffer, size, &audio->encoder.buffer, &audio->encoder.buffer_size);
			if(out_size){
				ret = trtp_manager_send_rtp(audio->rtp_manager, audio->encoder.buffer, out_size, TMEDIA_CODEC_PCM_FRAME_SIZE(audio->encoder.codec), tsk_false/*Marker*/, tsk_true/*lastPacket*/);
			}
			tsk_object_unref(audio->encoder.codec);
		}
		else{
			TSK_DEBUG_WARN("No encoder");
		}
	}

	return ret;
}


/* ============ Plugin interface ================= */

int tdav_session_audio_set(tmedia_session_t* self, const tmedia_param_t* param)
{
	int ret = 0;
	tdav_session_audio_t* audio;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	TSK_DEBUG_INFO("tdav_session_audio_set");

	audio = (tdav_session_audio_t*)self;

	if(param->plugin_type == tmedia_ppt_consumer){
		TSK_DEBUG_WARN("Not implemented");
	}
	else if(param->plugin_type == tmedia_ppt_producer){
		TSK_DEBUG_WARN("Not implemented");
	}
	else{
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
		else if(param->value_type == tmedia_pvt_pobject){
			if(tsk_striequals(param->key, "natt-ctx")){
				TSK_OBJECT_SAFE_FREE(audio->natt_ctx);
				audio->natt_ctx = tsk_object_ref(param->value);
			}
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
			if(audio->natt_ctx){
				ret = trtp_manager_set_natt_ctx(audio->rtp_manager, audio->natt_ctx);
			}
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

	if(!_tdav_has_at_least_one_codec(audio)){
		TSK_DEBUG_ERROR("No codec matched");
		return -2;
	}

	if(audio->rtp_manager){
		int ret;
		const tmedia_codec_t* codec = (const tmedia_codec_t*)TSK_LIST_FIRST_DATA(self->neg_codecs);
		/* RTP/RTCP manager: use latest information. */
		ret = trtp_manager_set_rtp_remote(audio->rtp_manager, audio->remote_ip, audio->remote_port);
		//trtp_manager_set_payload_type(audio->rtp_manager, codec->neg_format ? atoi(codec->neg_format) : atoi(codec->format));
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
		/* Denoise (AEC, Noise Suppression, AGC) */
		if(audio->denoise && audio->encoder.codec){
			tmedia_denoise_open(audio->denoise, TMEDIA_CODEC_PCM_FRAME_SIZE(audio->encoder.codec), TMEDIA_CODEC_RATE(audio->encoder.codec), tsk_true, 8000.0f, tsk_true, tsk_true);
		}

		/* for test */
		//trtp_manager_send_rtp(audio->rtp_manager, "test", 4, tsk_true);
		return ret;
	}
	else{
		TSK_DEBUG_ERROR("Invalid RTP/RTCP manager");
		return -3;
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

	return 0;
}

int tdav_session_audio_send_dtmf(tmedia_session_t* self, uint8_t event)
{
	tdav_session_audio_t* audio;
	tmedia_codec_t* codec;
	int ret, rate = 8000, ptime = 20;
	uint16_t duration;
	tdav_session_audio_dtmfe_t *dtmfe, *copy;
	static uint32_t timestamp = 0x3200;
	static uint32_t seq_num =  0;
	int format = 101;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	audio = (tdav_session_audio_t*)self;

	// Find the DTMF codec to use to use the RTP payload
	if((codec = tmedia_codec_find_by_format(TMEDIA_SESSION(audio)->codecs, TMEDIA_CODEC_FORMAT_DTMF))){
		rate = (int)codec->plugin->rate;
		format = atoi(codec->neg_format ? codec->neg_format : codec->format);
		TSK_OBJECT_SAFE_FREE(codec);
	}

	/* do we have an RTP manager? */
	if(!audio->rtp_manager){
		TSK_DEBUG_ERROR("No RTP manager associated to this session");
		return -2;
	}

	/* Create Events list */
	if(!audio->dtmf_events){
		audio->dtmf_events = tsk_list_create();
	}

	/* Create global reference to the timer manager */
	if(!audio->timer.created){
		if((ret = tsk_timer_mgr_global_ref())){
			TSK_DEBUG_ERROR("Failed to create Global Timer Manager");
			return ret;
		}
		audio->timer.created = tsk_true;
	}

	/* Start the timer manager */
	if(!audio->timer.started){
		if((ret = tsk_timer_mgr_global_start())){
			TSK_DEBUG_ERROR("Failed to start Global Timer Manager");
			return ret;
		}
		audio->timer.started = tsk_true;
	}


	/*	RFC 4733 - 5.  Examples
	
	+-------+-----------+------+--------+------+--------+--------+------+
   |  Time | Event     |   M  |  Time- |  Seq |  Event |  Dura- |   E  |
   |  (ms) |           |  bit |  stamp |   No |  Code  |   tion |  bit |
   +-------+-----------+------+--------+------+--------+--------+------+
   |     0 | "9"       |      |        |      |        |        |      |
   |       | starts    |      |        |      |        |        |      |
   |    50 | RTP       |  "1" |      0 |    1 |    9   |    400 |  "0" |
   |       | packet 1  |      |        |      |        |        |      |
   |       | sent      |      |        |      |        |        |      |
   |   100 | RTP       |  "0" |      0 |    2 |    9   |    800 |  "0" |
   |       | packet 2  |      |        |      |        |        |      |
   |       | sent      |      |        |      |        |        |      |
   |   150 | RTP       |  "0" |      0 |    3 |    9   |   1200 |  "0" |
   |       | packet 3  |      |        |      |        |        |      |
   |       | sent      |      |        |      |        |        |      |
   |   200 | RTP       |  "0" |      0 |    4 |    9   |   1600 |  "0" |
   |       | packet 4  |      |        |      |        |        |      |
   |       | sent      |      |        |      |        |        |      |
   |   200 | "9" ends  |      |        |      |        |        |      |
   |   250 | RTP       |  "0" |      0 |    5 |    9   |   1600 |  "1" |
   |       | packet 4  |      |        |      |        |        |      |
   |       | first     |      |        |      |        |        |      |
   |       | retrans-  |      |        |      |        |        |      |
   |       | mission   |      |        |      |        |        |      |
   |   300 | RTP       |  "0" |      0 |    6 |    9   |   1600 |  "1" |
   |       | packet 4  |      |        |      |        |        |      |
   |       | second    |      |        |      |        |        |      |
   |       | retrans-  |      |        |      |        |        |      |
   |       | mission   |      |        |      |        |        |      |
   =====================================================================
   |   880 | First "1" |      |        |      |        |        |      |
   |       | starts    |      |        |      |        |        |      |
   |   930 | RTP       |  "1" |   7040 |    7 |    1   |    400 |  "0" |
   |       | packet 5  |      |        |      |        |        |      |
   |       | sent      |      |        |      |        |        |      |
	*/
	
	// ref()(thread safeness)
	audio = tsk_object_ref(audio);
	
	duration = (rate * ptime)/1000;
	/* Not mandatory but elegant */
	timestamp += duration;

	copy = dtmfe = _tdav_session_audio_dtmfe_create(audio, event, duration*1, ++seq_num, timestamp, (uint8_t)format, tsk_true, tsk_false);
	tsk_list_push_back_data(audio->dtmf_events, (void**)&dtmfe);
	tsk_timer_mgr_global_schedule(ptime*0, _tdav_session_audio_dtmfe_timercb, copy);
	copy = dtmfe = _tdav_session_audio_dtmfe_create(audio, event, duration*2, ++seq_num, timestamp, (uint8_t)format, tsk_false, tsk_false);
	tsk_list_push_back_data(audio->dtmf_events, (void**)&dtmfe);
	tsk_timer_mgr_global_schedule(ptime*1, _tdav_session_audio_dtmfe_timercb, copy);
	copy = dtmfe = _tdav_session_audio_dtmfe_create(audio, event, duration*3, ++seq_num, timestamp, (uint8_t)format, tsk_false, tsk_false);
	tsk_list_push_back_data(audio->dtmf_events, (void**)&dtmfe);
	tsk_timer_mgr_global_schedule(ptime*2, _tdav_session_audio_dtmfe_timercb, copy);

	copy = dtmfe = _tdav_session_audio_dtmfe_create(audio, event, duration*4, ++seq_num, timestamp, (uint8_t)format, tsk_false, tsk_true);
	tsk_list_push_back_data(audio->dtmf_events, (void**)&dtmfe);
	tsk_timer_mgr_global_schedule(ptime*3, _tdav_session_audio_dtmfe_timercb, copy);
	copy = dtmfe = _tdav_session_audio_dtmfe_create(audio, event, duration*4, seq_num, timestamp, (uint8_t)format, tsk_false, tsk_true);
	tsk_list_push_back_data(audio->dtmf_events, (void**)&dtmfe);
	tsk_timer_mgr_global_schedule(ptime*4, _tdav_session_audio_dtmfe_timercb, copy);
	copy = dtmfe = _tdav_session_audio_dtmfe_create(audio, event, duration*4, seq_num, timestamp, (uint8_t)format, tsk_false, tsk_true);
	tsk_list_push_back_data(audio->dtmf_events, (void**)&dtmfe);
	tsk_timer_mgr_global_schedule(ptime*5, _tdav_session_audio_dtmfe_timercb, copy);

	// unref()(thread safeness)
	audio = tsk_object_unref(audio);

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
		if((self->M.lo = tsdp_header_M_create(self->plugin->media, audio->rtp_manager->rtp.public_port, "RTP/AVP"))){
			/* If NATT is active, do not rely on the global IP address Connection line */
			if(audio->natt_ctx){
				tsdp_header_M_add_headers(self->M.lo,
					TSDP_HEADER_C_VA_ARGS("IN", audio->useIPv6 ? "IP6" : "IP4", audio->rtp_manager->rtp.public_ip),
					tsk_null);
			}
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
			TSK_OBJECT_SAFE_FREE(self->neg_codecs);
			/* update negociated codecs */
			if((neg_codecs = tmedia_session_match_codec(self, self->M.ro))){
				self->neg_codecs = neg_codecs;
				TSK_OBJECT_SAFE_FREE(audio->encoder.codec);
			}
			/* from codecs to sdp */
			if(TSK_LIST_IS_EMPTY(self->neg_codecs) || ((self->neg_codecs->tail == self->neg_codecs->head) && IS_DTMF_CODEC(TSK_LIST_FIRST_DATA(self->neg_codecs)))){
				self->M.lo->port = 0; /* Keep the RTP transport and reuse it when we receive a reINVITE or UPDATE request */
				goto DONE;
			}
			else{
				tmedia_codec_to_sdp(self->neg_codecs, self->M.lo);
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
		///* 3GPP TS 24.229 - 6.1.1 General
		//	The UE shall include the MIME subtype "telephone-event" in the "m=" media descriptor in the SDP for audio media
		//	flows that support both audio codec and DTMF payloads in RTP packets as described in RFC 4733 [23].
		//*/
		//tsdp_header_M_add_fmt(self->M.lo, TMEDIA_CODEC_FORMAT_DTMF);
		//tsdp_header_M_add_headers(self->M.lo,
		//			TSDP_HEADER_A_VA_ARGS("fmtp", TMEDIA_CODEC_FORMAT_DTMF" 0-15"),
		//		tsk_null);
		//tsdp_header_M_add_headers(self->M.lo,
		//			TSDP_HEADER_A_VA_ARGS("rtpmap", TMEDIA_CODEC_FORMAT_DTMF" telephone-event/8000"),
		//		tsk_null);
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
			TSK_OBJECT_SAFE_FREE(audio->encoder.codec);
		}
		else{
			TSK_DEBUG_ERROR("None Match");
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
	* If the connnection is global, then the manager will call tdav_session_audio_set() */
	if(m->C && m->C->addr){
		tsk_strupdate(&audio->remote_ip, m->C->addr);
		audio->useIPv6 = tsk_striequals(m->C->addrtype, "IP6");
	}
	/* set remote port */
	audio->remote_port = m->port;
	

	return 0;
}

/* Internal function used to check that there is a least one valid codec in the negociated list */
tsk_bool_t _tdav_has_at_least_one_codec(const tdav_session_audio_t* session)
{
	const tsk_list_item_t* item;
	tsk_list_foreach(item, TMEDIA_SESSION(session)->neg_codecs){
		if(!IS_DTMF_CODEC(item->data)){
			return tsk_true;
		}
	}
	return tsk_false;
}


/* Internal function used to create new DTMF event */
tdav_session_audio_dtmfe_t* _tdav_session_audio_dtmfe_create(const tdav_session_audio_t* session, uint8_t event, uint16_t duration, uint32_t seq, uint32_t timestamp, uint8_t format, tsk_bool_t M, tsk_bool_t E)
{
	tdav_session_audio_dtmfe_t* dtmfe;
	static uint8_t volume = 10;
	static uint32_t ssrc = 0x5234A8;
	
	uint8_t pay[4] = {0};

	/* RFC 4733 - 2.3.  Payload Format
		0                   1                   2                   3
		0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	   |     event     |E|R| volume    |          duration             |
	   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	*/

	if(!(dtmfe = tsk_object_new(tdav_session_audio_dtmfe_def_t))){
		TSK_DEBUG_ERROR("Failed to create new DTMF event");
		return tsk_null;
	}
	dtmfe->session = session;

	if(!(dtmfe->packet = trtp_rtp_packet_create(ssrc, seq, timestamp, format, M))){
		TSK_DEBUG_ERROR("Failed to create DTMF RTP packet");
		TSK_OBJECT_SAFE_FREE(dtmfe);
		return tsk_null;
	}
	
	pay[0] = event;
	pay[1] |= ((E << 7) | (volume & 0x3F));
	pay[2] = (duration >> 8);
	pay[3] = (duration & 0xFF);

	/* set data */
	if((dtmfe->packet->payload.data = tsk_calloc(sizeof(pay), sizeof(uint8_t)))){
		memcpy(dtmfe->packet->payload.data, pay, sizeof(pay));
		dtmfe->packet->payload.size = sizeof(pay);
	}

	return dtmfe;
}

int _tdav_session_audio_dtmfe_timercb(const void* arg, tsk_timer_id_t timer_id)
{
	tdav_session_audio_dtmfe_t* dtmfe = (tdav_session_audio_dtmfe_t*)arg;
	int ret;

	if(!dtmfe || !dtmfe->session){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	/* Send the data */
	TSK_DEBUG_INFO("Sending DTMF event");
	ret = trtp_manager_send_rtp_2(dtmfe->session->rtp_manager, dtmfe->packet);

	/* Remove and delete the event from the queue */
	tsk_list_remove_item_by_data(dtmfe->session->dtmf_events, dtmfe);

	return ret;
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
		tsk_safeobj_init(session);
		if(!(session->consumer = tmedia_consumer_create(tmedia_audio))){
			TSK_DEBUG_ERROR("Failed to create Audio consumer");
		}
		if((session->producer = tmedia_producer_create(tmedia_audio))){
			tmedia_producer_set_callback(session->producer, tdav_session_audio_producer_cb, self);
		}
		else{
			TSK_DEBUG_ERROR("Failed to create Audio producer");
		}
		if(!(session->denoise = tmedia_denoise_create())){
			TSK_DEBUG_WARN("No Audio denoiser found");
		}
	}
	return self;
}
/* destructor */
static tsk_object_t* tdav_session_audio_dtor(tsk_object_t * self)
{ 
	tdav_session_audio_t *session = self;
	if(session){
		
		// Do it in this order (deinit self first)

		/* Timer manager */
		if(session->timer.started){
			if(session->dtmf_events){
				/* Cancel all events */
				tsk_list_item_t* item;
				tsk_list_foreach(item, session->dtmf_events){
					tsk_timer_mgr_global_cancel(((tdav_session_audio_dtmfe_t*)item->data)->timer_id);
				}
			}
			tsk_timer_mgr_global_stop();
		}
		if(session->timer.created){
			tsk_timer_mgr_global_unref();
		}
		/* CleanUp the DTMF events */
		TSK_OBJECT_SAFE_FREE(session->dtmf_events);

		/* deinit self (rtp manager should be destroyed after the producer) */
		TSK_OBJECT_SAFE_FREE(session->consumer);
		TSK_OBJECT_SAFE_FREE(session->producer);
		TSK_OBJECT_SAFE_FREE(session->rtp_manager);
		TSK_FREE(session->remote_ip);
		TSK_FREE(session->local_ip);
		TSK_OBJECT_SAFE_FREE(session->denoise);

		TSK_OBJECT_SAFE_FREE(session->encoder.codec);
		TSK_FREE(session->encoder.buffer);
		TSK_FREE(session->decoder.buffer);

		/* NAT Traversal context */
		TSK_OBJECT_SAFE_FREE(session->natt_ctx);

		tsk_safeobj_deinit(session);

		/* deinit base */
		tmedia_session_deinit(self);
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
	
	tdav_session_audio_set,
	tdav_session_audio_prepare,
	tdav_session_audio_start,
	tdav_session_audio_pause,
	tdav_session_audio_stop,

	/* Audio part */
	{
		tdav_session_audio_send_dtmf
	},

	tdav_session_audio_get_lo,
	tdav_session_audio_set_ro
};
const tmedia_session_plugin_def_t *tdav_session_audio_plugin_def_t = &tdav_session_audio_plugin_def_s;



//=================================================================================================
//	DTMF event object definition
//
static tsk_object_t* tdav_session_audio_dtmfe_ctor(tsk_object_t * self, va_list * app)
{
	tdav_session_audio_dtmfe_t *event = self;
	if(event){
		event->timer_id = TSK_INVALID_TIMER_ID;
	}
	return self;
}

static tsk_object_t* tdav_session_audio_dtmfe_dtor(tsk_object_t * self)
{ 
	tdav_session_audio_dtmfe_t *event = self;
	if(event){
		TSK_OBJECT_SAFE_FREE(event->packet);
	}

	return self;
}

static int tdav_session_audio_dtmfe_cmp(const tsk_object_t *_e1, const tsk_object_t *_e2)
{
	const tdav_session_audio_dtmfe_t *e1 = _e1;
	const tdav_session_audio_dtmfe_t *e2 = _e2;

	return (e1 - e2);
}

static const tsk_object_def_t tdav_session_audio_dtmfe_def_s = 
{
	sizeof(tdav_session_audio_dtmfe_t),
	tdav_session_audio_dtmfe_ctor, 
	tdav_session_audio_dtmfe_dtor,
	tdav_session_audio_dtmfe_cmp, 
};
const tsk_object_def_t *tdav_session_audio_dtmfe_def_t = &tdav_session_audio_dtmfe_def_s;
