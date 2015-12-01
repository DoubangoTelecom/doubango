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

/**@file tsip_message_common.h
 * @brief Common macros and enums for SIP messages.
 * Mostly used to avoid circular #includes.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef TSIP_MESSAGE_COMMON_H
#define TSIP_MESSAGE_COMMON_H

#include "tinysip_config.h"

TSIP_BEGIN_DECLS

#define TSIP_MESSAGE_VERSION_10					"SIP/1.0"
#define TSIP_MESSAGE_VERSION_20					"SIP/2.0"
#define TSIP_MESSAGE_VERSION_DEFAULT			TSIP_MESSAGE_VERSION_20

#define TSIP_MESSAGE_IS_REQUEST(self) ((self) ? (self)->type == tsip_request : tsk_false)
#define TSIP_MESSAGE_IS_RESPONSE(self) ((self) ? (self)->type == tsip_response : tsk_false)

#define TSIP_MESSAGE(self)				((tsip_message_t*)(self))
#define TSIP_MESSAGE_AS_RESPONSE(self)	(TSIP_MESSAGE_IS_RESPONSE((self)) ? (tsip_response_t*)(self) : tsk_null)
#define TSIP_MESSAGE_AS_REQUEST(self)	(TSIP_MESSAGE_IS_REQUEST((self)) ? (tsip_request_t*)(self) : tsk_null)


#define TSIP_RESPONSE_CODE(self)			 (TSIP_MESSAGE_IS_RESPONSE((self)) ? (self)->line.response.status_code : 0)
#define TSIP_RESPONSE_PHRASE(self)			 (TSIP_MESSAGE_IS_RESPONSE((self)) ? (self)->line.response.reason_phrase : tsk_null)

#define TSIP_REQUEST_METHOD(self)			 ((self)->line.request.method)
#define TSIP_REQUEST_URI(self)				 ((self)->line.request.uri)

#define TSIP_MESSAGE_CSEQ_METHOD(self)			((self)->CSeq ? (self)->CSeq->method : tsk_null)
#define TSIP_MESSAGE_HAS_CONTENT(self)			((self) && (self)->Content && (self)->Content->data)
#define TSIP_MESSAGE_CONTENT_LENGTH(self)		(uint32_t)(((self) && (self)->Content_Length) ? (self)->Content_Length->length : 0)
#define TSIP_MESSAGE_CONTENT_DATA(self)			(TSIP_MESSAGE_HAS_CONTENT(self) ? (self)->Content->data : tsk_null)
#define TSIP_MESSAGE_CONTENT_DATA_LENGTH(self)	(TSIP_MESSAGE_HAS_CONTENT(self) ? (self)->Content->size : 0)
#define TSIP_MESSAGE_CONTENT(self)				(TSIP_MESSAGE_HAS_CONTENT(self) ? (self)->Content : tsk_null)
#define TSIP_MESSAGE_CONTENT_TYPE(self)			(((self) && (self)->Content_Type) ? (self)->Content_Type->type : tsk_null)

#define TSIP_REQUEST_IS_ACK(self)			((self) && TSIP_MESSAGE_IS_REQUEST((self)) &&((self)->line.request.request_type==tsip_ACK))
#define TSIP_REQUEST_IS_BYE(self)			((self) && TSIP_MESSAGE_IS_REQUEST((self)) &&((self)->line.request.request_type==tsip_BYE))
#define TSIP_REQUEST_IS_CANCEL(self)		((self) && TSIP_MESSAGE_IS_REQUEST((self)) &&((self)->line.request.request_type==tsip_CANCEL))
#define TSIP_REQUEST_IS_INVITE(self)		((self) && TSIP_MESSAGE_IS_REQUEST((self)) &&((self)->line.request.request_type==tsip_INVITE))
#define TSIP_REQUEST_IS_OPTIONS(self)		((self) && TSIP_MESSAGE_IS_REQUEST((self)) &&((self)->line.request.request_type==tsip_OPTIONS))
#define TSIP_REQUEST_IS_REGISTER(self)		((self) && TSIP_MESSAGE_IS_REQUEST((self)) &&((self)->line.request.request_type==tsip_REGISTER))
#define TSIP_REQUEST_IS_SUBSCRIBE(self)		((self) && TSIP_MESSAGE_IS_REQUEST((self)) &&((self)->line.request.request_type==tsip_SUBSCRIBE))
#define TSIP_REQUEST_IS_NOTIFY(self)		((self) && TSIP_MESSAGE_IS_REQUEST((self)) &&((self)->line.request.request_type==tsip_NOTIFY))
#define TSIP_REQUEST_IS_REFER(self)			((self) && TSIP_MESSAGE_IS_REQUEST((self)) &&((self)->line.request.request_type==tsip_REFER))
#define TSIP_REQUEST_IS_INFO(self)			((self) && TSIP_MESSAGE_IS_REQUEST((self)) &&((self)->line.request.request_type==tsip_INFO))
#define TSIP_REQUEST_IS_UPDATE(self)		((self) && TSIP_MESSAGE_IS_REQUEST((self)) &&((self)->line.request.request_type==tsip_UPDATE))
#define TSIP_REQUEST_IS_MESSAGE(self)		((self) && TSIP_MESSAGE_IS_REQUEST((self)) &&((self)->line.request.request_type==tsip_MESSAGE))
#define TSIP_REQUEST_IS_PUBLISH(self)		((self) && TSIP_MESSAGE_IS_REQUEST((self)) &&((self)->line.request.request_type==tsip_PUBLISH))
#define TSIP_REQUEST_IS_PRACK(self)			((self) && TSIP_MESSAGE_IS_REQUEST((self)) &&((self)->line.request.request_type==tsip_PRACK))

#define TSIP_RESPONSE_IS_TO_ACK(self)			((self) && TSIP_MESSAGE_IS_RESPONSE((self)) && (self)->CSeq &&((self)->CSeq->type==tsip_ACK))
#define TSIP_RESPONSE_IS_TO_BYE(self)			((self) && TSIP_MESSAGE_IS_RESPONSE((self)) && (self)->CSeq &&((self)->CSeq->type==tsip_BYE))
#define TSIP_RESPONSE_IS_TO_CANCEL(self)		((self) && TSIP_MESSAGE_IS_RESPONSE((self)) && (self)->CSeq &&((self)->CSeq->type==tsip_CANCEL))
#define TSIP_RESPONSE_IS_TO_INVITE(self)		((self) && TSIP_MESSAGE_IS_RESPONSE((self)) && (self)->CSeq &&((self)->CSeq->type==tsip_INVITE))
#define TSIP_RESPONSE_IS_TO_OPTIONS(self)		((self) && TSIP_MESSAGE_IS_RESPONSE((self)) && (self)->CSeq &&((self)->CSeq->type==tsip_OPTIONS))
#define TSIP_RESPONSE_IS_TO_REGISTER(self)		((self) && TSIP_MESSAGE_IS_RESPONSE((self)) && (self)->CSeq &&((self)->CSeq->type==tsip_REGISTER))
#define TSIP_RESPONSE_IS_TO_SUBSCRIBE(self)		((self) && TSIP_MESSAGE_IS_RESPONSE((self)) && (self)->CSeq &&((self)->CSeq->type==tsip_SUBSCRIBE))
#define TSIP_RESPONSE_IS_TO_NOTIFY(self)		((self) && TSIP_MESSAGE_IS_RESPONSE((self)) && (self)->CSeq &&((self)->CSeq->type==tsip_NOTIFY))
#define TSIP_RESPONSE_IS_TO_REFER(self)			((self) && TSIP_MESSAGE_IS_RESPONSE((self)) && (self)->CSeq &&((self)->CSeq->type==tsip_REFER))
#define TSIP_RESPONSE_IS_TO_INFO(self)			((self) && TSIP_MESSAGE_IS_RESPONSE((self)) && (self)->CSeq &&((self)->CSeq->type==tsip_INFO))
#define TSIP_RESPONSE_IS_TO_UPDATE(self)		((self) && TSIP_MESSAGE_IS_RESPONSE((self)) && (self)->CSeq &&((self)->CSeq->type==tsip_UPDATE))
#define TSIP_RESPONSE_IS_TO_MESSAGE(self)		((self) && TSIP_MESSAGE_IS_RESPONSE((self)) && (self)->CSeq &&((self)->CSeq->type==tsip_MESSAGE))
#define TSIP_RESPONSE_IS_TO_PUBLISH(self)		((self) && TSIP_MESSAGE_IS_RESPONSE((self)) && (self)->CSeq &&((self)->CSeq->type==tsip_PUBLISH))
#define TSIP_RESPONSE_IS_TO_PRACK(self)			((self) && TSIP_MESSAGE_IS_RESPONSE((self)) && (self)->CSeq &&((self)->CSeq->type==tsip_PRACK))


#define TSIP_RESPONSE_IS(self, code)		(TSIP_RESPONSE_CODE((self)) == code)
#define TSIP_RESPONSE_IS_NXX(self, N)		(TSIP_MESSAGE_IS_RESPONSE((self)) && N##00<= TSIP_RESPONSE_CODE((self)) && TSIP_RESPONSE_CODE((self)) <= N##99)
#define TSIP_RESPONSE_IS_1XX(self)			TSIP_RESPONSE_IS_NXX(self, 1)
#define TSIP_RESPONSE_IS_2XX(self)			TSIP_RESPONSE_IS_NXX(self, 2)
#define TSIP_RESPONSE_IS_3XX(self)			TSIP_RESPONSE_IS_NXX(self, 3)
#define TSIP_RESPONSE_IS_4XX(self)			TSIP_RESPONSE_IS_NXX(self, 4)
#define TSIP_RESPONSE_IS_5XX(self)			TSIP_RESPONSE_IS_NXX(self, 5)
#define TSIP_RESPONSE_IS_6XX(self)			TSIP_RESPONSE_IS_NXX(self, 6)
#define TSIP_RESPONSE_IS_23456(self)		(TSIP_MESSAGE_IS_RESPONSE((self)) && 200<= TSIP_RESPONSE_CODE((self)) && TSIP_RESPONSE_CODE((self)) <= 699)
#define TSIP_RESPONSE_IS_3456(self)			(TSIP_MESSAGE_IS_RESPONSE((self)) && 300<= TSIP_RESPONSE_CODE((self)) && TSIP_RESPONSE_CODE((self)) <= 699)

/** Defines the message type (Request or Response). 
**/
typedef enum tsip_message_type_e
{
	tsip_unknown,
	tsip_request,
	tsip_response
}
tsip_message_type_t;

// SWIG
typedef enum tsip_request_type_e
{
	tsip_NONE = 0,

	tsip_ACK,
	tsip_BYE,
	tsip_CANCEL,
	tsip_INVITE,
	tsip_OPTIONS,
	tsip_REGISTER,
	tsip_SUBSCRIBE,
	tsip_NOTIFY,
	tsip_REFER,
	tsip_INFO,
	tsip_UPDATE,
	tsip_MESSAGE,
	tsip_PUBLISH,
	tsip_PRACK
}
tsip_request_type_t;

TSIP_END_DECLS

#endif /* TSIP_MESSAGE_COMMON_H */
