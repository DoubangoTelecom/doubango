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

/**@file tmsrp_header_Status.c
 * @brief MSRP 'Status' header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#include "tinymsrp/headers/tmsrp_header_Status.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/
%%{
	machine tmsrp_machine_parser_header_Status;

	# Includes
	include tmsrp_machine_utils "./ragel/tmsrp_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}
	
	action parse_namespace{
		TSK_PARSER_SET_INT(hdr_Status->_namespace);
	}
	
	action parse_code{
		TSK_PARSER_SET_INT(hdr_Status->code);
	}
	
	action parse_reason{
		TSK_PARSER_SET_STRING(hdr_Status->reason);
	}

	#/// @par ABNF :  Status	=  	 "Status:" SP namespace  SP status-code  [SP text-reason]
	#/// namespace	= 	3(DIGIT) ; "000" for all codes defined in RFC 4975
	#/// text-reason	= 	utf8text 
	Status = "Status:"i SP DIGIT{3}>tag %parse_namespace SP DIGIT{3}>tag %parse_code (SP utf8text>tag %parse_reason)?;
	
	# Entry point
	main := Status :>CRLF?;

}%%



tmsrp_header_Status_t* tmsrp_header_Status_create(short _namespace, short code, const char* reason)
{
	return tsk_object_new(TMSRP_HEADER_STATUS_VA_ARGS(_namespace, code, reason));
}

tmsrp_header_Status_t* tmsrp_header_Status_create_null()
{
	return tmsrp_header_Status_create(0, 200, tsk_null);
}


int tmsrp_header_Status_tostring(const tmsrp_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tmsrp_header_Status_t *Status = (const tmsrp_header_Status_t *)header;
				
		// Status: 000 200 OK
		return tsk_buffer_append_2(output, "%.3hi %.3hi%s%s", 
			Status->_namespace, 
			Status->code,
			Status->reason ? " " : "",
			Status->reason ? Status->reason : ""
			);
	}

	return -1;
}

tmsrp_header_Status_t *tmsrp_header_Status_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tmsrp_header_Status_t *hdr_Status = tmsrp_header_Status_create_null();

	const char *tag_start = tsk_null;

	%%write data;
	(void)(eof);
	(void)(tmsrp_machine_parser_header_Status_first_final);
	(void)(tmsrp_machine_parser_header_Status_error);
	(void)(tmsrp_machine_parser_header_Status_en_main);
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse 'Status' header.");
		TSK_OBJECT_SAFE_FREE(hdr_Status);
	}
	
	return hdr_Status;
}







//========================================================
//	Status header object definition
//

static tsk_object_t* tmsrp_header_Status_ctor(tsk_object_t *self, va_list * app)
{
	tmsrp_header_Status_t *Status = self;
	if(Status){
		TMSRP_HEADER(Status)->type = tmsrp_htype_Status;
		TMSRP_HEADER(Status)->tostring = tmsrp_header_Status_tostring;
#if defined(__GNUC__)
		Status->_namespace = (short)va_arg(*app, int);
		Status->code = (short)va_arg(*app, int);
#else
		Status->_namespace = va_arg(*app, short);
		Status->code = va_arg(*app, short);
#endif
		Status->reason = tsk_strdup( va_arg(*app, const char*) );
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Status header.");
	}
	return self;
}

static tsk_object_t* tmsrp_header_Status_dtor(tsk_object_t *self)
{
	tmsrp_header_Status_t *Status = self;
	if(Status){
		TSK_FREE(Status->reason);
	}
	else{
		TSK_DEBUG_ERROR("Null Status header.");
	}

	return self;
}

static const tsk_object_def_t tmsrp_header_Status_def_s = 
{
	sizeof(tmsrp_header_Status_t),
	tmsrp_header_Status_ctor,
	tmsrp_header_Status_dtor,
	tsk_null
};

const tsk_object_def_t *tmsrp_header_Status_def_t = &tmsrp_header_Status_def_s;
