/*
* Copyright (C) 2010-2011 Mamadou Diop.
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
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

/**@file tnet.h
 * @brief Protocol agnostic socket.
 *
 */
#ifndef TNET_SOCKET_H
#define TNET_SOCKET_H

#include "tinynet_config.h"
#include "tnet_types.h"
#include "tls/tnet_tls.h"
#include "tls/tnet_dtls.h"

#include "tsk_list.h"

/** List of all supported socket types.
* @def tnet_socket_type_e
**/
TNET_BEGIN_DECLS


typedef enum tnet_socket_type_e {
    tnet_socket_type_invalid				= 0x0000, /**< Invalid socket.*/

#define TNET_SOCKET_TYPE_IPV4				(0x0001 << 0)
#define TNET_SOCKET_TYPE_UDP				(0x0001 << 1)
#define TNET_SOCKET_TYPE_DTLS				(0x0001 << 2)
#define TNET_SOCKET_TYPE_TCP				(0x0001 << 3)
#define TNET_SOCKET_TYPE_TLS				(0x0001 << 4)
#define TNET_SOCKET_TYPE_SCTP				(0x0001 << 5)
#define TNET_SOCKET_TYPE_WS					(0x0001 << 6)
#define TNET_SOCKET_TYPE_WSS				(0x0001 << 7)
    tnet_socket_type_udp_ipv4				= (TNET_SOCKET_TYPE_IPV4 | TNET_SOCKET_TYPE_UDP), /**< UDP/IPv4 socket.*/
    tnet_socket_type_dtls_ipv4				= (TNET_SOCKET_TYPE_IPV4 | TNET_SOCKET_TYPE_DTLS), /**< DTLS/IPv4 socket.*/
    tnet_socket_type_tcp_ipv4				= (TNET_SOCKET_TYPE_IPV4 | TNET_SOCKET_TYPE_TCP), /**< TCP/IPv4 socket.*/
    tnet_socket_type_tls_ipv4				= (TNET_SOCKET_TYPE_IPV4 | TNET_SOCKET_TYPE_TLS), /**< TLS/IPv4 socket.*/
    tnet_socket_type_sctp_ipv4				= (TNET_SOCKET_TYPE_IPV4 | TNET_SOCKET_TYPE_SCTP), /**< SCTP/IPv4 socket.*/
    tnet_socket_type_ws_ipv4				= (TNET_SOCKET_TYPE_IPV4 | TNET_SOCKET_TYPE_WS), /**< WebSocket/IPv4 socket.*/
    tnet_socket_type_wss_ipv4				= (TNET_SOCKET_TYPE_IPV4 | TNET_SOCKET_TYPE_WSS), /**< WebSocket Secure/IPv4 socket.*/

#define TNET_SOCKET_TYPE_IPSEC				(0x0001 << 8)
    tnet_socket_type_udp_ipsec_ipv4			= (TNET_SOCKET_TYPE_IPSEC | tnet_socket_type_udp_ipv4), /**< UDP/IPSec/IPv4 socket.*/
    tnet_socket_type_dtls_ipsec_ipv4		= (TNET_SOCKET_TYPE_IPSEC | tnet_socket_type_dtls_ipv4), /**< DTLS/IPSec/IPv4 socket.*/
    tnet_socket_type_tcp_ipsec_ipv4			= (TNET_SOCKET_TYPE_IPSEC | tnet_socket_type_tcp_ipv4), /**< TCP/IPSec/IPv4 socket.*/
    tnet_socket_type_tls_ipsec_ipv4			= (TNET_SOCKET_TYPE_IPSEC | tnet_socket_type_tls_ipv4),	/**< TLS/IPSec /IPv4socket.*/
    tnet_socket_type_sctp_ipsec_ipv4		= (TNET_SOCKET_TYPE_IPSEC | tnet_socket_type_sctp_ipv4), /**< SCTP/IPSec/IPv4 socket.*/
    tnet_socket_type_ws_ipsec_ipv4		= (TNET_SOCKET_TYPE_IPSEC | tnet_socket_type_ws_ipv4), /**< WS/IPSec/IPv4 socket.*/
    tnet_socket_type_wss_ipsec_ipv4		= (TNET_SOCKET_TYPE_IPSEC | tnet_socket_type_wss_ipv4), /**< WSS/IPSec/IPv4 socket.*/

#define TNET_SOCKET_TYPE_IPV6				(0x0001 << 12)
    tnet_socket_type_udp_ipv6				= (TNET_SOCKET_TYPE_IPV6 | (tnet_socket_type_udp_ipv4 ^ TNET_SOCKET_TYPE_IPV4)),	/**< UDP/IPv6 socket.*/
    tnet_socket_type_dtls_ipv6				= (TNET_SOCKET_TYPE_IPV6 | (tnet_socket_type_dtls_ipv4 ^ TNET_SOCKET_TYPE_IPV4)),	/**< DTLS/IPv6 socket.*/
    tnet_socket_type_tcp_ipv6				= (TNET_SOCKET_TYPE_IPV6 | (tnet_socket_type_tcp_ipv4 ^ TNET_SOCKET_TYPE_IPV4)),	/**< TCP/IPv6 socket.*/
    tnet_socket_type_tls_ipv6				= (TNET_SOCKET_TYPE_IPV6 | (tnet_socket_type_tls_ipv4 ^ TNET_SOCKET_TYPE_IPV4)),	/**< TLS/IPv6 socket.*/
    tnet_socket_type_sctp_ipv6				= (TNET_SOCKET_TYPE_IPV6 | (tnet_socket_type_sctp_ipv4 ^ TNET_SOCKET_TYPE_IPV4)),	/**< SCTP/IPv6 socket.*/
    tnet_socket_type_ws_ipv6				= (TNET_SOCKET_TYPE_IPV6 | (tnet_socket_type_ws_ipv4 ^ TNET_SOCKET_TYPE_IPV4)),	/**< WS/IPv6 socket.*/
    tnet_socket_type_wss_ipv6				= (TNET_SOCKET_TYPE_IPV6 | (tnet_socket_type_wss_ipv4 ^ TNET_SOCKET_TYPE_IPV4)),	/**< WSS/IPv6 socket.*/
    tnet_socket_type_udp_ipsec_ipv6			= (TNET_SOCKET_TYPE_IPSEC | tnet_socket_type_udp_ipv6), /**< UDP/IPSec/IPv6 socket.*/
    tnet_socket_type_tcp_ipsec_ipv6			= (TNET_SOCKET_TYPE_IPSEC | tnet_socket_type_tcp_ipv6), /**< TCP/IPSec/IPv6 socket.*/
    tnet_socket_type_tls_ipsec_ipv6			= (TNET_SOCKET_TYPE_IPSEC | tnet_socket_type_tls_ipv6),	/**< TLS/IPSec/IPv6 socket.*/
    tnet_socket_type_sctp_ipsec_ipv6		= (TNET_SOCKET_TYPE_IPSEC | tnet_socket_type_sctp_ipv6),/**< SCTP/IPSec/IPv6 socket.*/
    tnet_socket_type_ws_ipsec_ipv6			= (TNET_SOCKET_TYPE_IPSEC | tnet_socket_type_ws_ipv6),	/**< WS/IPSec/IPv6 socket.*/
    tnet_socket_type_wss_ipsec_ipv6		= (TNET_SOCKET_TYPE_IPSEC | tnet_socket_type_wss_ipv6),/**< WSS/IPSec/IPv6 socket.*/

#define TNET_SOCKET_TYPE_IPV46				(TNET_SOCKET_TYPE_IPV4 | TNET_SOCKET_TYPE_IPV6)
    tnet_socket_type_udp_ipv46				= (TNET_SOCKET_TYPE_IPV46 | (tnet_socket_type_udp_ipv4 | tnet_socket_type_udp_ipv6)),	/**< UDP/IPv4/6 socket.*/
    tnet_socket_type_dtls_ipv46				= (TNET_SOCKET_TYPE_IPV46 | (tnet_socket_type_dtls_ipv4 | tnet_socket_type_dtls_ipv6)),	/**< DTLS/IPv4/6 socket.*/
    tnet_socket_type_tcp_ipv46				= (TNET_SOCKET_TYPE_IPV46 | (tnet_socket_type_tcp_ipv4 | tnet_socket_type_tcp_ipv6)),	/**< TCP/IPv4/6 socket.*/
    tnet_socket_type_tls_ipv46				= (TNET_SOCKET_TYPE_IPV46 | (tnet_socket_type_tls_ipv4 | tnet_socket_type_tls_ipv6)),	/**< TLS/IPv4/6 socket.*/
    tnet_socket_type_sctp_ipv46				= (TNET_SOCKET_TYPE_IPV46 | (tnet_socket_type_sctp_ipv4 | tnet_socket_type_sctp_ipv6)),	/**< SCTP/IPv4/6 socket.*/
    tnet_socket_type_ws_ipv46				= (TNET_SOCKET_TYPE_IPV46 | (tnet_socket_type_ws_ipv4 | tnet_socket_type_ws_ipv6)),	/**< WS/IPv4/6 socket.*/
    tnet_socket_type_wss_ipv46				= (TNET_SOCKET_TYPE_IPV46 | (tnet_socket_type_wss_ipv4 | tnet_socket_type_wss_ipv6)),	/**< WSS/IPv4/6 socket.*/
    tnet_socket_type_udp_ipsec_ipv46		= (TNET_SOCKET_TYPE_IPSEC | tnet_socket_type_udp_ipv46), /**< UDP/IPSec/IPv4/6 socket.*/
    tnet_socket_type_dtls_ipsec_ipv46		= (TNET_SOCKET_TYPE_IPSEC | tnet_socket_type_dtls_ipv46), /**< DTLS/IPSec/IPv4/6 socket.*/
    tnet_socket_type_tcp_ipsec_ipv46		= (TNET_SOCKET_TYPE_IPSEC | tnet_socket_type_tcp_ipv46), /**< TCP/IPSec/IPv4/6 socket.*/
    tnet_socket_type_tls_ipsec_ipv46		= (TNET_SOCKET_TYPE_IPSEC | tnet_socket_type_tls_ipv46),	/**< TLS/IPSec/IPv4/6 socket.*/
    tnet_socket_type_sctp_ipsec_ipv46		= (TNET_SOCKET_TYPE_IPSEC | tnet_socket_type_sctp_ipv46),/**< SCTP/IPSec/IPv4/6 socket.*/
    tnet_socket_type_ws_ipsec_ipv46		= (TNET_SOCKET_TYPE_IPSEC | tnet_socket_type_ws_ipv46),/**< WS/IPSec/IPv4/6 socket.*/
    tnet_socket_type_wss_ipsec_ipv46		= (TNET_SOCKET_TYPE_IPSEC | tnet_socket_type_wss_ipv46),/**< WSS/IPSec/IPv4/6 socket.*/
}
tnet_socket_type_t;


