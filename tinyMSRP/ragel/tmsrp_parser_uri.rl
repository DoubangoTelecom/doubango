/*
* Copyright (C) 2009-2015 Mamadou DIOP.
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

/**@file tmsrp_header_Dummy.c
 * @brief MSRP Dummy header.
 */
#include "tinymsrp/parsers/tmsrp_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/
%%{
	machine tmsrp_machine_parser_uri;

	# Includes
	include tmsrp_machine_utils "./ragel/tmsrp_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}

	#/* Sets HOST type */
	action is_ipv4 { uri->authority.host_type = tmsrp_host_ipv4; }
	action is_ipv6 { uri->authority.host_type = tmsrp_host_ipv6; }
	action is_hostname { uri->authority.host_type = tmsrp_host_hostname; }

	action parse_scheme{
		TSK_PARSER_SET_STRING(uri->scheme);
	}

	action parse_userinfo{
		TSK_PARSER_SET_STRING(uri->authority.userinfo);
	}

	action parse_host{
		TSK_PARSER_SET_STRING(uri->authority.host);
		if(uri->authority.host_type == tmsrp_host_ipv6){
			tsk_strunquote_2(&uri->authority.host, '[', ']');
		}
	}

	action parse_port{
		TSK_PARSER_SET_INT(uri->authority.port);
	}

	action parse_session_id{
		TSK_PARSER_SET_STRING(uri->session_id);
	}

	action parse_transport{
		TSK_PARSER_SET_STRING(uri->transport);
	}

	action parse_param{
		TSK_PARSER_ADD_PARAM(uri->params);
	}
	
	

	#//MSRP-URI	=  	msrp-scheme  "://" authority  ["/" session-id] ";" transport  *( ";" URI-parameter)
	#//msrp-scheme	= 	"msrp" / "msrps"
	#//session-id	= 	1*( unreserved / "+" / "=" / "/" )
	#//transport	= 	"tcp" / 1*ALPHANUM
	#//URI-parameter	= 	token ["=" token]
	#//authority	=  	[ userinfo  "@" ]   host    [ ":"   port ] 

	msrp_scheme = ("msrp" | "msrps")>tag %parse_scheme;

	userinfo = (unreserved | pct_encoded | sub_delims | ":")* >tag %parse_userinfo;
	myhost = ((IPv6reference >is_ipv6)>2 | (IPv4address >is_ipv4)>1 | (hostname >is_hostname)>0)>tag %parse_host;
	authority = (userinfo "@")? myhost (":" port>tag %parse_port)?;
	
	session_id = (unreserved | "+" | "=" | "/")+ >tag %parse_session_id;

	transport = ("tcp" | alphanum*)>tag %parse_transport;

	URI_parameter = (token ("=" token)?)>tag %parse_param;

	URI = msrp_scheme "://" authority ("/" session_id)? ";" transport ( ";" URI_parameter)*;
	
	# Entry point
	main := URI;
}%%

tmsrp_uri_t *tmsrp_uri_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tmsrp_uri_t *uri = tmsrp_uri_create_null();
	
	const char *tag_start = tsk_null;

	TSK_RAGEL_DISABLE_WARNINGS_BEGIN()
	%%write data;
	(void)(eof);
	(void)(tmsrp_machine_parser_uri_first_final);
	(void)(tmsrp_machine_parser_uri_error);
	(void)(tmsrp_machine_parser_uri_en_main);
	%%write init;
	%%write exec;
	TSK_RAGEL_DISABLE_WARNINGS_END()
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse MSRP/MSRPS header.");
		TSK_OBJECT_SAFE_FREE(uri);
	}
	
	return uri;
}