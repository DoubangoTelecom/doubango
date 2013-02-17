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


/**@file tsdp_header_Z.c
 * @brief SDP "z=" header (Time Zones).
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *
 * 
 */
#include "tinysdp/headers/tsdp_header_Z.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/
%%{
	machine tsdp_machine_parser_header_Z;

	# Includes
	include tsdp_machine_utils "./ragel/tsdp_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}

	action create_zone{
		if(!zone){
			zone = tsdp_zone_create_null();
		}
	}

	action add_zone{
		if(zone){
			tsk_list_push_back_data(hdr_Z->zones,(void**)&zone);
		}
	}

	action parse_time{
		if(zone){
			TSK_PARSER_SET_INTEGER_EX(zone->time, uint64_t, atoi64);
		}
	}

	action parse_typed_time{
		if(zone){
			TSK_PARSER_SET_STRING(zone->typed_time);
		}
	}

	action shifted{
		if(zone){
			zone->shifted_back = tsk_true;
		}
	}	

	fixed_len_time_unit	=  	"d" | "h" | "m" | "s";

	time = DIGIT+ >tag %parse_time;
	typed_time = (DIGIT+ fixed_len_time_unit?) >tag %parse_typed_time;

	Z = 'z' SP* "=" (time SP ("-"%shifted)? typed_time) >create_zone %add_zone 
		(SP time SP ("-"%shifted)? typed_time)* >create_zone %add_zone;
	
	# Entry point
	main := Z :>CRLF?;

}%%


tsdp_header_Z_t* tsdp_header_Z_create(uint64_t time, tsk_bool_t shifted_back, const char* typed_time)
{
	return tsk_object_new(TSDP_HEADER_Z_VA_ARGS(time, shifted_back, typed_time));
}

tsdp_header_Z_t* tsdp_header_Z_create_null()
{
	return tsdp_header_Z_create(0L, tsk_false, tsk_null);
}


tsdp_zone_t* tsdp_zone_create(uint64_t time, tsk_bool_t shifted_back, const char* typed_time) 
{
	return tsk_object_new(tsdp_zone_def_t, time, shifted_back, typed_time);
}

tsdp_zone_t* tsdp_zone_create_null()
{
	return tsdp_zone_create(0L, tsk_false, tsk_null);
}

int tsdp_header_Z_tostring(const tsdp_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tsdp_header_Z_t *Z = (const tsdp_header_Z_t *)header;
		const tsk_list_item_t *item;
		const tsdp_zone_t* zone;
		
		tsk_list_foreach(item, Z->zones){
			zone = (const tsdp_zone_t*)item->data;
			// time  SP ["-"] typed-time
			tsk_buffer_append_2(output, "%s%llu %s%s",
				TSK_LIST_IS_FIRST(Z->zones, item) ? "" : " ",
				zone->time,
				zone->shifted_back ? "-" : "",
				zone->typed_time
			);
		}
	}

	return -1;
}

tsdp_header_Z_t *tsdp_header_Z_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsdp_header_Z_t *hdr_Z = tsdp_header_Z_create_null();
	tsdp_zone_t* zone = tsk_null;
	
	const char *tag_start = tsk_null;

	%%write data;
	(void)(tsdp_machine_parser_header_Z_first_final);
	(void)(tsdp_machine_parser_header_Z_error);
	(void)(tsdp_machine_parser_header_Z_en_main);
	%%write init;
	%%write exec;
	
	if(zone){
		TSK_OBJECT_SAFE_FREE(zone);
	}

	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse \"z=\" header.");
		TSK_OBJECT_SAFE_FREE(hdr_Z);
	}
	
	return hdr_Z;
}







//========================================================
//	Z header object definition
//

static tsk_object_t* tsdp_header_Z_ctor(tsk_object_t *self, va_list * app)
{
	tsdp_header_Z_t *Z = self;
	if(Z){
		TSDP_HEADER(Z)->type = tsdp_htype_Z;
		TSDP_HEADER(Z)->tostring = tsdp_header_Z_tostring;
		TSDP_HEADER(Z)->rank = TSDP_HTYPE_Z_RANK;
		
		if((Z->zones = tsk_list_create())){
			uint64_t time = va_arg(*app, uint64_t);
			unsigned shifted_back = va_arg(*app, unsigned);
			const char* typed_time = va_arg(*app, const char*);

			if(typed_time){
				tsdp_zone_t *zone;
				if((zone = tsdp_zone_create(time, shifted_back, typed_time))){
					tsk_list_push_back_data(Z->zones,(void**)&zone);
				}
			}
		}
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Z header.");
	}
	return self;
}

static tsk_object_t* tsdp_header_Z_dtor(tsk_object_t *self)
{
	tsdp_header_Z_t *Z = self;
	if(Z){
		TSK_OBJECT_SAFE_FREE(Z->zones);
	}
	else{
		TSK_DEBUG_ERROR("Null Z header.");
	}

	return self;
}
static int tsdp_header_Z_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
	if(obj1 && obj2){
		return tsdp_header_rank_cmp(obj1, obj2);
	}
	else{
		return -1;
	}
}

static const tsk_object_def_t tsdp_header_Z_def_s = 
{
	sizeof(tsdp_header_Z_t),
	tsdp_header_Z_ctor,
	tsdp_header_Z_dtor,
	tsdp_header_Z_cmp
};

const tsk_object_def_t *tsdp_header_Z_def_t = &tsdp_header_Z_def_s;




//========================================================
//	Zone object definition
//

static tsk_object_t* tsdp_zone_ctor(tsk_object_t *self, va_list * app)
{
	tsdp_zone_t *zone = self;
	if(zone){
		zone->time = va_arg(*app, uint64_t);
		zone->shifted_back = va_arg(*app, tsk_bool_t);
		zone->typed_time = tsk_strdup( va_arg(*app, const char*) );
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new zone object.");
	}
	return self;
}

static tsk_object_t* tsdp_zone_dtor(tsk_object_t *self)
{
	tsdp_zone_t *zone = self;
	if(zone){
		TSK_FREE(zone->typed_time);
	}
	else{
		TSK_DEBUG_ERROR("Null zone object.");
	}

	return self;
}

static const tsk_object_def_t tsdp_zone_def_s = 
{
	sizeof(tsdp_zone_t),
	tsdp_zone_ctor,
	tsdp_zone_dtor,
	tsk_null
};

const tsk_object_def_t *tsdp_zone_def_t = &tsdp_zone_def_s;
