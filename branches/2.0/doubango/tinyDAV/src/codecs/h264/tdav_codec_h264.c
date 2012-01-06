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

/**@file tdav_codec_h264.c
 * @brief H.264 codec plugin using FFmpeg for decoding and x264 for encoding
 * RTP payloader/depayloader follows RFC 3984
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#include "tinydav/codecs/h264/tdav_codec_h264.h"

#if HAVE_FFMPEG && (!defined(HAVE_H264) || HAVE_H264)

#include "tinydav/codecs/h264/tdav_codec_h264_rtp.h"
#include "tinydav/video/tdav_converter_video.h"

#include "tinyrtp/rtp/trtp_rtp_packet.h"

#include "tsk_params.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

static int tdav_codec_h264_init(tdav_codec_h264_t* self, tdav_codec_h264_profile_t profile);
static int tdav_codec_h264_deinit(tdav_codec_h264_t* self);

static void tdav_codec_h264_encap(const tdav_codec_h264_t* h264, const uint8_t* pdata, tsk_size_t size);

/* ============ H.264 Base Profile X.X Plugin interface functions ================= */

#define tdav_codec_h264_fmtp_set tsk_null /* FIXME: should be removed from all plugins (useless) */

static int tdav_codec_h264_open(tmedia_codec_t* self)
{
	int ret;
	int size;


	tdav_codec_h264_t* h264 = (tdav_codec_h264_t*)self;

	if(!h264){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	
	/* the caller (base class) alreasy checked that the codec is not opened */


	//
	//	Encoder
	//
	h264->encoder.context = avcodec_alloc_context();
	avcodec_get_context_defaults(h264->encoder.context);

#if TDAV_UNDER_X86
	h264->encoder.context->dsp_mask = (FF_MM_MMX | FF_MM_MMXEXT | FF_MM_SSE);
#endif

	h264->encoder.context->pix_fmt		= PIX_FMT_YUV420P;
	h264->encoder.context->time_base.num  = 1;
	h264->encoder.context->time_base.den  = TMEDIA_CODEC_VIDEO(h264)->out.fps;
	h264->encoder.context->width = TMEDIA_CODEC_VIDEO(h264)->out.width;
	h264->encoder.context->height = TMEDIA_CODEC_VIDEO(h264)->out.height;

	h264->encoder.context->rc_lookahead = 0;

	//h264->encoder.context->refs = 1;
    h264->encoder.context->scenechange_threshold = 0;
    h264->encoder.context->me_subpel_quality = 0;
    h264->encoder.context->partitions = X264_PART_I4X4 | X264_PART_I8X8 | X264_PART_P8X8 | X264_PART_B8X8;
    h264->encoder.context->me_method = ME_EPZS;
    h264->encoder.context->trellis = 0;

	h264->encoder.context->me_range = 16;
	h264->encoder.context->max_qdiff = 4;
	h264->encoder.context->mb_qmin = h264->encoder.context->qmin = 10;
	h264->encoder.context->mb_qmax = h264->encoder.context->qmax = 51;
	h264->encoder.context->qcompress = 0.6f;
	h264->encoder.context->mb_decision = FF_MB_DECISION_SIMPLE;
	h264->encoder.context->flags2 |= CODEC_FLAG2_FASTPSKIP;
#if TDAV_UNDER_X86
	//h264->encoder.context->flags2 &= ~CODEC_FLAG2_PSY;
	//h264->encoder.context->flags2 |= CODEC_FLAG2_SSIM;
#endif
	h264->encoder.context->flags |= CODEC_FLAG_LOOP_FILTER;
	h264->encoder.context->flags |= CODEC_FLAG_GLOBAL_HEADER;
	h264->encoder.context->max_b_frames = 0;
	h264->encoder.context->b_frame_strategy = 1;
	h264->encoder.context->chromaoffset = 0;

	switch(TDAV_CODEC_H264_COMMON(h264)->profile){
		case tdav_codec_h264_bp10:
		default:
			h264->encoder.context->profile = FF_PROFILE_H264_BASELINE;
			h264->encoder.context->level = 10;
			break;
		case tdav_codec_h264_bp20:
			h264->encoder.context->profile = FF_PROFILE_H264_BASELINE;
			h264->encoder.context->level = 20;
			break;
		case tdav_codec_h264_bp30:
			h264->encoder.context->profile = FF_PROFILE_H264_BASELINE;
			h264->encoder.context->level = 30;
			break;
	}

	h264->encoder.context->crf = 22;
	//h264->encoder.context->cqp = 22;
	h264->encoder.context->thread_count = 0;
	h264->encoder.context->rtp_payload_size = H264_RTP_PAYLOAD_SIZE;
	h264->encoder.context->opaque = tsk_null;
	h264->encoder.context->gop_size = TMEDIA_CODEC_VIDEO(h264)->out.fps * 2; // Each 2 second(s)
		

	// Picture (YUV 420)
	if(!(h264->encoder.picture = avcodec_alloc_frame())){
		TSK_DEBUG_ERROR("Failed to create encoder picture");
		return -2;
	}
	avcodec_get_frame_defaults(h264->encoder.picture);
	

	size = avpicture_get_size(PIX_FMT_YUV420P, h264->encoder.context->width, h264->encoder.context->height);
	if(!(h264->encoder.buffer = tsk_calloc(size, sizeof(uint8_t)))){
		TSK_DEBUG_ERROR("Failed to allocate encoder buffer");
		return -2;
	}

	// Open encoder
	if((ret = avcodec_open(h264->encoder.context, h264->encoder.codec)) < 0){
		TSK_DEBUG_ERROR("Failed to open [%s] codec", TMEDIA_CODEC(h264)->plugin->desc);
		return ret;
	}

	//
	//	Decoder
	//
	h264->decoder.context = avcodec_alloc_context();
	avcodec_get_context_defaults(h264->decoder.context);
	
	h264->decoder.context->pix_fmt = PIX_FMT_YUV420P;
	h264->decoder.context->flags2 |= CODEC_FLAG2_FAST;
	h264->decoder.context->width = TMEDIA_CODEC_VIDEO(h264)->in.width;
	h264->decoder.context->height = TMEDIA_CODEC_VIDEO(h264)->in.height;
	
#if TDAV_UNDER_WINDOWS
	h264->decoder.context->dsp_mask = (FF_MM_MMX | FF_MM_MMXEXT | FF_MM_SSE);
#endif

	// Picture (YUV 420)
	if(!(h264->decoder.picture = avcodec_alloc_frame())){
		TSK_DEBUG_ERROR("Failed to create decoder picture");
		return -2;
	}
	avcodec_get_frame_defaults(h264->decoder.picture);

	// Open decoder
	if((ret = avcodec_open(h264->decoder.context, h264->decoder.codec)) < 0){
		TSK_DEBUG_ERROR("Failed to open [%s] codec", TMEDIA_CODEC(h264)->plugin->desc);
		return ret;
	}

	return 0;
}

static int tdav_codec_h264_close(tmedia_codec_t* self)
{
	tdav_codec_h264_t* h264 = (tdav_codec_h264_t*)self;

	if(!h264){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	/* the caller (base class) alreasy checked that the codec is opened */

	//
	//	Encoder
	//
	if(h264->encoder.context){
		avcodec_close(h264->encoder.context);
		av_free(h264->encoder.context);
		h264->encoder.context = tsk_null;
	}
	if(h264->encoder.picture){
		av_free(h264->encoder.picture);
		h264->encoder.picture = tsk_null;
	}
	if(h264->encoder.buffer){
		TSK_FREE(h264->encoder.buffer);
	}

	//
	//	Decoder
	//
	if(h264->decoder.context){
		avcodec_close(h264->decoder.context);
		av_free(h264->decoder.context);
		h264->decoder.context = tsk_null;
	}
	if(h264->decoder.picture){
		av_free(h264->decoder.picture);
		h264->decoder.picture = tsk_null;
	}
	TSK_FREE(h264->decoder.accumulator);
	h264->decoder.accumulator_pos = 0;

	return 0;
}

static tsk_size_t tdav_codec_h264_encode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size)
{
	int ret = 0;
	int size;

	tdav_codec_h264_t* h264 = (tdav_codec_h264_t*)self;

	if(!self || !in_data || !in_size || !out_data){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}

	if(!self->opened){
		TSK_DEBUG_ERROR("Codec not opened");
		return 0;
	}
	
	// wrap yuv420 buffer
	size = avpicture_fill((AVPicture *)h264->encoder.picture, (uint8_t*)in_data, PIX_FMT_YUV420P, h264->encoder.context->width, h264->encoder.context->height);
	if(size != in_size){
		/* guard */
		TSK_DEBUG_ERROR("Invalid size");
		return 0;
	}

	// send keyframe for:
	//	- the first frame
	//	- every second within the first 4seconds
	//  - every 5 seconds after the first 4seconds
	if(h264->encoder.frame_count++ == 0 
	   ||
	   ( (h264->encoder.frame_count < (int)TMEDIA_CODEC_VIDEO(h264)->out.fps * 4) && ((h264->encoder.frame_count % TMEDIA_CODEC_VIDEO(h264)->out.fps)==0) )
	   ||
	   ( (h264->encoder.frame_count % (TMEDIA_CODEC_VIDEO(h264)->out.fps * 5))==0 )
	   )
	{
		
		// You must patch FFmpeg to switch from X264_TYPE_AUTO to X264_TYPE_IDR or use r26402+
		h264->encoder.picture->pict_type = FF_I_TYPE;
		tdav_codec_h264_encap(h264, h264->encoder.context->extradata, (tsk_size_t)h264->encoder.context->extradata_size);
	}
	else{
		// Encode data
		//h264->encoder.picture->pts = AV_NOPTS_VALUE;
		h264->encoder.picture->pts = h264->encoder.frame_count;
		ret = avcodec_encode_video(h264->encoder.context, h264->encoder.buffer, size, h264->encoder.picture);	
		if(ret >0){
			tdav_codec_h264_encap(h264, h264->encoder.buffer, (tsk_size_t)ret);
		}
		h264->encoder.picture->pict_type = 0;//reset
	}

	return 0;
}

static tsk_size_t tdav_codec_h264_decode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size, const tsk_object_t* proto_hdr)
{
	tdav_codec_h264_t* h264 = (tdav_codec_h264_t*)self;
	const trtp_rtp_header_t* rtp_hdr = proto_hdr;
	
	const uint8_t* pay_ptr = tsk_null;
	tsk_size_t pay_size = 0;
	int ret;
	tsk_bool_t append_scp;
	tsk_size_t retsize = 0, size_to_copy = 0;
	static tsk_size_t xmax_size = (1920 * 1080 * 3) >> 3;
	static tsk_size_t start_code_prefix_size = sizeof(H264_START_CODE_PREFIX);
	int got_picture_ptr;

	if(!h264 || !in_data || !in_size || !out_data || !h264->decoder.context){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}
	
	//TSK_DEBUG_INFO("SeqNo=%hu", rtp_hdr->seq_num);

	/* Packet lost? */
	if(h264->decoder.last_seq != (rtp_hdr->seq_num - 1) && h264->decoder.last_seq){
		if(h264->decoder.last_seq == rtp_hdr->seq_num){
			// Could happen on some stupid emulators
			TSK_DEBUG_INFO("Packet duplicated, seq_num=%d", rtp_hdr->seq_num);
			return 0;
		}
		TSK_DEBUG_INFO("Packet lost, seq_num=%d", rtp_hdr->seq_num);
	}
	h264->decoder.last_seq = rtp_hdr->seq_num;


	/* 5.3. NAL Unit Octet Usage
	  +---------------+
      |0|1|2|3|4|5|6|7|
      +-+-+-+-+-+-+-+-+
      |F|NRI|  Type   |
      +---------------+
	*/
	if(*((uint8_t*)in_data) >> 7){
		TSK_DEBUG_WARN("F=1");
		/* reset accumulator */
		h264->decoder.accumulator = 0;
		return 0;
	}

	/* get payload */
	if((ret = tdav_codec_h264_get_pay(in_data, in_size, (const void**)&pay_ptr, &pay_size, &append_scp)) || !pay_ptr || !pay_size){
		TSK_DEBUG_ERROR("Depayloader failed to get H.264 content");
		return 0;
	}
	//append_scp = tsk_true;
	size_to_copy = pay_size + (append_scp ? start_code_prefix_size : 0);
	
	// start-accumulator
	if(!h264->decoder.accumulator){
		if(size_to_copy > xmax_size){
			TSK_DEBUG_ERROR("%u too big to contain valid encoded data. xmax_size=%u", size_to_copy, xmax_size);
			return 0;
		}
		if(!(h264->decoder.accumulator = tsk_calloc(size_to_copy, sizeof(uint8_t)))){
			TSK_DEBUG_ERROR("Failed to allocated new buffer");
			return 0;
		}
		h264->decoder.accumulator_size = size_to_copy;
	}
	if((h264->decoder.accumulator_pos + size_to_copy) >= xmax_size){
		TSK_DEBUG_ERROR("BufferOverflow");
		h264->decoder.accumulator_pos = 0;
		return 0;
	}
	if((h264->decoder.accumulator_pos + size_to_copy) > h264->decoder.accumulator_size){
		if(!(h264->decoder.accumulator = tsk_realloc(h264->decoder.accumulator, (h264->decoder.accumulator_pos + size_to_copy)))){
			TSK_DEBUG_ERROR("Failed to reallocated new buffer");
			h264->decoder.accumulator_pos = 0;
			h264->decoder.accumulator_size = 0;
			return 0;
		}
		h264->decoder.accumulator_size = (h264->decoder.accumulator_pos + size_to_copy);
	}

	if(append_scp){
		memcpy(&((uint8_t*)h264->decoder.accumulator)[h264->decoder.accumulator_pos], H264_START_CODE_PREFIX, start_code_prefix_size);
		h264->decoder.accumulator_pos += start_code_prefix_size;
	}
	memcpy(&((uint8_t*)h264->decoder.accumulator)[h264->decoder.accumulator_pos], pay_ptr, pay_size);
	h264->decoder.accumulator_pos += pay_size;
	// end-accumulator

	if(rtp_hdr->marker){
		AVPacket packet;
		

		/* decode the picture */
		av_init_packet(&packet);
		packet.size = h264->decoder.accumulator_pos;
		packet.data = h264->decoder.accumulator;
		ret = avcodec_decode_video2(h264->decoder.context, h264->decoder.picture, &got_picture_ptr, &packet);	

		if(ret <0){
			TSK_DEBUG_ERROR("=============Failed to decode the buffer");
		}
		else if(got_picture_ptr){
			tsk_size_t xsize;
			
			/* fill out */
			xsize = avpicture_get_size(h264->decoder.context->pix_fmt, h264->decoder.context->width, h264->decoder.context->height);
			if(*out_max_size<xsize){
				if((*out_data = tsk_realloc(*out_data, (xsize + FF_INPUT_BUFFER_PADDING_SIZE)))){
					*out_max_size = xsize;
				}
				else{
					*out_max_size = 0;
					return 0;
				}
			}
			retsize = xsize;
			TMEDIA_CODEC_VIDEO(h264)->in.width = h264->decoder.context->width;
			TMEDIA_CODEC_VIDEO(h264)->in.height = h264->decoder.context->height;
			avpicture_layout((AVPicture *)h264->decoder.picture, h264->decoder.context->pix_fmt, h264->decoder.context->width, h264->decoder.context->height,
					*out_data, retsize);
		}
		h264->decoder.accumulator_pos = 0;
	}

	return retsize;
}

