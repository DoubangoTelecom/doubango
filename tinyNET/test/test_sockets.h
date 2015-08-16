/* Copyright (C) 2014 Mamadou DIOP.
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
	struct sockaddr_storage to;
	
	TNET_SOCKET_TYPE_SET_IPV4(type);
	TNET_SOCKET_TYPE_SET_IPV6(type);
	TNET_SOCKET_TYPE_SET_IPV4Only(type);
	TNET_SOCKET_TYPE_SET_IPV6Only(type);
	TNET_SOCKET_TYPE_SET_IPV6Only(type);
	TNET_SOCKET_TYPE_SET_IPV4(type);
	TNET_SOCKET_TYPE_SET_IPV6(type);

	TNET_SOCKET_TYPE_SET_TLS(type);
	TNET_SOCKET_TYPE_SET_UDP(type);
	TNET_SOCKET_TYPE_SET_SCTP(type);
	TNET_SOCKET_TYPE_SET_TCP(type);
	
	tcp_socket = tnet_socket_create(TNET_SOCKET_HOST_ANY, TNET_SOCKET_PORT_ANY, type);

	if(!TNET_SOCKET_IS_VALID(tcp_socket))
	{
		TSK_OBJECT_SAFE_FREE(tcp_socket);
		return;
	}

	//if(!(test = tnet_sockaddr_init("www.google.com", 80, type, &to))){
	//	test = tnet_sockfd_connetto(tcp_socket->fd, (const struct sockaddr_storage *)&to);
	//}

	if(!(test = tnet_sockaddr_init("ipv6.google.com", 80, type, &to))){
		test = tnet_sockfd_connectto(tcp_socket->fd, (const struct sockaddr_storage *)&to);
	}

	TSK_OBJECT_SAFE_FREE(tcp_socket);
}

#endif /* TNET_TEST_SOCKETS_H */
