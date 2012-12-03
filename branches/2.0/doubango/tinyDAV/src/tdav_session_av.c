/*
* Copyright (C) 2012 Doubango Telecom <http://www.doubango.org>
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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

/**@file tdav_session_av.c
 * @brief Audio/Video base Session plugin
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 */
#include "tinydav/tdav_session_av.h"
#include "tinydav/codecs/dtmf/tdav_codec_dtmf.h"
#include "tinydav/codecs/fec/tdav_codec_red.h"
#include "tinydav/codecs/fec/tdav_codec_ulpfec.h"

#include "tinyrtp/trtp_manager.h"
#include "tinyrtp/rtp/trtp_rtp_packet.h"

#include "ice/tnet_ice_ctx.h"
#include "ice/tnet_ice_candidate.h"

#include "tinymedia/tmedia_consumer.h"
#include "tinymedia/tmedia_producer.h"
#include "tinymedia/tmedia_defaults.h"

#define TDAV_IS_DTMF_CODEC(codec) (TMEDIA_CODEC((codec))->plugin == tdav_codec_dtmf_plugin_def_t)
#define TDAV_IS_ULPFEC_CODEC(codec) (TMEDIA_CODEC((codec))->plugin == tdav_codec_ulpfec_plugin_def_t)
#define TDAV_IS_RED_CODEC(codec) (TMEDIA_CODEC((codec))->plugin == tdav_codec_red_plugin_def_t)

static int _tdav_session_av_red_cb(const void* callback_data, const struct trtp_rtp_packet_s* packet);

int tdav_session_av_init(tdav_session_av_t* self, tmedia_type_t media_type)
{
	uint64_t session_id;
	tmedia_profile_t profile = tmedia_defaults_get_profile(); // default profile, will be updated by the SIP session

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	/* base::init(): called by tmedia_session_create() */

	self->media_type = media_type;
	self->use_rtcp = tmedia_defaults_get_rtcp_enabled();
	self->use_rtcpmux = tmedia_defaults_get_rtcpmux_enabled();
	self->use_avpf = (profile == tmedia_profile_rtcweb); // negotiate if not RTCWeb profile
	
	tsk_safeobj_init(self);
	
	// session id
	if(!(session_id = TMEDIA_SESSION(self)->id)){ // set the session id if not already done
		TMEDIA_SESSION(self)->id = session_id = tmedia_session_get_unique_id();
	}
	// consumer
	TSK_OBJECT_SAFE_FREE(self->consumer);
	if(!(self->consumer = tmedia_consumer_create(self->media_type, session_id))){
		TSK_DEBUG_ERROR("Failed to create consumer for media type = %d", self->media_type);
	}
	// producer
	TSK_OBJECT_SAFE_FREE(self->producer);
	if(!(self->producer = tmedia_producer_create(self->media_type, session_id))){
		TSK_DEBUG_ERROR("Failed to create producer for media type = %d", self->media_type);
	}

#if HAVE_SRTP
		// This is the default value and can be updated by the user using "session_set('srtp-mode', mode_e)"
		self->srtp_mode = (profile == tmedia_profile_rtcweb) ? tmedia_srtp_mode_mandatory : tmedia_defaults_get_srtp_mode();
		self->use_srtp = (self->srtp_mode == tmedia_srtp_mode_mandatory); // if optional -> negotiate
#endif
	return 0;
}

