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


/**@file tsdp_header_R.c
 * @brief SDP "r=" header (Repeat Times).
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *
 * 
 */
#include "tinysdp/headers/tsdp_header_R.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/
%%{
	machine tsdp_machine_parser_header_R;

	# Includes
	include tsdp_machine_utils "./ragel/tsdp_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}

	action parse_repeat_interval{
		TSK_PARSER_SET_STRING(hdr_R->repeat_interval);
	}

	action parse_typed_time{
		if(!hdr_R->typed_time){
			TSK_PARSER_SET_STRING(hdr_R->typed_time);
		}
		else{
			TSK_PARSER_ADD_STRING(hdr_R->typed_times);
		}
	}

	fixed_len_time_unit	=  	"d" | "h" | "m" | "s";

	repeat_interval = (DIGIT+ fixed_len_time_unit?) >tag %parse_repeat_interval;
	typed_time = (DIGIT+ fixed_len_time_unit?) >tag %parse_typed_time;
	
	R = 'r' SP* "=" SP*<: repeat_interval :>SP typed_time (SP<: typed_time)+;
	
	# Entry point
	main := R :>CRLF?;

}%%



tsdp_header_R_t* tsdp_header_R_create()
{
	return tsk_object_new(TSDP_HEADER_R_VA_ARGS());
}

tsdp_header_R_t* tsdp_header_R_create_null()
{
	return tsdp_header_R_create();
}

int tsdp_header_R_tostring(const tsdp_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tsdp_header_R_t *R = (const tsdp_header_R_t *)header;
		const tsk_list_item_t* item;

		// r=7d 1h 0 25h
		tsk_buffer_append_2(output, "%s %s", 
			R->repeat_interval,
			R->typed_time
			);
		tsk_list_foreach(item, R->typed_times){
			tsk_string_t* string = item->data;
			tsk_buffer_append_2(output, " %s", TSK_STRING_STR(string));
		}
		
		return 0;
	}

	return -1;
}

tsdp_header_t* tsdp_header_R_clone(const tsdp_header_t* header)
{
	if(header){
		const tsdp_header_R_t *R = (const tsdp_header_R_t *)header;
		tsdp_header_R_t* clone;
		const tsk_list_item_t* item;

		if((clone = tsdp_header_R_create_null())){
			clone->repeat_interval = tsk_strdup(R->repeat_interval);
			clone->typed_time = tsk_strdup(R->typed_time);

			if(R->typed_times){
				clone->typed_times = tsk_list_create();
			}

			tsk_list_foreach(item, R->typed_times){
				tsk_string_t* string = tsk_string_create(TSK_STRING_STR(item->data));
				tsk_list_push_back_data(clone->typed_times, (void**)&string);
			}
		}
		return TSDP_HEADER(clone);
	}
	return tsk_null;
}

tsdp_header_R_t *tsdp_header_R_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsdp_header_R_t *hdr_R = tsdp_header_R_create_null();
	
	const char *tag_start = tsk_null;

	%%write data;
	(void)(tsdp_machine_parser_header_R_first_final);
	(void)(tsdp_machine_parser_header_R_error);
	(void)(tsdp_machine_parser_header_R_en_main);
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse \"r=\" header.");
		TSK_OBJECT_SAFE_FREE(hdr_R);
	}
	
	return hdr_R;
}







//========================================================
//	E header object definition
//

static tsk_object_t* tsdp_header_R_ctor(tsk_object_t *self, va_list * app)
{
	tsdp_header_R_t *R = self;
	if(R){
		TSDP_HEADER(R)->type = tsdp_htype_R;
		TSDP_HEADER(R)->tostring = tsdp_header_R_tostring;
		TSDP_HEADER(R)->clone = tsdp_header_R_clone;
		TSDP_HEADER(R)->rank = TSDP_HTYPE_R_RANK;
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new E header.");
	}
	return self;
}

static tsk_object_t* tsdp_header_R_dtor(tsk_object_t *self)
{
	tsdp_header_R_t *R = self;
	if(R){
		TSK_FREE(R->repeat_interval);
		TSK_FREE(R->typed_time);
		TSK_OBJECT_SAFE_FREE(R->typed_times);
	}
	else{
		TSK_DEBUG_ERROR("Null R header.");
	}

	return self;
}
static int tsdp_header_R_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
	if(obj1 && obj2){
		return tsdp_header_rank_cmp(obj1, obj2);
	}
	else{
		return -1;
	}
}

static const tsk_object_def_t tsdp_header_R_def_s = 
{
	sizeof(tsdp_header_R_t),
	tsdp_header_R_ctor,
	tsdp_header_R_dtor,
	tsdp_header_R_cmp
};

const tsk_object_def_t *tsdp_header_R_def_t = &tsdp_header_R_def_s;
