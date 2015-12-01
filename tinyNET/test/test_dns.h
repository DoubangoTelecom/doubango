/* Copyright (C) 2014 Mamadou DIOP.
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
#ifndef TNET_TEST_DNS_H
#define TNET_TEST_DNS_H

//#include "tnet_utils.h" /* tnet_address_t */

void test_dns_query()
{
	tnet_dns_ctx_t *ctx = tnet_dns_ctx_create();
	tnet_dns_response_t *response = tsk_null;
	const tsk_list_item_t* item;
	const tnet_dns_rr_t* rr;
	
	//if((response = tnet_dns_resolve(ctx, "_sip._udp.sip2sip.info", qclass_in, qtype_srv)))
	if((response = tnet_dns_resolve(ctx, "sip2sip.info", qclass_in, qtype_naptr)))
	{
		if(TNET_DNS_RESPONSE_IS_SUCCESS(response)){
			TSK_DEBUG_INFO("We got a success response from the DNS server.");
			// loop through the answers
			tsk_list_foreach(item, response->Answers){
				rr = item->data;
				if(rr->qtype == qtype_naptr){
					const tnet_dns_naptr_t *naptr = (const tnet_dns_naptr_t*)rr;
					
					TSK_DEBUG_INFO("order=%u pref=%u flags=%s services=%s regexp=%s replacement=%s", 
						naptr->order,
						naptr->preference,
						naptr->flags,
						naptr->services,
						naptr->regexp,
						naptr->replacement);
				}
			}
		}
		else{
			TSK_DEBUG_ERROR("We got an error response from the DNS server. Erro code: %u", response->Header.RCODE);
		}
	}
	
	tnet_dns_cache_clear(ctx);

	TSK_OBJECT_SAFE_FREE(response);
	TSK_OBJECT_SAFE_FREE(ctx);


	tsk_thread_sleep(2000);
}

void test_dns_srv()
{
	tnet_dns_ctx_t *ctx = tnet_dns_ctx_create();
	char* hostname = 0;
	tnet_port_t port = 0;

	if(!tnet_dns_query_srv(ctx, "_sip._udp.sip2sip.info", &hostname, &port)){
		TSK_DEBUG_INFO("DNS SRV succeed ==> hostname=%s and port=%u", hostname, port);
	}

	TSK_FREE(hostname);
	TSK_OBJECT_SAFE_FREE(ctx);

	tsk_thread_sleep(2000);
}

void test_dns_naptr_srv()
{
	tnet_dns_ctx_t *ctx = tnet_dns_ctx_create();
	char* hostname = tsk_null;
	tnet_port_t port = 0;

	if(!tnet_dns_query_naptr_srv(ctx, "sip2sip.info", "SIP+D2U", &hostname, &port)){
		TSK_DEBUG_INFO("DNS NAPTR+SRV succeed ==> hostname=%s and port=%u", hostname, port);
	}

	TSK_FREE(hostname);
	TSK_OBJECT_SAFE_FREE(ctx);

	tsk_thread_sleep(2000);
}

