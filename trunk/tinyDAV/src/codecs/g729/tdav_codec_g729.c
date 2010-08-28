/*
* Copyright (C) 2009-2010 Mamadou Diop.
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

/**@file tdav_codec_g729.c
 * @brief G729a codec.
 * Source from: http://www.itu.int/rec/T-REC-G.729-199611-S!AnnA/en
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinydav/codecs/g729/tdav_codec_g729.h"

#if HAVE_G729

#include "tsk_memory.h"
#include "tsk_debug.h"


#if defined(_MSC_VER)
#	pragma comment(lib, "../thirdparties/win32/lib/g729a/g729a.a")
#endif

#define L10_SIN_FRAME	L_FRAME
#define L10_BIN_FRAME	(L10_SIN_FRAME * 2)
#define L20_SIN_FRAME	(L10_SIN_FRAME * 2)

#define L10_BOUT_FRAME	10
#define L10_SOUT_FRAME	20
#define L20_BOUT_FRAME	20
#define L20_SOUT_FRAME	10

#if G729_HACK
#	define Pre_Process	__Pre_Process
#	define Set_zero		__Set_zero
#endif

/* ============ GSM Plugin interface ================= */

#define tdav_codec_g729a_fmtp_set tsk_null

int tdav_codec_g729a_open(tmedia_codec_t* self)
{
	tdav_codec_g729a_t* g729a = (tdav_codec_g729a_t*)self;
	
	// Initialize the encoder
	// FIXME: add a context state
	Init_Pre_Process();
	Init_Coder_ld8a();
	Set_zero(g729a->encoder.prm, PRM_SIZE);
	

	return 0;
}

int tdav_codec_g729a_close(tmedia_codec_t* self)
{
	tdav_codec_g729a_t* g729a = (tdav_codec_g729a_t*)self;

	

	return 0;
}
   

tsk_size_t tdav_codec_g729a_encode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size)
{	
	tsk_size_t out_size;
	//int i;
	tdav_codec_g729a_t* g729a = (tdav_codec_g729a_t*)self;
	int fcount = (in_size / L10_BIN_FRAME);
	Word16 serial[SERIAL_SIZE];
	

	if(!self || !in_data || !in_size || !out_data || (in_size % L10_SIN_FRAME)){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}

	out_size = 2*sizeof(serial);
	if(*out_max_size <out_size){
		if(!(*out_data = tsk_realloc(*out_data, out_size))){
			TSK_DEBUG_ERROR("Failed to allocate new buffer");
			*out_max_size = 0;
			return 0;
		}
		*out_max_size = out_size;
	}	

	out_size = 20;
	if(*out_max_size <out_size){
		if(!(*out_data = tsk_realloc(*out_data, out_size))){
			TSK_DEBUG_ERROR("Failed to allocate new buffer");
			*out_max_size = 0;
			return 0;
		}
		*out_max_size = out_size;
	}    
	
	/*out_size = ((in_size / (TMEDIA_CODEC_PCM_FRAME_SIZE(self) * sizeof(Word16))) * L20_BOUT_FRAME);

	// allocate new buffer if needed
	if(*out_max_size <out_size){
		if(!(*out_data = tsk_realloc(*out_data, out_size))){
			TSK_DEBUG_ERROR("Failed to allocate new buffer");
			*out_max_size = 0;
			return 0;
		}
		*out_max_size = out_size;
	}

	for(i=0; i<fcount; i++){
		Pre_Process(&((Word16*)in_data)[i*L10_SIN_FRAME], L10_SIN_FRAME);
		//Pre_Process(test, L10_SIN_FRAME);
		Coder_ld8a(g729a->encoder.prm);
		prm2bits_ld8k(g729a->encoder.prm, &((Word16*)*out_data)[i*5]);		
	}*/

	return out_size;
}

tsk_size_t tdav_codec_g729a_decode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size, const tsk_object_t* proto_hdr)
{
	tsk_size_t out_size = 0;
//	int ret;
	tdav_codec_g729a_t* g729a = (tdav_codec_g729a_t*)self;

	if(!self || !in_data || !in_size || !out_data /*|| (in_size % TDAV_g729a_FRAME_SIZE)*/){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}
	
	

	return out_size;
}

tsk_bool_t tdav_codec_g729a_fmtp_match(const tmedia_codec_t* codec, const char* fmtp)
{	/* always match */
	return tsk_true;
}

char* tdav_codec_g729a_fmtp_get(const tmedia_codec_t* codec)
{
	return tsk_strdup("annexb=no");
}

//
//	g729a Plugin definition
//

/* constructor */
static tsk_object_t* tdav_codec_g729a_ctor(tsk_object_t * self, va_list * app)
{
	tdav_codec_g729a_t *g729a = self;
	if(g729a){
		/* init base: called by tmedia_codec_create() */
		/* init self */
	}
	return self;
}
/* destructor */
static tsk_object_t* tdav_codec_g729a_dtor(tsk_object_t * self)
{ 
	tdav_codec_g729a_t *g729a = self;
	if(g729a){
		/* deinit base */
		tmedia_codec_audio_deinit(g729a);
		/* deinit self */
		
	}

	return self;
}
/* object definition */
static const tsk_object_def_t tdav_codec_g729a_def_s = 
{
	sizeof(tdav_codec_g729a_t),
	tdav_codec_g729a_ctor, 
	tdav_codec_g729a_dtor,
	tmedia_codec_cmp, 
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t tdav_codec_g729a_plugin_def_s = 
{
	&tdav_codec_g729a_def_s,

	tmedia_audio,
	"g729",
	"g729a Codec",
	TMEDIA_CODEC_FORMAT_G729,
	tsk_false,
	8000, // rate
	
	{ /* audio */
		1, // channels
		20 // ptime
	},

	/* video */
	{0},

	tdav_codec_g729a_open,
	tdav_codec_g729a_close,
	tdav_codec_g729a_encode,
	tdav_codec_g729a_decode,
	tdav_codec_g729a_fmtp_match,
	tdav_codec_g729a_fmtp_get,
	tdav_codec_g729a_fmtp_set
};
const tmedia_codec_plugin_def_t *tdav_codec_g729a_plugin_def_t = &tdav_codec_g729a_plugin_def_s;

#endif /* HAVE_G729 */