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

/**@file tmsrp_header_Byte_Range.c
 * @brief MSRP 'Byte-Range' header.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinyMSRP/headers/tmsrp_header_Byte-Range.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/
%%{
	machine tmsrp_machine_parser_header_Byte_Range;

	# Includes
	include tmsrp_machine_utils "./tmsrp_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}
	
	action parse_start{
		TSK_PARSER_SET_INTEGER_EX(hdr_Byte_Range->start, int64_t, atoi64);
	}
	
	action parse_end{
		if(tag_start && *tag_start == '*'){
			hdr_Byte_Range->end = -1;
		}
		else{
			TSK_PARSER_SET_INTEGER_EX(hdr_Byte_Range->end, int64_t, atoi64);
		}
	}
	
	action parse_total{
		if(tag_start && *tag_start == '*'){
			hdr_Byte_Range->end = -1;
		}
		else{
			TSK_PARSER_SET_INTEGER_EX(hdr_Byte_Range->total, int64_t, atoi64);
		}
	}

	#// Byte-Range	=  	 "Byte-Range:" SP range-start  "-" range-end  "/" total
	#//range-start	= 	1*DIGIT
	#//range-end	= 	1*DIGIT / "*"
	#//total		= 	1*DIGIT / "*"
	Byte_Range = "Byte-Range:"i SP DIGIT+>tag %parse_start "-" (DIGIT+ | "*")>tag %parse_end "/" (DIGIT+ | "*")>tag %parse_total;
	
	# Entry point
	main := Byte_Range :>CRLF?;

}%%

int tmsrp_header_Byte_Range_tostring(const tmsrp_header_t* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tmsrp_header_Byte_Range_t *Byte_Range = (const tmsrp_header_Byte_Range_t *)header;
		tsk_istr_t start, end, total;

		if(Byte_Range->start>=0){
			tsk_itoa(Byte_Range->start, &start);
		}
		if(Byte_Range->end>=0){
			tsk_itoa(Byte_Range->end, &end);
		}
		if(Byte_Range->total>=0){
			tsk_itoa(Byte_Range->total, &total);
		}
		
		return tsk_buffer_appendEx(output, "%s-%s/%s", 
			Byte_Range->start>=0 ? start : "*",
			Byte_Range->end>=0 ? end : "*",
			Byte_Range->total>=0 ? total : "*"
			);
	}

	return -1;
}

tmsrp_header_Byte_Range_t *tmsrp_header_Byte_Range_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tmsrp_header_Byte_Range_t *hdr_Byte_Range = TMSRP_HEADER_BYTE_RANGE_CREATE_NULL();

	const char *tag_start;

	%%write data;
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse 'Byte-Range' header.");
		TSK_OBJECT_SAFE_FREE(hdr_Byte_Range);
	}
	
	return hdr_Byte_Range;
}







//========================================================
//	Byte_Range header object definition
//

static void* tmsrp_header_Byte_Range_create(void *self, va_list * app)
{
	tmsrp_header_Byte_Range_t *Byte_Range = self;
	if(Byte_Range)
	{
		TMSRP_HEADER(Byte_Range)->type = tmsrp_htype_Byte_Range;
		TMSRP_HEADER(Byte_Range)->tostring = tmsrp_header_Byte_Range_tostring;
		
		Byte_Range->start = va_arg(*app, int64_t);
		Byte_Range->end = va_arg(*app, int64_t);
		Byte_Range->total = va_arg(*app, int64_t);
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Byte-Range header.");
	}
	return self;
}

static void* tmsrp_header_Byte_Range_destroy(void *self)
{
	tmsrp_header_Byte_Range_t *Byte_Range = self;
	if(Byte_Range){
	}
	else{
		TSK_DEBUG_ERROR("Null Byte-Range header.");
	}

	return self;
}
static int tmsrp_header_Byte_Range_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
	return -1;
}

static const tsk_object_def_t tmsrp_header_Byte_Range_def_s = 
{
	sizeof(tmsrp_header_Byte_Range_t),
	tmsrp_header_Byte_Range_create,
	tmsrp_header_Byte_Range_destroy,
	tmsrp_header_Byte_Range_cmp
};

const void *tmsrp_header_Byte_Range_def_t = &tmsrp_header_Byte_Range_def_s;
