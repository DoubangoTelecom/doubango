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

/**@file tsip_header_Authorization.c
 * @brief SIP Proxy-Authenticate header.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_Authorization.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>

/**@defgroup tsip_header_Authorization_group SIP Proxy-Authenticate header.
*/

/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_Authorization;

	# Includes
	include tsip_machine_utils "./tsip_machine_utils.rl";
	
	action tag
	{
		TSK_DEBUG_INFO("AUTHORIZATION:TAG");
		tag_start = p;
	}
	
	action is_digest
	{
		#//FIXME: Only Digest is supported
		TSK_DEBUG_INFO("AUTHORIZATION:IS_DIGEST");
		hdr_Authorization->scheme = tsk_strdup("Digest");
	}

	action parse_username
	{
		PARSER_SET_STRING(hdr_Authorization->username);
		tsk_strunquote(&hdr_Authorization->username);
		TSK_DEBUG_INFO("AUTHORIZATION:PARSE_USERNAME");
	}

	action parse_realm
	{
		PARSER_SET_STRING(hdr_Authorization->realm);
		tsk_strunquote(&hdr_Authorization->realm);
		TSK_DEBUG_INFO("AUTHORIZATION:PARSE_REALM");
	}

	action parse_nonce
	{
		PARSER_SET_STRING(hdr_Authorization->nonce);
		tsk_strunquote(&hdr_Authorization->nonce);
		TSK_DEBUG_INFO("AUTHORIZATION:PARSE_NONCE");
	}

	action parse_uri
	{
		PARSER_SET_STRING(hdr_Authorization->uri);
		TSK_DEBUG_INFO("AUTHORIZATION:PARSE_URI");
	}

	action parse_response
	{
		PARSER_SET_STRING(hdr_Authorization->response);
		tsk_strunquote(&hdr_Authorization->response);
		TSK_DEBUG_INFO("AUTHORIZATION:PARSE_RESPONSE");
	}

	action parse_algorithm
	{
		PARSER_SET_STRING(hdr_Authorization->algorithm);
		TSK_DEBUG_INFO("AUTHORIZATION:PARSE_ALGORITHM");
	}

	action parse_cnonce
	{
		PARSER_SET_STRING(hdr_Authorization->cnonce);
		tsk_strunquote(&hdr_Authorization->cnonce);
		TSK_DEBUG_INFO("AUTHORIZATION:PARSE_CNONCE");
	}

	action parse_opaque
	{
		PARSER_SET_STRING(hdr_Authorization->opaque);
		tsk_strunquote(&hdr_Authorization->opaque);
		TSK_DEBUG_INFO("AUTHORIZATION:PARSE_OPAQUE");
	}

	action parse_qop
	{
		PARSER_SET_STRING(hdr_Authorization->qop);
		//tsk_strunquote(&hdr_Authorization->qop);
		TSK_DEBUG_INFO("AUTHORIZATION:PARSE_QOP");
	}

	action parse_nc
	{
		PARSER_SET_STRING(hdr_Authorization->nc);
		TSK_DEBUG_INFO("AUTHORIZATION:PARSE_NC");
	}

	action parse_param
	{
		PARSER_ADD_PARAM(hdr_Authorization->params);
	}

	action eob
	{
		TSK_DEBUG_INFO("AUTHORIZATION:EOB");
	}

	#FIXME: Only Digest (MD5, AKAv1-MD5 and AKAv2-MD5) is supported
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
	message_qop = "qop"i EQUAL LDQUOT <: (any*)>tag %parse_qop :> RDQUOT;
	nonce_count = "nc"i EQUAL (LHEX{8})>tag %parse_nc;
	
	dig_resp = (username | realm | nonce | digest_uri | dresponse | algorithm | cnonce | opaque | message_qop | nonce_count)>1 | auth_param>0;
	digest_response = dig_resp ( COMMA <:dig_resp )*;
	credentials = ( "Digest"i LWS digest_response )>is_digest | other_response;
	Authorization = "Authorization"i HCOLON credentials;

	# Entry point
	main := Authorization :>CRLF @eob;

}%%

int tsip_header_Authorization_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_Authorization_t *Authorization = header;
		if(Authorization && Authorization->scheme)
		{
			return tsk_buffer_appendEx(output, "%s %s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s", 
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

				Authorization->qop ? ",qop=\"" : "",
				Authorization->qop ? Authorization->qop : "",
				Authorization->qop ? "\"" : "",

				Authorization->nc ? ",nc=" : "",
				Authorization->nc ? Authorization->nc : ""
				);
		}
	}
	return -1;
}

tsip_header_Authorization_t *tsip_header_Authorization_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Authorization_t *hdr_Authorization = TSIP_HEADER_AUTHORIZATION_CREATE();
	
	const char *tag_start;

	%%write data;
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% )
	{
		TSIP_HEADER_AUTHORIZATION_SAFE_FREE(hdr_Authorization);
	}
	
	return hdr_Authorization;
}







//========================================================
//	Authorization header object definition
//

/**@ingroup tsip_header_Authorization_group
*/
static void* tsip_header_Authorization_create(void *self, va_list * app)
{
	tsip_header_Authorization_t *Authorization = self;
	if(Authorization)
	{
		Authorization->type = tsip_htype_Authorization;
		Authorization->tostring = tsip_header_Authorization_tostring;
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new Authorization header.");
	}
	return self;
}

/**@ingroup tsip_header_Authorization_group
*/
static void* tsip_header_Authorization_destroy(void *self)
{
	tsip_header_Authorization_t *Authorization = self;
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
		
		TSK_LIST_SAFE_FREE(Authorization->params);
	}
	else TSK_DEBUG_ERROR("Null Authorization header.");

	return self;
}

static const tsk_object_def_t tsip_header_Authorization_def_s = 
{
	sizeof(tsip_header_Authorization_t),
	tsip_header_Authorization_create,
	tsip_header_Authorization_destroy,
	0
};
const void *tsip_header_Authorization_def_t = &tsip_header_Authorization_def_s;