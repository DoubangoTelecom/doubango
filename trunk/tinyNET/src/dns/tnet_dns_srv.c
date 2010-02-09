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
/**@file tnet_dns_srv.c
 * @brief DNS SeRVice locator record - RR - (RFC 2782).
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tnet_dns_srv.h"

#include "../tnet_types.h"

#include "tsk_string.h"
#include "tsk_memory.h"

//========================================================
//	[[DNS SRV]] object definition
//
static void* tnet_dns_srv_create(void * self, va_list * app)
{
	tnet_dns_srv_t *srv = self;
	if(srv)
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
		tnet_dns_rr_init(TNET_DNS_RR(srv), qtype_srv, qclass);
		TNET_DNS_RR(srv)->name = tsk_strdup(name);
		TNET_DNS_RR(srv)->rdlength = rdlength;
		TNET_DNS_RR(srv)->ttl = ttl;

		if(rddata && rdlength)
		{	// ==> DESERIALIZATION
			/* Priority */
			srv->priority = ntohs(*((uint16_t*)rddata));
			rddata += 2, offset += 2;
			/* Weight */
			srv->weight = ntohs(*((uint16_t*)rddata));
			rddata += 2, offset += 2;
			/* Port */
			srv->port = ntohs(*((uint16_t*)rddata));
			rddata += 2, offset += 2;
			/* Target */
			tnet_dns_rr_qname_deserialize(data, (dataEnd - rddata), &(srv->target), &offset);
		}
	}
	return self;
}

static void* tnet_dns_srv_destroy(void * self) 
{ 
	tnet_dns_srv_t *srv = self;
	if(srv)
	{
		/* deinit base */
		tnet_dns_rr_deinit(TNET_DNS_RR(srv));

		TSK_FREE(srv->target);
	}
	return self;
}

static int tnet_dns_srv_cmp(const void *obj1, const void *obj2)
{ 
	const tnet_dns_rr_t* rr1 = obj1;
	const tnet_dns_rr_t* rr2 = obj2;

	if(rr1 && rr2 && (rr1->qtype==qtype_srv) && (rr2->qtype==qtype_srv))
	{
		const tnet_dns_srv_t* srv1 = (tnet_dns_srv_t*)rr1;
		const tnet_dns_srv_t* srv2 = (tnet_dns_srv_t*)rr2;

		/* Compare priorities. */
		if(srv1->priority < srv2->priority){ /* Lowest priority is tried first. */
			return 1;
		}
		else if(srv1->priority > srv2->priority){
			return -1;
		}

		/* Compare weight */
		if(srv1->weight > srv2->weight){
			return 1;
		}
		else if(srv1->weight < srv2->weight){
			return -1;
		}
		
		return 0;
	}
	else return -1;
}

static const tsk_object_def_t tnet_dns_srv_def_s =
{
	sizeof(tnet_dns_srv_t),
	tnet_dns_srv_create,
	tnet_dns_srv_destroy,
	tnet_dns_srv_cmp,
};
const void *tnet_dns_srv_def_t = &tnet_dns_srv_def_s;
