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

/**@file tsip_header_User_Agent.c
 * @brief SIP User-Agent/t header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_User_Agent.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_User_Agent;

	# Includes
	include tsip_machine_utils "./ragel/tsip_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}

	action parse_user_agent{
		TSK_PARSER_SET_STRING(hdr_user_agent->value);
	}

	action eob{
	}
		
	User_Agent = "User-Agent"i HCOLON (any*)>tag %parse_user_agent;
	
	# Entry point
	main := User_Agent :>CRLF @eob;

}%%

tsip_header_User_Agent_t* tsip_header_User_Agent_create(const char* ua)
{
	return tsk_object_new(TSIP_HEADER_USER_AGENT_VA_ARGS(ua));
}

tsip_header_User_Agent_t* tsip_header_User_Agent_create_null()
{
	return tsip_header_User_Agent_create(tsk_null);
}

int tsip_header_User_Agent_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tsip_header_User_Agent_t *User_Agent = (const tsip_header_User_Agent_t *)header;
		if(User_Agent->value){
			return tsk_buffer_append(output, User_Agent->value, tsk_strlen(User_Agent->value));
		}
		return 0;
	}

	return -1;
}

tsip_header_User_Agent_t *tsip_header_User_Agent_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_User_Agent_t *hdr_user_agent = tsip_header_User_Agent_create_null();
	
	const char *tag_start = tsk_null;

	%%write data;
	(void)(eof);
	(void)(tsip_machine_parser_header_User_Agent_first_final);
	(void)(tsip_machine_parser_header_User_Agent_error);
	(void)(tsip_machine_parser_header_User_Agent_en_main);
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse 'User-Agent' header.");
		TSK_OBJECT_SAFE_FREE(hdr_user_agent);
	}
	
	return hdr_user_agent;
}







//========================================================
//	User_Agent header object definition
//

static tsk_object_t* tsip_header_User_Agent_ctor(tsk_object_t *self, va_list * app)
{
	tsip_header_User_Agent_t *User_Agent = self;
	if(User_Agent){
		TSIP_HEADER(User_Agent)->type = tsip_htype_User_Agent;
		TSIP_HEADER(User_Agent)->serialize = tsip_header_User_Agent_serialize;
		User_Agent->value = tsk_strdup(va_arg(*app, const char*));
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new User_Agent header.");
	}
	return self;
}

static tsk_object_t* tsip_header_User_Agent_dtor(tsk_object_t *self)
{
	tsip_header_User_Agent_t *User_Agent = self;
	if(User_Agent){
		TSK_FREE(User_Agent->value);
		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(User_Agent));
	}
	else{
		TSK_DEBUG_ERROR("Null User_Agent header.");
	}

	return self;
}

static const tsk_object_def_t tsip_header_User_Agent_def_s = 
{
	sizeof(tsip_header_User_Agent_t),
	tsip_header_User_Agent_ctor,
	tsip_header_User_Agent_dtor,
	tsk_null
};
const tsk_object_def_t *tsip_header_User_Agent_def_t = &tsip_header_User_Agent_def_s;
