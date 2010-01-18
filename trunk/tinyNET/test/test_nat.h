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
#ifndef TNET_TEST_NAT_H
#define TNET_TEST_NAT_H

//stun.ipv6.club.tw
#define STUN_SERVER_IP			"numb.viagenie.ca"
#define STUN_USERNAME			"bossiel@yahoo.fr"
#define STUN_PASSWORD			"xxyYYYOPUUEH"
#define STUN_SERVER_PORT		TNET_STUN_TCP_UDP_DEFAULT_PORT
#define STUN_SERVER_PROTO		tnet_socket_type_udp_ipv4

void test_nat_stun()
{
	tnet_socket_t *localSocket = 0;
	tnet_nat_context_handle_t *context = 0;

	char* public_ip = 0;
	tnet_port_t public_port = 0;
	
	if(!(localSocket = TNET_SOCKET_CREATE_EX(TNET_SOCKET_HOST_ANY, TNET_SOCKET_PORT_ANY, STUN_SERVER_PROTO, 0)))
	{
		goto bail;
	}

	context = TNET_NAT_CONTEXT_CREATE(localSocket->fd, localSocket->type, STUN_USERNAME, STUN_PASSWORD);

	if(tnet_nat_set_server_address(context, STUN_SERVER_IP))
	{
		TSK_DEBUG_ERROR("Failed to set STUN/TURN address.");
		goto bail;
	}

	if(tnet_nat_stun_bind(context, &public_ip, &public_port))
	{
		TSK_DEBUG_ERROR("Failed to get public IP/port using stun");
		goto bail;
	}

	TSK_DEBUG_INFO("Public IP/Port ==> %s:%u", public_ip, public_port);

bail:
	TNET_SOCKET_SAFE_FREE(localSocket);
	TNET_NAT_CONTEXT_SAFE_FREE(context);

	TSK_FREE(public_ip);
}

void test_nat_turn()
{
	tnet_socket_t *localSocket = 0;
	tnet_nat_context_handle_t *context = 0;

	char* public_ip = 0;
	tnet_port_t public_port = 0;
	
	if(!(localSocket = TNET_SOCKET_CREATE_EX(TNET_SOCKET_HOST_ANY, TNET_SOCKET_PORT_ANY, STUN_SERVER_PROTO, 0)))
	{
		goto bail;
	}

	context = TNET_NAT_CONTEXT_CREATE(localSocket->fd, localSocket->type, STUN_USERNAME, STUN_PASSWORD);

	if(tnet_nat_set_server_address(context, STUN_SERVER_IP))
	{
		TSK_DEBUG_ERROR("Failed to set STUN/TURN address.");
		goto bail;
	}

	// FIXME/ delete retval
	if(tnet_nat_turn_allocate(context))
	{
		TSK_DEBUG_ERROR("Failed to get public IP/port using stun");
		goto bail;
	}

	TSK_DEBUG_INFO("Public IP/Port ==> %s:%u", public_ip, public_port);

bail:
	TNET_SOCKET_SAFE_FREE(localSocket);
	TNET_NAT_CONTEXT_SAFE_FREE(context);

	TSK_FREE(public_ip);
}


void test_nat()
{
	//test_nat_stun();
	test_nat_turn();
}



#endif /* TNET_TEST_NAT_H */