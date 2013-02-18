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


/**@file tsdp_header_A.c
 * @brief SDP "a=" header (Attributes).
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *
 * 
 */
#include "tinysdp/headers/tsdp_header_A.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/
%%{
	machine tsdp_machine_parser_header_A;

	# Includes
	include tsdp_machine_utils "./ragel/tsdp_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}
	
	action parse_field{
		TSK_PARSER_SET_STRING(hdr_A->field);
	}
	
	action parse_value{
		TSK_PARSER_SET_STRING(hdr_A->value);
	}
	
	field = token>tag %parse_field;
	value = any*>tag %parse_value;
	
	A = 'a' SP* "=" SP*<: ((field ":" value) | (field));
	
	# Entry point
	main := A :>CRLF?;

}%%



tsdp_header_A_t* tsdp_header_A_create(const char* field, const char* value)
{
        return tsk_object_new(TSDP_HEADER_A_VA_ARGS(field, value));
}

tsdp_header_A_t* tsdp_header_A_create_null()
{
        return tsdp_header_A_create(tsk_null, tsk_null);
}


int tsdp_header_A_tostring(const tsdp_header_t* header, tsk_buffer_t* output)
{
        if(header){
                const tsdp_header_A_t *A = (const tsdp_header_A_t *)header;
                
                return tsk_buffer_append_2(output, "%s%s%s",
                        A->field,

                        A->value ? ":" : "",
                        A->value ? A->value : ""
                        );
        }

        return -1;
}

tsdp_header_t* tsdp_header_A_clone(const tsdp_header_t* header)
{
        if(header){
                const tsdp_header_A_t *A = (const tsdp_header_A_t *)header;
                return (tsdp_header_t*)tsdp_header_A_create(A->field, A->value);
        }
        return tsk_null;
}

tsdp_header_A_t *tsdp_header_A_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsdp_header_A_t *hdr_A = tsdp_header_A_create_null();
	
	const char *tag_start = tsk_null;

	%%write data;
	%%write init;
	(void)(tsdp_machine_parser_header_A_first_final);
	(void)(tsdp_machine_parser_header_A_error);
	(void)(tsdp_machine_parser_header_A_en_main);
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse \"a=\" header.");
		TSK_OBJECT_SAFE_FREE(hdr_A);
	}
	
	return hdr_A;
}


int tsdp_header_A_removeAll_by_field(tsdp_headers_A_L_t *attributes, const char* field)
{
        tsk_list_item_t* item;
        const tsdp_header_A_t* A;

        if(!attributes || !field){
                TSK_DEBUG_ERROR("Invalid parameter");
                return -1;
        }

again:
        tsk_list_foreach(item, attributes){
                if(!(A = item->data) || TSDP_HEADER(A)->type != tsdp_htype_A){
                        continue;
                }
                if(tsk_striequals(field, A->field)){
                        tsk_list_remove_item(attributes, item);
                        goto again;
                }
        }

        return 0;
}

int tsdp_header_A_removeAll_by_fields(tsdp_headers_A_L_t *attributes, const char** fields, tsk_size_t fields_count)
{
        tsk_size_t i;

        if(!attributes || !fields){
                TSK_DEBUG_ERROR("Invalid parameter");
                return -1;
        }

        for(i = 0; i < fields_count; ++i){
                if(!fields[i]){
                        continue;
                }
                tsdp_header_A_removeAll_by_field(attributes, fields[i]);
        }
        return 0;
}




//========================================================
//      A header object definition
//

static tsk_object_t* tsdp_header_A_ctor(tsk_object_t *self, va_list * app)
{
        tsdp_header_A_t *A = self;
        if(A)
        {
                TSDP_HEADER(A)->type = tsdp_htype_A;
                TSDP_HEADER(A)->tostring = tsdp_header_A_tostring;
                TSDP_HEADER(A)->clone = tsdp_header_A_clone;
                TSDP_HEADER(A)->rank = TSDP_HTYPE_A_RANK;
                
                A->field = tsk_strdup(va_arg(*app, const char*));
                A->value = tsk_strdup(va_arg(*app, const char*));
        }
        else{
                TSK_DEBUG_ERROR("Failed to create new A header.");
        }
        return self;
}

static tsk_object_t* tsdp_header_A_dtor(tsk_object_t *self)
{
        tsdp_header_A_t *A = self;
        if(A){
                TSK_FREE(A->field);
                TSK_FREE(A->value);
        }
        else{
                TSK_DEBUG_ERROR("Null A header.");
        }

        return self;
}
static int tsdp_header_A_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
        if(obj1 && obj2){
                return tsdp_header_rank_cmp(obj1, obj2);
        }
        else{
                return -1;
        }
}

static const tsk_object_def_t tsdp_header_A_def_s = 
{
        sizeof(tsdp_header_A_t),
        tsdp_header_A_ctor,
        tsdp_header_A_dtor,
        tsdp_header_A_cmp
};

const tsk_object_def_t *tsdp_header_A_def_t = &tsdp_header_A_def_s;
