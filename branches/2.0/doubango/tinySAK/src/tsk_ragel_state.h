/*
* Copyright (C) 2010-2011 Mamadou Diop.
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

/**@file tsk_ragel_state.h
 * @brief Ragel state for SIP, HTTP and MSRP parsing.
 *
 */
#ifndef TINYSAK_RAGEL_STATE_H
#define TINYSAK_RAGEL_STATE_H

#include "tinysak_config.h"
#include "tsk_params.h"

#include <string.h>

TSK_BEGIN_DECLS

#if defined(_MSC_VER)
#	define atoi64	_atoi64
#else
#	define atoi64	atoll
#endif

/**@ingroup tsk_ragel_state_group
*/
#define TSK_SCANNER_SET_STRING(string) \
	if(!string) \
	{ \
		int len = (int)(te  - ts);  \
		if(len >0) \
		{ \
			string = tsk_calloc(len+1, sizeof(char)), memcpy(string, ts, len); \
		} \
	}

/**@ingroup tsk_ragel_state_group
*/
#define TSK_PARSER_SET_STRING(string) \
	{  \
		int len = (int)(p  - tag_start);  \
		TSK_FREE(string); \
		if(len && tag_start){ \
			string = (char*)tsk_calloc(len+1, sizeof(char)), memcpy(string, tag_start, len); \
		}  \
	}

/**@ingroup tsk_ragel_state_group
*/
#define TSK_SCANNER_SET_INTEGER(integer) \
	{ \
		int len = (int)(te  - ts); \
		if(len>=0) \
		{ \
			char* tmp = tsk_calloc(len+1, sizeof(char)); \
			memcpy(tmp, ts, len); \
			integer = atoi(tmp); \
			tsk_free(&tmp); \
		} \
	}

/**@ingroup tsk_ragel_state_group
*/
#define TSK_PARSER_SET_INTEGER_EX(retval, type, func) \
	{ \
		int len = (int)(p  - tag_start); \
		if(len>=0) \
		{ \
			char* tmp = tsk_calloc(len+1, sizeof(char)); \
			memcpy(tmp, tag_start, len); \
			retval = (type) func(tmp); \
			tsk_free(&tmp); \
		} \
	}
/**@ingroup tsk_ragel_state_group
* @def TSK_PARSER_SET_INTEGER
*/
/**@ingroup tsk_ragel_state_group
* @def TSK_PARSER_SET_INT
*/
/**@ingroup tsk_ragel_state_group
* @def TSK_PARSER_SET_UINT
*/
/**@ingroup tsk_ragel_state_group
* @def TSK_PARSER_SET_FLOAT
*/
/**@ingroup tsk_ragel_state_group
* @def TSK_PARSER_SET_DOUBLE
*/
#define TSK_PARSER_SET_INTEGER(retval) TSK_PARSER_SET_INTEGER_EX(retval, int, atoi)
#define TSK_PARSER_SET_INT(retval) TSK_PARSER_SET_INTEGER(retval)
#define TSK_PARSER_SET_UINT(retval) TSK_PARSER_SET_INTEGER_EX(retval, uint32_t, atoi64)
#define TSK_PARSER_SET_FLOAT(retval) TSK_PARSER_SET_INTEGER_EX(retval, float, atof)
#define TSK_PARSER_SET_DOUBLE(retval) TSK_PARSER_SET_INTEGER_EX(retval, double, atof)

/**@ingroup tsk_ragel_state_group
*/
#define TSK_PARSER_ADD_PARAM(dest) \
	{ \
		tsk_size_t len = (tsk_size_t)(p  - tag_start); \
		tsk_param_t *param = tsk_params_parse_param(tag_start, len); \
		if(param) \
		{ \
			if(!dest) dest = tsk_list_create(); \
			tsk_list_push_back_data(dest, ((void**) &param)); \
		} \
	}

/**@ingroup tsk_ragel_state_group
*/
#define TSK_SACANNER_ADD_PARAM(dest) \
	{ \
		int len = (int)(te  - ts); \
		if(len >0) \
		{ \
			tsk_param_t *param = tsk_params_parse_param(ts, len); \
			if(param) \
			{ \
				if(!dest) dest = tsk_list_create(); \
				tsk_list_push_back_data(dest, ((void**) &param)); \
			} \
		} \
	}

/**@ingroup tsk_ragel_state_group
*/
#define TSK_PARSER_ADD_STRING(dest) \
	{ \
		tsk_size_t len = (tsk_size_t)(p  - tag_start); \
		tsk_string_t *string = tsk_string_create(tsk_null); \
		string->value = tsk_calloc(len+1, sizeof(char)), memcpy(string->value, tag_start, len); \
		if(!dest)  \
		{  \
			dest = tsk_list_create(); \
		} \
		tsk_list_push_back_data(dest, ((void**) &string)); \
	}

/**@ingroup tsk_ragel_state_group
* Ragel state.
*/
typedef struct tsk_ragel_state_s
{
	int cs; /**< Ragel current state. */
	const char *p; /**< Data pointing to the buffer to parse. */
	const char *pe; /**< Data end pointer. */
	const char *eof; /**< End of the file (in our case data) pointer. */
	const char *eoh; /**< End of the headers. */
		
	const char* tag_start; /**< Last tag start position set by ragel machine. */
	const char* tag_end; /**< The end of the ragel tag. */
}
tsk_ragel_state_t;


TINYSAK_API void tsk_ragel_state_init(tsk_ragel_state_t *state, const char *data, tsk_size_t size);

TSK_END_DECLS

#endif /* TINYSAK_RAGEL_STATE_H */

