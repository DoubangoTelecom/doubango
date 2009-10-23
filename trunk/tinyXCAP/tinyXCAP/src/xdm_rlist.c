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
RFC 4826 - Extensible Markup Language (XML) Formats for Representing Resource Lists

	resource-lists
		->list-1(name)
			->entry-1(uri, display-name)
			->entry-2(uri, display-name)
		->list-2(name)
		->list-3(name)
*/
#include "xdm_rlist.h"
#include "xdm_utils.h"

#define RLIST_SELECT_LIST_BY_NAME(lname)\
	xdm_xml_select_node(rlist->docPtr->children, \
	XDM_XML_NODE_SELECT_BY_NAME("resource-lists"), \
	XDM_XML_NODE_SELECT_BY_ATT("list", "name", lname), \
	XDM_XML_NODE_SELECT_CONTENT(), \
	XDM_XML_NODE_SELECT_END())

#define RLIST_XML_HEADER		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>" \
								"<resource-lists xmlns=\"urn:ietf:params:xml:ns:resource-lists\" " \
								"xmlns:xd=\"urn:oma:xml:xdm:xcap-directory\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
#define RLIST_XML_FOOTER		"</resource-lists>"

#define RLIST_RETURN_IF_INVALID(rlist) if(!rlist || !(rlist->docPtr)) return 0;

//static const char* xdm_rlist_ns = "urn:ietf:params:xml:ns:resource-lists";

/* init entry*/
void xdm_rlist_entry_init(xdm_rlist_entry_t *entry)
{
	memset(entry, 0, sizeof(xdm_rlist_entry_t));
}

/* set entry's uri and display-name */
void xdm_rlist_entry_set(xdm_rlist_entry_t *entry, const char* uri, const char* display_name)
{
	if(entry)
	{		
		xdm_strupdate(&(entry->uri), uri);
		xdm_strupdate(&(entry->display_name), display_name);
	}
}

/* free entry */
void xdm_rlist_entry_free(void **_entry)
{
	xdm_rlist_entry_t **entry = ((xdm_rlist_entry_t**)_entry);

	XDM_SAFE_FREE((*entry)->uri);
	XDM_SAFE_FREE((*entry)->display_name);
	XDM_SAFE_FREE((*entry)->list);
	XDM_SAFE_FREE((*entry)->last_modified);
	XDM_SAFE_FREE((*entry)->etag);

	free(*_entry);
	(*_entry) = 0;
}

/* init list */
void xdm_rlist_list_init(xdm_rlist_list_t *list)
{
	memset(list, 0, sizeof(xdm_rlist_list_t));
}

/* set list's name and display-name */
void xdm_rlist_list_set(xdm_rlist_list_t *list, const char* name, const char* display_name)
{
	if(list)
	{		
		xdm_strupdate(&(list->name), name);
		xdm_strupdate(&(list->display_name), display_name);
	}
}

/* free list */
void xdm_rlist_list_free(void **_list)
{
	xdm_rlist_list_t **list = ((xdm_rlist_list_t**)_list);

	XDM_SAFE_FREE((*list)->display_name);
	XDM_SAFE_FREE((*list)->name);

	free(*_list);
	(*_list) = 0;
}

/* init list2 */
void xdm_rlist_list2_init(xdm_rlist_list2_t *list2)
{
	memset(list2, 0, sizeof(xdm_rlist_list2_t));
}

/* set list2's name and display-name */
void xdm_rlist_list2_set(xdm_rlist_list2_t *list2, const char* name, const char* display_name)
{
	if(list2)
	{		
		xdm_strupdate(&(list2->name), name);
		xdm_strupdate(&(list2->display_name),display_name);
	}
}
/* add external eement to the list2 */
void xdm_rlist_list2_add_external(xdm_rlist_list2_t *list2, const char* anchor)
{
	if(list2)
	{
		xdm_rlist_external_t *external = 0;
		if(!(list2->externals)) 
		{
			XDM_LIST_CREATE(list2->externals);
		}

		XDM_RLIST_EXTERNAL_CREATE(external);
		xdm_rlist_external_set(external, anchor);
		xdm_list_add_data(list2->externals, ((void**)&external), xdm_rlist_external_free);
	}
}

