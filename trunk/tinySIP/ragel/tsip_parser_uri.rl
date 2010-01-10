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
	machine tsip_machine_userinfo;

	# Includes
	include tsip_machine_utils "./tsip_machine_utils.rl";

	action parse_toto
	{
	}

	action parse_titi
	{
	}

}%%

%%{
	machine tsip_machine_parser_uri;

	# Includes
	include tsip_machine_utils "./tsip_machine_utils.rl";
	include tsip_machine_userinfo;
		
	action tag
	{
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
	}

	action parse_user_name
	{
		PARSER_SET_STRING(uri->user_name);		
	}

	action parse_password
	{
		PARSER_SET_STRING(uri->password);	
	}

	action parse_host
	{
		PARSER_SET_STRING(uri->host);	
	}

	action parse_port
	{
		PARSER_SET_INTEGER(uri->port);	
	}

	action parse_param
	{
		PARSER_ADD_PARAM(uri->params);
	}

	action has_arobase
	{
		tsk_strcontains(tag_start, "@") 
	}

	action eob
	{
	}

	#uri_parameter = transport_param | user_param | method_param | ttl_param | maddr_param | lr_param | compression_param | target_param | cause_param | orig | gr_param | other_param;
	my_uri_parameter = (pname ( "=" pvalue )?) >tag %parse_param;
	uri_parameters = ( ";" my_uri_parameter )*;
	
	my_host = (hostname>is_hostname | IPv4address>is_ipv4 | IPv6reference>is_ipv6 );
	my_hostport = my_host>tag %parse_host ( ":" port>tag %parse_port )?;
	
	my_userinfo = ( user>tag %parse_user_name | telephone_subscriber ) :> ( ":" password>tag %parse_password )? "@";
	
	UNKNOWN_URI = (scheme)>tag %parse_scheme HCOLON <:any*;
	#SIP_URI = ("sip:"i %is_sip | "sips:"i %is_sips) (((my_userinfo my_hostport) when has_arobase) | (my_hostport)) uri_parameters headers?;
	#SIP_URI = ("sip:"i %is_sip | "sips:"i %is_sips) my_userinfo? my_hostport uri_parameters headers?;


	SIP_URI := |*
				("sip:"i %is_sip | "sips:"i %is_sips) => parse_scheme;

				 *|;



	#SIP_URI = 
	#	("sip:"i %is_sip | "sips:"i %is_sips)
	#	((any when has_arobase)*) %parse_toto | any* %parse_titi
	#	uri_parameters headers?;

	#SIP_URI = ( "sip:"i %is_sip | "sips:"i %is_sips ) > 0 >tag
	#	( ((any when has_arobase)*) %parse_toto | (any*) %parse_titi ) >1
	#	( uri_parameters headers? ) >2;

	#SIP_URI = 
	#	start:		(
	#					( "sip:"i %is_sip | "sips:"i %is_sips ) >tag %tag -> check
	#				),
	#	check:		(
	#					( (any* when has_arobase) )%parse_user_name -> usrinfo | (any*)-> hport
	#				),
	#	usrinfo:	(
	#					(any)* %parse_toto-> hport
	#				),
	#	hport:		(
	#					(any)* -> final
	#				);
	
	#TEL_URI = ("tel:"i %is_tel) telephone_subscriber;
	
	#URI =  SIP_URI;
	#URI =  ((SIP_URI)>1 | (TEL_URI)>1 | (UNKNOWN_URI)>0);
	
	# Entry point
	#main := ( 
	#		  ("sip:"i %is_sip | "sips:"i %is_sips) @ { fcall SIP_URI; } 
	#	  );

	sip_usrinfo		:= ( ( user>tag %parse_user_name ) :> ( ":" password>tag %parse_password )? :>> '@' ) @{ fgoto main; };
	main			:= |*
							("sip:"i %is_sip | "sips:"i %is_sips) > 100
							{
								if(tsk_strcontains(te, "@"))
								{
									fgoto sip_usrinfo;
								}
							};
							
							( (IPv6reference >is_ipv6)>89 | (IPv4address >is_ipv4)>88 | (hostname >is_hostname)>87 ) > 90
							{
								SCANNER_SET_STRING(uri->host);
							};							

							(":" port) >80
							{
								ts++;
								SCANNER_SET_INTEGER(uri->port);
							};
							
							( ";" (pname ( "=" pvalue )?) ) > 70
							{
								ts++;
								SACANNER_ADD_PARAM(uri->params);
							};
						
							(any*) > 0
							{
							};

						

					*|;

	#main := ({ fcall SIP_URI; });

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

	const char *ts = 0, *te = 0;
	int act =0;

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