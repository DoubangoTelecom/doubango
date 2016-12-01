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

/**@file tmsrp_sender.c
 * @brief MSRP sender.
 *
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 *

 */
#include "tinymsrp/session/tmsrp_sender.h"

#include "tnet_utils.h"

#include "tsk_thread.h"
#include "tsk_memory.h"
#include "tsk_string.h"
#include "tsk_time.h"
#include "tsk_debug.h"


static void* TSK_STDCALL run(void* self);


tmsrp_sender_t* tmsrp_sender_create(tmsrp_config_t* config, tnet_fd_t fd)
{
    return (tmsrp_sender_t*)tsk_object_new(tmsrp_sender_def_t, config, fd);
}

int tmsrp_sender_set_fd(tmsrp_sender_t* self, tnet_fd_t fd)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    self->fd = fd;
    return 0;
}

int tmsrp_sender_start(tmsrp_sender_t* self)
{
    int ret = -1;

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        goto bail;
    }

    TSK_RUNNABLE(self)->run = run;
    if((ret = tsk_runnable_start(TSK_RUNNABLE(self), tmsrp_data_out_def_t))) {
        goto bail;
    }

bail:
    return ret;
}

int tsmrp_sender_send_data(tmsrp_sender_t* self, const void* pdata, tsk_size_t size, const char* ctype, const char* wctype)
{
    tmsrp_data_out_t* data_out;

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if((data_out = tmsrp_data_out_create(pdata, size))) {
        if(ctype) {
            tsk_strupdate(&TMSRP_DATA(data_out)->ctype, ctype);
        }
        if(wctype) {
            tsk_strupdate(&TMSRP_DATA(data_out)->wctype, wctype);
        }
        TSK_RUNNABLE_ENQUEUE_OBJECT(self, data_out);
        return 0;
    }
    return -2;
}

int tsmrp_sender_send_file(tmsrp_sender_t* self, const char* filepath)
{
    tmsrp_data_out_t* data_out;

    if(!self || !filepath) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if((data_out = tmsrp_data_out_file_create(filepath))) {
        if(TMSRP_DATA(data_out)->isOK) {
            TSK_RUNNABLE_ENQUEUE_OBJECT(self, data_out);
            return 0;
        }
        else {
            TSK_OBJECT_SAFE_FREE(data_out);
            return -3;
        }
    }
    return -2;
}

int tmsrp_sender_stop(tmsrp_sender_t* self)
{
    int ret = -1;

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        goto bail;
    }

    if((ret = tsk_runnable_stop(TSK_RUNNABLE(self)))) {
        goto bail;
    }

bail:
    return ret;
}




