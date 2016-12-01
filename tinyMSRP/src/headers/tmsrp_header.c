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

/**@file tmsrp_header.c
 * @brief Defines a MSRP header (hname  ":" SP hval CRLF).
 *
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 *

 */
#include "tinymsrp/headers/tmsrp_header.h"

#include "tinymsrp/headers/tmsrp_header_Dummy.h"

#include "tsk_string.h"

/** Gets the name of the MSRP header with a type equal to @a type.
 * @param	type	The @a type of the header for which to retrieve the name.
 *
 * @return	The name of the header.
**/
const char* tmsrp_header_get_name(tmsrp_header_type_t type)
{
    switch(type) {
    case tmsrp_htype_Authentication_Info:
        return "Authentication-Info";
    case tmsrp_htype_Authorization:
        return "Authorization";
    case tmsrp_htype_Byte_Range:
        return "Byte-Range";
    case tmsrp_htype_Content_Type:
        return "Content-Type";
    case tmsrp_htype_Expires:
        return "Expires";
    case tmsrp_htype_Failure_Report:
        return "Failure-Report";
    case tmsrp_htype_From_Path:
        return "From-Path";
    case tmsrp_htype_Max_Expires:
        return "Max-Expires";
    case tmsrp_htype_Message_ID:
        return "Message-ID";
    case tmsrp_htype_Min_Expires:
        return "Min-Expires";
    case tmsrp_htype_Status:
        return "Status";
    case tmsrp_htype_Success_Report:
        return "Success-Report";
    case tmsrp_htype_To_Path:
        return "To-Path";
    case tmsrp_htype_Use_Path:
        return "Use-Path";
    case tmsrp_htype_WWW_Authenticate:
        return "WWW-Authenticate";

    default:
        return "unknown-header";
    }
}

const char* tmsrp_header_get_nameex(const tmsrp_header_t *self)
{
    if(self) {
        if(self->type == tmsrp_htype_Dummy) {
            return ((tmsrp_header_Dummy_t*)(self))->name;
        }
        else {
            return tmsrp_header_get_name(self->type);
        }
    }
    return "unknown-header";
}


int tmsrp_header_serialize(const tmsrp_header_t *self, tsk_buffer_t *output)
{
    int ret = -1;
    if(!self || !output) {
        return -1;
    }

    /* Name */
    tsk_buffer_append_2(output, "%s: ", tmsrp_header_get_nameex(self));

    /* Value */
    if((ret = self->tostring(self, output))) {
        // Abort?
    }

    /* CRLF*/
    ret = tsk_buffer_append(output, "\r\n", 2);

    return ret;
}

char* tmsrp_header_tostring(const tmsrp_header_t *self)
{
    char* ret = tsk_null;
    tsk_buffer_t* buffer;
    if(self && self->tostring) {
        if((buffer = tsk_buffer_create_null())) {
            self->tostring(self, buffer);
            ret = tsk_strndup(buffer->data, buffer->size);
            TSK_OBJECT_SAFE_FREE(buffer);
        }
    }
    return ret;
}