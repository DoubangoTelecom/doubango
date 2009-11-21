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

/**@file txc.h
 * @brief RCS/OMA/IETF XCAP API implementation
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */

#ifndef _TINYXCAP_TXC_H_
#define _TINYXCAP_TXC_H_

#include "tinyxcap_config.h"
#include <tsk.h>

#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>

#define TXC_AUIDS_COUNT				11
#define TXC_HTTP_DEFAULT_TIMEOUT	30000
#define TXC_HTTP_SUCCESS(request)	(200==request->status || 202==request->status)

#define TXC_IS_EMPTY_STRING(str) (!str || !strlen(str))

#define TXC_DOCUMENT_GLOBAL(auid_type) (auid_type == ietf_xcap_caps)

#define	TXC_MIME_TYPE_ELEMENT			"application/xcap-el+xml" /**< as per rfc 4825 subclause 15.2.1 */
#define	TXC_MIME_TYPE_ATTRIBUTE			"application/xcap-att+xml" /**< as per rfc 4825 subclause 15.2.2 */
#define	TXC_MIME_TYPE_NS				"application/xcap-ns+xml" /**< as per rfc 4825 subclause 15.2.3 */
#define	TXC_MIME_TYPE_ERROR				"application/xcap-error+xml" /**< as per rfc 4825 subclause 15.2.4 */
#define	TXC_MIME_TYPE_CAPS				"application/xcap-caps+xml" /**< as per rfc 4825 subclause 12.4 */
#define TXC_MIME_TYPE_RLIST				"application/resource-lists+xml" /**< as per rfc 4826 subclause 3.4.2 */
#define TXC_MIME_TYPE_RLS				"application/rls-services+xml" /**< as per rfc 4826 subclause 4.4.2 */
#define TXC_MIME_TYPE_IETF_PRESRULES	"application/auth-policy+xml" /**< as per rfc 5025 subclause 9.4 */
#define TXC_MIME_TYPE_OMA_PRESRULES		"application/auth-policy+xml" /**< as per [OMA-TS-Presence_SIMPLE_XDM-V1_1-20080627-A] subclause 5.1.1.5 */
#define TXC_MIME_TYPE_IETF_DIRECTORY	"application/directory+xml" /**< as per draft-garcia-simple-xcap-directory-00 subclause 9.2 */
#define TXC_MIME_TYPE_OMA_DIRECTORY		"application/vnd.oma.xcap-directory+xml" /**< as per [OMA-TS-XDM_Core-V1_1-20080627-A] subclause 6.7.2.2 */
#define TXC_MIME_TYPE_OMA_PRESCONT		"application/vnd.oma.pres-content+xml" /**< as per [OMA-TS-Presence-SIMPLE_Content_XDM-V1_0-20081223-C] subclause 5.1.5 */

#define TXC_AUID_CAPS			"xcap-caps" /**< as per rfc 4825 subclause 12.1 */
#define TXC_AUID_RLIST			"resource-lists" /**< as per rfc 4826 subclause 3.4.1 */
#define TXC_AUID_RLS			"rls-services" /**< as per rfc 4826 subclause 4.4.1 */
#define TXC_AUID_IETF_PRESRULES	"pres-rules" /**< as per rfc 5025 subclause 9.1 */
#define TXC_AUID_OMA_PRESRULES	"org.openmobilealliance.pres-rules" /**< as per [OMA-TS-Presence_SIMPLE_XDM-V1_1-20080627-A] subclause 5.1.1.2 */
#define TXC_AUID_IETF_DIRECTORY	"directory" /**<  as per draft-garcia-simple-xcap-directory-00 subclause 9.1 */
#define TXC_AUID_OMA_DIRECTORY	"org.openmobilealliance.xcap-directory" /**< as per [OMA-TS-XDM_Core-V1_1-20080627-A subclause 6.7.2.1 */
#define TXC_AUID_OMA_PRESCONT	"org.openmobilealliance.pres-content" /**< as per [OMA-TS-Presence-SIMPLE_Content_XDM-V1_0-20081223-C] subclause 5.1.2 */

