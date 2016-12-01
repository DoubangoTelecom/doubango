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

/**@file tmsrp_data.c
 * @brief MSRP input/output data.
 *
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 *

 */
#include "tinymsrp/session/tmsrp_data.h"

#include "tinymsrp/session/tmsrp_config.h"

#include "tinymsrp/parsers/tmsrp_parser_message.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#include <stdio.h> /* fopen, fclose ... */

#define TMSRP_DATA_IN_MAX_BUFFER 0xFFFFFF

tmsrp_data_in_t* tmsrp_data_in_create()
{
    return tsk_object_new(tmsrp_data_in_def_t);
}

tmsrp_data_out_t* _tmsrp_data_out_create(const void* pdata, tsk_size_t size, tsk_bool_t isfilepath)
{
    return tsk_object_new(tmsrp_data_out_def_t, pdata, size, isfilepath);
}

tmsrp_data_out_t* tmsrp_data_out_create(const void* pdata, tsk_size_t size)
{
    return _tmsrp_data_out_create(pdata, size, tsk_false);
}

tmsrp_data_out_t* tmsrp_data_out_file_create(const char* filepath)
{
    return _tmsrp_data_out_create(filepath, tsk_strlen(filepath), tsk_true);
}


/* =========================== Common ============================= */
static int tmsrp_data_deinit(tmsrp_data_t* self)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    TSK_FREE(self->id);
    TSK_FREE(self->ctype);
    TSK_FREE(self->wctype);

    return 0;
}







/* =========================== Incoming ============================= */

int tmsrp_data_in_put(tmsrp_data_in_t* self, const void* pdata, tsk_size_t size)
{
    int ret = -1;

    if(!self || !self->buffer || !pdata || !size) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return ret;
    }

    if((ret = tsk_buffer_append(self->buffer, pdata, size))) {
        TSK_DEBUG_ERROR("Failed to append data");
        tsk_buffer_cleanup(self->buffer);
        return ret;
    }
    else {
        if(TSK_BUFFER_SIZE(self->buffer) > TMSRP_DATA_IN_MAX_BUFFER) {
            tsk_buffer_cleanup(self->buffer);
            TSK_DEBUG_ERROR("Too many bytes are waiting.");
            return -3;
        }
    }

    return ret;
}

tmsrp_message_t* tmsrp_data_in_get(tmsrp_data_in_t* self)
{
    tmsrp_message_t* ret;
    tsk_size_t msg_size;

    if(!self || !self->buffer || !TSK_BUFFER_DATA(self->buffer) || !TSK_BUFFER_SIZE(self->buffer)) {
        //...this is not an error
        return tsk_null;
    }

    if((ret = tmsrp_message_parse_2(self->buffer->data, self->buffer->size, &msg_size))) {
        tsk_buffer_remove(self->buffer, 0, msg_size);
        return ret;
    }

    return tsk_null;
}


/* =========================== Outgoing ============================= */

tsk_buffer_t* tmsrp_data_out_get(tmsrp_data_out_t* self)
{
    tsk_buffer_t* ret = tsk_null;
    tsk_size_t toread;

    if(!self) {
        return tsk_null;
    }

    if(!(toread = self->size > TMSRP_MAX_CHUNK_SIZE ? TMSRP_MAX_CHUNK_SIZE : self->size)) {
        return tsk_null;
    }

    if(self->message) {
        ret = tsk_buffer_create(TSK_BUFFER_DATA(self->message), toread);
        tsk_buffer_remove(self->message, 0, toread);
        self->size = self->message->size;
    }
    else if(self->file) {
        // Buffer hack
        tsk_size_t read;
        ret = tsk_buffer_create_null();
        ret->data = tsk_calloc(toread, sizeof(uint8_t));
        ret->size = toread;
        if((read = (tsk_size_t)fread(ret->data, sizeof(uint8_t), toread, self->file)) == toread) {
            self->size -= toread;
        }
        else {
            TSK_OBJECT_SAFE_FREE(ret);
        }
    }


    return ret;
}