/**@def TNET_SOCKET_IS_VALID
* Checks the socket validity.
*/
#define TNET_SOCKET_IS_VALID(socket)		((socket) && TNET_SOCKET_TYPE_IS_VALID((socket)->type) && (socket)->fd >0)
#define TNET_SOCKET_TYPE_IS_VALID(type)		((type) !=tnet_socket_type_invalid)

#define TNET_SOCKET_TYPE_IS_STREAM(type)	( ((type & TNET_SOCKET_TYPE_UDP) !=  TNET_SOCKET_TYPE_UDP) && ((type & TNET_SOCKET_TYPE_DTLS) !=  TNET_SOCKET_TYPE_DTLS) )
#define TNET_SOCKET_TYPE_IS_DGRAM(type)		( !TNET_SOCKET_TYPE_IS_STREAM((type)) )

#define TNET_SOCKET_TYPE_IS_IPV4(type)		( ((type & TNET_SOCKET_TYPE_IPV4) ==  TNET_SOCKET_TYPE_IPV4) )
#define TNET_SOCKET_TYPE_IS_IPV6(type)		( ((type & TNET_SOCKET_TYPE_IPV6) ==  TNET_SOCKET_TYPE_IPV6) )
#define TNET_SOCKET_TYPE_IS_IPV46(type)		( TNET_SOCKET_TYPE_IS_IPV4(type) && TNET_SOCKET_TYPE_IS_IPV6(type) )

