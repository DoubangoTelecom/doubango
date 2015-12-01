/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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

/**@file tsk_xml.c
 * @brief Useful functions to manipulate xml documents.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tsk_xml.h"

#if HAVE_LIBXML2_H

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_macros.h"

#include <string.h>

/**@defgroup tsk_xml_group XML
*/

/**@page tsk_xml_page XML Tutorial
*/


/**@ingroup tsk_xml_group
* Initialize an XML namespace
* @param namespace The XML namespace to initialize.
*/
/*void tsk_xml_namespace_init(tsk_xml_namespace_t* namespace)
{
}*/

/**@ingroup tsk_xml_group
* Free an XML namespace
* @param namespace The namespace to free
*/
/*void tsk_xml_namespace_free(tsk_xml_namespace_t** namespace)
{
	TSK_FREE((*namespace)->prefix);
	TSK_FREE((*namespace)->value);
	TSK_FREE(namespace);
}*/

/**@ingroup tsk_xml_group
* Initialize an XML element
* @param element The XML element to initialize
*/
/*void tsk_xml_element_init(tsk_xml_element_t* element)
{
}*/

/**@ingroup tsk_xml_group
* Initialize an XML element and set values
* @param element The XML element to initialize
* @param name The element name
* @param value The element value
* @param type The element type
*/
void tsk_xml_element_init_set(tsk_xml_element_t** element, const char* name, const char* value, tsk_xml_type_t type)
{
	TSK_XML_ELEMENT_CREATE((*element));
	(*element)->elements = TSK_LIST_CREATE();
	(*element)->attributes = TSK_LIST_CREATE();
	(*element)->namespaces = TSK_LIST_CREATE();
	(*element)->name = tsk_strdup(name);
	(*element)->value = tsk_strdup(value);
	(*element)->type = type;
}

/**@ingroup tsk_xml_group
* Free an XML element
* @param _element The XML element to free
*/
/*void tsk_xml_element_free(void** _element)
{
	tsk_xml_element_t** element = (tsk_xml_element_t**)_element;

	TSK_FREE((*element)->name);
	TSK_FREE((*element)->value);
	TSK_OBJECT_SAFE_FREE((*element)->elements);
	TSK_OBJECT_SAFE_FREE((*element)->attributes);
	TSK_OBJECT_SAFE_FREE((*element)->namespaces);

	TSK_FREE(element);
}*/

/**@ingroup tsk_xml_group
* Initialize an XML attribute
* @param attribute The XML attribute to initialize
*/
/*void tsk_xml_attribute_init(tsk_xml_attribute_t* attribute)
{
}*/

/**@ingroup tsk_xml_group
* Free an XML attribute
* @param attribute The XML attribute to free
*/
/*void tsk_xml_attribute_free(tsk_xml_attribute_t** attribute)
{
	TSK_FREE((*attribute)->name);
	TSK_FREE((*attribute)->value);

	TSK_FREE(attribute);
}*/

/**@ingroup tsk_xml_group
* Get an XML namespace from an XML document
* @param docPtr A pointer to the XML document
* @param node The XML node from which to extract the namespace
* @param href The namespace href
* @retval The Namespace value matching our criteria (href)
*/
xmlNsPtr tsk_xml_get_namespace(xmlDocPtr docPtr, xmlNodePtr node, const char *href)
{
	xmlNs *ns = *xmlGetNsList(docPtr, node);
	while (ns)
	{
		if (tsk_striequals(ns->href, href)) return ns;
		else ns = ns->next;
	}

	return 0;
}

/**@ingroup tsk_xml_group
* Find an XML node by name
* @param curr The XML node from which to start
* @param name The name of the XML node to find
* @param ftype The find type
* @retval Returns the node which match our criteria. If none match, this method returns NULL.
*/
xmlNodePtr tsk_xml_find_node(const xmlNodePtr curr, const char* name, tsk_xml_node_find_type_t ftype)
{
	xmlNodePtr node = curr;
	
	while(node)
	{
		switch(ftype)
		{
		case nft_none:		return (tsk_striequals(node->name, name))? node : 0;
		case nft_children: node = node->children; break;
		case nft_parent:	node = node->parent; break;
		case nft_next:		node = node->next; break;
		case nft_prev:		node = node->prev; break;
		default: return 0;
		} /* switch */

		/* check and return value if match */
		if( node && (!name || tsk_striequals(node->name, name)) ) 
		//if( node && (name == 0 || !tsk_stricmp((const char*)node->name, name)) ) 
		{
			return node;
		}
	}

	return 0;
}

/**@ingroup tsk_xml_group
* Select an XML node
* @retval Returns the pointer to the node which match our criteria. If none match, this method returns NULL.
*/
xmlNodePtr tsk_xml_select_node(const xmlNodePtr root, ...)
{
	va_list list;
	int step;
	char* root_name = 0;
	xmlNodePtr node = root;

	if(!node || !(node->name)) return 0;

	/* initialize variable arguments */
	va_start(list, root);

	while( node && (step=va_arg(list, tsk_xml_node_select_type_t)) != nst_end)
	{
		switch(step)
		{
		case nst_by_name:
			{	/* name */
				const char* qname = va_arg(list, const char*);
				if(tsk_striequals(root->name, qname)){
					node = tsk_xml_find_node(node, 0, nft_children);
				}
				else{
					if(!tsk_striequals(node->name, qname))
					{	/* do not match */
						node = tsk_xml_find_node(node, qname, nft_next);
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
				node = tsk_xml_find_node(node, 0, nft_children);
				break;
			}

		case nst_att_value:
			{	/* qname, att_name */
				xmlAttrPtr attrPtr = 0;
				int found = 0;
				const char* qname = va_arg(list, const char*);
				const char* att_name = va_arg(list, const char*);
				node = tsk_xml_find_node(node, qname, nft_none);
				while( node && !found )
				{
					attrPtr = node->properties;
					while(attrPtr)
					{
						if(attrPtr->type == XML_ATTRIBUTE_NODE && attrPtr->children)
						{
							if( tsk_striequals(attrPtr->name, att_name) ){
								node = attrPtr->children;
								found = 1;
							}
						}
						attrPtr = attrPtr->next;
					}
					if(!found) node = tsk_xml_find_node(node, 0, nft_next);
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
				node = tsk_xml_find_node(node, qname, nft_none);
				while( node && !found )
				{
					attrPtr = node->properties;
					while(attrPtr)
					{
						if(attrPtr->type == XML_ATTRIBUTE_NODE && attrPtr->children)
						{
							if( tsk_striequals(attrPtr->name, att_name)
								&& ( (attrPtr->children->content && tsk_striequals(attrPtr->children->content, att_value)) || !att_value )
								){
									found = 1;
							}
						}
						attrPtr = attrPtr->next;
					}
					if(!found) node = tsk_xml_find_node(node, 0, nft_next);
				}

				if(found && node) break;
				else return 0;

				break;
			}
		default: return 0;
		} /* switch */

		/* skip all comments */
		TSK_XML_NODE_SKIP_COMMENTS(node);

	} /* while*/

	return node;
}

#endif /* HAVE_LIBXML2_H */

