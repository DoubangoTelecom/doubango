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

/**@file tnet_utils.h
 * @brief Network utilities functions.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TNET_UTILS_H
#define TNET_UTILS_H

#include "tinyNET_config.h"

#if TNET_UNDER_WINDOWS
#	include	<winsock2.h>
#	include	<ws2tcpip.h>
#else
#	include <sys/types.h>
#	include <sys/socket.h>
#	include <netinet/in.h>
#	include <arpa/inet.h>
#	include <netdb.h> 
#endif

typedef char tnet_hostname_t[/*NI_MAXHOST*/255];
#ifdef _WIN32_WCE
typedef TCHAR tnet_error_t[512];
#else
typedef char tnet_error_t[512];
#endif

TINYNET_API void tnet_getlasterror(tnet_error_t *error);

TINYNET_API int32_t tnet_getaddrinfo(const char *node, const char *service, const struct addrinfo *hints,  struct addrinfo **res);
TINYNET_API void tnet_freeaddrinfo(struct addrinfo *ai);
TINYNET_API int tnet_getPort(int32_t fd, uint16_t *port);

TINYNET_API int tnet_getnameinfo(const struct sockaddr *sa, socklen_t salen, char* node, socklen_t nodelen, char* service, socklen_t servicelen, int flags);
TINYNET_API int tnet_gethostname(tnet_hostname_t* result);

TINYNET_API int tnet_sockaddr_init(const char *host, uint16_t port, enum tnet_socket_type_e type, struct sockaddr *addr);

#define TNET_PRINT_LAST_ERROR() \
	{ \
		tnet_error_t error; \
		tnet_getlasterror(&error); \
		TSK_DEBUG_ERROR("Network error --> %s", error); \
	}

#endif /* TNET_UTILS_H */