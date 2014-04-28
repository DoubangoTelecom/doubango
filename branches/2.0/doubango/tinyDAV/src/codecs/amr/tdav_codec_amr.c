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

/**@file tdav_codec_amr.c
 * @brief AMR-NB and AMR-WB codecs.
 * RTP payloader/depayloader are based on RFC 4867
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#include "tinydav/codecs/amr/tdav_codec_amr.h"

#include "tsk_params.h"
#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#include <stdlib.h> /* atoi() */

#if HAVE_OPENCORE_AMR

#if defined(_MSC_VER)
#	pragma comment(lib, "..\\thirdparties\\win32\\lib\\opencore\\libopencore-amrnb.a")
#endif

#define NO_DATA 15
#define DEFAULT_ENC_MODE ((enum Mode)MR122) /* Higher, could be changed by remote party by using CMR */

/* From WmfDecBytesPerFrame in dec_input_format_tab.cpp */
static const int tdav_codec_amr_nb_sizes[] = { 12, 13, 15, 17, 19, 20, 26, 31, 5, 6, 5, 5, 0, 0, 0, 0 };
/* From pvamrwbdecoder_api.h, by dividing by 8 and rounding up */
static const int tdav_codec_amr_wb_sizes[] = { 17, 23, 32, 36, 40, 46, 50, 58, 60, 5, -1, -1, -1, -1, -1, -1 };

/* ============ Common ================= */
static int tdav_codec_amr_init(tdav_codec_amr_t* self, tdav_codec_amr_type_t type, tdav_codec_amr_mode_t mode);
static int tdav_codec_amr_deinit(tdav_codec_amr_t* self);
static tdav_codec_amr_mode_t tdav_codec_amr_get_mode(const char* fmtp);
static int tdav_codec_amr_parse_fmtp(tdav_codec_amr_t* self, const char* fmtp);
static tsk_size_t tdav_codec_amr_oa_decode(tdav_codec_amr_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size, const tsk_object_t* proto_hdr);
static tsk_size_t tdav_codec_amr_be_decode(tdav_codec_amr_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size, const tsk_object_t* proto_hdr);
static tsk_size_t tdav_codec_amr_be_encode(tdav_codec_amr_t* amr, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size);
static tsk_size_t tdav_codec_amr_oa_encode(tdav_codec_amr_t* amr, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size);
static uint8_t tdav_codec_amr_bitbuffer_read(const void* bits, tsk_size_t size, tsk_size_t start, tsk_size_t count);

/* ============ AMR-NB Plugin interface ================= 
	The AMR codec was originally developed and standardized by the
	European Telecommunications Standards Institute (ETSI) for GSM
	cellular systems.  It is now chosen by the Third Generation
	Partnership Project (3GPP) as the mandatory codec for third
	generation (3G) cellular systems [1].

	The AMR codec is a multi-mode codec that supports eight narrow band
	speech encoding modes with bit rates between 4.75 and 12.2 kbps.  The
	sampling frequency used in AMR is 8000 Hz and the speech encoding is
	performed on 20 ms speech frames.  Therefore, each encoded AMR speech
	frame represents 160 samples of the original speech.

	Among the eight AMR encoding modes, three are already separately
	adopted as standards of their own.  Particularly, the 6.7 kbps mode
	is adopted as PDC-EFR [18], the 7.4 kbps mode as IS-641 codec in TDMA
	[17], and the 12.2 kbps mode as GSM-EFR [16].
*/

int tdav_codec_amrnb_open(tmedia_codec_t* self)
{
	tdav_codec_amr_t* amrnb = (tdav_codec_amr_t*)self;

	if(!TDAV_CODEC_AMR(amrnb)->encoder){
		if(!(TDAV_CODEC_AMR(amrnb)->encoder = Encoder_Interface_init(0))){
			TSK_DEBUG_ERROR("Failed to initialize AMR-NB encoder");
			return -2;
		}
	}

	if(!TDAV_CODEC_AMR(amrnb)->decoder){
		if(!(TDAV_CODEC_AMR(amrnb)->decoder = Decoder_Interface_init())){
			TSK_DEBUG_ERROR("Failed to initialize AMR-NB encoder");
			return -2;
		}
	}

	return 0;
}

