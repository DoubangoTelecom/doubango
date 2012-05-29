/*
* Copyright (C) 2010-2011 Mamadou Diop.
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
 */
#include "tinydav/video/tdav_session_video.h"
#include "tinydav/video/tdav_converter_video.h"
#include "tinydav/video/jb/tdav_video_jb.h"
#include "tinydav/codecs/fec/tdav_codec_red.h"
#include "tinydav/codecs/fec/tdav_codec_ulpfec.h"


#include "tinymedia/tmedia_consumer.h"
#include "tinymedia/tmedia_producer.h"
#include "tinymedia/tmedia_defaults.h"
#include "tinymedia/tmedia_params.h"

#include "tinyrtp/trtp_manager.h"
#include "tinyrtp/rtcp/trtp_rtcp_header.h"
#include "tinyrtp/rtp/trtp_rtp_packet.h"
#include "tinyrtp/rtcp/trtp_rtcp_packet.h"
#include "tinyrtp/rtcp/trtp_rtcp_report_rr.h"
#include "tinyrtp/rtcp/trtp_rtcp_report_sr.h"
#include "tinyrtp/rtcp/trtp_rtcp_report_fb.h"

#include "tsk_memory.h"
#include "tsk_debug.h"

#define TDAV_SESSION_VIDEO_AVPF_PACKETS_MAX_MIN	20
#define TDAV_SESSION_VIDEO_AVPF_PACKETS_MAX_MAX	(TDAV_SESSION_VIDEO_AVPF_PACKETS_MAX_MIN << 3)
#define TDAV_SESSION_VIDEO_AVPF_FIR_INTERVAL_MIN	800 // millis

#define TDAV_SESSION_VIDEO_PKT_LOSS_PROB_BAD	2
#define TDAV_SESSION_VIDEO_PKT_LOSS_PROB_GOOD	6
#define TDAV_SESSION_VIDEO_PKT_LOSS_FACT_MIN	0
#define TDAV_SESSION_VIDEO_PKT_LOSS_FACT_MAX	8
#define TDAV_SESSION_VIDEO_PKT_LOSS_LOW			9
#define TDAV_SESSION_VIDEO_PKT_LOSS_MEDIUM		22
#define TDAV_SESSION_VIDEO_PKT_LOSS_HIGH		63

static const tmedia_codec_action_t __action_encode_idr = tmedia_codec_action_encode_idr;
static const tmedia_codec_action_t __action_encode_bw_up = tmedia_codec_action_bw_up;
static const tmedia_codec_action_t __action_encode_bw_down = tmedia_codec_action_bw_down;

// FIXME: lock
#define _tdav_session_video_codec_set(__self, key, value) \
	{ \
		static tmedia_param_t* __param = tsk_null; \
		if(!__param){ \
			__param = tmedia_param_create(tmedia_pat_set,  \
											tmedia_video,  \
											tmedia_ppt_codec,  \
											tmedia_pvt_int32, \
											"action", \
											(void*)&value); \
		} \
		if((__self)->encoder.codec && __param){ \
			/*tsk_mutex_lock((__self)->encoder.h_mutex);*/ \
			tmedia_codec_set((tmedia_codec_t*)(__self)->encoder.codec, __param); \
			/*tsk_mutex_unlock((__self)->encoder.h_mutex);*/ \
		} \
		/* TSK_OBJECT_SAFE_FREE(param); */ \
	}

#define _tdav_session_video_request_idr(__self) { \
	uint64_t __now = tsk_time_now(); \
	if((__now - (__self)->avpf.last_fir_time) > TDAV_SESSION_VIDEO_AVPF_FIR_INTERVAL_MIN){ /* guard to avoid sending too many FIR */ \
		_tdav_session_video_codec_set((__self), "action", __action_encode_idr); \
	} \
	(__self)->avpf.last_fir_time = __now; \
}
#define _tdav_session_video_bw_up(__self) _tdav_session_video_codec_set(__self, "action", __action_encode_bw_up)
#define _tdav_session_video_bw_down(__self) _tdav_session_video_codec_set(__self, "action", __action_encode_bw_down)

#define _tdav_session_video_reset_loss_prob(__self) \
{ \
	(__self)->encoder.pkt_loss_level = tdav_session_video_pkt_loss_level_low; \
	(__self)->encoder.pkt_loss_prob_bad = TDAV_SESSION_VIDEO_PKT_LOSS_PROB_BAD; \
	(__self)->encoder.pkt_loss_prob_good = TDAV_SESSION_VIDEO_PKT_LOSS_PROB_GOOD; \
}
static int _tdav_session_video_jb_cb(const tdav_video_jb_cb_data_xt* data);
static int _tdav_session_video_open_decoder(tdav_session_video_t* self, uint8_t payload_type);
static int _tdav_session_video_decode(tdav_session_video_t* self, const trtp_rtp_packet_t* packet);
static int _tdav_session_video_set_callbacks(tmedia_session_t* self);

