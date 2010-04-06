/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango.org>
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
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef THTTP_MESSAGE_H
#define THTTP_MESSAGE_H

#include "tinyHTTP_config.h"

#include "tinyHTTP/thttp_url.h"

//#include "tinyHTTP/headers/thttp_header_Call_ID.h"
//#include "tinyHTTP/headers/thttp_header_Contact.h"
#include "tinyHTTP/headers/thttp_header_Content_Length.h"
#include "tinyHTTP/headers/thttp_header_Content_Type.h"
//#include "tinyHTTP/headers/thttp_header_CSeq.h"
//#include "tinyHTTP/headers/thttp_header_Expires.h"
//#include "tinyHTTP/headers/thttp_header_From.h"
//#include "tinyHTTP/headers/thttp_header_P_Access_Network_Info.h"
//#include "tinyHTTP/headers/thttp_header_Via.h"

#include "tsk_object.h"
#include "tsk_buffer.h"

/**@ingroup thttp_message_group
* @def THTTP_MESSAGE_VERSION_10
* HTTP version 1.0.
*/
/**@ingroup thttp_message_group
* @def THTTP_MESSAGE_VERSION_11
* HTTP version 1.1.
*/
/**@ingroup thttp_message_group
* @def THTTP_MESSAGE_VERSION_20
* HTTP version 2.0.
*/
/**@ingroup thttp_message_group
* @def THTTP_MESSAGE_VERSION_DEFAULT
* Default HTTP version. Default value is @ref THTTP_MESSAGE_VERSION_11.
*/

THTTP_BEGIN_DECLS

#define THTTP_MESSAGE_VERSION_10					"HTTP/1.0"
#define THTTP_MESSAGE_VERSION_11					"HTTP/1.1"
#define THTTP_MESSAGE_VERSION_20					"HTTP/2.0"
#define THTTP_MESSAGE_VERSION_DEFAULT			THTTP_MESSAGE_VERSION_11

/**@ingroup thttp_message_group
* @def THTTP_MESSAGE_IS_REQUEST
* Checks whether the HTTP message is a request or not.
* @param self A pointer to a valid @ref thttp_message_t object.
* @retval @ref tsk_true if @a self is a request and @a tsk_false otherwise.
*/
/**@ingroup thttp_message_group
* @def THTTP_MESSAGE_IS_RESPONSE
* Checks whether the HTTP message is a response or not.
* @param self A pointer to a valid @ref thttp_message_t object.
* @retval @ref tsk_true if @a self is a response and @a tsk_false otherwise.
*/
#define THTTP_MESSAGE_IS_REQUEST(self) ((self) ? (self)->type == thttp_request : tsk_false)
#define THTTP_MESSAGE_IS_RESPONSE(self) ((self) ? (self)->type == thttp_response : tsk_false)

/**@ingroup thttp_message_group
* @def THTTP_MESSAGE
* Casts any pointer to a pointer to @ref thttp_message_t.
* @retval A pointer to @ref thttp_message_t.
*/
/**@ingroup thttp_message_group
* @def THTTP_MESSAGE_AS_RESPONSE
* Casts any pointer to a pointer to @ref thttp_response_t.
* @retval A pointer to @ref thttp_response_t.
*/
/**@ingroup thttp_message_group
* @def THTTP_MESSAGE_AS_REQUEST
* Casts any pointer to a pointer to @ref thttp_request_t.
* @retval A pointer to @ref thttp_request_t.
*/
#define THTTP_MESSAGE(self)				((thttp_message_t*)(self))
#define THTTP_MESSAGE_AS_RESPONSE(self)	((thttp_response_t*)(self))
#define THTTP_MESSAGE_AS_REQUEST(self)	((thttp_request_t*)(self))


/**@ingroup thttp_message_group
* @def THTTP_MESSAGE_CREATE
* Creates new HTTP message. Could be either a request or a response.
* @retval @ref thttp_message_t object.
* @sa @ref THTTP_REQUEST_CREATE<br>@ref THTTP_RESPONSE_CREATE
*/
/**@ingroup thttp_message_group
*@def THTTP_REQUEST_CREATE
* Creates new HTTP request.
* @param method The method (const char*). e.g. GET, POST, HEAD ...
* @param url The url (@ref thttp_url_t).
* @retval @ref thttp_request_t object.
*
* @code
// example
thttp_url_t* url;
thttp_request_t* request;
if((url = thttp_url_parse("http://www.google.com", strlen("http://www.google.com")))){
	request = THTTP_REQUEST_CREATE("GET", url);
	// ... 
	TSK_OBJECT_SAFE_FREE(url);
	TSK_OBJECT_SAFE_FREE(request);
}
* @endcode
*/
/**@ingroup thttp_message_group
*@def THTTP_RESPONSE_CREATE
* Creates a HTTP response.
* @param request The request (@ref thttp_request_t) from which to create the response.
* @param status_code The status code (short).
* @param reason_phrase The reason phrase (const char*).
* @retval @ref thttp_response_t object.
*
* @code
// example
//thttp_request_t* request;
thttp_response_t* response;
if((response = THTTP_RESPONSE_CREATE(request, 200, "OK"))){
	TSK_OBJECT_SAFE_FREE(response);
}
* @endcode
*/
#define THTTP_MESSAGE_CREATE()											tsk_object_new(thttp_message_def_t, (thttp_message_type_t)thttp_unknown)
#define THTTP_REQUEST_CREATE(method, url)								tsk_object_new(thttp_message_def_t, (thttp_message_type_t)thttp_request, (const char*)method, (const thttp_url_t*)url)
#define THTTP_RESPONSE_CREATE(request, status_code, reason_phrase)		tsk_object_new(thttp_message_def_t, (thttp_message_type_t)thttp_response, (const thttp_request_t*)request, (short)status_code, (const char*)reason_phrase)

