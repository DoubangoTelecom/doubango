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

#ifndef TEST_RLS_SERVICES
#define TEST_RLS_SERVICES

const char* rls_str0 = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
							"<rls-services xmlns=\"urn:ietf:params:xml:ns:rls-services\">"
							"<service uri=\"sip:1234578901@gsma.org;pres-list=rcs\">"
							"<resource-list>http://xcap1.gsma.com/services/resource-lists/users/sip:1234578901@gsma.org/index/~~/resource-lists/list%5B@name=%22rcs%22%5D</resource-list>"
							"<packages>"
							"<package>presence</package>"
							"</packages>"
							"</service>"
							"<service uri=\"sip:000000@gsma.org;pres-list=rcs\">"
							"<resource-list>http://xcap2.gsma.com/services/resource-lists/users/sip:1234578901@gsma.org/index/~~/resource-lists/list%5B@name=%22rcs%22%5D</resource-list>"
							"<packages>"
							"<package>mypackage</package>"
							"</packages>"
							"</service>"
							"</rls-services>";

/* test rls-services */
void test_rls_services(const char* buffer, size_t size)
{
	txc_rls_t* rls = 0;
	tsk_list_item_t* item = 0;
	tsk_list_t *list = 0;
	printf("\n---\nTEST RLS-SERVICES\n---\n");
	{
		/* create rls context */
		rls = TXC_RLS_CREATE(buffer, size);

		/* get all services */
		list = txc_rls_get_all_services(rls);

		/* dump services */
		tsk_list_foreach(item, list)
		{
			txc_rls_service_t *rls_service = ((txc_rls_service_t*)item->data);
			char* rls_service_str = txc_rls_service_serialize(rls_service);
			printf("\n %s \n", rls_service_str);
			TSK_FREE(rls_service_str);
		}

		/* free services */
		TSK_LIST_SAFE_FREE(list);
		
		/* free rls context */
		TXC_RLS_SAFE_FREE(rls);
	}
}

#endif /* TEST_RLS_SERVICES */