#define TNET_SOCKET_TYPE_IS_IPSEC(type)		( ((type & TNET_SOCKET_TYPE_IPSEC) ==  TNET_SOCKET_TYPE_IPSEC) )

#define TNET_SOCKET_TYPE_IS_UDP(type)		( ((type & TNET_SOCKET_TYPE_UDP) ==  TNET_SOCKET_TYPE_UDP) )
#define TNET_SOCKET_TYPE_IS_DTLS(type)		( ((type & TNET_SOCKET_TYPE_DTLS) ==  TNET_SOCKET_TYPE_DTLS) )
#define TNET_SOCKET_TYPE_IS_TCP(type)		( ((type & TNET_SOCKET_TYPE_TCP) ==  TNET_SOCKET_TYPE_TCP) )
#define TNET_SOCKET_TYPE_IS_TLS(type)		( ((type & TNET_SOCKET_TYPE_TLS) ==  TNET_SOCKET_TYPE_TLS) )
#define TNET_SOCKET_TYPE_IS_SCTP(type)		( ((type & TNET_SOCKET_TYPE_SCTP) ==  TNET_SOCKET_TYPE_SCTP) )
#define TNET_SOCKET_TYPE_IS_WS(type)		( ((type & TNET_SOCKET_TYPE_WS) ==  TNET_SOCKET_TYPE_WS) )
#define TNET_SOCKET_TYPE_IS_WSS(type)		( ((type & TNET_SOCKET_TYPE_WSS) ==  TNET_SOCKET_TYPE_WSS) )

