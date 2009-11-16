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

/**@file txc_oma-directory.c
 * @brief <a href="http://www.openmobilealliance.org/Technical/release_program/docs/XDM/V1_1-20080627-A/OMA-TS-XDM_Core-V1_1-20080627-A.pdf">[OMA-TS-XDM_Core-V1_1-20080627-A] - OMA XCAP directory subclause 6.7.2</a>
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */

#include "txc_oma-directory.h"
#include "txc_macros.h"

#include "tsk_macros.h"

/**@defgroup txc_oma_directory_group OMA XCAP directory
*/

/**@page txc_oma_directory_page OMA XCAP directory Tutorial (org.openmobilealliance.xcap-directory)
* @par Application Unique ID (AUID)
* - '<span style="text-decoration:underline;">org.openmobilealliance.xcap-directory</span>' as per [OMA-TS-XDM_Core-V1_1-20080627-A] subclause 6.7.2.1
* @par Default Document Namespace
* - '<span style="text-decoration:underline;">urn:oma:xml:xdm:xcap-directory</span>' as per [OMA-TS-XDM_Core-V1_1-20080627-A] subclause 6.7.2.3
* @par MIME Type
* - '<span style="text-decoration:underline;">application/vnd.oma.xcap-directory+xml</span>' as per [OMA-TS-XDM_Core-V1_1-20080627-A] subclause 6.7.2.2
* @par Default document name
* - '<span style="text-decoration:underline;">directory.xml</span>' as per [OMA-TS-XDM_Core-V1_1-20080627-A] subclause 6.7.2.7
*
* <H2>=== Create/deserialize an OMA XCAP directory document received from the XDMS ===</H2>
*
* @code
#include "txc_api.h" 

char* entry_str = 0;
txc_omadir_t* omadir = 0;
txc_omadir_folder_L_t* list = 0;
tsk_list_item_t* item = 0;

printf("\n---\nTEST OMA XCAP-DIRECTORY\n---\n");
{
	// create oma xcap directory context
	omadir = txc_omadir_create(buffer, size);

	// get all folders
	printf("\nget all folders\n");
	if(list = txc_omadir_get_all_folders(omadir))
	{
		tsk_list_foreach(item, list)
		{
			txc_omadir_folder_t *folder = ((txc_omadir_folder_t*)item->data);
			printf("auid: \"%s\"\n", folder->auid);
		}
		TSK_LIST_SAFE_FREE(list);
	}

	// get all entries in the folder with auid 'groups'
	printf("\nget all entries in the folder with auid 'groups'\n");
	if(list = txc_omadir_get_entries_by_folder(omadir, "groups"))
	{
		tsk_list_foreach(item, list)
		{
			txc_rlist_entry_t *entry = ((txc_rlist_entry_t*)item->data);
			printf("folder's auid:\"%s\" uri:\"%s\" display-name=\"%s\" etag\"%s\"\n", entry->list, entry->uri, entry->display_name, entry->etag);
			entry_str = txc_rlist_entry_serialize(entry);
			printf("serialized entry: \n%s\n", entry_str);
			TSK_FREE(entry_str);
		}
		TSK_LIST_SAFE_FREE(list);
	}

	// free the context
	txc_omadir_free(&omadir);
}
* @endcode
*/

#define OMADIR_RETURN_IF_INVALID(omadir) if(!omadir || !(omadir->docPtr)) return 0;

#define OMADIR_SELECT_FOLDER_BY_AUID(fo_auid)\
	tsk_xml_select_node(omadir->docPtr->children, \
	TSK_XML_NODE_SELECT_BY_NAME("xcap-directory"), \
	TSK_XML_NODE_SELECT_BY_ATT("folder", "auid", fo_auid), \
	TSK_XML_NODE_SELECT_CONTENT(), \
	TSK_XML_NODE_SELECT_END())


/**@ingroup txc_oma_directory_group
* Internal function to initialize an OMA XCAP directory folder.
* You must never call this method. You shall use @ref TXC_OMADIR_FOLDER_CREATE to create a folder.
* @param folder The folder to initialize
*/
void txc_omadir_folder_init(txc_omadir_folder_t *folder)
{
}

/**@ingroup txc_oma_directory_group
* Internal function to free an OMA XCAP directory folder.
* You must never call this method. You shall use @ref TXC_OMADIR_FOLDER_SAFE_FREE to safely free a folder.
* @param _folder The folder to free
* @sa @ref TXC_OMADIR_FOLDER_SAFE_FREE
*/
void txc_omadir_folder_free(void **_folder)
{
	txc_omadir_folder_t **folder = ((txc_omadir_folder_t**)_folder);

	TSK_FREE((*folder)->auid);

	free(*_folder);
	(*_folder) = 0;
}

/**@ingroup txc_oma_directory_group
* Deserialize an OMA XCAP folder directory from an XML node pointer.
* @param node The pointer to the XML node to deserialize
* @retval An OMA XCAP folder object. You MUST call @ref TXC_OMADIR_FOLDER_SAFE_FREE to free the returned object.
*/
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
		folder->auid = tsk_strdup2(TSK_XML_NODE_SAFE_GET_TEXTVALUE(node2));
	}
	else return 0;

	return folder;
}

/**@ingroup txc_oma_directory_group
* Used to create an OMA XCAP directory context
* @param buffer The XML buffer from which to create the context
* @param size The size of the XML buffer
* @retval An OMA XCAP directory context. You MUST use @ref txc_omadir_free to free the returned object.
* @sa @ref txc_omadir_free
*/
txc_omadir_t* txc_omadir_create(const char* buffer, size_t size)
{
	if(buffer && size)
	{
		txc_omadir_t* omadir = (txc_omadir_t*)tsk_calloc2(1, sizeof(txc_omadir_t));
		omadir->docPtr = xmlParseMemory(buffer, (int)size);

		return omadir;
	}

	return 0;
}

/**@ingroup txc_oma_directory_group
* Returns all folders from an OMA XCAP directory context.
* @param omadir The context from which to extract all folders
* @retval The list of all available folders. You MUST use @a TSK_LIST_SAFE_FREE to free the returned list.
* @sa @ref txc_omadir_get_entries_by_folder
*/
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

/**@ingroup txc_oma_directory_group
* Returns all entries from an OMA XCAP folder.
* @param omadir The OMA XCAP directory context created using @ref txc_omadir_create.
* @param fo_auid The auid of the folder holding the entries
* @retval The list of all available entries. You MUST use @a TSK_LIST_SAFE_FREE to free the returned list.
*/
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

/**@ingroup txc_oma_directory_group
* Free an OMA XCAP directory context previously created using @ref txc_omadir_create.
* @param omadir The context to free.
* @sa @ref txc_omadir_create
*/
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