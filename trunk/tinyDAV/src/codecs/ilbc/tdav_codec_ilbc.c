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

/**@file tdav_codec_ilbc.c
 * @brief iLBC codec
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinydav/codecs/ilbc/tdav_codec_ilbc.h"

#if HAVE_ILBC

#include "tsk_memory.h"
#include "tsk_debug.h"

#define TDAV_ILBC_MODE			20

/* ============ GSM Plugin interface ================= */

#define tdav_codec_ilbc_fmtp_set tsk_null

int tdav_codec_ilbc_open(tmedia_codec_t* self)
{
	tdav_codec_ilbc_t* ilbc = (tdav_codec_ilbc_t*)self;
	
	initEncode(&ilbc->encoder, TDAV_ILBC_MODE);
    initDecode(&ilbc->decoder, TDAV_ILBC_MODE, tsk_true/* Enhancer */);

	return 0;
}

int tdav_codec_ilbc_close(tmedia_codec_t* self)
{
	tdav_codec_ilbc_t* ilbc = (tdav_codec_ilbc_t*)self;

	//ilbc->encoder = {0};
	//ilbc->decoder = {0};

	return 0;
}

tsk_size_t tdav_codec_ilbc_encode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data)
{	
	tdav_codec_ilbc_t* ilbc = (tdav_codec_ilbc_t*)self;
	float block[BLOCKL_MAX];
	int k;
	
	if(!self || !in_data || !in_size || !out_data){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}
	
	/* free old buffer */
	if(*out_data){
		TSK_FREE(*out_data);
	}
	
   /* convert signal to float */
	for(k=0; k<ilbc->encoder.blockl; k++){
       block[k] = (float)((short*)in_data)[k];
	}
	
	/* allocate new buffer */
	if(!(*out_data = tsk_calloc(ilbc->encoder.no_of_bytes, 1))){
		TSK_DEBUG_ERROR("Failed to allocate new buffer");
		return 0;
	}
	
	/* do the actual encoding */
    iLBC_encode(*out_data, block, &ilbc->encoder);
	
	return ilbc->encoder.no_of_bytes;
}

tsk_size_t tdav_codec_ilbc_decode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, const tsk_object_t* proto_hdr)
{
	int blocks, i, k, block_size;
	float decblock[BLOCKL_MAX], dtmp;
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

	/* free old buffer */
	if(*out_data){
		TSK_FREE(*out_data);
	}

	/* allocate new buffer */
	if(!(*out_data = tsk_calloc(out_size, 1))){
		TSK_DEBUG_ERROR("Failed to allocate new buffer");
		return 0;
	}

	for(i = 0; i<blocks; i++){
		iLBC_decode(decblock, (&(uint8_t*)in_data)[i*block_size], &ilbc->decoder, 1/* Normal */);

       /* convert to short */
       for(k=0; k<ilbc->decoder.blockl; k++){
           dtmp=decblock[k];

           if (dtmp<MIN_SAMPLE)
               dtmp=MIN_SAMPLE;
           else if (dtmp>MAX_SAMPLE)
               dtmp=MAX_SAMPLE;
		  
		   ((short*)*out_data)[(i*block_size) + k] = ((short) dtmp);
       }
	}

	return out_size;
}

char* tdav_codec_ilbc_fmtp_get(const tmedia_codec_t* codec)
{
	char* fmtp = tsk_null;
	tsk_sprintf(&fmtp, "mode=%d", TDAV_ILBC_MODE);
	return fmtp;
}

tsk_bool_t tdav_codec_ilbc_fmtp_match(const tmedia_codec_t* codec, const char* fmtp)
{	/*	RFC 3952 - 5. Mapping To SDP Parameters
		
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


//
//	G.711u Plugin definition
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
	"iLBC",
	"iLBC codec",
	TMEDIA_CODEC_FORMAT_ILBC,
	tsk_true,
	8000, // rate
	
	{ /* audio */
		1, // channels
		20 // ptime
	},

	/* video */
	{0},

	tdav_codec_ilbc_open,
	tdav_codec_ilbc_close,
	tdav_codec_ilbc_encode,
	tdav_codec_ilbc_decode,
	tdav_codec_ilbc_fmtp_match,
	tdav_codec_ilbc_fmtp_get,
	tdav_codec_ilbc_fmtp_set
};
const tmedia_codec_plugin_def_t *tdav_codec_ilbc_plugin_def_t = &tdav_codec_ilbc_plugin_def_s;


#endif /* HAVE_ILBC */
