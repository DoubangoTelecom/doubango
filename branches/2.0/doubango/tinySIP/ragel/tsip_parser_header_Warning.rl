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

/**@file tsip_header_Warning.c
 * @brief SIP Warning header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_Warning.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>



/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_Warning;

	# Includes
	include tsip_machine_utils "./ragel/tsip_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}
	
	action create_warning{
		if(!curr_warning){
			curr_warning = tsip_header_Warning_create();
		}
	}

	action parse_agent{
		if(curr_warning){
			TSK_PARSER_SET_STRING(curr_warning->agent);
		}
	}

	action parse_text{
		if(curr_warning){
			TSK_PARSER_SET_STRING(curr_warning->text);
		}
	}

	action parse_code{
		if(curr_warning){
			TSK_PARSER_SET_INTEGER(curr_warning->code);
		}
	}

	action add_warning{
		if(curr_warning){
			tsk_list_push_back_data(hdr_warnings, ((void**) &curr_warning));
		}
	}

	action eob{
	}

	warn_code = DIGIT{3};
	pseudonym = token;
	warn_agent = hostport | pseudonym;
	warn_text = quoted_string;
	warning_value = (warn_code>tag %parse_code :>SP warn_agent>tag %parse_agent :>SP warn_text>tag %parse_text) >create_warning %add_warning;
	Warning = "Warning"i HCOLON warning_value ( COMMA warning_value )*;

	# Entry point
	main := Warning :>CRLF @eob;

}%%

tsip_header_Warning_t* tsip_header_Warning_create()
{
	return tsk_object_new(tsip_header_Warning_def_t);
}

int tsip_header_Warning_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tsip_header_Warning_t *Warning = (const tsip_header_Warning_t *)header;
		return tsk_buffer_append_2(output, "%d %s %s", 
			Warning->code, Warning->agent, Warning->text); /* warn-code  SP warn-agent  SP warn-text */
	}

	return -1;
}

tsip_header_Warnings_L_t *tsip_header_Warning_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Warnings_L_t *hdr_warnings = tsk_list_create();
	
	const char *tag_start = tsk_null;
	tsip_header_Warning_t *curr_warning = 0;

	%%write data;
	(void)(eof);
	(void)(tsip_machine_parser_header_Warning_first_final);
	(void)(tsip_machine_parser_header_Warning_error);
	(void)(tsip_machine_parser_header_Warning_en_main);
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse 'Warning' header.");
		TSK_OBJECT_SAFE_FREE(curr_warning);
		TSK_OBJECT_SAFE_FREE(hdr_warnings);
	}
	
	return hdr_warnings;
}





//========================================================
//	Warning header object definition
//

static tsk_object_t* tsip_header_Warning_ctor(tsk_object_t *self, va_list * app)
{
	tsip_header_Warning_t *Warning = self;
	if(Warning){
		TSIP_HEADER(Warning)->type = tsip_htype_Warning;
		TSIP_HEADER(Warning)->serialize = tsip_header_Warning_serialize;

		Warning->code = -1;
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Warning header.");
	}
	return self;
}

static tsk_object_t* tsip_header_Warning_dtor(tsk_object_t *self)
{
	tsip_header_Warning_t *Warning = self;
	if(Warning){
		TSK_FREE(Warning->agent);
		TSK_FREE(Warning->text);

		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Warning));
	}
	else{
		TSK_DEBUG_ERROR("Null Warning header.");
	}

	return self;
}

static const tsk_object_def_t tsip_header_Warning_def_s = 
{
	sizeof(tsip_header_Warning_t),
	tsip_header_Warning_ctor,
	tsip_header_Warning_dtor,
	tsk_null
};
const tsk_object_def_t *tsip_header_Warning_def_t = &tsip_header_Warning_def_s;
