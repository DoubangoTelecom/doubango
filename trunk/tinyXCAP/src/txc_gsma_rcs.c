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

#define GSME_RCS_RLIST_ADD_ANCHOR(lname)\
	tsk_sprintf(0, &anchor, \
		"%s/~~/resource-lists/list%%5B@name=%%22%s%%22%%5D", \
		doc_sel, lname); \
	txc_rlist_list2_add_external(list2, anchor); \
	TSK_SAFE_FREE2(anchor);

#define GSMA_RCS_ADD_SERVICE(lname, package)\
	tsk_sprintf(0, &uri, "%s;pres-list=%s", \
		context->xui, lname);\
	tsk_sprintf(0, &resource_list, \
		"%s/~~/resource-lists/list%%5B@name=%%22%s%%22%%5D", \
		doc_sel, lname); \
	txc_rls_service_set(service, uri, resource_list); \
	txc_rls_service_add_package(service, package); \
	tsk_list_add_data(services, ((void**) &service), txc_rls_service_free); \
	TSK_SAFE_FREE2(resource_list); \
	TSK_SAFE_FREE2(uri);

/* create default list2 with all mandatory list as per GSMA RCS2. this include oma lists */
/* ATTENTION: use 'TSK_LIST_SAFE_FREE' function to free the returned object */
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
	TSK_LIST_CREATE(rlist);

	/* list containing all contacts and named 'all' */
	TXC_RLIST_LIST2_CREATE(list2);
	txc_rlist_list2_set(list2, "all", "all contacts");
	tsk_list_add_data(rlist, ((void**) &list2), txc_rlist_list2_free);

	/* rcs */
	TXC_RLIST_LIST2_CREATE(list2);
	txc_rlist_list2_set(list2, TXC_RNAME_GSMA_RCS_RCS, TXC_RNAME_GSMA_RCS_RCS);
	txc_rlist_list2_add_entry(list2, context->xui, "own"); /* RCS 2 */
	GSME_RCS_RLIST_ADD_ANCHOR("all");
	tsk_list_add_data(rlist, ((void**) &list2), txc_rlist_list2_free);

	/* rcs_blockedcontacts */
	TXC_RLIST_LIST2_CREATE(list2);
	txc_rlist_list2_set(list2, TXC_RNAME_GSMA_RCS_BLOCKEDCONTACTS, TXC_RNAME_GSMA_RCS_BLOCKEDCONTACTS);
	tsk_list_add_data(rlist, ((void**) &list2), txc_rlist_list2_free);

	/* rcs_revokedcontacts” */
	TXC_RLIST_LIST2_CREATE(list2);
	txc_rlist_list2_set(list2, TXC_RNAME_GSMA_RCS_REVOKEDCONTACTS, TXC_RNAME_GSMA_RCS_REVOKEDCONTACTS);
	tsk_list_add_data(rlist, ((void**) &list2), txc_rlist_list2_free);

	/* oma_buddylist */
	TXC_RLIST_LIST2_CREATE(list2);
	txc_rlist_list2_set(list2, TXC_RNAME_OMA_BUDDYLIST, TXC_RNAME_OMA_BUDDYLIST);
	GSME_RCS_RLIST_ADD_ANCHOR(TXC_RNAME_GSMA_RCS_RCS);
	tsk_list_add_data(rlist, ((void**) &list2), txc_rlist_list2_free);

	/* oma_grantedcontacts */
	TXC_RLIST_LIST2_CREATE(list2);
	txc_rlist_list2_set(list2, TXC_RNAME_OMA_GRANTEDCONTACTS, TXC_RNAME_OMA_GRANTEDCONTACTS);
	GSME_RCS_RLIST_ADD_ANCHOR(TXC_RNAME_GSMA_RCS_RCS);
	GSME_RCS_RLIST_ADD_ANCHOR(TXC_RNAME_OMA_BUDDYLIST);
	tsk_list_add_data(rlist, ((void**) &list2), txc_rlist_list2_free);

	/* oma_blockedcontacts */
	TXC_RLIST_LIST2_CREATE(list2);
	txc_rlist_list2_set(list2, TXC_RNAME_OMA_BLOCKEDCONTACTS, TXC_RNAME_OMA_BLOCKEDCONTACTS);
	GSME_RCS_RLIST_ADD_ANCHOR(TXC_RNAME_GSMA_RCS_BLOCKEDCONTACTS);
	GSME_RCS_RLIST_ADD_ANCHOR(TXC_RNAME_GSMA_RCS_REVOKEDCONTACTS);
	tsk_list_add_data(rlist, ((void**) &list2), txc_rlist_list2_free);
	
bail:
	/* free doc sel */
	TSK_FREE(doc_sel);

	return rlist;
}

/* create default rls with all mandatory services as per GSMA RCS1 */
/* ATTENTION: use 'TSK_LIST_SAFE_FREE' function to free the returned object */
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
	TSK_LIST_CREATE(services);
	
	/* rcs */
	TXC_RLS_SERVICE_CREATE(service);
	GSMA_RCS_ADD_SERVICE(TXC_RNAME_GSMA_RCS_RCS, "presence");
	
	/* oma buddy list */
	TXC_RLS_SERVICE_CREATE(service);
	GSMA_RCS_ADD_SERVICE(TXC_RNAME_OMA_BUDDYLIST, "presence");
	
	bail:
	/* free doc sel */
	TSK_FREE(doc_sel);

	return services;
}

/* create default oma pres-rules document */
/* ATTENTION: use 'TSK_SAFE_FREE2' function to free the returned string */
char* txc_gsmarcs_create_oma_presrules(const txc_context_t* context)
{
	return txc_oma_presrules_serialize(context);
}

#undef GSME_RCS_LIST_ADD