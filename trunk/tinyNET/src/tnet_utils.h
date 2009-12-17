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

typedef int32_t tnet_fd_t;
typedef uint16_t tnet_port_t;
typedef char tnet_host_t[NI_MAXHOST];
typedef char tnet_ip_t[INET6_ADDRSTRLEN];

#ifdef _WIN32_WCE
typedef TCHAR tnet_error_t[512];
#else
typedef char tnet_error_t[512];
#endif

TINYNET_API void tnet_getlasterror(tnet_error_t *error);

TINYNET_API int tnet_getaddrinfo(const char *node, const char *service, const struct addrinfo *hints,  struct addrinfo **res);
TINYNET_API void tnet_freeaddrinfo(struct addrinfo *ai);
TINYNET_API int tnet_get_ip_n_port(tnet_fd_t fd, tnet_ip_t *ip, tnet_port_t *port);
#define tnet_get_ip(fd, ip) tnet_get_ip_n_port(fd, ip, 0)
#define tnet_get_port(fd, port) tnet_get_ip_n_port(fd, 0, port)

TINYNET_API int tnet_getnameinfo(const struct sockaddr *sa, socklen_t salen, char* node, socklen_t nodelen, char* service, socklen_t servicelen, int flags);
TINYNET_API int tnet_gethostname(tnet_host_t* result);

TINYNET_API int tnet_sockaddrinfo_init(const char *host, tnet_port_t port, enum tnet_socket_type_e type, struct sockaddr_storage *ai_addr, int *ai_family, int *ai_socktype, int *ai_protocol);
TINYNET_API int tnet_sockaddr_init(const char *host, tnet_port_t port, enum tnet_socket_type_e type, struct sockaddr_storage *addr);
TINYNET_API int tnet_sockfd_init(const char *host, tnet_port_t port, enum tnet_socket_type_e type, tnet_fd_t *fd);

TINYNET_API int tnet_sockfd_close(tnet_fd_t *fd);

#define TNET_PRINT_LAST_ERROR() \
	{ \
		tnet_error_t error; \
		tnet_getlasterror(&error); \
		TSK_DEBUG_ERROR("Network error --> %s", error); \
	}

#endif /* TNET_UTILS_H */