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

/**@file tsip_header_Warning.c
 * @brief SIP Warning header.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_Warning.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>

/**@defgroup tsip_header_Warning_group SIP Warning header.
*/

/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_Warning;

	# Includes
	include tsip_machine_utils "./tsip_machine_utils.rl";
	
	action tag
	{
		tag_start = p;
	}
	
	action create_warning
	{
		if(!curr_warning)
		{
			curr_warning = TSIP_HEADER_WARNING_CREATE();
		}
	}

	action parse_agent
	{
		if(curr_warning)
		{
			TSK_PARSER_SET_STRING(curr_warning->agent);
		}
	}

	action parse_text
	{
		if(curr_warning)
		{
			TSK_PARSER_SET_STRING(curr_warning->text);
		}
	}

	action parse_code
	{
		if(curr_warning)
		{
			TSK_PARSER_SET_INTEGER(curr_warning->code);
		}
	}

	action add_warning
	{
		if(curr_warning)
		{
			tsk_list_push_back_data(hdr_warnings, ((void**) &curr_warning));
		}
	}

	action eob
	{
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

int tsip_header_Warning_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_Warning_t *Warning = header;
		return tsk_buffer_appendEx(output, "%d %s %s", 
			Warning->code, Warning->agent, Warning->text); /* warn-code  SP warn-agent  SP warn-text */
	}

	return -1;
}

tsip_header_Warnings_L_t *tsip_header_Warning_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Warnings_L_t *hdr_warnings = TSK_LIST_CREATE();
	
	const char *tag_start;
	tsip_header_Warning_t *curr_warning = 0;

	%%write data;
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% )
	{
		TSK_OBJECT_SAFE_FREE(curr_warning);
		TSK_OBJECT_SAFE_FREE(hdr_warnings);
	}
	
	return hdr_warnings;
}





//========================================================
//	Warning header object definition
//

/**@ingroup tsip_header_Warning_group
*/
static void* tsip_header_Warning_create(void *self, va_list * app)
{
	tsip_header_Warning_t *Warning = self;
	if(Warning)
	{
		TSIP_HEADER(Warning)->type = tsip_htype_Warning;
		TSIP_HEADER(Warning)->tostring = tsip_header_Warning_tostring;

		Warning->code = -1;
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new Warning header.");
	}
	return self;
}

/**@ingroup tsip_header_Warning_group
*/
static void* tsip_header_Warning_destroy(void *self)
{
	tsip_header_Warning_t *Warning = self;
	if(Warning)
	{
		TSK_FREE(Warning->agent);
		TSK_FREE(Warning->text);

		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Warning));
	}
	else TSK_DEBUG_ERROR("Null Warning header.");

	return self;
}

static const tsk_object_def_t tsip_header_Warning_def_s = 
{
	sizeof(tsip_header_Warning_t),
	tsip_header_Warning_create,
	tsip_header_Warning_destroy,
	0
};
const void *tsip_header_Warning_def_t = &tsip_header_Warning_def_s;
