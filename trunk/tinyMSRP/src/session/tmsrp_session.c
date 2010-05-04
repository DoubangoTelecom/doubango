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

/**@file tmsrp_session.c
 * @brief MSRP Session to send/receive chuncks.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinymsrp/session/tmsrp_session.h"

#include "tnet_utils.h"

int tmsrp_session_start(tmsrp_session_t* self)
{
	if(!self || !self->localSocket){
		return -1;
	}

	return 0;
}

int tmsrp_session_stop(tmsrp_session_t* self)
{
	if(!self || !self->localSocket){
		return -1;
	}

	return 0;
}

//int tmsrp_send_file(tmsrp_session_t* self, const char* path)
//{
//	int ret = -1;
//
//	if(!self){
//		return ret;
//	}
//	self->isFile = tsk_true;
//
//	return ret;
//}
//
//int tmsrp_send_text(tmsrp_session_t* self, const char* text, const char* ctype)
//{
//	int ret = -1;
//
//	if(!self){
//		return ret;
//	}
//
//	return ret;
//}






//=================================================================================================
//	MSRP session object definition
//
static void* tmsrp_session_create(tsk_object_t* self, va_list * app)
{
	tmsrp_session_t *session = self;
	if(session){
		/*tmsrp_session_setup_t setup = va_arg(*app, tmsrp_session_setup_t);
		const char* host = va_arg(*app, const char*);
		tsk_bool_t useIPv6 = va_arg(*app, tsk_bool_t); 
		tsk_bool_t useTLS = va_arg(*app, tsk_bool_t);

		tnet_socket_type_t type = tnet_socket_type_tcp_ipv4;
		if(useIPv6){
			TNET_SOCKET_TYPE_SET_IPV6(type);
		}
		if(useTLS){
			TNET_SOCKET_TYPE_SET_TLS(type);
		}
		
		session->setup = setup;

		if(session->localSocket = TNET_SOCKET_CREATE(host, TNET_SOCKET_PORT_ANY, type)){
			if(session->setup != setup_active && session->setup != setup_actpass){
				tnet_sockfd_listen(session->localSocket->fd, 5);
			}
		}*/
	}
	return self;
}

static void* tmsrp_session_destroy(tsk_object_t * self)
{ 
	tmsrp_session_t *session = self;
	if(session){
		tnet_sockfd_close(&session->connectedFD);
		TSK_OBJECT_SAFE_FREE(session->localSocket);
	}
	return self;
}

static const tsk_object_def_t tmsrp_session_def_s = 
{
	sizeof(tmsrp_session_t),
	tmsrp_session_create, 
	tmsrp_session_destroy,
	tsk_null, 
};
const tsk_object_def_t *tmsrp_session_def_t = &tmsrp_session_def_s;