#define TNET_SOCKET_TYPE_IS_SECURE(type)	( TNET_SOCKET_TYPE_IS_IPSEC(type) || TNET_SOCKET_TYPE_IS_TLS(type) || TNET_SOCKET_TYPE_IS_DTLS(type) || TNET_SOCKET_TYPE_IS_WSS(type) )

#define TNET_SOCKET_TYPE_UNSET(type, OP)		(type = TNET_SOCKET_TYPE_IS_##OP(type) ? type ^= TNET_SOCKET_TYPE_##OP : type)
#define TNET_SOCKET_TYPE_SET(type, OP)          (type |= TNET_SOCKET_TYPE_##OP)

#define TNET_SOCKET_TYPE_SET_IPV4(type)		(type |= TNET_SOCKET_TYPE_IPV4)
#define TNET_SOCKET_TYPE_SET_IPV4Only(type)	(type = TNET_SOCKET_TYPE_IS_IPV6(type) ? (type ^TNET_SOCKET_TYPE_IPV6)|TNET_SOCKET_TYPE_IPV4 : type)
#define TNET_SOCKET_TYPE_SET_IPV6(type)		(type |= TNET_SOCKET_TYPE_IPV6)
#define TNET_SOCKET_TYPE_SET_IPV6Only(type)	(type = TNET_SOCKET_TYPE_IS_IPV4(type) ? (type ^TNET_SOCKET_TYPE_IPV4)|TNET_SOCKET_TYPE_IPV6 : type)
#define TNET_SOCKET_TYPE_SET_IPV46(type)	(type |= TNET_SOCKET_TYPE_IPV46)

#define TNET_SOCKET_TYPE_SET_IPSEC(type)	(type |=TNET_SOCKET_TYPE_IPSEC)

#define TNET_SOCKET_TYPE_SET_UDP(type)\
	(TNET_SOCKET_TYPE_UNSET(type,TCP), TNET_SOCKET_TYPE_UNSET(type,TLS), TNET_SOCKET_TYPE_UNSET(type,SCTP), TNET_SOCKET_TYPE_UNSET(type,WS), TNET_SOCKET_TYPE_UNSET(type,WSS), TNET_SOCKET_TYPE_UNSET(type,DTLS), type |=TNET_SOCKET_TYPE_UDP)
#define TNET_SOCKET_TYPE_SET_DTLS(type)\
	(TNET_SOCKET_TYPE_UNSET(type,TCP), TNET_SOCKET_TYPE_UNSET(type,TLS), TNET_SOCKET_TYPE_UNSET(type,SCTP), TNET_SOCKET_TYPE_UNSET(type,WS), TNET_SOCKET_TYPE_UNSET(type,WSS), TNET_SOCKET_TYPE_UNSET(type,UDP), type |=TNET_SOCKET_TYPE_DTLS)
#define TNET_SOCKET_TYPE_SET_TCP(type)\
	(TNET_SOCKET_TYPE_UNSET(type,UDP), TNET_SOCKET_TYPE_UNSET(type,TLS), TNET_SOCKET_TYPE_UNSET(type,SCTP), TNET_SOCKET_TYPE_UNSET(type,WS), TNET_SOCKET_TYPE_UNSET(type,WSS), TNET_SOCKET_TYPE_UNSET(type,DTLS), type |=TNET_SOCKET_TYPE_TCP)
