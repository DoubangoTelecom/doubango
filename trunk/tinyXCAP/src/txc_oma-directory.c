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
#include "txc_oma-directory.h"
#include "txc_macros.h"

#include "tsk_macros.h"

//static const char* txc_oma_directory_ns = "urn:oma:xml:xdm:xcap-directory";

#define OMADIR_RETURN_IF_INVALID(omadir) if(!omadir || !(omadir->docPtr)) return 0;

#define OMADIR_SELECT_FOLDER_BY_AUID(fo_auid)\
	tsk_xml_select_node(omadir->docPtr->children, \
	TSK_XML_NODE_SELECT_BY_NAME("xcap-directory"), \
	TSK_XML_NODE_SELECT_BY_ATT("folder", "auid", fo_auid), \
	TSK_XML_NODE_SELECT_CONTENT(), \
	TSK_XML_NODE_SELECT_END())


/* init folder */
void txc_omadir_folder_init(txc_omadir_folder_t *folder)
{
	memset(folder, 0, sizeof(txc_omadir_folder_t));
}

/* free folder */
void txc_omadir_folder_free(void **_folder)
{
	txc_omadir_folder_t **folder = ((txc_omadir_folder_t**)_folder);

	TSK_SAFE_FREE2((*folder)->auid);

	free(*_folder);
	(*_folder) = 0;
}

/* xml<->folder binding*/
/* ATTENTION: use 'TXC_OMADIR_FOLDER_SAFE_FREE' function to free the returned object */
txc_omadir_folder_t* txc_omadir_folder_from_xml(xmlNodePtr node)
{
	xmlNodePtr node2 = 0;
	txc_omadir_folder_t *folder = 0;

	if(node = tsk_xml_find_node(node, "folder", nft_none))
	{
		TXC_OMADIR_FOLDER_CREATE(folder);
		
		/* auid */
		node2 = tsk_xml_select_node(node, 
			TSK_XML_NODE_SELECT_ATT_VALUE("folder", "auid"), TSK_XML_NODE_SELECT_END());
		folder->auid = tsk_strdup(0, TSK_XML_NODE_SAFE_GET_TEXTVALUE(node2));
	}
	else return 0;

	return folder;
}

/* create oma xcap-directory context */
txc_omadir_t* txc_omadir_create(const char* buffer, size_t size)
{
	if(buffer && size)
	{
		txc_omadir_t* omadir = (txc_omadir_t*)malloc(sizeof(txc_omadir_t));
		memset(omadir, 0, sizeof(txc_omadir_t));
		omadir->docPtr = xmlParseMemory(buffer, (int)size);

		return omadir;
	}

	return 0;
}

/* get all folders in the oma xcap-directory document */
/* ATTENTION: use 'TSK_LIST_SAFE_FREE' function to free the returned object */
txc_omadir_folder_L_t* txc_omadir_get_all_folders(const txc_omadir_t* omadir)
{
	txc_omadir_folder_t* folder = 0;
	txc_omadir_folder_L_t* list = 0;
	xmlNodePtr node = 0;

	OMADIR_RETURN_IF_INVALID(omadir);
	
	/* root */
	node = tsk_xml_select_node(omadir->docPtr->children,
		TSK_XML_NODE_SELECT_BY_NAME("xcap-directory"),
		TSK_XML_NODE_SELECT_END());

	/* select first folder */
	if(!tsk_xml_find_node(node, "folder", nft_none)) node = tsk_xml_find_node(node, "folder", nft_next);
	if(node)
	{
		TSK_LIST_CREATE(list);
		do
		{
			folder = txc_omadir_folder_from_xml(node);
			tsk_list_add_data(list, ((void**) &folder), txc_omadir_folder_free);
		}
		while(node = tsk_xml_find_node(node, "folder", nft_next));
	}

	return list;
}

/* get all entries in the folder */
/* ATTENTION: use 'TSK_LIST_SAFE_FREE' function to free the returned object */
txc_rlist_entry_L_t* txc_omadir_get_entries_by_folder(const txc_omadir_t* omadir, const char* fo_auid)
{
	txc_rlist_entry_t *rlist_entry = 0;
	tsk_list_t* list = 0;
	xmlNodePtr node = 0;

	OMADIR_RETURN_IF_INVALID(omadir);

	node = OMADIR_SELECT_FOLDER_BY_AUID(fo_auid);
	if(!node) return 0;

	/* select first entry */
	if(!tsk_xml_find_node(node, "entry", nft_none)) node = tsk_xml_find_node(node, "entry", nft_next);
	if(node)
	{
		TSK_LIST_CREATE(list);
		do
		{
			rlist_entry = txc_rlist_entry_from_xml(node, fo_auid);
			tsk_list_add_data(list, ((void**) &rlist_entry), txc_rlist_entry_free);
		}
		while(node = tsk_xml_find_node(node, "entry", nft_next));
	}

	return list;
}

/* free oma dir context */
void txc_omadir_free(txc_omadir_t **omadir)
{
	if(*omadir)
	{	
		xmlFreeDoc((*omadir)->docPtr);
		
		free(*omadir);
		(*omadir)=0;
	}
}

#undef OMADIR_RETURN_IF_INVALID