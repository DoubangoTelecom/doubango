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

/**@file tmsrp_sender.h
 * @brief MSRP sender.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYMSRP_SENDER_H
#define TINYMSRP_SENDER_H

#include "tinyMSRP_config.h"

#include "tinyMSRP/session/tmsrp_data.h"
#include "tinyMSRP/session/tmsrp_config.h"

#include "tnet_types.h"

#include "tsk_runnable.h"

TMSRP_BEGIN_DECLS

#define TMSRP_SENDER_CREATE(config, fd)	tsk_object_new(tmsrp_sender_def_t, (tmsrp_config_t*)config, (tnet_fd_t) fd)

typedef struct tmsrp_sender_s
{
	TSK_DECLARE_RUNNABLE;

	tmsrp_datas_L_t* outputList;
	tmsrp_config_t* config;
	tnet_fd_t fd;
}
tmsrp_sender_t;

int tmsrp_sender_start(tmsrp_sender_t* self);
int tsmrp_sender_send_data(tmsrp_sender_t* self, const void* data, size_t size, const char* ctype);
int tsmrp_sender_send_file(tmsrp_sender_t* self, const char* filepath);
int tmsrp_sender_stop(tmsrp_sender_t* self);

const tsk_object_def_t *tmsrp_sender_def_t;

TMSRP_END_DECLS

#endif /* TINYMSRP_SENDER_H */
