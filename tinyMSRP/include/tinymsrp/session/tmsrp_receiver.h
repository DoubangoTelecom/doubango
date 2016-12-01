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

/**@file tmsrp_receiver.h
 * @brief MSRP receiver.
 *
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 *

 */
#ifndef TINYMSRP_RECEIVER_H
#define TINYMSRP_RECEIVER_H

#include "tinymsrp_config.h"

#include "tinymsrp/session/tmsrp_data.h"
#include "tinymsrp/session/tmsrp_config.h"

#include "tinymsrp/tmsrp_event.h"

#include "tnet_types.h"
#include "tnet_transport.h"

TMSRP_BEGIN_DECLS

typedef struct tmsrp_receiver_s {
    TSK_DECLARE_OBJECT;

    tmsrp_data_in_t* data_in;
    tmsrp_config_t* config;
    tnet_fd_t fd;
    tsk_buffer_t* buffer;

    struct {
        tmsrp_event_cb_f func;
        const void* data;
    } callback;
}
tmsrp_receiver_t;

TINYMSRP_API tmsrp_receiver_t* tmsrp_receiver_create(tmsrp_config_t* config, tnet_fd_t fd);
TINYMSRP_API int tmsrp_receiver_set_fd(tmsrp_receiver_t* self, tnet_fd_t fd);
TINYMSRP_API int tmsrp_receiver_recv(tmsrp_receiver_t* self, const void* data, tsk_size_t size);
TINYMSRP_API int tmsrp_receiver_start(tmsrp_receiver_t* self, const void* callback_data, tmsrp_event_cb_f func);
TINYMSRP_API int tmsrp_receiver_stop(tmsrp_receiver_t* self);

TINYMSRP_GEXTERN const tsk_object_def_t *tmsrp_receiver_def_t;

TMSRP_END_DECLS

#endif /* TINYMSRP_RECEIVER_H */
