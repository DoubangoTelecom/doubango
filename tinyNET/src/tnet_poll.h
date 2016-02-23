/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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

/**@file tnet_poll.h
 * @brief poll() method implementation for multiplexing network sockets.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef TNET_POLL_H
#define TNET_POLL_H

#include "tinynet_config.h"

TNET_BEGIN_DECLS

#if USE_POLL

#include "tnet_types.h"

typedef unsigned long tnet_nfds_t;

#if HAVE_POLL

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

typedef struct tnet_pollfd_s {
    tnet_fd_t  fd;
    short   events;
    short   revents;
}
tnet_pollfd_t;

#define TNET_POLLIN      0x0001    /* There is data to read */
#define TNET_POLLPRI     0x0002    /* There is urgent data to read */
#define TNET_POLLOUT     0x0004    /* Writing now will not block */
#define TNET_POLLERR     0x0008    /* Error condition */
#define TNET_POLLHUP     0x0010    /* Hung up */
#define TNET_POLLNVAL    0x0020    /* Invalid request: fd not open */

int tnet_poll(tnet_pollfd_t fds[ ], tnet_nfds_t nfds, int timeout);

#endif /* HAVE_POLL */

#endif /* TNET_USE_POLL */

TNET_END_DECLS

#endif /* TNET_POLL_H */

