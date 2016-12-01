/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou [at) doubango (DOT) org>
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

/**@file tmsrp_message.h
 * @brief MSRP message.
 *
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 *

 */
#ifndef TINYMSRP_MESSAGE_H
#define TINYMSRP_MESSAGE_H

#include "tinymsrp_config.h"

#include "tinymsrp/headers/tmsrp_header.h"

#include "tinymsrp/headers/tmsrp_header_Byte-Range.h"
#include "tinymsrp/headers/tmsrp_header_Content-Type.h"
#include "tinymsrp/headers/tmsrp_header_Failure-Report.h"
#include "tinymsrp/headers/tmsrp_header_From-Path.h"
#include "tinymsrp/headers/tmsrp_header_Message-ID.h"
#include "tinymsrp/headers/tmsrp_header_Status.h"
#include "tinymsrp/headers/tmsrp_header_Success-Report.h"
#include "tinymsrp/headers/tmsrp_header_To-Path.h"

TMSRP_BEGIN_DECLS

#define TMSRP_MESSAGE_IS_REQUEST(self) ((self) ? (self)->type == tmsrp_request : tsk_false)
#define TMSRP_MESSAGE_IS_RESPONSE(self) ((self) ? (self)->type == tmsrp_response : tsk_false)

#define TMSRP_MESSAGE(self)				((tmsrp_message_t*)(self))
#define TMSRP_MESSAGE_AS_RESPONSE(self)	((tmsrp_response_t*)(self))
#define TMSRP_MESSAGE_AS_REQUEST(self)	((tmsrp_request_t*)(self))


#define TMSRP_RESPONSE_CODE(self)			 (TMSRP_MESSAGE_IS_RESPONSE((self)) ? (self)->line.response.status : 0)
#define TMSRP_RESPONSE_PHRASE(self)			 (TMSRP_MESSAGE_IS_RESPONSE((self)) ? (self)->line.response.comment : tsk_null)

#define TMSRP_REQUEST_METHOD(self)			 (TMSRP_MESSAGE_IS_REQUEST((self)) ? (self)->line.request.method : tsk_null)
#define TMSRP_REQUEST_IS_SEND(self)			 (TMSRP_MESSAGE_IS_REQUEST(self) && (self)->line.request.type == tmsrp_SEND)
#define TMSRP_REQUEST_IS_REPORT(self)		 (TMSRP_MESSAGE_IS_REQUEST(self) && (self)->line.request.type == tmsrp_REPORT)
#define TMSRP_REQUEST_IS_AUTH(self)			 (TMSRP_MESSAGE_IS_REQUEST(self) && (self)->line.request.type == tmsrp_AUTH)

#define TMSRP_MESSAGE_HAS_CONTENT(message)	 ((message) && (message)->Content && (message)->Content->data)
#define TMSRP_MESSAGE_CONTENT(message)		 (TMSRP_MESSAGE_HAS_CONTENT(message) ? (message)->Content->data : 0)

#define TMSRP_RESPONSE_IS(self, code)		(TMSRP_RESPONSE_CODE((self)) == code)
#define TMSRP_RESPONSE_IS_NXX(self, N)		(TMSRP_MESSAGE_IS_RESPONSE((self)) && N##00<= TMSRP_RESPONSE_CODE((self)) && TMSRP_RESPONSE_CODE((self)) <= N##99)
#define TMSRP_RESPONSE_IS_1XX(self)			TMSRP_RESPONSE_IS_NXX(self, 1)
#define TMSRP_RESPONSE_IS_2XX(self)			TMSRP_RESPONSE_IS_NXX(self, 2)
#define TMSRP_RESPONSE_IS_3XX(self)			TMSRP_RESPONSE_IS_NXX(self, 3)
#define TMSRP_RESPONSE_IS_4XX(self)			TMSRP_RESPONSE_IS_NXX(self, 4)
#define TMSRP_RESPONSE_IS_5XX(self)			TMSRP_RESPONSE_IS_NXX(self, 5)
#define TMSRP_RESPONSE_IS_6XX(self)			TMSRP_RESPONSE_IS_NXX(self, 6)
#define TMSRP_RESPONSE_IS_23456(self)		(TMSRP_MESSAGE_IS_RESPONSE((self)) && 200<= TMSRP_RESPONSE_CODE((self)) && TMSRP_RESPONSE_CODE((self)) <= 699)

