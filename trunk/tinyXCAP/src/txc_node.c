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

/**@file txc_node.c
 * @brief RFC 4825 subclause <a href="http://tools.ietf.org/html/rfc4825#section-6.4">6.4 (Namespace Bindings for the Selector
)</a> and subclause <a href="http://tools.ietf.org/html/rfc4825#section-6.3">6.3 (Node Selector)</a>
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */

#include "txc_node.h"
#include "txc_document.h"
#include "txc_macros.h"

#include <stdarg.h>

#include "txc_document.h"
#include "txc_macros.h"

/**@defgroup txc_node_group XCAP node selector
*/


/**@page txc_node_page XCAP node selector Tutorial
*
* @sa txc_document_page
* 
* @par ABNF<br>
* Node Selector as per rfc 4825 subclause 6.3<br>
* node-selector          = element-selector ["/" terminal-selector]<br>
* terminal-selector      = attribute-selector / namespace-selector /<br>
* extension-selector<br>
* element-selector       = step *( "/" step)<br>
* step                   = by-name / by-pos / by-attr / by-pos-attr /<br>
* extension-selector<br>
* by-name                = NameorAny<br>
* by-pos                 = NameorAny "[" position "]"<br>
* position               = 1*DIGIT<br>
* attr-test              = "@" att-name "=" att-value<br>
* by-attr                = NameorAny "[" attr-test "]"<br>
* by-pos-attr            = NameorAny "[" position "]" "[" attr-test "]"<br>
* NameorAny              = QName / "*"   ; QName from XML Namespaces<br>
* att-name               = QName<br>
* att-value              = AttValue      ; from XML specification<br>
* attribute-selector     = "@" att-name<br>
* namespace-selector     = "namespace::*"<br>
* extension-selector     = 1*( %x00-2e / %x30-ff )  ; anything but "/"<br>
* 
* @par Settings<br>
* @a XCAP-ROOT = http://xcap.example.com:8080/services <br>
* @a XUI = sip:doubango@example.com <br>
* 
* <H3>=== Find list with {name='Default' and auid='resource-list'} ===</H3>
* @code
#include "txc_api.h"

char *url = txc_node_get_sel(context, ietf_resource_lists,
	TXC_NODE_SELECT_BY_ATTR("list", "name", "Default"), //==> /resource-lists/list[@name="Default"]
	TXC_NODE_SELECT_END()
	);
TSK_FREE(url);
* @endcode
* @sa txc_node_get_cust_sel for custom (user defined) requests.
*
* <H3>===Find list with {position='2' and auid='resource-list'} ===</H3>
* @code
#include "txc_api.h"

char *url = txc_node_get_sel(context, ietf_resource_lists,
	TXC_NODE_SELECT_BY_POS("list", 2), //==> /resource-lists/list[2]
	TXC_NODE_SELECT_END()
	);
TSK_FREE(url);
* @endcode
* @sa txc_node_get_cust_sel for custom (user defined) requests.
*
* <H3>=== Find list with {position='4' and using wildcard} ===</H3>
* @code
#include "txc_api.h

char *url = txc_node_get_sel(context, ietf_resource_lists,
	TXC_NODE_SELECT_BY_POS("*", 4), //==> /resource-lists/*[4]
	TXC_NODE_SELECT_END()
	);
TSK_FREE(url);
* @endcode
* @sa txc_node_get_cust_sel for custom (user defined) requests.
*
* <H3>=== Find entry with {uri='sip:doubango@example.com' and from list->name='Ennemies' and auid='resource-list'} ===</H3>
* @code
#include "txc_api.h

char *url = txc_node_get_sel(context, ietf_resource_lists,
	TXC_NODE_SELECT_BY_ATTR("list", "name", "Ennemies"),//==> /resource-lists/list[@name="Ennemies"]
	TXC_NODE_SELECT_BY_ATTR("entry", "uri", "sip:doubango@example.com"),//==> /resource-lists/list[@name="Ennemies"]/entry[@uri="sip:doubango@example.com"]
	TXC_NODE_SELECT_END()
	);
TSK_FREE(url);
* @endcode
* @sa txc_node_get_cust_sel for custom (user defined) requests.
*
* <H3>===Find entry's display-name with {uri='sip:doubango@example.com' and from list->name='Friends' and auid='resource-list' }===</H3>
* @code
#include "txc_api.h

char *url = txc_node_get_sel(context, ietf_resource_lists,
	TXC_NODE_SELECT_BY_ATTR("list", "name", "Friends"),//==> /resource-lists/list[@name="Friends"]
	TXC_NODE_SELECT_BY_ATTR("entry", "uri", "sip:doubango@example.com"),//==> /resource-lists/list[@name="Friends"]/entry[@uri="sip:doubango@example.com"]
	TXC_NODE_SELECT_BY_NAME("display-name"),//==> /resource-lists/list[@name="Friends"]/entry[@uri="sip:doubango@example.com"]/display-name
	TXC_NODE_SELECT_END()
	);
TSK_FREE(url);
* @endcode
* @sa txc_node_get_cust_sel for custom (user defined) requests.
*
* <H3>=== Find entry's display-name with {position='1' and from list->name='Friends' and auid='resource-list'} ===</H3>
* @code
#include "txc_api.h

char *url = txc_node_get_sel(context, ietf_resource_lists,
	TXC_NODE_SELECT_BY_ATTR("list", "name", "Friends"),//==> /resource-lists/list[@name="Friends"]
	TXC_NODE_SELECT_BY_POS("entry", 1),//==> /resource-lists/list[@name="Friends"]/entry[1]
	TXC_NODE_SELECT_BY_NAME("display-name"),//==> /resource-lists/list[@name="Friends"]/entry[1]/display-name
	TXC_NODE_SELECT_END()
	);
	TSK_FREE(url);
* @endcode
* @sa txc_node_get_cust_sel for custom (user defined) requests.
*
* <H3>=== Find entry's display-name with {uri='sip:doubango@example.com' and position = '23' and from list->name='Ennemies' and auid='resource-list'} ===</H3>
* @code
#include "txc_api.h

url = txc_node_get_sel(context, ietf_resource_lists,
	TXC_NODE_SELECT_BY_ATTR("list", "name", "Ennemies"),//==> /resource-lists/list[@name="Ennemies"]
	TXC_NODE_SELECT_BY_POS_ATTR("entry", 23, "uri", "sip:doubango@example.com"),//==> /resource-lists/list[@name="Ennemies"]/entry[23][@uri="sip:doubango@example.com"]
	TXC_NODE_SELECT_END()
	);
TSK_FREE(url);
* @endcode
* @sa txc_node_get_cust_sel for custom (user defined) requests.
*
* <H3>=== Namespace test  ===</H3>
* @code
#include "txc_api.h

char *url = txc_node_get_sel(context, ietf_resource_lists,
	TXC_NODE_SELECT_BY_NAME("foo"),//==> /foo
	TXC_NODE_SELECT_BY_NAME("a:bar"),//==> /foo/a:bar
	TXC_NODE_SELECT_BY_NAME("b:baz"),//==> /foo/a:bar/b:baz
	TXC_NODE_ADD_NAMESPACE("a", "urn:test:namespace1-uri"),//==>/foo/a:bar/b:baz?xmlns(a=urn:test:namespace1-uri)
	TXC_NODE_ADD_NAMESPACE("b", "urn:test:namespace2-uri"),//==>/foo/a:bar/b:baz?xmlns(a=urn:test:namespace1-uri)xmlns(b=urn:test:namespace2-uri)
	TXC_NODE_SELECT_END()
	);
TSK_FREE(url);
* @endcode
* @sa txc_node_get_cust_sel for custom (user defined) requests.
*/

