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
#ifndef _TINYXCAP_TXC_RLIST_H_
#define _TINYXCAP_TXC_RLIST_H_

#include "tinyxcap_config.h"
#include "tsk_xml.h"

/* urn:ietf:params:xml:ns:resource-lists */

#define TXC_RLIST_ENTRY_CREATE(entry)			TXC_XXX_CREATE2(entry, rlist_entry)
#define TXC_RLIST_ENTRY_SAFE_FREE(entry)	TXC_XXX_SAFE_FREE2(entry, rlist_entry)

#define TXC_RLIST_LIST_CREATE(rlist_list)		TXC_XXX_CREATE2(rlist_list, rlist_list)
#define TXC_RLIST_LIST_SAFE_FREE(rlist_list)	TXC_XXX_SAFE_FREE2(rlist_list, rlist_list)

#define TXC_RLIST_LIST2_CREATE(list2)			TXC_XXX_CREATE2(list2, rlist_list2)
#define TXC_RLIST_LIST2_SAFE_FREE(list2)		TXC_XXX_SAFE_FREE2(list2, rlist_list2)

#define TXC_RLIST_EXTERNAL_CREATE(external)		TXC_XXX_CREATE2(external, rlist_external)
#define TXC_RLIST_EXTERNAL_SAFE_FREE(external)	TXC_XXX_SAFE_FREE2(external, rlist_external)

/* entry */
typedef struct txc_rlist_entry_s
{
	char* uri;
	char* display_name;
	char* last_modified;
	char* etag;
	char* list;
}
txc_rlist_entry_t;
typedef tsk_list_t txc_rlist_entry_L_t; /* contains  txc_rlist_entry_t elements */

/* list */
typedef struct txc_rlist_list_s
{
	char* display_name;
	char* name;
}
txc_rlist_list_t;
typedef tsk_list_t txc_rlist_list_L_t; /* contains 'txc_rlist_list_t' elements*/
typedef tsk_list_t txc_rlist_listx_L_t; /* contains 'txc_rlist_list_t' or 'txc_rlist_list2_t' elements*/

/* external */
typedef struct txc_rlist_external_s
{
	char* anchor;
	char* list;
}
txc_rlist_external_t;
typedef tsk_list_t txc_rlist_external_L_t; /* contains 'txc_rlist_external_t' elements*/

/* list2 */
typedef struct txc_rlist_list2_s
{
	char* display_name;
	char* name;
	txc_rlist_entry_L_t*	entries;
	txc_rlist_external_L_t* externals;
}
txc_rlist_list2_t;
typedef tsk_list_t txc_rlist_list2_L_t; /* contains 'txc_rlist_list2_t' elements*/


/* resource-lists */
typedef struct txc_rlist_s
{
	xmlDocPtr docPtr;
}
txc_rlist_t;

void txc_rlist_list_init(txc_rlist_list_t *list);
void txc_rlist_list_set(txc_rlist_list_t *list, const char* name, const char* display_name);
void txc_rlist_list_free(void **list);

void txc_rlist_list2_init(txc_rlist_list2_t *list2);
void txc_rlist_list2_set(txc_rlist_list2_t *list2, const char* name, const char* display_name);
void txc_rlist_list2_add_external(txc_rlist_list2_t *list2, const char* anchor);
void txc_rlist_list2_add_entry(txc_rlist_list2_t *list2, const char* uri, const char* display_name);
void txc_rlist_list2_free(void **list2);

void txc_rlist_entry_init(txc_rlist_entry_t *entry);
void txc_rlist_entry_set(txc_rlist_entry_t *entry, const char* uri, const char* display_name);
void txc_rlist_entry_free(void **entry);

void txc_rlist_external_init(txc_rlist_external_t *external);
void txc_rlist_external_set(txc_rlist_external_t *external, const char* anchor);
void txc_rlist_external_free(void **external);

txc_rlist_entry_t* txc_rlist_entry_from_xml(const xmlNodePtr node, const char* lname);
txc_rlist_external_t* txc_rlist_external_from_xml(const xmlNodePtr node, const char* lname);
txc_rlist_list_t* txc_rlist_list_from_xml(const xmlNodePtr node);
txc_rlist_list2_t* txc_rlist_list2_from_xml(const xmlNodePtr node);

TINYXCAP_API txc_rlist_t* txc_rlist_create(const char* buffer, size_t size);
TINYXCAP_API txc_rlist_listx_L_t* txc_rlist_get_all_lists(const txc_rlist_t* rlist, int type2);
TINYXCAP_API txc_rlist_entry_L_t* txc_rlist_get_entries_by_list(const txc_rlist_t* rlist, const char* lname);
TINYXCAP_API txc_rlist_external_L_t* txc_rlist_get_externals_by_list(const txc_rlist_t* rlist, const char* lname);
TINYXCAP_API char* txc_rlist_entry_serialize(const txc_rlist_entry_t *entry);
TINYXCAP_API char* txc_rlist_entry_serialize2(const char* uri, const char* displayname);
TINYXCAP_API char* txc_rlist_external_serialize(const txc_rlist_external_t *external);
TINYXCAP_API char* txc_rlist_list_serialize(const txc_rlist_list_t *list);
TINYXCAP_API char* txc_rlist_list_serialize2(const char* name, const char* displayname);
TINYXCAP_API char* txc_rlist_list2_serialize(const txc_rlist_list2_t *list2);
TINYXCAP_API char* txc_rlist_rlist2_serialize(const tsk_list_t *rlist2);
TINYXCAP_API char* txc_rlist_add_xml_header(const char* xml_content);
TINYXCAP_API void txc_rlist_free(txc_rlist_t **rlist);

#endif /* _TINYXCAP_TXC_RLIST_H_ */