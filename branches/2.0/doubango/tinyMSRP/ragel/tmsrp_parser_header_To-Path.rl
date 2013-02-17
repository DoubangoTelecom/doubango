/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango.org>
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
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#include "tinymsrp/headers/tmsrp_header_To-Path.h"

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
	include tmsrp_machine_utils "./ragel/tmsrp_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}

	action parse_uri{
		int len = (int)(p  - tag_start);
		tmsrp_uri_t* uri;
		if((uri = tmsrp_uri_parse(tag_start, (tsk_size_t)len))){
			if(!header->uri){
				header->uri = uri;
			}
			else{
				if(!header->otherURIs){
					header->otherURIs = tsk_list_create();
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

tmsrp_header_To_Path_t* tmsrp_header_To_Path_create(const tmsrp_uri_t* uri)
{
	return tsk_object_new(TMSRP_HEADER_TO_PATH_VA_ARGS(uri));
}

tmsrp_header_To_Path_t* tmsrp_header_To_Path_create_null()
{
	return tmsrp_header_To_Path_create(tsk_null);
}

int tmsrp_header_To_Path_tostring(const tmsrp_header_t* header, tsk_buffer_t* output)
{
	if(header){
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

tmsrp_header_To_Path_t *tmsrp_header_To_Path_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tmsrp_header_To_Path_t *header = tmsrp_header_To_Path_create_null();

	const char *tag_start = tsk_null;

	%%write data;
	(void)(eof);
	(void)(tmsrp_machine_parser_header_To_Path_first_final);
	(void)(tmsrp_machine_parser_header_To_Path_error);
	(void)(tmsrp_machine_parser_header_To_Path_en_main);
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse 'To-Path' header.");
		TSK_OBJECT_SAFE_FREE(header);
	}
	
	return header;
}

tmsrp_header_To_Path_t *tmsrp_header_To_Path_clone(const tmsrp_header_To_Path_t* To_Path)
{
	tmsrp_header_To_Path_t* clone = tsk_null;
	
	if(!To_Path){
		goto bail;
	}

	clone = tmsrp_header_To_Path_create_null();
	clone->uri = tmsrp_uri_clone(To_Path->uri);
	if(To_Path->otherURIs){
		tsk_list_item_t *item;
		clone->otherURIs = tsk_list_create();

		tsk_list_foreach(item, To_Path->otherURIs){
			tmsrp_uri_t *uri = tmsrp_uri_clone(TMSRP_URI(item->data));
			tsk_list_push_back_data(clone->otherURIs, (void**)&uri);
		}
	}

bail:
	return clone;
}





//========================================================
//	To_Path header object definition
//

static tsk_object_t* tmsrp_header_To_Path_ctor(tsk_object_t *self, va_list * app)
{
	tmsrp_header_To_Path_t *To_Path = self;
	if(To_Path){
		TMSRP_HEADER(To_Path)->type = tmsrp_htype_To_Path;
		TMSRP_HEADER(To_Path)->tostring = tmsrp_header_To_Path_tostring;
		
		To_Path->uri = tsk_object_ref((void*)va_arg(*app, const tmsrp_uri_t*));
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new To-Path header.");
	}
	return self;
}

static tsk_object_t* tmsrp_header_To_Path_dtor(tsk_object_t *self)
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

static const tsk_object_def_t tmsrp_header_To_Path_def_s = 
{
	sizeof(tmsrp_header_To_Path_t),
	tmsrp_header_To_Path_ctor,
	tmsrp_header_To_Path_dtor,
	tsk_null
};

const tsk_object_def_t *tmsrp_header_To_Path_def_t = &tmsrp_header_To_Path_def_s;
