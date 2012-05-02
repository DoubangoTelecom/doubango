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

/**@file tdav_codec_h264_rtp.c
 * @brief H.264 payloader/depayloder as per RFC 3984
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#include "tinydav/codecs/h264/tdav_codec_h264_rtp.h"

#include "tinydav/codecs/h264/tdav_codec_h264_common.h"

#include "tinymedia/tmedia_codec.h"

#include "tsk_string.h"
#include "tsk_debug.h"

#include "tsk_memory.h"
#include <string.h> /* strlen() */
#include <stdlib.h> /* strtol() */

/*
*	ITU H.264 - http://www.itu.int/rec/T-REC-H.264-200903-S/en
*/

uint8_t H264_START_CODE_PREFIX[4] = { 0x00, 0x00, 0x00, 0x01 };

#define H264_NAL_UNIT_TYPE_HEADER_SIZE		1
#define H264_F_UNIT_TYPE_HEADER_SIZE		1
#define H264_FUA_HEADER_SIZE				2
#define H264_FUB_HEADER_SIZE				4
#define H264_NAL_AGG_MAX_SIZE			65535

int tdav_codec_h264_get_fua_pay(const uint8_t* in_data, tsk_size_t in_size, const void** out_data, tsk_size_t *out_size, tsk_bool_t* append_scp);
int tdav_codec_h264_get_nalunit_pay(const uint8_t* in_data, tsk_size_t in_size, const void** out_data, tsk_size_t *out_size);

// profile_level_id MUST be a "null-terminated" string
int tdav_codec_h264_parse_profile(const char* profile_level_id, profile_idc_t *p_idc, profile_iop_t *p_iop, level_idc_t *l_idc)
{
	uint32_t value;

	if(tsk_strlen(profile_level_id) != 6){
		TSK_DEBUG_ERROR("I say [%s] is an invalid profile-level-id", profile_level_id);
		return -1;
	}

	value = strtol(profile_level_id, tsk_null, 16);
	
	/* profile-idc */
	if(p_idc){
		switch((value >> 16)){
			case profile_idc_baseline:
				*p_idc = profile_idc_baseline;
				break;
			case profile_idc_extended:
				*p_idc = profile_idc_extended;
				break;
			case profile_idc_main:
				*p_idc = profile_idc_main;
				break;
			case profile_idc_high:
				*p_idc = profile_idc_high;
				break;
			default:
				*p_idc = profile_idc_none;
				break;
		}
	}
	
	/* profile-iop */
	if(p_iop){
		p_iop->constraint_set0_flag = ((value >> 8) & 0x80)>>7;
		p_iop->constraint_set1_flag = ((value >> 8) & 0x40)>>6;
		p_iop->constraint_set2_flag = ((value >> 8) & 0x20)>>5;
		p_iop->reserved_zero_5bits = ((value >> 8) & 0x1F);
	}

	/* level-idc */
	if(l_idc){
		switch((value & 0xFF)){
			case level_idc_1_0:
				*l_idc = level_idc_1_0;
				break;
			case level_idc_1_b:
				*l_idc = level_idc_1_b;
				break;
			case level_idc_1_1:
				*l_idc = level_idc_1_1;
				break;
			case level_idc_1_2:
				*l_idc = level_idc_1_2;
				break;
			case level_idc_1_3:
				*l_idc = level_idc_1_3;
				break;
			case level_idc_2_0:
				*l_idc = level_idc_2_0;
				break;
			case level_idc_2_1:
				*l_idc = level_idc_2_1;
				break;
			case level_idc_2_2:
				*l_idc = level_idc_2_2;
				break;
			case level_idc_3_0:
				*l_idc = level_idc_3_0;
				break;
			case level_idc_3_1:
				*l_idc = level_idc_3_1;
				break;
			case level_idc_3_2:
				*l_idc = level_idc_3_2;
				break;
			case level_idc_4_0:
				*l_idc = level_idc_4_0;
				break;
			case level_idc_4_1:
				*l_idc = level_idc_4_1;
				break;
			case level_idc_4_2:
				*l_idc = level_idc_4_2;
				break;
			case level_idc_5_0:
				*l_idc = level_idc_5_0;
				break;
			case level_idc_5_1:
				*l_idc = level_idc_5_1;
				break;
			case level_idc_5_2:
				*l_idc = level_idc_5_2;
				break;
			default:
				*l_idc = level_idc_none;
				break;
		}
	}

	return 0;
}

