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

/* ============ Audio Session ================= */

int tmedia_session_daudio_prepare(tmedia_session_t* self)
{
	tmedia_session_daudio_t* daudio;

	TSK_DEBUG_INFO("tmedia_session_daudio_prepare");

	daudio = (tmedia_session_daudio_t*)self;

	/* set local port */
	daudio->local_port = rand() ^ rand();

	return 0;
}

int tmedia_session_daudio_start(tmedia_session_t* self)
{
	TSK_DEBUG_INFO("tmedia_session_daudio_start");
	return 0;
}

int tmedia_session_daudio_stop(tmedia_session_t* self)
{
	tmedia_session_daudio_t* daudio;

	TSK_DEBUG_INFO("tmedia_session_daudio_stop");

	daudio = (tmedia_session_daudio_t*)self;

	/* very important */
	daudio->local_port = 0;

	return 0;
}

int tmedia_session_daudio_pause(tmedia_session_t* self)
{
	TSK_DEBUG_INFO("tmedia_session_daudio_pause");
	return 0;
}

const tsdp_header_M_t* tmedia_session_daudio_get_lo(tmedia_session_t* self)
{
	tmedia_session_daudio_t* daudio;

	TSK_DEBUG_INFO("tmedia_session_daudio_get_lo");

	if(!self || !self->plugin){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}

	daudio = (tmedia_session_daudio_t*)self;

	if(self->ro_changed && self->M.lo){
		TSK_OBJECT_SAFE_FREE(self->M.lo);
	}

	if(self->M.lo){
		return self->M.lo;
	}
	else if(!(self->M.lo = tsdp_header_M_create(self->plugin->media, daudio->local_port, "RTP/AVP"))){
		TSK_DEBUG_ERROR("Failed to create lo");
		return tsk_null;
	}

	/* from codecs to sdp */
	if(self->negociated_codec && !TSK_LIST_IS_EMPTY(self->codecs)){
		/* filter codecs */
		tmedia_codec_removeAll_exceptThis(self->codecs, self->negociated_codec);
		/* update format */
		if(self->negociated_codec->dyn){
			tmedia_codec_to_sdp_2(self->codecs->head->data, self->M.lo, self->negociated_format);
		}
		else{
			tmedia_codec_to_sdp_2(self->codecs->head->data, self->M.lo, tsk_null);
		}
	}
	else{
		tmedia_codec_to_sdp(self->codecs, self->M.lo);
	}

	return self->M.lo;
}

int tmedia_session_daudio_set_ro(tmedia_session_t* self, const tsdp_header_M_t* m)
{
	const tmedia_codec_t* codec;

	TSK_DEBUG_INFO("tmedia_session_daudio_set_ro");

	if((codec = tmedia_session_match_codec(self, m, &self->negociated_format))){
		/* update negociated codec */
		TSK_OBJECT_SAFE_FREE(self->negociated_codec);
		self->negociated_codec = tsk_object_ref((void*)codec);
		/* update remote offer */
		TSK_OBJECT_SAFE_FREE(self->M.ro);
		self->M.ro = tsk_object_ref((void*)m);
		
		return 0;
	}
	return -1;
}

/* ============ Video Session ================= */

int tmedia_session_dvideo_prepare(tmedia_session_t* self)
{
	tmedia_session_dvideo_t* dvideo;

	TSK_DEBUG_INFO("tmedia_session_dvideo_prepare");

	dvideo = (tmedia_session_dvideo_t*)self;

	/* set local port */
	dvideo->local_port = rand() ^ rand();

	return 0;
}

int tmedia_session_dvideo_start(tmedia_session_t* self)
{
	TSK_DEBUG_INFO("tmedia_session_dvideo_start");
	return -1;
}

int tmedia_session_dvideo_stop(tmedia_session_t* self)
{
	tmedia_session_dvideo_t* dvideo;

	TSK_DEBUG_INFO("tmedia_session_dvideo_stop");

	dvideo = (tmedia_session_dvideo_t*)self;

	/* very important */
	dvideo->local_port = 0;

	return 0;
}

int tmedia_session_dvideo_pause(tmedia_session_t* self)
{
	TSK_DEBUG_INFO("tmedia_session_dvideo_pause");
	return -1;
}

const tsdp_header_M_t* tmedia_session_dvideo_get_lo(tmedia_session_t* self)
{
	tmedia_session_dvideo_t* dvideo;

	TSK_DEBUG_INFO("tmedia_session_dvideo_get_lo");

	if(!self || !self->plugin){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}

	dvideo = (tmedia_session_dvideo_t*)self;

	if(self->ro_changed && self->M.lo){
		TSK_OBJECT_SAFE_FREE(self->M.lo);
	}

	if(self->M.lo){
		return self->M.lo;
	}
	else if(!(self->M.lo = tsdp_header_M_create(self->plugin->media, dvideo->local_port, "RTP/AVP"))){
		TSK_DEBUG_ERROR("Failed to create lo");
		return tsk_null;
	}

	/* from codecs to sdp */
	if(self->negociated_codec && !TSK_LIST_IS_EMPTY(self->codecs)){
		/* filter codecs */
		tmedia_codec_removeAll_exceptThis(self->codecs, self->negociated_codec);
		/* update format */
		if(self->negociated_codec->dyn){
			tmedia_codec_to_sdp_2(self->codecs->head->data, self->M.lo, self->negociated_format);
		}
		else{
			tmedia_codec_to_sdp_2(self->codecs->head->data, self->M.lo, tsk_null);
		}
	}
	else{
		tmedia_codec_to_sdp(self->codecs, self->M.lo);
	}	

	return self->M.lo;
}

int tmedia_session_dvideo_set_ro(tmedia_session_t* self, const tsdp_header_M_t* m)
{
	const tmedia_codec_t* codec;

	TSK_DEBUG_INFO("tmedia_session_dvideo_set_ro");

	if((codec = tmedia_session_match_codec(self, m, &self->negociated_format))){
		/* update negociated codec */
		TSK_OBJECT_SAFE_FREE(self->negociated_codec);
		self->negociated_codec = tsk_object_ref((void*)codec);
		/* update remote offer */
		TSK_OBJECT_SAFE_FREE(self->M.ro);
		self->M.ro = tsk_object_ref((void*)m);
		
		return 0;
	}
	return -1;
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

	if(self->ro_changed){
		TSK_OBJECT_SAFE_FREE(self->M.lo);
	}

	return tsk_null;
}

int tmedia_session_dmsrp_set_ro(tmedia_session_t* self, const tsdp_header_M_t* m)
{
	TSK_DEBUG_INFO("tmedia_session_dmsrp_set_ro");
	return -1;
}

//=================================================================================================
//	Dummy Audio session object definition
//
/* constructor */
static tsk_object_t* tmedia_session_daudio_ctor(tsk_object_t * self, va_list * app)
{
	tmedia_session_daudio_t *session = self;
	if(session){
		/* init base: called by tmedia_session_create() */
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
		tmedia_session_deinit(self);
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
	tmedia_session_cmp, 
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
		/* init base: called by tmedia_session_create() */
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
		tmedia_session_deinit(self);
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
	tmedia_session_cmp, 
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
		/* init base: called by tmedia_session_create() */
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
		tmedia_session_deinit(self);
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
	tmedia_session_cmp, 
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
	tmedia_session_dmsrp_set_ro
};
const tmedia_session_plugin_def_t *tmedia_session_dmsrp_plugin_def_t = &tmedia_session_dmsrp_plugin_def_s;
