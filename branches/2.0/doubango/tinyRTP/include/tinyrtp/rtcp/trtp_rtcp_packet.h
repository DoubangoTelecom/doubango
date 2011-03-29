/*
* Copyright (C) 2010-2011 Mamadou Diop.
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
#ifndef TINYRTP_RTCP_PACKET_H
#define TINYRTP_RTCP_PACKET_H

#include "tinyrtp_config.h"

#include "tsk_list.h"

TRTP_BEGIN_DECLS

#define TRTP_RTCP_PACKET(self)	((trtp_rtcp_packet_t*)(self))

// RFC 3550 12.1 RTCP Packet Types
typedef enum trtp_rtcp_packet_type_e
{
	trtp_rtcp_packet_type_sr = 200,
	trtp_rtcp_packet_type_rr = 201,
	trtp_rtcp_packet_type_sdes = 202,
	trtp_rtcp_packet_type_bye = 203,
	trtp_rtcp_packet_type_app = 204,
}
trtp_rtcp_packet_type_t;

typedef struct trtp_rtcp_packet_s
{
	TSK_DECLARE_OBJECT;

	struct trtp_rtcp_header_s *header;
}
trtp_rtcp_packet_t;

#define TRTP_DECLARE_RTCP_PACKET trtp_rtcp_packet_t __packet__
typedef tsk_list_t trtp_rtcp_packets_L_t; /**< List of @ref trtp_rtcp_packet_t elements */

TINYRTP_API int trtp_rtcp_packet_init(trtp_rtcp_packet_t* self, uint8_t version, uint8_t padding, uint8_t rc, trtp_rtcp_packet_type_t type, uint16_t length);
TINYRTP_API int trtp_rtcp_packet_set_length(trtp_rtcp_packet_t* self, uint16_t length);
TINYRTP_API int trtp_rtcp_packet_deinit(trtp_rtcp_packet_t* self);

TRTP_END_DECLS

#endif /* TINYRTP_RTCP_PACKET_H */
