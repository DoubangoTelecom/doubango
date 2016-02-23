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

/**@file tcomp_reqfeed.c
 * @brief  SIGCOMP requested feedback item as per rfc 3320 subclause 9.4.9.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *

 */
#include "tcomp_reqfeed.h"
#include "tcomp_buffer.h"

#include "tsk_memory.h"
#include "tsk_debug.h"

/** Creates new Requested feedback.
*/
tcomp_reqfeed_t* tcomp_reqfeed_create()
{
    return tsk_object_new(tcomp_reqfeed_def_t);
}

/**
* Reset the feedback.
* @param feedback The feedback to reset.
*/
void tcomp_reqfeed_reset(tcomp_reqfeed_t* feedback)
{
    if(feedback) {
        tcomp_buffer_freeBuff(feedback->item);
        tcomp_buffer_reset(feedback->item);

        feedback->Q = feedback->S = feedback->I = 0;
    }
    else {
        TSK_DEBUG_ERROR("Invalid parameter.");
    }
}




//========================================================
//	Requested feedback object definition
//
static void* tcomp_reqfeed_ctor(void * self, va_list * app)
{
    tcomp_reqfeed_t *feedback = self;
    if(feedback) {
        feedback->item = tcomp_buffer_create_null();
    }
    else {
        TSK_DEBUG_WARN("NULL feedback");
    }

    return self;
}
static void* tcomp_reqfeed_dtor(void* self)
{
    tcomp_reqfeed_t *feedback = self;
    if(feedback) {
        TSK_OBJECT_SAFE_FREE(feedback->item);
    }
    else {
        TSK_DEBUG_WARN("NULL feedback");
    }
    return self;
}

static const tsk_object_def_t tcomp_reqfeed_def_s = {
    sizeof(tcomp_reqfeed_t),
    tcomp_reqfeed_ctor,
    tcomp_reqfeed_dtor,
    tsk_null
};
const tsk_object_def_t *tcomp_reqfeed_def_t = &tcomp_reqfeed_def_s;
