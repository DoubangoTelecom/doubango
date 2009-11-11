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

/**@file txc.c
 * @brief RCS/OMA/IETF XCAP API implementation
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "txc.h"
#include "txc_macros.h"
#include "tsk_macros.h"

/** @mainpage TinySAK API Overview
*
* This file is an overview of TinyXCAP API.
*
* TinyXCAP is a tiny but fully featured RCS/OMA/IETF XCAP API implementation.
* This API is designed to efficiently work on embedded systems whith limited memory
* and low computing power.
*
* @par Getting Started
*
* - @ref txc_faq_page
* -
* - @ref txc_document_page
* - @ref txc_node_page
* -
* - @ref txc_xcap_caps_page 
* - @ref txc_rlist_page
* - @ref txc_rls_page
*
* @par Compliance
*
* - <a href="http://tools.ietf.org/html/rfc4825"> RFC 4825 - The Extensible Markup Language (XML) Configuration Access Protocol (XCAP)</a>
* - <a href="http://www.ietf.org/rfc/rfc4826.txt"> RFC 4826 - Extensible Markup Language (XML) Formats for Representing Resource Lists</a>
* - <a href="http://www.ietf.org/rfc/rfc4827.txt"> RFC 4827 - An Extensible Markup Language (XML) Configuration Access Protocol (XCAP) Usage for Manipulating Presence Document Contents</a>
* - <a href="http://www.ietf.org/rfc/rfc5025.txt"> RFC 5025 - Presence Authorization Rules</a>
* -
* - <a href="http://www.openmobilealliance.org/Technical/release_program/docs/XDM/V1_1-20080627-A/OMA-TS-XDM_Core-V1_1-20080627-A.pdf">OMA-TS-XDM_Core-V1_1-20080627-A</a>
* - <a href="http://www.openmobilealliance.org/Technical/release_program/docs/XDM_PRS_IMPL/V1_0-20080627-C/OMA-WP-XDM_1_1_Implementation_Guidelines-20080627-C.pdf"> OMA-WP-XDM_1_1_Implementation_Guidelines-20080627-C</a>
* - <a href="http://www.openmobilealliance.org/technical/release_program/docs/PresenceSIMPLE/V2_0-20081223-C/OMA-TS-Presence_SIMPLE_Content_XDM-V1_0-20081223-C.pdf"> OMA-TS-Presence-SIMPLE_Content_XDM-V1_0-20081223-C</a>
* -
* - <a href="http://www.gsmworld.com/documents/rcs/rcs2_june09/R2_090831_RCS_Release_2_Technical_Realisation_v1_0.pdf">RCS Release 2 (Jun ‘09) - Technical Realisation v1.0</a>
* - <a href="http://www.gsmworld.com/documents/rcs/rcs1_updates/R1_090831_RCS_Release_1_Technical_Realisation_v1_1.pdf">RCS Release 1 Updates (Jun ‘09) - Technical Realisation v1.1</a>
* - <a href="http://www.gsmworld.com/documents/1_technical_real_v1.pdf">RCS Release 1 (Dec 08)  - Technical Realisation v1.0</a>
*
* @par Supported AUIDs
*
* - xcap-caps
* - resource-lists 
* - rls-services pres-rules 
* - directory 
* - org.openmobilealliance.conv-history 
* - org.openmobilealliance.pres-rules 
* - org.openmobilealliance.xdm-directory 
* - org.openmobilealliance.deferred-list
* - org.openmobilealliance.pres-content 
* - org.openmobilealliance.groups
* - ...
*
* @par Supported Systems
* 
* - AIX
* - Embedded Linux
* - FreeBSD
* - HPUX
* - IRIX
* - Linux
* - LynxOS
* - Mac OS X 
* - Microsoft Windows XP/Vista/7
* - Microsoft Windows CE
* - NetBSD
* - OpenBSD
* - Solaris
* - Symbian S60
*
* @par Supported Architectures
*
* - ARM
* - ARMv4i
* - Intel 32/64-bits
* - Itanium
* - MIPS
* - PA
* - PowerPC
* - RISC
* - SPARC
*
*/


