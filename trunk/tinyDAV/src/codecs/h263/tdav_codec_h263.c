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

/**@file tdav_codec_h263.c
 * @brief H.263-1996 and H.263-1998 codec plugins.
 * RTP payloader follows RFC 4629 for H263+ and RFC 2190 for H263.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinydav/codecs/h263/tdav_codec_h263.h"

#if HAVE_FFMPEG

#include "tinydav/video/tdav_converter_video.h"

#include "tsk_time.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#define RTP_PAYLOAD_SIZE	500
#define MODE_A_SIZE			4 /* RFC 2190 section 5.1 */

static void *run(void* self);
static void tdav_codec_h263_rtp_callback(tdav_codec_h263_t *self, const void *data, tsk_size_t size, tsk_bool_t marker);
static void tdav_codec_h263p_rtp_callback(tdav_codec_h263_t *self, const void *data, tsk_size_t size, tsk_bool_t marker);

/* ============ Common To all H263 codecs ================= */

//int tdav_codec_h263_encap(tdav_codec_h263_t* self, tsk_buffer_t** buffer)
//{
//	/* 4 is sizeof(uint32_t) */
//	const uint8_t* pdata;
//	tsk_size_t size;
//	uint32_t i, last_index = 0, last_psc_gbsc_index = 0;
//
//	if(!buffer || !*buffer || ((*buffer)->size < 4)){
//		TSK_DEBUG_ERROR("Invalid parameter");
//		return -1;
//	}
//
//	pdata = (*buffer)->data;
//	size = (*buffer)->size;
//
//	for(i = 4; i<(size - 4); i++){
//		if((*((uint32_t*)(pdata + i)) & 0xffff8000) == 0x00008000){  /* PSC or (GBSC) found */
//		
//			if((i - last_index) >= RTP_PAYLOAD_SIZE){
//				list_psc_gbsc.push_back(last_psc_gbsc_index?last_psc_gbsc_index:i);
//				last_index = i;
//			}
//			last_psc_gbsc_index = i;
//		}
//	}
//	if(last_index < size - 3/*PSC/GBSC size*/){
//		list_psc_gbsc.push_back(size);
//	}
//
//	TSK_RUNNABLE_ENQUEUE_OBJECT(self->runnable, *buffer);
//
//	//*buffer = tsk_null;
//
//	return 0;
//}

int tdav_codec_h263_init(tdav_codec_h263_t* self, tdav_codec_h263_type_t type, enum CodecID encoder, enum CodecID decoder)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	
	
	self->type = type;

	if(!(self->encoder.codec = avcodec_find_encoder(encoder))){
		TSK_DEBUG_ERROR("Failed to find [%d]encoder", encoder);
	}

	if(!(self->decoder.codec = avcodec_find_decoder(decoder))){
		TSK_DEBUG_ERROR("Failed to find [%d] decoder", decoder);
	}

	/* allocations must be done by open() */
	return 0;
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

	TSK_OBJECT_SAFE_FREE(self->runnable);

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
	//	Runnable
	//
	h263->runnable = tdav_runnable_video_create(run, h263);
	if((ret = tdav_runnable_video_start(h263->runnable))){
		TSK_DEBUG_ERROR("Failed to create runnable (H263 codec)");
		return ret;
	}

	//
	//	Encoder
	//
	h263->encoder.context = avcodec_alloc_context();
	avcodec_get_context_defaults(h263->encoder.context);
	
	h263->encoder.context->pix_fmt		= PIX_FMT_YUV420P;
	h263->encoder.context->time_base.num  = 1;
	h263->encoder.context->time_base.den  = TMEDIA_CODEC_VIDEO(h263)->fps;
	h263->encoder.context->width = TMEDIA_CODEC_VIDEO(h263)->width;
	h263->encoder.context->height = TMEDIA_CODEC_VIDEO(h263)->height;
	//h263->encoder.context->max_b_frames = 0;

	h263->encoder.context->thread_count = 1;
	h263->encoder.context->rtp_payload_size = RTP_PAYLOAD_SIZE;
	h263->encoder.context->opaque = tsk_null;
	h263->encoder.context->bit_rate = (float) (120000) * 0.8;
	h263->encoder.context->gop_size = TMEDIA_CODEC_VIDEO(h263)->fps*5; /* each 5 seconds */
	
	// Picture (YUV 420)
	if(!(h263->encoder.picture = avcodec_alloc_frame())){
		TSK_DEBUG_ERROR("Failed to create encoder picture");
		return -2;
	}
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
	//h263->decoder.context = avcodec_alloc_context();

	return 0;
}

