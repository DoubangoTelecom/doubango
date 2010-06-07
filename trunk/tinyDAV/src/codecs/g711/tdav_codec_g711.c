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

/**@file tdav_codec_g711.c
 * @brief G.711u and G.711a (a.k.a PCMU and PCMA) codec plugins.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinydav/codecs/g711/tdav_codec_g711.h"

/* ============ G.711u Plugin interface ================= */

#define tdav_codec_g711u_fmtp_get tsk_null
#define tdav_codec_g711u_fmtp_set tsk_null

tsk_bool_t tdav_codec_g711u_fmtp_match(const tmedia_codec_t* codec, const char* fmtp)
{	/* always match */
	return tsk_true;
}


//=======================================================
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
	"PCMU",
	"G.711u codec (From tinyDAV)",
	TMEDIA_CODEC_FORMAT_G711u,
	tsk_false,
	8000, // rate
	
	{ /* audio */
		1 // channels
	},

	/* video */
	{0},

	tdav_codec_g711u_fmtp_match,
	tdav_codec_g711u_fmtp_get,
	tdav_codec_g711u_fmtp_set
};
const tmedia_codec_plugin_def_t *tdav_codec_g711u_plugin_def_t = &tdav_codec_g711u_plugin_def_s;


/* ============ G.711a Plugin interface ================= */

#define tdav_codec_g711a_fmtp_get tsk_null
#define tdav_codec_g711a_fmtp_set tsk_null

tsk_bool_t tdav_codec_g711a_fmtp_match(const tmedia_codec_t* codec, const char* fmtp)
{	/* always match */
	return tsk_true;
}


//======================================================
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
	"PCMA",
	"G.711a codec (From tinyDAV)",
	TMEDIA_CODEC_FORMAT_G711a,
	tsk_false,
	8000, // rate
	
	{ /* audio */
		1 // channels
	},

	/* video */
	{0},

	tdav_codec_g711a_fmtp_match,
	tdav_codec_g711a_fmtp_get,
	tdav_codec_g711a_fmtp_set
};
const tmedia_codec_plugin_def_t *tdav_codec_g711a_plugin_def_t = &tdav_codec_g711a_plugin_def_s;
