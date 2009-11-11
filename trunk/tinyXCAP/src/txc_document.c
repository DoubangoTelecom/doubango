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
/**@file txc_document.c
 * @brief <a href="http://tools.ietf.org/html/rfc4825#section-6.2">RFC 4825 subclause 6.2.  Document Selector</a>
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */

#include "txc_document.h"
#include "txc_macros.h"

/**@defgroup txc_document_group XCAP Document selector
*/


/**@page txc_document_page XCAP document selector Tutorial
* 
* @par Settings<br>
* @a XCAP-ROOT = http://xcap.example.com:8080/services <br>
* @a XUI = sip:doubango@example.com <br>
* 
* <H2>== Change resource-list default document name for a given XCAP context==</H2>
* @code
#include "txc_api.h"

txc_auid_update(context, ietf_resource_lists, "rlist.xml");
* @endcode
*
* <H2>== Well-known document selector ==</H2>
* A document is named well-known if we can retrieve it's selctor URI (name, auid and scope) from its AUID.
* For example, from xcap-caps AUID we know that the document's name should be 'default' within the 'global' scope.
* It is possible to change a default document name using @ref txc_auid_update function.
*
* IETF server capabilities
* @code
#include "txc_api.h"

char *url = TXC_DOC_GET_SEL(context, ietf_xcap_caps);
printf("Server capabilities url: \n%s\n***\n", url);
TSK_SAFE_FREE2(url);
* @endcode
* @a Output: http://xcap.example.com:8080/services/xcap-caps/global/index
*
* IETF resource-list
* @code
#include "txc_api.h" 

char *url = TXC_DOC_GET_SEL(context, ietf_resource_lists);
printf("IETF resource-list: \n%s\n***\n", url);
TSK_SAFE_FREE2(url);
* @endcode
* @a Output: http://xcap.example.com:8080/services/resource-lists/users/sip:doubango@example.com/index
*
* <H2>== Custom document selector ==</H2>
* A custom document selctor can be full or semi custom.
* Keep in mind that it is possible to change a default document name using @ref txc_auid_update function.
*
* <H3>=== Semi-custom document selector ===</H3>
* For semi-custom document selector, only the document's name is set by the user:
* @code
#include "txc_api.h" 

char *url = TXC_DOC_GET_SEL2(context, ietf_resource_lists, "usr-doc");
printf("semi-custom document selector: \n%s\n***\n", url);
TSK_SAFE_FREE2(url);
* @endcode
* @a Output: http://xcap.example.com:8080/services/resource-lists/users/sip:doubango@example.com/usr-doc

* <H3>=== Full custom selector ===</H3>
* For full-custom document selector, all members (xcap-root, auid, scope and document name) are set by the user:
* @code
#include "txc_api.h" 

char *url = txc_doc_get_cust_sel("http://usr-root", "usr-auid", "usr-xui", "usr-doc");
printf("full-custom document selector: \n%s\n***\n", url);
TSK_SAFE_FREE2(url);
* @endcode
* @a Output: http://usr-root/usr-auid/users/usr-xui/usr-doc
*/

/**@ingroup txc_document_group
* Create a custom XCAP document selector URI as per <a href="http://tools.ietf.org/html/rfc4825#section-6.2">RFC 4825 subclause 6.2</a>
* @param xcaproot The XCAP Root URI. A context that contains all the documents across all
* application usages and users that are managed by the server. For more information see <a href="http://tools.ietf.org/html/rfc4825#section-4">RFC 4825 subclause 4</a>
* @param auid The Application Unique ID.  unique identifier within the namespace of application unique IDs created by this specification
* that differentiates XCAP resources accessed by one application from XCAP resources accessed by another.
* For more information see <a href="http://tools.ietf.org/html/rfc4825#section-4">RFC 4825 subclause 4</a>
* @param xui The XCAP User Identifier. The XUI is a string, valid as a path element in an HTTP URI, that is associated with each user served
*  by the XCAP server. For more information see <a href="http://tools.ietf.org/html/rfc4825#section-4">RFC 4825 subclause 4</a>.
* For global document, this parameter must be NULL or equal to "global"
* @param name The document name.
* @retval The HTTP URI containing the XCAP root and document selector, resulting in the selection of a specific document.  As a
  result, performing a GET against the document URI would retrieve the document.
* If supplied parameters are not valide this method will return NULL.
* You must free the returned string using TSK_SAFE_FREE2 or TSK_SAFE_FREE.
* @sa @ref TXC_DOC_GET_SEL and @ref TXC_DOC_GET_SEL2
*/
char* txc_doc_get_cust_sel(const char *xcaproot, const char *auid, const char *xui, const char *name)
{
	char *selector = 0;

	if(!xcaproot || !auid || !name) return 0;

	if(!xui || tsk_equals("global", xui))
	{	/* xdm-root/auid-name/global/doc-name */
		tsk_sprintf(0, &selector, "%s/%s/global/%s", xcaproot, auid, name);
	}
	else
	{	/* xdm-root/auid-name/users/xui/doc-name */
		tsk_sprintf(0, &selector, "%s/%s/users/%s/%s", xcaproot, auid, xui, name);
	}

	return selector;
}