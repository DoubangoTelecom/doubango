/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou [at) doubango (DOT) org>
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
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/
/**@file txcap_selector.c
 * @brief XCAP selector.
 *
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 *

 */
#include "tinyxcap/txcap_selector.h"

#include "tinyxcap/txcap_document.h"
#include "tinyxcap/txcap_node.h"

/**@defgroup txcap_selector_group XCAP URL selector
*/

/** Internal function
 */
char* __txcap_selector_get_url(const txcap_stack_handle_t* stack, const char* auid_id, va_list* app)
{
    char* ret = tsk_null;
    char* node = tsk_null;
    if(!stack && !auid_id) {
        goto bail;
    }

    /* ==document== */
    if(!(ret = txcap_selector_get_document(stack, auid_id))) {
        TSK_DEBUG_ERROR("Failed to compute XCAP document URL.");
        goto bail;
    }

    /* ==node== */
    if((node = txcap_selector_get_node_2(auid_id, app))) {
        /* append node root */
        char* temp = tsk_null;
        tsk_sprintf(&temp, "/~~/%s", auid_id);
        tsk_strcat(&ret, temp);
        /* append node */
        tsk_strcat(&ret, node);
        /* free */
        TSK_FREE(temp);
        TSK_FREE(node);
    }

bail:
    return ret;
}

/** Internal function
 */
char* __txcap_selector_get_url_2(const char* xcap_root, const char* auid_id, const char* xui, const char* doc_name, va_list* app)
{
    char* ret = tsk_null;
    char* node = tsk_null;
    if(!xcap_root && !auid_id) {
        goto bail;
    }

    /* ==document== */
    if(!(ret = txcap_selector_get_document_2(xcap_root, auid_id, xui, doc_name))) {
        TSK_DEBUG_ERROR("Failed to compute XCAP document URL.");
        goto bail;
    }

    /* ==node== */
    if((node = txcap_selector_get_node_2(auid_id, app))) {
        /* append node root */
        char* temp = tsk_null;
        tsk_sprintf(&temp, "/~~/%s", auid_id);
        tsk_strcat(&ret, temp);
        /* append node */
        tsk_strcat(&ret, node);
        /* free */
        TSK_FREE(temp);
        TSK_FREE(node);
    }

bail:
    return ret;
}

/**@ingroup txcap_selector_group
* Gets the full XCAP URL with both the document and node selector parts.
* @param stack The XCAP stack from which to load the user's preference (xcap-root, AUIDs, XUI, name of the document, ...).
* Should be created using @ref txcap_stack_create().
* @param auid_id The Application Unique ID.  Unique identifier within the namespace of application unique IDs created by this specification (RFC 4825)
* that differentiates XCAP resources accessed by one application from XCAP resources accessed by another.
* For more information see <a href="http://tools.ietf.org/html/rfc4825#section-4">RFC 4825 subclause 4</a>.
* @param ... Any @a TXCAP_SELECTOR_NODE_SET*() macros. MUST ends with @ref TXCAP_SELECTOR_NODE_SET_NULL().
* @retval A well-formed (already escaped) HTTP/HTTPS URL which can be used as Request-Uri (e.g. GET, PUT or DELETE). As a
*  result, performing a GET against the return URL would retrieve a document, element or attribute.
*
* @code
// Retrieve the entry with: uri='sip:bob@example.com' and list='rcs'
// Expected node: /resource-lists/list[@name="rcs"]/entry[@uri="sip:bob@example.com"]
char* urlstring = txcap_selector_get_url(stack, "resource-lists",
	TXCAP_SELECTOR_NODE_SET_ATTRIBUTE("list", "name", "rcs"),
	TXCAP_SELECTOR_NODE_SET_ATTRIBUTE("entry", "uri", "sip:bob@example.com"),
	TXCAP_SELECTOR_NODE_SET_NULL());
TSK_DEBUG_INFO("%s\n", urlstring);
TSK_FREE(urlstring);
* @endcode
*
* @sa @ref txcap_selector_get_node<br>@ref txcap_selector_get_document<br> @ref txcap_selector_get_document_2
*/
char* txcap_selector_get_url(const txcap_stack_handle_t* stack, const char* auid_id, ...)
{
    char* ret;
    va_list ap;

    va_start(ap, auid_id);
    ret = __txcap_selector_get_url(stack, auid_id, &ap);
    va_end(ap);

    return ret;
}

/**@ingroup txcap_selector_group
* Gets the full XCAP URL with both the document and node selector parts.
* @param xcap_root The XCAP Root URI (valid HTTP/HTTPS URL). A context that contains all the documents across all
* application usages and users that are managed by the server. For more information see <a href="http://tools.ietf.org/html/rfc4825#section-4">RFC 4825 subclause 4</a>.
* @param auid_id The Application Unique ID.  Unique identifier within the namespace of application unique IDs created by this specification (RFC 4825)
* that differentiates XCAP resources accessed by one application from XCAP resources accessed by another.
* For more information see <a href="http://tools.ietf.org/html/rfc4825#section-4">RFC 4825 subclause 4</a>.
* @param xui The XCAP User Identifier. The XUI is a string, valid as a path element in an HTTP URI, that is associated with each user served
*  by the XCAP server. For more information see <a href="http://tools.ietf.org/html/rfc4825#section-4">RFC 4825 subclause 4</a>.
* For global document, this parameter should be NULL or equal to "global".
* @param doc_name The name of the document (e.g. index).
* @retval A well-formed HTTP/HTTPS URL which can be used as Request-Uri (e.g. GET, PUT or DELETE). As a
*  result, performing a GET against the return URL would retrieve a document, element or attribute.
* @code
* @endcode
*
* @sa @ref txcap_selector_get_url<br>@ref txcap_selector_get_node<br>@ref txcap_selector_get_document<br> @ref txcap_selector_get_document_2
*/
char* txcap_selector_get_url_2(const char* xcap_root, const char* auid_id, const char* xui, const char* doc_name, ...)
{
    char* ret;
    va_list ap;

    va_start(ap, doc_name);
    ret = __txcap_selector_get_url_2(xcap_root, auid_id, xui, doc_name, &ap);
    va_end(ap);

    return ret;
}
