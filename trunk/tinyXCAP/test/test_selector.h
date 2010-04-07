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
	urlstring = txcap_selector_get_document(stack, "xcap-caps");
	TSK_DEBUG_INFO("Document('xcap-caps') = %s\n", urlstring);
	TSK_FREE(urlstring);

	/* My custom xcap-caps */
	urlstring = txcap_selector_get_document(stack, "xcap-caps2");
	TSK_DEBUG_INFO("Document('xcap-caps2') = %s\n", urlstring);
	TSK_FREE(urlstring);

	/* IETF resource-lists */
	urlstring = txcap_selector_get_document(stack, "resource-lists");
	TSK_DEBUG_INFO("Document('resource-lists') = %s\n", urlstring);
	TSK_FREE(urlstring);

	/* My custom resource-lists */
	urlstring = txcap_selector_get_document(stack, "resource-lists2");
	TSK_DEBUG_INFO("Document('resource-lists2') = %s\n", urlstring);
	TSK_FREE(urlstring);

	/* Full custom resource-lists */
	urlstring = txcap_selector_get_document_2(XCAP_ROOT, "resource-lists3", XUI, "index3");
	TSK_DEBUG_INFO("Document('resource-lists2') = %s\n", urlstring);
	TSK_FREE(urlstring);


	TSK_OBJECT_SAFE_FREE(stack);
}

#endif /* _TEST_SELECTOR_H */
