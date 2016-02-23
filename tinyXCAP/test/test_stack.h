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

#define PAYLOAD_LIST "<list name=\"newlist\" xmlns=\"urn:ietf:params:xml:ns:resource-lists\">" \
					 "<display-name>newlist</display-name>" \
					 "</list>"
#define PAYLOAD_ENTRY "<entry uri=\""XUI"\" xmlns=\"urn:ietf:params:xml:ns:resource-lists\">" \
					  "<display-name>"XUI"</display-name>" \
					  "</entry>"

int test_stack_callback(const thttp_event_t *httpevent)
{
    switch(httpevent->type) {
    case thttp_event_message: { /* New HTTP message */
        if(THTTP_MESSAGE_IS_RESPONSE(httpevent->message)) {
            const thttp_header_ETag_t* etag;
            TSK_DEBUG_INFO("=== %d ==> %s", THTTP_RESPONSE_CODE(httpevent->message), THTTP_MESSAGE_CONTENT(httpevent->message));
            // You can use
            if((etag = (const thttp_header_ETag_t*)thttp_message_get_header(httpevent->message, thttp_htype_ETag))) {
                TSK_DEBUG_INFO("Etag=%s", etag->value);
            }
        }
        else {

        }
        break;
    }

    case thttp_event_auth_failed: {
        TSK_DEBUG_INFO("=== auth failed.");
        break;
    }

    case thttp_event_closed: { /* HTTP connection closed (informational) */
        TSK_DEBUG_INFO("=== closed.");
        break;
    }

    default: {
        break;
    }
    }
    return 0;
}