int tdav_codec_amrnb_close(tmedia_codec_t* self)
{
	tdav_codec_amr_t* amrnb = (tdav_codec_amr_t*)self;

	if(TDAV_CODEC_AMR(amrnb)->encoder){
		Encoder_Interface_exit(TDAV_CODEC_AMR(amrnb)->encoder);
		TDAV_CODEC_AMR(amrnb)->encoder = tsk_null;
	}

	if(TDAV_CODEC_AMR(amrnb)->decoder){
		Decoder_Interface_exit(TDAV_CODEC_AMR(amrnb)->decoder);
		TDAV_CODEC_AMR(amrnb)->decoder = tsk_null;
	}

	return 0;
}

tsk_size_t tdav_codec_amrnb_encode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size)
{	
	tdav_codec_amr_t* amr = (tdav_codec_amr_t*)self;
	
	switch(amr->mode){
		case tdav_codec_amr_mode_be:
			return tdav_codec_amr_be_encode(amr, in_data, in_size, out_data, out_max_size);
		default:
			return tdav_codec_amr_oa_encode(amr, in_data, in_size, out_data, out_max_size); 
	}
	
	return 0;
}

tsk_size_t tdav_codec_amrnb_decode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size, const tsk_object_t* proto_hdr)
{
	tdav_codec_amr_t* amr = (tdav_codec_amr_t*)self;

	switch(amr->mode){
		case tdav_codec_amr_mode_be:
			return tdav_codec_amr_be_decode(amr, in_data, in_size, out_data, out_max_size, proto_hdr);
		default:
			return tdav_codec_amr_oa_decode(amr, in_data, in_size, out_data, out_max_size, proto_hdr); 
	}
}

char* tdav_codec_amrnb_sdp_att_get(const tmedia_codec_t* codec, const char* att_name)
{
	const tdav_codec_amr_t* amr = (const tdav_codec_amr_t*)codec;

	/* We support all modes, all ... */
	if(amr){
		switch(amr->mode){
			case tdav_codec_amr_mode_be:
				return tsk_strdup("octet-align=0");
			default:
				return tsk_strdup("octet-align=1");
		}
	}
	return tsk_null;
}

tsk_bool_t tdav_codec_amrnb_sdp_att_match(const tmedia_codec_t* codec, const char* att_name, const char* att_value)
{	
	tdav_codec_amr_t* amr;
	if(!(amr = (tdav_codec_amr_t*)codec) || !att_name){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_false;
	}

	if(amr && tsk_striequals(att_name, "fmtp")){
		/* Match mode */
		if(tdav_codec_amr_get_mode(att_value) != amr->mode){
			TSK_DEBUG_INFO("Failed to match [%s]", att_value);
			return tsk_false;
		}
		/* check parameters validity */
		if(tdav_codec_amr_parse_fmtp(amr, att_value)){
			TSK_DEBUG_INFO("Failed to match [%s]", att_value);
			return tsk_false;
		}

		return tsk_true;
	}
	return tsk_false;
}


//
//	AMR-NB OA Plugin definition
//