#define TXC_DOCUMENT_CAPS			"index"		/**< as per rfc 4825 subclause 5.5 */
#define TXC_DOCUMENT_RLIST			"index"		/**< as per rfc 4826 subclause 3.4.7* */
#define TXC_DOCUMENT_RLS			"index"		/**< as per rfc 4826 subclause 4.4.7 */
#define TXC_DOCUMENT_IETF_PRESRULES	"index"		/**< as per rfc 5025 subclause 9.7 */
#define TXC_DOCUMENT_OMA_PRESRULES	"pres-rules"		/**< as per [OMA-TS-Presence_SIMPLE_XDM-V1_1-20080627-A] subclause 5.1.1.8 */
#define TXC_DOCUMENT_IETF_DIRECTORY	"directory.xml"		/**< as per draft-garcia-simple-xcap-directory-00 subclause 9.6 */
#define TXC_DOCUMENT_OMA_DIRECTORY	"directory.xml"		/**< as per [OMA-TS-XDM_Core-V1_1-20080627-A] subclause 6.7.2.7 */
#define TXC_DOCUMENT_OMA_PRESCONT	"oma_status-icon/rcs_status_icon"		/**< as per [R1_090831_RCS_Release_1_Technical_Realisation_v1_1] subclause 4.8.1 */

#define TXC_NS_CAPS				"urn:ietf:params:xml:ns:xcap-caps" /**< as per rfc 4825 subclause 12.3*/
#define TXC_NS_RLIST			"urn:ietf:params:xml:ns:resource-lists" /**< as per rfc 4826 subclause 3.4.4 */
#define TXC_NS_RLS				"urn:ietf:params:xml:ns:rls-services" /**< as per rfc 4826 subclause 4.4.4 */
#define TXC_NS_IETF_PRESRULES	"urn:ietf:params:xml:ns:pres-rules" /**< as per rfc 5025 subclause 9.3 */
#define TXC_NS_OMA_PRESRULES	"urn:ietf:params:xml:ns:common-policy" /**< as per [OMA-TS-Presence_SIMPLE_XDM-V1_1-20080627-A] subclause 5.1.1.3 */
#define TXC_NS_IETF_DIRECTORY	"urn:ietf:params:xml:ns:xcap-directory" /**< as per draft-garcia-simple-xcap-directory-00 subclause 6 */
#define TXC_NS_OMA_DIRECTORY	"urn:oma:xml:xdm:xcap-directory" /**< as per [OMA-TS-XDM_Core-V1_1-20080627-A] subclause 6.7.2.3 */
#define TXC_NS_OMA_PRESCONT		"urn:oma:xml:prs:pres-content" /**< as per [OMA-TS-Presence-SIMPLE_Content_XDM-V1_0-20081223-C] subclause 5.1.4 */

/**@def TXC_CONTEXT_CHECK
* Check context validity 
*/
#define TXC_CONTEXT_CHECK(context, panic) \
	if(!context->txc_root) panic = xpa_invalid_root; \
	else if(!context->xui) panic = xpa_invalid_xui; \
	else panic = xpa_success;

/**@def TXC_CONTEXT_CREATE
* Helper function to easly create and initialize an XCAP context.
* You MUST use @ref TXC_CONTEXT_SAFE_FREE to free a context.
* @sa @ref TXC_CONTEXT_SAFE_FREE
*/
/**@def TXC_CONTEXT_SAFE_FREE
* Helper function to safely free an XCAP context previously created using @ref TXC_CONTEXT_CREATE.
* @sa @ref TXC_CONTEXT_CREATE.
*/
#define TXC_CONTEXT_CREATE(this)		TXC_XXX_CREATE2(this, context)
#define TXC_CONTEXT_SAFE_FREE(this)	TXC_XXX_SAFE_FREE2(this, context)

/**@def TXC_REQUEST_CREATE
* Helper function to easly create and initialize an XCAP request
*/
/**@def TXC_REQUEST_SAFE_FREE
* Helper function to easly free an XCAP request
*/
/**@def TXC_REQUEST_RETURN_IF_PANIC
* Check if we should panic and leave the current section
*/
#define TXC_REQUEST_CREATE(this)		TXC_XXX_CREATE2(this, request)
#define TXC_REQUEST_SAFE_FREE(this)	TXC_XXX_SAFE_FREE2(this, request)
#define TXC_REQUEST_RETURN_IF_PANIC(request)\
	if(!TXC_PANIC_SUCCESS(request->panic)) return request;
#define TXC_REQUEST_RETURN_IF_NULL_URL(request)\
	if(!request->url) { request->panic =xpa_invalid_url;  return request; }

