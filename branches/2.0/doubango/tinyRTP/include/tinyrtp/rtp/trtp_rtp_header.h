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
/**@file trtp_rtp_header.h
 * @brief RTP header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 */
#ifndef TINYRTP_RTP_HEADER_H
#define TINYRTP_RTP_HEADER_H

#include "tinyrtp_config.h"
#include "tinymedia/tmedia_codec.h"
#include "tsk_buffer.h"

TRTP_BEGIN_DECLS

#define TRTP_RTP_HEADER_MIN_SIZE 12
#define TRTP_RTP_HEADER(self)	((trtp_rtp_header_t*)(self))

typedef struct trtp_rtp_header_s
{
	TSK_DECLARE_OBJECT;
	/* RFC 3550 section 5.1 - RTP Fixed Header Fields
		0                   1                   2                   3
		0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	   |V=2|P|X|  CC   |M|     PT      |       sequence number         |
	   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	   |                           timestamp                           |
	   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	   |           synchronization source (SSRC) identifier            |
	   +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
	   |            contributing source (CSRC) identifiers             |
	   |                             ....                              |
	   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	*/
	unsigned version:2;
	unsigned padding:1;
	unsigned extension:1;
	unsigned csrc_count:4;
	unsigned marker:1;
	unsigned payload_type:7;
	uint16_t seq_num;
	uint32_t timestamp;
	uint32_t ssrc;
	uint32_t csrc[15];

	// for internal use
	enum tmedia_codec_id_e codec_id;
}
trtp_rtp_header_t;

TINYRTP_API trtp_rtp_header_t* trtp_rtp_header_create_null();
TINYRTP_API trtp_rtp_header_t* trtp_rtp_header_create(uint32_t ssrc, uint16_t seq_num, uint32_t timestamp, uint8_t payload_type, tsk_bool_t marker);
TINYRTP_API tsk_size_t trtp_rtp_header_guess_serialbuff_size(const trtp_rtp_header_t *self);
TINYRTP_API tsk_size_t trtp_rtp_header_serialize_to(const trtp_rtp_header_t *self, void *buffer, tsk_size_t size);
TINYRTP_API tsk_buffer_t* trtp_rtp_header_serialize(const trtp_rtp_header_t *self);
TINYRTP_API trtp_rtp_header_t* trtp_rtp_header_deserialize(const void *data, tsk_size_t size);


TINYRTP_GEXTERN const tsk_object_def_t *trtp_rtp_header_def_t;

TRTP_END_DECLS

#endif /* TINYRTP_RTP_HEADER_H */
