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

/**@file tmsrp_media.h
 * @brief MSRP Media definition.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYMSRP_MEDIA_H
#define TINYMSRP_MEDIA_H

#include "tinyMSRP_config.h"

#include "tinyMSRP/session/tmsrp_sender.h"
#include "tinyMSRP/session/tmsrp_receiver.h"

#include "tinyMEDIA/tmedia.h"
#include "tsdp.h"

TMSRP_BEGIN_DECLS

#define TMSRP_MEDIA(self)	((tmsrp_media_t*)(self))

typedef enum tmsrp_session_setup_e
{
	setup_active,
	setup_passive,
	setup_actpass,
	setup_holdconn
}
tmsrp_session_setup_t;

typedef struct tmsrp_media_s
{
	TMED_DECLARE_MEDIA;
	
	tmsrp_config_t* config;
	tmsrp_session_setup_t setup;
	tnet_fd_t connectedFD; // FullDuplex Socket
	tmsrp_sender_t* sender;
	
	struct{
		struct tsdp_header_M_s* M;
		tnet_socket_t* socket;
	}local;

	struct{
		struct tsdp_header_M_s* M;
		//struct tsdp_header_C_s* C;
	}remote;

	struct{
		struct tsdp_header_M_s* M;
	}negociated;
}
tmsrp_media_t;

int tmsrp_send_file(tmsrp_media_t* self, const char* path);
int tmsrp_send_data(tmsrp_media_t* self, const void* data, size_t size, const char* ctype);

TINYMSRP_GEXTERN const tsk_object_def_t *tmsrp_media_def_t;
TINYMSRP_GEXTERN const tmedia_plugin_def_t *tmsrp_media_plugin_def_t;

TMSRP_END_DECLS

#endif /* TINYMSRP_MEDIA_H */
