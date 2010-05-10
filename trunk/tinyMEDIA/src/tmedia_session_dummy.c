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

/**@file tmedia_session_dummy.c
 * @brief Dummy sessions used for test only.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinymedia/tmedia_session_dummy.h"

#include "tsk_memory.h"
#include "tsk_debug.h"

extern int _tmedia_session_load_codecs(tmedia_session_t* self);

/* ============ Audio Session ================= */

int tmedia_session_daudio_prepare(tmedia_session_t* self)
{
	int ret;
	tmedia_session_daudio_t* daudio;

	TSK_DEBUG_INFO("tmedia_session_daudio_prepare");

	daudio = (tmedia_session_daudio_t*)self;

	if((ret = _tmedia_session_load_codecs(self))){
		TSK_DEBUG_ERROR("Failed to prepare audio session");
		return ret;
	}

	/* set local port */
	daudio->local_port = rand() ^ rand();

	return ret;
}

int tmedia_session_daudio_start(tmedia_session_t* self)
{
	TSK_DEBUG_INFO("tmedia_session_daudio_start");
	return 0;
}

int tmedia_session_daudio_stop(tmedia_session_t* self)
{
	TSK_DEBUG_INFO("tmedia_session_daudio_stop");
	return 0;
}

int tmedia_session_daudio_pause(tmedia_session_t* self)
{
	TSK_DEBUG_INFO("tmedia_session_daudio_pause");
	return 0;
}

const tsdp_header_M_t* tmedia_session_daudio_get_lo(tmedia_session_t* self)
{
	const tsk_list_item_t* item;
	const tmedia_codec_t* codec;
	char *fmtp, *rtpmap;
	tmedia_session_daudio_t* daudio;

	TSK_DEBUG_INFO("tmedia_session_daudio_get_lo");

	if(!self || !self->plugin){
		TSK_DEBUG_ERROR("Invalid parameter");
		return self->M.lo;
	}

	daudio = (tmedia_session_daudio_t*)self;

	if(self->M.lo){
		return self->M.lo;
	}
	else if(!(self->M.lo = tsdp_header_M_create(self->plugin->media, daudio->local_port, "RTP/AVP"))){
		TSK_DEBUG_ERROR("Failed to create lo");
		return tsk_null;
	}

	tsk_list_foreach(item, self->codecs){
		codec = item->data;
		
		/* add fmt */
		if(tsdp_header_M_add_fmt(self->M.lo, codec->format)){
			continue;
		}
		/* add rtpmap attribute */
		if((rtpmap = tmedia_codec_get_rtpmap(codec))){
			tsdp_header_M_add_headers(self->M.lo,
			TSDP_HEADER_A_VA_ARGS("rtpmap", rtpmap),
			tsk_null);
			TSK_FREE(rtpmap);
		}
		/* add fmtp attribute */
		if((fmtp = tmedia_codec_get_fmtp(codec))){
			tsdp_header_M_add_headers(self->M.lo,
			TSDP_HEADER_A_VA_ARGS("fmtp", fmtp),
			tsk_null);
			TSK_FREE(fmtp);
		}
	}

	return self->M.lo;
}

const tsdp_header_M_t* tmedia_session_daudio_get_no(tmedia_session_t* self)
{
	TSK_DEBUG_INFO("tmedia_session_daudio_get_no");
	return tsk_null;
}

int tmedia_session_daudio_set_ro(tmedia_session_t* self, const tsdp_header_M_t* m)
{
	TSK_DEBUG_INFO("tmedia_session_daudio_set_ro");
	return tsk_null;
}

/* ============ Video Session ================= */

int tmedia_session_dvideo_prepare(tmedia_session_t* self)
{
	TSK_DEBUG_INFO("tmedia_session_dvideo_prepare");
	return 0;
}

int tmedia_session_dvideo_start(tmedia_session_t* self)
{
	TSK_DEBUG_INFO("tmedia_session_dvideo_start");
	return 0;
}

int tmedia_session_dvideo_stop(tmedia_session_t* self)
{
	TSK_DEBUG_INFO("tmedia_session_dvideo_stop");
	return 0;
}

int tmedia_session_dvideo_pause(tmedia_session_t* self)
{
	TSK_DEBUG_INFO("tmedia_session_dvideo_pause");
	return 0;
}

const tsdp_header_M_t* tmedia_session_dvideo_get_lo(tmedia_session_t* self)
{
	TSK_DEBUG_INFO("tmedia_session_dvideo_get_lo");
	return tsk_null;
}

const tsdp_header_M_t* tmedia_session_dvideo_get_no(tmedia_session_t* self)
{
	TSK_DEBUG_INFO("tmedia_session_dvideo_get_no");
	return tsk_null;
}

int tmedia_session_dvideo_set_ro(tmedia_session_t* self, const tsdp_header_M_t* m)
{
	TSK_DEBUG_INFO("tmedia_session_dvideo_set_ro");
	return tsk_null;
}

/* ============ Msrp Session ================= */

int tmedia_session_dmsrp_prepare(tmedia_session_t* self)
{
	TSK_DEBUG_INFO("tmedia_session_dmsrp_prepare");
	return 0;
}

int tmedia_session_dmsrp_start(tmedia_session_t* self)
{
	TSK_DEBUG_INFO("tmedia_session_dmsrp_start");
	return 0;
}

int tmedia_session_dmsrp_stop(tmedia_session_t* self)
{
	TSK_DEBUG_INFO("tmedia_session_dmsrp_stop");
	return 0;
}

