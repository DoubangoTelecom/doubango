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
/**@file txcap_document.c
 * @brief XCAP documents.
 *
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 *

 */
#include "tinyxcap/txcap_document.h"

#include "tinyxcap/txcap_auid.h"

/**@ingroup txcap_selector_group
* Create a custom XCAP document selector URI as per <a href="http://tools.ietf.org/html/rfc4825#section-6.2">RFC 4825 subclause 6.2</a>.
* @param stack The XCAP stack from which to load the user's preference (xcap-root, AUIDs, XUI, name of the document, ...).
* Should be created using @ref txcap_stack_create().
* @param auid_id The Application Unique ID.  Unique identifier within the namespace of application unique IDs created by this specification (RFC 4825)
* that differentiates XCAP resources accessed by one application from XCAP resources accessed by another.
* For more information see <a href="http://tools.ietf.org/html/rfc4825#section-4">RFC 4825 subclause 4</a>.
* @retval The HTTP/HTTPS URI containing the XCAP root and document selector, resulting in the selection of a specific document.  As a
*  result, performing a GET against the document URI would retrieve the document.
* If supplied parameters are not valid this method will return NULL.
* You must free the returned string.
*
* @code
* // resource-lists document
* char* urlstring;
* if((urlstring = txcap_selector_get_document(stack, "resource-lists"))){
*	TSK_FREE(urlstring);
* }
* @endcode
*
* @code
* // xcap-caps document
* char* urlstring;
* if((urlstring = txcap_selector_get_document(stack, "xcap-caps"))){
*	TSK_FREE(urlstring);
* }
* @endcode
*
* @sa @ref txcap_selector_get_document_2<br>@ref txcap_selector_get_url
*/
char* txcap_selector_get_document(const txcap_stack_handle_t* stack, const char* auid_id)
{
    char* ret = tsk_null;
    txcap_auid_t* auid;

    const txcap_stack_t* xcap_stack = stack;
    if(xcap_stack && xcap_stack->auids && auid_id) {
        if((auid = txcap_auid_get_by_id(xcap_stack->auids, auid_id))) {
            ret = txcap_selector_get_document_2(xcap_stack->xcap_root, auid->id, auid->global? tsk_null: xcap_stack->xui, auid->document_name);
            TSK_OBJECT_SAFE_FREE(auid);
        }
        else {
            TSK_DEBUG_ERROR("Fail to find AUID with id %s.", auid_id);
        }
    }
    else {
        TSK_DEBUG_ERROR("Invalid parameter.");
    }
    return ret;
}

/**@ingroup txcap_selector_group
* Create a custom XCAP document selector URI as per <a href="http://tools.ietf.org/html/rfc4825#section-6.2">RFC 4825 subclause 6.2</a>.
* @param xcap_root The XCAP Root URI (valid HTTP/HTTPS URL). A context that contains all the documents across all
* application usages and users that are managed by the server. For more information see <a href="http://tools.ietf.org/html/rfc4825#section-4">RFC 4825 subclause 4</a>.
* @param auid_id The Application Unique ID.  Unique identifier within the namespace of application unique IDs created by this specification (RFC 4825)
* that differentiates XCAP resources accessed by one application from XCAP resources accessed by another.
* For more information see <a href="http://tools.ietf.org/html/rfc4825#section-4">RFC 4825 subclause 4</a>.
* @param xui The XCAP User Identifier. The XUI is a string, valid as a path element in an HTTP URI, that is associated with each user served
*  by the XCAP server. For more information see <a href="http://tools.ietf.org/html/rfc4825#section-4">RFC 4825 subclause 4</a>.
* For global document, this parameter should be NULL or equal to "global".
* @param doc_name The name of the document (e.g. index).
* @retval The HTTP/HTTPS URI containing the XCAP root and document selector, resulting in the selection of a specific document.  As a
  result, performing a GET against the document URI would retrieve the document.
* If supplied parameters are not valid this method will return NULL.
* You must free the returned string.
*
* @code
* // resource-lists document
* char* urlstring;
* if((urlstring = txcap_selector_get_document("http://doubango.org:8080/services", "resource-lists", "sip:bob@doubango.org", "index"))){
*	TSK_FREE(urlstring);
* }
* @endcode
*
* @code
* // xcap-caps document
* char* urlstring;
* if((urlstring = txcap_selector_get_document("http://doubango.org:8080/services", "xcap-caps", "global", "index"))){
*	TSK_FREE(urlstring);
* }
* @endcode
*
* @sa @ref txcap_selector_get_document<br>@ref txcap_selector_get_url
*/
char* txcap_selector_get_document_2(const char* xcap_root, const char* auid_id, const char* xui, const char* doc_name)
{
    char* ret = tsk_null;

    if(!xcap_root || !auid_id || !doc_name) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }

    if(!xui || tsk_striequals("global", xui)) {
        /* xdm-root/auid-name/global/doc-name */
        tsk_sprintf(&ret, "%s/%s/global/%s", xcap_root, auid_id, doc_name);
    }
    else {
        /* xdm-root/auid-name/users/xui/doc-name */
        tsk_sprintf(&ret, "%s/%s/users/%s/%s", xcap_root, auid_id, xui, doc_name);
    }

    return ret;
}