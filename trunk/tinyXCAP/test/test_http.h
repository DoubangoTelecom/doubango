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
#ifndef TEST_HTTP_H
#define TEST_HTTP_H

#include "txc_api.h"

#define HTTP_LOOP						0

#define HTTP_RUN_TEST_ALL				0
#define HTTP_RUN_TEST_XCAP_CAPS			1
#define HTTP_RUN_TEST_OMADIR			1
#define HTTP_RUN_TEST_RLIST				1

/*=== Predicates 
===*/
int find_list_by_name(const tsk_list_item_t* item, const void* name)
{
	const txc_rlist_list2_t* rlist2 = (txc_rlist_list2_t*) item->data;
	return tsk_equals(rlist2->name, (const char*)name);
}

/*=== add list
===*/
int test_http_add_list(txc_context_t *context, const char* name, const char* displayname, const char* anchor)
{
	int ret = 0,error = 0;
	txc_request_t *request = 0;
	char* list_str = 0;

	TXC_REQUEST_CREATE(request);
	
	request->url = txc_node_get_sel(context, ietf_resource_lists,
			TXC_NODE_SELECT_BY_ATTR("list", "name", name),
			TXC_NODE_SELECT_END()
			);
	list_str = txc_rlist_list_serialize2(name, displayname, anchor);
	txc_content_set(request->content, list_str, strlen(list_str), 0);
	TSK_FREE(list_str);
	
	/* send the request to the server */
	if((error = txc_xcap_perform(context, request, op_create, sl_element)) || !TXC_HTTP_SUCCESS(request))
	{
		printf("test_http_rlist/// failed to add a list error:%d status:%d\n", error, request->status);
		ret = -6; goto bail;
	}
		
bail:
	TXC_REQUEST_SAFE_FREE(request);
	return ret;
}

/*=== testing xcap-caps :
- get server capabilities 
- update available auids
===*/
int test_http_caps(txc_context_t *context)
{
	txc_request_t* request = 0;
	xcap_caps_t* xcap_caps = 0;
	int ret = 0, error = 0;

	printf("---\nTEST_HTTP_CAPS\n---\n");

	TXC_REQUEST_CREATE(request);

	/*== get xcap-caps document an update avalable auids ==*/
	request->url = TXC_DOC_GET_SEL(context, ietf_xcap_caps);
	request->auid = tsk_strdup2(context->auids[ietf_xcap_caps].name);

	if((error = txc_xcap_perform(context, request, op_fetch, sl_document)) || !TXC_HTTP_SUCCESS(request))
	{
		printf("test_http_caps/// (get(xcap-caps)):%d status:%d\n", error, request->status);
		ret = -1;
		goto bail; /* xcap-caps is mandatory */
	}
	else if(request->content)
	{
		/* update available auids */
		xcap_caps = txc_xcap_caps_create(request->content->data, request->content->size);
		txc_context_update_available_auids(context, xcap_caps->auids);
		TXC_REQUEST_SAFE_FREE(request);
	}
	else 
	{	
		ret = -2;
		goto bail;
	}

bail:
	
	TXC_REQUEST_SAFE_FREE(request);
	txc_xcap_caps_free(&xcap_caps);

	return ret;
}


/*== testing oma directory
- get document from the server
- update document names
===*/
int test_http_omadir(txc_context_t *context)
{
	txc_request_t* request = 0;
	txc_omadir_t* omadir = 0;
	int ret = 0, error = 0;

	printf("---\nTEST_HTTP_OMADIR\n---\n");

	/* check if oma-dir is supported by the xdms */
	if(!txc_auid_is_available(context->auids, oma_directory))
	{
		printf("test_http_omadir/// oma-directory not available\n");
		return 0;
	}

	TXC_REQUEST_CREATE(request);

	/* create request */
	request->url = TXC_DOC_GET_SEL(context, oma_directory);
	request->auid = tsk_strdup2(context->auids[oma_directory].name);

	/*== get content from the xdms and update document names ==*/
	if((error = txc_xcap_perform(context, request, op_fetch, sl_document)) || !TXC_HTTP_SUCCESS(request))
	{
		printf("test_http_omadir/// oma-directory error:%d status:%d\n", error, request->status);
		ret = -1; goto bail;
	}
	else if(request->content)
	{
		/* update document names */
		omadir = txc_omadir_create(request->content->data, request->content->size);
		/* TODO */		
		ret = 0;
	}
	else goto bail;
		
bail:
	TXC_REQUEST_SAFE_FREE(request);
	txc_omadir_free(&omadir);
	
	return ret;
}

