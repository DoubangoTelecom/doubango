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
/**@file tnet_dhcp6_duid.h
 * @brief DHCPv6 (RFC 3315) DHCP Unique Identifier (DUID) as defined in subclause 9.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef TNET_DHCP6_duid_H
#define TNET_DHCP6_duid_H

#include "tinynet_config.h"

#include "tnet_hardwares.h"
#include "tsk_list.h"
#include "tsk_buffer.h"

TNET_BEGIN_DECLS

#define TNET_DHCP6_DUID(self)							((tnet_dhcp6_duid_t*)(self))
#define TNET_DHCP6_DUID_LLT(self)						((tnet_dhcp6_duid_llt_t*)(self))
#define TNET_DHCP6_DUID_EN(self)						((tnet_dhcp6_duid_en_t*)(self))
#define TNET_DHCP6_DUID_LL(self)						((tnet_dhcp6_duid_ll_t*)(self))

/** List of DHCPv6 DUIDs types as per RFC 3315 subclause 9.1.
*/
typedef enum tnet_dhcp6_duid_type_e
{
	dhcp6_duid_linklayer_plus_time = 1, /**< Link-layer address plus time. */
	dhcp6_duid_Vendor_assigned_id = 2, /**< Vendor-assigned unique ID based on Enterprise Number. */
	dhcp6_duid_linklayer = 3, /**< Link-layer address. */
}
tnet_dhcp6_duid_type_t;

/** DHCP Unique Identifier (DUID) base class (subclause 9).
*/
typedef struct tnet_dhcp6_duid_s
{
	TSK_DECLARE_OBJECT;
	
	tsk_bool_t initialized;

	tnet_dhcp6_duid_type_t type; /* DUID type. 2-bytes value. */
}
tnet_dhcp6_duid_t;

typedef tsk_list_t tnet_dhcp6_duids_L_t;

#define TNET_DECLARE_DHCP6_DUID tnet_dhcp6_duid_t tnet_dhcp6_duid

int tnet_dhcp6_duid_init(tnet_dhcp6_duid_t *self, tnet_dhcp6_duid_type_t type);
int tnet_dhcp6_duid_deinit(tnet_dhcp6_duid_t *self);

tnet_dhcp6_duid_t* tnet_dhcp6_duid_deserialize(const void* data, tsk_size_t size);
int tnet_dhcp6_duid_serialize(const tnet_dhcp6_duid_t* self, tsk_buffer_t *output);

/*=======================================================================================
*	RFC 3315 - 9.2. DUID Based on Link-layer Address Plus Time [DUID-LLT]
*=======================================================================================*/

/** DUID Based on Link-layer Address Plus Time [DUID-LLT]
*/
typedef struct tnet_dhcp6_duid_llt_s
{
	TNET_DECLARE_DHCP6_DUID;
	/*
	0                   1                   2                   3
	0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|               1               |    hardware type (16 bits)    |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|                        time (32 bits)                         |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	.                                                               .
	.             link-layer address (variable length)              .
	.                                                               .
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	*/
	/* The hardware type code as maintained by IANA. */
	tnet_hardware_type_t htype;
	/* The time value is the time that the DUID is generated represented in seconds 
	since midnight (UTC), January 1, 2000, modulo 2^32. */
	uint32_t time;
	/* The link-layer address.  */
	tsk_buffer_t *address;
}
tnet_dhcp6_duid_llt_t;


/*=======================================================================================
*	RFC 3315 - 9.3. DUID Assigned by Vendor Based on Enterprise Number [DUID-EN]
*=======================================================================================*/

/** DUID Assigned by Vendor Based on Enterprise Number [DUID-EN]
*/
typedef struct tnet_dhcp6_duid_en_s
{
	TNET_DECLARE_DHCP6_DUID;
	/*
	0                   1                   2                   3
	0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|               2               |       enterprise-number       |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|   enterprise-number (contd)   |                               |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                               |
	.                           identifier                          .
	.                       (variable length)                       .
	.                                                               .
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	*/
	/* Tthe vendor's registered Private Enterprise Number as maintained by IANA.
		For more information: http://www.iana.org/assignments/enterprise-numbers. */
	uint32_t en;
	/* The link-layer address.  */
	tsk_buffer_t *indentifier;
}
tnet_dhcp6_duid_en_t;

/*=======================================================================================
*	RFC 3315 - 9.4. DUID Based on Link-layer Address [DUID-LL]
*=======================================================================================*/

/** DUID Based on Link-layer Address [DUID-LL]
*/
typedef struct tnet_dhcp6_duid_ll_s
{
	TNET_DECLARE_DHCP6_DUID;
	/*
	0                   1                   2                   3
     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |               3               |    hardware type (16 bits)    |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    .                                                               .
    .             link-layer address (variable length)              .
    .                                                               .
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	*/
	/* The hardware type code as maintained by IANA. */
	tnet_hardware_type_t htype;
	/* The link-layer address.  */
	tsk_buffer_t *address;
}
tnet_dhcp6_duid_ll_t;






TINYNET_API tnet_dhcp6_duid_llt_t* tnet_dhcp6_duid_llt_create(const void* payload, tsk_size_t payload_size);
TINYNET_API tnet_dhcp6_duid_en_t* tnet_dhcp6_duid_en_create(const void* payload, tsk_size_t payload_size);
TINYNET_API tnet_dhcp6_duid_ll_t* tnet_dhcp6_duid_ll_create(const void* payload, tsk_size_t payload_size);




TINYNET_GEXTERN const tsk_object_def_t *tnet_dhcp6_duid_llt_def_t;
TINYNET_GEXTERN const tsk_object_def_t *tnet_dhcp6_duid_en_def_t;
TINYNET_GEXTERN const tsk_object_def_t *tnet_dhcp6_duid_ll_def_t;

TNET_END_DECLS

#endif /* TNET_DHCP6_duid_H */
