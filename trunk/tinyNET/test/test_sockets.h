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
#ifndef TNET_TEST_SOCKETS_H
#define TNET_TEST_SOCKETS_H

void test_sockets()
{
	int test;
	tnet_socket_tcp_t * tcp_socket;
	tnet_socket_type_t type = tnet_socket_type_udp_ipv4;
	
	TNET_SOCKET_TYPE_AS_IPV6(type);
	TNET_SOCKET_TYPE_AS_IPV4(type);

	TNET_SOCKET_TYPE_AS_TLS(type);
	TNET_SOCKET_TYPE_AS_UDP(type);
	TNET_SOCKET_TYPE_AS_SCTP(type);
	TNET_SOCKET_TYPE_AS_TCP(type);
	
	tcp_socket = TNET_SOCKET_CREATE(TNET_SOCKET_HOST_ANY, TNET_SOCKET_PORT_ANY, type);
	

	if(!TNET_SOCKET_IS_VALID(tcp_socket))
	{
		TSK_OBJECT_SAFE_FREE(tcp_socket);
		return;
	}

	test = tnet_socket_stream_connectto(tcp_socket, "www.google.com", 80);

	TSK_OBJECT_SAFE_FREE(tcp_socket);
}

#endif /* TNET_TEST_SOCKETS_H */