#define TXC_ELEMENT_CONTENT_TYPE "application/xcap-el+xml" /* RFC 4825 subclause 15.2.1 */


static char* __internal_txc_node_get_cust_sel(const char* doc_selector, const char *auid, va_list steps)
{
	char *uri = 0;

	int step;
	char *result=0, *node_root = 0, *_namespace=0, *step_str = 0;
	tsk_heap_t heap;

	/* check parameters validity */
	if(!doc_selector || !auid) return 0;
	
	/* initialize memory heap */
	tsk_heap_init(&heap);

	/* set base uri*/
	uri = tsk_strdup2(doc_selector);

	/* append node root*/
	tsk_sprintf(&heap, &node_root, "/~~/%s", auid);
	tsk_strcat2(&uri, node_root);
	
	while( (step=va_arg(steps, txc_node_step_t)) != 0xFF)
	{
		step_str = 0;

		switch(step)
		{
		case by_name:
			{	/* qname */
				const char* qname = va_arg(steps, const char*);
				tsk_sprintf(&heap, &step_str, "/%s", qname);
				tsk_strcat2(&uri, (const char*)step_str);
				break;
			}

		case by_pos:
			{	/* qname, position */
				const char* qname = va_arg(steps, const char*);
				int position = va_arg(steps, int);
				tsk_sprintf(&heap, &step_str, "/%s%%5B%d%%5D", qname, position);
				tsk_strcat2(&uri, (const char*)step_str);
				break;
			}

		case by_attr:
			{	/* qname, att_name, att_value */
				const char* qname = va_arg(steps, const char*);
				char* att_name = tsk_url_encode(&heap, va_arg(steps, const char*));
				char* att_value = tsk_url_encode(&heap, va_arg(steps, const char*));
				tsk_sprintf(&heap, &step_str, "/%s%%5B@%s=%%22%s%%22%%5D", qname, att_name, att_value);
				tsk_strcat2(&uri, (const char*)step_str);
				break;
			}

		case by_pos_attr:
			{	/* qname, position, att_name, att_value */
				const char* qname = va_arg(steps, const char*);
				int position = va_arg(steps, int);
				char* att_name = tsk_url_encode(&heap, va_arg(steps, const char*));
				char* att_value = tsk_url_encode(&heap, va_arg(steps, const char*));
				tsk_sprintf(&heap, &step_str, "/%s%%5B%d%%5D%%5B@%s=%%22%s%%22%%5D", qname, position, att_name, att_value);
				tsk_strcat2(&uri, (const char*)step_str);
				break;
			}

		case 0xF0:
			{	/* prefix ns */
				const char* prefix = va_arg(steps, const char*);
				const char* ns = va_arg(steps, const char*);
				tsk_sprintf(&heap, &step_str, "%sxmlns(%s=%%22%s%%22)", _namespace?"":"%3F",prefix, ns);
				tsk_strcat(&heap, &_namespace, (const char*)step_str);
				break;
			}
		default: break;

		}/* switch */
	}
		
	/* add namespace and free it*/
	tsk_strcat2(&uri, (const char*)_namespace);
	
	/* Free heap */
	tsk_heap_cleanup(&heap);

	return uri;
}