int tdav_codec_h264_get_pay(const void* in_data, tsk_size_t in_size, const void** out_data, tsk_size_t *out_size, tsk_bool_t* append_scp)
{
	const uint8_t* pdata = in_data;
	if(!in_data || !in_size || !out_data || !out_size){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	*out_data = tsk_null;
	*out_size = 0;
	*append_scp = tsk_true;

	/* 5.3. NAL Unit Octet Usage
	  +---------------+
      |0|1|2|3|4|5|6|7|
      +-+-+-+-+-+-+-+-+
      |F|NRI|  Type   |
      +---------------+
	*/
	switch((pdata[0] & 0x1F)){
		case undefined_0:
		case undefined_30:
		case undefined_31:
		case stap_a:
		case stap_b:
		case mtap16:
		case mtap24:
		case fu_b:
			break;
		case fu_a:
			return tdav_codec_h264_get_fua_pay(pdata, in_size, out_data, out_size, append_scp);
		default: /* NAL unit (1-23) */
			return tdav_codec_h264_get_nalunit_pay(pdata, in_size, out_data, out_size);
	}

	TSK_DEBUG_WARN("%d not supported as valid NAL Unit type", (*pdata & 0x1F));
	return -1;
}


int tdav_codec_h264_get_fua_pay(const uint8_t* in_data, tsk_size_t in_size, const void** out_data, tsk_size_t *out_size, tsk_bool_t* append_scp)
{
	if(in_size <=2){
		TSK_DEBUG_ERROR("Too short");
		return -1;
	}
	/* RFC 3984 - 5.8. Fragmentation Units (FUs)

		 0                   1                   2                   3
       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      | FU indicator  |   FU header   |                               |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                               |
      |                                                               |
      |                         FU payload                            |
      |                                                               |
      |                               +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                               :...OPTIONAL RTP padding        |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

	  The FU indicator octet has the following format:

      +---------------+
      |0|1|2|3|4|5|6|7|
      +-+-+-+-+-+-+-+-+
      |F|NRI|  Type   |
      +---------------+

	  The FU header has the following format:

      +---------------+
      |0|1|2|3|4|5|6|7|
      +-+-+-+-+-+-+-+-+
      |S|E|R|  Type   |
      +---------------+
	*/

	if((in_data[1] & 0x80) == 0x80 /*S*/){
		/* discard "FU indicator" 
		S: 1 bit
			When set to one, the Start bit indicates the start of a fragmented
			NAL unit.  When the following FU payload is not the start of a
			fragmented NAL unit payload, the Start bit is set to zero.
		*/
		if(in_size> H264_NAL_UNIT_TYPE_HEADER_SIZE){
			uint8_t hdr;
			*out_data = (in_data + H264_NAL_UNIT_TYPE_HEADER_SIZE);
			*out_size = (in_size - H264_NAL_UNIT_TYPE_HEADER_SIZE);

			// F, NRI and Type
			hdr = (in_data[0] & 0xe0) /* F,NRI from "FU indicator"*/ | (in_data[1] & 0x1f) /* type from "FU header" */;
			*((uint8_t*)*out_data) = hdr;
			// Need to append Start Code Prefix
			*append_scp = tsk_true;
		}
		else{
			TSK_DEBUG_ERROR("Too short");
			return -1;
		}
	}
	else{
		/* "FU indicator" and "FU header" */
		if(in_size> H264_FUA_HEADER_SIZE){
			*out_data = (in_data + H264_FUA_HEADER_SIZE);
			*out_size = (in_size - H264_FUA_HEADER_SIZE);
			*append_scp = tsk_false;
		}
		else{
			TSK_DEBUG_ERROR("Too short");
			return -1;
		}
	}

	return 0;
}

int tdav_codec_h264_get_nalunit_pay(const uint8_t* in_data, tsk_size_t in_size, const void** out_data, tsk_size_t *out_size)
{

/*	5.6. Single NAL Unit Packet
	
	 0                   1                   2                   3
       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |F|NRI|  type   |                                               |
      +-+-+-+-+-+-+-+-+                                               |
      |                                                               |
      |               Bytes 2..n of a Single NAL unit                 |
      |                                                               |
      |                               +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                               :...OPTIONAL RTP padding        |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/

	*out_data = in_data;
	*out_size =  in_size;

	return 0;
}

void tdav_codec_h264_rtp_callback(struct tdav_codec_h264_common_s *self, const void *data, tsk_size_t size, tsk_bool_t marker)
{
	uint8_t* pdata = (uint8_t*)data;

	//TSK_DEBUG_INFO("%x %x %x %x -- %u", pdata[0], pdata[1], pdata[2], pdata[3], size);

	if(size>4 && pdata[0] == H264_START_CODE_PREFIX[0] && pdata[1] == H264_START_CODE_PREFIX[1]){
		if(pdata[2] == H264_START_CODE_PREFIX[3]){
			pdata += 3, size -= 3;
		}
		else if(pdata[2] == H264_START_CODE_PREFIX[2] && pdata[3] == H264_START_CODE_PREFIX[3]){
			pdata += 4, size -= 4;
		}
	}
	
	//TSK_DEBUG_INFO("==> SCP %2x %2x %2x %2x", pdata[0], pdata[1], pdata[2], pdata[3]);

	if(size < H264_RTP_PAYLOAD_SIZE){
		/* Can be packet in a Single Nal Unit */
		// Send data over the network
		if(TMEDIA_CODEC_VIDEO(self)->out.callback){
			TMEDIA_CODEC_VIDEO(self)->out.result.buffer.ptr = pdata;
			TMEDIA_CODEC_VIDEO(self)->out.result.buffer.size = size;
			TMEDIA_CODEC_VIDEO(self)->out.result.duration = (3003* (30/TMEDIA_CODEC_VIDEO(self)->out.fps));
			TMEDIA_CODEC_VIDEO(self)->out.result.last_chunck = marker;
			TMEDIA_CODEC_VIDEO(self)->out.callback(&TMEDIA_CODEC_VIDEO(self)->out.result);
		}
	}
	else if(size > H264_NAL_UNIT_TYPE_HEADER_SIZE){
		/* Should be Fragmented as FUA */
		uint8_t fua_hdr[H264_FUA_HEADER_SIZE]; /* "FU indicator" and "FU header" - 2bytes */
		fua_hdr[0] = pdata[0] & 0x60/* F=0 */, fua_hdr[0] |= fu_a;
		fua_hdr[1] = 0x80/* S=1,E=0,R=0 */, fua_hdr[1] |= pdata[0] & 0x1f; /* type */
		// discard header
		pdata += H264_NAL_UNIT_TYPE_HEADER_SIZE;
		size -= H264_NAL_UNIT_TYPE_HEADER_SIZE;

		while(size){
			tsk_size_t packet_size = TSK_MIN(H264_RTP_PAYLOAD_SIZE, size);

			if(self->rtp.size < (packet_size + H264_FUA_HEADER_SIZE)){
				if(!(self->rtp.ptr = tsk_realloc(self->rtp.ptr, (packet_size + H264_FUA_HEADER_SIZE)))){
					TSK_DEBUG_ERROR("Failed to allocate new buffer");
					return;
				}
				self->rtp.size = (packet_size + H264_FUA_HEADER_SIZE);
			}
			// set E bit
			if((size - packet_size) == 0){
				// Last packet
				fua_hdr[1] |= 0x40;
			}
			// copy FUA header
			memcpy(self->rtp.ptr, fua_hdr, H264_FUA_HEADER_SIZE);
			// reset "S" bit
			fua_hdr[1] &= 0x7F;
			// copy data
			memcpy((self->rtp.ptr + H264_FUA_HEADER_SIZE), pdata, packet_size);
			pdata += packet_size;
			size -= packet_size;

			// send data
			if(TMEDIA_CODEC_VIDEO(self)->out.callback){
				TMEDIA_CODEC_VIDEO(self)->out.result.buffer.ptr = self->rtp.ptr;
				TMEDIA_CODEC_VIDEO(self)->out.result.buffer.size = (packet_size + H264_FUA_HEADER_SIZE);
				TMEDIA_CODEC_VIDEO(self)->out.result.duration = (3003* (30/TMEDIA_CODEC_VIDEO(self)->out.fps));
				TMEDIA_CODEC_VIDEO(self)->out.result.last_chunck = (size == 0);
				TMEDIA_CODEC_VIDEO(self)->out.callback(&TMEDIA_CODEC_VIDEO(self)->out.result);
			}
		}
	}
}