/* constructor */
static tsk_object_t* tdav_codec_amrnb_oa_ctor(tsk_object_t * self, va_list * app)
{
	tdav_codec_amr_t *amrnb_oa = self;
	if(amrnb_oa){
		/* init base: called by tmedia_codec_create() */
		/* init self */
		tdav_codec_amr_init(TDAV_CODEC_AMR(amrnb_oa), tdav_codec_amr_type_nb, tdav_codec_amr_mode_oa);
	}
	return self;
}
/* destructor */
static tsk_object_t* tdav_codec_amrnb_oa_dtor(tsk_object_t * self)
{ 
	tdav_codec_amr_t *amrnb_oa = self;
	if(amrnb_oa){
		/* deinit base */
		tmedia_codec_audio_deinit(amrnb_oa);
		/* deinit self */
		tdav_codec_amr_deinit(TDAV_CODEC_AMR(amrnb_oa));
	}

	return self;
}
/* object definition */
static const tsk_object_def_t tdav_codec_amrnb_oa_def_s = 
{
	sizeof(tdav_codec_amr_t),
	tdav_codec_amrnb_oa_ctor, 
	tdav_codec_amrnb_oa_dtor,
	tmedia_codec_cmp, 
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t tdav_codec_amrnb_oa_plugin_def_s = 
{
	&tdav_codec_amrnb_oa_def_s,

	tmedia_audio,
	tmedia_codec_id_amr_nb_oa,
	"AMR",
	"AMR Narrow Band - Octet Aligned (libopencore-amr)",
	TMEDIA_CODEC_FORMAT_AMR_NB_OA,
	tsk_true,
	8000, // rate
	
	{ /* audio */
		1, // channels
		20 // ptime
	},

	/* video */
	{0},

	tsk_null, // set()
	tdav_codec_amrnb_open,
	tdav_codec_amrnb_close,
	tdav_codec_amrnb_encode,
	tdav_codec_amrnb_decode,
	tdav_codec_amrnb_sdp_att_match,
	tdav_codec_amrnb_sdp_att_get
};
const tmedia_codec_plugin_def_t *tdav_codec_amrnb_oa_plugin_def_t = &tdav_codec_amrnb_oa_plugin_def_s;

//
//	AMR-NB BE Plugin definition
//

/* constructor */
static tsk_object_t* tdav_codec_amrnb_be_ctor(tsk_object_t * self, va_list * app)
{
	tdav_codec_amr_t *amrnb_be = self;
	if(amrnb_be){
		/* init base: called by tmedia_codec_create() */
		/* init self */
		tdav_codec_amr_init(TDAV_CODEC_AMR(amrnb_be), tdav_codec_amr_type_nb, tdav_codec_amr_mode_be);
	}
	return self;
}
/* destructor */
static tsk_object_t* tdav_codec_amrnb_be_dtor(tsk_object_t * self)
{ 
	tdav_codec_amr_t *amrnb_be = self;
	if(amrnb_be){
		/* deinit base */
		tmedia_codec_audio_deinit(amrnb_be);
		/* deinit self */
		tdav_codec_amr_deinit(TDAV_CODEC_AMR(amrnb_be));
	}

	return self;
}
/* object definition */
static const tsk_object_def_t tdav_codec_amrnb_be_def_s = 
{
	sizeof(tdav_codec_amr_t),
	tdav_codec_amrnb_be_ctor, 
	tdav_codec_amrnb_be_dtor,
	tmedia_codec_cmp, 
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t tdav_codec_amrnb_be_plugin_def_s = 
{
	&tdav_codec_amrnb_be_def_s,

	tmedia_audio,
	tmedia_codec_id_amr_nb_be,
	"AMR",
	"AMR Narrow Band - Bandwidth-Efficient (libopencore-amr)",
	TMEDIA_CODEC_FORMAT_AMR_NB_BE,
	tsk_true,
	8000, // rate
	
	{ /* audio */
		1, // channels
		0 // ptime @deprecated
	},

	/* video */
	{0},

	tsk_null, // set()
	tdav_codec_amrnb_open,
	tdav_codec_amrnb_close,
	tdav_codec_amrnb_encode,
	tdav_codec_amrnb_decode,
	tdav_codec_amrnb_sdp_att_match,
	tdav_codec_amrnb_sdp_att_get
};
const tmedia_codec_plugin_def_t *tdav_codec_amrnb_be_plugin_def_t = &tdav_codec_amrnb_be_plugin_def_s;









//
// Common functions
//

static int tdav_codec_amr_init(tdav_codec_amr_t* self, tdav_codec_amr_type_t type, tdav_codec_amr_mode_t mode)
{
	if(self){
		self->type = type;
		self->mode = mode;
		self->encoder_mode = DEFAULT_ENC_MODE;

		return 0;
	}
	else{
		TSK_DEBUG_ERROR("Invalid Parameter");
		return -1;
	}
}

static int tdav_codec_amr_deinit(tdav_codec_amr_t* self)
{
	if(self){
		switch(self->type){
			case tdav_codec_amr_type_nb:
				{	/* AMR-NB */
					if(self->encoder){
						Encoder_Interface_exit(self->encoder);
						self->encoder = tsk_null;
					}
					if(self->decoder){
						Decoder_Interface_exit(self->decoder);
						self->decoder = tsk_null;
					}
					break;
				}
			case tdav_codec_amr_type_wb:
				{	/* AMR-WB */
					break;
				}
		}

		return 0;
	}
	else{
		TSK_DEBUG_ERROR("Invalid Parameter");
		return -1;
	}
}

static tdav_codec_amr_mode_t tdav_codec_amr_get_mode(const char* fmtp)
{
	/* RFC 4867 - 8.1.  AMR Media Type Registration
		octet-align: Permissible values are 0 and 1.  If 1, octet-aligned
		operation SHALL be used.  If 0 or if not present, bandwidth-efficient operation is employed.
	*/
	tdav_codec_amr_mode_t mode = tdav_codec_amr_mode_be;
	tsk_size_t size = tsk_strlen(fmtp);
	int start, end;
	
	if((start = tsk_strindexOf(fmtp, size, "octet-align")) !=-1){
		tsk_param_t* param;
		if((end = tsk_strindexOf((fmtp+start), (size-start), ";")) == -1){
			end = size;
		}
		if((param = tsk_params_parse_param((fmtp+start), (end-start)))){
			if(param->value && tsk_strequals(param->value, "1")){
				mode = tdav_codec_amr_mode_oa;
			}
			TSK_OBJECT_SAFE_FREE(param);
		}
	}
	return mode;
}

int tdav_codec_amr_parse_fmtp(tdav_codec_amr_t* self, const char* fmtp)
{
	int ret = 0;
	int val_int;
	const char* val_str;
	//--tdav_codec_amr_mode_t mode = self->mode;
	tsk_params_L_t* params = tsk_null;

	if((params = tsk_params_fromstring(fmtp, ";", tsk_true))){
		/* Do not check "octet-align" => already done by the caller of this function */
		
		/* === mode-set ===*/
		if((val_str = tsk_params_get_param_value(params, "mode-set"))){
			char* modes = tsk_strdup(val_str);
			char *pch, *saveptr;
			int mode_int;
			pch = tsk_strtok_r(modes, ", ", &saveptr);
			while(pch){
				mode_int = atoi(pch);
				self->modes |= 0x0001 << mode_int;
				pch = tsk_strtok_r(tsk_null, ", ", &saveptr);
			}
			
			TSK_FREE(modes);
		}
		else{
			self->modes = 0xFFFF;
		}

		/* === interleaving ===*/
		if((val_int = tsk_params_get_param_value_as_int(params, "interleaving")) != -1){
			TSK_DEBUG_WARN("interleaving not supported");
			ret = -1; goto bail;
		}
		/* === mode-change-period ===*/
		if((val_int = tsk_params_get_param_value_as_int(params, "mode-change-period")) != -1){
			if(val_int != 1 && val_int != 2){
				TSK_DEBUG_ERROR("Invalid [mode-change-period]");
				ret = -1; goto bail;
			}
			self->mcp = (unsigned)val_int;
		}
		/* === mode-change-capability ===*/
		if((val_int = tsk_params_get_param_value_as_int(params, "mode-change-capability")) != -1){
			if(val_int != 1 && val_int != 2){
				TSK_DEBUG_ERROR("Invalid [mode-change-capability]");
				ret = -1; goto bail;
			}
			self->mcc = (unsigned)val_int;
		}
		/* === mode-change-neighbor ===*/
		if((val_int = tsk_params_get_param_value_as_int(params, "mode-change-neighbor")) != -1){
			if(val_int != 0 && val_int != 1){
				TSK_DEBUG_ERROR("Invalid [mode-change-neighbor]");
				ret = -1; goto bail;
			}
			self->mcn = (unsigned)val_int;
		}
	}

bail:
	TSK_OBJECT_SAFE_FREE(params);
	return ret;
}


/* RFC 4867 - 4.2.  Payload Structure
	+----------------+-------------------+----------------
	| payload header | table of contents | speech data ...
	+----------------+-------------------+----------------
*/
/*	RFC 4867 - 4.4.2.  The Payload Table of Contents and Frame CRCs
		The table of contents (ToC) consists of a list of ToC entries, each representing a speech frame.
		+---------------------+
		| list of ToC entries |
		+---------------------+
		| list of frame CRCs  | (optional)
		- - - - - - - - - - -
		Note, for ToC entries with FT=14 or 15, there will be no
		corresponding speech frame or frame CRC present in the payload.
*/


static tsk_size_t tdav_codec_amr_be_encode(tdav_codec_amr_t* amr, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size)
{	
	tsk_size_t out_size = 0, i;
	int ret_size;
	uint8_t ToC;
	static uint8_t CMR = NO_DATA /* No interleaving */;

	uint8_t outbuf[60 + 1]; /* enought for both NB and WB at ptime=20ms */
	if(!amr || !in_data || !in_size || !out_data || (amr->mode != tdav_codec_amr_mode_be)){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}

	/* Encode */
	if((ret_size = Encoder_Interface_Encode(amr->encoder, amr->encoder_mode, in_data, outbuf, 0)) <= 0){
		TSK_DEBUG_ERROR("Encoder_Interface_Encode() failed");
		goto bail;
	}
	
	
	/* allocate output buffer */
	if((int)*out_max_size <ret_size){
		if(!(*out_data = tsk_realloc(*out_data, ret_size))){
			*out_max_size = 0;
			TSK_DEBUG_ERROR("Failed to allocate new buffer");
			goto bail;
		}
		*out_max_size = ret_size;
	}

	out_size = ret_size;

	/* CMR (4bits) */
	((uint8_t*)*out_data)[0] = (CMR<<4);
	/* ToC (Always ONE Frame, don't need to test for [F]) (6bits)*/
	ToC = outbuf[0]>>2/*2*[P]*/;
	((uint8_t*)*out_data)[0] |=  (ToC >> 2/*[Q],[1-FT]*/) & 0xF; /* 4bits */
	((uint8_t*)*out_data)[1] = (ToC & 0x3/*[1-FT],[Q]*/)<<6; /* 2bits */

	/* === THERE ARE 2 EXTRA BITS === */

	for(i=1; i<out_size-1; i++){
		((uint8_t*)*out_data)[i] |= outbuf[i]>>2;/* 6bits */
		((uint8_t*)*out_data)[i+1] = outbuf[i]<<6;/* 2bits */
	}

bail:
	return out_size;
}

tsk_size_t tdav_codec_amr_be_decode(tdav_codec_amr_t* amr, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size, const tsk_object_t* proto_hdr)
{
	tsk_size_t out_size = 0, pcm_frame_size = 0, index = 0;
	const uint8_t* pdata = (const uint8_t*)in_data;
	//--const uint8_t* pend = (pdata + in_size);
	uint8_t CMR;
	int toc_entries = 0, i, k; // ToC entries count

	if(!amr || !in_data || !in_size || !out_data || (amr->mode != tdav_codec_amr_mode_be)){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}

	/* compute PCM frame size */
	switch(TDAV_CODEC_AMR(amr)->type){
		case tdav_codec_amr_type_nb:
			pcm_frame_size = 160 * sizeof(short);
			break;
		case tdav_codec_amr_type_wb:
			pcm_frame_size = 320 * sizeof(short);
			break;
		default:
			TSK_DEBUG_ERROR("Invalid AMR type");
			return 0;
	}

	/* CMR (4bits) */
	CMR = tdav_codec_amr_bitbuffer_read(in_data, (in_size*8), index, 4);
	index += 4;
	if(CMR != NO_DATA){
		amr->encoder_mode = (enum Mode)CMR;
	}
	
	/* F(1bit), FT(4bits), Q(1bit) */
	/* count ToC entries */
	do{	/* At least ONE ToC */
		++toc_entries;
		++pdata;
		index += 6;
	}
	while((index < (in_size*8)) && (tdav_codec_amr_bitbuffer_read(in_data, (in_size*8), (index-6), 1)/* F */));
	
	for(i = 0; (i<toc_entries && (in_size < (in_size*8))) ; i++){
		int size = -1;
		uint8_t* speech_data = tsk_null;
		//--int speech_data_size = 0;
		uint8_t ToC = tdav_codec_amr_bitbuffer_read(in_data, (in_size*8), 4/*CMR*/ + (i*6), 6);
		
		switch(TDAV_CODEC_AMR(amr)->type){
			case tdav_codec_amr_type_nb:
				size = tdav_codec_amr_nb_sizes[(ToC>>1)&0x0F/* FT */];
				break;
			case tdav_codec_amr_type_wb:
				size = tdav_codec_amr_wb_sizes[(ToC>>1)&0x0F/* FT */];
				break;
		}		
		
		if((speech_data = tsk_calloc((size + 2/* ToC + '\0' */), sizeof(uint8_t)))){
			/* copy ToC */
			speech_data[0] = (ToC & 0x1F)<<2/* 2*[P] */;  /* ToC as OA layout */
			/* copy speech data */
			for(k=0; k<size; k++){
				speech_data[1 + k] = tdav_codec_amr_bitbuffer_read(in_data, (in_size*8), index, 8);
				index+=8;
				if((k==size-1) && (index%8)){
					speech_data[1 + k] <<= (8-(index%8)); //clean
				}
			}
			
			/* allocate/reallocate speech data */
			if(*out_max_size <(out_size + pcm_frame_size)){
				if(!(*out_data = tsk_realloc(*out_data, (out_size + pcm_frame_size)))){
					TSK_DEBUG_ERROR("Failed to allocate new buffer");
					*out_max_size = 0;
					TSK_FREE(speech_data);
					goto bail;
				}
				*out_max_size = out_size + pcm_frame_size;
			}

			/* decode speech data */
			Decoder_Interface_Decode(amr->decoder, speech_data, &((short*)*out_data)[out_size/sizeof(short)], 0);
			out_size += pcm_frame_size, pdata+= size;

			TSK_FREE(speech_data);
		}
	}

bail:
	return out_size;
}

static tsk_size_t tdav_codec_amr_oa_encode(tdav_codec_amr_t* amr, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size)
{	
	tsk_size_t out_size = 0;
	int ret_size;
	static uint8_t CMR = NO_DATA /* No interleaving */;

	uint8_t outbuf[60 + 1]; /* enought for both NB and WB at ptime=20ms */
	if(!amr || !in_data || !in_size || !out_data || (amr->mode != tdav_codec_amr_mode_oa)){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}

	/* Encode */
	if((ret_size = Encoder_Interface_Encode(amr->encoder, amr->encoder_mode, in_data, outbuf, 0)) <= 0){
		TSK_DEBUG_ERROR("Encoder_Interface_Encode() failed");
		goto bail;
	}
	
	out_size = ret_size + 1 /* CMR without interleaving */;
	/* allocate output buffer */
	if(*out_max_size <out_size){
		if(!(*out_data = tsk_realloc(*out_data, out_size))){
			TSK_DEBUG_ERROR("Failed to allocate new buffer");
			*out_max_size = out_size = 0;
			goto bail;
		}
		*out_max_size = out_size;
	}

	/* CMR */
	((uint8_t*)*out_data)[0] = (CMR << 4);
	/* Only ONE ToC --> believe me */
	memcpy(&((uint8_t*)*out_data)[1], outbuf, ret_size);

bail:
	return out_size;
}

static tsk_size_t tdav_codec_amr_oa_decode(tdav_codec_amr_t* amr, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size, const tsk_object_t* proto_hdr)
{
	tsk_size_t out_size = 0, pcm_frame_size = 0;
	const uint8_t* pdata = (const uint8_t*)in_data;
	const uint8_t* pend = (pdata + in_size);
	uint8_t CMR;
	int toc_entries = 0, i; // ToC entries count

	if(!amr || !in_data || !in_size || !out_data || (amr->mode != tdav_codec_amr_mode_oa)){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}

	/* compute PCM frame size */
	switch(TDAV_CODEC_AMR(amr)->type){
		case tdav_codec_amr_type_nb:
			pcm_frame_size = 160 * sizeof(short);
			break;
		case tdav_codec_amr_type_wb:
			pcm_frame_size = 320 * sizeof(short);
			break;
		default:
			TSK_DEBUG_ERROR("Invalid AMR type");
			return 0;
	}	

	/* RFC 4867 - 4.4.  Octet-Aligned Mode
		In octet-aligned mode, the payload header consists of a 4-bit CMR, 4
	   reserved bits, and optionally, an 8-bit interleaving header, as shown
	   below:

		0                   1
		0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
	   +-+-+-+-+-+-+-+-+- - - - - - - -
	   |  CMR  |R|R|R|R|  ILL  |  ILP  |
	   +-+-+-+-+-+-+-+-+- - - - - - - -

	   CMR (4 bits): same as defined in Section 4.3.1.

	   "interleaving" not supported ==> could ignore ILL and ILP (wich are optional)
	*/
	
	CMR = (*pdata++ >> 4);
	if(CMR != NO_DATA){
		/* The codec mode request received in the CMR field is valid until the
		next codec mode request is received, i.e., a newly received CMR value
		corresponding to a speech mode, or NO_DATA overrides the previously
		received CMR value corresponding to a speech mode or NO_DATA. */
		amr->encoder_mode = (enum Mode)CMR; // As we support all modes, do not check for validity
	}
	
	/*	
		A ToC entry takes the following format in octet-aligned mode:
		0 1 2 3 4 5 6 7
		+-+-+-+-+-+-+-+-+
		|F|  FT   |Q|P|P|
		+-+-+-+-+-+-+-+-+

		F (1 bit): see definition in Section 4.3.2.
		FT (4 bits, unsigned integer): see definition in Section 4.3.2.
		Q (1 bit): see definition in Section 4.3.2.
		P bits: padding bits, MUST be set to zero, and MUST be ignored on reception.
	*/	

	/* count ToC entries */
	do{	/* At least ONE ToC */
		++toc_entries;
		++pdata;
	}
	while(pdata && (pdata < pend) && (pdata[-1] >> 7/* F */));

	for(i = 0; (i<toc_entries && (pdata < pend)) ; i++){
		int size = -1;
		uint8_t* speech_data = tsk_null;
		//--int speech_data_size = 0;
		uint8_t ToC = ((const uint8_t*)in_data)[1/*CMR...*/ + i];
		switch(TDAV_CODEC_AMR(amr)->type){
			case tdav_codec_amr_type_nb:
				size = tdav_codec_amr_nb_sizes[(ToC>>3) & 0x0F/* FT */];
				break;
			case tdav_codec_amr_type_wb:
				size = tdav_codec_amr_wb_sizes[(ToC>>3) & 0x0F/* FT */];
				break;
		}
		
		/* check size */
		if(size <0 || ((pdata + size) > pend)){
			TSK_DEBUG_ERROR("Invalid size");
			break;
		}
		
		if((speech_data = tsk_calloc((size + 2/* ToC + '\0' */), sizeof(uint8_t)))){
			/* copy ToC */
			*speech_data = ToC & 0x7F/* with 'F'=0 */;
			/* copy speech data */
			memcpy((speech_data + 1), pdata, size);
			/* allocate/reallocate speech data */
			if(*out_max_size <(out_size + pcm_frame_size)){
				if(!(*out_data = tsk_realloc(*out_data, (out_size + pcm_frame_size)))){
					TSK_DEBUG_ERROR("Failed to allocate new buffer");
					*out_max_size = 0;
					TSK_FREE(speech_data);
					goto bail;
				}
				*out_max_size = (out_size + pcm_frame_size);
			}
			/* decode speech data */
			Decoder_Interface_Decode(amr->decoder, speech_data, &((short*)*out_data)[out_size/sizeof(short)], 0);
			out_size += pcm_frame_size, pdata+= size;

			TSK_FREE(speech_data);
		}
	}

bail:
	return out_size;
}


static uint8_t tdav_codec_amr_bitbuffer_read(const void* bits, tsk_size_t size, tsk_size_t start, tsk_size_t count)
{
	uint8_t byte, left, right, pad;

	if(!bits || !size || count>8){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}
	
	if((start + count) > size){
		count = (size - start);
	}

	pad = start ? (8 - (start % 8)) : count;
	left = ((uint8_t*)bits)[start/8] << (8-pad);
	right = ((uint8_t*)bits)[((start+count)<size ? (start+count) : start)/8] >> pad;

	if((start && (start % 8) != ((start+count)%8)) || (!start && count>8)){
		/* overlap */
		byte = (left | right) >> (8-count);
	}
	else{
		byte = (left | right) & (0xFF >> (8-count));
	}

	return byte;
}

#endif /* HAVE_OPENCORE_AMR */