int tdav_codec_h263_close(tmedia_codec_t* self)
{
	int ret;

	tdav_codec_h263_t* h263 = (tdav_codec_h263_t*)self;

	if(!h263){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	/* the caller (base class) already checked that the codec is opened */

	//
	// Runnable
	//
	if((ret = tdav_runnable_video_stop(h263->runnable))){
		TSK_DEBUG_ERROR("Failed to stop runnable (H263 codec)");
		// ... do not exit, continue
	}

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

	return 0;
}

tsk_size_t tdav_codec_h263_encode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data)
{
	int ret;
	int size;

	tdav_codec_h263_t* h263 = (tdav_codec_h263_t*)self;

	if(!self || !in_data || ! in_size || !out_data){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}
	
	// delete old buffer
	if(*out_data){
		TSK_FREE(*out_data);
	}

	// wrap yuv420 buffer
	size = avpicture_fill((AVPicture *)h263->encoder.picture, (uint8_t*)in_data, PIX_FMT_YUV420P, h263->encoder.context->width, h263->encoder.context->height);
	if(size != in_size){
		/* guard */
		TSK_DEBUG_ERROR("Invalid size");
		return 0;
	}
	/* Flip */
	tdav_converter_video_flip(h263->encoder.picture, h263->encoder.context->height);

	// Encode data
	h263->encoder.picture->pts = AV_NOPTS_VALUE;
	ret = avcodec_encode_video(h263->encoder.context, h263->encoder.buffer, size, h263->encoder.picture);
	if(ret < 0){
		ret = 0;
	}
	else{
		if((*out_data = tsk_calloc(ret, sizeof(uint8_t)))){
			memcpy(*out_data, h263->encoder.buffer, ret);
		}
		else{
			TSK_DEBUG_ERROR("Failed to allocate output buffer");
			ret = 0;
		}
	}
	
	if(ret/* > RTP_PAYLOAD_SIZE*/){
		tsk_buffer_t* buffer = tsk_buffer_create_null();
		tsk_buffer_takeownership(buffer, out_data, (tsk_size_t)ret);
		TSK_RUNNABLE_ENQUEUE_OBJECT(h263->runnable, buffer);
		ret = 0;
	}

	return (tsk_size_t)ret;
}

tsk_size_t tdav_codec_h263_decode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data)
{
	return 0;
}

tsk_bool_t tdav_codec_h263_fmtp_match(const tmedia_codec_t* codec, const char* fmtp)
{	
	/* check whether we can match this fmtp with our local
	* check size, maxbr, fps ...*/
	return tsk_true;
}

char* tdav_codec_h263_fmtp_get(const tmedia_codec_t* self)
{
	return tsk_strdup("CIF=2/MaxBR=3840;QCIF=2/MaxBR=1920");
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
	return 0;
}

int tdav_codec_h263p_close(tmedia_codec_t* self)
{
	return 0;
}

tsk_size_t tdav_codec_h263p_encode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data)
{
	return 0;
}

tsk_size_t tdav_codec_h263p_decode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data)
{
	return 0;
}

tsk_bool_t tdav_codec_h263p_fmtp_match(const tmedia_codec_t* codec, const char* fmtp)
{	
	/* check whether we can match this fmtp with our local
	* check size, maxbr, fps ...*/
	return tsk_true;
}

char* tdav_codec_h263p_fmtp_get(const tmedia_codec_t* self)
{
	return tsk_strdup("CIF=2/MaxBR=3840;QCIF=2/MaxBR=1920");
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
	return 0;
}

int tdav_codec_h263pp_close(tmedia_codec_t* self)
{
	return 0;
}

tsk_size_t tdav_codec_h263pp_encode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data)
{
	return 0;
}

tsk_size_t tdav_codec_h263pp_decode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data)
{
	return 0;
}

tsk_bool_t tdav_codec_h263pp_fmtp_match(const tmedia_codec_t* codec, const char* fmtp)
{	
	/* check whether we can match this fmtp with our local
	* check size, maxbr, fps ...*/
	return tsk_true;
}

