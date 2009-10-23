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
#ifndef _TINYXCAP_XDM_RLS_H_
#define _TINYXCAP_XDM_RLS_H_

#include "tinyxcap_config.h"
#include "xdm_xml.h"

/* urn:ietf:params:xml:ns:rls-services */

#define XDM_RLS_SERVICE_CREATE(service)		XDM_XXX_CREATE(service, rls_service)
#define XDM_RLS_SERVICE_SAFE_FREE(service)	XDM_XXX_SAFE_FREE(service, rls_service)

/* service */
typedef struct xdm_rls_service_s
{
	char* uri;
	char* resource_list;
	xdm_list_t* packages;
}
xdm_rls_service_t;
typedef xdm_list_t xdm_rls_service_L_t; /* contains 'xdm_rls_service_t' elements */

/* rls-service */
typedef struct xdm_rls_s
{
	xmlDocPtr docPtr;
}
xdm_rls_t;

void xdm_rls_service_init(xdm_rls_service_t *service);
void xdm_rls_service_set(xdm_rls_service_t *service, const char* uri, const char* resource_list);
void xdm_rls_service_add_package(xdm_rls_service_t *service, const char* package);
void xdm_rls_service_free(void **service);

xdm_rls_service_t* xdm_rls_service_from_xml(const xmlNodePtr node);

TINYXCAP_API xdm_rls_t* xdm_rls_create(const char* buffer, size_t size);
TINYXCAP_API xdm_rls_service_L_t* xdm_rls_get_all_services(const xdm_rls_t* rls);
TINYXCAP_API char* xdm_rls_service_serialize(const xdm_rls_service_t *service);
TINYXCAP_API char* xdm_rls_rls_serialize(const xdm_list_t *services);
TINYXCAP_API void xdm_rls_free(xdm_rls_t **rls);

#endif /* _TINYXCAP_XDM_RLS_H_ */