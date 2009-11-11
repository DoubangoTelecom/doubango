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

#include "test_xcap-caps.h"
#include "test_rls.h"
#include "test_rlist.h"
#include "test_gsma_rcs.h"

#ifndef TEST_XCAP
#define TEST_XCAP

#define TEST_XCAP_CONTEXT_INIT(context) \
	context->user_agent = xdm_strdup("XDM-client/OMA1.1"); \
	context->xdm_root = xdm_strdup("http://192.168.0.14:8080/services"); \
	context->password = xdm_strdup("doubango"); \
	context->xui = xdm_strdup("sip:doubango@example.com"); \
	context->pragma = xdm_strdup("keep-alive");

/* delete all documents */
void testxcap_deleteall(xdm_context_t *context)
{
	xdm_list_item_t* item = 0;
	
	xdm_list_foreach(item, context->auids)
	{
		xdm_auid_t *auid = ((xdm_auid_t*)item->data);
		if(auid->available && auid->type != ietf_xcap_caps)
		{
			xdm_request_t *request= xdm_doc_get_sel(context, auid->type);
			xdm_xcap_perform(context, request, op_delete, sl_document);
			XDM_REQUEST_SAFE_FREE(request);
		}
	}
}

/* test xcap-caps 
	- get xcap-caps documents
	- update available auids
*/
int testxcap_xcap_caps(xdm_context_t *context)
{
	xdm_request_t* request = 0;
	xcap_caps_t* xcap_caps = 0;
	int ret = 0, error = 0;
	
	/* create request */
	request= xdm_doc_get_sel(context, ietf_xcap_caps);
	
	/* ge content from the xdms */
	if((error = xdm_xcap_perform(context, request, op_fetch, sl_document)) || !XDM_HTTP_SUCCESS(request))
	{
		printf("xcap-caps error:%d status:%d\n", error, request->status);
		goto bail; /* xcap-caps is mandatory */
	}
	else if(request->content)
	{
		/* update available auids */
		xcap_caps = xdm_xcap_caps_create(request->content->data, request->content->size);
		xdm_conttext_update_available_auids(context, xcap_caps->auids);

		ret = 1;
	}
	else goto bail;

bail:
	/* free request */
	XDM_REQUEST_SAFE_FREE(request);
	
	/* free xcap-caps */
	xdm_xcap_caps_free(&xcap_caps);

	return ret;
}

/* test oma directory 
	- get oma dir document if supported
	- update documents names
*/
int testxcap_omadir(xdm_context_t *context)
{
	xdm_request_t* request = 0;
	xdm_omadir_t* omadir = 0;
	int ret = 0, error = 0;

	/* check if oma-dir is supported by the xdms */
	if(!xdm_auid_is_available(context->auids, oma_directory))
	{
		printf("oma-directory not available\n");
		return 0;
	}

	/* create request */
	request= xdm_doc_get_sel(context, oma_directory);

	/* ge content from the xdms */
	if((error = xdm_xcap_perform(context, request, op_fetch, sl_document)) || !XDM_HTTP_SUCCESS(request))
	{
		printf("oma-directory error:%d status:%d\n", error, request->status);
		goto bail; /* xcap-caps is mandatory */
	}
	else if(request->content)
	{
		/* update document names */
		omadir = xdm_omadir_create(request->content->data, request->content->size);
		/* TODO */		
		ret = 1;
	}
	else goto bail;
		
bail:
	/* free request */
	XDM_REQUEST_SAFE_FREE(request);

	/* free omadir */
	xdm_omadir_free(&omadir);
	
	return ret;
}

