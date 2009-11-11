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

/**@file txc_rlist.c
 * @brief RFC 4826 subclause 3: <a href="http://tools.ietf.org/html/rfc4826#section-3">Resource Lists Documents</a>
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */

/*
RFC 4826 - Extensible Markup Language (XML) Formats for Representing Resource Lists

	resource-lists
		->list-1(name)
			->entry-1(uri, display-name)
			->entry-2(uri, display-name)
		->list-2(name)
		->list-3(name)
*/
#include "txc_rlist.h"
#include "txc.h"
#include "txc_macros.h"
#include "tsk_string.h"

#include "tsk_memory.h"
#include "tsk_macros.h"

#include <string.h>

/**@defgroup txc_rlist_group XCAP Resource Lists Documents 
*/


/**@page txc_rlist_page XCAP Resource Lists Documents Tutorial (resource-lists)
* @par Application Unique ID (AUID)
* - '<span style="text-decoration:underline;">resource-lists</span>' as per rfc 4826 subclause 3.4.1
* @par Default Document Namespace
* - '<span style="text-decoration:underline;">urn:ietf:params:xml:ns:resource-lists</span>' as per rfc 4826 subclause 3.4.4
* @par MIME Type
* - '<span style="text-decoration:underline;">application/resource-lists+xml</span>' as per rfc 4826 subclause 3.4.2
* @par Default document name
* - '<span style="text-decoration:underline;">index</span>' as per rfc 4826 subclause 3.4.7*
*
* <H2>== Deserialize and dump a resource-list document received from an XDMS==</H2>
* @code
#include "txc_api.h" 

char* entry_str = 0;
txc_rlist_t* rlist = 0;
txc_rlist_listx_L_t* list = 0;
tsk_list_item_t *item1 = 0, *item2 = 0, *item3 = 0;

printf("\n---\nTEST RESOURCE-LISTS-2\n---\n");
{
	// create rlist context
	rlist = txc_rlist_create(rlist_str, size);

	// get all lists
	printf("\nget all lists\n");
	if(list = txc_rlist_get_all_lists(rlist, 1))
	{
		tsk_list_foreach(item1, list)
		{
			txc_rlist_list2_t *rlist_list2 = ((txc_rlist_list2_t*)item1->data);

			// name and display-name 
			printf("name\"%s\" display-name=\"%s\"\n", rlist_list2->name, rlist_list2->display_name);

			// ==entries==
			tsk_list_foreach(item2, rlist_list2->entries)
			{
				txc_rlist_entry_t *entry = ((txc_rlist_entry_t*)item2->data);
				char* entry_str = txc_rlist_entry_serialize(entry);
				printf("\n%s\n", entry_str);
				TSK_SAFE_FREE2(entry_str);
			}

			// ==externals==
			tsk_list_foreach(item3, rlist_list2->externals)
			{
				txc_rlist_external_t *external = ((txc_rlist_external_t*)item3->data);
				char* external_str = txc_rlist_external_serialize(external);
				printf("\n%s\n", external_str);
				TSK_SAFE_FREE2(external_str);
			}

		}
		TSK_LIST_SAFE_FREE(list);
	}

// free rlist context
txc_rlist_free(&rlist);
* @endcode
*
* <H2>== Get all entries in the list named 'rcs_revokedcontacts'==</H2>
* @code
#include "txc_api.h" 

txc_rlist_t* rlist = 0;
tsk_list_t* list = 0;

... initialization ...

printf("\nget all entries in the list named 'rcs_revokedcontacts'\n");
	if(list = txc_rlist_get_entries_by_list(rlist, "rcs_revokedcontacts"))
	{
		tsk_list_foreach(item, list)
		{
			txc_rlist_entry_t *entry = ((txc_rlist_entry_t*)item->data);
			printf("list:\"%s\" uri:\"%s\" display-name=\"%s\" last-modified\"%s\"\n", entry->list, entry->uri, entry->display_name, entry->last_modified);
		}
		TSK_LIST_SAFE_FREE(list);
	}
* @endcode
*/

