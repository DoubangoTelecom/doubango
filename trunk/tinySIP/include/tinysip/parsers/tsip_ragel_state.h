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

/**@file tsip_ragel_state.h.
 * @brief Ragel state for sip parsing.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYSIP_RAGEL_STATE_H
#define TINYSIP_RAGEL_STATE_H

#include "tinysip_config.h"

#include <stdint.h>


#define PARSER_SET_STRING(string) \
	if(!string) \
	{ \
		int len = (int)(p  - tag_start);  \
		if(string)free(string); \
		string = tsk_calloc(len+1, sizeof(char)), memcpy(string, tag_start, len); \
	}

#define PARSER_SET_INTEGER(integer) \
	int len = (int)(p  - tag_start); \
	if(len>0) \
	{ \
		char* tmp = tsk_calloc(len+1, sizeof(char)); \
		memcpy(tmp, tag_start, len); \
		integer = atoi(tmp); \
		free(tmp); \
	}

#define PARSER_ADD_PARAM(dest) \
	size_t len = (size_t)(p  - tag_start); \
	tsk_param_t *param = tsk_params_parse_param(tag_start, len); \
	if(param) \
	{ \
		if(!dest) dest = TSK_LIST_CREATE(); \
		tsk_list_pushback_data(dest, ((void**) &param)); \
	}

#define PARSER_ADD_STRING(dest) \
	size_t len = (size_t)(p  - tag_start); \
	tsk_string_t *string = TSK_STRING_CREATE(0); \
	string->value = tsk_calloc(len+1, sizeof(char)), memcpy(string->value, tag_start, len); \
	if(!dest)  \
	{  \
		dest = TSK_LIST_CREATE(); \
	} \
	tsk_list_pushback_data(dest, ((void**) &string));

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @struct	tsip_parser_s
///
/// @brief	Ragel state.
///
/// @author	Mamadou
/// @date	12/4/2009
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsip_ragel_state_s
{
	int cs; /**< Ragel current state. */
	const char *p; /**< Data pointing to the buffer to parse. */
	const char *pe; /**< Data end pointer. */
	const char *eof; /**< End of the file (in our cas data) pointer. */
		
	const char* tag_start; /**< Last tag start position set by ragel machine. */
	const char* tag_end; /**< The end of the ragel tag. */
}
tsip_ragel_state_t;

TINYSIP_API void tsip_ragel_state_init(tsip_ragel_state_t *state, const char *data, size_t size);



#endif /* TINYSIP_RAGEL_STATE_H */