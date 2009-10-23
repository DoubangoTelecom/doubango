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

#ifndef _TEST_GSMA_RCS_H_
#define _TEST_GSMA_RCS_H_

#include "xdm_api.h" 

/* test gsma rcs */
void test_gsma_rcs()
{
	xdm_context_t* context = 0;
	xdm_list_t *rlist2 = 0, *services;
	char *rlist2_str = 0, *rls_str = 0, *oma_presrules_str = 0;

	/* create our xdm context */
	XDM_CONTEXT_CREATE(context);

	context->user_agent = xdm_strdup("XDM-client/OMA1.1");
	context->xdm_root = xdm_strdup("http://xcap.example.com/services");
	context->password = xdm_strdup("mysecret");
	context->xui = xdm_strdup("sip:doubango@example.com");

	/* get rcs lists */
	rlist2 = xdm_gsmarcs_create_rlist2(context);
	rlist2_str = xdm_rlist_rlist2_serialize(rlist2);
	printf("rcs resource-lists: %s\n", rlist2_str);
	XDM_SAFE_FREE(rlist2_str);
	XDM_LIST_SAFE_FREE(rlist2);

	/* get rls services */
	services = xdm_gsmarcs_create_rls(context);
	rls_str = xdm_rls_rls_serialize(services);
	printf("rcs rls-services: %s\n", rls_str);
	XDM_SAFE_FREE(rls_str);
	XDM_LIST_SAFE_FREE(services);

	/* get oma pres-rules */
	oma_presrules_str = xdm_gsmarcs_create_oma_presrules(context);
	printf("rcs oma pres-rules: %s\n", oma_presrules_str);
	XDM_SAFE_FREE(oma_presrules_str);

	/* free context */
	XDM_CONTEXT_SAFE_FREE(context);
}


#endif /* _TEST_GSMA_RCS_H_ */