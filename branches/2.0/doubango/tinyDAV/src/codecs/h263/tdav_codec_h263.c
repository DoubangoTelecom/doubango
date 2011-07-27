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

/**@file tdav_codec_h263.c
 * @brief H.263-1996 and H.263-1998 codec plugins.
 * RTP payloader follows RFC 4629 for H263+ and RFC 2190 for H263.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#include "tinydav/codecs/h263/tdav_codec_h263.h"

#if HAVE_FFMPEG

#include "tinydav/video/tdav_converter_video.h"

#include "tinyrtp/rtp/trtp_rtp_packet.h"

#include "tnet_endianness.h"

#include "tsk_time.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#define RTP_PAYLOAD_SIZE	750

#define H263P_HEADER_SIZE		2
#define H263_HEADER_MODE_A_SIZE 4
#define H263_HEADER_MODE_B_SIZE 8
#define H263_HEADER_MODE_C_SIZE 12

static void tdav_codec_h263_rtp_callback(tdav_codec_h263_t *self, const void *data, tsk_size_t size, tsk_bool_t marker);
static void tdav_codec_h263p_rtp_callback(tdav_codec_h263_t *self, const void *data, tsk_size_t size, tsk_bool_t frag, tsk_bool_t marker);

static void tdav_codec_h263_encap(const tdav_codec_h263_t* h263, const uint8_t* pdata, tsk_size_t size);

/* ============ Common To all H263 codecs ================= */

int tdav_codec_h263_init(tdav_codec_h263_t* self, tdav_codec_h263_type_t type, enum CodecID encoder, enum CodecID decoder)
{
	int ret = 0;
	
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	
	self->type = type;

	if(!(self->encoder.codec = avcodec_find_encoder(encoder))){
		TSK_DEBUG_ERROR("Failed to find [%d]encoder", encoder);
		ret = -2;
	}

	if(!(self->decoder.codec = avcodec_find_decoder(decoder))){
		TSK_DEBUG_ERROR("Failed to find [%d] decoder", decoder);
		ret = -3;
	}

	/* allocations MUST be done by open() */
	return ret;
}

int tdav_codec_h263_deinit(tdav_codec_h263_t* self)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	self->encoder.codec = tsk_null;
	self->decoder.codec = tsk_null;

	// FFMpeg resources are destroyed by close()
	
	
	
	TSK_FREE(self->rtp.ptr);
	self->rtp.size = 0;

	return 0;
}



/* ============ H.263-1996 Plugin interface ================= */

