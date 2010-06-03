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

/**@file tmedia_codec_dummy.c
 * @brief Dummy codecs used for test only.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinymedia/tmedia_codec_dummy.h"

#include "tsk_debug.h"

#define tmedia_codec_dpcmu_fmtp_get tsk_null
#define tmedia_codec_dpcmu_fmtp_set tsk_null
#define tmedia_codec_dpcma_fmtp_get tsk_null
#define tmedia_codec_dpcma_fmtp_set tsk_null

tsk_bool_t tmedia_codec_dpcmu_fmtp_match(const tmedia_codec_t* codec, const char* fmtp)
{	/* always match */
	return tsk_true;
}

tsk_bool_t tmedia_codec_dpcma_fmtp_match(const tmedia_codec_t* codec, const char* fmtp)
{	/* always match */
	return tsk_true;
}

//=================================================================================================
//	Dummy G.711u object definition
//
/* constructor */
static tsk_object_t* tmedia_codec_dpcmu_ctor(tsk_object_t * self, va_list * app)
{
	tmedia_codec_dpcmu_t *dpcmu = self;
	if(dpcmu){
		/* init base: called by tmedia_codec_create() */
		/* init self */
	}
	return self;
}
/* destructor */
static tsk_object_t* tmedia_codec_dpcmu_dtor(tsk_object_t * self)
{ 
	tmedia_codec_dpcmu_t *dpcmu = self;
	if(dpcmu){
		/* deinit base */
		tmedia_codec_audio_deinit(dpcmu);
		/* deinit self */
	}

	return self;
}
/* object definition */
static const tsk_object_def_t tmedia_codec_dpcmu_def_s = 
{
	sizeof(tmedia_codec_dpcmu_t),
	tmedia_codec_dpcmu_ctor, 
	tmedia_codec_dpcmu_dtor,
	tmedia_codec_cmp, 
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t tmedia_codec_dpcmu_plugin_def_s = 
{
	&tmedia_codec_dpcmu_def_s,

	tmedia_audio,
	"G.711u",
	"Dummy G.711u codec",
	TMEDIA_CODEC_FORMAT_G711u,
	tsk_false,
	8000, // rate
	
	{ /* audio */
		1 // channels
	},

	/* video */
	{0},

	tmedia_codec_dpcmu_fmtp_match,
	tmedia_codec_dpcmu_fmtp_get,
	tmedia_codec_dpcmu_fmtp_set
};
const tmedia_codec_plugin_def_t *tmedia_codec_dpcmu_plugin_def_t = &tmedia_codec_dpcmu_plugin_def_s;

//=================================================================================================
//	Dummy G.711a object definition
//
/* constructor */
static tsk_object_t* tmedia_codec_dpcma_ctor(tsk_object_t * self, va_list * app)
{
	tmedia_codec_dpcma_t *dpcma = self;
	if(dpcma){
		/* init base: called by tmedia_codec_create() */
		/* init self */
	}
	return self;
}
/* destructor */
static tsk_object_t* tmedia_codec_dpcma_dtor(tsk_object_t * self)
{ 
	tmedia_codec_dpcma_t *dpcma = self;
	if(dpcma){
		/* deinit base */
		tmedia_codec_audio_deinit(dpcma);
		/* deinit self */
	}

	return self;
}
/* object definition */
static const tsk_object_def_t tmedia_codec_dpcma_def_s = 
{
	sizeof(tmedia_codec_dpcma_t),
	tmedia_codec_dpcma_ctor, 
	tmedia_codec_dpcma_dtor,
	tmedia_codec_cmp, 
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t tmedia_codec_dpcma_plugin_def_s = 
{
	&tmedia_codec_dpcma_def_s,

	tmedia_audio,
	"G.711a",
	"Dummy G.711a codec",
	TMEDIA_CODEC_FORMAT_G711a,
	tsk_false,
	8000, // rate
	
	{ /* audio */
		1 // channels
	},

	/* video */
	{0},

	tmedia_codec_dpcma_fmtp_match,
	tmedia_codec_dpcma_fmtp_get,
	tmedia_codec_dpcma_fmtp_set
};
const tmedia_codec_plugin_def_t *tmedia_codec_dpcma_plugin_def_t = &tmedia_codec_dpcma_plugin_def_s;

