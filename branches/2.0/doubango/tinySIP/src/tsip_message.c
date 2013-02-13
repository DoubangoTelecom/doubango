/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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

/**@file tsip_message.c
 * @brief Represents a SIP message. A SIP message is either a request from a client to a server, or a
 * response from a server to a client. See RFC 3261 suc-bclause 7.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

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
static int __pred_find_string_by_value(const tsk_list_item_t *item, const void *stringVal)
{
	if(item && item->data){
		tsk_string_t *string = item->data;
		return tsk_stricmp(string->value, stringVal);
	}
	return -1;
}

/*== Predicate function to find tsip_header_t object by type. */
static int __pred_find_header_by_type(const tsk_list_item_t *item, const void *tsip_htype)
{
	if(item && item->data){
		tsip_header_t *header = item->data;
		tsip_header_type_t htype = *((tsip_header_type_t*)tsip_htype);
		return (header->type - htype);
	}
	return -1;
}

tsip_message_t* tsip_message_create()
{
	return tsk_object_new(tsip_message_def_t, tsip_unknown);
}

tsip_request_t* tsip_request_create(const char* method, const tsip_uri_t* uri)
{
	return tsk_object_new(tsip_message_def_t, tsip_request, method, uri);
}

tsip_response_t* tsip_response_create(const tsip_request_t* request, short status_code, const char* reason_phrase)
{
	return tsk_object_new(tsip_message_def_t, tsip_response, request, status_code, reason_phrase);
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
	
	if(self && hdr){
		tsip_header_t *header = tsk_object_ref((void*)hdr);

		switch(header->type){
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

int tsip_message_add_headers(tsip_message_t *self, ...)
{
	const tsk_object_def_t* objdef;
	tsip_header_t *header;
	va_list ap;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	va_start(ap, self);
	while((objdef = va_arg(ap, const tsk_object_def_t*))){
		if((header = tsk_object_new_2(objdef, &ap))){
			tsip_message_add_header(self, header);
			TSK_OBJECT_SAFE_FREE(header);
		}
	}
	va_end(ap);

	return 0;
}

int tsip_message_add_content(tsip_message_t *self, const char* content_type, const void* content, tsk_size_t size)
{
	if(self){
		if(content_type){
			TSK_OBJECT_SAFE_FREE(self->Content_Type);
		}
		TSK_OBJECT_SAFE_FREE(self->Content_Length);
		TSK_OBJECT_SAFE_FREE(self->Content);

		if(content_type){
			TSIP_MESSAGE_ADD_HEADER(self, TSIP_HEADER_CONTENT_TYPE_VA_ARGS(content_type));
		}
		TSIP_MESSAGE_ADD_HEADER(self, TSIP_HEADER_CONTENT_LENGTH_VA_ARGS(size));
		self->Content = tsk_buffer_create(content, size);

		return 0;
	}
	return -1;
}

const tsip_header_t *tsip_message_get_headerAt(const tsip_message_t *self, tsip_header_type_t type, tsk_size_t index)
{
	/* Do not forget to update tinyWRAP::SipMessage::getHeaderAt() */
	tsk_size_t pos = 0;
	const tsk_list_item_t *item;
	const tsip_header_t* hdr = tsk_null;

	if(self){		
		switch(type)
		{
		case tsip_htype_Via:
			if(index == 0){
				hdr = (const tsip_header_t*)self->firstVia;
				goto bail;
			}else pos++; break;
		case tsip_htype_From:
			if(index == 0){
				hdr = (const tsip_header_t*)self->From;
				goto bail;
			}else pos++; break;
		case tsip_htype_To:
			if(index == 0){
				hdr = (const tsip_header_t*)self->To;
				goto bail;
			}else pos++; break;
		case tsip_htype_Contact:
			if(index == 0){
				hdr = (const tsip_header_t*)self->Contact;
				goto bail;
			}else pos++; break;
		case tsip_htype_Call_ID:
			if(index == 0){
				hdr = (const tsip_header_t*)self->Call_ID;
				goto bail;
			}else pos++; break;
		case tsip_htype_CSeq:
			if(index == 0){
				hdr = (const tsip_header_t*)self->CSeq;
				goto bail;
			}else pos++; break;
		case tsip_htype_Expires:
			if(index == 0){
				hdr = (const tsip_header_t*)self->Expires;
				goto bail;
			}else pos++; break;
		case tsip_htype_Content_Type:
			if(index == 0){
				hdr = (const tsip_header_t*)self->Content_Type;
				goto bail;
			}else pos++; break;
		case tsip_htype_Content_Length:
			if(index == 0){
				hdr = (const tsip_header_t*)self->Content_Length;
				goto bail;
			}else pos++; break;
		default:
			break;
		}

		tsk_list_foreach(item, self->headers){
			if(!__pred_find_header_by_type(item, &type)){
				if(pos++ >= index){
					hdr = item->data;
					break;
				}
			}
		}
	}

bail:
	return hdr;
}

const tsip_header_t *tsip_message_get_headerLast(const tsip_message_t *self, tsip_header_type_t type)
{
	const tsip_header_t *hdr, *last = tsk_null;
	tsk_size_t index = 0;
	while((hdr = tsip_message_get_headerAt(self, type, index++))){
		last = hdr;
	}
	return last;
}

const tsip_header_t *tsip_message_get_header(const tsip_message_t *self, tsip_header_type_t type)
{
	return tsip_message_get_headerAt(self, type, 0);
}

/**
* Indicates whether the sepecified method is listed in the SIP 'Allow' header. 
*
* @param [in,out]	self	The SIP message holding the 'Allow' header. 
* @param [in,out]	method	The method to look for. 
*
* @return	@a tsk_true if succeed and @a tsk_false otherwise. 
*/
tsk_bool_t tsip_message_allowed(const tsip_message_t *self, const char* method)
{
	int index = 0;
	tsip_header_Allow_t *hdr_allow;

	if(self){
		while( (hdr_allow = (tsip_header_Allow_t*)tsip_message_get_headerAt(self, tsip_htype_Allow, index++)) ){
			if(tsk_list_find_item_by_pred(hdr_allow->methods, __pred_find_string_by_value, method)){
				return tsk_true;
			}
		}
	}
	return tsk_false;
}

tsk_bool_t tsip_message_supported(const tsip_message_t *self, const char* option)
{
	int index = 0;
	tsip_header_Supported_t *hdr_supported;

	if(self){
		while( (hdr_supported = (tsip_header_Supported_t*)tsip_message_get_headerAt(self, tsip_htype_Supported, index++)) ){
			if(tsk_list_find_item_by_pred(hdr_supported->options, __pred_find_string_by_value, option)){
				return tsk_true;
			}
		}
	}
	return tsk_false;
}


tsk_bool_t tsip_message_required(const tsip_message_t *self, const char* option)
{
	int index = 0;
	tsip_header_Require_t *hdr_require;

	if(self){
		while( (hdr_require = (tsip_header_Require_t*)tsip_message_get_headerAt(self, tsip_htype_Require, index++)) ){
			if(tsk_list_find_item_by_pred(hdr_require->options, __pred_find_string_by_value, option)){
				return tsk_true;
			}
		}
	}
	return tsk_false;
}

int64_t tsip_message_getExpires(const tsip_message_t *self)
{	
	if(self){
		if(self->Expires){
			return self->Expires->delta_seconds;
		}

		// FIXME: You MUST choose the right contact
		if(self->Contact){
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
	if(!self || !output){
		return -1;
	}

	if(TSIP_MESSAGE_IS_REQUEST(self)){
		/*Method SP Request_URI SP SIP_Version CRLF*/
		/* Method */
		tsk_buffer_append_2(output, "%s ", self->line.request.method);
		/* Request URI (without quotes but with params)*/
		tsip_uri_serialize(self->line.request.uri, tsk_true, tsk_false, output);
		/* SIP VERSION */
		tsk_buffer_append_2(output, " %s\r\n", TSIP_MESSAGE_VERSION_DEFAULT);
	}
	else{
		/*SIP_Version SP Status_Code SP Reason_Phrase CRLF*/
		tsk_buffer_append_2(output, "%s %hi %s\r\n", TSIP_MESSAGE_VERSION_DEFAULT, TSIP_RESPONSE_CODE(self), TSIP_RESPONSE_PHRASE(self));
	}

	/* First Via */
	if(self->firstVia){
		tsip_header_serialize(TSIP_HEADER(self->firstVia), output);
	}
	
	/* From */
	if(self->From){
		tsip_header_serialize(TSIP_HEADER(self->From), output);
	}
	/* To */
	if(self->To){
		tsip_header_serialize(TSIP_HEADER(self->To), output);
	}
	/* Contact */
	if(self->Contact){
		tsip_header_serialize(TSIP_HEADER(self->Contact), output);
	}
	/* Call_id */
	if(self->Call_ID){
		tsip_header_serialize(TSIP_HEADER(self->Call_ID), output);
	}
	/* CSeq */
	if(self->CSeq){
		tsip_header_serialize(TSIP_HEADER(self->CSeq), output);
	}
	/* Expires */
	if(self->Expires){
		tsip_header_serialize(TSIP_HEADER(self->Expires), output);
	}
	/* Content-Type */
	if(self->Content_Type){
		tsip_header_serialize(TSIP_HEADER(self->Content_Type), output);
	}
	/* Content-Length*/
	if(self->Content_Length){
		tsip_header_serialize(TSIP_HEADER(self->Content_Length), output);
	}

	/* All other headers */
	{
		tsk_list_item_t *item;
		tsk_list_foreach(item, self->headers){
			tsip_header_serialize(TSIP_HEADER(item->data), output);
		}
	}

	/* EMPTY LINE */
	tsk_buffer_append(output, "\r\n", 2);

	/* CONTENT */
	if(TSIP_MESSAGE_HAS_CONTENT(self)){
		tsk_buffer_append(output, TSK_BUFFER_TO_STRING(self->Content), TSK_BUFFER_SIZE(self->Content));
	}

	return 0;
}

tsip_request_type_t tsip_request_get_type(const char* method)
{
	if(tsk_strnullORempty(method)){
		return tsip_NONE;
	}

	if(tsk_striequals(method, "ACK")){
		return tsip_ACK;
	}else if(tsk_striequals(method, "BYE")){
		return tsip_BYE;
	}else if(tsk_striequals(method, "CANCEL")){
		return tsip_CANCEL;
	}else if(tsk_striequals(method, "INVITE")){
		return tsip_INVITE;
	}else if(tsk_striequals(method, "OPTIONS")){
		return tsip_OPTIONS;
	}else if(tsk_striequals(method, "REGISTER")){
		return tsip_REGISTER;
	}else if(tsk_striequals(method, "SUBSCRIBE")){
		return tsip_SUBSCRIBE;
	}else if(tsk_striequals(method, "NOTIFY")){
		return tsip_NOTIFY;
	}else if(tsk_striequals(method, "REFER")){
		return tsip_REFER;
	}else if(tsk_striequals(method, "INFO")){
		return tsip_INFO;
	}else if(tsk_striequals(method, "UPDATE")){
		return tsip_UPDATE;
	}else if(tsk_striequals(method, "MESSAGE")){
		return tsip_MESSAGE;
	}else if(tsk_striequals(method, "PUBLISH")){
		return tsip_PUBLISH;
	}else if(tsk_striequals(method, "PRACK")){
		return tsip_PRACK;
	}

	return tsip_NONE;
}

tsip_request_t *tsip_request_new(const char* method, const tsip_uri_t *request_uri, const tsip_uri_t *from, const tsip_uri_t *to, const char *call_id, int32_t cseq)
{
	tsip_request_t* request;

	/* RFC 3261 8.1.1 Generating the Request
		A valid SIP request formulated by a UAC MUST, at a minimum, contain
		the following header fields: To, From, CSeq, Call-ID, Max-Forwards,
		and Via; all of these header fields are mandatory in all SIP
		requests.  These six header fields are the fundamental building
		blocks of a SIP message, as they jointly provide for most of the
		critical message routing services including the addressing of
		messages, the routing of responses, limiting message propagation,
		ordering of messages, and the unique identification of transactions.
		These header fields are in addition to the mandatory request line,
		which contains the method, Request-URI, and SIP version.
	*/

	if((request = tsip_request_create(method, request_uri))){
		tsip_message_add_headers(request,
			TSIP_HEADER_TO_VA_ARGS(to?to->display_name:tsk_null, to, tsk_null),
			TSIP_HEADER_FROM_VA_ARGS(from?from->display_name:tsk_null, from, tsk_null),
			TSIP_HEADER_CSEQ_VA_ARGS(cseq, method),
			TSIP_HEADER_CALL_ID_VA_ARGS(call_id),
			TSIP_HEADER_MAX_FORWARDS_VA_ARGS(TSIP_HEADER_MAX_FORWARDS_DEFAULT),
			/* Via will be added by the transport layer */
			/* TSIP_HEADER_USER_AGENT_VA_ARGS(TSIP_HEADER_USER_AGENT_DEFAULT), */
			TSIP_HEADER_CONTENT_LENGTH_VA_ARGS(0),

			tsk_null);
	}
	
	return request;
}

tsip_response_t *tsip_response_new(short status_code, const char* reason_phrase, const tsip_request_t *request)
{
	tsip_response_t *response = tsk_null;

	if(request){
		if((response = tsip_response_create(request, status_code, reason_phrase))){
			tsip_message_add_headers(response,
				/* TSIP_HEADER_USER_AGENT_VA_ARGS(TSIP_HEADER_USER_AGENT_DEFAULT), */ /* To be compliant with OMA SIMPLE IM v1.0*/
				TSIP_HEADER_CONTENT_LENGTH_VA_ARGS(0),

				tsk_null);
		}
	}

	return response;
}









//========================================================
//	SIP message object definition
//

/**@ingroup tsip_message_group
*/
static tsk_object_t* tsip_message_ctor(tsk_object_t *self, va_list * app)
{
	tsip_message_t *message = self;
	if(message)
	{
		message->type = va_arg(*app, tsip_message_type_t); 
		message->headers = tsk_list_create();
		message->local_fd = TNET_INVALID_FD;
		message->line.request.request_type = tsip_NONE;


		switch(message->type)
		{
		case tsip_unknown:
			{
				break;
			}

		case tsip_request:
			{
				message->line.request.method = tsk_strdup(va_arg(*app, const char*));
				message->line.request.uri = tsk_object_ref((void*)va_arg(*app, const tsip_uri_t*));

				message->line.request.request_type = tsip_request_get_type(message->line.request.method);
				break;
			}

		case tsip_response:
			{
				const tsip_request_t* request = va_arg(*app, const tsip_request_t*);
#if defined(__GNUC__)
				message->line.response.status_code = (short)va_arg(*app, int);
#else
				message->line.response.status_code = va_arg(*app, short);
#endif
				message->line.response.reason_phrase = tsk_strdup(va_arg(*app, const char*)); 
				
				/* Copy network information */
				message->local_fd = request->local_fd;
				message->remote_addr = request->remote_addr;
				message->src_net_type = request->src_net_type;
				message->dst_net_type = request->dst_net_type;

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
				if(message->firstVia){
					tsk_size_t index = 1;
					const tsip_header_t * via;
					while((via = tsip_message_get_headerAt(request, tsip_htype_Via, index++))){
						tsip_message_add_header(message, via);
					}
				}
				/* Record routes */
				{
					tsk_size_t index = 0;
					const tsip_header_t *record_route;
					while((record_route = tsip_message_get_headerAt(request, tsip_htype_Record_Route, index++))){
						tsip_message_add_header(message, record_route);
					}
				}
				message->To = tsk_object_ref((void*)request->To);
				
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
static tsk_object_t* tsip_message_dtor(tsk_object_t *self)
{
	tsip_message_t *message = self;
	if(message){
		if(TSIP_MESSAGE_IS_REQUEST(message)){
			TSK_FREE(message->line.request.method);
			TSK_OBJECT_SAFE_FREE(message->line.request.uri);
		}
		else if(TSIP_MESSAGE_IS_RESPONSE(message)){
			TSK_FREE(message->line.response.reason_phrase);
		}

		TSK_FREE(message->sip_version);

		TSK_OBJECT_SAFE_FREE(message->Call_ID);
		TSK_OBJECT_SAFE_FREE(message->Contact);
		TSK_OBJECT_SAFE_FREE(message->Content_Length);
		TSK_OBJECT_SAFE_FREE(message->Content_Type);
		TSK_OBJECT_SAFE_FREE(message->CSeq);
		TSK_OBJECT_SAFE_FREE(message->firstVia);
		TSK_OBJECT_SAFE_FREE(message->From);
		TSK_OBJECT_SAFE_FREE(message->Expires);
		TSK_OBJECT_SAFE_FREE(message->To);
		
		TSK_OBJECT_SAFE_FREE(message->Content);

		TSK_OBJECT_SAFE_FREE(message->headers);

		TSK_FREE(message->sigcomp_id);

		TSK_FREE(message->dst_address);
	}
	else TSK_DEBUG_ERROR("Null SIP message.");

	return self;
}

static const tsk_object_def_t tsip_message_def_s = 
{
	sizeof(tsip_message_t),
	tsip_message_ctor,
	tsip_message_dtor,
	tsk_null
};
const tsk_object_def_t *tsip_message_def_t = &tsip_message_def_s;

