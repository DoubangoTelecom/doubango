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
#include "txc_macros.h"
#include "tsk_string.h"

#include "tsk_memory.h"
#include "tsk_macros.h"

#include <string.h>

#define RLIST_SELECT_LIST_BY_NAME(lname)\
	tsk_xml_select_node(rlist->docPtr->children, \
	TSK_XML_NODE_SELECT_BY_NAME("resource-lists"), \
	TSK_XML_NODE_SELECT_BY_ATT("list", "name", lname), \
	TSK_XML_NODE_SELECT_CONTENT(), \
	TSK_XML_NODE_SELECT_END())

#define RLIST_XML_HEADER		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>" \
								"<resource-lists xmlns=\"urn:ietf:params:xml:ns:resource-lists\" " \
								"xmlns:xd=\"urn:oma:xml:xdm:xcap-directory\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
#define RLIST_XML_FOOTER		"</resource-lists>"

#define RLIST_RETURN_IF_INVALID(rlist) if(!rlist || !(rlist->docPtr)) return 0;

//static const char* txc_rlist_ns = "urn:ietf:params:xml:ns:resource-lists";

/* init entry*/
void txc_rlist_entry_init(txc_rlist_entry_t *entry)
{
	memset(entry, 0, sizeof(txc_rlist_entry_t));
}

/* set entry's uri and display-name */
void txc_rlist_entry_set(txc_rlist_entry_t *entry, const char* uri, const char* display_name)
{
	if(entry)
	{		
		tsk_strupdate(0, &(entry->uri), uri);
		tsk_strupdate(0, &(entry->display_name), display_name);
	}
}

/* free entry */
void txc_rlist_entry_free(void **_entry)
{
	txc_rlist_entry_t **entry = ((txc_rlist_entry_t**)_entry);

	TSK_SAFE_FREE2((*entry)->uri);
	TSK_SAFE_FREE2((*entry)->display_name);
	TSK_SAFE_FREE2((*entry)->list);
	TSK_SAFE_FREE2((*entry)->last_modified);
	TSK_SAFE_FREE2((*entry)->etag);

	tsk_free(0, _entry);
}

/* init list */
void txc_rlist_list_init(txc_rlist_list_t *list)
{
	memset(list, 0, sizeof(txc_rlist_list_t));
}

/* set list's name and display-name */
void txc_rlist_list_set(txc_rlist_list_t *list, const char* name, const char* display_name)
{
	if(list)
	{		
		tsk_strupdate(0, &(list->name), name);
		tsk_strupdate(0, &(list->display_name), display_name);
	}
}

/* free list */
void txc_rlist_list_free(void **_list)
{
	txc_rlist_list_t **list = ((txc_rlist_list_t**)_list);

	TSK_SAFE_FREE2((*list)->display_name);
	TSK_SAFE_FREE2((*list)->name);

	tsk_free(0, _list);
}

/* init list2 */
void txc_rlist_list2_init(txc_rlist_list2_t *list2)
{
	memset(list2, 0, sizeof(txc_rlist_list2_t));
}

/* set list2's name and display-name */
void txc_rlist_list2_set(txc_rlist_list2_t *list2, const char* name, const char* display_name)
{
	if(list2)
	{		
		tsk_strupdate(0, &(list2->name), name);
		tsk_strupdate(0, &(list2->display_name),display_name);
	}
}
/* add external eement to the list2 */
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

/* and an entry to the list */
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

/* free list2 */
void txc_rlist_list2_free(void **_list2)
{
	txc_rlist_list2_t **list2 = ((txc_rlist_list2_t**)_list2);

	TSK_SAFE_FREE2((*list2)->display_name);
	TSK_SAFE_FREE2((*list2)->name);
	TSK_LIST_SAFE_FREE((*list2)->externals);
	TSK_LIST_SAFE_FREE((*list2)->entries);

	tsk_free(0, _list2);
}

/* init external */
void txc_rlist_external_init(txc_rlist_external_t *external)
{
	memset(external, 0, sizeof(txc_rlist_external_t));
}

/* set external's anchor */
void txc_rlist_external_set(txc_rlist_external_t *external, const char* anchor)
{
	if(external)
	{
		tsk_strupdate(0, &(external->anchor), anchor);
	}
}

/* free external */
void txc_rlist_external_free(void **_external)
{
	txc_rlist_external_t **external = ((txc_rlist_external_t**)_external);

	TSK_SAFE_FREE2((*external)->anchor);
	TSK_SAFE_FREE2((*external)->list);

	tsk_free(0, _external);
}

