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

/**@file tsip_header_P_Charging_Function_Addresses.h
 * @brief SIP header 'P-Charging-Function-Addresses'.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TSIP_HEADER_P_CHARGING_FUNCTION_ADDRESSES_H_
#define _TSIP_HEADER_P_CHARGING_FUNCTION_ADDRESSES_H_

#include "tinysip_config.h"
#include "tinysip/headers/tsip_header.h"

TSIP_BEGIN_DECLS

/**@def TSIP_HEADER_P_ASSOCIATED_URI_CREATE
* Creates new sip 'P-Associated-URI' header.  You must call @ref TSK_OBJECT_SAFE_FREE to free the header.
* @sa TSK_OBJECT_SAFE_FREE.
*/
#define TSIP_HEADER_P_CHARGING_FUNCTION_ADDRESSES_CREATE()	tsk_object_new(tsip_header_P_Charging_Function_Addresses_def_t)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @struct	
///
/// @brief	SIP header 'P-Charging-Function-Addresses' as per RFC 3455.
/// @author	Mamadou
/// @date	12/3/2009
///
/// @par ABNF: P-Charging-Function-Addresses = P-Charging-Addr
/// P-Charging-Addr	= 	"P-Charging-Function-Addresses" HCOLON charge-addr-params *( SEMI charge-addr-params )
/// charge-addr-params	= 	ccf / ecf / generic-param
/// ccf	= 	"ccf" EQUAL gen-value
/// ecf	= 	"ecf" EQUAL gen-value
/// 	
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsip_header_P_Charging_Function_Addresses_s
{	
	TSIP_DECLARE_HEADER;
	char* ccf;
	char* ecf;
}
tsip_header_P_Charging_Function_Addresses_t;

typedef tsk_list_t tsip_header_P_Charging_Function_Addressess_L_t;

tsip_header_P_Charging_Function_Addressess_L_t *tsip_header_P_Charging_Function_Addresses_parse(const char *data, size_t size);

TINYSIP_GEXTERN const void *tsip_header_P_Charging_Function_Addresses_def_t;

TSIP_END_DECLS

#endif /* _TSIP_HEADER_P_CHARGING_FUNCTION_ADDRESSES_H_ */

