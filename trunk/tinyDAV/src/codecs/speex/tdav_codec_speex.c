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

/**@file tdav_codec_speex.c
 * @brief Speex codecs
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinydav/codecs/speex/tdav_codec_speex.h"

#if HAVE_LIB_SPEEX

#include "tsk_memory.h"
#include "tsk_debug.h"

/* ============ Common ================= */
int tdav_codec_speex_init(tdav_codec_speex_t* self, tdav_codec_speex_type_t type);
int tdav_codec_speex_deinit(tdav_codec_speex_t* self);

/* ============ iLBC Plugin interface ================= */

#define tdav_codec_speex_fmtp_set tsk_null

int tdav_codec_speex_open(tmedia_codec_t* self)
{
	tdav_codec_speex_t* speex = (tdav_codec_speex_t*)self;
	tsk_size_t size = 0;
	
	switch(speex->type){
		case tdav_codec_speex_type_nb:
			speex->encoder.state = speex_encoder_init(&speex_nb_mode);
			speex->decoder.state = speex_decoder_init(&speex_nb_mode);

			speex_decoder_ctl(speex->decoder.state, SPEEX_GET_FRAME_SIZE, &speex->decoder.size);
			speex->decoder.size = (speex->decoder.size ? speex->decoder.size : 160) * sizeof(spx_int16_t);
			if(!(speex->decoder.buffer = tsk_calloc(speex->decoder.size, 1))){
				speex->decoder.size = 0;
				TSK_DEBUG_ERROR("Failed to allocate new buffer");
				return -3;
			}

			speex_encoder_ctl(speex->encoder.state, SPEEX_GET_FRAME_SIZE, &speex->encoder.size);
			if(!speex->encoder.size){
				speex->encoder.size = 20;
			}
			break;
		default:
			TSK_DEBUG_ERROR("Not implemented");
			return -2;
	}

	speex_bits_init(&speex->encoder.bits);
	speex_bits_init(&speex->decoder.bits);
	speex_bits_reset(&speex->encoder.bits);
	speex_bits_reset(&speex->decoder.bits);

	return 0;
}

int tdav_codec_speex_close(tmedia_codec_t* self)
{
	tdav_codec_speex_t* speex = (tdav_codec_speex_t*)self;

	

	return 0;
}

tsk_size_t tdav_codec_speex_encode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data)
{	
	tdav_codec_speex_t* speex = (tdav_codec_speex_t*)self;
	tsk_size_t outsize = 0;
	
	if(!self || !in_data || !in_size || !out_data){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}
	
	/* free old buffer */
	if(*out_data){
		TSK_FREE(*out_data);
	}
	
	speex_bits_reset(&speex->encoder.bits);
	speex_encode_int(speex->encoder.state, (spx_int16_t*)in_data, &speex->encoder.bits);

	if((*out_data = tsk_calloc(speex->encoder.size, 1))){
		outsize = speex_bits_write(&speex->encoder.bits, *out_data, speex->encoder.size);
	}

   return outsize;
}

tsk_size_t tdav_codec_speex_decode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, const tsk_object_t* proto_hdr)
{
	int ret;
	tsk_size_t out_size = 0;
	tdav_codec_speex_t* speex = (tdav_codec_speex_t*)self;

	if(!self || !in_data || !in_size || !out_data){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}
	
	if(*out_data){
		TSK_FREE(*out_data);
	}

	/* initializes the bit-stream */
	speex_bits_read_from(&speex->decoder.bits, (char*)in_data, in_size);

	do{
		/* performs decode() */
		if((ret = speex_decode_int(speex->decoder.state, &speex->decoder.bits, speex->decoder.buffer))){
			TSK_DEBUG_ERROR("Failed to decode the buffer. retcode=%d", ret);
			break;
		}
		/* copy output buffer */
		if((*out_data = tsk_realloc(*out_data, out_size + (speex->decoder.size)))){
			memcpy(&((uint8_t*)*out_data)[out_size], speex->decoder.buffer, speex->decoder.size);
			out_size += speex->decoder.size;
		}
	}
	while(speex_bits_remaining(&speex->decoder.bits) >= 5);
	

	return out_size;
}

char* tdav_codec_speex_fmtp_get(const tmedia_codec_t* codec)
{
	return tsk_null;
}

tsk_bool_t tdav_codec_speex_fmtp_match(const tmedia_codec_t* codec, const char* fmtp)
{	
	return tsk_true;
}


//
//	Speex Narrow Band Plugin definition
//

/* constructor */
static tsk_object_t* tdav_codec_speex_nb_ctor(tsk_object_t * self, va_list * app)
{
	tdav_codec_speex_t *speex = self;
	if(speex){
		/* init base: called by tmedia_codec_create() */
		/* init self */
		tdav_codec_speex_init(speex, tdav_codec_speex_type_nb);
	}
	return self;
}
/* destructor */
static tsk_object_t* tdav_codec_speex_nb_dtor(tsk_object_t * self)
{ 
	tdav_codec_speex_t *speex = self;
	if(speex){
		/* deinit base */
		tmedia_codec_audio_deinit(speex);
		/* deinit self */
		tdav_codec_speex_deinit(speex);
	}

	return self;
}
/* object definition */
static const tsk_object_def_t tdav_codec_speex_nb_def_s = 
{
	sizeof(tdav_codec_speex_t),
	tdav_codec_speex_nb_ctor, 
	tdav_codec_speex_nb_dtor,
	tmedia_codec_cmp, 
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t tdav_codec_speex_nb_plugin_def_s = 
{
	&tdav_codec_speex_nb_def_s,

	tmedia_audio,
	"SPEEX",
	"Speex Narrow Band Codec",
	TMEDIA_CODEC_FORMAT_SPEEX_NB,
	tsk_true,
	8000, // rate
	
	{ /* audio */
		1, // channels
		20 // ptime
	},

	/* video */
	{0},

	tdav_codec_speex_open,
	tdav_codec_speex_close,
	tdav_codec_speex_encode,
	tdav_codec_speex_decode,
	tdav_codec_speex_fmtp_match,
	tdav_codec_speex_fmtp_get,
	tdav_codec_speex_fmtp_set
};
const tmedia_codec_plugin_def_t *tdav_codec_speex_nb_plugin_def_t = &tdav_codec_speex_nb_plugin_def_s;


//
// Common functions
//
int tdav_codec_speex_init(tdav_codec_speex_t* self, tdav_codec_speex_type_t type)
{
	if(self){
		self->type = type;
		return 0;
	}
	else{
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
}

int tdav_codec_speex_deinit(tdav_codec_speex_t* self)
{
	if(self){
		if(self->decoder.state){
			speex_decoder_destroy(self->decoder.state);
			self->decoder.state = tsk_null;
		}
		speex_bits_destroy(&self->decoder.bits);
		if(self->decoder.buffer){
			TSK_FREE(self->decoder.buffer);
			self->decoder.size = 0;
		}

		if(self->encoder.state){
			speex_encoder_destroy(self->encoder.state);
			self->encoder.state = tsk_null;
		}
		speex_bits_destroy(&self->encoder.bits);
		self->encoder.size = 0;
		
		return 0;
	}
	else{
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
}

#endif /* HAVE_LIB_SPEEX */
