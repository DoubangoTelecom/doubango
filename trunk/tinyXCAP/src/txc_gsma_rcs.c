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

/**@file txc_gsma_rcs.c
 * @brief GSMA <a href="http://www.gsmworld.com/documents/rcs/rcs1_updates/R1_090831_RCS_Release_1_Technical_Realisation_v1_1.pdf">RCS 1</a> and <a href="http://www.gsmworld.com/documents/rcs/rcs2_june09/R2_090831_RCS_Release_2_Technical_Realisation_v1_0.pdf ">RCS 2</a> documents
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */

/* both gsma rcs1 and rcs2 */
/* GSMA RCS2: http://www.gsmworld.com/documents/rcs/rcs2_june09/R2_090831_RCS_Release_2_Technical_Realisation_v1_0.pdf */
/* GSMA RCS1: http://www.gsmworld.com/documents/rcs/rcs1_updates/R1_090831_RCS_Release_1_Technical_Realisation_v1_1.pdf */
#include "txc_gsma_rcs.h"
#include "txc_oma.h"
#include "txc_document.h"
#include "txc_oma-presrules.h"
#include "txc_macros.h"

#include "tsk_memory.h"
#include "tsk_macros.h"

/**@defgroup txc_gsma_rcs_group GSMA RCS phase 1 & 2
*/

/**@page txc_gsma_rcs_page GSMA RCS phase 1 & 2 Tutorial
*
* <H2>=== Create and serialize a GSMA RCS resource-lists, rls-services and pres-rules document ===</H2>
*
* @code
#include "txc_api.h" 

txc_context_t* context = 0;
txc_rlist_list2_L_t *rlist2 = 0, *services;
char *rlist2_str = 0, *rls_str = 0, *oma_presrules_str = 0;

// create our xdm context
TXC_CONTEXT_CREATE(context);

// Initialize the context 
context->user_agent = tsk_strdup("XDM-client/OMA1.1");
context->xdm_root = tsk_strdup("http://xcap.example.com/services");
context->password = tsk_strdup("mysecret");
context->xui = tsk_strdup("sip:doubango@example.com");

// Create an GSMA RCS2 resource-lists document object and serialize the document for test
rlist2 = txc_gsmarcs_create_rlist2(context);
rlist2_str = txc_rlist_list22_serialize(rlist2);
printf("rcs resource-lists: %s\n", rlist2_str);
TSK_FREE(rlist2_str);
TSK_LIST_SAFE_FREE(rlist2);

// Create an GSMA RCS2 rls-services document object and serialize the document for test
services = txc_gsmarcs_create_rls(context);
rls_str = txc_rls_services_serialize(services);
printf("rcs rls-services: %s\n", rls_str);
TSK_FREE(rls_str);
TSK_LIST_SAFE_FREE(services);

// Create an GSMA RCS2 pres-rules document as XML string
oma_presrules_str = txc_gsmarcs_create_oma_presrules(context);
printf("rcs oma pres-rules: %s\n", oma_presrules_str);
TSK_FREE(oma_presrules_str);

// free context
TXC_CONTEXT_SAFE_FREE(context);
* @endcode
*/

#define GSME_RCS_RLIST_ADD_ANCHOR(lname)\
	tsk_sprintf(&anchor, \
		"%s/~~/resource-lists/list%%5B@name=%%22%s%%22%%5D", \
		doc_sel, lname); \
	txc_rlist_list2_add_external(list2, anchor); \
	TSK_FREE(anchor);

#define GSMA_RCS_ADD_SERVICE(lname, package)\
	tsk_sprintf(&uri, "%s;pres-list=%s", \
		context->xui, lname);\
	tsk_sprintf(&resource_list, \
		"%s/~~/resource-lists/list%%5B@name=%%22%s%%22%%5D", \
		doc_sel, lname); \
	txc_rls_service_set(service, uri, resource_list); \
	txc_rls_service_add_package(service, package); \
	tsk_list_add_data(services, ((void**) &service)); \
	TSK_FREE(resource_list); \
	TSK_FREE(uri);


