/*
* Copyright (C) 2012 Doubango Telecom <http://www.doubango.org>
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

#include "tsk_buffer.h"
#include "tsk_list.h"

TRTP_BEGIN_DECLS

#define TRTP_RTCP_PACKET(self)	((trtp_rtcp_packet_t*)(self))
#define TRTP_DECLARE_RTCP_PACKET trtp_rtcp_packet_t __packet__

// RFC 3550 12.1 RTCP Packet Types
// RFC 4585
// RFC 5104 (FIXME: not supported yet!)
typedef enum trtp_rtcp_packet_type_e {
    trtp_rtcp_packet_type_sr = 200,
    trtp_rtcp_packet_type_rr = 201,
    trtp_rtcp_packet_type_sdes = 202,
    trtp_rtcp_packet_type_bye = 203,
    trtp_rtcp_packet_type_app = 204,
    trtp_rtcp_packet_type_rtpfb = 205,
    trtp_rtcp_packet_type_psfb = 206
}
trtp_rtcp_packet_type_t;

typedef struct trtp_rtcp_packet_s {
    TSK_DECLARE_OBJECT;

    struct trtp_rtcp_header_s *header;
}
trtp_rtcp_packet_t;

typedef tsk_list_t trtp_rtcp_packets_L_t; /**< List of @ref trtp_rtcp_packet_t elements */

trtp_rtcp_packet_t* trtp_rtcp_packet_create(struct trtp_rtcp_header_s* header);
int trtp_rtcp_packet_init(trtp_rtcp_packet_t* self, uint8_t version, uint8_t padding, uint8_t rc, trtp_rtcp_packet_type_t type, uint16_t length_in_bytes);
trtp_rtcp_packet_t* trtp_rtcp_packet_deserialize(const void* data, tsk_size_t size);
int trtp_rtcp_packet_serialize_to(const trtp_rtcp_packet_t* self, void* data, tsk_size_t size);
tsk_buffer_t* trtp_rtcp_packet_serialize(const trtp_rtcp_packet_t* self, tsk_size_t num_bytes_pad);
int trtp_rtcp_packet_add_packet(trtp_rtcp_packet_t* self, trtp_rtcp_packet_t* packet, tsk_bool_t front);
TINYRTP_API const trtp_rtcp_packet_t* trtp_rtcp_packet_get_at(const trtp_rtcp_packet_t* self, trtp_rtcp_packet_type_t type, tsk_size_t index);
TINYRTP_API const trtp_rtcp_packet_t* trtp_rtcp_packet_get(const trtp_rtcp_packet_t* self, trtp_rtcp_packet_type_t type);
tsk_size_t trtp_rtcp_packet_get_size(const trtp_rtcp_packet_t* self);

int trtp_rtcp_packet_deinit(trtp_rtcp_packet_t* self);

TRTP_END_DECLS

#endif /* TINYRTP_RTCP_PACKET_H */
