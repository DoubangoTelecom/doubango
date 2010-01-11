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
#include "tinysip/headers/tsip_header_Max_Forwards.h"
#include "tinysip/headers/tsip_header_Require.h"
#include "tinysip/headers/tsip_header_Supported.h"
#include "tinysip/headers/tsip_header_User_Agent.h"


#include "tsk_debug.h"
#include "tsk_memory.h"

#include <string.h>

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

int	tsip_message_add_header(tsip_message_t *self, const tsip_header_t *hdr)
{
	#define ADD_HEADER(type, field) \
		case tsip_htype_##type: \
			{ \
				if(!self->field) \
				{ \
					self->field = (tsip_header_##type##_t*)header; \
					return 0; \
				} \
				break; \
			}
	
	if(self && hdr)
	{
		tsip_header_t *header = tsk_object_ref((void*)hdr);

		switch(header->type)
		{
			ADD_HEADER(Via, firstVia);
			ADD_HEADER(From, From);
			ADD_HEADER(To, To);
			ADD_HEADER(Contact, Contact);
			ADD_HEADER(Call_ID, Call_ID);
			ADD_HEADER(CSeq, CSeq);
			ADD_HEADER(Expires, Expires);
			ADD_HEADER(Content_Type, Content_Type);
			ADD_HEADER(Content_Length, Content_Length);

			default: break;
		}

		tsk_list_push_back_data(self->headers, (void**)&header);

		return 0;
	}
	return -1;
}

int tsip_message_add_headers(tsip_message_t *self, const tsip_headers_L_t *headers)
{
	tsk_list_item_t *item = 0;
	if(self)
	{
		tsk_list_foreach(item, headers)
		{
			tsip_message_add_header(self, item->data);
		}
		return 0;
	}
	return -1;
}

const tsip_header_t *tsip_message_get_headerAt(const tsip_message_t *self, tsip_header_type_t type, size_t index)
{
	size_t pos = 0;
	tsk_list_item_t *item = 0;
	if(self)
	{
		tsk_list_foreach(item, self->headers)
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

const tsip_header_t *tsip_message_get_header(const tsip_message_t *self, tsip_header_type_t type)
{
	return tsip_message_get_headerAt(self, type, 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @fn	TSIP_BOOLEAN tsip_message_allowed(const tsip_message_t *message, const char* method)
///
/// @brief	Indicates whether the sepecified method is listed in the SIP 'Allow' header. 
///
/// @author	Mamadou
/// @date	12/9/2009
///
/// @param [in,out]	self	The SIP message holding the 'Allow' header. 
/// @param [in,out]	method	The method to look for. 
///
/// @return	@ref TSIP_TRUE if succeed and @ref TSIP_FALSE otherwise. 
////////////////////////////////////////////////////////////////////////////////////////////////////
TSIP_BOOLEAN tsip_message_allowed(const tsip_message_t *self, const char* method)
{
	int index = 0;
	tsip_header_Allow_t *hdr_allow;

	if(self)
	{
		while( hdr_allow = (tsip_header_Allow_t*)tsip_message_get_headerAt(self, tsip_htype_Allow, index++) )
		{
			if(tsk_list_find_item_by_pred(hdr_allow->methods, pred_find_string_by_value, method))
			{
				return TSIP_TRUE;
			}
		}
	}
	return TSIP_FALSE;
}

TSIP_BOOLEAN tsip_message_supported(const tsip_message_t *self, const char* option)
{
	int index = 0;
	tsip_header_Supported_t *hdr_supported;

	if(self)
	{
		while( hdr_supported = (tsip_header_Supported_t*)tsip_message_get_headerAt(self, tsip_htype_Supported, index++) )
		{
			if(tsk_list_find_item_by_pred(hdr_supported->options, pred_find_string_by_value, option))
			{
				return TSIP_TRUE;
			}
		}
		}
	return TSIP_FALSE;
}


TSIP_BOOLEAN tsip_message_required(const tsip_message_t *self, const char* option)
{
	int index = 0;
	tsip_header_Require_t *hdr_require;

	if(self)
	{
		while( hdr_require = (tsip_header_Require_t*)tsip_message_get_headerAt(self, tsip_htype_Require, index++) )
		{
			if(tsk_list_find_item_by_pred(hdr_require->options, pred_find_string_by_value, option))
			{
				return TSIP_TRUE;
			}
		}
	}
	return TSIP_FALSE;
}

int32_t tsip_message_getExpires(const tsip_message_t *self)
{	
	if(self)
	{
		if(self->Expires)
		{
			return self->Expires->delta_seconds;
		}

		// FIXME: You MUST choose the right contact
		if(self->Contact)
		{
			return self->Contact->expires;
		}
	}
	return -1;
}

uint32_t tsip_message_getContent_length(const tsip_message_t *self)
{
	return (self && self->Content_Length) ? self->Content_Length->length : 0;
}

int tsip_message_tostring(const tsip_message_t *self, tsk_buffer_t *output)
{
	if(!self)
	{
		return -1;
	}

	if(TSIP_MESSAGE_IS_REQUEST(self))
	{
		/*Method SP Request_URI SP SIP_Version CRLF*/
		/* Method */
		tsk_buffer_appendEx(output, "%s ", self->method);
		/* Request URI */
		tsip_uri_tostring(self->uri, 0, 0, output);
		/* SIP VERSION */
		tsk_buffer_appendEx(output, " %s\r\n", TSIP_MESSAGE_VERSION_DEFAULT);
	}
	else
	{
		/*SIP_Version SP Status_Code SP Reason_Phrase CRLF*/
		tsk_buffer_appendEx(output, "%s %hi %s\r\n", TSIP_MESSAGE_VERSION_DEFAULT, TSIP_RESPONSE_CODE(self), TSIP_RESPONSE_PHRASE(self));
	}

	/* First Via */
	if(self->firstVia) tsip_header_tostring(TSIP_HEADER(self->firstVia), output);
	/* From */
	if(self->From) tsip_header_tostring(TSIP_HEADER(self->From), output);
	/* To */
	if(self->To) tsip_header_tostring(TSIP_HEADER(self->To), output);
	/* Contact */
	if(self->Contact) tsip_header_tostring(TSIP_HEADER(self->Contact), output);
	/* Call_id */
	if(self->Call_ID) tsip_header_tostring(TSIP_HEADER(self->Call_ID), output);
	/* CSeq */
	if(self->CSeq) tsip_header_tostring(TSIP_HEADER(self->CSeq), output);
	/* Expires */
	if(self->Expires) tsip_header_tostring(TSIP_HEADER(self->Expires), output);
	/* Content-Type */
	if(self->Content_Type) tsip_header_tostring(TSIP_HEADER(self->Content_Type), output);
	/* Content-Length*/
	if(self->Content_Length) tsip_header_tostring(TSIP_HEADER(self->Content_Length), output);

	/* All other headers */
	{
		tsk_list_item_t *item;
		tsk_list_foreach(item, self->headers)
		{
			tsip_header_t *hdr = item->data;
			tsip_header_tostring(hdr, output);
		}
	}

	/* EMPTY LINE */
	tsk_buffer_append(output, "\r\n", 2);

	/* CONTENT */
	if(TSIP_MESSAGE_HAS_CONTENT(self))
	{
		tsk_buffer_append(output, TSK_BUFFER_TO_STRING(self->Content), TSK_BUFFER_SIZE(self->Content));
	}

	return 0;
}

tsip_request_t *tsip_request_new(const char* method, const tsip_uri_t *request_uri, const tsip_uri_t *from, const tsip_uri_t *to, const char *call_id, int32_t cseq)
{
	tsip_request_t* request;
	//tsip_header_Allow_t *allow;
	//tsip_header_Max_Forwards_t *maxfor;

	request = TSIP_REQUEST_CREATE(method, request_uri);

	TSIP_MESSAGE_ADD_HEADER(request, TSIP_HEADER_FROM_VA_ARGS(0, from, 0));
	TSIP_MESSAGE_ADD_HEADER(request, TSIP_HEADER_TO_VA_ARGS(0, to, 0));
	TSIP_MESSAGE_ADD_HEADER(request, TSIP_HEADER_CALL_ID_VA_ARGS(call_id));
	TSIP_MESSAGE_ADD_HEADER(request, TSIP_HEADER_CSEQ_VA_ARGS(cseq, method));
	TSIP_MESSAGE_ADD_HEADER(request, TSIP_HEADER_MAX_FORWARDS_VA_ARGS(TSIP_HEADER_MAX_FORWARDS_DEFAULT));
	TSIP_MESSAGE_ADD_HEADER(request, TSIP_HEADER_USER_AGENT_VA_ARGS(TSIP_HEADER_USER_AGENT_DEFAULT));


	/*request->From = TSIP_HEADER_FROM_CREATE(0, from, 0);
	request->To = TSIP_HEADER_TO_CREATE(0, to, 0);
	request->Call_ID = TSIP_HEADER_CALL_ID_CREATE(call_id);
	request->CSeq = TSIP_HEADER_CSEQ_CREATE(1);*/
	
	/* allow */
	/*allow = tsip_header_Allow_parse(TSIP_HEADER_STR, strlen(TSIP_HEADER_STR));
	tsip_message_add_header(request, TSIP_HEADER(allow));
	TSIP_HEADER_ALLOW_SAFE_FREE(allow);*/

	/* max forward */
	/*maxfor = TSIP_HEADER_MAX_FORWARDS_CREATE(TSIP_HEADER_MAX_FORWARDS_DEFAULT);
	tsip_message_add_header(request, TSIP_HEADER(maxfor));
	TSIP_HEADER_MAX_FORWARDS_SAFE_FREE(maxfor);*/

	/* User-Agent */

	return request;
}

tsip_response_t *tsip_response_new(short status_code, const char* reason_phrase, const tsip_request_t *request)
{
	tsip_response_t *response = 0;

	if(request)
	{
		response = TSIP_RESPONSE_CREATE(request, status_code, reason_phrase);
		TSIP_MESSAGE_ADD_HEADER(response, TSIP_HEADER_USER_AGENT_VA_ARGS(TSIP_HEADER_USER_AGENT_DEFAULT)); /* To be compliant with OMA SIMPLE IM v1.0*/
		/*
			Copy other headers
		*/
	}

	return response;
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
		message->type = va_arg(*app, tsip_message_type_t); 
		message->headers = TSK_LIST_CREATE();
		message->sockfd = TNET_INVALID_FD;
		message->Content_Length = TSIP_HEADER_CONTENT_LENGTH_CREATE(0);


		switch(message->type)
		{
		case tsip_unknown:
			{
				break;
			}

		case tsip_request:
			{
				message->method = tsk_strdup(va_arg(*app, const char*));
				message->uri = tsk_object_ref((void*)va_arg(*app, const tsip_uri_t*)); 
				break;
			}

		case tsip_response:
			{
				const tsip_request_t* request = va_arg(*app, const tsip_request_t*);
#if defined(__GNUC__)
				message->status_code = (short)va_arg(*app, int);
#else
				message->status_code = va_arg(*app, short);
#endif
				message->reason_phrase = tsk_strdup(va_arg(*app, const char*)); 
				
				/* Copy sockfd */
				message->sockfd = request->sockfd;

				/*
				RFC 3261 - 8.2.6.2 Headers and Tags

				The From field of the response MUST equal the From header field of
				the request.  The Call-ID header field of the response MUST equal the
				Call-ID header field of the request.  The CSeq header field of the
				response MUST equal the CSeq field of the request.  The Via header
				field values in the response MUST equal the Via header field values
				in the request and MUST maintain the same ordering.

				If a request contained a To tag in the request, the To header field
				in the response MUST equal that of the request.  However, if the To
				header field in the request did not contain a tag, the URI in the To
				header field in the response MUST equal the URI in the To header
				field; additionally, the UAS MUST add a tag to the To header field in
				the response (with the exception of the 100 (Trying) response, in
				which a tag MAY be present).  This serves to identify the UAS that is
				responding, possibly resulting in a component of a dialog ID.  The
				same tag MUST be used for all responses to that request, both final
				and provisional (again excepting the 100 (Trying)).  Procedures for
				the generation of tags are defined in Section 19.3.
				*/
				message->From = tsk_object_ref((void*)request->From);
				message->Call_ID = tsk_object_ref((void*)request->Call_ID);
				message->CSeq = tsk_object_ref((void*)request->CSeq);
				message->firstVia = tsk_object_ref((void*)request->firstVia);
				/* All other VIAs */
				{
					size_t index = 0;
					const tsip_header_t * via;
					while((via = tsip_message_get_headerAt(request, tsip_htype_Via, index++)))
					{
						tsip_message_add_header(message, via);
					}
				}
				/* Record routes */
				{
					size_t index = 0;
					const tsip_header_t *record_route;
					while((record_route = tsip_message_get_headerAt(request, tsip_htype_Record_Route, index++)))
					{
						tsip_message_add_header(message, record_route);
					}
				}
				message->To = tsk_object_ref((void*)request->To);
				
				// FIXME: What about record routes?
				break;
			}
		}
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
			TSK_FREE(message->method);
			TSIP_URI_SAFE_FREE(message->uri);
		}
		else if(TSIP_MESSAGE_IS_RESPONSE(message))
		{
			TSK_FREE(message->reason_phrase);
		}

		TSK_FREE(message->sip_version);

		tsk_object_unref(message->Call_ID);
		tsk_object_unref(message->Contact);
		tsk_object_unref(message->Content_Length);
		tsk_object_unref(message->Content_Type);
		tsk_object_unref(message->CSeq);
		tsk_object_unref(message->firstVia);
		tsk_object_unref(message->From);
		tsk_object_unref(message->Expires);
		tsk_object_unref(message->To);
		
		TSK_BUFFER_SAFE_FREE(message->Content);

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

