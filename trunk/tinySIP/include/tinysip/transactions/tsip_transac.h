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
* GNU Lesser General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

/**@file tsip_transac.h
 * @brief SIP transaction base class as per RFC 3261 subclause 17.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYSIP_TRANSAC_H
#define TINYSIP_TRANSAC_H

#include "tinysip_config.h"

#include "tsip.h"
#include "tinysip/tsip_timers.h"
#include "tinysip/tsip_message.h"

#include "tsk_object.h"
#include "tsk_list.h"

#define TSIP_TRANSAC(self)				((tsip_transac_t*)(self))
#define TSIP_TRANSAC_GET_TYPE(self)		TSIP_TRANSAC(self)->type

typedef enum tsip_transac_type_e
{
	tsip_ict, /**< Invite Client Transaction. */
	tsip_ist, /**< Invite Server Transaction. */
	tsip_nict, /**< Non-Invite Client Transaction. */
	tsip_nist, /**< Non-Invite Server Transaction. */
}
tsip_transac_type_t;

#define TSIP_DECLARE_TRANSAC struct { \
	TSK_DECLARE_OBJECT; \
	\
	const tsip_stack_handle_t * stack;\
	\
	tsip_transac_type_t type; \
	\
	unsigned reliable:1; \
	\
	int32_t cseq_value; \
	char* cseq_method; \
	\
	char* callid; \
}

typedef TSIP_DECLARE_TRANSAC tsip_transac_t;

typedef tsk_list_t tsip_transacs_L_t;

int tsip_transac_init(tsip_transac_t *self, const tsip_stack_handle_t * stack, tsip_transac_type_t type, unsigned reliable, int32_t cseq_value, const char* cseq_method, const char* callid);
int tsip_transac_send(tsip_transac_t *self, const tsip_message_t *msg);
int tsip_transac_deinit(tsip_transac_t *transac);

#endif /* TINYSIP_TRANSAC_H */