/* and an entry to the list */
void xdm_rlist_list2_add_entry(xdm_rlist_list2_t *list2, const char* uri, const char* display_name)
{
	if(list2)
	{
		xdm_rlist_entry_t *entry = 0;
		XDM_RLIST_ENTRY_CREATE(entry);
		xdm_rlist_entry_set(entry, uri, display_name);

		if(!(list2->entries)) 
		{
			XDM_LIST_CREATE(list2->entries);
		}
		xdm_list_add_data(list2->entries, ((void**)&entry), xdm_rlist_entry_free);
	}
}

/* free list2 */
void xdm_rlist_list2_free(void **_list2)
{
	xdm_rlist_list2_t **list2 = ((xdm_rlist_list2_t**)_list2);

	XDM_SAFE_FREE((*list2)->display_name);
	XDM_SAFE_FREE((*list2)->name);
	XDM_LIST_SAFE_FREE((*list2)->externals);
	XDM_LIST_SAFE_FREE((*list2)->entries);

	free(*_list2);
	(*_list2) = 0;
}

/* init external */
void xdm_rlist_external_init(xdm_rlist_external_t *external)
{
	memset(external, 0, sizeof(xdm_rlist_external_t));
}

/* set external's anchor */
void xdm_rlist_external_set(xdm_rlist_external_t *external, const char* anchor)
{
	if(external)
	{
		xdm_strupdate(&(external->anchor), anchor);
	}
}

/* free external */
void xdm_rlist_external_free(void **_external)
{
	xdm_rlist_external_t **external = ((xdm_rlist_external_t**)_external);

	XDM_SAFE_FREE((*external)->anchor);
	XDM_SAFE_FREE((*external)->list);

	free(*_external);
	(*_external) = 0;
}

/* xml<->entry binding*/
/* ATTENTION: use 'XDM_RLIST_ENTRY_SAFE_FREE' function to free the returned object */
xdm_rlist_entry_t* xdm_rlist_entry_from_xml(const xmlNodePtr node, const char* lname)
{
	xmlNodePtr node2 = 0;
	xdm_rlist_entry_t *rlist_entry = 0;

	if(xdm_xml_find_node(node, "entry", nft_none))
	{
		XDM_RLIST_ENTRY_CREATE(rlist_entry);
		rlist_entry->list = xdm_strdup(lname);

		/* display-name */
		node2 = xdm_xml_select_node(node, XDM_XML_NODE_SELECT_BY_NAME("entry"), 
			XDM_XML_NODE_SELECT_BY_NAME("display-name"), XDM_XML_NODE_SELECT_END());
		rlist_entry->display_name = xdm_strdup(XDM_XML_NODE_SAFE_GET_TEXTVALUE(node2));
		
		/* uri */
		node2 = xdm_xml_select_node(node, 
			XDM_XML_NODE_SELECT_ATT_VALUE("entry", "uri"), XDM_XML_NODE_SELECT_END());
		rlist_entry->uri = xdm_strdup(XDM_XML_NODE_SAFE_GET_TEXTVALUE(node2));

		/* last-modified */
		node2 = xdm_xml_select_node(node, 
			XDM_XML_NODE_SELECT_ATT_VALUE("entry", "last-modified"), XDM_XML_NODE_SELECT_END());
		rlist_entry->last_modified = xdm_strdup(XDM_XML_NODE_SAFE_GET_TEXTVALUE(node2));

		/* etag */
		node2 = xdm_xml_select_node(node, 
			XDM_XML_NODE_SELECT_ATT_VALUE("entry", "etag"), XDM_XML_NODE_SELECT_END());
		rlist_entry->etag = xdm_strdup(XDM_XML_NODE_SAFE_GET_TEXTVALUE(node2));
		
	}
	else return 0;

	return rlist_entry;
}

/* xml<->entry binding*/
/* ATTENTION: use 'XDM_RLIST_EXTERNAL_SAFE_FREE' function to free the returned object */
xdm_rlist_external_t* xdm_rlist_external_from_xml(const xmlNodePtr node, const char* lname)
{
	xmlNodePtr node2 = 0;
	xdm_rlist_external_t *rlist_external = 0;

	if(xdm_xml_find_node(node, "external", nft_none))
	{
		XDM_RLIST_EXTERNAL_CREATE(rlist_external);
		rlist_external->list = xdm_strdup(lname);
		
		/* anchor */
		node2 = xdm_xml_select_node(node, 
			XDM_XML_NODE_SELECT_ATT_VALUE("external", "anchor"),
			XDM_XML_NODE_SELECT_END());
		rlist_external->anchor = xdm_strdup(XDM_XML_NODE_SAFE_GET_TEXTVALUE(node2));
		
	}else return 0;

	return rlist_external;
}

