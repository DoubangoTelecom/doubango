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

/**@file thttp_event.c
 * @brief HTTP/HTTPS event.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinyhttp/thttp_event.h"
#include "tinyhttp/thttp_message.h"

#include "tsk_string.h"




thttp_event_t* thttp_event_create(thttp_event_type_t type, const thttp_session_handle_t* session, const char* description, const thttp_message_t* message)
{
    return tsk_object_new(thttp_event_def_t, type, session, description, message);
}







//========================================================
//	HTTP event object definition
//
static void* thttp_event_ctor(void * self, va_list * app)
{
    thttp_event_t *httpevent = self;
    if(httpevent) {
        httpevent->type = va_arg(*app, thttp_event_type_t);
        httpevent->session = va_arg(*app, const thttp_session_handle_t*);
        httpevent->description = tsk_strdup( va_arg(*app, const char *) );
        httpevent->message = tsk_object_ref((void*)va_arg(*app, thttp_message_t *));
    }
    return self;
}

static void* thttp_event_dtor(void * self)
{
    thttp_event_t *httpevent = self;
    if(httpevent) {
        TSK_FREE(httpevent->description);
        TSK_OBJECT_SAFE_FREE(httpevent->message);
    }
    return self;
}

static const tsk_object_def_t thttp_event_def_s = {
    sizeof(thttp_event_t),
    thttp_event_ctor,
    thttp_event_dtor,
    tsk_null,
};
const void *thttp_event_def_t = &thttp_event_def_s;
