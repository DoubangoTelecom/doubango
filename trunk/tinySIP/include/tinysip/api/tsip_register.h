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

/**@file tsip_register.h
 * @brief Public registration (REGISTER) functions.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYSIP_TSIP_REGISTER_H
#define TINYSIP_TSIP_REGISTER_H

#include "tinysip_config.h"

#include "tinysip/tsip_event.h"

TSIP_BEGIN_DECLS

#define TSIP_REGISTER_EVENT(self)		((tsip_register_event_t*)(self))

typedef enum tsip_register_event_type_e
{
	tsip_i_register,
	tsip_ai_register,
	tsip_o_register,
	tsip_ao_register,
	
	tsip_i_unregister,
	tsip_ai_unregister,
	tsip_o_unregister,
	tsip_ao_unregister,
}
tsip_register_event_type_t;

typedef struct tsip_register_event_e
{
	TSIP_DECLARE_EVENT;

	tsip_register_event_type_t type;
}
tsip_register_event_t;

int tsip_register_event_signal(tsip_register_event_type_t type, struct tsip_stack_s *stack, tsip_operation_id_t opid, short status_code, const char *phrase, const struct tsip_message_s* sipmessage);

typedef int (*tsip_register_callback)(const tsip_register_event_t *sipevent);

TINYSIP_GEXTERN const void *tsip_register_event_def_t;

TSIP_END_DECLS

#endif /* TINYSIP_TSIP_REGISTER_H */
