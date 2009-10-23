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

#ifndef _TINYXCAP_XDM_H_
#define _TINYXCAP_XDM_H_

#include "tinyxcap_config.h"
#include "xdm_list.h"

#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>

#define XDM_HTTP_DEFAULT_TIMEOUT	30000
#define XDM_HTTP_SUCCESS(request)	(200==request->status || 202==request->status)

#define XDM_SAFE_FREE(ptr) if(ptr) {(void)free(ptr); ptr=0;}
#define XDM_FREE(ptr) if(ptr)free(ptr);
#define XDM_IS_EMPTY_STRING(str) (!str || !strlen(str))

#define XDM_DOCUMENT_GLOBAL(auid_type) (auid_type == ietf_xcap_caps)
#define	XDM_MIME_TYPE_ELEMENT	"application/xcap-el+xml" /* as per rfc 4825 subclause 15.2.1 */
#define	XDM_MIME_TYPE_ATTRIBUTE	"application/xcap-att+xml" /* as per rfc 4825 subclause 15.2.2 */
#define	XDM_MIME_TYPE_NS		"application/xcap-ns+xml" /* as per rfc 4825 subclause 15.2.3 */
#define	XDM_MIME_TYPE_ERROR		"application/xcap-error+xml" /* as per rfc 4825 subclause 15.2.4 */
#define	XDM_MIME_TYPE_CAPS		"application/xcap-caps+xml" /* as per rfc 4825 subclause 15.2.5 */

