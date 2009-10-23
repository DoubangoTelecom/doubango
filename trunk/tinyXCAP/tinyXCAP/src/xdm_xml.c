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
#include "xdm_xml.h"
#include "xdm_utils.h"


/* init namespace */
void xdm_xml_namespace_init(xdm_xml_namespace_t* namespace)
{
	memset(namespace, 0, sizeof(xdm_xml_namespace_t));
}

/* free namespace */
void xdm_xml_namespace_free(xdm_xml_namespace_t** namespace)
{
	XDM_SAFE_FREE((*namespace)->prefix);
	XDM_SAFE_FREE((*namespace)->value);
	free(*namespace);

	(*namespace) = 0;
}

/* init element */
void xdm_xml_element_init(xdm_xml_element_t* element)
{
	memset(element, 0, sizeof(xdm_xml_element_t));
}

/* init element and set values*/
void xdm_xml_element_init_set(xdm_xml_element_t** element, const char* name, const char* value, xdm_xml_type_t type)
{
	XDM_XML_ELEMENT_CREATE((*element));
	XDM_LIST_CREATE((*element)->elements);
	XDM_LIST_CREATE((*element)->attributes);
	XDM_LIST_CREATE((*element)->namespaces);
	(*element)->name = xdm_strdup(name);
	(*element)->value = xdm_strdup(value);
	(*element)->type = type;
}

/* free element */
void xdm_xml_element_free(void** _element)
{
	xdm_xml_element_t** element = (xdm_xml_element_t**)_element;

	XDM_SAFE_FREE((*element)->name);
	XDM_SAFE_FREE((*element)->value);
	XDM_LIST_SAFE_FREE((*element)->elements);
	XDM_LIST_SAFE_FREE((*element)->attributes);
	XDM_LIST_SAFE_FREE((*element)->namespaces);

	free(*element);
	(*element) = 0;
}

/* init attribute */
void xdm_xml_attribute_init(xdm_xml_attribute_t* attribute)
{
	memset(attribute, 0, sizeof(xdm_xml_attribute_t));
}

/* free attribute */
void xdm_xml_attribute_free(xdm_xml_attribute_t** attribute)
{
	XDM_SAFE_FREE((*attribute)->name);
	XDM_SAFE_FREE((*attribute)->value);

	free(*attribute);
	(*attribute) = 0;
}

/* get namespace from xml document */
xmlNsPtr xdm_xml_get_namespace(xmlDocPtr docPtr, xmlNodePtr node, const char *href)
{
	xmlNs *ns = *xmlGetNsList(docPtr, node);
	while (ns)
	{
		if (!strcmp(ns->href, href)) return ns;
		else ns = ns->next;
	}

	return 0;
}

/* find node by name */
xmlNodePtr xdm_xml_find_node(const xmlNodePtr curr, const char* name, xdm_xml_node_find_type_t ftype)
{
	xmlNodePtr node = curr;
	
	while(node)
	{
		switch(ftype)
		{
		case nft_none:		return (!xdm_stricmp(node->name, name))? node : 0;
		case nft_children: node = node->children; break;
		case nft_parent:	node = node->parent; break;
		case nft_next:		node = node->next; break;
		case nft_prev:		node = node->prev; break;
		default: return 0;
		} /* switch */

		/* check and return value if match */
		if( node && (!name || !xdm_stricmp(node->name, name)) ) return node;
	}

	return 0;
}

/* select node */
xmlNodePtr xdm_xml_select_node(const xmlNodePtr root, ...)
{
	va_list list;
	int step;
	char* root_name = 0;
	xmlNodePtr node = root;

	if(!node || !(node->name)) return 0;

	/* initialize variable arguments */
	va_start(list, root);

	while( node && (step=va_arg(list, xdm_xml_node_select_type_t)) != nst_end)
	{
		switch(step)
		{
		case nst_by_name:
			{	/* name */
				const char* qname = va_arg(list, const char*);
				if(!xdm_stricmp(root->name, qname)){
					node = xdm_xml_find_node(node, 0, nft_children);
				}
				else{
					if(xdm_stricmp(node->name, qname))
					{	/* do not match */
						node = xdm_xml_find_node(node, qname, nft_next);
					}
					else
					{ /* already match */
						node = node->children;
					}
				}
				break;
			}

		case nst_content:
			{	/**/
				node = xdm_xml_find_node(node, 0, nft_children);
				break;
			}

		case nst_att_value:
			{	/* qname, att_name */
				xmlAttrPtr attrPtr = 0;
				int found = 0;
				const char* qname = va_arg(list, const char*);
				const char* att_name = va_arg(list, const char*);
				node = xdm_xml_find_node(node, qname, nft_none);
				while( node && !found )
				{
					attrPtr = node->properties;
					while(attrPtr)
					{
						if(attrPtr->type == XML_ATTRIBUTE_NODE && attrPtr->children)
						{
							if( !xdm_stricmp(attrPtr->name, att_name) ){
								node = attrPtr->children;
								found = 1;
							}
						}
						attrPtr = attrPtr->next;
					}
					if(!found) node = xdm_xml_find_node(node, 0, nft_next);
				}
				break;
			}

		case nst_by_att:
			{	/* qname att_name att_value */
				xmlAttrPtr attrPtr = 0;
				int found = 0;
				const char* qname = va_arg(list, const char*);
				const char* att_name = va_arg(list, const char*);
				const char* att_value = va_arg(list, const char*);
				node = xdm_xml_find_node(node, qname, nft_none);
				while( node && !found )
				{
					attrPtr = node->properties;
					while(attrPtr)
					{
						if(attrPtr->type == XML_ATTRIBUTE_NODE && attrPtr->children)
						{
							if( !xdm_stricmp(attrPtr->name, att_name)
								&& ( (attrPtr->children->content && !xdm_stricmp(attrPtr->children->content, att_value)) || !att_value )
								){
									found = 1;
							}
						}
						attrPtr = attrPtr->next;
					}
					if(!found) node = xdm_xml_find_node(node, 0, nft_next);
				}

				if(found && node) break;
				else return 0;

				break;
			}
		default: return 0;
		} /* switch */

		/* skip all comments */
		XDM_XML_NODE_SKIP_COMMENTS(node);

	} /* while*/

	return node;
}