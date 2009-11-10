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

#include "txc_document.h"
#include "txc_macros.h"

/* create document selector as per rfc 4825 subclause 6.2*/
/* ATTENTION: the @context MUST be initialized using 'txc_context_init' or created using 'TXC_CONTEXT_CREATE' macro*/
/* ATTENTION: use 'TXC_REQUEST_SAFE_FREE' macro to free the returned object */
txc_request_t* txc_doc_get_sel(const txc_context_t* context, const xcap_auid_type_t auid_type)
{
	txc_request_t* request = 0;
	const txc_auid_t* auid = 0;
	
	/* create request and check context validity*/
	TXC_REQUEST_CREATE(request);
	TXC_CONTEXT_CHECK(context, request->panic);
	TXC_REQUEST_RETURN_IF_PANIC(request);

	/* get associated auid object*/
	auid = txc_auid_findby_type(context->auids, auid_type);
	TXC_AUID_CHECK(auid, request->panic);
	TXC_REQUEST_RETURN_IF_PANIC(request);

	/* auid */
	request->auid = tsk_strdup(0, auid->name);

	/* url */
	if(TXC_DOCUMENT_GLOBAL(auid_type))
	{	/* xdm-root/auid-name/global/doc-name */
		tsk_sprintf(0, &(request->url), "%s/%s/global/%s", context->txc_root, auid->name, auid->document);
	}
	else
	{	/* xdm-root/auid-name/users/xui/doc-name */
		tsk_sprintf(0, &(request->url), "%s/%s/users/%s/%s", context->txc_root, auid->name, context->xui, auid->document);
	}
	
	/* content type */
	//request->content_type = tsk_strdup(auid->content_type);

	return request;
}


/* create document selector as per rfc 4825 subclause 6.2*/
/* ATTENTION: the @context MUST be initialized using 'txc_context_init' or created using 'TXC_CONTEXT_CREATE' macro*/
/* ATTENTION: use 'TSK_SAFE_FREE2' macro to free the returned string */
char* txc_doc_get_sel2(const txc_context_t* context, const xcap_auid_type_t auid_type)
{
	char* sel = 0;
	txc_request_t* request = 0;
	if((request = txc_doc_get_sel(context, auid_type)) && (TXC_PANIC_SUCCESS(request->panic)))
	{
		sel = tsk_strdup(0, request->url);
	}

	TXC_REQUEST_SAFE_FREE(request);
	return sel;
}