/* test resource-lists 
	- get resource-lists document or create it if does not exist
	- add an entry
	- modify the entry display-name
	- delete the entry
*/
int testxcap_rlist(xdm_context_t *context)
{
	xdm_request_t* request = 0;
	int ret = 0, error = 0;
		
	/* check resource-lists availability*/
	if(!xdm_auid_is_available(context->auids, ietf_resource_lists))
	{
		printf("\nresource-lists not available\n");
		return 0;
	}
	
	/* create our request */
	request = xdm_doc_get_sel(context, ietf_resource_lists);
	
	/* get document from the xdms */
	if((error = xdm_xcap_perform(context, request, op_fetch, sl_document)) || !XDM_HTTP_SUCCESS(request))
	{
		if(request->status == 404)
		{
			/* create new resource-lists document as per gsma rcs specs */
			xdm_rlist_list2_L_t* rlist2 = xdm_gsmarcs_create_rlist2(context);
			char *rlist2_str = xdm_rlist_rlist2_serialize(rlist2);
			xdm_content_set(request->content, (const char*)rlist2_str, strlen(rlist2_str), 0);/* set content */
			XDM_SAFE_FREE(rlist2_str);
			XDM_LIST_SAFE_FREE(rlist2);

			/* send content to the xdms */
			if((error = xdm_xcap_perform(context, request, op_create, sl_document)) || !XDM_HTTP_SUCCESS(request))
			{
				/* we cannot create the document ==> set the corresponding auid as unavailable */
				// xdm_auid_set_availability(context->auids, ietf_resource_lists, 0);
				goto bail;
			}			
		}
		else
		{
			printf("resource-lists error:%d status:%d\n", error, request->status);
			goto bail;
		}
	}
	else if(request->content)
	{
		/* deserialize and dump */
		//test_rlist2(request->content->data, request->content->size);
		
		/** add entry **/
		{
			xdm_request_t *r1 = xdm_node_get_sel(context, ietf_resource_lists,
					XDM_NODE_SELECT_BY_ATTR("list", "name", "all"),
					XDM_NODE_SELECT_BY_ATTR("entry", "uri", "sip:doubango@example.com"),
					XDM_NODE_SELECT_END()
					);
			char* entry_str = xdm_rlist_entry_serialize2("sip:doubango@example.com", "doubango");
			xdm_content_set(r1->content, entry_str, strlen(entry_str), 0);
			XDM_SAFE_FREE(entry_str);
			
			/* send the request to the server */
			if((error = xdm_xcap_perform(context, r1, op_create, sl_element)) || !XDM_HTTP_SUCCESS(r1))
			{
				printf("failed to add an entry error:%d status:%d\n", error, request->status);
				XDM_REQUEST_SAFE_FREE(r1);
				goto bail;
			}
			XDM_REQUEST_SAFE_FREE(r1);
		}

		/** change the entry's display-name **/
		{
			xdm_request_t *r1 = xdm_node_get_sel(context, ietf_resource_lists,
					XDM_NODE_SELECT_BY_ATTR("list", "name", "all"),
					XDM_NODE_SELECT_BY_ATTR("entry", "uri", "sip:doubango@example.com"),
					//XDM_NODE_SELECT_BY_NAME("display-name"),
					XDM_NODE_SELECT_END()
					);
			char* entry_str = xdm_rlist_entry_serialize2("sip:doubango@example.com", "doubango2");
			xdm_content_set(r1->content, entry_str, strlen(entry_str), 0);
			XDM_SAFE_FREE(entry_str);
			
			/* send the request to the server */
			if((error = xdm_xcap_perform(context, r1, op_replace, sl_element)) || !XDM_HTTP_SUCCESS(r1))
			{
				printf("failed to update an entry error:%d status:%d\n", error, request->status);
				XDM_REQUEST_SAFE_FREE(r1);
				goto bail;
			}
			XDM_REQUEST_SAFE_FREE(r1);
		}

		/** delete the entry **/
		{
			xdm_request_t *r1 = xdm_node_get_sel(context, ietf_resource_lists,
					XDM_NODE_SELECT_BY_ATTR("list", "name", "all"),
					XDM_NODE_SELECT_BY_ATTR("entry", "uri", "sip:doubango@example.com"),
					XDM_NODE_SELECT_END()
					);			
			/* send the request to the server */
			if((error = xdm_xcap_perform(context, r1, op_delete, sl_element)) || !XDM_HTTP_SUCCESS(r1))
			{
				printf("failed to delete an entry error:%d status:%d\n", error, request->status);
				XDM_REQUEST_SAFE_FREE(r1);
				goto bail;
			}
			XDM_REQUEST_SAFE_FREE(r1);
		}

		/** add list **/
		{
			xdm_request_t *r1 = xdm_node_get_sel(context, ietf_resource_lists,
					XDM_NODE_SELECT_BY_ATTR("list", "name", "newlist"),
					XDM_NODE_SELECT_END()
					);
			char* list_str = xdm_rlist_list_serialize2("newlist", "newlist");
			xdm_content_set(r1->content, list_str, strlen(list_str), 0);
			XDM_SAFE_FREE(list_str);
			
			/* send the request to the server */
			if((error = xdm_xcap_perform(context, r1, op_create, sl_element)) || !XDM_HTTP_SUCCESS(r1))
			{
				printf("failed to add a list error:%d status:%d\n", error, request->status);
				XDM_REQUEST_SAFE_FREE(r1);
				goto bail;
			}
			XDM_REQUEST_SAFE_FREE(r1);
		}

	}
	else goto bail;

bail:

	/* free request */
	XDM_REQUEST_SAFE_FREE(request);

	return ret;
}

/* testing xcap*/
void test_xcap()
{
	xdm_context_t *context = 0;
	XDM_CONTEXT_CREATE(context);
	TEST_XCAP_CONTEXT_INIT(context);
	context->reuse_http_connection = 1;

	testxcap_xcap_caps(context);
	//testxcap_deleteall(context);
	//testxcap_omadir(context);
	testxcap_rlist(context);

	XDM_CONTEXT_SAFE_FREE(context);
}

#endif /* TEST_XCAP */