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

#include "tinymedia/tmedia_params.h"

#include "tsk_params.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#include <libavcodec/avcodec.h>

typedef struct tdav_codec_h264_s
{
	TDAV_DECLARE_CODEC_H264_COMMON;

	// Encoder
	struct{
		AVCodec* codec;
		AVCodecContext* context;
		AVFrame* picture;
		void* buffer;
		int64_t frame_count;
		tsk_bool_t force_idr;
		int32_t quality; // [1-31]
		int rotation;
	} encoder;
	
	// decoder
	struct{
		AVCodec* codec;
		AVCodecContext* context;
		AVFrame* picture;

		void* accumulator;
		tsk_size_t accumulator_pos;
		tsk_size_t accumulator_size;
		uint16_t last_seq;
	} decoder;
}
tdav_codec_h264_t;

#define TDAV_H264_GOP_SIZE_IN_SECONDS		25

static int tdav_codec_h264_init(tdav_codec_h264_t* self, profile_idc_t profile);
static int tdav_codec_h264_deinit(tdav_codec_h264_t* self);
static int tdav_codec_h264_open_encoder(tdav_codec_h264_t* self);
static int tdav_codec_h264_close_encoder(tdav_codec_h264_t* self);
static int tdav_codec_h264_open_decoder(tdav_codec_h264_t* self);
static int tdav_codec_h264_close_decoder(tdav_codec_h264_t* self);

static void tdav_codec_h264_encap(const tdav_codec_h264_t* h264, const uint8_t* pdata, tsk_size_t size);

/* ============ H.264 Base/Main Profile X.X Plugin interface functions ================= */

static int tdav_codec_h264_set(tmedia_codec_t* self, const tmedia_param_t* param)
{
	tdav_codec_h264_t* h264 = (tdav_codec_h264_t*)self;
	if(!self->opened){
		TSK_DEBUG_ERROR("Codec not opened");
		return -1;
	}
	if(param->value_type == tmedia_pvt_int32){
		if(tsk_striequals(param->key, "action")){
			tmedia_codec_action_t action = (tmedia_codec_action_t)TSK_TO_INT32((uint8_t*)param->value);
			switch(action){
				case tmedia_codec_action_encode_idr:
					{
						h264->encoder.force_idr = tsk_true;
						break;
					}
				case tmedia_codec_action_bw_down:
					{
						h264->encoder.quality = TSK_CLAMP(1, (h264->encoder.quality + 1), 31);
						h264->encoder.context->global_quality = FF_QP2LAMBDA * h264->encoder.quality;
						break;
					}
				case tmedia_codec_action_bw_up:
					{
						h264->encoder.quality = TSK_CLAMP(1, (h264->encoder.quality - 1), 31);
						h264->encoder.context->global_quality = FF_QP2LAMBDA * h264->encoder.quality;
						break;
					}
			}
			return 0;
		}
		else if(tsk_striequals(param->key, "rotation")){
			int rotation = *((int32_t*)param->value);
			if(h264->encoder.rotation != rotation){
				if(self->opened){
					int ret;
					h264->encoder.rotation = rotation;
					if((ret = tdav_codec_h264_close_encoder(h264))){
						return ret;
					}
					if((ret = tdav_codec_h264_open_encoder(h264))){
						return ret;
					}
#if 0 // Not working
					if((ret = avcodec_close(h264->encoder.context))){
						TSK_DEBUG_ERROR("Failed to close [%s] codec", TMEDIA_CODEC(h264)->plugin->desc);
						return ret;
					}
					h264->encoder.context->width = (rotation == 90 || rotation == 270) ? TMEDIA_CODEC_VIDEO(h264)->out.height : TMEDIA_CODEC_VIDEO(h264)->out.width;
					h264->encoder.context->height = (rotation == 90 || rotation == 270) ? TMEDIA_CODEC_VIDEO(h264)->out.width : TMEDIA_CODEC_VIDEO(h264)->out.height;
					if((ret = avcodec_open(h264->encoder.context, h264->encoder.codec)) < 0){
						TSK_DEBUG_ERROR("Failed to open [%s] codec", TMEDIA_CODEC(h264)->plugin->desc);
						return ret;
					}
					h264->encoder.force_idr = tsk_true;
#endif
				}
			}
			return 0;
		}
	}
	return -1;
}


