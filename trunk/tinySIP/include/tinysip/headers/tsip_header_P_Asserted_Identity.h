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
* GNU General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

/**@file tsip_header_P_Asserted_Identity.h
 * @brief SIP header 'P-Asserted-Identity' as per RFC 3325.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TSIP_HEADER_P_ASSERTED_IDENTITY_H_
#define _TSIP_HEADER_P_ASSERTED_IDENTITY_H_

#include "tinySIP_config.h"
#include "tinySIP/headers/tsip_header.h"

#include "tinySIP/tsip_uri.h"

TSIP_BEGIN_DECLS

/**@def TSIP_HEADER_P_ASSERTED_IDENTITY_CREATE
* Creates new sip 'P_Asserted_Identity' header.  You must call @ref TSK_OBJECT_SAFE_FREE to free the header.
* @sa TSK_OBJECT_SAFE_FREE.
*/
#define TSIP_HEADER_P_ASSERTED_IDENTITY_CREATE()	tsk_object_new(tsip_header_P_Asserted_Identity_def_t)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @struct	
///
/// @brief	SIP header 'P-Asserted-Identity' as per RFC 3325.
/// @author	Mamadou
/// @date	12/3/2009
///
/// @par ABNF: P-Asserted-Identity = PAssertedID
/// PAssertedID	=  	"P-Asserted-Identity" HCOLON PAssertedID-value *(COMMA PAssertedID-value)
/// PAssertedID-value	= 	name-addr / addr-spec
/// 	
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsip_header_P_Asserted_Identity_s
{	
	TSIP_DECLARE_HEADER;

	char *display_name;
	tsip_uri_t *uri;
}
tsip_header_P_Asserted_Identity_t;

typedef tsk_list_t tsip_header_P_Asserted_Identities_L_t;

tsip_header_P_Asserted_Identities_L_t *tsip_header_P_Asserted_Identity_parse(const char *data, size_t size);

TINYSIP_GEXTERN const void *tsip_header_P_Asserted_Identity_def_t;

TSIP_END_DECLS

#endif /* _TSIP_HEADER_P_ASSERTED_IDENTITY_H_ */