//
//	H.263-1996 object definition
//
int tdav_codec_h263_open(tmedia_codec_t* self)
{
	int ret;
	int size;

	tdav_codec_h263_t* h263 = (tdav_codec_h263_t*)self;

	if(!h263){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	/* the caller (base class) already checked that the codec is not opened */

	//
	//	Encoder
	//
	h263->encoder.context = avcodec_alloc_context();
	avcodec_get_context_defaults(h263->encoder.context);
	
	h263->encoder.context->pix_fmt		= PIX_FMT_YUV420P;
	h263->encoder.context->time_base.num  = 1;
	h263->encoder.context->time_base.den  = TMEDIA_CODEC_VIDEO(h263)->out.fps;
	h263->encoder.context->width = TMEDIA_CODEC_VIDEO(h263)->out.width;
	h263->encoder.context->height = TMEDIA_CODEC_VIDEO(h263)->out.height;

	/*h263->encoder.context->mb_qmin =*/ h263->encoder.context->qmin = 4;
	/*h263->encoder.context->mb_qmax =*/ h263->encoder.context->qmax = 31;
	h263->encoder.context->mb_decision = FF_MB_DECISION_RD;

	h263->encoder.context->thread_count = 1;
	h263->encoder.context->rtp_payload_size = RTP_PAYLOAD_SIZE;
	h263->encoder.context->opaque = tsk_null;
	h263->encoder.context->gop_size = TMEDIA_CODEC_VIDEO(h263)->out.fps*2; /* each 2 seconds */
	h263->encoder.context->flags |= CODEC_FLAG_QSCALE;
	h263->encoder.context->global_quality = FF_QP2LAMBDA * tmedia_get_video_qscale(self->bl);

	// Picture (YUV 420)
	if(!(h263->encoder.picture = avcodec_alloc_frame())){
		TSK_DEBUG_ERROR("Failed to create encoder picture");
		return -2;
	}
	avcodec_get_frame_defaults(h263->encoder.picture);
	//if((ret = avpicture_alloc((AVPicture*)h263->encoder.picture, PIX_FMT_YUV420P, h263->encoder.context->width, h263->encoder.context->height))){
	//	TSK_DEBUG_ERROR("Failed to allocate encoder picture");
	//	return ret;
	//}
	
	size = avpicture_get_size(PIX_FMT_YUV420P, h263->encoder.context->width, h263->encoder.context->height);
	if(!(h263->encoder.buffer = tsk_calloc(size, sizeof(uint8_t)))){
		TSK_DEBUG_ERROR("Failed to allocate encoder buffer");
		return -2;
	}


	// RTP Callback
	switch(h263->type){
		case tdav_codec_h263_1996:
			{	/* H263 - 1996 */
				break;
			}
		case tdav_codec_h263_1998:
			{	/* H263 - 1998 */
				h263->encoder.context->flags |= CODEC_FLAG_H263P_UMV;		/* Annex D+ */
				h263->encoder.context->flags |= CODEC_FLAG_AC_PRED;			/* Annex I and T */
				h263->encoder.context->flags |= CODEC_FLAG_LOOP_FILTER;		/* Annex J */
				h263->encoder.context->flags |= CODEC_FLAG_H263P_SLICE_STRUCT;	/* Annex K */
				h263->encoder.context->flags |= CODEC_FLAG_H263P_AIV;			/* Annex S */
				break;
			}
		case tdav_codec_h263_2000:
			{	/* H263 - 2000 */
				h263->encoder.context->flags |= CODEC_FLAG_H263P_UMV;		/* Annex D+ */
				h263->encoder.context->flags |= CODEC_FLAG_AC_PRED;			/* Annex I and T */
				h263->encoder.context->flags |= CODEC_FLAG_LOOP_FILTER;		/* Annex J */
				h263->encoder.context->flags |= CODEC_FLAG_H263P_SLICE_STRUCT;	/* Annex K */
				h263->encoder.context->flags |= CODEC_FLAG_H263P_AIV;			/* Annex S */
				break;
			}
	}
	// Open encoder
	if((ret = avcodec_open(h263->encoder.context, h263->encoder.codec)) < 0){
		TSK_DEBUG_ERROR("Failed to open [%s] codec", TMEDIA_CODEC(h263)->plugin->desc);
		return ret;
	}

	//
	//	Decoder
	//
	h263->decoder.context = avcodec_alloc_context();
	avcodec_get_context_defaults(h263->decoder.context);
	
	h263->decoder.context->pix_fmt = PIX_FMT_YUV420P;
	h263->decoder.context->width = TMEDIA_CODEC_VIDEO(h263)->in.width;
	h263->decoder.context->height = TMEDIA_CODEC_VIDEO(h263)->in.height;

	// Picture (YUV 420)
	if(!(h263->decoder.picture = avcodec_alloc_frame())){
		TSK_DEBUG_ERROR("Failed to create decoder picture");
		return -2;
	}
	avcodec_get_frame_defaults(h263->decoder.picture);

	size = avpicture_get_size(PIX_FMT_YUV420P, h263->decoder.context->width, h263->decoder.context->height);
	if(!(h263->decoder.accumulator = tsk_calloc((size + FF_INPUT_BUFFER_PADDING_SIZE), sizeof(uint8_t)))){
		TSK_DEBUG_ERROR("Failed to allocate decoder buffer");
		return -2;
	}

	// Open decoder
	if((ret = avcodec_open(h263->decoder.context, h263->decoder.codec)) < 0){
		TSK_DEBUG_ERROR("Failed to open [%s] codec", TMEDIA_CODEC(h263)->plugin->desc);
		return ret;
	}

	return 0;
}

int tdav_codec_h263_close(tmedia_codec_t* self)
{
	tdav_codec_h263_t* h263 = (tdav_codec_h263_t*)self;

	if(!h263){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	/* the caller (base class) already checked that the codec is opened */

	//
	//	Encoder
	//
	if(h263->encoder.context){
		avcodec_close(h263->encoder.context);
		av_free(h263->encoder.context);
	}
	if(h263->encoder.picture){
		av_free(h263->encoder.picture);
	}
	if(h263->encoder.buffer){
		TSK_FREE(h263->encoder.buffer);
	}

	//
	//	Decoder
	//
	if(h263->decoder.context){
		avcodec_close(h263->decoder.context);
		av_free(h263->decoder.context);
	}
	if(h263->decoder.picture){
		av_free(h263->decoder.picture);
	}
	if(h263->decoder.accumulator){
		TSK_FREE(h263->decoder.accumulator);
		h263->decoder.accumulator_pos = 0;
	}

	return 0;
}

tsk_size_t tdav_codec_h263_encode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size)
{
	int ret;
	int size;

	tdav_codec_h263_t* h263 = (tdav_codec_h263_t*)self;

	if(!self || !in_data || !in_size || !out_data){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}

	// wrap yuv420 buffer
	size = avpicture_fill((AVPicture *)h263->encoder.picture, (uint8_t*)in_data, PIX_FMT_YUV420P, h263->encoder.context->width, h263->encoder.context->height);
	if(size != in_size){
		/* guard */
		TSK_DEBUG_ERROR("Invalid size");
		return 0;
	}
	
	// Flip
	if(self->video.flip.encoded){
		tdav_converter_video_flip(h263->encoder.picture, h263->encoder.context->height);
	}

	h263->encoder.picture->pts = AV_NOPTS_VALUE;
	h263->encoder.picture->quality = h263->encoder.context->global_quality;
	ret = avcodec_encode_video(h263->encoder.context, h263->encoder.buffer, size, h263->encoder.picture);
	if(ret > 0){
		tdav_codec_h263_encap(h263, h263->encoder.buffer, (tsk_size_t)ret);
	}

	return 0;
}

tsk_size_t tdav_codec_h263_decode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size, const tsk_object_t* proto_hdr)
{
	uint8_t F, P, sbit, ebit;
	const uint8_t* pdata = in_data;
	const uint8_t* pay_ptr;
	tsk_size_t pay_size;
	tsk_size_t hdr_size;
	tsk_size_t xsize, retsize = 0;
	int got_picture_ptr;
	int ret;

	tdav_codec_h263_t* h263 = (tdav_codec_h263_t*)self;
	const trtp_rtp_header_t* rtp_hdr = proto_hdr;

	if(!self || !in_data || !in_size || !out_data || !h263->decoder.context){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}

	/*	RFC 2190
		get F and P bits, used to determine the header Mode (A, B or C)
		F: 1 bit 
		The flag bit indicates the mode of the payload header. F=0, mode A;
		F=1, mode B or mode C depending on P bit defined below.
		P: 1 bit
		Optional PB-frames mode as defined by the H.263 [4]. "0" implies
		normal I or P frame, "1" PB-frames. When F=1, P also indicates modes:
		mode B if P=0, mode C if P=1.
	*/
	F = *pdata >> 7;
	P = (*pdata >> 6) & 0x01;

	/* SBIT and EBIT */
	sbit = (*pdata >> 3) & 0x0F;
	ebit = (*pdata & 0x07);
	
	if(F == 0){
		/*	MODE A
			0                   1                   2                   3
			0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
			+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
			|F|P|SBIT |EBIT | SRC |I|U|S|A|R      |DBQ| TRB |    TR         |
			+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		*/
		hdr_size = H263_HEADER_MODE_A_SIZE;
	}
	else if(P == 0){ // F=1 and P=0
		/* MODE B
			 0                   1                   2                   3
			0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
			+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
			|F|P|SBIT |EBIT | SRC | QUANT   |  GOBN   |   MBA           |R  |
			+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
			|I|U|S|A| HMV1        | VMV1        | HMV2        | VMV2        |
			+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		*/
		hdr_size = H263_HEADER_MODE_B_SIZE;
	}
	else{ // F=1 and P=1
		/* MODE C 
			 0                   1                   2                   3
			0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
			+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
			|F|P|SBIT |EBIT | SRC | QUANT   |  GOBN   |   MBA           |R  |
			+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
			|I|U|S|A| HMV1        | VMV1        | HMV2        | VMV2        |
			+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
			| RR                                  |DBQ| TRB |    TR         |
			+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		*/
		hdr_size = H263_HEADER_MODE_C_SIZE;
	}

	/* Check size */
	if(in_size < hdr_size){
		TSK_DEBUG_ERROR("Too short");
		return 0;
	}

	pay_ptr = (pdata + hdr_size);
	pay_size = (in_size - hdr_size);
	xsize = avpicture_get_size(h263->decoder.context->pix_fmt, h263->decoder.context->width, h263->decoder.context->height);

	/* Packet lost? */
	if(h263->decoder.last_seq != (rtp_hdr->seq_num - 1) && h263->decoder.last_seq){
		if(h263->decoder.last_seq == rtp_hdr->seq_num){
			// Could happen on some stupid emulators
			//TSK_DEBUG_INFO("Packet duplicated, seq_num=%d", rtp_hdr->seq_num);
			return 0;
		}
		TSK_DEBUG_INFO("Packet lost, seq_num=%d", rtp_hdr->seq_num);
	}
	h263->decoder.last_seq = rtp_hdr->seq_num;
	
	if((int)(h263->decoder.accumulator_pos + pay_size) <= xsize){
		if((h263->decoder.ebit + sbit) == 8){ /* Perfect one Byte to clean up */
			if(h263->decoder.accumulator_pos){
				((uint8_t*)h263->decoder.accumulator)[h263->decoder.accumulator_pos-1] = (((uint8_t*)h263->decoder.accumulator)[h263->decoder.accumulator_pos-1] & (0xFF << h263->decoder.ebit)) |
					(*pay_ptr & (0xFF >> sbit));
			}
			pay_ptr++, pay_size--;
		}
		h263->decoder.ebit = ebit;

		memcpy(&((uint8_t*)h263->decoder.accumulator)[h263->decoder.accumulator_pos], pay_ptr, pay_size);
		h263->decoder.accumulator_pos += pay_size;
	}
	else{
		TSK_DEBUG_WARN("Buffer overflow");
		h263->decoder.accumulator_pos = 0;
		return 0;
	}
	
	if(rtp_hdr->marker){
		AVPacket packet;
		/* allocate destination buffer */
		if(*out_max_size <xsize){
			if(!(*out_data = tsk_realloc(*out_data, xsize))){
				TSK_DEBUG_ERROR("Failed to allocate new buffer");
				h263->decoder.accumulator_pos = 0;
				*out_max_size = 0;
				return 0;
			}
			*out_max_size = xsize;
		}		

		av_init_packet(&packet);
		packet.size = h263->decoder.accumulator_pos;
		packet.data = h263->decoder.accumulator;
		ret = avcodec_decode_video2(h263->decoder.context, h263->decoder.picture, &got_picture_ptr, &packet);

		if(ret <0 || !got_picture_ptr){
			TSK_DEBUG_WARN("Failed to decode the buffer");
		}
		else{
			retsize = xsize;
			TMEDIA_CODEC_VIDEO(h263)->in.width = h263->decoder.context->width;
			TMEDIA_CODEC_VIDEO(h263)->in.height = h263->decoder.context->height;
			// flip
			if(self->video.flip.decoded){
				tdav_converter_video_flip(h263->decoder.picture, h263->decoder.context->height);
			}
			/* copy picture into a linear buffer */
			avpicture_layout((AVPicture *)h263->decoder.picture, h263->decoder.context->pix_fmt, h263->decoder.context->width, h263->decoder.context->height,
				*out_data, retsize);
		}
		/* in all cases: reset accumulator */
		h263->decoder.accumulator_pos = 0;		
	}

	return retsize;
}

