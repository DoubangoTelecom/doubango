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

/**@file tmedia_session_ghost.c
 * @brief Ghost session.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinymedia/tmedia_session_ghost.h"

#include "tsk_memory.h"
#include "tsk_debug.h"

/* ============ Ghost Session ================= */

static int tmedia_session_ghost_prepare(tmedia_session_t* self)
{
	return 0;
}

static int tmedia_session_ghost_start(tmedia_session_t* self)
{
	return 0;
}

static int tmedia_session_ghost_stop(tmedia_session_t* self)
{
	TSK_DEBUG_INFO("tmedia_session_ghost_stop");
	return 0;
}

static int tmedia_session_ghost_pause(tmedia_session_t* self)
{
	return 0;
}

static const tsdp_header_M_t* tmedia_session_ghost_get_lo(tmedia_session_t* self)
{
	tmedia_session_ghost_t* ghost;

	ghost = (tmedia_session_ghost_t*)self;

	if(self->M.lo){
		return self->M.lo;
	}
	else if(!(self->M.lo = tsdp_header_M_create(ghost->media, 0, ghost->proto ? ghost->proto: "RTP/AVP"))){
		TSK_DEBUG_ERROR("Failed to create lo");
		return tsk_null;
	}

	// add format
	if(!tsk_strnullORempty(ghost->first_format)){
		tsk_string_t* fmt = tsk_string_create(ghost->first_format);
		if(!self->M.lo->FMTs){
			self->M.lo->FMTs = tsk_list_create();
		}
		tsk_list_push_back_data(self->M.lo->FMTs, (void**)&fmt);
		TSK_OBJECT_SAFE_FREE(fmt);
	}

	return self->M.lo;
}

static int tmedia_session_ghost_set_ro(tmedia_session_t* self, const tsdp_header_M_t* m)
{
	return 0;
}







//=================================================================================================
//	Ghost session object definition
//
/* constructor */
static tsk_object_t* tmedia_session_ghost_ctor(tsk_object_t * self, va_list * app)
{
	tmedia_session_ghost_t *session = self;
	if(session){
		/* init base */
		tmedia_session_init(TMEDIA_SESSION(session), tmedia_none);
		/* init self */
	}
	return self;
}
/* destructor */
static tsk_object_t* tmedia_session_ghost_dtor(tsk_object_t * self)
{ 
	tmedia_session_ghost_t *session = self;
	if(session){
		/* deinit base */
		tmedia_session_deinit(TMEDIA_SESSION(session));
		/* deinit self */
		TSK_FREE(session->media);
		TSK_FREE(session->proto);
		TSK_FREE(session->first_format);
	}

	return self;
}
/* object definition */
static const tsk_object_def_t tmedia_session_ghost_def_s = 
{
	sizeof(tmedia_session_ghost_t),
	tmedia_session_ghost_ctor, 
	tmedia_session_ghost_dtor,
	tmedia_session_cmp, 
};
/* plugin definition*/
static const tmedia_session_plugin_def_t tmedia_session_ghost_plugin_def_s = 
{
	&tmedia_session_ghost_def_s,
	
	tmedia_ghost,
	"ghost",
	
	tsk_null, /* set() */
	tsk_null, /* get() */
	tmedia_session_ghost_prepare,
	tmedia_session_ghost_start,
	tmedia_session_ghost_stop,
	tmedia_session_ghost_pause,

	/* Audio part */
	{ tsk_null },

	tmedia_session_ghost_get_lo,
	tmedia_session_ghost_set_ro
};
const tmedia_session_plugin_def_t *tmedia_session_ghost_plugin_def_t = &tmedia_session_ghost_plugin_def_s;
