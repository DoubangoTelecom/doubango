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

/**@file tdav_codec_ilbc.c
 * @brief iLBC codec
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#include "tinydav/codecs/ilbc/tdav_codec_ilbc.h"

#if HAVE_ILBC

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#define TDAV_ILBC_MODE			20

/* ============ iLBC Plugin interface ================= */

#define tdav_codec_ilbc_set tsk_null

static int tdav_codec_ilbc_open(tmedia_codec_t* self)
{
	tdav_codec_ilbc_t* ilbc = (tdav_codec_ilbc_t*)self;
	
	initEncode(&ilbc->encoder, TDAV_ILBC_MODE);
    initDecode(&ilbc->decoder, TDAV_ILBC_MODE, tsk_true/* Enhancer */);

	return 0;
}

static int tdav_codec_ilbc_close(tmedia_codec_t* self)
{
	tdav_codec_ilbc_t* ilbc = (tdav_codec_ilbc_t*)self;

	//ilbc->encoder = {0};
	//ilbc->decoder = {0};

	return 0;
}

static tsk_size_t tdav_codec_ilbc_encode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size)
{	
	tdav_codec_ilbc_t* ilbc = (tdav_codec_ilbc_t*)self;
	int k;
	
	if(!self || !in_data || !in_size || !out_data){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}
	
   /* convert signal to float */
	for(k=0; k<ilbc->encoder.blockl; k++){
       ilbc->encblock[k] = (float)((short*)in_data)[k];
	}
	
	/* allocate new buffer if needed */
	if((int)*out_max_size <ilbc->encoder.no_of_bytes){
		if(!(*out_data = tsk_realloc(*out_data, ilbc->encoder.no_of_bytes))){
			TSK_DEBUG_ERROR("Failed to allocate new buffer");
			*out_max_size = 0;
			return 0;
		}
		*out_max_size = ilbc->encoder.no_of_bytes;
	}
	
	/* do the actual encoding */
    iLBC_encode(*out_data, ilbc->encblock, &ilbc->encoder);
	
	return ilbc->encoder.no_of_bytes;
}

static tsk_size_t tdav_codec_ilbc_decode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size, const tsk_object_t* proto_hdr)
{
	int blocks, i, k, block_size;
	float dtmp;
	tsk_size_t out_size;
	tdav_codec_ilbc_t* ilbc = (tdav_codec_ilbc_t*)self;

	if(!self || !in_data || !in_size || !out_data){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}

	if((in_size % NO_OF_BYTES_20MS) == 0){
		/* Using 20ms mode */
		blocks = (in_size/NO_OF_BYTES_20MS);
		out_size = (BLOCKL_20MS * blocks) * sizeof(short);
		block_size = out_size/blocks;
		if(ilbc->decoder.mode != 20){
			 initDecode(&ilbc->decoder, 20, tsk_true/* Enhancer */);
		}
	}
	else if((in_size % NO_OF_BYTES_30MS) == 0){
		/* Using 30ms mode */
		blocks = (in_size/NO_OF_BYTES_30MS);
		out_size = (BLOCKL_30MS * blocks) * sizeof(short);
		block_size = out_size/blocks;
		if(ilbc->decoder.mode != 30){
			 initDecode(&ilbc->decoder, 30, tsk_true/* Enhancer */);
		}
	}
	else{
		TSK_DEBUG_ERROR("Invalid iLBC mode");
		return 0;
	}

	/* allocate new buffer if needed */
	if(*out_max_size<out_size){
		if(!(*out_data = tsk_realloc(*out_data, out_size))){
			TSK_DEBUG_ERROR("Failed to allocate new buffer");
			*out_max_size = 0;
			return 0;
		}
		*out_max_size = out_size;
	}

	for(i = 0; i<blocks; i++){
		iLBC_decode(ilbc->decblock, &((uint8_t*)in_data)[i*block_size], &ilbc->decoder, 1/* Normal */);

       /* convert to short */
       for(k=0; k<ilbc->decoder.blockl; k++){
           dtmp=ilbc->decblock[k];

		   if(dtmp<MIN_SAMPLE){
               dtmp = MIN_SAMPLE;
		   }
		   else if(dtmp>MAX_SAMPLE){
               dtmp = MAX_SAMPLE;
		   }
		  
		   ((short*)*out_data)[(i*block_size) + k] = ((short) dtmp);
       }
	}

	return out_size;
}

static char* tdav_codec_ilbc_sdp_att_get(const tmedia_codec_t* codec, const char* att_name)
{
	if(tsk_striequals(att_name, "fmtp")){
		char* fmtp = tsk_null;
		tsk_sprintf(&fmtp, "mode=%d", TDAV_ILBC_MODE);
		return fmtp;
	}
	return tsk_null;
}

static tsk_bool_t tdav_codec_ilbc_sdp_att_match(const tmedia_codec_t* codec, const char* att_name, const char* att_value)
{	
	if(tsk_striequals(att_name, "fmtp")){
		/*	RFC 3952 - 5. Mapping To SDP Parameters
			
			The offer contains the preferred mode of the offerer.  The answerer
			may agree to that mode by including the same mode in the answer, or
			may include a different mode.  The resulting mode used by both
			parties SHALL be the lower of the bandwidth modes in the offer and
			answer.

			That is, an offer of "mode=20" receiving an answer of "mode=30" will
			result in "mode=30" being used by both participants.  Similarly, an
			offer of "mode=30" and an answer of "mode=20" will result in
			"mode=30" being used by both participants.

			This is important when one end point utilizes a bandwidth constrained
			link (e.g., 28.8k modem link or slower), where only the lower frame
			size will work.
		*/
		return tsk_true; // FIXME
	}
	return tsk_true;
}


//
//	iLBC Plugin definition
//

/* constructor */
static tsk_object_t* tdav_codec_ilbc_ctor(tsk_object_t * self, va_list * app)
{
	tdav_codec_ilbc_t *ilbc = self;
	if(ilbc){
		/* init base: called by tmedia_codec_create() */
		/* init self */
	}
	return self;
}
/* destructor */
static tsk_object_t* tdav_codec_ilbc_dtor(tsk_object_t * self)
{ 
	tdav_codec_ilbc_t *ilbc = self;
	if(ilbc){
		/* deinit base */
		tmedia_codec_audio_deinit(ilbc);
		/* deinit self */
		//ilbc->encoder = {0};
		//ilbc->decoder = {0};
	}

	return self;
}
/* object definition */
static const tsk_object_def_t tdav_codec_ilbc_def_s = 
{
	sizeof(tdav_codec_ilbc_t),
	tdav_codec_ilbc_ctor, 
	tdav_codec_ilbc_dtor,
	tmedia_codec_cmp, 
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t tdav_codec_ilbc_plugin_def_s = 
{
	&tdav_codec_ilbc_def_s,

	tmedia_audio,
	tmedia_codec_id_ilbc,
	"iLBC",
	"iLBC codec (libILBc)",
	TMEDIA_CODEC_FORMAT_ILBC,
	tsk_true,
	8000, // rate
	
	{ /* audio */
		1, // channels
		20 // ptime
	},

	/* video */
	{0},

	tdav_codec_ilbc_set,
	tdav_codec_ilbc_open,
	tdav_codec_ilbc_close,
	tdav_codec_ilbc_encode,
	tdav_codec_ilbc_decode,
	tdav_codec_ilbc_sdp_att_match,
	tdav_codec_ilbc_sdp_att_get
};
const tmedia_codec_plugin_def_t *tdav_codec_ilbc_plugin_def_t = &tdav_codec_ilbc_plugin_def_s;


#endif /* HAVE_ILBC */
