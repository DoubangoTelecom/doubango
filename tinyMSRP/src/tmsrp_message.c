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

/**@file tmsrp_message.c
 * @brief MSRP message.
 *
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 *

 */

#include "tinymsrp/tmsrp_message.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

/*== Predicate function to find tmsrp_header_t object by type. */
static int pred_find_header_by_type(const tsk_list_item_t *item, const void *tmsrp_htype)
{
    if(item && item->data) {
        tmsrp_header_t *header = item->data;
        tmsrp_header_type_t htype = *((tmsrp_header_type_t*)tmsrp_htype);
        return (header->type - htype);
    }
    return -1;
}

/*== Predicate function to find tmsrp_header_t object by name. */
static int pred_find_header_by_name(const tsk_list_item_t *item, const void *name)
{
    if(item && item->data && name) {
        tmsrp_header_t *header = item->data;
        return tsk_stricmp(tmsrp_header_get_nameex(header), (const char*)name);
    }
    return -1;
}


tmsrp_message_t* tmsrp_message_create(tmsrp_message_type_t type, const char* tid, const char* method, short status, const char* comment)
{
    tmsrp_message_t* message;
    if((message = tsk_object_new(tmsrp_message_def_t))) {
        message->type = type;
        message->tid = tsk_strdup(tid);
        if(message->type == tmsrp_response) {
            message->line.response.status = status;
            message->line.response.comment = tsk_strdup(comment);
        }
        else {
            message->line.request.method = tsk_strdup(method);
            message->line.request.type = tmsrp_request_get_type(method);
        }

        message->headers = tsk_list_create();

        message->end_line.tid = tsk_strdup(message->tid);
        message->end_line.cflag = '$';
    }
    return message;
}

tmsrp_message_t* tmsrp_request_create(const char* tid, const char* method)
{
    return tmsrp_message_create(tmsrp_request, tid, method, 0, tsk_null);
}

tmsrp_message_t* tmsrp_response_create(const char* tid, short status, const char* comment)
{
    return tmsrp_message_create(tmsrp_response, tid, tsk_null, status, comment);
}

tmsrp_message_t* tmsrp_message_create_null()
{
    return tmsrp_message_create(tmsrp_unknown, tsk_null, tsk_null, 0, tsk_null);
}


