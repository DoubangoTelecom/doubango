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
#include "txc_api.h" 

#ifndef TEST_OMA_DIRECTORY
#define TEST_OMA_DIRECTORY

// http://www.openmobilealliance.com/technical/release_program/docs/XDM/V2_0-20070724-C/OMA-TS-XDM_Core-V2_0-20070724-C.pdf subclause C.3
const char* omadir_str0 = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
							"<xcap-directory xmlns=\"urn:oma:xml:xdm:xcap-directory\" >"
							"<folder auid=\"resource-lists\">"
							"<entry uri=\"http://xcap.example.com/resource-lists/users/sip:joebloggs@example.com/index\" etag=\"pqr999\" last-modified=\"1279068803188\" size=\"2738\"/>"
							"</folder>"
							"<folder auid=\"groups\">"
							"<entry "
							"uri=\"http://xcap.example.com/org.openmobilealliance.groups/users/sip:joebloggs@example.com/skiing\" etag=\"abc123\"/>"
							"<entry "
							"uri=\"http://xcap.example.com/org.openmobilealliance.groups/users/sip:joebloggs@example.com/shopping\" etag=\"def456\"/>"
							"</folder>"
							"</xcap-directory>";

/* test OMA directory (urn:oma:xml:xdm:xcap-directory) */
void test_oma_directory(const char* buffer, size_t size)
{
	char* entry_str = 0;
	txc_omadir_t* omadir = 0;
	txc_omadir_folder_L_t* list = 0;
	tsk_list_item_t* item = 0;
	
	printf("\n---\nTEST OMA XCAP-DIRECTORY\n---\n");
	{
		/* create oma xcap directory context */
		omadir = TXC_OMADIR_CREATE(buffer, size);

		/* get all folders */
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

		/* get all entries in the folder with auid 'groups' */
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

		/* free the context */
		TXC_OMADIR_SAFE_FREE(omadir);
	}
}

#endif /* TEST_OMA_DIRECTORY */