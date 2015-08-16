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
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_P_Access_Network_Info.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

#include <string.h>



/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_P_Access_Network_Info;

	# Includes
	include tsip_machine_utils "./ragel/tsip_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}

	action parse_value{
		TSK_PARSER_SET_STRING(hdr_ani->value);
	}

	action eob{
	}
		
	P_Access_Network_Info = "P-Access-Network-Info"i HCOLON (any*)>tag %parse_value;
	
	# Entry point
	main := P_Access_Network_Info :>CRLF @eob;

}%%


tsip_header_P_Access_Network_Info_t* tsip_header_P_Access_Network_Info_create(const char* value)
{
	return tsk_object_new(TSIP_HEADER_P_ACCESS_NETWORK_INFO_VA_ARGS(value));
}

tsip_header_P_Access_Network_Info_t* tsip_header_P_Access_Network_Info_create_null()
{
	return tsip_header_P_Access_Network_Info_create(tsk_null);
}

int tsip_header_P_Access_Network_Info_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tsip_header_P_Access_Network_Info_t *P_Access_Network_Info = (const tsip_header_P_Access_Network_Info_t *)header;
		if(P_Access_Network_Info->value){
			tsk_buffer_append(output, P_Access_Network_Info->value, tsk_strlen(P_Access_Network_Info->value));
		}
		return 0;
	}

	return -1;
}

tsip_header_P_Access_Network_Info_t *tsip_header_P_Access_Network_Info_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_P_Access_Network_Info_t *hdr_ani = tsip_header_P_Access_Network_Info_create_null();
	
	const char *tag_start = tsk_null;

	%%write data;
	(void)(eof);
	(void)(tsip_machine_parser_header_P_Access_Network_Info_first_final);
	(void)(tsip_machine_parser_header_P_Access_Network_Info_error);
	(void)(tsip_machine_parser_header_P_Access_Network_Info_en_main);
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse 'P-Access-Network-Info' header.");
		TSK_OBJECT_SAFE_FREE(hdr_ani);
	}
	
	return hdr_ani;
}







//========================================================
//	P_Access_Network_Info header object definition
//

static tsk_object_t* tsip_header_P_Access_Network_Info_ctor(tsk_object_t *self, va_list * app)
{
	tsip_header_P_Access_Network_Info_t *P_Access_Network_Info = self;
	if(P_Access_Network_Info){
		P_Access_Network_Info->value = tsk_strdup(va_arg(*app, const char *));
		TSIP_HEADER(P_Access_Network_Info)->type = tsip_htype_P_Access_Network_Info;
		TSIP_HEADER(P_Access_Network_Info)->serialize = tsip_header_P_Access_Network_Info_serialize;
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new P_Access_Network_Info header.");
	}
	return self;
}

static tsk_object_t* tsip_header_P_Access_Network_Info_dtor(tsk_object_t *self)
{
	tsip_header_P_Access_Network_Info_t *P_Access_Network_Info = self;
	if(P_Access_Network_Info){
		TSK_FREE(P_Access_Network_Info->value);
	}
	else{
		TSK_DEBUG_ERROR("Null P_Access_Network_Info header.");
	}

	return self;
}

static const tsk_object_def_t tsip_header_P_Access_Network_Info_def_s = 
{
	sizeof(tsip_header_P_Access_Network_Info_t),
	tsip_header_P_Access_Network_Info_ctor,
	tsip_header_P_Access_Network_Info_dtor,
	tsk_null
};
const tsk_object_def_t *tsip_header_P_Access_Network_Info_def_t = &tsip_header_P_Access_Network_Info_def_s;
