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
/**@file tnet_dns_mx.c
 * @brief DNS Mail eXchange record - RR - (RFC 1035).
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tnet_dns_mx.h"

#include "../tnet_types.h"

#include "tsk_string.h"
#include "tsk_memory.h"

//========================================================
//	[[DNS MX]] object definition
//
static void* tnet_dns_mx_create(void * self, va_list * app)
{
	tnet_dns_mx_t *mx = self;
	if(mx)
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
		tnet_dns_rr_init(TNET_DNS_RR(mx), qtype_mx, qclass);
		TNET_DNS_RR(mx)->name = tsk_strdup(name);
		TNET_DNS_RR(mx)->rdlength = rdlength;
		TNET_DNS_RR(mx)->ttl = ttl;

		if(rddata && rdlength)
		{	// ==> DESERIALIZATION
			/* PREFERENCE */
			mx->preference = ntohs(*((uint16_t*)rddata));
			rddata += 2, offset += 2;
			/* EXCHANGE */
			tnet_dns_rr_qname_deserialize(data, (dataEnd - rddata), &(mx->exchange), &offset);
		}
	}
	return self;
}

static void* tnet_dns_mx_destroy(void * self) 
{ 
	tnet_dns_mx_t *mx = self;
	if(mx)
	{
		/* deinit base */
		tnet_dns_rr_deinit(TNET_DNS_RR(mx));

		TSK_FREE(mx->exchange);
	}
	return self;
}

static const tsk_object_def_t tnet_dns_mx_def_s =
{
	sizeof(tnet_dns_mx_t),
	tnet_dns_mx_create,
	tnet_dns_mx_destroy,
	0,
};
const void *tnet_dns_mx_def_t = &tnet_dns_mx_def_s;
