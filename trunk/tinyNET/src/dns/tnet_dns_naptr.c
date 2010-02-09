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
/**@file tnet_dns_naptr.c
 * @brief DNS Naming Authority Pointer - RR - (RFC 3403).
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tnet_dns_naptr.h"


#include "../tnet_types.h"

#include "tsk_memory.h"
#include "tsk_string.h"



//========================================================
//	[[DNS NAPTR]] object definition
//
static void* tnet_dns_naptr_create(void * self, va_list * app)
{
	tnet_dns_naptr_t *naptr = self;
	if(naptr)
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
		tnet_dns_rr_init(TNET_DNS_RR(naptr), qtype_naptr, qclass);
		TNET_DNS_RR(naptr)->name = tsk_strdup(name);
		TNET_DNS_RR(naptr)->rdlength = rdlength;
		TNET_DNS_RR(naptr)->ttl = ttl;

		if(rddata && rdlength)
		{	// ==> DESERIALIZATION
			/* ORDER */
			naptr->order = ntohs(*((uint16_t*)rddata));
			rddata += 2, offset += 2;
			/* PREFERENCE */
			naptr->preference = ntohs(*((uint16_t*)rddata));
			rddata += 2, offset += 2;
			/* FLAGS */
			tnet_dns_rr_charstring_deserialize(data, (dataEnd - rddata), &(naptr->flags), &offset);
			rddata += offset;
			/* SERVICES */
			tnet_dns_rr_charstring_deserialize(data, (dataEnd - rddata), &(naptr->services), &offset);
			rddata += offset;
			/* REGEXP */
			tnet_dns_rr_charstring_deserialize(data, (dataEnd - rddata), &(naptr->regexp), &offset);
			rddata += offset;
			/* REPLACEMENT */
			tnet_dns_rr_qname_deserialize(data, (dataEnd - rddata), &(naptr->replacement), &offset);
		}
	}
	return self;
}

static void* tnet_dns_naptr_destroy(void * self) 
{ 
	tnet_dns_naptr_t *naptr = self;
	if(naptr)
	{
		/* deinit base */
		tnet_dns_rr_deinit(TNET_DNS_RR(naptr));
		
		TSK_FREE(naptr->flags);
		TSK_FREE(naptr->services);
		TSK_FREE(naptr->regexp);
		TSK_FREE(naptr->replacement);
	}
	return self;
}

static int tnet_dns_naptr_cmp(const void *obj1, const void *obj2)
{ 
	const tnet_dns_rr_t* rr1 = obj1;
	const tnet_dns_rr_t* rr2 = obj2;

	if(rr1 && rr2 && (rr1->qtype==qtype_naptr) && (rr2->qtype==qtype_naptr))
	{
		const tnet_dns_naptr_t* naptr1 = (tnet_dns_naptr_t*)rr1;
		const tnet_dns_naptr_t* naptr2 = (tnet_dns_naptr_t*)rr2;

		/* Compare orders. */
		if(naptr1->order < naptr2->order){ /* Lowest order is tried first. */
			return 1;
		}
		else if(naptr1->order > naptr2->order){
			return -1;
		}

		/* Compare preference */
		if(naptr1->order < naptr2->order){ /* Lowest preference is tried first. */
			return 1;
		}
		else if(naptr1->order > naptr2->order){
			return -1;
		}
		
		return 0;
	}
	else return -1;
}

static const tsk_object_def_t tnet_dns_naptr_def_s =
{
	sizeof(tnet_dns_naptr_t),
	tnet_dns_naptr_create,
	tnet_dns_naptr_destroy,
	tnet_dns_naptr_cmp,
};
const void *tnet_dns_naptr_def_t = &tnet_dns_naptr_def_s;
