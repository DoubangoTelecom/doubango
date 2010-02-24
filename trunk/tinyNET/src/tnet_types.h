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
* GNU General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

/**@file tnet_types.h
 * @brief ????.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TNET_TYPES_H
#define TNET_TYPES_H

#include "tinyNET_config.h"

#if TNET_UNDER_WINDOWS
#	include	<winsock2.h>
#	include	<ws2tcpip.h>
#	include <iphlpapi.h>
#else
#	include <sys/types.h>
#	include <sys/socket.h>
#	include <sys/select.h>
#	include <netinet/in.h>
#	include <arpa/inet.h>
#	include <netdb.h>
#	include <fcntl.h>
#	include <sys/ioctl.h>
#	include <unistd.h>
#	include <net/if.h>
#	if HAVE_IFADDRS
#		include <ifaddrs.h>
#	endif
#	if TNET_HAVE_POLL
#		include <poll.h>
#	endif /* TNET_HAVE_POLL */
#endif

#if defined(TNET_HAVE_SCTP)
#include <netinet/sctp.h>
#endif

#include "tsk_errno.h"

TNET_BEGIN_DECLS

typedef int32_t tnet_fd_t;
typedef uint16_t tnet_port_t;
typedef int32_t tnet_family_t;
typedef char tnet_host_t[NI_MAXHOST];
typedef char tnet_ip_t[INET6_ADDRSTRLEN];


#if TNET_UNDER_WINDOWS
#	define TNET_INVALID_SOCKET				INVALID_SOCKET
#	define TNET_ERROR_WOULDBLOCK			WSAEWOULDBLOCK
#	define TNET_ERROR_INPROGRESS			WSAEINPROGRESS
#	define TNET_ERROR_CONNRESET				WSAECONNRESET
#	define TNET_ERROR_INTR					WSAEINTR
#	if defined(_WIN32_WCE)
#		define tnet_gai_strerror(...)		"FIXME"
#	else
#		define tnet_gai_strerror			gai_strerrorA
#	endif
#else
#	define TNET_INVALID_SOCKET				-1
#	define TNET_ERROR_WOULDBLOCK			EWOULDBLOCK
#	define TNET_ERROR_INPROGRESS			EINPROGRESS
#	define TNET_ERROR_CONNRESET				ECONNRESET
#	define TNET_ERROR_INTR					EINTR
#	define tnet_gai_strerror				gai_strerror
#endif
#define TNET_INVALID_FD				TNET_INVALID_SOCKET

#ifdef _WIN32_WCE
typedef TCHAR tnet_error_t[512];
#else
typedef char tnet_error_t[512];
#endif

TNET_END_DECLS

#endif /* TNET_TYPES_H */


