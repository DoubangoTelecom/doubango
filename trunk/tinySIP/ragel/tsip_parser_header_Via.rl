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

/**@file tsip_header_Via.c
 * @brief SIP Via/v header as per RFC 3261 subclause 20.42.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_Via.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

/**@defgroup tsip_header_Via_group SIP Via header.
*/

/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_Via;

	# Includes
	include tsip_machine_utils "./tsip_machine_utils.rl";


	action tag
	{
		TSK_DEBUG_INFO("VIA:TAG");
		tag_start = p;
	}

	action parse_protocol_name
	{
		int len = (int)(p  - tag_start);
		hdr_via->proto_name = tsk_calloc(1, len+1);
		memcpy(hdr_via->proto_name, tag_start, len);
	}

	action parse_protocol_version
	{
		int len = (int)(p  - tag_start);
		hdr_via->proto_version = tsk_calloc(1, len+1);
		memcpy(hdr_via->proto_version, tag_start, len);
	}

	action parse_host
	{
		int len = (int)(p  - tag_start);
		hdr_via->host = tsk_calloc(1, len+1);
		memcpy(hdr_via->host, tag_start, len);
	}

	action parse_port
	{
		int len = (int)(p  - tag_start);
		if(len)
		{
			char* tmp = tsk_calloc(1, len+1);
			memcpy(tmp, tag_start, len);
			hdr_via->port = atoi(tmp);
			free(tmp);	
		}
	}

	action parse_transport
	{
		int len = (int)(p  - tag_start);
		hdr_via->transport = tsk_calloc(1, len+1);
		memcpy(hdr_via->transport, tag_start, len);
	}

	action parse_ttl
	{
		int len = (int)(p  - tag_start);
		if(len)
		{
			char* tmp = tsk_calloc(1, len+1);
			memcpy(tmp, tag_start, len);
			hdr_via->ttl = atoi(tmp);
			free(tmp);	
		}
	}

	action parse_maddr
	{
		int len = (int)(p  - tag_start);
		hdr_via->maddr = tsk_calloc(1, len+1);
		memcpy(hdr_via->maddr, tag_start, len);
	}
	
	action parse_received
	{
		int len = (int)(p  - tag_start);
		hdr_via->received = tsk_calloc(1, len+1);
		memcpy(hdr_via->received, tag_start, len);
	}

	action parse_branch
	{
		int len = (int)(p  - tag_start);
		hdr_via->branch = tsk_calloc(1, len+1);
		memcpy(hdr_via->branch, tag_start, len);
	}

	action parse_comp
	{
		int len = (int)(p  - tag_start);
		hdr_via->comp = tsk_calloc(1, len+1);
		memcpy(hdr_via->comp, tag_start, len);
	}

	action parse_rport
	{
		int len = (int)(p  - tag_start);

		if(len)
		{
			char* tmp = tsk_calloc(1, len+1);
			memcpy(tmp, tag_start, len);
			hdr_via->rport = atoi(tmp);
			free(tmp);	
		}
	}
	
	action eob
	{
		
	}

	protocol_name = "SIP"i | token >tag %parse_protocol_name;
	protocol_version = token >tag %parse_protocol_version;
	transport = "UDP"i | "TCP"i | "TLS"i | "SCTP"i | "TLS-SCTP"i | other_transport >tag %parse_transport;
	sent_protocol = protocol_name SLASH protocol_version SLASH transport;
	sent_by = host>tag %parse_host ( COLON port >tag %parse_port )?;
	via_ttl = "ttl"i EQUAL ttl >tag %parse_ttl;
	via_maddr = "maddr"i EQUAL host >tag %parse_maddr;
	via_received = "received"i EQUAL ( IPv4address | IPv6address )>tag %parse_received;
	via_branch = "branch"i EQUAL token >tag %parse_branch;
	via_compression = "comp"i EQUAL ( "sigcomp"i | other_compression )>tag %parse_comp;
	response_port = "rport"i ( EQUAL DIGIT+ >tag %parse_rport )?;
	via_extension = generic_param;
	via_params = via_ttl | via_maddr | via_received | via_branch | via_compression | response_port | via_extension;
	via_parm = sent_protocol LWS sent_by ( SEMI via_params )*;
	Via = ( "Via"i | "v"i ) HCOLON via_parm ( COMMA via_parm )* @eob;
	
	# Entry point
	main := Via;
}%%

tsip_header_Via_t *tsip_header_Via_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = 0;
	tsip_header_Via_t *hdr_via = TSIP_HEADER_VIA_CREATE();
	
	const char *tag_start;

	%%write data;
	%%write init;
	%%write exec;
	
	if(cs == tsip_machine_parser_header_Via_error)
	{
		TSIP_HEADER_VIA_SAFE_FREE(hdr_via);
	}
	
	return hdr_via;
}











//========================================================
//	Via header object definition
//

/**@ingroup tsip_header_Via_group
*/
static void* tsip_header_Via_create(void *self, va_list * app)
{
	tsip_header_Via_t *via = self;
	if(via)
	{
		via->type = tsip_htype_Via;
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new Via header.");
	}
	return self;
}

/**@ingroup tsip_header_Via_group
*/
static void* tsip_header_Via_destroy(void *self)
{
	tsip_header_Via_t *via = self;
	if(via)
	{
		TSK_FREE(via->branch);
		TSK_FREE(via->comp);
		TSK_FREE(via->host);
		TSK_FREE(via->maddr);
		TSK_FREE(via->proto_name);
		TSK_FREE(via->proto_version);
		TSK_FREE(via->received);
		TSK_FREE(via->sigcomp_id);
		TSK_FREE(via->transport);
	}
	else TSK_DEBUG_ERROR("Null Via header.");

	return self;
}

static const tsk_object_def_t tsip_header_Via_def_s = 
{
	sizeof(tsip_header_Via_t),
	tsip_header_Via_create,
	tsip_header_Via_destroy,
	0
};
const void *tsip_header_Via_def_t = &tsip_header_Via_def_s;