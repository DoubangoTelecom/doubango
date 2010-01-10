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

/**@file tsip_message.h
 * @brief Represents a SIP message. A SIP message is either a request from a client to a server, or a
 * response from a server to a client. See RFC 3261 suc-bclause 7.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TSIP_MESSAGE_H
#define TSIP_MESSAGE_H

#include "tinysip_config.h"

#include "tinysip/headers/tsip_header_Call_ID.h"
#include "tinysip/headers/tsip_header_Contact.h"
#include "tinysip/headers/tsip_header_Content_Length.h"
#include "tinysip/headers/tsip_header_Content_Type.h"
#include "tinysip/headers/tsip_header_CSeq.h"
#include "tinysip/headers/tsip_header_Expires.h"
#include "tinysip/headers/tsip_header_From.h"
#include "tinysip/headers/tsip_header_P_Access_Network_Info.h"
#include "tinysip/headers/tsip_header_To.h"
#include "tinysip/headers/tsip_header_Via.h"

#include "tnet_types.h"

#include "tsk_object.h"
#include "tsk_buffer.h"

TSIP_BEGIN_DECLS

#define TSIP_MESSAGE_VERSION_10					"SIP/1.0"
#define TSIP_MESSAGE_VERSION_20					"SIP/2.0"
#define TSIP_MESSAGE_VERSION_DEFAULT			TSIP_MESSAGE_VERSION_20

#define TSIP_MESSAGE_IS_REQUEST(self) ((self) ? (self)->type == tsip_request : 0)
#define TSIP_MESSAGE_IS_RESPONSE(self) ((self) ? (self)->type == tsip_response : 0)

#define TSIP_MESSAGE(self)				((tsip_message_t*)(self))
#define TSIP_MESSAGE_AS_RESPONSE(self)	((tsip_response_t*)(self))
#define TSIP_MESSAGE_AS_REQUEST(self)	((tsip_request_t*)(self))


/**@def TSIP_MESSAGE_CREATE
* Creates new sip messgae. Could be either a request or a response.
* You must call @ref TSIP_MESSAGE_SAFE_FREE to free the message.
* @sa TSIP_MESSAGE_SAFE_FREE.
*/
/**@def TSIP_MESSAGE_SAFE_FREE
* Safely free a sip messgae previously created using TSIP_MESSAGE_CREATE.
* @sa TSIP_MESSAGE_CREATE.
*/
#define TSIP_MESSAGE_CREATE()											tsk_object_new(tsip_message_def_t, (tsip_message_type_t)tsip_unknown)
#define TSIP_REQUEST_CREATE(method, uri)								tsk_object_new(tsip_message_def_t, (tsip_message_type_t)tsip_request, (const char*)method, (const tsip_uri_t*)uri)
#define TSIP_RESPONSE_CREATE(request, status_code, reason_phrase)		tsk_object_new(tsip_message_def_t, (tsip_message_type_t)tsip_response, (const tsip_request_t*)request, (short)status_code, (const char*)reason_phrase)
#define TSIP_MESSAGE_SAFE_FREE(self)									tsk_object_unref(self), self = 0
#define TSIP_REQUEST_SAFE_FREE(self)									TSIP_MESSAGE_SAFE_FREE(self)
#define TSIP_RESPONSE_SAFE_FREE(self)									TSIP_MESSAGE_SAFE_FREE(self)


#define TSIP_RESPONSE_CODE(self)			 ((self)->line_status.status_code)
#define TSIP_RESPONSE_PHRASE(self)			 ((self)->line_status.reason_phrase)

#define TSIP_REQUEST_METHOD(self)			 ((self)->line_request.method)
#define TSIP_REQUEST_URI(self)				 ((self)->line_request.uri)

#define TSIP_MESSAGE_CONTENT_LENGTH(message) (uint32_t)(((message) && (message)->Content_Length) ? (message)->Content_Length->length : 0)
#define TSIP_MESSAGE_CONTENT(message)		 (TSIP_MESSAGE_HAS_CONTENT(message) ? (message)->Content : 0)
#define TSIP_MESSAGE_HAS_CONTENT(message)	 ((message) && (message)->Content)

