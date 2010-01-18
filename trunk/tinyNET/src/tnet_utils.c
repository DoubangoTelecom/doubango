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

/**@file tnet_utils.c
 * @brief Network utilities functions.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */

#include "tnet_utils.h"

#include "tsk_debug.h"
#include "tsk_string.h"
#include "tsk_memory.h"

#include "tnet_socket.h"

#include <string.h>

/**
 * @fn	void tnet_getlasterror(tnet_error_t *error)
 *
 * @brief	Gets last network error description.
 *
 * @author	Mamadou
 * @date	12/12/2009
 *
 * @param [out]	error	The short description of the last network error. 
**/
void tnet_getlasterror(tnet_error_t *error)
{
	int err  = tnet_geterrno();
	memset(*error, 0, sizeof(*error));

#if TNET_UNDER_WINDOWS
	{
#ifdef _WIN32_WCE
		FormatMessage
#else
		FormatMessageA
#endif
		(
		  FORMAT_MESSAGE_FROM_SYSTEM, 
		  0,
		  err,
		  0,
		  *error, 
		  sizeof(*error)-1,
		  0);
	}
#else
	//strerror(errno);
	memcpy(*error, "Unknown error.", sizeof("Unknown error"));
#endif
}

int tnet_geterrno()
{
#if TNET_UNDER_WINDOWS
	return WSAGetLastError();
#else
	return errno;
#endif
}

/**
 * @fn	int tnet_getaddrinfo(const char *node, const char *service,
 * 		const struct addrinfo *hints, struct addrinfo **res)
 *
 * @brief	converts human-readable text strings representing hostnames or IP addresses into a dynamically allocated linked list of struct addrinfo structures. 
 *			You MUST call @ref tnet_freeaddrinfo() function to free the result.
 *
 * @author	Mamadou
 * @date	12/11/2009
 *
 * @param [in]	node	A pointer to a NULL-terminated ANSI string that contains a host (node) name or a numeric host address string. For the Internet protocol, the numeric host address string is a dotted-decimal IPv4 address or an IPv6 hex address.. 
 * @param [in]	service	A pointer to a NULL-terminated ANSI string that contains either a service name or port number represented as a string. 
 * @param [in]	hints	A pointer to an addrinfo structure that provides hints about the type of socket the caller supports. 
 * @param [out]	res		A pointer to a linked list of one or more addrinfo structures that contains response information about the host. 
 *
 * @return	Success returns zero. Failure returns a nonzero error code.
**/
int tnet_getaddrinfo(const char *node, const char *service, const struct addrinfo *hints,  struct addrinfo **res)
{
	return getaddrinfo(node, service, hints, res);
}

/**
 * @fn	void tnet_freeaddrinfo(struct addrinfo *ai)
 *
 * @brief	This function frees address information previously allocated using @ref tnet_getaddrinfo.
 *
 * @author	Mamadou
 * @date	12/11/2009
 *
 * @param [in]	The address information to free. 
**/
void tnet_freeaddrinfo(struct addrinfo *ai)
{
	freeaddrinfo(ai);
}

int tnet_get_sockaddr(tnet_fd_t fd, struct sockaddr_storage *result)
{
	if(fd >0)
	{
		socklen_t namelen = sizeof(*result);
		return getsockname(fd, (struct sockaddr*)result, &namelen);
	}
	return -1;
}

tnet_socket_type_t tnet_get_socket_type(tnet_fd_t fd)
{
	tnet_socket_type_t type = tnet_socket_type_invalid;
	
	/*if(fd >0)
	{
		struct sockaddr_storage ss;
		if(!tnet_get_sockaddr(fd, &ss))
		{
			if(((struct sockaddr *)&ss)->sa_family == AF_INET)
			{
				TNET_SOCKET_TYPE_AS_IPV4(type);
			}
			else if(((struct sockaddr *)&ss)->sa_family == AF_INET6)
			{
				TNET_SOCKET_TYPE_AS_IPV6(type);
			}
		}
	}*/

	return type;
}

