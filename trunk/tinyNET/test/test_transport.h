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
#ifndef TNET_TEST_TRANSPORT_H
#define TNET_TEST_TRANSPORT_H

#define REMOTE_IPV4 "192.168.0.15"
#define SIP_MESSAGE \
	"REGISTER sip:wonderland.net SIP/2.0\r\n" \
	"Via: SIP/2.0/UDP %s:%d;comp=sigcomp;rport;branch=z9hG4bK1245420841406\r\n" \
	"From: \"Mamadou\" <sip:mamadou@wonderland.net>;tag=29358\r\n" \
	"To: <sip:mamadou@wonderland.net>;tag= 12345\r\n" \
	"Call-ID: M-fa53180346f7f55ceb8d8670f9223dbb\r\n" \
	"CSeq: 201 REGISTER\r\n" \
	"Max-Forwards: 70\r\n" \
	"Contact: <sip:mamadou@%s:%d;comp=sigcomp;transport=%s>\r\n" \
	"Expires: 600\r\n" \
	"\r\n"

void test_transport_tcp_ipv4()
{
	tnet_socket_type_t type = tnet_socket_type_tcp_ipv4;
	tnet_transport_handle_t *transport = tnet_transport_start(0, 0, type, "TCP/IPV4 TRANSPORT");
	if(!transport)
	{
		TSK_DEBUG_ERROR("Failed to create TCP/IPv4 transport.");
		return;
	}

	tsk_thread_sleep(1000);

	if(tnet_transport_connectto(transport, REMOTE_IPV4, 5060))
	{
		TSK_DEBUG_ERROR("Failed to connect TCP/IPv4 transport.");
		tnet_transport_shutdown(&transport);
		return;
	}

	while(!tnet_transport_get_isconnected(transport))
	{
		/* Connecto succeed but not connected yet.*/
		tsk_thread_sleep(2000);
	}

	{
		char* message = 0;
		const char* ip =tnet_transport_get_tip(transport);
		uint16_t port = tnet_transport_get_tport(transport);
		tsk_sprintf(&message, SIP_MESSAGE, ip, port, ip, port, "tcp");
		if(tnet_transport_send(transport, message, strlen(message)))
		{
			TSK_DEBUG_ERROR("Failed to send data using TCP/IPv4 transport.");
			tnet_transport_shutdown(&transport);
			TSK_FREE(message);
			return;
		}
		TSK_FREE(message);
	}
}


void test_transport_udp_ipv4()
{
	tnet_socket_type_t type = tnet_socket_type_udp_ipv4;
	tnet_transport_handle_t *transport = tnet_transport_start(0, 0, type, "UDP/IPV4 TRANSPORT");
	if(!transport)
	{
		TSK_DEBUG_ERROR("Failed to create UDP/IPv4 transport.");
		return;
	}

	tsk_thread_sleep(1000);

	{
		struct sockaddr addr; 
		char* message = 0;
		const char* ip =tnet_transport_get_tip(transport);
		uint16_t port = tnet_transport_get_tport(transport);
		tsk_sprintf(&message, SIP_MESSAGE, ip, port, ip, port, "udp");
		tnet_sockaddr_init(REMOTE_IPV4, 5060, type, &addr);
		if(tnet_transport_sendto(transport, &addr, message, strlen(message)))
		{
			TSK_DEBUG_ERROR("Failed to send data using TCP/IPv4 transport.");
			tnet_transport_shutdown(&transport);
			TSK_FREE(message);
			return;
		}
		TSK_FREE(message);
	}
}

void test_transport()
{
	test_transport_tcp_ipv4();
	//test_transport_udp_ipv4();

	/*struct sockaddr addr; 
	tnet_socket_type_t type = tnet_socket_type_tcp_ipv4;
	tnet_transport_handle_t *handle = tnet_transport_start(0, 5060, type, "TCP/IPV4 TRANSPORT");
	//tnet_transport_handle_t *handle = tnet_transport_start(0, 5060, type, "UDP/IPV4 TRANSPORT");

	tsk_thread_sleep(1000);

	tnet_sockaddr_init("192.168.0.15", 5060, type, &addr);
	//tnet_transport_sendto(handle, &addr, "mamadou", sizeof("mamadou"));
	tnet_transport_connectto(handle, &addr);*/
	
}


#endif /* TNET_TEST_TRANSPORT_H*/