#define RLIST_SELECT_LIST_BY_NAME(lname)\
	tsk_xml_select_node(rlist->docPtr->children, \
	TSK_XML_NODE_SELECT_BY_NAME("resource-lists"), \
	TSK_XML_NODE_SELECT_BY_ATT("list", "name", lname), \
	TSK_XML_NODE_SELECT_CONTENT(), \
	TSK_XML_NODE_SELECT_END())

#define RLIST_XML_HEADER		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>" \
								"<resource-lists xmlns=\""TXC_NS_RLIST"\" " \
								"xmlns:xd=\"urn:oma:xml:xdm:xcap-directory\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
#define RLIST_XML_FOOTER		"</resource-lists>"

#define RLIST_RETURN_IF_INVALID(rlist) if(!rlist || !(rlist->docPtr)) return 0;

/**@ingroup txc_rlist_group
* Internal function to initialize a previously created entry
* @param entry The entry to initilialize
* @sa TXC_RLIST_ENTRY_CREATE
*/
void txc_rlist_entry_init(txc_rlist_entry_t *entry)
{
	memset(entry, 0, sizeof(txc_rlist_entry_t));
}

/**@ingroup txc_rlist_group
* Set the entry's uri and display-name
* @param entry The entry to update
* @param uri The new entry's uri
* @param display_name The new entry's display-name
*/
void txc_rlist_entry_set(txc_rlist_entry_t *entry, const char* uri, const char* display_name)
{
	if(entry)
	{		
		tsk_strupdate2(&(entry->uri), uri);
		tsk_strupdate2(&(entry->display_name), display_name);
	}
}

/**@ingroup txc_rlist_group
* Internal function to free an entry previously created using @ref TXC_RLIST_ENTRY_SAFE_FREE.
* You MUST call @ref TXC_RLIST_ENTRY_SAFE_FREE to safely free your entry.
* @sa @ref TXC_RLIST_ENTRY_SAFE_FREE
*/
void txc_rlist_entry_free(void **_entry)
{
	txc_rlist_entry_t **entry = ((txc_rlist_entry_t**)_entry);

	TSK_SAFE_FREE2((*entry)->uri);
	TSK_SAFE_FREE2((*entry)->display_name);
	TSK_SAFE_FREE2((*entry)->list);
	TSK_SAFE_FREE2((*entry)->last_modified);
	TSK_SAFE_FREE2((*entry)->etag);

	tsk_free2(_entry);
}

/**@ingroup txc_rlist_group
* Internal function to initialize a previously created @ref txc_rlist_list_t object.
* You MUST use @ref TXC_RLIST_LIST_CREATE to create and intialize your simple list.
* @param list The list to initialize
* @sa @ref TXC_RLIST_LIST_CREATE
*/
void txc_rlist_list_init(txc_rlist_list_t *list)
{
	memset(list, 0, sizeof(txc_rlist_list_t));
}

/**@ingroup txc_rlist_group
* Update the list's name and display-name
* @param list The list to update
* @param name The new name
* @param display_name The new display-name
*/
void txc_rlist_list_set(txc_rlist_list_t *list, const char* name, const char* display_name)
{
	if(list)
	{	
		tsk_strupdate2(&(list->name), name);
		tsk_strupdate2(&(list->display_name), display_name);
	}
}

/**@ingroup txc_rlist_group
* Internal function to free a previously created @ref txc_rlist_list_t object. 
* You MUST use @ref TXC_RLIST_LIST_SAFE_FREE to free your simple list.
* @param _list The simple list to free
* @sa @ref TXC_RLIST_LIST_SAFE_FREE
*/
void txc_rlist_list_free(void **_list)
{
	txc_rlist_list_t **list = ((txc_rlist_list_t**)_list);

	TSK_SAFE_FREE2((*list)->display_name);
	TSK_SAFE_FREE2((*list)->name);

	tsk_free2(_list);
}

