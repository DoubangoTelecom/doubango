/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)yahoo.fr>
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
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinyMSRP/headers/tmsrp_header_Message-Id.h"

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
	include tmsrp_machine_utils "./tmsrp_machine_utils.rl";
	
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

int tmsrp_header_Message_ID_tostring(const tmsrp_header_t* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tmsrp_header_Message_ID_t *Message_Id = (const tmsrp_header_Message_ID_t *)header;
		if(Message_Id->value){
			return tsk_buffer_append(output, Message_Id->value, strlen(Message_Id->value));
		}
		return 0;
	}

	return -1;
}

tmsrp_header_Message_ID_t *tmsrp_header_Message_ID_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tmsrp_header_Message_ID_t *hdr_Message_Id = TMSRP_HEADER_MESSAGE_ID_CREATE_NULL();
	
	const char *tag_start;

	%%write data;
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse 'Message-Id' header.");
		TSK_OBJECT_SAFE_FREE(hdr_Message_Id);
	}
	
	return hdr_Message_Id;
}







//========================================================
//	Message_Id header object definition
//

static void* tmsrp_header_Message_ID_create(void *self, va_list * app)
{
	tmsrp_header_Message_ID_t *Message_Id = self;
	if(Message_Id)
	{
		TMSRP_HEADER(Message_Id)->type = tmsrp_htype_Message_ID;
		TMSRP_HEADER(Message_Id)->tostring = tmsrp_header_Message_ID_tostring;
		
		Message_Id->value = tsk_strdup(va_arg(*app, const char*));
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Message-Id header.");
	}
	return self;
}

static void* tmsrp_header_Message_ID_destroy(void *self)
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
static int tmsrp_header_Message_ID_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
	return -1;
}

static const tsk_object_def_t tmsrp_header_Message_ID_def_s = 
{
	sizeof(tmsrp_header_Message_ID_t),
	tmsrp_header_Message_ID_create,
	tmsrp_header_Message_ID_destroy,
	tmsrp_header_Message_ID_cmp
};

const void *tmsrp_header_Message_ID_def_t = &tmsrp_header_Message_ID_def_s;
