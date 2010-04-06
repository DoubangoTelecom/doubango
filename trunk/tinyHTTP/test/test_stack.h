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
#ifndef _TEST_STACK_H_
#define _TEST_STACK_H_

#include "tinyhttp.h"

int test_stack_callback(const thttp_event_t *httpevent)
{
	thttp_session_id_t id = thttp_session_get_id(httpevent->session);
	switch(httpevent->type){
		case thttp_event_message: /* New HTTP message */
			{
				TSK_DEBUG_INFO("sid=%llu", id);
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

		case thttp_event_auth_failed:
			{
				TSK_DEBUG_INFO("auth failed sid=%llu", id);
			}

		case thttp_event_closed: /* HTTP connection closed (informational) */
			{
				TSK_DEBUG_INFO("closed sid=%llu", id);
			}
	}
	
	return 0;
}

void test_stack()
{
	thttp_session_handle_t *session = tsk_null;
	int ret;

	thttp_stack_handle_t* stack = thttp_stack_create(test_stack_callback,
		THTTP_STACK_SET_NULL());

	if((ret = thttp_stack_start(stack))){
		TSK_DEBUG_ERROR("Failed to start the HTTP/HTTPS stack.");
		goto bail;
	}
	

	// http://ipv6.google.com/
	//op = THTTP_OPERATION_CREATE(stack,
	//	THTTP_OPERATION_SET_PARAM("method", "GET"),
	//	THTTP_OPERATION_SET_PARAM("URL", "http://siptest.colibria.com:8080/services/resource-lists/users/sip:mercuro1@colibria.com/index"),
	//	
	//	THTTP_OPERATION_SET_HEADER("Content-Type", "application/resource-lists+xml"),
	//	THTTP_OPERATION_SET_HEADER("Pragma", "No-Cache"),
	//	THTTP_OPERATION_SET_HEADER("Connection", "Keep-Alive"),
	//	THTTP_OPERATION_SET_HEADER("User-Agent", "XDM-client/OMA1.1"),
	//	THTTP_OPERATION_SET_HEADER("X-3GPP-Intended-Identity", "sip:mercuro1@colibria.com"),
	//	
	//	THTTP_OPERATION_SET_NULL());
	//thttp_operation_perform(op);

	/* creates session */
	session = thttp_session_create(stack,
		// session-level parameters
		THTTP_SESSION_SET_PARAM("timeout", "6000"),

		// session-level headers
		THTTP_SESSION_SET_HEADER("Pragma", "No-Cache"),
		THTTP_SESSION_SET_HEADER("Connection", "Keep-Alive"),
		THTTP_SESSION_SET_HEADER("User-Agent", "XDM-client/OMA1.1"),
		THTTP_SESSION_SET_HEADER("X-3GPP-Intended-Identity", "sip:mercuro1@colibria.com"),
		
		THTTP_SESSION_SET_NULL()); /* MUST always be present */

	thttp_session_set(session,
		// credentials
		THTTP_SESSION_SET_CRED("sip:mercuro1@colibria.com", "mercuro13"),

		THTTP_SESSION_SET_NULL());
	
	//thttp_action_GET(session, "http://ipv6.google.com",
	//	THTTP_ACTION_SET_HEADER("Content-Type", "application/resource-lists+xml"),
		
	//	THTTP_ACTION_SET_NULL());

	//thttp_action_GET(session, "http://www.google.com",
		//THTTP_ACTION_SET_HEADER("Content-Type", "application/resource-lists+xml"),
		
	//	THTTP_ACTION_SET_NULL());

	//thttp_action_GET(session, "http://www.doubango.org",
		
	//	THTTP_ACTION_SET_NULL());	

	/* Gets resource-lists document */
	thttp_action_GET(session, "http://siptest.colibria.com:8080/services/resource-lists/users/sip:mercuro1@colibria.com/index",
		THTTP_ACTION_SET_HEADER("Content-Type", "application/resource-lists+xml"),
		
		tsk_null);

	getchar();

	//TSK_OBJECT_SAFE_FREE(session);

	//getchar();

	/* Gets xcap-caps document */
	thttp_action_GET(session, "http://siptest.colibria.com:8080/services/xcap-caps/global/index",
		THTTP_ACTION_SET_HEADER("Content-Type", "application/xcap-caps+xml"),
		
		tsk_null);

	getchar();

	thttp_action_GET(session, "http://siptest.colibria.com:8080/services/resource-lists/users/sip:mercuro1@colibria.com/properties-resource-list.xml",
		THTTP_ACTION_SET_HEADER("Content-Type", "application/resource-lists+xml"),
		
		tsk_null);

	getchar();

	//thttp_operation_perform(op);

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
	TSK_OBJECT_SAFE_FREE(session);
	TSK_OBJECT_SAFE_FREE(stack);
}

#endif /* _TEST_STACK_H_ */


