/*
* Copyright (C) 2009 Mamadou Diop.
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
/**@file tnet_dns_cname.c
 * @brief DNS Mail eXchange record - RR - (RFC 1035).
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tnet_dns_cname.h"

#include "../tnet_types.h"

#include "tsk_string.h"
#include "tsk_memory.h"

//=================================================================================================
//	[[DNS CNAME]] object definition
//
static void* tnet_dns_cname_create(void * self, va_list * app)
{
	tnet_dns_cname_t *cname = self;
	if(cname)
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

		/* init base */
		tnet_dns_rr_init(TNET_DNS_RR(cname), qtype_cname, qclass);
		TNET_DNS_RR(cname)->name = tsk_strdup(name);
		TNET_DNS_RR(cname)->rdlength = rdlength;
		TNET_DNS_RR(cname)->ttl = ttl;

		if(rdlength)
		{	// ==> DESERIALIZATION
			/* CNAME */
			tnet_dns_rr_qname_deserialize(data, &(cname->cname), &offset);
		}
	}
	return self;
}

static void* tnet_dns_cname_destroy(void * self) 
{ 
	tnet_dns_cname_t *cname = self;
	if(cname)
	{
		/* deinit base */
		tnet_dns_rr_deinit(TNET_DNS_RR(cname));

		TSK_FREE(cname->cname);
	}
	return self;
}

static const tsk_object_def_t tnet_dns_cname_def_s =
{
	sizeof(tnet_dns_cname_t),
	tnet_dns_cname_create,
	tnet_dns_cname_destroy,
	0,
};
const void *tnet_dns_cname_def_t = &tnet_dns_cname_def_s;