static tsk_bool_t tdav_codec_h264_fmtp_match(const tmedia_codec_t* codec, const char* fmtp)
{
	tdav_codec_h264_t* h264 = (tdav_codec_h264_t*)codec;
	tsk_params_L_t* params = tsk_null;
	int val_int, sx = -1, sy = -1;
	const char* val_str;
	tsk_bool_t ret = tsk_true;
	tdav_codec_h264_profile_t profile;

	if(!h264){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_false;
	}

	TSK_DEBUG_INFO("Trying to match [%s]", fmtp);

	/* Check whether the profile match (If the profile is missing, then we consider that it's ok) */
	if(((profile = tdav_codec_h264_common_get_profile(fmtp)) != tdav_codec_h264_bp99) && (profile != TDAV_CODEC_H264_COMMON(h264)->profile)){
		TSK_DEBUG_INFO("Profile not matching");
		return tsk_false;
	}

	/* e.g. profile-level-id=42e00a; packetization-mode=1; max-br=452; max-mbps=11880 */
	if((params = tsk_params_fromstring(fmtp, ";", tsk_true))){
		
		/* === max-br ===*/
		if((val_int = tsk_params_get_param_value_as_int(params, "max-br")) != -1){
			// should compare "max-br"?
			TMEDIA_CODEC_VIDEO(h264)->out.max_br = val_int*1000;
		}

		/* === max-mbps ===*/
		if((val_int = tsk_params_get_param_value_as_int(params, "max-mbps")) != -1){
			// should compare "max-mbps"?
			TMEDIA_CODEC_VIDEO(h264)->out.max_mbps = val_int*1000;
		}

		/* === packetization-mode ===*/
		if((val_int = tsk_params_get_param_value_as_int(params, "packetization-mode")) != -1){
			if((packetization_mode_t)val_int == Single_NAL_Unit_Mode || (packetization_mode_t)val_int == Non_Interleaved_Mode){
				TDAV_CODEC_H264_COMMON(h264)->pack_mode = (packetization_mode_t)val_int;
			}
			else{
				TSK_DEBUG_INFO("packetization-mode not matching");
				ret = tsk_false;
				goto bail;
			}
		}

		/* === sx and sy used by doubango clients ===*/
		sx = tsk_params_get_param_value_as_int(params, "sx");
		sy = tsk_params_get_param_value_as_int(params, "sy");

		/* === profile-level-id ===*/
		if((val_str = tsk_params_get_param_value(params, "profile-level-id"))){
			level_idc_t l_idc;
			int std_w, std_h;
			/* profile-idc and level-idc already tested by tdav_codec_h264_get_profile() */
			tdav_codec_h264_parse_profile(val_str, tsk_null, tsk_null, &l_idc);
			switch(l_idc){
				case level_idc_1_0:
				case level_idc_1_b:
				case level_idc_1_1:
					std_w = 176, std_h = 144;
					break;
				default:
					std_w = 352, std_h = 288;
					break;
			}
			
			//set it high to avoid overflow on the accumulator (codec::open)
			// will be update with codec::context after first successful decode
			TMEDIA_CODEC_VIDEO(h264)->in.width = std_w, TMEDIA_CODEC_VIDEO(h264)->in.height = std_h;
			if(sx > 0 && sy > 0){
				//while((sx > std_w && sy > std_h) || (sx > std_h && sy > std_w)){
				//	sx >>= 1;
				//	sy >>= 1;
				//}
				TMEDIA_CODEC_VIDEO(h264)->out.width = sx&(~1), TMEDIA_CODEC_VIDEO(h264)->out.height = sy&(~1);
			}
			else{
				TMEDIA_CODEC_VIDEO(h264)->out.width = std_w, TMEDIA_CODEC_VIDEO(h264)->out.height = std_h;
			}
		}
	}

bail:
	TSK_OBJECT_SAFE_FREE(params);
	return ret;
}

