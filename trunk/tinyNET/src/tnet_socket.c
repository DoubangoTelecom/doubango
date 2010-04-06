/*
* Copyright (C) 2009 Mamadou Diop.
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

#include "tnet_socket.h"

#include "tnet_utils.h"

#include "tsk_string.h"
#include "tsk_debug.h"

#include <string.h>

/**@defgroup tnet_socket_group Protocol agnostic socket.
* Protocol agnostic BSD/Windows sockets.
*/

/**@ingroup tnet_socket_group
 * 	Closes a socket.
 * @param sock The socket to close. 
 * @retval	Zero if succeed and nonzero error code otherwise. 
**/
int tnet_socket_close(tnet_socket_t *sock)
{
	return tnet_sockfd_close(&(sock->fd));	
}

int tnet_socket_set_tlsfiles(tnet_socket_tls_t* socket, int isClient, const char* tlsfile_ca, const char* tlsfile_pvk, const char* tlsfile_pbk)
{
	if(socket){
		return -1;
	}
	
	if(!TNET_SOCKET_TYPE_IS_TLS(socket->type)){
		TSK_DEBUG_ERROR("Not TLS socket.");
		return -2;
	}

	if(socket->tlshandle){
		TSK_DEBUG_ERROR("TLS files already set.");
		return -3;
	}
	
	if((socket->tlshandle = tnet_sockfd_set_tlsfiles(socket->fd, isClient, tlsfile_ca, tlsfile_pvk, tlsfile_pbk))){
		return 0;
	}
	else{
		return -4;
	}
}

//=================================================================================================
//	SOCKET object definition
//
static void* tnet_socket_create(void * self, va_list * app)
{
	tnet_socket_t *sock = self;
	if(sock)
	{
		int status;
		int nonblocking;
		int bindsocket;
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
		nonblocking = va_arg(*app, int);
		bindsocket = va_arg(*app, int);

		memset(local_hostname, 0, sizeof(local_hostname));

		/* Get the local host name */
		if(host != TNET_SOCKET_HOST_ANY && !tsk_strempty(host)){
			memcpy(local_hostname, host, strlen(host)>sizeof(local_hostname)-1 ? sizeof(local_hostname)-1 : strlen(host));
		}
		else
		{
			if(TNET_SOCKET_TYPE_IS_IPV6(sock->type)){
				memcpy(local_hostname, "::", 2);
			}
			else{
				memcpy(local_hostname, "0.0.0.0", 7);
			}
			//if((status = tnet_gethostname(&local_hostname)))
			//{
			//	TNET_PRINT_LAST_ERROR("gethostname have failed.");
			//	goto bail;
			//}
		}

		/* hints address info structure */
		memset(&hints, 0, sizeof(hints));
		hints.ai_family = TNET_SOCKET_TYPE_IS_IPV46(sock->type) ? AF_UNSPEC : (TNET_SOCKET_TYPE_IS_IPV6(sock->type) ? AF_INET6 : AF_INET);
		hints.ai_socktype = TNET_SOCKET_TYPE_IS_STREAM(sock->type) ? SOCK_STREAM : SOCK_DGRAM;
		hints.ai_protocol = TNET_SOCKET_TYPE_IS_STREAM(sock->type) ? IPPROTO_TCP : IPPROTO_UDP;
		hints.ai_flags = AI_PASSIVE
#if !TNET_UNDER_WINDOWS || _WIN32_WINNT>=0x600
			| AI_ADDRCONFIG
#endif
			;

		/* Performs getaddrinfo */
		if((status = tnet_getaddrinfo(local_hostname, port, &hints, &result))){
			TNET_PRINT_LAST_ERROR("getaddrinfo have failed.");
			goto bail;
		}
		
		/* Find our address. */
		for(ptr = result; ptr; ptr = ptr->ai_next)
		{
			sock->fd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
			if(ptr->ai_family != AF_INET6 && ptr->ai_family != AF_INET){
				continue;
			}
			
			if(bindsocket){
				/* Bind the socket */
				if((status = bind(sock->fd, ptr->ai_addr, ptr->ai_addrlen))){
					TNET_PRINT_LAST_ERROR("bind have failed.");
					tnet_socket_close(sock);
					continue;
				}

				/* Get local IP string. */
				if(status = tnet_get_ip_n_port(sock->fd , &sock->ip, &sock->port)) /* % */
				//if((status = tnet_getnameinfo(ptr->ai_addr, ptr->ai_addrlen, sock->ip, sizeof(sock->ip), 0, 0, NI_NUMERICHOST)))
				{
					TNET_PRINT_LAST_ERROR("Failed to get local IP and port.");
					tnet_socket_close(sock);
					continue;
				}
//				else{
//#if TNET_UNDER_WINDOWS
//					int index;
//					if((index = tsk_strindexOf(sock->ip, strlen(sock->ip), "%")) > 0){
//						*(sock->ip + index) = '\0';
//					}
//#endif
//				}
			}

			/* sets the real socket type (if ipv46) */
			if(ptr->ai_family == AF_INET6){
				TNET_SOCKET_TYPE_SET_IPV6Only(sock->type);
			}
			else{
				TNET_SOCKET_TYPE_SET_IPV4Only(sock->type);
			}
			break;
		}
		
		/* Check socket validity. */
		if(!TNET_SOCKET_IS_VALID(sock)) 
		{
			TNET_PRINT_LAST_ERROR("Invalid socket.");
			goto bail;
		}		

		/* To avoid "Address already in use" error */
		{
#if defined(SOLARIS)
			char yes = '1';
#else
			int yes = 1;
#endif
			if(setsockopt(sock->fd, SOL_SOCKET, SO_REUSEADDR, (char*)&yes, sizeof(int))){
				TNET_PRINT_LAST_ERROR("setsockopt(SO_REUSEADDR) have failed.");
			}
		}

		/* Sets the socket to nonblocking mode */
		if(nonblocking){
			if((status = tnet_sockfd_set_nonblocking(sock->fd))){
				goto bail;
			}
		}

bail:
		/* Free addrinfo */
		tnet_freeaddrinfo(result);

		/* Close socket if failed. */
		if(status){
			tnet_socket_close(sock);
		}

	}
	return self;
}

static void* tnet_socket_destroy(void * self)
{ 
	tnet_socket_t *sock = self;
	
	if(sock){
		/* Close the socket. */
		if(sock->fd > 0){
			tnet_socket_close(sock);
		}
		/* Clean up TLS handle*/
		if(sock->tlshandle){
			TSK_OBJECT_SAFE_FREE(sock->tlshandle);
		}
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


