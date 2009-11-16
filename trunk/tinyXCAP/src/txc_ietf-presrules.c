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

/**@file txc_ietf-presrules.c
 * @brief <a href="http://tools.ietf.org/html/rfc5025">RFC 5025 - IETF Presence Authorization Rules</a>
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */

#include "txc_ietf-pres-rules.h"
#include "txc_document.h"
#include "txc_oma.h"

/**@defgroup txc_ietf_pres_rules_group IETF Presence Authorization Rules
*/

/**@page txc_ietf_pres_rules_page IETF Presence Authorization Rules Tutorial (pres-rules)
* @par Application Unique ID (AUID)
* - '<span style="text-decoration:underline;">pres-rules</span>' as per rfc 5025 subclause 9.1
* @par Default Document Namespace
* - '<span style="text-decoration:underline;">urn:ietf:params:xml:ns:pres-rules</span>' as per rfc 5025 subclause 9.3
* @par MIME Type
* - '<span style="text-decoration:underline;">application/auth-policy+xml</span>' as per rfc 5025 subclause 9.4
* @par Default document name
* - '<span style="text-decoration:underline;">index</span>' as per rfc 5025 subclause 9.7
*/

/* 1= owner uri ; 2=oma_grantedcontacts uri; 3=oma_blockedcontacts*/
#define TXC_IETF_PRESRULES_TEMPLATE "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" \
									"<cr:ruleset xmlns:cr=\"urn:ietf:params:xml:ns:common-policy\" xmlns=\"urn:ietf:params:xml:ns:pres-rules\">" \
									/* wp_prs_allow_own */ \
									 "<cr:rule id=\"wp_prs_allow_own\">" \
										"<cr:conditions>" \
										  "<cr:identity>" \
											"<cr:one id=\"%s\" />" \
										  "</cr:identity>" \
										"</cr:conditions>" \
										"<cr:actions>" \
										  "<sub-handling>allow</sub-handling>" \
										"</cr:actions>" \
										"<cr:transformations>" \
										  "<provide-services>" \
											"<all-services />" \
										  "</provide-services>" \
										  "<provide-persons>" \
											"<all-persons />" \
										  "</provide-persons>" \
										  "<provide-all-attributes />" \
										  "<provide-devices>" \
											"<all-devices />" \
										  "</provide-devices>" \
										"</cr:transformations>" \
									  "</cr:rule>" \
									  /* rcs_allow_services_anonymous */ \
									  "<cr:rule id=\"rcs_allow_services_anonymous\">" \
										"<cr:conditions>" \
										  "<anonymous-request xmlns=\"urn:oma:xml:xdm:common-policy\" />" \
										"</cr:conditions>" \
										"<cr:actions>" \
										  "<sub-handling>allow</sub-handling>" \
										"</cr:actions>" \
										"<cr:transformations>" \
										  "<provide-services>" \
											"<all-services />" \
										  "</provide-services>" \
										  "<provide-persons>" \
											"<all-persons />" \
										  "</provide-persons>" \
										  "<provide-all-attributes />" \
										  "<provide-devices>" \
											"<all-devices />" \
										  "</provide-devices>" \
										"</cr:transformations>" \
									  "</cr:rule>" \
									  /* wp_prs_unlisted */ \
									  "<cr:rule id=\"wp_prs_unlisted\">" \
										"<cr:conditions>" \
										  "<other-identity xmlns=\"urn:oma:xml:xdm:common-policy\" />" \
										"</cr:conditions>" \
										"<cr:actions>" \
										  "<sub-handling>confirm</sub-handling>" \
										"</cr:actions>" \
									  "</cr:rule>" \
									  /* wp_prs_grantedcontacts */ \
									  "<cr:rule id=\"wp_prs_grantedcontacts\">" \
										"<cr:conditions>" \
										  "<external-list xmlns=\"urn:oma:xml:xdm:common-policy\">" \
											"<entry anc=\"%s\" />" \
										  "</external-list>" \
										"</cr:conditions>" \
										"<cr:actions>" \
										  "<sub-handling>allow</sub-handling>" \
										"</cr:actions>" \
										"<cr:transformations>" \
										  "<provide-services>" \
											"<all-services />" \
										  "</provide-services>" \
										  "<provide-persons>" \
											"<all-persons />" \
										  "</provide-persons>" \
										  "<provide-all-attributes />" \
										  "<provide-devices>" \
											"<all-devices />" \
										  "</provide-devices>" \
										"</cr:transformations>" \
									  "</cr:rule>" \
									  /* wp_prs_blockedcontacts */ \
									  "<cr:rule id=\"wp_prs_blockedcontacts\">" \
										"<cr:conditions>" \
										  "<external-list xmlns=\"urn:oma:xml:xdm:common-policy\">" \
											"<entry anc=\"%s\" />" \
										  "</external-list>" \
										"</cr:conditions>" \
										"<cr:actions>" \
										  "<sub-handling>block</sub-handling>" \
										"</cr:actions>" \
									  "</cr:rule>" \
									"</cr:ruleset>" \

/**@ingroup txc_ietf_pres_rules_group
* Used to create an initial IETF pres-rules document.
* @param context The xcap context for which to create the IETF pres-rules document.
* @retval The XML document to PUT to the XDMS. You MUST call @a TSK_FREE to free the returned string.
*/
char* txc_ietf_presrules_serialize(const txc_context_t *context)
{
	char *ietf_presrules_str = 0, *oma_grantedcontacts = 0, *oma_blockedcontacts = 0, *doc_sel = 0;
	
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
	tsk_sprintf(0, &ietf_presrules_str, TXC_IETF_PRESRULES_TEMPLATE,
		context->xui, oma_grantedcontacts, oma_blockedcontacts);
	
bail:
	TSK_FREE(doc_sel);
	TSK_FREE(oma_grantedcontacts);
	TSK_FREE(oma_blockedcontacts);

	return ietf_presrules_str;
}

#undef TXC_IETF_PRESRULES_TEMPLATE