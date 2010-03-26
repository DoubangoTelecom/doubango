/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)yahoo.fr>
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

/**@file thttp_header_Authorization.c
 * @brief HTTP Authorization header.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinyhttp/headers/thttp_header_Authorization.h"

#include "tinyhttp/parsers/thttp_parser_url.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>

/**@defgroup thttp_header_Authorization_group HTTP Proxy-Authenticate header.
*/

/***********************************
*	Ragel state machine.
*/
%%{
	machine thttp_machine_parser_header_Authorization;

	# Includes
	include thttp_machine_utils "./thttp_machine_utils.rl";
	
	action tag
	{
		tag_start = p;
	}
	
	action is_digest
	{
		#//FIXME: Only Digest is supported
		hdr_Authorization->scheme = tsk_strdup("Digest");
	}

	action is_auth
	{
		THTTP_HEADER(hdr_Authorization)->type = thttp_htype_Authorization;
	}

	action is_proxy
	{
		THTTP_HEADER(hdr_Authorization)->type = thttp_htype_Proxy_Authorization;
	}

	action parse_username
	{
		TSK_PARSER_SET_STRING(hdr_Authorization->username);
		tsk_strunquote(&hdr_Authorization->username);
	}

	action parse_realm
	{
		TSK_PARSER_SET_STRING(hdr_Authorization->realm);
		tsk_strunquote(&hdr_Authorization->realm);
	}

	action parse_nonce
	{
		TSK_PARSER_SET_STRING(hdr_Authorization->nonce);
		tsk_strunquote(&hdr_Authorization->nonce);
	}

	action parse_uri
	{
		TSK_PARSER_SET_STRING(hdr_Authorization->uri);
	}

	action parse_response
	{
		TSK_PARSER_SET_STRING(hdr_Authorization->response);
		tsk_strunquote(&hdr_Authorization->response);
	}

	action parse_algorithm
	{
		TSK_PARSER_SET_STRING(hdr_Authorization->algorithm);
	}

	action parse_cnonce
	{
		TSK_PARSER_SET_STRING(hdr_Authorization->cnonce);
		tsk_strunquote(&hdr_Authorization->cnonce);
	}

	action parse_opaque
	{
		TSK_PARSER_SET_STRING(hdr_Authorization->opaque);
		tsk_strunquote(&hdr_Authorization->opaque);
	}

	action parse_qop
	{
		TSK_PARSER_SET_STRING(hdr_Authorization->qop);
		//tsk_strunquote(&hdr_Authorization->qop);
	}

	action parse_nc
	{
		TSK_PARSER_SET_STRING(hdr_Authorization->nc);
	}

	action parse_param
	{
		TSK_PARSER_ADD_PARAM(THTTP_HEADER_PARAMS(hdr_Authorization));
	}

	action eob
	{
	}
	
	#FIXME: Only Digest (MD5, AKAv1-MD5 and AKAv2-MD5) is supported
	qop_value  = "auth" | "auth-int" | token;
	other_response = (any+);
	auth_param = generic_param>tag %parse_param;
	
	username = "username"i EQUAL quoted_string>tag %parse_username;
	realm = "realm"i EQUAL quoted_string>tag %parse_realm;
	nonce = "nonce"i EQUAL quoted_string>tag %parse_nonce;
	digest_uri = "uri"i EQUAL LDQUOT <: (any*)>tag %parse_uri :> RDQUOT;
	#dresponse = "response"i EQUAL LDQUOT <: (LHEX{32})>tag %parse_response :> RDQUOT;
	dresponse = "response"i EQUAL quoted_string>tag %parse_response;
	algorithm = "algorithm"i EQUAL <:token>tag %parse_algorithm;
	cnonce = "cnonce"i EQUAL quoted_string>tag %parse_cnonce;
	opaque = "opaque"i EQUAL quoted_string>tag %parse_opaque;
	message_qop = "qop"i EQUAL qop_value>tag %parse_qop;
	nonce_count = "nc"i EQUAL (LHEX{8})>tag %parse_nc;
	
	dig_resp = (username | realm | nonce | digest_uri | dresponse | algorithm | cnonce | opaque | message_qop | nonce_count)@1 | auth_param@0;
	digest_response = dig_resp ( COMMA <:dig_resp )*;
	credentials = ( "Digest"i LWS digest_response )>is_digest | other_response;
	Authorization = ("Authorization"i>is_auth | "Proxy-Authorization"i>is_proxy) HCOLON credentials;

	# Entry point
	main := Authorization :>CRLF @eob;

}%%

