/*
* Copyright (C) 2009-2015 Mamadou DIOP.
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

/**@file tmsrp_header_Message_Id.c
 * @brief MSRP 'Message-Id' header.
 *
 */
#include "tinymsrp/headers/tmsrp_header_Message-ID.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/
%%{
	machine tmsrp_machine_parser_header_Message_Id;

	# Includes
	include tmsrp_machine_utils "./ragel/tmsrp_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}

	action parse_id{
		TSK_PARSER_SET_STRING(hdr_Message_Id->value);
	}
	
	#// "Message-ID:" SP ident  
	Message_Id = "Message-ID:"i SP ident>tag %parse_id;
	
	# Entry point
	main := Message_Id :>CRLF?;

}%%


tmsrp_header_Message_ID_t* tmsrp_header_Message_ID_create(const char* value)
{
	return tsk_object_new(TMSRP_HEADER_MESSAGE_ID_VA_ARGS(value));
}

tmsrp_header_Message_ID_t* tmsrp_header_Message_ID_create_null()
{
	return tmsrp_header_Message_ID_create(tsk_null);
}


int tmsrp_header_Message_ID_tostring(const tmsrp_header_t* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tmsrp_header_Message_ID_t *Message_Id = (const tmsrp_header_Message_ID_t *)header;
		if(Message_Id->value){
			return tsk_buffer_append(output, Message_Id->value, tsk_strlen(Message_Id->value));
		}
		return 0;
	}

	return -1;
}

tmsrp_header_Message_ID_t *tmsrp_header_Message_ID_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tmsrp_header_Message_ID_t *hdr_Message_Id = tmsrp_header_Message_ID_create_null();
	
	const char *tag_start = tsk_null;

	TSK_RAGEL_DISABLE_WARNINGS_BEGIN()
	%%write data;
	(void)(eof);
	(void)(tmsrp_machine_parser_header_Message_Id_first_final);
	(void)(tmsrp_machine_parser_header_Message_Id_error);
	(void)(tmsrp_machine_parser_header_Message_Id_en_main);
	%%write init;
	%%write exec;
	TSK_RAGEL_DISABLE_WARNINGS_END()

	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse 'Message-Id' header.");
		TSK_OBJECT_SAFE_FREE(hdr_Message_Id);
	}
	
	return hdr_Message_Id;
}







//========================================================
//	Message_Id header object definition
//

static tsk_object_t* tmsrp_header_Message_ID_ctor(tsk_object_t *self, va_list * app)
{
	tmsrp_header_Message_ID_t *Message_Id = self;
	if(Message_Id){
		TMSRP_HEADER(Message_Id)->type = tmsrp_htype_Message_ID;
		TMSRP_HEADER(Message_Id)->tostring = tmsrp_header_Message_ID_tostring;
		
		Message_Id->value = tsk_strdup(va_arg(*app, const char*));
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Message-Id header.");
	}
	return self;
}

static tsk_object_t* tmsrp_header_Message_ID_dtor(tsk_object_t *self)
{
	tmsrp_header_Message_ID_t *Message_Id = self;
	if(Message_Id){
		TSK_FREE(Message_Id->value);
	}
	else{
		TSK_DEBUG_ERROR("Null Message-Id header.");
	}

	return self;
}

static const tsk_object_def_t tmsrp_header_Message_ID_def_s = 
{
	sizeof(tmsrp_header_Message_ID_t),
	tmsrp_header_Message_ID_ctor,
	tmsrp_header_Message_ID_dtor,
	tsk_null
};

const tsk_object_def_t *tmsrp_header_Message_ID_def_t = &tmsrp_header_Message_ID_def_s;
