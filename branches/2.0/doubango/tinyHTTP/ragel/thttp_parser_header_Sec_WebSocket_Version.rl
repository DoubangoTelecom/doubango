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

/**@file thttp_header_Sec_WebSocket_Version.c
 * @brief WebSocket "Sec-WebSocket-Key" header.
 */
#include "tinyhttp/headers/thttp_header_Sec_WebSocket_Version.h"

#include "tsk_debug.h"

#include <string.h>

%%{
	machine thttp_machine_parser_header_Sec_WebSocket_Version;

	# Includes
	include thttp_machine_utils "./ragel/thttp_machine_utils.rl";
	include thttp_machine_ws "./ragel/thttp_machine_ws.rl";

	action tag { tag_start = p; }
	action eob { }

	action add_version{
		TSK_PARSER_ADD_STRING(hdr_Sec_WebSocket_Version->values);
	}

	Sec_WebSocket_Version_Value = version>tag %add_version ( COMMA version>tag %add_version )*;

	# Entry point
	main := "Sec-WebSocket-Version"i SP* HCOLON SP*<: Sec_WebSocket_Version_Value :>CRLF @eob;
}%%

thttp_header_Sec_WebSocket_Version_t* thttp_header_Sec_WebSocket_Version_create(const char* version)
{
	return tsk_object_new(THTTP_HEADER_SEC_WEBSOCKET_VERSION_VA_ARGS(version));
}

thttp_header_Sec_WebSocket_Version_t* thttp_header_Sec_WebSocket_Version_create_null()
{ 
	return thttp_header_Sec_WebSocket_Version_create(tsk_null);
}


int thttp_header_Sec_WebSocket_Version_tostring(const thttp_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const thttp_header_Sec_WebSocket_Version_t *Sec_WebSocket_Version = (const thttp_header_Sec_WebSocket_Version_t*)header;
		if(Sec_WebSocket_Version->values){
			const tsk_list_item_t* item;
			const char* str;
			tsk_list_foreach(item, Sec_WebSocket_Version->values){
				if((str = TSK_STRING_STR(item->data))){
					tsk_buffer_append(output, str, tsk_strlen(str));
				}
			}
		}
		return 0;
	}

	return -1;
}

thttp_header_Sec_WebSocket_Version_t *thttp_header_Sec_WebSocket_Version_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	thttp_header_Sec_WebSocket_Version_t *hdr_Sec_WebSocket_Version = thttp_header_Sec_WebSocket_Version_create_null();
	
	const char *tag_start = tsk_null;

	TSK_RAGEL_DISABLE_WARNINGS_BEGIN()
	%%write data;
	(void)(eof);
	(void)(thttp_machine_parser_header_Sec_WebSocket_Version_first_final);
	(void)(thttp_machine_parser_header_Sec_WebSocket_Version_error);
	(void)(thttp_machine_parser_header_Sec_WebSocket_Version_en_main);
	%%write init;
	%%write exec;
	TSK_RAGEL_DISABLE_WARNINGS_END()
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse Sec-WebSocket-Version header.");
		TSK_OBJECT_SAFE_FREE(hdr_Sec_WebSocket_Version);
	}
	
	return hdr_Sec_WebSocket_Version;
}







//========================================================
//	Sec_WebSocket_Version header object definition
//

static tsk_object_t* thttp_header_Sec_WebSocket_Version_ctor(tsk_object_t *self, va_list * app)
{
	thttp_header_Sec_WebSocket_Version_t *Sec_WebSocket_Version = self;
	if(Sec_WebSocket_Version){
		const char* version;
		Sec_WebSocket_Version->values = tsk_list_create();
		THTTP_HEADER(Sec_WebSocket_Version)->type = thttp_htype_Sec_WebSocket_Version;
		THTTP_HEADER(Sec_WebSocket_Version)->tostring = thttp_header_Sec_WebSocket_Version_tostring;

		if((version = va_arg(*app, const char*))){
			tsk_string_t* str_ver = tsk_string_create(version);
			if(str_ver){
				tsk_list_push_back_data(Sec_WebSocket_Version->values, ((void**) &str_ver));
			}
		}
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Sec-WebSocket-Version header.");
	}
	return self;
}

static tsk_object_t* thttp_header_Sec_WebSocket_Version_dtor(tsk_object_t *self)
{
	thttp_header_Sec_WebSocket_Version_t *Sec_WebSocket_Version = self;
	if(Sec_WebSocket_Version){
		TSK_OBJECT_SAFE_FREE(Sec_WebSocket_Version->values);
		TSK_OBJECT_SAFE_FREE(THTTP_HEADER_PARAMS(Sec_WebSocket_Version));
	}
	else{
		TSK_DEBUG_ERROR("Null Sec_WebSocket_Version header.");
	}

	return self;
}

static const tsk_object_def_t thttp_header_Sec_WebSocket_Version_def_s = 
{
	sizeof(thttp_header_Sec_WebSocket_Version_t),
	thttp_header_Sec_WebSocket_Version_ctor,
	thttp_header_Sec_WebSocket_Version_dtor,
	tsk_null
};
const tsk_object_def_t *thttp_header_Sec_WebSocket_Version_def_t = &thttp_header_Sec_WebSocket_Version_def_s;
