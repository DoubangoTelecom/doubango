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

/**@file thttp_event.c
 * @brief HTTP/HTTPS event.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinyHTTP/thttp_event.h"
#include "tinyHTTP/thttp_message.h"

#include "tsk_string.h"











//========================================================
//	HTTP event object definition
//
static void* thttp_event_create(void * self, va_list * app)
{
	thttp_event_t *httpevent = self;
	if(httpevent)
	{
#if defined(__GNUC__)
		httpevent->code = (short)va_arg(*app, int);
#else
		httpevent->code = va_arg(*app, short);
#endif
		httpevent->phrase = tsk_strdup( va_arg(*app, const char *) );
		httpevent->message = tsk_object_ref((void*)va_arg(*app, thttp_message_t *));
	}
	return self;
}

static void* thttp_event_destroy(void * self)
{ 
	thttp_event_t *httpevent = self;
	if(httpevent)
	{
		TSK_FREE(httpevent->phrase);
		TSK_OBJECT_SAFE_FREE(httpevent->message);
	}
	return self;
}

static int thttp_event_cmp(const void *obj1, const void *obj2)
{
	return -1;
}

static const tsk_object_def_t thttp_event_def_s = 
{
	sizeof(thttp_event_t),
	thttp_event_create, 
	thttp_event_destroy,
	thttp_event_cmp, 
};
const void *thttp_event_def_t = &thttp_event_def_s;