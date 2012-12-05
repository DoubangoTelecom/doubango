/*
* Copyright (C) 2012 Doubango Telecom <http://www.doubango.org>
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

/**@file tdav_session_t140.c
 * @brief Session for T140 protocol (RFC 4103/5194/2198)
 */
#include "tinydav/t140/tdav_session_t140.h"
#include "tinydav/t140/tdav_consumer_t140.h"
#include "tinydav/t140/tdav_producer_t140.h"
#include "tinydav/codecs/fec/tdav_codec_red.h"

#include "tinymedia/tmedia_consumer.h"
#include "tinymedia/tmedia_producer.h"
#include "tinymedia/tmedia_defaults.h"

#include "tinyrtp/trtp_manager.h"
#include "tinyrtp/rtp/trtp_rtp_packet.h"

#include "tsk_debug.h"

#include <limits.h>

#define T140_BLOCKS_MAX_COUNT			20 /* number of blocks to buffer for packet loss detection and reordering */
#define T140_BLOCK_MAX_TRANSMIT_COUNT	4 /* maximum number of times to retransmit a bloc when RED is ON */	

#define T140_ZERO_WIDTH_NO_BREAK_SPACE 0xEFBBBF /* send at the begining of the T.140 session */
#define T140_BACKSPACE		0x08
#define T140_ESC            0x1B
#define T140_CR				0x0D
#define T140_LF				0x0A
#define T140_CR_LF          0x0D0A
#define T140_BELL           0x07
#define T140_SOS            0x98
#define T140_STRING_TERM    0x9C
#define T140_GRAPHIC_START  0x9B
#define T140_GRAPHIC_END    0x6D
#define T140_LOSS_CHAR_CHAR 0xFFFD
#define T140_LOSS_UTF8		0xEFBFBD

#define T140_WAIT_FOR_MISSING_PKT_RED_OFF	500  /*Time to wait for missing pkts when RED is OFF (in ms) */
#define T140_WAIT_FOR_MISSING_PKT_RED_ON	3000  /*Time to wait for missing pkts when RED is ON (in ms) */
#define T140_WAIT_FOR_BUFFERING				300		/* Time to wait for buffering (T.140 blocks forming) */
#define T140_WAIT_FOR_IDLE					10000 /* Time to wait before entering in IDLE state */

static const char zero_width_no_break_space[3] = 
{ 
	(T140_ZERO_WIDTH_NO_BREAK_SPACE >> 16) & 0xFF, 
	(T140_ZERO_WIDTH_NO_BREAK_SPACE >> 8) & 0xFF, 
	T140_ZERO_WIDTH_NO_BREAK_SPACE & 0xFF
};

typedef struct t140_block_s
{
	TSK_DECLARE_OBJECT;

	uint8_t pt;
	uint16_t seq_num;
	uint32_t timestamp;
	uint32_t transmit_count; // number of times this block have been transmitted
	// whether the block have been forwarded to the end user for UI display or sent through the RTP channel
	tsk_bool_t delivered;
	// whether the block contains data skipped because too late
	tsk_bool_t too_late;
	struct{
		uint8_t* ptr;
		tsk_size_t size;
	}data;
}
t140_block_t;
#define T140_BLOCK(self) ((t140_block_t*)(self))
extern const tsk_object_def_t *t140_block_def_t;

static int _tdav_session_t140_producer_enc_cb(const void* callback_data, const void* buffer, tsk_size_t size);
static int _tdav_session_t140_save_outgoing_data(tdav_session_t140_t* self, const void* data_ptr, tsk_size_t data_size);
static tsk_size_t _tdav_session_t140_encap_red(tdav_session_t140_t* self, void** out_data_ptr, tsk_size_t* out_data_size, tsk_bool_t *last_data);
static int _tdav_session_t140_rtp_cb(const void* callback_data, const struct trtp_rtp_packet_s* packet);
static int _tdav_session_t140_recv_red(tdav_session_t140_t* self, const struct trtp_rtp_packet_s* packet);
static int _tdav_session_t140_recv_raw(tdav_session_t140_t* self, const void* data_ptr, tsk_size_t data_size);
static int _tdav_session_t140_decode_and_deliver_blocks(tdav_session_t140_t* self, tsk_bool_t called_from_timer);
static int _tdav_session_t140_consume_data(tdav_session_t140_t* self, enum tmedia_t140_data_type_e data_type, const void *data_ptr, tsk_size_t data_size);
static int _tdav_session_t140_timer_cb(const void* arg, tsk_timer_id_t timer_id);
static int _tdav_session_t140_pred_block_with_transmit_count_more_than_or_equal(const tsk_list_item_t *item, const void *transmit_count);

static tsk_bool_t _tdav_session_t140_blocks_has_gap(tdav_session_t140_blocks_L_t* blocks);
static tsk_bool_t _tdav_session_t140_blocks_has_seqnum(tdav_session_t140_blocks_L_t* blocks, uint16_t seq_num);
static int _tdav_session_t140_blocks_add(tdav_session_t140_blocks_L_t* blocks, int64_t* blocks_count, t140_block_t** block, int64_t blocks_count_max);
static t140_block_t* _tdav_session_t140_block_create(uint8_t pt, uint16_t seq_num, uint32_t timestamp, tsk_bool_t delivered, const void* data_ptr, tsk_size_t data_size);

#define T140_RESIZE_BUFFER(_in_buff_ptr, _in_buff_size, _new_size) \
if((_in_buff_size) < (_new_size)){ \
	if(!((_in_buff_ptr) = tsk_realloc((_in_buff_ptr), (_new_size)))){ \
		TSK_DEBUG_ERROR("Failed to allocate new buffer"); \
		(_in_buff_size) = 0; \
	} \
	(_in_buff_size) = _new_size; \
}

