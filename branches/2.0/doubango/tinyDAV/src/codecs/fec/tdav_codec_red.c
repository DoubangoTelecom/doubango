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

/**@file tdav_codec_red.c
 * @brief RTP Payload for Redundant Audio Data as per RFC 2198
 */
#include "tinydav/codecs/fec/tdav_codec_red.h"

#include "tinyrtp/rtp/trtp_rtp_packet.h"

#include "tsk_memory.h"
#include "tsk_time.h"
#include "tsk_debug.h"

typedef struct tdav_codec_red_s
{
	TMEDIA_DECLARE_CODEC_VIDEO;

	tdav_codec_red_rtppacket_cb_f callback;
	const void* callback_data;
}
tdav_codec_red_t;

int tdav_codec_red_set_callback(tdav_codec_red_t *self, tdav_codec_red_rtppacket_cb_f callback, const void* callback_data)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	self->callback = callback;
	self->callback_data = callback_data;

	return 0;
}

static int tdav_codec_red_open(tmedia_codec_t* self)
{
	return 0;
}

static int tdav_codec_red_close(tmedia_codec_t* self)
{
	return 0;
}

static tsk_size_t tdav_codec_red_encode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size)
{
	tdav_codec_red_t *red = (tdav_codec_red_t *)self;
	tsk_size_t xsize = (in_size + 1);
	static const uint8_t __first_octet = 0x00; // F=1, PT=0. Up to the caller to update this first octet with the right PT.

	if(!red || !in_data || !in_size || !out_data || !out_max_size){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}

	if(*out_max_size < xsize){
		if(!(*out_data = tsk_realloc(*out_data, xsize))){
			TSK_DEBUG_ERROR("Failed to realloc data");
			*out_max_size = 0;
		}
		*out_max_size = xsize;
	}

	((uint8_t*)*out_data)[0] = __first_octet;
	memcpy(&((uint8_t*)*out_data)[1], in_data, in_size);

	return xsize;
}

static tsk_size_t tdav_codec_red_decode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size, const tsk_object_t* proto_hdr)
{
	tdav_codec_red_t* red = (tdav_codec_red_t*)self;
	const trtp_rtp_header_t* rtp_hdr = proto_hdr;
	trtp_rtp_packet_t* red_rtp_pkt = tsk_null;
	const uint8_t* pdata = in_data;
	const uint8_t* red_hdr = in_data;
	tsk_size_t red_hdrs_count, i;
	tsk_bool_t last;
	uint8_t F;
	uint16_t timestamp_offset, block_length;

	if(!red || !in_data || (in_size < TDAV_CODEC_RED_MIN_PKT_SIZE)|| !out_data){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}

	if(!red->callback){
		TSK_DEBUG_WARN("Not callback installed for RED data");
		return 0;
	}

	if((F = (pdata[0] & 0x80)) == 0){
		i = 1;
		red_hdrs_count = 1;
	}
	else{
		for(i = 0, red_hdrs_count = 0; i < in_size; i+= 4, ++red_hdrs_count){
			if((F = (pdata[i] & 0x80)) == 0){ ++i; ++red_hdrs_count;  break; }
		}
	}

	if(i >= in_size){
		TSK_DEBUG_ERROR("Invalid data");
		return 0;
	}
	
	pdata += i;
	in_size -= i;

	for(i = 0; i < red_hdrs_count && in_size > 0; ++i){
		TSK_OBJECT_SAFE_FREE(red_rtp_pkt);
		if(!(red_rtp_pkt = trtp_rtp_packet_create_null())){
			TSK_DEBUG_ERROR("Failed to create RTP packet");
			continue;
		}
		if(!(red_rtp_pkt->header = trtp_rtp_header_create(rtp_hdr->ssrc, rtp_hdr->seq_num, rtp_hdr->timestamp, rtp_hdr->payload_type, rtp_hdr->marker))){
			TSK_DEBUG_ERROR("Failed to create RTP header");
			continue;
		}

		// Must create an RTP packet for each RED chunck as they will be saved in the JB
		last = (i == (red_hdrs_count - 1));
		F = (red_hdr[0] & 0x80);
		red_rtp_pkt->header->payload_type = (red_hdr[0] & 0x7F);
		
		if(last || !F){
			/*
			 0 1 2 3 4 5 6 7
             +-+-+-+-+-+-+-+-+
             |0|   Block PT  |
             +-+-+-+-+-+-+-+-+
			 */
			block_length = in_size;
		}
		else{
			/*
			+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		    |1| block PT=7  |  timestamp offset         |   block length    |
		    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
			*/
			timestamp_offset = ((red_hdr[1] << 8) | red_hdr[2]) >> 2;
			block_length = ((red_hdr[2] & 0x03) << 8) | red_hdr[3];
			if(block_length > in_size){
				TSK_DEBUG_ERROR("Invalid 'block length'");
				break;
			}
			red_rtp_pkt->header->timestamp += timestamp_offset;
			red_hdr += 4;
		}

		// decode
		if(red->callback){
			// do not use "data_const" as payload will be saved in the jitter buffer and decoded later (async)
			if((red_rtp_pkt->payload.data = tsk_malloc(block_length))){
				memcpy(red_rtp_pkt->payload.data, pdata, block_length);
				red_rtp_pkt->payload.size = block_length;
				red->callback(red->callback_data, red_rtp_pkt);
			}
		}

		pdata += block_length;
		in_size -= block_length;
	}
	
	TSK_OBJECT_SAFE_FREE(red_rtp_pkt);

	return 0; // must be always zero
}

static tsk_bool_t tdav_codec_red_sdp_att_match(const tmedia_codec_t* codec, const char* att_name, const char* att_value)
{
	return tsk_true;
}

static char* tdav_codec_red_sdp_att_get(const tmedia_codec_t* self, const char* att_name)
{
	return tsk_null;
}


/* ============ red object definition ================= */

/* constructor */
static tsk_object_t* tdav_codec_red_ctor(tsk_object_t * self, va_list * app)
{
	tdav_codec_red_t *red = self;
	if(red){
		/* init base: called by tmedia_codec_create() */
		/* init self */
		
	}
	return self;
}
/* destructor */
static tsk_object_t* tdav_codec_red_dtor(tsk_object_t * self)
{ 
	tdav_codec_red_t *red = self;
	if(red){
		/* deinit base */
		tmedia_codec_video_deinit(red);
		/* deinit self */
		
	}

	return self;
}
/* object definition */
static const tsk_object_def_t tdav_codec_red_def_s = 
{
	sizeof(tdav_codec_red_t),
	tdav_codec_red_ctor, 
	tdav_codec_red_dtor,
	tmedia_codec_cmp, 
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t tdav_codec_red_plugin_def_s = 
{
	&tdav_codec_red_def_s,

	(/* tmedia_video | tmedia_audio | */tmedia_t140), // FIXME: for now is only supported with T.140
	tmedia_codec_id_red,
	"red",
	"red codec",
	TMEDIA_CODEC_FORMAT_RED,
	tsk_true,
	1000, // rate: FIXME: for now it's only for T.140
	
	/* audio */
	{ 0 },

	/* video (defaul width,height,fps) */
	{176, 144, 15},

	tsk_null, // set()
	tdav_codec_red_open,
	tdav_codec_red_close,
	tdav_codec_red_encode,
	tdav_codec_red_decode,
	tdav_codec_red_sdp_att_match,
	tdav_codec_red_sdp_att_get
};
const tmedia_codec_plugin_def_t *tdav_codec_red_plugin_def_t = &tdav_codec_red_plugin_def_s;