int tnet_get_ip_n_port(tnet_fd_t fd, tnet_ip_t *ip, tnet_port_t *port)
{
	*port = 0;

	if(fd > 0)
	{
		int status;
		struct sockaddr_storage ss;
		if((status = tnet_get_sockaddr(fd, &ss)))
		{
			TSK_DEBUG_ERROR("TNET_GET_SOCKADDR has failed with status code: %d", status);
			return -1;
		}
		
		if(((struct sockaddr *)&ss)->sa_family == AF_INET)
		{
			struct sockaddr_in *sin = ((struct sockaddr_in*)&ss);
			if(port)
			{
				*port = ntohs(sin->sin_port);
			}
			if(ip)
			{
				if((status = tnet_getnameinfo((struct sockaddr*)sin, sizeof(*sin), *ip, sizeof(*ip), 0, 0, NI_NUMERICHOST)))
				{
					return status;
				}
			}
		}
		else if(((struct sockaddr *)&ss)->sa_family == AF_INET6)
		{
			struct sockaddr_in6 *sin6 = ((struct sockaddr_in6*)&ss);
			if(port)
			{
				*port = ntohs(sin6->sin6_port);
			}
			if(ip)
			{
				if((status = tnet_getnameinfo((struct sockaddr*)sin6, sizeof(*sin6), *ip, sizeof(*ip), 0, 0, NI_NUMERICHOST)))
				{
					return status;
				}
			}
		}
		else
		{
			TSK_DEBUG_ERROR("Unsupported address family.");
			return -1;
		}
		
		return 0;
	}
	TSK_DEBUG_ERROR("Could not use an invalid socket description.");
	return -1;
}

int tnet_getnameinfo(const struct sockaddr *sa, socklen_t salen, char* node, socklen_t nodelen, char* service, socklen_t servicelen, int flags)
{
	return getnameinfo(sa, salen, node, nodelen, service, servicelen, flags);
}

int tnet_gethostname(tnet_host_t* result)
{
	return gethostname(*result, sizeof(*result));
}

int tnet_sockaddrinfo_init(const char *host, tnet_port_t port, enum tnet_socket_type_e type, struct sockaddr_storage *ai_addr, int *ai_family, int *ai_socktype, int *ai_protocol)
{
	int status = 0;
	struct addrinfo *result = 0;
	struct addrinfo *ptr = 0;
	struct addrinfo hints;
	tsk_istr_t p;

	tsk_itoa(port, &p);

	/* hints address info structure */
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = TNET_SOCKET_TYPE_IS_IPV6(type) ? AF_INET6 : AF_INET;
	hints.ai_socktype = TNET_SOCKET_TYPE_IS_STREAM(type) ? SOCK_STREAM : SOCK_DGRAM;
	hints.ai_protocol = TNET_SOCKET_TYPE_IS_STREAM(type) ? IPPROTO_TCP : IPPROTO_UDP;
	hints.ai_flags = AI_PASSIVE; /* Bind to the local machine. */

	/* Performs getaddrinfo */
	if((status = tnet_getaddrinfo(host, p, &hints, &result)))
	{
		TNET_PRINT_LAST_ERROR();
		goto bail;
	}
	
	/* Find our address. */
	for(ptr = result; ptr; ptr = ptr->ai_next)
	{
		if(ptr->ai_family == hints.ai_family && ptr->ai_socktype == hints.ai_socktype && ptr->ai_protocol == hints.ai_protocol)
		{
			/* duplicate addrinfo ==> Bad idea
			*ai = tsk_calloc(1, sizeof (struct addrinfo));
			memcpy (*ai, ptr, sizeof (struct addrinfo));
			(*ai)->ai_addr = tsk_calloc(1, ptr->ai_addrlen);
			memcpy((*ai)->ai_addr, ptr->ai_addr, ptr->ai_addrlen);
			(*ai)->ai_addrlen = ptr->ai_addrlen;
			(*ai)->ai_next = 0;
			(*ai)->ai_canonname = 0;*/

			if(ai_addr)memcpy(ai_addr, ptr->ai_addr, ptr->ai_addrlen);
			if(ai_family) *ai_family = ptr->ai_family;
			if(ai_socktype) *ai_socktype = ptr->ai_socktype;
			if(ai_protocol) *ai_protocol = ptr->ai_protocol;
			
			break;
		}
	}

bail:
	tnet_freeaddrinfo(result);

	return status;
}

