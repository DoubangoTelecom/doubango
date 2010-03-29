/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)yahoo.fr>
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

/**@file thttp_message.h
 * @brief Represents a HTTP message. A HTTP message is either a request from a client to a server, or a
 * response from a server to a client.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef THTTP_MESSAGE_H
#define THTTP_MESSAGE_H

#include "tinyhttp_config.h"

#include "tinyhttp/thttp_url.h"

//#include "tinyhttp/headers/thttp_header_Call_ID.h"
//#include "tinyhttp/headers/thttp_header_Contact.h"
#include "tinyhttp/headers/thttp_header_Content_Length.h"
#include "tinyhttp/headers/thttp_header_Content_Type.h"
//#include "tinyhttp/headers/thttp_header_CSeq.h"
//#include "tinyhttp/headers/thttp_header_Expires.h"
//#include "tinyhttp/headers/thttp_header_From.h"
//#include "tinyhttp/headers/thttp_header_P_Access_Network_Info.h"
//#include "tinyhttp/headers/thttp_header_To.h"
//#include "tinyhttp/headers/thttp_header_Via.h"

#include "tsk_object.h"
#include "tsk_buffer.h"

THTTP_BEGIN_DECLS

#define THTTP_MESSAGE_VERSION_10					"HTTP/1.0"
#define THTTP_MESSAGE_VERSION_11					"HTTP/1.1"
#define THTTP_MESSAGE_VERSION_20					"HTTP/2.0"
#define THTTP_MESSAGE_VERSION_DEFAULT			THTTP_MESSAGE_VERSION_11

#define THTTP_MESSAGE_IS_REQUEST(self) ((self) ? (self)->type == thttp_request : 0)
#define THTTP_MESSAGE_IS_RESPONSE(self) ((self) ? (self)->type == thttp_response : 0)

#define THTTP_MESSAGE(self)				((thttp_message_t*)(self))
#define THTTP_MESSAGE_AS_RESPONSE(self)	((thttp_response_t*)(self))
#define THTTP_MESSAGE_AS_REQUEST(self)	((thttp_request_t*)(self))


/**@def THTTP_MESSAGE_CREATE
* Creates new http messgae. Could be either a request or a response.
* You must call @ref TSK_OBJECT_SAFE_FREE to free the message.
* @sa TSK_OBJECT_SAFE_FREE.
*/
#define THTTP_MESSAGE_CREATE()											tsk_object_new(thttp_message_def_t, (thttp_message_type_t)thttp_unknown)
#define THTTP_REQUEST_CREATE(method, url)								tsk_object_new(thttp_message_def_t, (thttp_message_type_t)thttp_request, (const char*)method, (const thttp_url_t*)url)
#define THTTP_RESPONSE_CREATE(request, status_code, reason_phrase)		tsk_object_new(thttp_message_def_t, (thttp_message_type_t)thttp_response, (const thttp_request_t*)request, (short)status_code, (const char*)reason_phrase)


#define THTTP_RESPONSE_CODE(self)			 (THTTP_MESSAGE_IS_RESPONSE((self)) ? (self)->status_code : 0)
#define THTTP_RESPONSE_PHRASE(self)			 ((self)->reason_phrase)

#define THTTP_REQUEST_METHOD(self)			 ((self)->method)
#define THTTP_REQUEST_URI(self)				 ((self)->url)

#define THTTP_MESSAGE_CONTENT_LENGTH(message) (uint32_t)(((message) && (message)->Content_Length) ? (message)->Content_Length->length : 0)
#define THTTP_MESSAGE_CONTENT(message)		 (THTTP_MESSAGE_HAS_CONTENT(message) ? (message)->Content->data : 0)
#define THTTP_MESSAGE_HAS_CONTENT(message)	 ((message) && (message)->Content)

#define THTTP_RESPONSE_IS(self, code)		(THTTP_RESPONSE_CODE((self)) == code)
#define THTTP_RESPONSE_IS_NXX(self, N)		(N##00<= THTTP_RESPONSE_CODE((self)) && THTTP_RESPONSE_CODE((self)) <= N##99)
#define THTTP_RESPONSE_IS_1XX(self)			THTTP_RESPONSE_IS_NXX(self, 1)
#define THTTP_RESPONSE_IS_2XX(self)			THTTP_RESPONSE_IS_NXX(self, 2)
#define THTTP_RESPONSE_IS_3XX(self)			THTTP_RESPONSE_IS_NXX(self, 3)
#define THTTP_RESPONSE_IS_4XX(self)			THTTP_RESPONSE_IS_NXX(self, 4)
#define THTTP_RESPONSE_IS_5XX(self)			THTTP_RESPONSE_IS_NXX(self, 5)
#define THTTP_RESPONSE_IS_6XX(self)			THTTP_RESPONSE_IS_NXX(self, 6)
#define THTTP_RESPONSE_IS_23456(self)		(200<= THTTP_RESPONSE_CODE((self)) && THTTP_RESPONSE_CODE((self)) <= 699)

