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
#ifndef _TEST_MSRPSESSION_H
#define _TEST_MSRPSESSION_H

#include "tinySDP/headers/tsdp_header_M.h"
#include "tinySDP/headers/tsdp_header_Dummy.h"

#include "tinySDP/parsers/tsdp_parser_message.h"

#define REMOTE_SDP1 \
	"c=IN IP4 192.168.0.15\r\n" \
    "m=message 2000 TCP/MSRP *\r\n" \
    "a=accept-types:text/plain\r\n" \
    "a=path:msrp://atlanta.example.com:7654/jshA7weztas;tcp\r\n" \
	"a=setup:passive\r\n" \
    "a=connection:new\r\n"

#define REMOTE_SDP2 \
    "m=message 2000 TCP/MSRP *\r\n" \
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
		if((m = tmedia_get_local_offer(msrp,
				TSDP_HEADER_I_VA_ARGS("This is my latest picture"),
				TSDP_HEADER_A_VA_ARGS("sendonly", tsk_null),
				TSDP_HEADER_A_VA_ARGS("file-selector", "\"My cool picture.jpg\" type:image/jpeg size:32349 hash:sha-1:72:24:5F:E8:65:3D:DA:F3:71:36:2F:86:D4:71:91:3E:E4:A2:CE:2E"),
				TSDP_HEADER_A_VA_ARGS("file-transfer-id", "vBnG916bdberum2fFEABR1FR3ExZMUrd"),
				TSDP_HEADER_A_VA_ARGS("file-disposition", "attachment"),
				TSDP_HEADER_A_VA_ARGS("file-date", "creation:\"Mon, 15 May 2006 15:01:31 +0300\""),
				TSDP_HEADER_A_VA_ARGS("file-icon", "cid:id2@alicepc.example.com"),
				TSDP_HEADER_A_VA_ARGS("file-range", "1-32349"),
				
				TSDP_HEADER_DUMMY_VA_ARGS("x", "007"),
				
				tsk_null
			))){
			if((str = tsdp_header_tostring(TSDP_HEADER(m)))){
				TSK_DEBUG_INFO("m(offer)=%s", str);
				TSK_FREE(str);
			}
		}

		/*if((m = tmedia_get_local_offer(msrp,
				TSDP_HEADER_I_VA_ARGS("Message Transfer"),
				TSDP_HEADER_A_VA_ARGS("accept-types", "text/plain")
				
				tsk_null
			))){
			if((str = tsdp_header_tostring(TSDP_HEADER(m)))){
				TSK_DEBUG_INFO("m(offer)=%s", str);
				TSK_FREE(str);
			}
		}*/
		
		// Set offer
		if((remote = tsdp_message_parse(REMOTE_SDP, strlen(REMOTE_SDP)))){
			tmedia_set_remote_offer(msrp, remote);
			TSK_OBJECT_SAFE_FREE(remote);
		}
		
		tmedia_get_negotiated_offer(msrp);
		
		tmedia_start(msrp);
		//tmedia_pause(msrp);
		
		/*
		tmedia_perform(msrp, tma_msrp_send_data,
				TSK_PARAM_VA_ARGS("Content", "hello world!"),

				tsk_null);
		*/
		
		tmedia_perform(msrp, tma_msrp_send_file,
			TSK_PARAM_VA_ARGS("Path", "C:\\Projects\\Doubango\\doubango.ncb"),

			tsk_null);
		

		getchar();

		tmedia_stop(msrp);
		
		TSK_OBJECT_SAFE_FREE(msrp);
	}
}

#endif /* _TEST_MSRPSESSION_H */
