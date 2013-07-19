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

/**@file tdav_codec_g711.c
 * @brief G.711u and G.711a (a.k.a PCMU and PCMA) codec plugins.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#include "tinydav/codecs/g711/tdav_codec_g711.h"

#include "tinydav/codecs/g711/g711.h" /* algorithms */

#include "tsk_memory.h"
#include "tsk_debug.h"

/* ============ G.711u Plugin interface ================= */

#define tdav_codec_g711u_open tsk_null
#define tdav_codec_g711u_close tsk_null
#define tdav_codec_g711u_sdp_att_get tsk_null

static tsk_size_t tdav_codec_g711u_encode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size)
{
	register tsk_size_t i;
	register uint8_t* pout_data;
	register int16_t* pin_data;
	tsk_size_t out_size;
	
	if(!self || !in_data || !in_size || !out_data){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}

	out_size = (in_size >> 1);
	
	if(*out_max_size <out_size){
		if(!(*out_data = tsk_realloc(*out_data, out_size))){
			TSK_DEBUG_ERROR("Failed to allocate new buffer");
			*out_max_size = 0;
			return 0;
		}
		*out_max_size = out_size;
	}
	
	pout_data = *out_data;
	pin_data = (int16_t*)in_data;
	for(i = 0; i<out_size; i++){
		pout_data[i] = linear2ulaw(pin_data[i]);
	}
	
	return out_size;
}

static tsk_size_t tdav_codec_g711u_decode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size, const tsk_object_t* proto_hdr)
{
	tsk_size_t i;
	tsk_size_t out_size;

	if(!self || !in_data || !in_size || !out_data){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}

	out_size = (in_size << 1);

	/* allocate new buffer */
	if(*out_max_size<out_size){
		if(!(*out_data = tsk_realloc(*out_data, out_size))){
			TSK_DEBUG_ERROR("Failed to allocate new buffer");
			*out_max_size = 0;
			return 0;
		}
		*out_max_size = out_size;
	}

	for(i = 0; i<in_size; i++){
		((short*)*out_data)[i] = ulaw2linear(((uint8_t*)in_data)[i]);
	}
	
	return out_size;
}

static tsk_bool_t tdav_codec_g711u_sdp_att_match(const tmedia_codec_t* codec, const char* att_name, const char* att_value)
{	/* always match */
	return tsk_true;
}


//
//	G.711u Plugin definition
//

/* constructor */
static tsk_object_t* tdav_codec_g711u_ctor(tsk_object_t * self, va_list * app)
{
	tdav_codec_g711u_t *g711u = self;
	if(g711u){
		/* init base: called by tmedia_codec_create() */
		/* init self */
	}
	return self;
}
/* destructor */
static tsk_object_t* tdav_codec_g711u_dtor(tsk_object_t * self)
{ 
	tdav_codec_g711u_t *g711u = self;
	if(g711u){
		/* deinit base */
		tmedia_codec_audio_deinit(g711u);
		/* deinit self */
	}

	return self;
}
/* object definition */
static const tsk_object_def_t tdav_codec_g711u_def_s = 
{
	sizeof(tdav_codec_g711u_t),
	tdav_codec_g711u_ctor, 
	tdav_codec_g711u_dtor,
	tmedia_codec_cmp, 
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t tdav_codec_g711u_plugin_def_s = 
{
	&tdav_codec_g711u_def_s,
	
	tmedia_audio,
	tmedia_codec_id_pcmu,
	"PCMU",
	"G.711u codec (native)",
	TMEDIA_CODEC_FORMAT_G711u,
	tsk_false,
	8000, // rate
	
	{ /* audio */
		1, // channels
		0 // ptime @deprecated
	},
	
	/* video */
	{0},
	
	tsk_null, // set()
	tdav_codec_g711u_open,
	tdav_codec_g711u_close,
	tdav_codec_g711u_encode,
	tdav_codec_g711u_decode,
	tdav_codec_g711u_sdp_att_match,
	tdav_codec_g711u_sdp_att_get
};
const tmedia_codec_plugin_def_t *tdav_codec_g711u_plugin_def_t = &tdav_codec_g711u_plugin_def_s;


/* ============ G.711a Plugin interface ================= */

#define tdav_codec_g711a_open tsk_null
#define tdav_codec_g711a_close tsk_null
#define tdav_codec_g711a_sdp_att_get tsk_null

static tsk_size_t tdav_codec_g711a_encode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size)
{
	register tsk_size_t i;
	register uint8_t* pout_data;
	register int16_t* pin_data;
	tsk_size_t out_size;
	
	if(!self || !in_data || !in_size || !out_data){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}

	out_size = (in_size >> 1);
	
	if(*out_max_size < out_size){
		if(!(*out_data = tsk_realloc(*out_data, out_size))){
			TSK_DEBUG_ERROR("Failed to allocate new buffer");
			*out_max_size = 0;
			return 0;
		}
		*out_max_size = out_size;
	}
	
	pout_data = *out_data;
	pin_data = (int16_t*)in_data;
	for(i = 0; i<out_size; i++){
		pout_data[i] = linear2alaw(pin_data[i]);
	}

	return out_size;
}

