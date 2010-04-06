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
#ifndef TEST_SELECTOR_H
#define TEST_SELECTOR_H

#include "txc_api.h" 

/* test xcap selector */
void test_selector()
 {
	txc_context_t* context;
	char *url = 0;
	
	/* create our XCAP context */
	TXC_CONTEXT_CREATE(context);

	printf("---\nTEST XCAP SELECTOR\n---\n");

	/* Initialize the context */
	TEST_XXXX_CONTEXT_INIT(context);

	//txc_auid_update(context, ietf_resource_lists, "rlist.xml");
	
	/* IETF server capabilities */
	url = TXC_DOC_GET_SEL(context, ietf_xcap_caps);
	printf("Server capabilities url: \n%s\n***\n", url);
	TSK_FREE(url);

	/* IETF resource-list */
	url = TXC_DOC_GET_SEL(context, ietf_resource_lists);
	printf("IETF resource-list: \n%s\n***\n", url);
	TSK_FREE(url);

	/* IETF resource-list (2): semi-custom docment name*/
	url = TXC_DOC_GET_SEL2(context, ietf_resource_lists, "usr-doc");
	printf("IETF resource-list(2): \n%s\n***\n", url);
	TSK_FREE(url);

	/* IETF resource-list (3): full-custom docment name*/
	url = txc_doc_get_cust_sel("http://usr-root", "usr-auid", "usr-xui", "usr-doc");
	printf("IETF resource-list(3): \n%s\n***\n", url);
	TSK_FREE(url);
	
	/* Retrieve list named 'Default' in the resource list */
	url = txc_node_get_sel(context, ietf_resource_lists,
		TXC_NODE_SELECT_BY_ATTR("list", "name", "Default"), //==> /resource-lists/list[@name="Default"]
		TXC_NODE_SELECT_END()
		);
	printf("Retrieve list named 'Default' in the resource list: \n%s\n***\n", url);
	TSK_FREE(url);

	/* Retrieve the second list in the resource list */
	url = txc_node_get_sel(context, ietf_resource_lists,
		TXC_NODE_SELECT_BY_POS("list", 2), //==> /resource-lists/list[2]
		TXC_NODE_SELECT_END()
		);
	printf("Retrieve the second list in the resource list: \n%s\n***\n", url);
	TSK_FREE(url);
	
	/* Retrieve the fourth list in the resource list(using wildcard!!!)*/
	url = txc_node_get_sel(context, ietf_resource_lists,
		TXC_NODE_SELECT_BY_POS("*", 4), //==> /resource-lists/*[4]
		TXC_NODE_SELECT_END()
		);
	printf("Retrieve the fourth list in the resource list(using wildcard!!!): \n%s\n***\n", url);
	TSK_FREE(url);

	/* Retrieve an entry with uri equals to 'sip:doubango@example.com' into a list named 'Ennemies' in the resource list */
	url = txc_node_get_sel(context, ietf_resource_lists,
		TXC_NODE_SELECT_BY_ATTR("list", "name", "Ennemies"),//==> /resource-lists/list[@name="Ennemies"]
		TXC_NODE_SELECT_BY_ATTR("entry", "uri", "sip:doubango@example.com"),//==> /resource-lists/list[@name="Ennemies"]/entry[@uri="sip:doubango@example.com"]
		TXC_NODE_SELECT_END()
		);
	printf("Retrieve an entry with an uri equals to 'sip:doubango@example.com' into a list named 'Ennemies': \n%s\n***\n", url);
	TSK_FREE(url);

	/* Retrieve doubango's display-name (we suppose doubango is a friend) */
	url = txc_node_get_sel(context, ietf_resource_lists,
		TXC_NODE_SELECT_BY_ATTR("list", "name", "Friends"),//==> /resource-lists/list[@name="Friends"]
		TXC_NODE_SELECT_BY_ATTR("entry", "uri", "sip:doubango@example.com"),//==> /resource-lists/list[@name="Friends"]/entry[@uri="sip:doubango@example.com"]
		TXC_NODE_SELECT_BY_NAME("display-name"),//==> /resource-lists/list[@name="Friends"]/entry[@uri="sip:doubango@example.com"]/display-name
		TXC_NODE_SELECT_END()
		);
	printf("Retrieve doubango's display-name (we suppose doubango is a friend): \n%s\n***\n", url);
	TSK_FREE(url);

	/* Retrieve the display-name of the first entry in the list of the Ennemies*/
	url = txc_node_get_sel(context, ietf_resource_lists,
		TXC_NODE_SELECT_BY_ATTR("list", "name", "Friends"),//==> /resource-lists/list[@name="Friends"]
		TXC_NODE_SELECT_BY_POS("entry", 1),//==> /resource-lists/list[@name="Friends"]/entry[1]
		TXC_NODE_SELECT_BY_NAME("display-name"),//==> /resource-lists/list[@name="Friends"]/entry[1]/display-name
		TXC_NODE_SELECT_END()
		);
	printf("Retrieve the display-name of the first entry in the list of the Ennemies: \n%s\n***\n", url);
	TSK_FREE(url);
	
	/* Retrieve an entry with uri equals to 'sip:doubango@example.com' at position 23 into a list named 'Ennemies' in the resource list */
	url = txc_node_get_sel(context, ietf_resource_lists,
		TXC_NODE_SELECT_BY_ATTR("list", "name", "Ennemies"),//==> /resource-lists/list[@name="Ennemies"]
		TXC_NODE_SELECT_BY_POS_ATTR("entry", 23, "uri", "sip:doubango@example.com"),//==> /resource-lists/list[@name="Ennemies"]/entry[23][@uri="sip:doubango@example.com"]
		TXC_NODE_SELECT_END()
		);
	printf("Retrieve an entry with uri equals to 'sip:doubango@example.com' at position 23 into a list named 'Ennemies' in the resource list: \n%s\n***\n", url);
	TSK_FREE(url);
	
	/* Namespace test */
	/*request /foo/a:bar/b:baz?xmlns(a=urn:test:namespace1-uri)xmlns(b=urn:test:namespace2-uri)*/
	url = txc_node_get_sel(context, ietf_resource_lists,
		TXC_NODE_SELECT_BY_NAME("foo"),//==> /foo
		TXC_NODE_SELECT_BY_NAME("a:bar"),//==> /foo/a:bar
		TXC_NODE_SELECT_BY_NAME("b:baz"),//==> /foo/a:bar/b:baz
		TXC_NODE_ADD_NAMESPACE("a", "urn:test:namespace1-uri"),//==>/foo/a:bar/b:baz?xmlns(a=urn:test:namespace1-uri)
		TXC_NODE_ADD_NAMESPACE("b", "urn:test:namespace2-uri"),//==>/foo/a:bar/b:baz?xmlns(a=urn:test:namespace1-uri)xmlns(b=urn:test:namespace2-uri)
		TXC_NODE_SELECT_END()
		);
	printf("Namespace test: \n%s\n***\n", url);
	TSK_FREE(url);
	
	/* free allocated memory*/
	TXC_CONTEXT_SAFE_FREE(context);
}
#endif /* TEST_SELECTOR_H */
