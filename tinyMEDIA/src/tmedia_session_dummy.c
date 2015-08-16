/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinymedia/tmedia_session_dummy.h"

#include "tsk_memory.h"
#include "tsk_debug.h"

/* ============ Audio Session ================= */

int tmedia_session_daudio_set(tmedia_session_t* self, const tmedia_param_t* param)
{
	tmedia_session_daudio_t* daudio;

	daudio = (tmedia_session_daudio_t*)self;

	return 0;
}

int tmedia_session_daudio_get(tmedia_session_t* self, tmedia_param_t* param)
{
	return 0;
}

int tmedia_session_daudio_prepare(tmedia_session_t* self)
{
	tmedia_session_daudio_t* daudio;

	daudio = (tmedia_session_daudio_t*)self;

	/* set local port */
	daudio->local_port = rand() ^ rand();

	return 0;
}

int tmedia_session_daudio_start(tmedia_session_t* self)
{
	return 0;
}

int tmedia_session_daudio_stop(tmedia_session_t* self)
{
	tmedia_session_daudio_t* daudio;

	daudio = (tmedia_session_daudio_t*)self;

	/* very important */
	daudio->local_port = 0;

	return 0;
}

int tmedia_session_daudio_send_dtmf(tmedia_session_t* self, uint8_t event)
{
	return 0;
}

int tmedia_session_daudio_pause(tmedia_session_t* self)
{
	return 0;
}

const tsdp_header_M_t* tmedia_session_daudio_get_lo(tmedia_session_t* self)
{
	tmedia_session_daudio_t* daudio;
	tsk_bool_t changed = tsk_false;

	if(!self || !self->plugin){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}

	daudio = (tmedia_session_daudio_t*)self;

	if(self->ro_changed && self->M.lo){
		/* Codecs */
		tsdp_header_A_removeAll_by_field(self->M.lo->Attributes, "fmtp");
		tsdp_header_A_removeAll_by_field(self->M.lo->Attributes, "rtpmap");
		tsk_list_clear_items(self->M.lo->FMTs);
		
		/* QoS */
		tsdp_header_A_removeAll_by_field(self->M.lo->Attributes, "curr");
		tsdp_header_A_removeAll_by_field(self->M.lo->Attributes, "des");
		tsdp_header_A_removeAll_by_field(self->M.lo->Attributes, "conf");
	}

	changed = (self->ro_changed || !self->M.lo);
	
	if(!self->M.lo && !(self->M.lo = tsdp_header_M_create(self->plugin->media, daudio->local_port, "RTP/AVP"))){
		TSK_DEBUG_ERROR("Failed to create lo");
		return tsk_null;
	}

	if(changed){
		/* from codecs to sdp */
		tmedia_codec_to_sdp(self->neg_codecs ? self->neg_codecs : self->codecs, self->M.lo);
		/* QoS */
		if(self->qos){
			tmedia_qos_tline_t* ro_tline;
			if(self->M.ro && (ro_tline = tmedia_qos_tline_from_sdp(self->M.ro))){
				tmedia_qos_tline_set_ro(self->qos, ro_tline);
				TSK_OBJECT_SAFE_FREE(ro_tline);
			}
			tmedia_qos_tline_to_sdp(self->qos, self->M.lo);
		}
	}
	

	return self->M.lo;
}

int tmedia_session_daudio_set_ro(tmedia_session_t* self, const tsdp_header_M_t* m)
{
	tmedia_codecs_L_t* neg_codecs;

	if((neg_codecs = tmedia_session_match_codec(self, m))){
		/* update negociated codecs */
		TSK_OBJECT_SAFE_FREE(self->neg_codecs);
		self->neg_codecs = neg_codecs;
		/* update remote offer */
		TSK_OBJECT_SAFE_FREE(self->M.ro);
		self->M.ro = tsk_object_ref((void*)m);
		
		return 0;
	}
	return -1;
}

/* ============ Video Session ================= */

int tmedia_session_dvideo_set(tmedia_session_t* self, const tmedia_param_t* param)
{
	tmedia_session_dvideo_t* dvideo;

	dvideo = (tmedia_session_dvideo_t*)self;

	return 0;
}

int tmedia_session_dvideo_get(tmedia_session_t* self, tmedia_param_t* param)
{
	return 0;
}

int tmedia_session_dvideo_prepare(tmedia_session_t* self)
{
	tmedia_session_dvideo_t* dvideo;

	dvideo = (tmedia_session_dvideo_t*)self;

	/* set local port */
	dvideo->local_port = rand() ^ rand();

	return 0;
}

int tmedia_session_dvideo_start(tmedia_session_t* self)
{
	return -1;
}

int tmedia_session_dvideo_stop(tmedia_session_t* self)
{
	tmedia_session_dvideo_t* dvideo;

	dvideo = (tmedia_session_dvideo_t*)self;

	/* very important */
	dvideo->local_port = 0;

	return 0;
}

