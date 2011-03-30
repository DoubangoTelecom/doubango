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
#ifndef TINYRTP_RTCP_HEADER_H
#define TINYRTP_RTCP_HEADER_H

#include "tinyrtp_config.h"

#include "tsk_buffer.h"
#include "tsk_list.h"

#define TRTP_RTCP_HEADER_SIZE				4
#define TRTP_RTCP_HEADER_VERSION_DEFAULT	2

TRTP_BEGIN_DECLS

typedef struct trtp_rtcp_header_s
{
	TSK_DECLARE_OBJECT;
	
	unsigned version:2;
	unsigned padding:1;
	unsigned rc:5;
	enum trtp_rtcp_packet_type_e type; /**< Packet Type on 8bits */
	uint16_t length;
}
trtp_rtcp_header_t;

#define TRTP_DECLARE_RTCP_HEADER trtp_rtcp_header_t __header__
typedef tsk_list_t trtp_rtcp_headers_L_t; /**< List of @ref trtp_rtcp_header_t elements */

TINYRTP_API trtp_rtcp_header_t* trtp_rtcp_header_create_null();
TINYRTP_API trtp_rtcp_header_t* trtp_rtcp_header_create(uint8_t version, uint8_t padding, uint8_t rc, enum trtp_rtcp_packet_type_e type, uint16_t length);
TINYRTP_API tsk_buffer_t* trtp_rtcp_header_serialize(const trtp_rtcp_header_t *self);
TINYRTP_API int trtp_rtcp_header_serialize_2(const trtp_rtcp_header_t *self, uint8_t output[TRTP_RTCP_HEADER_SIZE]);
TINYRTP_API trtp_rtcp_header_t* trtp_rtcp_header_deserialize(const void *data, tsk_size_t size);
TINYRTP_API int trtp_rtcp_header_deserialize_2(trtp_rtcp_header_t** self, const void *data, tsk_size_t size);

TINYRTP_GEXTERN const tsk_object_def_t *trtp_rtcp_header_def_t;

TRTP_END_DECLS

#endif /* TINYRTP_RTCP_HEADER_H */