/**@page txc_faq_page Frequently asked questions
*
* - <a href="#faq1">1. How to change default document name?</a>
* - <a href="#faq2">2. How to use document selectors?</a>
* - <a href="#faq3">3. How to use node selectors?</a>
*
* - <a href="#faq4">4. How to create/deserialize an xcap-caps document received from the XDMS?</a>
* - <a href="#faq5">5. How to create/deserialize a resource-lists document received from the XDMS?</a>
* - <a href="#faq6">6. How to create/deserialize a rls-services document received from the XDMS?</a>
*
* @anchor faq1
* @par 1. How to change default document name?
* When the application is initialized all documents are loaded with a default name (e.g. server capabilities (xcap-caps) default document name is 'index').<br>
* It is possible to change these names using @ref txc_auid_update function.
*
* @anchor faq2
* @par 2. How to use document selectors?
* For more information about document selectors please refer to the @ref txc_document_page.
*
* @anchor faq3
* @par 3. How to use node selectors?
* For more information about node selectors please refer to the @ref txc_node_page.
*
* @anchor faq4
* @par 4. How to create/deserialize an xcap-caps document received from the XDMS?
* For more information about how to handle xcap-caps documents please refer to @ref txc_xcap_caps_page.
*
* @anchor faq5
* @par 5. How to create/deserialize a resource-lists document received from the XDMS?
* For more information about how to handle resource-lists documents please refer to @ref txc_rlist_page.
*
* @anchor faq6
* @par 6. How to create/deserialize a rls-services document received from the XDMS?
* For more information about how to handle rls-services documents please refer to @ref txc_rls_page.
*/

#define PANIC_AND_JUMP(code, request)\
	{\
	request->panic = code; \
	goto bail;\
	}

/* get content type */
static const char* get_mime_type(const txc_context_t* context, const txc_request_t* request, txc_selection_type_t sel)
{
	/* return the user provided content type */
	if(request && request->content && request->content->type) return request->content->type;
	
	switch(sel)
	{
	case sl_element:return TXC_MIME_TYPE_ELEMENT;
	case sl_document: 
		{
			const txc_auid_t* auid = txc_auid_findby_name(((txc_context_t*)context)->auids, (const char*)request->auid);
			return auid?auid->content_type:"application/unknown+xml";
		}
	case sl_attribute:return TXC_MIME_TYPE_ATTRIBUTE;
	default: return "application/unknown+xml";
	}/* switch */
}

/* init content */
void txc_content_init(txc_content_t *content)
{
	memset(content, 0, sizeof(txc_content_t));
}

/* set content values */
void txc_content_set(txc_content_t* content, const char *data, size_t size, const char *type)
{
	if(content)
	{
		tsk_strupdate2(&(content->data), data);
		tsk_strupdate2(&(content->type), type);
		content->size = size;
	}
}

/* free content */
void txc_content_free(txc_content_t** content)
{
	if(*content)
	{
		TSK_FREE((*content)->data);
		TSK_FREE((*content)->type);
		
		free(*content);
		(*content) = 0;
	}
}

/* init auid */
void txc_auid_init(txc_auid_t *auid)
{
	memset(auid, 0, sizeof(txc_auid_t));
}

/* find auid object by type */
const txc_auid_t* txc_auid_findby_type(const AUIDS_T auids, xcap_auid_type_t auid_type)
{
	int i;
	for(i=0; i< sizeof(AUIDS_T)/sizeof(txc_auid_t); i++)
	{
		if(auids[i].type == auid_type)
		{
			return &auids[i];
		}
	}
	return 0;
}

/* find auid object by type */
const txc_auid_t* txc_auid_findby_name(const AUIDS_T auids, const char* name)
{
	int i;
	for(i=0; i< sizeof(AUIDS_T)/sizeof(txc_auid_t); i++)
	{
		if(tsk_equals(auids[i].name, name))
		{
			return &auids[i];
		}
	}
	return 0;
}

/* returns 1 if availabe and 0 otherwise */
int txc_auid_is_available(const AUIDS_T auids, xcap_auid_type_t type)
{
	const txc_auid_t *auid = txc_auid_findby_type(auids, type);
	return (auid && auid->available) ? 1 : 0;
}

/* change auid availability */
void txc_auid_set_availability(AUIDS_T auids, xcap_auid_type_t type, int avail)
{
	const txc_auid_t* auid = txc_auid_findby_type(auids, type);
	if(auid)
	{
		((txc_auid_t*)auid)->available = avail;
	}
}

/**
* change the document name of the auid with type= 
*/
void txc_auid_update(txc_context_t* context, xcap_auid_type_t type, const char* document)
{
	const txc_auid_t* auid = txc_auid_findby_type(context->auids, type);
	if(auid)
	{
		tsk_strupdate(&context->heap, &(((txc_auid_t*)auid)->document), document);
	}
}

