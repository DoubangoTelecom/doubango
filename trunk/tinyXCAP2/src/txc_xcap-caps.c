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

/**@file txc_xcap-caps.c
 * @brief RFC 4825 subclause <a href="http://tools.ietf.org/html/rfc4825#section-12">12. XCAP Server Capabilities</a>
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */

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

#include "txc_xcap-caps.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_macros.h"

#include <string.h>

/**@defgroup txc_xcap_caps_group XCAP Server Capabilities
*/


/**@page txc_xcap_caps_page XCAP Server Capabilities Tutorial (xcap-caps)
* @par Application Unique ID (AUID)
* - '<span style="text-decoration:underline;">xcap-caps</span>' as per rfc 4825 subclause 12.1
* @par Default Document Namespace
* - '<span style="text-decoration:underline;">urn:ietf:params:xml:ns:xcap-caps</span>' as per rfc 4825 subclause 12.3
* @par MIME Type
* - '<span style="text-decoration:underline;">application/xcap-caps+xml</span>' as per rfc 4825 subclause 15.2.5
* @par Default document name
* - '<span style="text-decoration:underline;">index</span>' as per rfc 4825 subclause 5.5
*
* <H2>=== Create/deserialize an xcap-caps document received from the XDMS ===</H2>
*
* @code
#include "txc_api.h" 

*xcap_caps_t* xcap_caps = 0;
tsk_list_item_t* item = 0;

// deserialize/create the context from the XML buffer
xcap_caps = txc_xcap_caps_create(buffer, size);

// dump namespaces
tsk_list_foreach(item, xcap_caps->namespaces)
	printf("namespace: %s\n", ((char*)item->data));

// dump extensions
tsk_list_foreach(item, xcap_caps->extensions)
	printf("extension: %s\n", ((char*)item->data));

// dump auids
tsk_list_foreach(item, xcap_caps->auids)
	printf("auid: %s\n", ((char*)item->data));

// free the context
txc_xcap_caps_free(&xcap_caps);
* @endcode
*/

#define CAPS_RETURN_IF_INVALID(caps) if(!caps || !caps->docPtr || !caps->root) return 0;

/**@ingroup txc_xcap_caps_group
* Internal function to deserialize into list an element of xcap-caps document (auids, extensions or namespaces) 
* @param rootNode The XML document root node
* @param root The XML root node from which to start
* @param name1 The root element name (e.g. 'auids')
* @param name2 The name of the element childs (e.g. 'auid')
* @retval The list of the element childs as strings. You should never call this method. 
* Instead you should call @ref txc_xcap_caps_create to deserialize the whole document. 
*/
static tsk_list_t* txc_xcap_caps_xxx(xmlNodePtr rootNode, const char* root, const char* name1, const char* name2)
{
	tsk_list_t *xxx = 0;
	tsk_list_item_t* item = 0;
	xmlNodePtr node = 0;
		
	xxx = TSK_LIST_CREATE();
	
	node = tsk_xml_select_node(rootNode, 
		TSK_XML_NODE_SELECT_BY_NAME(root),
		TSK_XML_NODE_SELECT_BY_NAME(name1),
		TSK_XML_NODE_SELECT_CONTENT(),
		TSK_XML_NODE_SELECT_END());
	
	while(node = tsk_xml_find_node(node, name2, nft_next))
	{
		if(node->children && node->children->type == XML_TEXT_NODE)
		{
			item = TSK_LIST_ITEM_CREATE();
			item->data = TSK_STRING_CREATE((const char*)node->children->content);
			tsk_list_add_item(xxx, &item);
		}
	}
	
	return xxx;
}

/**@ingroup txc_xcap_caps_group
* Create an xcap-caps context.
* @param buffer The XML buffer from which to create (or deserialize) the xcap-caps context
* @param size The size of the XML @a buffer
* @retval The deserialized xcap-caps document. You must call @ref txc_xcap_caps_free to free the returned context.
*/
xcap_caps_t* txc_xcap_caps_create(const char* buffer, size_t size)
{
	xmlNodePtr root = 0;
	xmlDocPtr docPtr = 0;
	xcap_caps_t* caps = (xcap_caps_t*)tsk_calloc(1, sizeof(xcap_caps_t));
	
	docPtr = xmlParseMemory(buffer, (int)size);
	if(docPtr) root = docPtr->children;
	if(root)
	{
		caps->auids = txc_xcap_caps_xxx(root, "xcap-caps", "auids", "auid");
		caps->extensions = txc_xcap_caps_xxx(root, "xcap-caps", "extensions", "extension");
		caps->namespaces = txc_xcap_caps_xxx(root, "xcap-caps", "namespaces", "namespace");
	}

	xmlFreeDoc(docPtr);

	return caps;
}

