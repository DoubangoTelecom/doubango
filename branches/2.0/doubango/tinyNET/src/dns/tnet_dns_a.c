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
/**@file tnet_dns_a.c
 * @brief DNS Address record - RR - (RFC 1035).
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tnet_dns_a.h"

#include "../tnet_types.h"
#include "../tnet_endianness.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

/** Creates new DNS A Resource Record.
*/
tnet_dns_a_t* tnet_dns_a_create(const char* name, tnet_dns_qclass_t qclass, uint32_t ttl, uint16_t rdlength, const void* data, tsk_size_t offset)
{
	return tsk_object_new(tnet_dns_a_def_t, name, qclass, ttl, rdlength, data, offset);
}


//=================================================================================================
//	[[DNS A]] object definition
//
static tsk_object_t* tnet_dns_a_ctor(tsk_object_t * self, va_list * app)
{
	tnet_dns_a_t *a = self;
	if(a){
		const char* name = va_arg(*app, const char*);
		tnet_dns_qclass_t qclass = va_arg(*app, tnet_dns_qclass_t);
		uint32_t ttl = va_arg(*app, uint32_t);
		uint16_t rdlength = tsk_va_arg_u16(*app);
		const void* data = va_arg(*app, const void*);
		tsk_size_t offset = va_arg(*app, tsk_size_t);

		const uint8_t* rddata = (((uint8_t*)data) + offset);
		//const uint8_t* dataEnd = (rddata + rdlength);

		/* init base */
		tnet_dns_rr_init(TNET_DNS_RR(a), qtype_a, qclass);
		TNET_DNS_RR(a)->name = tsk_strdup(name);
		TNET_DNS_RR(a)->rdlength = rdlength;
		TNET_DNS_RR(a)->ttl = ttl;

		if(rddata && rdlength && (rdlength == 4/* 32bits */)){
			// ==> DESERIALIZATION
			/* ADDRESS */
			uint32_t address = tnet_htonl_2(rddata);
			tsk_sprintf(&(a->address), "%u.%u.%u.%u", (address>>24)&0xFF, (address>>16)&0xFF, (address>>8)&0xFF, (address>>0)&0xFF);
		}
		else{
			TSK_DEBUG_ERROR("Invalid IPv4 address.");
		}

	}
	return self;
}

static tsk_object_t* tnet_dns_a_dtor(tsk_object_t * self) 
{ 
	tnet_dns_a_t *a = self;
	if(a){
		/* deinit base */
		tnet_dns_rr_deinit(TNET_DNS_RR(a));

		TSK_FREE(a->address);
	}
	return self;
}

static const tsk_object_def_t tnet_dns_a_def_s =
{
	sizeof(tnet_dns_a_t),
	tnet_dns_a_ctor,
	tnet_dns_a_dtor,
	tsk_null,
};
const tsk_object_def_t *tnet_dns_a_def_t = &tnet_dns_a_def_s;
