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

/**@file tsip_subscribe.h
 * @brief Public registration (SUBSCRIBE) functions.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYSIP_TSIP_SUBSCRIBE_H
#define TINYSIP_TSIP_SUBSCRIBE_H

#include "tinysip_config.h"

#include "tinysip/tsip_event.h"

TSIP_BEGIN_DECLS

#define TSIP_SUBSCRIBE_EVENT(self)		((tsip_subscribe_event_t*)(self))

typedef enum tsip_subscribe_event_type_e
{
	tsip_i_subscribe,
	tsip_ai_subscribe,
	tsip_o_subscribe,
	tsip_ao_subscribe,
	
	tsip_i_unsubscribe,
	tsip_ai_unsubscribe,
	tsip_o_unsubscribe,
	tsip_ao_unsubscribe,

	tsip_i_notify,
	tsip_ai_notify,
	tsip_o_notify,
	tsip_ao_notify
}
tsip_subscribe_event_type_t;

typedef struct tsip_subscribe_event_e
{
	TSIP_DECLARE_EVENT;

	tsip_subscribe_event_type_t type;
}
tsip_subscribe_event_t;

int tsip_subscribe_event_signal(tsip_subscribe_event_type_t type, struct tsip_stack_s *stack, tsip_operation_id_t opid, short status_code, const char *phrase, const struct tsip_message_s* sipmessage);

typedef int (*tsip_subscribe_callback)(const tsip_subscribe_event_t *sipevent);

TINYSIP_GEXTERN const void *tsip_subscribe_event_def_t;

TSIP_END_DECLS

#endif /* TINYSIP_TSIP_SUBSCRIBE_H */
