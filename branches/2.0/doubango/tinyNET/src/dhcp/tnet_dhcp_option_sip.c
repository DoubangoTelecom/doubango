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
/**@file tnet_dhcp_option_sip.c
 * @brief  Dynamic Host Configuration Protocol (DHCP-for-IPv4) Option for 
 *			Session Initiation Protocol (SIP) Servers as per RFC 3361.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tnet_dhcp_option_sip.h"

#include "dns/tnet_dns_rr.h"

#include "../tnet_types.h"
#include "../tnet_endianness.h"

#include "tsk_memory.h"
#include "tsk_string.h"

tnet_dhcp_option_sip_t* tnet_dhcp_option_sip_create(const void* payload, tsk_size_t payload_size)
{
	return tsk_object_new(tnet_dhcp_option_sip_def_t, payload, payload_size);
}

//
//	[[DHCP SIP4]] object definition
//
static tsk_object_t* tnet_dhcp_option_sip_ctor(tsk_object_t * self, va_list * app)
{
	tnet_dhcp_option_sip_t *option = self;
	if(option){
		const void* payload = va_arg(*app, const void*);
		tsk_size_t payload_size = va_arg(*app, tsk_size_t);

		const uint8_t* payloadPtr = (const uint8_t*)payload;
		const uint8_t* payloadEnd = (payloadPtr + payload_size);
		
		/* init base */
		tnet_dhcp_option_init(TNET_DHCP_OPTION(option), dhcp_code_SIP_Servers_DHCP_Option);

		option->servers = tsk_list_create();

		/* Set values as per RFC 3361. */
		if(*payloadPtr == 0){ /* enc=0 */
			/*
			+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
			|120|27 | 0 | 7 |'e'|'x'|'a'|'m'|'p'|'l'|'e'| 3 |'c'|'o'|'m'| 0 |
			+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
			+---+---+---+---+---+---+---+---+---+---+---+---+---+ | 7
			|'e'|'x'|'a'|'m'|'p'|'l'|'e'| 3 |'n'|'e'|'t'| 0 | +---+---+---
			+---+---+---+---+---+---+---+---+---+---+
			*/
			tsk_size_t offset = 1;
			char* server = 0;
			payloadPtr++;
			while((payloadPtr < payloadEnd) && !tnet_dns_rr_qname_deserialize(payload, &server, &offset)){
				tsk_string_t* string = tsk_string_create(server);
				tsk_list_push_back_data(option->servers, (void*)&string);
				TSK_FREE(server);
				payloadPtr += offset;
			}
		}
		else{
			/*
			Code   Len   enc   Address 1               Address 2
			+-----+-----+-----+-----+-----+-----+-----+-----+--
			| 120 |  n  |  1  | a1  | a2  | a3  | a4  | a1  |  ...
			+-----+-----+-----+-----+-----+-----+-----+-----+--
			*/
			uint32_t address;
			tsk_string_t* addrstring;
			char* ip4 = 0;

			while(payloadPtr < payloadEnd){
				++payloadPtr;
				address = tnet_htonl_2(payloadPtr);
				
				tsk_sprintf(&ip4, "%u.%u.%u.%u", (address>>24)&0xFF, (address>>16)&0xFF, (address>>8)&0xFF, (address>>0)&0xFF);
				
				addrstring = tsk_string_create(ip4);
				tsk_list_push_back_data(option->servers, (void*)&addrstring);

				TSK_FREE(ip4);

				payloadPtr+= 4;
			}
		}
	}
	return self;
}

static tsk_object_t* tnet_dhcp_option_sip_dtor(tsk_object_t * self) 
{ 
	tnet_dhcp_option_sip_t *option = self;
	if(option){
		/* deinit base */
		tnet_dhcp_option_deinit(TNET_DHCP_OPTION(option));

		TSK_OBJECT_SAFE_FREE(option->servers);
	}
	return self;
}

static const tsk_object_def_t tnet_dhcp_option_sip_def_s =
{
	sizeof(tnet_dhcp_option_sip_t),
	tnet_dhcp_option_sip_ctor,
	tnet_dhcp_option_sip_dtor,
	tsk_null,
};
const tsk_object_def_t *tnet_dhcp_option_sip_def_t = &tnet_dhcp_option_sip_def_s;
