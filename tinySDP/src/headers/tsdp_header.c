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

/**@file tsdp_header.c
 * @brief Defines a SDP header/line (<type>=<value>).
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysdp/headers/tsdp_header.h"

#include "tinysdp/headers/tsdp_header_Dummy.h"

#include "tsk_string.h"

int tsdp_header_rank_cmp(const tsdp_header_t* hdr1, const tsdp_header_t* hdr2)
{
	if(hdr1 && hdr2){
		return (hdr1->rank - hdr2->rank);
	}
	else{
		return -1;
	}
}

tsdp_header_t* tsdp_header_clone(const tsdp_header_t* self)
{
	if(self){
		return self->clone(self);
	}
	return tsk_null;
}

/** Gets the name of the SDP header with a type equal to @a type. 
 * @param	type	The @a type of the header for which to retrieve the name. 
 *
 * @return	The name of the header.
**/
char tsdp_header_get_name(tsdp_header_type_t type)
{
	switch(type)
	{
		case tsdp_htype_A: return 'a';			
		case tsdp_htype_B: return 'b';
		case tsdp_htype_C: return 'c';
		case tsdp_htype_E: return 'e';
		case tsdp_htype_I: return 'i';
		case tsdp_htype_K: return 'k';
		case tsdp_htype_M: return 'm';
		case tsdp_htype_O: return 'o';
		case tsdp_htype_P: return 'p';
		case tsdp_htype_R: return 'r';
		case tsdp_htype_S: return 's';
		case tsdp_htype_T: return 't';
		case tsdp_htype_U: return 'u';
		case tsdp_htype_V: return 'v';
		case tsdp_htype_Z: return 'z';

		default: return '*';
	}
}

char tsdp_header_get_nameex(const tsdp_header_t *self)
{
	if(self){
		if(self->type == tsdp_htype_Dummy){
			return ((tsdp_header_Dummy_t*)(self))->name;
		}
		else{
			return tsdp_header_get_name(self->type);
		}
	}
	return '*';
}


int tsdp_header_serialize(const tsdp_header_t *self, tsk_buffer_t *output)
{
	static char name;
	int ret = -1;
	if(!self || !output){
		return -1;
	}

	/* Name */
	name = tsdp_header_get_nameex(self);
	tsk_buffer_append_2(output, "%c=", name);

	/* Value */
	if((ret = self->tostring(self, output))){
		// Abort?
	}

	/* CRLF*/
	if(output->size>2){
		if(*(TSK_BUFFER_TO_U8(output)+TSK_BUFFER_SIZE(output)-2) != '\r' 
			&& *(TSK_BUFFER_TO_U8(output)+TSK_BUFFER_SIZE(output)-1) != '\n'){
			ret = tsk_buffer_append(output, "\r\n", 2);
		}
	}
	else{
		ret = tsk_buffer_append(output, "\r\n", 2);
	}

	return ret;
}

char* tsdp_header_tostring(const tsdp_header_t *self)
{
	tsk_buffer_t* output = tsk_buffer_create_null();
	char* str = tsk_null;

	if(!(tsdp_header_serialize(self, output))){
		str = tsk_strndup(TSK_BUFFER_DATA(output), TSK_BUFFER_SIZE(output));
	}
	
	TSK_OBJECT_SAFE_FREE(output);
	return str;
}