int tnet_sockaddr_init(const char *host, tnet_port_t port, tnet_socket_type_t type, struct sockaddr_storage *addr)
{
	int status;
	struct sockaddr_storage ai_addr;

	if((status = tnet_sockaddrinfo_init(host, port, type, &ai_addr, 0, 0, 0)))
	{
		return status;
	}
	
	memcpy(addr, &ai_addr, sizeof(ai_addr));

	return status;
}

int tnet_sockfd_init(const char *host, tnet_port_t port, enum tnet_socket_type_e type, tnet_fd_t *fd)
{
	int status = -1;
	struct sockaddr_storage ai_addr;
	int ai_family, ai_socktype, ai_protocol;
	*fd = TNET_INVALID_SOCKET;
	
	if((status = tnet_sockaddrinfo_init(host, port, type, &ai_addr, &ai_family, &ai_socktype, &ai_protocol)))
	{
		goto bail;
	}
	
	if((*fd = socket(ai_family, ai_socktype, ai_protocol)) == TNET_INVALID_SOCKET)
	{
		TNET_PRINT_LAST_ERROR();
		goto bail;
	}

#if TNET_USE_POLL
	if((status = tnet_sockfd_set_nonblocking(*fd)))
	{
		goto bail;
	}
#endif
	
#if TNET_HAVE_SS_LEN
	if((status = bind(*fd, (const struct sockaddr*)&ai_addr, ai_addr.ss_len)))
#else
	if((status = bind(*fd, (const struct sockaddr*)&ai_addr, sizeof(ai_addr))))
#endif
	{
		TNET_PRINT_LAST_ERROR();
		tnet_sockfd_close(fd);
		
		goto bail;
	}
	
bail:
	return (*fd == TNET_INVALID_SOCKET) ? status : 0;
}


int tnet_sockfd_set_mode(tnet_fd_t fd, int nonBlocking)
{
	if(fd != TNET_INVALID_FD)
	{
#if TNET_UNDER_WINDOWS
	ULONG mode = nonBlocking;
	if(ioctlsocket(fd, FIONBIO, &mode))
	//if(WSAIoctl(fd, FIONBIO, &nonblocking, sizeof(nonblocking), NULL, 0, NULL, NULL, NULL) == SOCKET_ERROR)
	{
		TNET_PRINT_LAST_ERROR();
		return -1;
	}
#else
	int flags;
    if((flags = fcntl(fd, F_GETFL, 0)) < 0) 
	{ 
		TNET_PRINT_LAST_ERROR();
		return -1;
	} 
	if(fcntl(fd, F_SETFL, flags | (nonBlocking ? O_NONBLOCK : O_BLOCK)) < 0)
	{ 
		TNET_PRINT_LAST_ERROR();
		return -1;
	} 
#endif

	// int on = 1;
	// ioctl(fd, FIONBIO, (char *)&on);

	}
	return 0;
}

int tnet_sockfd_sendto(tnet_fd_t fd, const struct sockaddr *to, const void* buf, size_t size)
{
	if(fd == TNET_INVALID_FD)
	{
		TSK_DEBUG_ERROR("Using invalid FD to send data.");
		return -1;
	}

#if TNET_HAVE_SS_LEN
	return sendto(fd, buf, size, 0, to, to->ss_len);
#else
	return sendto(fd, buf, size, 0, to, sizeof(*to));
#endif
}

int tnet_sockfd_close(tnet_fd_t *fd)
{
	int ret;
#if TNET_UNDER_WINDOWS
	ret = closesocket(*fd);
#else
	ret = close(*fd);
#endif

	*fd = TNET_INVALID_SOCKET;
	return ret;
}

