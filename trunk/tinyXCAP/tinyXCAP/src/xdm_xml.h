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
#ifndef _TINYXCAP_XDM_XML_H_
#define _TINYXCAP_XDM_XML_H_

#include "tinyxcap_config.h"
#include "xdm_list.h"
#include <libxml/tree.h>

#define XDM_XML_NODE_IS_TEXTVALUE(node)							(node && node->type==XML_TEXT_NODE)
#define XDM_XML_NODE_SAFE_GET_TEXTVALUE(node)					XDM_XML_NODE_IS_TEXTVALUE(node)?(node->content):0
#define XDM_XML_NODE_SKIP_COMMENTS(node)						while(node && (node->type==XML_COMMENT_NODE)) node = xdm_xml_find_node(node, 0, nft_next);
		

#define XDM_XML_NODE_SELECT_BY_NAME(qname)						nst_by_name, qname
#define XDM_XML_NODE_SELECT_BY_ATT(qname, att_name, att_value)	nst_by_att, qname, att_name, att_value
#define XDM_XML_NODE_SELECT_ATT_VALUE(qname, att_name)			nst_att_value, qname, att_name
#define XDM_XML_NODE_SELECT_CONTENT()							nst_content
#define XDM_XML_NODE_SELECT_END()								nst_end

#define XDM_XML_NAMESPACE_CREATE(namespace) XDM_XXX_CREATE(namespace, xml_namespace)
#define XDM_XML_NAMESPACE_FREE(namespace) XDM_XXX_SAFE_FREE(namespace, xml_namespace)

#define XDM_XML_ELEMENT_CREATE(element) XDM_XXX_CREATE(element, xml_element)
#define XDM_XML_ELEMENT_FREE(element) XDM_XXX_SAFE_FREE(element, xml_element)

#define XDM_XML_ATTRIBUTE_CREATE(attribute) XDM_XXX_CREATE(attribute, xml_attribute)
#define XDM_XML_ATTRIBUTE_FREE(attribute) XDM_XXX_SAFE_FREE(attribute, xml_attribute)	

/* C and XML Schema mapping */
typedef enum xdm_xml_type_e
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
xdm_xml_type_t;

/* node finding types */
typedef enum xdm_xml_node_find_type_s
{
	nft_none,
	nft_children,
	nft_parent,
	nft_next,
	nft_prev
}
xdm_xml_node_find_type_t;

/* elt selection types */
typedef enum xdm_xml_node_select_type_s
{
	nst_by_name,
	nst_by_att,
	nst_att_value,
	nst_content,
	nst_end
}
xdm_xml_node_select_type_t;

/* xml namespace */
typedef struct xdm_xml_namespace_s
{
	char* prefix;
	char* value;
}
xdm_xml_namespace_t;

/* xml attribute */
typedef struct xdm_xml_attribute_s
{
	char* name;
	void* value;
	xdm_xml_type_t type;
}
xdm_xml_attribute_t;

/* xml element */
typedef struct xdm_xml_element_s
{
	char* name;
	void* value;
	xdm_list_t* elements;
	xdm_list_t* attributes;
	xdm_list_t* namespaces;
	xdm_xml_type_t type;
}
xdm_xml_element_t;

typedef xdm_list_t xdm_xml_elements_t;
typedef xdm_list_t xdm_xml_attributes_t;
typedef xdm_list_t xdm_xml_namespaces_t;

#define xdm_xml_elements_init		xdm_list_init
#define xdm_xml_attributes_init		xdm_list_init
#define xdm_xml_namespaces_init		xdm_list_init

#define xdm_xml_elements_add		xdm_list_add_data
#define xdm_xml_attributes_add		xdm_list_add_data
#define xdm_xml_namespaces_add		xdm_list_add_data

#define xdm_xml_elements_free		xdm_list_free
#define xdm_xml_attributes_free		xdm_list_free
#define xdm_xml_namespaces_free		xdm_list_free

void xdm_xml_namespace_init(xdm_xml_namespace_t* namespace);
void xdm_xml_namespace_free(xdm_xml_namespace_t** namespace);

void xdm_xml_element_init(xdm_xml_element_t* element);
void xdm_xml_element_init_set(xdm_xml_element_t** element, const char* name, const char* value, xdm_xml_type_t type);
void xdm_xml_element_free(void** element);

void xdm_xml_attribute_init(xdm_xml_attribute_t* attribute);
void xdm_xml_attribute_free(xdm_xml_attribute_t** attribute);

xmlNsPtr xdm_xml_get_namespace(xmlDocPtr docPtr, xmlNodePtr node, const char *href);
xmlNodePtr xdm_xml_find_node(const xmlNodePtr curr, const char* name, xdm_xml_node_find_type_t ftype);
xmlNodePtr xdm_xml_select_node(const xmlNodePtr root, ...);


#endif /* _TINYXCAP_XDM_XML_H_ */