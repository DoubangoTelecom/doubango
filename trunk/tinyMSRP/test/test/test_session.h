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
* GNU General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/
#ifndef _TEST_MSRPSESSION_H
#define _TEST_MSRPSESSION_H

#include "tinySDP/headers/tsdp_header_M.h"
#include "tinySDP/parsers/tsdp_parser_message.h"

#define REMOTE_SDP1 \
	"c=IN IP4 192.168.0.15\r\n" \
    "m=message 5060 TCP/MSRP *\r\n" \
    "a=accept-types:text/plain\r\n" \
    "a=path:msrp://atlanta.example.com:7654/jshA7weztas;tcp\r\n" \
	"a=setup:passive\r\n" \
    "a=connection:new\r\n"

#define REMOTE_SDP2 \
    "m=message 5060 TCP/MSRP *\r\n" \
	"c=IN IP4 192.168.0.15\r\n" \
    "a=accept-types:text/plain\r\n" \
    "a=path:msrp://atlanta.example.com:7654/jshA7weztas;tcp\r\n" \
	"a=setup:passive\r\n" \
    "a=connection:new\r\n"

#define REMOTE_SDP REMOTE_SDP1

void test_session()
{
	tmedia_t* msrp = tsk_null;
	char* str;
	const tsdp_header_M_t *m;
	tsdp_message_t* remote;
	
	// Register dummy media
	tmedia_plugin_register(tmsrp_media_plugin_def_t);
	
	if((msrp = tmedia_factory_create("msrp", TNET_SOCKET_HOST_ANY, tnet_socket_type_tcp_ipv4))){
		
		// Get offer
		if((m = tmedia_get_local_offer(msrp))){
			if((str = tsdp_header_tostring(TSDP_HEADER(m)))){
				TSK_DEBUG_INFO("m(offer)=%s", str);
				TSK_FREE(str);
			}
		}
		
		// Set offer
		if((remote = tsdp_message_parse(REMOTE_SDP, strlen(REMOTE_SDP)))){
			tmedia_set_remote_offer(msrp, remote);
			TSK_OBJECT_SAFE_FREE(remote);
		}
		
		tmedia_get_negotiated_offer(msrp);
		
		tmedia_start(msrp);
		tmedia_pause(msrp);
		tmedia_stop(msrp);
		
		TSK_OBJECT_SAFE_FREE(msrp);
	}
}

#endif /* _TEST_MSRPSESSION_H */
