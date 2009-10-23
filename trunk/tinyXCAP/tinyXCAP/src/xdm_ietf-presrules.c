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
/*
	RFC 5025 - Presence Authorization Rules
*/

#include "xdm_ietf-pres-rules.h"
#include "xdm_document.h"
#include "xdm_oma.h"
#include "xdm_utils.h"

//static const char* xdm_ietf_pres_rules_ns = "urn:ietf:params:xml:ns:pres-rules";

/* 1= owner uri ; 2=oma_grantedcontacts uri; 3=oma_blockedcontacts*/
#define XDM_IETF_PRESRULES_TEMPLATE "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" \
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

/* will serialize a complete pres-rules whith xml header*/
/* ATTENTION: use 'XDM_SAFE_FREE' macro to free the returned string */
char* xdm_ietf_presrules_serialize(const xdm_context_t *context)
{
	char *ietf_presrules_str = 0, *oma_grantedcontacts = 0, *oma_blockedcontacts = 0, *doc_sel = 0;
	
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
	xdm_sprintf(&ietf_presrules_str, XDM_IETF_PRESRULES_TEMPLATE,
		context->xui, oma_grantedcontacts, oma_blockedcontacts);
	
bail:
	XDM_FREE(doc_sel);
	XDM_FREE(oma_grantedcontacts);
	XDM_FREE(oma_blockedcontacts);

	return ietf_presrules_str;
}

#undef XDM_IETF_PRESRULES_TEMPLATE