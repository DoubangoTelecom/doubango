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
/**@file tnet_dns_aaaA.c
 * @brief DNS IPv6 address record - RR - (RFC 3596).
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tnet_dns_aaaa.h"

#include "../tnet_types.h"
#include "../tnet_endianness.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

/** Creates new DNS AAAA Resource Record.
*/

tnet_dns_aaaa_t* tnet_dns_aaaa_create(const char* name, tnet_dns_qclass_t qclass, uint32_t ttl, uint16_t rdlength, const void* data, tsk_size_t offset)
{
	return tsk_object_new(tnet_dns_aaaa_def_t, name, qclass, ttl, rdlength, data, offset);
}


//=================================================================================================
//	[[DNS AAAA]] object definition
//
static tsk_object_t* tnet_dns_aaaa_ctor(tsk_object_t * self, va_list * app)
{
	tnet_dns_aaaa_t *aaaa = self;
	if(aaaa){
		const char* name = va_arg(*app, const char*);
		tnet_dns_qclass_t qclass = va_arg(*app, tnet_dns_qclass_t);
		uint32_t ttl = va_arg(*app, uint32_t);
		uint16_t rdlength = tsk_va_arg_u16(*app);
		const void* data = va_arg(*app, const void*);
		tsk_size_t offset = va_arg(*app, tsk_size_t);

		const uint8_t* rddata = (((uint8_t*)data) + offset);
		//const uint8_t* dataEnd = (rddata + rdlength);

		/* init base */
		tnet_dns_rr_init(TNET_DNS_RR(aaaa), qtype_aaaa, qclass);
		TNET_DNS_RR(aaaa)->name = tsk_strdup(name);
		TNET_DNS_RR(aaaa)->rdlength = rdlength;
		TNET_DNS_RR(aaaa)->ttl = ttl;

		if(rddata && rdlength && (rdlength == 16/* 128bits */)){
			// ==> DESERIALIZATION
			/* ADDRESS */
			tsk_sprintf(&(aaaa->address), "%x:%x:%x:%x:%x:%x:%x:%x",
				tnet_ntohs_2(&rddata[0]), tnet_ntohs_2(&rddata[2]), tnet_ntohs_2(&rddata[4]), tnet_ntohs_2(&rddata[6]),
				tnet_ntohs_2(&rddata[8]), tnet_ntohs_2(&rddata[10]), tnet_ntohs_2(&rddata[12]), tnet_ntohs_2(&rddata[14]));
		}
		else{
			TSK_DEBUG_ERROR("Invalid IPv6 address.");
		}

	}
	return self;
}

static tsk_object_t* tnet_dns_aaaa_dtor(tsk_object_t * self) 
{ 
	tnet_dns_aaaa_t *aaaa = self;
	if(aaaa){
		/* deinit base */
		tnet_dns_rr_deinit(TNET_DNS_RR(aaaa));

		TSK_FREE(aaaa->address);
	}
	return self;
}

static const tsk_object_def_t tnet_dns_aaaa_def_s =
{
	sizeof(tnet_dns_aaaa_t),
	tnet_dns_aaaa_ctor,
	tnet_dns_aaaa_dtor,
	tsk_null,
};
const tsk_object_def_t *tnet_dns_aaaa_def_t = &tnet_dns_aaaa_def_s;