int tmedia_session_dvideo_pause(tmedia_session_t* self)
{
	return -1;
}

const tsdp_header_M_t* tmedia_session_dvideo_get_lo(tmedia_session_t* self)
{
	tmedia_session_dvideo_t* dvideo;
	tsk_bool_t changed = tsk_false;

	if(!self || !self->plugin){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}

	dvideo = (tmedia_session_dvideo_t*)self;

	if(self->ro_changed && self->M.lo){
		/* Codecs */
		tsdp_header_A_removeAll_by_field(self->M.lo->Attributes, "fmtp");
		tsdp_header_A_removeAll_by_field(self->M.lo->Attributes, "rtpmap");
		tsk_list_clear_items(self->M.lo->FMTs);
		
		/* QoS */
		tsdp_header_A_removeAll_by_field(self->M.lo->Attributes, "curr");
		tsdp_header_A_removeAll_by_field(self->M.lo->Attributes, "des");
		tsdp_header_A_removeAll_by_field(self->M.lo->Attributes, "conf");
	}

	changed = (self->ro_changed || !self->M.lo);
	
	if(!self->M.lo && !(self->M.lo = tsdp_header_M_create(self->plugin->media, dvideo->local_port, "RTP/AVP"))){
		TSK_DEBUG_ERROR("Failed to create lo");
		return tsk_null;
	}

	if(changed){
		/* from codecs to sdp */
		tmedia_codec_to_sdp(self->neg_codecs ? self->neg_codecs : self->codecs, self->M.lo);
		/* QoS */
		if(self->qos){
			tmedia_qos_tline_t* ro_tline;
			if(self->M.ro && (ro_tline = tmedia_qos_tline_from_sdp(self->M.ro))){
				tmedia_qos_tline_set_ro(self->qos, ro_tline);
				TSK_OBJECT_SAFE_FREE(ro_tline);
			}
			tmedia_qos_tline_to_sdp(self->qos, self->M.lo);
		}
	}

	return self->M.lo;
}

int tmedia_session_dvideo_set_ro(tmedia_session_t* self, const tsdp_header_M_t* m)
{
	tmedia_codecs_L_t* neg_codecs;

	if((neg_codecs = tmedia_session_match_codec(self, m))){
		/* update negociated codecs */
		TSK_OBJECT_SAFE_FREE(self->neg_codecs);
		self->neg_codecs = neg_codecs;
		/* update remote offer */
		TSK_OBJECT_SAFE_FREE(self->M.ro);
		self->M.ro = tsk_object_ref((void*)m);
		
		return 0;
	}
	return -1;
}

/* ============ Msrp Session ================= */

int tmedia_session_dmsrp_set(tmedia_session_t* self, const tmedia_param_t* param)
{
	tmedia_session_dmsrp_t* dmsrp;

	dmsrp = (tmedia_session_dmsrp_t*)self;

	return 0;
}

int tmedia_session_dmsrp_get(tmedia_session_t* self, tmedia_param_t* param)
{
	return 0;
}

int tmedia_session_dmsrp_prepare(tmedia_session_t* self)
{
	return 0;
}

int tmedia_session_dmsrp_start(tmedia_session_t* self)
{
	return 0;
}

int tmedia_session_dmsrp_stop(tmedia_session_t* self)
{
	return 0;
}

int tmedia_session_dmsrp_pause(tmedia_session_t* self)
{
	return 0;
}

const tsdp_header_M_t* tmedia_session_dmsrp_get_lo(tmedia_session_t* self)
{
	if(self->ro_changed){
		TSK_OBJECT_SAFE_FREE(self->M.lo);
	}

	return tsk_null;
}

int tmedia_session_dmsrp_set_ro(tmedia_session_t* self, const tsdp_header_M_t* m)
{
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
	
	tmedia_session_daudio_set,
	tmedia_session_daudio_get,
	tmedia_session_daudio_prepare,
	tmedia_session_daudio_start,
	tmedia_session_daudio_pause,
	tmedia_session_daudio_stop,
	
	/* Audio part */
	{ tsk_null },

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
	
	tmedia_session_dvideo_set,
	tmedia_session_dvideo_get,
	tmedia_session_dvideo_prepare,
	tmedia_session_dvideo_start,
	tmedia_session_dvideo_pause,
	tmedia_session_dvideo_stop,

	/* Audio part */
	{ tsk_null },

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
	
	tmedia_session_dmsrp_set,
	tmedia_session_dmsrp_get,
	tmedia_session_dmsrp_prepare,
	tmedia_session_dmsrp_start,
	tmedia_session_dmsrp_pause,
	tmedia_session_dmsrp_stop,
	
	/* Audio part */
	{ tsk_null },

	tmedia_session_dmsrp_get_lo,
	tmedia_session_dmsrp_set_ro
};
const tmedia_session_plugin_def_t *tmedia_session_dmsrp_plugin_def_t = &tmedia_session_dmsrp_plugin_def_s;
