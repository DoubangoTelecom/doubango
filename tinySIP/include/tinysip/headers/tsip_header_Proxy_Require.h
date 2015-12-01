/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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

/**@file tsip_header_Proxy_Require.h
 * @brief SIP header 'Proxy-Require'.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef _TSIP_HEADER_PROXY_REQUIRE_H_
#define _TSIP_HEADER_PROXY_REQUIRE_H_

#include "tinysip_config.h"
#include "tinysip/headers/tsip_header.h"

TSIP_BEGIN_DECLS


#define TSIP_HEADER_PROXY_REQUIRE_VA_ARGS(option)	tsip_header_Proxy_Require_def_t, (const char*)option

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief	SIP header 'Proxy-Proxy_Require'.
///
/// @par ABNF: Proxy_Require	= 	"Proxy_Require" HCOLON option-tag *(COMMA option-tag)
/// 	
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsip_header_Proxy_Require_s
{	
	TSIP_DECLARE_HEADER;

	tsk_strings_L_t *options;
}
tsip_header_Proxy_Require_t;

TINYSIP_API tsip_header_Proxy_Require_t* tsip_header_Proxy_Require_create(const char* option);
TINYSIP_API tsip_header_Proxy_Require_t* tsip_header_Proxy_Require_create_null();

TINYSIP_API tsip_header_Proxy_Require_t *tsip_header_Proxy_Require_parse(const char *data, tsk_size_t size);

TINYSIP_GEXTERN const tsk_object_def_t *tsip_header_Proxy_Require_def_t;

TSIP_END_DECLS

#endif /* _TSIP_HEADER_PROXY_REQUIRE_H_ */

