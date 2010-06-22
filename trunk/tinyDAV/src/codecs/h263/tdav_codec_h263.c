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

/**@file tdav_codec_h263.c
 * @brief H.263-1996 and H.263-1998 codec plugins.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinydav/codecs/h263/tdav_codec_h263.h"

#include "tsk_debug.h"

/* ============ H.263-1996 Plugin interface ================= */

//
//	H.263-1996 object definition
//
int tdav_codec_h263_open(tmedia_codec_t* self)
{
	return 0;
}

int tdav_codec_h263_close(tmedia_codec_t* self)
{
	return 0;
}

tsk_size_t tdav_codec_h263_fmtp_encode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data)
{
	return 0;
}

tsk_size_t tdav_codec_h263_fmtp_decode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data)
{
	return 0;
}

tsk_bool_t tdav_codec_h263_fmtp_match(const tmedia_codec_t* codec, const char* fmtp)
{	
	/* check whether we can match this fmtp with our local
	* check size, maxbr, fps ...*/
	return tsk_true;
}

char* tdav_codec_h263_fmtp_get(const tmedia_codec_t* self)
{
	return tsk_strdup("CIF=2/MaxBR=3840;QCIF=2/MaxBR=1920");
}

int tdav_codec_h263_fmtp_set(tmedia_codec_t* self, const char* fmtp)
{
	TSK_DEBUG_INFO("remote fmtp=%s", fmtp);
	return 0;
}

/* constructor */
static tsk_object_t* tdav_codec_h263_ctor(tsk_object_t * self, va_list * app)
{
	tdav_codec_h263_t *h263 = self;
	if(h263){
		/* init base: called by tmedia_codec_create() */
		/* init self */
	}
	return self;
}
/* destructor */
static tsk_object_t* tdav_codec_h263_dtor(tsk_object_t * self)
{ 
	tdav_codec_h263_t *h263 = self;
	if(h263){
		/* deinit base */
		tmedia_codec_video_deinit(h263);
		/* deinit self */
	}

	return self;
}
/* object definition */
static const tsk_object_def_t tdav_codec_h263_def_s = 
{
	sizeof(tdav_codec_h263_t),
	tdav_codec_h263_ctor, 
	tdav_codec_h263_dtor,
	tmedia_codec_cmp, 
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t tdav_codec_h263_plugin_def_s = 
{
	&tdav_codec_h263_def_s,

	tmedia_video,
	"H263",
	"H263-1996 codec",
	TMEDIA_CODEC_FORMAT_H263,
	tsk_false,
	90000, // rate
	
	/* audio */
	{ 0 },

	/* video */
	{176, 144},

	tdav_codec_h263_open,
	tdav_codec_h263_close,
	tdav_codec_h263_fmtp_encode,
	tdav_codec_h263_fmtp_decode,
	tdav_codec_h263_fmtp_match,
	tdav_codec_h263_fmtp_get,
	tdav_codec_h263_fmtp_set
};
const tmedia_codec_plugin_def_t *tdav_codec_h263_plugin_def_t = &tdav_codec_h263_plugin_def_s;























/* ============ H.263-1998 Plugin interface ================= */

//
//	H.263-1998 object definition
//

int tdav_codec_h263p_open(tmedia_codec_t* self)
{
	return 0;
}

int tdav_codec_h263p_close(tmedia_codec_t* self)
{
	return 0;
}

tsk_size_t tdav_codec_h263p_fmtp_encode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data)
{
	return 0;
}

tsk_size_t tdav_codec_h263p_fmtp_decode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data)
{
	return 0;
}

tsk_bool_t tdav_codec_h263p_fmtp_match(const tmedia_codec_t* codec, const char* fmtp)
{	
	/* check whether we can match this fmtp with our local
	* check size, maxbr, fps ...*/
	return tsk_true;
}

char* tdav_codec_h263p_fmtp_get(const tmedia_codec_t* self)
{
	return tsk_strdup("CIF=2/MaxBR=3840;QCIF=2/MaxBR=1920");
}

int tdav_codec_h263p_fmtp_set(tmedia_codec_t* self, const char* fmtp)
{
	TSK_DEBUG_INFO("remote fmtp=%s", fmtp);
	return 0;
}

