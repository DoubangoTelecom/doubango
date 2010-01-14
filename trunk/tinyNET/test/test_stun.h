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

#define STUN_SERVER_IP			"192.168.16.104"
#define STUN_SERVER_PORT		TNET_STUN_TCP_UDP_DEFAULT_PORT
#define STUN_SERVER_PROTO		tnet_socket_type_udp_ipv4

void test_stun_transacid()
{
	tnet_stun_transacid_t transcid;
	tnet_stun_transacid_string_t transcid_string;

	memset(transcid_string, '\0', sizeof(transcid_string));

	tnet_stun_random_transacid(transcid);
	tsk_str_from_hex(transcid, TNET_STUN_TRANSAC_ID_LENGTH, transcid_string);

	TSK_DEBUG_INFO("STUN transac id:%s", transcid_string);
}

void test_sun_sendMessage()
{
	tnet_stun_message_t message;
	tsk_buffer_t *buffer = 0;
	tnet_socket_t *socket = 0;
	struct sockaddr_storage to;

	message.type = tsm_binding_request;
	message.length = 0;
	message.cookie = TNET_STUN_MAGIC_COOKIE;
	tnet_stun_random_transacid(message.transaction_id);

	if(!(buffer = tnet_stun_message_serialize(&message)))
	{
		goto bail;
	}

	/* Create blocking socket and bind it */
	socket = TNET_SOCKET_CREATE_EX(TNET_SOCKET_HOST_ANY, TNET_SOCKET_PORT_ANY, STUN_SERVER_PROTO, 0);
	if(!TNET_SOCKET_IS_VALID(socket))
	{
		goto bail;
	}

	/* Create stun server's sockaddr structure */
	if(tnet_sockaddr_init(STUN_SERVER_IP, STUN_SERVER_PORT, STUN_SERVER_PROTO, &to))
	{
		goto bail;
	}

	tnet_socket_sendto(socket, (struct sockaddr*)&to, buffer->data, buffer->size);

bail:
	TNET_SOCKET_SAFE_FREE(socket);
	TSK_BUFFER_SAFE_FREE(buffer);
}

void test_stun()
{
	//test_stun_transacid();
	test_sun_sendMessage();
}

#endif /* TNET_TEST_STUN_H */