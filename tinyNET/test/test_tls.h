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
#ifndef TNET_TEST_TLS_H
#define TNET_TEST_TLS_H

#define TEST_TLS_REMOTE_IP "192.168.16.225"
#define TEST_TLS_REMOTE_PORT 4061

#define TLS_TEST_SIP_MESSAGE \
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

static int tnet_tls_cb(const tnet_transport_event_t* e)
{
    switch(e->type) {
    case event_data: {
        TSK_DEBUG_INFO("--- TLS ---\n%s\n", (const char*)e->data);
        break;
    }
    case event_closed:
    case event_connected:
    default: {
        break;
    }
    }
    return 0;
}


void test_tls()
{
    tnet_transport_handle_t *transport = tnet_transport_create(TNET_SOCKET_HOST_ANY, TNET_SOCKET_PORT_ANY, tnet_socket_type_tls_ipv4, "TLS/IPV4 TRANSPORT");

    tnet_ip_t ip;
    tnet_port_t port;
    tnet_fd_t fd = TNET_INVALID_FD;

    if(tnet_transport_start(transport)) {
        TSK_DEBUG_ERROR("Failed to create %s.", tnet_transport_get_description(transport));
        return;
    }

    /* Set our callback function */
    tnet_transport_set_callback(transport, tnet_tls_cb, "callbackdata");



    /* Connect to the SIP Registrar */
    if((fd = tnet_transport_connectto_2(transport, TEST_TLS_REMOTE_IP, TEST_TLS_REMOTE_PORT)) == TNET_INVALID_FD) {
        TSK_DEBUG_ERROR("Failed to connect %s.", tnet_transport_get_description(transport));
        return;
    }

    if(tnet_sockfd_waitUntilWritable(fd, TNET_CONNECT_TIMEOUT)) {
        TSK_DEBUG_ERROR("%d milliseconds elapsed and the socket is still not connected.", TNET_CONNECT_TIMEOUT);
        tnet_transport_remove_socket(transport, &fd);
        return;
    }

    /* Send our SIP message */
    {
        char* message = 0;
        tnet_transport_get_ip_n_port(transport, fd, &ip, &port);
        tsk_sprintf(&message, TLS_TEST_SIP_MESSAGE, "TLS", ip, port, port, ip, port, "tls");

        if(!tnet_transport_send(transport, fd, message, strlen(message))) {
            TSK_DEBUG_ERROR("Failed to send data using TCP/IPv4 transport.");
            TSK_FREE(message);
            return;
        }
        TSK_FREE(message);
    }

    TSK_OBJECT_SAFE_FREE(transport);
}

#endif /* TNET_TEST_TLS_H */