/* constructor */
static tsk_object_t* tdav_codec_h263p_ctor(tsk_object_t * self, va_list * app)
{
	tdav_codec_h263p_t *h263p = self;
	if(h263p){
		/* init base: called by tmedia_codec_create() */
		/* init self */
	}
	return self;
}
/* destructor */
static tsk_object_t* tdav_codec_h263p_dtor(tsk_object_t * self)
{ 
	tdav_codec_h263p_t *h263p = self;
	if(h263p){
		/* deinit base */
		tmedia_codec_video_deinit(h263p);
		/* deinit self */
	}

	return self;
}
/* object definition */
static const tsk_object_def_t tdav_codec_h263p_def_s = 
{
	sizeof(tdav_codec_h263p_t),
	tdav_codec_h263p_ctor, 
	tdav_codec_h263p_dtor,
	tmedia_codec_cmp, 
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t tdav_codec_h263p_plugin_def_s = 
{
	&tdav_codec_h263p_def_s,

	tmedia_video,
	"H263-1998",
	"H263-1998 codec",
	TMEDIA_CODEC_FORMAT_H263_1998,
	tsk_true,
	90000, // rate
	
	/* audio */
	{ 0 },

	/* video */
	{176, 144},

	tdav_codec_h263p_open,
	tdav_codec_h263p_close,
	tdav_codec_h263p_fmtp_encode,
	tdav_codec_h263p_fmtp_decode,
	tdav_codec_h263p_fmtp_match,
	tdav_codec_h263p_fmtp_get,
	tdav_codec_h263p_fmtp_set
};
const tmedia_codec_plugin_def_t *tdav_codec_h263p_plugin_def_t = &tdav_codec_h263p_plugin_def_s;














/* ============ H.263-2000 Plugin interface ================= */

//
//	H.263-2000 object definition
//

int tdav_codec_h263pp_open(tmedia_codec_t* self)
{
	return 0;
}

int tdav_codec_h263pp_close(tmedia_codec_t* self)
{
	return 0;
}

tsk_size_t tdav_codec_h263pp_fmtp_encode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data)
{
	return 0;
}

tsk_size_t tdav_codec_h263pp_fmtp_decode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data)
{
	return 0;
}

tsk_bool_t tdav_codec_h263pp_fmtp_match(const tmedia_codec_t* codec, const char* fmtp)
{	
	/* check whether we can match this fmtp with our local
	* check size, maxbr, fps ...*/
	return tsk_true;
}

char* tdav_codec_h263pp_fmtp_get(const tmedia_codec_t* self)
{
	return tsk_strdup("CIF=2/MaxBR=3840;QCIF=2/MaxBR=1920");
}

int tdav_codec_h263pp_fmtp_set(tmedia_codec_t* self, const char* fmtp)
{
	TSK_DEBUG_INFO("remote fmtp=%s", fmtp);
	return 0;
}

/* constructor */
static tsk_object_t* tdav_codec_h263pp_ctor(tsk_object_t * self, va_list * app)
{
	tdav_codec_h263pp_t *h263pp = self;
	if(h263pp){
		/* init base: called by tmedia_codec_create() */
		/* init self */
	}
	return self;
}
/* destructor */
static tsk_object_t* tdav_codec_h263pp_dtor(tsk_object_t * self)
{ 
	tdav_codec_h263pp_t *h263pp = self;
	if(h263pp){
		/* deinit base */
		tmedia_codec_video_deinit(h263pp);
		/* deinit self */
	}

	return self;
}
/* object definition */
static const tsk_object_def_t tdav_codec_h263pp_def_s = 
{
	sizeof(tdav_codec_h263pp_t),
	tdav_codec_h263pp_ctor, 
	tdav_codec_h263pp_dtor,
	tmedia_codec_cmp, 
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t tdav_codec_h263pp_plugin_def_s = 
{
	&tdav_codec_h263pp_def_s,

	tmedia_video,
	"H263-2000",
	"H263-2000 codec",
	TMEDIA_CODEC_FORMAT_H263_2000,
	tsk_true,
	90000, // rate
	
	/* audio */
	{ 0 },

	/* video */
	{176, 144},
	tdav_codec_h263pp_open,
	tdav_codec_h263pp_close,
	tdav_codec_h263pp_fmtp_encode,
	tdav_codec_h263pp_fmtp_decode,
	tdav_codec_h263pp_fmtp_match,
	tdav_codec_h263pp_fmtp_get,
	tdav_codec_h263pp_fmtp_set
};
const tmedia_codec_plugin_def_t *tdav_codec_h263pp_plugin_def_t = &tdav_codec_h263pp_plugin_def_s;
