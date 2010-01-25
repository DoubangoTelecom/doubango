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
/**@file tnet_dns_rr.c
 * @brief DNS Resource Record (RFC 1034 and 1035).
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tnet_dns_rr.h"

#include "tsk_memory.h"

int tnet_dns_rr_init(tnet_dns_rr_t *rr, tnet_dns_qtype_t qtype, tnet_dns_qclass_t qclass)
{
	if(rr)
	{
		if(!rr->initialized)
		{
			rr->qtype = qtype;
			rr->qclass = qclass;
			
			rr->initialized = 1;
			return 0;
		}
		return -2;
	}
	return -1;
}

int tnet_dns_rr_deinit(tnet_dns_rr_t *rr)
{
	if(rr)
	{
		if(rr->initialized)
		{
			TSK_FREE(rr->name);
			TSK_FREE(rr->rpdata);
			
			rr->initialized = 0;
			return 0;
		}
		return -2;
	}
	return -1;
}





//========================================================
//	[[DNS RR]] object definition
//
static void* tnet_dns_rr_create(void * self, va_list * app)
{
	tnet_dns_rr_t *rr = self;
	if(rr)
	{
		tnet_dns_rr_init(rr, qtype_any, qclass_any);
	}
	return self;
}

static void* tnet_dns_rr_destroy(void * self) 
{ 
	tnet_dns_rr_t *rr = self;
	if(rr)
	{
		tnet_dns_rr_deinit(rr);
	}
	return self;
}

static const tsk_object_def_t tnet_dns_rr_def_s =
{
	sizeof(tnet_dns_rr_t),
	tnet_dns_rr_create,
	tnet_dns_rr_destroy,
	0,
};
const void *tnet_dns_rr_def_t = &tnet_dns_rr_def_s;