/**@ingroup txc_gsma_rcs_group
* Create a GSMA RCS2 resource-lists document.
* @param context The XCAP context from which to create the document.
* @retval The resource-list document as @ref txc_rlist_list2_L_t object.
* You MUST call @a TSK_LIST_SAFE_FREE to free the returned object.
* @sa @ref txc_gsmarcs_create_rls
*/
txc_rlist_list2_L_t* txc_gsmarcs_create_rlist2(const txc_context_t* context)
{
	txc_rlist_list2_L_t* rlist = 0;
	txc_rlist_list2_t *list2 = 0;
	char* anchor = 0, *doc_sel = 0;
	
	/* get document selector */
	if( !context || !(doc_sel = TXC_DOC_GET_SEL(context, ietf_resource_lists)) )
	{
		goto bail;
	}

	/* create list to be returned */
	rlist = TSK_LIST_CREATE();

	/* list containing all contacts and named 'all' */
	list2 = TXC_RLIST_LIST2_CREATE();
	txc_rlist_list2_set(list2, "all", "all contacts");
	tsk_list_add_data(rlist, ((void**) &list2));

	/* rcs */
	list2 = TXC_RLIST_LIST2_CREATE();
	txc_rlist_list2_set(list2, TXC_RNAME_GSMA_RCS_RCS, TXC_RNAME_GSMA_RCS_RCS);
	txc_rlist_list2_add_entry(list2, context->xui, "own"); /* RCS 2 */
	GSME_RCS_RLIST_ADD_ANCHOR("all");
	tsk_list_add_data(rlist, ((void**) &list2));

	/* rcs_blockedcontacts */
	list2 = TXC_RLIST_LIST2_CREATE();
	txc_rlist_list2_set(list2, TXC_RNAME_GSMA_RCS_BLOCKEDCONTACTS, TXC_RNAME_GSMA_RCS_BLOCKEDCONTACTS);
	tsk_list_add_data(rlist, ((void**) &list2));

	/* rcs_revokedcontacts” */
	list2 = TXC_RLIST_LIST2_CREATE();
	txc_rlist_list2_set(list2, TXC_RNAME_GSMA_RCS_REVOKEDCONTACTS, TXC_RNAME_GSMA_RCS_REVOKEDCONTACTS);
	tsk_list_add_data(rlist, ((void**) &list2));

	/* oma_buddylist */
	list2 = TXC_RLIST_LIST2_CREATE();
	txc_rlist_list2_set(list2, TXC_RNAME_OMA_BUDDYLIST, TXC_RNAME_OMA_BUDDYLIST);
	GSME_RCS_RLIST_ADD_ANCHOR(TXC_RNAME_GSMA_RCS_RCS);
	tsk_list_add_data(rlist, ((void**) &list2));

	/* oma_grantedcontacts */
	list2 = TXC_RLIST_LIST2_CREATE();
	txc_rlist_list2_set(list2, TXC_RNAME_OMA_GRANTEDCONTACTS, TXC_RNAME_OMA_GRANTEDCONTACTS);
	GSME_RCS_RLIST_ADD_ANCHOR(TXC_RNAME_GSMA_RCS_RCS);
	GSME_RCS_RLIST_ADD_ANCHOR(TXC_RNAME_OMA_BUDDYLIST);
	tsk_list_add_data(rlist, ((void**) &list2));

	/* oma_blockedcontacts */
	list2 = TXC_RLIST_LIST2_CREATE();
	txc_rlist_list2_set(list2, TXC_RNAME_OMA_BLOCKEDCONTACTS, TXC_RNAME_OMA_BLOCKEDCONTACTS);
	GSME_RCS_RLIST_ADD_ANCHOR(TXC_RNAME_GSMA_RCS_BLOCKEDCONTACTS);
	GSME_RCS_RLIST_ADD_ANCHOR(TXC_RNAME_GSMA_RCS_REVOKEDCONTACTS);
	tsk_list_add_data(rlist, ((void**) &list2));
	
bail:
	/* free doc sel */
	TSK_FREE(doc_sel);

	return rlist;
}

/**@ingroup txc_gsma_rcs_group
* Create a GSMA RCS2 rls-services document.
* @param context The XCAP context from which to create the document.
* @retval The rls-services document as @ref txc_rls_service_L_t object.
* You MUST call @a TSK_LIST_SAFE_FREE to free the returned object.
* @sa @ref txc_gsmarcs_create_rlist2
*/
txc_rls_service_L_t* txc_gsmarcs_create_rls(const txc_context_t* context)
{
	txc_rls_service_t *service = 0;
	txc_rls_service_L_t* services = 0;
	char *uri = 0, *resource_list = 0, *doc_sel = 0;
	
	/* get document selector */
	if( !context || !(doc_sel = TXC_DOC_GET_SEL(context, ietf_resource_lists)) )
	{
		goto bail;
	}

	/* create list to be returned */
	services = TSK_LIST_CREATE();
	
	/* rcs */
	service = TXC_RLS_SERVICE_CREATE();
	GSMA_RCS_ADD_SERVICE(TXC_RNAME_GSMA_RCS_RCS, "presence");
	
	/* oma buddy list */
	service = TXC_RLS_SERVICE_CREATE();
	GSMA_RCS_ADD_SERVICE(TXC_RNAME_OMA_BUDDYLIST, "presence");
	
	bail:
	/* free doc sel */
	TSK_FREE(doc_sel);

	return services;
}

/**@ingroup txc_gsma_rcs_group
* Serialize the GSMA RCS2 pres-rules document as XML string.
* @param context The XCAP context from which to create the document.
* @retval The resource-list document as @ref txc_rls_service_L_t object.
* You MUST call @a TSK_FREE to free the returned string.
*/
char* txc_gsmarcs_create_oma_presrules(const txc_context_t* context)
{
	return txc_oma_presrules_serialize(context);
}

#undef GSME_RCS_LIST_ADD