static void* TSK_STDCALL run(void* self)
{
    tsk_list_item_t *curr;
    tmsrp_sender_t *sender = (tmsrp_sender_t*)self;
    tmsrp_data_out_t *data_out;
    tsk_buffer_t* chunck, *message = tsk_buffer_create_null();
    tsk_size_t start;
    tsk_size_t end;
    tsk_size_t total;
    tsk_istr_t tid;
    int64_t __now = (int64_t)tsk_time_now();
    tsk_bool_t error = tsk_false;

    TSK_DEBUG_INFO("MSRP SENDER::run -- START");

    TSK_RUNNABLE_RUN_BEGIN(sender);

    if((curr = TSK_RUNNABLE_POP_FIRST(sender))) {
        if(!(data_out = (tmsrp_data_out_t*)curr->data)) {
            continue;
        }

        error = tsk_false;
        start = 1;
        total = data_out->size;

        while(TSK_RUNNABLE(self)->running && !error && (chunck = tmsrp_data_out_get(data_out))) {
            tmsrp_request_t* SEND;
            // set end
            end = (start + chunck->size) - 1;
            // compute new transaction id
            tsk_itoa(++__now, &tid);
            // create SEND request
            SEND = tmsrp_request_create(tid, "SEND");
            // T-Path and From-Path (because of otherURIs)
            SEND->To = tsk_object_ref(sender->config->To_Path);
            SEND->From = tsk_object_ref(sender->config->From_Path);
            // add other headers
            tmsrp_message_add_headers(SEND,
                                      TMSRP_HEADER_MESSAGE_ID_VA_ARGS(TMSRP_DATA(data_out)->id),
                                      // TMSRP_HEADER_BYTE_RANGE_VA_ARGS(start, end, total), => See below
                                      TMSRP_HEADER_FAILURE_REPORT_VA_ARGS(sender->config->Failure_Report ? freport_yes : freport_no),
                                      TMSRP_HEADER_SUCCESS_REPORT_VA_ARGS(sender->config->Success_Report),

                                      tsk_null);
            // add data
            if(start == 1 && chunck->size && tsk_striequals(TMSRP_DATA(data_out)->ctype, "message/CPIM")) {
                tsk_buffer_t* content_cpim = tsk_buffer_create_null();
                if(content_cpim) {
                    tsk_buffer_append_2(content_cpim, "Subject: %s\r\n\r\nContent-Type: %s\r\n\r\n",
                                        "test", TMSRP_DATA(data_out)->wctype);
                    end += content_cpim->size;
                    total += content_cpim->size;
                    tsk_buffer_append(content_cpim, chunck->data, chunck->size);
                    tmsrp_message_add_content(SEND, TMSRP_DATA(data_out)->ctype, content_cpim->data, content_cpim->size);
                    TSK_OBJECT_SAFE_FREE(content_cpim);
                }
                else {
                    TSK_DEBUG_ERROR("Failed to allocate new buffer");
                }
            }
            else {
                tmsrp_message_add_content(SEND, TMSRP_DATA(data_out)->ctype, chunck->data, chunck->size);
            }
            // add byte range here not before: think about message/cpim
            tmsrp_message_add_headers(SEND,
                                      TMSRP_HEADER_BYTE_RANGE_VA_ARGS(start, end, total),

                                      tsk_null);


            // set continuation flag
            SEND->end_line.cflag = (end == total) ? '$' : '+';
            // serialize and send
            if(!(tmsrp_message_serialize(SEND, message))) {
                if(tnet_sockfd_send(sender->fd, message->data, message->size, 0) == 0) {
                    error = tsk_true;
                    // abort
                }
            }
            tsk_buffer_cleanup(message);

            // set start
            start = (end + 1);
            // cleanup
            TSK_OBJECT_SAFE_FREE(chunck);
            TSK_OBJECT_SAFE_FREE(SEND);

            /* wait */
            if(sender->chunck_duration) {
                tsk_thread_sleep(sender->chunck_duration);
            }
        }


        tsk_object_unref(curr);
    }

    TSK_RUNNABLE_RUN_END(self);

    TSK_OBJECT_SAFE_FREE(message);

    TSK_DEBUG_INFO("MSRP SENDER::run -- STOP");

    return 0;
}



//=================================================================================================
//	MSRP sender object definition
//
static void* tmsrp_sender_ctor(tsk_object_t * self, va_list *app)
{
    tmsrp_sender_t *sender = (tmsrp_sender_t*)self;
    if(sender) {
        sender->config = (tmsrp_config_t*)tsk_object_ref(va_arg(*app, tmsrp_config_t*));
        sender->fd = va_arg(*app, tnet_fd_t);

        sender->outgoingList = tsk_list_create();
    }
    return self;
}

static void* tmsrp_sender_dtor(tsk_object_t * self)
{
    tmsrp_sender_t *sender = self;
    if(sender) {
        /* Stop */
        tmsrp_sender_stop(sender);

        TSK_OBJECT_SAFE_FREE(sender->config);
        TSK_OBJECT_SAFE_FREE(sender->outgoingList);
        // the FD is owned by the transport ...do not close it
    }
    return self;
}

static const tsk_object_def_t tmsrp_sender_def_s = {
    sizeof(tmsrp_sender_t),
    tmsrp_sender_ctor,
    tmsrp_sender_dtor,
    tsk_null,
};
const tsk_object_def_t *tmsrp_sender_def_t = &tmsrp_sender_def_s;