tsk_bool_t tdav_codec_h263_fmtp_match(const tmedia_codec_t* codec, const char* fmtp)
{	
	tsk_bool_t ret = tsk_false;
	tmedia_codec_video_t* h263 = (tmedia_codec_video_t*)codec;
	tsk_params_L_t* params = tsk_null;

	if((params = tsk_params_fromstring(fmtp, ";", tsk_true))){
		switch(codec->bl){
			case tmedia_bl_low:
			default:
				if(tsk_params_have_param(params, "QCIF")){
					h263->in.width = h263->out.width = 176, h263->in.height = h263->out.height = 144;
					ret = tsk_true;
				}
				else if(tsk_params_have_param(params, "SQCIF")){
					h263->in.width = h263->out.width = 128, h263->in.height = h263->out.height = 96;
					ret = tsk_true;
				}
				break;

			case tmedia_bl_medium:
			case tmedia_bl_hight:
			case tmedia_bl_unrestricted:
				if(tsk_params_have_param(params, "CIF")){
					h263->in.width = h263->out.width = 352, h263->in.height = h263->out.height = 288;
					ret = tsk_true;
				}
				else if(tsk_params_have_param(params, "QCIF")){
					h263->in.width = h263->out.width = 176, h263->in.height = h263->out.height = 144;
					ret = tsk_true;
				}
				else if(tsk_params_have_param(params, "SQCIF")){
					h263->in.width = h263->out.width = 128, h263->in.height = h263->out.height = 96;
					ret = tsk_true;
				}
				else { // Default: to be fixed
					h263->in.width = h263->out.width = 352, h263->in.height = h263->out.height = 288;
					ret = tsk_true;
				}

				break;
		}
	}
	TSK_OBJECT_SAFE_FREE(params);

	return ret;
}

