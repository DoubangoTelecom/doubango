/*
* Copyright (C) 2010-2015 Mamadou Diop.
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

/**@file thttp_header_Sec_WebSocket_Protocol.c
 * @brief WebSocket "Sec-WebSocket-Protocol" header.
 *
 */
#include "tinyhttp/headers/thttp_header_Sec_WebSocket_Protocol.h"

#include "tsk_debug.h"

#include <string.h>

%%{
	machine thttp_machine_parser_header_Sec_WebSocket_Protocol;

	# Includes
	include thttp_machine_utils "./ragel/thttp_machine_utils.rl";
	include thttp_machine_ws "./ragel/thttp_machine_ws.rl";

	action tag { tag_start = p; }
	action eob { }

	action add_protocol{
		TSK_PARSER_ADD_STRING(hdr_Sec_WebSocket_Protocol->values);
	}

	Sec_WebSocket_Protocol_Value = token>tag %add_protocol ( COMMA token>tag %add_protocol )*;

	# Entry point
	main := "Sec-WebSocket-Protocol"i SP* HCOLON SP*<: Sec_WebSocket_Protocol_Value :>CRLF @eob;
}%%

thttp_header_Sec_WebSocket_Protocol_t* thttp_header_Sec_WebSocket_Protocol_create(const char* protocol)
{
	return tsk_object_new(THTTP_HEADER_SEC_WEBSOCKET_PROTOCOL_VA_ARGS(protocol));
}

thttp_header_Sec_WebSocket_Protocol_t* thttp_header_Sec_WebSocket_Protocol_create_null()
{ 
	return thttp_header_Sec_WebSocket_Protocol_create(tsk_null);
}


int thttp_header_Sec_WebSocket_Protocol_tostring(const thttp_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const thttp_header_Sec_WebSocket_Protocol_t *Sec_WebSocket_Protocol = (const thttp_header_Sec_WebSocket_Protocol_t*)header;
		if(Sec_WebSocket_Protocol->values){
			const tsk_list_item_t* item;
			const char* str;
			tsk_list_foreach(item, Sec_WebSocket_Protocol->values){
				if((str = TSK_STRING_STR(item->data))){
					tsk_buffer_append(output, str, tsk_strlen(str));
				}
			}
		}
		return 0;
	}

	return -1;
}

thttp_header_Sec_WebSocket_Protocol_t *thttp_header_Sec_WebSocket_Protocol_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	thttp_header_Sec_WebSocket_Protocol_t *hdr_Sec_WebSocket_Protocol = thttp_header_Sec_WebSocket_Protocol_create_null();
	
	const char *tag_start = tsk_null;

	TSK_RAGEL_DISABLE_WARNINGS_BEGIN()
	%%write data;
	(void)(eof);
	(void)(thttp_machine_parser_header_Sec_WebSocket_Protocol_first_final);
	(void)(thttp_machine_parser_header_Sec_WebSocket_Protocol_error);
	(void)(thttp_machine_parser_header_Sec_WebSocket_Protocol_en_main);
	%%write init;
	%%write exec;
	TSK_RAGEL_DISABLE_WARNINGS_END()
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse Sec-WebSocket-Protocol header.");
		TSK_OBJECT_SAFE_FREE(hdr_Sec_WebSocket_Protocol);
	}
	
	return hdr_Sec_WebSocket_Protocol;
}







//========================================================
//	Sec_WebSocket_Protocol header object definition
//

static tsk_object_t* thttp_header_Sec_WebSocket_Protocol_ctor(tsk_object_t *self, va_list * app)
{
	thttp_header_Sec_WebSocket_Protocol_t *Sec_WebSocket_Protocol = self;
	if(Sec_WebSocket_Protocol){
		const char* protocol;
		Sec_WebSocket_Protocol->values = tsk_list_create();
		THTTP_HEADER(Sec_WebSocket_Protocol)->type = thttp_htype_Sec_WebSocket_Protocol;
		THTTP_HEADER(Sec_WebSocket_Protocol)->tostring = thttp_header_Sec_WebSocket_Protocol_tostring;

		if((protocol = va_arg(*app, const char*))){
			tsk_string_t* str_ver = tsk_string_create(protocol);
			if(str_ver){
				tsk_list_push_back_data(Sec_WebSocket_Protocol->values, ((void**) &str_ver));
			}
		}
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Sec-WebSocket-Protocol header.");
	}
	return self;
}

static tsk_object_t* thttp_header_Sec_WebSocket_Protocol_dtor(tsk_object_t *self)
{
	thttp_header_Sec_WebSocket_Protocol_t *Sec_WebSocket_Protocol = self;
	if(Sec_WebSocket_Protocol){
		TSK_OBJECT_SAFE_FREE(Sec_WebSocket_Protocol->values);
		TSK_OBJECT_SAFE_FREE(THTTP_HEADER_PARAMS(Sec_WebSocket_Protocol));
	}
	else{
		TSK_DEBUG_ERROR("Null Sec_WebSocket_Protocol header.");
	}

	return self;
}

static const tsk_object_def_t thttp_header_Sec_WebSocket_Protocol_def_s = 
{
	sizeof(thttp_header_Sec_WebSocket_Protocol_t),
	thttp_header_Sec_WebSocket_Protocol_ctor,
	thttp_header_Sec_WebSocket_Protocol_dtor,
	tsk_null
};
const tsk_object_def_t *thttp_header_Sec_WebSocket_Protocol_def_t = &thttp_header_Sec_WebSocket_Protocol_def_s;
