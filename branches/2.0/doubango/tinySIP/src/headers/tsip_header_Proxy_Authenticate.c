
/* #line 1 "tsip_parser_header_Proxy_Authenticate.rl" */
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

/**@file tsip_header_Proxy_Authenticate.c
 * @brief SIP Proxy-Authenticate header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_Proxy_Authenticate.h"

#include "tinyhttp/headers/thttp_header_WWW_Authenticate.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>

tsip_header_Proxy_Authenticate_t* tsip_header_Proxy_Authenticate_create()
{
	return tsk_object_new(tsip_header_Proxy_Authenticate_def_t);
}

int tsip_header_Proxy_Authenticate_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tsip_header_Proxy_Authenticate_t *Proxy_Authenticate = (const tsip_header_Proxy_Authenticate_t*)header;
		if(Proxy_Authenticate && Proxy_Authenticate->scheme){
			return tsk_buffer_append_2(output, "%s realm=\"%s\"%s%s%s%s%s%s%s%s%s%s%s%s,stale=%s%s%s", 
				Proxy_Authenticate->scheme,
				Proxy_Authenticate->realm ? Proxy_Authenticate->realm : "",
				
				Proxy_Authenticate->domain ? ",domain=\"" : "",
				Proxy_Authenticate->domain ? Proxy_Authenticate->domain : "",
				Proxy_Authenticate->domain ? "\"" : "",
				
				
				Proxy_Authenticate->qop ? ",qop=\"" : "",
				Proxy_Authenticate->qop ? Proxy_Authenticate->qop : "",
				Proxy_Authenticate->qop ? "\"" : "",


				Proxy_Authenticate->nonce ? ",nonce=\"" : "",
				Proxy_Authenticate->nonce ? Proxy_Authenticate->nonce : "",
				Proxy_Authenticate->nonce ? "\"" : "",

				Proxy_Authenticate->opaque ? ",opaque=\"" : "",
				Proxy_Authenticate->opaque ? Proxy_Authenticate->opaque : "",
				Proxy_Authenticate->opaque ? "\"" : "",

				Proxy_Authenticate->stale ? "TRUE" : "FALSE",

				Proxy_Authenticate->algorithm ? ",algorithm=" : "",
				Proxy_Authenticate->algorithm ? Proxy_Authenticate->algorithm : ""
				);
		}
	}
	return -1;
}

tsip_header_Proxy_Authenticate_t *tsip_header_Proxy_Authenticate_parse(const char *data, tsk_size_t size)
{
	tsip_header_Proxy_Authenticate_t *sip_hdr = 0;
	thttp_header_Proxy_Authenticate_t* http_hdr;

	if((http_hdr = thttp_header_Proxy_Authenticate_parse(data, size))){
		sip_hdr = tsip_header_Proxy_Authenticate_create();

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
//	Proxy_Authenticate header object definition
//

/**@ingroup tsip_header_Proxy_Authenticate_group
*/
static tsk_object_t* tsip_header_Proxy_Authenticate_ctor(tsk_object_t *self, va_list * app)
{
	tsip_header_Proxy_Authenticate_t *Proxy_Authenticate = self;
	if(Proxy_Authenticate){
		TSIP_HEADER(Proxy_Authenticate)->type = tsip_htype_Proxy_Authenticate;
		TSIP_HEADER(Proxy_Authenticate)->serialize = tsip_header_Proxy_Authenticate_serialize;
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Proxy_Authenticate header.");
	}
	return self;
}

/**@ingroup tsip_header_Proxy_Authenticate_group
*/
static tsk_object_t* tsip_header_Proxy_Authenticate_dtor(tsk_object_t *self)
{
	tsip_header_Proxy_Authenticate_t *Proxy_Authenticate = self;
	if(Proxy_Authenticate){
		TSK_FREE(Proxy_Authenticate->scheme);
		TSK_FREE(Proxy_Authenticate->realm);
		TSK_FREE(Proxy_Authenticate->domain);
		TSK_FREE(Proxy_Authenticate->nonce);
		TSK_FREE(Proxy_Authenticate->opaque);
		TSK_FREE(Proxy_Authenticate->algorithm);
		TSK_FREE(Proxy_Authenticate->qop);

		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Proxy_Authenticate));
	}
	else{
		TSK_DEBUG_ERROR("Null Proxy_Authenticate header.");
	}

	return self;
}

static const tsk_object_def_t tsip_header_Proxy_Authenticate_def_s = 
{
	sizeof(tsip_header_Proxy_Authenticate_t),
	tsip_header_Proxy_Authenticate_ctor,
	tsip_header_Proxy_Authenticate_dtor,
	tsk_null
};
const tsk_object_def_t *tsip_header_Proxy_Authenticate_def_t = &tsip_header_Proxy_Authenticate_def_s;
