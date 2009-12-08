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

/**@file tsip_message.c
 * @brief Represents a SIP message. A SIP message is either a request from a client to a server, or a
 * response from a server to a client. See RFC 3261 suc-bclause 7.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/tsip_message.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

/**@defgroup tsip_message_group SIP message (either request or response).
*/
























//========================================================
//	SIP message object definition
//

/**@ingroup tsip_message_group
*/
static void* tsip_message_create(void *self, va_list * app)
{
	tsip_message_t *message = self;
	if(message)
	{
		message->type = tsip_unknown;

		/*message->type = va_arg(*app, tsip_message_type_t);
		if(TSIP_MESSAGE_IS_REQUEST(message))
		{
			message->line_request = tsk_calloc(1, sizeof(tsip_request_line_t));
		}
		else
		{
			message->line_status = tsk_calloc(1, sizeof(tsip_status_line_t));
		}*/
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new sip message.");
	}
	return self;
}

/**@ingroup tsip_message_group
*/
static void* tsip_message_destroy(void *self)
{
	tsip_message_t *message = self;
	if(message)
	{
		if(TSIP_MESSAGE_IS_REQUEST(message))
		{
			TSK_FREE(message->line_request.method);
			TSK_FREE(message->line_request.uri);
		}
		else if(TSIP_MESSAGE_IS_RESPONSE(message))
		{
			TSK_FREE(message->line_status.reason_phrase);
		}

		TSK_FREE(message->sip_version);

		TSIP_HEADER_FROM_SAFE_FREE(message->From);
		TSIP_HEADER_VIA_SAFE_FREE(message->firstVia);
	}
	else TSK_DEBUG_ERROR("Null SIP message.");

	return self;
}

static const tsk_object_def_t tsip_message_def_s = 
{
	sizeof(tsip_message_t),
	tsip_message_create,
	tsip_message_destroy,
	0
};
const void *tsip_message_def_t = &tsip_message_def_s;