void test_stack()
{
    txcap_stack_handle_t* stack = tsk_null;
    int ret;

    stack = txcap_stack_create(test_stack_callback, XUI, PASSWORD, XCAP_ROOT,
#if defined(ANDROID)
                               TXCAP_STACK_SET_OPTION(TXCAP_STACK_OPTION_LOCAL_IP, "10.0.2.15"),
#endif

                               // stack-level options
                               TXCAP_STACK_SET_OPTION(TXCAP_STACK_OPTION_TIMEOUT, "6000"),

                               // stack-level headers
                               TXCAP_STACK_SET_HEADER("Connection", "Keep-Alive"),
                               TXCAP_STACK_SET_HEADER("User-Agent", "XDM-client/OMA1.1"),
                               TXCAP_STACK_SET_HEADER("X-3GPP-Intended-Identity", XUI),

                               TXCAP_STACK_SET_NULL());

    if((ret = txcap_stack_start(stack))) {
        goto bail;
    }

    /* add user auids */
    txcap_stack_set(stack,
                    // add new AUIDs
                    TXCAP_STACK_SET_AUID("xcap-caps2", "application/xcap-caps2+xml", "urn:ietf:params:xml:ns:xcap-caps2", "index2", tsk_true),
                    TXCAP_STACK_SET_AUID("resource-lists2", "application/resource-lists+xml2", "urn:ietf:params:xml:ns:resource-lists2", "index2", tsk_false),

                    TXCAP_STACK_SET_NULL()); /* mandatory */

    //== get xcap-caps document
    ret = txcap_action_fetch_document(stack,
                                      // action-level options
                                      TXCAP_ACTION_SET_OPTION(TXCAP_ACTION_OPTION_TIMEOUT, "6000"),
                                      // action-level headers
                                      TXCAP_ACTION_SET_HEADER("Pragma", "No-Cache"),
                                      // selector
                                      TXCAP_ACTION_SET_SELECTOR("xcap-caps",
                                              TXCAP_SELECTOR_NODE_SET_NULL()),
                                      // ends parameters
                                      TXCAP_ACTION_SET_NULL()
                                     );

    getchar();
    /*
    	//== get resource-lists document
    	ret = txcap_action_fetch_document(stack,
    		// action-level options
    		TXCAP_ACTION_SET_OPTION(TXCAP_ACTION_OPTION_TIMEOUT, "6000"),
    		//action-level headers
    		TXCAP_ACTION_SET_HEADER("Pragma", "No-Cache"),
    		// selector
    		TXCAP_ACTION_SET_SELECTOR("resource-lists",
    			TXCAP_SELECTOR_NODE_SET_NULL()),
    		// ends parameters
    		TXCAP_ACTION_SET_NULL()
    		);

    	getchar();

    	//== get 'rcs' list
    	ret = txcap_action_fetch_element(stack,
    		// action-level options
    		TXCAP_ACTION_SET_OPTION(TXCAP_ACTION_OPTION_TIMEOUT, "6000"),
    		// action-level headers
    		TXCAP_ACTION_SET_HEADER("Pragma", "No-Cache"),
    		// selector
    		TXCAP_ACTION_SET_SELECTOR("resource-lists",
    			TXCAP_SELECTOR_NODE_SET_ATTRIBUTE("list", "name", "rcs"),
    			TXCAP_SELECTOR_NODE_SET_NULL()),
    		// ends parameters
    		TXCAP_ACTION_SET_NULL()
    		);

    	getchar();

    	//== Retrieve the 2nd list
    	ret = txcap_action_fetch_element(stack,
    		// action-level options
    		TXCAP_ACTION_SET_OPTION(TXCAP_ACTION_OPTION_TIMEOUT, "6000"),
    		// headers
    		TXCAP_ACTION_SET_HEADER("Pragma", "No-Cache"),
    		// action-level selector
    		TXCAP_ACTION_SET_SELECTOR("resource-lists",
    			TXCAP_SELECTOR_NODE_SET_POS("list", 2),
    			TXCAP_SELECTOR_NODE_SET_NULL()),
    		// ends parameters
    		TXCAP_ACTION_SET_NULL()
    		);

    	getchar();

    	//== Retrieve the 4th list (wildcard)
    	ret = txcap_action_fetch_element(stack,
    		// action-level options
    		TXCAP_ACTION_SET_OPTION(TXCAP_ACTION_OPTION_TIMEOUT, "6000"),
    		// headers
    		TXCAP_ACTION_SET_HEADER("Pragma", "No-Cache"),
    		// action-level selector
    		TXCAP_ACTION_SET_SELECTOR("resource-lists",
    			TXCAP_SELECTOR_NODE_SET_POS("*", 4),
    			TXCAP_SELECTOR_NODE_SET_NULL()),
    		// ends parameters
    		TXCAP_ACTION_SET_NULL()
    		);

    	getchar();

    	//== Retrieve the entry with: uri=XUI and list='rcs'
    	ret = txcap_action_fetch_element(stack,
    		// action-level options
    		TXCAP_ACTION_SET_OPTION(TXCAP_ACTION_OPTION_TIMEOUT, "6000"),
    		// headers
    		TXCAP_ACTION_SET_HEADER("Pragma", "No-Cache"),
    		// action-level selector
    		TXCAP_ACTION_SET_SELECTOR("resource-lists",
    			TXCAP_SELECTOR_NODE_SET_ATTRIBUTE("list", "name", "rcs"),
    			TXCAP_SELECTOR_NODE_SET_ATTRIBUTE("entry", "uri", XUI),
    			TXCAP_SELECTOR_NODE_SET_NULL()),
    		// ends parameters
    		TXCAP_ACTION_SET_NULL()
    		);

    	getchar();

    	//== Retrieve XUI's display-name ==> 404
    	ret = txcap_action_fetch_attribute(stack,
    	// action-level options
    	TXCAP_ACTION_SET_OPTION(TXCAP_ACTION_OPTION_TIMEOUT, "6000"),
    	// headers
    	TXCAP_ACTION_SET_HEADER("Pragma", "No-Cache"),
    	// action-level selector
    	TXCAP_ACTION_SET_SELECTOR("resource-lists",
    		TXCAP_SELECTOR_NODE_SET_ATTRIBUTE("list", "name", "rcs"),
    		TXCAP_SELECTOR_NODE_SET_ATTRIBUTE("entry", "uri", XUI),
    		TXCAP_SELECTOR_NODE_SET_NAME("display-name"),
    		TXCAP_SELECTOR_NODE_SET_NULL()),
    	// ends parameters
    	TXCAP_ACTION_SET_NULL()
    	);

    	getchar();

    	//== Retrieve the display-name of the fisrt entry from the 'rcs' list ==> 404
    	ret = txcap_action_fetch_attribute(stack,
    		// action-level options
    		TXCAP_ACTION_SET_OPTION(TXCAP_ACTION_OPTION_TIMEOUT, "6000"),
    		// headers
    		TXCAP_ACTION_SET_HEADER("Pragma", "No-Cache"),
    		// action-level selector
    		TXCAP_ACTION_SET_SELECTOR("resource-lists",
    			TXCAP_SELECTOR_NODE_SET_ATTRIBUTE("list", "name", "rcs"),
    			TXCAP_SELECTOR_NODE_SET_POS("entry", 1),
    			TXCAP_SELECTOR_NODE_SET_NAME("display-name"),
    			TXCAP_SELECTOR_NODE_SET_NULL()),
    		// ends parameters
    		TXCAP_ACTION_SET_NULL()
    		);

    		getchar();

    	//== Retrieve the entry with: uri='XUI' from position 1 from 'rcs' list
    	ret = txcap_action_fetch_element(stack,
    	// action-level options
    	TXCAP_ACTION_SET_OPTION(TXCAP_ACTION_OPTION_TIMEOUT, "6000"),
    	// headers
    	TXCAP_ACTION_SET_HEADER("Pragma", "No-Cache"),
    	// action-level selector
    	TXCAP_ACTION_SET_SELECTOR("resource-lists",
    		TXCAP_SELECTOR_NODE_SET_ATTRIBUTE("list", "name", "rcs"),
    		TXCAP_SELECTOR_NODE_SET_POS_ATTRIBUTE("entry", 1, "uri", XUI),
    		TXCAP_SELECTOR_NODE_SET_NULL()),
    	// ends parameters
    	TXCAP_ACTION_SET_NULL()
    	);

    	getchar();

    	//== Add newlist
    	ret = txcap_action_create_element(stack,
    		// selector
    		TXCAP_ACTION_SET_SELECTOR("resource-lists",
    			TXCAP_SELECTOR_NODE_SET_ATTRIBUTE("list", "name", "newlist"),
    			TXCAP_SELECTOR_NODE_SET_NULL()),
    		// payload
    		TXCAP_ACTION_SET_PAYLOAD(PAYLOAD_LIST, strlen(PAYLOAD_LIST)),
    		// ends parameters
    		TXCAP_ACTION_SET_NULL()
    		);
    	getchar();

    	//== Add entry
    	ret = txcap_action_create_element(stack,
    		// selector
    		TXCAP_ACTION_SET_SELECTOR("resource-lists",
    			TXCAP_SELECTOR_NODE_SET_ATTRIBUTE("list", "name", "newlist"),
    			TXCAP_SELECTOR_NODE_SET_ATTRIBUTE("entry", "uri", XUI),
    			TXCAP_SELECTOR_NODE_SET_NULL()),
    		// payload
    		TXCAP_ACTION_SET_PAYLOAD(PAYLOAD_ENTRY, strlen(PAYLOAD_ENTRY)),
    		// ends parameters
    		TXCAP_ACTION_SET_NULL()
    		);
    	getchar();

    	//== Add entry using dummy URI
    	ret = txcap_action_create_element(stack,
    		// dummy uri
    		TXCAP_ACTION_SET_REQUEST_URI("http://doubango.org/services/dummy/"XUI),
    		// payload
    		TXCAP_ACTION_SET_PAYLOAD(PAYLOAD_ENTRY, strlen(PAYLOAD_ENTRY)),
    		// ends parameters
    		TXCAP_ACTION_SET_NULL()
    		);
    	getchar();

    	//== Delete entry
    	ret = txcap_action_delete_element(stack,
    		// selector
    		TXCAP_ACTION_SET_SELECTOR("resource-lists",
    			TXCAP_SELECTOR_NODE_SET_ATTRIBUTE("list", "name", "newlist"),
    			TXCAP_SELECTOR_NODE_SET_ATTRIBUTE("entry", "uri", XUI),
    			TXCAP_SELECTOR_NODE_SET_NULL()),
    		// ends parameters
    		TXCAP_ACTION_SET_NULL()
    		);
    	getchar();




    	ret = txcap_action_fetch_document(stack,
    		// action-level options
    		TXCAP_ACTION_SET_OPTION(TXCAP_ACTION_OPTION_TIMEOUT, "6000"),
    		//action-level headers
    		TXCAP_ACTION_SET_HEADER("Pragma", "No-Cache"),
    		// selector
    		TXCAP_ACTION_SET_SELECTOR("org.openmobilealliance.pres-content",
    			TXCAP_SELECTOR_NODE_SET_NULL()),
    		// ends parameters
    		TXCAP_ACTION_SET_NULL()
    		);
    	*/
bail:
    getchar();


    TSK_OBJECT_SAFE_FREE(stack);
}

#endif /* _TEST_STACK_H */