/**@ingroup txc_rlist_group
* Internal function to initialize a newly created @ref txc_rlist_list2_t object.
* You MUST use @ref TXC_RLIST_LIST2_CREATE to create and initialize your complex list.
* @param list2 The complex list to initialize
* @sa @ref TXC_RLIST_LIST2_CREATE
*/
void txc_rlist_list2_init(txc_rlist_list2_t *list2)
{
	memset(list2, 0, sizeof(txc_rlist_list2_t));
}

/**@ingroup txc_rlist_group
* Set list2's name and display-name
* @param list2 The complex list to update
* @param name The new name
* @param display_name The new display name
*/
void txc_rlist_list2_set(txc_rlist_list2_t *list2, const char* name, const char* display_name)
{
	if(list2)
	{		
		tsk_strupdate2(&(list2->name), name);
		tsk_strupdate2(&(list2->display_name),display_name);
	}
}
/**@ingroup txc_rlist_group
* Add external ement to a complex list.
* @param list2 The list to update.
* @param anchor The anchor of the external element to add.
*/
void txc_rlist_list2_add_external(txc_rlist_list2_t *list2, const char* anchor)
{
	if(list2)
	{
		txc_rlist_external_t *external = 0;
		if(!(list2->externals)) 
		{
			TSK_LIST_CREATE(list2->externals);
		}

		TXC_RLIST_EXTERNAL_CREATE(external);
		txc_rlist_external_set(external, anchor);
		tsk_list_add_data(list2->externals, ((void**)&external), txc_rlist_external_free);
	}
}

/**@ingroup txc_rlist_group
* Add an entry to a complex list
* @param list2 The list to update
* @param uri The uri of the entry to add
* @param display_name The display-name of the entry to add
*/
void txc_rlist_list2_add_entry(txc_rlist_list2_t *list2, const char* uri, const char* display_name)
{
	if(list2)
	{
		txc_rlist_entry_t *entry = 0;
		TXC_RLIST_ENTRY_CREATE(entry);
		txc_rlist_entry_set(entry, uri, display_name);

		if(!(list2->entries)) 
		{
			TSK_LIST_CREATE(list2->entries);
		}
		tsk_list_add_data(list2->entries, ((void**)&entry), txc_rlist_entry_free);
	}
}

/**@ingroup txc_rlist_group
* Internal function to free a previously created @ref txc_rlist_list2_t object using @ref TXC_RLIST_LIST2_CREATE.
* You MUST use @ref TXC_RLIST_LIST2_SAFE_FREE to safely free your complex list.
* @param _list2 The complex list to free
* @sa @ref TXC_RLIST_LIST2_SAFE_FREE
*/
void txc_rlist_list2_free(void **_list2)
{
	txc_rlist_list2_t **list2 = ((txc_rlist_list2_t**)_list2);

	TSK_SAFE_FREE2((*list2)->display_name);
	TSK_SAFE_FREE2((*list2)->name);
	TSK_LIST_SAFE_FREE((*list2)->externals);
	TSK_LIST_SAFE_FREE((*list2)->entries);

	tsk_free2(_list2);
}

/**@ingroup txc_rlist_group
* Internal function to initialize an external element.
* You MUST call @ref TXC_RLIST_EXTERNAL_CREATE to create and initialize your element.
* @param external The external element to initialize.
* @sa @ref TXC_RLIST_EXTERNAL_CREATE.
*/
void txc_rlist_external_init(txc_rlist_external_t *external)
{
	memset(external, 0, sizeof(txc_rlist_external_t));
}

/**@ingroup txc_rlist_group
* Sets the external's anchor value
* @param external The external element to update
* @param anchor The new anchor
*/
void txc_rlist_external_set(txc_rlist_external_t *external, const char* anchor)
{
	if(external)
	{
		tsk_strupdate2(&(external->anchor), anchor);
	}
}

