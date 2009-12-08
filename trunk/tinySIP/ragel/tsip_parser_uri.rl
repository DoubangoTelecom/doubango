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

/**@file tsip_parser_uri.c
 * @brief SIP/SIPS/TEL URI parser.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

/**@defgroup tsip_parser_uri_group SIP/SIPS/TEL URI parser.
*/

/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_uri;

	# Includes
	include tsip_machine_utils "./tsip_machine_utils.rl";
		
	action tag
	{
		TSK_DEBUG_INFO("URI:TAG");
		tag_start = p;
	}

	#/* Sets URI type */
	action is_sip { uri->scheme = tsk_strdup("sip"), uri->type = uri_sip; }
	action is_sips { uri->scheme = tsk_strdup("sips"), uri->type = uri_sips; }
	action is_tel { uri->scheme = tsk_strdup("tel"), uri->type = uri_tel; }

	#/* Sets HOST type */
	action is_ipv4 { uri->host_type = uri->host_type = host_ipv4; }
	action is_ipv6 { uri->host_type = uri->host_type = host_ipv6; }
	action is_hostname { uri->host_type = uri->host_type = host_hostname; }

	action parse_scheme
	{
		PARSER_SET_STRING(uri->scheme);		
		TSK_DEBUG_INFO("URI:SCHEME");
	}

	action parse_user_name
	{
		PARSER_SET_STRING(uri->user_name);		
		TSK_DEBUG_INFO("URI:USER_NAME");
	}

	action parse_password
	{
		PARSER_SET_STRING(uri->password);	
		TSK_DEBUG_INFO("URI:PARSE_PASSWORD");
	}

	action parse_host
	{
		PARSER_SET_STRING(uri->host);	
		TSK_DEBUG_INFO("URI:PARSE_HOST");
	}

	action parse_port
	{
		PARSER_SET_INTEGER(uri->port);	
		TSK_DEBUG_INFO("URI:PARSE_PORT");
	}

	action parse_param
	{
		PARSER_ADD_PARAM(uri->params);
		TSK_DEBUG_INFO("URI:PARSE_PARAM");
	}

	action eob
	{
		TSK_DEBUG_INFO("URI:EOB");
	}

	#uri_parameter = transport_param | user_param | method_param | ttl_param | maddr_param | lr_param | compression_param | target_param | cause_param | orig | gr_param | other_param;
	my_uri_parameter = (pname ( "=" pvalue )?) >tag %parse_param;
	uri_parameters = ( ";" my_uri_parameter )*;
	
	my_host = (hostname>is_hostname | IPv4address>is_ipv4 | IPv6reference>is_ipv6 );
	my_hostport = my_host>tag %parse_host ( ":" port>tag %parse_port )?;
	
	my_userinfo = ( user>tag %parse_user_name | telephone_subscriber ) :> ( ":" password>tag %parse_password )? "@";
	
	UNKNOWN_URI = (scheme & !("sip"i | "sips"i | "tel"i))>tag %parse_scheme HCOLON <:any*;
	SIP_URI = ("sip:"i %is_sip | "sips:"i %is_sips) my_userinfo? my_hostport uri_parameters headers?;
	TEL_URI = ("tel:"i >is_tel) telephone_subscriber;
	
	URI =  (SIP_URI | TEL_URI | UNKNOWN_URI);
	
	# Entry point
	main := URI;

}%%

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @fn	tsip_uri_t *tsip_uri_parse(const char *data, size_t size)
///
/// @brief	Parses SIP/SIPS/TEL URI. 
///
/// @author	Mamadou
/// @date	12/6/2009
///
/// @param [in,out]	data	Data from which to parse the uri. 
/// @param	size			The data size. 
///
/// @return	null if it fails, else the sip/sips/tel uri. 
////////////////////////////////////////////////////////////////////////////////////////////////////
tsip_uri_t *tsip_uri_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_uri_t *uri = TSIP_URI_CREATE(uri_unknown);
	
	const char *tag_start = 0;
	
	%%write data;
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% )
	{
		TSK_DEBUG_ERROR("Failed to parse SIP/SIPS/TEL URI.");
		TSIP_URI_SAFE_FREE(uri);
	}
	
	return uri;
}