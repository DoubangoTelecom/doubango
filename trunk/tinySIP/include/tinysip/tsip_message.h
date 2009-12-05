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

#include "tinysip/headers/tsip_header_Via.h"

#include "tsk_object.h"
#include "tsk_safeobj.h"

#define TSIP_MESSAGE_IS_REQUEST(message) ((message) ? (message)->type == tsip_request : 0)
#define TSIP_MESSAGE_IS_RESPONSE(message) ((message) ? (message)->type == tsip_response : 0)


/**@def TSIP_MESSAGE_CREATE
* Creates new sip messgae. Could be either a request or a response.
* You must call @ref TSIP_MESSAGE_SAFE_FREE to free the message.
* @sa TSIP_MESSAGE_SAFE_FREE.
*/
/**@def TSIP_MESSAGE_SAFE_FREE
* Safely free a sip messgae previously created using TSIP_MESSAGE_CREATE.
* @sa TSIP_MESSAGE_CREATE.
*/
#define TSIP_MESSAGE_CREATE()		tsk_object_new(tsip_message_def_t)
#define TSIP_MESSAGE_SAFE_FREE(self)	tsk_object_unref(self), self = 0

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
	char *uri;	/**< The Request-URI is a SIP or SIPS URI as described in Section 19.1 or a general URI (RFC 2396 [5]).  It indicates
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
	TSK_DECLARE_SAFEOBJ;
	
	char *sip_version; /**< The SIP version. Only 'SIP/2.0' is supported. */
	tsip_message_type_t type; /**< The type of this SIP message. */

	union
	{
		tsip_request_line_t line_request; /**< Request status line. Only if message type is @ref tsip_request. */
		tsip_status_line_t line_status; /**< Status line. Only if message type is @ref tsip_response. */
	};
	
	
	tsip_header_Via_t *firstVia; /**< First Via header. */
}
tsip_message_t;

typedef tsip_message_t tsip_request_t; /**< SIP request message. */
typedef tsip_message_t tsip_response_t; /**< SIP response message. */

TINYSIP_API const void *tsip_message_def_t;

#endif /* TSIP_MESSAGE_H */