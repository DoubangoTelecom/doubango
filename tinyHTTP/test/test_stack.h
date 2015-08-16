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

int test_stack_callback(const thttp_event_t *httpevent)
{
	thttp_session_id_t id = thttp_session_get_id(httpevent->session);
	switch(httpevent->type){
		case thttp_event_message: /* New HTTP message */
			{
				TSK_DEBUG_INFO("sid=%llu", id);
				if(THTTP_MESSAGE_IS_RESPONSE(httpevent->message)){
					const thttp_header_ETag_t* etag;
					TSK_DEBUG_INFO("=== %d ==> %s", THTTP_RESPONSE_CODE(httpevent->message), THTTP_MESSAGE_CONTENT(httpevent->message));
					// You can use 
					if((etag = (const thttp_header_ETag_t*)thttp_message_get_header(httpevent->message, thttp_htype_ETag))){
						TSK_DEBUG_INFO("Etag=%s", etag->value);
					}
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
				break;
			}

		case thttp_event_closed: /* HTTP connection closed (informational) */
			{
				TSK_DEBUG_INFO("closed sid=%llu", id);
				break;
			}

		case thttp_event_transport_error: /* HTTP connection closed (informational) */
			{
				TSK_DEBUG_INFO("Transport sid=%llu", id);
				break;
			}
	}
	
	return 0;
}

#define PAYLOAD "<entry uri=\"sip:samba@micromethod.com\" xmlns=\"urn:ietf:params:xml:ns:resource-lists\">" \
				"<display-name>samba</display-name>" \
				"</entry>"

void test_stack()
{
	thttp_session_handle_t *session = tsk_null;
	int ret;

	thttp_stack_handle_t* stack = thttp_stack_create(test_stack_callback,
		THTTP_STACK_SET_TLS_ENABLED(tsk_true),
		//THTTP_STACK_SET_TLS_CERTS_VERIFY(tsk_false),
		//THTTP_STACK_SET_LOCAL_IP("0.0.0.0"),
		//THTTP_STACK_SET_LOCAL_PORT(8080),
		//THTTP_STACK_SET_PROXY("msp.f-secure.com", 443),

		THTTP_STACK_SET_NULL());

	if((ret = thttp_stack_start(stack))){
		TSK_DEBUG_ERROR("Failed to start the HTTP/HTTPS stack.");
		goto bail;
	}
	

	// http://ipv6.google.com/
	//op = THTTP_OPERATION_CREATE(stack,
	//	THTTP_OPERATION_SET_PARAM("method", "GET"),
	//	THTTP_OPERATION_SET_PARAM("URL", "http://siptest.doubango.org:8080/services/resource-lists/users/sip:mercuro1@doubango.org/index"),
	//	
	//	THTTP_OPERATION_SET_HEADER("Content-Type", "application/resource-lists+xml"),
	//	THTTP_OPERATION_SET_HEADER("Pragma", "No-Cache"),
	//	THTTP_OPERATION_SET_HEADER("Connection", "Keep-Alive"),
	//	THTTP_OPERATION_SET_HEADER("User-Agent", "XDM-client/OMA1.1"),
	//	THTTP_OPERATION_SET_HEADER("X-3GPP-Intended-Identity", "sip:mercuro1@doubango.org"),
	//	
	//	THTTP_OPERATION_SET_NULL());
	//thttp_operation_perform(op);

	/* creates session */
	session = thttp_session_create(stack,
		// session-level options
		THTTP_SESSION_SET_OPTION(THTTP_SESSION_OPTION_TIMEOUT, "6000"),

		// session-level headers
		THTTP_SESSION_SET_HEADER("Pragma", "No-Cache"),
		THTTP_SESSION_SET_HEADER("Connection", "Keep-Alive"),
		// THTTP_SESSION_SET_HEADER("Connection", "close"),
		THTTP_SESSION_SET_HEADER("User-Agent", "doubango 1.0"),
		
		THTTP_SESSION_SET_NULL()); /* MUST always be present */

	//ret = thttp_action_GET(session, "http://siptest.doubango.org:8080/services/resource-lists/users/sip:mercuro1@doubango.org/index",
	//	THTTP_ACTION_SET_HEADER("Content-Type", "application/resource-lists+xml"),
	//	
	//	tsk_null);

	//getchar();

	//thttp_action_GET(session, "http://siptest.doubango.org:8080/services/resource-lists/users/sip:mercuro1@doubango.org/index",
	//	THTTP_ACTION_SET_HEADER("Content-Type", "application/resource-lists+xml"),
	//	
	//	tsk_null);

	//getchar();

	// HTTPS
	thttp_action_GET(session, "https://msp.f-secure.com/web-test/common/test.html",
		// action-level options
		THTTP_ACTION_SET_OPTION(THTTP_ACTION_OPTION_TIMEOUT, "2500"),
		
		// action-level headers
		THTTP_ACTION_SET_HEADER("User-Agent", "Doubango"),
		THTTP_ACTION_SET_HEADER("Connection", "Keep-Alive"),
		
		THTTP_ACTION_SET_NULL());
	getchar();
	

	// IPv6
	/*thttp_action_GET(session, "http://ipv6.google.com",
		// action-level options
		THTTP_ACTION_SET_OPTION(THTTP_ACTION_OPTION_TIMEOUT, "2500"),
		
		// action-level headers
		THTTP_ACTION_SET_HEADER("User-Agent", "XDM-client/OMA1.1"),
		THTTP_ACTION_SET_HEADER("Connection", "Keep-Alive"),
		
		THTTP_ACTION_SET_NULL());
	getchar();*/

	/*ret = thttp_action_GET(session, "http://doubango.org",
			// action-level options
			THTTP_ACTION_SET_OPTION(THTTP_ACTION_OPTION_TIMEOUT, "2500"),
			
			THTTP_ACTION_SET_NULL());

	getchar();*/

	//thttp_action_GET(session, "http://www.google.com",
		//THTTP_ACTION_SET_HEADER("Content-Type", "application/resource-lists+xml"),
		
	//	THTTP_ACTION_SET_NULL());

	//thttp_action_GET(session, "http://www.doubango.org",
		
	//	THTTP_ACTION_SET_NULL());	

	/* Gets resource-lists document */
	/*thttp_action_GET(session, "http://siptest.doubango.org:8080/services/resource-lists/users/sip:mercuro1@doubango.org/index",
		THTTP_ACTION_SET_HEADER("Content-Type", "application/resource-lists+xml"),
		
		tsk_null);

	getchar();*/

	//TSK_OBJECT_SAFE_FREE(session);

	//getchar();

	/* Gets xcap-caps document */
	/*thttp_action_GET(session, "http://siptest.doubango.org:8080/services/xcap-caps/global/index",
		THTTP_ACTION_SET_HEADER("Content-Type", "application/xcap-caps+xml"),
		
		tsk_null);

	getchar();*/

	/*thttp_action_GET(session, "http://siptest.doubango.org:8080/services/resource-lists/users/sip:mercuro1@doubango.org/properties-resource-list.xml",
		THTTP_ACTION_SET_HEADER("Content-Type", "application/resource-lists+xml"),
		
		tsk_null);

	getchar();*/

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


