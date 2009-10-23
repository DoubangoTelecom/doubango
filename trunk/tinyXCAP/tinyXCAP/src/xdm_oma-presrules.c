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

/* as per RCS1: http://www.gsmworld.com/documents/rcs/rcs1_updates/R1_090831_RCS_Release_1_Technical_Realisation_v1_1.pdf subclause 4.4*/
#include "xdm_oma-presrules.h"
#include "xdm_document.h"
#include "xdm_oma.h"
#include "xdm_utils.h"

/* 1= owner uri ; 2=oma_grantedcontacts uri; 3=oma_blockedcontacts*/
#define XDM_OMA_PRESRULES_TEMPLATE "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" \
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


/* will serialize a complete pres-rules whith xml header*/
/* ATTENTION: use 'XDM_SAFE_FREE' macro to free the returned string */
char* xdm_oma_presrules_serialize(const xdm_context_t *context)
{
	char *oma_presrules_str = 0, *oma_grantedcontacts = 0, *oma_blockedcontacts = 0, *doc_sel = 0;
	
	/* get document selector */
	if( !context || !(doc_sel = xdm_doc_get_sel2(context, ietf_resource_lists)) )
	{
		goto bail;
	}

	/* oma granted contacts */
	xdm_sprintf(&oma_grantedcontacts,
		"%s/~~/resource-lists/list%%5B@name=%%22%s%%22%%5D",
		doc_sel, XDM_RNAME_OMA_GRANTEDCONTACTS);

	/* oma blocked contacts */
	xdm_sprintf(&oma_blockedcontacts,
		"%s/~~/resource-lists/list%%5B@name=%%22%s%%22%%5D",
		doc_sel, XDM_RNAME_OMA_BLOCKEDCONTACTS);

	/* append all */
	xdm_sprintf(&oma_presrules_str, XDM_OMA_PRESRULES_TEMPLATE,
		context->xui, oma_grantedcontacts, oma_blockedcontacts);

bail:
	XDM_FREE(doc_sel);
	XDM_FREE(oma_grantedcontacts);
	XDM_FREE(oma_blockedcontacts);

	return oma_presrules_str;
}

#undef XDM_OMA_PRESRULES_TEMPLATE