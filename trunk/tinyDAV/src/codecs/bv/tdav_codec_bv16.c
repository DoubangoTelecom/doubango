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

/**@file tdav_codec_bv16.c
 * @brief BroadVoice16 codec
 * The payloader/depayloader follow RFC 4298
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinydav/codecs/bv/tdav_codec_bv16.h"

#if HAVE_BV16

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

/* ============ BV16 Plugin interface ================= */

#define tdav_codec_bv16_fmtp_get tsk_null
#define tdav_codec_bv16_fmtp_set tsk_null

int tdav_codec_bv16_open(tmedia_codec_t* self)
{
	tdav_codec_bv16_t* bv16 = (tdav_codec_bv16_t*)self;

	if(!bv16->encoder.state){
		bv16->encoder.state = allocWord16(0, sizeof(struct BV16_Encoder_State)/2-1);
		Reset_BV16_Encoder((struct BV16_Encoder_State*)bv16->encoder.state);
	}

	if(!bv16->decoder.state){
		bv16->decoder.state = allocWord16(0, sizeof(struct BV16_Decoder_State)/2-1);
		Reset_BV16_Decoder((struct BV16_Decoder_State*)bv16->decoder.state);
	}

	return 0;
}

int tdav_codec_bv16_close(tmedia_codec_t* self)
{
	tdav_codec_bv16_t* bv16 = (tdav_codec_bv16_t*)self;
	

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
	//tsk_size_t out_size;
	//int ret;
	tdav_codec_bv16_t* bv16 = (tdav_codec_bv16_t*)self;

	if(!self || !in_data || !in_size || !out_data || (in_size % TDAV_BV16_FRAME_SIZE)){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}
	
	return 0;
}

tsk_bool_t tdav_codec_bv16_fmtp_match(const tmedia_codec_t* codec, const char* fmtp)
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
	"BV16",
	"BroadVoice16 Rate",
	TMEDIA_CODEC_FORMAT_BV16,
	tsk_true,
	8000, // rate
	
	{ /* audio */
		1, // channels
		20 // ptime
	},

	/* video */
	{0},

	tdav_codec_bv16_open,
	tdav_codec_bv16_close,
	tdav_codec_bv16_encode,
	tdav_codec_bv16_decode,
	tdav_codec_bv16_fmtp_match,
	tdav_codec_bv16_fmtp_get,
	tdav_codec_bv16_fmtp_set
};
const tmedia_codec_plugin_def_t *tdav_codec_bv16_plugin_def_t = &tdav_codec_bv16_plugin_def_s;


#endif /* HAVE_BV16 */
