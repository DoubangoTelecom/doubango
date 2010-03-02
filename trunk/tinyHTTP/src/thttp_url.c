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

/**@file thttp_url.c
 * @brief HTTP URL.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinyhttp/thttp_url.h"

#include "tinyhttp/parsers/thttp_parser_url.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"

#include <string.h>

/**@defgroup thttp_url_group HTTP URL.
*/

int __thttp_url_serialize(const thttp_url_t *url, tsk_buffer_t *output)
{	
	return 0;
}

int thttp_url_serialize(const thttp_url_t *url, tsk_buffer_t *output)
{
	return -1;
}

char* thttp_url_tostring(const thttp_url_t *url)
{
	return 0;
}

thttp_url_t *thttp_url_clone(const thttp_url_t *url)
{
	return 0;
}








//========================================================
//	HTTP/HTTPS URL object definition
//

/**@ingroup thttp_url_group
*/
static void* thttp_url_create(void *self, va_list * app)
{
	thttp_url_t *url = self;
	if(url)
	{
		url->type = va_arg(*app, thttp_url_type_t);
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new SIP/SIPS/TEL.");
	}
	return self;
}

/**@ingroup thttp_url_group
*/
static void* thttp_url_destroy(void *self)
{
	thttp_url_t *url = self;
	if(url)
	{
		TSK_FREE(url->scheme);
		TSK_FREE(url->host);
		TSK_FREE(url->password);
	}
	else TSK_DEBUG_ERROR("Null HTTP/HTTPS URL.");

	return self;
}

static const tsk_object_def_t thttp_url_def_s = 
{
	sizeof(thttp_url_t),
	thttp_url_create,
	thttp_url_destroy,
	0
};
const void *thttp_url_def_t = &thttp_url_def_s;
