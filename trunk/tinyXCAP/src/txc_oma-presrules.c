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

/**@file txc_oma-presrules.c
 * @brief <a href="http://www.openmobilealliance.org/Technical/release_program/docs/PresenceSIMPLE/V1_1-20080627-A/OMA-TS-Presence_SIMPLE_XDM-V1_1-20080627-A.pdf">[OMA-TS-Presence_SIMPLE_XDM-V1_1-20080627-A] - OMA Presence Authorization Rules</a>
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */

/* as per RCS1: http://www.gsmworld.com/documents/rcs/rcs1_updates/R1_090831_RCS_Release_1_Technical_Realisation_v1_1.pdf subclause 4.4*/
#include "txc_oma-presrules.h"
#include "txc_document.h"
#include "txc_oma.h"

/**@defgroup txc_oma_pres_rules_group OMA Presence Authorization Rules
*/

/**@page txc_oma_pres_rules_page OMA Presence Authorization Rules Tutorial (org.openmobilealliance.pres-rules)
* @par Application Unique ID (AUID)
* - '<span style="text-decoration:underline;">org.openmobilealliance.pres-rules</span>' as per [OMA-TS-Presence_SIMPLE_XDM-V1_1-20080627-A] subclause 5.1.1.2
* @par Default Document Namespace
* - '<span style="text-decoration:underline;">urn:ietf:params:xml:ns:common-policy</span>' as per [OMA-TS-Presence_SIMPLE_XDM-V1_1-20080627-A] subclause 5.1.1.3
* @par MIME Type
* - '<span style="text-decoration:underline;">application/auth-policy+xml</span>' as per [OMA-TS-Presence_SIMPLE_XDM-V1_1-20080627-A] subclause 5.1.1.5
* @par Default document name
* - '<span style="text-decoration:underline;">pres-rules</span>' as per [OMA-TS-Presence_SIMPLE_XDM-V1_1-20080627-A] subclause 5.1.1.8
*/

/* 1= owner uri ; 2=oma_grantedcontacts uri; 3=oma_blockedcontacts*/
#define TXC_OMA_PRESRULES_TEMPLATE "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" \
									"<cr:ruleset " \
									"xmlns:ocp=\"urn:oma:xml:xdm:common-policy\" " \
									"xmlns:pr=\"urn:ietf:params:xml:ns:pres-rules\" " \
									"xmlns:cr=\"urn:ietf:params:xml:ns:common-policy\">" \
									/* wp_prs_allow_own */ \
									"<cr:rule id=\"wp_prs_allow_own\">" \
										"<cr:conditions>" \
											"<cr:identity>" \
												"<cr:one id=\"%s\"/>" \
											"</cr:identity>" \
										"</cr:conditions>" \
										"<cr:actions>" \
											"<pr:sub-handling>allow</pr:sub-handling>" \
										"</cr:actions>" \
										"<cr:transformations>" \
											"<pr:provide-services>" \
												"<pr:all-services/>" \
											"</pr:provide-services>" \
											"<pr:provide-persons>" \
												"<pr:all-persons/>" \
											"</pr:provide-persons>" \
											"<pr:provide-devices>" \
												"<pr:all-devices/>" \
											"</pr:provide-devices>" \
											"<pr:provide-all-attributes/>" \
										"</cr:transformations>" \
									"</cr:rule>" \
									/* <! -- This rule allows all service capabilities to be sent for anonymous requests --> */ \
									/* <! -- in order to realize the service capabilities to all requirement --> */ \
									/* <! -- This rule replaces the default “wp_prs_block_anonymous” rule --> */ \
									/* <! -- Note: May be modified to only allow RCS specified services --> */ \
									"<cr:rule id=\"rcs_allow_services_anonymous\">" \
										"<cr:conditions>" \
											"<ocp:anonymous-request/>" \
										"</cr:conditions>" \
										"<cr:actions>" \
											"<pr:sub-handling>allow</pr:sub-handling>" \
										"</cr:actions>" \
										"<cr:transformations>" \
											"<pr:provide-services>" \
												"<pr:all-services/>" \
											"</pr:provide-services>" \
											"<pr:provide-all-attributes/>" \
										"</cr:transformations>" \
									"</cr:rule>" \
									/* wp_prs_unlisted */ \
									"<cr:rule id=\"wp_prs_unlisted\">" \
										"<cr:conditions>" \
											"<ocp:other-identity/>" \
										"</cr:conditions>" \
										"<cr:actions>" \
											"<pr:sub-handling>confirm</pr:sub-handling>" \
										"</cr:actions>" \
									"</cr:rule>" \
									/* wp_prs_grantedcontacts */ \
									"<cr:rule id=\"wp_prs_grantedcontacts\">" \
										"<cr:conditions>" \
											"<ocp:external-list>" \
												"<ocp:entry anc=\"%s\"/>" \
											"</ocp:external-list>" \
										"</cr:conditions>" \
										"<cr:actions>" \
											"<pr:sub-handling>allow</pr:sub-handling>" \
										"</cr:actions>" \
										"<cr:transformations>" \
											"<pr:provide-services>" \
												"<pr:all-services/>" \
											"</pr:provide-services>" \
											"<pr:provide-persons>" \
												"<pr:all-persons/>" \
											"</pr:provide-persons>" \
											"<pr:provide-devices>" \
												"<pr:all-devices/>" \
											"</pr:provide-devices>" \
											"<pr:provide-all-attributes/>" \
										"</cr:transformations>" \
									"</cr:rule>" \
									/* wp_prs_blockedcontacts */ \
									"<cr:rule id=\"wp_prs_blockedcontacts\">" \
										"<cr:conditions>" \
											"<ocp:external-list>" \
												"<ocp:entry anc=\"%s\"/>" \
											"</ocp:external-list>" \
										"</cr:conditions>" \
										"<cr:actions>" \
											"<pr:sub-handling>block</pr:sub-handling>" \
										"</cr:actions>" \
										"</cr:rule>" \
									"" \
									"</cr:ruleset>"


/**@ingroup txc_oma_pres_rules_group
* Used to create an initial OMA pres-rules document.
* @param context The xcap context for which to create the OMA pres-rules document.
* @retval The XML document to PUT to the XDMS. You MUST call @a TSK_SAFE_FREE2 to free the returned string.
*/
char* txc_oma_presrules_serialize(const txc_context_t *context)
{
	char *oma_presrules_str = 0, *oma_grantedcontacts = 0, *oma_blockedcontacts = 0, *doc_sel = 0;
	
	/* get document selector */
	if( !context || !(doc_sel = TXC_DOC_GET_SEL(context, ietf_resource_lists)) )
	{
		goto bail;
	}

	/* oma granted contacts */
	tsk_sprintf(0, &oma_grantedcontacts,
		"%s/~~/resource-lists/list%%5B@name=%%22%s%%22%%5D",
		doc_sel, TXC_RNAME_OMA_GRANTEDCONTACTS);

	/* oma blocked contacts */
	tsk_sprintf(0, &oma_blockedcontacts,
		"%s/~~/resource-lists/list%%5B@name=%%22%s%%22%%5D",
		doc_sel, TXC_RNAME_OMA_BLOCKEDCONTACTS);

	/* append all */
	tsk_sprintf(0, &oma_presrules_str, TXC_OMA_PRESRULES_TEMPLATE,
		context->xui, oma_grantedcontacts, oma_blockedcontacts);

bail:
	TSK_FREE(doc_sel);
	TSK_FREE(oma_grantedcontacts);
	TSK_FREE(oma_blockedcontacts);

	return oma_presrules_str;
}

#undef TXC_OMA_PRESRULES_TEMPLATE