/* free auid */
void txc_auid_free(void **_auid)
{
	txc_auid_t **auid = (txc_auid_t **)_auid;
	TSK_FREE((*auid)->name);
	TSK_FREE((*auid)->description);
	TSK_FREE((*auid)->content_type);
	TSK_FREE((*auid)->document);

	tsk_free2(_auid);
}

/* initialize xdm context */
/* ATTENTION: context MUST be initialized before use*/
void txc_context_init(txc_context_t* context)
{
	int i;

	memset(context, 0, sizeof(txc_context_t));

	/* initialize the context's memory heap */
	tsk_heap_init(&context->heap);

	/* copy all default auids */
	for(i = 0; i< (sizeof(AUIDS_T)/sizeof(txc_auid_t)); i++)
	{
		context->auids[i].type = txc_auids[i].type;
		context->auids[i].available = txc_auids[i].available;
		context->auids[i].content_type = tsk_strdup(&context->heap, txc_auids[i].content_type);
		context->auids[i].description = tsk_strdup(&context->heap, txc_auids[i].description);
		context->auids[i].document = tsk_strdup(&context->heap, txc_auids[i].document);
		context->auids[i].name = tsk_strdup(&context->heap, txc_auids[i].name);
	}

	/* initialize libcurl */
#ifdef WIN32
	curl_global_init(CURL_GLOBAL_WIN32);
#else
	curl_global_init();
#endif
}

/* update available auids using those provided by the xdms */
void txc_context_update_available_auids(txc_context_t *context, const tsk_list_t* avail_auids)
{
	tsk_list_item_t *item = 0;
	
	if(!context || !avail_auids) return;

	/* avail auids */
	tsk_list_foreach(item, avail_auids)
	{
		/* context auids */
		const txc_auid_t* auid = txc_auid_findby_name(context->auids, ((const char*)item->data));
		if(auid && !(auid->available)) ((txc_auid_t*)auid)->available = 1;
	}
}

/* free a previously initialized context */
void txc_context_free(txc_context_t** context)
{
	TSK_FREE((*context)->txc_root);
	TSK_FREE((*context)->xui);
	TSK_FREE((*context)->password);
	TSK_FREE((*context)->proxy_host);
	TSK_FREE((*context)->proxy_usr);
	TSK_FREE((*context)->proxy_pwd);
	TSK_FREE((*context)->user_agent);
	TSK_FREE((*context)->pragma);
	TXC_EASYHANDLE_SAFE_FREE((*context)->easyhandle);

	/* cleanup the heap */
	tsk_heap_cleanup(&((*context)->heap));

	/* free the context */
	tsk_free2(context);

	/* cleanup libcurl */
	curl_global_cleanup();
}

/* initialize xdm request */
/* ATTENTION: request MUST be initialized before use*/
void txc_request_init(txc_request_t* request)
{
	memset(request, 0, sizeof(txc_request_t));
	TXC_CONTENT_CREATE(request->content);

	request->status = 0;
	request->panic = xpa_success;
	request->timeout = TXC_HTTP_DEFAULT_TIMEOUT;
}

/* free a previously initialized request */
void txc_request_free(txc_request_t** request)
{
	TSK_FREE((*request)->auid);
	TSK_FREE((*request)->url);
	TSK_FREE((*request)->http_accept);
	TSK_FREE((*request)->http_expect);
	TXC_CONTENT_SAFE_FREE((*request)->content);
	
	tsk_free2(request);
}

/* libcurl write callback */
size_t write_data(void *buffer, size_t size, size_t nmemb, txc_content_t *userp)
{
	size_t total_size = (size * nmemb);
	if(!userp) return -1;
	
	/* new chinck ==> realloc */
	userp->data = (char *)realloc(userp->data, (1 + userp->size + total_size));
	if (userp->data)
	{	/* append new chunck */
		memcpy(userp->data + userp->size, buffer, total_size);
		userp->size += total_size;
		userp->data[userp->size] = '\0';
	}
	return total_size;
}

/* libcurl read callback */
size_t read_data(void *buffer, size_t size, size_t nmemb, txc_content_t *userp)
{
	size_t bytes_to_copy = 0;
    if(!userp) return -1;
	
	if(userp->cursor >= userp->size) return 0;

    bytes_to_copy = nmemb < userp->size ? nmemb : userp->size;
    memcpy(buffer, userp->data, bytes_to_copy);
    userp->cursor += bytes_to_copy;
    return bytes_to_copy;
}

