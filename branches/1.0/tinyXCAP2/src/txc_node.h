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

/**@file txc_node.h
 * @brief RFC 4825 subclause <a href="http://tools.ietf.org/html/rfc4825#section-6.4">6.4 (Namespace Bindings for the Selector
)</a> and subclause <a href="http://tools.ietf.org/html/rfc4825#section-6.3">6.3 (Node Selector)</a>
 *
 * 
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */

#ifndef _TINYXCAP_TXC_ELEMENT_H_
#define _TINYXCAP_TXC_ELEMENT_H_

#include "tinyxcap_config.h"
#include "txc.h"

/**@ingroup txc_node_group
* Node selection step. Select by name.
* As per <a href="http://tools.ietf.org/html/rfc4825#section-6.3">RFC 4825 subclause 6.3</a>
* @param qname The QName grammar is defined in the XML namespaces [<a href="http://tools.ietf.org/html/rfc4825#ref-3">3</a>] specification.
*/
#define TXC_NODE_SELECT_BY_NAME(qname)										by_name, qname

/**@ingroup txc_node_group
* Node selection step. Select by position.
* As per <a href="http://tools.ietf.org/html/rfc4825#section-6.3">RFC 4825 subclause 6.3</a>
* @param qname The QName grammar is defined in the XML namespaces [<a href="http://tools.ietf.org/html/rfc4825#ref-3">3</a>] specification.
* @param position 1*DIGIT
*/
#define TXC_NODE_SELECT_BY_POS(qname, position)								by_pos, qname, position

/**@ingroup txc_node_group
* Node selection step. Select by attribute.
* As per <a href="http://tools.ietf.org/html/rfc4825#section-6.3">RFC 4825 subclause 6.3</a>
* @param qname The QName grammar is defined in the XML namespaces [<a href="http://tools.ietf.org/html/rfc4825#ref-3">3</a>] specification.
* @param att_name QName
* @param att_value AttValue grammar is defined in the XML specification XML 1.0 [<a href="http://tools.ietf.org/html/rfc4825#ref-1">1</a>].
*/
#define TXC_NODE_SELECT_BY_ATTR(qname, att_name, att_value)					by_attr, qname, att_name, att_value

/**@ingroup txc_node_group
* Node selection step. Select by position and attribute.
* As per <a href="http://tools.ietf.org/html/rfc4825#section-6.3">RFC 4825 subclause 6.3</a>
* @param qname The QName grammar is defined in the XML namespaces [<a href="http://tools.ietf.org/html/rfc4825#ref-3">3</a>] specification.
* @param position 1*DIGIT
* @param att_name QName
* @param att_value AttValue grammar is defined in the XML specification XML 1.0 [<a href="http://tools.ietf.org/html/rfc4825#ref-1">1</a>].
*/
#define TXC_NODE_SELECT_BY_POS_ATTR(qname, position, att_name, att_value)	by_pos_attr, qname, position, att_name, att_value


/**@ingroup txc_node_group
* Node selection step. Append XML namespace("namespace::*").
* As per <a href="http://tools.ietf.org/html/rfc4825#section-6.3">RFC 4825 subclause 6.3</a>
* @param prefix The XML namespace prefix
* @param ns The XML namespace value
*/
#define TXC_NODE_ADD_NAMESPACE(prefix, ns)									0xF0, prefix, ns

/**@ingroup txc_node_group
* Node selection step. Close/End selection.
* As per <a href="http://tools.ietf.org/html/rfc4825#section-6.3">RFC 4825 subclause 6.3</a>
* You MUST always be set at last position. Is mandatory.
*/
#define TXC_NODE_SELECT_END()												0xFF

TINYXCAP_API char* txc_node_get_cust_sel(const char* doc_selector, const char *auid, ...);
TINYXCAP_API char* txc_node_get_sel(const txc_context_t* context, const xcap_auid_type_t auid_type, ...);

#endif /* _TINYXCAP_TXC_ELEMENT_H_ */