//=================================================================================================
//	MSRP incoming data object definition
//
static void* tmsrp_data_in_ctor(tsk_object_t * self, va_list * app)
{
    tmsrp_data_in_t *data_in = self;
    if(data_in) {
        data_in->buffer = tsk_buffer_create_null();
    }
    return self;
}

static void* tmsrp_data_in_dtor(tsk_object_t * self)
{
    tmsrp_data_in_t *data_in = self;
    if(data_in) {
        tmsrp_data_deinit(TMSRP_DATA(data_in));
        TSK_OBJECT_SAFE_FREE(data_in->buffer);
    }

    return self;
}

static const tsk_object_def_t tmsrp_data_in_def_s = {
    sizeof(tmsrp_data_in_t),
    tmsrp_data_in_ctor,
    tmsrp_data_in_dtor,
    tsk_null,
};
const tsk_object_def_t *tmsrp_data_in_def_t = &tmsrp_data_in_def_s;

//=================================================================================================
//	MSRP outgoing data object definition
//
static void* tmsrp_data_out_ctor(tsk_object_t * self, va_list * app)
{
    tmsrp_data_out_t *data_out = self;
    if(data_out) {
        tsk_istr_t id;
        const void* pdata = va_arg(*app, const void*);
        tsk_size_t size = va_arg(*app, tsk_size_t);
        tsk_bool_t isfilepath = va_arg(*app, tsk_bool_t);

        if(isfilepath) {
            if((data_out->file = fopen((const char*)pdata, "rb"))) {
                int ret;
                if((ret = fseek(data_out->file, 0L, SEEK_END))) {
                    TSK_DEBUG_ERROR("fseek for file:[%s] failed with error code %d.", (const char*)pdata, ret);
                    TMSRP_DATA(data_out)->isOK = tsk_false;
                }
                else {
                    data_out->size = ftell(data_out->file);
                    if((ret = fseek(data_out->file, 0L, SEEK_SET))) {
                        TSK_DEBUG_ERROR("fseek for file:[%s] failed with error code %d.", (const char*)pdata, ret);
                        TMSRP_DATA(data_out)->isOK = tsk_false;
                    }
                    else {
                        TMSRP_DATA(data_out)->isOK = tsk_true;
                    }
                }
            }
            else {
                TSK_DEBUG_ERROR("Failed to open(rb) this file:[%s]", (const char*)pdata);
                TMSRP_DATA(data_out)->isOK = tsk_false;
            }
        }
        else {
            if((data_out->message = tsk_buffer_create(pdata, size))) {
                TMSRP_DATA(data_out)->isOK = (data_out->message->size == size);
                data_out->size = data_out->message->size;
            }
        }

        // content type
        TMSRP_DATA(data_out)->ctype = tsk_strdup("application/octet-stream");
        TMSRP_DATA(data_out)->wctype = tsk_strdup("text/plain");
        // random id
        tsk_strrandom(&id);
        TMSRP_DATA(data_out)->id = tsk_strdup(id);
    }
    return self;
}

static void* tmsrp_data_out_dtor(tsk_object_t * self)
{
    tmsrp_data_out_t *data_out = self;
    if(data_out) {
        tmsrp_data_deinit(TMSRP_DATA(data_out));
        TSK_OBJECT_SAFE_FREE(data_out->message);

        if(data_out->file) {
            fclose(data_out->file);
            data_out->file = tsk_null;
        }
    }

    return self;
}

static const tsk_object_def_t tmsrp_data_out_def_s = {
    sizeof(tmsrp_data_out_t),
    tmsrp_data_out_ctor,
    tmsrp_data_out_dtor,
    tsk_null,
};
const tsk_object_def_t *tmsrp_data_out_def_t = &tmsrp_data_out_def_s;
