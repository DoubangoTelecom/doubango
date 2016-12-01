/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou [at) doubango (DOT) org>
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

/**@file tmsrp_event.c
 * @brief MSRP/MSRPS Event.
 *
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 *

 */
#include "tinymsrp/tmsrp_event.h"

#include "tsk_debug.h"

tmsrp_event_t* tmsrp_event_create(const void* callback_data, tsk_bool_t outgoing, tmsrp_event_type_t type, tmsrp_message_t* message)
{
    tmsrp_event_t* _event;
    if((_event = tsk_object_new(tmsrp_event_def_t))) {
        _event->callback_data = callback_data;
        _event->outgoing = outgoing;
        _event->type = type;
        _event->message = tsk_object_ref(message);
    }
    else {
        TSK_DEBUG_ERROR("Faile to create new MSRP event");
    }

    return _event;
}

//========================================================
//	MSRP Event definition
//

/**@ingroup tmsrp_event_group
*/
static tsk_object_t* tmsrp_event_ctor(tsk_object_t *self, va_list * app)
{
    tmsrp_event_t *_event = self;
    if(_event) {
    }
    else {
        TSK_DEBUG_ERROR("Failed to create new MSRP Event");
    }
    return self;
}

/**@ingroup tmsrp_event_group
*/
static tsk_object_t* tmsrp_event_dtor(tsk_object_t *self)
{
    tmsrp_event_t *_event = self;
    if(_event) {
        TSK_OBJECT_SAFE_FREE(_event->message);
    }
    else {
        TSK_DEBUG_ERROR("Null MSRP Event");
    }

    return self;
}

static const tsk_object_def_t tmsrp_event_def_s = {
    sizeof(tmsrp_event_t),
    tmsrp_event_ctor,
    tmsrp_event_dtor,
    tsk_null
};
const tsk_object_def_t *tmsrp_event_def_t = &tmsrp_event_def_s;
