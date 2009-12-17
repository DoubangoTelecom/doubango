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
	"REGISTER sip:micromethod.com SIP/2.0\r\n" \
	"Via: SIP/2.0/%s %s:%d;rport;branch=z9hG4bK1245420841406%d\r\n" \
	"From: <sip:mamadou@micromethod.com>;tag=29358\r\n" \
	"To: <sip:mamadou@micromethod.com>\r\n" \
	"Call-ID: M-fa53180346f7f55ceb8d8670f9223dbb\r\n" \
	"CSeq: 201 REGISTER\r\n" \
	"Max-Forwards: 70\r\n" \
	"Contact: <sip:mamadou@%s:%d;transport=%s>\r\n" \
	"Expires: 20\r\n" \
	"\r\n"


static int tnet_tcp_data_read(tnet_fd_t fd, const void* data, size_t size)
{
	/* Do quick job. */
	TSK_DEBUG_INFO("--- UDP ---\n%s\n", data);
	return 0;
}

static int tnet_udp_data_read(tnet_fd_t fd, const void* data, size_t size)
{
	/* Do quick job. */
	TSK_DEBUG_INFO("--- UDP ---\n%s\n", data);
	return 0;
}

void test_transport_tcp_ipv4()
{
	tnet_socket_type_t type = tnet_socket_type_tcp_ipv4;
	tnet_ip_t ip;
	tnet_port_t port;
	tnet_fd_t fd = INVALID_SOCKET;

	tnet_transport_handle_t *transport = tnet_transport_start(TNET_SOCKET_HOST_ANY, TNET_SOCKET_PORT_ANY, type, "TCP/IPV4 TRANSPORT");
	if(!transport)
	{
		TSK_DEBUG_ERROR("Failed to create TCP/IPv4 transport.");
		return;
	}

	//tsk_thread_sleep(500);

	/* Connect to the SIP Registrar */
	if((fd = tnet_transport_connectto(transport, REMOTE_IPV4, 5060)) == INVALID_SOCKET)
	{
		TSK_DEBUG_ERROR("Failed to connect TCP/IPv4 transport.");
		tnet_transport_shutdown(&transport);
		return;
	}

	/* Set our callback function */
	tnet_transport_set_callback(transport, fd, tnet_tcp_data_read);

	//while(!tnet_transport_get_isconnected(transport))
	{
		/* Connecto succeed but not connected yet.*/
		tsk_thread_sleep(500);
	}

	/* Send our SIP message */
	{
		char* message = 0;
		tnet_transport_get_ip_n_port(transport, fd, &ip, &port);
		tsk_sprintf(&message, SIP_MESSAGE, "TCP", ip, port, port, ip, port, "tcp");

		if(!tnet_transport_send(transport, fd, message, strlen(message)))
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
	tnet_ip_t ip;
	tnet_port_t port;
	tnet_fd_t fd = INVALID_SOCKET;

	tnet_transport_handle_t *transport = tnet_transport_start(TNET_SOCKET_HOST_ANY, TNET_SOCKET_PORT_ANY, type, "UDP/IPV4 TRANSPORT");
	if(!transport)
	{
		TSK_DEBUG_ERROR("Failed to create UDP/IPv4 transport.");
		return;
	}

	//tsk_thread_sleep(2000);

	/* Connect to our SIP REGISTRAR */
	if((fd = tnet_transport_connectto(transport, REMOTE_IPV4, 5060)) == INVALID_SOCKET)
	{
		TSK_DEBUG_ERROR("Failed to connect UDP/IPv4 transport.");
		tnet_transport_shutdown(&transport);
		return;
	}

	//tsk_thread_sleep(2000);

	/* Set our callback function */
	tnet_transport_set_callback(transport, fd, tnet_udp_data_read);

	/* Send our SIP message */
	{
		char* message = 0;
		tnet_transport_get_ip_n_port(transport, fd, &ip, &port);
		tsk_sprintf(&message, SIP_MESSAGE, "UDP", ip, port, port, ip, port, "udp");

		if(!tnet_transport_send(transport, fd, message, strlen(message)))
		{
			TSK_DEBUG_ERROR("Failed to send data using UDP/IPv4 transport.");
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