// Codec callback (From codec to the network)
// or Producer callback to send() data "as is"
static int tdav_session_video_raw_cb(const tmedia_video_encode_result_xt* result)
{
	tdav_session_av_t* base = (tdav_session_av_t*)result->usr_data;
	tdav_session_video_t* video = (tdav_session_video_t*)result->usr_data;
	trtp_rtp_packet_t* packet = tsk_null;
	int ret = 0;
	tsk_size_t s;
	
	if(base->rtp_manager && base->rtp_manager->is_started){
		if((packet = trtp_rtp_packet_create(base->rtp_manager->rtp.ssrc, base->rtp_manager->rtp.seq_num, base->rtp_manager->rtp.timestamp, base->rtp_manager->rtp.payload_type, result->last_chunck))){
			tsk_size_t rtp_hdr_size;
			if(result->last_chunck){
				base->rtp_manager->rtp.timestamp += result->duration;
			}

			packet->payload.data_const = result->buffer.ptr;
			packet->payload.size = result->buffer.size;
			s = trtp_manager_send_rtp_packet(base->rtp_manager, packet, tsk_false); // encrypt and send data
			if(s < TRTP_RTP_HEADER_MIN_SIZE) { 
				TSK_DEBUG_ERROR("Failed to send packet"); 
				goto bail; 
			}
			++base->rtp_manager->rtp.seq_num;
			rtp_hdr_size = TRTP_RTP_HEADER_MIN_SIZE + (packet->header->csrc_count << 2);
			// Save packet
			// FIXME: only if AVPF is enabled
			if(1){
				trtp_rtp_packet_t* packet_avpf = tsk_object_ref(packet);
				// when SRTP is used, "serial_buffer" will contains the encoded buffer with both RTP header and payload
				// Hack the RTP packet payload to point to the the SRTP data instead of unencrypted ptr
				packet_avpf->payload.size = (s - rtp_hdr_size);
				packet_avpf->payload.data_const = tsk_null;
				if(!(packet_avpf->payload.data = tsk_malloc(packet_avpf->payload.size))){// FIXME: to be optimized (reuse memory address)
					TSK_DEBUG_ERROR("failed to allocate buffer");
					goto bail;
				}
				memcpy(packet_avpf->payload.data, (((const uint8_t*)base->rtp_manager->rtp.serial_buffer.ptr) + rtp_hdr_size), packet_avpf->payload.size);
				tsk_list_lock(video->avpf.packets);
				if(video->avpf.count > video->avpf.max){
					tsk_list_remove_first_item(video->avpf.packets);
				}
				else{
					++video->avpf.count;
				}

				tsk_list_push_back_data(video->avpf.packets, (void**)&packet_avpf);
				tsk_list_unlock(video->avpf.packets);
			}

			// Send FEC packet
			// FIXME: protect only Intra and Params packets
			if(video->ulpfec.codec && (s > TRTP_RTP_HEADER_MIN_SIZE)){
				packet->payload.data_const = (((const uint8_t*)base->rtp_manager->rtp.serial_buffer.ptr) + rtp_hdr_size);
				packet->payload.size = (s - rtp_hdr_size);
				ret = tdav_codec_ulpfec_enc_protect((struct tdav_codec_ulpfec_s*)video->ulpfec.codec, packet);
				if(result->last_chunck){
					trtp_rtp_packet_t* packet_fec;
					if((packet_fec = trtp_rtp_packet_create(base->rtp_manager->rtp.ssrc, video->ulpfec.seq_num++, video->ulpfec.timestamp, video->ulpfec.payload_type, tsk_true))){
						// serialize the FEC payload packet packet
						s = tdav_codec_ulpfec_enc_serialize((const struct tdav_codec_ulpfec_s*)video->ulpfec.codec, &video->encoder.buffer, &video->encoder.buffer_size);
						if(s > 0){
							packet_fec->payload.data_const = video->encoder.buffer;
							packet_fec->payload.size = s;
							s = trtp_manager_send_rtp_packet(base->rtp_manager, packet_fec, tsk_true/* already encrypted */);
						}
						TSK_OBJECT_SAFE_FREE(packet_fec);
					}
					video->ulpfec.timestamp += result->duration;
					ret = tdav_codec_ulpfec_enc_reset((struct tdav_codec_ulpfec_s*)video->ulpfec.codec);
				}
			}
#if 0
			// Send RED Packet
			if(ret == 0 && video->red.codec){
				// don't need to lock as the buffer is never used by other codecs
				tsk_size_t red_pay_size = video->red.codec->plugin->encode(
						video->red.codec, 
						buffer, size, 
						&video->encoder.buffer, &video->encoder.buffer_size
					);
				if(red_pay_size > 1){
					packet->header->payload_type = video->red.payload_type;
					((uint8_t*)video->encoder.buffer)[0] = packet->header->payload_type;
					packet->payload.data_const = video->encoder.buffer;
					packet->payload.size = red_pay_size;
					trtp_manager_send_rtp_2(base->rtp_manager, packet);
				}
			}
#endif
		}
	}
	else{
		TSK_DEBUG_ERROR("Failed to create packet");
	}
	
bail:
	TSK_OBJECT_SAFE_FREE(packet);
	return ret;
}

// Codec Callback after decoding
static int tdav_session_video_decode_cb(const tmedia_video_decode_result_xt* result)
{
	tdav_session_av_t* base = (tdav_session_av_t*)result->usr_data;

	switch(result->type){
		case tmedia_video_decode_result_type_error:
			{
				TSK_DEBUG_INFO("Decoding failed -> send Full Intra Request (FIR)");
				trtp_manager_signal_frame_corrupted(base->rtp_manager, ((const trtp_rtp_header_t*)result->proto_hdr)->ssrc);
				break;
			}
	}
	return 0;
}