int tmsrp_message_add_header(tmsrp_message_t *self, const tmsrp_header_t *hdr)
{
#define ADD_HEADER(type, field) \
		case tmsrp_htype_##type: \
			{ \
				if(!self->field) \
				{ \
					self->field = (tmsrp_header_##type##_t*)header; \
					return 0; \
				} \
				break; \
			}

    if(self && hdr) {
        tmsrp_header_t *header = tsk_object_ref((void*)hdr);

        switch(header->type) {
            ADD_HEADER(To_Path, To);
            ADD_HEADER(From_Path, From);
            ADD_HEADER(Message_ID, MessageID);
            ADD_HEADER(Byte_Range, ByteRange);
            ADD_HEADER(Failure_Report, FailureReport);
            ADD_HEADER(Success_Report, SuccessReport);
            ADD_HEADER(Status, Status);
            ADD_HEADER(Content_Type, ContentType);

        default:
            break;
        }

        tsk_list_push_back_data(self->headers, (void**)&header);

        return 0;
    }
    return -1;
}

int tmsrp_message_add_headers(tmsrp_message_t *self, ...)
{
    const tsk_object_def_t* objdef;
    tmsrp_header_t *header;
    va_list ap;

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    va_start(ap, self);
    while((objdef = va_arg(ap, const tsk_object_def_t*))) {
        if((header = tsk_object_new_2(objdef, &ap))) {
            tmsrp_message_add_header(self, header);
            TSK_OBJECT_SAFE_FREE(header);
        }
    }
    va_end(ap);

    return 0;
}

tmsrp_request_type_t tmsrp_request_get_type(const char* method)
{
    if(method) {
        if(tsk_strequals(method, "SEND")) {
            return tmsrp_SEND;
        }
        else if(tsk_strequals(method, "REPORT")) {
            return tmsrp_REPORT;
        }
        else if(tsk_strequals(method, "AUTH")) {
            return tmsrp_AUTH;
        }
    }
    return tmsrp_NONE;
}

const tmsrp_header_t *tmsrp_message_get_headerAt(const tmsrp_message_t *self, tmsrp_header_type_t type, tsk_size_t index)
{
    tsk_size_t pos = 0;
    tsk_list_item_t *item;
    const tmsrp_header_t* hdr = 0;

    if(self) {
        switch(type) {
        case tmsrp_htype_To_Path:
            if(index == 0) {
                hdr = (const tmsrp_header_t*)self->To;
                goto bail;
            }
            else {
                pos++;
            }
            break;
        case tmsrp_htype_From_Path:
            if(index == 0) {
                hdr = (const tmsrp_header_t*)self->From;
                goto bail;
            }
            else {
                pos++;
            }
            break;
        case tmsrp_htype_Message_ID:
            if(index == 0) {
                hdr = (const tmsrp_header_t*)self->MessageID;
                goto bail;
                break;
            }
            else {
                pos++;
            }
        case tmsrp_htype_Byte_Range:
            if(index == 0) {
                hdr = (const tmsrp_header_t*)self->ByteRange;
                goto bail;
            }
            else {
                pos++;
            }
            break;
        case tmsrp_htype_Failure_Report:
            if(index == 0) {
                hdr = (const tmsrp_header_t*)self->FailureReport;
                goto bail;
            }
            else {
                pos++;
            }
            break;
        case tmsrp_htype_Success_Report:
            if(index == 0) {
                hdr = (const tmsrp_header_t*)self->SuccessReport;
                goto bail;
            }
            else {
                pos++;
            }
            break;
        case tmsrp_htype_Status:
            if(index == 0) {
                hdr = (const tmsrp_header_t*)self->Status;
                goto bail;
            }
            else {
                pos++;
            }
            break;
        case tmsrp_htype_Content_Type:
            if(index == 0) {
                hdr = (const tmsrp_header_t*)self->ContentType;
                goto bail;
            }
            else {
                pos++;
            }
            break;
        default:
            break;
        }

        tsk_list_foreach(item, self->headers) {
            if(!pred_find_header_by_type(item, &type)) {
                if(pos++ >= index) {
                    hdr = item->data;
                    break;
                }
            }
        }
    }

bail:
    return hdr;
}

const tmsrp_header_t *tmsrp_message_get_header(const tmsrp_message_t *self, tmsrp_header_type_t type)
{
    return tmsrp_message_get_headerAt(self, type, 0);
}

const tmsrp_header_t *tmsrp_message_get_headerByName(const tmsrp_message_t *self, const char* name)
{
    if(self && self->headers) {
        const tsk_list_item_t* item;
        if((item = tsk_list_find_item_by_pred(self->headers, pred_find_header_by_name, name))) {
            return item->data;
        }
    }
    return tsk_null;
}

int tmsrp_message_add_content(tmsrp_message_t *self, const char* content_type, const void* content, tsk_size_t size)
{
    if(self) {
        if(content_type) {
            TSK_OBJECT_SAFE_FREE(self->ContentType);
        }
        TSK_OBJECT_SAFE_FREE(self->Content);

        if(content_type) {
            TMSRP_MESSAGE_ADD_HEADER(self, TMSRP_HEADER_CONTENT_TYPE_VA_ARGS(content_type));
        }
        self->Content = tsk_buffer_create(content, size);

        return 0;
    }
    return -1;
}

int tmsrp_message_serialize(const tmsrp_message_t *self, tsk_buffer_t *output)
{
    if(!self || !output) {
        return -1;
    }

    if(TMSRP_MESSAGE_IS_REQUEST(self)) {
        /* 	pMSRP    SP    transact-id    SP    method    CRLF */
        tsk_buffer_append_2(output, "MSRP %s %s\r\n", self->tid, self->line.request.method);
    }
    else {
        /* 	pMSRP    SP    transact-id    SP    status-code    [SP comment]   CRLF */
        tsk_buffer_append_2(output, "MSRP %s %3hi%s%s\r\n",
                            self->tid,
                            self->line.response.status,
                            self->line.response.comment ? " " : "",
                            self->line.response.comment ? self->line.response.comment  : ""
                           );
    }

    /* To-Path */
    if(self->To) {
        tmsrp_header_serialize(TMSRP_HEADER(self->To), output);
    }
    /* From-Path */
    if(self->From) {
        tmsrp_header_serialize(TMSRP_HEADER(self->From), output);
    }
    /* Message-Id */
    if(self->MessageID) {
        tmsrp_header_serialize(TMSRP_HEADER(self->MessageID), output);
    }
    /* Byte-Range */
    if(self->ByteRange) {
        tmsrp_header_serialize(TMSRP_HEADER(self->ByteRange), output);
    }
    /* Failure-Report */
    if(self->FailureReport) {
        tmsrp_header_serialize(TMSRP_HEADER(self->FailureReport), output);
    }
    /* Success-Report */
    if(self->SuccessReport) {
        tmsrp_header_serialize(TMSRP_HEADER(self->SuccessReport), output);
    }
    /* Status */
    if(self->Status) {
        tmsrp_header_serialize(TMSRP_HEADER(self->Status), output);
    }

    /* All other headers (Other-Mime-headers)
    	- Should be empty if no content is added (see below) but ...
    */
    {
        tsk_list_item_t *item;
        tsk_list_foreach(item, self->headers) {
            tmsrp_header_t *hdr = item->data;
            tmsrp_header_serialize(hdr, output);
        }
    }

    /* RFC 4975 - 7.1. Constructing Requests
    	A request with no body MUST NOT include a Content-Type or any other
    	MIME-specific header fields.  A request without a body MUST contain
    	an end-line after the final header field.  No extra CRLF will be
    	present between the header section and the end-line.
    */
    /* CONTENT */
    if(TMSRP_MESSAGE_HAS_CONTENT(self)) {
        /* Content-Type */
        if(self->ContentType) {
            tmsrp_header_serialize(TMSRP_HEADER(self->ContentType), output);
        }
        tsk_buffer_append(output, "\r\n", 2);
        tsk_buffer_append(output, TSK_BUFFER_TO_STRING(self->Content), TSK_BUFFER_SIZE(self->Content));
        tsk_buffer_append(output, "\r\n", 2);
    }

    /* END LINE */
    tsk_buffer_append_2(output, "-------%s%c\r\n", self->end_line.tid, self->end_line.cflag);

    return 0;
}


char* tmsrp_message_tostring(const tmsrp_message_t *self)
{
    tsk_buffer_t* output;
    char* ret = tsk_null;

    if((output = tsk_buffer_create_null())) {
        if(!tmsrp_message_serialize(self, output)) {
            ret = tsk_strndup(output->data, output->size);
        }
        TSK_OBJECT_SAFE_FREE(output);
    }
    return ret;
}




























//=================================================================================================
//	MSRP object definition
//
static void* tmsrp_message_ctor(tsk_object_t * self, va_list * app)
{
    tmsrp_message_t *message = self;
    if(message) {
    }
    return self;
}

static void* tmsrp_message_dtor(tsk_object_t * self)
{
    tmsrp_message_t *message = self;
    if(message) {
        TSK_FREE(message->tid);

        // request
        if(TMSRP_MESSAGE_IS_REQUEST(message)) {
            TSK_FREE(message->line.request.method);
        }
        // response
        if(TMSRP_MESSAGE_IS_RESPONSE(message)) {
            TSK_FREE(message->line.response.comment);
        }

        // Very common headers
        TSK_OBJECT_SAFE_FREE(message->To);
        TSK_OBJECT_SAFE_FREE(message->From);

        TSK_OBJECT_SAFE_FREE(message->MessageID);

        TSK_OBJECT_SAFE_FREE(message->ByteRange);
        TSK_OBJECT_SAFE_FREE(message->FailureReport);
        TSK_OBJECT_SAFE_FREE(message->SuccessReport);
        TSK_OBJECT_SAFE_FREE(message->Status);

        // all other headers
        TSK_OBJECT_SAFE_FREE(message->headers);

        // content
        TSK_OBJECT_SAFE_FREE(message->ContentType);
        TSK_OBJECT_SAFE_FREE(message->Content);

        // end-line
        TSK_FREE(message->end_line.tid);
    }
    return self;
}

static int tmsrp_message_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
    return -1;
}

static const tsk_object_def_t tmsrp_message_def_s = {
    sizeof(tmsrp_message_t),
    tmsrp_message_ctor,
    tmsrp_message_dtor,
    tmsrp_message_cmp,
};
const tsk_object_def_t *tmsrp_message_def_t = &tmsrp_message_def_s;