/**@def TXC_CONTENT_CREATE
* Helper function to easly create and initialize an XCAP content.
* You MUST call @ref TXC_CONTENT_SAFE_FREE to free an XCAP content.
* @sa @ref TXC_CONTENT_SAFE_FREE 
*/
/**@def TXC_CONTENT_SAFE_FREE
* Helper function to easly free an XCAP content previously created using @ref TXC_CONTENT_CREATE.
* @sa @ref TXC_CONTENT_CREATE
*/
#define TXC_CONTENT_CREATE(this)		TXC_XXX_CREATE2(this, content)
#define TXC_CONTENT_SAFE_FREE(this)	TXC_XXX_SAFE_FREE2(this, content)

/* easyhandle */
#define TXC_EASYHANDLE_SAFE_FREE(easyhandle) \
	if(easyhandle) { curl_easy_cleanup(easyhandle); easyhandle = 0; }


/**@def TXC_AUID_CREATE
* Create and initialize an auid.
* You MUST use @ref TXC_AUID_SAFE_FREE to free an auid.
* @sa @ref TXC_AUID_SAFE_FREE
*/
/**@def TXC_AUID_SAFE_FREE
* Safely free an auid previously created using @ref TXC_AUID_CREATE.
* @sa @ref TXC_AUID_CREATE
*/

#define TXC_AUID_CREATE(this)		TXC_XXX_CREATE2(this, auid)
#define TXC_AUID_SAFE_FREE(this)	TXC_XXX_SAFE_FREE2(this, auid)

#define TXC_PANIC_SUCCESS(panic) ( (panic == xpa_success) )

/**@typedef txc_panic_t
* Well-known XCAP panic codes.
*/
typedef enum txc_panic_e
{
	xpa_success = 0x00,
	xpa_unsupported_auid = -0xFF,
	xpa_invalid_root,
	xpa_invalid_xui,
	xpa_invalid_doc_name,
	xpa_invalid_url,
	xpa_invalid_proxy,
	xpa_invalid_authtype,
	xpa_libcurl_error,
	xpa_func_not_implemented,
	xpa_func_obsolete
}
txc_panic_t;

/**@typedef txc_content_t
* XCAP buffer.
*/
typedef struct txc_content_s
{
	char *data;
	char *type;
	size_t cursor;
	size_t size;
}
txc_content_t;

/**@typedef txc_oper_type_t
* XCAP operation.
*/
typedef enum txc_oper_type_e
{
	op_create,
	op_replace, 
	op_fetch,
	op_delete
}
txc_oper_type_t;

/**@typedef txc_selection_type_t
* XCAP selection type.
*/
typedef enum txc_selection_type_e
{
	sl_element,
	sl_document, 
	sl_attribute
}
txc_selection_type_t;

/**@typedef txc_node_step_t
* XCAP node selector step as per rfc 4825 subclause 6.3.
*/
typedef enum txc_node_step_e
{
	by_name,
	by_pos,
	by_attr,
	by_pos_attr
}
txc_node_step_t;

/**@typedef xcap_auid_type_t
* Well-known XCAP AUIDs.
*/
typedef enum xcap_auid_type_e
{
	ietf_xcap_caps,			/**< xcap-caps */
	ietf_resource_lists,	/**< resource-lists */
	ietf_rls_services,		/**< rls-services */
	ietf_pres_rules,		/**< pres-rules */
	ietf_directory,			/**< directory */

	oma_conv_history,		/**< org.openmobilealliance.conv-history */
	oma_pres_rules,			/**< org.openmobilealliance.pres-rules */
	oma_directory,			/**< org.openmobilealliance.xcap-directory */
	oma_deferred_list,		/**< org.openmobilealliance.deferred-list */
	oma_pres_content,		/**< org.openmobilealliance.pres-content*/
	oma_shared_groups		/**< org.openmobilealliance.groups */
}
xcap_auid_type_t;

/**@typedef txc_auid_t
* XCAP auid.
*/
typedef struct txc_auid_s
{
	xcap_auid_type_t type; /**< The auid type*/
	char* name; /**< The auid name or id (e.g. 'xcap-caps' or 'rls-services' or 'resource-lists' or ...)*/
	char* content_type; /**< The default MIME-type of the document associated to this auid. */
	char* document; /**< The default name of the document associated to this auid. */
	unsigned available:1; /**< The auid availability. */
}
txc_auid_t;
typedef tsk_list_t txc_auid_L_t; /**< Contains @ref txc_auid_t elements */
typedef txc_auid_t AUIDS_T[TXC_AUIDS_COUNT];

