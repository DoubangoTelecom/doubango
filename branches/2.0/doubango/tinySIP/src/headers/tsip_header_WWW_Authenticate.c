
/* #line 1 "tsip_parser_header_WWW_Authenticate.rl" */
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

/**@file tsip_header_WWW_Authenticate.c
 * @brief SIP WWW-Authenticate header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_WWW_Authenticate.h"

#include "tinyhttp/headers/thttp_header_WWW_Authenticate.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>

tsip_header_WWW_Authenticate_t* tsip_header_WWW_Authenticate_create()
{
	return tsk_object_new(tsip_header_WWW_Authenticate_def_t);
}

int tsip_header_WWW_Authenticate_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tsip_header_WWW_Authenticate_t *WWW_Authenticate = (const tsip_header_WWW_Authenticate_t *)header;
		if(WWW_Authenticate && WWW_Authenticate->scheme){
			return tsk_buffer_append_2(output, "%s realm=\"%s\"%s%s%s%s%s%s%s%s%s%s%s%s,stale=%s%s%s", 
				WWW_Authenticate->scheme,
				WWW_Authenticate->realm ? WWW_Authenticate->realm : "",
				
				WWW_Authenticate->domain ? ",domain=\"" : "",
				WWW_Authenticate->domain ? WWW_Authenticate->domain : "",
				WWW_Authenticate->domain ? "\"" : "",
				
				
				WWW_Authenticate->qop ? ",qop=\"" : "",
				WWW_Authenticate->qop ? WWW_Authenticate->qop : "",
				WWW_Authenticate->qop ? "\"" : "",


				WWW_Authenticate->nonce ? ",nonce=\"" : "",
				WWW_Authenticate->nonce ? WWW_Authenticate->nonce : "",
				WWW_Authenticate->nonce ? "\"" : "",

				WWW_Authenticate->opaque ? ",opaque=\"" : "",
				WWW_Authenticate->opaque ? WWW_Authenticate->opaque : "",
				WWW_Authenticate->opaque ? "\"" : "",

				WWW_Authenticate->stale ? "TRUE" : "FALSE",

				WWW_Authenticate->algorithm ? ",algorithm=" : "",
				WWW_Authenticate->algorithm ? WWW_Authenticate->algorithm : ""
				);
		}
	}
	return -1;
}

tsip_header_WWW_Authenticate_t *tsip_header_WWW_Authenticate_parse(const char *data, tsk_size_t size)
{
	tsip_header_WWW_Authenticate_t *sip_hdr = 0;
	thttp_header_WWW_Authenticate_t* http_hdr;

	if((http_hdr = thttp_header_WWW_Authenticate_parse(data, size))){
		sip_hdr = tsip_header_WWW_Authenticate_create();

		sip_hdr->scheme = tsk_strdup(http_hdr->scheme);
		sip_hdr->realm = tsk_strdup(http_hdr->realm);
		sip_hdr->domain = tsk_strdup(http_hdr->domain);
		sip_hdr->nonce = tsk_strdup(http_hdr->nonce);
		sip_hdr->opaque = tsk_strdup(http_hdr->opaque);
		sip_hdr->algorithm = tsk_strdup(http_hdr->algorithm);
		sip_hdr->qop = tsk_strdup(http_hdr->qop);
		sip_hdr->stale = http_hdr->stale;
		
		TSIP_HEADER(sip_hdr)->params = tsk_object_ref(THTTP_HEADER(http_hdr)->params);

		TSK_OBJECT_SAFE_FREE(http_hdr);
	}
	
	return sip_hdr;
}







//========================================================
//	WWW_Authenticate header object definition
//

static tsk_object_t* tsip_header_WWW_Authenticate_ctor(tsk_object_t *self, va_list * app)
{
	tsip_header_WWW_Authenticate_t *WWW_Authenticate = self;
	if(WWW_Authenticate){
		TSIP_HEADER(WWW_Authenticate)->type = tsip_htype_WWW_Authenticate;
		TSIP_HEADER(WWW_Authenticate)->serialize = tsip_header_WWW_Authenticate_serialize;
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new WWW_Authenticate header.");
	}
	return self;
}

static tsk_object_t* tsip_header_WWW_Authenticate_dtor(tsk_object_t *self)
{
	tsip_header_WWW_Authenticate_t *WWW_Authenticate = self;
	if(WWW_Authenticate){
		TSK_FREE(WWW_Authenticate->scheme);
		TSK_FREE(WWW_Authenticate->realm);
		TSK_FREE(WWW_Authenticate->domain);
		TSK_FREE(WWW_Authenticate->nonce);
		TSK_FREE(WWW_Authenticate->opaque);
		TSK_FREE(WWW_Authenticate->algorithm);
		TSK_FREE(WWW_Authenticate->qop);

		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(WWW_Authenticate));
	}
	else{
		TSK_DEBUG_ERROR("Null WWW_Authenticate header.");
	}

	return self;
}

static const tsk_object_def_t tsip_header_WWW_Authenticate_def_s = 
{
	sizeof(tsip_header_WWW_Authenticate_t),
	tsip_header_WWW_Authenticate_ctor,
	tsip_header_WWW_Authenticate_dtor,
	tsk_null
};
const tsk_object_def_t *tsip_header_WWW_Authenticate_def_t = &tsip_header_WWW_Authenticate_def_s;
