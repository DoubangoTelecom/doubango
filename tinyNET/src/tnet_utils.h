/*
* Copyright (C) 2010-2015 Mamadou DIOP.
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

/**@file tnet_utils.h
 * @brief Network utility functions.
 *
 */
#ifndef TNET_UTILS_H
#define TNET_UTILS_H

#include "tinynet_config.h"

#include "tnet_socket.h"
#include "tnet_types.h"

TNET_BEGIN_DECLS

/**@ingroup tnet_utils_group
*/
#if !defined(TNET_CONNECT_TIMEOUT)
#	define TNET_CONNECT_TIMEOUT		2000
#endif /* TNET_CONNECT_TIMEOUT */

/**Interface.
*/
typedef struct tnet_interface_s {
    TSK_DECLARE_OBJECT;

    unsigned index;

    char* description;
    uint8_t* mac_address;
    tsk_size_t mac_address_length;
}
tnet_interface_t;


/**Address.
*/
typedef struct tnet_address_s {
    TSK_DECLARE_OBJECT;

    tnet_family_t family;

    unsigned unicast:1;
    unsigned anycast:1;
    unsigned multicast:1;
    unsigned dnsserver:1;

    char* ip;
}
tnet_address_t;

TINYNET_API void tnet_getlasterror(tnet_error_t *error);
TINYNET_API int tnet_geterrno();

TINYNET_API tnet_interfaces_L_t* tnet_get_interfaces();
TINYNET_API tnet_addresses_L_t* tnet_get_addresses(tnet_family_t family, tsk_bool_t unicast, tsk_bool_t anycast, tsk_bool_t multicast, tsk_bool_t dnsserver, long if_index);
#define tnet_get_addresses_all()			tnet_get_addresses(AF_UNSPEC, 1, 1, 1, 1, -1)
#define tnet_get_addresses_all_unicast()	tnet_get_addresses(AF_UNSPEC, 1, 0, 0, 0, -1)
#define tnet_get_addresses_unicast4()		tnet_get_addresses(AF_INET, 1, 0, 0, 0, -1)
#define tnet_get_addresses_unicast6()		tnet_get_addresses(AF_INET6, 1, 0, 0, 0, -1)
#define tnet_get_addresses_all_anycast()	tnet_get_addresses(AF_UNSPEC, 0, 1, 0, 0, -1)
#define tnet_get_addresses_anycast4()		tnet_get_addresses(AF_INET, 0, 1, 0, 0, -1)
#define tnet_get_addresses_anycast6()		tnet_get_addresses(AF_INET6, 0, 1, 0, 0, -1)
#define tnet_get_addresses_all_multicast()	tnet_get_addresses(AF_UNSPEC, 0, 0, 1, 0, -1)
#define tnet_get_addresses_multicast4()		tnet_get_addresses(AF_INET, 0, 0, 1, 0, -1)
#define tnet_get_addresses_multicast6()		tnet_get_addresses(AF_INET6, 0, 0, 1, 0, -1)
#define tnet_get_addresses_all_dnsservers()	tnet_get_addresses(AF_UNSPEC, 0, 0, 0, 1, -1)
#define tnet_get_addresses_dnsservers4()	tnet_get_addresses(AF_INET, 0, 0, 0, 1, -1)
#define tnet_get_addresses_dnsservers6()	tnet_get_addresses(AF_INET6, 0, 0, 0, 1, -1)
TINYNET_API int tnet_get_mac_address(tnet_mac_address* address);

TINYNET_API int tnet_getbestsource(const char* destination, tnet_port_t port, tnet_socket_type_t type, tnet_ip_t *source);
TINYNET_API int tnet_getaddrinfo(const char *node, const char *service, const struct addrinfo *hints,  struct addrinfo **res);
TINYNET_API void tnet_freeaddrinfo(struct addrinfo *ai);
TINYNET_API int tnet_getsockname(tnet_fd_t fd, struct sockaddr_storage *result);
TINYNET_API int tnet_getpeername(tnet_fd_t fd, struct sockaddr_storage *result);
TINYNET_API tnet_family_t tnet_get_family(const char* host, tnet_port_t port);
TINYNET_API tnet_socket_type_t tnet_get_type(const char* host, tnet_port_t port);
TINYNET_API tsk_bool_t tnet_is_ipv6(const char* host, tnet_port_t port);
TINYNET_API int tnet_get_ip_n_port(tnet_fd_t fd, tsk_bool_t getlocal, tnet_ip_t *ip, tnet_port_t *port);
TINYNET_API int tnet_get_sockip_n_port(const struct sockaddr *addr, tnet_ip_t *ip, tnet_port_t *port);
TINYNET_API int tnet_get_peerip_n_port(tnet_fd_t localFD, tnet_ip_t *ip, tnet_port_t *port);
#define tnet_get_ip(fd, getlocal, ip) tnet_get_ip_n_port(fd, getlocal, ip, 0)
#define tnet_get_port(fd, getlocal, port) tnet_get_ip_n_port(fd, getlocal, tsk_null, port)
#define tnet_get_sockip(addr, ip) tnet_get_sockip_n_port(addr, ip, 0)
#define tnet_get_sockport(addr, port) tnet_get_sockip_n_port(addr, 0, port)
#define tnet_get_peerip(localFD, ip) tnet_get_peerip_n_port(localFD, ip, 0)
#define tnet_get_peerport(localFD, port) tnet_get_peerip_n_port(localFD, 0, port)

TINYNET_API tsk_bool_t tnet_is_loopback(const struct sockaddr *sa);
TINYNET_API tsk_bool_t tnet_is_linklocal(const struct sockaddr *sa);

