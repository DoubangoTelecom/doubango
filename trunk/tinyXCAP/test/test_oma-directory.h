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

#include "xdm_api.h" 

/* test OMA directory (urn:oma:xml:xdm:xcap-directory) */
void test_oma_directory()
{
	char* entry_str = 0;
	xdm_omadir_t* omadir = 0;
	xdm_list_t* list = 0;
	xdm_list_item_t* item = 0;
	// http://www.openmobilealliance.com/technical/release_program/docs/XDM/V2_0-20070724-C/OMA-TS-XDM_Core-V2_0-20070724-C.pdf subclause C.3
	const char* omadir_str = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
								"<xcap-directory xmlns=\"urn:oma:xml:xdm:xcap-directory\" >"
								"<folder auid=\"resource-lists\">"
								"<entry uri=\"http://xcap.example.com/resource-lists/users/sip:joebloggs@example.com/index\" etag=\"pqr999\"/>"
								"</folder>"
								"<folder auid=\"groups\">"
								"<entry "
								"uri=\"http://xcap.example.com/org.openmobilealliance.groups/users/sip:joebloggs@example.com/skiing\" etag=\"abc123\"/>"
								"<entry "
								"uri=\"http://xcap.example.com/org.openmobilealliance.groups/users/sip:joebloggs@example.com/shopping\" etag=\"def456\"/>"
								"</folder>"
								"</xcap-directory>";
	printf("\n---\nTEST OMA XCAP-DIRECTORY\n---\n");
	{
		/* create rlist context */
		omadir = xdm_omadir_create(omadir_str, strlen(omadir_str));

		/* get all folders */
		printf("\nget all folders\n");
		if(list = xdm_omadir_get_all_folders(omadir))
		{
			xdm_list_foreach(item, list)
			{
				xdm_omadir_folder_t *folder = ((xdm_omadir_folder_t*)item->data);
				printf("auid: \"%s\"\n", folder->auid);
			}
			XDM_LIST_SAFE_FREE(list);
		}

		/* get all entries in the folder with auid 'groups' */
		printf("\nget all entries in the folder with auid 'groups'\n");
		if(list = xdm_omadir_get_entries_by_folder(omadir, "groups"))
		{
			xdm_list_foreach(item, list)
			{
				xdm_rlist_entry_t *entry = ((xdm_rlist_entry_t*)item->data);
				printf("folder's auid:\"%s\" uri:\"%s\" display-name=\"%s\" etag\"%s\"\n", entry->list, entry->uri, entry->display_name, entry->etag);
				entry_str = xdm_rlist_entry_serialize(entry);
				printf("serialized entry: \n%s\n", entry_str);
				XDM_SAFE_FREE(entry_str);
			}
			XDM_LIST_SAFE_FREE(list);
		}

		/* free omadir */
		xdm_omadir_free(&omadir);
	}
}