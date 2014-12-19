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

/**@file tmsrp_header_Byte_Range.c
 * @brief MSRP 'Byte-Range' header.
 */
#include "tinymsrp/headers/tmsrp_header_Byte-Range.h"

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
	include tmsrp_machine_utils "./ragel/tmsrp_machine_utils.rl";
	
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
			hdr_Byte_Range->total = -1;
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

tmsrp_header_Byte_Range_t* tmsrp_header_Byte_Range_create(int64_t start, int64_t end, int64_t total)
{
	return tsk_object_new(TMSRP_HEADER_BYTE_RANGE_VA_ARGS(start, end, total));
}

tmsrp_header_Byte_Range_t* tmsrp_header_Byte_Range_create_null()
{
	return tmsrp_header_Byte_Range_create(1, -1, -1);
}

int tmsrp_header_Byte_Range_tostring(const tmsrp_header_t* header, tsk_buffer_t* output)
{
	if(header){
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
		
		return tsk_buffer_append_2(output, "%s-%s/%s", 
			Byte_Range->start>=0 ? start : "*",
			Byte_Range->end>=0 ? end : "*",
			Byte_Range->total>=0 ? total : "*"
			);
	}

	return -1;
}

tmsrp_header_Byte_Range_t *tmsrp_header_Byte_Range_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tmsrp_header_Byte_Range_t *hdr_Byte_Range = tmsrp_header_Byte_Range_create_null();

	const char *tag_start = tsk_null;

	TSK_RAGEL_DISABLE_WARNINGS_BEGIN()
	%%write data;
	(void)(eof);
	(void)(tmsrp_machine_parser_header_Byte_Range_first_final);
	(void)(tmsrp_machine_parser_header_Byte_Range_error);
	(void)(tmsrp_machine_parser_header_Byte_Range_en_main);
	%%write init;
	%%write exec;
	TSK_RAGEL_DISABLE_WARNINGS_END()
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse 'Byte-Range' header.");
		TSK_OBJECT_SAFE_FREE(hdr_Byte_Range);
	}
	
	return hdr_Byte_Range;
}







//========================================================
//	Byte_Range header object definition
//

static tsk_object_t* tmsrp_header_Byte_Range_ctor(tsk_object_t *self, va_list * app)
{
	tmsrp_header_Byte_Range_t *Byte_Range = self;
	if(Byte_Range){
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

static tsk_object_t* tmsrp_header_Byte_Range_dtor(tsk_object_t *self)
{
	tmsrp_header_Byte_Range_t *Byte_Range = self;
	if(Byte_Range){
	}
	else{
		TSK_DEBUG_ERROR("Null Byte-Range header.");
	}

	return self;
}

static const tsk_object_def_t tmsrp_header_Byte_Range_def_s = 
{
	sizeof(tmsrp_header_Byte_Range_t),
	tmsrp_header_Byte_Range_ctor,
	tmsrp_header_Byte_Range_dtor,
	tsk_null
};

const tsk_object_def_t *tmsrp_header_Byte_Range_def_t = &tmsrp_header_Byte_Range_def_s;
