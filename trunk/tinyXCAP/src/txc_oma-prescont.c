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
/* implements [OMA-TS-Presence-SIMPLE_Content_XDM-V1_0-20081223-C] */
#include "txc_oma-prescont.h"


//static const char* txc_oma_prescont_ns = "urn:oma:xml:prs:pres-content";

/* create oma pres-content object */
/* ATTENTION: use 'txc_oma_pres_cont_free' function to free the returned object*/
txc_oma_prescont_t* txc_oma_prescont_create(const char* buffer, size_t size)
{
	xmlNodePtr node = 0;
	xmlDocPtr docPtr = 0;
	txc_oma_prescont_t* pres_cont = 0;

	/* parse xml document */
	docPtr = xmlParseMemory(buffer, (int)size);
	if(!docPtr || !docPtr->children) return 0;

	node = tsk_xml_select_node(docPtr->children,
		TSK_XML_NODE_SELECT_BY_NAME("content"),
		TSK_XML_NODE_SELECT_END());

	/* select content */
	if(node)
	{
		/* init pres-cont object */
		pres_cont = (txc_oma_prescont_t*)malloc(sizeof(txc_oma_prescont_t));
		memset(pres_cont, 0, sizeof(txc_oma_prescont_t));
		do
		{
			if( !tsk_stricmp(node->name, "mime-Type") )
			{
				pres_cont->mime_type = tsk_strdup(0, TSK_XML_NODE_SAFE_GET_TEXTVALUE(node->children));
			}
			else if(!tsk_stricmp(node->name, "encoding") )
			{
				pres_cont->encoding = tsk_strdup(0, TSK_XML_NODE_SAFE_GET_TEXTVALUE(node->children));
			}
			else if( !tsk_stricmp(node->name, "description") )
			{
				pres_cont->description = tsk_strdup(0, TSK_XML_NODE_SAFE_GET_TEXTVALUE(node->children));
			}
			else if( !tsk_stricmp(node->name, "data") )
			{
				pres_cont->data = tsk_strdup(0, TSK_XML_NODE_SAFE_GET_TEXTVALUE(node->children));
			}
		}
		while(node = tsk_xml_find_node(node, 0, nft_next));
	}


	xmlFreeDoc(docPtr);
	return pres_cont;
}

/* init oma pres-content object */
void txc_oma_prescont_free(txc_oma_prescont_t **pres_cont)
{
	TSK_SAFE_FREE2((*pres_cont)->mime_type);
	TSK_SAFE_FREE2((*pres_cont)->encoding);
	TSK_SAFE_FREE2((*pres_cont)->description);
	TSK_SAFE_FREE2((*pres_cont)->data);

	free(*pres_cont);
	(*pres_cont) = 0;
}
