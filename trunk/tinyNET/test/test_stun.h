/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou@yahoo.fr>
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
* GNU Lesser General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/
#ifndef TNET_TEST_STUN_H
#define TNET_TEST_STUN_H

//#define STUN_SERVER_IP			"numb.viagenie.ca"
//#define STUN_SERVER_PORT		TNET_STUN_TCP_UDP_DEFAULT_PORT
//#define STUN_SERVER_PROTO		tnet_socket_type_udp_ipv4

// http://tools.ietf.org/html/draft-ietf-behave-stun-test-vectors-04

void test_stun_dump_transacid(tnet_stun_transacid_t transcid)
{
	char transac_idstriing[TNET_STUN_TRANSACID_SIZE*2+1];
	tsk_str_from_hex(transcid, TNET_STUN_TRANSACID_SIZE, transac_idstriing);

	transac_idstriing[sizeof(transac_idstriing)-1] = '\0'; 

	TSK_DEBUG_INFO("STUN transac id:%s", transac_idstriing);
}

void test_sun_sendMessage()
{
	/*tnet_stun_message_handle_t *message = 0;
	tsk_buffer_t *buffer = 0;
	tnet_socket_t *socket = 0;
	struct sockaddr_storage to;


	message = TNET_STUN_MESSAGE_CREATE();
	tnet_stun_message_set_type(message, tsm_binding_request);
	tnet_stun_message_set_random_transacid(message);

	if(!(buffer = tnet_stun_message_serialize(message)))
	{
		goto bail;
	}

	// Create blocking socket and bind it 
	socket = TNET_SOCKET_CREATE_EX(TNET_SOCKET_HOST_ANY, TNET_SOCKET_PORT_ANY, STUN_SERVER_PROTO, 0);
	if(!TNET_SOCKET_IS_VALID(socket))
	{
		goto bail;
	}

	// Create stun server's sockaddr structure 
	if(tnet_sockaddr_init(STUN_SERVER_IP, STUN_SERVER_PORT, STUN_SERVER_PROTO, &to))
	{
		goto bail;
	}

	tnet_socket_sendto(socket, (struct sockaddr*)&to, buffer->data, buffer->size);

bail:
	TNET_STUN_MESSAGE_SAFE_FREE(message);
	TNET_SOCKET_SAFE_FREE(socket);
	TSK_BUFFER_SAFE_FREE(buffer);*/
}

void test_stun_context()
{
//	tnet_socket_t *localSocket = 0;
//	tnet_stun_context_t *context = 0;
//
//	/* Somewhere in Your application ==> Create and bind your local socket
//	*/
//	if(!(localSocket = TNET_SOCKET_CREATE_EX(TNET_SOCKET_HOST_ANY, TNET_SOCKET_PORT_ANY, STUN_SERVER_PROTO, 0)))
//	{
//		goto bail;
//	}
//	
//	/*	Create your STUN2 context
//	*/
//	if(!(context = TNET_STUN_CONTEXT_CREATE("myusername", "mypassword", localSocket->fd)))
//	{
//		goto bail;
//	}
//	
//	tnet_stun_resolve(context, STUN_SERVER_IP, STUN_SERVER_PORT, STUN_SERVER_PROTO);
//
//bail:
//	TNET_SOCKET_SAFE_FREE(localSocket);
//	TNET_STUN_CONTEXT_SAFE_FREE(context);
}

void test_stun()
{
	//test_stun_transacid();
	//test_sun_sendMessage();
	test_stun_context();
}

#endif /* TNET_TEST_STUN_H */