/**@typedef txc_request_t
* XCAP request.
*/
typedef struct txc_request_s
{
	txc_content_t* content; /**< The request content. */
	char* url; /**< The destination address. */
	char* http_accept; /**< C string to put into the HTTP Accept header */
	char* http_expect; /**< C string to put into the HTTP Expect header */
	//char http_error[CURL_ERROR_SIZE];
	txc_panic_t panic; /**< Panic code */
	
	long status; /**< HTTP status code. */
	long timeout; /**< HTTP request timeout. */

	CURL* easyhandle; /**< Curl handle */
	struct curl_slist *headers; /**< curl headers */
}
txc_request_t;

/**@typedef txc_http_callback
* Function pointer to alert the user when new data becomes available.
* You MUST use @ref TXC_REQUEST_SAFE_FREE to free the returned request.
*@param request XCAP request holding data sent by the XDMS.
*/
typedef void (*txc_http_callback)(txc_request_t *request);

/**@typedef txc_context_t
* XCAP context.
*/
typedef struct txc_context_s
{
	char* xui;							/**< RFC 4825 subclause 4: username. Also used to fill @b "X-3GPP-Intended-Identity" header.*/
	char* password;						/**< user password. */
	char* proxy_host;					/**< HTTP proxy host. */
	int	  proxy_port;					/**< HTTP proxy port. */
	char* proxy_usr;					/**< HTTP proxy user name. */
	char* proxy_pwd;					/**< HTTP proxy password */
	unsigned   proxy_tunneling:1;				/**< Use HTTP  proxy tunneling, default is false*/
	char* txc_root;						/**< RFC 4825 subclause 6.1 */
	char* pragma;						/**< HTTP pragma */
	char* user_agent;					/**< The user agent. */
	AUIDS_T auids;	/**< Contains list of all default auids. */
	tsk_heap_t heap; /**< Internal heap*/
	
	void* tid;							/**< Thread id */
	tsk_semaphore_t *semaphore;			/**< Pthread semaphore */

	CURLM *multihandle;					/**< Curl multi-handle */
	CURL* easyhandle;					/**< Curl handle. */
	unsigned running:1;					/**< If still running */
	unsigned followredirect;				/**< Follow redirect when performing HTTP/XCAP operations */
	unsigned reuse_http_connection:1;			/**< reuse the same easyhandle to send/receive data */
	txc_http_callback http_callback;		/**< HTTP callback to pass data from the XDMS to the user */
}
txc_context_t;

TINYXCAP_API const char* txc_mime_type_get(const txc_context_t *context, xcap_auid_type_t auid_type, txc_selection_type_t sel);

TINYXCAP_API void txc_content_init(txc_content_t *content);
TINYXCAP_API void txc_content_set(txc_content_t* content, const char *data, size_t size, const char *type);
TINYXCAP_API void txc_content_free(txc_content_t** content);

void txc_auid_init(txc_auid_t *auid);
TINYXCAP_API const txc_auid_t* txc_auid_findby_type(const AUIDS_T auids, xcap_auid_type_t type);
TINYXCAP_API const txc_auid_t* txc_auid_findby_name(const AUIDS_T auids, const char* name);
TINYXCAP_API int txc_auid_is_available(const AUIDS_T auids, xcap_auid_type_t type);
TINYXCAP_API void txc_auid_set_availability(AUIDS_T auids, xcap_auid_type_t type, int avail);
TINYXCAP_API void txc_auid_update(txc_context_t* context, xcap_auid_type_t type, const char* document);
void txc_auid_free(void **auid);

TINYXCAP_API void txc_context_init(txc_context_t* context);
TINYXCAP_API void txc_context_update_available_auids(txc_context_t *context, const tsk_list_t* avail_auids);
TINYXCAP_API void txc_context_free(txc_context_t** context);

TINYXCAP_API void txc_request_init(txc_request_t* request);
TINYXCAP_API void txc_request_free(txc_request_t** request);


TINYXCAP_API int txc_xcap_perform(txc_context_t* context, txc_request_t* request, txc_oper_type_t oper, txc_selection_type_t sel);

TINYXCAP_API int txc_xcap_send(txc_context_t* context, txc_request_t** request, txc_oper_type_t oper, txc_selection_type_t sel);

#endif /* _TINYXCAP_TXC_H_ */