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

/**@file thttp_parser_url.c
 * @brief HTTP/HTTPS URL parser.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinyhttp/parsers/thttp_parser_url.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

/**@defgroup thttp_parser_url_group HTTP/HTTPS URL parser.
*/

/***********************************
*	Ragel state machine.
*/
%%{
	machine thttp_machine_parser_url;

	# Includes
	include thttp_machine_utils "./thttp_machine_utils.rl";
			
	action tag
	{
		tag_start = p;
	}

	#/* Sets URL type */
	action is_http { url->scheme = tsk_strdup("http"), url->type = url_http; }
	action is_https { url->scheme = tsk_strdup("https"), url->type = url_https; }

	action eob
	{
	}

	main := (("http:"i>tag %is_http | "https:"i>tag %is_https) any*) @eob;
	
}%%

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @fn	thttp_url_t *thttp_url_parse(const char *data, size_t size)
///
/// @brief	Parses HTTP/HTTPS URL. 
///
/// @author	Mamadou
/// @date	12/6/2009
///
/// @param [in,out]	data	Data from which to parse the url. 
/// @param	size			The data size. 
///
/// @return	null if it fails, else the http/https/tel url. 
////////////////////////////////////////////////////////////////////////////////////////////////////
thttp_url_t *thttp_url_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;

	const char *ts = 0, *te = 0;
	int act =0;

	thttp_url_t *url = THTTP_URL_CREATE(url_unknown);
	
	const char *tag_start = 0;
	
	%%write data;
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% )
	{
		TSK_DEBUG_ERROR("Failed to parse HTTP/HTTPS URL.");
		TSK_OBJECT_SAFE_FREE(url);
	}
	
	return url;
}