int tmedia_session_dmsrp_pause(tmedia_session_t* self)
{
	TSK_DEBUG_INFO("tmedia_session_dmsrp_pause");
	return 0;
}

const tsdp_header_M_t* tmedia_session_dmsrp_get_lo(tmedia_session_t* self)
{
	TSK_DEBUG_INFO("tmedia_session_dmsrp_get_lo");
	return tsk_null;
}

const tsdp_header_M_t* tmedia_session_dmsrp_get_no(tmedia_session_t* self)
{
	TSK_DEBUG_INFO("tmedia_session_dmsrp_get_no");
	return tsk_null;
}

int tmedia_session_dmsrp_set_ro(tmedia_session_t* self, const tsdp_header_M_t* m)
{
	TSK_DEBUG_INFO("tmedia_session_dmsrp_set_ro");
	return tsk_null;
}

//=================================================================================================
//	Dummy Audio session object definition
//
/* constructor */
static tsk_object_t* tmedia_session_daudio_ctor(tsk_object_t * self, va_list * app)
{
	tmedia_session_daudio_t *session = self;
	if(session){
		/* init base */
		/* init self */
	}
	return self;
}
/* destructor */
static tsk_object_t* tmedia_session_daudio_dtor(tsk_object_t * self)
{ 
	tmedia_session_daudio_t *session = self;
	if(session){
		/* deinit base */
		/* deinit self */
	}

	return self;
}
/* object definition */
static const tsk_object_def_t tmedia_session_daudio_def_s = 
{
	sizeof(tmedia_session_daudio_t),
	tmedia_session_daudio_ctor, 
	tmedia_session_daudio_dtor,
	tmedia_codec_cmp, 
};
/* plugin definition*/
static const tmedia_session_plugin_def_t tmedia_session_daudio_plugin_def_s = 
{
	&tmedia_session_daudio_def_s,
	
	tmedia_audio,
	"audio",
	
	tmedia_session_daudio_prepare,
	tmedia_session_daudio_start,
	tmedia_session_daudio_stop,
	tmedia_session_daudio_pause,

	tmedia_session_daudio_get_lo,
	tmedia_session_daudio_get_no,
	tmedia_session_daudio_set_ro
};
const tmedia_session_plugin_def_t *tmedia_session_daudio_plugin_def_t = &tmedia_session_daudio_plugin_def_s;


//=================================================================================================
//	Dummy Video session object definition
//
/* constructor */
static tsk_object_t* tmedia_session_dvideo_ctor(tsk_object_t * self, va_list * app)
{
	tmedia_session_dvideo_t *session = self;
	if(session){
		/* init base */
		/* init self */
	}
	return self;
}
/* destructor */
static tsk_object_t* tmedia_session_dvideo_dtor(tsk_object_t * self)
{ 
	tmedia_session_dvideo_t *session = self;
	if(session){
		/* deinit base */
		/* deinit self */
	}

	return self;
}
/* object definition */
static const tsk_object_def_t tmedia_session_dvideo_def_s = 
{
	sizeof(tmedia_session_dvideo_t),
	tmedia_session_dvideo_ctor, 
	tmedia_session_dvideo_dtor,
	tmedia_codec_cmp, 
};
/* plugin definition*/
static const tmedia_session_plugin_def_t tmedia_session_dvideo_plugin_def_s = 
{
	&tmedia_session_dvideo_def_s,
	
	tmedia_video,
	"video",
	
	tmedia_session_dvideo_prepare,
	tmedia_session_dvideo_start,
	tmedia_session_dvideo_stop,
	tmedia_session_dvideo_pause,

	tmedia_session_dvideo_get_lo,
	tmedia_session_dvideo_get_no,
	tmedia_session_dvideo_set_ro
};
const tmedia_session_plugin_def_t *tmedia_session_dvideo_plugin_def_t = &tmedia_session_dvideo_plugin_def_s;


//=================================================================================================
//	Dummy Msrp session object definition
//
/* constructor */
static tsk_object_t* tmedia_session_dmsrp_ctor(tsk_object_t * self, va_list * app)
{
	tmedia_session_dmsrp_t *session = self;
	if(session){
		/* init base */
		/* init self */
	}
	return self;
}
/* destructor */
static tsk_object_t* tmedia_session_dmsrp_dtor(tsk_object_t * self)
{ 
	tmedia_session_dmsrp_t *session = self;
	if(session){
		/* deinit base */
		/* deinit self */
	}

	return self;
}
/* object definition */
static const tsk_object_def_t tmedia_session_dmsrp_def_s = 
{
	sizeof(tmedia_session_dmsrp_t),
	tmedia_session_dmsrp_ctor, 
	tmedia_session_dmsrp_dtor,
	tmedia_codec_cmp, 
};
/* plugin definition*/
static const tmedia_session_plugin_def_t tmedia_session_dmsrp_plugin_def_s = 
{
	&tmedia_session_dmsrp_def_s,
	
	tmedia_msrp,
	"message",
	
	tmedia_session_dmsrp_prepare,
	tmedia_session_dmsrp_start,
	tmedia_session_dmsrp_stop,
	tmedia_session_dmsrp_pause,

	tmedia_session_dmsrp_get_lo,
	tmedia_session_dmsrp_get_no,
	tmedia_session_dmsrp_set_ro
};
const tmedia_session_plugin_def_t *tmedia_session_dmsrp_plugin_def_t = &tmedia_session_dmsrp_plugin_def_s;