/**@ingroup txc_rlist_group
* Internal function to free a previously created @ref txc_rlist_external_t object.
* You must call @ref TXC_RLIST_EXTERNAL_SAFE_FREE to free your object.
* @param _external The external element to free
* @sa @ref TXC_RLIST_EXTERNAL_SAFE_FREE
*/
void txc_rlist_external_free(void **_external)
{
	txc_rlist_external_t **external = ((txc_rlist_external_t**)_external);

	TSK_SAFE_FREE2((*external)->anchor);
	TSK_SAFE_FREE2((*external)->list);

	tsk_free2(_external);
}

/**@ingroup txc_rlist_group
* Internal function to deserialize an entry element from an XML node pointer.
* You MUST call @ref TXC_RLIST_ENTRY_SAFE_FREE to free the returned object.
* @param node The XML node pointer from which to deserialize/create the entry
* @param lname The name of the list holding the entry to deserialize/create
* @retval A @ref txc_rlist_entry_t object or NULL if deserialization fail.
*/
txc_rlist_entry_t* txc_rlist_entry_from_xml(const xmlNodePtr node, const char* lname)
{
	xmlNodePtr node2 = 0;
	txc_rlist_entry_t *rlist_entry = 0;

	if(tsk_xml_find_node(node, "entry", nft_none))
	{
		TXC_RLIST_ENTRY_CREATE(rlist_entry);
		rlist_entry->list = tsk_strdup2(lname);

		/* display-name */
		node2 = tsk_xml_select_node(node, TSK_XML_NODE_SELECT_BY_NAME("entry"), 
			TSK_XML_NODE_SELECT_BY_NAME("display-name"), TSK_XML_NODE_SELECT_END());
		rlist_entry->display_name = tsk_strdup2(TSK_XML_NODE_SAFE_GET_TEXTVALUE(node2));
		
		/* uri */
		node2 = tsk_xml_select_node(node, 
			TSK_XML_NODE_SELECT_ATT_VALUE("entry", "uri"), TSK_XML_NODE_SELECT_END());
		rlist_entry->uri = tsk_strdup2(TSK_XML_NODE_SAFE_GET_TEXTVALUE(node2));

		/* last-modified */
		node2 = tsk_xml_select_node(node, 
			TSK_XML_NODE_SELECT_ATT_VALUE("entry", "last-modified"), TSK_XML_NODE_SELECT_END());
		rlist_entry->last_modified = tsk_strdup2(TSK_XML_NODE_SAFE_GET_TEXTVALUE(node2));

		/* etag */
		node2 = tsk_xml_select_node(node, 
			TSK_XML_NODE_SELECT_ATT_VALUE("entry", "etag"), TSK_XML_NODE_SELECT_END());
		rlist_entry->etag = tsk_strdup2(TSK_XML_NODE_SAFE_GET_TEXTVALUE(node2));
		
	}
	else return 0;

	return rlist_entry;
}

/**@ingroup txc_rlist_group
* Internal function to deserialize an external element from an XML node pointer.
* You MUST call @ref TXC_RLIST_EXTERNAL_SAFE_FREE to free the returned object.
* @param node The XML node pointer from which to deserialize/create the element
* @param lname The name of the list holding the element to deserialize/create
* @retval A @ref txc_rlist_external_t object or NULL if deserialization fail.
*/
txc_rlist_external_t* txc_rlist_external_from_xml(const xmlNodePtr node, const char* lname)
{
	xmlNodePtr node2 = 0;
	txc_rlist_external_t *rlist_external = 0;

	if(tsk_xml_find_node(node, "external", nft_none))
	{
		TXC_RLIST_EXTERNAL_CREATE(rlist_external);
		rlist_external->list = tsk_strdup2(lname);
		
		/* anchor */
		node2 = tsk_xml_select_node(node, 
			TSK_XML_NODE_SELECT_ATT_VALUE("external", "anchor"),
			TSK_XML_NODE_SELECT_END());
		rlist_external->anchor = tsk_strdup2(TSK_XML_NODE_SAFE_GET_TEXTVALUE(node2));
		
	}else return 0;

	return rlist_external;
}

