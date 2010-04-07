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
* GNU Lesser General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/
#ifndef _TEST_STACK_H
#define _TEST_STACK_H

int test_stack_callback(const thttp_event_t *httpevent)
{
	return 0;
}

void test_stack()
{
	txcap_stack_handle_t* stack = tsk_null;
	int ret;

	stack = txcap_stack_create(test_stack_callback, "sip:mamadou@micromethod.com", "mysecret", "http://192.168.16.104:8080/services",
		
		// options
		TXCAP_STACK_SET_OPTION(TXCAP_STACK_OPTION_TIMEOUT, "6000"),
		
		// stack-level headers
		TXCAP_STACK_SET_HEADER("Pragma", "No-Cache"),
		TXCAP_STACK_SET_HEADER("Connection", "Keep-Alive"),
		TXCAP_STACK_SET_HEADER("User-Agent", "XDM-client/OMA1.1"),
		
		TXCAP_STACK_SET_NULL());
	
	if((ret = txcap_stack_start(stack))){
		goto bail;
	}

	/* add user auids */
	txcap_stack_set(stack,
		TXCAP_STACK_SET_AUID("xcap-caps2", "application/xcap-caps2+xml", "urn:ietf:params:xml:ns:xcap-caps2", "index2", tsk_true),
		TXCAP_STACK_SET_AUID("resource-lists2", "application/resource-lists+xml2", "urn:ietf:params:xml:ns:resource-lists2", "index2", tsk_false),
		
		TXCAP_STACK_SET_NULL()); /* mandatory */

bail:
	TSK_OBJECT_SAFE_FREE(stack);
}

#endif /* _TEST_STACK_H */
