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

/**@file tmsrp_header_Failure_Report.c
 * @brief MSRP 'Failure-Report' header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#include "tinymsrp/headers/tmsrp_header_Failure-Report.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/
%%{
	machine tmsrp_machine_parser_header_Failure_Report;

	# Includes
	include tmsrp_machine_utils "./ragel/tmsrp_machine_utils.rl";
	
	action is_yes{
		hdr_Failure_Report->type = freport_yes;
	}

	action is_no{
		hdr_Failure_Report->type = freport_no;
	}

	action ispartial{
		hdr_Failure_Report->type = freport_partial;
	}
	
	#// "Failure-Report:" ( "yes" / "no" / "partial" )  
	Failure_Report = "Failure-Report:"i SP ("yes"i %is_yes | "no"i %is_no | "partial"i %ispartial);
	
	# Entry point
	main := Failure_Report :>CRLF?;

}%%


tmsrp_header_Failure_Report_t* tmsrp_header_Failure_Report_create(tmsrp_freport_type_t freport_type)
{
	return tsk_object_new(TMSRP_HEADER_FAILURE_REPORT_VA_ARGS(freport_type));
}

tmsrp_header_Failure_Report_t* tmsrp_header_Failure_Report_create_null()
{
	return tmsrp_header_Failure_Report_create(freport_yes);
}

int tmsrp_header_Failure_Report_tostring(const tmsrp_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tmsrp_header_Failure_Report_t *Failure_Report = (const tmsrp_header_Failure_Report_t *)header;
		const char* value = (Failure_Report->type == freport_yes) ? "yes" : (Failure_Report->type == freport_no ? "no" : "partial");
		return tsk_buffer_append(output, value, strlen(value));
	}

	return -1;
}

tmsrp_header_Failure_Report_t *tmsrp_header_Failure_Report_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tmsrp_header_Failure_Report_t *hdr_Failure_Report = tmsrp_header_Failure_Report_create_null();

	%%write data;
	(void)(eof);
	(void)(tmsrp_machine_parser_header_Failure_Report_first_final);
	(void)(tmsrp_machine_parser_header_Failure_Report_error);
	(void)(tmsrp_machine_parser_header_Failure_Report_en_main);
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse 'Failure-Report' header.");
		TSK_OBJECT_SAFE_FREE(hdr_Failure_Report);
	}
	
	return hdr_Failure_Report;
}







//========================================================
//	Failure_Report header object definition
//

static tsk_object_t* tmsrp_header_Failure_Report_ctor(tsk_object_t *self, va_list * app)
{
	tmsrp_header_Failure_Report_t *Failure_Report = self;
	if(Failure_Report){
		TMSRP_HEADER(Failure_Report)->type = tmsrp_htype_Failure_Report;
		TMSRP_HEADER(Failure_Report)->tostring = tmsrp_header_Failure_Report_tostring;
		
		Failure_Report->type = va_arg(*app, tmsrp_freport_type_t);
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Failure-Report header.");
	}
	return self;
}

static tsk_object_t* tmsrp_header_Failure_Report_dtor(tsk_object_t *self)
{
	tmsrp_header_Failure_Report_t *Failure_Report = self;
	if(Failure_Report){
	}
	else{
		TSK_DEBUG_ERROR("Null Failure-Report header.");
	}

	return self;
}

static const tsk_object_def_t tmsrp_header_Failure_Report_def_s = 
{
	sizeof(tmsrp_header_Failure_Report_t),
	tmsrp_header_Failure_Report_ctor,
	tmsrp_header_Failure_Report_dtor,
	tsk_null
};

const tsk_object_def_t *tmsrp_header_Failure_Report_def_t = &tmsrp_header_Failure_Report_def_s;