/**@ingroup txc_rlist_group
* Internal function to deserialize a list element from an XML node pointer.
* You MUST call @ref TXC_RLIST_LIST_SAFE_FREE to free the returned object.
* @param node The XML node pointer from which to deserialize/create the element
* @retval A @ref txc_rlist_list_t object or NULL if deserialization fail.
*/
txc_rlist_list_t* txc_rlist_list_from_xml(const xmlNodePtr node)
{
	xmlNodePtr node2 = 0;
	txc_rlist_list_t *rlist_list = 0;

	if(tsk_xml_find_node(node, "list", nft_none))
	{
		TXC_RLIST_LIST_CREATE(rlist_list);
		
		/* name */
		node2 = tsk_xml_select_node(node, 
			TSK_XML_NODE_SELECT_ATT_VALUE("list", "name"),
			TSK_XML_NODE_SELECT_END());
		rlist_list->name = tsk_strdup2(TSK_XML_NODE_SAFE_GET_TEXTVALUE(node2));

		/* display-name */
		node2 = tsk_xml_select_node(node, TSK_XML_NODE_SELECT_BY_NAME("list"),
			TSK_XML_NODE_SELECT_BY_NAME("display-name"),
			TSK_XML_NODE_SELECT_END());
		rlist_list->display_name = tsk_strdup2(TSK_XML_NODE_SAFE_GET_TEXTVALUE(node2));
		
	}else return 0;

	return rlist_list;
}

/**@ingroup txc_rlist_group
* Internal function to deserialize a list element from an XML node pointer.
* You MUST call @ref TXC_RLIST_LIST2_SAFE_FREE to free the returned object.
* @param node The XML node pointer from which to deserialize/create the element
* @retval A @ref txc_rlist_list2_t object or NULL if deserialization fail.
*/
txc_rlist_list2_t* txc_rlist_list2_from_xml(const xmlNodePtr node)
{
	xmlNodePtr node2 = 0;
	txc_rlist_list2_t *rlist_list2 = 0;

	if(tsk_xml_find_node(node, "list", nft_none))
	{
		TXC_RLIST_LIST_CREATE(rlist_list2);
		
		/** name **/
		node2 = tsk_xml_select_node(node, 
			TSK_XML_NODE_SELECT_ATT_VALUE("list", "name"),
			TSK_XML_NODE_SELECT_END());
		rlist_list2->name = tsk_strdup2(TSK_XML_NODE_SAFE_GET_TEXTVALUE(node2));

		/** display-name **/
		node2 = tsk_xml_select_node(node, TSK_XML_NODE_SELECT_BY_NAME("list"),
			TSK_XML_NODE_SELECT_BY_NAME("display-name"),
			TSK_XML_NODE_SELECT_END());
		rlist_list2->display_name = tsk_strdup2(TSK_XML_NODE_SAFE_GET_TEXTVALUE(node2));

		/*== externals ==*/
		/* select first external */
		if(! (node2 = tsk_xml_find_node(node->children, "external", nft_none)) ) node2 = tsk_xml_find_node(node->children, "external", nft_next);
		if(node2)
		{
			txc_rlist_external_t *rlist_external = 0;
			TSK_LIST_CREATE(rlist_list2->externals);
			do
			{
				rlist_external = txc_rlist_external_from_xml(node2, rlist_list2->name);
				tsk_list_add_data(rlist_list2->externals, ((void**) &rlist_external), txc_rlist_external_free);
			}
			while(node2 = tsk_xml_find_node(node2, "external", nft_next));
		}
		
		/*== entries ==*/
		/* select first entry */
		if(!(node2 = tsk_xml_find_node(node->children, "entry", nft_none))) node2 = tsk_xml_find_node(node->children, "entry", nft_next);
		if(node2)
		{
			txc_rlist_entry_t *rlist_entry = 0;
			TSK_LIST_CREATE(rlist_list2->entries);
			do
			{
				rlist_entry = txc_rlist_entry_from_xml(node2, rlist_list2->name);
				tsk_list_add_data(rlist_list2->entries, ((void**) &rlist_entry), txc_rlist_entry_free);
			}
			while(node2 = tsk_xml_find_node(node2, "entry", nft_next));
		}
	}else return 0;

	return rlist_list2;
}

