/*
* Copyright (C) 2009-2010 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango.org>
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
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TNET_SOCKET_H
#define TNET_SOCKET_H

#include "tinynet_config.h"
#include "tnet_types.h"
#include "tls/tnet_tls.h"

#include "tsk_list.h"

/** List of all supported socket types.
* @def tnet_socket_type_e
**/
TNET_BEGIN_DECLS


typedef enum tnet_socket_type_e
{
	tnet_socket_type_invalid				= 0x0000, /**< Invalid socket.*/

#define TNET_SOCKET_TYPE_IPV4				(0x0001 << 0)
#define TNET_SOCKET_TYPE_UDP				(0x0001 << 1)
#define TNET_SOCKET_TYPE_TCP				(0x0001 << 2)
#define TNET_SOCKET_TYPE_TLS				(0x0001 << 3)
#define TNET_SOCKET_TYPE_SCTP				(0x0001 << 4)
	tnet_socket_type_udp_ipv4				= (TNET_SOCKET_TYPE_IPV4 | TNET_SOCKET_TYPE_UDP), /**< UDP/IPv4 socket.*/
	tnet_socket_type_tcp_ipv4				= (TNET_SOCKET_TYPE_IPV4 | TNET_SOCKET_TYPE_TCP), /**< TCP/IPv4 socket.*/
	tnet_socket_type_tls_ipv4				= (TNET_SOCKET_TYPE_IPV4 | TNET_SOCKET_TYPE_TLS), /**< TLS/IPv4 socket.*/
	tnet_socket_type_sctp_ipv4				= (TNET_SOCKET_TYPE_IPV4 | TNET_SOCKET_TYPE_SCTP), /**< SCTP/IPv4 socket.*/

#define TNET_SOCKET_TYPE_IPSEC				(0x0001 << 8)
	tnet_socket_type_udp_ipsec_ipv4			= (TNET_SOCKET_TYPE_IPSEC | tnet_socket_type_udp_ipv4), /**< UDP/IPSec/IPv4 socket.*/
	tnet_socket_type_tcp_ipsec_ipv4			= (TNET_SOCKET_TYPE_IPSEC | tnet_socket_type_tcp_ipv4), /**< TCP/IPSec/IPv4 socket.*/
	tnet_socket_type_tls_ipsec_ipv4			= (TNET_SOCKET_TYPE_IPSEC | tnet_socket_type_tls_ipv4),	/**< TLS/IPSec /IPv4socket.*/
	tnet_socket_type_sctp_ipsec_ipv4		= (TNET_SOCKET_TYPE_IPSEC | tnet_socket_type_sctp_ipv4), /**< SCTP/IPSec/IPv4 socket.*/

#define TNET_SOCKET_TYPE_IPV6				(0x0001 << 12)
	tnet_socket_type_udp_ipv6				= (TNET_SOCKET_TYPE_IPV6 | (tnet_socket_type_udp_ipv4 ^ TNET_SOCKET_TYPE_IPV4)),	/**< UDP/IPv6 socket.*/
	tnet_socket_type_tcp_ipv6				= (TNET_SOCKET_TYPE_IPV6 | (tnet_socket_type_tcp_ipv4 ^ TNET_SOCKET_TYPE_IPV4)),	/**< TCP/IPv6 socket.*/
	tnet_socket_type_tls_ipv6				= (TNET_SOCKET_TYPE_IPV6 | (tnet_socket_type_tls_ipv4 ^ TNET_SOCKET_TYPE_IPV4)),	/**< TLS/IPv6 socket.*/
	tnet_socket_type_sctp_ipv6				= (TNET_SOCKET_TYPE_IPV6 | (tnet_socket_type_sctp_ipv4 ^ TNET_SOCKET_TYPE_IPV4)),	/**< SCTP/IPv6 socket.*/
	tnet_socket_type_udp_ipsec_ipv6			= (TNET_SOCKET_TYPE_IPSEC | tnet_socket_type_udp_ipv6), /**< UDP/IPSec/IPv6 socket.*/
	tnet_socket_type_tcp_ipsec_ipv6			= (TNET_SOCKET_TYPE_IPSEC | tnet_socket_type_tcp_ipv6), /**< TCP/IPSec/IPv6 socket.*/
	tnet_socket_type_tls_ipsec_ipv6			= (TNET_SOCKET_TYPE_IPSEC | tnet_socket_type_tls_ipv6),	/**< TLS/IPSec/IPv6 socket.*/
	tnet_socket_type_sctp_ipsec_ipv6		= (TNET_SOCKET_TYPE_IPSEC | tnet_socket_type_sctp_ipv6),/**< SCTP/IPSec/IPv6 socket.*/

#define TNET_SOCKET_TYPE_IPV46				(TNET_SOCKET_TYPE_IPV4 | TNET_SOCKET_TYPE_IPV6)
	tnet_socket_type_udp_ipv46				= (TNET_SOCKET_TYPE_IPV46 | (tnet_socket_type_udp_ipv4 | tnet_socket_type_udp_ipv6)),	/**< UDP/IPv4/6 socket.*/
	tnet_socket_type_tcp_ipv46				= (TNET_SOCKET_TYPE_IPV46 | (tnet_socket_type_tcp_ipv4 | tnet_socket_type_tcp_ipv6)),	/**< TCP/IPv4/6 socket.*/
	tnet_socket_type_tls_ipv46				= (TNET_SOCKET_TYPE_IPV46 | (tnet_socket_type_tls_ipv4 | tnet_socket_type_tls_ipv6)),	/**< TLS/IPv4/6 socket.*/
	tnet_socket_type_sctp_ipv46				= (TNET_SOCKET_TYPE_IPV46 | (tnet_socket_type_sctp_ipv4 | tnet_socket_type_sctp_ipv6)),	/**< SCTP/IPv4/6 socket.*/
	tnet_socket_type_udp_ipsec_ipv46		= (TNET_SOCKET_TYPE_IPSEC | tnet_socket_type_udp_ipv46), /**< UDP/IPSec/IPv4/6 socket.*/
	tnet_socket_type_tcp_ipsec_ipv46		= (TNET_SOCKET_TYPE_IPSEC | tnet_socket_type_tcp_ipv46), /**< TCP/IPSec/IPv4/6 socket.*/
	tnet_socket_type_tls_ipsec_ipv46		= (TNET_SOCKET_TYPE_IPSEC | tnet_socket_type_tls_ipv46),	/**< TLS/IPSec/IPv4/6 socket.*/
	tnet_socket_type_sctp_ipsec_ipv46		= (TNET_SOCKET_TYPE_IPSEC | tnet_socket_type_sctp_ipv46),/**< SCTP/IPSec/IPv4/6 socket.*/
}
tnet_socket_type_t;