void test_enum()
{
	tnet_dns_ctx_t *ctx = tnet_dns_ctx_create();
	tnet_dns_response_t* response = tsk_null;
//	const tsk_list_item_t* item;
//	const tnet_dns_naptr_t* record;
	char* uri = tsk_null;
	const char* e164num = "+1-800-555-5555";
	//const char* e164num = "+33660188661";

	//tnet_dns_add_server(ctx, "192.168.16.9");

	//if((uri = tnet_dns_enum_2(ctx, "E2U+SIP", e164num, "e164.org"))){
	if((uri = tnet_dns_enum_2(ctx, "E2U+SIP", e164num, "e164.org"))){
		TSK_DEBUG_INFO("URI=%s", uri);
		TSK_FREE(uri);
	}
	else{
		TSK_DEBUG_ERROR("ENUM(%s) failed", e164num);
	}
	
	/*if((response = tnet_dns_enum(ctx, "+1-800-555-5555", "e164.org"))){
		if(TNET_DNS_RESPONSE_IS_SUCCESS(response)){
			TSK_DEBUG_INFO("We got a success response from the DNS server.");
			// loop through the answers
			tsk_list_foreach(item, response->Answers){
				record = item->data;
				
				TSK_DEBUG_INFO("order=%u pref=%u flags=%s services=%s regexp=%s replacement=%s", 
					record->order,
					record->preference,
					record->flags,
					record->services,
					record->regexp,
					record->replacement);
			}
		}
		else{
			TSK_DEBUG_ERROR("We got an error response from the DNS server. Erro code: %u", response->Header.RCODE);
		}
	}*/

	
	TSK_OBJECT_SAFE_FREE(response);
	TSK_OBJECT_SAFE_FREE(ctx);

	tsk_thread_sleep(2000);
}


typedef struct regexp_test_s{
	const char* e164num;
	const char* regexp;
	const char* xres;
}
regexp_test_t;

regexp_test_t regexp_tests[] = {
	"+18005551234", "!^.*$!sip:customer-service@example.com!i", "sip:customer-service@example.com",
	"+18005551234", "!^.*$!mailto:information@example.com!i", "mailto:information@example.com",

	"+18005555555", "!^\\+1800(.*)$!sip:1641641800\\1@tollfree.sip-happens.com!", "sip:16416418005555555@tollfree.sip-happens.com",
	"+18005555555", "!^\\+1800(.*)$!sip:1641641800\\1@sip.tollfreegateway.com!", "sip:16416418005555555@sip.tollfreegateway.com",
	
	"+468971234", "!^+46(.*)$!ldap://ldap.telco.se/cn=0\\1!", "ldap://ldap.telco.se/cn=08971234",
	"+468971234", "!^+46(.*)$!mailto:spam@paf.se!", "mailto:spam@paf.se",

	"urn:cid:199606121851.1@bar.example.com", "!!^urn:cid:.+@([^\\.]+\\.)(.*)$!\\2!i", "example.com",
};

void test_regex()
{
	char* ret;
	size_t i;

	for(i=0; i< sizeof(regexp_tests)/sizeof(regexp_test_t); i++)
	{
		if((ret = tnet_dns_regex_parse(regexp_tests[i].e164num, regexp_tests[i].regexp))){
			TSK_DEBUG_INFO("ENUM(%s) = %s", regexp_tests[i].e164num, ret);
			if(!tsk_strequals(ret, regexp_tests[i].xres)){
				TSK_DEBUG_ERROR("Failed to match ENUM(%s)", regexp_tests[i].e164num);
			}
			TSK_FREE(ret);
		}
		else{
			TSK_DEBUG_ERROR("Failed to parse ENUM(%s)", regexp_tests[i].e164num);
		}

		TSK_DEBUG_INFO("---------");
	}
}

void test_resolvconf()
{
	tnet_addresses_L_t * servers;
	const tnet_address_t* address;
	const tsk_list_item_t* item;
	const char* path = "C:\\tmp\\resolv32.conf";
	//const char* path = "C:\\tmp\\resolv.conf";
	//const char* path = "/etc/resolv.conf";
	
	if((servers = tnet_dns_resolvconf_parse(path))){
		tsk_list_foreach(item, servers){
			address = item->data;

			TSK_DEBUG_INFO("DNS Server host=%s Family=%d", address->ip, address->family);
		}

		TSK_OBJECT_SAFE_FREE(servers);
	}
	else{
		TSK_DEBUG_ERROR("Failed to parse DNS servers from %s.", path);
	}
}

void test_dns()
{
	test_dns_naptr_srv();
	//test_dns_srv();
	//test_dns_query();
	//test_enum();
	//test_regex();
	//test_resolvconf();
}


#endif /* TNET_TEST_DNS_H */
