/****************************************************************************
			 _             _                             
			| |           | |                            
		  _ | | ___  _   _| | _   ____ ____   ____  ___  
		 / || |/ _ \| | | | || \ / _  |  _ \ / _  |/ _ \ 
		( (_| | |_| | |_| | |_) | ( | | | | ( ( | | |_| |
		 \____|\___/ \____|____/ \_||_|_| |_|\_|| |\___/ 
											(_____|   
	
	Copyright (C) 2009 xxxyyyzzz <imsframework(at)gmail.com>

	This file is part of Open Source Doubango IMS Client Framework project.

    DOUBANGO is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
	
    DOUBANGO is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.
	
    You should have received a copy of the GNU General Public License
    along with DOUBANGO.
****************************************************************************/
#ifndef TEST_SELECTOR_H
#define TEST_SELECTOR_H

#include "xdm_api.h" 

/* test xcap selector */
void test_selector()
{
	xdm_context_t* context;
	xdm_request_t* request;

	/* create our xdm context */
	XDM_CONTEXT_CREATE(context);

	printf("---\nTEST XCAP SELECTOR\n---\n");

	context->user_agent = xdm_strdup("XDM-client/OMA1.1");
	context->xdm_root = xdm_strdup("http://xcap.example.com:8080/services");
	context->password = xdm_strdup("mysecret");
	context->xui = xdm_strdup("sip:doubango@example.com");
	
	/* IETF server capabilities */
	request = xdm_doc_get_sel(context, ietf_xcap_caps);
	printf("Server capabilities url: \n%s\n***\n", request->url);
	XDM_REQUEST_SAFE_FREE(request);
	
	/* IETF resource-list */
	request = xdm_doc_get_sel(context, ietf_resource_lists);
	printf("IETF resource-list: \n%s\n***\n", request->url);
	XDM_REQUEST_SAFE_FREE(request);
	
	/* Retrieve list named 'Default' in the resource list */
	request = xdm_node_get_sel(context, ietf_resource_lists,
		XDM_NODE_SELECT_BY_ATTR("list", "name", "Default"), //==> /resource-lists/list[@name="Default"]
		XDM_NODE_SELECT_END()
		);
	printf("Retrieve list named 'Default' in the resource list: \n%s\n***\n", request->url);
	XDM_REQUEST_SAFE_FREE(request);

	/* Retrieve the second list in the resource list */
	request = xdm_node_get_sel(context, ietf_resource_lists,
		XDM_NODE_SELECT_BY_POS("list", 2), //==> /resource-lists/list[2]
		XDM_NODE_SELECT_END()
		);
	printf("Retrieve the second list in the resource list: \n%s\n***\n", request->url);
	XDM_REQUEST_SAFE_FREE(request);
	
	/* Retrieve the fourth list in the resource list(using wildcard!!!)*/
	request = xdm_node_get_sel(context, ietf_resource_lists,
		XDM_NODE_SELECT_BY_POS("*", 4), //==> /resource-lists/*[4]
		XDM_NODE_SELECT_END()
		);
	printf("Retrieve the fourth list in the resource list(using wildcard!!!): \n%s\n***\n", request->url);
	XDM_REQUEST_SAFE_FREE(request);

	/* Retrieve an entry with uri equals to 'sip:doubango@example.com' into a list named 'Ennemies' in the resource list */
	request = xdm_node_get_sel(context, ietf_resource_lists,
		XDM_NODE_SELECT_BY_ATTR("list", "name", "Ennemies"),//==> /resource-lists/list[@name="Ennemies"]
		XDM_NODE_SELECT_BY_ATTR("entry", "uri", "sip:doubango@example.com"),//==> /resource-lists/list[@name="Ennemies"]/entry[@uri="sip:doubango@example.com"]
		XDM_NODE_SELECT_END()
		);
	printf("Retrieve an entry with an uri equals to 'sip:doubango@example.com' into a list named 'Ennemies': \n%s\n***\n", request->url);
	XDM_REQUEST_SAFE_FREE(request);

	/* Retrieve doubango's display-name (we suppose doubango is a friend) */
	request = xdm_node_get_sel(context, ietf_resource_lists,
		XDM_NODE_SELECT_BY_ATTR("list", "name", "Friends"),//==> /resource-lists/list[@name="Friends"]
		XDM_NODE_SELECT_BY_ATTR("entry", "uri", "sip:doubango@example.com"),//==> /resource-lists/list[@name="Friends"]/entry[@uri="sip:doubango@example.com"]
		XDM_NODE_SELECT_BY_NAME("display-name"),//==> /resource-lists/list[@name="Friends"]/entry[@uri="sip:doubango@example.com"]/display-name
		XDM_NODE_SELECT_END()
		);
	printf("Retrieve doubango's display-name (we suppose doubango is a friend): \n%s\n***\n", request->url);
	XDM_REQUEST_SAFE_FREE(request);

	/* Retrieve the display-name of the first entry in the list of the Ennemies*/
	request = xdm_node_get_sel(context, ietf_resource_lists,
		XDM_NODE_SELECT_BY_ATTR("list", "name", "Friends"),//==> /resource-lists/list[@name="Friends"]
		XDM_NODE_SELECT_BY_POS("entry", 1),//==> /resource-lists/list[@name="Friends"]/entry[1]
		XDM_NODE_SELECT_BY_NAME("display-name"),//==> /resource-lists/list[@name="Friends"]/entry[1]/display-name
		XDM_NODE_SELECT_END()
		);
	printf("Retrieve the display-name of the first entry in the list of the Ennemies: \n%s\n***\n", request->url);
	XDM_REQUEST_SAFE_FREE(request);
	
	/* Retrieve an entry with uri equals to 'sip:doubango@example.com' at position 23 into a list named 'Ennemies' in the resource list */
	request = xdm_node_get_sel(context, ietf_resource_lists,
		XDM_NODE_SELECT_BY_ATTR("list", "name", "Ennemies"),//==> /resource-lists/list[@name="Ennemies"]
		XDM_NODE_SELECT_BY_POS_ATTR("entry", 23, "uri", "sip:doubango@example.com"),//==> /resource-lists/list[@name="Ennemies"]/entry[23][@uri="sip:doubango@example.com"]
		XDM_NODE_SELECT_END()
		);
	printf("Retrieve an entry with uri equals to 'sip:doubango@example.com' at position 23 into a list named 'Ennemies' in the resource list: \n%s\n***\n", request->url);
	XDM_REQUEST_SAFE_FREE(request);
	
	/* Namespace test */
	/* /foo/a:bar/b:baz?xmlns(a=urn:test:namespace1-uri)xmlns(b=urn:test:namespace2-uri)*/
	request = xdm_node_get_sel(context, ietf_resource_lists,
		XDM_NODE_SELECT_BY_NAME("foo"),//==> /foo
		XDM_NODE_SELECT_BY_NAME("a:bar"),//==> /foo/a:bar
		XDM_NODE_SELECT_BY_NAME("b:baz"),//==> /foo/a:bar/b:baz
		XDM_NODE_ADD_NAMESPACE("a", "urn:test:namespace1-uri"),//==>/foo/a:bar/b:baz?xmlns(a=urn:test:namespace1-uri)
		XDM_NODE_ADD_NAMESPACE("b", "urn:test:namespace2-uri"),//==>/foo/a:bar/b:baz?xmlns(a=urn:test:namespace1-uri)xmlns(b=urn:test:namespace2-uri)
		XDM_NODE_SELECT_END()
		);
	printf("Namespace test: \n%s\n***\n", request->url);
	XDM_REQUEST_SAFE_FREE(request);

	/* free allocated memory*/
	XDM_REQUEST_SAFE_FREE(request);
	XDM_CONTEXT_SAFE_FREE(context);
}
#endif /* TEST_SELECTOR_H */