/* xml<->list binding*/
/* ATTENTION: use 'XDM_RLIST_LIST_SAFE_FREE' function to free the returned object */
xdm_rlist_list_t* xdm_rlist_list_from_xml(const xmlNodePtr node)
{
	xmlNodePtr node2 = 0;
	xdm_rlist_list_t *rlist_list = 0;

	if(xdm_xml_find_node(node, "list", nft_none))
	{
		XDM_RLIST_LIST_CREATE(rlist_list);
		
		/* name */
		node2 = xdm_xml_select_node(node, 
			XDM_XML_NODE_SELECT_ATT_VALUE("list", "name"),
			XDM_XML_NODE_SELECT_END());
		rlist_list->name = xdm_strdup(XDM_XML_NODE_SAFE_GET_TEXTVALUE(node2));

		/* display-name */
		node2 = xdm_xml_select_node(node, XDM_XML_NODE_SELECT_BY_NAME("list"),
			XDM_XML_NODE_SELECT_BY_NAME("display-name"),
			XDM_XML_NODE_SELECT_END());
		rlist_list->display_name = xdm_strdup(XDM_XML_NODE_SAFE_GET_TEXTVALUE(node2));
		
	}else return 0;

	return rlist_list;
}

/* xml<->list binding*/
/* ATTENTION: use 'XDM_RLIST_LIST2_SAFE_FREE' function to free the returned object */
xdm_rlist_list2_t* xdm_rlist_list2_from_xml(const xmlNodePtr node)
{
	xmlNodePtr node2 = 0;
	xdm_rlist_list2_t *rlist_list2 = 0;

	if(xdm_xml_find_node(node, "list", nft_none))
	{
		XDM_RLIST_LIST_CREATE(rlist_list2);
		
		/** name **/
		node2 = xdm_xml_select_node(node, 
			XDM_XML_NODE_SELECT_ATT_VALUE("list", "name"),
			XDM_XML_NODE_SELECT_END());
		rlist_list2->name = xdm_strdup(XDM_XML_NODE_SAFE_GET_TEXTVALUE(node2));

		/** display-name **/
		node2 = xdm_xml_select_node(node, XDM_XML_NODE_SELECT_BY_NAME("list"),
			XDM_XML_NODE_SELECT_BY_NAME("display-name"),
			XDM_XML_NODE_SELECT_END());
		rlist_list2->display_name = xdm_strdup(XDM_XML_NODE_SAFE_GET_TEXTVALUE(node2));

		/** externals **/
		/* select first external */
		if(! (node2 = xdm_xml_find_node(node->children, "external", nft_none)) ) node2 = xdm_xml_find_node(node->children, "external", nft_next);
		if(node2)
		{
			xdm_rlist_external_t *rlist_external = 0;
			XDM_LIST_CREATE(rlist_list2->externals);
			do
			{
				rlist_external = xdm_rlist_external_from_xml(node2, rlist_list2->name);
				xdm_list_add_data(rlist_list2->externals, ((void**) &rlist_external), xdm_rlist_external_free);
			}
			while(node2 = xdm_xml_find_node(node2, "external", nft_next));
		}
		
		/** entries **/
		/* select first entry */
		if(!(node2 = xdm_xml_find_node(node->children, "entry", nft_none))) node2 = xdm_xml_find_node(node->children, "entry", nft_next);
		if(node2)
		{
			xdm_rlist_entry_t *rlist_entry = 0;
			XDM_LIST_CREATE(rlist_list2->entries);
			do
			{
				rlist_entry = xdm_rlist_entry_from_xml(node2, rlist_list2->name);
				xdm_list_add_data(rlist_list2->entries, ((void**) &rlist_entry), xdm_rlist_entry_free);
			}
			while(node2 = xdm_xml_find_node(node2, "entry", nft_next));
		}
	}else return 0;

	return rlist_list2;
}

/* create resource-lists document */
xdm_rlist_t* xdm_rlist_create(const char* buffer, size_t size)
{
	if(buffer && size)
	{
		xdm_rlist_t* rlist = (xdm_rlist_t*)malloc(sizeof(xdm_rlist_t));
		memset(rlist, 0, sizeof(xdm_rlist_t));
		rlist->docPtr = xmlParseMemory(buffer, (int)size);

		return rlist;
	}

	return 0;
}

