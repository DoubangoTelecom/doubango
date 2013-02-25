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

/**@file thttp_parser_url.c
 * @brief HTTP/HTTPS URL parser.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinyhttp/parsers/thttp_parser_url.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

/***********************************
*	Ragel state machine.
*/
%%{
	machine thttp_machine_parser_url;

	# Includes
	include thttp_machine_utils "./ragel/thttp_machine_utils.rl";
			
	action tag{
		tag_start = p;
	}

	#/* Sets URL type */
	action is_http { url->scheme = tsk_strdup("http"), url->type = thttp_url_http; }
	action is_https { url->scheme = tsk_strdup("https"), url->type = thttp_url_https; }

	#/* Sets HOST type */
	action is_ipv4 { url->host_type = url->host_type = thttp_host_ipv4; }
	action is_ipv6 { url->host_type = url->host_type = thttp_host_ipv6; }
	action is_hostname { url->host_type = url->host_type = thttp_host_hostname; }

	action parse_host{
		TSK_PARSER_SET_STRING(url->host);
	}

	action parse_port{
		have_port = 1;
		TSK_PARSER_SET_INT(url->port);
	}

	action parse_hpath{
		TSK_PARSER_SET_STRING(url->hpath);
	}

	action parse_search{
		TSK_PARSER_SET_STRING(url->search);
	}

	action eob{
	}

	#// RFC 1738: "http://" hostport [ "/" hpath [ "?" search ]]
	#// FIXME: hpath is no optional (see above) but in my def. I use it as opt (any*).

	search = any* >tag %parse_search;
	hpath = any* >tag %parse_hpath;
	port = DIGIT+ >tag %parse_port;
	myhost = ((IPv6reference >is_ipv6) | (IPv4address >is_ipv4) | (hostname >is_hostname)) >tag %parse_host;
	hostport = myhost ( ":" port )?;
	main := ( (("http:"i>tag %is_http | "https:"i>tag %is_https) "//")? hostport? :>("/" hpath :>("?" search)?)? ) @eob;
	#main := ( hostport? :>("/" hpath :>("?" search)?)? ) @eob;
	
}%%

/**@ingroup thttp_url_group
* Parses a HTTP/HTTPS url.
* @param urlstring A pointer to a valid url string. If the port is missing, then it's default value will be 443 if
* the scheme is 'https' and 80 otherwise.<br>
* @param length The length of the url string.
* @retval A well-defined object representing the url string.
*
* @code
* thttp_url_t* url = thttp_url_parse("http://www.google.com", tsk_strlen("http://www.google.com"));
* @endcode
*
* @sa @ref thttp_url_tostring<br>@ref thttp_url_serialize
**/
thttp_url_t *thttp_url_parse(const char *urlstring, tsk_size_t length)
{
	tsk_bool_t have_port = tsk_false;
	int cs = 0;
	const char *p = urlstring;
	const char *pe = p + length;
	const char *eof = pe;

	const char *ts = 0, *te = 0;
	int act =0;

	thttp_url_t *url = thttp_url_create(thttp_url_unknown);
	
	const char *tag_start = 0;
	
	%%write data;
	(void)(ts);
	(void)(te);
	(void)(act);
	(void)(eof);
	(void)(thttp_machine_parser_url_first_final);
	(void)(thttp_machine_parser_url_error);
	(void)(thttp_machine_parser_url_en_main);
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse HTTP/HTTPS URL: '%.*s'", length, urlstring);
		TSK_OBJECT_SAFE_FREE(url);
	}
	else if(!have_port){
		if(url->type == thttp_url_https){
			url->port = 443;
		}
		else{
			url->port = 80;
		}
	}
	
	return url;
}
