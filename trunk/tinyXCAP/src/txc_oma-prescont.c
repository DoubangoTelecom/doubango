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
/**@file txc_oma-prescont.c
 * @brief <a href="http://www.openmobilealliance.org/technical/release_program/docs/PresenceSIMPLE/V2_0-20081223-C/OMA-TS-Presence_SIMPLE_Content_XDM-V1_0-20081223-C.pdf">[OMA-TS-Presence-SIMPLE_Content_XDM-V1_0-20081223-C] - OMA Presence Content (Avatar)</a>
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */

#include "txc_oma-prescont.h"

/**@defgroup txc_oma_prescont_group OMA Presence Content (Avatars)
*/

/**@page txc_oma_prescont_page OMA Presence Content Tutorial (org.openmobilealliance.pres-content)
* @par Application Unique ID (AUID)
* - '<span style="text-decoration:underline;">org.openmobilealliance.pres-content</span>' as per [OMA-TS-Presence-SIMPLE_Content_XDM-V1_0-20081223-C] subclause 5.1.2
* @par Default Document Namespace
* - '<span style="text-decoration:underline;">urn:oma:xml:prs:pres-content</span>' as per [OMA-TS-Presence-SIMPLE_Content_XDM-V1_0-20081223-C] subclause 5.1.4
* @par MIME Type
* - '<span style="text-decoration:underline;">application/vnd.oma.pres-content+xml</span>' as per [OMA-TS-Presence-SIMPLE_Content_XDM-V1_0-20081223-C] subclause 5.1.5
* @par Default document name
* - '<span style="text-decoration:underline;">oma_status-icon/rcs_status_icon</span>' as per [R1_090831_RCS_Release_1_Technical_Realisation_v1_1] subclause 4.8.1
*
* <H2>=== Create/deserialize an OMA Presence Content (avatar) document received from the XDMS ===</H2>
*
* @code
#include "txc_api.h"

// OMA pres-cont context
txc_oma_prescont_t* omaprescont = 0;

printf("\n---\nTEST OMA PRES-CONTENT\n---\n");
{
	// create oma-pres-content context
	omaprescont = txc_oma_prescont_create(buffer, size);

	// dump
	printf("\ndump pres-content\n");
	if(omaprescont)
	{
		printf("mime-type: \"%s\" encoding: \"%s\" description: \"%s\" data: \"%s\"\n", 
			omaprescont->mime_type, omaprescont->encoding, omaprescont->description, omaprescont->data);
	}
	// free omapres-content context
	txc_oma_prescont_free(&omaprescont);
}
* @endcode
*
*/

/**@ingroup txc_oma_prescont_group
* Create an OMA presence content object from an XML string.
* @param buffer The XML string from which to create the pres-cont object
* @param size The size of the XML string
* @retval The OMA pres-cont object. You MUST call @ref txc_oma_prescont_free to free the returned object.
* @sa @ref txc_oma_prescont_free
*/
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
		pres_cont = (txc_oma_prescont_t*)tsk_calloc(1, sizeof(txc_oma_prescont_t));
		do
		{
			if( tsk_strequals(node->name, "mime-Type") )
			{
				pres_cont->mime_type = tsk_strdup(TSK_XML_NODE_SAFE_GET_TEXTVALUE(node->children));
			}
			else if( tsk_strequals(node->name, "encoding") )
			{
				pres_cont->encoding = tsk_strdup(TSK_XML_NODE_SAFE_GET_TEXTVALUE(node->children));
			}
			else if( tsk_strequals(node->name, "description") )
			{
				pres_cont->description = tsk_strdup(TSK_XML_NODE_SAFE_GET_TEXTVALUE(node->children));
			}
			else if( tsk_strequals(node->name, "data") )
			{
				pres_cont->data = tsk_strdup(TSK_XML_NODE_SAFE_GET_TEXTVALUE(node->children));
			}
		}
		while(node = tsk_xml_find_node(node, 0, nft_next));
	}


	xmlFreeDoc(docPtr);
	return pres_cont;
}

/**@ingroup txc_oma_prescont_group
* Free an OMA presence content object previously created using @ref txc_oma_prescont_create.
* @param pres_cont The object to free.
* @sa @ref txc_oma_prescont_create
*/
void txc_oma_prescont_free(txc_oma_prescont_t **pres_cont)
{
	TSK_FREE((*pres_cont)->mime_type);
	TSK_FREE((*pres_cont)->encoding);
	TSK_FREE((*pres_cont)->description);
	TSK_FREE((*pres_cont)->data);

	free(*pres_cont);
	(*pres_cont) = 0;
}
