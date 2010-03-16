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

/**@file tmsrp_uri.c
 * @brief MSRP/MSRPS URI.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinyMSRP/tmsrp_uri.h"

#include "tinyMSRP/parsers/tmsrp_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"
#include "tsk_params.h"
#include "tsk_url.h"

#include <string.h>

/**@defgroup tmsrp_uri_group MSRP/MSRPS/TEL URI
*/

int tmsrp_uri_serialize(const tmsrp_uri_t *uri, tsk_buffer_t *output)
{
	tsk_istr_t port;
	if(!uri || !output){
		return -1;
	}

	if(uri->authority.port >= 0){
		tsk_itoa(uri->authority.port, &port);
	}

	/* msrp://atlanta.example.com:7654/jshA7weztas;tcp
	* msrp-scheme  "://" authority  ["/" session-id] ";" transport  *( ";" URI-parameter)
	* authority	=  	[ userinfo  "@" ]   host    [ ":"   port ]
	*/
	tsk_buffer_appendEx(output, "%s://%s%s%s%s%s%s%s%s%s;%s",

		// scheme
		uri->scheme,

		// authority
		uri->authority.userinfo ? uri->authority.userinfo : "",
		uri->authority.userinfo ? "@" : "",
		uri->authority.host_type == host_ipv6 ? "[" : "",
		uri->authority.host,
		uri->authority.host_type == host_ipv6 ? "]" : "",
		uri->authority.port >= 0 ? ":" : "",
		uri->authority.port >= 0 ? port : "",

		// session-id
		uri->session_id ? "/" : "",
		uri->session_id ? uri->session_id : "",
		
		// transport
		uri->transport
		);
	
	/* Params */
	if(!TSK_LIST_IS_EMPTY(uri->params)){
		tsk_buffer_append(output, ";", 1);
		tsk_params_tostring(uri->params, ';', output);
	}
	
	return 0;
}

char* tmsrp_uri_tostring(const tmsrp_uri_t *uri)
{
	tsk_buffer_t *output = TSK_BUFFER_CREATE_NULL();
	char* ret = 0;

	if(!tmsrp_uri_serialize(uri, output)){
		ret = tsk_strndup((const char*)output->data, output->size);
	}
	else{
		TSK_DEBUG_ERROR("Failed to serialize URI.");
	}

	TSK_OBJECT_SAFE_FREE(output);
	return ret;
}

tmsrp_uri_t *tmsrp_uri_clone(const tmsrp_uri_t *uri)
{
	tmsrp_uri_t *newuri = 0;
	tsk_buffer_t *output = TSK_BUFFER_CREATE_NULL();
	tmsrp_uri_serialize(uri, output);
	newuri = tmsrp_uri_parse(output->data, output->size);
	TSK_OBJECT_SAFE_FREE(output);

	return newuri;
}








//========================================================
//	MSRP/MSRPS/TEL URI object definition
//

/**@ingroup tmsrp_uri_group
*/
static void* tmsrp_uri_create(void *self, va_list * app)
{
	tmsrp_uri_t *uri = self;
	if(uri){
		uri->scheme = tsk_strdup( va_arg(*app, const char*) );

		// authority
		uri->authority.host = tsk_strdup( va_arg(*app, const char*) );
		uri->authority.host_type = va_arg(*app, tmsrp_host_type_t);
		uri->authority.port = va_arg(*app, int32_t);
		 
		uri->session_id = tsk_strdup( va_arg(*app, const char*) );
		uri->transport = tsk_strdup( va_arg(*app, const char*) );

		uri->params = TSK_LIST_CREATE(); /* Empty list. */
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new MSRP/MSRPS.");
	}
	return self;
}

/**@ingroup tmsrp_uri_group
*/
static void* tmsrp_uri_destroy(void *self)
{
	tmsrp_uri_t *uri = self;
	if(uri)
	{
		TSK_FREE(uri->scheme);
		// authority
		TSK_FREE(uri->authority.userinfo);
		TSK_FREE(uri->authority.host);
		//
		TSK_FREE(uri->session_id);
		TSK_FREE(uri->transport);
		
		TSK_OBJECT_SAFE_FREE(uri->params);
	}
	else TSK_DEBUG_ERROR("Null MSRP/MSRPS URI.");

	return self;
}

static int tmsrp_uri_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
	return -1;
}

static const tsk_object_def_t tmsrp_uri_def_s = 
{
	sizeof(tmsrp_uri_t),
	tmsrp_uri_create,
	tmsrp_uri_destroy,
	tmsrp_uri_cmp
};
const void *tmsrp_uri_def_t = &tmsrp_uri_def_s;
