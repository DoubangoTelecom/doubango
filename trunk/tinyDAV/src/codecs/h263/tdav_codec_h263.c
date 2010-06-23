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
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinydav/codecs/h263/tdav_codec_h263.h"

#include "tsk_debug.h"

static void tdav_codec_h263_rtp_callback(struct AVCodecContext *avctx, void *data, int size, int mb_nb);
static void tdav_codec_h263p_rtp_callback(struct AVCodecContext *avctx, void *data, int size, int mb_nb);

/* ============ Common To all H263 codecs ================= */

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

	return 0;
}



/* ============ H.263-1996 Plugin interface ================= */

//
//	H.263-1996 object definition
//
int tdav_codec_h263_open(tmedia_codec_t* self)
{
	int ret;
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
	h263->encoder.context->pix_fmt		= PIX_FMT_YUV420P;
	h263->encoder.context->time_base.num  = 1;
	h263->encoder.context->time_base.den  = TMEDIA_CODEC_VIDEO(h263)->fps;
	h263->encoder.context->width = TMEDIA_CODEC_VIDEO(h263)->width;
	h263->encoder.context->height = TMEDIA_CODEC_VIDEO(h263)->height;
	h263->encoder.context->max_b_frames = 0;
	
	h263->encoder.context->thread_count = 1;
	h263->encoder.context->rtp_payload_size = 800; /* to be configured */
	h263->encoder.context->opaque = h263;
	h263->encoder.context->gop_size = TMEDIA_CODEC_VIDEO(h263)->fps*1; /* each 1 second */
	
	switch(h263->type){
		case tdav_codec_h263_1996:
			{	/* H263 - 1996 */
				h263->encoder.context->rtp_callback = tdav_codec_h263_rtp_callback;
				break;
			}
		case tdav_codec_h263_1998:
			{	/* H263 - 1998 */
				h263->encoder.context->rtp_callback = tdav_codec_h263p_rtp_callback;
				h263->encoder.context->flags |= CODEC_FLAG_H263P_UMV;		/* Annex D+ */
				h263->encoder.context->flags |= CODEC_FLAG_AC_PRED;			/* Annex I and T */
				h263->encoder.context->flags |= CODEC_FLAG_LOOP_FILTER;		/* Annex J */
				h263->encoder.context->flags |= CODEC_FLAG_H263P_SLICE_STRUCT;	/* Annex K */
				h263->encoder.context->flags |= CODEC_FLAG_H263P_AIV;			/* Annex S */
				break;
			}
		case tdav_codec_h263_2000:
			{	/* H263 - 2000 */
				h263->encoder.context->rtp_callback = tdav_codec_h263p_rtp_callback;
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

	//
	//	Decoder
	//
	if(h263->decoder.context){
		avcodec_close(h263->decoder.context);
		av_free(h263->decoder.context);
	}

	return 0;
}

tsk_size_t tdav_codec_h263_fmtp_encode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data)
{
	return 0;
}

tsk_size_t tdav_codec_h263_fmtp_decode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data)
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
	tdav_codec_h263_fmtp_encode,
	tdav_codec_h263_fmtp_decode,
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

tsk_size_t tdav_codec_h263p_fmtp_encode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data)
{
	return 0;
}

tsk_size_t tdav_codec_h263p_fmtp_decode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data)
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
	tdav_codec_h263p_fmtp_encode,
	tdav_codec_h263p_fmtp_decode,
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

tsk_size_t tdav_codec_h263pp_fmtp_encode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data)
{
	return 0;
}

tsk_size_t tdav_codec_h263pp_fmtp_decode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data)
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
	tdav_codec_h263pp_fmtp_encode,
	tdav_codec_h263pp_fmtp_decode,
	tdav_codec_h263pp_fmtp_match,
	tdav_codec_h263pp_fmtp_get,
	tdav_codec_h263pp_fmtp_set
};
const tmedia_codec_plugin_def_t *tdav_codec_h263pp_plugin_def_t = &tdav_codec_h263pp_plugin_def_s;



/* ============ Callbacks ================= */

static void tdav_codec_h263_rtp_callback(struct AVCodecContext *avctx, void *data, int size, int mb_nb)
{

}

static void tdav_codec_h263p_rtp_callback(struct AVCodecContext *avctx, void *data, int size, int mb_nb)
{

}