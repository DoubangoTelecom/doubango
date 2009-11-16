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

/**@file tsk_xml.h
 * @brief Useful functions to manipulate xml document.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TINYSAK_XML_H_
#define _TINYSAK_XML_H_

#include "tinySAK_config.h"
#include "tsk_heap.h"
#include "tsk_list.h"
#include <libxml/tree.h>

#define TSK_XML_NODE_IS_TEXTVALUE(node)							(node && node->type==XML_TEXT_NODE)
#define TSK_XML_NODE_SAFE_GET_TEXTVALUE(node)					TSK_XML_NODE_IS_TEXTVALUE(node)?(node->content):0
#define TSK_XML_NODE_SKIP_COMMENTS(node)						while(node && (node->type==XML_COMMENT_NODE)) node = tsk_xml_find_node(node, 0, nft_next);
		

#define TSK_XML_NODE_SELECT_BY_NAME(qname)						nst_by_name, qname
#define TSK_XML_NODE_SELECT_BY_ATT(qname, att_name, att_value)	nst_by_att, qname, att_name, att_value
#define TSK_XML_NODE_SELECT_ATT_VALUE(qname, att_name)			nst_att_value, qname, att_name
#define TSK_XML_NODE_SELECT_CONTENT()							nst_content
#define TSK_XML_NODE_SELECT_END()								nst_end

#define TSK_XML_NAMESPACE_CREATE(namespace) TSK_XXX_CREATE(0, namespace, xml_namespace)
#define TSK_XML_NAMESPACE_FREE(namespace) TSK_XXX_SAFE_FREE(0, namespace, xml_namespace)

#define TSK_XML_ELEMENT_CREATE(element) TSK_XXX_CREATE(0, element, xml_element)
#define TSK_XML_ELEMENT_FREE(element) TSK_XXX_SAFE_FREE(0, element, xml_element)

#define TSK_XML_ATTRIBUTE_CREATE(attribute) TSK_XXX_CREATE(0, attribute, xml_attribute)
#define TSK_XML_ATTRIBUTE_FREE(attribute) TSK_XXX_SAFE_FREE(0, attribute, xml_attribute)

#define TSK_XML_SERIALIZE(result, element, strValue)\
	if(strValue) tsk_sprintf(0, &result, "<"##element##">%s</"##element##">", strValue); \
	else tsk_strupdate2(&result, "");

/** C and XML Schema mapping */
typedef enum tsk_xml_type_e
{
	xt_none,
	xt_string,			/* xsd:string */
	xt_byte,			/* xsd:byte */
	xt_unsignedByte,	/* xsd:unsignedByte */
	xt_short,			/* xsd:short */
	xt_unsignedShort,	/* xsd:unsignedShort */
	xt_int,				/* xsd:int */
	xt_unsignedInt,		/* xsd:unsignedInt */
	xt_long,			/* xsd:long */
	xt_boolean,			/* xsd:boolean */
	xt_float,			/* xsd:float */
	xt_double			/* xsd:double */
}
tsk_xml_type_t;

/** Node finding types */
typedef enum tsk_xml_node_find_type_s
{
	nft_none,
	nft_children,
	nft_parent,
	nft_next,
	nft_prev
}
tsk_xml_node_find_type_t;

/** Element selection types */
typedef enum tsk_xml_node_select_type_s
{
	nst_by_name,
	nst_by_att,
	nst_att_value,
	nst_content,
	nst_end
}
tsk_xml_node_select_type_t;

/** XML namespace */
typedef struct tsk_xml_namespace_s
{
	char* prefix;
	char* value;
}
tsk_xml_namespace_t;

/** XML attribute */
typedef struct tsk_xml_attribute_s
{
	char* name;
	void* value;
	tsk_xml_type_t type;
}
tsk_xml_attribute_t;

/** XML element */
typedef struct tsk_xml_element_s
{
	char* name;
	void* value;
	tsk_list_t* elements;
	tsk_list_t* attributes;
	tsk_list_t* namespaces;
	tsk_xml_type_t type;
}
tsk_xml_element_t;

typedef tsk_list_t tsk_xml_elements_t;
typedef tsk_list_t tsk_xml_attributes_t;
typedef tsk_list_t tsk_xml_namespaces_t;

#define tsk_xml_elements_init		tsk_list_init
#define tsk_xml_attributes_init		tsk_list_init
#define tsk_xml_namespaces_init		tsk_list_init

#define tsk_xml_elements_add		tsk_list_add_data
#define tsk_xml_attributes_add		tsk_list_add_data
#define tsk_xml_namespaces_add		tsk_list_add_data

#define tsk_xml_elements_free		tsk_list_free
#define tsk_xml_attributes_free		tsk_list_free
#define tsk_xml_namespaces_free		tsk_list_free

TINYSAK_API void tsk_xml_namespace_init(tsk_xml_namespace_t* _namespace);
TINYSAK_API void tsk_xml_namespace_free(tsk_xml_namespace_t** _namespace);

TINYSAK_API void tsk_xml_element_init(tsk_xml_element_t* element);
TINYSAK_API void tsk_xml_element_init_set(tsk_xml_element_t** element, const char* name, const char* value, tsk_xml_type_t type);
TINYSAK_API void tsk_xml_element_free(void** element);

TINYSAK_API void tsk_xml_attribute_init(tsk_xml_attribute_t* attribute);
TINYSAK_API void tsk_xml_attribute_free(tsk_xml_attribute_t** attribute);

TINYSAK_API xmlNsPtr tsk_xml_get_namespace(xmlDocPtr docPtr, xmlNodePtr node, const char *href);
TINYSAK_API xmlNodePtr tsk_xml_find_node(const xmlNodePtr curr, const char* name, tsk_xml_node_find_type_t ftype);
TINYSAK_API xmlNodePtr tsk_xml_select_node(const xmlNodePtr root, ...);

#endif /* _TINYSAK_XML_H_ */