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
#ifndef _TINYXCAP_XDM_RLIST_H_
#define _TINYXCAP_XDM_RLIST_H_

#include "tinyxcap_config.h"
#include "xdm_xml.h"

/* urn:ietf:params:xml:ns:resource-lists */

#define XDM_RLIST_ENTRY_CREATE(entry)			XDM_XXX_CREATE(entry, rlist_entry)
#define XDM_RLIST_ENTRY_SAFE_FREE(entry)	XDM_XXX_SAFE_FREE(entry, rlist_entry)

#define XDM_RLIST_LIST_CREATE(rlist_list)		XDM_XXX_CREATE(rlist_list, rlist_list)
#define XDM_RLIST_LIST_SAFE_FREE(rlist_list)	XDM_XXX_SAFE_FREE(rlist_list, rlist_list)

#define XDM_RLIST_LIST2_CREATE(list2)			XDM_XXX_CREATE(list2, rlist_list2)
#define XDM_RLIST_LIST2_SAFE_FREE(list2)		XDM_XXX_SAFE_FREE(list2, rlist_list2)

#define XDM_RLIST_EXTERNAL_CREATE(external)		XDM_XXX_CREATE(external, rlist_external)
#define XDM_RLIST_EXTERNAL_SAFE_FREE(external)	XDM_XXX_SAFE_FREE(external, rlist_external)

/* entry */
typedef struct xdm_rlist_entry_s
{
	char* uri;
	char* display_name;
	char* last_modified;
	char* etag;
	char* list;
}
xdm_rlist_entry_t;
typedef xdm_list_t xdm_rlist_entry_L_t; /* contains  xdm_rlist_entry_t elements */

/* list */
typedef struct xdm_rlist_list_s
{
	char* display_name;
	char* name;
}
xdm_rlist_list_t;
typedef xdm_list_t xdm_rlist_list_L_t; /* contains 'xdm_rlist_list_t' elements*/
typedef xdm_list_t xdm_rlist_listx_L_t; /* contains 'xdm_rlist_list_t' or 'xdm_rlist_list2_t' elements*/

/* external */
typedef struct xdm_rlist_external_s
{
	char* anchor;
	char* list;
}
xdm_rlist_external_t;
typedef xdm_list_t xdm_rlist_external_L_t; /* contains 'xdm_rlist_external_t' elements*/

/* list2 */
typedef struct xdm_rlist_list2_s
{
	char* display_name;
	char* name;
	xdm_rlist_entry_L_t*	entries;
	xdm_rlist_external_L_t* externals;
}
xdm_rlist_list2_t;
typedef xdm_list_t xdm_rlist_list2_L_t; /* contains 'xdm_rlist_list2_t' elements*/


/* resource-lists */
typedef struct xdm_rlist_s
{
	xmlDocPtr docPtr;
}
xdm_rlist_t;

void xdm_rlist_list_init(xdm_rlist_list_t *list);
void xdm_rlist_list_set(xdm_rlist_list_t *list, const char* name, const char* display_name);
void xdm_rlist_list_free(void **list);

void xdm_rlist_list2_init(xdm_rlist_list2_t *list2);
void xdm_rlist_list2_set(xdm_rlist_list2_t *list2, const char* name, const char* display_name);
void xdm_rlist_list2_add_external(xdm_rlist_list2_t *list2, const char* anchor);
void xdm_rlist_list2_add_entry(xdm_rlist_list2_t *list2, const char* uri, const char* display_name);
void xdm_rlist_list2_free(void **list2);

void xdm_rlist_entry_init(xdm_rlist_entry_t *entry);
void xdm_rlist_entry_set(xdm_rlist_entry_t *entry, const char* uri, const char* display_name);
void xdm_rlist_entry_free(void **entry);

void xdm_rlist_external_init(xdm_rlist_external_t *external);
void xdm_rlist_external_set(xdm_rlist_external_t *external, const char* anchor);
void xdm_rlist_external_free(void **external);

xdm_rlist_entry_t* xdm_rlist_entry_from_xml(const xmlNodePtr node, const char* lname);
xdm_rlist_external_t* xdm_rlist_external_from_xml(const xmlNodePtr node, const char* lname);
xdm_rlist_list_t* xdm_rlist_list_from_xml(const xmlNodePtr node);
xdm_rlist_list2_t* xdm_rlist_list2_from_xml(const xmlNodePtr node);

TINYXCAP_API xdm_rlist_t* xdm_rlist_create(const char* buffer, size_t size);
TINYXCAP_API xdm_rlist_listx_L_t* xdm_rlist_get_all_lists(const xdm_rlist_t* rlist, int type2);
TINYXCAP_API xdm_rlist_entry_L_t* xdm_rlist_get_entries_by_list(const xdm_rlist_t* rlist, const char* lname);
TINYXCAP_API xdm_rlist_external_L_t* xdm_rlist_get_externals_by_list(const xdm_rlist_t* rlist, const char* lname);
TINYXCAP_API char* xdm_rlist_entry_serialize(const xdm_rlist_entry_t *entry);
TINYXCAP_API char* xdm_rlist_entry_serialize2(const char* uri, const char* displayname);
TINYXCAP_API char* xdm_rlist_external_serialize(const xdm_rlist_external_t *external);
TINYXCAP_API char* xdm_rlist_list_serialize(const xdm_rlist_list_t *list);
TINYXCAP_API char* xdm_rlist_list_serialize2(const char* name, const char* displayname);
TINYXCAP_API char* xdm_rlist_list2_serialize(const xdm_rlist_list2_t *list2);
TINYXCAP_API char* xdm_rlist_rlist2_serialize(const xdm_list_t *rlist2);
TINYXCAP_API char* xdm_rlist_add_xml_header(const char* xml_content);
TINYXCAP_API void xdm_rlist_free(xdm_rlist_t **rlist);

#endif /* _TINYXCAP_XDM_RLIST_H_ */