char* tdav_codec_h263pp_fmtp_get(const tmedia_codec_t* self)
{
	return tsk_strdup("CIF=2/MaxBR=3840;QCIF=2/MaxBR=1920");
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

static void *run(void* self)
{
	uint32_t i, last_index, last_psc_gbsc_index;
	tsk_list_item_t *curr;

	const uint8_t* pdata;
	tsk_size_t size;

	const tdav_codec_h263_t* h263 = ((tdav_runnable_video_t*)self)->userdata;

	TSK_DEBUG_INFO("H263 thread === START");

	TSK_RUNNABLE_RUN_BEGIN(self);
	
	if((curr = TSK_RUNNABLE_POP_FIRST(self))){
		/* 4 is sizeof(uint32_t) */
		pdata = ((const tsk_buffer_t*)curr->data)->data;
		size = ((const tsk_buffer_t*)curr->data)->size;
		last_index = 0, last_psc_gbsc_index = 0;

		if(size < RTP_PAYLOAD_SIZE){
			goto last;
		}

		for(i = 4; i<(size - 4); i++){
//#if ANDROID
			if(pdata[i] == 0x00 && pdata[i+1] == 0x00 && pdata[i+2]>=0x80){  /* PSC or (GBSC) found */
//#else
//			if((*((uint32_t*)(pdata + i)) & 0xffff8000) == 0x00008000){  /* PSC or (GBSC) found */
//#endif
				if((i - last_index) >= RTP_PAYLOAD_SIZE){
					switch(h263->type){
						case tdav_codec_h263_1996:
							tdav_codec_h263_rtp_callback((tdav_codec_h263_t*) h263, pdata+last_index/*pdata + (last_psc_gbsc_index ? last_psc_gbsc_index : i)*/,
								(i - last_index), (last_index == size));
							break;
						default:
							tdav_codec_h263p_rtp_callback((tdav_codec_h263_t*) h263, pdata + (last_psc_gbsc_index ? last_psc_gbsc_index : i),
								(i - last_index), (last_index == size));
							break;
					}
					last_index = i;
				}
				last_psc_gbsc_index = i;
			}
		}
last:
		if(last_index < size - 3/*PSC/GBSC size*/){
			switch(h263->type){
				case tdav_codec_h263_1996:
					tdav_codec_h263_rtp_callback((tdav_codec_h263_t*) h263, pdata + last_index,
						(size - last_index), tsk_true);
					break;
				default:
					tdav_codec_h263p_rtp_callback((tdav_codec_h263_t*) h263, pdata + last_index,
						(size - last_index), tsk_true);
					break;
			}
		}

		tsk_object_unref(curr);
	}
	
	TSK_RUNNABLE_RUN_END(self);

	TSK_DEBUG_INFO("H263 thread === STOP");

	return 0;
}

static void tdav_codec_h263_rtp_callback(tdav_codec_h263_t *self, const void *data, tsk_size_t size, tsk_bool_t marker)
{
	uint8_t* rtp; // FIXXXXXXXXXME: use one global buffer

	if(!(rtp = tsk_calloc((size + MODE_A_SIZE), sizeof(uint8_t)))){
		TSK_DEBUG_ERROR("Failed to allocate new buffer");
		return;
	}
	else{
		memcpy((rtp + MODE_A_SIZE), data, size);
	}

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
	if((*((uint32_t*)data) & 0xffff8000) == 0x00008000){ /* PSC */
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
		uint8_t format;
		const uint8_t* ptr = (((const uint8_t*)data) + 4); // Bits 3-10 of PTYPE

		//for(i=0;i<10;i++){
		//	printf("%2x ", ((const uint8_t*)data)[i]);
		//}

		// Format = 4,5,6
		format = ((*ptr) & 0x38)>>3;
		//int i=0;
		//i++;
	}

	//*(rtp + 1)=0x50;

	// Send data over the network
	if(TMEDIA_CODEC_VIDEO(self)->callback){
		TMEDIA_CODEC_VIDEO(self)->callback(TMEDIA_CODEC_VIDEO(self)->callback_data, rtp, (size + MODE_A_SIZE), (3003* (30/TMEDIA_CODEC_VIDEO(self)->fps)), marker);
	}

	TSK_FREE(rtp);
}

static void tdav_codec_h263p_rtp_callback(tdav_codec_h263_t *self, const void *data, tsk_size_t size, tsk_bool_t marker)
{
// Send data over the network
		//if(TMEDIA_CODEC_VIDEO(h263)->callback){
		//	TMEDIA_CODEC_VIDEO(h263)->callback(TMEDIA_CODEC_VIDEO(h263)->callback_data, buffer->data, buffer->size, 160, marker);
		//}
}



#endif /* HAVE_FFMPEG */