char* tdav_codec_h263_fmtp_get(const tmedia_codec_t* self)
{
	switch(self->bl){
		case tmedia_bl_low:
		default:
			return tsk_strdup("QCIF=2;SQCIF=2");
			break;
		case tmedia_bl_medium:
		case tmedia_bl_hight:
		case tmedia_bl_unrestricted:
			return tsk_strdup("CIF=2;QCIF=2;SQCIF=2");
			break;
	}
}

int tdav_codec_h263_fmtp_set(tmedia_codec_t* self, const char* fmtp)
{
	TSK_DEBUG_INFO("remote fmtp=%s", fmtp);
	return 0;
}

/* constructor */
static tsk_object_t* tdav_codec_h263_ctor(tsk_object_t * self, va_list * app)
{
	tdav_codec_h263_t *h263 = self;
	if(h263){
		/* init base: called by tmedia_codec_create() */
		/* init self */
		tdav_codec_h263_init(TDAV_CODEC_H263(self), tdav_codec_h263_1996, CODEC_ID_H263, CODEC_ID_H263);
	}
	return self;
}
/* destructor */
static tsk_object_t* tdav_codec_h263_dtor(tsk_object_t * self)
{ 
	tdav_codec_h263_t *h263 = self;
	if(h263){
		/* deinit base */
		tmedia_codec_video_deinit(h263);
		/* deinit self */
		tdav_codec_h263_deinit(TDAV_CODEC_H263(self));
		
	}

	return self;
}
/* object definition */
static const tsk_object_def_t tdav_codec_h263_def_s = 
{
	sizeof(tdav_codec_h263_t),
	tdav_codec_h263_ctor, 
	tdav_codec_h263_dtor,
	tmedia_codec_cmp, 
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t tdav_codec_h263_plugin_def_s = 
{
	&tdav_codec_h263_def_s,

	tmedia_video,
	"H263",
	"H263-1996 codec",
	TMEDIA_CODEC_FORMAT_H263,
	tsk_false,
	90000, // rate
	
	/* audio */
	{ 0 },

	/* video */
	{176, 144, 15},

	tdav_codec_h263_open,
	tdav_codec_h263_close,
	tdav_codec_h263_encode,
	tdav_codec_h263_decode,
	tdav_codec_h263_fmtp_match,
	tdav_codec_h263_fmtp_get,
	tdav_codec_h263_fmtp_set
};
const tmedia_codec_plugin_def_t *tdav_codec_h263_plugin_def_t = &tdav_codec_h263_plugin_def_s;























/* ============ H.263-1998 Plugin interface ================= */

//
//	H.263-1998 object definition
//

int tdav_codec_h263p_open(tmedia_codec_t* self)
{
	return tdav_codec_h263_open(self);
}

int tdav_codec_h263p_close(tmedia_codec_t* self)
{
	return tdav_codec_h263_close(self);
}

tsk_size_t tdav_codec_h263p_encode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size)
{
	return tdav_codec_h263_encode(self, in_data, in_size, out_data, out_max_size);
}

