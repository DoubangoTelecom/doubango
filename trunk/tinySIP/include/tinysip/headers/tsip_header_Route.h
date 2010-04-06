/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango.org>
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
* GNU General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

/**@file tsip_header_Route.h
 * @brief SIP header 'Route'.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TSIP_HEADER_ROUTE_H_
#define _TSIP_HEADER_ROUTE_H_

#include "tinySIP_config.h"
#include "tinySIP/headers/tsip_header.h"

#include "tinySIP/tsip_uri.h"

TSIP_BEGIN_DECLS

/**@def TSIP_HEADER_ROUTE_CREATE
* Creates new sip 'Route' header.  You must call @ref TSK_OBJECT_SAFE_FREE to free the header.
* @sa TSK_OBJECT_SAFE_FREE.
*/
#define TSIP_HEADER_ROUTE_VA_ARGS(uri)		tsip_header_Route_def_t, (const tsip_uri_t*)uri
#define TSIP_HEADER_ROUTE_CREATE(uri)		tsk_object_new(TSIP_HEADER_ROUTE_VA_ARGS(uri))
#define TSIP_HEADER_ROUTE_CREATE_NULL()		TSIP_HEADER_ROUTE_CREATE(0)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @struct	
///
/// @brief	SIP header 'Route'.
/// @author	Mamadou
/// @date	12/3/2009
///
/// @par ABNF : Route	= 	"Route" HCOLON route-param *(COMMA route-param)
///							route-param	= 	name-addr *( SEMI rr-param )
/// 	
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsip_header_Route_s
{	
	TSIP_DECLARE_HEADER;

	char *display_name;
	tsip_uri_t *uri;
}
tsip_header_Route_t;

//#define TSIP_DECLARE_HEADER_ROUTE	tsip_header_Route_t header_Route

typedef tsk_list_t tsip_header_Routes_L_t;

tsip_header_Routes_L_t *tsip_header_Route_parse(const char *data, size_t size);

TINYSIP_GEXTERN const void *tsip_header_Route_def_t;

TSIP_END_DECLS

#endif /* _TSIP_HEADER_ROUTE_H_ */

