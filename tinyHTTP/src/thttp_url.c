/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinyhttp/thttp_url.h"

#include "tinyhttp/parsers/thttp_parser_url.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"

#include <string.h>

/**@defgroup thttp_url_group HTTP/HTTPS URL
*/

/**@ingroup thttp_url_group
* @param type The type of the url to create.
* Creates new HTTP/HTTPS url.
*/
thttp_url_t* thttp_url_create(thttp_url_type_t type)
{
	return tsk_object_new(thttp_url_def_t, type);
}

/**@ingroup thttp_url_group
*/
int thttp_url_serialize(const thttp_url_t *url, tsk_buffer_t *output)
{
	/* RFC 1738: "http://" hostport [ "/" hpath [ "?" search ]] */
	return tsk_buffer_append_2(output, "%s://%s:%u%s%s%s%s", 

		url->scheme,
		url->host,
		url->port,

		url->hpath ? "/" : "",
		url->hpath ? url->hpath : "",

		url->search ? "?" : "",
		url->search ? url->search : ""
		);
}

/**@ingroup thttp_url_group
*/
char* thttp_url_tostring(const thttp_url_t *url)
{
	tsk_buffer_t *output = tsk_buffer_create_null();
	char* ret = 0;

	if(!thttp_url_serialize(url,  output)){
		ret = tsk_strndup((const char*)output->data, output->size);
	}
	else{
		TSK_DEBUG_ERROR("Failed to serialize HTTP URL.");
	}

	TSK_OBJECT_SAFE_FREE(output);
	return ret;
}

/**@ingroup thttp_url_group
*/
thttp_url_t *thttp_url_clone(const thttp_url_t *url)
{
	thttp_url_t *newurl = 0;
	tsk_buffer_t *output = tsk_buffer_create_null();
	thttp_url_serialize(url, output);
	newurl = thttp_url_parse(output->data, output->size);
	TSK_OBJECT_SAFE_FREE(output);

	return newurl;
}

/**@ingroup thttp_url_group
*/
tsk_bool_t thttp_url_isvalid(const char* urlstring)
{
	thttp_url_t *url;
	if(!urlstring){
		return tsk_false;
	}
	
	if((url = thttp_url_parse(urlstring, tsk_strlen(urlstring)))){
		TSK_OBJECT_SAFE_FREE(url);
		return tsk_true;
	}
	else{
		return tsk_false;
	}
}






//========================================================
//	HTTP/HTTPS URL object definition
//

/**@ingroup thttp_url_group
*/
static tsk_object_t* thttp_url_ctor(tsk_object_t *self, va_list * app)
{
	thttp_url_t *url = self;
	if(url){
		url->type = va_arg(*app, thttp_url_type_t);
		if(url->type == thttp_url_https){
			url->port = 443;
		}
		else{
			url->port = 80;
		}
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new SIP/SIPS/TEL.");
	}
	return self;
}

/**@ingroup thttp_url_group
*/
static tsk_object_t* thttp_url_dtor(tsk_object_t *self)
{
	thttp_url_t *url = self;
	if(url){
		TSK_FREE(url->scheme);
		TSK_FREE(url->host);
		TSK_FREE(url->hpath);
		TSK_FREE(url->search);
	}
	else{
		TSK_DEBUG_ERROR("Null HTTP/HTTPS URL.");
	}

	return self;
}

static const tsk_object_def_t thttp_url_def_s = 
{
	sizeof(thttp_url_t),
	thttp_url_ctor,
	thttp_url_dtor,
	tsk_null
};
const tsk_object_def_t *thttp_url_def_t = &thttp_url_def_s;