#define TNET_SOCKET_TYPE_SET_TLS(type)\
	(TNET_SOCKET_TYPE_UNSET(type,TCP), TNET_SOCKET_TYPE_UNSET(type,UDP), TNET_SOCKET_TYPE_UNSET(type,SCTP), TNET_SOCKET_TYPE_UNSET(type,WS), TNET_SOCKET_TYPE_UNSET(type,WSS), TNET_SOCKET_TYPE_UNSET(type,DTLS), type |=TNET_SOCKET_TYPE_TLS)
#define TNET_SOCKET_TYPE_SET_SCTP(type)\
	(TNET_SOCKET_TYPE_UNSET(type,TCP), TNET_SOCKET_TYPE_UNSET(type,TLS), TNET_SOCKET_TYPE_UNSET(type,UDP), TNET_SOCKET_TYPE_UNSET(type,WS), TNET_SOCKET_TYPE_UNSET(type,WSS), TNET_SOCKET_TYPE_UNSET(type,DTLS), type |=TNET_SOCKET_TYPE_SCTP)
#define TNET_SOCKET_TYPE_SET_WS(type)\
	(TNET_SOCKET_TYPE_UNSET(type,TCP), TNET_SOCKET_TYPE_UNSET(type,TLS), TNET_SOCKET_TYPE_UNSET(type,UDP), TNET_SOCKET_TYPE_UNSET(type,SCTP), TNET_SOCKET_TYPE_UNSET(type,WSS), TNET_SOCKET_TYPE_UNSET(type,DTLS), type |=TNET_SOCKET_TYPE_WS)
#define TNET_SOCKET_TYPE_SET_WSS(type)\
	(TNET_SOCKET_TYPE_UNSET(type,TCP), TNET_SOCKET_TYPE_UNSET(type,TLS), TNET_SOCKET_TYPE_UNSET(type,UDP), TNET_SOCKET_TYPE_UNSET(type,WS), TNET_SOCKET_TYPE_UNSET(type,SCTP), TNET_SOCKET_TYPE_UNSET(type,DTLS), type |=TNET_SOCKET_TYPE_WSS)

/**
* @def TNET_SOCKET_HOST_ANY
* Any IPv4/IPv6 host.
*/
/**
* @def TNET_SOCKET_PORT_ANY
* Any port.
*/
#define TNET_SOCKET_HOST_ANY tsk_null
#define TNET_SOCKET_PORT_ANY 0

/**
* Socket.
*/
typedef struct tnet_socket_s {
    TSK_DECLARE_OBJECT;

    tnet_socket_type_t type;
    tnet_fd_t fd;
    tnet_ip_t ip;
    uint16_t port;

    tnet_tls_socket_handle_t* tlshandle;
    tnet_dtls_socket_handle_t* dtlshandle;
}
tnet_socket_t;

typedef tnet_socket_t tnet_socket_udp_t; /**< UDP socket. */
typedef tnet_socket_t tnet_socket_tcp_t; /**< TCP socket. */
typedef tnet_socket_t tnet_socket_tls_t; /**< TLS socket. */
typedef tnet_socket_t tnet_socket_ws_t; /**< WS socket. */
typedef tnet_socket_t tnet_socket_wss_t; /**< WSS socket. */
typedef tnet_socket_t tnet_socket_ipsec_t; /**< IPSec socket. */
typedef tsk_list_t tnet_sockets_L_t; /**< List of @ref tnet_socket_t elements. */

TINYNET_API tnet_socket_t* tnet_socket_create_2(const char*host, tnet_port_t port, tnet_socket_type_t type, tsk_bool_t nonblocking, tsk_bool_t bindsocket);
TINYNET_API tnet_socket_t* tnet_socket_create(const char* host, tnet_port_t port, tnet_socket_type_t type);
TINYNET_API int tnet_socket_send_stream(tnet_socket_t* self, const void* data, tsk_size_t size);
TINYNET_API int tnet_socket_handle_brokenpipe(tnet_socket_t* self);

TINYNET_GEXTERN const tsk_object_def_t *tnet_socket_def_t;


TNET_END_DECLS

#endif /* TNET_SOCKET_H */