/* xml<->entry binding*/
/* ATTENTION: use 'TXC_RLIST_ENTRY_SAFE_FREE' function to free the returned object */
txc_rlist_entry_t* txc_rlist_entry_from_xml(const xmlNodePtr node, const char* lname)
{
	xmlNodePtr node2 = 0;
	txc_rlist_entry_t *rlist_entry = 0;

	if(tsk_xml_find_node(node, "entry", nft_none))
	{
		TXC_RLIST_ENTRY_CREATE(rlist_entry);
		rlist_entry->list = tsk_strdup(0, lname);

		/* display-name */
		node2 = tsk_xml_select_node(node, TSK_XML_NODE_SELECT_BY_NAME("entry"), 
			TSK_XML_NODE_SELECT_BY_NAME("display-name"), TSK_XML_NODE_SELECT_END());
		rlist_entry->display_name = tsk_strdup(0, TSK_XML_NODE_SAFE_GET_TEXTVALUE(node2));
		
		/* uri */
		node2 = tsk_xml_select_node(node, 
			TSK_XML_NODE_SELECT_ATT_VALUE("entry", "uri"), TSK_XML_NODE_SELECT_END());
		rlist_entry->uri = tsk_strdup(0, TSK_XML_NODE_SAFE_GET_TEXTVALUE(node2));

		/* last-modified */
		node2 = tsk_xml_select_node(node, 
			TSK_XML_NODE_SELECT_ATT_VALUE("entry", "last-modified"), TSK_XML_NODE_SELECT_END());
		rlist_entry->last_modified = tsk_strdup(0, TSK_XML_NODE_SAFE_GET_TEXTVALUE(node2));

		/* etag */
		node2 = tsk_xml_select_node(node, 
			TSK_XML_NODE_SELECT_ATT_VALUE("entry", "etag"), TSK_XML_NODE_SELECT_END());
		rlist_entry->etag = tsk_strdup(0, TSK_XML_NODE_SAFE_GET_TEXTVALUE(node2));
		
	}
	else return 0;

	return rlist_entry;
}

/* xml<->entry binding*/
/* ATTENTION: use 'TXC_RLIST_EXTERNAL_SAFE_FREE' function to free the returned object */
txc_rlist_external_t* txc_rlist_external_from_xml(const xmlNodePtr node, const char* lname)
{
	xmlNodePtr node2 = 0;
	txc_rlist_external_t *rlist_external = 0;

	if(tsk_xml_find_node(node, "external", nft_none))
	{
		TXC_RLIST_EXTERNAL_CREATE(rlist_external);
		rlist_external->list = tsk_strdup(0, lname);
		
		/* anchor */
		node2 = tsk_xml_select_node(node, 
			TSK_XML_NODE_SELECT_ATT_VALUE("external", "anchor"),
			TSK_XML_NODE_SELECT_END());
		rlist_external->anchor = tsk_strdup(0, TSK_XML_NODE_SAFE_GET_TEXTVALUE(node2));
		
	}else return 0;

	return rlist_external;
}

/* xml<->list binding*/
/* ATTENTION: use 'TXC_RLIST_LIST_SAFE_FREE' function to free the returned object */
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
		rlist_list->name = tsk_strdup(0, TSK_XML_NODE_SAFE_GET_TEXTVALUE(node2));

		/* display-name */
		node2 = tsk_xml_select_node(node, TSK_XML_NODE_SELECT_BY_NAME("list"),
			TSK_XML_NODE_SELECT_BY_NAME("display-name"),
			TSK_XML_NODE_SELECT_END());
		rlist_list->display_name = tsk_strdup(0, TSK_XML_NODE_SAFE_GET_TEXTVALUE(node2));
		
	}else return 0;

	return rlist_list;
}

/* xml<->list binding*/
/* ATTENTION: use 'TXC_RLIST_LIST2_SAFE_FREE' function to free the returned object */
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
		rlist_list2->name = tsk_strdup(0, TSK_XML_NODE_SAFE_GET_TEXTVALUE(node2));

		/** display-name **/
		node2 = tsk_xml_select_node(node, TSK_XML_NODE_SELECT_BY_NAME("list"),
			TSK_XML_NODE_SELECT_BY_NAME("display-name"),
			TSK_XML_NODE_SELECT_END());
		rlist_list2->display_name = tsk_strdup(0, TSK_XML_NODE_SAFE_GET_TEXTVALUE(node2));

		/** externals **/
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
		
		/** entries **/
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

/* create resource-lists document */
txc_rlist_t* txc_rlist_create(const char* buffer, size_t size)
{
	if(buffer && size)
	{
		txc_rlist_t* rlist = (txc_rlist_t*)malloc(sizeof(txc_rlist_t));
		memset(rlist, 0, sizeof(txc_rlist_t));
		rlist->docPtr = xmlParseMemory(buffer, (int)size);

		return rlist;
	}

	return 0;
}

/* get all lists(txc_rlist_list_t or txc_rlist_list2_t) */
/* ATTENTION: use 'TSK_LIST_SAFE_FREE' function to free the returned object */
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

/* returns all entries into the list named @lname */
/* ATTENTION: use 'TSK_LIST_SAFE_FREE' function to free the returned object */
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