#define TSIP_RESPONSE_IS(self, code)		(TSIP_RESPONSE_CODE((self)) == code)
#define TSIP_RESPONSE_IS_NXX(self, N)		(N##00<= TSIP_RESPONSE_CODE((self)) && TSIP_RESPONSE_CODE((self)) <= N##99)
#define TSIP_RESPONSE_IS_1XX(self)			TSIP_RESPONSE_IS_NXX(self, 1)
#define TSIP_RESPONSE_IS_2XX(self)			TSIP_RESPONSE_IS_NXX(self, 2)
#define TSIP_RESPONSE_IS_3XX(self)			TSIP_RESPONSE_IS_NXX(self, 3)
#define TSIP_RESPONSE_IS_4XX(self)			TSIP_RESPONSE_IS_NXX(self, 4)
#define TSIP_RESPONSE_IS_5XX(self)			TSIP_RESPONSE_IS_NXX(self, 5)
#define TSIP_RESPONSE_IS_6XX(self)			TSIP_RESPONSE_IS_NXX(self, 6)
#define TSIP_RESPONSE_IS_23456(self)		(200<= TSIP_RESPONSE_CODE((self)) && TSIP_RESPONSE_CODE((self)) <= 699)

/**
 * @enum	tsip_message_type_t
 *
 * @brief	Defines the message type (Request or Response). 
**/
typedef enum tsip_message_type_e
{
	tsip_unknown,
	tsip_request,
	tsip_response
}
tsip_message_type_t;

/**
 * @struct	tsip_request_line_t
 *
 * @brief	Defines SIP request header line. 
 * ABNF: Request-Line  =  Method SP Request-URI SP SIP-Version CRLF 
 * Ref: RFC 3261 subclause 7.1.
 * @sa		tsip_status_line_t.
 *
 * @author	Mamadou
 * @date	12/3/2009
**/
typedef struct tsip_request_line_s
{
	char *method; /**< SIP method name. e.g REGISTER, ACK or INVITE.*/
	tsip_uri_t *uri;	/**< The Request-URI is a SIP or SIPS URI as described in Section 19.1 or a general URI (RFC 2396 [5]).  It indicates
           the user or service to which this request is being addressed. The Request-URI MUST NOT contain unescaped spaces or control
           characters and MUST NOT be enclosed in "<>". */
}
tsip_request_line_t;

/**
 * @struct	tsip_status_line_t
 *
 * @brief	Defines SIP response status line. 
 * ABNF: Status-Line  =  SIP-Version SP Status-Code SP Reason-Phrase CRLF 
 * Ref: RFC 3261 subclause 7.2. 
 * @sa		tsip_request_line_t.
 *
 * @author	Mamadou
 * @date	12/3/2009
**/
typedef struct tsip_status_line_s
{
	short status_code; /**< 3-digit integer result code that indicates the outcome of an attempt to understand and satisfy a request. */
	char *reason_phrase; /**< Textual description related to the status code. */
}
tsip_status_line_t;

