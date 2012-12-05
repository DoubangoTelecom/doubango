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

/**@file tsip_parser_uri.c
 * @brief SIP/SIPS/TEL URI parser.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_uri;

	# Includes
	include tsip_machine_utils "./ragel/tsip_machine_utils.rl";
	#include tsip_machine_userinfo;
		
	action tag{
		tag_start = p;
	}

	#/* Sets URI type */
	action is_sip { uri->scheme = tsk_strdup("sip"), uri->type = uri_sip; }
	action is_sips { uri->scheme = tsk_strdup("sips"), uri->type = uri_sips; }
	action is_tel { uri->scheme = tsk_strdup("tel"), uri->type = uri_tel; }

	#/* Sets HOST type */
	action is_ipv4 { uri->host_type = host_ipv4; }
	action is_ipv6 { uri->host_type = host_ipv6; }
	action is_hostname { uri->host_type = host_hostname; }

	action parse_scheme{
		TSK_PARSER_SET_STRING(uri->scheme);
	}

	action parse_user_name{
		TSK_PARSER_SET_STRING(uri->user_name);
	}

	action parse_password{
		TSK_PARSER_SET_STRING(uri->password);
	}

	action parse_host{
		TSK_PARSER_SET_STRING(uri->host);
	}

	action parse_port{
		TSK_PARSER_SET_INTEGER(uri->port);	
	}

	action parse_param{
		TSK_PARSER_ADD_PARAM(uri->params);
	}

	action eob{
	}

	my_uri_parameter = (pname ( "=" pvalue )?) >tag %parse_param;
	uri_parameters = ( ";" my_uri_parameter )*;

	sip_usrinfo		:= ( ( user>tag %parse_user_name ) :> ( ':' password>tag %parse_password )? :>> '@' ) @{ fgoto main; };
	
	main			:= |*
							("sip:"i>tag %is_sip | "sips:"i>tag %is_sips) @100
							{
								if(tsk_strcontains(te, (pe - te), "@")){
									fgoto sip_usrinfo;
								}
							};
							
							("tel:"i %is_tel (any+)>tag %parse_user_name :> uri_parameters) @100 { };
							
							( (IPv6reference >is_ipv6)@89 | (IPv4address >is_ipv4)@88 | (hostname >is_hostname)@87 ) @90
							{
								TSK_SCANNER_SET_STRING(uri->host);
								if(uri->host_type == host_ipv6){
									tsk_strunquote_2(&uri->host, '[', ']');
								}
							};							

							(":" port)@80
							{
								ts++;
								TSK_SCANNER_SET_INTEGER(uri->port);
							};
							
							( uri_parameters ) @70	{  };
							(any)+ @0				{  };

						

					*|;

	#main := ({ fcall SIP_URI; });

}%%

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @ingroup tsip_uri_group
///
/// Creates SIP/SIPS/TEL URI from string buffer. 
///
///
/// @param data	Pointer to a string buffer from which to create the URI object.
/// @param	size The size of the string buffer. 
///
/// @retval	@ref tsip_uri_t* object if succeed and Null otherwise.
/// 
/// @code
/// tsip_uri_t* uri;
/// if((uri = tsip_uri_parse("sip:bob@open-ims.test", strlen("sip:bob@open-ims.test")))){
///      printf("success");
/// }
/// else{
///      printf("error");
/// }
/// TSK_OBJECT_SAFE_FREE(uri);
/// @endcode
///
/// @sa @ref tsip_uri_create()
////////////////////////////////////////////////////////////////////////////////////////////////////
tsip_uri_t *tsip_uri_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;

	const char *ts = tsk_null, *te = tsk_null;
	int act = 0;

	tsip_uri_t *uri = tsip_uri_create(uri_unknown);
	
	const char *tag_start = tsk_null;
	
	%%write data;
	(void)(eof);
	(void)(void)(tsip_machine_parser_uri_first_final);
	(void)(void)(tsip_machine_parser_uri_error);
	(void)(void)(tsip_machine_parser_uri_en_sip_usrinfo);
	(void)(void)(tsip_machine_parser_uri_en_main);
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse SIP/SIPS/TEL URI.");
		TSK_OBJECT_SAFE_FREE(uri);
	}
	
	return uri;
}