tsk_bool_t tdav_session_av_set(tdav_session_av_t* self, const tmedia_param_t* param)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_false;
	}

	// try with base class first
	if(tmedia_session_set_2(TMEDIA_SESSION(self), param)){
		return tsk_true;
	}

	if(param->plugin_type == tmedia_ppt_consumer && self->consumer){
		return (tmedia_consumer_set(self->consumer, param) == 0);
	}
	else if(param->plugin_type == tmedia_ppt_producer && self->producer){
		return tmedia_producer_set(self->producer, param);
	}
	else if(param->plugin_type == tmedia_ppt_session){
		if(param->value_type == tmedia_pvt_pchar){
			if(tsk_striequals(param->key, "remote-ip")){
				if(param->value){
					tsk_strupdate(&self->remote_ip, param->value);
					return tsk_true;
				}
			}
			else if(tsk_striequals(param->key, "local-ip")){
				tsk_strupdate(&self->local_ip, param->value);
				return tsk_true;
			}
			else if(tsk_striequals(param->key, "local-ipver")){
				self->use_ipv6 = tsk_striequals(param->value, "ipv6");
				return tsk_true;
			}
		}
		else if(param->value_type == tmedia_pvt_int32){
			if(tsk_striequals(param->key, "srtp-mode")){
#if HAVE_SRTP
				self->srtp_mode = (tmedia_srtp_mode_t)TSK_TO_INT32((uint8_t*)param->value);
				return tsk_true;
#endif
			}
			else if(tsk_striequals(param->key, "rtp-ssrc")){
				self->rtp_ssrc = *((uint32_t*)param->value);
				if(self->rtp_manager && self->rtp_ssrc){
					self->rtp_manager->rtp.ssrc = self->rtp_ssrc;
				}
				return tsk_true;
			}
			else if(tsk_striequals(param->key, "rtcp-enabled")){
				self->use_rtcp = (TSK_TO_INT32((uint8_t*)param->value) != 0);
				return tsk_true;
			}
			else if(tsk_striequals(param->key, "rtcpmux-enabled")){
				self->use_rtcpmux = (TSK_TO_INT32((uint8_t*)param->value) != 0);
				return tsk_true;
			}
			else if(tsk_striequals(param->key, "avpf-enabled")){
				self->use_avpf = (TSK_TO_INT32((uint8_t*)param->value) != 0);
				return tsk_true;
			}
		}
		else if(param->value_type == tmedia_pvt_pobject){
			if(tsk_striequals(param->key, "natt-ctx")){
				TSK_OBJECT_SAFE_FREE(self->natt_ctx);
				self->natt_ctx = tsk_object_ref(param->value);
				return tsk_true;
			}
			else if(tsk_striequals(param->key, "ice-ctx")){
				TSK_OBJECT_SAFE_FREE(self->ice_ctx);
				self->ice_ctx = tsk_object_ref(param->value);
				if(self->rtp_manager){
					trtp_manager_set_ice_ctx(self->rtp_manager, self->ice_ctx);
				}
				return tsk_true;
			}
		}
	}

	return tsk_false;
}

tsk_bool_t tdav_session_av_get(tdav_session_av_t* self, tmedia_param_t* param)
{
	if(!self || !param){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_false;
	}

	// try with the base class to see if this option is supported or not
	if(tmedia_session_get(TMEDIA_SESSION(self), param)){
		return tsk_true;
	}

	if(param->plugin_type == tmedia_ppt_session){
		if(param->value_type == tmedia_pvt_int32){
			if(tsk_striequals(param->key, "srtp-enabled")){
#if HAVE_SRTP
				if(self->rtp_manager){
					((int8_t*)param->value)[0] = self->use_srtp ? 1 : 0;
					return tsk_true;
				}
#endif
			}
		}
	}

	return tsk_false;
}

int tdav_session_av_prepare(tdav_session_av_t* self)
{
	int ret = 0;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}	

	/* set local port */
	if(!self->rtp_manager){
		self->rtp_manager = self->ice_ctx ? trtp_manager_create_2(self->ice_ctx)
			: trtp_manager_create(self->use_rtcp, self->local_ip, self->use_ipv6);
		if(self->rtp_manager){
			ret = trtp_manager_set_port_range(self->rtp_manager, tmedia_defaults_get_rtp_port_range_start(), tmedia_defaults_get_rtp_port_range_stop());
			self->rtp_manager->use_rtcp = self->use_rtcp;
			ret = trtp_manager_prepare(self->rtp_manager);
			if(self->natt_ctx){
				ret = trtp_manager_set_natt_ctx(self->rtp_manager, self->natt_ctx);
			}
			if(self->rtp_ssrc){
				self->rtp_manager->rtp.ssrc = self->rtp_ssrc;
			}
		}
	}

	/* SRTP */
#if HAVE_SRTP
	{
		if(self->remote_srtp_neg.pending){
			char* str = tsk_null;
			self->remote_srtp_neg.pending = tsk_false;
			tsk_sprintf(&str, "%d %s inline:%s", self->remote_srtp_neg.tag, trtp_srtp_crypto_type_strings[self->remote_srtp_neg.crypto_type], self->remote_srtp_neg.key);
			trtp_srtp_set_remote(self->rtp_manager, str);
			TSK_FREE(str);
		}
	}
#endif

	/* Consumer will be prepared in tdav_session_audio_start() */
	/* Producer will be prepared in tdav_session_audio_start() */

	return ret;
}

