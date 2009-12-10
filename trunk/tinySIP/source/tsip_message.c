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

#include "tinysip/headers/tsip_header_Allow.h"
#include "tinysip/headers/tsip_header_Contact.h"
#include "tinysip/headers/tsip_header_Require.h"
#include "tinysip/headers/tsip_header_Supported.h"


#include "tsk_debug.h"
#include "tsk_memory.h"

/**@defgroup tsip_message_group SIP message (either request or response).
*/

/*== Predicate function to find tsk_string_t object by val*/
static int pred_find_string_by_value(const tsk_list_item_t *item, const void *stringVal)
{
	if(item && item->data)
	{
		tsk_string_t *string = item->data;
		return tsk_stricmp(string->value, stringVal);
	}
	return -1;
}

/*== Predicate function to find tsip_header_t object by type. */
static int pred_find_header_by_type(const tsk_list_item_t *item, const void *tsip_htype)
{
	if(item && item->data)
	{
		tsip_header_t *header = item->data;
		tsip_header_type_t htype = *((tsip_header_type_t*)tsip_htype);
		return (header->type - htype);
	}
	return -1;
}


tsip_header_t *tsip_message_get_headerAt(const tsip_message_t *message, tsip_header_type_t type, size_t index)
{
	size_t pos = 0;
	tsk_list_item_t *item = 0;
	if(message)
	{
		tsk_list_foreach(item, message->headers)
		{
			if(!pred_find_header_by_type(item, &type))
			{
				if(pos++ >= index)
				{
					break;
				}
			}
		}
	}
	return item ? item->data : 0;
}

tsip_header_t *tsip_message_get_header(const tsip_message_t *message, tsip_header_type_t type)
{
	return tsip_message_get_headerAt(message, type, 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @fn	TSIP_BOOLEAN tsip_message_allowed(const tsip_message_t *message, const char* method)
///
/// @brief	Indicates whether the sepecified method is listed in the SIP 'Allow' header. 
///
/// @author	Mamadou
/// @date	12/9/2009
///
/// @param [in,out]	message	The SIP message holding the 'Allow' header. 
/// @param [in,out]	method	The method to look for. 
///
/// @return	@ref TSIP_TRUE if succeed and @ref TSIP_FALSE otherwise. 
////////////////////////////////////////////////////////////////////////////////////////////////////
TSIP_BOOLEAN tsip_message_allowed(const tsip_message_t *message, const char* method)
{
	int index = 0;
	tsip_header_Allow_t *hdr_allow;

	if(message)
	{
		while( hdr_allow = (tsip_header_Allow_t*)tsip_message_get_headerAt(message, tsip_htype_Allow, index++) )
		{
			if(tsk_list_find_item_by_pred(hdr_allow->methods, pred_find_string_by_value, method))
			{
				return TSIP_TRUE;
			}
		}
	}
	return TSIP_FALSE;
}

TSIP_BOOLEAN tsip_message_supported(const tsip_message_t *message, const char* option)
{
	int index = 0;
	tsip_header_Supported_t *hdr_supported;

	if(message)
	{
		while( hdr_supported = (tsip_header_Supported_t*)tsip_message_get_headerAt(message, tsip_htype_Supported, index++) )
		{
			if(tsk_list_find_item_by_pred(hdr_supported->options, pred_find_string_by_value, option))
			{
				return TSIP_TRUE;
			}
		}
		}
	return TSIP_FALSE;
}


TSIP_BOOLEAN tsip_message_required(const tsip_message_t *message, const char* option)
{
	int index = 0;
	tsip_header_Require_t *hdr_require;

	if(message)
	{
		while( hdr_require = (tsip_header_Require_t*)tsip_message_get_headerAt(message, tsip_htype_Require, index++) )
		{
			if(tsk_list_find_item_by_pred(hdr_require->options, pred_find_string_by_value, option))
			{
				return TSIP_TRUE;
			}
		}
	}
	return TSIP_FALSE;
}


int32_t tsip_message_getExpires(const tsip_message_t *message)
{	
	if(message)
	{
		if(message->Expires)
		{
			return message->Expires->delta_seconds;
		}

		// FIXME: You MUST choose the right contact
		if(message->Contact)
		{
			int index = 0;
			const tsip_contact_t *contact;
			while(contact = tsip_header_Contact_get_ContactAt(message->Contact, index++))
			{
				if(contact->expires >=0)
				{
					return contact->expires;
				}
			}
		}
	}
	return -1;
}

uint32_t tsip_message_getContent_length(const tsip_message_t *message)
{
	return (message && message->Content_Length) ? message->Content_Length->length : 0;
}















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
		message->headers = TSK_LIST_CREATE();
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

		tsk_object_unref(message->Call_ID);
		tsk_object_unref(message->Contact);
		tsk_object_unref(message->Content_Length);
		tsk_object_unref(message->CSeq);
		tsk_object_unref(message->firstVia);
		tsk_object_unref(message->From);
		tsk_object_unref(message->Expires);
		tsk_object_unref(message->To);
		

		TSK_LIST_SAFE_FREE(message->headers);
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
