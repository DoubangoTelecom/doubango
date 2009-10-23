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

#include "xdm_document.h"
#include "xdm_utils.h"

/* create document selector as per rfc 4825 subclause 6.2*/
/* ATTENTION: the @context MUST be initialized using 'xdm_context_init' or created using 'XDM_CONTEXT_CREATE' macro*/
/* ATTENTION: use 'XDM_REQUEST_SAFE_FREE' macro to free the returned object */
xdm_request_t* xdm_doc_get_sel(const xdm_context_t* context, const xcap_auid_type_t auid_type)
{
	xdm_request_t* request = 0;
	const xdm_auid_t* auid = 0;
	
	/* create request and check context validity*/
	XDM_REQUEST_CREATE(request);
	XDM_CONTEXT_CHECK(context, request->panic);
	XDM_REQUEST_RETURN_IF_PANIC(request);

	/* get associated auid object*/
	auid = xdm_auid_findby_type(context->auids, auid_type);
	XDM_AUID_CHECK(auid, request->panic);
	XDM_REQUEST_RETURN_IF_PANIC(request);

	/* auid */
	request->auid = xdm_strdup(auid->name);

	/* url */
	if(XDM_DOCUMENT_GLOBAL(auid_type))
	{	/* xdm-root/auid-name/global/doc-name */
		xdm_sprintf(&(request->url), "%s/%s/global/%s", context->xdm_root, auid->name, auid->document);
	}
	else
	{	/* xdm-root/auid-name/users/xui/doc-name */
		xdm_sprintf(&(request->url), "%s/%s/users/%s/%s", context->xdm_root, auid->name, context->xui, auid->document);
	}
	
	/* content type */
	//request->content_type = xdm_strdup(auid->content_type);

	return request;
}


/* create document selector as per rfc 4825 subclause 6.2*/
/* ATTENTION: the @context MUST be initialized using 'xdm_context_init' or created using 'XDM_CONTEXT_CREATE' macro*/
/* ATTENTION: use 'XDM_SAFE_FREE' macro to free the returned string */
char* xdm_doc_get_sel2(const xdm_context_t* context, const xcap_auid_type_t auid_type)
{
	char* sel = 0;
	xdm_request_t* request = 0;
	if((request = xdm_doc_get_sel(context, auid_type)) && (XDM_PANIC_SUCCESS(request->panic)))
	{
		sel = xdm_strdup(request->url);
	}

	XDM_REQUEST_SAFE_FREE(request);
	return sel;
}