tsk_size_t tdav_codec_h263p_decode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size, const tsk_object_t* proto_hdr)
{
	uint8_t P, V, PLEN, PEBIT;
	uint8_t* pdata = (uint8_t*)in_data;
	const uint8_t* pay_ptr;
	tsk_size_t pay_size;
	int hdr_size = H263P_HEADER_SIZE;
	tsk_size_t xsize, retsize = 0;
	int got_picture_ptr;
	int ret;

	tdav_codec_h263_t* h263 = (tdav_codec_h263_t*)self;
	const trtp_rtp_header_t* rtp_hdr = proto_hdr;

	if(!self || !in_data || !in_size || ((int)in_size <= hdr_size) || !out_data || !h263->decoder.context){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}

/*
	5.1.  General H.263+ Payload Header

         0                   1
         0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        |   RR    |P|V|   PLEN    |PEBIT|
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
	P = (pdata[0] & 0x04)>>2;
	V = (pdata[0] & 0x02)>>1;
	PLEN = (((pdata[0] & 0x01)<<5) | pdata[1]>>3);
	PEBIT = pdata[1] & 0x07;
	
	if(V){
		/* 
			Indicates the presence of an 8-bit field containing information
			for Video Redundancy Coding (VRC), which follows immediately after
			the initial 16 bits of the payload header, if present.  For syntax
			and semantics of that 8-bit VRC field, see Section 5.2.
		*/
	}
	if(PLEN){
			/*
				Length, in bytes, of the extra picture header.  If no extra
				picture header is attached, PLEN is 0.  If PLEN>0, the extra
				picture header is attached immediately following the rest of the
				payload header.  Note that the length reflects the omission of the
				first two bytes of the picture start code (PSC).  See Section 6.1.
			*/
		hdr_size += PLEN;
		if(PEBIT){
			/*	
				Indicates the number of bits that shall be ignored in the last
				byte of the picture header.  If PLEN is not zero, the ignored bits
				shall be the least significant bits of the byte.  If PLEN is zero,
				then PEBIT shall also be zero.
			*/
			TSK_DEBUG_WARN("PEBIT ignored");
		}
	}
	if(P){ /* MUST be done after PLEN and PEBIT */
			/*	
				Indicates the picture start or a picture segment (GOB/Slice) start
				or a video sequence end (EOS or EOSBS).  Two bytes of zero bits
				then have to be prefixed to the payload of such a packet to
				compose a complete picture/GOB/slice/EOS/EOSBS start code.  This
				bit allows the omission of the two first bytes of the start codes,
				thus improving the compression ratio.
			*/
		hdr_size -= 2;
		pdata[hdr_size] = 0x00, pdata[hdr_size + 1] = 0x00;
	}

	pay_ptr = (pdata + hdr_size);
	pay_size = (in_size - hdr_size);
	xsize = avpicture_get_size(h263->decoder.context->pix_fmt, h263->decoder.context->width, h263->decoder.context->height);

	/* Packet lost? */
	if(h263->decoder.last_seq != (rtp_hdr->seq_num - 1) && h263->decoder.last_seq){
		if(h263->decoder.last_seq == rtp_hdr->seq_num){
			// Could happen on some stupid emulators
			//TSK_DEBUG_INFO("Packet duplicated, seq_num=%d", rtp_hdr->seq_num);
			return 0;
		}
		TSK_DEBUG_INFO("Packet lost, seq_num=%d", rtp_hdr->seq_num);
	}
	h263->decoder.last_seq = rtp_hdr->seq_num;

	if((int)(h263->decoder.accumulator_pos + pay_size) <= xsize){
		/* PEBIT is ignored */
		memcpy(&((uint8_t*)h263->decoder.accumulator)[h263->decoder.accumulator_pos], pay_ptr, pay_size);
		h263->decoder.accumulator_pos += pay_size;
	}
	else{
		TSK_DEBUG_WARN("Buffer overflow");
		h263->decoder.accumulator_pos = 0;
		return 0;
	}
	
	if(rtp_hdr->marker){
		AVPacket packet;
		/* allocate destination buffer */
		if(*out_max_size < xsize){
			if(!(*out_data = tsk_realloc(*out_data, xsize))){
				TSK_DEBUG_ERROR("Failed to allocate new buffer");
				*out_max_size = 0;
				h263->decoder.accumulator_pos = 0;
				return 0;
			}
			*out_max_size = xsize;
		}	

		/* decode the picture */
		av_init_packet(&packet);
		packet.size = h263->decoder.accumulator_pos;
		packet.data = h263->decoder.accumulator;
		ret = avcodec_decode_video2(h263->decoder.context, h263->decoder.picture, &got_picture_ptr, &packet);		

		if(ret <0 || !got_picture_ptr){
			TSK_DEBUG_WARN("Failed to decode the buffer");
		}
		else{
			retsize = xsize;
			// flip
			if(self->video.flip.decoded){
				tdav_converter_video_flip(h263->decoder.picture, h263->decoder.context->height);
			}
			/* copy picture into a linear buffer */
			avpicture_layout((AVPicture *)h263->decoder.picture, h263->decoder.context->pix_fmt, h263->decoder.context->width, h263->decoder.context->height,
				*out_data, retsize);
		}
		/* in all cases: reset accumulator */
		h263->decoder.accumulator_pos = 0;		
	}

	return retsize;
}

tsk_bool_t tdav_codec_h263p_fmtp_match(const tmedia_codec_t* self, const char* fmtp)
{	
	return tdav_codec_h263_fmtp_match(self, fmtp);
}

char* tdav_codec_h263p_fmtp_get(const tmedia_codec_t* self)
{
	return tdav_codec_h263_fmtp_get(self);
}

int tdav_codec_h263p_fmtp_set(tmedia_codec_t* self, const char* fmtp)
{
	TSK_DEBUG_INFO("remote fmtp=%s", fmtp);
	return 0;
}