/* returns a list of externals by list name (txc_rlist_external_t) */
/* ATTENTION: use 'TSK_LIST_SAFE_FREE' function to free the returned object */
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

/* serialize an external */
/* ATTENTION: use 'TSK_SAFE_FREE2' macro to free the returned string */
char* txc_rlist_external_serialize(const txc_rlist_external_t *external)
{
	char* external_str = 0;
	if(!external) return 0;

	tsk_sprintf(0, &external_str, "<external anchor=\"%s\" />", external->anchor);
	return external_str;
}

/* serialize an entry */
/* ATTENTION: use 'TSK_SAFE_FREE2' macro to free the returned string */
char* txc_rlist_entry_serialize(const txc_rlist_entry_t *entry)
{
	return entry ? txc_rlist_entry_serialize2(entry->uri, entry->display_name) : 0;
}

/* serialize an entry */
/* ATTENTION: use 'TSK_SAFE_FREE2' macro to free the returned string */
char* txc_rlist_entry_serialize2(const char* uri, const char* displayname)
{
	char* entry_str = 0;
	if(!uri || !displayname) return 0;

	tsk_sprintf(0, &entry_str,
				"<entry uri=\"%s\" xmlns=\"urn:ietf:params:xml:ns:resource-lists\">"
					"<display-name>%s</display-name>"
				"</entry>",
				uri, displayname);
	return entry_str;
}

/* serialize list */
/* ATTENTION: use 'TSK_SAFE_FREE2' macro to free the returned string */
char* txc_rlist_list_serialize(const txc_rlist_list_t *list)
{
	return list ? txc_rlist_list_serialize2(list->name, list->display_name) : 0;
}

/* serialize list */
/* ATTENTION: use 'TSK_SAFE_FREE2' macro to free the returned string */
char* txc_rlist_list_serialize2(const char* name, const char* displayname)
{
	char* list_str = 0;

	if(!name || !displayname) return 0;
	
	/* serialize */
	 tsk_sprintf(0, &list_str,
				"<list name=\"%s\" xmlns=\"urn:ietf:params:xml:ns:resource-lists\">"
					"<display-name>%s</display-name>"
				"</list>",
				name, displayname);
	return list_str;
}

/* serialize list2 */
/* ATTENTION: use 'TSK_SAFE_FREE2' macro to free the returned string */
char* txc_rlist_list2_serialize(const txc_rlist_list2_t *list2)
{
	char* list2_str = 0;
	tsk_list_item_t *item = 0;

	/* check validity */
	if(!list2) return 0;

	/* name and display-name */
	tsk_sprintf(0, &list2_str,
				"<list name=\"%s\" xmlns=\"urn:ietf:params:xml:ns:resource-lists\">"
					"<display-name>%s</display-name>",
				list2->name, list2->display_name);
	
	/* entries */
	tsk_list_foreach(item, list2->entries)
	{
		txc_rlist_entry_t *entry = ((txc_rlist_entry_t*)item->data);
		char* entry_str = txc_rlist_entry_serialize(entry);
		tsk_strcat(0, &list2_str, (const char*)entry_str);
		TSK_SAFE_FREE2(entry_str);
	}
	
	/*externals*/
	tsk_list_foreach(item, list2->externals)
	{
		txc_rlist_external_t *external = ((txc_rlist_external_t*)item->data);
		char* external_str = txc_rlist_external_serialize(external);
		tsk_strcat(0, &list2_str, (const char*)external_str);
		TSK_FREE(external_str);
	}
	
	/* close list */
	tsk_strcat(0, &list2_str, "</list>");

	return list2_str;
}

/* will serialize a complete resource-lists whith xml header*/
/* ATTENTION: use 'TSK_SAFE_FREE2' macro to free the returned string */
char* txc_rlist_rlist2_serialize(const tsk_list_t *rlist2)
{
	tsk_list_item_t* item = 0;
	char* rlist2_str = 0;

	if(!rlist2) return 0;

	/* xml header */
	tsk_strcat(0, &rlist2_str, RLIST_XML_HEADER);

	tsk_list_foreach(item, rlist2)
	{
		/* get list2 */
		txc_rlist_list2_t *list2 = ((txc_rlist_list2_t*)item->data);
		char* list2_str = txc_rlist_list2_serialize(list2);
		tsk_strcat(0, &rlist2_str, list2_str);
		TSK_FREE(list2_str);
	}
	
	/* xml footer */
	tsk_strcat(0, &rlist2_str, RLIST_XML_FOOTER);

	return rlist2_str;
}

/* add xml header to the content */
/* ATTENTION: use 'TSK_SAFE_FREE2' macro to free the returned string */
char* txc_rlist_add_xml_header(const char* xml_content)
{
	char* xml = 0;

	/* append xml header */
	tsk_sprintf(0, &xml,
				"%s%s%s",
				RLIST_XML_HEADER, xml_content, RLIST_XML_FOOTER);
	return xml;
}

/* free resource-lists document */
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