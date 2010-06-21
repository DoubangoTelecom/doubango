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

/**@file tdav_session_msrp.h
 * @brief The Message Session Relay Protocol (MSRP) session.
 * Used for both Message (RFC 4975) and file transfer (RFC 5547).
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */

#ifndef TINYDAV_SESSION_MSRP_H
#define TINYDAV_SESSION_MSRP_H

#include "tinydav_config.h"

#include "tinymedia/tmedia_session.h"

TDAV_BEGIN_DECLS

// Forward declaration
struct trtp_manager_s;
struct tdav_consumer_msrp_s;

typedef struct tdav_session_msrp_s
{
	TMEDIA_DECLARE_SESSION_MSRP;

	tsk_bool_t useIPv6;

	char* local_ip;

	char* remote_ip;
	uint16_t remote_port;

	struct tmedia_consumer_s* consumer;
	struct tmedia_producer_s* producer;
}
tdav_session_msrp_t;


TINYDAV_GEXTERN const tmedia_session_plugin_def_t *tdav_session_msrp_plugin_def_t;

TDAV_END_DECLS

#endif /* TINYDAV_SESSION_MSRP_H */
