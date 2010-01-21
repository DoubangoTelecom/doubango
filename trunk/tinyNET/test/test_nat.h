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

//stun.ekiga.net
//#define STUN_SERVER_IP			"numb.viagenie.ca"
#define STUN_SERVER_IP			"numb.viagenie.ca"
#define STUN_USERNAME			"bossiel@yahoo.fr"
#define STUN_PASSWORD			"diopmama"
#define STUN_SERVER_PORT		TNET_STUN_TCP_UDP_DEFAULT_PORT
#define STUN_SERVER_PROTO		tnet_socket_type_udp_ipv4

void test_nat_stun()
{
	tnet_socket_t *socket1 = 0, *socket2 = 0;
	tnet_nat_context_handle_t *context = 0;

	char* public_ip1 = 0, *public_ip2 = 0;
	tnet_port_t public_port1 = 0, public_port2 = 0;
	
	if(!(socket1 = TNET_SOCKET_CREATE(TNET_SOCKET_HOST_ANY, TNET_SOCKET_PORT_ANY, STUN_SERVER_PROTO))
		|| !(socket2 = TNET_SOCKET_CREATE(TNET_SOCKET_HOST_ANY, TNET_SOCKET_PORT_ANY, STUN_SERVER_PROTO)))
	{
		goto bail;
	}

	context = TNET_NAT_CONTEXT_CREATE(STUN_SERVER_PROTO, STUN_USERNAME, STUN_PASSWORD);

	if(tnet_nat_set_server_address(context, STUN_SERVER_IP))
	{
		TSK_DEBUG_ERROR("Failed to set STUN/TURN address.");
		goto bail;
	}

	if(tnet_nat_stun_bind(context, socket1->fd, &public_ip1, &public_port1)
		|| tnet_nat_stun_bind(context, socket2->fd, &public_ip2, &public_port2))
	{
		TSK_DEBUG_ERROR("Failed to get public IP/port using stun");
		goto bail;
	}


	TSK_DEBUG_INFO("Public IP1/Port1 ==> %s:%u", public_ip1, public_port1);
	TSK_DEBUG_INFO("Public IP2/Port2 ==> %s:%u", public_ip2, public_port2);

bail:
	TSK_OBJECT_SAFE_FREE(socket1);
	TSK_OBJECT_SAFE_FREE(socket2);

	TSK_FREE(public_ip1);
	TSK_FREE(public_ip1);

	TSK_OBJECT_SAFE_FREE(context);
}

void test_nat_turn()
{
	tnet_socket_t *socket1 = 0, *socket2 = 0;
	tnet_nat_context_handle_t *context = 0;
	tnet_turn_allocation_id_t alloc_id1, alloc_id2;
	int ret;
	
	if(!(socket1 = TNET_SOCKET_CREATE(TNET_SOCKET_HOST_ANY, TNET_SOCKET_PORT_ANY, STUN_SERVER_PROTO))
		|| !(socket2 = TNET_SOCKET_CREATE(TNET_SOCKET_HOST_ANY, TNET_SOCKET_PORT_ANY, STUN_SERVER_PROTO)))
	{
		goto bail;
	}

	context = TNET_NAT_CONTEXT_CREATE(STUN_SERVER_PROTO, STUN_USERNAME, STUN_PASSWORD);

	if(tnet_nat_set_server_address(context, STUN_SERVER_IP))
	{
		TSK_DEBUG_ERROR("Failed to set STUN/TURN address.");
		goto bail;
	}

	/* == ALLOC
	*/
	alloc_id1 = tnet_nat_turn_allocate(context, socket1->fd);
	alloc_id2 = tnet_nat_turn_allocate(context, socket2->fd);
	if(TNET_TURN_IS_VALID_ALLOCATION_ID(alloc_id1)|| TNET_TURN_IS_VALID_ALLOCATION_ID(alloc_id2))
	{
		TSK_DEBUG_INFO("TURN allocation succeeded and id1=%llu and id2=%llu", alloc_id1, alloc_id2);
	}
	else
	{
		TSK_DEBUG_ERROR("TURN allocation failed.");
		goto bail;
	}

	tsk_thread_sleep(2000);

	/* == UNALLOC
	*/
	if((ret = tnet_nat_turn_unallocate(context, alloc_id1)) || (ret = tnet_nat_turn_unallocate(context, alloc_id2)))
	{
		TSK_DEBUG_ERROR("TURN unallocation failed with error code: %d.", ret);
		goto bail;
	}
	else
	{
		TSK_DEBUG_INFO("TURN unallocation succeeded.");
	}

bail:
	TSK_OBJECT_SAFE_FREE(socket1);
	TSK_OBJECT_SAFE_FREE(socket2);

	TSK_OBJECT_SAFE_FREE(context);
}


void test_nat()
{
	//test_nat_stun();
	test_nat_turn();
	tsk_thread_sleep(1000);
}



#endif /* TNET_TEST_NAT_H */