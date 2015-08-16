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
#ifndef _TEST_SELECTOR_H
#define _TEST_SELECTOR_H

void test_selector()
{
	txcap_stack_handle_t* stack = tsk_null;
	char* urlstring;

	stack = txcap_stack_create(tsk_null, XUI, PASSWORD, XCAP_ROOT,
		TXCAP_STACK_SET_NULL());

	/* add user auids */
	txcap_stack_set(stack,
		TXCAP_STACK_SET_AUID("xcap-caps2", "application/xcap-caps2+xml", "urn:ietf:params:xml:ns:xcap-caps2", "index2", tsk_true),
		TXCAP_STACK_SET_AUID("resource-lists2", "application/resource-lists+xml2", "urn:ietf:params:xml:ns:resource-lists2", "index2", tsk_false),
		
		TXCAP_STACK_SET_NULL()); /* mandatory */

	/* IETF server capabilities */
	urlstring = txcap_selector_get_url(stack, "xcap-caps",
		TXCAP_SELECTOR_NODE_SET_NULL());
	TSK_DEBUG_INFO("Document('xcap-caps') = %s\n", urlstring);
	TSK_FREE(urlstring);

	/* My custom xcap-caps */
	urlstring = txcap_selector_get_url(stack, "xcap-caps2",
		TXCAP_SELECTOR_NODE_SET_NULL());
	TSK_DEBUG_INFO("Document('xcap-caps2') = %s\n", urlstring);
	TSK_FREE(urlstring);

	/* IETF resource-lists */
	urlstring = txcap_selector_get_url(stack, "resource-lists",
		TXCAP_SELECTOR_NODE_SET_NULL());
	TSK_DEBUG_INFO("Document('resource-lists') = %s\n", urlstring);
	TSK_FREE(urlstring);

	/* My custom resource-lists */
	urlstring = txcap_selector_get_url(stack, "resource-lists2",
		TXCAP_SELECTOR_NODE_SET_NULL());
	TSK_DEBUG_INFO("Document('resource-lists2') = %s\n", urlstring);
	TSK_FREE(urlstring);

	/* Full custom resource-lists */
	urlstring = txcap_selector_get_document_2(XCAP_ROOT, "resource-lists3", XUI, "index3");
	TSK_DEBUG_INFO("Document('resource-lists2') = %s\n", urlstring);
	TSK_FREE(urlstring);

	/* Retrieve 'rcs' list
	* Expected node: /resource-lists/list[@name="rcs"]
	*/
	urlstring = txcap_selector_get_url(stack, "resource-lists",
		TXCAP_SELECTOR_NODE_SET_ATTRIBUTE("list", "name", "rcs"),
		TXCAP_SELECTOR_NODE_SET_NULL());
	TSK_DEBUG_INFO("'rcs' list = %s\n", urlstring);
	TSK_FREE(urlstring);
	
	/* Retrieve the 2nd list
	* Expected node: /resource-lists/list[2]
	*/
	urlstring = txcap_selector_get_url(stack, "resource-lists",
		TXCAP_SELECTOR_NODE_SET_POS("list", 2),
		TXCAP_SELECTOR_NODE_SET_NULL());
	TSK_DEBUG_INFO("2nd list = %s\n", urlstring);
	TSK_FREE(urlstring);

	/* Retrieve the 4th list (wildcard)
	* Expected node: /resource-lists/*[4]
	*/
	urlstring = txcap_selector_get_url(stack, "resource-lists",
		TXCAP_SELECTOR_NODE_SET_POS("*", 4),
		TXCAP_SELECTOR_NODE_SET_NULL());
	TSK_DEBUG_INFO("4th list (wildcard) = %s\n", urlstring);
	TSK_FREE(urlstring);

	/* Retrieve the entry with: uri='sip:bob@example.com' and list='rcs'
	* Expected node: /resource-lists/list[@name="rcs"]/entry[@uri="sip:bob@example.com"]
	*/
	urlstring = txcap_selector_get_url(stack, "resource-lists",
		TXCAP_SELECTOR_NODE_SET_ATTRIBUTE("list", "name", "rcs"),
		TXCAP_SELECTOR_NODE_SET_ATTRIBUTE("entry", "uri", XUI),
		TXCAP_SELECTOR_NODE_SET_NULL());
	TSK_DEBUG_INFO("By uri and list = %s\n", urlstring);
	TSK_FREE(urlstring);

	/* Retrieve bob's display-name 
	* Expected node: /resource-lists/list[@name="rcs"]/entry[@uri="sip:bob@example.com"]/display-name
	*/
	urlstring = txcap_selector_get_url(stack, "resource-lists",
		TXCAP_SELECTOR_NODE_SET_ATTRIBUTE("list", "name", "rcs"),
		TXCAP_SELECTOR_NODE_SET_ATTRIBUTE("entry", "uri", XUI),
		TXCAP_SELECTOR_NODE_SET_NAME("display-name"),
		TXCAP_SELECTOR_NODE_SET_NULL());
	TSK_DEBUG_INFO("bob's display-name = %s\n", urlstring);
	TSK_FREE(urlstring);

	/* Retrieve the display-name of the fisrt entry from the 'rcs' list
	* Expected node: /resource-lists/list[@name="rcs"]/entry[1]/display-name
	*/
	urlstring = txcap_selector_get_url(stack, "resource-lists",
		TXCAP_SELECTOR_NODE_SET_ATTRIBUTE("list", "name", "rcs"),
		TXCAP_SELECTOR_NODE_SET_POS("entry", 1),
		TXCAP_SELECTOR_NODE_SET_NAME("display-name"),
		TXCAP_SELECTOR_NODE_SET_NULL());
	TSK_DEBUG_INFO("display-name of 1st entry = %s\n", urlstring);
	TSK_FREE(urlstring);

	/* Retrieve the entry with: uri='sip:bob@example.com' from position 23 from 'rcs' list
	* Expected node: /resource-lists/list[@name="rcs"]/entry[23][@uri="sip:bob@example.com"]
	*/
	urlstring = txcap_selector_get_url(stack, "resource-lists",
		TXCAP_SELECTOR_NODE_SET_ATTRIBUTE("list", "name", "rcs"),
		TXCAP_SELECTOR_NODE_SET_POS_ATTRIBUTE("entry", 23, "uri", XUI),
		TXCAP_SELECTOR_NODE_SET_NULL());
	TSK_DEBUG_INFO("bob at pos 23 from 'rcs' = %s\n", urlstring);
	TSK_FREE(urlstring);

	/* Namespace test
	* Expected node: /foo/a:bar/b:baz?xmlns(a=urn:test:namespace1-uri)xmlns(b=urn:test:namespace2-uri)
	*/
	urlstring = txcap_selector_get_url(stack, "resource-lists",
		TXCAP_SELECTOR_NODE_SET_NAME("foo"),
		TXCAP_SELECTOR_NODE_SET_NAME("a:bar"),
		TXCAP_SELECTOR_NODE_SET_NAME("b:baz"),
		TXCAP_SELECTOR_NODE_SET_NAMESPACE("a", "urn:namespace1-uri"),
		TXCAP_SELECTOR_NODE_SET_NAMESPACE("b", "urn:namespace2-uri"),
		TXCAP_SELECTOR_NODE_SET_NULL());
	TSK_DEBUG_INFO("Namespace test = %s\n", urlstring);
	TSK_FREE(urlstring);


	TSK_OBJECT_SAFE_FREE(stack);
}

#endif /* _TEST_SELECTOR_H */
