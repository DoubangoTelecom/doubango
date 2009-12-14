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

#include "tnet_socket.h"

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
	memset(*error, 0, sizeof(*error));

#if TNET_UNDER_WINDOWS
	{
		int err  = WSAGetLastError();
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
	memcpy(*error, "Unknown error.", sizeof(Unknown error));
#endif
}


/**
 * @fn	int32_t tnet_getaddrinfo(const char *node, const char *service,
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
int32_t tnet_getaddrinfo(const char *node, const char *service, const struct addrinfo *hints,  struct addrinfo **res)
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

int tnet_getPort(int32_t fd, uint16_t *port)
{
	*port = 0;

	if(fd > 0)
	{
		int status;
		struct sockaddr_storage ss;
		//struct hostent *hp;
		struct sockaddr_in *sin;
		struct sockaddr_in6 *sin6;
		socklen_t namelen = sizeof(ss);
		if(status = getsockname(fd, (struct sockaddr*)&ss, &namelen))
		{
			TSK_DEBUG_ERROR("GETSOCKNAME has failed with status code: %d", status);
			return -1;
		}
		switch(ss.ss_family)
		{
		case AF_INET:
			{
				sin = ((struct sockaddr_in*)&ss);
				*port = ntohs(sin->sin_port);
				//hp = gethostbyaddr(&(sin->sin_addr), sizeof(sin->sin_addr), ss.ss_family);
				break;
			}

		case AF_INET6:
			{
				sin6 = ((struct sockaddr_in6*)&ss);
				*port = ntohs(sin6->sin6_port);
				//hp = gethostbyaddr(&(sin6->sin6_addr), sizeof(sin6->sin6_addr), ss.ss_family);
				break;
			}

		default:
			{
				TSK_DEBUG_ERROR("Unsupported address family.");
				return -1;
			}
			
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

int tnet_gethostname(tnet_hostname_t* result)
{
	return gethostname(*result, sizeof(*result));
}

int tnet_sockaddr_init(const char *host, uint16_t port, tnet_socket_type_t type, struct sockaddr *addr)
{
	int status;
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
	if(status = tnet_getaddrinfo(host, p, &hints, &result))
	{
		TNET_PRINT_LAST_ERROR();
		goto bail;
	}
	
	/* Find our address. */
	for(ptr = result; ptr; ptr = ptr->ai_next)
	{
		if(ptr->ai_family == hints.ai_family && ptr->ai_socktype == hints.ai_socktype && ptr->ai_protocol == hints.ai_protocol)
		{
			memcpy(addr, ptr->ai_addr, ptr->ai_addrlen);
			break;
		}
	}

bail:
	tnet_freeaddrinfo(result);

	return status;
}