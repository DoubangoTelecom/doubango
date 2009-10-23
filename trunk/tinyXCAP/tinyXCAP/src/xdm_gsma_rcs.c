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

/* both gsma rcs1 and rcs2 */
/* GSMA RCS2: http://www.gsmworld.com/documents/rcs/rcs2_june09/R2_090831_RCS_Release_2_Technical_Realisation_v1_0.pdf */
/* GSMA RCS1: http://www.gsmworld.com/documents/rcs/rcs1_updates/R1_090831_RCS_Release_1_Technical_Realisation_v1_1.pdf */
#include "xdm_gsma_rcs.h"
#include "xdm_oma.h"
#include "xdm_document.h"
#include "xdm_oma-presrules.h"

#include "xdm_utils.h"

#define GSME_RCS_RLIST_ADD_ANCHOR(lname)\
	xdm_sprintf(&anchor, \
		"%s/~~/resource-lists/list%%5B@name=%%22%s%%22%%5D", \
		doc_sel, lname); \
	xdm_rlist_list2_add_external(list2, anchor); \
	XDM_SAFE_FREE(anchor);

#define GSMA_RCS_ADD_SERVICE(lname, package)\
	xdm_sprintf(&uri, "%s;pres-list=%s", \
		context->xui, lname);\
	xdm_sprintf(&resource_list, \
		"%s/~~/resource-lists/list%%5B@name=%%22%s%%22%%5D", \
		doc_sel, lname); \
	xdm_rls_service_set(service, uri, resource_list); \
	xdm_rls_service_add_package(service, package); \
	xdm_list_add_data(services, ((void**) &service), xdm_rls_service_free); \
	XDM_SAFE_FREE(resource_list); \
	XDM_SAFE_FREE(uri);

/* create default list2 with all mandatory list as per GSMA RCS2. this include oma lists */
/* ATTENTION: use 'XDM_LIST_SAFE_FREE' function to free the returned object */
xdm_rlist_list2_L_t* xdm_gsmarcs_create_rlist2(const xdm_context_t* context)
{
	xdm_rlist_list2_L_t* rlist = 0;
	xdm_rlist_list2_t *list2 = 0;
	char* anchor = 0, *doc_sel = 0;
	
	/* get document selector */
	if( !context || !(doc_sel = xdm_doc_get_sel2(context, ietf_resource_lists)) )
	{
		goto bail;
	}

	/* create list to be returned */
	XDM_LIST_CREATE(rlist);

	/* list containing all contacts and named 'all' */
	XDM_RLIST_LIST2_CREATE(list2);
	xdm_rlist_list2_set(list2, "all", "all contacts");
	xdm_list_add_data(rlist, ((void**) &list2), xdm_rlist_list2_free);

	/* rcs */
	XDM_RLIST_LIST2_CREATE(list2);
	xdm_rlist_list2_set(list2, XDM_RNAME_GSMA_RCS_RCS, XDM_RNAME_GSMA_RCS_RCS);
	xdm_rlist_list2_add_entry(list2, context->xui, "own"); /* RCS 2 */
	GSME_RCS_RLIST_ADD_ANCHOR("all");
	xdm_list_add_data(rlist, ((void**) &list2), xdm_rlist_list2_free);

	/* rcs_blockedcontacts */
	XDM_RLIST_LIST2_CREATE(list2);
	xdm_rlist_list2_set(list2, XDM_RNAME_GSMA_RCS_BLOCKEDCONTACTS, XDM_RNAME_GSMA_RCS_BLOCKEDCONTACTS);
	xdm_list_add_data(rlist, ((void**) &list2), xdm_rlist_list2_free);

	/* rcs_revokedcontacts” */
	XDM_RLIST_LIST2_CREATE(list2);
	xdm_rlist_list2_set(list2, XDM_RNAME_GSMA_RCS_REVOKEDCONTACTS, XDM_RNAME_GSMA_RCS_REVOKEDCONTACTS);
	xdm_list_add_data(rlist, ((void**) &list2), xdm_rlist_list2_free);

	/* oma_buddylist */
	XDM_RLIST_LIST2_CREATE(list2);
	xdm_rlist_list2_set(list2, XDM_RNAME_OMA_BUDDYLIST, XDM_RNAME_OMA_BUDDYLIST);
	GSME_RCS_RLIST_ADD_ANCHOR(XDM_RNAME_GSMA_RCS_RCS);
	xdm_list_add_data(rlist, ((void**) &list2), xdm_rlist_list2_free);

	/* oma_grantedcontacts */
	XDM_RLIST_LIST2_CREATE(list2);
	xdm_rlist_list2_set(list2, XDM_RNAME_OMA_GRANTEDCONTACTS, XDM_RNAME_OMA_GRANTEDCONTACTS);
	GSME_RCS_RLIST_ADD_ANCHOR(XDM_RNAME_GSMA_RCS_RCS);
	GSME_RCS_RLIST_ADD_ANCHOR(XDM_RNAME_OMA_BUDDYLIST);
	xdm_list_add_data(rlist, ((void**) &list2), xdm_rlist_list2_free);

	/* oma_blockedcontacts */
	XDM_RLIST_LIST2_CREATE(list2);
	xdm_rlist_list2_set(list2, XDM_RNAME_OMA_BLOCKEDCONTACTS, XDM_RNAME_OMA_BLOCKEDCONTACTS);
	GSME_RCS_RLIST_ADD_ANCHOR(XDM_RNAME_GSMA_RCS_BLOCKEDCONTACTS);
	GSME_RCS_RLIST_ADD_ANCHOR(XDM_RNAME_GSMA_RCS_REVOKEDCONTACTS);
	xdm_list_add_data(rlist, ((void**) &list2), xdm_rlist_list2_free);
	
bail:
	/* free doc sel */
	XDM_FREE(doc_sel);

	return rlist;
}

/* create default rls with all mandatory services as per GSMA RCS1 */
/* ATTENTION: use 'XDM_LIST_SAFE_FREE' function to free the returned object */
xdm_rls_service_L_t* xdm_gsmarcs_create_rls(const xdm_context_t* context)
{
	xdm_rls_service_t *service = 0;
	xdm_rls_service_L_t* services = 0;
	char *uri = 0, *resource_list = 0, *doc_sel = 0;
	
	/* get document selector */
	if( !context || !(doc_sel = xdm_doc_get_sel2(context, ietf_resource_lists)) )
	{
		goto bail;
	}

	/* create list to be returned */
	XDM_LIST_CREATE(services);
	
	/* rcs */
	XDM_RLS_SERVICE_CREATE(service);
	GSMA_RCS_ADD_SERVICE(XDM_RNAME_GSMA_RCS_RCS, "presence");
	
	/* oma buddy list */
	XDM_RLS_SERVICE_CREATE(service);
	GSMA_RCS_ADD_SERVICE(XDM_RNAME_OMA_BUDDYLIST, "presence");
	
	bail:
	/* free doc sel */
	XDM_FREE(doc_sel);

	return services;
}

/* create default oma pres-rules document */
/* ATTENTION: use 'XDM_SAFE_FREE' function to free the returned string */
char* xdm_gsmarcs_create_oma_presrules(const xdm_context_t* context)
{
	return xdm_oma_presrules_serialize(context);
}

#undef GSME_RCS_LIST_ADD