/* constructor */
static tsk_object_t* tdav_codec_h263p_ctor(tsk_object_t * self, va_list * app)
{
	tdav_codec_h263p_t *h263p = self;
	if(h263p){
		/* init base: called by tmedia_codec_create() */
		/* init self */
		tdav_codec_h263_init(TDAV_CODEC_H263(self), tdav_codec_h263_1998, CODEC_ID_H263P, CODEC_ID_H263);
	}
	return self;
}
/* destructor */
static tsk_object_t* tdav_codec_h263p_dtor(tsk_object_t * self)
{ 
	tdav_codec_h263p_t *h263p = self;
	if(h263p){
		/* deinit base */
		tmedia_codec_video_deinit(h263p);
		/* deinit self */
		tdav_codec_h263_deinit(TDAV_CODEC_H263(self));
	}

	return self;
}
/* object definition */
static const tsk_object_def_t tdav_codec_h263p_def_s = 
{
	sizeof(tdav_codec_h263p_t),
	tdav_codec_h263p_ctor, 
	tdav_codec_h263p_dtor,
	tmedia_codec_cmp, 
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t tdav_codec_h263p_plugin_def_s = 
{
	&tdav_codec_h263p_def_s,

	tmedia_video,
	"H263-1998",
	"H263-1998 codec",
	TMEDIA_CODEC_FORMAT_H263_1998,
	tsk_true,
	90000, // rate
	
	/* audio */
	{ 0 },

	/* video */
	{176, 144, 15},

	tdav_codec_h263p_open,
	tdav_codec_h263p_close,
	tdav_codec_h263p_encode,
	tdav_codec_h263p_decode,
	tdav_codec_h263p_fmtp_match,
	tdav_codec_h263p_fmtp_get,
	tdav_codec_h263p_fmtp_set
};
const tmedia_codec_plugin_def_t *tdav_codec_h263p_plugin_def_t = &tdav_codec_h263p_plugin_def_s;














/* ============ H.263-2000 Plugin interface ================= */

//
//	H.263-2000 object definition
//

int tdav_codec_h263pp_open(tmedia_codec_t* self)
{
	return tdav_codec_h263_open(self);
}

int tdav_codec_h263pp_close(tmedia_codec_t* self)
{
	return tdav_codec_h263_close(self);
}

tsk_size_t tdav_codec_h263pp_encode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size)
{
	return tdav_codec_h263_encode(self, in_data, in_size, out_data, out_max_size);
}

tsk_size_t tdav_codec_h263pp_decode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size, const tsk_object_t* proto_hdr)
{
	return tdav_codec_h263p_decode(self, in_data, in_size, out_data, out_max_size, proto_hdr);
}

tsk_bool_t tdav_codec_h263pp_fmtp_match(const tmedia_codec_t* self, const char* fmtp)
{	
	return tdav_codec_h263_fmtp_match(self, fmtp);
}

char* tdav_codec_h263pp_fmtp_get(const tmedia_codec_t* self)
{
	return tdav_codec_h263_fmtp_get(self);
}

int tdav_codec_h263pp_fmtp_set(tmedia_codec_t* self, const char* fmtp)
{
	TSK_DEBUG_INFO("remote fmtp=%s", fmtp);
	return 0;
}