int tdav_session_av_start(tdav_session_av_t* self, const tmedia_codec_t* best_codec)
{
	int ret;
	if(!self || !best_codec){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	// RED codec
	TSK_OBJECT_SAFE_FREE(self->red.codec);
	self->red.payload_type = 0;
	if((self->red.codec = tsk_object_ref((tsk_object_t*)tdav_session_av_get_red_codec(self)))){
		self->red.payload_type = atoi(self->red.codec->neg_format);
		if(!TMEDIA_CODEC(self->red.codec)->opened){
			if((ret = tmedia_codec_open(self->red.codec))){
				TSK_DEBUG_ERROR("Failed to open [%s] codec", self->red.codec->plugin->desc);
				return ret;
			}
		}
		// set RED callback (unencapsulated data)
		ret = tdav_codec_red_set_callback((struct tdav_codec_red_s*)self->red.codec, _tdav_session_av_red_cb, self);
	}

	// ULPFEC
	TSK_OBJECT_SAFE_FREE(self->ulpfec.codec);
	self->ulpfec.payload_type = 0;
	if((self->ulpfec.codec = tsk_object_ref((tsk_object_t*)tdav_session_av_get_ulpfec_codec(self)))){
		self->ulpfec.payload_type = atoi(self->ulpfec.codec->neg_format);
		if(!TMEDIA_CODEC(self->ulpfec.codec)->opened){
			if((ret = tmedia_codec_open(self->ulpfec.codec))){
				TSK_DEBUG_ERROR("Failed to open [%s] codec", self->ulpfec.codec->plugin->desc);
				return ret;
			}
		}
	}

	if(self->rtp_manager){
		int ret;
		/* RTP/RTCP manager: use latest information. */

		// these information will be updated when the RTP manager starts if ICE is enabled
		ret = trtp_manager_set_rtp_remote(self->rtp_manager, self->remote_ip, self->remote_port);
		ret = trtp_manager_set_payload_type(self->rtp_manager, best_codec->neg_format ? atoi(best_codec->neg_format) : atoi(best_codec->format));
		self->rtp_manager->use_rtcpmux = self->use_rtcpmux;
		ret = trtp_manager_start(self->rtp_manager);

		// because of AudioUnit under iOS => prepare both consumer and producer then start() at the same time
		/* prepare consumer and producer */
		if(self->producer) ret = tmedia_producer_prepare(self->producer, best_codec);
		if(self->consumer) ret = tmedia_consumer_prepare(self->consumer, best_codec);
		
		/* start consumer and producer */
		if(self->consumer) ret = tmedia_consumer_start(self->consumer);
		if(self->producer) ret = tmedia_producer_start(self->producer);

		// not that the RTP manager is activated check that SRTP is correctly activated
#if HAVE_SRTP
		self->use_srtp = trtp_srtp_is_active(self->rtp_manager);
#endif
		
		return ret;
	}
	else{
		TSK_DEBUG_ERROR("Invalid RTP/RTCP manager");
		return -3;
	}

	return 0;
}

int tdav_session_av_stop(tdav_session_av_t* self)
{
	tmedia_codec_t* codec;
	tsk_list_item_t* item;
	int ret = 0;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	/* Consumer */
	if(self->consumer){
		ret = tmedia_consumer_stop(self->consumer);
	}
	/* Producer */
	if(self->producer){
		ret = tmedia_producer_stop(self->producer);
	}

	/* RTP/RTCP manager */
	if(self->rtp_manager){
		ret = trtp_manager_stop(self->rtp_manager);
	}

	/* close codecs to force open() for next start (e.g SIP UPDATE with SDP) */
	if(TMEDIA_SESSION(self)->neg_codecs){
		tsk_list_foreach(item, TMEDIA_SESSION(self)->neg_codecs){
			if(!(codec = TMEDIA_CODEC(item->data))){
				continue;
			}
			ret = tmedia_codec_close(codec);
		}
	}

	return ret;
}

int tdav_session_av_pause(tdav_session_av_t* self)
{
	int ret = 0;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	/* Consumer */
	if(self->consumer){
		ret = tmedia_consumer_pause(self->consumer);
	}
	/* Producer */
	if(self->producer){
		ret = tmedia_producer_pause(self->producer);
	}

	return ret;
}

const tsdp_header_M_t* tdav_session_av_get_lo(tdav_session_av_t* self, tsk_bool_t *updated)
{
	tmedia_session_t* base = TMEDIA_SESSION(self);
	tsk_bool_t have_libsrtp = tsk_false;

	if(!base || !base->plugin || !updated){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}
	
	*updated = tsk_false;
#if HAVE_SRTP
	have_libsrtp = tsk_true;
#endif

	if(!self->rtp_manager || (!self->ice_ctx && !self->rtp_manager->transport)){
		if(self->rtp_manager && (!self->ice_ctx && !self->rtp_manager->transport)){ // reINVITE or UPDATE (manager was destroyed when stoppped)
			if(trtp_manager_prepare(self->rtp_manager)){
				TSK_DEBUG_ERROR("Failed to prepare transport");
				return tsk_null;
			}
		}
		else{
			TSK_DEBUG_ERROR("RTP/RTCP manager in invalid");
			return tsk_null;
		}
	}
	
	if(base->ro_changed && base->M.lo){
		/* Codecs */
		tsdp_header_A_removeAll_by_field(base->M.lo->Attributes, "fmtp");
		tsdp_header_A_removeAll_by_field(base->M.lo->Attributes, "rtpmap");
		tsk_list_clear_items(base->M.lo->FMTs);

		/* QoS */
		tsdp_header_A_removeAll_by_field(base->M.lo->Attributes, "curr");
		tsdp_header_A_removeAll_by_field(base->M.lo->Attributes, "des");
		tsdp_header_A_removeAll_by_field(base->M.lo->Attributes, "conf");

		/* SRTP */
		tsdp_header_A_removeAll_by_field(base->M.lo->Attributes, "crypto");

		/* ICE */
		tsdp_header_A_removeAll_by_field(base->M.lo->Attributes, "candidate");
		tsdp_header_A_removeAll_by_field(base->M.lo->Attributes, "ice-ufrag");
		tsdp_header_A_removeAll_by_field(base->M.lo->Attributes, "ice-pwd");

		/* SDPCapNeg */
		tsdp_header_A_removeAll_by_field(base->M.lo->Attributes, "tcap");
		tsdp_header_A_removeAll_by_field(base->M.lo->Attributes, "acap");
		tsdp_header_A_removeAll_by_field(base->M.lo->Attributes, "pcfg");
		
		// Others
		tsdp_header_A_removeAll_by_field(base->M.lo->Attributes, "mid");
		tsdp_header_A_removeAll_by_field(base->M.lo->Attributes, "rtcp-mux");
		tsdp_header_A_removeAll_by_field(base->M.lo->Attributes, "ssrc");
	}

	*updated = (base->ro_changed || !base->M.lo);

	if(!base->M.lo){
		if((base->M.lo = tsdp_header_M_create(base->plugin->media, self->rtp_manager->rtp.public_port, "RTP/AVP"))){
			/* If NATT is active, do not rely on the global IP address Connection line */
			if(self->natt_ctx){
				tsdp_header_M_add_headers(base->M.lo,
					TSDP_HEADER_C_VA_ARGS("IN", self->use_ipv6 ? "IP6" : "IP4", self->rtp_manager->rtp.public_ip),
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
			if(self->media_type == tmedia_audio){
				tsdp_header_M_add_headers(base->M.lo,
					TSDP_HEADER_A_VA_ARGS("ptime", "20"),
					TSDP_HEADER_A_VA_ARGS("silenceSupp", "off - - - -"),
					tsk_null);
				// the "telephone-event" fmt/rtpmap is added below
			}
		}
		else{
			TSK_DEBUG_ERROR("Failed to create lo");
			return tsk_null;
		}
	}
	
	if(*updated){
		tmedia_codecs_L_t* neg_codecs = tsk_null;

		if(base->M.ro){
			TSK_OBJECT_SAFE_FREE(base->neg_codecs);
			/* update negociated codecs */
			if((neg_codecs = tmedia_session_match_codec(base, base->M.ro))){
				base->neg_codecs = neg_codecs;
			}
			/* from codecs to sdp */
			if(TSK_LIST_IS_EMPTY(base->neg_codecs) || ((base->neg_codecs->tail == base->neg_codecs->head) && TDAV_IS_DTMF_CODEC(TSK_LIST_FIRST_DATA(base->neg_codecs)))){
				base->M.lo->port = 0; /* Keep the RTP transport and reuse it when we receive a reINVITE or UPDATE request */
				TSK_DEBUG_INFO("No codec matching for media type = %d", (int32_t)self->media_type);
				goto DONE;
			}
			else{
				tmedia_codec_to_sdp(base->neg_codecs, base->M.lo);
			}
		}
		else{
			/* from codecs to sdp */
			tmedia_codec_to_sdp(base->codecs, base->M.lo);
		}

		/* SDPCapNeg: RFC 5939 */
		{
			if(base->M.ro){
				self->use_avpf = (tsk_striequals(base->M.ro->proto, "RTP/AVPF") || tsk_striequals(base->M.ro->proto, "RTP/SAVPF"));
				if(!self->use_avpf){
					if(self->sdp_neg.remote_best_pcfg.t_proto){
						self->use_avpf = (tsk_striequals(self->sdp_neg.remote_best_pcfg.t_proto, "RTP/AVPF") || tsk_striequals(self->sdp_neg.remote_best_pcfg.t_proto, "RTP/SAVPF"));
					}
				}
				if(self->sdp_neg.remote_best_pcfg.t_proto){
					// Any supported proto -> response with "a=acfg"
					if(self->use_avpf/* AVPF,SAVPF */ || tsk_striequals(self->sdp_neg.remote_best_pcfg.t_proto, "RTP/AVP") || tsk_striequals(self->sdp_neg.remote_best_pcfg.t_proto, "RTP/SAVP")){
						char *str_acfg = tsk_null;
						tsk_sprintf(&str_acfg, "%d t=%d", self->sdp_neg.remote_best_pcfg.tag, self->sdp_neg.remote_best_pcfg.t_tag);
						tsdp_header_M_add_headers(base->M.lo,
							TSDP_HEADER_A_VA_ARGS("acfg", str_acfg),
							tsk_null);
						TSK_FREE(str_acfg);
					}
				}
			}
			else{
				if(!self->use_avpf){ // only negotiate if not already using AVPF
					tsk_bool_t enable_srtp = tsk_true;
					#if HAVE_SRTP
					enable_srtp = (have_libsrtp && (self->srtp_mode == tmedia_srtp_mode_mandatory || self->srtp_mode == tmedia_srtp_mode_optional));
					#endif
					// "a=acap:1 crypto" is not included because most of SIP client don't support RFC 5939
					// "a=crypto" is always used to indicate optional support for SRTP
					tsdp_header_M_add_headers(base->M.lo,
						TSDP_HEADER_A_VA_ARGS("tcap", enable_srtp ? "1 RTP/SAVPF" : "1 RTP/AVPF"),
						TSDP_HEADER_A_VA_ARGS("pcfg", "1 t=1"),
						tsk_null);
					
				}
			}
		}

		/* Hold/Resume */
		tsdp_header_M_set_holdresume_att(base->M.lo, base->lo_held, base->ro_held);
		
		/* SRTP */
#if HAVE_SRTP
		{
			tsk_bool_t is_srtp_remote_mandatory = (base->M.ro && (tsk_striequals(base->M.ro->proto, "RTP/SAVP") || tsk_striequals(base->M.ro->proto, "RTP/SAVPF")));
			tsk_bool_t is_srtp_remote_optional = (base->M.ro && (tsdp_header_M_findA(base->M.ro, "crypto") != tsk_null));
			if((self->srtp_mode == tmedia_srtp_mode_optional && (is_srtp_remote_optional || is_srtp_remote_mandatory || !base->M.ro)) || self->srtp_mode == tmedia_srtp_mode_mandatory){
				const trtp_srtp_ctx_xt *ctx = tsk_null;
				tsk_size_t ctx_count = 0, ctx_idx;
				char* str = tsk_null;
				// local
				trtp_srtp_get_ctx_local(self->rtp_manager, &ctx, &ctx_count);
				for(ctx_idx = 0; ctx_idx < ctx_count; ++ctx_idx){
					tsk_sprintf(&str, "%d %s inline:%s", ctx[ctx_idx].tag, trtp_srtp_crypto_type_strings[ctx[ctx_idx].crypto_type], ctx[ctx_idx].key_str);
					tsdp_header_M_add_headers(base->M.lo,
						TSDP_HEADER_A_VA_ARGS("crypto", str),
						tsk_null);
					TSK_FREE(str);
				}
			}
			
			if(is_srtp_remote_mandatory || (self->srtp_mode == tmedia_srtp_mode_mandatory) || trtp_srtp_is_initialized(self->rtp_manager)){
				self->use_srtp = tsk_true;
			}
		}
#endif

		/* Update Proto*/
		tsk_strupdate(&base->M.lo->proto, 
			self->use_srtp ? (self->use_avpf ? "RTP/SAVPF" : "RTP/SAVP") : (self->use_avpf ? "RTP/AVPF" : "RTP/AVP")
		);

		// RFC 5761: RTCP/RTP muxing
		if(self->use_rtcpmux){
			tsdp_header_M_add_headers(base->M.lo, TSDP_HEADER_A_VA_ARGS("rtcp-mux", tsk_null), tsk_null);
		}

		// draft-lennox-mmusic-sdp-source-attributes-01
		if(self->media_type == tmedia_audio || self->media_type == tmedia_video){
			char* str = tsk_null;
			tsk_sprintf(&str, "%u cname:%s", self->rtp_manager->rtp.ssrc, "ldjWoB60jbyQlR6e");
			tsdp_header_M_add_headers(base->M.lo, TSDP_HEADER_A_VA_ARGS("ssrc", str), tsk_null);
			tsk_sprintf(&str, "%u mslabel:%s", self->rtp_manager->rtp.ssrc, "6994f7d1-6ce9-4fbd-acfd-84e5131ca2e2");
			tsdp_header_M_add_headers(base->M.lo, TSDP_HEADER_A_VA_ARGS("ssrc", str), tsk_null);
			tsk_sprintf(&str, "%u label:%s", self->rtp_manager->rtp.ssrc, "Doubango");
			tsdp_header_M_add_headers(base->M.lo, TSDP_HEADER_A_VA_ARGS("ssrc", str), tsk_null);
			TSK_FREE(str);
		}

		/* ICE */
		if(self->ice_ctx){
			tsk_size_t index = 0;
			const tnet_ice_candidate_t* candidate;
			tsk_bool_t remote_use_rtcpmux = (base->M.ro && (tsdp_header_M_findA(base->M.ro, "rtcp-mux") != tsk_null));

			// FIXME: for RTCP, use "RFC 3605"in addition to "rtcp-mux"
			
			// "a=ice-mismatch" if "C=" line is not included in the candidates
			if((candidate = tnet_ice_ctx_get_local_candidate_at(self->ice_ctx, 0))){ // at least one candidate
				base->M.lo->port = candidate->socket->port;

				tsdp_header_M_remove(base->M.lo, tsdp_htype_C);
				tsdp_header_M_add_headers(base->M.lo,
						TSDP_HEADER_C_VA_ARGS("IN", TNET_SOCKET_TYPE_IS_IPV6(candidate->socket->type) ? "IP6" : "IP4", candidate->socket->ip),
						tsk_null);
				tsdp_header_M_add_headers(base->M.lo,
							TSDP_HEADER_A_VA_ARGS("ice-ufrag", candidate->ufrag),
							TSDP_HEADER_A_VA_ARGS("ice-pwd", candidate->pwd),
							tsk_null);
				// RTCWeb
				tsdp_header_M_add_headers(base->M.lo,
					TSDP_HEADER_A_VA_ARGS("mid", self->media_type == tmedia_audio ? "audio" : "video"),
						tsk_null);	
				
				while((candidate = tnet_ice_ctx_get_local_candidate_at(self->ice_ctx, index++))){
					if(self->use_rtcpmux && remote_use_rtcpmux && candidate->comp_id == TNET_ICE_CANDIDATE_COMPID_RTCP){
						continue; // do not add RTCP candidates if RTCP-MUX is activated (local + remote)
					}
					tsdp_header_M_add_headers(base->M.lo,
							TSDP_HEADER_A_VA_ARGS("candidate", tnet_ice_candidate_tostring((tnet_ice_candidate_t*)candidate)),
							tsk_null);
				}
			}
		}
		else{
			if(base->M.lo->C){
				tsk_strupdate(&base->M.lo->C->addr, self->rtp_manager->rtp.public_ip);
				tsk_strupdate(&base->M.lo->C->addrtype, (self->use_ipv6 ? "IP6" : "IP4"));
			}
			base->M.lo->port = self->rtp_manager->rtp.public_port;
		}

		if(self->media_type == tmedia_audio){
			///* 3GPP TS 24.229 - 6.1.1 General
			//	The UE shall include the MIME subtype "telephone-event" in the "m=" media descriptor in the SDP for audio media
			//	flows that support both audio codec and DTMF payloads in RTP packets as described in RFC 4733 [23].
			//*/
			//tsdp_header_M_add_fmt(base->M.lo, TMEDIA_CODEC_FORMAT_DTMF);
			//tsdp_header_M_add_headers(base->M.lo,
			//			TSDP_HEADER_A_VA_ARGS("fmtp", TMEDIA_CODEC_FORMAT_DTMF" 0-15"),
			//		tsk_null);
			//tsdp_header_M_add_headers(base->M.lo,
			//			TSDP_HEADER_A_VA_ARGS("rtpmap", TMEDIA_CODEC_FORMAT_DTMF" telephone-event/8000"),
			//		tsk_null);
		}

		/* QoS */
		if(base->qos){
			tmedia_qos_tline_t* ro_tline;
			if(base->M.ro && (ro_tline = tmedia_qos_tline_from_sdp(base->M.ro))){
				tmedia_qos_tline_set_ro(base->qos, ro_tline);
				TSK_OBJECT_SAFE_FREE(ro_tline);
			}
			tmedia_qos_tline_to_sdp(base->qos, base->M.lo);
		}
DONE:;
	} // updated

	return base->M.lo;
}

int tdav_session_av_set_ro(tdav_session_av_t* self, const struct tsdp_header_M_s* m, tsk_bool_t *updated)
{
	tmedia_codecs_L_t* neg_codecs;
	tsk_bool_t is_srtp_remote_mandatory;
	tsk_bool_t crypto_matched = tsk_false;
	tmedia_session_t* base = TMEDIA_SESSION(self);

	if(!base || !m || !updated){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	/* update remote offer */
	TSK_OBJECT_SAFE_FREE(base->M.ro);
	base->M.ro = tsk_object_ref((void*)m);

	*updated = tsk_false;
	is_srtp_remote_mandatory = (tsk_striequals(m->proto, "RTP/SAVP") || tsk_striequals(m->proto, "RTP/SAVPF"));

	if(base->M.lo){
		if((neg_codecs = tmedia_session_match_codec(base, m))){
			/* update negociated codecs */
			TSK_OBJECT_SAFE_FREE(base->neg_codecs);
			base->neg_codecs = neg_codecs;
			*updated = tsk_true;
		}
		else{
			TSK_DEBUG_ERROR("None Match");
			return -1;
		}
		/* QoS */
		if(base->qos){
			tmedia_qos_tline_t* ro_tline;
			if(base->M.ro && (ro_tline = tmedia_qos_tline_from_sdp(base->M.ro))){
				tmedia_qos_tline_set_ro(base->qos, ro_tline);
				TSK_OBJECT_SAFE_FREE(ro_tline);
			}
		}
		/* AVPF */
		if(tsk_striequals(base->M.lo->proto, "RTP/AVPF") || tsk_striequals(base->M.lo->proto, "RTP/SAVPF")){
			self->use_avpf = tsk_true;
		}
	}

	/* get connection associated to this media line
	* If the connnection is global, then the manager will call tdav_session_audio_set() */
	if(m->C && m->C->addr){
		tsk_strupdate(&self->remote_ip, m->C->addr);
		self->use_ipv6 = tsk_striequals(m->C->addrtype, "IP6");
	}
	/* set remote port */
	self->remote_port = m->port;

	/* RTCP-MUX */
	self->use_rtcpmux &= (tsdp_header_M_findA(m, "rtcp-mux") != tsk_null);
	if(self->ice_ctx){
		tnet_ice_ctx_set_rtcpmux(self->ice_ctx, self->use_rtcpmux);
	}

	/* SDPCapNeg: RFC 5939 */
	{
		const tsdp_header_A_t *A_pcfg, *A_tcap;
		tsk_size_t i, j;
		char c_pcfg;
		int tag_pcfg, tag_pcfg_t;
		i = 0;
		TSK_FREE(self->sdp_neg.remote_best_pcfg.t_proto);
		while((A_pcfg = tsdp_header_M_findA_at(base->M.ro, "pcfg", i++))){
			char* v_pcfg = strtok((char*)A_pcfg->value, " ");
			tag_pcfg = atoi(v_pcfg);
			if(v_pcfg && (v_pcfg = strtok(tsk_null, " "))){
				do{
					if(sscanf(v_pcfg, "%c=%d", &c_pcfg, &tag_pcfg_t) >= 2){
						j = 0;
						if(c_pcfg == 't'){
							while((A_tcap = tsdp_header_M_findA_at(base->M.ro, "tcap", j++))){
								char* v_tcap = strtok((char*)A_tcap->value, " ");
								if((v_tcap && atoi(v_tcap) == tag_pcfg_t)){
									if((v_tcap = strtok(tsk_null, " "))){
										// for now only get the best proto
										self->sdp_neg.remote_best_pcfg.tag = tag_pcfg; 
										self->sdp_neg.remote_best_pcfg.t_tag = tag_pcfg_t;
										tsk_strupdate(&self->sdp_neg.remote_best_pcfg.t_proto, v_tcap);
										goto SDPCapNegDone;
									}
								}
							}
						}
					}
				}
				while((v_pcfg = strtok(tsk_null, " ")));
			}
		}
SDPCapNegDone:;
	}

	/* SRTP */
#if HAVE_SRTP
	if(self->srtp_mode == tmedia_srtp_mode_optional || self->srtp_mode == tmedia_srtp_mode_mandatory){
		tsk_size_t i = 0;
		const tsdp_header_A_t* A;
		int ret;
		while((A = tsdp_header_M_findA_at(m, "crypto", i++))){
			if(self->rtp_manager){
				if((ret = trtp_srtp_set_remote(self->rtp_manager, A->value)) == 0){
					crypto_matched = tsk_true;
					break;
				}
			}
			else{
				if((ret = trtp_srtp_match_line(A->value, &self->remote_srtp_neg.tag, (int32_t*)&self->remote_srtp_neg.crypto_type, self->remote_srtp_neg.key, (sizeof(self->remote_srtp_neg.key) - 1))) == 0){
					crypto_matched = tsk_true;
					self->remote_srtp_neg.pending = tsk_true;
					break;
				}
			}
		}
		if((self->srtp_mode == tmedia_srtp_mode_mandatory) && !crypto_matched){// local require but none match
			TSK_DEBUG_ERROR("SRTP negotiation failed");
			return -3;
		}
	}
	
	self->use_srtp = trtp_srtp_is_initialized(self->rtp_manager);
	
#endif
	
	if(is_srtp_remote_mandatory && !crypto_matched){// remote require but none match
		TSK_DEBUG_ERROR("SRTP negotiation failed");
		return -4;
	}

	return 0;
}

const tmedia_codec_t* tdav_session_av_get_best_neg_codec(const tdav_session_av_t* self)
{
	const tsk_list_item_t* item;
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}
	
	tsk_list_foreach(item, TMEDIA_SESSION(self)->neg_codecs){
		// exclude DTMF, RED and ULPFEC
		if(!TDAV_IS_DTMF_CODEC(item->data) && !TDAV_IS_ULPFEC_CODEC(item->data) && !TDAV_IS_RED_CODEC(item->data) 
			&& TMEDIA_CODEC(item->data)->plugin && TMEDIA_CODEC(item->data)->plugin->encode && TMEDIA_CODEC(item->data)->plugin->decode){
			return TMEDIA_CODEC(item->data);
		}
	}
	return tsk_null;
}

const tmedia_codec_t* tdav_session_av_get_red_codec(const tdav_session_av_t* self)
{
	const tsk_list_item_t* item;
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}
	
	tsk_list_foreach(item, TMEDIA_SESSION(self)->neg_codecs){
		const tmedia_codec_t* codec = (const tmedia_codec_t*)item->data;
		if(TDAV_IS_RED_CODEC(codec)){
			return TMEDIA_CODEC(item->data);
		}
	}
	return tsk_null;
}

static int _tdav_session_av_red_cb(const void* callback_data, const struct trtp_rtp_packet_s* packet)
{
	tdav_session_av_t* self = (tdav_session_av_t*)callback_data;
	if(self->rtp_manager && self->rtp_manager->rtp.callback){
		return self->rtp_manager->rtp.callback(self->rtp_manager->rtp.callback_data, packet);
	}
	return 0;
}

const tmedia_codec_t* tdav_session_av_get_ulpfec_codec(const tdav_session_av_t* self)
{
	const tsk_list_item_t* item;
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}
	
	tsk_list_foreach(item, TMEDIA_SESSION(self)->neg_codecs){
		if(TDAV_IS_ULPFEC_CODEC(item->data)){
			return TMEDIA_CODEC(item->data);
		}
	}
	return tsk_null;
}

int tdav_session_av_deinit(tdav_session_av_t* self)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	/* deinit self (rtp manager should be destroyed after the producer) */
	TSK_OBJECT_SAFE_FREE(self->consumer);
	TSK_OBJECT_SAFE_FREE(self->producer);
	TSK_OBJECT_SAFE_FREE(self->rtp_manager);
	TSK_FREE(self->sdp_neg.remote_best_pcfg.t_proto);
	TSK_FREE(self->remote_ip);
	TSK_FREE(self->local_ip);

	/* RED and ULPFEC codecs */
	TSK_OBJECT_SAFE_FREE(self->red.codec);
	TSK_OBJECT_SAFE_FREE(self->ulpfec.codec);

	/* NAT Traversal context */
	TSK_OBJECT_SAFE_FREE(self->natt_ctx);
	TSK_OBJECT_SAFE_FREE(self->ice_ctx);

	tsk_safeobj_deinit(self);

	/* deinit base */
	tmedia_session_deinit(TMEDIA_SESSION(self));

	return 0;
}