/**@ingroup txc_xcap_caps_group
* Free an xcap-caps context previously deserialize/created using @ref txc_xcap_caps_create
* @param caps The xcap-caps context to free
*/
void txc_xcap_caps_free(xcap_caps_t** caps)
{
	if(*caps)
	{
		TSK_LIST_SAFE_FREE((*caps)->auids);
		TSK_LIST_SAFE_FREE((*caps)->namespaces);
		TSK_LIST_SAFE_FREE((*caps)->extensions);

		tsk_free(caps);
	}
}

#undef CAPS_RETURN_IF_INVALID

//xcap_caps_t* txc_xcap_caps_create()
//{
//	int i;
//	txc_xml_element_t *auids = 0, *namespaces = 0, *elt;
//	
//	xcap_caps_t* caps = (xcap_caps_t*)malloc(sizeof(xcap_caps_t));
//	memset(caps, 0, sizeof(xcap_caps_t));
//	TSK_LIST_CREATE(caps->root); 
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
//	txc_xml_element_init_set(&auids, "auids", "", xt_none); /*auids root*/
//	for(i=0; i<4;i++)
//	{
//		switch(i)
//		{
//		case 0:
//			{	/* <auid>xcap-caps</auid> */
//				txc_xml_element_init_set(&elt, "auid", "xcap-caps", xt_string);
//				txc_xml_elements_add(auids->elements, &elt, txc_xml_element_free);
//				break;
//			}
//
//		case 1:
//			{	/* <auid>resource-lists</auid> */
//				txc_xml_element_init_set(&elt, "auid", "resource-lists", xt_string);
//				txc_xml_elements_add(auids->elements, &elt, txc_xml_element_free);
//				break;
//			}
//
//		case 2:
//			{	/* <auid>rls-services</auid> */
//				txc_xml_element_init_set(&elt, "auid", "rls-services", xt_string);
//				txc_xml_elements_add(auids->elements, &elt, txc_xml_element_free);
//				break;
//			}
//
//		case 3:
//			{	/* <auid>org.openmobilealliance.xcap-directory</auid> */
//				txc_xml_element_init_set(&elt, "auid", "org.openmobilealliance.xcap-directory", xt_string);
//				txc_xml_elements_add(auids->elements, &elt, txc_xml_element_free);
//				break;
//			}
//		}
//	}
//
//	//
//	// namespaces
//	//
//	txc_xml_element_init_set(&namespaces, "namespaces", "", xt_none); /*namespaces root*/
//	for(i=0; i<5;i++)
//	{
//		switch(i)
//		{
//		case 0:
//			{	/* <namespace>urn:ietf:params:xml:ns:xcap-caps</namespace> */
//				txc_xml_element_init_set(&elt, "namespace", "<namespace>urn:ietf:params:xml:ns:xcap-caps</namespace>", xt_string);
//				txc_xml_elements_add(namespaces->elements, &elt, txc_xml_element_free);
//				break;
//			}
//
//		case 1:
//			{	/* <namespace>urn:ietf:params:xml:ns:xcap-error</namespace> */
//				txc_xml_element_init_set(&elt, "namespace", "<namespace>urn:ietf:params:xml:ns:xcap-error</namespace>", xt_string);
//				txc_xml_elements_add(namespaces->elements, &elt, txc_xml_element_free);
//				break;
//			}
//
//		case 2:
//			{	/* <namespace>urn:ietf:params:xml:ns:resource-lists</namespace> */
//				txc_xml_element_init_set(&elt, "namespace", "<namespace>urn:ietf:params:xml:ns:resource-lists</namespace>", xt_string);
//				txc_xml_elements_add(namespaces->elements, &elt, txc_xml_element_free);
//				break;
//			}
//
//		case 3:
//			{	/* <namespace>urn:ietf:params:xml:ns:rls-services</namespace> */
//				txc_xml_element_init_set(&elt, "namespace", "<namespace>urn:ietf:params:xml:ns:rls-services</namespace>", xt_string);
//				txc_xml_elements_add(namespaces->elements, &elt, txc_xml_element_free);
//				break;
//			}
//
//		case 4:
//			{	/* <namespace>urn:oma:params:ns:resource-list:oma-uriusage</namespace> */
//				txc_xml_element_init_set(&elt, "namespace", "urn:oma:params:ns:resource-list:oma-uriusage</namespace>", xt_string);
//				txc_xml_elements_add(namespaces->elements, &elt, txc_xml_element_free);
//				break;
//			}
//		}
//	}
//	
//	/* add auids */
//	tsk_list_add_data(caps->root, ((void**) &auids), txc_xml_element_free);
//
//	/* add namespaces */
//	tsk_list_add_data(caps->root, ((void**) &namespaces), txc_xml_element_free);
//
//	return caps;
//}