/**	Defines the message type (Request or Response).
**/
typedef enum tmsrp_message_type_e {
    tmsrp_unknown,
    tmsrp_request,
    tmsrp_response
}
tmsrp_message_type_t;

typedef enum tmsrp_request_type_e {
    tmsrp_NONE = 0,

    tmsrp_SEND,
    tmsrp_REPORT,
    tmsrp_AUTH
    //...
}
tmsrp_request_type_t;

typedef struct tmsrp_message_s {
    TSK_DECLARE_OBJECT;

    tmsrp_message_type_t type;
    char* tid;
    union {
        struct {
            char* method;
            tmsrp_request_type_t type;
        } request;

        struct {
            short status;
            char* comment;
        } response;
    } line;

    // Very common headers
    tmsrp_header_To_Path_t* To;
    tmsrp_header_From_Path_t* From;

    tmsrp_header_Message_ID_t* MessageID;

    tmsrp_header_Byte_Range_t* ByteRange;
    tmsrp_header_Failure_Report_t* FailureReport;
    tmsrp_header_Success_Report_t* SuccessReport;
    tmsrp_header_Status_t* Status;

    //! List of @ref tmsrp_header_t elements.
    tmsrp_headers_L_t* headers;

    // Content
    tmsrp_header_Content_Type_t* ContentType;
    tsk_buffer_t *Content;

    // End line
    struct {
        char* tid;
        char cflag;
    } end_line;
}
tmsrp_message_t;

typedef tmsrp_message_t tmsrp_request_t; /**< MSRP request message. */
typedef tmsrp_message_t tmsrp_response_t; /**< MSRP response message. */


TINYMSRP_API tmsrp_message_t* tmsrp_message_create(tmsrp_message_type_t type, const char* tid, const char* method, short status, const char* comment);
TINYMSRP_API tmsrp_message_t* tmsrp_request_create(const char* tid, const char* method);
TINYMSRP_API tmsrp_message_t* tmsrp_response_create(const char* tid, short status, const char* comment);
TINYMSRP_API tmsrp_message_t* tmsrp_message_create_null();

TINYMSRP_API int tmsrp_message_add_header(tmsrp_message_t *self, const tmsrp_header_t *hdr);
TINYMSRP_API int tmsrp_message_add_headers(tmsrp_message_t *self, ...);

#if 0
static void TMSRP_MESSAGE_ADD_HEADER(tmsrp_message_t *self, ...)
{
    va_list ap;
    tmsrp_header_t *header;
    const tsk_object_def_t *objdef;

    va_start(ap, self);
    objdef = va_arg(ap, const tsk_object_def_t*);
    header = tsk_object_new_2(objdef, &ap);
    va_end(ap);

    tmsrp_message_add_header(self, header);
    tsk_object_unref(header);
}
#else
#define TMSRP_MESSAGE_ADD_HEADER(self, objdef, ...)						\
	{																	\
		tmsrp_header_t *header = (tmsrp_header_t *)tsk_object_new(objdef, ##__VA_ARGS__);	\
		tmsrp_message_add_header(self, header);							\
		tsk_object_unref(header);										\
	}
#endif

TINYMSRP_API tmsrp_request_type_t tmsrp_request_get_type(const char* method);
TINYMSRP_API const tmsrp_header_t *tmsrp_message_get_headerAt(const tmsrp_message_t *self, tmsrp_header_type_t type, tsk_size_t index);
TINYMSRP_API const tmsrp_header_t *tmsrp_message_get_header(const tmsrp_message_t *self, tmsrp_header_type_t type);
TINYMSRP_API const tmsrp_header_t *tmsrp_message_get_headerByName(const tmsrp_message_t *self, const char* name);

TINYMSRP_API int tmsrp_message_add_content(tmsrp_message_t *self, const char* content_type, const void* content, tsk_size_t size);

TINYMSRP_API int tmsrp_message_serialize(const tmsrp_message_t *self, tsk_buffer_t *output);
TINYMSRP_API char* tmsrp_message_tostring(const tmsrp_message_t *self);


TINYMSRP_GEXTERN const tsk_object_def_t *tmsrp_message_def_t;

TMSRP_END_DECLS

#endif /* TINYMSRP_MESSAGE_H */
