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

#include "../tnet_types.h"

#include "tsk_memory.h"
#include "tsk_debug.h"

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

tnet_dns_rr_t* tnet_dns_rr_deserialize(const void* data, size_t size)
{
	tnet_dns_rr_t *rr = 0;
	const uint8_t* dataPtr = data;
	const uint8_t* dataEnd = (dataPtr+size);
	tnet_dns_qtype_t qtype;
	tnet_dns_qclass_t qclass;
	char* qname = 0;

	/* == Parse QNAME
	*/
	while(dataPtr && dataPtr<dataEnd)
	{
		/* FIXME: do .... */
		dataPtr++;

		if(!*dataPtr)
		{
			/* Terminator found. */
			break;
		}
	}
	
	/* Check validity */
	if(dataPtr>=dataEnd)
	{
		goto bail;
	}

	qtype = (tnet_dns_qtype_t)ntohs(*((uint16_t*)dataPtr));
	dataPtr += 2;
	qclass = (tnet_dns_qclass_t)ntohs(*((uint16_t*)dataPtr));
	dataPtr += 2;

	switch(qtype)
	{
		case qtype_a:
			{
				TSK_DEBUG_ERROR("NOT IMPLEMENTED");
				break;
			}

		case qtype_aaa:
			{
				TSK_DEBUG_ERROR("NOT IMPLEMENTED");
				break;
			}

		case qtype_cname:
			{
				TSK_DEBUG_ERROR("NOT IMPLEMENTED");
				break;
			}

		case qtype_mx:
			{
				break;
			}

		case qtype_naptr:
			{
				TSK_DEBUG_ERROR("NOT IMPLEMENTED");
				break;
			}

		case qtype_ns:
			{
				TSK_DEBUG_ERROR("NOT IMPLEMENTED");
				break;
			}

		case qtype_opt:
			{
				TSK_DEBUG_ERROR("NOT IMPLEMENTED");
				break;
			}

		case qtype_ptr:
			{
				TSK_DEBUG_ERROR("NOT IMPLEMENTED");
				break;
			}

		case qtype_soa:
			{
				TSK_DEBUG_ERROR("NOT IMPLEMENTED");
				break;
			}

		case qtype_srv:
			{
				TSK_DEBUG_ERROR("NOT IMPLEMENTED");
				break;
			}

		case qtype_txt:
			{
				TSK_DEBUG_ERROR("NOT IMPLEMENTED");
				break;
			}

		default:
			{
				TSK_DEBUG_ERROR("NOT IMPLEMENTED");
				break;
			}
	}

bail:
	TSK_FREE(qname);
	return rr;
}


int tnet_dns_rr_serialize(const tnet_dns_rr_t* rr, tsk_buffer_t *output)
{
	if(!rr || !output)
	{
		return -1;
	}

	/* NAME
	*/
	{
		
	}
	
	/* TYPE
	*/
	{
		uint16_t qtype = htons(rr->qtype);
		tsk_buffer_append(output, &(qtype), 2);
	}

	/* CLASS
	*/
	{
		uint16_t qclass = htons(rr->qclass);
		tsk_buffer_append(output, &(qclass), 2);
	}

	/* TTL
	*/
	{
		uint32_t ttl = htonl(rr->ttl);
		tsk_buffer_append(output, &(ttl), 4);
	}

	/* RDLENGTH
	*/
	{
		uint16_t length = htons(rr->rdlength);
		tsk_buffer_append(output, &(length), 2);
	}
	
	/* RDATA
	*/
	
	switch(rr->qtype)
	{
		case qtype_a:
			{
				TSK_DEBUG_ERROR("NOT IMPLEMENTED");
				break;
			}

		case qtype_aaa:
			{
				TSK_DEBUG_ERROR("NOT IMPLEMENTED");
				break;
			}

		case qtype_cname:
			{
				TSK_DEBUG_ERROR("NOT IMPLEMENTED");
				break;
			}

		case qtype_mx:
			{
				TSK_DEBUG_ERROR("NOT IMPLEMENTED");
				break;
			}

		case qtype_naptr:
			{
				TSK_DEBUG_ERROR("NOT IMPLEMENTED");
				break;
			}

		case qtype_ns:
			{
				TSK_DEBUG_ERROR("NOT IMPLEMENTED");
				break;
			}

		case qtype_opt:
			{
				TSK_DEBUG_ERROR("NOT IMPLEMENTED");
				break;
			}

		case qtype_ptr:
			{
				TSK_DEBUG_ERROR("NOT IMPLEMENTED");
				break;
			}

		case qtype_soa:
			{
				TSK_DEBUG_ERROR("NOT IMPLEMENTED");
				break;
			}

		case qtype_srv:
			{
				TSK_DEBUG_ERROR("NOT IMPLEMENTED");
				break;
			}

		case qtype_txt:
			{
				TSK_DEBUG_ERROR("NOT IMPLEMENTED");
				break;
			}

		default:
			{
				TSK_DEBUG_ERROR("NOT IMPLEMENTED");
				break;
			}
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