int thttp_header_Authorization_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const thttp_header_Authorization_t *Authorization = header;
		if(Authorization && Authorization->scheme)
		{
			return tsk_buffer_appendEx(output, "%s %s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s", 
				Authorization->scheme,

				Authorization->username ? "username=\"" : "",
				Authorization->username ? Authorization->username : "",
				Authorization->username ? "\"" : "",

				Authorization->realm ? ",realm=\"" : "",
				Authorization->realm ? Authorization->realm : "",
				Authorization->realm ? "\"" : "",

				Authorization->nonce ? ",nonce=\"" : "",
				Authorization->nonce ? Authorization->nonce : "",
				Authorization->nonce ? "\"" : "",

				Authorization->uri ? ",uri=\"" : "",
				Authorization->uri ? Authorization->uri : "",
				Authorization->uri ? "\"" : "",
				
				Authorization->response ? ",response=\"" : "",
				Authorization->response ? Authorization->response : "",
				Authorization->response ? "\"" : "",
				
				Authorization->algorithm ? ",algorithm=" : "",
				Authorization->algorithm ? Authorization->algorithm : "",

				Authorization->cnonce ? ",cnonce=\"" : "",
				Authorization->cnonce ? Authorization->cnonce : "",
				Authorization->cnonce ? "\"" : "",

				Authorization->opaque ? ",opaque=\"" : "",
				Authorization->opaque ? Authorization->opaque : "",
				Authorization->opaque ? "\"" : "",

				Authorization->qop ? ",qop=" : "",
				Authorization->qop ? Authorization->qop : "",

				Authorization->nc ? ",nc=" : "",
				Authorization->nc ? Authorization->nc : ""
				);
		}
	}
	return -1;
}

thttp_header_Authorization_t *thttp_header_Authorization_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	thttp_header_Authorization_t *hdr_Authorization = THTTP_HEADER_AUTHORIZATION_CREATE();
	
	const char *tag_start;

	%%write data;
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% )
	{
		TSK_OBJECT_SAFE_FREE(hdr_Authorization);
	}
	
	return hdr_Authorization;
}

thttp_header_Proxy_Authorization_t *thttp_header_Proxy_Authorization_parse(const char *data, size_t size)
{
	return thttp_header_Authorization_parse(data, size);
}



//========================================================
//	Authorization header object definition
//

/**@ingroup thttp_header_Authorization_group
*/
static void* thttp_header_Authorization_create(void *self, va_list * app)
{
	thttp_header_Authorization_t *Authorization = self;
	if(Authorization)
	{
		THTTP_HEADER(Authorization)->type = thttp_htype_Authorization;
		THTTP_HEADER(Authorization)->tostring = thttp_header_Authorization_tostring;
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new Authorization header.");
	}
	return self;
}

/**@ingroup thttp_header_Authorization_group
*/
static void* thttp_header_Authorization_destroy(void *self)
{
	thttp_header_Authorization_t *Authorization = self;
	if(Authorization)
	{
		TSK_FREE(Authorization->scheme);
		TSK_FREE(Authorization->username);
		TSK_FREE(Authorization->realm);
		TSK_FREE(Authorization->nonce);
		TSK_FREE(Authorization->uri);
		TSK_FREE(Authorization->response);
		TSK_FREE(Authorization->algorithm);
		TSK_FREE(Authorization->cnonce);
		TSK_FREE(Authorization->opaque);
		TSK_FREE(Authorization->qop);
		TSK_FREE(Authorization->nc);
		
		TSK_OBJECT_SAFE_FREE(THTTP_HEADER_PARAMS(Authorization));
	}
	else TSK_DEBUG_ERROR("Null Authorization header.");

	return self;
}

static const tsk_object_def_t thttp_header_Authorization_def_s = 
{
	sizeof(thttp_header_Authorization_t),
	thttp_header_Authorization_create,
	thttp_header_Authorization_destroy,
	0
};
const void *thttp_header_Authorization_def_t = &thttp_header_Authorization_def_s;