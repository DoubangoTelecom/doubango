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
#ifndef _TEST_SIPURI_H
#define _TEST_SIPURI_H

#include "tinysip/parsers/tsip_parser_uri.h"

const char* uris[] = 
{
	
	//== SIP:
	"sip:alice@atlanta.com;p1=23",
	"sip:*666*@atlanta.com",
	"sip:#66#@atlanta.com",
	"sip:alice:secretword@atlanta.com",
	"sip:alice:secretword@atlanta.com:65535;transport=tcp",
    "sip:+1-212-555-1212:1234@gateway.com;user=phone",
	"sip:alice@192.0.2.4:5060",
	"sip:alice@[1111::aaa:bbb:ccc:ddd]:5060",
	"sip:atlanta.com",
	"sip:alice@[1111::aaa:bbb:ccc:ddd]",
	"sip:alice@[1111::aaa:bbb:ccc:ddd]:5060;user=phone",
	"sip:alice@1111::aaa:bbb:ccc:ddd",
	"sip:alice@[::127]",
	"sip:ss2.biloxi.example.com;lr",// FIXME
    "sip:atlanta.com;method=REGISTER?to=alice%40atlanta.com",
    "sip:alice@atlanta.com;maddr=239.255.255.1;ttl=15",
    "sip:alice@atlanta.com;comp=sigcomp",
	"sip:atlanta.com;method=REGISTER?to=alice%40atlanta.com",
    "sip:alice@atlanta.com?subject=project%20x&priority=urgent",

	//== SIPS:
	"sips:alice@atlanta.com",
    "sips:alice:secretword@atlanta.com;transport=tcp",
    "sips:+1-212-555-1212:1234@gateway.com;user=phone",
    "sips:alice@192.0.2.4",
    "sips:atlanta.com;method=REGISTER?to=alice%40atlanta.com",
    "sips:alice@atlanta.com;maddr=239.255.255.1;ttl=15",
    "sips:alice@atlanta.com;comp=sigcomp",
	"sips:atlanta.com;method=REGISTER?to=alice%40atlanta.com",
    "sips:alice@atlanta.com?subject=project%20x&priority=urgent",

	//== TEL:
	"tel:+1-201-555-0123",
    "tel:7042;phone-context=example.com;ff=ff",
    "tel:863-1234;phone-context=+1-914-555",
	"tel:#666#",
};
#include "tsk_string.h"

void test_uri_tostring(const tsip_uri_t *uri)
{
	tsk_buffer_t *buffer = TSK_BUFFER_CREATE_NULL();

	tsip_uri_tostring(uri, 1, 1, buffer);
	TSK_DEBUG_INFO("uri_to_string=%s", TSK_BUFFER_TO_STRING(buffer));

	TSK_OBJECT_SAFE_FREE(buffer);
}

void test_uri_parser()
{
	int i;
	tsk_list_item_t *item = 0;

	for(i=0; i<sizeof(uris)/sizeof(const char*); i++)
	{
		tsip_uri_t *uri = tsip_uri_parse(uris[i], strlen(uris[i]));
				
		printf("\n== Parsing {{ %s }} ==\n\n", uris[i]);
		
		if(uri)
		{
			printf("scheme: %s\n", uri->scheme);
			printf("user-name: %s\n", uri->user_name);
			printf("password: %s\n", uri->password);
			printf("host: %s\n", uri->host);
			printf("port: %d\n", uri->port);
			printf("host-type: %s\n", uri->host_type == host_ipv4 ? "IPv4" : (uri->host_type == host_ipv6 ? "IPv6" : (uri->host_type == host_hostname ? "HOSTNAME" : "UNKNOWN")) );
			
			printf("---PARAMS---\n");

			/* dump all parameters */
			tsk_list_foreach(item, uri->params)
			{
				tsk_param_t* param = item->data;
				printf("-->%s=%s\n", param->name, param->value);
			}

			printf("Is-secure: %s\n", TSIP_URI_IS_SECURE(uri) ? "YES" : "NO");

			test_uri_tostring(uri);
		}
		else
		{
			printf("INVALID SIP URI.\n");
		}

		printf("\n\n");
		
		TSK_OBJECT_SAFE_FREE(uri);
	}
}

void test_uri()
{
	test_uri_parser();
}

#endif /* _TEST_SIPURI_H */