/**@def TNET_SOCKET_IS_VALID
* Checks the socket validity.
*/
#define TNET_SOCKET_IS_VALID(socket)		((socket) && (socket->type !=tnet_socket_type_invalid) && (socket)->fd >0)

#define TNET_SOCKET_TYPE_IS_STREAM(type)	( ((type & TNET_SOCKET_TYPE_UDP) !=  TNET_SOCKET_TYPE_UDP) )
#define TNET_SOCKET_TYPE_IS_DGRAM(type)		( ((type & TNET_SOCKET_TYPE_UDP) ==  TNET_SOCKET_TYPE_UDP) )

#define TNET_SOCKET_TYPE_IS_IPV4(type)		( ((type & TNET_SOCKET_TYPE_IPV4) ==  TNET_SOCKET_TYPE_IPV4) )
#define TNET_SOCKET_TYPE_IS_IPV6(type)		( ((type & TNET_SOCKET_TYPE_IPV6) ==  TNET_SOCKET_TYPE_IPV6) )
#define TNET_SOCKET_TYPE_IS_IPV46(type)		( TNET_SOCKET_TYPE_IS_IPV4(type) && TNET_SOCKET_TYPE_IS_IPV6(type) )

#define TNET_SOCKET_TYPE_IS_IPSEC(type)		( ((type & TNET_SOCKET_TYPE_IPSEC) ==  TNET_SOCKET_TYPE_IPSEC) )

#define TNET_SOCKET_TYPE_IS_UDP(type)		( ((type & TNET_SOCKET_TYPE_UDP) ==  TNET_SOCKET_TYPE_UDP) )
#define TNET_SOCKET_TYPE_IS_TCP(type)		( ((type & TNET_SOCKET_TYPE_TCP) ==  TNET_SOCKET_TYPE_TCP) )
#define TNET_SOCKET_TYPE_IS_TLS(type)		( ((type & TNET_SOCKET_TYPE_TLS) ==  TNET_SOCKET_TYPE_TLS) )
#define TNET_SOCKET_TYPE_IS_SCTP(type)		( ((type & TNET_SOCKET_TYPE_SCTP) ==  TNET_SOCKET_TYPE_SCTP) )

