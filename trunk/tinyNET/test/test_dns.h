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
#ifndef TNET_TEST_DNS_H
#define TNET_TEST_DNS_H

#include "dns/tnet_dns_naptr.h"

void test_dns_query()
{
	tnet_dns_ctx_t *ctx = TNET_DNS_CTX_CREATE();
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
					tnet_dns_naptr_t *naptr = (tnet_dns_naptr_t*)rr;
					
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
	
	TSK_OBJECT_SAFE_FREE(response);
	TSK_OBJECT_SAFE_FREE(ctx);


	tsk_thread_sleep(2000);
}

void test_dns_srv()
{
	tnet_dns_ctx_t *ctx = TNET_DNS_CTX_CREATE();
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
	tnet_dns_ctx_t *ctx = TNET_DNS_CTX_CREATE();
	char* hostname = 0;
	tnet_port_t port = 0;

	if(!tnet_dns_query_naptr_srv(ctx, "sip2sip.info", "SIP+D2U", &hostname, &port)){
		TSK_DEBUG_INFO("DNS NAPTR+SRV succeed ==> hostname=%s and port=%u", hostname, port);
	}

	TSK_FREE(hostname);
	TSK_OBJECT_SAFE_FREE(ctx);

	tsk_thread_sleep(2000);
}

void test_dns()
{
	//test_dns_naptr_srv();
	//test_dns_srv();
	test_dns_query();
}


#endif /* TNET_TEST_DNS_H */
