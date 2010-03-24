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

/**@file tmsrp_media.c
 * @brief MSRP Media definition.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinyMSRP/session/tmsrp_media.h"


#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"



int tmsrp_media_set_params(tmedia_t* self, const tsk_params_L_t* params)
{
	tmsrp_media_t *msrp = TMSRP_MEDIA(self);
	TSK_DEBUG_INFO("tmsrp_media_set_params");

	return 0;
}

int	tmsrp_media_start(tmedia_t* self)
{
	tmsrp_media_t *msrp = TMSRP_MEDIA(self);
	TSK_DEBUG_INFO("tmsrp_media_start");

	return 0;
}

int	tmsrp_media_pause(tmedia_t* self)
{
	tmsrp_media_t *msrp = TMSRP_MEDIA(self);
	TSK_DEBUG_INFO("tmsrp_media_pause");

	return 0;
}

int	tmsrp_media_stop(tmedia_t* self)
{
	tmsrp_media_t *msrp = TMSRP_MEDIA(self);
	TSK_DEBUG_INFO("tmsrp_media_stop");

	return 0;
}

char* tmsrp_media_get_local_offer(tmedia_t* self)
{
	tmsrp_media_t *msrp = TMSRP_MEDIA(self);
	TSK_DEBUG_INFO("tmsrp_media_get_local_offer");

	return tsk_null;
}

char* tmsrp_media_get_negotiated_offer(tmedia_t* self)
{
	tmsrp_media_t *msrp = TMSRP_MEDIA(self);
	TSK_DEBUG_INFO("tmsrp_media_get_negotiated_offer");

	return tsk_null;
}

int tmsrp_media_set_remote_offer(tmedia_t* self, const char* offer)
{
	tmsrp_media_t *msrp = TMSRP_MEDIA(self);
	TSK_DEBUG_INFO("tmsrp_media_set_remote_offer");

	return 0;
}




//========================================================
//	Dummy media object definition
//
static void* tmsrp_media_create(tsk_object_t *self, va_list * app)
{
	tmsrp_media_t *msrp = self;
	if(msrp)
	{
		// Parameters include at least the media name		
		tmedia_init(TMEDIA(msrp), va_arg(*app, const char*));

		TMEDIA(msrp)->protocol = tsk_strdup("TCP/MSRP");
		msrp->sdp_ctx = TSDP_CTX_CREATE();
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new dummy media.");
	}
	return self;
}

static void* tmsrp_media_destroy(tsk_object_t *self)
{
	tmsrp_media_t *msrp = self;
	if(msrp){
		tmedia_deinit(TMEDIA(msrp));
		TSK_OBJECT_SAFE_FREE(msrp->sdp_ctx);
	}
	else{
		TSK_DEBUG_ERROR("Null dummy media.");
	}

	return self;
}
static int tmsrp_media_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
	return -1;
}

static const tsk_object_def_t tmsrp_media_def_s = 
{
	sizeof(tmsrp_media_t),
	tmsrp_media_create,
	tmsrp_media_destroy,
	tmsrp_media_cmp
};

const tsk_object_def_t *tmsrp_media_def_t = &tmsrp_media_def_s;

//========================================================
//	Dummy media plugin definition
//
static const tmedia_plugin_def_t tmsrp_media_plugin_def_s = 
{
	&tmsrp_media_def_s,
	"message",

	tmsrp_media_set_params,

	tmsrp_media_start,
	tmsrp_media_pause,
	tmsrp_media_stop,

	tmsrp_media_get_local_offer,
	tmsrp_media_get_negotiated_offer,
	tmsrp_media_set_remote_offer
};
const tmedia_plugin_def_t *tmsrp_media_plugin_def_t = &tmsrp_media_plugin_def_s;

