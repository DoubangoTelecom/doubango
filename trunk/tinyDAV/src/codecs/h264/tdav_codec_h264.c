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

/**@file tdav_codec_h264.c
 * @brief H.264 codec plugin
 * RTP payloader/depayloader follows RFC 3984
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinydav/codecs/h264/tdav_codec_h264.h"

#include "tinydav/codecs/h264/tdav_codec_h264_rtp.h"
#include "tinydav/video/tdav_converter_video.h"

#include "tinyrtp/rtp/trtp_rtp_packet.h"

#include "tsk_params.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#define H264_PACKETIZATION_MODE	Non_Interleaved_Mode
#define H264_MAX_BR				452
#define H264_MAX_MBPS			11880

static void *run(void* self);

int tdav_codec_h264_init(tdav_codec_h264_t* self, tdav_codec_h264_profile_t profile);
int tdav_codec_h264_deinit(tdav_codec_h264_t* self);
tdav_codec_h264_profile_t tdav_codec_h264_get_profile(const char* fmtp);

/* ============ H.264 Base Profile X.X Plugin interface functions ================= */

#define tdav_codec_h264_fmtp_set tsk_null /* FIXME: should be removed from all plugins (useless) */

int tdav_codec_h264_open(tmedia_codec_t* self)
{
	int ret;
	int size;

	tdav_codec_h264_t* h264 = (tdav_codec_h264_t*)self;

	if(!h264){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	/* the caller (base class) already checked that the codec is not opened */

	//
	//	Runnable
	//
	h264->runnable = tdav_runnable_video_create(run, h264);
	if((ret = tdav_runnable_video_start(h264->runnable))){
		TSK_DEBUG_ERROR("Failed to create runnable (H264 codec)");
		return ret;
	}

	//
	//	Encoder
	//
	h264->encoder.context = avcodec_alloc_context();
	avcodec_get_context_defaults(h264->encoder.context);

	h264->encoder.context->pix_fmt		= PIX_FMT_YUV420P;
	h264->encoder.context->time_base.num  = 1;
	h264->encoder.context->time_base.den  = TMEDIA_CODEC_VIDEO(h264)->fps;
	h264->encoder.context->width = TMEDIA_CODEC_VIDEO(h264)->width;
	h264->encoder.context->height = TMEDIA_CODEC_VIDEO(h264)->height;

	h264->encoder.context->me_range = 16;
	h264->encoder.context->max_qdiff = 4;
	/*h264->encoder.context->mb_qmin =*/ h264->encoder.context->qmin = 10;
	/*h264->encoder.context->mb_qmax =*/ h264->encoder.context->qmax = 51;
	//h264->encoder.context->qcompress = 0.6f;
	//h264->encoder.context->mb_decision = FF_MB_DECISION_SIMPLE;
	//h264->encoder.context->flags |= CODEC_FLAG_LOOP_FILTER;
	h264->encoder.context->max_b_frames = 0;

	//h264->encoder.context->thread_count = 0;
	h264->encoder.context->rtp_payload_size = 0;
	//h264->encoder.context->opaque = tsk_null;
	h264->encoder.context->bit_rate = (float) (TMEDIA_CODEC_VIDEO(h264)->max_br) * 0.80f;
	h264->encoder.context->bit_rate_tolerance = (int) (TMEDIA_CODEC_VIDEO(h264)->max_br * 0.20f);
	h264->encoder.context->gop_size = TMEDIA_CODEC_VIDEO(h264)->fps*1; /* each 1 seconds */

	// Picture (YUV 420)
	if(!(h264->encoder.picture = avcodec_alloc_frame())){
		TSK_DEBUG_ERROR("Failed to create encoder picture");
		return -2;
	}
	avcodec_get_frame_defaults(h264->encoder.picture);
	//if((ret = avpicture_alloc((AVPicture*)h264->encoder.picture, PIX_FMT_YUV420P, h264->encoder.context->width, h264->encoder.context->height))){
	//	TSK_DEBUG_ERROR("Failed to allocate encoder picture");
	//	return ret;
	//}
	
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
	h264->decoder.context->width = TMEDIA_CODEC_VIDEO(h264)->width;
	h264->decoder.context->height = TMEDIA_CODEC_VIDEO(h264)->height;
	

	// Picture (YUV 420)
	if(!(h264->decoder.picture = avcodec_alloc_frame())){
		TSK_DEBUG_ERROR("Failed to create decoder picture");
		return -2;
	}
	avcodec_get_frame_defaults(h264->decoder.picture);

	size = avpicture_get_size(PIX_FMT_YUV420P, h264->decoder.context->width, h264->decoder.context->height);
	if(!(h264->decoder.accumulator = tsk_calloc((size + FF_INPUT_BUFFER_PADDING_SIZE), sizeof(uint8_t)))){
		TSK_DEBUG_ERROR("Failed to allocate decoder buffer");
		return -2;
	}

	// Open decoder
	if((ret = avcodec_open(h264->decoder.context, h264->decoder.codec)) < 0){
		TSK_DEBUG_ERROR("Failed to open [%s] codec", TMEDIA_CODEC(h264)->plugin->desc);
		return ret;
	}

	return 0;
}

int tdav_codec_h264_close(tmedia_codec_t* self)
{
	int ret;

	tdav_codec_h264_t* h264 = (tdav_codec_h264_t*)self;

	if(!h264){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	/* the caller (base class) already checked that the codec is opened */

	//
	// Runnable
	//
	if((ret = tdav_runnable_video_stop(h264->runnable))){
		TSK_DEBUG_ERROR("Failed to stop runnable (H263 codec)");
		// ... do not exit, continue
	}

	//
	//	Encoder
	//
	if(h264->encoder.context){
		avcodec_close(h264->encoder.context);
		av_free(h264->encoder.context);
	}
	if(h264->encoder.picture){
		av_free(h264->encoder.picture);
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
	}
	if(h264->decoder.picture){
		av_free(h264->decoder.picture);
	}
	if(h264->decoder.accumulator){
		TSK_FREE(h264->decoder.accumulator);
	}

	return 0;
}
#include "tsk_time.h"
tsk_size_t tdav_codec_h264_encode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data)
{
	int ret;
	int size;

	tdav_codec_h264_t* h264 = (tdav_codec_h264_t*)self;

	if(!self || !in_data || !in_size || !out_data){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}
	
	// delete old buffer
	if(*out_data){
		TSK_FREE(*out_data);
	}

	// wrap yuv420 buffer
	size = avpicture_fill((AVPicture *)h264->encoder.picture, (uint8_t*)in_data, PIX_FMT_YUV420P, h264->encoder.context->width, h264->encoder.context->height);
	if(size != in_size){
		/* guard */
		TSK_DEBUG_ERROR("Invalid size");
		return 0;
	}
	/* Flip */
	tdav_converter_video_flip(h264->encoder.picture, h264->encoder.context->height);

	// Encode data
	h264->encoder.picture->pts = tsk_time_epoch();//AV_NOPTS_VALUE;
	h264->encoder.picture->pict_type = FF_I_TYPE;
	ret = avcodec_encode_video(h264->encoder.context, h264->encoder.buffer, size, h264->encoder.picture);
	if(ret <= 0){
		ret = 0;
	}
	else{
		if((*out_data = tsk_calloc(ret, sizeof(uint8_t)))){
			memcpy(*out_data, h264->encoder.buffer, ret);
		}
		else{
			TSK_DEBUG_ERROR("Failed to allocate output buffer");
			ret = 0;
		}
	}
	
	if(ret/* > RTP_PAYLOAD_SIZE*/){
		tsk_buffer_t* buffer = tsk_buffer_create_null();
		tsk_buffer_takeownership(buffer, out_data, (tsk_size_t)ret);
		TSK_RUNNABLE_ENQUEUE_OBJECT(h264->runnable, buffer);
		ret = 0;
	}

	return (tsk_size_t)ret;
}

tsk_size_t tdav_codec_h264_decode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, const tsk_object_t* proto_hdr)
{
	tdav_codec_h264_t* h264 = (tdav_codec_h264_t*)self;
	const trtp_rtp_header_t* rtp_hdr = proto_hdr;
	
	const uint8_t* pay_ptr = tsk_null;
	tsk_size_t pay_size = 0;
	int ret;
	tsk_bool_t append_scp;
	tsk_size_t xsize, retsize = 0;
	int got_picture_ptr;

	if(!h264 || !in_data || !in_size || !out_data || !h264->decoder.context){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}

	/* Packet lost? */
	if(h264->decoder.last_seq != (rtp_hdr->seq_num - 1) && h264->decoder.last_seq){
		TSK_DEBUG_INFO("Packet lost");
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
	xsize = avpicture_get_size(h264->decoder.context->pix_fmt, h264->decoder.context->width, h264->decoder.context->height);
	
	if((int)(h264->decoder.accumulator_pos + pay_size) <= xsize){
		if(append_scp){
			memcpy(&((uint8_t*)h264->decoder.accumulator)[h264->decoder.accumulator_pos], H264_START_CODE_PREFIX, sizeof(H264_START_CODE_PREFIX));
			h264->decoder.accumulator_pos +=sizeof(H264_START_CODE_PREFIX);
		}

		memcpy(&((uint8_t*)h264->decoder.accumulator)[h264->decoder.accumulator_pos], pay_ptr, pay_size);
		h264->decoder.accumulator_pos += pay_size;
	}
	else{
		TSK_DEBUG_WARN("Buffer overflow");
		h264->decoder.accumulator_pos = 0;
		return 0;
	}

	if(rtp_hdr->marker){
		//AVPacket packet;
		/* allocate destination buffer */
		if(!(*out_data = tsk_calloc(xsize, sizeof(uint8_t)))){
			TSK_DEBUG_ERROR("Failed to allocate new buffer");
			h264->decoder.accumulator_pos = 0;
			return 0;
		}		

		/* wrap the output buffer (picture's internal buffer will be allocated by FFMpeg)*/
		//avpicture_fill((AVPicture *)h264->decoder.picture, *out_data, PIX_FMT_YUV420P, h264->decoder.context->width, h264->decoder.context->height);
		/* decode the vodeo frame (accumulator already padded -FF_INPUT_BUFFER_PADDING_SIZE)*/
		/*av_init_packet(&packet);
		packet.size = h264->decoder.accumulator_pos;
		packet.data = h264->decoder.accumulator;
		ret = avcodec_decode_video2(h264->decoder.context, h264->decoder.picture, &got_picture_ptr, &packet);*/
		ret = avcodec_decode_video(h264->decoder.context, h264->decoder.picture, &got_picture_ptr, h264->decoder.accumulator, h264->decoder.accumulator_pos);
		

		if(ret <0 || !got_picture_ptr){
			TSK_DEBUG_WARN("Failed to decode the buffer");
		}
		else{
			retsize = xsize;
			/* flip */
#if FLIP_DECODED_PICT
			tdav_converter_video_flip(h264->decoder.picture, h264->decoder.context->height);
#endif
			/* copy picture into a linear buffer */
			avpicture_layout((AVPicture *)h264->decoder.picture, h264->decoder.context->pix_fmt, h264->decoder.context->width, h264->decoder.context->height,
				*out_data, retsize);
		}
		/* in all cases: reset accumulator */
		h264->decoder.accumulator_pos = 0;		
	}

	return retsize;
}

tsk_bool_t tdav_codec_h264_fmtp_match(const tmedia_codec_t* codec, const char* fmtp)
{
	tdav_codec_h264_t* h264 = (tdav_codec_h264_t*)codec;
	tsk_params_L_t* params = tsk_null;
	int val_int;
	const char* val_str;
	tsk_bool_t ret = tsk_true;

	if(!h264){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_false;
	}

	/* Check whether the profile match */
	if(tdav_codec_h264_get_profile(fmtp) != h264->profile){
		return tsk_false;
	}

	/* e.g. profile-level-id=42e00a; packetization-mode=1; max-br=452; max-mbps=11880 */
	if((params = tsk_params_fromstring(fmtp, ';', tsk_true))){
		
		/* === max-br ===*/
		if((val_int = tsk_params_get_param_value_as_int(params, "max-br")) != -1){
			// should compare "max-br"?
			TMEDIA_CODEC_VIDEO(h264)->max_br = val_int*1000;
		}

		/* === max-mbps ===*/
		if((val_int = tsk_params_get_param_value_as_int(params, "max-mbps")) != -1){
			// should compare "max-mbps"?
			TMEDIA_CODEC_VIDEO(h264)->max_mbps = val_int*1000;
		}

		/* === packetization-mode ===*/
		if((val_int = tsk_params_get_param_value_as_int(params, "packetization-mode")) != -1){
			if((packetization_mode_t)val_int == Single_NAL_Unit_Mode || (packetization_mode_t)val_int == Non_Interleaved_Mode){
				h264->pack_mode = (packetization_mode_t)val_int;
			}
			else{
				ret = tsk_false;
				goto bail;
			}
		}

		/* === profile-level-id ===*/
		if((val_str = tsk_params_get_param_value(params, "profile-level-id"))){
			level_idc_t l_idc;
			/* profile-idc and level-idc already tested by tdav_codec_h264_get_profile() */
			tdav_codec_h264_parse_profile(val_str, tsk_null, tsk_null, &l_idc);
			switch(l_idc){
				case level_idc_1_0:
				case level_idc_1_b:
				case level_idc_1_1:
					TMEDIA_CODEC_VIDEO(h264)->width = 176, TMEDIA_CODEC_VIDEO(h264)->height = 144;
					break;
				default:
					TMEDIA_CODEC_VIDEO(h264)->width = 352, TMEDIA_CODEC_VIDEO(h264)->height = 288;
					break;
			}
		}
	}

bail:
	TSK_OBJECT_SAFE_FREE(params);
	return ret;
}

char* tdav_codec_h264_fmtp_get(const tmedia_codec_t* self)
{
	tdav_codec_h264_t* h264 = (tdav_codec_h264_t*)self;
	char* fmtp = tsk_null;

	if(!h264){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}

	switch(h264->profile){
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
			h264->pack_mode, TMEDIA_CODEC_VIDEO(h264)->max_br/1000, TMEDIA_CODEC_VIDEO(h264)->max_mbps/1000);
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
		tmedia_codec_video_deinit(self);
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
		tmedia_codec_video_deinit(self);
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
		tmedia_codec_video_deinit(self);
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
	
	self->pack_mode = H264_PACKETIZATION_MODE;
	self->profile = profile;
	TMEDIA_CODEC_VIDEO(self)->max_mbps = H264_MAX_MBPS*1000;
	TMEDIA_CODEC_VIDEO(self)->max_br = H264_MAX_BR*1000;

	// At this time self->plugin is Null
	TMEDIA_CODEC_VIDEO(self)->width = 176;
	TMEDIA_CODEC_VIDEO(self)->height = 144;
	TMEDIA_CODEC_VIDEO(self)->fps = 15;

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
	
	TSK_OBJECT_SAFE_FREE(self->runnable);
	
	TSK_FREE(self->rtp.ptr);
	self->rtp.size = 0;

	return 0;
}

tdav_codec_h264_profile_t tdav_codec_h264_get_profile(const char* fmtp)
{
	tdav_codec_h264_profile_t profile = tdav_codec_h264_bp99;
	tsk_size_t size = tsk_strlen(fmtp);
	int start, end;
	
	if((start = tsk_strindexOf(fmtp, size, "profile-level-id")) !=-1){
		tsk_param_t* param;
		if((end = tsk_strindexOf((fmtp+start), (size-start), ";")) == -1){
			end = size;
		}
		if((param = tsk_params_parse_param((fmtp+start), (end-start)))){
			profile_idc_t p_idc;
			level_idc_t l_idc;
			if(param->value){
				tsk_strtrim_both(&param->value);
			}
			
			tdav_codec_h264_parse_profile(param->value, &p_idc, tsk_null, &l_idc);
			
			switch(p_idc){
				case profile_idc_baseline:
					switch(l_idc){
						case level_idc_1_0:
						case level_idc_1_b:
						case level_idc_1_1:
						case level_idc_1_2:
						case level_idc_1_3:
							profile = tdav_codec_h264_bp10;
							break;
						case level_idc_2_0:
						case level_idc_2_1:
						case level_idc_2_2:
							profile = tdav_codec_h264_bp20;
							break;
						case level_idc_3_0:
							profile = tdav_codec_h264_bp30;
							break;
					}
					break;
				case profile_idc_extended:
				case profile_idc_main:
				case profile_idc_high:
				default:
					/* Not supported */
					break;
			}

			TSK_OBJECT_SAFE_FREE(param);
		}
	}
	return profile;
}


void *run(void* self)
{
	uint32_t i, last_scp, prev_scp;
	tsk_list_item_t *curr;

	const uint8_t* pdata;
	tsk_size_t size;
	tsk_bool_t frag;

	const tdav_codec_h264_t* h264 = ((tdav_runnable_video_t*)self)->userdata;
	static uint32_t size_of_scp = sizeof(H264_START_CODE_PREFIX); /* we know it's equal to 4 ..but */

	TSK_DEBUG_INFO("H264 thread === START");

	TSK_RUNNABLE_RUN_BEGIN(self);
	
	if((curr = TSK_RUNNABLE_POP_FIRST(self))){
		/* 4 is sizeof(uint32_t) */
		pdata = ((const tsk_buffer_t*)curr->data)->data;
		size = ((const tsk_buffer_t*)curr->data)->size;
		last_scp = 0, prev_scp = 0;
		frag = tsk_false;

#if 1
		if(size < H264_RTP_PAYLOAD_SIZE){
			goto last;
		}
#else
		goto last;
#endif

#define START_CODE_PREFIX		16777216 /* 0x10000000 */
		for(i = size_of_scp; i<(size - size_of_scp); i++){
			if( *( ((uint32_t*) &pdata[i])-0 ) == START_CODE_PREFIX ){
			//if(pdata[i] == H264_START_CODE_PREFIX[0] && pdata[i+1] == H264_START_CODE_PREFIX[1] && pdata[i+2] == H264_START_CODE_PREFIX[2] && pdata[i+3] == H264_START_CODE_PREFIX[3]){  /* Found Start Code Prefix */
				prev_scp = last_scp;
				if((i - last_scp) >= H264_RTP_PAYLOAD_SIZE || 1){
					tdav_codec_h264_rtp_callback((tdav_codec_h264_t*) h264, pdata + prev_scp,
						(i - prev_scp), (prev_scp == size));
				}
				last_scp = i;
			}
		}
last:
		if(last_scp < size){
				tdav_codec_h264_rtp_callback((tdav_codec_h264_t*) h264, pdata + last_scp,
					(size - last_scp), tsk_true);
		}

		tsk_object_unref(curr);
	}
	
	TSK_RUNNABLE_RUN_END(self);

	TSK_DEBUG_INFO("H264 thread === STOP");

	return tsk_null;
}