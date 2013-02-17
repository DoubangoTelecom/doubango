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

/**@file tsip_header_Server.c
 * @brief SIP Server header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_Server.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

#include <string.h>



/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_Server;

	# Includes
	include tsip_machine_utils "./ragel/tsip_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}

	action parse_server{
		TSK_PARSER_SET_STRING(hdr_server->value);
	}

	action eob{
	}
	
	# product_version = token;
	# product = token ( SLASH product_version )?;
	# server_val = product | comment;
	# Server = "Server"i HCOLON (server_val ( LWS server_val )*)>tag %parse_server;

	Server = "Server"i HCOLON (any*)>tag %parse_server;
	
	# Entry point
	main := Server :>CRLF @eob;

}%%

tsip_header_Server_t* tsip_header_server_create(const char* server)
{
	return tsk_object_new(TSIP_HEADER_SERVER_VA_ARGS(server));
}

tsip_header_Server_t* tsip_header_server_create_null()
{
	return tsip_header_server_create(tsk_null);
}

int tsip_header_Server_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tsip_header_Server_t *Server = (const tsip_header_Server_t *)header;
		if(Server->value){
			return tsk_buffer_append(output, Server->value, tsk_strlen(Server->value));
		}
		return 0;
	}

	return -1;
}

tsip_header_Server_t *tsip_header_Server_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Server_t *hdr_server = tsip_header_server_create_null();
	
	const char *tag_start = tsk_null;

	%%write data;
	(void)(eof);
	(void)(tsip_machine_parser_header_Server_first_final);
	(void)(tsip_machine_parser_header_Server_error);
	(void)(tsip_machine_parser_header_Server_en_main);
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse 'Server' header.");
		TSK_OBJECT_SAFE_FREE(hdr_server);
	}
	
	return hdr_server;
}







//========================================================
//	Server header object definition
//

static tsk_object_t* tsip_header_Server_ctor(tsk_object_t *self, va_list * app)
{
	tsip_header_Server_t *Server = self;
	if(Server){
		TSIP_HEADER(Server)->type = tsip_htype_Server;
		TSIP_HEADER(Server)->serialize = tsip_header_Server_serialize;
		Server->value = tsk_strdup(va_arg(*app, const char*));
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Server header.");
	}
	return self;
}

static tsk_object_t* tsip_header_Server_dtor(tsk_object_t *self)
{
	tsip_header_Server_t *Server = self;
	if(Server){
		TSK_FREE(Server->value);
		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Server));
	}
	else{
		TSK_DEBUG_ERROR("Null Server header.");
	}

	return self;
}

static const tsk_object_def_t tsip_header_Server_def_s = 
{
	sizeof(tsip_header_Server_t),
	tsip_header_Server_ctor,
	tsip_header_Server_dtor,
	tsk_null
};
const tsk_object_def_t *tsip_header_Server_def_t = &tsip_header_Server_def_s;
