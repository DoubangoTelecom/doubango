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
#ifndef TINYRTP_RTCP_SDES_ITEM_H
#define TINYRTP_RTCP_SDES_ITEM_H

#include "tinyrtp_config.h"

#include "tsk_list.h"
#include "tsk_buffer.h"

#define TRTP_RTCP_SDES_ITEM_MIN_SIZE 2 /* Type + Length */

#define TRTP_RTCP_SDES_ITEM(self) ((trtp_rtcp_sdes_item_t*)(self))

TRTP_BEGIN_DECLS

// RFC 3550 12.2 SDES Types
typedef enum trtp_rtcp_sdes_item_type_e
{
	trtp_rtcp_sdes_item_type_end = 0, /**< end of SDES list */
	trtp_rtcp_sdes_item_type_cname = 1, /**< canonical name*/
	trtp_rtcp_sdes_item_type_name = 2, /**< user name */
	trtp_rtcp_sdes_item_type_email = 3, /**< user's electronic mail address*/
	trtp_rtcp_sdes_item_type_phone = 4, /**< user's phone number  */
	trtp_rtcp_sdes_item_type_loc = 5, /**< geographic user location*/
	trtp_rtcp_sdes_item_type_tool = 6, /**< name of application or tool*/
	trtp_rtcp_sdes_item_type_note = 7, /**< notice about the source*/
	trtp_rtcp_sdes_item_type_priv = 8, /**< private extensions*/
}
trtp_rtcp_sdes_item_type_t;

typedef struct trtp_rtcp_sdes_item_s
{
	TSK_DECLARE_OBJECT;

	trtp_rtcp_sdes_item_type_t type;
	tsk_buffer_t *data;
}
trtp_rtcp_sdes_item_t;


typedef tsk_list_t trtp_rtcp_sdes_items_L_t; /**< List of @ref trtp_rtcp_sdes_item_t elements */

trtp_rtcp_sdes_item_t* trtp_rtcp_sdes_item_create(trtp_rtcp_sdes_item_type_t type, const void* data, uint8_t length);
trtp_rtcp_sdes_item_t* trtp_rtcp_sdes_item_deserialize(const void* data, tsk_size_t size);
tsk_buffer_t* trtp_rtcp_sdes_item_serialize(const trtp_rtcp_sdes_item_t* self);
int trtp_rtcp_sdes_item_serialize_to(const trtp_rtcp_sdes_item_t* self, void* data, tsk_size_t size);
tsk_size_t trtp_rtcp_sdes_item_get_size(const trtp_rtcp_sdes_item_t* self);

TRTP_END_DECLS

#endif /* TINYRTP_RTCP_SDES_ITEM_H */
