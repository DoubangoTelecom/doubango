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
#ifndef TNET_TEST_TRANSPORT_H
#define TNET_TEST_TRANSPORT_H

//#define REMOTE_IP4	"proxy.sipthor.net"//"192.168.0.15"
#define REMOTE_IP4	"192.168.0.13"
#define REMOTE_IP6	"2a01:e35:8632:7050:6122:2706:2124:32cb"
#define REMOTE_IP REMOTE_IP4
#define REMOTE_PORT 5083

#if defined(ANDROID) /* FIXME */
#	define LOCAL_IP4	"10.0.2.15"
#else
#	define LOCAL_IP4	TNET_SOCKET_HOST_ANY
#endif
#define LOCAL_IP6	TNET_SOCKET_HOST_ANY

#if defined(ANDROID)
#	define LOCAL_PORT 5060
#else
#	define LOCAL_PORT TNET_SOCKET_PORT_ANY
#endif

#define SIP_MESSAGE \
	"REGISTER sip:micromethod.com SIP/2.0\r\n" \
	"Via: SIP/2.0/%s %s:%d;rport;branch=z9hG4bK1245420841406%d\r\n" \
	"From: <sip:mamadou@micromethod.com>;tag=29358\r\n" \
	"To: <sip:mamadou@micromethod.com>\r\n" \
	"Call-ID: M-fa53180346f7f55ceb8d8670f9223dbb\r\n" \
	"CSeq: 201 REGISTER\r\n" \
	"Max-Forwards: 70\r\n" \
	"Contact: <sip:mamadou@%s:%d;transport=%s>\r\n" \
	"Expires: 10\r\n" \
	"\r\n"


static int tnet_tcp_cb(const tnet_transport_event_t* e)
{
	switch(e->type){
		case event_data:
			{
				TSK_DEBUG_INFO("--- TCP ---\n%s\n", (const char*)e->data);
				break;
			}
		case event_closed:
		case event_connected:
		default:
			{
				break;
			}
	}
	return 0;
}

static int tnet_udp_cb(const tnet_transport_event_t* e)
{
	switch(e->type){
		case event_data:
			{
				TSK_DEBUG_INFO("--- UDP ---\n%s\n", (const char*)e->data);
				break;
			}
		case event_closed:
		case event_connected:
		default: break;
			
	}
	return 0;
}

void test_transport_tcp_ipv4(tnet_transport_handle_t *transport)
{
	//tnet_socket_type_t type = tnet_socket_type_tcp_ipv4;
	tnet_ip_t ip;
	tnet_port_t port;
	tnet_fd_t fd = TNET_INVALID_FD;

	/* Set our callback function */
	tnet_transport_set_callback(transport, tnet_tcp_cb, "callbackdata");

	if(tnet_transport_start(transport)){
		TSK_DEBUG_ERROR("Failed to create %s.", tnet_transport_get_description(transport));
		return;
	}
	
	/* Connect to the SIP Registrar */
	if((fd = tnet_transport_connectto_2(transport, REMOTE_IP, REMOTE_PORT)) == TNET_INVALID_FD){
		TSK_DEBUG_ERROR("Failed to connect %s.", tnet_transport_get_description(transport));
		return;
	}
	
	if(tnet_sockfd_waitUntilWritable(fd, TNET_CONNECT_TIMEOUT)){
		TSK_DEBUG_ERROR("%d milliseconds elapsed and the socket is still not connected.", TNET_CONNECT_TIMEOUT);
		tnet_transport_remove_socket(transport, &fd);
		return;
	}
	

	/* Send our SIP message */
	{
		char* message = 0;
		tnet_transport_get_ip_n_port(transport, fd, &ip, &port);
		tsk_sprintf(&message, SIP_MESSAGE, "TCP", ip, port, port, ip, port, "tcp");

		if(!tnet_transport_send(transport, fd, message, strlen(message)))
		{
			TSK_DEBUG_ERROR("Failed to send data using %s.", tnet_transport_get_description(transport));
			TSK_FREE(message);
			return;
		}
		TSK_FREE(message);
	}
	
}


int test_transport_udp_ipv4(tnet_transport_handle_t *transport)
{
	//tnet_socket_type_t type = tnet_socket_type_udp_ipv4;
	tnet_ip_t ip;
	tnet_port_t port;
	tnet_fd_t fd = TNET_INVALID_FD;
	
	/* Set our callback function */
	tnet_transport_set_callback(transport, tnet_udp_cb, "callbackdata");

	if(tnet_transport_start(transport)){
		TSK_DEBUG_ERROR("Failed to create %s.", tnet_transport_get_description(transport));
		return -1;
	}

	/* Connect to our SIP REGISTRAR */
	if((fd = tnet_transport_connectto_2(transport, REMOTE_IP, REMOTE_PORT)) == TNET_INVALID_FD){
		TSK_DEBUG_ERROR("Failed to connect %s.", tnet_transport_get_description(transport));
		//tnet_transport_shutdown(transport);
		return -2;
	}

	if(tnet_sockfd_waitUntilWritable(fd, TNET_CONNECT_TIMEOUT)){
		TSK_DEBUG_ERROR("%d milliseconds elapsed and the socket is still not connected.", TNET_CONNECT_TIMEOUT);
		tnet_transport_remove_socket(transport, &fd);
		return -3;
	}

	//tsk_thread_sleep(2000);

	/* Send our SIP message */
	/*while(1)*/{
		char* message = 0;
		tnet_transport_get_ip_n_port(transport, fd, &ip, &port);
		//memset(ip, 0, sizeof(ip));
		//memcpy(ip, "192.168.0.12", 12);
		tsk_sprintf(&message, SIP_MESSAGE, "UDP", ip, port, port, ip, port, "udp");

		if(!tnet_transport_send(transport, fd, message, strlen(message)))
		{
			TSK_DEBUG_ERROR("Failed to send data using %s.", tnet_transport_get_description(transport));
			//tnet_transport_shutdown(transport);
			TSK_FREE(message);
			return -4;
		}
		TSK_FREE(message);
	}

	return 0;
}

void test_transport()
{
#define TEST_TCP 1
#define TEST_UDP 0


#if TEST_UDP
	tnet_transport_handle_t *udp = tnet_transport_create(LOCAL_IP4, LOCAL_PORT, tnet_socket_type_udp_ipv4, "UDP/IPV4 TRANSPORT");
	test_transport_udp_ipv4(udp);
#endif

#if TEST_TCP
	tnet_transport_handle_t *tcp = tnet_transport_create(LOCAL_IP4, LOCAL_PORT, tnet_socket_type_tcp_ipv4, "TCP/IPV4 TRANSPORT");
	test_transport_tcp_ipv4(tcp);
#endif	

//#if defined(ANDROID)
	tsk_thread_sleep(1000000);
//#else
	getchar();
//#endif

#if TEST_UDP
	TSK_OBJECT_SAFE_FREE(udp);
#endif

#if TEST_TCP
	TSK_OBJECT_SAFE_FREE(tcp);
#endif
}


#endif /* TNET_TEST_TRANSPORT_H*/
