/*
* Copyright (C) 2010-2011 Mamadou Diop.
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
 */

#ifndef TINYDAV_SESSION_MSRP_H
#define TINYDAV_SESSION_MSRP_H

#include "tinydav_config.h"

#include "tinymsrp/session/tmsrp_sender.h"
#include "tinymsrp/session/tmsrp_receiver.h"
#include "tmsrp.h"

#include "tnet_transport.h"

#include "tinymedia/tmedia_session.h"

TDAV_BEGIN_DECLS

typedef enum tdav_msrp_setup_e
{
	msrp_setup_active,
	msrp_setup_passive,
	msrp_setup_actpass,
	msrp_setup_holdconn
}
tdav_msrp_setup_t;

typedef enum tdav_msrp_dir_e
{
	tdav_msrp_dir_none = 0x00,
	tdav_msrp_dir_sendonly = 0x01 << 0,
	tdav_msrp_dir_recvonly = 0x01 << 1,
	tdav_msrp_dir_sendrecv = (tdav_msrp_dir_sendonly | tdav_msrp_dir_recvonly),
}
tdav_msrp_dir_t;

typedef struct tdav_session_msrp_s
{
	TMEDIA_DECLARE_SESSION_MSRP;

	tsk_bool_t useIPv6;

	tnet_transport_t *transport;
	tmsrp_config_t* config;
	tdav_msrp_setup_t setup;
	tnet_fd_t connectedFD; // FullDuplex Socket
	tmsrp_sender_t* sender;
	tmsrp_receiver_t* receiver;

	char* local_ip;
	//uint16_t local_port;

	/* NAT Traversal context */
	struct tnet_nat_ctx_s* natt_ctx;

	char* remote_ip;
	uint16_t remote_port;

	tdav_msrp_dir_t dir;
	char* neg_accept_type;
	char* neg_accept_w_type;
	char* accept_types;
	char* accept_w_types;
	uint64_t chunck_duration;

	struct {
		char* path; //full-path
		char* selector;
		char* disposition;
		char* date;
		char* icon;
		char* transfer_id;
		unsigned sent:1;
	} file;
	
	unsigned fresh_conn:1;
	unsigned offerer:1;
	unsigned send_bodiless:1;
}
tdav_session_msrp_t;

TINYDAV_GEXTERN const tmedia_session_plugin_def_t *tdav_session_msrp_plugin_def_t;

TDAV_END_DECLS

#endif /* TINYDAV_SESSION_MSRP_H */