/**@ingroup txc_node_group
* Create a custom XCAP node selector URI as per <a href="http://tools.ietf.org/html/rfc4825#section-6.3">RFC 4825 subclause 6.3</a>
* @param doc_selector The XCAP document selector URI. The HTTP URI containing the XCAP root and document selector, resulting in the selection of a specific document.  
* As a result, performing a GET against the document URI would retrieve the document. This URI can be retrieved using @ref txc_doc_get_cust_sel or 
* @ref TXC_DOC_GET_SEL or @ref TXC_DOC_GET_SEL2.
* @param auid The Application Unique ID.  unique identifier within the namespace of application unique IDs created by this specification
* that differentiates XCAP resources accessed by one application from XCAP resources accessed by another.
* For more information see <a href="http://tools.ietf.org/html/rfc4825#section-4">RFC 4825 subclause 4</a>
* @param ... The node selection steps. MUST be @ref TXC_NODE_SELECT_BY_NAME or @ref TXC_NODE_SELECT_BY_POS or @ref TXC_NODE_SELECT_BY_ATTR or @ref TXC_NODE_SELECT_BY_POS_ATTR 
* or @ref TXC_NODE_ADD_NAMESPACE and end with @ref TXC_NODE_SELECT_END. For more information please refer to @ref txc_node_page.
* @retval The HTTP URI containing the XCAP root, document selector, node selector separator, and node selector, resulting in the
* selection of a specific XML node. If supplied parameters are not valide this method will return NULL.
* You must free the returned string using TSK_FREE or TSK_SAFE_FREE.
* @sa txc_node_get_sel
*/
char* txc_node_get_cust_sel(const char* doc_selector, const char *auid, ...)
{
	va_list args;
	char * uri;

	va_start(args, auid);

	uri = __internal_txc_node_get_cust_sel(doc_selector, auid, args);

	va_end(args);

	return uri;
}

/**@ingroup txc_node_group
* Create a custom XCAP node selector URI as per <a href="http://tools.ietf.org/html/rfc4825#section-6.3">RFC 4825 subclause 6.3</a>
* @param context The XCAP context with valid xcap-root and  xui.
* @param auid_type The Application Unique ID.  Unique identifier within the namespace of application unique IDs created by this specification
* that differentiates XCAP resources accessed by one application from XCAP resources accessed by another.
* For more information see <a href="http://tools.ietf.org/html/rfc4825#section-4">RFC 4825 subclause 4</a>
* @param ... The node selection steps. MUST be @ref TXC_NODE_SELECT_BY_NAME or @ref TXC_NODE_SELECT_BY_POS or @ref TXC_NODE_SELECT_BY_ATTR or @ref TXC_NODE_SELECT_BY_POS_ATTR 
* or @ref TXC_NODE_ADD_NAMESPACE and end with @ref TXC_NODE_SELECT_END. For more information please refer to @ref txc_node_page.
* @retval The HTTP URI containing the XCAP root, document selector, node selector separator, and node selector, resulting in the
* selection of a specific XML node. If supplied parameters are not valide this method will return NULL.
* You must free the returned string using TSK_FREE or TSK_SAFE_FREE.
* @sa txc_node_get_cust_sel
*/
char* txc_node_get_sel(const txc_context_t* context, const xcap_auid_type_t auid_type, ...)
{
	va_list args;
	char* doc_selector = 0, *uri = 0;

	/* check parameters validity */
	if(!context || !context->xui || !context->txc_root) return 0;

	/* Get document selector */
	doc_selector = TXC_DOC_GET_SEL(context, auid_type);

	/* initialize variable arguments */
	va_start(args, auid_type);

	/* Get full uri */
	uri = __internal_txc_node_get_cust_sel(doc_selector, context->auids[auid_type].name, args);
	
	/* free document selector*/
	TSK_FREE(doc_selector);

	/* reset variable arguments */
	va_end(args);

	return uri;
}