static char* tdav_codec_h264_fmtp_get(const tmedia_codec_t* self)
{
	tdav_codec_h264_t* h264 = (tdav_codec_h264_t*)self;
	char* fmtp = tsk_null;

	if(!h264){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}

	switch(TDAV_CODEC_H264_COMMON(h264)->profile){
		case tdav_codec_h264_bp10:
			fmtp = tsk_strdup("profile-level-id=42e00a");
			break;
		case tdav_codec_h264_bp20:
			fmtp = tsk_strdup("profile-level-id=42e014");
			break;
		case tdav_codec_h264_bp30:
			fmtp = tsk_strdup("profile-level-id=42e01e");
			break;
	}

	if(fmtp){
		tsk_strcat_2(&fmtp, "; packetization-mode=%d; max-br=%d; max-mbps=%d",
			TDAV_CODEC_H264_COMMON(h264)->pack_mode, TMEDIA_CODEC_VIDEO(h264)->in.max_br/1000, TMEDIA_CODEC_VIDEO(h264)->in.max_mbps/1000);
	}

	return fmtp;
}




/* ============ H.264 Base Profile 1.0 Plugin interface ================= */

/* constructor */
static tsk_object_t* tdav_codec_h264_bp10_ctor(tsk_object_t * self, va_list * app)
{
	tdav_codec_h264_t *h264 = self;
	if(h264){
		/* init base: called by tmedia_codec_create() */
		/* init self */
		tdav_codec_h264_init(h264, tdav_codec_h264_bp10);
	}
	return self;
}
/* destructor */
static tsk_object_t* tdav_codec_h264_bp10_dtor(tsk_object_t * self)
{ 
	tdav_codec_h264_t *h264 = self;
	if(h264){
		/* deinit base */
		tdav_codec_h264_common_deinit(self);
		/* deinit self */
		tdav_codec_h264_deinit(h264);
		
	}

	return self;
}
/* object definition */
static const tsk_object_def_t tdav_codec_h264_bp10_def_s = 
{
	sizeof(tdav_codec_h264_t),
	tdav_codec_h264_bp10_ctor, 
	tdav_codec_h264_bp10_dtor,
	tmedia_codec_cmp, 
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t tdav_codec_h264_bp10_plugin_def_s = 
{
	&tdav_codec_h264_bp10_def_s,

	tmedia_video,
	"H264",
	"H264 Base Profile 1.0",
	TMEDIA_CODEC_FORMAT_H264_BP10,
	tsk_true,
	90000, // rate
	
	/* audio */
	{ 0 },

	/* video */
	{176, 144, 15},

	tdav_codec_h264_open,
	tdav_codec_h264_close,
	tdav_codec_h264_encode,
	tdav_codec_h264_decode,
	tdav_codec_h264_fmtp_match,
	tdav_codec_h264_fmtp_get,
	tdav_codec_h264_fmtp_set
};
const tmedia_codec_plugin_def_t *tdav_codec_h264_bp10_plugin_def_t = &tdav_codec_h264_bp10_plugin_def_s;


/* ============ H.264 Base Profile 2.0 Plugin interface ================= */

/* constructor */
static tsk_object_t* tdav_codec_h264_bp20_ctor(tsk_object_t * self, va_list * app)
{
	tdav_codec_h264_t *h264 = self;
	if(h264){
		/* init base: called by tmedia_codec_create() */
		/* init self */
		tdav_codec_h264_init(h264, tdav_codec_h264_bp20);
	}
	return self;
}
/* destructor */
static tsk_object_t* tdav_codec_h264_bp20_dtor(tsk_object_t * self)
{ 
	tdav_codec_h264_t *h264 = self;
	if(h264){
		/* deinit base */
		tdav_codec_h264_common_deinit(self);
		/* deinit self */
		tdav_codec_h264_deinit(h264);
		
	}

	return self;
}
/* object definition */
static const tsk_object_def_t tdav_codec_h264_bp20_def_s = 
{
	sizeof(tdav_codec_h264_t),
	tdav_codec_h264_bp20_ctor, 
	tdav_codec_h264_bp20_dtor,
	tmedia_codec_cmp, 
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t tdav_codec_h264_bp20_plugin_def_s = 
{
	&tdav_codec_h264_bp20_def_s,

	tmedia_video,
	"H264",
	"H264 Base Profile 2.0",
	TMEDIA_CODEC_FORMAT_H264_BP20,
	tsk_true,
	90000, // rate
	
	/* audio */
	{ 0 },

	/* video */
	{352, 288, 15},

	tdav_codec_h264_open,
	tdav_codec_h264_close,
	tdav_codec_h264_encode,
	tdav_codec_h264_decode,
	tdav_codec_h264_fmtp_match,
	tdav_codec_h264_fmtp_get,
	tdav_codec_h264_fmtp_set
};
const tmedia_codec_plugin_def_t *tdav_codec_h264_bp20_plugin_def_t = &tdav_codec_h264_bp20_plugin_def_s;


/* ============ H.264 Base Profile 3.0 Plugin interface ================= */

/* constructor */
static tsk_object_t* tdav_codec_h264_bp30_ctor(tsk_object_t * self, va_list * app)
{
	tdav_codec_h264_t *h264 = self;
	if(h264){
		/* init base: called by tmedia_codec_create() */
		/* init self */
		tdav_codec_h264_init(h264, tdav_codec_h264_bp30);
	}
	return self;
}
/* destructor */
static tsk_object_t* tdav_codec_h264_bp30_dtor(tsk_object_t * self)
{ 
	tdav_codec_h264_t *h264 = self;
	if(h264){
		/* deinit base */
		tdav_codec_h264_common_deinit(self);
		/* deinit self */
		tdav_codec_h264_deinit(h264);
		
	}

	return self;
}
/* object definition */
static const tsk_object_def_t tdav_codec_h264_bp30_def_s = 
{
	sizeof(tdav_codec_h264_t),
	tdav_codec_h264_bp30_ctor, 
	tdav_codec_h264_bp30_dtor,
	tmedia_codec_cmp, 
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t tdav_codec_h264_bp30_plugin_def_s = 
{
	&tdav_codec_h264_bp30_def_s,

	tmedia_video,
	"H264",
	"H264 Base Profile 3.0",
	TMEDIA_CODEC_FORMAT_H264_BP30,
	tsk_true,
	90000, // rate
	
	/* audio */
	{ 0 },

	/* video */
	{352, 288, 15},

	tdav_codec_h264_open,
	tdav_codec_h264_close,
	tdav_codec_h264_encode,
	tdav_codec_h264_decode,
	tdav_codec_h264_fmtp_match,
	tdav_codec_h264_fmtp_get,
	tdav_codec_h264_fmtp_set
};
const tmedia_codec_plugin_def_t *tdav_codec_h264_bp30_plugin_def_t = &tdav_codec_h264_bp30_plugin_def_s;




































/* ============ Common To all H264 codecs ================= */

int tdav_codec_h264_init(tdav_codec_h264_t* self, tdav_codec_h264_profile_t profile)
{
	int ret = 0;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if((ret = tdav_codec_h264_common_init(TDAV_CODEC_H264_COMMON(self)))){
		TSK_DEBUG_ERROR("tdav_codec_h264_common_init() faile with error code=%d", ret);
		return ret;
	}
	
	TDAV_CODEC_H264_COMMON(self)->pack_mode = H264_PACKETIZATION_MODE;
	TDAV_CODEC_H264_COMMON(self)->profile = profile;
	TMEDIA_CODEC_VIDEO(self)->in.max_mbps = TMEDIA_CODEC_VIDEO(self)->out.max_mbps = H264_MAX_MBPS*1000;
	TMEDIA_CODEC_VIDEO(self)->in.max_br = TMEDIA_CODEC_VIDEO(self)->out.max_br = H264_MAX_BR*1000;

	// At this time self->plugin is Null
	TMEDIA_CODEC_VIDEO(self)->in.width = TMEDIA_CODEC_VIDEO(self)->out.width = 176;
	TMEDIA_CODEC_VIDEO(self)->in.height = TMEDIA_CODEC_VIDEO(self)->out.height = 144;
	TMEDIA_CODEC_VIDEO(self)->in.fps = TMEDIA_CODEC_VIDEO(self)->out.fps = 15;

	if(!(self->encoder.codec = avcodec_find_encoder(CODEC_ID_H264))){
		TSK_DEBUG_ERROR("Failed to find H.264 encoder");
		ret = -2;
	}

	if(!(self->decoder.codec = avcodec_find_decoder(CODEC_ID_H264))){
		TSK_DEBUG_ERROR("Failed to find H.264 decoder");
		ret = -3;
	}

	/* allocations MUST be done by open() */
	return ret;
}

int tdav_codec_h264_deinit(tdav_codec_h264_t* self)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	self->encoder.codec = tsk_null;
	self->decoder.codec = tsk_null;

	// FFMpeg resources are destroyed by close()

	return 0;
}

static void tdav_codec_h264_encap(const tdav_codec_h264_t* h264, const uint8_t* pdata, tsk_size_t size)
{
	register int32_t i;
	int32_t last_scp, prev_scp;
	static int32_t size_of_scp = sizeof(H264_START_CODE_PREFIX); /* we know it's equal to 4 ..but */

	if(!pdata || !size){
		return;
	}

	last_scp = 0, prev_scp = 0;
	for(i = size_of_scp; i<(int32_t)(size - size_of_scp); i++){
		if(pdata[i] == H264_START_CODE_PREFIX[0] && pdata[i+1] == H264_START_CODE_PREFIX[1] && pdata[i+2] == H264_START_CODE_PREFIX[2] && pdata[i+3] == H264_START_CODE_PREFIX[3]){  /* Found Start Code Prefix */
			prev_scp = last_scp;
			if((i - last_scp) >= H264_RTP_PAYLOAD_SIZE || 1){
				tdav_codec_h264_rtp_callback(TDAV_CODEC_H264_COMMON(h264), pdata + prev_scp,
					(i - prev_scp), (prev_scp == size));
			}
			last_scp = i;
		}
	}

	if(last_scp < (int32_t)size){
			tdav_codec_h264_rtp_callback(TDAV_CODEC_H264_COMMON(h264), pdata + last_scp,
				(size - last_scp), tsk_true);
	}
}

#endif /* HAVE_FFMPEG */