#if TNET_HAVE_SA_LEN
#	define tnet_get_sockaddr_size(psockaddr)	(psockaddr)->sa_len
#else
#	define tnet_get_sockaddr_size(psockaddr)	((psockaddr)->sa_family == AF_INET6 ? sizeof(struct sockaddr_in6): ((psockaddr)->sa_family == AF_INET ? sizeof(struct sockaddr_in) : sizeof(*(psockaddr))))
#endif

#if TNET_UNDER_WINDOWS
#	define tnet_ioctlt ioctlsocket /* FIXME: use WSAIoctl */
#	define tnet_soccket(family, type, protocol) WSASocket((family), (type), (protocol), NULL, 0, WSA_FLAG_OVERLAPPED)
#else
#	define tnet_ioctlt ioctl
#	define tnet_soccket(family, type, protocol) socket((family), (type), (protocol))
#endif

TINYNET_API int tnet_get_fd_max_allowed(tsk_size_t* size);
TINYNET_API int tnet_set_fd_max_allowed(tsk_size_t size);
TINYNET_API int tnet_get_fd_opened_count(tsk_size_t* count);

TINYNET_API int tnet_getnameinfo(const struct sockaddr *sa, socklen_t salen, char* node, socklen_t nodelen, char* service, socklen_t servicelen, int flags);
TINYNET_API int tnet_gethostname(tnet_host_t* result);

TINYNET_API int tnet_inet_pton(int af, const char* src, void* dst);
TINYNET_API const char *tnet_inet_ntop(int af, const void *src, char * dst, int size);

TINYNET_API int tnet_sockfd_waitUntil(tnet_fd_t fd, long timeout, tsk_bool_t writable);
#define tnet_sockfd_waitUntilWritable(fd, timeout) tnet_sockfd_waitUntil(fd, timeout, tsk_true)
#define tnet_sockfd_waitUntilReadable(fd, timeout) tnet_sockfd_waitUntil(fd, timeout, tsk_false)
TINYNET_API int tnet_sockfd_joingroup6(tnet_fd_t fd, const char* multiaddr, unsigned iface_index);
TINYNET_API int tnet_sockfd_leavegroup6(tnet_fd_t fd, const char* multiaddr, unsigned iface_index);

TINYNET_API int tnet_resolve(const char *fqdn, tnet_port_t port, tnet_socket_type_t type, tnet_ip_t* out_ip, tnet_port_t* out_port);

TINYNET_API int tnet_sockaddrinfo_init(const char *host, tnet_port_t port, tnet_socket_type_t type, struct sockaddr_storage *ai_addr, int *ai_family, int *ai_socktype, int *ai_protocol);
TINYNET_API int tnet_sockaddr_init(const char *host, tnet_port_t port, tnet_socket_type_t type, struct sockaddr_storage *addr);
TINYNET_API int tnet_sockfd_init(const char *host, tnet_port_t port, tnet_socket_type_t type, tnet_fd_t *fd);

TINYNET_API int tnet_sockfd_set_mode(tnet_fd_t fd, int nonBlocking);
TINYNET_API int tnet_sockfd_reuseaddr(tnet_fd_t fd, int reuseAddr);
#define tnet_sockfd_set_nonblocking(fd)	tnet_sockfd_set_mode(fd, 1)
#define tnet_sockfd_set_blocking(fd)	tnet_sockfd_set_mode(fd, 0)

TINYNET_API int tnet_sockfd_sendto(tnet_fd_t fd, const struct sockaddr *to, const void* buf, tsk_size_t size);
TINYNET_API int tnet_sockfd_recvfrom(tnet_fd_t fd, void* buf, tsk_size_t size, int flags, struct sockaddr *from);
TINYNET_API tsk_size_t tnet_sockfd_send(tnet_fd_t fd, const void* buf, tsk_size_t size, int flags);
TINYNET_API int tnet_sockfd_recv(tnet_fd_t fd, void* buf, tsk_size_t size, int flags);
TINYNET_API int tnet_sockfd_connectto(tnet_fd_t fd, const struct sockaddr_storage *to);
TINYNET_API int tnet_sockfd_listen(tnet_fd_t fd, int backlog);
TINYNET_API tnet_fd_t tnet_sockfd_accept(tnet_fd_t fd, struct sockaddr *addr, socklen_t *addrlen);

TINYNET_API int tnet_sockfd_close(tnet_fd_t *fd);
TINYNET_API int tnet_sockfd_shutdown(tnet_fd_t fd);

TINYNET_API tnet_proxy_type_t tnet_proxy_type_from_string(const char* type);
TINYNET_API const char* tnet_proxy_type_to_string(tnet_proxy_type_t type);

/**Prints last network error to @b stderr.
*/
#define TNET_PRINT_LAST_ERROR(FMT, ...) \
	{ \
		tnet_error_t error; \
		tnet_getlasterror(&error); \
		TSK_DEBUG_ERROR(FMT, ##__VA_ARGS__); \
		TSK_DEBUG_ERROR("(SYSTEM)NETWORK ERROR ==>%s", error) \
	}


tnet_interface_t* tnet_interface_create(const char* description, const void* mac_address, tsk_size_t mac_address_length);
tnet_address_t* tnet_address_create(const char* ip);

TINYNET_GEXTERN const tsk_object_def_t *tnet_interface_def_t;
TINYNET_GEXTERN const tsk_object_def_t *tnet_address_def_t;

TNET_END_DECLS

#endif /* TNET_UTILS_H */


