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

/**@file tnet.h
 * @brief Protocol agnostic socket.
 *
 */

#include "tnet_socket.h"

#include "tnet_utils.h"

#include "tsk_string.h"
#include "tsk_debug.h"

#include <string.h>

/**@defgroup tnet_socket_group Protocol agnostic socket
*
*
* <h2>10.1	Sockets</h2>
* For performance reason, all sockets created using tinyNET are non-blocking by default.
* The newly created socket will be automatically bound to associate it with an IP address and port number. @ref tnet_socket_create() function is used to create and bind a
* non-blocking socket. You should use @ref tnet_socket_create_2() function to control whether the socket should be bound or not. The same function is used to force the stack to create a blocking socket.<br>
* A socket object is defined like this:<br>
*
* @code
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
* @endcode
* To create a socket:
* @code
// (create udp ipv4 or ipv6 socket)
tnet_socket_t* socket = tnet_socket_create(
TNET_SOCKET_HOST_ANY, // local ip address/hostname to bind to
TNET_SOCKET_PORT_ANY, // local port number to bind to
tnet_socket_type_udp_ipv46 // the socket type (IPv4 or IPv6)
);
// TNET_SOCKET_HOST_ANY --> bind to "0.0.0.0" or "::"
// TNET_SOCKET_PORT_ANY --> bind to any available port
* @endcode

* <b>TNET_SOCKET_TYPE_IS_*()</b> macros are used to determine:
* -	The socket type (stream, dgram),
* -	The socket protocol (udp, tcp, tls, sctp, ipsec),
* -	The IP version (ipv6, ipv4),
* -	…
* <br>
* A socket is a well-defined object and should be destroyed using @a TSK_DECLARE_SAFE_FREE() macro.
* A socket will be automatically closed when destroyed.
*
*/

static int tnet_socket_close(tnet_socket_t *sock);