// Producer callback (From the producer to the network) => encode data before send()
static int tdav_session_video_producer_enc_cb(const void* callback_data, const void* buffer, tsk_size_t size)
{
	tdav_session_video_t* video = (tdav_session_video_t*)callback_data;
	tdav_session_av_t* base = (tdav_session_av_t*)callback_data;
	tsk_size_t yuv420p_size = 0;
	int ret = 0;

	if(!base){
		TSK_DEBUG_ERROR("Null session");
		return 0;
	}

	if(TMEDIA_SESSION(base)->lo_held){
		return 0;
	}

	if(base->rtp_manager && video->encoder.codec){
		/* encode */
		tsk_size_t out_size = 0;

		if(!base->rtp_manager->is_started){
			TSK_DEBUG_ERROR("Not started");
			return 0;
		}
	
#define PRODUCER_SIZE_CHANGED (video->conv.producerWidth != base->producer->video.width) || (video->conv.producerHeight != base->producer->video.height) \
|| (video->conv.xProducerSize != size)
#define ENCODED_NEED_FLIP TMEDIA_CODEC_VIDEO(video->encoder.codec)->out.flip
		// Video codecs only accept YUV420P buffers ==> do conversion if needed or producer doesn't have the right size
		if((base->producer->video.chroma != TMEDIA_CODEC_VIDEO(video->encoder.codec)->out.chroma) || PRODUCER_SIZE_CHANGED || ENCODED_NEED_FLIP){
			// Create video converter if not already done or producer size has changed
			if(!video->conv.toYUV420 || PRODUCER_SIZE_CHANGED){
				TSK_OBJECT_SAFE_FREE(video->conv.toYUV420);
				video->conv.producerWidth = base->producer->video.width;
				video->conv.producerHeight = base->producer->video.height;
				video->conv.xProducerSize = size;
				
				if(!(video->conv.toYUV420 = tdav_converter_video_create(base->producer->video.width, base->producer->video.height, base->producer->video.chroma, TMEDIA_CODEC_VIDEO(video->encoder.codec)->out.width, TMEDIA_CODEC_VIDEO(video->encoder.codec)->out.height,
					TMEDIA_CODEC_VIDEO(video->encoder.codec)->out.chroma))){
					TSK_DEBUG_ERROR("Failed to create video converter");
					ret = -5;
					goto bail;
				}
			}
		}

		if(video->conv.toYUV420){
			// update one-shot parameters
			tdav_converter_video_init(video->conv.toYUV420, base->producer->video.rotation, TMEDIA_CODEC_VIDEO(video->encoder.codec)->out.flip);
			// convert data to yuv420p
			yuv420p_size = tdav_converter_video_convert(video->conv.toYUV420, buffer, &video->encoder.conv_buffer, &video->encoder.conv_buffer_size);
			if(!yuv420p_size || !video->encoder.conv_buffer){
				TSK_DEBUG_ERROR("Failed to convert XXX buffer to YUV42P");
				ret = -6;
				goto bail;
			}
		}

		// Encode data
		tsk_mutex_lock(video->encoder.h_mutex);
		if(video->encoder.conv_buffer && yuv420p_size){
			/* producer doesn't support yuv42p */
			out_size = video->encoder.codec->plugin->encode(video->encoder.codec, video->encoder.conv_buffer, yuv420p_size, &video->encoder.buffer, &video->encoder.buffer_size);
		}
		else{
			/* producer supports yuv42p */
			out_size = video->encoder.codec->plugin->encode(video->encoder.codec, buffer, size,  &video->encoder.buffer, &video->encoder.buffer_size);
		}
		tsk_mutex_unlock(video->encoder.h_mutex);

		if(out_size){
			/* Never called, see tdav_session_video_raw_cb() */
			trtp_manager_send_rtp(base->rtp_manager, video->encoder.buffer, out_size, 6006, tsk_true, tsk_true);
		}
bail: ;
	}
	else{
		TSK_DEBUG_ERROR("Invalid parameter");
		ret = -1;
	}

	return ret;
}

