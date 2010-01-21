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

/**@file tsip_uri.c
 * @brief SIP/SIPS/TEL URI.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/tsip_uri.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"

#include <string.h>

/**@defgroup tsip_uri_group
*/

int __tsip_uri_tostring(const tsip_uri_t *uri, int with_params, tsk_buffer_t *output)
{
	tsk_istr_t port;

	if(uri->port) tsk_itoa(uri->port, &port);

	/* sip:alice:secretword@atlanta.com:65535 */
	tsk_buffer_appendEx(output, "%s%s%s%s%s%s%s%s%s", 

		uri->scheme ? uri->scheme : "",
		uri->scheme ? ":" : "",

		uri->user_name ? uri->user_name : "",

		uri->password ? ":" : "",
		uri->password ? uri->password : "",

		uri->host ? (uri->user_name ? "@" : "") : "",
		uri->host ? uri->host : "",

		uri->port ? ":" : "",
		uri->port ? port : ""
		);
	
	/* Params */
	if(with_params && !TSK_LIST_IS_EMPTY(uri->params))
	{
		tsk_buffer_append(output, ";", 1);
		tsk_params_tostring(uri->params, ';', output);
	}
	
	return 0;
}

int tsip_uri_tostring(const tsip_uri_t *uri, int with_params, int quote, tsk_buffer_t *output)
{
	if(quote)
	{
		if(uri->display_name)
		{
			tsk_buffer_appendEx(output, "\"%s\"", uri->display_name);
		}

		tsk_buffer_append(output, "<", 1);
		__tsip_uri_tostring(uri, with_params, output);
		tsk_buffer_append(output, ">", 1);

		return 0;
	}
	else
	{
		__tsip_uri_tostring(uri, with_params, output);

		return 0;
	}

	return -1;
}


tsip_uri_t *tsip_uri_clone(const tsip_uri_t *uri, int with_params, int quote)
{
	tsip_uri_t *newuri;
	tsk_buffer_t *output = TSK_BUFFER_CREATE_NULL();
	tsip_uri_tostring(uri, with_params, quote, output);
	newuri = tsip_uri_parse(output->data, output->size);
	TSK_OBJECT_SAFE_FREE(output);

	return newuri;
}








//========================================================
//	SIP/SIPS/TEL URI object definition
//

/**@ingroup tsip_uri_group
*/
static void* tsip_uri_create(void *self, va_list * app)
{
	tsip_uri_t *uri = self;
	if(uri)
	{
		uri->type = va_arg(*app, tsip_uri_type_t);
		uri->params = TSK_LIST_CREATE(); /* Empty list. */
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new SIP/SIPS/TEL.");
	}
	return self;
}

/**@ingroup tsip_uri_group
*/
static void* tsip_uri_destroy(void *self)
{
	tsip_uri_t *uri = self;
	if(uri)
	{
		TSK_FREE(uri->scheme);
		TSK_FREE(uri->host);
		TSK_FREE(uri->user_name);
		TSK_FREE(uri->password);
		TSK_FREE(uri->display_name);
		TSK_OBJECT_SAFE_FREE(uri->params);
	}
	else TSK_DEBUG_ERROR("Null SIP/SIPS/TEL URI.");

	return self;
}

static const tsk_object_def_t tsip_uri_def_s = 
{
	sizeof(tsip_uri_t),
	tsip_uri_create,
	tsip_uri_destroy,
	0
};
const void *tsip_uri_def_t = &tsip_uri_def_s;
