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

/**@file thttp_header.c
 * @brief Defines a HTTP header (field-name: field-value).
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinyhttp/headers/thttp_header.h"

#include "tinyhttp/headers/thttp_header_Dummy.h"

/**@defgroup thttp_header_group HTTP Headers
*/

const char *thttp_header_get_name(thttp_header_type_t type)
{
    switch(type) {
    case thttp_htype_Authorization:
        return "Authorization";
    case thttp_htype_Content_Length:
        return "Content-Length";
    case thttp_htype_Content_Type:
        return "Content-Type";
    case thttp_htype_ETag:
        return "ETag";
    case thttp_htype_Proxy_Authenticate:
        return "Proxy-Authenticate";
    case thttp_htype_Proxy_Authorization:
        return "Proxy-Authorization";
    case thttp_htype_Transfer_Encoding:
        return "Transfer-Encoding";
    case thttp_htype_WWW_Authenticate:
        return "WWW-Authenticate";
    case thttp_htype_Sec_WebSocket_Accept:
        return "Sec-WebSocket-Accept";
    case thttp_htype_Sec_WebSocket_Protocol:
        return "Sec-WebSocket-Protocol";
    case thttp_htype_Sec_WebSocket_Key:
        return "Sec-WebSocket-Key";
    case thttp_htype_Sec_WebSocket_Version:
        return "Sec-WebSocket-Version";

    default:
        return "unknown-header";
    }
}

const char *thttp_header_get_nameex(const thttp_header_t *self)
{
    if(self) {
        if(self->type == thttp_htype_Dummy) {
            return ((thttp_header_Dummy_t*)self)->name;
        }
        else {
            return thttp_header_get_name(self->type);
        }
    }
    return "unknown-header";
}

char thttp_header_get_param_separator(const thttp_header_t *self)
{
    if(self) {
        switch(self->type) {
        case thttp_htype_Authorization:
        case thttp_htype_Proxy_Authorization:
        case thttp_htype_Proxy_Authenticate:
        case thttp_htype_WWW_Authenticate:
            return ',';
        default:
            return ';';
        }
    }
    return 0;
}

/**@ingroup thttp_header_group
*/
int thttp_header_serialize(const thttp_header_t *self, tsk_buffer_t *output)
{
    int ret = -1;
    static const char* hname;
    static char separator;

    if(self && THTTP_HEADER(self)->tostring) {
        tsk_list_item_t *item;

        hname = thttp_header_get_nameex(self);
        ret = 0; // for empty lists

        /* Header name */
        tsk_buffer_append_2(output, "%s: ", hname);

        /*  Header value.*/
        if((ret = THTTP_HEADER(self)->tostring(self, output))) {
            // CHECK all headers return value!
            //return ret;
        }

        /* Parameters */
        tsk_list_foreach(item, self->params) {
            tsk_param_t* param = item->data;
            separator = thttp_header_get_param_separator(self);
            if((ret = tsk_buffer_append_2(output, param->value?"%c%s=%s":"%c%s", separator, param->name, param->value))) {
                return ret;
            }
        }

        /* CRLF */
        tsk_buffer_append(output, "\r\n", 2);
    }
    return ret;
}

/**@ingroup thttp_header_group
*/
char* thttp_header_tostring(const thttp_header_t *self)
{
    tsk_buffer_t *output = tsk_buffer_create_null();
    char* ret = tsk_null;

    if(!thttp_header_serialize(self, output)) {
        ret = tsk_strndup(output->data, output->size);
    }

    TSK_OBJECT_SAFE_FREE(output);
    return ret;
}

/**@ingroup thttp_header_group
*/
char* thttp_header_value_tostring(const thttp_header_t *self)
{
    tsk_buffer_t *output = tsk_buffer_create_null();
    char* ret = tsk_null;

    if(!self->tostring(self, output)) {
        ret = tsk_strndup(output->data, output->size);
    }

    TSK_OBJECT_SAFE_FREE(output);
    return ret;
}
