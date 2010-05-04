/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)yahoo.fr>
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
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYMSRP_RECEIVER_H
#define TINYMSRP_RECEIVER_H

#include "tinymsrp_config.h"

#include "tinymsrp/session/tmsrp_data.h"
#include "tinymsrp/session/tmsrp_config.h"

#include "tnet_types.h"
#include "tnet_transport.h"

TMSRP_BEGIN_DECLS

#define TMSRP_RECEIVER_CREATE(config, fd)	tsk_object_new(tmsrp_receiver_def_t, (tmsrp_config_t*)config, (tnet_fd_t) fd)

int tmsrp_transport_layer_stream_cb(const tnet_transport_event_t* e);

typedef struct tmsrp_receiver_s
{
	TSK_DECLARE_OBJECT;

	tmsrp_data_in_t* data_in;
	tmsrp_config_t* config;
	tnet_fd_t fd;
}
tmsrp_receiver_t;

int tmsrp_receiver_start(tmsrp_receiver_t* self);
int tmsrp_receiver_stop(tmsrp_receiver_t* self);

const tsk_object_def_t *tmsrp_receiver_def_t;

TMSRP_END_DECLS

#endif /* TINYMSRP_RECEIVER_H */