/*== testing resource-lists
- get resource-lists documents
- if the document does not exist create it
- add new entry
- add new list
===*/
int test_http_rlist(txc_context_t* context)
{
	txc_request_t* request = 0;
	int ret = 0, error = 0;

	printf("---\nTEST_HTTP_RLIST\n---\n");

	/* check if oma-dir is supported by the xdms */
	if(!txc_auid_is_available(context->auids, ietf_resource_lists))
	{
		printf("test_http_rlist/// resource-lists not available\n");
		return 0;
	}

	TXC_REQUEST_CREATE(request);
	
	/* create request */
	request->url = TXC_DOC_GET_SEL(context, ietf_resource_lists);
	request->auid = tsk_strdup2(context->auids[ietf_resource_lists].name);

	/*== get content from the xdms and update document names ==*/
	if((error = txc_xcap_perform(context, request, op_fetch, sl_document)) || !TXC_HTTP_SUCCESS(request))
	{
		if(request->status == 404)
		{
			/* create new resource-lists document as per gsma rcs specs and PUT it to the xdms */
			txc_rlist_list2_L_t* rlist2 = txc_gsmarcs_create_rlist2(context);
			char *rlist2_str = txc_rlist_list2_L_serialize(rlist2);
			txc_content_set(request->content, (const char*)rlist2_str, strlen(rlist2_str), 0);/* set content */
			TSK_FREE(rlist2_str);
			TSK_LIST_SAFE_FREE(rlist2);

			/* send content to the xdms */
			if((error = txc_xcap_perform(context, request, op_create, sl_document)) || !TXC_HTTP_SUCCESS(request))
			{
				/* we cannot create the document ==> set the corresponding auid as unavailable */
				 txc_auid_set_availability(context->auids, ietf_resource_lists, 0);
				 ret = -1; goto bail;
			}
		}
		else
		{
			printf("test_http_rlist/// resource-lists error:%d status:%d\n", error, request->status);
			ret = -2; goto bail;
		}
	}
	else if(request->content)
	{
		/*== deserialize the document ==*/
		{
			txc_rlist_t* rlist = txc_rlist_create(request->content->data, request->content->size);
			txc_rlist_listx_L_t* list2 = 0;
			if(!rlist) { ret = -3; goto bail; }
			
			/* get all lists */
			if(list2 = txc_rlist_get_all_lists(rlist, 1))
			{
				if(!tsk_list_find_item(list2, find_list_by_name, TXC_RNAME_GSMA_RCS_RCS))
				{
					test_http_add_list(context, TXC_RNAME_GSMA_RCS_RCS, TXC_RNAME_GSMA_RCS_RCS, TXC_RNAME_GSMA_RCS_RCS);
				}

				if(!tsk_list_find_item(list2, find_list_by_name, TXC_RNAME_OMA_BUDDYLIST))
				{
					test_http_add_list(context, TXC_RNAME_OMA_BUDDYLIST, TXC_RNAME_OMA_BUDDYLIST, TXC_RNAME_OMA_BUDDYLIST);
				}
				// To be continued ...

				TSK_LIST_SAFE_FREE(list2);
			}
			txc_rlist_free(&rlist);
		}
	}
	else 
	{
		ret = -20; goto bail;
	}
	
	TXC_REQUEST_SAFE_FREE(request);
	TXC_REQUEST_CREATE(request);

	/*== add/remove entry ==*/
	{
		char* entry_str = 0;
		request->url = txc_node_get_sel(context, ietf_resource_lists,
				TXC_NODE_SELECT_BY_ATTR("list", "name", "all"),
				TXC_NODE_SELECT_BY_ATTR("entry", "uri", "sip:mercuro2@colibria.com"),
				TXC_NODE_SELECT_END()
				);
		entry_str = txc_rlist_entry_serialize2("sip:mercuro2@colibria.com", "mercuro2");
		txc_content_set(request->content, entry_str, strlen(entry_str), 0);
		TSK_FREE(entry_str);
		
		if((error = txc_xcap_perform(context, request, op_create, sl_element)) || !TXC_HTTP_SUCCESS(request))
		{	/* add */
			printf("test_http_rlist/// failed to add an entry error:%d status:%d\n", error, request->status);
			ret = -4; goto bail;
		}
		TXC_CONTENT_SAFE_FREE(request->content);
		if((error = txc_xcap_perform(context, request, op_delete, sl_element)) || !TXC_HTTP_SUCCESS(request))
		{	/* remove */
			printf("test_http_rlist/// failed to remove an entry error:%d status:%d\n", error, request->status);
			ret = -5; goto bail;
		}
	}

	TXC_REQUEST_SAFE_FREE(request);
	TXC_REQUEST_CREATE(request);

	/** add list **/
	{
		char* list_str = 0;
		request->url = txc_node_get_sel(context, ietf_resource_lists,
				TXC_NODE_SELECT_BY_ATTR("list", "name", "newlist"),
				TXC_NODE_SELECT_END()
				);
		list_str = txc_rlist_list_serialize2("newlist", "newlist", 0);
		txc_content_set(request->content, list_str, strlen(list_str), 0);
		TSK_FREE(list_str);
		
		/* send the request to the server */
		if((error = txc_xcap_perform(context, request, op_create, sl_element)) || !TXC_HTTP_SUCCESS(request))
		{
			printf("test_http_rlist/// failed to add a list error:%d status:%d\n", error, request->status);
			ret = -6; goto bail;
		}
	}

		
bail:
	TXC_REQUEST_SAFE_FREE(request);
	
	return ret;
}

/* Test all http heatures */
int test_http()
{
	int ret = 0;
	txc_context_t* context = 0;

	TXC_CONTEXT_CREATE(context);
	TEST_XXXX_CONTEXT_INIT(context);

	/* xcap-caps */
#if HTTP_RUN_TEST_XCAP_CAPS || HTTP_RUN_TEST_ALL
	if(ret = test_http_caps(context)) return ret;
#endif

	/* oma-dir */
#if HTTP_RUN_TEST_OMADIR || HTTP_RUN_TEST_ALL
	if(ret = test_http_omadir(context)) return ret;
#endif

	/* resource-lists */
#if HTTP_RUN_TEST_RLIST || HTTP_RUN_TEST_ALL
	if(ret = test_http_rlist(context)) return ret;
#endif

	TXC_CONTEXT_SAFE_FREE(context);
	return ret;
}

#endif /* TEST_HTTP_H */