// RTP callback (Network -> Decoder -> Consumer)
static int tdav_session_video_rtp_cb(const void* callback_data, const trtp_rtp_packet_t* packet)
{
	tdav_session_video_t* video = (tdav_session_video_t*)callback_data;

	if(!video || !packet || !packet->header){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(packet->header->payload_type == video->red.payload_type){
		static void* __red_buffer_ptr = tsk_null; // Never used
		static tsk_size_t __red_buffer_size = 0; // Never used
		if(!video->red.codec){
			TSK_DEBUG_ERROR("No RED codec could be found");
			return -2;
		}
		// Decode RED data
		video->red.codec->plugin->decode(
				video->red.codec, 
				(packet->payload.data ? packet->payload.data : packet->payload.data_const), packet->payload.size, 
				&__red_buffer_ptr, &__red_buffer_size, 
				packet->header
			);
		return 0;
	}
	else if(packet->header->payload_type == video->ulpfec.payload_type){
		if(!video->ulpfec.codec){
			TSK_DEBUG_ERROR("No ULPFEC codec could be found");
			return -2;
		}
		// FIXME: do something
		return 0;
	}
	else{
		return tdav_video_jb_put(video->jb, (trtp_rtp_packet_t*)packet);
	}
}

// RTCP callback (Network -> This)
static int tdav_session_video_rtcp_cb(const void* callback_data, const trtp_rtcp_packet_t* packet)
{
	const trtp_rtcp_report_psfb_t* psfb;
	const trtp_rtcp_report_rtpfb_t* rtpfb;
	const trtp_rtcp_rblocks_L_t* blocks = tsk_null;
	
	tdav_session_video_t* video = (tdav_session_video_t*)callback_data;
	tdav_session_av_t* base = (tdav_session_av_t*)callback_data;
	tsk_size_t i;

	if((blocks = (packet->header->type == trtp_rtcp_packet_type_rr) ? ((const trtp_rtcp_report_rr_t*)packet)->blocks : 
		(packet->header->type == trtp_rtcp_packet_type_sr ? ((const trtp_rtcp_report_sr_t*)packet)->blocks : tsk_null))){
		const tsk_list_item_t* item;
		const trtp_rtcp_rblock_t* block;
		tsk_list_foreach(item, blocks){
			if(!(block = item->data)) continue;
			if(base->rtp_manager->rtp.ssrc == block->ssrc){
				tdav_session_video_pkt_loss_level_t pkt_loss_level = tdav_session_video_pkt_loss_level_low;
				if(block->fraction > TDAV_SESSION_VIDEO_PKT_LOSS_HIGH)  pkt_loss_level = tdav_session_video_pkt_loss_level_high;
				else if(block->fraction > TDAV_SESSION_VIDEO_PKT_LOSS_MEDIUM)  pkt_loss_level = tdav_session_video_pkt_loss_level_medium;
				if(pkt_loss_level == tdav_session_video_pkt_loss_level_high || (pkt_loss_level > video->encoder.pkt_loss_level)){ // high or low -> medium
					video->encoder.pkt_loss_level = pkt_loss_level;
					if(video->encoder.pkt_loss_prob_bad-- <= 0){
						int32_t new_pkt_loss_fact = TSK_CLAMP(TDAV_SESSION_VIDEO_PKT_LOSS_FACT_MIN, (video->encoder.pkt_loss_fact + 1), TDAV_SESSION_VIDEO_PKT_LOSS_FACT_MAX);
						if(video->encoder.pkt_loss_fact != new_pkt_loss_fact){
							TSK_DEBUG_INFO("Downgrade bandwidth %d->%d", video->encoder.pkt_loss_fact, new_pkt_loss_fact);
							video->encoder.pkt_loss_fact = new_pkt_loss_fact;
							_tdav_session_video_bw_down(video);
						}
						_tdav_session_video_reset_loss_prob(video);
					}
				}
				else{
					if(video->encoder.pkt_loss_prob_good-- <= 0){
						int32_t new_pkt_loss_fact = TSK_CLAMP(TDAV_SESSION_VIDEO_PKT_LOSS_FACT_MIN, (video->encoder.pkt_loss_fact - 1), TDAV_SESSION_VIDEO_PKT_LOSS_FACT_MAX);
						if(video->encoder.pkt_loss_fact != new_pkt_loss_fact){
							TSK_DEBUG_INFO("Upgrade bandwidth %d->%d", video->encoder.pkt_loss_fact, new_pkt_loss_fact);
							video->encoder.pkt_loss_fact = new_pkt_loss_fact;
							_tdav_session_video_bw_up(video);
						}
						_tdav_session_video_reset_loss_prob(video);
					}
				}
				break;
			}
		}
	}

	i = 0;
	while((psfb = (const trtp_rtcp_report_psfb_t*)trtp_rtcp_packet_get_at(packet, trtp_rtcp_packet_type_psfb, i++))){
		switch(psfb->fci_type){
			case trtp_rtcp_psfb_fci_type_fir:
				{
					_tdav_session_video_request_idr(video);
					break;
				}
			case trtp_rtcp_psfb_fci_type_pli:
				{
					uint64_t now = tsk_time_now();
					if((now - video->avpf.last_pli_time) < 500){ // more than one PLI in 500ms
						_tdav_session_video_request_idr(video);
					}
					video->avpf.last_pli_time = now;
					break;
				}
			default: break;
		}
	}
	i = 0;
	while((rtpfb = (const trtp_rtcp_report_rtpfb_t*)trtp_rtcp_packet_get_at(packet, trtp_rtcp_packet_type_rtpfb, i++))){
		switch(rtpfb->fci_type){
			case trtp_rtcp_rtpfb_fci_type_nack:
				{
					if(rtpfb->nack.blp && rtpfb->nack.pid){
						tsk_size_t i;
						int32_t j;
						uint16_t pid, blp;
						const tsk_list_item_t* item;
						const trtp_rtp_packet_t* pkt_rtp;
						for(i = 0; i < rtpfb->nack.count; ++i){
							static const int32_t __Pow2[16] = { 0x8000, 0x4000, 0x2000, 0x1000, 0x800, 0x400, 0x200, 0x100, 0x80, 0x40, 0x20, 0x10, 0x8, 0x4, 0x2, 0x1 };
							int32_t blp_count;
							blp = rtpfb->nack.blp[i];
							blp_count = blp ? 16 : 0;
							
							for(j = -1; j < blp_count; ++j){
								if(j == -1 || (blp & (1 << __Pow2[j]))){
									pid = (rtpfb->nack.pid[i] + (j + 1));
									tsk_list_lock(video->avpf.packets);
									tsk_list_foreach(item, video->avpf.packets){
										if(!(pkt_rtp = item->data)){
											continue;
										}										

										if(pkt_rtp->header->seq_num > pid){
											int32_t old_max = video->avpf.max;
											int32_t len_drop = (pkt_rtp->header->seq_num - pid);
											video->avpf.max = TSK_CLAMP(TDAV_SESSION_VIDEO_AVPF_PACKETS_MAX_MIN, (old_max + len_drop), TDAV_SESSION_VIDEO_AVPF_PACKETS_MAX_MAX);
											TSK_DEBUG_INFO("**NACK requesting dropped frames [requested_frame=%d, len_drop=%d, new_buff_size=%d]. RTT is probably too high.", pid, len_drop, video->avpf.max);
											tsk_list_clear_items(video->avpf.packets);
											video->avpf.count = 0;
											_tdav_session_video_request_idr(video);
											goto done;
										}
										if(pkt_rtp->header->seq_num == pid){
											// TSK_DEBUG_INFO("NACK Found=%d", pid);
											trtp_manager_send_rtp_packet(base->rtp_manager, pkt_rtp, tsk_true);
											break;
										}
										if(item == video->avpf.packets->tail){
											// must never be called
											TSK_DEBUG_INFO("**NACK Not Found=%d", pid);
										}
									}
done:
									tsk_list_unlock(video->avpf.packets);
								}
							}
						}
					}
					break;
				}
		}
	}

	return 0;
}

// From jitter buffer to codec
static int _tdav_session_video_jb_cb(const tdav_video_jb_cb_data_xt* data)
{
	tdav_session_video_t* video = (tdav_session_video_t*)data->usr_data;
	tdav_session_av_t* base = (tdav_session_av_t*)data->usr_data;

	switch(data->type){
		case tdav_video_jb_cb_data_type_rtp:
			{
				return _tdav_session_video_decode(video, data->rtp.pkt);
			}
		case tdav_video_jb_cb_data_type_tmfr:
			{
				return trtp_manager_signal_jb_error(base->rtp_manager, data->ssrc);
			}
		case tdav_video_jb_cb_data_type_fl:
			{
				tsk_size_t i, j, k;
				uint16_t seq_nums[16];
				for(i = 0; i < data->fl.count; i+=16){
					for(j = 0, k = i; j < 16 && k < data->fl.count; ++j, ++k){
						seq_nums[j] = (data->fl.seq_num + i + j);
					}
					trtp_manager_signal_pkt_loss(base->rtp_manager, data->ssrc, seq_nums, j);
				}
				break;
			}
	}

	return 0;
}

int _tdav_session_video_open_decoder(tdav_session_video_t* self, uint8_t payload_type)
{
	int ret = 0;

	if((self->decoder.payload_type != payload_type) || !self->decoder.codec){
		tsk_istr_t format;
		TSK_OBJECT_SAFE_FREE(self->decoder.codec);
		tsk_itoa(payload_type, &format);
		if(!(self->decoder.codec = tmedia_codec_find_by_format(TMEDIA_SESSION(self)->neg_codecs, format)) || !self->decoder.codec->plugin || !self->decoder.codec->plugin->decode){
			TSK_DEBUG_ERROR("%s is not a valid payload for this session", format);
			ret = -2;
			goto bail;
		}
		self->decoder.payload_type = payload_type;
	}
	// Open codec if not already done
	if(!TMEDIA_CODEC(self->decoder.codec)->opened){
		if((ret = tmedia_codec_open(self->decoder.codec))){
			TSK_DEBUG_ERROR("Failed to open [%s] codec", self->decoder.codec->plugin->desc);
			goto bail;
		}
	}

bail:
	return ret;
}

static int _tdav_session_video_decode(tdav_session_video_t* self, const trtp_rtp_packet_t* packet)
{
	tdav_session_av_t* base = (tdav_session_av_t*)self;
	static const trtp_rtp_header_t* rtp_header = tsk_null;
	int ret = 0;

	if(!self || !packet || !packet->header){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	tsk_safeobj_lock(base);

	if(base->consumer){
		tsk_size_t out_size;
				
		// Find the codec to use to decode the RTP payload
		if(!self->decoder.codec || self->decoder.payload_type != packet->header->payload_type){
			if((ret = _tdav_session_video_open_decoder(self, packet->header->payload_type))){
				goto bail;
			}
		}		
		
		// Decode data
		out_size = self->decoder.codec->plugin->decode(
				self->decoder.codec, 
				(packet->payload.data ? packet->payload.data : packet->payload.data_const), packet->payload.size, 
				&self->decoder.buffer, &self->decoder.buffer_size,
				packet->header
			);
		// check
		if(!out_size || !self->decoder.buffer){
			goto bail;
		}

		// Convert decoded data to the consumer chroma and size
#define CONSUMER_INSIZE_MISMATCH				((base->consumer->video.in.width * base->consumer->video.in.height * 3)>>1 != out_size)// we have good reasons not to use 1.5f
#define CONSUMER_IN_N_DISPLAY_MISMATCH		(base->consumer->video.in.width != base->consumer->video.display.width || base->consumer->video.in.height != base->consumer->video.display.height)
#define CONSUMER_DISPLAY_N_CODEC_MISMATCH		(base->consumer->video.display.width != TMEDIA_CODEC_VIDEO(self->decoder.codec)->in.width || base->consumer->video.display.height != TMEDIA_CODEC_VIDEO(self->decoder.codec)->in.height)
#define CONSUMER_DISPLAY_N_CONVERTER_MISMATCH	( (self->conv.fromYUV420 && self->conv.fromYUV420->dstWidth != base->consumer->video.display.width) || (self->conv.fromYUV420 && self->conv.fromYUV420->dstHeight != base->consumer->video.display.height) )
#define CONSUMER_CHROMA_MISMATCH	(base->consumer->video.display.chroma != TMEDIA_CODEC_VIDEO(self->decoder.codec)->in.chroma)
#define DECODED_NEED_FLIP	(TMEDIA_CODEC_VIDEO(self->decoder.codec)->in.flip)

		if((CONSUMER_CHROMA_MISMATCH || CONSUMER_DISPLAY_N_CODEC_MISMATCH || CONSUMER_IN_N_DISPLAY_MISMATCH || CONSUMER_INSIZE_MISMATCH || CONSUMER_DISPLAY_N_CONVERTER_MISMATCH || DECODED_NEED_FLIP)){

			// Create video converter if not already done
			if(!self->conv.fromYUV420 || CONSUMER_DISPLAY_N_CONVERTER_MISMATCH || CONSUMER_INSIZE_MISMATCH){
				TSK_OBJECT_SAFE_FREE(self->conv.fromYUV420);
				// update in (set by the codec)
				base->consumer->video.in.width = TMEDIA_CODEC_VIDEO(self->decoder.codec)->in.width;//decoded width
				base->consumer->video.in.height = TMEDIA_CODEC_VIDEO(self->decoder.codec)->in.height;//decoded height
				
				// important: do not override the display size (used by the end-user) unless requested
				if(base->consumer->video.display.auto_resize){
					base->consumer->video.display.width = base->consumer->video.in.width;
					base->consumer->video.display.height = base->consumer->video.in.height;
				}
				// create converter
				if(!(self->conv.fromYUV420 = tdav_converter_video_create(TMEDIA_CODEC_VIDEO(self->decoder.codec)->in.width, TMEDIA_CODEC_VIDEO(self->decoder.codec)->in.height, TMEDIA_CODEC_VIDEO(self->decoder.codec)->in.chroma, base->consumer->video.display.width, base->consumer->video.display.height,
					base->consumer->video.display.chroma))){
					TSK_DEBUG_ERROR("Failed to create video converter");
					ret = -3;
					goto bail;
				}
			}
		}

		if(self->conv.fromYUV420){
			// update one-shot parameters
			tdav_converter_video_init(self->conv.fromYUV420, 0/*rotation*/, TMEDIA_CODEC_VIDEO(self->decoder.codec)->in.flip);
			// convert data to the consumer's chroma
			out_size = tdav_converter_video_convert(self->conv.fromYUV420, self->decoder.buffer, &self->decoder.conv_buffer, &self->decoder.conv_buffer_size);
			if(!out_size || !self->decoder.conv_buffer){
				TSK_DEBUG_ERROR("Failed to convert YUV420 buffer to consumer's chroma");
				ret = -4;
				goto bail;
			}

			tmedia_consumer_consume(base->consumer, self->decoder.conv_buffer, out_size, rtp_header);
			if(!self->decoder.conv_buffer){
				/* taken  by the consumer */
				self->decoder.conv_buffer_size = 0;
			}
			
		}
		else{
			tmedia_consumer_consume(base->consumer, self->decoder.buffer, out_size, rtp_header);
			if(!self->decoder.buffer){
				/* taken  by the consumer */
				self->decoder.buffer_size = 0;
			}
		}		
	}

bail:
	tsk_safeobj_unlock(base);

	return ret;
}


/* ============ Plugin interface ================= */

static int tdav_session_video_set(tmedia_session_t* self, const tmedia_param_t* param)
{
	int ret = 0;
	tdav_session_video_t* video;
	tdav_session_av_t* base;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(tdav_session_av_set(TDAV_SESSION_AV(self), param) == tsk_true){
		return 0;
	}

	video = (tdav_session_video_t*)self;
	base = (tdav_session_av_t*)self;

	if(param->plugin_type == tmedia_ppt_codec){
		tsk_mutex_lock(video->encoder.h_mutex);
		ret = tmedia_codec_set((tmedia_codec_t*)video->encoder.codec, param);
		tsk_mutex_unlock(video->encoder.h_mutex);
	}
	else if(param->plugin_type == tmedia_ppt_consumer){
		if(!base->consumer){
			TSK_DEBUG_ERROR("No consumer associated to this session");
			return -1;
		}
		if(param->value_type == tmedia_pvt_int32){
			if(tsk_striequals(param->key, "flip")){
				tsk_list_item_t* item;
				tsk_bool_t flip = (tsk_bool_t)TSK_TO_INT32((uint8_t*)param->value);
				tmedia_codecs_L_t *codecs = tsk_object_ref(self->codecs);
				tsk_list_foreach(item, codecs){
					TMEDIA_CODEC_VIDEO(item->data)->in.flip = flip;
				}
				tsk_object_unref(codecs);
			}
		}
		ret = tmedia_consumer_set(base->consumer, param);
	}
	else if(param->plugin_type == tmedia_ppt_producer){
		if(!base->producer){
			TSK_DEBUG_ERROR("No producer associated to this session");
			return -1;
		}
		if(param->value_type == tmedia_pvt_int32){
			if(tsk_striequals(param->key, "flip")){
				tsk_list_item_t* item;
				tsk_bool_t flip = (tsk_bool_t)TSK_TO_INT32((uint8_t*)param->value);
				tmedia_codecs_L_t *codecs = tsk_object_ref(self->codecs);
				tsk_list_foreach(item, codecs){
					TMEDIA_CODEC_VIDEO(item->data)->out.flip = flip;
				}
				tsk_object_unref(codecs);
			}
		}
		ret = tmedia_producer_set(base->producer, param);
	}
	else{
		if(param->value_type == tmedia_pvt_int32){
			if(tsk_striequals(param->key, "bandwidth-level")){
				tsk_list_item_t* item;
				self->bl = (tmedia_bandwidth_level_t) TSK_TO_INT32((uint8_t*)param->value);
				self->codecs = tsk_object_ref(self->codecs);
				tsk_list_foreach(item, self->codecs){
					((tmedia_codec_t*)item->data)->bl = self->bl;
				}
				tsk_object_unref(self->codecs);
			}
		}
	}

	return ret;
}

static int tdav_session_video_get(tmedia_session_t* self, tmedia_param_t* param)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	
	if(tdav_session_av_get(TDAV_SESSION_AV(self), param) == tsk_true){
		return 0;
	}

	TSK_DEBUG_ERROR("Not expected");
	return -2;
}

static int tdav_session_video_prepare(tmedia_session_t* self)
{
	tdav_session_av_t* base = (tdav_session_av_t*)(self);
	int ret;

	if((ret = tdav_session_av_prepare(base))){
		TSK_DEBUG_ERROR("tdav_session_av_prepare(video) failed");
		return ret;
	}

	if(base->rtp_manager){
		ret = trtp_manager_set_rtp_callback(base->rtp_manager, tdav_session_video_rtp_cb, base);
		ret = trtp_manager_set_rtcp_callback(base->rtp_manager, tdav_session_video_rtcp_cb, base);
	}

	return ret;
}

static int tdav_session_video_start(tmedia_session_t* self)
{
	int ret;
	tdav_session_video_t* video;
	const tmedia_codec_t* codec;
	tdav_session_av_t* base;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	video = (tdav_session_video_t*)self;
	base = (tdav_session_av_t*)self;

	// ENCODER codec
	if(!(codec = tdav_session_av_get_best_neg_codec(base))){
		TSK_DEBUG_ERROR("No codec matched");
		return -2;
	}
	tsk_mutex_lock(video->encoder.h_mutex);
	TSK_OBJECT_SAFE_FREE(video->encoder.codec);
	video->encoder.codec = tsk_object_ref((tsk_object_t*)codec);
	if(!TMEDIA_CODEC(video->encoder.codec)->opened){
		if((ret = tmedia_codec_open(video->encoder.codec))){
			TSK_DEBUG_ERROR("Failed to open [%s] codec", video->encoder.codec->plugin->desc);
			return ret;
		}
	}
	tsk_mutex_unlock(video->encoder.h_mutex);

	// RED codec
	TSK_OBJECT_SAFE_FREE(video->red.codec);
	video->red.payload_type = 0;
	if((video->red.codec = tsk_object_ref((tsk_object_t*)tdav_session_av_get_red_codec(base)))){
		video->red.payload_type = atoi(video->red.codec->neg_format);
		if(!TMEDIA_CODEC(video->red.codec)->opened){
			if((ret = tmedia_codec_open(video->red.codec))){
				TSK_DEBUG_ERROR("Failed to open [%s] codec", video->red.codec->plugin->desc);
				return ret;
			}
		}
	}

	// ULPFEC
	TSK_OBJECT_SAFE_FREE(video->ulpfec.codec);
	video->ulpfec.payload_type = 0;
	if((video->ulpfec.codec = tsk_object_ref((tsk_object_t*)tdav_session_av_get_ulpfec_codec(base)))){
		video->ulpfec.payload_type = atoi(video->ulpfec.codec->neg_format);
		if(!TMEDIA_CODEC(video->ulpfec.codec)->opened){
			if((ret = tmedia_codec_open(video->ulpfec.codec))){
				TSK_DEBUG_ERROR("Failed to open [%s] codec", video->ulpfec.codec->plugin->desc);
				return ret;
			}
		}
	}

	if((ret = tdav_video_jb_start(video->jb))){
		TSK_DEBUG_ERROR("Failed to start jitter buffer");
		return ret;
	}

	if((ret = tdav_session_av_start(base, video->encoder.codec))){
		TSK_DEBUG_ERROR("tdav_session_av_start(video) failed");
		return ret;
	}	

	return ret;
}

static int tdav_session_video_stop(tmedia_session_t* self)
{
	int ret;
		
	ret = tdav_video_jb_stop(TDAV_SESSION_VIDEO(self)->jb);
	ret = tdav_session_av_stop(TDAV_SESSION_AV(self));
	TSK_OBJECT_SAFE_FREE(TDAV_SESSION_VIDEO(self)->encoder.codec);
	TSK_OBJECT_SAFE_FREE(TDAV_SESSION_VIDEO(self)->decoder.codec);
	return ret;
}

static int tdav_session_video_pause(tmedia_session_t* self)
{
	return tdav_session_av_pause(TDAV_SESSION_AV(self));
}

static const tsdp_header_M_t* tdav_session_video_get_lo(tmedia_session_t* self)
{
	tsk_bool_t updated = tsk_false;
	const tsdp_header_M_t* ret;
	tdav_session_av_t* base = TDAV_SESSION_AV(self);


	if(!(ret = tdav_session_av_get_lo(base, &updated))){
		TSK_DEBUG_ERROR("tdav_session_av_get_lo(video) failed");
		return tsk_null;
	}

	if(updated){
		// set callbacks
		_tdav_session_video_set_callbacks(self);
	}
	
	return ret;
}

static int tdav_session_video_set_ro(tmedia_session_t* self, const tsdp_header_M_t* m)
{
	int ret;
	tsk_bool_t updated = tsk_false;
	tdav_session_av_t* base = TDAV_SESSION_AV(self);

	if((ret = tdav_session_av_set_ro(base, m, &updated))){
		TSK_DEBUG_ERROR("tdav_session_av_set_ro(video) failed");
		return ret;
	}

	if(updated){
		// set callbacks
		ret = _tdav_session_video_set_callbacks(self);
	}	

	return ret;
}

static int _tdav_session_video_set_callbacks(tmedia_session_t* self)
{
	if(self){
		tsk_list_item_t* item;
		tsk_list_foreach(item, TMEDIA_SESSION(self)->neg_codecs){
			// set codec callbacks
			tmedia_codec_video_set_enc_callback(TMEDIA_CODEC_VIDEO(item->data), tdav_session_video_raw_cb, self);
			tmedia_codec_video_set_dec_callback(TMEDIA_CODEC_VIDEO(item->data), tdav_session_video_decode_cb, self);
			// set RED callback: redundant data to decode and send to the consumer
			if(TMEDIA_CODEC(item->data)->plugin == tdav_codec_red_plugin_def_t){
				tdav_codec_red_set_callback((struct tdav_codec_red_s *)(item->data), tdav_session_video_rtp_cb, self);
			}
		}
	}
	return 0;
}



//=================================================================================================
//	Session Video Plugin object definition
//
/* constructor */
static tsk_object_t* tdav_session_video_ctor(tsk_object_t * self, va_list * app)
{
	tdav_session_video_t *video = self;
	if(video){
		int ret;
		tdav_session_av_t *base = TDAV_SESSION_AV(self);
		static const tsk_bool_t is_audio = tsk_false;

		/* init() base */
		if((ret = tdav_session_av_init(base, is_audio)) != 0){
			TSK_DEBUG_ERROR("tdav_session_av_init(video) failed");
			return tsk_null;
		}
		
		/* init() self */
		if(!(video->encoder.h_mutex = tsk_mutex_create())){
			TSK_DEBUG_ERROR("Failed to create encode mutex");
			return tsk_null;
		}
		if(!(video->avpf.packets = tsk_list_create())){
			TSK_DEBUG_ERROR("Failed to create list");
			return tsk_null;
		}
		if(!(video->jb = tdav_video_jb_create())){
			TSK_DEBUG_ERROR("Failed to create jitter buffer");
			return tsk_null;
		}
		tdav_video_jb_set_callback(video->jb, _tdav_session_video_jb_cb, video);

		if(base->producer){
			tmedia_producer_set_enc_callback(base->producer, tdav_session_video_producer_enc_cb, self);
			tmedia_producer_set_raw_callback(base->producer, tdav_session_video_raw_cb, self);
		}
		video->avpf.max = TDAV_SESSION_VIDEO_AVPF_PACKETS_MAX_MIN;
		video->encoder.pkt_loss_level = tdav_session_video_pkt_loss_level_low;
		video->encoder.pkt_loss_prob_bad = 0; // honor first report
		video->encoder.pkt_loss_prob_good = TDAV_SESSION_VIDEO_PKT_LOSS_PROB_GOOD;
	}
	return self;
}
/* destructor */
static tsk_object_t* tdav_session_video_dtor(tsk_object_t * self)
{ 
	tdav_session_video_t *video = self;
	if(video){
		tdav_session_video_stop((tmedia_session_t*)video);
		// deinit self (rtp manager should be destroyed after the producer)
		TSK_OBJECT_SAFE_FREE(video->conv.toYUV420);
		TSK_OBJECT_SAFE_FREE(video->conv.fromYUV420);

		TSK_FREE(video->encoder.buffer);
		TSK_FREE(video->encoder.conv_buffer);
		TSK_FREE(video->decoder.buffer);
		TSK_FREE(video->decoder.conv_buffer);

		TSK_OBJECT_SAFE_FREE(video->encoder.codec);
		TSK_OBJECT_SAFE_FREE(video->decoder.codec);
		TSK_OBJECT_SAFE_FREE(video->red.codec);
		TSK_OBJECT_SAFE_FREE(video->ulpfec.codec);

		TSK_OBJECT_SAFE_FREE(video->avpf.packets);

		TSK_OBJECT_SAFE_FREE(video->jb);

		if(video->encoder.h_mutex){
			tsk_mutex_destroy(&video->encoder.h_mutex);
		}

		/* deinit() base */
		tdav_session_av_deinit(TDAV_SESSION_AV(video));
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
	
	tdav_session_video_set,
	tdav_session_video_get,
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
