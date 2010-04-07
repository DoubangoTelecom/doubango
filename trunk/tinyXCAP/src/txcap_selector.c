/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango.org>
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
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinyXCAP/txcap_selector.h"


/**@defgroup txcap_selector_group XCAP URL selector
*/

/**@ingroup txcap_selector_group
* Gets the full XCAP URL with both the document and node selector parts.
* @param stack The XCAP stack from which to load the user's preference (xcap-root, AUIDs, XUI, name of the document, ...).
* Should be created using @ref txcap_stack_create().
* @param auid_id The Application Unique ID.  Unique identifier within the namespace of application unique IDs created by this specification (RFC 4825)
* that differentiates XCAP resources accessed by one application from XCAP resources accessed by another.
* For more information see <a href="http://tools.ietf.org/html/rfc4825#section-4">RFC 4825 subclause 4</a>.
* @retval A well-formed HTTP/HTTPS URL which can be used as Request-Uri (e.g. GET, PUT or DELETE). As a
*  result, performing a GET against the return URL would retrieve a document, element or attribute.
*
* @code
* @endcode
*
* @sa @ref txcap_selector_get_full_2<br>@ref txcap_selector_get_node<br>@ref txcap_selector_get_document<br> @ref txcap_selector_get_document_2
*/
char* txcap_selector_get_full(const txcap_stack_handle_t* stack, const char* auid_id, ...)
{

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
* @sa @ref txcap_selector_get_full<br>@ref txcap_selector_get_node<br>@ref txcap_selector_get_document<br> @ref txcap_selector_get_document_2
*/
*/
char* txcap_selector_get_full_2(const char* xcap_root, const char* auid_id, const char* xui, const char* doc_name, ...)
{

}