/* ioctl function to handle HTTP PUT or POST with a multi-pass authentication method */
curlioerr ioctl_callback(CURL *handle, int cmd, txc_content_t *userp)
{
    switch (cmd) 
	{
    case CURLIOCMD_NOP: return CURLIOE_OK;
    case CURLIOCMD_RESTARTREAD:
		{
			userp->cursor = 0;
			return CURLIOE_OK;
		}
    default: return CURLIOE_UNKNOWNCMD;
    }
}

/* perform xcap operation */
/* returns 0 if success and false otherwise. check request panic code for more information */
/* if request panic code is equal to 'xpa_libcurl_error', then you should check the return code*/
int txc_xcap_perform(txc_context_t* context, txc_request_t* request, txc_oper_type_t oper, txc_selection_type_t sel)
{
	struct curl_slist *headers = 0;
	CURLcode code = CURLE_OK;
	char* temp_str = 0;
	
	/* check context */
	TXC_CONTEXT_CHECK(context, request->panic);

	/* Get an easy handle */
	if(!context->easyhandle) context->easyhandle = curl_easy_init();
	else if(!(context->reuse_http_connection)){
		curl_easy_cleanup(context->easyhandle);
		context->easyhandle = curl_easy_init();
	}
	if(!context->easyhandle) PANIC_AND_JUMP(xpa_libcurl_error, request)

#if DEBUG || _DEBUG
		curl_easy_setopt(context->easyhandle, CURLOPT_VERBOSE, 1);
#endif

	/* set xcap URL */
	if(request->url && (code=curl_easy_setopt(context->easyhandle, CURLOPT_URL, request->url)))
		PANIC_AND_JUMP(xpa_libcurl_error, request)

	/* set request timeout */
	if(code=curl_easy_setopt(context->easyhandle, CURLOPT_TIMEOUT, request->timeout/1000))
		PANIC_AND_JUMP(xpa_libcurl_error, request)

	switch(oper)
	{
		case op_fetch:
			{	/* GET */
				curl_easy_setopt(context->easyhandle, CURLOPT_CUSTOMREQUEST, "GET");

				/* tell libcurl to pass all data to this function */
				if(code=curl_easy_setopt(context->easyhandle, CURLOPT_WRITEFUNCTION, write_data))
					PANIC_AND_JUMP(xpa_libcurl_error, request)
				
				/* set data for our callback  */
				if(code=curl_easy_setopt(context->easyhandle, CURLOPT_WRITEDATA, (request->content)))
					PANIC_AND_JUMP(xpa_libcurl_error, request)
					
				break;
			}

		case op_create:
		case op_replace:
			{	/* PUT */
				curl_easy_setopt(context->easyhandle, CURLOPT_CUSTOMREQUEST, "PUT");

				/* read callback */
				if(code=curl_easy_setopt(context->easyhandle, CURLOPT_READFUNCTION, read_data))
					PANIC_AND_JUMP(xpa_libcurl_error, request)
				if(code=curl_easy_setopt(context->easyhandle, CURLOPT_UPLOAD, 1L))
					PANIC_AND_JUMP(xpa_libcurl_error, request)
				
				/* content */
				if(request->content && request->content->data && request->content->size)
				{
					/* content data*/
					if(code=curl_easy_setopt(context->easyhandle, CURLOPT_READDATA , (request->content)))
						PANIC_AND_JUMP(xpa_libcurl_error, request)

					/* content length */
					tsk_sprintf(0, &temp_str, "Content-Length: %u", request->content->size);
					headers = curl_slist_append(headers, temp_str);
					TSK_SAFE_FREE2(temp_str);
					
					/* ioctl function callback */
					if(code=curl_easy_setopt(context->easyhandle, CURLOPT_IOCTLFUNCTION , ioctl_callback))
						PANIC_AND_JUMP(xpa_libcurl_error, request)
					
					/* ioctl data */
					if(code=curl_easy_setopt(context->easyhandle, CURLOPT_IOCTLDATA , (request->content)))
						PANIC_AND_JUMP(xpa_libcurl_error, request)
				}
				break;
			}

		case op_delete:
			{	/* DELETE */
				 curl_easy_setopt(context->easyhandle, CURLOPT_CUSTOMREQUEST, "DELETE"); 
				break;
			}
	}/* switch */

	/* set user name */
	if(context->xui && (code=curl_easy_setopt(context->easyhandle, CURLOPT_USERNAME, context->xui)))
		PANIC_AND_JUMP(xpa_libcurl_error, request)

	/* set user password */
	if(context->password && (code=curl_easy_setopt(context->easyhandle, CURLOPT_PASSWORD, context->password)))
		PANIC_AND_JUMP(xpa_libcurl_error, request)

	/* authentication type */
	if((code=curl_easy_setopt(context->easyhandle, CURLOPT_HTTPAUTH, CURLAUTH_ANY)) ||
		(code=curl_easy_setopt(context->easyhandle, CURLOPT_PROXYAUTH, CURLAUTH_ANY)))
			PANIC_AND_JUMP(xpa_libcurl_error, request)
	
	/* set user-agent */
	if(context->user_agent && (code=curl_easy_setopt(context->easyhandle, CURLOPT_USERAGENT, context->user_agent)))
		PANIC_AND_JUMP(xpa_libcurl_error, request)

	/* X-3GPP-Intended-Identity */
	tsk_sprintf(0, &temp_str, "X-3GPP-Intended-Identity: \"%s\"", context->xui);
	headers = curl_slist_append(headers, temp_str);
	TSK_SAFE_FREE2(temp_str);

	/* Content-Type */
	tsk_sprintf(0, &temp_str, "Content-Type: %s", get_mime_type(context, request, sel));
	headers = curl_slist_append(headers, temp_str);
	TSK_SAFE_FREE2(temp_str);
	
	/* set proxy host */
	if(context->proxy_host && (code=curl_easy_setopt(context->easyhandle, CURLOPT_PROXY, context->proxy_host)))
		PANIC_AND_JUMP(xpa_libcurl_error, request)

	/* set proxy port */
	if(context->proxy_port && (code=curl_easy_setopt(context->easyhandle, CURLOPT_PROXYPORT, context->proxy_port)))
		PANIC_AND_JUMP(xpa_libcurl_error, request)

	/* set proxy usr*/
	if(context->proxy_usr && (code=curl_easy_setopt(context->easyhandle, CURLOPT_PROXYUSERNAME, context->proxy_usr)))
		PANIC_AND_JUMP(xpa_libcurl_error, request)

	/* set proxy pwd */
	if(context->proxy_pwd && (code=curl_easy_setopt(context->easyhandle, CURLOPT_PROXYPASSWORD, context->proxy_pwd)))
		PANIC_AND_JUMP(xpa_libcurl_error, request)

	/* http proxy tunneling? */
	if(context->proxy_tunneling && (code=curl_easy_setopt(context->easyhandle, CURLOPT_HTTPPROXYTUNNEL, 1L)))
		PANIC_AND_JUMP(xpa_libcurl_error, request)

	/* Pragma */
	if(context->pragma)
	{
		tsk_sprintf(0, &temp_str, "Pragma: %s", context->pragma);
		headers = curl_slist_append(headers, temp_str);
		TSK_SAFE_FREE2(temp_str);
	}

	/* Accept */
	if(request->http_accept)
	{
		tsk_sprintf(0, &temp_str, "Accept: \"%s\"", request->http_accept);
		headers = curl_slist_append(headers, temp_str);
		TSK_SAFE_FREE2(temp_str);
	}
	else headers = curl_slist_append(headers, "Accept:");
	

	/* Expect */
	if(request->http_expect)
	{
		tsk_sprintf(0, &temp_str, "Expect: %s", request->http_expect);
		headers = curl_slist_append(headers, temp_str);
		TSK_SAFE_FREE2(temp_str);
	}
	
	/* pass our list of custom made headers */
	if(code=curl_easy_setopt(context->easyhandle, CURLOPT_HTTPHEADER, headers))
		PANIC_AND_JUMP(xpa_libcurl_error, request)

	/*** perform operation ***/
	if(code=curl_easy_perform(context->easyhandle))
		PANIC_AND_JUMP(xpa_libcurl_error, request)
	
	/* get response code */
	if(code=curl_easy_getinfo(context->easyhandle, CURLINFO_RESPONSE_CODE, &(request->status)))
		PANIC_AND_JUMP(xpa_libcurl_error, request)
	
	/* get response content-type */
	if(code=curl_easy_getinfo(context->easyhandle, CURLINFO_CONTENT_TYPE, (&temp_str)))
		PANIC_AND_JUMP(xpa_libcurl_error, request)
	else request->content->type = tsk_strdup2((const char*)temp_str); 
		
bail:

	/* free the header list */
	curl_slist_free_all(headers); 

	/* cleanup libcurl handle if !stateful*/
	if(!(context->reuse_http_connection)) 
		TXC_EASYHANDLE_SAFE_FREE(context->easyhandle);

	return code;
}