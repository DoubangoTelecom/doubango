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

/**@file tsip_message.h
 * @brief Represents a SIP message. A SIP message is either a request from a client to a server, or a
 * response from a server to a client. See RFC 3261 suc-bclause 7.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef TSIP_MESSAGE_H
#define TSIP_MESSAGE_H

#include "tinysip_config.h"

#include "tinysip/tsip_message_common.h"

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
#include "tnet_socket.h"

#include "tsk_object.h"
#include "tsk_buffer.h"

TSIP_BEGIN_DECLS

///**
// * @struct	tsip_request_line_t
// *
// * @brief	Defines SIP request header line.
// * ABNF: Request-Line  =  Method SP Request-URI SP SIP-Version CRLF
// * Ref: RFC 3261 subclause 7.1.
// * @sa		tsip_status_line_t.
// *
// * @author	Mamadou
// * @date	12/3/2009
//**/
//typedef struct tsip_request_line_s
//{
//	char *method; /**< SIP method name. e.g REGISTER, ACK or INVITE.*/
//	tsip_uri_t *uri;	/**< The Request-URI is a SIP or SIPS URI as described in Section 19.1 or a general URI (RFC 2396 [5]).  It indicates
//           the user or service to which this request is being addressed. The Request-URI MUST NOT contain unescaped spaces or control
//           characters and MUST NOT be enclosed in "<>". */
//}
//tsip_request_line_t;
//
///**
// * @struct	tsip_status_line_t
// *
// * @brief	Defines SIP response status line.
// * ABNF: Status-Line  =  SIP-Version SP Status-Code SP Reason-Phrase CRLF
// * Ref: RFC 3261 subclause 7.2.
// * @sa		tsip_request_line_t.
// *
// * @author	Mamadou
// * @date	12/3/2009
//**/
//typedef struct tsip_status_line_s
//{
//	short status_code; /**< 3-digit integer result code that indicates the outcome of an attempt to understand and satisfy a request. */
//	char *reason_phrase; /**< Textual description related to the status code. */
//}
//tsip_status_line_t;

/**
 * @struct	tsip_message_t
 *
 * @brief	Represents a SIP message. A SIP message is either a request from a client to a server,
 * 			or a response from a server to a client. See RFC 3261 suc-bclause 7.
 *			You must call @ref tsip_message_create to create a sip message and TSK_OBJECT_SAFE_FREE to free the message.
 *
 * @author	Mamadou
 * @date	12/2/2009
**/
typedef struct tsip_message_s {
    TSK_DECLARE_OBJECT;

    char *sip_version; /**< The SIP version. Only 'SIP/2.0' is supported. */
    tsip_message_type_t type; /**< The type of this SIP message. */

    /* Request Line */
    union {
        struct {
            char *method; /**< SIP method name. e.g REGISTER, ACK or INVITE.*/
            tsip_uri_t *uri;	/**< The Request-URI is a SIP or SIPS URI as described in Section 19.1 or a general URI (RFC 2396 [5]).  It indicates
				   the user or service to which this request is being addressed. The Request-URI MUST NOT contain unescaped spaces or control
				   characters and MUST NOT be enclosed in "<>". */
            tsip_request_type_t request_type;
        } request;
        struct {
            short status_code; /**< 3-digit integer result code that indicates the outcome of an attempt to understand and satisfy a request. */
            char *reason_phrase; /**< Textual description related to the status code. */
        } response;
    } line;


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

    /*== to hack the message */
    char* sigcomp_id;
    tnet_fd_t local_fd;
    struct sockaddr_storage remote_addr; // Only valid for Dgram
    tsk_bool_t update;
    enum tnet_socket_type_e src_net_type;
    enum tnet_socket_type_e dst_net_type;
    char* dst_address;
    tnet_port_t dst_port;
}
tsip_message_t;

typedef tsip_message_t tsip_request_t; /**< SIP request message. */
typedef tsip_message_t tsip_response_t; /**< SIP response message. */


TINYSIP_API int	tsip_message_add_header(tsip_message_t *self, const tsip_header_t *hdr);
TINYSIP_API int tsip_message_add_headers(tsip_message_t *self, ...);
TINYSIP_API int tsip_message_add_content(tsip_message_t *self, const char* content_type, const void* content, tsk_size_t size);

#if defined(__SYMBIAN32__) && 0
static void TSIP_MESSAGE_ADD_HEADER(tsip_message_t *self, ...)
{
    va_list ap;
    tsip_header_t *header;
    const tsk_object_def_t *objdef;

    va_start(ap, self);
    objdef = va_arg(ap, const tsk_object_def_t*);
    header = (tsip_header_t *)tsk_object_new_2(objdef, &ap);
    va_end(ap);

    tsip_message_add_header(self, header);
    tsk_object_unref(header);
}
#else
#define TSIP_MESSAGE_ADD_HEADER(self, objdef, ...)										\
	{																					\
		tsip_header_t *header = (tsip_header_t *)tsk_object_new(objdef, ##__VA_ARGS__);	\
		tsip_message_add_header(self, header);											\
		tsk_object_unref(header);														\
	}
#endif

TINYSIP_API const tsip_header_t *tsip_message_get_headerAt(const tsip_message_t *self, tsip_header_type_t type, tsk_size_t index);
TINYSIP_API const tsip_header_t *tsip_message_get_headerLast(const tsip_message_t *self, tsip_header_type_t type);
TINYSIP_API const tsip_header_t *tsip_message_get_header(const tsip_message_t *self, tsip_header_type_t type);
TINYSIP_API tsk_bool_t tsip_message_allowed(const tsip_message_t *self, const char* method);
TINYSIP_API tsk_bool_t tsip_message_supported(const tsip_message_t *self, const char* option);
TINYSIP_API tsk_bool_t tsip_message_required(const tsip_message_t *self, const char* option);



TINYSIP_API int64_t		tsip_message_getExpires(const tsip_message_t *message);
TINYSIP_API uint32_t	tsip_message_getContent_length(const tsip_message_t *message);
TINYSIP_API int32_t		tsip_message_getCSeq(const tsip_message_t *message);

TINYSIP_API int tsip_message_tostring(const tsip_message_t *self, tsk_buffer_t *output);

TINYSIP_API tsip_request_type_t tsip_request_get_type(const char* method);
TINYSIP_API tsip_request_t *tsip_request_new(const char* method, const tsip_uri_t *request_uri, const tsip_uri_t *from, const tsip_uri_t *to, const char *call_id, int32_t cseq);
TINYSIP_API tsip_response_t *tsip_response_new(short status_code, const char* reason_phrase, const tsip_request_t *request);

TINYSIP_API tsip_message_t* tsip_message_create();
TINYSIP_API tsip_request_t* tsip_request_create(const char* method, const tsip_uri_t* uri);
TINYSIP_API tsip_response_t* tsip_response_create(const tsip_request_t* request, short status_code, const char* reason_phrase);

TINYSIP_GEXTERN const tsk_object_def_t *tsip_message_def_t;

TSIP_END_DECLS

#endif /* TSIP_MESSAGE_H */