static int tdav_codec_h264_open(tmedia_codec_t* self)
{
	int ret;
	tdav_codec_h264_t* h264 = (tdav_codec_h264_t*)self;

	if(!h264){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	
	/* the caller (base class) already checked that the codec is not opened */

	//	Encoder
	if((ret = tdav_codec_h264_open_encoder(h264))){
		return ret;
	}

	//	Decoder
	if((ret = tdav_codec_h264_open_decoder(h264))){
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

	//	Encoder
	tdav_codec_h264_close_encoder(h264);

	//	Decoder
	tdav_codec_h264_close_decoder(h264);

	return 0;
}

static tsk_size_t tdav_codec_h264_encode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size)
{
	int ret = 0;
	int size;
	tsk_bool_t send_idr, send_hdr;

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

	// send IDR for:
	//	- the first frame
	//  - remote peer requested an IDR
	//	- every second within the first 4seconds
	send_idr = (
		h264->encoder.frame_count++ == 0
		|| h264 ->encoder.force_idr
		|| ( (h264->encoder.frame_count < (int)TMEDIA_CODEC_VIDEO(h264)->out.fps * 4) && ((h264->encoder.frame_count % TMEDIA_CODEC_VIDEO(h264)->out.fps)==0) )
	   );

	// send SPS and PPS headers for:
	//  - IDR frames (not required but it's the easiest way to deal with pakt loss)
	//  - every 5 seconds after the first 4seconds
	send_hdr = (
		send_idr
		|| ( (h264->encoder.frame_count % (TMEDIA_CODEC_VIDEO(h264)->out.fps * 5))==0 )
		);
	if(send_hdr){
		tdav_codec_h264_encap(h264, h264->encoder.context->extradata, (tsk_size_t)h264->encoder.context->extradata_size);
	}
	
	// Encode data
#if LIBAVCODEC_VERSION_MAJOR <= 53
	h264->encoder.picture->pict_type = send_idr ? FF_I_TYPE : 0;
#else
    h264->encoder.picture->pict_type = send_idr ? AV_PICTURE_TYPE_I : AV_PICTURE_TYPE_NONE;
#endif
	h264->encoder.picture->pts = h264->encoder.frame_count;
	h264->encoder.picture->quality = h264->encoder.context->global_quality;
	// h264->encoder.picture->pts = h264->encoder.frame_count; MUST NOT
	ret = avcodec_encode_video(h264->encoder.context, h264->encoder.buffer, size, h264->encoder.picture);	
	if(ret > 0){
		tdav_codec_h264_encap(h264, h264->encoder.buffer, (tsk_size_t)ret);
	}
	h264 ->encoder.force_idr = tsk_false;

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
	if((h264->decoder.last_seq + 1) != rtp_hdr->seq_num && h264->decoder.last_seq){
		TSK_DEBUG_INFO("Packet lost, seq_num=%d", (h264->decoder.last_seq + 1));
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
			TSK_DEBUG_INFO("Failed to decode the buffer with error code =%d", ret);
			if(TMEDIA_CODEC_VIDEO(self)->in.callback){
				TMEDIA_CODEC_VIDEO(self)->in.result.type = tmedia_video_decode_result_type_error;
				TMEDIA_CODEC_VIDEO(self)->in.result.proto_hdr = proto_hdr;
				TMEDIA_CODEC_VIDEO(self)->in.callback(&TMEDIA_CODEC_VIDEO(self)->in.result);
			}
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

static tsk_bool_t tdav_codec_h264_sdp_att_match(const tmedia_codec_t* codec, const char* att_name, const char* att_value)
{
	tdav_codec_h264_t* h264 = (tdav_codec_h264_t*)codec;
	tsk_bool_t ret = tsk_true;

	if(!h264){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_false;
	}

	TSK_DEBUG_INFO("Trying to match [%s:%s]", att_name, att_value);

	if(tsk_striequals(att_name, "fmtp")){
		int val_int;
		profile_idc_t profile;
		level_idc_t level;
		tsk_params_L_t* params;

		/* Check whether the profile match (If the profile is missing, then we consider that it's ok) */
		if(tdav_codec_h264_get_profile_and_level(att_value, &profile, &level) != 0){
			TSK_DEBUG_ERROR("Not valid profile-level: %s", att_value);
			return tsk_false;
		}
		if(TDAV_CODEC_H264_COMMON(codec)->profile != profile){
			return tsk_false;
		}
		else{
			if(TDAV_CODEC_H264_COMMON(codec)->level != level){
				unsigned width, height;
				TDAV_CODEC_H264_COMMON(codec)->level = TSK_MIN(TDAV_CODEC_H264_COMMON(codec)->level, level);
				if(tdav_codec_h264_common_size_from_level(TDAV_CODEC_H264_COMMON(codec)->level, &width, &height) != 0){
					return tsk_false;
				}
				TMEDIA_CODEC_VIDEO(codec)->in.width = TMEDIA_CODEC_VIDEO(codec)->out.width = width;
				TMEDIA_CODEC_VIDEO(codec)->in.height = TMEDIA_CODEC_VIDEO(codec)->out.height = height;
			}
		}

		/* e.g. profile-level-id=42e00a; packetization-mode=1; max-br=452; max-mbps=11880 */
		if((params = tsk_params_fromstring(att_value, ";", tsk_true))){
			
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
		}
bail:
		TSK_OBJECT_SAFE_FREE(params);
	}
	else if(tsk_striequals(att_name, "imageattr")){
		unsigned in_width, in_height, out_width, out_height;
		unsigned width, height;
		tsk_size_t s;
		if(tmedia_parse_video_imageattr(att_value, TMEDIA_CODEC_VIDEO(codec)->pref_size, &in_width, &in_height, &out_width, &out_height) != 0){
			return tsk_false;
		}
		// check that 'imageattr' is comform to H.264 'profile-level'
		if(tdav_codec_h264_common_size_from_level(TDAV_CODEC_H264_COMMON(codec)->level, &width, &height) != 0){
			return tsk_false;
		}
		if((s = ((width * height * 3) >> 1)) < ((in_width * in_height * 3) >> 1) || s < ((out_width * out_height * 3) >> 1)){
			return tsk_false;
		}

		TMEDIA_CODEC_VIDEO(codec)->in.width = in_width;
		TMEDIA_CODEC_VIDEO(codec)->in.height = in_height;
		TMEDIA_CODEC_VIDEO(codec)->out.width = out_width;
		TMEDIA_CODEC_VIDEO(codec)->out.height = out_height;
	}

	return ret;
}

static char* tdav_codec_h264_sdp_att_get(const tmedia_codec_t* self, const char* att_name)
{
	tdav_codec_h264_t* h264 = (tdav_codec_h264_t*)self;
	
	if(!h264 || !att_name){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}

	if(tsk_striequals(att_name, "fmtp")){
		char* fmtp = tsk_null;
#if 1
		tsk_sprintf(&fmtp, "profile-level-id=%x; packetization-mode=%d", ((TDAV_CODEC_H264_COMMON(h264)->profile << 16) | TDAV_CODEC_H264_COMMON(h264)->level), TDAV_CODEC_H264_COMMON(h264)->pack_mode);
#else
		tsk_strcat_2(&fmtp, "profile-level-id=%s; packetization-mode=%d; max-br=%d; max-mbps=%d",
			profile_level, TDAV_CODEC_H264_COMMON(h264)->pack_mode, TMEDIA_CODEC_VIDEO(h264)->in.max_br/1000, TMEDIA_CODEC_VIDEO(h264)->in.max_mbps/1000);
#endif
		return fmtp;
	}
	else if(tsk_striequals(att_name, "imageattr")){
		return tmedia_get_video_imageattr(TMEDIA_CODEC_VIDEO(self)->pref_size, 
			TMEDIA_CODEC_VIDEO(self)->in.width, TMEDIA_CODEC_VIDEO(self)->in.height, TMEDIA_CODEC_VIDEO(self)->out.width, TMEDIA_CODEC_VIDEO(self)->out.height);
	}
	return tsk_null;
}




/* ============ H.264 Base Profile Plugin interface ================= */

/* constructor */
static tsk_object_t* tdav_codec_h264_base_ctor(tsk_object_t * self, va_list * app)
{
	tdav_codec_h264_t *h264 = self;
	if(h264){
		/* init base: called by tmedia_codec_create() */
		/* init self */
		if(tdav_codec_h264_init(h264, profile_idc_baseline) != 0){
			return tsk_null;
		}
	}
	return self;
}
/* destructor */
static tsk_object_t* tdav_codec_h264_base_dtor(tsk_object_t * self)
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
static const tsk_object_def_t tdav_codec_h264_base_def_s = 
{
	sizeof(tdav_codec_h264_t),
	tdav_codec_h264_base_ctor, 
	tdav_codec_h264_base_dtor,
	tmedia_codec_cmp, 
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t tdav_codec_h264_base_plugin_def_s = 
{
	&tdav_codec_h264_base_def_s,

	tmedia_video,
	tmedia_codec_id_h264_bp,
	"H264",
	"H264 Base Profile",
	TMEDIA_CODEC_FORMAT_H264_BP,
	tsk_true,
	90000, // rate
	
	/* audio */
	{ 0 },

	/* video */
	{176, 144, 15},

	tdav_codec_h264_set,
	tdav_codec_h264_open,
	tdav_codec_h264_close,
	tdav_codec_h264_encode,
	tdav_codec_h264_decode,
	tdav_codec_h264_sdp_att_match,
	tdav_codec_h264_sdp_att_get
};
const tmedia_codec_plugin_def_t *tdav_codec_h264_base_plugin_def_t = &tdav_codec_h264_base_plugin_def_s;

/* ============ H.264 Main Profile Plugin interface ================= */

/* constructor */
static tsk_object_t* tdav_codec_h264_main_ctor(tsk_object_t * self, va_list * app)
{
	tdav_codec_h264_t *h264 = self;
	if(h264){
		/* init base: called by tmedia_codec_create() */
		/* init self */
		if(tdav_codec_h264_init(h264, profile_idc_main) != 0){
			return tsk_null;
		}
	}
	return self;
}
/* destructor */
static tsk_object_t* tdav_codec_h264_main_dtor(tsk_object_t * self)
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
static const tsk_object_def_t tdav_codec_h264_main_def_s = 
{
	sizeof(tdav_codec_h264_t),
	tdav_codec_h264_main_ctor, 
	tdav_codec_h264_main_dtor,
	tmedia_codec_cmp, 
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t tdav_codec_h264_main_plugin_def_s = 
{
	&tdav_codec_h264_main_def_s,

	tmedia_video,
	tmedia_codec_id_h264_mp,
	"H264",
	"H264 Main Profile",
	TMEDIA_CODEC_FORMAT_H264_MP,
	tsk_true,
	90000, // rate
	
	/* audio */
	{ 0 },

	/* video */
	{176, 144, 15},

	tdav_codec_h264_set,
	tdav_codec_h264_open,
	tdav_codec_h264_close,
	tdav_codec_h264_encode,
	tdav_codec_h264_decode,
	tdav_codec_h264_sdp_att_match,
	tdav_codec_h264_sdp_att_get
};
const tmedia_codec_plugin_def_t *tdav_codec_h264_main_plugin_def_t = &tdav_codec_h264_main_plugin_def_s;





































/* ============ Common To all H264 codecs ================= */

int tdav_codec_h264_open_encoder(tdav_codec_h264_t* self)
{
	int ret;
	tsk_size_t size;

	if(self->encoder.context){
		TSK_DEBUG_ERROR("Encoder already opened");
		return -1;
	}

	self->encoder.context = avcodec_alloc_context();
	avcodec_get_context_defaults(self->encoder.context);

#if TDAV_UNDER_X86 && LIBAVCODEC_VERSION_MAJOR <= 53
	self->encoder.context->dsp_mask = (FF_MM_MMX | FF_MM_MMXEXT | FF_MM_SSE);
#endif

	self->encoder.context->pix_fmt		= PIX_FMT_YUV420P;
	self->encoder.context->time_base.num  = 1;
	self->encoder.context->time_base.den  = TMEDIA_CODEC_VIDEO(self)->out.fps;
	self->encoder.context->width = (self->encoder.rotation == 90 || self->encoder.rotation == 270) ? TMEDIA_CODEC_VIDEO(self)->out.height : TMEDIA_CODEC_VIDEO(self)->out.width;
	self->encoder.context->height = (self->encoder.rotation == 90 || self->encoder.rotation == 270) ? TMEDIA_CODEC_VIDEO(self)->out.width : TMEDIA_CODEC_VIDEO(self)->out.height;

	self->encoder.context->bit_rate = ((TMEDIA_CODEC_VIDEO(self)->out.width * TMEDIA_CODEC_VIDEO(self)->out.height * 256 / 352 / 288) * 1000);
	self->encoder.context->rc_min_rate = (self->encoder.context->bit_rate >> 3);
	self->encoder.context->rc_max_rate = self->encoder.context->bit_rate;

#if LIBAVCODEC_VERSION_MAJOR <= 53
	self->encoder.context->rc_lookahead = 0;
#endif
	self->encoder.context->global_quality = FF_QP2LAMBDA * self->encoder.quality;
	
	self->encoder.context->scenechange_threshold = 0;
    self->encoder.context->me_subpel_quality = 0;
#if LIBAVCODEC_VERSION_MAJOR <= 53
    self->encoder.context->partitions = X264_PART_I4X4 | X264_PART_I8X8 | X264_PART_P8X8 | X264_PART_B8X8;
#endif
    self->encoder.context->me_method = ME_EPZS;
    self->encoder.context->trellis = 0;

	self->encoder.context->me_range = 16;
	self->encoder.context->qmin = 10;
	self->encoder.context->qmax = 51;
#if LIBAVCODEC_VERSION_MAJOR <= 53
    self->encoder.context->mb_qmin = self->encoder.context->qmin;
	self->encoder.context->mb_qmax = self->encoder.context->qmax;
#endif
	self->encoder.context->qcompress = 0.6f;
	self->encoder.context->mb_decision = FF_MB_DECISION_SIMPLE;
#if LIBAVCODEC_VERSION_MAJOR <= 53
	self->encoder.context->flags2 |= CODEC_FLAG2_FASTPSKIP;
#else 
    self->encoder.context->flags2 |= CODEC_FLAG2_FAST;
#endif
#if TDAV_UNDER_X86
	//self->encoder.context->flags2 &= ~CODEC_FLAG2_PSY;
	//self->encoder.context->flags2 |= CODEC_FLAG2_SSIM;
#endif
	self->encoder.context->flags |= CODEC_FLAG_LOOP_FILTER;
	self->encoder.context->flags |= CODEC_FLAG_GLOBAL_HEADER;
    self->encoder.context->flags |= CODEC_FLAG_LOW_DELAY;
	self->encoder.context->max_b_frames = 0;
	self->encoder.context->b_frame_strategy = 1;
	self->encoder.context->chromaoffset = 0;

	switch(TDAV_CODEC_H264_COMMON(self)->profile){
		case profile_idc_baseline:
		default:
			self->encoder.context->profile = FF_PROFILE_H264_BASELINE;
			self->encoder.context->level = TDAV_CODEC_H264_COMMON(self)->level;
			break;
		case profile_idc_main:
			self->encoder.context->profile = FF_PROFILE_H264_MAIN;
			self->encoder.context->level = TDAV_CODEC_H264_COMMON(self)->level;
			break;
	}
	
	self->encoder.context->rtp_payload_size = H264_RTP_PAYLOAD_SIZE;
	self->encoder.context->opaque = tsk_null;
	self->encoder.context->gop_size = (TMEDIA_CODEC_VIDEO(self)->out.fps * TDAV_H264_GOP_SIZE_IN_SECONDS);
	

	// Picture (YUV 420)
	if(!(self->encoder.picture = avcodec_alloc_frame())){
		TSK_DEBUG_ERROR("Failed to create encoder picture");
		return -2;
	}
	avcodec_get_frame_defaults(self->encoder.picture);
	

	size = avpicture_get_size(PIX_FMT_YUV420P, self->encoder.context->width, self->encoder.context->height);
	if(!(self->encoder.buffer = tsk_calloc(size, sizeof(uint8_t)))){
		TSK_DEBUG_ERROR("Failed to allocate encoder buffer");
		return -2;
	}

	// Open encoder
	if((ret = avcodec_open(self->encoder.context, self->encoder.codec)) < 0){
		TSK_DEBUG_ERROR("Failed to open [%s] codec", TMEDIA_CODEC(self)->plugin->desc);
		return ret;
	}

	return ret;
}

int tdav_codec_h264_close_encoder(tdav_codec_h264_t* self)
{
	if(self->encoder.context){
		avcodec_close(self->encoder.context);
		av_free(self->encoder.context);
		self->encoder.context = tsk_null;
	}
	if(self->encoder.picture){
		av_free(self->encoder.picture);
		self->encoder.picture = tsk_null;
	}
	if(self->encoder.buffer){
		TSK_FREE(self->encoder.buffer);
	}
	self->encoder.frame_count = 0;

	return 0;
}

int tdav_codec_h264_open_decoder(tdav_codec_h264_t* self)
{
	int ret;

	if(self->decoder.context){
		TSK_DEBUG_ERROR("Decoder already opened");
		return -1;
	}

	self->decoder.context = avcodec_alloc_context();
	avcodec_get_context_defaults(self->decoder.context);
	
	self->decoder.context->pix_fmt = PIX_FMT_YUV420P;
	self->decoder.context->flags2 |= CODEC_FLAG2_FAST;
	self->decoder.context->width = TMEDIA_CODEC_VIDEO(self)->in.width;
	self->decoder.context->height = TMEDIA_CODEC_VIDEO(self)->in.height;
	
#if TDAV_UNDER_WINDOWS
	self->decoder.context->dsp_mask = (FF_MM_MMX | FF_MM_MMXEXT | FF_MM_SSE);
#endif

	// Picture (YUV 420)
	if(!(self->decoder.picture = avcodec_alloc_frame())){
		TSK_DEBUG_ERROR("Failed to create decoder picture");
		return -2;
	}
	avcodec_get_frame_defaults(self->decoder.picture);

	// Open decoder
	if((ret = avcodec_open(self->decoder.context, self->decoder.codec)) < 0){
		TSK_DEBUG_ERROR("Failed to open [%s] codec", TMEDIA_CODEC(self)->plugin->desc);
		return ret;
	}

	return ret;
}

int tdav_codec_h264_close_decoder(tdav_codec_h264_t* self)
{
	if(self->decoder.context){
		avcodec_close(self->decoder.context);
		av_free(self->decoder.context);
		self->decoder.context = tsk_null;
	}
	if(self->decoder.picture){
		av_free(self->decoder.picture);
		self->decoder.picture = tsk_null;
	}
	TSK_FREE(self->decoder.accumulator);
	self->decoder.accumulator_pos = 0;

	return 0;
}

int tdav_codec_h264_init(tdav_codec_h264_t* self, profile_idc_t profile)
{
	int ret = 0;
	level_idc_t level;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if((ret = tdav_codec_h264_common_init(TDAV_CODEC_H264_COMMON(self)))){
		TSK_DEBUG_ERROR("tdav_codec_h264_common_init() faile with error code=%d", ret);
		return ret;
	}

	if((ret = tdav_codec_h264_common_level_from_size(TMEDIA_CODEC_VIDEO(self)->out.width, TMEDIA_CODEC_VIDEO(self)->out.height, &level))){
		TSK_DEBUG_ERROR("Failed to find level for size=[%u, %u]", TMEDIA_CODEC_VIDEO(self)->out.width, TMEDIA_CODEC_VIDEO(self)->out.height);
		return ret;
	}
	
	TDAV_CODEC_H264_COMMON(self)->pack_mode = H264_PACKETIZATION_MODE;
	TDAV_CODEC_H264_COMMON(self)->profile = profile;
	TDAV_CODEC_H264_COMMON(self)->level = level;
	TMEDIA_CODEC_VIDEO(self)->in.max_mbps = TMEDIA_CODEC_VIDEO(self)->out.max_mbps = H264_MAX_MBPS*1000;
	TMEDIA_CODEC_VIDEO(self)->in.max_br = TMEDIA_CODEC_VIDEO(self)->out.max_br = H264_MAX_BR*1000;

	if(!(self->encoder.codec = avcodec_find_encoder(CODEC_ID_H264))){
		TSK_DEBUG_ERROR("Failed to find H.264 encoder");
		ret = -2;
	}

	if(!(self->decoder.codec = avcodec_find_decoder(CODEC_ID_H264))){
		TSK_DEBUG_ERROR("Failed to find H.264 decoder");
		ret = -3;
	}

	self->encoder.quality = 1;

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
