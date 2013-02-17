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
///**@file tsip_header_WWW_Authenticate.c
// * @brief SIP WWW-Authenticate header.
// *
// * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
// *
//
// */
//#include "tinysip/headers/tsip_header_WWW_Authenticate.h"
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
//	machine tsip_machine_parser_header_WWW_Authenticate;
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
//		hdr_WWW_Authenticate->scheme = tsk_strdup("Digest");
//	}
//
//	action parse_realm
//	{
//		TSK_PARSER_SET_STRING(hdr_WWW_Authenticate->realm);
//		tsk_strunquote(&hdr_WWW_Authenticate->realm);
//	}
//
//	action parse_domain
//	{
//		TSK_PARSER_SET_STRING(hdr_WWW_Authenticate->domain);
//		//tsk_strunquote(&hdr_WWW_Authenticate->domain);
//	}
//
//	action parse_nonce
//	{
//		TSK_PARSER_SET_STRING(hdr_WWW_Authenticate->nonce);
//		tsk_strunquote(&hdr_WWW_Authenticate->nonce);
//	}
//
//	action parse_opaque
//	{
//		TSK_PARSER_SET_STRING(hdr_WWW_Authenticate->opaque);
//		tsk_strunquote(&hdr_WWW_Authenticate->opaque);
//	}
//
//	action parse_stale
//	{
//		hdr_WWW_Authenticate->stale = tsk_strniequals(tag_start, "true", 4);
//	}
//
//	action parse_algorithm
//	{
//		TSK_PARSER_SET_STRING(hdr_WWW_Authenticate->algorithm);
//	}
//
//	action parse_qop
//	{
//		TSK_PARSER_SET_STRING(hdr_WWW_Authenticate->qop);
//		//tsk_strunquote(&hdr_WWW_Authenticate->qop);
//	}
//
//	action parse_param
//	{
//		TSK_PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(hdr_WWW_Authenticate));
//	}
//
//	action eob
//	{
//	}
//
//	#FIXME: Only Digest (MD5, AKAv1-MD5 and AKAv2-MD5) is supported
//	other_challenge = (any+);
//	auth_param = generic_param>tag %parse_param;
//
//	realm = "realm"i EQUAL quoted_string>tag %parse_realm;
//	domain = "domain"i EQUAL LDQUOT <: (any*)>tag %parse_domain :> RDQUOT;
//	nonce = "nonce"i EQUAL quoted_string>tag %parse_nonce;
//	opaque = "opaque"i EQUAL quoted_string>tag %parse_opaque;
//	stale = "stale"i EQUAL ( "true"i | "false"i )>tag %parse_stale;
//	algorithm = "algorithm"i EQUAL <:token>tag %parse_algorithm;
//	qop_options = "qop"i EQUAL LDQUOT <: (any*)>tag %parse_qop :> RDQUOT;
//	
//	digest_cln = (realm | domain | nonce | opaque | stale | algorithm | qop_options)@1 | auth_param@0;
//	challenge = ( "Digest"i LWS digest_cln ( COMMA <:digest_cln )* )>is_digest | other_challenge;
//	WWW_Authenticate = "WWW-Authenticate"i HCOLON challenge;
//
//	# Entry point
//	main := WWW_Authenticate :>CRLF @eob;
//
//}%%
//
//int tsip_header_WWW_Authenticate_serialize(const tsip_header_t* header, tsk_buffer_t* output)
//{
//	if(header)
//	{
//		const tsip_header_WWW_Authenticate_t *WWW_Authenticate = header;
//		if(WWW_Authenticate && WWW_Authenticate->scheme)
//		{
//			return tsk_buffer_append_2(output, "%s realm=\"%s\"%s%s%s%s%s%s%s%s%s%s%s%s,stale=%s%s%s", 
//				WWW_Authenticate->scheme,
//				WWW_Authenticate->realm ? WWW_Authenticate->realm : "",
//				
//				WWW_Authenticate->domain ? ",domain=\"" : "",
//				WWW_Authenticate->domain ? WWW_Authenticate->domain : "",
//				WWW_Authenticate->domain ? "\"" : "",
//				
//				
//				WWW_Authenticate->qop ? ",qop=\"" : "",
//				WWW_Authenticate->qop ? WWW_Authenticate->qop : "",
//				WWW_Authenticate->qop ? "\"" : "",
//
//
//				WWW_Authenticate->nonce ? ",nonce=\"" : "",
//				WWW_Authenticate->nonce ? WWW_Authenticate->nonce : "",
//				WWW_Authenticate->nonce ? "\"" : "",
//
//				WWW_Authenticate->opaque ? ",opaque=\"" : "",
//				WWW_Authenticate->opaque ? WWW_Authenticate->opaque : "",
//				WWW_Authenticate->opaque ? "\"" : "",
//
//				WWW_Authenticate->stale ? "TRUE" : "FALSE",
//
//				WWW_Authenticate->algorithm ? ",algorithm=" : "",
//				WWW_Authenticate->algorithm ? WWW_Authenticate->algorithm : ""
//				);
//		}
//	}
//	return -1;
//}
//
//tsip_header_WWW_Authenticate_t *tsip_header_WWW_Authenticate_parse(const char *data, tsk_size_t size)
//{
//	int cs = 0;
//	const char *p = data;
//	const char *pe = p + size;
//	const char *eof = pe;
//	tsip_header_WWW_Authenticate_t *hdr_WWW_Authenticate = TSIP_HEADER_WWW_AUTHENTICATE_CREATE();
//	
//	const char *tag_start = tsk_null;
//
//	%%write data;
//	%%write init;
//	%%write exec;
//	
//	if( cs < %%{ write first_final; }%% )
//	{
//		TSK_OBJECT_SAFE_FREE(hdr_WWW_Authenticate);
//	}
//	
//	return hdr_WWW_Authenticate;
//}
//
//
//
//
//
//
//
////========================================================
////	WWW_Authenticate header object definition
////
//
//static tsk_object_t* tsip_header_WWW_Authenticate_ctor(tsk_object_t *self, va_list * app)
//{
//	tsip_header_WWW_Authenticate_t *WWW_Authenticate = self;
//	if(WWW_Authenticate)
//	{
//		TSIP_HEADER(WWW_Authenticate)->type = tsip_htype_WWW_Authenticate;
//		TSIP_HEADER(WWW_Authenticate)->serialize = tsip_header_WWW_Authenticate_serialize;
//	}
//	else
//	{
//		TSK_DEBUG_ERROR("Failed to create new WWW_Authenticate header.");
//	}
//	return self;
//}
//
//static tsk_object_t* tsip_header_WWW_Authenticate_dtor(tsk_object_t *self)
//{
//	tsip_header_WWW_Authenticate_t *WWW_Authenticate = self;
//	if(WWW_Authenticate)
//	{
//		TSK_FREE(WWW_Authenticate->scheme);
//		TSK_FREE(WWW_Authenticate->realm);
//		TSK_FREE(WWW_Authenticate->domain);
//		TSK_FREE(WWW_Authenticate->nonce);
//		TSK_FREE(WWW_Authenticate->opaque);
//		TSK_FREE(WWW_Authenticate->algorithm);
//		TSK_FREE(WWW_Authenticate->qop);
//
//		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(WWW_Authenticate));
//	}
//	else TSK_DEBUG_ERROR("Null WWW_Authenticate header.");
//
//	return self;
//}
//
//static const tsk_object_def_t tsip_header_WWW_Authenticate_def_s = 
//{
//	sizeof(tsip_header_WWW_Authenticate_t),
//	tsip_header_WWW_Authenticate_ctor,
//	tsip_header_WWW_Authenticate_dtor,
//	0
//};
//const void *tsip_header_WWW_Authenticate_def_t = &tsip_header_WWW_Authenticate_def_s;
