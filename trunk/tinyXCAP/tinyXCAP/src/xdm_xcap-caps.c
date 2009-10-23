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
/*
	xcap-caps
		->namespaces
			->namespace-1
			-> ...
			->namespace-2
		->extensions
			->extension-1
			->...
			->extension-n
		->auids
			->auid-1
			->...
			->auid-n

*/

#include "xdm_xcap-caps.h"
#include "xdm_utils.h"

#define CAPS_RETURN_IF_INVALID(caps) if(!caps || !caps->docPtr || !caps->root) return 0;

//static const char* xdm_xcap_caps_ns = "urn:ietf:params:xml:ns:xcap-caps ";

/* get xxx(auids, namespaces or extensions) */
static xdm_list_t* xdm_xcap_caps_xxx(xmlNodePtr rootNode, const char* root, const char* name1, const char* name2)
{
	xdm_list_t *xxx = 0;
	xdm_list_item_t* item = 0;
	xmlNodePtr node = 0;
		
	XDM_LIST_CREATE(xxx);
	
	node = xdm_xml_select_node(rootNode, 
		XDM_XML_NODE_SELECT_BY_NAME(root),
		XDM_XML_NODE_SELECT_BY_NAME(name1),
		XDM_XML_NODE_SELECT_CONTENT(),
		XDM_XML_NODE_SELECT_END());
	
	while(node = xdm_xml_find_node(node, name2, nft_next))
	{
		if(node->children && node->children->type == XML_TEXT_NODE)
		{
			XDM_LIST_ITEM_CREATE(item);
			item->data = xdm_strdup((const char*)node->children->content);
			xdm_list_add_item(xxx, &item);
		}
	}
	
	return xxx;
}

/* create xcap-caps context */
/* ATTENTION: use 'xdm_xcap_caps_free' function to free the returned object*/
xcap_caps_t* xdm_xcap_caps_create(const char* buffer, size_t size)
{
	xmlNodePtr root = 0;
	xmlDocPtr docPtr = 0;
	xcap_caps_t* caps = (xcap_caps_t*)malloc(sizeof(xcap_caps_t));
	memset(caps, 0, sizeof(xcap_caps_t));

	docPtr = xmlParseMemory(buffer, (int)size);
	if(docPtr) root = docPtr->children;
	if(root)
	{
		caps->auids = xdm_xcap_caps_xxx(root, "xcap-caps", "auids", "auid");
		caps->extensions = xdm_xcap_caps_xxx(root, "xcap-caps", "extensions", "extension");
		caps->namespaces = xdm_xcap_caps_xxx(root, "xcap-caps", "namespaces", "namespace");
	}

	xmlFreeDoc(docPtr);

	return caps;
}

/* free data (xcap_caps_t) */
void xdm_xcap_caps_free(xcap_caps_t** caps)
{
	if(*caps)
	{
		XDM_LIST_SAFE_FREE((*caps)->auids);
		XDM_LIST_SAFE_FREE((*caps)->namespaces);
		XDM_LIST_SAFE_FREE((*caps)->extensions);

		free(*caps);
		(*caps)=0;
	}
}

#undef CAPS_RETURN_IF_INVALID

//xcap_caps_t* xdm_xcap_caps_create()
//{
//	int i;
//	xdm_xml_element_t *auids = 0, *namespaces = 0, *elt;
//	
//	xcap_caps_t* caps = (xcap_caps_t*)malloc(sizeof(xcap_caps_t));
//	memset(caps, 0, sizeof(xcap_caps_t));
//	XDM_LIST_CREATE(caps->root); 
//	
//
//	
//
//
//
//
//	//
//	// auids
//	//
//	xdm_xml_element_init_set(&auids, "auids", "", xt_none); /*auids root*/
//	for(i=0; i<4;i++)
//	{
//		switch(i)
//		{
//		case 0:
//			{	/* <auid>xcap-caps</auid> */
//				xdm_xml_element_init_set(&elt, "auid", "xcap-caps", xt_string);
//				xdm_xml_elements_add(auids->elements, &elt, xdm_xml_element_free);
//				break;
//			}
//
//		case 1:
//			{	/* <auid>resource-lists</auid> */
//				xdm_xml_element_init_set(&elt, "auid", "resource-lists", xt_string);
//				xdm_xml_elements_add(auids->elements, &elt, xdm_xml_element_free);
//				break;
//			}
//
//		case 2:
//			{	/* <auid>rls-services</auid> */
//				xdm_xml_element_init_set(&elt, "auid", "rls-services", xt_string);
//				xdm_xml_elements_add(auids->elements, &elt, xdm_xml_element_free);
//				break;
//			}
//
//		case 3:
//			{	/* <auid>org.openmobilealliance.xcap-directory</auid> */
//				xdm_xml_element_init_set(&elt, "auid", "org.openmobilealliance.xcap-directory", xt_string);
//				xdm_xml_elements_add(auids->elements, &elt, xdm_xml_element_free);
//				break;
//			}
//		}
//	}
//
//	//
//	// namespaces
//	//
//	xdm_xml_element_init_set(&namespaces, "namespaces", "", xt_none); /*namespaces root*/
//	for(i=0; i<5;i++)
//	{
//		switch(i)
//		{
//		case 0:
//			{	/* <namespace>urn:ietf:params:xml:ns:xcap-caps</namespace> */
//				xdm_xml_element_init_set(&elt, "namespace", "<namespace>urn:ietf:params:xml:ns:xcap-caps</namespace>", xt_string);
//				xdm_xml_elements_add(namespaces->elements, &elt, xdm_xml_element_free);
//				break;
//			}
//
//		case 1:
//			{	/* <namespace>urn:ietf:params:xml:ns:xcap-error</namespace> */
//				xdm_xml_element_init_set(&elt, "namespace", "<namespace>urn:ietf:params:xml:ns:xcap-error</namespace>", xt_string);
//				xdm_xml_elements_add(namespaces->elements, &elt, xdm_xml_element_free);
//				break;
//			}
//
//		case 2:
//			{	/* <namespace>urn:ietf:params:xml:ns:resource-lists</namespace> */
//				xdm_xml_element_init_set(&elt, "namespace", "<namespace>urn:ietf:params:xml:ns:resource-lists</namespace>", xt_string);
//				xdm_xml_elements_add(namespaces->elements, &elt, xdm_xml_element_free);
//				break;
//			}
//
//		case 3:
//			{	/* <namespace>urn:ietf:params:xml:ns:rls-services</namespace> */
//				xdm_xml_element_init_set(&elt, "namespace", "<namespace>urn:ietf:params:xml:ns:rls-services</namespace>", xt_string);
//				xdm_xml_elements_add(namespaces->elements, &elt, xdm_xml_element_free);
//				break;
//			}
//
//		case 4:
//			{	/* <namespace>urn:oma:params:ns:resource-list:oma-uriusage</namespace> */
//				xdm_xml_element_init_set(&elt, "namespace", "urn:oma:params:ns:resource-list:oma-uriusage</namespace>", xt_string);
//				xdm_xml_elements_add(namespaces->elements, &elt, xdm_xml_element_free);
//				break;
//			}
//		}
//	}
//	
//	/* add auids */
//	xdm_list_add_data(caps->root, ((void**) &auids), xdm_xml_element_free);
//
//	/* add namespaces */
//	xdm_list_add_data(caps->root, ((void**) &namespaces), xdm_xml_element_free);
//
//	return caps;
//}