/**
 * @enum	thttp_message_type_t
 *
 * @brief	Defines the message type (Request or Response). 
**/
typedef enum thttp_message_type_e
{
	thttp_unknown,
	thttp_request,
	thttp_response
}
thttp_message_type_t;

/**
 * @struct	thttp_message_t
 *
 * @brief	Represents a HTTP message. A HTTP message is either a request from a client to a server,
 * 			or a response from a server to a client. See RFC 3261 suc-bclause 7. 
 *			You must call @ref THTTP_MESSAGE_CREATE to create a http message and TSK_OBJECT_SAFE_FREE to free the message.
 *
 * @author	Mamadou
 * @date	12/2/2009
**/
typedef struct thttp_message_s
{
	TSK_DECLARE_OBJECT;
	
	char *http_version; /**< The HTTP version. Only 'HTTP/1.1' is supported. */
	thttp_message_type_t type; /**< The type of this HTTP message. */

#if !defined(__C99__) /* C99 does not allow unnamed structs/unions */
	union
	{
		struct
		{
#endif
			char *method;
			thttp_url_t *url;
#if !defined(__C99__)
		};
		struct
		{
#endif
			short status_code;
			char *reason_phrase;
#if !defined(__C99__)
		};
	};
	
#endif
	/*== MOST COMMON HEADERS. */
	thttp_header_Content_Type_t *Content_Type;
	thttp_header_Content_Length_t *Content_Length;
	tsk_buffer_t *Content;

	/*== OTHER HEADERS*/
	thttp_headers_L_t *headers;
}
thttp_message_t;

typedef thttp_message_t thttp_request_t; /**< HTTP request message. */
typedef thttp_message_t thttp_response_t; /**< HTTP response message. */

//
TINYHTTP_API int thttp_message_add_header(thttp_message_t *self, const thttp_header_t *hdr);
TINYHTTP_API int thttp_message_add_headers(thttp_message_t *self, const thttp_headers_L_t *headers);
TINYHTTP_API int thttp_message_add_content(thttp_message_t *self, const char* content_type, const void* content, size_t size);

#if !defined(_MSC_VER) || defined(__GNUC__)
static void THTTP_MESSAGE_ADD_HEADER(thttp_message_t *self, ...)
	{
		va_list ap;
		thttp_header_t *header;
		const tsk_object_def_t *objdef;
		
		va_start(ap, self);
		objdef = va_arg(ap, const tsk_object_def_t*);
		header = tsk_object_new_2(objdef, &ap);
		va_end(ap);

		thttp_message_add_header(self, header);
		tsk_object_unref(header);
	}
#else
#define THTTP_MESSAGE_ADD_HEADER(self, objdef, ...)						\
	{																	\
		thttp_header_t *header = tsk_object_new(objdef, __VA_ARGS__);	\
		thttp_message_add_header(self, header);							\
		tsk_object_unref(header);										\
	}
#endif

TINYHTTP_API const thttp_header_t *thttp_message_get_headerAt(const thttp_message_t *self, thttp_header_type_t type, size_t index);
TINYHTTP_API const thttp_header_t *thttp_message_get_header(const thttp_message_t *self, thttp_header_type_t type);
TINYHTTP_API const thttp_header_t *thttp_message_get_headerByName(const thttp_message_t *self, const char* name);

TINYHTTP_API int thttp_message_tostring(const thttp_message_t *self, tsk_buffer_t *output);

TINYHTTP_API thttp_request_t *thttp_request_new(const char* method, const thttp_url_t *request_url);
TINYHTTP_API thttp_response_t *thttp_response_new(short status_code, const char* reason_phrase, const thttp_request_t *request);

TINYHTTP_GEXTERN const void *thttp_message_def_t;

THTTP_END_DECLS

#endif /* THTTP_MESSAGE_H */

