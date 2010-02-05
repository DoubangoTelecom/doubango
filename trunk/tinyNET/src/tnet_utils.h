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

#include "tnet_socket.h"
#include "tnet_types.h"

#include "tsk_list.h"

TNET_BEGIN_DECLS

#define TNET_INTERFACE_CREATE(description, mac_address, mac_address_length)	tsk_object_new(tnet_interface_def_t, (const char*)description, (const uint8_t*)mac_address, (size_t)mac_address_length)
#define TNET_ADDRESS_CREATE(ip)												tsk_object_new(tnet_address_def_t, (const char*)ip)

typedef struct tnet_interface_s
{
	TSK_DECLARE_OBJECT;
	
	char* description;
	uint8_t* mac_address;
	size_t mac_address_length;
}
tnet_interface_t;
typedef tsk_list_t tnet_interfaces_L_t;

typedef struct tnet_address_s
{
	TSK_DECLARE_OBJECT;

	tnet_family_t family;

	unsigned unicast:1;
	unsigned anycast:1;
	unsigned multicast:1;
	unsigned dnsserver:1;

	char* ip;
}
tnet_address_t;
typedef tsk_list_t tnet_addresses_L_t;

TINYNET_API void tnet_getlasterror(tnet_error_t *error);
TINYNET_API int tnet_geterrno();

TINYNET_API tnet_interfaces_L_t* tnet_get_interfaces();
TINYNET_API tnet_addresses_L_t* tnet_get_addresses(tnet_family_t family, unsigned unicast, unsigned anycast, unsigned multicast, unsigned dnsserver);
#define tnet_get_addresses_all()			tnet_get_addresses(AF_UNSPEC, 1, 1, 1, 1)
#define tnet_get_addresses_all_unicast()	tnet_get_addresses(AF_UNSPEC, 1, 0, 0, 0)
#define tnet_get_addresses_unicast4()		tnet_get_addresses(AF_INET, 1, 0, 0, 0)
#define tnet_get_addresses_unicast6()		tnet_get_addresses(AF_INET6, 1, 0, 0, 0)
#define tnet_get_addresses_all_anycast()	tnet_get_addresses(AF_UNSPEC, 0, 1, 0, 0)
#define tnet_get_addresses_anycast4()		tnet_get_addresses(AF_INET, 0, 1, 0, 0)
#define tnet_get_addresses_anycast6()		tnet_get_addresses(AF_INET6, 0, 1, 0, 0)
#define tnet_get_addresses_all_multicast()	tnet_get_addresses(AF_UNSPEC, 0, 0, 1, 0)
#define tnet_get_addresses_multicast4()		tnet_get_addresses(AF_INET, 0, 0, 1, 0)
#define tnet_get_addresses_multicast6()		tnet_get_addresses(AF_INET6, 0, 0, 1, 0)
#define tnet_get_addresses_all_dnsservers()	tnet_get_addresses(AF_UNSPEC, 0, 0, 0, 1)
#define tnet_get_addresses_dnsservers4()	tnet_get_addresses(AF_INET, 0, 0, 0, 1)
#define tnet_get_addresses_dnsservers6()	tnet_get_addresses(AF_INET6, 0, 0, 0, 1)

TINYNET_API int tnet_getaddrinfo(const char *node, const char *service, const struct addrinfo *hints,  struct addrinfo **res);
TINYNET_API void tnet_freeaddrinfo(struct addrinfo *ai);
TINYNET_API int tnet_get_sockaddr(tnet_fd_t fd, struct sockaddr_storage *result);
TINYNET_API tnet_socket_type_t tnet_get_socket_type(tnet_fd_t fd);
TINYNET_API int tnet_get_ip_n_port(tnet_fd_t fd, tnet_ip_t *ip, tnet_port_t *port);
TINYNET_API int tnet_get_sockip_n_port(struct sockaddr *addr, tnet_ip_t *ip, tnet_port_t *port);
#define tnet_get_ip(fd, ip) tnet_get_ip_n_port(fd, ip, 0)
#define tnet_get_port(fd, port) tnet_get_ip_n_port(fd, 0, port)
#define tnet_get_sockip(addr, ip) tnet_get_sockip_n_port(addr, ip, 0)
#define tnet_get_sockport(addr, port) tnet_get_sockip_n_port(addr, 0, port)

TINYNET_API int tnet_getnameinfo(const struct sockaddr *sa, socklen_t salen, char* node, socklen_t nodelen, char* service, socklen_t servicelen, int flags);
TINYNET_API int tnet_gethostname(tnet_host_t* result);

TINYNET_API int tnet_sockaddrinfo_init(const char *host, tnet_port_t port, tnet_socket_type_t type, struct sockaddr_storage *ai_addr, int *ai_family, int *ai_socktype, int *ai_protocol);
TINYNET_API int tnet_sockaddr_init(const char *host, tnet_port_t port, tnet_socket_type_t type, struct sockaddr_storage *addr);
TINYNET_API int tnet_sockfd_init(const char *host, tnet_port_t port, tnet_socket_type_t type, tnet_fd_t *fd);

TINYNET_API int tnet_sockfd_set_mode(tnet_fd_t fd, int nonBlocking);
#define tnet_sockfd_set_nonblocking(fd)	tnet_sockfd_set_mode(fd, 1)
#define tnet_sockfd_set_blocking(fd)	tnet_sockfd_set_mode(fd, 0)

TINYNET_API int tnet_sockfd_sendto(tnet_fd_t fd, const struct sockaddr *to, const void* buf, size_t size);
TINYNET_API int tnet_sockfd_recvfrom(tnet_fd_t fd, void* buf, size_t size, int flags, struct sockaddr *from);
TINYNET_API int tnet_sockfd_send(tnet_fd_t fd, void* buf, size_t size, int flags);
TINYNET_API int tnet_sockfd_recv(tnet_fd_t fd, void* buf, size_t size, int flags);

TINYNET_API int tnet_sockfd_close(tnet_fd_t *fd);

#define TNET_PRINT_LAST_ERROR() \
	{ \
		tnet_error_t error; \
		tnet_getlasterror(&error); \
		TSK_DEBUG_ERROR("Network error --> %s", error); \
	}


#if TSK_UNDER_WINDOWS
#	define tnet_ioctlt ioctlsocket /* FIXME: use WSAIoctl */
#else
#	define tnet_ioctlt ioctl
#endif


TINYNET_GEXTERN const void *tnet_interface_def_t;
TINYNET_GEXTERN const void *tnet_address_def_t;

TNET_END_DECLS

#endif /* TNET_UTILS_H */


