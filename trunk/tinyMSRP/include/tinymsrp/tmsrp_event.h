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

/**@file tmsrp_event.h
 * @brief MSRP/MSRPS Event.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYMSRP_EVENT_H
#define TINYMSRP_EVENT_H

#include "tinymsrp_config.h"

#include "tinymsrp/tmsrp_message.h"

#include "tsk_params.h"
#include "tsk_buffer.h"

TMSRP_BEGIN_DECLS

typedef struct tmsrp_event_s
{
	TSK_DECLARE_OBJECT;

	const void* callback_data;
	unsigned outgoing:1;

	tmsrp_message_t* message;
}
tmsrp_event_t;

typedef int (*tmsrp_event_cb_f)(tmsrp_event_t* _event);

tmsrp_event_t* tmsrp_event_create(const void* callback_data, tsk_bool_t outgoing, tmsrp_message_t* message);

TINYMSRP_GEXTERN const tsk_object_def_t *tmsrp_event_def_t;

TMSRP_END_DECLS

#endif /* TINYMSRP_EVENT_H */
