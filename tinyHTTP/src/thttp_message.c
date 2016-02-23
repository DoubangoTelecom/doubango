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

/**@file thttp_message.c
 * @brief Represents a HTTP message. A HTTP message is either a request from a client to a server, or a
 * response from a server to a client.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinyhttp/thttp_message.h"

#include "tsk_debug.h"

/**@defgroup thttp_message_group HTTP Message
*/

static int pred_find_string_by_value(const tsk_list_item_t *item, const void *stringVal)
{
    if(item && item->data) {
        tsk_string_t *string = item->data;
        return tsk_stricmp(string->value, stringVal);
    }
    return -1;
}

/*== Predicate function to find thttp_header_t object by type. */
static int pred_find_header_by_type(const tsk_list_item_t *item, const void *thttp_htype)
{
    if(item && item->data) {
        thttp_header_t *header = item->data;
        thttp_header_type_t htype = *((thttp_header_type_t*)thttp_htype);
        return (header->type - htype);
    }
    return -1;
}

/*== Predicate function to find thttp_header_t object by name. */
static int pred_find_header_by_name(const tsk_list_item_t *item, const void *name)
{
    if(item && item->data) {
        thttp_header_t *header = item->data;
        return tsk_stricmp(thttp_header_get_nameex(header), name);
    }
    return -1;
}


/**@ingroup thttp_message_group
* Creates new HTTP message. Could be either a request or a response.
* @retval @ref thttp_message_t object.
* @sa @ref thttp_request_create()<br>@ref thttp_response_create()
*/
thttp_message_t* thttp_message_create()
{
    return tsk_object_new(thttp_message_def_t, thttp_unknown);
}

/**@ingroup thttp_message_group
* Creates new HTTP request.
* @param method The method (const char*). e.g. GET, POST, HEAD ...
* @param url The url (@ref thttp_url_t).
* @retval @ref thttp_request_t object.
*
* @code
// example
thttp_url_t* url;
thttp_request_t* request;
if((url = thttp_url_parse("http://www.google.com", tsk_strlen("http://www.google.com")))){
	request = thttp_request_create("GET", url);
	// ...
	TSK_OBJECT_SAFE_FREE(url);
	TSK_OBJECT_SAFE_FREE(request);
}
* @endcode
*/
thttp_request_t* thttp_request_create(const char* method, const thttp_url_t* url)
{
    return tsk_object_new(thttp_message_def_t, thttp_request, method, url);
}



/**@ingroup thttp_message_group
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
if((response = thttp_response_create(request, 200, "OK"))){
	TSK_OBJECT_SAFE_FREE(response);
}
* @endcode
*/
thttp_response_t* thttp_response_create(const thttp_request_t* request, short status_code, const char* reason_phrase)
{
    return tsk_object_new(thttp_message_def_t, thttp_response, request, status_code, reason_phrase);
}


