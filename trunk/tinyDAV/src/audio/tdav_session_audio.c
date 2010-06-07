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

/**@file tdav_session_audio.c
 * @brief Audio Session plugin.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinydav/audio/tdav_session_audio.h"

#include "tsk_memory.h"
#include "tsk_debug.h"

extern const tmedia_codec_t* _tmedia_session_match_codec(tmedia_session_t* self, const tsdp_header_M_t* M, char** format);

/* ============ Plugin interface ================= */

int tdav_session_audio_prepare(tmedia_session_t* self)
{
	tdav_session_audio_t* audio;

	TSK_DEBUG_INFO("tdav_session_audio_prepare");

	audio = (tdav_session_audio_t*)self;

	/* set local port */
	audio->local_port = rand() ^ rand();

	return 0;
}

int tdav_session_audio_start(tmedia_session_t* self)
{
	TSK_DEBUG_INFO("tdav_session_audio_start");
	return 0;
}

int tdav_session_audio_stop(tmedia_session_t* self)
{
	tdav_session_audio_t* audio;

	TSK_DEBUG_INFO("tdav_session_audio_stop");

	audio = (tdav_session_audio_t*)self;

	/* very important */
	audio->local_port = 0;

	return 0;
}

int tdav_session_audio_pause(tmedia_session_t* self)
{
	TSK_DEBUG_INFO("tdav_session_audio_pause");
	return 0;
}

const tsdp_header_M_t* tdav_session_audio_get_lo(tmedia_session_t* self)
{
	tdav_session_audio_t* audio;

	TSK_DEBUG_INFO("tdav_session_audio_get_lo");

	if(!self || !self->plugin){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}

	audio = (tdav_session_audio_t*)self;

	if(self->ro_changed && self->M.lo){
		TSK_OBJECT_SAFE_FREE(self->M.lo);
	}

	if(self->M.lo){
		return self->M.lo;
	}
	else if(!(self->M.lo = tsdp_header_M_create(self->plugin->media, audio->local_port, "RTP/AVP"))){
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

int tdav_session_audio_set_ro(tmedia_session_t* self, const tsdp_header_M_t* m)
{
	const tmedia_codec_t* codec;

	TSK_DEBUG_INFO("tdav_session_audio_set_ro");

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





//=================================================================================================
//	Session Audio Plugin object definition
//
/* constructor */
static tsk_object_t* tdav_session_audio_ctor(tsk_object_t * self, va_list * app)
{
	tdav_session_audio_t *session = self;
	if(session){
		/* init base: called by tmedia_session_create() */
		/* init self */
	}
	return self;
}
/* destructor */
static tsk_object_t* tdav_session_audio_dtor(tsk_object_t * self)
{ 
	tdav_session_audio_t *session = self;
	if(session){
		/* deinit base */
		tmedia_session_deinit(self);
		/* deinit self */
	}

	return self;
}
/* object definition */
static const tsk_object_def_t tdav_session_audio_def_s = 
{
	sizeof(tdav_session_audio_t),
	tdav_session_audio_ctor, 
	tdav_session_audio_dtor,
	tmedia_session_cmp, 
};
/* plugin definition*/
static const tmedia_session_plugin_def_t tdav_session_audio_plugin_def_s = 
{
	&tdav_session_audio_def_s,
	
	tmedia_audio,
	"audio",
	
	tdav_session_audio_prepare,
	tdav_session_audio_start,
	tdav_session_audio_stop,
	tdav_session_audio_pause,

	tdav_session_audio_get_lo,
	tdav_session_audio_set_ro
};
const tmedia_session_plugin_def_t *tdav_session_audio_plugin_def_t = &tdav_session_audio_plugin_def_s;