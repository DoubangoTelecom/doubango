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
#include "xdm_oma-directory.h"
#include "xdm_utils.h"

//static const char* xdm_oma_directory_ns = "urn:oma:xml:xdm:xcap-directory";

#define OMADIR_RETURN_IF_INVALID(omadir) if(!omadir || !(omadir->docPtr)) return 0;

#define OMADIR_SELECT_FOLDER_BY_AUID(fo_auid)\
	xdm_xml_select_node(omadir->docPtr->children, \
	XDM_XML_NODE_SELECT_BY_NAME("xcap-directory"), \
	XDM_XML_NODE_SELECT_BY_ATT("folder", "auid", fo_auid), \
	XDM_XML_NODE_SELECT_CONTENT(), \
	XDM_XML_NODE_SELECT_END())


/* init folder */
void xdm_omadir_folder_init(xdm_omadir_folder_t *folder)
{
	memset(folder, 0, sizeof(xdm_omadir_folder_t));
}

/* free folder */
void xdm_omadir_folder_free(void **_folder)
{
	xdm_omadir_folder_t **folder = ((xdm_omadir_folder_t**)_folder);

	XDM_SAFE_FREE((*folder)->auid);

	free(*_folder);
	(*_folder) = 0;
}

/* xml<->folder binding*/
/* ATTENTION: use 'XDM_OMADIR_FOLDER_SAFE_FREE' function to free the returned object */
xdm_omadir_folder_t* xdm_omadir_folder_from_xml(xmlNodePtr node)
{
	xmlNodePtr node2 = 0;
	xdm_omadir_folder_t *folder = 0;

	if(node = xdm_xml_find_node(node, "folder", nft_none))
	{
		XDM_OMADIR_FOLDER_CREATE(folder);
		
		/* auid */
		node2 = xdm_xml_select_node(node, 
			XDM_XML_NODE_SELECT_ATT_VALUE("folder", "auid"), XDM_XML_NODE_SELECT_END());
		folder->auid = xdm_strdup(XDM_XML_NODE_SAFE_GET_TEXTVALUE(node2));
	}
	else return 0;

	return folder;
}

/* create oma xcap-directory context */
xdm_omadir_t* xdm_omadir_create(const char* buffer, size_t size)
{
	if(buffer && size)
	{
		xdm_omadir_t* omadir = (xdm_omadir_t*)malloc(sizeof(xdm_omadir_t));
		memset(omadir, 0, sizeof(xdm_omadir_t));
		omadir->docPtr = xmlParseMemory(buffer, (int)size);

		return omadir;
	}

	return 0;
}

/* get all folders in the oma xcap-directory document */
/* ATTENTION: use 'XDM_LIST_SAFE_FREE' function to free the returned object */
xdm_omadir_folder_L_t* xdm_omadir_get_all_folders(const xdm_omadir_t* omadir)
{
	xdm_omadir_folder_t* folder = 0;
	xdm_omadir_folder_L_t* list = 0;
	xmlNodePtr node = 0;

	OMADIR_RETURN_IF_INVALID(omadir);
	
	/* root */
	node = xdm_xml_select_node(omadir->docPtr->children,
		XDM_XML_NODE_SELECT_BY_NAME("xcap-directory"),
		XDM_XML_NODE_SELECT_END());

	/* select first folder */
	if(!xdm_xml_find_node(node, "folder", nft_none)) node = xdm_xml_find_node(node, "folder", nft_next);
	if(node)
	{
		XDM_LIST_CREATE(list);
		do
		{
			folder = xdm_omadir_folder_from_xml(node);
			xdm_list_add_data(list, ((void**) &folder), xdm_omadir_folder_free);
		}
		while(node = xdm_xml_find_node(node, "folder", nft_next));
	}

	return list;
}

/* get all entries in the folder */
/* ATTENTION: use 'XDM_LIST_SAFE_FREE' function to free the returned object */
xdm_rlist_entry_L_t* xdm_omadir_get_entries_by_folder(const xdm_omadir_t* omadir, const char* fo_auid)
{
	xdm_rlist_entry_t *rlist_entry = 0;
	xdm_list_t* list = 0;
	xmlNodePtr node = 0;

	OMADIR_RETURN_IF_INVALID(omadir);

	node = OMADIR_SELECT_FOLDER_BY_AUID(fo_auid);
	if(!node) return 0;

	/* select first entry */
	if(!xdm_xml_find_node(node, "entry", nft_none)) node = xdm_xml_find_node(node, "entry", nft_next);
	if(node)
	{
		XDM_LIST_CREATE(list);
		do
		{
			rlist_entry = xdm_rlist_entry_from_xml(node, fo_auid);
			xdm_list_add_data(list, ((void**) &rlist_entry), xdm_rlist_entry_free);
		}
		while(node = xdm_xml_find_node(node, "entry", nft_next));
	}

	return list;
}

/* free oma dir context */
void xdm_omadir_free(xdm_omadir_t **omadir)
{
	if(*omadir)
	{	
		xmlFreeDoc((*omadir)->docPtr);
		
		free(*omadir);
		(*omadir)=0;
	}
}

#undef OMADIR_RETURN_IF_INVALID