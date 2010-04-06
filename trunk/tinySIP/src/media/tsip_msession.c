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

/**@file tsip_msession.c
 * @brief SIP media session.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinySIP/media/tsip_msession.h"



int tsip_msession_start(tsip_msession_t* self)
{
	int ret = -1;

	if(!self){
		goto bail;
	}

bail:
	return ret;
}

int tsip_msession_pause(tsip_msession_t* self)
{
	int ret = -1;

	if(!self){
		goto bail;
	}

bail:
	return ret;
}

int tsip_msession_stop(tsip_msession_t* self)
{
	int ret = -1;

	if(!self){
		goto bail;
	}

bail:
	return ret;
}















//========================================================
//	SIP media session object definition
//
static tsk_object_t* tsip_msession_create(void * self, va_list * app)
{
	tsip_msession_t *ms = self;
	if(ms){
	}
	return self;
}

static tsk_object_t* tsip_msession_destroy(void * self)
{ 
	tsip_msession_t *ms = self;
	if(ms){
	}
	return self;
}

static const tsk_object_def_t tsip_msession_def_s = 
{
	sizeof(tsip_msession_t),
	tsip_msession_create, 
	tsip_msession_destroy,
	tsk_null, 
};
const void *tsip_msession_def_t = &tsip_msession_def_s;