/* macro helpers to create/free structures */
#define XDM_XXX_CREATE(xxx, prefix)\
	xxx = (xdm_##prefix##_t*)malloc(sizeof(xdm_##prefix##_t)); \
	xdm_##prefix##_init(xxx);
#define XDM_XXX_SAFE_FREE(xxx, prefix)\
	if(xxx) { xdm_##prefix##_free(&(xxx)); xxx = 0; }

/* check context validity */
#define XDM_CONTEXT_CHECK(context, panic) \
	if(!context->xdm_root) panic = xpa_invalid_root; \
	else if(!context->xui) panic = xpa_invalid_xui; \
	else panic = xpa_success;

/* context */
#define XDM_CONTEXT_CREATE(_context)		XDM_XXX_CREATE(_context, context)
#define XDM_CONTEXT_SAFE_FREE(_context)	XDM_XXX_SAFE_FREE(_context, context)

/* request */
#define XDM_REQUEST_CREATE(_request)		XDM_XXX_CREATE(_request, request)
#define XDM_REQUEST_SAFE_FREE(_request)	XDM_XXX_SAFE_FREE(_request, request)
#define XDM_REQUEST_RETURN_IF_PANIC(request)\
	if(!XDM_PANIC_SUCCESS(request->panic)) return request;

/* content */
#define XDM_CONTENT_CREATE(_content)		XDM_XXX_CREATE(_content, content)
#define XDM_CONTENT_SAFE_FREE(_content)	XDM_XXX_SAFE_FREE(_content, content)

/* easyhandle */
#define XDM_EASYHANDLE_SAFE_FREE(easyhandle) \
	if(easyhandle) { curl_easy_cleanup(easyhandle); easyhandle = 0; }

/* auid */
#define XDM_AUID_CHECK(auid, panic)\
	if(!auid) panic = xpa_unsupported_auid;
#define XDM_AUID_CREATE(_auid)		XDM_XXX_CREATE(_auid, auid)
#define XDM_AUID_SAFE_FREE(_auid)	XDM_XXX_SAFE_FREE(_auid, auid)

#define XDM_PANIC_SUCCESS(panic) ( (panic == xpa_success) )


/*Node Selector as per rfc 4825 subclause 6.3
node-selector          = element-selector ["/" terminal-selector]
terminal-selector      = attribute-selector / namespace-selector /
extension-selector
element-selector       = step *( "/" step)
step                   = by-name / by-pos / by-attr / by-pos-attr /
extension-selector
by-name                = NameorAny
by-pos                 = NameorAny "[" position "]"
position               = 1*DIGIT
attr-test              = "@" att-name "=" att-value
by-attr                = NameorAny "[" attr-test "]"
by-pos-attr            = NameorAny "[" position "]" "[" attr-test "]"
NameorAny              = QName / "*"   ; QName from XML Namespaces
att-name               = QName
att-value              = AttValue      ; from XML specification
attribute-selector     = "@" att-name
namespace-selector     = "namespace::*"
extension-selector     = 1*( %x00-2e / %x30-ff )  ; anything but "/"
*/
#define XDM_NODE_SELECT_BY_NAME(qname)										by_name, qname
#define XDM_NODE_SELECT_BY_POS(qname, position)								by_pos, qname, position
#define XDM_NODE_SELECT_BY_ATTR(qname, att_name, att_value)					by_attr, qname, att_name, att_value
#define XDM_NODE_SELECT_BY_POS_ATTR(qname, position, att_name, att_value)	by_pos_attr, qname, position, att_name, att_value
#define XDM_NODE_ADD_NAMESPACE(prefix, ns)									0xF0, prefix, ns
#define XDM_NODE_SELECT_END()												0xFF

/* listing all possible xdm panic codes */
typedef enum xdm_panic_e
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
xdm_panic_t;

/* xdm buffer */
typedef struct xdm_content_s
{
	char *data;
	char *type;
	size_t cursor;
	size_t size;
}
xdm_content_t;

/* xdm operation */
typedef enum xdm_oper_type_e
{
	op_create,
	op_replace, 
	op_fetch,
	op_delete
}
xdm_oper_type_t;

/* xdm selection type */
typedef enum xdm_selection_type_e
{
	sl_element,
	sl_document, 
	sl_attribute
}
xdm_selection_type_t;

/* xdm node selector step as per rfc 4825 subclause 6.3 */
typedef enum xdm_node_step_e
{
	by_name,
	by_pos,
	by_attr,
	by_pos_attr
}
xdm_node_step_t;

/* supported AUIDs */
typedef enum xcap_auid_type_e
{
	ietf_xcap_caps,			/* RFC 4825 sublause 12 */
	ietf_resource_lists,	/* RFC 4825 sublause ... */
	ietf_rls_services,		/* */
	ietf_pres_rules,		/* */
	ietf_directory,			/* */

	oma_conv_history,		/* */
	oma_pres_rules,			/* */
	oma_directory,			/* */
	oma_deferred_list,		/* */
	oma_pres_content,		/* */
	oma_shared_groups		/* */
}
xcap_auid_type_t;

/* [auid<->content type] mapping */
typedef struct xdm_auid_s
{
	xcap_auid_type_t type;
	char* name;
	char* description;
	char* content_type;
	char* document;
	int available;
}
xdm_auid_t;
typedef xdm_list_t xdm_auid_L_t; /* contains 'xdm_auid_t' elements */

/* all supported auids */
static const xdm_auid_t xdm_auids[11] =
{
	{ietf_xcap_caps,		"xcap-caps",								"IETF server capabilities", XDM_MIME_TYPE_CAPS,							"index",			1},
	{ietf_resource_lists,	"resource-lists",							"IETF resource-list",		"application/resource-lists+xml",			"index",			0},
	{ietf_rls_services,		"rls-services",								"IETF RLS service",			"application/rls-services+xml",				"index",			0},
	{ietf_pres_rules,		"pres-rules",								"IETF pres-rules",			"application/auth-policy+xml",				"pres-rules",		0},
	{ietf_directory,		"directory",								"IETF xdm directory",		"application/directory+xml",				"directory.xml",	0},

	{oma_conv_history,		"org.openmobilealliance.conv-history",		"OMA conversation history", "application/vnd.oma.im.history-list+xml",	"conv-history",		0},
	{oma_pres_rules,		"org.openmobilealliance.pres-rules",		"OMA presence rules",		"application/auth-policy+xml",				"pres-rules",		0},
	{oma_directory,			"org.openmobilealliance.xdm-directory",		"OMA xdm directory",		"application/vnd.oma.xdm-directory+xml",	"directory.xml",	0},
	{oma_deferred_list,		"org.openmobilealliance.deferred-list",		"OMA deferred list",		"application/vnd.oma.im.deferred-list+xml", "deferred-list",	0},
	{oma_pres_content,		"org.openmobilealliance.pres-content",		"OMA presence content",		"application/vnd.oma.pres-content+xml",		"oma_status-icon",	0},
	{oma_shared_groups,		"org.openmobilealliance.groups",			"OMA shared groups",		"application/vnd.oma.poc.groups+xml",		"index"}
};

/* xdm request */
typedef struct xdm_request_s
{
	char* auid;	
	xdm_content_t* content;
	char* url;
	char* http_accept;
	char* http_expect;
	//char http_error[CURL_ERROR_SIZE];
	xdm_panic_t panic;
	
	long status;
	long timeout;
}
xdm_request_t;

typedef struct xdm_context_s
{
	char* xui;							/* RFC 4825 subclause 4: username*/
	char* password;						/* user password */
	char* proxy_host;					/* proxy host */
	int	  proxy_port;					/* proxy port */
	char* proxy_usr;					/* proxy user name */
	char* proxy_pwd;					/* proxy password */
	int   proxy_tunneling;				/* use proxy tunneling, default is false*/
	char* xdm_root;						/* RFC 4825 subclause 6.1 */
	char* pragma;						/* HTTP pragma */
	char* user_agent;
	xdm_auid_L_t* auids;				/* contains list of all default auids */

	CURL* easyhandle;					/* curl handle */
	int reuse_http_connection;			/* reuse the same easyhandle to send/receive data */
}
xdm_context_t;

TINYXCAP_API void xdm_content_init(xdm_content_t *content);
TINYXCAP_API void xdm_content_set(xdm_content_t* content, const char *data, size_t size, const char *type);
TINYXCAP_API void xdm_content_free(xdm_content_t** content);

void xdm_auid_init(xdm_auid_t *auid);
TINYXCAP_API xdm_auid_t* xdm_auid_findby_type(const xdm_auid_L_t* auids, xcap_auid_type_t type);
TINYXCAP_API xdm_auid_t* xdm_auid_findby_name(const xdm_auid_L_t* auids, const char* name);
TINYXCAP_API int xdm_auid_is_available(const xdm_auid_L_t* auids, xcap_auid_type_t type);
TINYXCAP_API void xdm_auid_set_availability(const xdm_auid_L_t* auids, xcap_auid_type_t type, int avail);
TINYXCAP_API void xdm_auid_update(xdm_context_t* context, xcap_auid_type_t type, const char* document);
void xdm_auid_free(void **auid);

TINYXCAP_API void xdm_context_init(xdm_context_t* context);
TINYXCAP_API void xdm_conttext_update_available_auids(xdm_context_t *context, const xdm_list_t* avail_auids);
TINYXCAP_API void xdm_context_free(xdm_context_t** context);

TINYXCAP_API void xdm_request_init(xdm_request_t* request);
TINYXCAP_API void xdm_request_free(xdm_request_t** request);


TINYXCAP_API int xdm_xcap_perform(xdm_context_t* context, xdm_request_t* request, xdm_oper_type_t oper, xdm_selection_type_t sel);

#endif /* _TINYXCAP_XDM_H_ */