/**
 * @struct	tsip_message_t
 *
 * @brief	Represents a SIP message. A SIP message is either a request from a client to a server,
 * 			or a response from a server to a client. See RFC 3261 suc-bclause 7. 
 *			You must call @ref TSIP_MESSAGE_CREATE to create a sip message and TSIP_MESSAGE_SAFE_FREE to free the message.
 *
 * @author	Mamadou
 * @date	12/2/2009
**/
typedef struct tsip_message_s
{
	TSK_DECLARE_OBJECT;
	
	char *sip_version; /**< The SIP version. Only 'SIP/2.0' is supported. */
	tsip_message_type_t type; /**< The type of this SIP message. */

	// FIXME: http://docs.google.com/viewer?a=v&q=cache%3AE5v90Lcl5TkJ%3Awww.open-std.org%2FJTC1%2FSC22%2FWG14%2Fwww%2Fdocs%2Fn1406.pdf+anonymous+union+has+no+member&hl=en&sig=AHIEtbTdnPDnQLKpWQB9-nBYQg_BB4jIHQ&pli=1
#if !defined(__GNUC__)
	union
	{
#endif
		tsip_request_line_t line_request; /**< Request status line. Only if message type is @ref tsip_request. */
		tsip_status_line_t line_status; /**< Status line. Only if message type is @ref tsip_response. */
#if !defined(__GNUC__)
	};
#endif
	
	/*== MOST COMMON HEADERS. */
	tsip_header_Via_t *firstVia; /**< First Via header. */
	tsip_header_From_t *From;
	tsip_header_To_t *To;
	tsip_header_Contact_t *Contact;
	tsip_header_Call_ID_t *Call_ID;
	tsip_header_CSeq_t *CSeq;
	tsip_header_Expires_t *Expires;

	tsip_header_Content_Type_t *Content_Type;
	tsip_header_Content_Length_t *Content_Length;
	tsk_buffer_t *Content;

	/*== OTHER HEADERS*/
	tsip_headers_L_t *headers;

	/*== */
	tnet_fd_t sockfd;
}
tsip_message_t;

typedef tsip_message_t tsip_request_t; /**< SIP request message. */
typedef tsip_message_t tsip_response_t; /**< SIP response message. */


TINYSIP_API int	tsip_message_add_header(tsip_message_t *self, const tsip_header_t *hdr);
TINYSIP_API int tsip_message_add_headers(tsip_message_t *self, const tsip_headers_L_t *headers);

#if !defined(_MSC_VER) || defined(__GNUC__)
static void TSIP_MESSAGE_ADD_HEADER(tsip_message_t *self, ...)
	{
		va_list ap;
		tsip_header_t *header;
		const tsk_object_def_t *objdef;
		
		va_start(ap, self);
		objdef = va_arg(ap, const tsk_object_def_t*);
		header = tsk_object_new2(objdef, &ap);
		va_end(ap);

		tsip_message_add_header(self, header);
		tsk_object_unref(header);
	}
#else
#define TSIP_MESSAGE_ADD_HEADER(self, objdef, ...)						\
	{																	\
		tsip_header_t *header = tsk_object_new(objdef, __VA_ARGS__);	\
		tsip_message_add_header(self, header);							\
		tsk_object_unref(header);										\
	}
#endif

TINYSIP_API const tsip_header_t *tsip_message_get_headerAt(const tsip_message_t *self, tsip_header_type_t type, size_t index);
TINYSIP_API const tsip_header_t *tsip_message_get_header(const tsip_message_t *self, tsip_header_type_t type);
TINYSIP_API TSIP_BOOLEAN tsip_message_allowed(const tsip_message_t *self, const char* method);
TINYSIP_API TSIP_BOOLEAN tsip_message_supported(const tsip_message_t *self, const char* option);
TINYSIP_API TSIP_BOOLEAN tsip_message_required(const tsip_message_t *self, const char* option);



TINYSIP_API int32_t		tsip_message_getExpires(const tsip_message_t *message);
TINYSIP_API uint32_t	tsip_message_getContent_length(const tsip_message_t *message);
TINYSIP_API int32_t		tsip_message_getCSeq(const tsip_message_t *message);

TINYSIP_API int tsip_message_tostring(const tsip_message_t *self, tsk_buffer_t *output);

TINYSIP_API tsip_request_t *tsip_request_new(const char* method, const tsip_uri_t *request_uri, const tsip_uri_t *from, const tsip_uri_t *to, const char *call_id, int32_t cseq);
TINYSIP_API tsip_response_t *tsip_response_new(short status_code, const char* reason_phrase, const tsip_request_t *request);

TINYSIP_GEXTERN const void *tsip_message_def_t;

TSIP_END_DECLS

#endif /* TSIP_MESSAGE_H */

