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
/**@file txc_document.h
 * @brief <a href="http://tools.ietf.org/html/rfc4825#section-6.2">RFC 4825 subclause 6.2.  Document Selector</a>
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */

#ifndef _TINYXCAP_TXC_DOCUMENT_H_
#define _TINYXCAP_TXC_DOCUMENT_H_

#include "tinyxcap_config.h"
#include "txc.h"

TINYXCAP_API char* txc_doc_get_cust_sel(const char *xcaproot, const char *auid, const char *xui, const char *name);

/**@ingroup txc_document_group
* Get the document selector for a well-known auid. This method will also select for you the default document name as
* defined in the corresponding specification (in this priority -> RCS:OMA:GSMA:RFC:3GPP)
* @param context A valid XCAP context
* @param auid_type The document's AUID (typeof @ref xcap_auid_type_t)
* @retval The full document's selector URI
* @sa TXC_DOC_GET_SEL2
*/
#define TXC_DOC_GET_SEL(context, auid_type) txc_doc_get_cust_sel(context->txc_root, context->auids[auid_type].name, TXC_DOCUMENT_GLOBAL(auid_type)? 0 : context->xui, context->auids[auid_type].document)

/**@ingroup txc_document_group
* Instead of @ref TXC_DOC_GET_SEL, you can provide your own document name
*/
#define TXC_DOC_GET_SEL2(context, auid_type, doc_name) txc_doc_get_cust_sel(context->txc_root, context->auids[auid_type].name, TXC_DOCUMENT_GLOBAL(auid_type)? 0: context->xui, doc_name)

#endif /* _TINYXCAP_TXC_DOCUMENT_H_ */