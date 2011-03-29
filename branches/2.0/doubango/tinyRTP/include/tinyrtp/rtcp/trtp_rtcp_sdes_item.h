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
#ifndef TINYRTP_RTCP_SDES_ITEM_H
#define TINYRTP_RTCP_SDES_ITEM_H

#include "tinyrtp_config.h"

#include "tsk_list.h"
#include "tsk_buffer.h"

#define TRTP_RTCP_SDES_HEADER_SIZE 4

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
	struct{
		unsigned version:2;
		unsigned padding:1;
		unsigned source_count:5;
		trtp_rtcp_sdes_item_type_t type; /**< Packet type: 8bits */
		uint16_t length;
	} header;
}
trtp_rtcp_sdes_item_t;

#define TRTP_DECLARE_RTCP_SDES_ITEM trtp_rtcp_sdes_item_t __sdes_item__
typedef tsk_list_t trtp_rtcp_sdes_items_L_t; /**< List of @ref trtp_rtcp_sdes_item_t elements */
int trtp_rtcp_sdes_item_init(trtp_rtcp_sdes_item_t* self, uint8_t version, uint8_t padding, trtp_rtcp_sdes_item_type_t type, uint16_t length);
trtp_rtcp_sdes_item_t* trtp_rtcp_sdes_item_create(uint8_t version, uint8_t padding, trtp_rtcp_sdes_item_type_t type, uint16_t length);
trtp_rtcp_sdes_item_t* trtp_rtcp_sdes_item_deserialize(const void* data, tsk_size_t size);
tsk_buffer_t* trtp_rtcp_sdes_item_serialize(trtp_rtcp_sdes_item_t* self);

// RFC 3550 6.5.1 CNAME: Canonical End-Point Identifier SDES Item
typedef struct trtp_rtcp_sdes_item_cname_s
{
	TRTP_DECLARE_RTCP_SDES_ITEM;
	char* cname;
}
trtp_rtcp_sdes_item_cname_t;
TINYRTP_GEXTERN const tsk_object_def_t *trtp_rtcp_sdes_item_cname_def_t;
trtp_rtcp_sdes_item_cname_t* trtp_rtcp_sdes_item_cname_create_null();
trtp_rtcp_sdes_item_cname_t* trtp_rtcp_sdes_item_cname_create(const char* cname);


// RFC 3550 6.5.2 NAME: User Name SDES Item
typedef struct trtp_rtcp_sdes_item_name_s
{
	TRTP_DECLARE_RTCP_SDES_ITEM;
	char* name;
}
trtp_rtcp_sdes_item_name_t;
TINYRTP_GEXTERN const tsk_object_def_t *trtp_rtcp_sdes_item_name_def_t;
trtp_rtcp_sdes_item_name_t* trtp_rtcp_sdes_item_name_create_null();
trtp_rtcp_sdes_item_name_t* trtp_rtcp_sdes_item_name_create(const char* name);

// RFC 3550 6.5.3 EMAIL: Electronic Mail Address SDES Item
typedef struct trtp_rtcp_sdes_item_email_s
{
	TRTP_DECLARE_RTCP_SDES_ITEM;
	char* email;
}
trtp_rtcp_sdes_item_email_t;
TINYRTP_GEXTERN const tsk_object_def_t *trtp_rtcp_sdes_item_email_def_t;
trtp_rtcp_sdes_item_email_t* trtp_rtcp_sdes_item_email_create_null();
trtp_rtcp_sdes_item_email_t* trtp_rtcp_sdes_item_email_create(const char* email);

// RFC 3550 6.5.4 PHONE: Phone Number SDES Item
typedef struct trtp_rtcp_sdes_item_phone_s
{
	TRTP_DECLARE_RTCP_SDES_ITEM;
	char* phone;
}
trtp_rtcp_sdes_item_phone_t;
TINYRTP_GEXTERN const tsk_object_def_t *trtp_rtcp_sdes_item_phone_def_t;
trtp_rtcp_sdes_item_phone_t* trtp_rtcp_sdes_item_phone_create_null();
trtp_rtcp_sdes_item_phone_t* trtp_rtcp_sdes_item_phone_create(const char* phone);

// RFC 3550 6.5.5 LOC: Geographic User Location SDES Item
typedef struct trtp_rtcp_sdes_item_loc_s
{
	TRTP_DECLARE_RTCP_SDES_ITEM;
	char* loc;
}
trtp_rtcp_sdes_item_loc_t;
TINYRTP_GEXTERN const tsk_object_def_t *trtp_rtcp_sdes_item_loc_def_t;
trtp_rtcp_sdes_item_loc_t* trtp_rtcp_sdes_item_loc_create_null();
trtp_rtcp_sdes_item_loc_t* trtp_rtcp_sdes_item_loc_create(const char* loc);

// RFC 3550 6.5.6 TOOL: Application or Tool Name SDES Item
typedef struct trtp_rtcp_sdes_item_tool_s
{
	TRTP_DECLARE_RTCP_SDES_ITEM;
	char* tool;
}
trtp_rtcp_sdes_item_tool_t;
TINYRTP_GEXTERN const tsk_object_def_t *trtp_rtcp_sdes_item_tool_def_t;
trtp_rtcp_sdes_item_tool_t* trtp_rtcp_sdes_item_tool_create_null();

// RFC 3550 6.5.7 NOTE: Notice/Status SDES Item
typedef struct trtp_rtcp_sdes_item_note_s
{
	TRTP_DECLARE_RTCP_SDES_ITEM;
	char* note;
}
trtp_rtcp_sdes_item_note_t;
TINYRTP_GEXTERN const tsk_object_def_t *trtp_rtcp_sdes_item_note_def_t;
trtp_rtcp_sdes_item_note_t* trtp_rtcp_sdes_item_note_create_null();
trtp_rtcp_sdes_item_note_t* trtp_rtcp_sdes_item_note_create(const char* note);

// RFC 3550 6.5.8 PRIV: Private Extensions SDES Item
typedef struct trtp_rtcp_sdes_item_priv_s
{
	TRTP_DECLARE_RTCP_SDES_ITEM;
	tsk_buffer_t* priv;
}
trtp_rtcp_sdes_item_priv_t;
TINYRTP_GEXTERN const tsk_object_def_t *trtp_rtcp_sdes_item_priv_def_t;
trtp_rtcp_sdes_item_priv_t* trtp_rtcp_sdes_item_priv_create_null();
trtp_rtcp_sdes_item_priv_t* trtp_rtcp_sdes_item_priv_create(const void* data, tsk_size_t size);

TRTP_END_DECLS

#endif /* TINYRTP_RTCP_SDES_ITEM_H */