static int tdav_session_t140_send_data(tmedia_session_t* self, enum tmedia_t140_data_type_e data_type, const void* data_ptr, unsigned data_size);
#define tdav_session_t140_send_data_2(self, data_type) tdav_session_t140_send_data((self), (data_type), tsk_null, 0)

static int tdav_session_t140_set(tmedia_session_t* self, const tmedia_param_t* param)
{
	int ret = 0;
	tdav_session_t140_t* t140;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(tdav_session_av_set(TDAV_SESSION_AV(self), param) == tsk_true){
		return 0;
	}

	t140 = (tdav_session_t140_t*)self;

	if(param->plugin_type == tmedia_ppt_consumer){
		TSK_DEBUG_ERROR("Not expected consumer(%s)", param->key);
	}
	else if(param->plugin_type == tmedia_ppt_producer){
		TSK_DEBUG_ERROR("Not expected producer_set(%s)", param->key);
	}
	else{
		TSK_DEBUG_ERROR("Not expected producer_set(%s)", param->key);
	}

	return ret;
}

static int tdav_session_t140_get(tmedia_session_t* self, tmedia_param_t* param)
{
	if(!self || !param){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	
	if(tdav_session_av_get(TDAV_SESSION_AV(self), param) == tsk_true){
		return 0;
	}

	TSK_DEBUG_ERROR("(%s) not expected param key for plugin_type=%d", param->key, param->plugin_type);
	return -2;
}

static int tdav_session_t140_prepare(tmedia_session_t* self)
{
	tdav_session_av_t* base = (tdav_session_av_t*)(self);
	int ret;

	if((ret = tdav_session_av_prepare(base))){
		TSK_DEBUG_ERROR("tdav_session_av_prepare(t140) failed");
		return ret;
	}

	if(base->rtp_manager){
		ret = trtp_manager_set_rtp_callback(base->rtp_manager, _tdav_session_t140_rtp_cb, base);
	}

	return ret;
}

static int tdav_session_t140_start(tmedia_session_t* self)
{
	int ret;
	tdav_session_t140_t* t140;
	const tmedia_codec_t* best_codec;
	tdav_session_av_t* base;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	t140 = (tdav_session_t140_t*)self;
	base = (tdav_session_av_t*)self;
	
	if(!(best_codec = tdav_session_av_get_best_neg_codec(base))){
		TSK_DEBUG_ERROR("No codec matched");
		return -2;
	}
	
	TSK_OBJECT_SAFE_FREE(t140->encoder.codec);
	t140->encoder.codec = tsk_object_ref((tsk_object_t*)best_codec);

	if((ret = tdav_session_av_start(base, best_codec))){
		TSK_DEBUG_ERROR("tdav_session_av_start(t140) failed");
		return ret;
	}

	if((ret = tsk_timer_manager_start(t140->h_timer)) != 0){
		TSK_DEBUG_ERROR("Failed to start the timer");
		return ret;
	}

	if(base->rtp_manager){
		int pt;
		t140->encoder.payload_type = t140->encoder.codec->neg_format ? atoi(t140->encoder.codec->neg_format) : atoi(t140->encoder.codec->format);
		pt = base->red.codec ? 
			( base->red.codec->neg_format ? atoi(base->red.codec->neg_format) : atoi(base->red.codec->format) ) :
			( t140->encoder.payload_type );
		trtp_manager_set_payload_type(base->rtp_manager, pt);
	}

	t140->decoder.last_seq_num = -1;

	t140->started = (ret == 0);

	if(t140->started){
		t140->encoder.timer_idle.timeout = T140_WAIT_FOR_IDLE;
		t140->encoder.timer_buffering.timeout = T140_WAIT_FOR_BUFFERING;
		t140->encoder.timer_idle.id = tsk_timer_manager_schedule(t140->h_timer, t140->encoder.timer_idle.timeout, _tdav_session_t140_timer_cb, t140);
		TSK_DEBUG_INFO("T.140 - Start IDLE timer");
	}

	return ret;
}

static int tdav_session_t140_pause(tmedia_session_t* self)
{
	return 0;
}

static int tdav_session_t140_stop(tmedia_session_t* self)
{
	int ret;
	ret = tsk_timer_manager_stop(TDAV_SESSION_T140(self)->h_timer);
	ret = tdav_session_av_stop(TDAV_SESSION_AV(self));
	TSK_OBJECT_SAFE_FREE(TDAV_SESSION_T140(self)->encoder.codec);
	TSK_OBJECT_SAFE_FREE(TDAV_SESSION_T140(self)->decoder.codec);
	TDAV_SESSION_T140(self)->started = tsk_false;
	return ret;
}

static const tsdp_header_M_t* tdav_session_t140_get_lo(tmedia_session_t* self)
{
	tsk_bool_t updated = tsk_false;
	const tsdp_header_M_t* ret;
	tdav_session_av_t* base = TDAV_SESSION_AV(self);


	if(!(ret = tdav_session_av_get_lo(base, &updated))){
		TSK_DEBUG_ERROR("tdav_session_av_get_lo(t140) failed");
		return tsk_null;
	}

	if(updated){
		tsk_safeobj_lock(base);
		TSK_OBJECT_SAFE_FREE(TDAV_SESSION_T140(self)->encoder.codec);
		tsk_safeobj_unlock(base);
	}
	
	return ret;
}

static int tdav_session_t140_set_ro(tmedia_session_t* self, const tsdp_header_M_t* ro)
{
	int ret;
	tsk_bool_t updated = tsk_false;
	tdav_session_av_t* base = TDAV_SESSION_AV(self);

	if((ret = tdav_session_av_set_ro(base, ro, &updated))){
		TSK_DEBUG_ERROR("tdav_session_av_set_ro(t140) failed");
		return ret;
	}

	if(updated){
		tsk_safeobj_lock(base);
		TSK_OBJECT_SAFE_FREE(TDAV_SESSION_T140(self)->encoder.codec);
		tsk_safeobj_unlock(base);
	}	

	return ret;
}

static int tdav_session_t140_set_ondata_cbfn(tmedia_session_t* self, const void* context, tmedia_session_t140_ondata_cb_f func)
{
	tdav_session_t140_t* t140 = (tdav_session_t140_t*)self;
	tdav_session_av_t* base = (tdav_session_av_t*)self;
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	t140->cb_ondata.context = context;
	t140->cb_ondata.func = func;

	if(base->consumer){
		return tdav_consumer_t140_set_ondata_cbfn(TDAV_CONSUMER_T140(base->consumer), context, func);
	}
	return -2;
}

static int tdav_session_t140_send_data(tmedia_session_t* self, enum tmedia_t140_data_type_e data_type, const void* data_ptr, unsigned data_size)
{
	tdav_session_av_t* base = (tdav_session_av_t*)self;
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(base->producer){
		return tdav_producer_send_data(TDAV_PRODUCER_T140(base->producer), data_type, data_ptr, data_size);
	}
	return -2;
}

// RTP/RTCP callback (From the network to the consumer)
static int _tdav_session_t140_rtp_cb(const void* callback_data, const struct trtp_rtp_packet_s* packet)
{
	tdav_session_t140_t* t140 = (tdav_session_t140_t*)callback_data;
	tdav_session_av_t* base = (tdav_session_av_t*)callback_data;

	if(!t140 || !packet || !packet->header){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(!t140->started || !base->consumer){
		TSK_DEBUG_ERROR("Not ready yet");
		return -1;
	}

	if(packet->header->payload_type == base->red.payload_type){
		//static void* __red_buffer_ptr = tsk_null; // Never used
		//static tsk_size_t __red_buffer_size = 0; // Never used
		if(!base->red.codec){
			TSK_DEBUG_ERROR("No RED codec could be found");
			return -2;
		}
		// Decode RED data
		/* base->red.codec->plugin->decode(
				base->red.codec, 
				(packet->payload.data ? packet->payload.data : packet->payload.data_const), packet->payload.size, 
				&__red_buffer_ptr, &__red_buffer_size, 
				packet->header
			);
		return 0;
		*/
		_tdav_session_t140_recv_red(t140, packet);
	}
	else{
		tsk_size_t out_size;
		// Find the codec to use to decode the RTP payload
		if(!t140->decoder.codec || t140->decoder.payload_type != packet->header->payload_type){
			tsk_istr_t format;
			TSK_OBJECT_SAFE_FREE(t140->decoder.codec);
			tsk_itoa(packet->header->payload_type, &format);
			if(!(t140->decoder.codec = tmedia_codec_find_by_format(TMEDIA_SESSION(t140)->neg_codecs, format)) || !t140->decoder.codec->plugin || !t140->decoder.codec->plugin->decode){
				TSK_DEBUG_ERROR("%s is not a valid payload for this session", format);
				return -2;
			}
			t140->decoder.payload_type = packet->header->payload_type;
		}
		
		// Open codec if not already done
		if(!TMEDIA_CODEC(t140->decoder.codec)->opened){
			int ret;
			tsk_safeobj_lock(base);
			if((ret = tmedia_codec_open(t140->decoder.codec))){
				tsk_safeobj_unlock(base);
				TSK_OBJECT_SAFE_FREE(t140->decoder.codec);
				TSK_DEBUG_ERROR("Failed to open [%s] codec", t140->decoder.codec->plugin->desc);
				return ret;
			}
			tsk_safeobj_unlock(base);
		}
		// Decode data
		out_size = t140->decoder.codec->plugin->decode(t140->decoder.codec, packet->payload.data, packet->payload.size, &t140->decoder.buffer, &t140->decoder.buffer_size, packet->header);
		if(out_size){
			_tdav_session_t140_recv_raw(t140, t140->decoder.buffer, out_size);
		}
	}

	return 0;
}

// Producer callback (From the producer to the network). Will encode() data before sending
static int _tdav_session_t140_producer_enc_cb(const void* callback_data, const void* buffer, tsk_size_t size)
{
	int ret = 0;

	tdav_session_t140_t* t140 = (tdav_session_t140_t*)callback_data;
	tdav_session_av_t* base = (tdav_session_av_t*)callback_data;

	if(!t140){
		TSK_DEBUG_ERROR("Null session");
		return 0;
	}
	
	if(t140->started && base->rtp_manager && base->rtp_manager->is_started && t140->encoder.codec){
		/* encode */
		//tsk_size_t out_size = 0;

		if(!base->rtp_manager->is_started){
			TSK_DEBUG_ERROR("Not started");
			return 0;
		}

		// Open codec if not already done
		if(!t140->encoder.codec->opened){
			tsk_safeobj_lock(base);
			if((ret = tmedia_codec_open(t140->encoder.codec))){
				tsk_safeobj_unlock(base);
				TSK_DEBUG_ERROR("Failed to open [%s] codec", t140->encoder.codec->plugin->desc);
				return -4;
			}
			tsk_safeobj_unlock(base);
		}
		
		ret = _tdav_session_t140_save_outgoing_data(t140, buffer, size);
		
		// start buffering timer if not already done
		if(!TSK_TIMER_ID_IS_VALID(t140->encoder.timer_buffering.id)){
			TSK_DEBUG_INFO("T.140 - Schedule buffering timer");
			t140->encoder.timer_buffering.id = tsk_timer_manager_schedule(t140->h_timer, t140->encoder.timer_buffering.timeout, _tdav_session_t140_timer_cb, t140);
		}
		
		//if((t140->encoder.codec = tsk_object_ref(t140->encoder.codec))){ /* Thread safeness (SIP reINVITE or UPDATE could update the encoder) */
			//out_size = t140->encoder.codec->plugin->encode(t140->encoder.codec, buffer, size, &t140->encoder.buffer, &t140->encoder.buffer_size);
			//if(out_size){
				// trtp_manager_send_rtp(base->rtp_manager, t140->encoder.buffer, out_size, TMEDIA_CODEC_PCM_FRAME_SIZE(t140->encoder.codec), tsk_false/*Marker*/, tsk_true/*lastPacket*/);
			//}
			//tsk_object_unref(t140->encoder.codec);
		//}
	}
	return ret;
}

// save data before sending (when timer fires)
static int _tdav_session_t140_save_outgoing_data(tdav_session_t140_t* self, const void* data_ptr, tsk_size_t data_size)
{
	tdav_session_av_t* base = (tdav_session_av_t*)self;

	if(!self || !data_ptr || !data_size){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	tsk_safeobj_lock(base);

	T140_RESIZE_BUFFER(self->encoder.buffer, self->encoder.buffer_size, (self->encoder.buffer_idx + data_size));
	if(self->encoder.buffer && self->encoder.buffer_size){
		memcpy(&((uint8_t*)self->encoder.buffer)[self->encoder.buffer_idx], data_ptr, data_size);
		self->encoder.buffer_idx += data_size;
	}

	tsk_safeobj_unlock(base);

	return 0;
}

// encapsulated data for sending
static tsk_size_t _tdav_session_t140_encap_red(tdav_session_t140_t* self, void** out_data_ptr, tsk_size_t* out_data_size, tsk_bool_t *last_data)
{
	tdav_session_av_t* base = (tdav_session_av_t*)self;
	t140_block_t* block, *block_curr = tsk_null;
	t140_block_t* red_blocks[T140_BLOCK_MAX_TRANSMIT_COUNT] = { tsk_null };
	tsk_size_t red_blocks_idx, idx, red_blocks_size, red_blocks_hdrs_size;
	tsk_list_item_t* item;
	uint16_t rtp_seq_num;
	uint32_t* pout_data_ptr32;
	uint8_t* pout_data_ptr8;
	static const uint32_t max_transmit_count = T140_BLOCK_MAX_TRANSMIT_COUNT;

	if(!self || !base->rtp_manager || !out_data_ptr || !out_data_size || !last_data){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}

	*last_data = tsk_false;

	tsk_safeobj_lock(base);
	tsk_list_lock(self->encoder.blocks);

	// cancel IDLE timer
	if(TSK_TIMER_ID_IS_VALID(self->encoder.timer_idle.id)){
		tsk_timer_manager_cancel(self->h_timer, self->encoder.timer_idle.id);
		self->encoder.timer_idle.id = TSK_INVALID_TIMER_ID;
		TSK_DEBUG_INFO("T.140 - Cancel IDLE timer");
	}

	rtp_seq_num = base->rtp_manager->rtp.seq_num;

	if(self->encoder.buffer && self->encoder.buffer_idx){
		block_curr = _tdav_session_t140_block_create(self->encoder.payload_type, rtp_seq_num, 0, tsk_false, self->encoder.buffer, self->encoder.buffer_idx);
		self->encoder.buffer_idx = 0;
	}

	// build RED blocks
	red_blocks_idx = 0;
	tsk_list_foreach(item, self->encoder.blocks){
		if(!(block = item->data) || (block->transmit_count + 1) >= T140_BLOCK_MAX_TRANSMIT_COUNT){
			continue;
		}

		if(block->seq_num <= rtp_seq_num && (idx = (rtp_seq_num - block->seq_num)) < T140_BLOCK_MAX_TRANSMIT_COUNT){
			++block->transmit_count; // increment transmission count
			red_blocks[(T140_BLOCK_MAX_TRANSMIT_COUNT - idx - 1)] = block; // backwards counting
			++red_blocks_idx;
		}
		
		if((red_blocks_idx + 1) >= T140_BLOCK_MAX_TRANSMIT_COUNT){
			break;
		}
	}

	// compute RED blocks size
	red_blocks_hdrs_size = ((T140_BLOCK_MAX_TRANSMIT_COUNT - 1) << 2) + 1;
	red_blocks_size = red_blocks_hdrs_size;
	for(idx = 0; idx < T140_BLOCK_MAX_TRANSMIT_COUNT; ++idx){
		if(red_blocks[idx] && red_blocks[idx]->data.ptr && red_blocks[idx]->data.size){
			red_blocks_size += red_blocks[idx]->data.size;
		}
	}
	if(block_curr && block_curr->data.ptr && block_curr->data.size){
		red_blocks_size += block_curr->data.size;
	}

	// resize output buffer
	T140_RESIZE_BUFFER(*out_data_ptr, *out_data_size, red_blocks_size);
	if(!*out_data_ptr) {
		red_blocks_size = 0;
		goto bail;
	}

	// compute RED headers
	pout_data_ptr32 = (uint32_t*)*out_data_ptr;
	for(idx = 0; idx < T140_BLOCK_MAX_TRANSMIT_COUNT; ++idx){
		if((idx + 1) == T140_BLOCK_MAX_TRANSMIT_COUNT){ // last header ?
			*((uint8_t*)&pout_data_ptr32[idx]) = (self->encoder.payload_type & 0x7F); // F=0 | PT
		}
		else{
			pout_data_ptr32[idx] = tnet_htonl(
					( (0x80 | (self->encoder.payload_type & 0x7F)) << 24 ) | // F=1 | PT
					( 0x0000 << 8 ) | // imestamp offset
					( red_blocks[idx] ? (red_blocks[idx]->data.size & 0xFF) : 0x00 ) // block length
				);
		}
	}

	// compute RED data
	pout_data_ptr8 = &((uint8_t*)*out_data_ptr)[red_blocks_hdrs_size];
	for(idx = 0; idx < T140_BLOCK_MAX_TRANSMIT_COUNT; ++idx){
		if(red_blocks[idx] && red_blocks[idx]->data.ptr && red_blocks[idx]->data.size){
			memcpy(pout_data_ptr8, red_blocks[idx]->data.ptr, red_blocks[idx]->data.size);
			pout_data_ptr8 += red_blocks[idx]->data.size;
		}
	}
	if(block_curr && block_curr->data.ptr && block_curr->data.size){
		memcpy(pout_data_ptr8, block_curr->data.ptr, block_curr->data.size);
		pout_data_ptr8 += block_curr->data.size;
	}

	if((*last_data = (red_blocks_hdrs_size == red_blocks_size))){
		// no data available -> clear items, send empty block, schedule idle timer, cancel buffering timer
		if(TSK_TIMER_ID_IS_VALID(self->encoder.timer_buffering.id)){
			tsk_timer_manager_cancel(self->h_timer, self->encoder.timer_buffering.id);
			self->encoder.timer_buffering.id = TSK_INVALID_TIMER_ID;
			TSK_DEBUG_INFO("T.140 - Cancel buffering timer");
		}
		self->encoder.timer_idle.id = tsk_timer_manager_schedule(self->h_timer, self->encoder.timer_idle.timeout, _tdav_session_t140_timer_cb, self);
		TSK_DEBUG_INFO("T.140 - Schedule IDLE timer");
		tsk_list_clear_items(self->encoder.blocks);
	}
	else{
		// clean up all blocks retransmitted more than max times
		while(tsk_list_remove_item_by_pred(self->encoder.blocks, _tdav_session_t140_pred_block_with_transmit_count_more_than_or_equal, &max_transmit_count)) ;
	}

bail:
	if(block_curr){
		_tdav_session_t140_blocks_add(self->encoder.blocks, &self->encoder.blocks_count, &block_curr, INT_MAX);
		TSK_OBJECT_SAFE_FREE(block_curr);
	}
	tsk_list_unlock(self->encoder.blocks);
	tsk_safeobj_unlock(base);

	return red_blocks_size;
}

// decode data from RTP channel
static int _tdav_session_t140_recv_red(tdav_session_t140_t* self, const struct trtp_rtp_packet_s* packet)
{
	const uint8_t* pdata;
	const uint8_t* red_hdr;
	tsk_size_t red_hdrs_count, i, in_size;
	tsk_bool_t last, block_add;
	uint8_t F, pt;
	int32_t pkt_loss_start = -1, pkt_loss_count, seq_num;
	uint16_t timestamp_offset, block_length;

	if(!self || !packet || !packet->header || !(packet->payload.data || packet->payload.data_const) || (packet->payload.size < TDAV_CODEC_RED_MIN_PKT_SIZE)){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	pdata = red_hdr = packet->payload.data ? packet->payload.data : packet->payload.data_const;
	in_size = packet->payload.size;
	
	if((F = (pdata[0] & 0x80)) == 0){
		i = 1;
		red_hdrs_count = 1;
	}
	else{
		for(i = 0, red_hdrs_count = 0; i < in_size; i+= 4, ++red_hdrs_count){
			if((F = (pdata[i] & 0x80)) == 0){ ++i; ++red_hdrs_count;  break; }
		}
	}
	
	if(i == in_size){
		TSK_DEBUG_INFO("T.140 - Empty block");
		return 0;
	}
	
	pdata += i;
	in_size -= i;
	if(self->decoder.last_seq_num != -1 && (self->decoder.last_seq_num + 1) != packet->header->seq_num){
		pkt_loss_start = (self->decoder.last_seq_num + 1);
		pkt_loss_count = (packet->header->seq_num - pkt_loss_start);
		TSK_DEBUG_INFO("T.140 - Packet loss[curr=%d, loss_start=%d, loss_count=%d]", packet->header->seq_num, pkt_loss_start, pkt_loss_count);
	}

	for(i = 0, block_add = tsk_false; i < red_hdrs_count; ++i, block_add = tsk_false){
		last = (i == (red_hdrs_count - 1));
		F = (red_hdr[0] & 0x80);
		pt = (red_hdr[0] & 0x7F);
		if(last || !F){
			/*
			 0 1 2 3 4 5 6 7
             +-+-+-+-+-+-+-+-+
             |0|   Block PT  |
             +-+-+-+-+-+-+-+-+
			 */
			block_length = in_size;
			seq_num = packet->header->seq_num;
			timestamp_offset = 0;
			block_add = tsk_true;
		}
		else{
			/*
				+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
				|1| block PT=7  |  timestamp offset         |   block length    |
				+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
			*/
			seq_num = packet->header->seq_num - (red_hdrs_count - 1 - i); // inferred by counting backwards
			block_add = (pkt_loss_start != -1 && (seq_num <= pkt_loss_start && pkt_loss_start >= seq_num));	
			timestamp_offset = ((red_hdr[1] << 8) | red_hdr[2]) >> 2;
			block_length = ((red_hdr[2] & 0x03) << 8) | red_hdr[3];
			/* block_add &= (block_length > 0); */ // take empty block to avoid scheduling pkt loss timer
			if(pkt_loss_start != -1){
				TSK_DEBUG_INFO("retransmit seq_num=%d, takken=%d", seq_num, block_add);
			}
			if(block_length > in_size){
				TSK_DEBUG_ERROR("Invalid 'block length'");
				break;
			}
			red_hdr += 4;
		}
		
		if(block_add && !_tdav_session_t140_blocks_has_seqnum(self->decoder.blocks, seq_num)){
			t140_block_t* block = _tdav_session_t140_block_create(
				pt, 
				seq_num,
				(packet->header->timestamp + timestamp_offset), 
				tsk_false,
				pdata,
				block_length);
			_tdav_session_t140_blocks_add(self->decoder.blocks, &self->decoder.blocks_count, &block, T140_BLOCKS_MAX_COUNT);
			TSK_OBJECT_SAFE_FREE(block);
		}
			

		pdata += block_length;
		in_size -= block_length;
	}

	self->decoder.last_seq_num = packet->header->seq_num;
	
	return _tdav_session_t140_decode_and_deliver_blocks(self, tsk_false);
}

static int _tdav_session_t140_recv_raw(tdav_session_t140_t* self, const void* data_ptr, tsk_size_t data_size)
{
	tsk_size_t i, j, cmd_extra_len, utf8_start, utf8_end;
	tsk_bool_t is_cmd;
	enum tmedia_t140_data_type_e cmd_type;
	const uint8_t* _data_ptr = (const uint8_t*)data_ptr;

	if(!self || !data_ptr || !data_size){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	for(i = 0, j = 0, is_cmd = tsk_false, cmd_extra_len = 0; i < data_size; ++i, is_cmd = tsk_false, cmd_extra_len = 0){
		switch(_data_ptr[i]){
			case T140_BACKSPACE:
				{
					TSK_DEBUG_INFO("T.140 - BACKSPACE");
					is_cmd = tsk_true;
					cmd_type = tmedia_t140_data_type_backspace;
					break;
				}
			case T140_ESC:
				{
					TSK_DEBUG_INFO("T.140 - ESC");
					is_cmd = tsk_true;
					cmd_type = tmedia_t140_data_type_esc;
					break;
				}
			case T140_LF:
				{
					TSK_DEBUG_INFO("T.140 - LF");
					is_cmd = tsk_true;
					cmd_type = tmedia_t140_data_type_lf;
					break;
				}
			case T140_CR:
				{
					if((i + 1) < data_size && _data_ptr[i + 1] == T140_LF){
						// CR_LF
						TSK_DEBUG_INFO("T.140 - CRLF");
						cmd_extra_len = 1;
						is_cmd = tsk_true;
						cmd_type = tmedia_t140_data_type_cr_lf;
					}
					else{
						// CR
						TSK_DEBUG_INFO("T.140 - CR");
						is_cmd = tsk_true;
						cmd_type = tmedia_t140_data_type_cr;
					}
					break;
				}
			case T140_BELL:
				{
					TSK_DEBUG_INFO("T.140 - BELL");
					is_cmd = tsk_true;
					cmd_type = tmedia_t140_data_type_bell;
					break;
				}
			case T140_SOS:
				{
					TSK_DEBUG_INFO("T.140 - SOS");
					is_cmd = tsk_true;
					cmd_type = tmedia_t140_data_type_sos;
					break;
				}
			case T140_STRING_TERM:
				{
					TSK_DEBUG_INFO("T.140 - ST");
					is_cmd = tsk_true;
					cmd_type = tmedia_t140_data_type_string_term;
					break;
				}
			case (T140_LOSS_CHAR_CHAR >> 8):
				{
					if((i + 1) < data_size && _data_ptr[i + 1] == (T140_LOSS_CHAR_CHAR & 0xFF)){
						TSK_DEBUG_INFO("T.140 - LOSS_CHAR_CHAR");
						cmd_extra_len = 1;
						is_cmd = tsk_true;
						cmd_type = tmedia_t140_data_type_loss_char_char;
					}
					break;
				}
			case T140_GRAPHIC_START:
				{
					TSK_DEBUG_INFO("T.140 - GRAPHIC_START");
					is_cmd = tsk_true;
					cmd_type = tmedia_t140_data_type_graphic_start;
					break;
				}
			case T140_GRAPHIC_END:
				{
					TSK_DEBUG_INFO("T.140 - GRAPHIC_END");
					is_cmd = tsk_true;
					cmd_type = tmedia_t140_data_type_graphic_end;
					break;
				}
			// case (T140_ZERO_WIDTH_NO_BREAK_SPACE >> 16):
			// case (T140_LOSS_UTF8 >> 16):
			case 0xEF:
				{
					if((i + 2) < data_size && _data_ptr[i + 1] == ((T140_ZERO_WIDTH_NO_BREAK_SPACE >> 8) & 0xFF) && _data_ptr[i + 2] == (T140_ZERO_WIDTH_NO_BREAK_SPACE & 0xFF)){
						TSK_DEBUG_INFO("T.140 - ZERO_WIDTH_NO_BREAK_SPACE");
						cmd_extra_len = 2;
						is_cmd = tsk_true;
						cmd_type = tmedia_t140_data_type_zero_width_no_break_space;
					}
					else if((i + 2) < data_size && _data_ptr[i + 1] == ((T140_LOSS_UTF8 >> 8) & 0xFF) && _data_ptr[i + 2] == (T140_LOSS_UTF8 & 0xFF)){
						TSK_DEBUG_INFO("T.140 - LOSS_UTF8");
						cmd_extra_len = 2;
						is_cmd = tsk_true;
						cmd_type = tmedia_t140_data_type_loss_utf8;
					}
					break;
				}
		}//switch
		
		if(is_cmd || ((i + 1) >= data_size)){
			utf8_start = j;
			utf8_end = utf8_start + ( (j < i || (j == i && !is_cmd)) ? (i - j + 1) : 0 );
			if(utf8_start < utf8_end){ // there is utf8 data to deliver
				_tdav_session_t140_consume_data(self, tmedia_t140_data_type_utf8, &_data_ptr[utf8_start], (utf8_end - utf8_start));
			}
			if(is_cmd){ // there is cmd data to deliver
				i += cmd_extra_len;
				_tdav_session_t140_consume_data(self, cmd_type, tsk_null, 0);
			}
			j = i;
		}
	}// for

	return 0;
}

static int _tdav_session_t140_decode_and_deliver_blocks(tdav_session_t140_t* self, tsk_bool_t called_from_timer)
{
	tdav_session_av_t* base = (tdav_session_av_t*)self;
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	tsk_safeobj_lock(base);
	tsk_list_lock(self->decoder.blocks);

	if(!called_from_timer && _tdav_session_t140_blocks_has_gap(self->decoder.blocks)){
		self->decoder.timer_pkt_loss.timeout = (base->red.codec ? T140_WAIT_FOR_MISSING_PKT_RED_ON : T140_WAIT_FOR_MISSING_PKT_RED_OFF);
		self->decoder.timer_pkt_loss.id = tsk_timer_manager_schedule(self->h_timer, self->decoder.timer_pkt_loss.timeout, _tdav_session_t140_timer_cb, self);
	}
	else{
		const tsk_list_item_t* item;
		const t140_block_t* block;

		tsk_list_foreach(item, self->decoder.blocks){
			if(!(block = (const t140_block_t*)item->data) || block->delivered || block->too_late){
				continue;
			}
			if(block->data.ptr && block->data.size){
				_tdav_session_t140_recv_raw(self, block->data.ptr, block->data.size);
			}
			((t140_block_t*)block)->delivered = tsk_true;
		}
	}

	tsk_list_unlock(self->decoder.blocks);
	tsk_safeobj_unlock(base);

	return 0;
}

static int _tdav_session_t140_consume_data(tdav_session_t140_t* self, enum tmedia_t140_data_type_e data_type, const void *data_ptr, tsk_size_t data_size)
{
	tdav_session_av_t* base = (tdav_session_av_t*)self;
	if(!self || !base->consumer){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	T140_RESIZE_BUFFER(self->decoder.consumer_buffer, self->decoder.consumer_buffer_size, (data_size + 4));
	if(self->decoder.consumer_buffer){
		*((int32_t*)self->decoder.consumer_buffer) = data_type;
		if(data_ptr && data_size){
			memcpy(&((uint8_t*)self->decoder.consumer_buffer)[4], data_ptr, data_size);
		}
		tmedia_consumer_consume(base->consumer, self->decoder.consumer_buffer, (data_size + 4), tsk_null);
	}
	return 0;
}

static int _tdav_session_t140_pred_block_with_transmit_count_more_than_or_equal(const tsk_list_item_t *item, const void *transmit_count)
{
	if(item && item->data){
		t140_block_t *block = item->data;
		return (block->transmit_count >= *((uint32_t*)transmit_count)) ? 0 : -1;
	}
	return -1;
}

static int _tdav_session_t140_timer_cb(const void* arg, tsk_timer_id_t timer_id)
{
	tdav_session_t140_t* t140 = (tdav_session_t140_t*)arg;
	tdav_session_av_t* base = (tdav_session_av_t*)arg;
	static const tsk_bool_t called_from_timer = tsk_true;
	int ret = 0;

	tsk_safeobj_lock(base);

	if(timer_id == t140->decoder.timer_pkt_loss.id){
		ret = _tdav_session_t140_decode_and_deliver_blocks(t140, called_from_timer);
	}
	else if(timer_id == t140->encoder.timer_idle.id){
		ret = tdav_session_t140_send_data_2(TMEDIA_SESSION(t140), tmedia_t140_data_type_zero_width_no_break_space);
		t140->encoder.timer_idle.id = tsk_timer_manager_schedule(t140->h_timer, t140->encoder.timer_idle.timeout, _tdav_session_t140_timer_cb, t140);
	}
	else if(timer_id == t140->encoder.timer_buffering.id){
		void* buffer_ptr;
		tsk_size_t buffer_size;
		tsk_bool_t was_idle = TSK_TIMER_ID_IS_VALID(t140->encoder.timer_idle.id);
		tsk_bool_t last_data = tsk_false;
		if(base->red.codec){
			// encapsulated red data and schedule timers if required
			buffer_size = _tdav_session_t140_encap_red(t140, &t140->encoder.red_buffer, &t140->encoder.red_buffer_size, &last_data);
			buffer_ptr = t140->encoder.red_buffer;
		}
		else{
			// send buffered data
			buffer_ptr = t140->encoder.buffer;
			buffer_size = t140->encoder.buffer_idx;
			last_data = tsk_true;
			t140->encoder.buffer_idx = 0;
		}
		if(buffer_ptr && buffer_size){
			trtp_manager_send_rtp(base->rtp_manager, buffer_ptr, buffer_size, 300, was_idle, last_data);
		}
		t140->encoder.timer_buffering.id = last_data
			? TSK_INVALID_TIMER_ID 
			: tsk_timer_manager_schedule(t140->h_timer, t140->encoder.timer_buffering.timeout, _tdav_session_t140_timer_cb, t140);
	}

	tsk_safeobj_unlock(base);

	return ret;
}


//=================================================================================================
//	Session Audio Plugin object definition
//
/* constructor */
static tsk_object_t* tdav_session_t140_ctor(tsk_object_t * self, va_list * app)
{
	tdav_session_t140_t *t140 = self;
	if(t140){
		int ret;
		tdav_session_av_t *base = TDAV_SESSION_AV(self);

		/* init() base */
		if((ret = tdav_session_av_init(base, tmedia_t140)) != 0){
			TSK_DEBUG_ERROR("tdav_session_av_init(t140) failed");
			return tsk_null;
		}

		/* init() self */
		t140->encoder.timer_buffering.id = t140->decoder.timer_pkt_loss.id = TSK_INVALID_TIMER_ID;
		if(base->producer){
			tmedia_producer_set_enc_callback(base->producer, _tdav_session_t140_producer_enc_cb, t140);
		}
		if(base->consumer){
		}
		if(!(t140->encoder.blocks = tsk_list_create())){
			TSK_DEBUG_ERROR("Failed to create encoder blocks list");
			return tsk_null;
		}
		if(!(t140->decoder.blocks = tsk_list_create())){
			TSK_DEBUG_ERROR("Failed to create decoder blocks list");
			return tsk_null;
		}
		if(!(t140->h_timer = tsk_timer_manager_create())){
			TSK_DEBUG_ERROR("Failed to create timer manager");
			return tsk_null;
		}
	}
	return self;
}
/* destructor */
static tsk_object_t* tdav_session_t140_dtor(tsk_object_t * self)
{ 
	tdav_session_t140_t *t140 = self;
	if(t140){
		tdav_session_t140_stop((tmedia_session_t*)t140);
		TSK_OBJECT_SAFE_FREE(t140->h_timer);
		// Do it in this order (deinit self first)
		TSK_OBJECT_SAFE_FREE(t140->encoder.codec);
		TSK_FREE(t140->encoder.buffer);
		TSK_FREE(t140->encoder.red_buffer);
		TSK_OBJECT_SAFE_FREE(t140->encoder.blocks);
		TSK_OBJECT_SAFE_FREE(t140->decoder.codec);
		TSK_FREE(t140->decoder.buffer);
		TSK_FREE(t140->decoder.consumer_buffer);
		TSK_OBJECT_SAFE_FREE(t140->decoder.blocks);

		/* deinit base */
		tdav_session_av_deinit(TDAV_SESSION_AV(self));
	}

	return self;
}
/* object definition */
static const tsk_object_def_t tdav_session_t140_def_s = 
{
	sizeof(tdav_session_t140_t),
	tdav_session_t140_ctor, 
	tdav_session_t140_dtor,
	tmedia_session_cmp, 
};
/* plugin definition*/
static const tmedia_session_plugin_def_t tdav_session_t140_plugin_def_s = 
{
	&tdav_session_t140_def_s,

	tmedia_t140,
	"text",

	tdav_session_t140_set,
	tdav_session_t140_get,
	tdav_session_t140_prepare,
	tdav_session_t140_start,
	tdav_session_t140_pause,
	tdav_session_t140_stop,

	/* Audio part */
	{0},

	tdav_session_t140_get_lo,
	tdav_session_t140_set_ro,

	/* T.140 part */
	{
		tdav_session_t140_set_ondata_cbfn,
		tdav_session_t140_send_data
	}
};
const tmedia_session_plugin_def_t *tdav_session_t140_plugin_def_t = &tdav_session_t140_plugin_def_s;

//=================================================================================================
//	T140Block object definition
//
static tsk_object_t* t140_block_ctor(tsk_object_t * self, va_list * app)
{
	t140_block_t *block = self;
	if(block){		
	}
	return self;
}
static tsk_object_t* t140_block_dtor(tsk_object_t * self)
{ 
	t140_block_t *block = self;
	if(block){
		TSK_FREE(block->data.ptr);
	}

	return self;
}
static int t140_block_cmp(const tsk_object_t *_b1, const tsk_object_t *_b2)
{
	const t140_block_t *b1 = _b1;
	const t140_block_t *b2 = _b2;

	if(b1 && b2){
		return (int)(b1->seq_num - b2->seq_num);
	}
	else if(!b1 && !b2) return 0;
	else return -1;
}
static const tsk_object_def_t t140_block_def_s = 
{
	sizeof(t140_block_t),
	t140_block_ctor, 
	t140_block_dtor,
	t140_block_cmp, 
};
const tsk_object_def_t *t140_block_def_t = &t140_block_def_s;


static tsk_bool_t _tdav_session_t140_blocks_has_gap(tdav_session_t140_blocks_L_t* blocks)
{
	if(blocks && !TSK_LIST_IS_EMPTY(blocks)){
		const tsk_list_item_t* item;
		int32_t last_seq_num = -1;
		tsk_bool_t has_gap = tsk_false;
		tsk_list_lock(blocks);
		tsk_list_foreach(item, blocks){
			if(last_seq_num > 0 && ((last_seq_num + 1) != T140_BLOCK(item->data)->seq_num)){
				has_gap = tsk_true;
			}
			last_seq_num = T140_BLOCK(item->data)->seq_num;
			if(has_gap){
				break;
			}
		}
		tsk_list_unlock(blocks);
		return has_gap;
	}
	return tsk_false;
}

static tsk_bool_t _tdav_session_t140_blocks_has_seqnum(tdav_session_t140_blocks_L_t* blocks, uint16_t seq_num)
{
	if(blocks){
		const tsk_list_item_t* item;
		tsk_bool_t has_seqnum = tsk_false;
		tsk_list_lock(blocks);
		tsk_list_foreach(item, blocks){
			if(seq_num == T140_BLOCK(item->data)->seq_num){
				has_seqnum = tsk_true;
				break;
			}
		}
		tsk_list_unlock(blocks);
		return has_seqnum;
	}
	return tsk_false;
}

static int _tdav_session_t140_blocks_add(tdav_session_t140_blocks_L_t* blocks, int64_t* blocks_count, t140_block_t** block, int64_t blocks_count_max)
{
	if(blocks && blocks_count && block){
		tsk_list_lock(blocks);
		if(tsk_list_push_ascending_data(blocks, (void**)block) == 0){
			if((*blocks_count = (*blocks_count) + 1) > blocks_count_max){
				tsk_list_item_t* first_item = tsk_list_pop_first_item(blocks);
				TSK_OBJECT_SAFE_FREE(first_item);
			}
		}
		tsk_list_unlock(blocks);
	}
	return 0;
}

static t140_block_t* _tdav_session_t140_block_create(uint8_t pt, uint16_t seq_num, uint32_t timestamp, tsk_bool_t delivered, const void* data_ptr, tsk_size_t data_size)
{
	void* _data_ptr = tsk_null;
	t140_block_t* block = tsk_null;

	if(data_ptr && data_size){
		if(!(_data_ptr = tsk_malloc(data_size))){
			TSK_DEBUG_ERROR("Failed to alloc data with size = %u", data_size);
			goto bail;
		}
		memcpy(_data_ptr, data_ptr, data_size);
	}
	if(!(block = tsk_object_new(t140_block_def_t))){
		TSK_DEBUG_ERROR("Failed to create new block object");
		goto bail;
	}
	if(_data_ptr){
		block->data.ptr = _data_ptr, _data_ptr = tsk_null;
		block->data.size = data_size;
	}
	block->pt = pt;
	block->seq_num = seq_num;
	block->timestamp = timestamp;

bail:
	TSK_FREE(_data_ptr);
	return block;
}