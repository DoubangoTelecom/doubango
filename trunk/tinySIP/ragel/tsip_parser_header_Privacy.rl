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

/**@file tsip_header_Privacy.c
 * @brief SIP Privacy header.
 *
 *	  Header field          where   proxy ACK BYE CAN INV OPT REG
 *    ___________________________________________________________
 *    Privacy                        amrd  o   o   o   o   o   o
 * 
 *    Header field                        SUB NOT PRK IFO UPD MSG
 *    ___________________________________________________________
 *    Privacy                              o   o   o   o   o   o
 * 
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_Privacy.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

#include <string.h>

/**@defgroup tsip_header_Privacy_group SIP Privacy header.
*/

/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_Privacy;

	# Includes
	include tsip_machine_utils "./tsip_machine_utils.rl";
	
	action tag
	{
		tag_start = p;
	}

	action parse_priv_value
	{
		TSK_PARSER_ADD_STRING(hdr_privacy->values);
	}

	action eob
	{
	}
	
	priv_value = ("header"i | "session"i | "user"i | "none"i | "critical"i | "id"i | "history"i)@1 | token@0;
	Privacy_hdr = "Privacy"i HCOLON priv_value>tag %parse_priv_value ( ";" priv_value>tag %parse_priv_value )*;
	
	# Entry point
	main := Privacy_hdr :>CRLF @eob;

}%%


int tsip_header_Privacy_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_Privacy_t *Privacy = header;
		tsk_list_item_t *item;
		tsk_string_t *str;
		int ret = 0;

		tsk_list_foreach(item, Privacy->values)
		{
			str = item->data;
			if(item == Privacy->values->head)
			{
				tsk_buffer_append(output, str->value, strlen(str->value));
			}
			else
			{
				tsk_buffer_appendEx(output, ";%s", str->value);
			}
		}

		return ret;
	}

	return -1;
}


tsip_header_Privacy_t *tsip_header_Privacy_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Privacy_t *hdr_privacy = TSIP_HEADER_PRIVACY_CREATE();
	
	const char *tag_start;

	%%write data;
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% )
	{
		TSK_OBJECT_SAFE_FREE(hdr_privacy);
	}
	
	return hdr_privacy;
}







//========================================================
//	Privacy header object definition
//

/**@ingroup tsip_header_Privacy_group
*/
static void* tsip_header_Privacy_create(void *self, va_list * app)
{
	tsip_header_Privacy_t *Privacy = self;
	if(Privacy)
	{
		TSIP_HEADER(Privacy)->type = tsip_htype_Privacy;
		TSIP_HEADER(Privacy)->tostring = tsip_header_Privacy_tostring;
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new Privacy header.");
	}
	return self;
}

/**@ingroup tsip_header_Privacy_group
*/
static void* tsip_header_Privacy_destroy(void *self)
{
	tsip_header_Privacy_t *Privacy = self;
	if(Privacy)
	{
		TSK_OBJECT_SAFE_FREE(Privacy->values);
	}
	else TSK_DEBUG_ERROR("Null Privacy header.");

	return self;
}

static const tsk_object_def_t tsip_header_Privacy_def_s = 
{
	sizeof(tsip_header_Privacy_t),
	tsip_header_Privacy_create,
	tsip_header_Privacy_destroy,
	0
};
const void *tsip_header_Privacy_def_t = &tsip_header_Privacy_def_s;