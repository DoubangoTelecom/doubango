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
* GNU Lesser General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

/**@file tsip_header_Call_ID.c
 * @brief SIP Call-ID/i header.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_Call_ID.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>

/**@defgroup tsip_header_Call_ID_group SIP Call-ID header.
*/

/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_Call_ID;

	# Includes
	include tsip_machine_utils "./tsip_machine_utils.rl";
	
	action tag
	{
		tag_start = p;
	}
	
	action parse_value
	{
		PARSER_SET_STRING(hdr_call_id->value);
	}

	action eob
	{
	}

	callid = word ( "@" word )?;
	Call_ID = ( "Call-ID"i | "i"i ) HCOLON callid>tag %parse_value;
	
	# Entry point
	main := Call_ID :>CRLF @eob;

}%%

int tsip_header_Call_ID_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_Call_ID_t *Call_ID = header;
		if(Call_ID->value)
		{
			return tsk_buffer_append(output, Call_ID->value, strlen(Call_ID->value));
		}
	}
	return -1;
}

void tsip_header_Call_ID_random(tsk_istr_t *result)
{
	uint64_t epoch = tsk_time_epoch();
	tsk_itoa(epoch, result);
}

tsip_header_Call_ID_t *tsip_header_Call_ID_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Call_ID_t *hdr_call_id = TSIP_HEADER_CALL_ID_CREATE(0);
	
	const char *tag_start;

	%%write data;
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% )
	{
		TSK_OBJECT_SAFE_FREE(hdr_call_id);
	}
	
	return hdr_call_id;
}







//========================================================
//	Call_ID header object definition
//

/**@ingroup tsip_header_Call_ID_group
*/
static void* tsip_header_Call_ID_create(void *self, va_list * app)
{
	tsip_header_Call_ID_t *Call_ID = self;
	if(Call_ID)
	{
		Call_ID->value = tsk_strdup(va_arg(*app, const char *));
		TSIP_HEADER(Call_ID)->type = tsip_htype_Call_ID;
		TSIP_HEADER(Call_ID)->tostring = tsip_header_Call_ID_tostring;
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new Call_ID header.");
	}
	return self;
}

/**@ingroup tsip_header_Call_ID_group
*/
static void* tsip_header_Call_ID_destroy(void *self)
{
	tsip_header_Call_ID_t *Call_ID = self;
	if(Call_ID)
	{
		TSK_FREE(Call_ID->value);
		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Call_ID));
	}
	else TSK_DEBUG_ERROR("Null Call_ID header.");

	return self;
}

static const tsk_object_def_t tsip_header_Call_ID_def_s = 
{
	sizeof(tsip_header_Call_ID_t),
	tsip_header_Call_ID_create,
	tsip_header_Call_ID_destroy,
	0
};
const void *tsip_header_Call_ID_def_t = &tsip_header_Call_ID_def_s;