#define TNET_SOCKET_TYPE_IS_SECURE(type)	(TNET_SOCKET_TYPE_IS_IPSEC(type) || TNET_SOCKET_TYPE_IS_TLS(type) )

#define TNET_SOCKET_TYPE_UNSET(type, OP)		(type = TNET_SOCKET_TYPE_IS_##OP(type) ? type ^= TNET_SOCKET_TYPE_##OP : type)

#define TNET_SOCKET_TYPE_SET_IPV4(type)		(type |= TNET_SOCKET_TYPE_IPV4)
#define TNET_SOCKET_TYPE_SET_IPV4Only(type)	(type = TNET_SOCKET_TYPE_IS_IPV6(type) ? (type ^TNET_SOCKET_TYPE_IPV6)|TNET_SOCKET_TYPE_IPV4 : type)
#define TNET_SOCKET_TYPE_SET_IPV6(type)		(type |= TNET_SOCKET_TYPE_IPV6)
#define TNET_SOCKET_TYPE_SET_IPV6Only(type)	(type = TNET_SOCKET_TYPE_IS_IPV4(type) ? (type ^TNET_SOCKET_TYPE_IPV4)|TNET_SOCKET_TYPE_IPV6 : type)

#define TNET_SOCKET_TYPE_SET_IPSEC(type)	(type |=TNET_SOCKET_TYPE_IPSEC)

#define TNET_SOCKET_TYPE_SET_UDP(type)	(TNET_SOCKET_TYPE_UNSET(type,TCP), TNET_SOCKET_TYPE_UNSET(type,TLS), TNET_SOCKET_TYPE_UNSET(type,SCTP), type |=TNET_SOCKET_TYPE_UDP)
#define TNET_SOCKET_TYPE_SET_TCP(type)	(TNET_SOCKET_TYPE_UNSET(type,UDP), TNET_SOCKET_TYPE_UNSET(type,TLS), TNET_SOCKET_TYPE_UNSET(type,SCTP), type |=TNET_SOCKET_TYPE_TCP)
#define TNET_SOCKET_TYPE_SET_TLS(type)	(TNET_SOCKET_TYPE_UNSET(type,TCP), TNET_SOCKET_TYPE_UNSET(type,UDP), TNET_SOCKET_TYPE_UNSET(type,SCTP), type |=TNET_SOCKET_TYPE_TLS)
#define TNET_SOCKET_TYPE_SET_SCTP(type)	(TNET_SOCKET_TYPE_UNSET(type,TCP), TNET_SOCKET_TYPE_UNSET(type,TLS), TNET_SOCKET_TYPE_UNSET(type,UDP), type |=TNET_SOCKET_TYPE_SCTP)

/**
* @def TNET_SOCKET_HOST_ANY
* Any IPv4/IPv6 host.
*/
/**
* @def TNET_SOCKET_PORT_ANY
* Any port.
*/
#define TNET_SOCKET_HOST_ANY 0
#define TNET_SOCKET_PORT_ANY 0

/**
* Socket.
*/
typedef struct tnet_socket_s
{
	TSK_DECLARE_OBJECT;
	
	tnet_socket_type_t type;
	tnet_fd_t fd;
	tnet_ip_t ip;
	uint16_t port;

	tnet_tls_socket_handle_t* tlshandle;
}
tnet_socket_t;

typedef tnet_socket_t tnet_socket_udp_t; /**< UDP socket. */
typedef tnet_socket_t tnet_socket_tcp_t; /**< TCP socket. */
typedef tnet_socket_t tnet_socket_tls_t; /**< TLS socket. */
typedef tnet_socket_t tnet_socket_ipsec_t; /**< IPSec socket. */
typedef tsk_list_t tnet_sockets_L_t; /**< List of @ref tnet_socket_t elements. */

int tnet_socket_set_tlsfiles(tnet_socket_tls_t* socket, int isClient, const char* tlsfile_ca, const char* tlsfile_pvk, const char* tlsfile_pbk);

TINYNET_API tnet_socket_t* tnet_socket_create_2(const char*host, tnet_port_t port, tnet_socket_type_t type, tsk_bool_t nonblocking, tsk_bool_t bindsocket);
TINYNET_API tnet_socket_t* tnet_socket_create(const char* host, tnet_port_t port, tnet_socket_type_t type);

TINYNET_GEXTERN const tsk_object_def_t *tnet_socket_def_t;


TNET_END_DECLS

#endif /* TNET_SOCKET_H */

