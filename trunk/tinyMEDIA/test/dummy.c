/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou@yahoo.fr>
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
#include "dummy.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"



int dummy_set_params(tmedia_t* self, const tsk_params_L_t* params)
{
	dummy_t *dummy = DUMMY(self);
	TSK_DEBUG_INFO("dummy_set_params");

	return 0;
}

int	dummy_start(tmedia_t* self)
{
	dummy_t *dummy = DUMMY(self);
	TSK_DEBUG_INFO("dummy_start");

	return 0;
}

int	dummy_pause(tmedia_t* self)
{
	dummy_t *dummy = DUMMY(self);
	TSK_DEBUG_INFO("dummy_pause");

	return 0;
}

int	dummy_stop(tmedia_t* self)
{
	dummy_t *dummy = DUMMY(self);
	TSK_DEBUG_INFO("dummy_stop");

	return 0;
}

char* dummy_get_local_offer(tmedia_t* self)
{
	dummy_t *dummy = DUMMY(self);
	TSK_DEBUG_INFO("dummy_get_local_offer");

	return tsk_strdup(dummy->local_sdp);
}

char* dummy_get_negotiated_offer(tmedia_t* self)
{
	dummy_t *dummy = DUMMY(self);
	TSK_DEBUG_INFO("dummy_get_negotiated_offer");

	return tsk_strdup(dummy->negotiated_sdp);
}

int dummy_set_remote_offer(tmedia_t* self, const char* offer)
{
	dummy_t *dummy = DUMMY(self);
	TSK_DEBUG_INFO("dummy_set_remote_offer");

	return 0;
}




//========================================================
//	Dummy media object definition
//
static void* dummy_create(tsk_object_t *self, va_list * app)
{
	dummy_t *dummy = self;
	if(dummy)
	{
		// Parameters include at least the media name		
		tmedia_init(TMEDIA(dummy), va_arg(*app, const char*));

		TMEDIA(dummy)->protocol = tsk_strdup("TCP/DUMMY");
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new dummy media.");
	}
	return self;
}

static void* dummy_destroy(tsk_object_t *self)
{
	dummy_t *dummy = self;
	if(dummy){
		tmedia_deinit(TMEDIA(dummy));
		TSK_FREE(dummy->local_sdp);
		TSK_FREE(dummy->remote_sdp);
		TSK_FREE(dummy->negotiated_sdp);
	}
	else{
		TSK_DEBUG_ERROR("Null dummy media.");
	}

	return self;
}
static int dummy_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
	return -1;
}

static const tsk_object_def_t dummy_def_s = 
{
	sizeof(dummy_t),
	dummy_create,
	dummy_destroy,
	dummy_cmp
};

const tsk_object_def_t *dummy_def_t = &dummy_def_s;

//========================================================
//	Dummy media plugin definition
//
static const tmedia_plugin_def_t dummy_plugin_def_s = 
{
	&dummy_def_s,
	"dummy",

	dummy_set_params,

	dummy_start,
	dummy_pause,
	dummy_stop,

	dummy_get_local_offer,
	dummy_get_negotiated_offer,
	dummy_set_remote_offer
};
const tmedia_plugin_def_t *dummy_plugin_def_t = &dummy_plugin_def_s;

