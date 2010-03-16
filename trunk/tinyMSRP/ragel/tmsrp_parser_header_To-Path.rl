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
* GNU General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

/**@file tmsrp_header_To_Path.c
 * @brief MSRP "To-Path" header.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinyMSRP/headers/tmsrp_header_To-Path.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/
%%{
	machine tmsrp_machine_parser_header_To_Path;

	# Includes
	include tmsrp_machine_utils "./tmsrp_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}

	action parse_uri{
		int len = (int)(p  - tag_start);
		tmsrp_uri_t* uri;
		if((uri = tmsrp_uri_parse(tag_start, (size_t)len))){
			if(!header->uri){
				header->uri = uri;
			}
			else{
				if(!header->otherURIs){
					header->otherURIs = TSK_LIST_CREATE();
				}
				tsk_list_push_back_data(header->otherURIs, ((void**) &uri));
			}
		}
	}
		
	MSRP_URI = (any -- SP)* >tag %parse_uri;

	#//"To-Path:" SP MSRP-URI  *( SP MSRP-URI ) 
	To_Path = "To-Path:"i SP MSRP_URI (SP <:MSRP_URI)*;
	
	# Entry point
	main := To_Path :>CRLF?;

}%%

int tmsrp_header_To_Path_tostring(const tmsrp_header_t* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tmsrp_header_To_Path_t *To_Path = (const tmsrp_header_To_Path_t *)header;
		const tsk_list_item_t *item;

		if(To_Path->uri){
			tmsrp_uri_serialize(To_Path->uri, output);
		}
		tsk_list_foreach(item, To_Path->otherURIs){
			tsk_buffer_append(output, " ", 1);
			tmsrp_uri_serialize(TMSRP_URI(item->data), output);
		}
	}

	return -1;
}

tmsrp_header_To_Path_t *tmsrp_header_To_Path_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tmsrp_header_To_Path_t *header = TMSRP_HEADER_TO_PATH_CREATE_NULL();

	const char *tag_start;

	%%write data;
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse 'To-Path' header.");
		TSK_OBJECT_SAFE_FREE(header);
	}
	
	return header;
}







//========================================================
//	To_Path header object definition
//

static void* tmsrp_header_To_Path_create(void *self, va_list * app)
{
	tmsrp_header_To_Path_t *To_Path = self;
	if(To_Path)
	{
		TMSRP_HEADER(To_Path)->type = tmsrp_htype_To_Path;
		TMSRP_HEADER(To_Path)->tostring = tmsrp_header_To_Path_tostring;
		
		To_Path->uri = tsk_object_ref((void*)va_arg(*app, const tmsrp_uri_t*));
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new To-Path header.");
	}
	return self;
}

static void* tmsrp_header_To_Path_destroy(void *self)
{
	tmsrp_header_To_Path_t *To_Path = self;
	if(To_Path){
		TSK_OBJECT_SAFE_FREE(To_Path->uri);
		TSK_OBJECT_SAFE_FREE(To_Path->otherURIs);
	}
	else{
		TSK_DEBUG_ERROR("Null To-Path header.");
	}

	return self;
}
static int tmsrp_header_To_Path_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
	return -1;
}

static const tsk_object_def_t tmsrp_header_To_Path_def_s = 
{
	sizeof(tmsrp_header_To_Path_t),
	tmsrp_header_To_Path_create,
	tmsrp_header_To_Path_destroy,
	tmsrp_header_To_Path_cmp
};

const void *tmsrp_header_To_Path_def_t = &tmsrp_header_To_Path_def_s;