#if 0
FILE* file = tsk_null;
int count = 0;
#endif
static tsk_size_t tdav_codec_g711a_decode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size, const tsk_object_t* proto_hdr)
{
	tsk_size_t i, out_size;
	
	if(!self || !in_data || !in_size || !out_data){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}
	out_size = (in_size << 1);
#if 0
	if(!file && count<=1000){
		file = fopen("./g711a.pcm", "wb");
	}
#endif
	/* allocate new buffer */
	if(*out_max_size < out_size){
		if(!(*out_data = tsk_realloc(*out_data, out_size))){
			TSK_DEBUG_ERROR("Failed to allocate new buffer");
			*out_max_size = 0;
			return 0;
		}
		*out_max_size = out_size;
	}
	
	for(i = 0; i<in_size; i++){
		((short*)*out_data)[i] = alaw2linear(((uint8_t*)in_data)[i]);
	}
#if 0
	if(++count<=1000){
		fwrite(*out_data, sizeof(short), in_size, file);
	}
	else if(file){
		fclose(file);
		file = tsk_null;
	}
#endif
	return out_size;
}

static tsk_bool_t tdav_codec_g711a_sdp_att_match(const tmedia_codec_t* codec, const char* att_name, const char* att_value)
{	/* always match */
	return tsk_true;
}


//
//	G.711a Plugin definition
//

/* constructor */
static tsk_object_t* tdav_codec_g711a_ctor(tsk_object_t * self, va_list * app)
{
	tdav_codec_g711a_t *g711a = self;
	if(g711a){
		/* init base: called by tmedia_codec_create() */
		/* init self */
	}
	return self;
}
/* destructor */
static tsk_object_t* tdav_codec_g711a_dtor(tsk_object_t * self)
{ 
	tdav_codec_g711a_t *g711a = self;
	if(g711a){
		/* deinit base */
		tmedia_codec_audio_deinit(g711a);
		/* deinit self */
	}

	return self;
}
/* object definition */
static const tsk_object_def_t tdav_codec_g711a_def_s = 
{
	sizeof(tdav_codec_g711a_t),
	tdav_codec_g711a_ctor, 
	tdav_codec_g711a_dtor,
	tmedia_codec_cmp, 
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t tdav_codec_g711a_plugin_def_s = 
{
	&tdav_codec_g711a_def_s,

	tmedia_audio,
	tmedia_codec_id_pcma,
	"PCMA",
	"G.711a codec (native)",
	TMEDIA_CODEC_FORMAT_G711a,
	tsk_false,
	8000, // rate
	
	{ /* audio */
		1, // channels
		0 // ptime @deprecated
	},

	/* video */
	{0},

	tsk_null, // set()
	tdav_codec_g711a_open,
	tdav_codec_g711a_close,
	tdav_codec_g711a_encode,
	tdav_codec_g711a_decode,
	tdav_codec_g711a_sdp_att_match,
	tdav_codec_g711a_sdp_att_get
};
const tmedia_codec_plugin_def_t *tdav_codec_g711a_plugin_def_t = &tdav_codec_g711a_plugin_def_s;
