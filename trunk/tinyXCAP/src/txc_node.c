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

#include "txc_node.h"
#include "txc_document.h"

#include <stdarg.h>

#define TXC_ELEMENT_CONTENT_TYPE "application/xcap-el+xml" /* RFC 4825 subclause 15.2.1 */

/* create node selector as per rcf 4825 subclause 6.3 */
/* ATTENTION: use 'TXC_REQUEST_SAFE_FREE' macro to free the returned object */
txc_request_t* txc_node_get_sel(const txc_context_t* context, const xcap_auid_type_t auid_type, ...)
{
	txc_request_t* request = 0;
	const txc_auid_t* auid = 0;

	va_list list;
	int step;
	char *result=0, *node_root = 0, *_namespace=0;
	
	/* get document selector */
	request = txc_doc_get_sel(context, auid_type);
	TXC_REQUEST_RETURN_IF_PANIC(request);

	/* append not root*/
	tsk_sprintf(0, &node_root, "/~~/%s", request->auid);
	tsk_strcat(0, &(request->url), node_root);
	TSK_SAFE_FREE2(node_root);
	
	/* initialize variable arguments */
	va_start(list, auid_type);

	while( (step=va_arg(list, txc_node_step_t)) != 0xFF)
	{
		switch(step)
		{
		case by_name:
			{	/* qname */
				const char* qname = va_arg(list, const char*);
				char* selector = 0;
				tsk_sprintf(0, &selector, "/%s", qname);
				tsk_strcat(0, &(request->url), (const char*)selector);
				free(selector);
				break;
			}

		case by_pos:
			{	/* qname, position */
				const char* qname = va_arg(list, const char*);
				int position = va_arg(list, int);
				char* selector = 0;
				tsk_sprintf(0, &selector, "/%s%%5B%d%%5D", qname, position);
				tsk_strcat(0, &(request->url), (const char*)selector);
				free(selector);
				break;
			}

		case by_attr:
			{	/* qname, att_name, att_value */
				const char* qname = va_arg(list, const char*);
				char* att_name = tsk_url_encode(0, va_arg(list, const char*));
				char* att_value = tsk_url_encode(0, va_arg(list, const char*));
				char* selector = 0;
				tsk_sprintf(0, &selector, "/%s%%5B@%s=%%22%s%%22%%5D", qname, att_name, att_value);
				tsk_strcat(0, &(request->url), (const char*)selector);
				free(selector), free(att_name), free(att_value);
				break;
			}

		case by_pos_attr:
			{	/* qname, position, att_name, att_value */
				const char* qname = va_arg(list, const char*);
				int position = va_arg(list, int);
				char* att_name = tsk_url_encode(0, va_arg(list, const char*));
				char* att_value = tsk_url_encode(0, va_arg(list, const char*));
				char* selector = 0;
				tsk_sprintf(0, &selector, "/%s%%5B%d%%5D%%5B@%s=%%22%s%%22%%5D", qname, position, att_name, att_value);
				tsk_strcat(0, &(request->url), (const char*)selector);
				free(selector), free(att_name), free(att_value);
				break;
			}

		case 0xF0:
			{	/* prefix ns */
				const char* prefix = va_arg(list, const char*);
				const char* ns = va_arg(list, const char*);
				char* selector = 0;
				tsk_sprintf(0, &selector, "%sxmlns(%s=%%22%s%%22)", _namespace?"":"%3F",prefix, ns);
				tsk_strcat(0, &_namespace, (const char*)selector);
				free(selector);
				break;
			}
		default: break;

		}/* switch */
	}
	
	/* reset variable arguments */
	va_end(list);
	
	/* add namespace and free it*/
	tsk_strcat(0, &(request->url), (const char*)_namespace);
	TSK_FREE(_namespace);
	
	/* content type */
	//TSK_SAFE_FREE2(request->content_type);
	//request->content_type = tsk_strdup(TXC_ELEMENT_CONTENT_TYPE);

	return request;
}