/**@ingroup txc_rlist_group
* Create a resource-lists context.
* @param buffer The XML buffer from wich to create the context
* @param size The size of the XML buffer
* @retval @ref txc_rlist_t object holding a pointer to the XML buffer.
* You must call @ref txc_rlist_free to free the returned object.
* @sa @ref txc_rlist_free
*/
txc_rlist_t* txc_rlist_create(const char* buffer, size_t size)
{
	if(buffer && size)
	{
		txc_rlist_t* rlist = (txc_rlist_t*)tsk_malloc2(sizeof(txc_rlist_t));
		memset(rlist, 0, sizeof(txc_rlist_t));
		rlist->docPtr = xmlParseMemory(buffer, (int)size);

		return rlist;
	}

	return 0;
}

/**@ingroup txc_rlist_group
* Returns all lists holded by this context
* @param rlist The context created using @ref txc_rlist_create
* @param type2 The type of list to return
* @retval Returns @ref txc_rlist_list2_t object if @a type2 and @ref txc_rlist_list_t otherwise.
* You MUST call TSK_LIST_SAFE_FREE to safely free the returned object.
*/
txc_rlist_listx_L_t* txc_rlist_get_all_lists(const txc_rlist_t* rlist, int type2)
{
	txc_rlist_listx_L_t* list = 0;
	xmlNodePtr node = 0;

	RLIST_RETURN_IF_INVALID(rlist);
	
	/* root */
	node = tsk_xml_select_node(rlist->docPtr->children,
		TSK_XML_NODE_SELECT_BY_NAME("resource-lists"),
		TSK_XML_NODE_SELECT_END());

	/* select first list */
	if(!tsk_xml_find_node(node, "list", nft_none)) node = tsk_xml_find_node(node, "list", nft_next);
	if(node)
	{
		TSK_LIST_CREATE(list);
		do
		{
			if(type2)
			{
				txc_rlist_list2_t* rlist_list2 = txc_rlist_list2_from_xml(node);
				tsk_list_add_data(list, ((void**) &rlist_list2), txc_rlist_list2_free);
			}
			else
			{
				txc_rlist_list_t* rlist_list = txc_rlist_list_from_xml(node);
				tsk_list_add_data(list, ((void**) &rlist_list), txc_rlist_list_free);
			}
		}
		while(node = tsk_xml_find_node(node, "list", nft_next));
	}

	return list;
}

/**@ingroup txc_rlist_group
* Returns all entries from a list element
* @param rlist The context created using @ref txc_rlist_create
* @param lname The name of the list holding the entries to return
* @retval Returns @ref txc_rlist_entry_L_t object if exist and NULL otherwise.
* You MUST call TSK_LIST_SAFE_FREE to safely free the returned object.
*/
txc_rlist_entry_L_t* txc_rlist_get_entries_by_list(const txc_rlist_t* rlist, const char* lname)
{
	txc_rlist_entry_t *rlist_entry = 0;
	txc_rlist_entry_L_t* list = 0;
	xmlNodePtr node = 0;

	RLIST_RETURN_IF_INVALID(rlist);

	node = RLIST_SELECT_LIST_BY_NAME(lname);
	if(!node) return 0;

	TSK_LIST_CREATE(list);

	/* select first entry */
	if(!tsk_xml_find_node(node, "entry", nft_none)) node = tsk_xml_find_node(node, "entry", nft_next);
	if(node)
	{
		do
		{
			rlist_entry = txc_rlist_entry_from_xml(node, lname);
			tsk_list_add_data(list, ((void**) &rlist_entry), txc_rlist_entry_free);
		}
		while(node = tsk_xml_find_node(node, "entry", nft_next));
	}

	return list;
}