/**@ingroup tnet_socket_group
* Creates a new socket.
* To check that the returned socket is valid use @ref TNET_SOCKET_IS_VALID function.
* @param host FQDN (e.g. www.doubango.org) or IPv4/IPv6 IP string.
* @param port The local/remote port used to receive/send data. Set the port value to @ref TNET_SOCKET_PORT_ANY to bind to a random port.
* @param type The type of the socket. See @ref tnet_socket_type_t.
* @param nonblocking Indicates whether to create non-blocking socket.
* @param bindsocket Indicates whether to bind the newly created socket or not.
* @retval @ref tnet_socket_t object.
* @sa @ref tnet_socket_create.
*/
tnet_socket_t* tnet_socket_create_2(const char* host, tnet_port_t port_, tnet_socket_type_t type, tsk_bool_t nonblocking, tsk_bool_t bindsocket)
{
    tnet_socket_t *sock;
    if ((sock = tsk_object_new(tnet_socket_def_t))) {
        int status;
        tsk_istr_t port;
        struct addrinfo *result = tsk_null;
        struct addrinfo *ptr = tsk_null;
        struct addrinfo hints;
        tnet_host_t local_hostname;

        sock->port = port_;
        tsk_itoa(sock->port, &port);
        sock->type = type;

        memset(local_hostname, 0, sizeof(local_hostname));

        /* Get the local host name */
        if (host != TNET_SOCKET_HOST_ANY && !tsk_strempty(host)) {
            memcpy(local_hostname, host, tsk_strlen(host) > sizeof(local_hostname) - 1 ? sizeof(local_hostname) - 1 : tsk_strlen(host));
        }
        else {
            if (TNET_SOCKET_TYPE_IS_IPV6(sock->type)) {
                memcpy(local_hostname, "::", 2);
            }
            else {
                memcpy(local_hostname, "0.0.0.0", 7);
            }
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
        if ((status = tnet_getaddrinfo(local_hostname, port, &hints, &result))) {
            TNET_PRINT_LAST_ERROR("tnet_getaddrinfo(family=%d, hostname=%s and port=%s) failed: [%s]",
                                  hints.ai_family, local_hostname, port, tnet_gai_strerror(status));
            goto bail;
        }

        /* Find our address. */
        for (ptr = result; ptr; ptr = ptr->ai_next) {
            sock->fd = (tnet_fd_t)tnet_soccket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
            if (ptr->ai_family != AF_INET6 && ptr->ai_family != AF_INET) {
                continue;
            }
            /* To avoid "Address already in use" error
            * Check issue 368 (https://code.google.com/p/doubango/issues/detail?id=368) to understand why it's not used for UDP/DTLS.
            */
            //
            if (TNET_SOCKET_TYPE_IS_STREAM(sock->type)) {
                if ((status = tnet_sockfd_reuseaddr(sock->fd, 1))) {
                    // do not break...continue
                }
            }

            if (bindsocket) {
                /* Bind the socket */
                if ((status = bind(sock->fd, ptr->ai_addr, (int)ptr->ai_addrlen))) {
                    TNET_PRINT_LAST_ERROR("bind to [%s:%s]have failed", local_hostname, port);
                    tnet_socket_close(sock);
                    continue;
                }

                /* Get local IP string. */
                if ((status = tnet_get_ip_n_port(sock->fd, tsk_true/*local*/, &sock->ip, &sock->port))) /* % */
                    //if((status = tnet_getnameinfo(ptr->ai_addr, ptr->ai_addrlen, sock->ip, sizeof(sock->ip), 0, 0, NI_NUMERICHOST)))
                {
                    TNET_PRINT_LAST_ERROR("Failed to get local IP and port.");
                    tnet_socket_close(sock);
                    continue;
                }
            }

            /* sets the real socket type (if ipv46) */
            if (ptr->ai_family == AF_INET6) {
                TNET_SOCKET_TYPE_SET_IPV6Only(sock->type);
            }
            else {
                TNET_SOCKET_TYPE_SET_IPV4Only(sock->type);
            }
            break;
        }

        /* Check socket validity. */
        if (!TNET_SOCKET_IS_VALID(sock)) {
            TNET_PRINT_LAST_ERROR("Invalid socket.");
            goto bail;
        }

#if TNET_UNDER_IPHONE || TNET_UNDER_IPHONE_SIMULATOR
        /* disable SIGPIPE signal */
        {
            int yes = 1;
            if (setsockopt(sock->fd, SOL_SOCKET, SO_NOSIGPIPE, (char*)&yes, sizeof(int))) {
                TNET_PRINT_LAST_ERROR("setsockopt(SO_NOSIGPIPE) have failed.");
            }
        }
#endif /* TNET_UNDER_IPHONE */

        /* Sets the socket to nonblocking mode */
        if(nonblocking) {
            if((status = tnet_sockfd_set_nonblocking(sock->fd))) {
                goto bail;
            }
        }

bail:
        /* Free addrinfo */
        tnet_freeaddrinfo(result);

        /* Close socket if failed. */
        if (status) {
            if (TNET_SOCKET_IS_VALID(sock)) {
                tnet_socket_close(sock);
            }
            return tsk_null;
        }
    }

    return sock;
}

/**@ingroup tnet_socket_group
* Creates a non-blocking socket and bind it.
* To check that the returned socket is valid use @ref TNET_SOCKET_IS_VALID function.
* @param host FQDN (e.g. www.doubango.org) or IPv4/IPv6 IP string.
* @param port The local/remote port used to receive/send data. Set the port value to @ref TNET_SOCKET_PORT_ANY to bind to a random port.
* @param type The type of the socket. See @ref tnet_socket_type_t.
* @retval @ref tnet_socket_t object.
*/
tnet_socket_t* tnet_socket_create(const char* host, tnet_port_t port, tnet_socket_type_t type)
{
    return tnet_socket_create_2(host, port, type, tsk_true, tsk_true);
}

/**@ingroup tnet_socket_group
* Returns the number of bytes sent (or negative value on error)
*/
int tnet_socket_send_stream(tnet_socket_t* self, const void* data, tsk_size_t size)
{
    if (!self || self->fd == TNET_INVALID_FD || !data || !size || !TNET_SOCKET_TYPE_IS_STREAM(self->type)) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if (self->tlshandle && (TNET_SOCKET_TYPE_IS_TLS(self->type) || TNET_SOCKET_TYPE_IS_WSS(self->type))) {
        return tnet_tls_socket_send(self->tlshandle, data, size) == 0 ? (int)size : -1; // returns zero on success
    }

    return (int)tnet_sockfd_send(self->fd, data, size, 0);
}

/**@ingroup tnet_socket_group
 * @retval	Zero if succeed and nonzero error code otherwise.
 */
int tnet_socket_handle_brokenpipe(tnet_socket_t* self)
{
    int ret;
    tnet_fd_t fd_old, fd_new;
    if (!self || !TNET_SOCKET_TYPE_IS_DGRAM(self->type)) { // Must be UDP
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    fd_old = self->fd;
    fd_new = TNET_INVALID_FD;

    // close old fd
    ret = tnet_sockfd_close(&self->fd);
    // try to create an fd binding to the same address
    if ((ret = tnet_sockfd_init(self->ip, self->port, self->type, &fd_new)) != 0) {
        TNET_PRINT_LAST_ERROR("Find to bind to %s:%d", self->ip, self->port);
        // TODO: Create completly new socket?
        return ret;
    }
#if TNET_UNDER_IPHONE || TNET_UNDER_IPHONE_SIMULATOR
    /* disable SIGPIPE signal */
    {
        int yes = 1;
        if (setsockopt(fd_new, SOL_SOCKET, SO_NOSIGPIPE, (char*)&yes, sizeof(int))) {
            TNET_PRINT_LAST_ERROR("setsockopt(%d, SO_NOSIGPIPE) have failed", fd_new);
        }
    }
#endif /* TNET_UNDER_IPHONE || TNET_UNDER_IPHONE_SIMULATOR */
    TSK_DEBUG_INFO("Broken pipe result for {%s:%d}: %d -> %d", self->ip, self->port, fd_old, fd_new);
    self->fd = fd_new;
    return 0;
}

/**@ingroup tnet_socket_group
 * 	Closes a socket.
 * @param sock The socket to close.
 * @retval	Zero if succeed and nonzero error code otherwise.
 **/
static int tnet_socket_close(tnet_socket_t *sock)
{
    return tnet_sockfd_close(&(sock->fd));
}

//=================================================================================================
//	SOCKET object definition
//
static tsk_object_t* tnet_socket_ctor(tsk_object_t * self, va_list * app)
{
    tnet_socket_t *sock = self;
    if (sock) {
    }
    return self;
}

static tsk_object_t* tnet_socket_dtor(tsk_object_t * self)
{
    tnet_socket_t *sock = self;

    if (sock) {
        /* Close the socket */
        if (TNET_SOCKET_IS_VALID(sock)) {
            tnet_socket_close(sock);
        }
        /* Clean up TLS handle */
        TSK_OBJECT_SAFE_FREE(sock->tlshandle);

        /* Clean up DTLS handle */
        TSK_OBJECT_SAFE_FREE(sock->dtlshandle);
    }

    return self;
}

static const tsk_object_def_t tnet_socket_def_s = {
    sizeof(tnet_socket_t),
    tnet_socket_ctor,
    tnet_socket_dtor,
    tsk_null,
};
const tsk_object_def_t *tnet_socket_def_t = &tnet_socket_def_s;


