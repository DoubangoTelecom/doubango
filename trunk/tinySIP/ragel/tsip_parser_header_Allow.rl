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

/**@file tsip_header_Allow.c
 * @brief SIP Allow header.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_Allow.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include <string.h>

/**@defgroup tsip_header_Allow_group SIP Allow header.
*/

/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_Allow;

	# Includes
	include tsip_machine_utils "./tsip_machine_utils.rl";
	
	action tag
	{
		tag_start = p;
	}
	
	action parse_method
	{
		PARSER_ADD_STRING(hdr_allow->methods);
	}

	action eob
	{
	}
	
	Allow = "Allow"i HCOLON ( Method>tag %parse_method ( COMMA Method>tag %parse_method )* )?;
	
	# Entry point
	main := Allow :>CRLF @eob;

}%%

int tsip_header_Allow_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_Allow_t *Allow = header;
		tsk_list_item_t *item;
		tsk_string_t *str;
		int ret = 0;

		tsk_list_foreach(item, Allow->methods)
		{
			str = item->data;
			if(item == Allow->methods->head)
			{
				tsk_buffer_append(output, str->value, strlen(str->value));
			}
			else
			{
				tsk_buffer_appendEx(output, ",%s", str->value);
			}
		}

		return ret;
	}

	return -1;
}

tsip_header_Allow_t *tsip_header_Allow_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Allow_t *hdr_allow = TSIP_HEADER_ALLOW_CREATE();
	
	const char *tag_start;

	%%write data;
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% )
	{
		TSK_OBJECT_SAFE_FREE(hdr_allow);
	}
	
	return hdr_allow;
}







//========================================================
//	Allow header object definition
//

/**@ingroup tsip_header_Allow_group
*/
static void* tsip_header_Allow_create(void *self, va_list * app)
{
	tsip_header_Allow_t *Allow = self;
	if(Allow)
	{
		/*const char* methods = va_arg(*app, const char *);
		if(methods && !tsk_strempty(methods))
		{
			Allow->methods = tsip_header_Allow_parse(methods, strlen(methods));
		}*/
		TSIP_HEADER(Allow)->type = tsip_htype_Allow;
		TSIP_HEADER(Allow)->tostring = tsip_header_Allow_tostring;
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new Allow header.");
	}
	return self;
}

/**@ingroup tsip_header_Allow_group
*/
static void* tsip_header_Allow_destroy(void *self)
{
	tsip_header_Allow_t *Allow = self;
	if(Allow)
	{
		TSK_OBJECT_SAFE_FREE(Allow->methods);
	}
	else TSK_DEBUG_ERROR("Null Allow header.");

	return self;
}

static const tsk_object_def_t tsip_header_Allow_def_s = 
{
	sizeof(tsip_header_Allow_t),
	tsip_header_Allow_create,
	tsip_header_Allow_destroy,
	0
};
const void *tsip_header_Allow_def_t = &tsip_header_Allow_def_s;