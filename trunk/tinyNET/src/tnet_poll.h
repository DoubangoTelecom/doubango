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

/**@file tnet_poll.h
 * @brief poll() method implementation for multiplexing network sockets.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TNET_POLL_H
#define TNET_POLL_H

#include "tinyNET_config.h"

TNET_BEGIN_DECLS

#if TNET_USE_POLL

#include "tnet_types.h"

typedef unsigned long tnet_nfds_t;

#if TNET_HAVE_POLL

typedef struct pollfd tnet_pollfd_t;

#define TNET_POLLRDNORM  POLLRDNORM 
#define TNET_POLLRDBAND  POLLRDBAND
#define TNET_POLLIN      POLLIN
#define TNET_POLLPRI     POLLPRI

#define TNET_POLLWRNORM  POLLWRNORM
#define TNET_POLLOUT     POLLOUT
#define TNET_POLLWRBAND  POLLWRBAND

#define TNET_POLLERR     POLLERR
#define TNET_POLLHUP     POLLHUP
#define TNET_POLLNVAL    POLLNVAL

#if TNET_UNDER_WINDOWS
#	define tnet_poll	WSAPoll
#else
#	define tnet_poll	poll
#endif /* TNET_UNDER_WINDOWS */

#else

typedef struct tnet_pollfd_s
{
    tnet_fd_t  fd;
    short   events;
    short   revents;
}
tnet_pollfd_t;

#define TNET_POLLRDNORM  0x0100
#define TNET_POLLRDBAND  0x0200
#define TNET_POLLIN      (TNET_POLLRDNORM | TNET_POLLRDBAND)
#define TNET_POLLPRI     0x0400

#define TNET_POLLWRNORM  0x0010
#define TNET_POLLOUT     (TNET_POLLWRNORM)
#define TNET_POLLWRBAND  0x0020

#define TNET_POLLERR     0x0001
#define TNET_POLLHUP     0x0002
#define TNET_POLLNVAL    0x0004

int tnet_poll(tnet_pollfd_t fds[ ], tnet_nfds_t nfds, int timeout);

#endif /* TNET_HAVE_POLL */

#endif /* TNET_USE_POLL */

TNET_END_DECLS

#endif /* TNET_POLL_H */
