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

/**@file tsip_header_P_Access_Network_Info.c
 * @brief SIP P_Access_Network_Info header as per RFC 3455.
 *
 *   Header field          where  proxy  ACK BYE CAN INV OPT REG
   ___________________________________________________________
   P-Access-Network-Info         dr     -   o   -   o   o   o

   Header field                    SUB NOT PRA INF UPD MSG REF
   ___________________________________________________________
   P-Access-Network-Info            o   o   o   o   o   o   o
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_P_Access_Network_Info.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

#include <string.h>

/**@defgroup tsip_header_P_Access_Network_Info_group SIP P_Access_Network_Info header.
*/

/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_P_Access_Network_Info;

	# Includes
	include tsip_machine_utils "./tsip_machine_utils.rl";
	
	action tag
	{
		tag_start = p;
	}

	action parse_value
	{
		TSK_PARSER_SET_STRING(hdr_ani->value);
	}

	action eob
	{
	}
		
	P_Access_Network_Info = "P-Access-Network-Info"i HCOLON (any*)>tag %parse_value;
	
	# Entry point
	main := P_Access_Network_Info :>CRLF @eob;

}%%

int tsip_header_P_Access_Network_Info_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_P_Access_Network_Info_t *P_Access_Network_Info = header;
		if(P_Access_Network_Info->value)
		{
			tsk_buffer_append(output, P_Access_Network_Info->value, strlen(P_Access_Network_Info->value));
		}
		return 0;
	}

	return -1;
}

tsip_header_P_Access_Network_Info_t *tsip_header_P_Access_Network_Info_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_P_Access_Network_Info_t *hdr_ani = TSIP_HEADER_P_ACCESS_NETWORK_INFO_CREATE_NULL();
	
	const char *tag_start;

	%%write data;
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% )
	{
		TSK_OBJECT_SAFE_FREE(hdr_ani);
	}
	
	return hdr_ani;
}







//========================================================
//	P_Access_Network_Info header object definition
//

/**@ingroup tsip_header_P_Access_Network_Info_group
*/
static void* tsip_header_P_Access_Network_Info_create(void *self, va_list * app)
{
	tsip_header_P_Access_Network_Info_t *P_Access_Network_Info = self;
	if(P_Access_Network_Info)
	{
		P_Access_Network_Info->value = tsk_strdup(va_arg(*app, const char *));
		TSIP_HEADER(P_Access_Network_Info)->type = tsip_htype_P_Access_Network_Info;
		TSIP_HEADER(P_Access_Network_Info)->tostring = tsip_header_P_Access_Network_Info_tostring;
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new P_Access_Network_Info header.");
	}
	return self;
}

/**@ingroup tsip_header_P_Access_Network_Info_group
*/
static void* tsip_header_P_Access_Network_Info_destroy(void *self)
{
	tsip_header_P_Access_Network_Info_t *P_Access_Network_Info = self;
	if(P_Access_Network_Info)
	{
		TSK_FREE(P_Access_Network_Info->value);
	}
	else TSK_DEBUG_ERROR("Null P_Access_Network_Info header.");

	return self;
}

static const tsk_object_def_t tsip_header_P_Access_Network_Info_def_s = 
{
	sizeof(tsip_header_P_Access_Network_Info_t),
	tsip_header_P_Access_Network_Info_create,
	tsip_header_P_Access_Network_Info_destroy,
	0
};
const void *tsip_header_P_Access_Network_Info_def_t = &tsip_header_P_Access_Network_Info_def_s;