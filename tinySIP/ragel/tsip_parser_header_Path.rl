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

/**@file tsip_header_Path.c
 * @brief SIP Service-Path header as per RFC 3608.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_Path.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>



/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_Path;

	# Includes
	include tsip_machine_utils "./ragel/tsip_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}
	
	action create_path{
		if(!curr_path){
			curr_path = tsip_header_Path_create_null();
		}
	}

	action parse_display_name{
		if(curr_path){
			TSK_PARSER_SET_STRING(curr_path->display_name);
			tsk_strunquote(&curr_path->display_name);
		}
	}

	action parse_uri{
		if(curr_path && !curr_path->uri){
			int len = (int)(p  - tag_start);
			if(curr_path && !curr_path->uri){
				if((curr_path->uri = tsip_uri_parse(tag_start, (tsk_size_t)len)) && curr_path->display_name){
					curr_path->uri->display_name = tsk_strdup(curr_path->display_name);
				}
			}
		}
	}

	action parse_param{
		if(curr_path){
			TSK_PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(curr_path));
		}
	}

	action add_path{
		if(curr_path){
			tsk_list_push_back_data(hdr_paths, ((void**) &curr_path));
		}
	}

	action eob{
	}

	
	URI = (scheme HCOLON any+)>tag %parse_uri;
	display_name = (( token LWS )+ | quoted_string)>tag %parse_display_name;
	my_name_addr = display_name? :>LAQUOT<: URI :>RAQUOT;

	rr_param = (generic_param)>tag %parse_param;
	
	path_value	= 	(my_name_addr ( SEMI rr_param )*) >create_path %add_path;
	Path	= 		"Path" HCOLON path_value (COMMA path_value)*;

	# Entry point
	main := Path :>CRLF @eob;

}%%


tsip_header_Path_t* tsip_header_Path_create(const tsip_uri_t* uri)
{
	return tsk_object_new(TSIP_HEADER_PATH_VA_ARGS(uri));
}

tsip_header_Path_t* tsip_header_Path_create_null()
{
	return tsip_header_Path_create(tsk_null);
}

int tsip_header_Path_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tsip_header_Path_t *Path = (const tsip_header_Path_t *)header;
		int ret = 0;
		
		/* Uri with hacked display-name*/
		if((ret = tsip_uri_serialize(Path->uri, tsk_true, tsk_true, output))){
			return ret;
		}
		
		return ret;
	}

	return -1;
}

tsip_header_Paths_L_t *tsip_header_Path_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Paths_L_t *hdr_paths = tsk_list_create();
	
	const char *tag_start = tsk_null;
	tsip_header_Path_t *curr_path = tsk_null;

	%%write data;
	(void)(eof);
	(void)(tsip_machine_parser_header_Path_first_final);
	(void)(tsip_machine_parser_header_Path_error);
	(void)(tsip_machine_parser_header_Path_en_main);
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse 'Path' header.");
		TSK_OBJECT_SAFE_FREE(curr_path);
		TSK_OBJECT_SAFE_FREE(hdr_paths);
	}
	
	return hdr_paths;
}





//========================================================
//	Path header object definition
//

static tsk_object_t* tsip_header_Path_ctor(tsk_object_t *self, va_list * app)
{
	tsip_header_Path_t *Path = self;
	if(Path){
		const tsip_uri_t* uri = va_arg(*app, const tsip_uri_t*);

		TSIP_HEADER(Path)->type = tsip_htype_Path;
		TSIP_HEADER(Path)->serialize = tsip_header_Path_serialize;
		if(uri){
			Path->uri = tsk_object_ref((void*)uri);
		}
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Path header.");
	}
	return self;
}

static tsk_object_t* tsip_header_Path_dtor(tsk_object_t *self)
{
	tsip_header_Path_t *Path = self;
	if(Path){
		TSK_FREE(Path->display_name);
		TSK_OBJECT_SAFE_FREE(Path->uri);

		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Path));
	}
	else{
		TSK_DEBUG_ERROR("Null Path header.");
	}

	return self;
}

static const tsk_object_def_t tsip_header_Path_def_s = 
{
	sizeof(tsip_header_Path_t),
	tsip_header_Path_ctor,
	tsip_header_Path_dtor,
	tsk_null
};
const tsk_object_def_t *tsip_header_Path_def_t = &tsip_header_Path_def_s;