/* constructor */
static tsk_object_t* tdav_codec_h263pp_ctor(tsk_object_t * self, va_list * app)
{
	tdav_codec_h263pp_t *h263pp = self;
	if(h263pp){
		/* init base: called by tmedia_codec_create() */
		/* init self */
		tdav_codec_h263_init(TDAV_CODEC_H263(self), tdav_codec_h263_2000, CODEC_ID_H263P, CODEC_ID_H263);
	}
	return self;
}
/* destructor */
static tsk_object_t* tdav_codec_h263pp_dtor(tsk_object_t * self)
{ 
	tdav_codec_h263pp_t *h263pp = self;
	if(h263pp){
		/* deinit base */
		tmedia_codec_video_deinit(h263pp);
		/* deinit self */
		tdav_codec_h263_deinit(TDAV_CODEC_H263(self));
	}

	return self;
}
/* object definition */
static const tsk_object_def_t tdav_codec_h263pp_def_s = 
{
	sizeof(tdav_codec_h263pp_t),
	tdav_codec_h263pp_ctor, 
	tdav_codec_h263pp_dtor,
	tmedia_codec_cmp, 
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t tdav_codec_h263pp_plugin_def_s = 
{
	&tdav_codec_h263pp_def_s,

	tmedia_video,
	"H263-2000",
	"H263-2000 codec",
	TMEDIA_CODEC_FORMAT_H263_2000,
	tsk_true,
	90000, // rate
	
	/* audio */
	{ 0 },

	/* video */
	{176, 144, 15},

	tdav_codec_h263pp_open,
	tdav_codec_h263pp_close,
	tdav_codec_h263pp_encode,
	tdav_codec_h263pp_decode,
	tdav_codec_h263pp_fmtp_match,
	tdav_codec_h263pp_fmtp_get,
	tdav_codec_h263pp_fmtp_set
};
const tmedia_codec_plugin_def_t *tdav_codec_h263pp_plugin_def_t = &tdav_codec_h263pp_plugin_def_s;



/* ============ Callbacks ================= */

static void tdav_codec_h263_encap(const tdav_codec_h263_t* h263, const uint8_t* pdata, tsk_size_t size)
{
	tsk_bool_t frag = tsk_false;
	register uint32_t i, last_index = 0;

	if(size < RTP_PAYLOAD_SIZE){
		goto last;
	}

	for(i = 4; i<(size - 4); i++){
		if(pdata[i] == 0x00 && pdata[i+1] == 0x00 && pdata[i+2]>=0x80){  /* PSC or (GBSC) found */
			if((i - last_index) >= RTP_PAYLOAD_SIZE || tsk_true/* FIXME */){
				switch(h263->type){
					case tdav_codec_h263_1996:
						tdav_codec_h263_rtp_callback((tdav_codec_h263_t*) h263, pdata+last_index,
							(i - last_index), (last_index == size));
						break;
					default:
						tdav_codec_h263p_rtp_callback((tdav_codec_h263_t*) h263, pdata + last_index,
							(i - last_index), frag, (last_index == size));
						frag = tsk_true;
						break;
				}
				last_index = i;
			}
		}
	}
last:
	if(last_index < size){
		switch(h263->type){
			case tdav_codec_h263_1996:
				tdav_codec_h263_rtp_callback((tdav_codec_h263_t*) h263, pdata + last_index,
					(size - last_index), tsk_true);
				break;
			default:
				tdav_codec_h263p_rtp_callback((tdav_codec_h263_t*) h263, pdata + last_index,
					(size - last_index), frag, tsk_true);
				break;
		}
	}
}


static void tdav_codec_h263_rtp_callback(tdav_codec_h263_t *self, const void *data, tsk_size_t size, tsk_bool_t marker)
{
	uint8_t* pdata = (uint8_t*)data;

	if(self->rtp.size < (size + H263_HEADER_MODE_A_SIZE)){
		if(!(self->rtp.ptr = tsk_realloc(self->rtp.ptr, (size + H263_HEADER_MODE_A_SIZE)))){
			TSK_DEBUG_ERROR("Failed to allocate new buffer");
			return;
		}
		self->rtp.size = (size + H263_HEADER_MODE_A_SIZE);
	}
	memcpy((self->rtp.ptr + H263_HEADER_MODE_A_SIZE), data, size);

	/* http://eu.sabotage.org/www/ITU/H/H0263e.pdf section 5.1
	* 5.1.1 Picture Start Code (PSC) (22 bits) - PSC is a word of 22 bits. Its value is 0000 0000 0000 0000 1 00000.

	*
	* 5.1.1 Picture Start Code (PSC) (22 bits)
	* 5.1.2 Temporal Reference (TR) (8 bits)
	* 5.1.3 Type Information (PTYPE) (Variable Length)
	*	– Bit 1: Always "1", in order to avoid start code emulation.
	*	– Bit 2: Always "0", for distinction with Recommendation H.261.
	
	*	– Bit 3: Split screen indicator, "0" off, "1" on.
	*	– Bit 4: Document camera indicator, "0" off, "1" on.
	*	– Bit 5: Full Picture Freeze Release, "0" off, "1" on.
	*	– Bits 6-8: Source Format, "000" forbidden, "001" sub-QCIF, "010" QCIF, "011" CIF,
		"100" 4CIF, "101" 16CIF, "110" reserved, "111" extended PTYPE.
		If bits 6-8 are not equal to "111", which indicates an extended PTYPE (PLUSPTYPE), the following
		five bits are also present in PTYPE:
		– Bit 9: Picture Coding Type, "0" INTRA (I-picture), "1" INTER (P-picture).
		– Bit 10: Optional Unrestricted Motion Vector mode (see Annex D), "0" off, "1" on.
		– Bit 11: Optional Syntax-based Arithmetic Coding mode (see Annex E), "0" off, "1" on.
		– Bit 12: Optional Advanced Prediction mode (see Annex F), "0" off, "1" on.
		– Bit 13: Optional PB-frames mode (see Annex G), "0" normal I- or P-picture, "1" PB-frame.
	*/
	if(pdata[0] == 0x00 && pdata[1] == 0x00 && (pdata[2] & 0xfc)==0x80){ /* PSC */
		/* RFC 2190 -5.1 Mode A
			0                   1                   2                   3
			0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
			+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
			|F|P|SBIT |EBIT | SRC |I|U|S|A|R      |DBQ| TRB |    TR         |
			+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		
		SRC : 3 bits
		Source format, bit 6,7 and 8 in PTYPE defined by H.263 [4], specifies
		the resolution of the current picture.
		
		I:  1 bit.
		Picture coding type, bit 9 in PTYPE defined by H.263[4], "0" is
		intra-coded, "1" is inter-coded.
		*/
		
		// PDATA[4] ======> Bits 3-10 of PTYPE
		uint32_t rtp_hdr = 0;
		uint8_t format, pict_type;
		
		// Source Format = 4,5,6
		format = (pdata[4] & 0x3C)>>2;
		// Picture Coding Type = 7
		pict_type = (pdata[4] & 0x02)>>1;
		// RTP mode A header
		((uint8_t*)&rtp_hdr)[1] = (format <<5) | (pict_type << 4);
		//rtp_hdr = tnet_htonl(rtp_hdr);
		memcpy(self->rtp.ptr, &rtp_hdr, sizeof(rtp_hdr));
	}

	// Send data over the network
	if(TMEDIA_CODEC_VIDEO(self)->callback){
		TMEDIA_CODEC_VIDEO(self)->callback(TMEDIA_CODEC_VIDEO(self)->callback_data, self->rtp.ptr, (size + H263_HEADER_MODE_A_SIZE), (3003* (30/TMEDIA_CODEC_VIDEO(self)->out.fps)), marker);
	}
}

static void tdav_codec_h263p_rtp_callback(tdav_codec_h263_t *self, const void *data, tsk_size_t size, tsk_bool_t frag, tsk_bool_t marker)
{
	uint8_t* pdata = (uint8_t*)data;
	//uint8_t rtp_hdr[2] = {0x00, 0x00};
	//tsk_bool_t eos = tsk_false;

	const void* _ptr = tsk_null;
	tsk_size_t _size = 0;
	//static tsk_bool_t frag = tsk_false;
	//tsk_bool_t found_gob = tsk_false;

	/* RFC 4629 - 5.1. General H.263+ Payload Header
		The H.263+ payload header is structured as follows:
         0                   1
         0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        |   RR    |P|V|   PLEN    |PEBIT|
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	*/

	/* http://eu.sabotage.org/www/ITU/H/H0263e.pdf
	*
	* 5.1.1 Picture Start Code (PSC) (22 bits)
	*	->PSC is a word of 22 bits. Its value is 0000 0000 0000 0000 1 00000.
	* 5.1.27 End Of Sequence (EOS) (22 bits)
	*	->A codeword of 22 bits. Its value is 0000 0000 0000 0000 1 11111
	* 5.2.2 Group of Block Start Code (GBSC) (17 bits)
	*	->A word of 17 bits. Its value is 0000 0000 0000 0000 1
	* C.4.1 End Of Sub-Bitstream code (EOSBS) (23 bits)
	*	->The EOSBS code is a codeword of 23 bits. Its value is 0000 0000 0000 0000 1 11110 0
	*
	*
	* 5.2.3 Group Number (GN) (5 bits)
	*	-> last 5 bits
	*/
	//if(pdata[0] == 0x00 && pdata[1] == 0x00 && pdata[2] >= 0x80){ /* PSC or EOS or GBSC */
	//	uint8_t GN = ((pdata[2]>>2) & 0x1F);
	//	found_gob = tsk_true;
	//	//TSK_DEBUG_INFO("GN=%u", pdata[2]);
	//
	//	/*	RFC 4629 - 6.1.1. Packets that begin with a Picture Start Code
	//		A packet that begins at the location of a Picture, GOB, slice, EOS,
	//		or EOSBS start code shall omit the first two (all zero) bytes from
	//		the H.263+ bitstream and signify their presence by setting P=1 in the
	//		payload header.
	//	*/

	//	if(GN == 0x00){ /* PSC 00000 */
	//		/* Use the two first bytes as RTP header */
	//		//pdata[0] |= 0x04; // P=1

	//		/*
	//		 0                   1                   2                   3
	//		0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	//		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	//		|   RR    |1|V|0|0|0|0|0|0|0|0|0| bitstream data without the    :
	//		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	//		: first two 0 bytes of the PSC
	//		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	//		*/

	//		//TSK_DEBUG_INFO("H263 - PSC");
	//	}
	//	else if(GN == 0x1F){ /* EOS 11111 */
	//		/* Use the two first bytes as RTP header */
	//		//pdata[0] |= 0x04; // P=1
	//		eos = tsk_true;
	//		/* RFC 4629 - 6.1.3. Packets that begin with an EOS or EOSBS Code 
	//			0                   1                   2
	//			0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3
	//			+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	//			|   RR    |1|V|0|0|0|0|0|0|0|0|0|1|1|1|1|1|1|0|0|
	//			+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	//		*/
	//		//TSK_DEBUG_INFO("H263 - EOS");
	//	}
	//	else /*if((GN >> 4) == 0x01)*/{ /* GBSC  10000 */
	//		/* Use the two first bytes as RTP header */
	//		//pdata[0] |= 0x04; // P=1
	//		
	//		/* RFC 4629 - 6.1.2. Packets that begin with GBSC or SSC
	//	0                   1                   2                   3
	//	0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	//	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	//	|   RR    |1|V|0 0 1 0 0 1|PEBIT|1 0 0 0 0 0| picture header    :
	//	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	//	: starting with TR, PTYPE ...                                   |
	//	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	//	| ...                                           | bitstream     :
	//	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	//	: data starting with GBSC/SSC without its first two 0 bytes
	//	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	//		*/
	//		//TSK_DEBUG_INFO("H263 - GBSC");
	//		found_gob = tsk_false;
	//	}
	//	//else if(EOSBS) -> Not Supported
	//}
	//else{
	//	/* 6.2. Encapsulating Follow-on Packet (P=0) */
	//	int i = 0;
	//	i++;
	//}

	//if(/*eos*/!found_gob && frag){
	//	if(self->rtp.size < (size + 2/* H263+ Header size */)){
	//		if(!(self->rtp.ptr = tsk_realloc(self->rtp.ptr, (size + 2)))){
	//			TSK_DEBUG_ERROR("Failed to allocate new buffer");
	//			return;
	//		}
	//		self->rtp.size = (size + 2);
	//	}
	//	/* RFC 4629 - 6. Packetization Schemes */
	//	//rtp_hdr[0] |= 0x00;
	//	//memcpy(self->rtp.ptr, rtp_hdr/* zeros-> is it corretc? */, 2);
	//	//memcpy((self->rtp.ptr + 2), pdata, size);
	//	//_ptr = self->rtp.ptr;
	//	//_size = (size + 2);

	//	pdata[0] |= pdata[2] > 0x80 ? 0x04 : 0x04;
	//	_ptr = pdata;
	//	_size = size;
	//}
	//else{
	//	pdata[0] |= pdata[2] > 0x80 ? 0x04 : 0x04;
	//	_ptr = pdata;
	//	_size = size;
	//}

// FIXME
	pdata[0] |= pdata[2] > 0x80 ? 0x04 : 0x04;
	_ptr = pdata;
	_size = size;
	

	// Send data over the network
	if(TMEDIA_CODEC_VIDEO(self)->callback){
		TMEDIA_CODEC_VIDEO(self)->callback(TMEDIA_CODEC_VIDEO(self)->callback_data, _ptr, _size, (3003* (30/TMEDIA_CODEC_VIDEO(self)->out.fps)), marker);
	}
}



#endif /* HAVE_FFMPEG */