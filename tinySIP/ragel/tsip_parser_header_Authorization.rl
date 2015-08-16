///*
//* Copyright (C) 2010-2011 Mamadou Diop.
//*
//* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
//*	
//* This file is part of Open Source Doubango Framework.
//*
//* DOUBANGO is free software: you can redistribute it and/or modify
//* it under the terms of the GNU General Public License as published by
//* the Free Software Foundation, either version 3 of the License, or
//* (at your option) any later version.
//*	
//* DOUBANGO is distributed in the hope that it will be useful,
//* but WITHOUT ANY WARRANTY; without even the implied warranty of
//* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//* GNU General Public License for more details.
//*	
//* You should have received a copy of the GNU General Public License
//* along with DOUBANGO.
//*
//*/
//
///**@file tsip_header_Authorization.c
// * @brief SIP Authorization header.
// *
// * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
// *
//
// */
//#include "tinysip/headers/tsip_header_Authorization.h"
//
//#include "tinysip/parsers/tsip_parser_uri.h"
//
//#include "tsk_debug.h"
//#include "tsk_memory.h"
//#include "tsk_time.h"
//
//#include <string.h>
//

//
///***********************************
//*	Ragel state machine.
//*/
//%%{
//	machine tsip_machine_parser_header_Authorization;
//
//	# Includes
//	include tsip_machine_utils "./ragel/tsip_machine_utils.rl";
//	
//	action tag
//	{
//		tag_start = p;
//	}
//	
//	action is_digest
//	{
//		#//FIXME: Only Digest is supported
//		hdr_Authorization->scheme = tsk_strdup("Digest");
//	}
//
//	action parse_username
//	{
//		TSK_PARSER_SET_STRING(hdr_Authorization->username);
//		tsk_strunquote(&hdr_Authorization->username);
//	}
//
//	action parse_realm
//	{
//		TSK_PARSER_SET_STRING(hdr_Authorization->realm);
//		tsk_strunquote(&hdr_Authorization->realm);
//	}
//
//	action parse_nonce
//	{
//		TSK_PARSER_SET_STRING(hdr_Authorization->nonce);
//		tsk_strunquote(&hdr_Authorization->nonce);
//	}
//
//	action parse_uri
//	{
//		TSK_PARSER_SET_STRING(hdr_Authorization->line.request.uri);
//	}
//
//	action parse_response
//	{
//		TSK_PARSER_SET_STRING(hdr_Authorization->response);
//		tsk_strunquote(&hdr_Authorization->response);
//	}
//
//	action parse_algorithm
//	{
//		TSK_PARSER_SET_STRING(hdr_Authorization->algorithm);
//	}
//
//	action parse_cnonce
//	{
//		TSK_PARSER_SET_STRING(hdr_Authorization->cnonce);
//		tsk_strunquote(&hdr_Authorization->cnonce);
//	}
//
//	action parse_opaque
//	{
//		TSK_PARSER_SET_STRING(hdr_Authorization->opaque);
//		tsk_strunquote(&hdr_Authorization->opaque);
//	}
//
//	action parse_qop
//	{
//		TSK_PARSER_SET_STRING(hdr_Authorization->qop);
//		//tsk_strunquote(&hdr_Authorization->qop);
//	}
//
//	action parse_nc
//	{
//		TSK_PARSER_SET_STRING(hdr_Authorization->nc);
//	}
//
//	action parse_param
//	{
//		TSK_PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(hdr_Authorization));
//	}
//
//	action eob
//	{
//	}
//
//	#FIXME: Only Digest (MD5, AKAv1-MD5 and AKAv2-MD5) is supported
//	qop_value  = "auth" | "auth-int" | token;
//	other_response = (any+);
//	auth_param = generic_param>tag %parse_param;
//	
//	username = "username"i EQUAL quoted_string>tag %parse_username;
//	realm = "realm"i EQUAL quoted_string>tag %parse_realm;
//	nonce = "nonce"i EQUAL quoted_string>tag %parse_nonce;
//	digest_uri = "uri"i EQUAL LDQUOT <: (any*)>tag %parse_uri :> RDQUOT;
//	#dresponse = "response"i EQUAL LDQUOT <: (LHEX{32})>tag %parse_response :> RDQUOT;
//	dresponse = "response"i EQUAL quoted_string>tag %parse_response;
//	algorithm = "algorithm"i EQUAL <:token>tag %parse_algorithm;
//	cnonce = "cnonce"i EQUAL quoted_string>tag %parse_cnonce;
//	opaque = "opaque"i EQUAL quoted_string>tag %parse_opaque;
//	message_qop = "qop"i EQUAL qop_value>tag %parse_qop;
//	nonce_count = "nc"i EQUAL (LHEX{8})>tag %parse_nc;
//	
//	dig_resp = (username | realm | nonce | digest_uri | dresponse | algorithm | cnonce | opaque | message_qop | nonce_count)@1 | auth_param@0;
//	digest_response = dig_resp ( COMMA <:dig_resp )*;
//	credentials = ( "Digest"i LWS digest_response )>is_digest | other_response;
//	Authorization = "Authorization"i HCOLON credentials;
//
//	# Entry point
//	main := Authorization :>CRLF @eob;
//
//}%%
//
//int tsip_header_Authorization_serialize(const tsip_header_t* header, tsk_buffer_t* output)
//{
//	if(header)
//	{
//		const tsip_header_Authorization_t *Authorization = header;
//		if(Authorization && Authorization->scheme)
//		{
//			return tsk_buffer_append_2(output, "%s %s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s", 
//				Authorization->scheme,
//
//				Authorization->username ? "username=\"" : "",
//				Authorization->username ? Authorization->username : "",
//				Authorization->username ? "\"" : "",
//
//				Authorization->realm ? ",realm=\"" : "",
//				Authorization->realm ? Authorization->realm : "",
//				Authorization->realm ? "\"" : "",
//
//				Authorization->nonce ? ",nonce=\"" : "",
//				Authorization->nonce ? Authorization->nonce : "",
//				Authorization->nonce ? "\"" : "",
//
//				Authorization->line.request.uri ? ",uri=\"" : "",
//				Authorization->line.request.uri ? Authorization->line.request.uri : "",
//				Authorization->line.request.uri ? "\"" : "",
//				
//				Authorization->response ? ",response=\"" : "",
//				Authorization->response ? Authorization->response : "",
//				Authorization->response ? "\"" : "",
//				
//				Authorization->algorithm ? ",algorithm=" : "",
//				Authorization->algorithm ? Authorization->algorithm : "",
//
//				Authorization->cnonce ? ",cnonce=\"" : "",
//				Authorization->cnonce ? Authorization->cnonce : "",
//				Authorization->cnonce ? "\"" : "",
//
//				Authorization->opaque ? ",opaque=\"" : "",
//				Authorization->opaque ? Authorization->opaque : "",
//				Authorization->opaque ? "\"" : "",
//
//				Authorization->qop ? ",qop=" : "",
//				Authorization->qop ? Authorization->qop : "",
//
//				Authorization->nc ? ",nc=" : "",
//				Authorization->nc ? Authorization->nc : ""
//				);
//		}
//	}
//	return -1;
//}
//
//tsip_header_Authorization_t *tsip_header_Authorization_parse(const char *data, tsk_size_t size)
//{
//	int cs = 0;
//	const char *p = data;
//	const char *pe = p + size;
//	const char *eof = pe;
//	tsip_header_Authorization_t *hdr_Authorization = tsip_header_Authorization_create();
//	
//	const char *tag_start = tsk_null;
//
//	%%write data;
//	%%write init;
//	%%write exec;
//	
//	if( cs < %%{ write first_final; }%% )
//	{
//		TSK_OBJECT_SAFE_FREE(hdr_Authorization);
//	}
//	
//	return hdr_Authorization;
//}
//
//
//
//
//
//
//
////========================================================
////	Authorization header object definition
////
//
//static tsk_object_t* tsip_header_Authorization_ctor(tsk_object_t *self, va_list * app)
//{
//	tsip_header_Authorization_t *Authorization = self;
//	if(Authorization)
//	{
//		TSIP_HEADER(Authorization)->type = tsip_htype_Authorization;
//		TSIP_HEADER(Authorization)->serialize = tsip_header_Authorization_serialize;
//	}
//	else
//	{
//		TSK_DEBUG_ERROR("Failed to create new Authorization header.");
//	}
//	return self;
//}
//
//static tsk_object_t* tsip_header_Authorization_dtor(tsk_object_t *self)
//{
//	tsip_header_Authorization_t *Authorization = self;
//	if(Authorization)
//	{
//		TSK_FREE(Authorization->scheme);
//		TSK_FREE(Authorization->username);
//		TSK_FREE(Authorization->realm);
//		TSK_FREE(Authorization->nonce);
//		TSK_FREE(Authorization->line.request.uri);
//		TSK_FREE(Authorization->response);
//		TSK_FREE(Authorization->algorithm);
//		TSK_FREE(Authorization->cnonce);
//		TSK_FREE(Authorization->opaque);
//		TSK_FREE(Authorization->qop);
//		TSK_FREE(Authorization->nc);
//		
//		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Authorization));
//	}
//	else TSK_DEBUG_ERROR("Null Authorization header.");
//
//	return self;
//}
//
//static const tsk_object_def_t tsip_header_Authorization_def_s = 
//{
//	sizeof(tsip_header_Authorization_t),
//	tsip_header_Authorization_ctor,
//	tsip_header_Authorization_dtor,
//	0
//};
//const void *tsip_header_Authorization_def_t = &tsip_header_Authorization_def_s;
