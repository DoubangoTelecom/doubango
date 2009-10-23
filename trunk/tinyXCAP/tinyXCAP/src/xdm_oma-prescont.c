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
/* implements [OMA-TS-Presence-SIMPLE_Content_XDM-V1_0-20081223-C] */
#include "xdm_oma-prescont.h"
#include "xdm_utils.h"


//static const char* xdm_oma_prescont_ns = "urn:oma:xml:prs:pres-content";

/* create oma pres-content object */
/* ATTENTION: use 'xdm_oma_pres_cont_free' function to free the returned object*/
xdm_oma_prescont_t* xdm_oma_prescont_create(const char* buffer, size_t size)
{
	xmlNodePtr node = 0;
	xmlDocPtr docPtr = 0;
	xdm_oma_prescont_t* pres_cont = 0;

	/* parse xml document */
	docPtr = xmlParseMemory(buffer, (int)size);
	if(!docPtr || !docPtr->children) return 0;

	node = xdm_xml_select_node(docPtr->children,
		XDM_XML_NODE_SELECT_BY_NAME("content"),
		XDM_XML_NODE_SELECT_END());

	/* select content */
	if(node)
	{
		/* init pres-cont object */
		pres_cont = (xdm_oma_prescont_t*)malloc(sizeof(xdm_oma_prescont_t));
		memset(pres_cont, 0, sizeof(xdm_oma_prescont_t));
		do
		{
			if( !xdm_stricmp(node->name, "mime-Type") )
			{
				pres_cont->mime_type = xdm_strdup(XDM_XML_NODE_SAFE_GET_TEXTVALUE(node->children));
			}
			else if(!xdm_stricmp(node->name, "encoding") )
			{
				pres_cont->encoding = xdm_strdup(XDM_XML_NODE_SAFE_GET_TEXTVALUE(node->children));
			}
			else if( !xdm_stricmp(node->name, "description") )
			{
				pres_cont->description = xdm_strdup(XDM_XML_NODE_SAFE_GET_TEXTVALUE(node->children));
			}
			else if( !xdm_stricmp(node->name, "data") )
			{
				pres_cont->data = xdm_strdup(XDM_XML_NODE_SAFE_GET_TEXTVALUE(node->children));
			}
		}
		while(node = xdm_xml_find_node(node, 0, nft_next));
	}


	xmlFreeDoc(docPtr);
	return pres_cont;
}

/* init oma pres-content object */
void xdm_oma_prescont_free(xdm_oma_prescont_t **pres_cont)
{
	XDM_SAFE_FREE((*pres_cont)->mime_type);
	XDM_SAFE_FREE((*pres_cont)->encoding);
	XDM_SAFE_FREE((*pres_cont)->description);
	XDM_SAFE_FREE((*pres_cont)->data);

	free(*pres_cont);
	(*pres_cont) = 0;
}
