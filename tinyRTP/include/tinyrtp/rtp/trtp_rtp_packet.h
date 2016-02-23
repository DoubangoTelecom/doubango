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
/**@file trtp_rtp_packet.h
 * @brief RTP packet.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 */
#ifndef TINYRTP_RTP_PACKET_H
#define TINYRTP_RTP_PACKET_H

#include "tinyrtp_config.h"

#include "tinyrtp/rtp/trtp_rtp_header.h"

#include "tsk_object.h"

TRTP_BEGIN_DECLS


typedef struct trtp_rtp_packet_s {
    TSK_DECLARE_OBJECT;

    trtp_rtp_header_t* header;

    struct {
        void* data;
        const void* data_const; // never free()d. an alternative to "data"
        tsk_size_t size;
    } payload;

    /* extension header as per RFC 3550 section 5.3.1 */
    struct {
        void* data;
        tsk_size_t size; /* contains the first two 16-bit fields */
    } extension;
}
trtp_rtp_packet_t;
typedef tsk_list_t trtp_rtp_packets_L_t;

TINYRTP_API trtp_rtp_packet_t* trtp_rtp_packet_create_null();
TINYRTP_API trtp_rtp_packet_t* trtp_rtp_packet_create(uint32_t ssrc, uint16_t seq_num, uint32_t timestamp, uint8_t payload_type, tsk_bool_t marker);
TINYRTP_API trtp_rtp_packet_t* trtp_rtp_packet_create_2(const trtp_rtp_header_t* header);
TINYRTP_API tsk_size_t trtp_rtp_packet_guess_serialbuff_size(const trtp_rtp_packet_t *self);
TINYRTP_API tsk_size_t trtp_rtp_packet_serialize_to(const trtp_rtp_packet_t *self, void* buffer, tsk_size_t size);
TINYRTP_API tsk_buffer_t* trtp_rtp_packet_serialize(const trtp_rtp_packet_t *self, tsk_size_t num_bytes_pad);
TINYRTP_API trtp_rtp_packet_t* trtp_rtp_packet_deserialize(const void *data, tsk_size_t size);


TINYRTP_GEXTERN const tsk_object_def_t *trtp_rtp_packet_def_t;

TRTP_END_DECLS

#endif /* TINYRTP_RTP_PACKET_H */