/**@ingroup thttp_message_group
*/
int	thttp_message_add_header(thttp_message_t *self, const thttp_header_t *hdr)
{
#define ADD_HEADER(type, field) \
		case thttp_htype_##type: \
			{ \
				if(!self->field) \
				{ \
					self->field = (thttp_header_##type##_t*)header; \
					return 0; \
				} \
				break; \
			}

    if(self && hdr) {
        thttp_header_t *header = tsk_object_ref((void*)hdr);

        switch(header->type) {
            ADD_HEADER(Content_Type, Content_Type);
            ADD_HEADER(Content_Length, Content_Length);

        default:
            break;
        }

        tsk_list_push_back_data(self->headers, (void**)&header);

        return 0;
    }
    return -1;
}

/**@ingroup thttp_message_group
*/
int thttp_message_add_headers(thttp_message_t *self, const thttp_headers_L_t *headers)
{
    tsk_list_item_t *item = 0;
    if(self) {
        tsk_list_foreach(item, headers) {
            thttp_message_add_header(self, item->data);
        }
        return 0;
    }
    return -1;
}

int thttp_message_add_headers_2(thttp_message_t *self, ...)
{
    const tsk_object_def_t* objdef;
    thttp_header_t *header;
    va_list ap;

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    va_start(ap, self);
    while((objdef = va_arg(ap, const tsk_object_def_t*))) {
        if((header = tsk_object_new_2(objdef, &ap))) {
            thttp_message_add_header(self, header);
            TSK_OBJECT_SAFE_FREE(header);
        }
    }
    va_end(ap);

    return 0;
}

/**@ingroup thttp_message_group
*/
int thttp_message_add_content(thttp_message_t *self, const char* content_type, const void* content, tsk_size_t size)
{
    if(self && content && size) {
        if(content_type) {
            TSK_OBJECT_SAFE_FREE(self->Content_Type);
        }
        TSK_OBJECT_SAFE_FREE(self->Content_Length);
        TSK_OBJECT_SAFE_FREE(self->Content);

        if(content_type) {
            THTTP_MESSAGE_ADD_HEADER(self, THTTP_HEADER_CONTENT_TYPE_VA_ARGS(content_type));
        }
        THTTP_MESSAGE_ADD_HEADER(self, THTTP_HEADER_CONTENT_LENGTH_VA_ARGS(size));
        self->Content = tsk_buffer_create(content, size);

        return 0;
    }
    return -1;
}

/**@ingroup thttp_message_group
*/
int thttp_message_append_content(thttp_message_t *self, const void* content, tsk_size_t size)
{
    if(self && content && size) {
        if(!self->Content) {
            self->Content = tsk_buffer_create(content, size);
        }
        else {
            tsk_buffer_append(self->Content, content, size);
        }

        if(!self->Content_Length) {
            THTTP_MESSAGE_ADD_HEADER(self, THTTP_HEADER_CONTENT_LENGTH_VA_ARGS(size));
        }
        else {
            self->Content_Length->length += (uint32_t)size;
        }
        return 0;
    }

    return -1;
}

/**@ingroup thttp_message_group
*/
const thttp_header_t *thttp_message_get_headerAt(const thttp_message_t *self, thttp_header_type_t type, tsk_size_t index)
{
    tsk_size_t pos = 0;
    tsk_list_item_t *item;
    const thttp_header_t* hdr = tsk_null;

    if(self) {
        switch(type) {
        case thttp_htype_Content_Type:
            if(index == 0) {
                hdr = (const thttp_header_t*)self->Content_Type;
                goto bail;
            }
            else {
                pos++;
            }
            break;
        case thttp_htype_Content_Length:
            if(index == 0) {
                hdr = (const thttp_header_t*)self->Content_Length;
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

/**@ingroup thttp_message_group
*/
const thttp_header_t *thttp_message_get_header(const thttp_message_t *self, thttp_header_type_t type)
{
    return thttp_message_get_headerAt(self, type, 0);
}

/**@ingroup thttp_message_group
*/
const thttp_header_t *thttp_message_get_headerByName(const thttp_message_t *self, const char* name)
{
    //tsk_size_t pos = 0;
    tsk_list_item_t *item;
    const thttp_header_t* hdr = tsk_null;

    if(self) {
        if(tsk_striequals(name, "Content-Type")) {
            hdr = (const thttp_header_t*)self->Content_Type;
            goto bail;
        }

        if(tsk_striequals(name, "Content-Length")) {
            hdr = (const thttp_header_t*)self->Content_Length;
            goto bail;
        }

        tsk_list_foreach(item, self->headers) {
            if(!pred_find_header_by_name(item, name)) {
                hdr = item->data;
                break;
            }
        }
    }

bail:
    return hdr;
}

/**@ingroup thttp_message_group
*/
int thttp_message_serialize(const thttp_message_t *self, tsk_buffer_t *output)
{
    if(!self || !output) {
        return -1;
    }

    if(THTTP_MESSAGE_IS_REQUEST(self)) {
        /*Method SP Request-URI SP HTTP-Version CRLF*/
        /* Method */
        tsk_buffer_append_2(output, "%s ", self->line.request.method);
        /* Request URI: hpath?search */
        if(self->line.request.url) {
            tsk_buffer_append_2(output, "/%s%s%s ",
                                self->line.request.url->hpath ? self->line.request.url->hpath : "",
                                self->line.request.url->search ? "?" : "",
                                self->line.request.url->search ? self->line.request.url->search : ""
                               );
        }
        /* HTTP VERSION */
        tsk_buffer_append_2(output, "%s\r\n", THTTP_MESSAGE_VERSION_DEFAULT);
        /* HOST */
        if(self->line.request.url && self->line.request.url->host && self->line.request.url->port) {
            tsk_buffer_append_2(output, "Host: %s:%u\r\n", self->line.request.url->host, self->line.request.url->port);
        }
    }
    else {
        /*HTTP-Version SP Status-Code SP Reason-Phrase CRLF*/
        tsk_buffer_append_2(output, "%s %hi %s\r\n", THTTP_MESSAGE_VERSION_DEFAULT, THTTP_RESPONSE_CODE(self), THTTP_RESPONSE_PHRASE(self));
    }

    /* Content-Type */
    if(self->Content_Type) {
        thttp_header_serialize(THTTP_HEADER(self->Content_Type), output);
    }
    /* Content-Length*/
    if(self->Content_Length) {
        thttp_header_serialize(THTTP_HEADER(self->Content_Length), output);
    }

    /* All other headers */
    {
        tsk_list_item_t *item;
        tsk_list_foreach(item, self->headers) {
            thttp_header_t *hdr = item->data;
            thttp_header_serialize(hdr, output);
        }
    }

    /* EMPTY LINE */
    tsk_buffer_append(output, "\r\n", 2);

    /* CONTENT */
    if(THTTP_MESSAGE_HAS_CONTENT(self)) {
        tsk_buffer_append(output, TSK_BUFFER_TO_STRING(self->Content), TSK_BUFFER_SIZE(self->Content));
    }

    return 0;
}

/**@ingroup thttp_message_group
*/
char* thttp_message_tostring(const thttp_message_t *self)
{
    char* ret = tsk_null;
    tsk_buffer_t* output = tsk_buffer_create_null();

    if(!thttp_message_serialize(self, output)) {
        ret = tsk_strndup(output->data, output->size);
    }

    TSK_OBJECT_SAFE_FREE(output);
    return ret;
}

/**@ingroup thttp_message_group
*/
thttp_request_t *thttp_request_new(const char* method, const thttp_url_t *request_url)
{
    thttp_request_t* request = 0;

    if((request = thttp_request_create(method, request_url))) {
        THTTP_MESSAGE_ADD_HEADER(request, THTTP_HEADER_CONTENT_LENGTH_VA_ARGS(0));
    }
    return request;
}

/**@ingroup thttp_message_group
*/
thttp_response_t *thttp_response_new(short status_code, const char* reason_phrase, const thttp_request_t *request)
{
    thttp_response_t *response = 0;

    if(request) {
        response = thttp_response_create(request, status_code, reason_phrase);
        THTTP_MESSAGE_ADD_HEADER(response, THTTP_HEADER_CONTENT_LENGTH_VA_ARGS(0));
        /*
        	Copy other headers
        */
    }

    return response;
}









//========================================================
//	HTTP message object definition
//

/**@ingroup thttp_message_group
*/
static tsk_object_t* thttp_message_ctor(tsk_object_t *self, va_list * app)
{
    thttp_message_t *message = self;
    if(message) {
        message->type = va_arg(*app, thttp_message_type_t);
        message->headers = tsk_list_create();

        switch(message->type) {
        case thttp_unknown: {
            break;
        }

        case thttp_request: {
            message->line.request.method = tsk_strdup(va_arg(*app, const char*));
            message->line.request.url = tsk_object_ref((void*)va_arg(*app, const thttp_url_t*));
            break;
        }

        case thttp_response: {
            /*const thttp_request_t* request =*/ va_arg(*app, const thttp_request_t*);
#if defined(__GNUC__)
            message->line.response.status_code = (short)va_arg(*app, int);
#else
            message->line.response.status_code = va_arg(*app, short);
#endif
            message->line.response.reason_phrase = tsk_strdup(va_arg(*app, const char*));
            break;
        }
        }
    }
    else {
        TSK_DEBUG_ERROR("Failed to create new http message.");
    }
    return self;
}

/**@ingroup thttp_message_group
*/
static tsk_object_t* thttp_message_dtor(tsk_object_t *self)
{
    thttp_message_t *message = self;
    if(message) {
        if(THTTP_MESSAGE_IS_REQUEST(message)) {
            TSK_FREE(message->line.request.method);
            TSK_OBJECT_SAFE_FREE(message->line.request.url);
        }
        else if(THTTP_MESSAGE_IS_RESPONSE(message)) {
            TSK_FREE(message->line.response.reason_phrase);
        }

        TSK_FREE(message->http_version);

        TSK_OBJECT_SAFE_FREE(message->Content_Length);
        TSK_OBJECT_SAFE_FREE(message->Content_Type);

        TSK_OBJECT_SAFE_FREE(message->Content);

        TSK_OBJECT_SAFE_FREE(message->headers);
    }
    else {
        TSK_DEBUG_ERROR("Null HTTP message.");
    }

    return self;
}

static const tsk_object_def_t thttp_message_def_s = {
    sizeof(thttp_message_t),
    thttp_message_ctor,
    thttp_message_dtor,
    tsk_null
};
const tsk_object_def_t *thttp_message_def_t = &thttp_message_def_s;