/* get all lists(xdm_rlist_list_t or xdm_rlist_list2_t) */
/* ATTENTION: use 'XDM_LIST_SAFE_FREE' function to free the returned object */
xdm_rlist_listx_L_t* xdm_rlist_get_all_lists(const xdm_rlist_t* rlist, int type2)
{
	xdm_rlist_listx_L_t* list = 0;
	xmlNodePtr node = 0;

	RLIST_RETURN_IF_INVALID(rlist);
	
	/* root */
	node = xdm_xml_select_node(rlist->docPtr->children,
		XDM_XML_NODE_SELECT_BY_NAME("resource-lists"),
		XDM_XML_NODE_SELECT_END());

	/* select first list */
	if(!xdm_xml_find_node(node, "list", nft_none)) node = xdm_xml_find_node(node, "list", nft_next);
	if(node)
	{
		XDM_LIST_CREATE(list);
		do
		{
			if(type2)
			{
				xdm_rlist_list2_t* rlist_list2 = xdm_rlist_list2_from_xml(node);
				xdm_list_add_data(list, ((void**) &rlist_list2), xdm_rlist_list2_free);
			}
			else
			{
				xdm_rlist_list_t* rlist_list = xdm_rlist_list_from_xml(node);
				xdm_list_add_data(list, ((void**) &rlist_list), xdm_rlist_list_free);
			}
		}
		while(node = xdm_xml_find_node(node, "list", nft_next));
	}

	return list;
}

/* returns all entries into the list named @lname */
/* ATTENTION: use 'XDM_LIST_SAFE_FREE' function to free the returned object */
xdm_rlist_entry_L_t* xdm_rlist_get_entries_by_list(const xdm_rlist_t* rlist, const char* lname)
{
	xdm_rlist_entry_t *rlist_entry = 0;
	xdm_rlist_entry_L_t* list = 0;
	xmlNodePtr node = 0;

	RLIST_RETURN_IF_INVALID(rlist);

	node = RLIST_SELECT_LIST_BY_NAME(lname);
	if(!node) return 0;

	XDM_LIST_CREATE(list);

	/* select first entry */
	if(!xdm_xml_find_node(node, "entry", nft_none)) node = xdm_xml_find_node(node, "entry", nft_next);
	if(node)
	{
		do
		{
			rlist_entry = xdm_rlist_entry_from_xml(node, lname);
			xdm_list_add_data(list, ((void**) &rlist_entry), xdm_rlist_entry_free);
		}
		while(node = xdm_xml_find_node(node, "entry", nft_next));
	}

	return list;
}

/* returns a list of externals by list name (xdm_rlist_external_t) */
/* ATTENTION: use 'XDM_LIST_SAFE_FREE' function to free the returned object */
xdm_rlist_external_L_t* xdm_rlist_get_externals_by_list(const xdm_rlist_t* rlist, const char* lname)
{
	xdm_rlist_external_t *rlist_external = 0;
	xdm_rlist_external_L_t* list = 0;
	xmlNodePtr node = 0;

	RLIST_RETURN_IF_INVALID(rlist);

	node = RLIST_SELECT_LIST_BY_NAME(lname);
	if(!node) return 0;
	
	/* select first external */
	if(!xdm_xml_find_node(node, "external", nft_none)) node = xdm_xml_find_node(node, "external", nft_next);
	if(node)
	{
		XDM_LIST_CREATE(list);
		do
		{
			rlist_external = xdm_rlist_external_from_xml(node, lname);
			xdm_list_add_data(list, ((void**) &rlist_external), xdm_rlist_external_free);
		}
		while(node = xdm_xml_find_node(node, "external", nft_next));
	}

	return list;
}

/* serialize an external */
/* ATTENTION: use 'XDM_SAFE_FREE' macro to free the returned string */
char* xdm_rlist_external_serialize(const xdm_rlist_external_t *external)
{
	char* external_str = 0;
	if(!external) return 0;

	xdm_sprintf(&external_str, "<external anchor=\"%s\" />", external->anchor);
	return external_str;
}

