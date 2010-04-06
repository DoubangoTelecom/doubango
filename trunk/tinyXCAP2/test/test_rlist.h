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
#ifndef TEST_RLIST_H
#define TEST_RLIST_H

#include "txc_api.h" 

/* http://www.gsmworld.com/documents/rcs/rcs2_june09/R2_090831_RCS_Release_2_Technical_Realisation_v1_0.pdf subclause 11.2*/
	const char* rlist_str0 = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"/*//xmnls:xd=\"urn:oma:xml:xdm:xcap-directory\"*/
							"<resource-lists xmlns=\"urn:ietf:params:xml:ns:resource-lists\" xmnls:xd=\"urn:oma:xml:xdm:xcap-directory\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance3\">"
							"<!-- The list oma_buddylist contains references to any individual list used according to OMA IG for presence subscriptions. -->"
							"<list name=\"oma_buddylist\">"
							"<external anchor=\"http://xcap.example.org/resource-lists/users/sip:RCSUser@example.org/index/~~/resource-lists/list%5B@name=%22rcs%22%5D\"/>"
							"</list>"
							"<!-- The list oma_grantedcontacts contains the list of all granted contacts -->"
							"<list name=\"oma_grantedcontacts\">"
							"<external anchor=\"http://xcap.example.org/resource-lists/users/sip:RCSUser@example.org/index/~~/resource-lists/list%5B@name=%22rcs%22%5D\"/>"
							"</list>"
							"<!-- The list oma_blockedcontacts contains the list of all blocked contacts. -->"
							"<list name=\"oma_blockedcontacts\">"
							"<external anchor=\"http://xcap.example.org/resource-lists/users/sip:RCSUser@example.org/index/~~/resource-lists/list%5B@name=%22rcs_blockedcontacts%22%5D\"/>"
							"<external anchor=\"http://xcap.example.org/resource-lists/users/sip:RCSUser@example.org/index/~~/resource-lists/list%5B@name=%22rcs_revokedcontacts%22%5D\"/>"
							"</list>"
							"<!-- The list of buddies the owner wants be able to get presence information for -->"
							"<!-- The RCS presentity is always part of this list, refer to e 11.4 -->"
							"<list name=\"rcs\">"
							"<display-name>My presence buddies</display-name>"
							"<!-- The URI below is just an example of the own's user Id -->"
							"<entry uri=\"tel:+1234578901\" />"
							"</list>"
							"<!-- The list of blocked contacts -->"
							"<list name=\"rcs_blockedcontacts\">"
							"<display-name>My blocked contacts</display-name>"
							"</list>"
							"<!-- The list of revoked contacts -->"
							"<list name=\"rcs_revokedcontacts\">"
							"<display-name>My revoked contacts</display-name>"
							"<entry uri=\"tel:+123457\" xd:last-modified=\"2008-12-24T14:32:14Z\">"
							"<display-name>123457's display name</display-name>"
							"</entry>"
							"<entry uri=\"tel:+123456\" xd:last-modified=\"2008-12-24T14:32:13Z\"/>"
							"</list>"
							"</resource-lists>";

/* test rlist (resource-lists) */
void test_rlist(const char* rlist_str, size_t size)
{
	char* entry_str = 0;
	txc_rlist_t* rlist = 0;
	tsk_list_t* list = 0;
	tsk_list_item_t* item = 0;

	printf("\n---\nTEST RESOURCE-LISTS\n---\n");
	{
		/* create rlist context */
		rlist = TXC_RLIST_CREATE(rlist_str, size);

		/* get all lists */
		printf("\nget all lists\n");
		if(list = txc_rlist_get_all_lists(rlist, 0))
		{
			tsk_list_foreach(item, list)
			{
				txc_rlist_list_t *rlist_list = ((txc_rlist_list_t*)item->data);
				printf("name\"%s\" display-name=\"%s\"\n", rlist_list->name, rlist_list->display_name);
			}
			TSK_LIST_SAFE_FREE(list);
		}

		/* get all entries in the list named 'ennemies' */
		printf("\nget all entries in the list named 'rcs_revokedcontacts'\n");
		if(list = txc_rlist_get_entries_by_list(rlist, "rcs_revokedcontacts"))
		{
			tsk_list_foreach(item, list)
			{
				txc_rlist_entry_t *entry = ((txc_rlist_entry_t*)item->data);
				printf("list:\"%s\" uri:\"%s\" display-name=\"%s\" last-modified\"%s\"\n", entry->list, entry->uri, entry->display_name, entry->last_modified);
				entry_str = txc_rlist_entry_serialize(entry);
				printf("serialized entry: \n%s\n", entry_str);
				TSK_FREE(entry_str);
			}
			TSK_LIST_SAFE_FREE(list);
		}

		/* get all externals in the list named 'friends' */
		printf("\nget all externals in the list named 'oma_blockedcontacts'\n");
		if(list = txc_rlist_get_externals_by_list(rlist, "oma_blockedcontacts"))
		{
			tsk_list_foreach(item, list)
			{
				txc_rlist_external_t *external = ((txc_rlist_external_t*)item->data);
				printf("list:\"%s\" anchor:\"%s\"\n", external->list, external->anchor);
			}
			TSK_LIST_SAFE_FREE(list);
		}

		/* free rlist */
		TXC_RLIST_SAFE_FREE(rlist);
	}
}

void test_rlist2(const char* rlist_str, size_t size)
{
	char* entry_str = 0;
	txc_rlist_t* rlist = 0;
	txc_rlist_listx_L_t* list = 0;
	tsk_list_item_t *item1 = 0, *item2 = 0, *item3 = 0;

	printf("\n---\nTEST RESOURCE-LISTS-2\n---\n");
	{
		/* create rlist context */
		rlist = TXC_RLIST_CREATE(rlist_str, size);

		/* get all lists */
		printf("\nget all lists\n");
		if(list = txc_rlist_get_all_lists(rlist, 1))
		{
			tsk_list_foreach(item1, list)
			{
				txc_rlist_list2_t *rlist_list2 = ((txc_rlist_list2_t*)item1->data);

				/* name and display-name */
				printf("name\"%s\" display-name=\"%s\"\n", rlist_list2->name, rlist_list2->display_name);

				/* entries */
				tsk_list_foreach(item2, rlist_list2->entries)
				{
					txc_rlist_entry_t *entry = ((txc_rlist_entry_t*)item2->data);
					char* entry_str = txc_rlist_entry_serialize(entry);
					printf("\n%s\n", entry_str);
					TSK_FREE(entry_str);
				}

				/*externals*/
				tsk_list_foreach(item3, rlist_list2->externals)
				{
					txc_rlist_external_t *external = ((txc_rlist_external_t*)item3->data);
					char* external_str = txc_rlist_external_serialize(external);
					printf("\n%s\n", external_str);
					TSK_FREE(external_str);
				}

			}
			TSK_LIST_SAFE_FREE(list);
		}

		/* free rlist */
		TXC_RLIST_SAFE_FREE(rlist);
	}
}

#endif /* TEST_RLIST_H */