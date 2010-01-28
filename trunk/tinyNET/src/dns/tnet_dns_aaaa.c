/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou@yahoo.fr>
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
* GNU Lesser General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/
/**@file tnet_dns_aaaaaaa.c
 * @brief DNS IPv6 address record - RR - (RFC 3596).
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tnet_dns_aaaa.h"

#include "../tnet_types.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"


//========================================================
//	[[DNS AAAA]] object definition
//
static void* tnet_dns_aaaa_create(void * self, va_list * app)
{
	tnet_dns_aaaa_t *aaaa = self;
	if(aaaa)
	{
		const char* name = va_arg(*app, const char*);
		tnet_dns_qclass_t qclass = va_arg(*app, tnet_dns_qclass_t);
		uint32_t ttl = va_arg(*app, uint32_t);
#if defined(__GNUC__)
		uint16_t rdlength = (uint16_t)va_arg(*app, unsigned);
#else
		uint16_t rdlength = va_arg(*app, uint16_t);
#endif
		const void* data = va_arg(*app, const void*);
		size_t offset = va_arg(*app, size_t);

		const uint8_t* rddata = (((uint8_t*)data) + offset);
		const uint8_t* dataEnd = (rddata + rdlength);

		/* init base */
		tnet_dns_rr_init(TNET_DNS_RR(aaaa), qtype_aaaa, qclass);
		TNET_DNS_RR(aaaa)->name = tsk_strdup(name);
		TNET_DNS_RR(aaaa)->rdlength = rdlength;
		TNET_DNS_RR(aaaa)->ttl = ttl;

		if(rddata && rdlength && (rdlength == 16/* 128bits */))
		{	// ==> DESERIALIZATION
			/* ADDRESS */
			tsk_sprintf(&(aaaa->address), "%x:%x:%x:%x:%x:%x:%x:%x",
				ntohs(*((uint16_t*)&rddata[0])), ntohs(*((uint16_t*)&rddata[2])), ntohs(*((uint16_t*)&rddata[4])), ntohs(*((uint16_t*)&rddata[6])),
				ntohs(*((uint16_t*)&rddata[8])), ntohs(*((uint16_t*)&rddata[10])), ntohs(*((uint16_t*)&rddata[12])), ntohs(*((uint16_t*)&rddata[14])));
		}
		else
		{
			TSK_DEBUG_ERROR("Invalid IPv6 address.");
		}

	}
	return self;
}

static void* tnet_dns_aaaa_destroy(void * self) 
{ 
	tnet_dns_aaaa_t *aaaa = self;
	if(aaaa)
	{
		/* deinit base */
		tnet_dns_rr_deinit(TNET_DNS_RR(aaaa));

		TSK_FREE(aaaa->address);
	}
	return self;
}

static const tsk_object_def_t tnet_dns_aaaa_def_s =
{
	sizeof(tnet_dns_aaaa_t),
	tnet_dns_aaaa_create,
	tnet_dns_aaaa_destroy,
	0,
};
const void *tnet_dns_aaaa_def_t = &tnet_dns_aaaa_def_s;