/* serialize an entry */
/* ATTENTION: use 'XDM_SAFE_FREE' macro to free the returned string */
char* xdm_rlist_entry_serialize(const xdm_rlist_entry_t *entry)
{
	return entry ? xdm_rlist_entry_serialize2(entry->uri, entry->display_name) : 0;
}

/* serialize an entry */
/* ATTENTION: use 'XDM_SAFE_FREE' macro to free the returned string */
char* xdm_rlist_entry_serialize2(const char* uri, const char* displayname)
{
	char* entry_str = 0;
	if(!uri || !displayname) return 0;

	xdm_sprintf(&entry_str,
				"<entry uri=\"%s\" xmlns=\"urn:ietf:params:xml:ns:resource-lists\">"
					"<display-name>%s</display-name>"
				"</entry>",
				uri, displayname);
	return entry_str;
}

/* serialize list */
/* ATTENTION: use 'XDM_SAFE_FREE' macro to free the returned string */
char* xdm_rlist_list_serialize(const xdm_rlist_list_t *list)
{
	return list ? xdm_rlist_list_serialize2(list->name, list->display_name) : 0;
}

/* serialize list */
/* ATTENTION: use 'XDM_SAFE_FREE' macro to free the returned string */
char* xdm_rlist_list_serialize2(const char* name, const char* displayname)
{
	char* list_str = 0;

	if(!name || !displayname) return 0;
	
	/* serialize */
	 xdm_sprintf(&list_str,
				"<list name=\"%s\" xmlns=\"urn:ietf:params:xml:ns:resource-lists\">"
					"<display-name>%s</display-name>"
				"</list>",
				name, displayname);
	return list_str;
}

/* serialize list2 */
/* ATTENTION: use 'XDM_SAFE_FREE' macro to free the returned string */
char* xdm_rlist_list2_serialize(const xdm_rlist_list2_t *list2)
{
	char* list2_str = 0;
	xdm_list_item_t *item = 0;

	/* check validity */
	if(!list2) return 0;

	/* name and display-name */
	xdm_sprintf(&list2_str,
				"<list name=\"%s\" xmlns=\"urn:ietf:params:xml:ns:resource-lists\">"
					"<display-name>%s</display-name>",
				list2->name, list2->display_name);
	
	/* entries */
	xdm_list_foreach(item, list2->entries)
	{
		xdm_rlist_entry_t *entry = ((xdm_rlist_entry_t*)item->data);
		char* entry_str = xdm_rlist_entry_serialize(entry);
		xdm_strcat(&list2_str, (const char*)entry_str);
		XDM_FREE(entry_str);
	}
	
	/*externals*/
	xdm_list_foreach(item, list2->externals)
	{
		xdm_rlist_external_t *external = ((xdm_rlist_external_t*)item->data);
		char* external_str = xdm_rlist_external_serialize(external);
		xdm_strcat(&list2_str, (const char*)external_str);
		XDM_FREE(external_str);
	}
	
	/* close list */
	xdm_strcat(&list2_str, "</list>");

	return list2_str;
}

/* will serialize a complete resource-lists whith xml header*/
/* ATTENTION: use 'XDM_SAFE_FREE' macro to free the returned string */
char* xdm_rlist_rlist2_serialize(const xdm_list_t *rlist2)
{
	xdm_list_item_t* item = 0;
	char* rlist2_str = 0;

	if(!rlist2) return 0;

	/* xml header */
	xdm_strcat(&rlist2_str, RLIST_XML_HEADER);

	xdm_list_foreach(item, rlist2)
	{
		/* get list2 */
		xdm_rlist_list2_t *list2 = ((xdm_rlist_list2_t*)item->data);
		char* list2_str = xdm_rlist_list2_serialize(list2);
		xdm_strcat(&rlist2_str, list2_str);
		XDM_FREE(list2_str);
	}
	
	/* xml footer */
	xdm_strcat(&rlist2_str, RLIST_XML_FOOTER);

	return rlist2_str;
}

/* add xml header to the content */
/* ATTENTION: use 'XDM_SAFE_FREE' macro to free the returned string */
char* xdm_rlist_add_xml_header(const char* xml_content)
{
	char* xml = 0;

	/* append xml header */
	xdm_sprintf(&xml,
				"%s%s%s",
				RLIST_XML_HEADER, xml_content, RLIST_XML_FOOTER);
	return xml;
}

/* free resource-lists document */
void xdm_rlist_free(xdm_rlist_t **rlist )
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