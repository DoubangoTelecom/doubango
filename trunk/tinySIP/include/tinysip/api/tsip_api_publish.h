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

/**@file tsip_api_publish.h
 * @brief Public piblication (PUBLISH) functions.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYSIP_TSIP_PUBLISH_H
#define TINYSIP_TSIP_PUBLISH_H

#include "tinysip_config.h"

#include "tinysip/tsip_event.h"

TSIP_BEGIN_DECLS

#define TSIP_PUBLISH_EVENT(self)		((tsip_publish_event_t*)(self))

typedef enum tsip_publish_event_type_e
{
	tsip_i_publish,
	tsip_ai_publish,
	tsip_o_publish,
	tsip_ao_publish,
	
	tsip_i_unpublish,
	tsip_ai_unpublish,
	tsip_o_unpublish,
	tsip_ao_unpublish
}
tsip_publish_event_type_t;

typedef struct tsip_publish_event_e
{
	TSIP_DECLARE_EVENT;

	tsip_publish_event_type_t type;
}
tsip_publish_event_t;

int tsip_publish_event_signal(tsip_publish_event_type_t type, struct tsip_stack_s *stack, tsip_operation_id_t opid, short status_code, const char *phrase, const struct tsip_message_s* sipmessage);

TINYSIP_GEXTERN const void *tsip_publish_event_def_t;

TSIP_END_DECLS

#endif /* TINYSIP_TSIP_PUBLISH_H */
