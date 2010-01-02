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

/**@file tnet.h
 * @brief Protocol agnostic socket.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */

#include "tnet_socket.h"

#include "tnet_utils.h"

#include "tsk_string.h"
#include "tsk_debug.h"


/**
 * @fn	int tnet_socket_close(const tnet_socket_t *socket)
 *
 * @brief	Closes a socket. 
 *
 * @author	Mamadou
 * @date	12/11/2009
 *
 * @param [in]	sock	If non-null, the socket. 
 *
 * @return	Zero if succeed and nonzero error code otherwise. 
**/
int tnet_socket_close(tnet_socket_t *sock)
{
	return tnet_sockfd_close(&(sock->fd));	
}


int tnet_socket_stream_connectto(tnet_socket_tcp_t *sock, const char* host, tnet_port_t port)
{
	int status = 0;
	tsk_istr_t _port;
	struct addrinfo *result = 0;
	struct addrinfo *ptr = 0;
	struct addrinfo hints;

	if(!TNET_SOCKET_IS_VALID(sock))
	{
		TSK_DEBUG_ERROR("Could not connect invalid socket.");
		return -1;
	}

	tsk_itoa(port, &_port);

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = TNET_SOCKET_TYPE_IS_IPV6(sock->type) ? AF_INET6 : AF_INET;
	hints.ai_socktype = TNET_SOCKET_TYPE_IS_STREAM(sock->type) ? SOCK_STREAM : SOCK_DGRAM;
	hints.ai_protocol = TNET_SOCKET_TYPE_IS_STREAM(sock->type) ? IPPROTO_TCP : IPPROTO_UDP;

	
	if((status = tnet_getaddrinfo(host, _port, &hints, &result)))
	{
		TNET_PRINT_LAST_ERROR();
		goto bail;
	}

	for(ptr = result; ptr; ptr = ptr->ai_next) 
	{
		if(ptr->ai_family == hints.ai_family && ptr->ai_socktype == hints.ai_socktype && ptr->ai_protocol == hints.ai_protocol)
		{
			if((status = connect(sock->fd, ptr->ai_addr, ptr->ai_addrlen)))
			{
				TNET_PRINT_LAST_ERROR();
				goto bail;
			}

			TSK_DEBUG_INFO("Successfuly connected to: %s on port %d\n", host, port);
			break;
		}
	}

bail:
	tnet_freeaddrinfo(result);
	return status;
}


int tnet_socket_dgram_sendto(tnet_socket_tcp_t *socket, const struct sockaddr *to, const void* buf, size_t size)
{
	if(!TNET_SOCKET_IS_VALID(socket))
	{
		TSK_DEBUG_ERROR("Using invalid socket to send data.");
		return -1;
	}

	return sendto(socket->fd, buf, size, 0, to, TNET_SOCKET_TYPE_IS_IPV6(socket->type) ? sizeof(struct sockaddr_in6) : sizeof(struct sockaddr_in));
}




//========================================================
//	String object definition
//
static void* tnet_socket_create(void * self, va_list * app)
{
	tnet_socket_t *sock = self;
	if(sock)
	{
		int status;
		tsk_istr_t port;
		struct addrinfo *result = 0;
		struct addrinfo *ptr = 0;
		struct addrinfo hints;
		tnet_host_t local_hostname;

		const char *host = va_arg(*app, const char*);
#if defined(__GNUC__)
		sock->port = (tnet_port_t)va_arg(*app, unsigned);
#else
		sock->port = va_arg(*app, tnet_port_t);
#endif
		tsk_itoa(sock->port, &port);
		sock->type = va_arg(*app, tnet_socket_type_t);

		/* Get the local host name */
		if(host != TNET_SOCKET_HOST_ANY && !tsk_strempty(host))
		{
			memset(local_hostname, 0, sizeof(local_hostname));
			memcpy(local_hostname, host, strlen(host)>sizeof(local_hostname)-1 ? sizeof(local_hostname)-1 : strlen(host));
		}
		else
		{
			if((status = tnet_gethostname(&local_hostname)))
			{
				TNET_PRINT_LAST_ERROR();
				goto bail;
			}
		}

		/* hints address info structure */
		memset(&hints, 0, sizeof(hints));
		hints.ai_family = TNET_SOCKET_TYPE_IS_IPV6(sock->type) ? AF_INET6 : AF_INET;
		hints.ai_socktype = TNET_SOCKET_TYPE_IS_STREAM(sock->type) ? SOCK_STREAM : SOCK_DGRAM;
		hints.ai_protocol = TNET_SOCKET_TYPE_IS_STREAM(sock->type) ? IPPROTO_TCP : IPPROTO_UDP;
		hints.ai_flags = AI_PASSIVE; /* Bind to the local machine. */

		/* Performs getaddrinfo */
		if((status = tnet_getaddrinfo(local_hostname, port, &hints, &result)))
		{
			TNET_PRINT_LAST_ERROR();
			goto bail;
		}
		
		/* Find our address. */
		for(ptr = result; ptr; ptr = ptr->ai_next)
		{
			//if(ptr->ai_family == hints.ai_family && ptr->ai_socktype == hints.ai_socktype && ptr->ai_protocol == hints.ai_protocol)
			{
				sock->fd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
				/* Get local IP string. */
				if((status = tnet_getnameinfo(ptr->ai_addr, ptr->ai_addrlen, sock->ip, sizeof(sock->ip), 0, 0, NI_NUMERICHOST)))
				{
					TNET_PRINT_LAST_ERROR();
					tnet_socket_close(sock);
					continue;
				}
				else
				{
					if((status = bind(sock->fd, ptr->ai_addr, ptr->ai_addrlen)))
					{
						TNET_PRINT_LAST_ERROR();
						tnet_socket_close(sock);
						continue;
					}
					else
					{
						if(sock->port == TNET_SOCKET_PORT_ANY && (status = tnet_get_port(sock->fd, &(sock->port))))
						{
							TNET_PRINT_LAST_ERROR();
							tnet_socket_close(sock);
							continue;
						}
						else break;
					}
				}
			}
		}
		
		/* Check socket validity. */
		if(!TNET_SOCKET_IS_VALID(sock)) 
		{
			TNET_PRINT_LAST_ERROR();
			goto bail;
		}		

		/* To avoid "Address already in use" error */
		{
#if defined(SOLARIS)
			char yes = '1';
#else
			int yes = 1;
#endif
			if(setsockopt(sock->fd, SOL_SOCKET, SO_REUSEADDR, (char*)&yes, sizeof(int)))
			{
				TNET_PRINT_LAST_ERROR();
			}
		}

		/* Sets the socket to nonblocking mode */
#if TNET_USE_POLL
		if((status = tnet_sockfd_set_nonblocking(sock->fd)))
		{
			goto bail;
		}
#endif

bail:
		/* Free addrinfo */
		tnet_freeaddrinfo(result);

		/* Close socket if failed. */
		if(status)
		{
			tnet_socket_close(sock);
		}

	}
	return self;
}

static void* tnet_socket_destroy(void * self)
{ 
	tnet_socket_t *sock = self;
	if(sock && sock->fd > 0)
	{
		/* Close the socket. */
		tnet_socket_close(sock);
	}

	return self;
}

static const tsk_object_def_t tnet_socket_def_s = 
{
	sizeof(tnet_socket_t),
	tnet_socket_create, 
	tnet_socket_destroy,
	0, 
};
const void *tnet_socket_def_t = &tnet_socket_def_s;

