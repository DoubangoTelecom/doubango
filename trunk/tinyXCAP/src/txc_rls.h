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


/**@file txc_rls.h
 * @brief RFC 4826 subclause 4: <a href="http://tools.ietf.org/html/rfc4826#section-4">RLS Services Documents</a>
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */

#ifndef _TINYXCAP_TXC_RLS_H_
#define _TINYXCAP_TXC_RLS_H_

#include "tinyxcap_config.h"
#include "tsk_xml.h"

/**@def TXC_RLS_SERVICE_CREATE
* Create an initialize an rls service. You MUST use @ref TXC_RLS_SERVICE_SAFE_FREE to free the object.
* @sa @ref TXC_RLS_SERVICE_SAFE_FREE
*/
/**@def TXC_RLS_SERVICE_SAFE_FREE
* Safely free an rls service previously created using @ref TXC_RLS_SERVICE_CREATE.
* @sa @ref TXC_RLS_SERVICE_CREATE
*/
#define TXC_RLS_SERVICE_CREATE(service)		TXC_XXX_CREATE2(service, rls_service)
#define TXC_RLS_SERVICE_SAFE_FREE(service)	TXC_XXX_SAFE_FREE2(service, rls_service)

/**
* RLS-SERVICE
*/
typedef struct txc_rls_service_s
{
	char* uri; /**< The uri of the service*/
	char* resource_list; /**< The resource-lists associated to this rls-service */
	tsk_list_t* packages; /**< The list of available packages */
}
txc_rls_service_t;
typedef tsk_list_t txc_rls_service_L_t; /**< List of @ref txc_rls_service_t elements */

/**
* RLS-SERVICE context
*/
typedef struct txc_rls_s
{
	xmlDocPtr docPtr; /**< Pointer to the XML document */
}
txc_rls_t;

void txc_rls_service_init(txc_rls_service_t *service);
void txc_rls_service_set(txc_rls_service_t *service, const char* uri, const char* resource_list);
void txc_rls_service_add_package(txc_rls_service_t *service, const char* package);
void txc_rls_service_free(void **service);

txc_rls_service_t* txc_rls_service_from_xml(const xmlNodePtr node);

TINYXCAP_API txc_rls_t* txc_rls_create(const char* buffer, size_t size);
TINYXCAP_API txc_rls_service_L_t* txc_rls_get_all_services(const txc_rls_t* rls);
TINYXCAP_API char* txc_rls_service_serialize(const txc_rls_service_t *service);
TINYXCAP_API char* txc_rls_services_serialize(const tsk_list_t *services);
TINYXCAP_API void txc_rls_free(txc_rls_t **rls);

#endif /* _TINYXCAP_TXC_RLS_H_ */