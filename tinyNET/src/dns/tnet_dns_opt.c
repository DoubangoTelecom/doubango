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
/**@file tnet_dns_opt.c
 * @brief DNS OPT pseudo-RR (RFC 2671).
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tnet_dns_opt.h"



/** Creates new DNS OPT Resource Record.
*/
tnet_dns_opt_t* tnet_dns_opt_create(tsk_size_t payload_size)
{
	return tsk_object_new(tnet_dns_opt_def_t, payload_size);
}









//=================================================================================================
//	[[DNS OPT]] object definition
//
static tsk_object_t* tnet_dns_opt_ctor(tsk_object_t * self, va_list * app)
{
	tnet_dns_opt_t *rr_opt = self;
	if(rr_opt){
		uint16_t payload_size = (uint16_t)va_arg(*app, tsk_size_t);

		/* init base */
		tnet_dns_rr_init(TNET_DNS_RR(rr_opt), qtype_opt, qclass_any);

		/*
			 NAME         domain name    empty (root domain)
			 TYPE         u_int16_t      OPT
			 CLASS        u_int16_t      sender's UDP payload size
			 TTL          u_int32_t      extended RCODE and flags
			 RDLEN        u_int16_t      describes RDATA
			 RDATA        octet stream   {attribute,value} pairs
			
		*/
		TNET_DNS_RR(rr_opt)->qclass = payload_size;
	}
	return self;
}

static tsk_object_t* tnet_dns_opt_dtor(tsk_object_t * self) 
{ 
	tnet_dns_opt_t *rr_opt = self;
	if(rr_opt){
		/* deinit base */
		tnet_dns_rr_deinit(TNET_DNS_RR(rr_opt));
	}
	return self;
}

static const tsk_object_def_t tnet_dns_opt_def_s =
{
	sizeof(tnet_dns_opt_t),
	tnet_dns_opt_ctor,
	tnet_dns_opt_dtor,
	tsk_null,
};
const tsk_object_def_t *tnet_dns_opt_def_t = &tnet_dns_opt_def_s;