/**@ingroup thttp_message_group
*@def THTTP_RESPONSE_CODE
* Gets the status code of the response.
* @param self A pointer to a @ref thttp_response_t object.
* @retval The status code (short).
*/
/**@ingroup thttp_message_group
*@def THTTP_RESPONSE_PHRASE
* Gets the reason phrase of the response.
* @param self A pointer to a @ref thttp_response_t object.
* @retval The phrase (const char*).
*/

#define THTTP_RESPONSE_CODE(self)			 (THTTP_MESSAGE_IS_RESPONSE((self)) ? (self)->status_code : 0)
#define THTTP_RESPONSE_PHRASE(self)			 ((self)->reason_phrase)

/**@ingroup thttp_message_group
*@def THTTP_REQUEST_METHOD
* Gets the method of the request.
* @param self A pointer to a @ref thttp_request_t object.
* @retval The method (const char*).
*/
/**@ingroup thttp_message_group
*@def THTTP_REQUEST_URL
* Gets the URL of the request.
* @param self A pointer to a @ref thttp_request_t object.
* @retval The Url (@ref thttp_url_t).
*/
#define THTTP_REQUEST_METHOD(self)			 ((self) ? (self)->method : tsk_null)
#define THTTP_REQUEST_URL(self)				 ((self) ? (self)->url : tsk_null)

/**@ingroup thttp_message_group
*@def THTTP_MESSAGE_CONTENT_LENGTH
* Gets the content length.
* @param self A pointer to a @ref thttp_message_t object.
* @retval The content length (uint32_t).
*/
/**@ingroup thttp_message_group
*@def THTTP_MESSAGE_CONTENT
* Gets the content value.
* @param self A pointer to a @ref thttp_message_t object.
* @retval A pointer to the content (void*).
*/
/**@ingroup thttp_message_group
*@def THTTP_MESSAGE_HAS_CONTENT
* Checks whether the message has a content or not.
* @param self A pointer to a @ref thttp_message_t object.
* @retval @ref tsk_true if the message has a content and @a tsk_false otherwise.
*/
#define THTTP_MESSAGE_CONTENT_LENGTH(self) (uint32_t)(((self) && (self)->Content_Length) ? (self)->Content_Length->length : 0)
#define THTTP_MESSAGE_CONTENT(self)		 (THTTP_MESSAGE_HAS_CONTENT(self) ? (self)->Content->data : 0)
#define THTTP_MESSAGE_HAS_CONTENT(self)	 ((self) && (self)->Content)

#define THTTP_RESPONSE_IS(self, code)		(THTTP_RESPONSE_CODE((self)) == code)
#define THTTP_RESPONSE_IS_NXX(self, N)		(N##00<= THTTP_RESPONSE_CODE((self)) && THTTP_RESPONSE_CODE((self)) <= N##99)
#define THTTP_RESPONSE_IS_1XX(self)			THTTP_RESPONSE_IS_NXX(self, 1)
#define THTTP_RESPONSE_IS_2XX(self)			THTTP_RESPONSE_IS_NXX(self, 2)
#define THTTP_RESPONSE_IS_3XX(self)			THTTP_RESPONSE_IS_NXX(self, 3)
#define THTTP_RESPONSE_IS_4XX(self)			THTTP_RESPONSE_IS_NXX(self, 4)
#define THTTP_RESPONSE_IS_5XX(self)			THTTP_RESPONSE_IS_NXX(self, 5)
#define THTTP_RESPONSE_IS_6XX(self)			THTTP_RESPONSE_IS_NXX(self, 6)
#define THTTP_RESPONSE_IS_23456(self)		(200<= THTTP_RESPONSE_CODE((self)) && THTTP_RESPONSE_CODE((self)) <= 699)

/**@ingroup thttp_message_group
* Defines the message type (Request or Response). 
**/
typedef enum thttp_message_type_e
{
	thttp_unknown,
	thttp_request,
	thttp_response
}
thttp_message_type_t;

/**@ingroup thttp_message_group
 *Represents a HTTP message. A HTTP message is either a request from a client to a server,
 * 	or a response from a server to a client.
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
TINYHTTP_API int thttp_message_append_content(thttp_message_t *self, const void* content, size_t size);

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

TINYHTTP_API int thttp_message_serialize(const thttp_message_t *self, tsk_buffer_t *output);
TINYHTTP_API char* thttp_message_tostring(const thttp_message_t *self);

TINYHTTP_API thttp_request_t *thttp_request_new(const char* method, const thttp_url_t *request_url);
TINYHTTP_API thttp_response_t *thttp_response_new(short status_code, const char* reason_phrase, const thttp_request_t *request);

TINYHTTP_GEXTERN const void *thttp_message_def_t;

THTTP_END_DECLS

#endif /* THTTP_MESSAGE_H */

