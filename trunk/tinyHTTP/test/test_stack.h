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
#ifndef _TEST_STACK_H_
#define _TEST_STACK_H_

int test_stack_callback(const thttp_event_t *httpevent)
{
	switch(httpevent->type){
		case thttp_event_message: /* New HTTP message */
			{
				TSK_DEBUG_INFO("opid=%llu", httpevent->opid);
				if(THTTP_RESPONSE_IS_2XX(httpevent->message)){
					TSK_DEBUG_INFO("=== 2xx ==> %s", THTTP_MESSAGE_CONTENT(httpevent->message));
					// You can use 
				}
				else{
					if(THTTP_MESSAGE_IS_RESPONSE(httpevent->message)){
						TSK_DEBUG_INFO("=== code ==> %u", THTTP_RESPONSE_CODE(httpevent->message));
					}
				}
				break;
			}

		case thttp_event_closed: /* HTTP connection closed */
			{
				TSK_DEBUG_INFO("opid=%llu", httpevent->opid);
				// Delete the associated operation if you want
				// perform(op) on a closed operation will open a new TCP/TLS connection.
			}
	}
	
	return 0;
}

void test_stack()
{
	thttp_operation_handle_t *op = 0;
	thttp_stack_handle_t* stack = thttp_stack_create(test_stack_callback, 
		THTTP_STACK_SET_NULL());

	if(thttp_stack_start(stack)){
		goto bail;
	}

	op = THTTP_OPERATION_CREATE(stack,
		THTTP_OPERATION_SET_PARAM("method", "GET"),
		THTTP_OPERATION_SET_PARAM("URL", "http://siptest.colibria.com:8080/services/resource-lists/users/sip:mercuro1@colibria.com/index"),
		
		THTTP_OPERATION_SET_HEADER("Content-Type", "application/resource-lists+xml"),
		THTTP_OPERATION_SET_HEADER("Pragma", "No-Cache"),
		THTTP_OPERATION_SET_HEADER("Connection", "Keep-Alive"),
		THTTP_OPERATION_SET_HEADER("User-Agent", "XDM-client/OMA1.1"),
		THTTP_OPERATION_SET_HEADER("X-3GPP-Intended-Identity", "sip:mercuro1@colibria.com"),
		
		THTTP_OPERATION_SET_NULL());
	thttp_operation_perform(op);

	getchar();

	thttp_operation_perform(op);

/*
	op = THTTP_OPERATION_CREATE(stack,
		THTTP_OPERATION_SET_PARAM("method", "GET"),
		//THTTP_OPERATION_SET_PARAM("URL", "https://msp.f-secure.com/web-test/common/test.html"),
		THTTP_OPERATION_SET_PARAM("URL", "http://www.doubango.org"),
		
		THTTP_OPERATION_SET_HEADER("Pragma", "No-Cache"),
		THTTP_OPERATION_SET_HEADER("Connection", "Keep-Alive"),
		THTTP_OPERATION_SET_HEADER("User-Agent", "XDM-client/OMA1.1"),
				
		THTTP_OPERATION_SET_NULL());
	thttp_operation_perform(op);
*/
	/*thttp_operation_set(op,
		THTTP_OPERATION_SET_PARAM("method", "HEAD"),
		
		THTTP_OPERATION_SET_NULL());
	thttp_operation_perform(op);*/

	getchar();
bail:
	TSK_OBJECT_SAFE_FREE(op);
	TSK_OBJECT_SAFE_FREE(stack);
}

#endif /* _TEST_STACK_H_ */


