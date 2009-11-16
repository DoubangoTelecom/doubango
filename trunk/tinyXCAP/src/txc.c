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
* - @ref txc_ietf_pres_rules_page
* - @ref txc_oma_pres_rules_page
* - @ref txc_ietf_directory_page
* - @ref txc_oma_directory_page
* - @ref txc_gsma_rcs_page
* - @ref txc_oma_prescont_page
*
* @par Compliance
*
*==== <span style="text-decoration:underline;"> IETF</span> ====
*
* - <a href="http://www.ietf.org/rfc/rfc4745.txt"> RFC 4745 - Common Policy: A Document Format for Expressing Privacy Preferences </a>
* - <a href="http://www.ietf.org/rfc/rfc4825.txt"> RFC 4825 - The Extensible Markup Language (XML) Configuration Access Protocol (XCAP)</a>
* - <a href="http://www.ietf.org/rfc/rfc4826.txt"> RFC 4826 - Extensible Markup Language (XML) Formats for Representing Resource Lists</a>
* - <a href="http://www.ietf.org/rfc/rfc4827.txt"> RFC 4827 - An Extensible Markup Language (XML) Configuration Access Protocol (XCAP) Usage for Manipulating Presence Document Contents</a>
* - <a href="http://www.ietf.org/rfc/rfc5025.txt"> RFC 5025 - Presence Authorization Rules</a>
*
*==== <span style="text-decoration:underline;"> OMA</span> ====
*
* - <a href="http://www.openmobilealliance.org/Technical/release_program/docs/XDM/V1_1-20080627-A/OMA-TS-XDM_Core-V1_1-20080627-A.pdf">OMA-TS-XDM_Core-V1_1-20080627-A</a>
* - <a href="http://www.openmobilealliance.org/Technical/release_program/docs/XDM_PRS_IMPL/V1_0-20080627-C/OMA-WP-XDM_1_1_Implementation_Guidelines-20080627-C.pdf">OMA-WP-XDM_1_1_Implementation_Guidelines-20080627-C</a>
* - <a href="http://www.openmobilealliance.org/technical/release_program/docs/PresenceSIMPLE/V2_0-20081223-C/OMA-TS-Presence_SIMPLE_Content_XDM-V1_0-20081223-C.pdf">OMA-TS-Presence-SIMPLE_Content_XDM-V1_0-20081223-C</a>
* - <a href="http://www.openmobilealliance.org/Technical/release_program/docs/PresenceSIMPLE/V1_1-20080627-A/OMA-TS-Presence_SIMPLE_XDM-V1_1-20080627-A.pdf">OMA-TS-Presence_SIMPLE_XDM-V1_1-20080627-A</a>
*
*====<span style="text-decoration:underline;"> GSMA RCS</span> ====
*
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
* - <a href="#faq7">7. How to create/deserialize an OMA XCAP directory document received from the XDMS?</a>
* - <a href="#faq20">20. How to create and serialize GSMA RCS documents?</a>
* - <a href="#faq20">21. How to parse an OMA pres-content (avatar) document received from the XDMS?</a>
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
*
* @anchor faq7
* @par 7. How to create/deserialize an OMA XCAP directory document received from the XDMS?
* For more information about how to handle OMA XCAP directory documents please refer to @ref txc_oma_directory_page.
*
* @anchor faq20
* @par 20. How to create and serialize GSMA RCS documents?
* For more information about how to handle GSAM RCS documents please refer to @ref txc_gsma_rcs_page.
`*
* @anchor faq21
* @par 21. How to parse an OMA pres-content (avatar) document received from the XDMS?
* For more information about how to handle OMA Presence Content documents (Avatars) please refer to @ref txc_oma_prescont_page.
*/



/**@defgroup txc_group GSMA RCS phase 1 & 2
*/

static void* curl_async_process(void* arg);

#define PANIC_AND_JUMP(code, request)\
	{\
	request->panic = code; \
	goto bail;\
	}

/*== List of all supported auids.
*/
static const AUIDS_T txc_auids =
{
	{ietf_xcap_caps,		TXC_AUID_CAPS,								TXC_MIME_TYPE_CAPS,							TXC_DOCUMENT_CAPS,			1},
	{ietf_resource_lists,	TXC_AUID_RLIST,								TXC_MIME_TYPE_RLIST,						TXC_DOCUMENT_RLIST,			0},
	{ietf_rls_services,		TXC_AUID_RLS,								TXC_MIME_TYPE_RLS,							TXC_DOCUMENT_RLS,			0},
	{ietf_pres_rules,		TXC_AUID_IETF_PRESRULES,					TXC_MIME_TYPE_IETF_PRESRULES,				TXC_DOCUMENT_IETF_PRESRULES,		0},
	{ietf_directory,		TXC_AUID_IETF_DIRECTORY,					TXC_MIME_TYPE_IETF_DIRECTORY,				TXC_DOCUMENT_IETF_DIRECTORY,	0},

	{oma_conv_history,		"org.openmobilealliance.conv-history",		"application/vnd.oma.im.history-list+xml",	"conv-history",		0},
	{oma_pres_rules,		TXC_AUID_OMA_PRESRULES,						TXC_MIME_TYPE_OMA_PRESRULES,				TXC_DOCUMENT_OMA_PRESRULES,		0},
	{oma_directory,			TXC_AUID_OMA_DIRECTORY,						TXC_MIME_TYPE_OMA_DIRECTORY,				TXC_DOCUMENT_OMA_DIRECTORY,	0},
	{oma_deferred_list,		"org.openmobilealliance.deferred-list",		"application/vnd.oma.im.deferred-list+xml", "deferred-list",	0},
	{oma_pres_content,		TXC_AUID_OMA_PRESCONT,						TXC_MIME_TYPE_OMA_PRESCONT,					TXC_DOCUMENT_OMA_PRESCONT,	0},
	{oma_shared_groups,		"org.openmobilealliance.groups",			"application/vnd.oma.poc.groups+xml",		"index"}
};

/*== Internal function to get the content type */
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
			return auid ? auid->content_type : "application/unknown+xml";
		}
	case sl_attribute:return TXC_MIME_TYPE_ATTRIBUTE;
	default: return "application/unknown+xml";
	}/* switch */
}

/**@ingroup txc_group
* Internal function to initialize a previously create XCAP content.
* You MUST use @ref TXC_CONTENT_CREATE to create and initialize you content.
* @param content The content to initialize
* @sa @ref TXC_CONTENT_CREATE
*/
void txc_content_init(txc_content_t *content)
{
}

/**@ingroup txc_group
* Update the XCAP content values.
* @param content The content to update
* @param data The new data
* @param size <the data size
* @param type The content type
*/
void txc_content_set(txc_content_t* content, const char *data, size_t size, const char *type)
{
	if(content)
	{
		tsk_strupdate2(&(content->data), data);
		tsk_strupdate2(&(content->type), type);
		content->size = size;
	}
}

/**@ingroup txc_group
* Internal function to free an XCAP content previously created using @ref TXC_CONTENT_CREATE.
* You MUST call @ref TXC_CONTENT_SAFE_FREE to safely free your content.
* @param content The content to free.
* @sa @ref TXC_CONTENT_SAFE_FREE
*/
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

/**@ingroup txc_group
* Internal function to initialize an previously created auid.
* You MUST call @ref TXC_AUID_CREATE to initialize an create an auid.
* @param auid The auid to initialize.
*/
void txc_auid_init(txc_auid_t *auid)
{
}

/**@ingroup txc_group
* Find an auid by type.
* @param auids List of all available auids into which to search.
* @param auid_type The type of the auid to find.
* @retval A matching auid.
*/
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

/**@ingroup txc_group
* Find an auid by name.
* @param auids List of all available auids into which to search.
* @param name The name of the auid to find.
* @retval A matching auid.
*/
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

/**@ingroup txc_group
* Check if an auid is available. An auid is available if it's supported by the XDMS.
* You MUST use @ref txc_auid_set_availability to update the auid availabilty.
* @param auids The list of auids to loop throught
* @param type The type of the auid for which to test availability
* @retval Returns 1 if availabe and 0 otherwise 
*/
int txc_auid_is_available(const AUIDS_T auids, xcap_auid_type_t type)
{
	const txc_auid_t *auid = txc_auid_findby_type(auids, type);
	return (auid && auid->available) ? 1 : 0;
}

/**@ingroup txc_group
* Changes the auid availability.
* An auid is available if it's supported by the XDMS.
* To get the auids supported by the XDMS you MUST get the xcap-caps document (HTTP GET) using @ref txc_xcap_perform function.
* @param auids The list of auids to loop throught
* @param type The type of the auid to update
* @param avail The new availability
* @sa @ref txc_auid_is_available
*/
void txc_auid_set_availability(AUIDS_T auids, xcap_auid_type_t type, int avail)
{
	const txc_auid_t* auid = txc_auid_findby_type(auids, type);
	if(auid)
	{
		((txc_auid_t*)auid)->available = avail;
	}
}

/**@ingroup txc_group
* Changes the document name associated to an AUID.
* @param context The xcap context created using @ ref TXC_CONTEXT_CREATE
* @param type The type of the auid to update
* @param document The new document name
*/
void txc_auid_update(txc_context_t* context, xcap_auid_type_t type, const char* document)
{
	const txc_auid_t* auid = txc_auid_findby_type(context->auids, type);
	if(auid)
	{
		tsk_strupdate(&context->heap, &(((txc_auid_t*)auid)->document), document);
	}
}

/**@ingroup txc_group
* Internal function called to free an auid previously created using @ref TXC_AUID_CREATE.
* You MUST call @ref TXC_AUID_SAFE_FREE to free an auid.
* @param _auid The auid to free.
* @sa @ref TXC_AUID_SAFE_FREE
*/
void txc_auid_free(void **_auid)
{
	txc_auid_t **auid = (txc_auid_t **)_auid;
	TSK_FREE((*auid)->name);
	TSK_FREE((*auid)->content_type);
	TSK_FREE((*auid)->document);

	tsk_free2(_auid);
}

/**@ingroup txc_group
* Internal function to initialize an XCAP context.
* You MUST use @ref TXC_CONTEXT_CREATE to create an initialize a context.
* @param context The XCAP context to initialize. 
* @sa @ref TXC_CONTEXT_CREATE
*/
void txc_context_init(txc_context_t* context)
{
	int i;

	/* initialize the context's memory heap */
	tsk_heap_init(&context->heap);

	/* copy all default auids */
	for(i = 0; i< (sizeof(AUIDS_T)/sizeof(txc_auid_t)); i++)
	{
		context->auids[i].type = txc_auids[i].type;
		context->auids[i].available = txc_auids[i].available;
		context->auids[i].content_type = tsk_strdup(&context->heap, txc_auids[i].content_type);
		context->auids[i].document = tsk_strdup(&context->heap, txc_auids[i].document);
		context->auids[i].name = tsk_strdup(&context->heap, txc_auids[i].name);
	}

	/* initialize libcurl */
#if defined(WIN32) || defined(_WIN32) || defined(_WIN32_WCE)
	curl_global_init(CURL_GLOBAL_WIN32);
#else
	curl_global_init();
#endif

	/* Init Curl multihandle */
	context->multihandle = curl_multi_init();

	/* Pthread semaphore + thread */
	TSK_SEMAPHORE_CREATE(context->semaphore);
	context->running = 1;
	tsk_thread_create(&(context->tid), curl_async_process, context);
}

/**@ingroup txc_group
* Update available auids using those provided by the XDMS.
* @param context The XCAP context holding the auids to update.
* @param avail_auids The list of auids to use.
*/
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

/**@ingroup txc_group
* Internal function used to free an XCAP context previously created using @ref TXC_CONTEXT_CREATE.
* You MUST use @ref TXC_CONTEXT_SAFE_FREE to safely free a context.
* @param context The context to free.
* @sa @ref TXC_CONTEXT_SAFE_FREE
*/
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

	/* Pthread semaphore + static thread */
	(*context)->running = 0;
	tsk_semaphore_increment((*context)->semaphore);
	tsk_thread_join(&((*context)->tid));
	TSK_SEMAPHORE_SAFE_FREE((*context)->semaphore);

	/* cleanup the heap */
	tsk_heap_cleanup(&((*context)->heap));

	/* cleanup multihandle*/
	curl_multi_cleanup((*context)->multihandle);

	/* free the context */
	tsk_free2(context);

	/* cleanup libcurl */
	curl_global_cleanup();
}

/**@ingroup txc_group
* Internal function to initialize an XCAP request.
* You MUST use @ref TXC_REQUEST_CREATE to create and initialize an XCAP request.
* @param request The request to initialize.
* @sa @ref TXC_REQUEST_CREATE
*/
void txc_request_init(txc_request_t* request)
{
	TXC_CONTENT_CREATE(request->content);

	request->status = 0;
	request->panic = xpa_success;
	request->timeout = TXC_HTTP_DEFAULT_TIMEOUT;
}

/**@ingroup txc_group
* Internal function to free an XCAP request previously created using @ref TXC_REQUEST_CREATE.
* You MUST use @ref TXC_REQUEST_SAFE_FREE to free an XCAP request.
* @param request The XCAP request to free.
* @sa @ref TXC_REQUEST_SAFE_FREE
**/
void txc_request_free(txc_request_t** request)
{
	TSK_FREE((*request)->auid);
	TSK_FREE((*request)->url);
	TSK_FREE((*request)->http_accept);
	TSK_FREE((*request)->http_expect);
	TXC_CONTENT_SAFE_FREE((*request)->content);
	
	/* Curl easyhandle cleanup */
	curl_easy_cleanup((*request)->easyhandle);
	
	tsk_free2(request);
}

/*== libcurl write callback */
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

/*== libcurl read callback */
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

/*== ioctl function to handle HTTP PUT or POST with a multi-pass authentication method */
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

CURLcode txc_easyhandle_init(txc_context_t* context, txc_request_t* request, txc_oper_type_t oper, txc_selection_type_t sel)
{
	CURLcode code = CURLE_OK;
	struct curl_slist *headers = 0;
	char* temp_str = 0;
	tsk_heap_t heap;

	if(request->easyhandle) curl_easy_cleanup(request->easyhandle);

	request->easyhandle = curl_easy_init();
	tsk_heap_init(&heap);

#if DEBUG || _DEBUG
		curl_easy_setopt(request->easyhandle, CURLOPT_VERBOSE, 1);
#endif

	/* set xcap URL */
	if(request->url && (code=curl_easy_setopt(request->easyhandle, CURLOPT_URL, request->url)))
	{
		PANIC_AND_JUMP(xpa_libcurl_error, request)
	}

	/* set request timeout */
	if(code=curl_easy_setopt(request->easyhandle, CURLOPT_TIMEOUT, request->timeout/1000))
	{
		PANIC_AND_JUMP(xpa_libcurl_error, request)
	}

	/* set user name */
	if(context->xui && (code=curl_easy_setopt(request->easyhandle, CURLOPT_USERNAME, context->xui)))
	{
		PANIC_AND_JUMP(xpa_libcurl_error, request)
	}

	/* set user password */
	if(context->password && (code=curl_easy_setopt(request->easyhandle, CURLOPT_PASSWORD, context->password)))
	{
		PANIC_AND_JUMP(xpa_libcurl_error, request)
	}

	/* authentication type */
	if((code=curl_easy_setopt(request->easyhandle, CURLOPT_HTTPAUTH, CURLAUTH_ANY)) ||
		(code=curl_easy_setopt(request->easyhandle, CURLOPT_PROXYAUTH, CURLAUTH_ANY)))
	{
			PANIC_AND_JUMP(xpa_libcurl_error, request)
	}
	
	/* set user-agent */
	if(context->user_agent && (code=curl_easy_setopt(request->easyhandle, CURLOPT_USERAGENT, context->user_agent)))
	{
		PANIC_AND_JUMP(xpa_libcurl_error, request)
	}

	/* X-3GPP-Intended-Identity */
	tsk_sprintf(&heap, &temp_str, "X-3GPP-Intended-Identity: \"%s\"", context->xui);
	headers = curl_slist_append(headers, temp_str);

	/* Content-Type */
	tsk_sprintf(&heap, &temp_str, "Content-Type: %s", get_mime_type(context, request, sel));
	headers = curl_slist_append(headers, temp_str);
	
	/* set proxy host */
	if(context->proxy_host && (code=curl_easy_setopt(request->easyhandle, CURLOPT_PROXY, context->proxy_host)))
	{
		PANIC_AND_JUMP(xpa_libcurl_error, request)
	}

	/* set proxy port */
	if(context->proxy_port && (code=curl_easy_setopt(request->easyhandle, CURLOPT_PROXYPORT, context->proxy_port)))
	{
		PANIC_AND_JUMP(xpa_libcurl_error, request)
	}

	/* set proxy usr*/
	if(context->proxy_usr && (code=curl_easy_setopt(request->easyhandle, CURLOPT_PROXYUSERNAME, context->proxy_usr)))
	{
		PANIC_AND_JUMP(xpa_libcurl_error, request)
	}

	/* set proxy pwd */
	if(context->proxy_pwd && (code=curl_easy_setopt(request->easyhandle, CURLOPT_PROXYPASSWORD, context->proxy_pwd)))
	{
		PANIC_AND_JUMP(xpa_libcurl_error, request)
	}

	/* http proxy tunneling? */
	if(context->proxy_tunneling && (code=curl_easy_setopt(request->easyhandle, CURLOPT_HTTPPROXYTUNNEL, 1L)))
	{
		PANIC_AND_JUMP(xpa_libcurl_error, request)
	}

	/* Pragma */
	if(context->pragma)
	{
		tsk_sprintf(&heap, &temp_str, "Pragma: %s", context->pragma);
		headers = curl_slist_append(headers, temp_str);
	}

	/* Accept */
	if(request->http_accept)
	{
		tsk_sprintf(&heap, &temp_str, "Accept: \"%s\"", request->http_accept);
		headers = curl_slist_append(headers, temp_str);
	}
	else headers = curl_slist_append(headers, "Accept:");
	

	/* Expect */
	if(request->http_expect)
	{
		tsk_sprintf(&heap, &temp_str, "Expect: %s", request->http_expect);
		headers = curl_slist_append(headers, temp_str);
	}
	
	/* pass our list of custom made headers */
	if(code=curl_easy_setopt(request->easyhandle, CURLOPT_HTTPHEADER, headers))
	{
		PANIC_AND_JUMP(xpa_libcurl_error, request)
	}

	/*== */
	switch(oper)
	{
		case op_fetch:
			{	/* GET */
				curl_easy_setopt(request->easyhandle, CURLOPT_CUSTOMREQUEST, "GET");

				/* tell libcurl to pass all data to this function */
				if(code=curl_easy_setopt(request->easyhandle, CURLOPT_WRITEFUNCTION, write_data))
				{
					PANIC_AND_JUMP(xpa_libcurl_error, request)
				}
				
				/* set data for our callback  */
				if(code=curl_easy_setopt(request->easyhandle, CURLOPT_WRITEDATA, (request->content)))
				{
					PANIC_AND_JUMP(xpa_libcurl_error, request)
				}
					
				break;
			}

		case op_create:
		case op_replace:
			{	/* PUT */
				curl_easy_setopt(request->easyhandle, CURLOPT_CUSTOMREQUEST, "PUT");

				/* read callback */
				if(code=curl_easy_setopt(request->easyhandle, CURLOPT_READFUNCTION, read_data))
				{
					PANIC_AND_JUMP(xpa_libcurl_error, request)
				}
				if(code=curl_easy_setopt(request->easyhandle, CURLOPT_UPLOAD, 1L))
				{
					PANIC_AND_JUMP(xpa_libcurl_error, request)
				}
				
				/* content */
				if(request->content && request->content->data && request->content->size)
				{
					/* content data*/
					if(code=curl_easy_setopt(request->easyhandle, CURLOPT_READDATA , (request->content)))
					{
						PANIC_AND_JUMP(xpa_libcurl_error, request)
					}

					/* content length */
					tsk_sprintf(&heap, &temp_str, "Content-Length: %u", request->content->size);
					headers = curl_slist_append(headers, temp_str);
					
					/* ioctl function callback */
					if(code=curl_easy_setopt(request->easyhandle, CURLOPT_IOCTLFUNCTION , ioctl_callback))
					{
						PANIC_AND_JUMP(xpa_libcurl_error, request)
					}
					
					/* ioctl data */
					if(code=curl_easy_setopt(request->easyhandle, CURLOPT_IOCTLDATA , (request->content)))
					{
						PANIC_AND_JUMP(xpa_libcurl_error, request)
					}
				}
				break;
			}

		case op_delete:
			{	/* DELETE */
				 curl_easy_setopt(request->easyhandle, CURLOPT_CUSTOMREQUEST, "DELETE"); 
				break;
			}
	}/* switch */
		
bail:

	/* free the header list */
	//--curl_slist_free_all(headers);
	tsk_heap_cleanup(&heap);

	return code;
}


static void* curl_async_process(void* arg)
{
	int running_handles = 0;
	int rc = 0;
	struct timeval timeout;

	txc_context_t *context = (txc_context_t*)arg;
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;

	while(context && context->multihandle && context->running) 
	{
		fd_set read_fd_set, write_fd_set, exc_fd_set;
		int max_fd;

		if(!running_handles)
		{
			tsk_semaphore_decrement(context->semaphore);

			if(!context->running) break;
		}

		while(CURLM_CALL_MULTI_PERFORM == curl_multi_perform(context->multihandle, &running_handles));
		
		while(running_handles && context->running) 
		{
			FD_ZERO(&read_fd_set);
			FD_ZERO(&write_fd_set);
			FD_ZERO(&exc_fd_set);

			curl_multi_fdset(context->multihandle, &read_fd_set, &write_fd_set, &exc_fd_set, &max_fd);

			rc = select(max_fd+1, &read_fd_set, &write_fd_set, &exc_fd_set, &timeout);

			switch(rc) 
			{
				case -1:
				case 0:
					running_handles = 0;
					break;
				default:
					while(CURLM_CALL_MULTI_PERFORM == curl_multi_perform(context->multihandle, &running_handles));
			} /* switch(rc)  */

		} /* while(running_handles) */

	} /* while(context && context->multihandle && context->running)  */

#if defined(DEBUG) || defined(_DEBUG)
	printf("CURL_ASYNC_PROCESS::EXIT\n\n");
#endif
	return 0;
}


int txc_xcap_send(txc_context_t* context, txc_request_t* request, txc_oper_type_t oper, txc_selection_type_t sel)
{
	CURLcode easycode = CURLE_OK;
	CURLMcode multicode = CURLM_OK;
	
	/* Initialize the easyhandle */
	if((easycode = txc_easyhandle_init(context, request, oper, sel)))
	{
		return easycode;
	}
	
	/* Add the easy handle */
	if((multicode = curl_multi_add_handle(context->multihandle, request->easyhandle)))
	{
		return multicode;
	}
	else
	{
		tsk_semaphore_increment(context->semaphore);
	}
	
	return 0;
}

//
///**@ingroup txc_group
//* Send HTTP request (GET, PUT or DELETE) to the XDMS in order to remove, change or query the user documents.
//* @param context The XCAP context to use. This context hold information about the user preferences and connection information.
//* @param request The request to send to the XDMS. The request MUST contain the destination uri.
//* For more information about how to get this information please see @ref TXC_DOC_GET_SEL or @ref TXC_DOC_GET_SEL2 or @ref txc_node_get_cust_sel or @ref txc_node_get_sel.
//* @param oper The operation to perform (create, replace, fetch or delete).
//* @param sel The selection type (element, document or attribute).
//* @retval Returns Curl error code (CURLcode). Zero means all is ok.
//* If the return value is nonzero code you should check both the request panic code and the request status code for more information about the error.
//* The request ststus code contains the HHTP status (1xx, 2xx, ...) and the request panic code contains the error type (invalid url, invalid xcaproot ....).
//*/
//int txc_xcap_perform(txc_context_t* context, txc_request_t* request, txc_oper_type_t oper, txc_selection_type_t sel)
//{
//	struct curl_slist *headers = 0;
//	CURLcode code = CURLE_OK;
//	char* temp_str = 0;
//	tsk_heap_t heap;
//	
//	tsk_heap_init(&heap);
//
//	/* check context */
//	TXC_CONTEXT_CHECK(context, request->panic);
//
//	/* Get an easy handle */
//	if(!context->easyhandle) context->easyhandle = curl_easy_init();
//	else if(!(context->reuse_http_connection)){
//		curl_easy_cleanup(context->easyhandle);
//		context->easyhandle = curl_easy_init();
//	}
//	if(!context->easyhandle) PANIC_AND_JUMP(xpa_libcurl_error, request)
//
//#if DEBUG || _DEBUG
//		curl_easy_setopt(context->easyhandle, CURLOPT_VERBOSE, 1);
//#endif
//
//	/* set xcap URL */
//	if(request->url && (code=curl_easy_setopt(context->easyhandle, CURLOPT_URL, request->url)))
//		PANIC_AND_JUMP(xpa_libcurl_error, request)
//
//	/* set request timeout */
//	if(code=curl_easy_setopt(context->easyhandle, CURLOPT_TIMEOUT, request->timeout/1000))
//		PANIC_AND_JUMP(xpa_libcurl_error, request)
//
//	switch(oper)
//	{
//		case op_fetch:
//			{	/* GET */
//				curl_easy_setopt(context->easyhandle, CURLOPT_CUSTOMREQUEST, "GET");
//
//				/* tell libcurl to pass all data to this function */
//				if(code=curl_easy_setopt(context->easyhandle, CURLOPT_WRITEFUNCTION, write_data))
//					PANIC_AND_JUMP(xpa_libcurl_error, request)
//				
//				/* set data for our callback  */
//				if(code=curl_easy_setopt(context->easyhandle, CURLOPT_WRITEDATA, (request->content)))
//					PANIC_AND_JUMP(xpa_libcurl_error, request)
//					
//				break;
//			}
//
//		case op_create:
//		case op_replace:
//			{	/* PUT */
//				curl_easy_setopt(context->easyhandle, CURLOPT_CUSTOMREQUEST, "PUT");
//
//				/* read callback */
//				if(code=curl_easy_setopt(context->easyhandle, CURLOPT_READFUNCTION, read_data))
//					PANIC_AND_JUMP(xpa_libcurl_error, request)
//				if(code=curl_easy_setopt(context->easyhandle, CURLOPT_UPLOAD, 1L))
//					PANIC_AND_JUMP(xpa_libcurl_error, request)
//				
//				/* content */
//				if(request->content && request->content->data && request->content->size)
//				{
//					/* content data*/
//					if(code=curl_easy_setopt(context->easyhandle, CURLOPT_READDATA , (request->content)))
//						PANIC_AND_JUMP(xpa_libcurl_error, request)
//
//					/* content length */
//					tsk_sprintf(&heap, &temp_str, "Content-Length: %u", request->content->size);
//					headers = curl_slist_append(headers, temp_str);
//					
//					
//					/* ioctl function callback */
//					if(code=curl_easy_setopt(context->easyhandle, CURLOPT_IOCTLFUNCTION , ioctl_callback))
//						PANIC_AND_JUMP(xpa_libcurl_error, request)
//					
//					/* ioctl data */
//					if(code=curl_easy_setopt(context->easyhandle, CURLOPT_IOCTLDATA , (request->content)))
//						PANIC_AND_JUMP(xpa_libcurl_error, request)
//				}
//				break;
//			}
//
//		case op_delete:
//			{	/* DELETE */
//				 curl_easy_setopt(context->easyhandle, CURLOPT_CUSTOMREQUEST, "DELETE"); 
//				break;
//			}
//	}/* switch */
//
//	/* set user name */
//	if(context->xui && (code=curl_easy_setopt(context->easyhandle, CURLOPT_USERNAME, context->xui)))
//		PANIC_AND_JUMP(xpa_libcurl_error, request)
//
//	/* set user password */
//	if(context->password && (code=curl_easy_setopt(context->easyhandle, CURLOPT_PASSWORD, context->password)))
//		PANIC_AND_JUMP(xpa_libcurl_error, request)
//
//	/* authentication type */
//	if((code=curl_easy_setopt(context->easyhandle, CURLOPT_HTTPAUTH, CURLAUTH_ANY)) ||
//		(code=curl_easy_setopt(context->easyhandle, CURLOPT_PROXYAUTH, CURLAUTH_ANY)))
//			PANIC_AND_JUMP(xpa_libcurl_error, request)
//	
//	/* set user-agent */
//	if(context->user_agent && (code=curl_easy_setopt(context->easyhandle, CURLOPT_USERAGENT, context->user_agent)))
//		PANIC_AND_JUMP(xpa_libcurl_error, request)
//
//	/* X-3GPP-Intended-Identity */
//	tsk_sprintf(&heap, &temp_str, "X-3GPP-Intended-Identity: \"%s\"", context->xui);
//	headers = curl_slist_append(headers, temp_str);
//	
//
//	/* Content-Type */
//	tsk_sprintf(&heap, &temp_str, "Content-Type: %s", get_mime_type(context, request, sel));
//	headers = curl_slist_append(headers, temp_str);
//	
//	
//	/* set proxy host */
//	if(context->proxy_host && (code=curl_easy_setopt(context->easyhandle, CURLOPT_PROXY, context->proxy_host)))
//		PANIC_AND_JUMP(xpa_libcurl_error, request)
//
//	/* set proxy port */
//	if(context->proxy_port && (code=curl_easy_setopt(context->easyhandle, CURLOPT_PROXYPORT, context->proxy_port)))
//		PANIC_AND_JUMP(xpa_libcurl_error, request)
//
//	/* set proxy usr*/
//	if(context->proxy_usr && (code=curl_easy_setopt(context->easyhandle, CURLOPT_PROXYUSERNAME, context->proxy_usr)))
//		PANIC_AND_JUMP(xpa_libcurl_error, request)
//
//	/* set proxy pwd */
//	if(context->proxy_pwd && (code=curl_easy_setopt(context->easyhandle, CURLOPT_PROXYPASSWORD, context->proxy_pwd)))
//		PANIC_AND_JUMP(xpa_libcurl_error, request)
//
//	/* http proxy tunneling? */
//	if(context->proxy_tunneling && (code=curl_easy_setopt(context->easyhandle, CURLOPT_HTTPPROXYTUNNEL, 1L)))
//		PANIC_AND_JUMP(xpa_libcurl_error, request)
//
//	/* Pragma */
//	if(context->pragma)
//	{
//		tsk_sprintf(&heap, &temp_str, "Pragma: %s", context->pragma);
//		headers = curl_slist_append(headers, temp_str);
//		
//	}
//
//	/* Accept */
//	if(request->http_accept)
//	{
//		tsk_sprintf(&heap, &temp_str, "Accept: \"%s\"", request->http_accept);
//		headers = curl_slist_append(headers, temp_str);
//		
//	}
//	else headers = curl_slist_append(headers, "Accept:");
//	
//
//	/* Expect */
//	if(request->http_expect)
//	{
//		tsk_sprintf(&heap, &temp_str, "Expect: %s", request->http_expect);
//		headers = curl_slist_append(headers, temp_str);
//		
//	}
//	
//	/* pass our list of custom made headers */
//	if(code=curl_easy_setopt(context->easyhandle, CURLOPT_HTTPHEADER, headers))
//		PANIC_AND_JUMP(xpa_libcurl_error, request)
//
//	/*** perform operation ***/
//	if(code=curl_easy_perform(context->easyhandle))
//		PANIC_AND_JUMP(xpa_libcurl_error, request)
//	
//	/* get response code */
//	if(code=curl_easy_getinfo(context->easyhandle, CURLINFO_RESPONSE_CODE, &(request->status)))
//		PANIC_AND_JUMP(xpa_libcurl_error, request)
//	
//	/* get response content-type */
//	if(code=curl_easy_getinfo(context->easyhandle, CURLINFO_CONTENT_TYPE, (&temp_str)))
//		PANIC_AND_JUMP(xpa_libcurl_error, request)
//	else if(request->content) request->content->type = tsk_strdup2((const char*)temp_str); 
//		
//bail:
//
//	tsk_heap_cleanup(&heap);
//
//	/* free the header list */
//	curl_slist_free_all(headers); 
//
//	/* cleanup libcurl handle if !stateful*/
//	if(!(context->reuse_http_connection)) 
//		TXC_EASYHANDLE_SAFE_FREE(context->easyhandle);
//
//	return code;
//}