/**@ingroup txc_rlist_group
* Returns all external elements from a list element
* @param rlist The context created using @ref txc_rlist_create
* @param lname The name of the list holding the entries to return
* @retval Returns @ref txc_rlist_external_L_t object if exist and NULL otherwise.
* You MUST call TSK_LIST_SAFE_FREE to safely free the returned object.
*/
txc_rlist_external_L_t* txc_rlist_get_externals_by_list(const txc_rlist_t* rlist, const char* lname)
{
	txc_rlist_external_t *rlist_external = 0;
	txc_rlist_external_L_t* list = 0;
	xmlNodePtr node = 0;

	RLIST_RETURN_IF_INVALID(rlist);

	node = RLIST_SELECT_LIST_BY_NAME(lname);
	if(!node) return 0;
	
	/* select first external */
	if(!tsk_xml_find_node(node, "external", nft_none)) node = tsk_xml_find_node(node, "external", nft_next);
	if(node)
	{
		TSK_LIST_CREATE(list);
		do
		{
			rlist_external = txc_rlist_external_from_xml(node, lname);
			tsk_list_add_data(list, ((void**) &rlist_external), txc_rlist_external_free);
		}
		while(node = tsk_xml_find_node(node, "external", nft_next));
	}

	return list;
}

/**@ingroup txc_rlist_group
* Serialize an external element
* @param external The object to serialize
* @retval XML string representing the serialized object
* You MUST call TSK_SAFE_FREE2 macro to free the returned string.
*/
char* txc_rlist_external_serialize(const txc_rlist_external_t *external)
{
	char* external_str = 0;
	if(!external) return 0;

	tsk_sprintf(0, &external_str, "<external anchor=\"%s\" />", external->anchor);
	return external_str;
}

/**
* Serialize an entry element
* @param entry The object to serialize
* @retval XML string representing the serialized object
* You MUST call TSK_SAFE_FREE2 macro to free the returned string.
* @sa @ref txc_rlist_entry_serialize2
*/
char* txc_rlist_entry_serialize(const txc_rlist_entry_t *entry)
{
	return entry ? txc_rlist_entry_serialize2(entry->uri, entry->display_name) : 0;
}

/**@ingroup txc_rlist_group
* Serialize an entry element
* @param uri The uri of the entry to serialize
* @param displayname The display-name of the entry to serialize
* @retval XML string representing the serialized object
* You MUST call TSK_SAFE_FREE2 macro to free the returned string.
* @sa @ref txc_rlist_entry_serialize
*/
char* txc_rlist_entry_serialize2(const char* uri, const char* displayname)
{
	char* entry_str = 0;
	if(!uri || !displayname) return 0;

	tsk_sprintf(0, &entry_str,
				"<entry uri=\"%s\" xmlns=\""TXC_NS_RLIST"\">"
					"<display-name>%s</display-name>"
				"</entry>",
				uri, displayname);
	return entry_str;
}

/**@ingroup txc_rlist_group
* Serialize simple list element
* @param list The list to serialize
* @retval XML string representing the serialized object
* You MUST call TSK_SAFE_FREE2 macro to free the returned string.
* @sa @ref txc_rlist_list_serialize2
*/
char* txc_rlist_list_serialize(const txc_rlist_list_t *list)
{
	return list ? txc_rlist_list_serialize2(list->name, list->display_name) : 0;
}

/**@ingroup txc_rlist_group
* Serialize simple list element
* @param name The name of the list to serialize
* @param displayname The display-name of the list to serialize
* @retval XML string representing the serialized object
* You MUST call TSK_SAFE_FREE2 macro to free the returned string.
* @sa @ref txc_rlist_list_serialize
*/
char* txc_rlist_list_serialize2(const char* name, const char* displayname)
{
	char* list_str = 0;

	if(!name || !displayname) return 0;
	
	/* serialize */
	 tsk_sprintf(0, &list_str,
				"<list name=\"%s\" xmlns=\""TXC_NS_RLIST"\">"
					"<display-name>%s</display-name>"
				"</list>",
				name, displayname);
	return list_str;
}

