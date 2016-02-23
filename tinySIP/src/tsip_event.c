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

/**@file tsip_event.c
 * @brief SIP event.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/tsip_event.h"

#include "tsip.h"

#include "tinysip/tsip_message.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

/* internal function used to create base SIP event */
tsip_event_t* tsip_event_create(tsip_ssession_t* ss, short code, const char* phrase, const tsip_message_t* sipmessage, tsip_event_type_t type)
{
    tsip_event_t* e;
    if((e = tsk_object_new(tsip_event_def_t, ss, code, phrase, sipmessage, type))) {
        tsip_event_init(e, ss, code, phrase, sipmessage, type);
    }
    return e;
}

/* initialize a sip sevent */
int tsip_event_init(tsip_event_t* self, tsip_ssession_t *ss, short code, const char *phrase, const tsip_message_t* sipmessage, tsip_event_type_t type)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    self->ss = tsk_object_ref(ss);
    self->code = code;
    tsk_strupdate(&(self->phrase), phrase);
    self->type = type;
    if(sipmessage) {
        self->sipmessage = tsk_object_ref((void*)sipmessage);
    }
    return 0;
}

/* signal new event (enqueue) */
int tsip_event_signal(tsip_event_type_t type, tsip_ssession_t* ss, short code, const char *phrase)
{
    return tsip_event_signal_2(type, ss, code, phrase, tsk_null);
}

int tsip_event_signal_2(tsip_event_type_t type, tsip_ssession_t* ss, short code, const char *phrase, const struct tsip_message_s* sipmessage)
{
    tsip_event_t* e;
    if((e = tsip_event_create(ss, code, phrase, sipmessage, type))) {
        TSK_RUNNABLE_ENQUEUE_OBJECT_SAFE(TSK_RUNNABLE(ss->stack), e);
        return 0;
    }
    return -1;
}

int tsip_event_deinit(tsip_event_t* self)
{
    if(self) {
        TSK_OBJECT_SAFE_FREE(self->ss);

        TSK_FREE(self->phrase);
        TSK_OBJECT_SAFE_FREE(self->sipmessage);

        return 0;
    }
    return -1;
}














//========================================================
//	SIP event object definition
//
static tsk_object_t* tsip_event_ctor(tsk_object_t * self, va_list * app)
{
    tsip_event_t *sipevent = self;
    if(sipevent) {
//		const tsip_message_t* sipmessage;
//		tsip_stack_t *stack;
//		tsip_ssession_handle_t *SSESSION;
//		short code;
//		const char *phrase;
//		tsip_event_type_t type;
//
//		stack = va_arg(*app, tsip_stack_handle_t *);
//		SSESSION = va_arg(*app, tsip_ssession_handle_t*);
//
//#if defined(__GNUC__)
//		code = (short)va_arg(*app, int);
//#else
//		code = va_arg(*app, short);
//#endif
//		phrase = va_arg(*app, const char *);
//
//		sipmessage = va_arg(*app, const tsip_message_t*);
//		type = va_arg(*app, tsip_event_type_t);
//
//		tsip_event_init(self, stack, SSESSION, code, phrase, sipmessage, type);
    }
    return self;
}

static tsk_object_t* tsip_event_dtor(tsk_object_t * self)
{
    tsip_event_t *sipevent = self;
    if(sipevent) {
        tsip_event_deinit(sipevent);
    }
    return self;
}

static int tsip_event_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
    return -1;
}

static const tsk_object_def_t tsip_event_def_s = {
    sizeof(tsip_event_t),
    tsip_event_ctor,
    tsip_event_dtor,
    tsip_event_cmp,
};
const tsk_object_def_t *tsip_event_def_t = &tsip_event_def_s;
