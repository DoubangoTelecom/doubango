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

/**@file tmsrp_event.h
 * @brief MSRP/MSRPS Event.
 *
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 *

 */
#ifndef TINYMSRP_EVENT_H
#define TINYMSRP_EVENT_H

#include "tinymsrp_config.h"

#include "tinymsrp/tmsrp_message.h"

#include "tsk_params.h"
#include "tsk_buffer.h"

TMSRP_BEGIN_DECLS

typedef enum tmsrp_event_type_e {
    tmsrp_event_type_none,
    tmsrp_event_type_connected,
    tmsrp_event_type_disconnected,
    tmsrp_event_type_message,
}
tmsrp_event_type_t;

typedef struct tmsrp_event_s {
    TSK_DECLARE_OBJECT;

    const void* callback_data;
    unsigned outgoing:1;


    tmsrp_event_type_t type;
    tmsrp_message_t* message;
}
tmsrp_event_t;

typedef int (*tmsrp_event_cb_f)(tmsrp_event_t* _event);

TINYMSRP_API tmsrp_event_t* tmsrp_event_create(const void* callback_data, tsk_bool_t outgoing, tmsrp_event_type_t type, tmsrp_message_t* message);

TINYMSRP_GEXTERN const tsk_object_def_t *tmsrp_event_def_t;

TMSRP_END_DECLS

#endif /* TINYMSRP_EVENT_H */