/**@ingroup txc_rlist_group
* Serialize one complex list element without XML header/declaration.
* @param list2 The the complex list to serialize
* @retval XML string representing the serialized object
* You MUST call TSK_SAFE_FREE2 macro to free the returned string.
* @sa @ref txc_rlist_list22_serialize
*/
char* txc_rlist_list2_serialize(const txc_rlist_list2_t *list2)
{
	char* list2_str = 0;
	tsk_list_item_t *item = 0;

	/* check validity */
	if(!list2) return 0;

	/* name and display-name */
	tsk_sprintf(0, &list2_str,
				"<list name=\"%s\" xmlns=\""TXC_NS_RLIST"\">"
					"<display-name>%s</display-name>",
				list2->name, list2->display_name);
	
	/* entries */
	tsk_list_foreach(item, list2->entries)
	{
		txc_rlist_entry_t *entry = ((txc_rlist_entry_t*)item->data);
		char* entry_str = txc_rlist_entry_serialize(entry);
		tsk_strcat2(&list2_str, (const char*)entry_str);
		TSK_SAFE_FREE2(entry_str);
	}
	
	/*externals*/
	tsk_list_foreach(item, list2->externals)
	{
		txc_rlist_external_t *external = ((txc_rlist_external_t*)item->data);
		char* external_str = txc_rlist_external_serialize(external);
		tsk_strcat2(&list2_str, (const char*)external_str);
		TSK_FREE(external_str);
	}
	
	/* close list */
	tsk_strcat2(&list2_str, "</list>");

	return list2_str;
}

/**@ingroup txc_rlist_group
* Serialize a complete/entire resource-lists document with an XML header/declaration.
* @param list22 The 2xcomplex list to serialize
* @retval XML string representing the serialized object
* You MUST call TSK_SAFE_FREE2 macro to free the returned string.
* @sa @ref txc_rlist_list2_serialize
*/
char* txc_rlist_list22_serialize(const txc_rlist_list2_LL_t *list22)
{
	tsk_list_item_t* item = 0;
	char* rlist2_str = 0;

	if(!list22) return 0;

	/* xml header */
	tsk_strcat2(&rlist2_str, RLIST_XML_HEADER);

	tsk_list_foreach(item, list22)
	{
		/* get list2 */
		txc_rlist_list2_t *list2 = ((txc_rlist_list2_t*)item->data);
		char* list2_str = txc_rlist_list2_serialize(list2);
		tsk_strcat2(&rlist2_str, list2_str);
		TSK_FREE(list2_str);
	}
	
	/* xml footer */
	tsk_strcat2(&rlist2_str, RLIST_XML_FOOTER);

	return rlist2_str;
}

/**@ingroup txc_rlist_group
* Add an XML declaration/header to the content
* @param xml_content The content to decore
* @retval  Decorated XML content.
* You MUST call TSK_SAFE_FREE2 to free the returned string.
*/
char* txc_rlist_add_xml_header(const char* xml_content)
{
	char* xml = 0;

	/* append xml header */
	tsk_sprintf(0, &xml,
				"%s%s%s",
				RLIST_XML_HEADER, xml_content, RLIST_XML_FOOTER);
	return xml;
}

/**@ingroup txc_rlist_group
* Function to free a previously created context using @ref txc_rlist_create.
* @param rlist The context the free.
* @sa @ref txc_rlist_create
*/
void txc_rlist_free(txc_rlist_t **rlist )
{
	if(*rlist)
	{	
		xmlFreeDoc((*rlist)->docPtr);
		
		free(*rlist);
		(*rlist)=0;
	}
}

#undef RLIST_SELECT_LIST_BY_NAME
#undef RLIST_RETURN_IF_INVALID

#undef RLIST_XML_HEADER
#undef RLIST_XML_FOOTER