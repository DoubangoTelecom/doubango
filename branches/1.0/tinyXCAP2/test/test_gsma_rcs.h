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

#ifndef _TEST_GSMA_RCS_H_
#define _TEST_GSMA_RCS_H_

#include "txc_api.h" 

/* test gsma rcs */
void test_gsma_rcs()
{
	txc_context_t* context = 0;
	txc_rlist_list2_L_t *rlist2 = 0, *services;
	char *rlist2_str = 0, *rls_str = 0, *oma_presrules_str = 0;
	
	/* create our xdm context */
	TXC_CONTEXT_CREATE(context);

	/*context->user_agent = xdm_strdup("XDM-client/OMA1.1");
	context->xdm_root = xdm_strdup("http://xcap.example.com/services");
	context->password = xdm_strdup("mysecret");
	context->xui = xdm_strdup("sip:doubango@example.com");*/

	/* Initialize the context */
	TEST_XXXX_CONTEXT_INIT(context);

	/* Create an GSMA RCS2 resource-lists document object and serialize the document for test */
	rlist2 = txc_gsmarcs_create_rlist2(context);
	rlist2_str = txc_rlist_list2_L_serialize(rlist2);
	printf("rcs resource-lists: %s\n", rlist2_str);
	TSK_FREE(rlist2_str);
	TSK_LIST_SAFE_FREE(rlist2);

	/* Create an GSMA RCS2 rls-services document object and serialize the document for test*/
	services = txc_gsmarcs_create_rls(context);
	rls_str = txc_rls_services_serialize(services);
	printf("rcs rls-services: %s\n", rls_str);
	TSK_FREE(rls_str);
	TSK_LIST_SAFE_FREE(services);

	/* Create an GSMA RCS2 pres-rules document as XML string */
	oma_presrules_str = txc_gsmarcs_create_oma_presrules(context);
	printf("rcs oma pres-rules: %s\n", oma_presrules_str);
	TSK_FREE(oma_presrules_str);

	/* free context */
	TXC_CONTEXT_SAFE_FREE(context);
}


#endif /* _TEST_GSMA_RCS_H_ */