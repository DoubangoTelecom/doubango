/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango.org>
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
#ifndef _TEST_HTTPURL_H
#define _TEST_HTTPURL_H

#include "tinyhttp/parsers/thttp_parser_url.h"

const char* urls[] = 
{
	
	//== HTTP:
	"http://www.doubango.org",
	"http://www.doubango.org/?p=1",
	"http://doubango.org:452/test",
	"http://doubango.org:45/test?p=452",

	//== HTTPS:
	"https://www.doubango.org",
	"https://www.doubango.org/?p=1",
	"https://doubango.org:452/test",
	"https://doubango.org:45/test?p=452",
};
#include "tsk_string.h"

void test_url_tostring(const thttp_url_t *url)
{
	char* ret = thttp_url_tostring(url);
	TSK_DEBUG_INFO("url_to_string=%s", ret);
	TSK_FREE(ret);
}

void test_url_parser()
{
	int i;

	for(i=0; i<sizeof(urls)/sizeof(const char*); i++)
	{
		thttp_url_t *url = thttp_url_parse(urls[i], strlen(urls[i]));
				
		printf("\n== Parsing {{ %s }} ==\n\n", urls[i]);
		
		if(url)
		{
			printf("scheme: %s\n", url->scheme);
			printf("host: %s\n", url->host);
			printf("port: %u\n", url->port);
			printf("hpath: %s\n", url->hpath);
			printf("search: %s\n", url->search);
			printf("host-type: %s\n", url->host_type == thttp_host_ipv4 ? "IPv4" : (url->host_type == thttp_host_ipv6 ? "IPv6" : (url->host_type == thttp_host_hostname ? "HOSTNAME" : "UNKNOWN")) );
			
			printf("---PARAMS---\n");

			printf("Is-secure: %s\n", THTTP_URL_IS_SECURE(url) ? "YES" : "NO");

			test_url_tostring(url);
		}
		else
		{
			printf("INVALID HTTP URL.\n");
		}

		printf("\n\n");
		getchar();
		
		TSK_OBJECT_SAFE_FREE(url);
	}
}

void test_url()
{
	test_url_parser();
}

#endif /* _TEST_HTTPURL_H */
