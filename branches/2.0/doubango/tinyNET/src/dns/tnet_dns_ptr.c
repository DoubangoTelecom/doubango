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
/**@file tnet_dns_ptr.c
 * @brief DNS Pointer record - RR - (RFC 1035).
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tnet_dns_ptr.h"

#include "../tnet_types.h"

#include "tsk_string.h"
#include "tsk_memory.h"

/** Creates new DNS PTR Resource Record.
*/


tnet_dns_ptr_t* tnet_dns_ptr_create(const char* name, tnet_dns_qclass_t qclass, uint32_t ttl, uint16_t rdlength, const void*data, tsk_size_t offset)
{
	return tsk_object_new(tnet_dns_ptr_def_t, name, qclass, ttl, rdlength, data, offset);
}

//=================================================================================================
//	[[DNS PTR]] object definition
//
static tsk_object_t* tnet_dns_ptr_ctor(tsk_object_t * self, va_list * app)
{
	tnet_dns_ptr_t *ptr = self;
	if(ptr){
		const char* name = va_arg(*app, const char*);
		tnet_dns_qclass_t qclass = va_arg(*app, tnet_dns_qclass_t);
		uint32_t ttl = va_arg(*app, uint32_t);
		uint16_t rdlength = tsk_va_arg_u16(*app);
		const void* data = va_arg(*app, const void*);
		tsk_size_t offset = va_arg(*app, tsk_size_t);

		/* init base */
		tnet_dns_rr_init(TNET_DNS_RR(ptr), qtype_ptr, qclass);
		TNET_DNS_RR(ptr)->name = tsk_strdup(name);
		TNET_DNS_RR(ptr)->rdlength = rdlength;
		TNET_DNS_RR(ptr)->ttl = ttl;

		if(rdlength){
			// ==> DESERIALIZATION
			/* PTRDNAME */
			tnet_dns_rr_qname_deserialize(data, &(ptr->ptrdname), &offset);
		}
	}
	return self;
}

static tsk_object_t* tnet_dns_ptr_dtor(tsk_object_t * self) 
{ 
	tnet_dns_ptr_t *ptr = self;
	if(ptr){
		/* deinit base */
		tnet_dns_rr_deinit(TNET_DNS_RR(ptr));

		TSK_FREE(ptr->ptrdname);
	}
	return self;
}

static const tsk_object_def_t tnet_dns_ptr_def_s =
{
	sizeof(tnet_dns_ptr_t),
	tnet_dns_ptr_ctor,
	tnet_dns_ptr_dtor,
	tsk_null,
};
const tsk_object_def_t *tnet_dns_ptr_def_t = &tnet_dns_ptr_def_s;
