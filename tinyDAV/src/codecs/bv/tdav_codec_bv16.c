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

/**@file tdav_codec_bv16.c
 * @brief BroadVoice16 codec
 * The payloader/depayloader follow RFC 4298
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#include "tinydav/codecs/bv/tdav_codec_bv16.h"

#if HAVE_BV16

#if defined(_MSC_VER)
#	pragma comment(lib, "..\\thirdparties\\win32\\lib\\BroadVoice16\\libbv16.a")
#endif

#include "tsk_memory.h"
#include "tsk_debug.h"

#include "typedef.h"
#include "bvcommon.h"
#include "bv16cnst.h"
#include "bv16strct.h"
#include "bv16.h"
#include "utility.h"
#if G192BITSTREAM
#include "g192.h"
#else
#include "bitpack.h"
#endif
#include "memutil.h" 

/* RFC 4298 - 3.1.  BroadVoice16 Bit Stream Definition */
#define TDAV_BV16_FRAME_SIZE			10
#define FRSZ_IN_U8						(FRSZ*2)	

/* ============ BV16 Plugin interface ================= */

#define tdav_codec_bv16_sdp_att_get tsk_null
#define tdav_codec_bv16_fmtp_set tsk_null

static int sizestate = sizeof(struct BV16_Encoder_State);
static int sizebitstream = sizeof(struct BV16_Bit_Stream);

int tdav_codec_bv16_open(tmedia_codec_t* self)
{
	tdav_codec_bv16_t* bv16 = (tdav_codec_bv16_t*)self;

	if(!bv16->encoder.state){
		bv16->encoder.state = allocWord16(0, sizestate/2-1);
		Reset_BV16_Encoder((struct BV16_Encoder_State*)bv16->encoder.state);
	}
	if(!bv16->encoder.bs){
		bv16->encoder.bs = allocWord16(0, sizebitstream/2-1);
	}
	if(!bv16->encoder.x){
		bv16->encoder.x = allocWord16(0, FRSZ-1);
	}

	if(!bv16->decoder.state){
		bv16->decoder.state = allocWord16(0, sizestate/2-1);
		Reset_BV16_Decoder((struct BV16_Decoder_State*)bv16->decoder.state);
	}
	if(!bv16->decoder.bs){
		bv16->decoder.bs = allocWord16(0, sizebitstream/2-1);
	}
	if(!bv16->decoder.x){
		bv16->decoder.x = allocWord16(0, FRSZ-1);
	}

	return 0;
}

int tdav_codec_bv16_close(tmedia_codec_t* self)
{
	tdav_codec_bv16_t* bv16 = (tdav_codec_bv16_t*)self;
	
	if(bv16->encoder.state){
		 deallocWord16(bv16->encoder.state, 0, sizestate/2-1);
		 bv16->encoder.state = tsk_null;
	}
	if(bv16->encoder.bs){
		deallocWord16(bv16->encoder.bs, 0, sizebitstream/2-1);
		bv16->encoder.bs = tsk_null;
	}
	if(bv16->encoder.x){
		deallocWord16(bv16->encoder.x, 0, FRSZ-1);
		bv16->encoder.x = tsk_null;
	}

	if(bv16->decoder.state){
		 deallocWord16(bv16->decoder.state, 0, sizestate/2-1);
		 bv16->decoder.state = tsk_null;
	}
	if(bv16->encoder.bs){
		deallocWord16(bv16->decoder.bs, 0, sizebitstream/2-1);
		bv16->decoder.bs = tsk_null;
	}
	if(bv16->decoder.x){
		deallocWord16(bv16->decoder.x, 0, FRSZ-1);
		bv16->decoder.x = tsk_null;
	}
	
     

	return 0;
}

tsk_size_t tdav_codec_bv16_encode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size)
{	
	//tsk_size_t out_size = 0;
	tdav_codec_bv16_t* bv16 = (tdav_codec_bv16_t*)self;

	if(!self || !in_data || !in_size || !out_data){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}

	return 0;
}

tsk_size_t tdav_codec_bv16_decode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size, const tsk_object_t* proto_hdr)
{
	tsk_size_t out_size = 0;
	int i;
	tdav_codec_bv16_t* bv16 = (tdav_codec_bv16_t*)self;
	uint8_t mama[600];

	if(!self || !in_data || !in_size || !out_data || (in_size % TDAV_BV16_FRAME_SIZE)){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}
	
	for(i=0; i<(int)in_size; i+=TDAV_BV16_FRAME_SIZE){
		BV16_BitUnPack(mama, (struct BV16_Bit_Stream*)bv16->decoder.bs);
		//BV16_BitUnPack(&((UWord8 *)in_data)[i], (struct BV16_Bit_Stream*)bv16->decoder.bs);
		BV16_Decode((struct BV16_Bit_Stream*)bv16->decoder.bs, (struct BV16_Decoder_State*)bv16->decoder.state, bv16->decoder.x);

		
		if(*out_max_size<(out_size + FRSZ_IN_U8)){
			if(!(*out_data = tsk_realloc(*out_data, (out_size + FRSZ_IN_U8)))){
				TSK_DEBUG_ERROR("Failed to allocate new buffer");
				*out_max_size = 0;
				return 0;
			}
			*out_max_size = (out_size + FRSZ_IN_U8);
		}
		memcpy(&((uint8_t*)* out_data)[out_size], bv16->decoder.x, FRSZ_IN_U8);
		out_size += FRSZ_IN_U8;
	}
	

	return out_size;
}

tsk_bool_t tdav_codec_bv16_sdp_att_match(const tmedia_codec_t* codec, const char* att_name, const char* att_value)
{	/* always match */
	return tsk_true;
}


//
//	BV16 Plugin definition
//

/* constructor */
static tsk_object_t* tdav_codec_bv16_ctor(tsk_object_t * self, va_list * app)
{
	tdav_codec_bv16_t *bv16 = self;
	if(bv16){
		/* init base: called by tmedia_codec_create() */
		/* init self */
	}
	return self;
}
/* destructor */
static tsk_object_t* tdav_codec_bv16_dtor(tsk_object_t * self)
{ 
	tdav_codec_bv16_t *bv16 = self;
	if(bv16){
		/* deinit base */
		tmedia_codec_audio_deinit(bv16);
		/* deinit self */
		
	}

	return self;
}
/* object definition */
static const tsk_object_def_t tdav_codec_bv16_def_s = 
{
	sizeof(tdav_codec_bv16_t),
	tdav_codec_bv16_ctor, 
	tdav_codec_bv16_dtor,
	tmedia_codec_cmp, 
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t tdav_codec_bv16_plugin_def_s = 
{
	&tdav_codec_bv16_def_s,

	tmedia_audio,
	tmedia_codec_id_bv16,
	"BV16",
	"BroadVoice16 Rate",
	TMEDIA_CODEC_FORMAT_BV16,
	tsk_true,
	8000, // rate
	
	{ /* audio */
		1, // channels
		0 // ptime @deprecated
	},

	/* video */
	{0},

	tdav_codec_bv16_open,
	tdav_codec_bv16_close,
	tdav_codec_bv16_encode,
	tdav_codec_bv16_decode,
	tdav_codec_bv16_sdp_att_match,
	tdav_codec_bv16_sdp_att_get,
	tdav_codec_bv16_fmtp_set
};
const tmedia_codec_plugin_def_t *tdav_codec_bv16_plugin_def_t = &tdav_codec_bv16_plugin_def_s;


#endif /* HAVE_BV16 */
