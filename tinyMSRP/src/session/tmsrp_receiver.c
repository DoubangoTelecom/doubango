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

/**@file tmsrp_receiver.c
 * @brief MSRP receiver.
 *
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 *

 */
#include "tinymsrp/session/tmsrp_receiver.h"

#include "tmsrp.h"

#include "tnet_utils.h"

#include "tsk_memory.h"
#include "tsk_string.h"
#include "tsk_debug.h"

static void _tmsrp_receiver_alert_user(tmsrp_receiver_t* self, tsk_bool_t outgoing, tmsrp_message_t* message)
{
    if(self->callback.func) {
        tmsrp_event_t* _event = tmsrp_event_create(self->callback.data, outgoing, tmsrp_event_type_message, message);
        self->callback.func(_event);
        TSK_OBJECT_SAFE_FREE(_event);
    }
}

tmsrp_receiver_t* tmsrp_receiver_create(tmsrp_config_t* config, tnet_fd_t fd)
{
    return tsk_object_new(tmsrp_receiver_def_t, config, fd);
}

int tmsrp_receiver_set_fd(tmsrp_receiver_t* self, tnet_fd_t fd)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    self->fd = fd;
    return 0;
}

int tmsrp_receiver_start(tmsrp_receiver_t* self, const void* callback_data, tmsrp_event_cb_f func)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    self->callback.data = callback_data;
    self->callback.func = func;

    return 0;
}

int tmsrp_receiver_stop(tmsrp_receiver_t* self)
{
    return 0;
}

int tmsrp_receiver_recv(tmsrp_receiver_t* self, const void* data, tsk_size_t size)
{
    tmsrp_message_t* message;

    if(!self || !data || !size) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    // put the data
    tmsrp_data_in_put(self->data_in, data, size);
    // get msrp messages
    while((message = tmsrp_data_in_get(self->data_in))) {

        /* alert that we have received a message (Request or Response) */
        _tmsrp_receiver_alert_user(self, tsk_false, message);

        //
        //	REQUEST
        //
        if(TMSRP_MESSAGE_IS_REQUEST(message)) {
            /* ============= SEND =============== */
            if(TMSRP_REQUEST_IS_SEND(message)) {
                tmsrp_response_t* r2xx;
                tmsrp_request_t* REPORT;

                // send 200 OK
                if((r2xx = tmsrp_create_response(message, 200, "OK"))) {
                    if(tmsrp_message_serialize(r2xx, self->buffer) == 0 && self->buffer->data) {
                        tnet_sockfd_send(self->fd, self->buffer->data, self->buffer->size, 0);
                    }

                    tsk_buffer_cleanup(self->buffer);
                    TSK_OBJECT_SAFE_FREE(r2xx);
                }
                // send REPORT
                if(tmsrp_isReportRequired(message, tsk_false)) {
                    if((REPORT = tmsrp_create_report(message, 200, "OK"))) {
                        if(tmsrp_message_serialize(REPORT, self->buffer) == 0 && self->buffer->data) {
                            tnet_sockfd_send(self->fd, self->buffer->data, self->buffer->size, 0);
                        }
                        tsk_buffer_cleanup(self->buffer);
                        TSK_OBJECT_SAFE_FREE(REPORT);
                    }
                }
            }
            /* ============= REPORT =============== */
            if(TMSRP_REQUEST_IS_REPORT(message)) {
                tmsrp_response_t* r2xx;

                // send 200 OK
                if((r2xx = tmsrp_create_response(message, 200, "Report received"))) {
                    if(tmsrp_message_serialize(r2xx, self->buffer) == 0 && self->buffer->data) {
                        tnet_sockfd_send(self->fd, self->buffer->data, self->buffer->size, 0);
                    }

                    tsk_buffer_cleanup(self->buffer);
                    TSK_OBJECT_SAFE_FREE(r2xx);
                }
            }

            /* ============= AUTH =============== */
            /* ============= METHOD =============== */
        }
        //
        //	RESPONSE
        //
        else {
            //short code = TMSRP_RESPONSE_CODE(message);
            //TSK_DEBUG_INFO("code=%u, tid=%s, phrase=%s", code, message->tid, TMSRP_RESPONSE_PHRASE(message));
        }


        // alert user layer

        TSK_OBJECT_SAFE_FREE(message);
    }

    return 0;
}


//=================================================================================================
//	MSRP receiver object definition
//
static void* tmsrp_receiver_ctor(tsk_object_t * self, va_list *app)
{
    tmsrp_receiver_t *receiver = self;
    if(receiver) {
        receiver->config = tsk_object_ref(va_arg(*app, tmsrp_config_t*));
        receiver->fd = va_arg(*app, tnet_fd_t);

        receiver->data_in = tmsrp_data_in_create();
        receiver->buffer = tsk_buffer_create_null();
    }
    return self;
}

static void* tmsrp_receiver_dtor(tsk_object_t * self)
{
    tmsrp_receiver_t *receiver = self;
    if(receiver) {
        /* Stop */
        tmsrp_receiver_stop(receiver);

        TSK_OBJECT_SAFE_FREE(receiver->config);
        TSK_OBJECT_SAFE_FREE(receiver->data_in);
        TSK_OBJECT_SAFE_FREE(receiver->buffer);
        // the FD is owned by the transport ...do not close it
    }
    return self;
}

static const tsk_object_def_t tmsrp_receiver_def_s = {
    sizeof(tmsrp_receiver_t),
    tmsrp_receiver_ctor,
    tmsrp_receiver_dtor,
    tsk_null,
};
const tsk_object_def_t *tmsrp_receiver_def_t = &tmsrp_receiver_def_s;

