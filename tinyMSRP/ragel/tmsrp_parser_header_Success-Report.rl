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

/**@file tmsrp_header_Success_Report.c
 * @brief MSRP 'Success-Report' header.
 */
#include "tinymsrp/headers/tmsrp_header_Success-Report.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/
%%{
	machine tmsrp_machine_parser_header_Success_Report;

	# Includes
	include tmsrp_machine_utils "./ragel/tmsrp_machine_utils.rl";
	
	action is_yes{
		hdr_Success_Report->yes = 1;
	}

	action is_no{
		hdr_Success_Report->yes = 0;
	}
	
	#// "Success-Report:" ( "yes" / "no" )   
	Success_Report = "Success-Report:"i SP ("yes"i %is_yes | "no"i %is_no);
	
	# Entry point
	main := Success_Report :>CRLF?;

}%%

tmsrp_header_Success_Report_t* tmsrp_header_Success_Report_create(tsk_bool_t isSuccess)
{
	return tsk_object_new(TMSRP_HEADER_SUCCESS_REPORT_VA_ARGS(isSuccess));
}

tmsrp_header_Success_Report_t* tmsrp_header_Success_Report_create_null()
{
	return tmsrp_header_Success_Report_create(tsk_false);
}

int tmsrp_header_Success_Report_tostring(const tmsrp_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tmsrp_header_Success_Report_t *Success_Report = (const tmsrp_header_Success_Report_t *)header;
		const char* value = Success_Report->yes ? "yes" : "no";
		return tsk_buffer_append(output, value, tsk_strlen(value));
	}

	return -1;
}

tmsrp_header_Success_Report_t *tmsrp_header_Success_Report_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tmsrp_header_Success_Report_t *hdr_Success_Report = tmsrp_header_Success_Report_create_null();

	TSK_RAGEL_DISABLE_WARNINGS_BEGIN()
	%%write data;
	(void)(eof);
	(void)(tmsrp_machine_parser_header_Success_Report_first_final);
	(void)(tmsrp_machine_parser_header_Success_Report_error);
	(void)(tmsrp_machine_parser_header_Success_Report_en_main);
	%%write init;
	%%write exec;
	TSK_RAGEL_DISABLE_WARNINGS_END()
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse 'Success-Report' header.");
		TSK_OBJECT_SAFE_FREE(hdr_Success_Report);
	}
	
	return hdr_Success_Report;
}







//========================================================
//	Success_Report header object definition
//

static tsk_object_t* tmsrp_header_Success_Report_ctor(tsk_object_t *self, va_list * app)
{
	tmsrp_header_Success_Report_t *Success_Report = self;
	if(Success_Report){
		TMSRP_HEADER(Success_Report)->type = tmsrp_htype_Success_Report;
		TMSRP_HEADER(Success_Report)->tostring = tmsrp_header_Success_Report_tostring;
		
		Success_Report->yes = va_arg(*app, tsk_bool_t) ? 1 : 0;
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Success-Report header.");
	}
	return self;
}

static tsk_object_t* tmsrp_header_Success_Report_dtor(tsk_object_t *self)
{
	tmsrp_header_Success_Report_t *Success_Report = self;
	if(Success_Report){
	}
	else{
		TSK_DEBUG_ERROR("Null Success-Report header.");
	}

	return self;
}

static const tsk_object_def_t tmsrp_header_Success_Report_def_s = 
{
	sizeof(tmsrp_header_Success_Report_t),
	tmsrp_header_Success_Report_ctor,
	tmsrp_header_Success_Report_dtor,
	tsk_null
};

const tsk_object_def_t *tmsrp_header_Success_Report_def_t = &tmsrp_header_Success_Report_def_s;
