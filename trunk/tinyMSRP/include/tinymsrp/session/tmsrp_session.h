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

/**@file tmsrp_session.h
 * @brief MSRP Session to send/receive chuncks.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYMSRP_SESSION_H
#define TINYMSRP_SESSION_H

#include "tinymsrp_config.h"

#include "tnet_socket.h"

#include "tsk_object.h"

TMSRP_BEGIN_DECLS

#define TMSRP_SESSION_VA_ARGS(setup, host, useIPv6, useTLS)	tmsrp_session_def_t, (tmsrp_session_setup_t)setup, (const char*)host, (tsk_bool_t)useIPv6, (tsk_bool_t)useTLS
#define TMSRP_SESSION_CREATE(setup, host, useIPv6, useTLS)	tsk_object_new(TMSRP_SESSION_VA_ARGS(setup, host, useIPv6, useTLS))
#define TMSRP_SESSION_ACTIVE_CREATE(host, useIPv6, useTLS)	TMSRP_SESSION_CREATE(setup_active, host, useIPv6, useTLS)
#define TMSRP_SESSION_PASSIVE_CREATE(host, useIPv6, useTLS)	TMSRP_SESSION_CREATE(setup_passive, host, useIPv6, useTLS)
#define TMSRP_SESSION_ACTPASS_CREATE(host, useIPv6, useTLS)	TMSRP_SESSION_CREATE(setup_actpass, host, useIPv6, useTLS)
#define TMSRP_SESSION_HOLDCONN_CREATE(host, useIPv6, useTLS)	TMSRP_SESSION_CREATE(setup_holdconn, host, useIPv6, useTLS)
#define TMSRP_SESSION_CREATE_NULL()		TMSRP_SESSION_CREATE(setup_actpass, TNET_SOCKET_HOST_ANY, tsk_false, tsk_false)

#define TMSRP_SESSION(self)	((tmsrp_session_t*)(self))

typedef struct tmsrp_session_s
{
	TSK_DECLARE_OBJECT;

	tsk_bool_t isFile;
	//tmsrp_session_setup_t setup;

	tnet_fd_t connectedFD; // FullDuplex Socket
	tnet_socket_t* localSocket;
}
tmsrp_session_t;

int tmsrp_session_start(tmsrp_session_t* self);
int tmsrp_session_stop(tmsrp_session_t* self);

//int tmsrp_send_file(tmsrp_session_t* self, const char* path);
//int tmsrp_send_text(tmsrp_session_t* self, const char* text, const char* ctype);

const tsk_object_def_t *tmsrp_session_def_t;

TMSRP_END_DECLS

#endif /* TINYMSRP_SESSION_H */
