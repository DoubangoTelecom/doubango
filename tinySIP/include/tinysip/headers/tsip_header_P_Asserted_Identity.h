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

/**@file tsip_header_P_Asserted_Identity.h
 * @brief SIP header 'P-Asserted-Identity' as per RFC 3325.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef _TSIP_HEADER_P_ASSERTED_IDENTITY_H_
#define _TSIP_HEADER_P_ASSERTED_IDENTITY_H_

#include "tinysip_config.h"
#include "tinysip/headers/tsip_header.h"

#include "tinysip/tsip_uri.h"

TSIP_BEGIN_DECLS


////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief	SIP header 'P-Asserted-Identity' as per RFC 3325.
///
/// @par ABNF: P-Asserted-Identity = PAssertedID
/// PAssertedID	=  	"P-Asserted-Identity" HCOLON PAssertedID-value *(COMMA PAssertedID-value)
/// PAssertedID-value	= 	name-addr / addr-spec
///
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsip_header_P_Asserted_Identity_s {
    TSIP_DECLARE_HEADER;

    char *display_name;
    tsip_uri_t *uri;
}
tsip_header_P_Asserted_Identity_t;

typedef tsk_list_t tsip_header_P_Asserted_Identities_L_t;

TINYSIP_API tsip_header_P_Asserted_Identity_t* tsip_header_P_Asserted_Identity_create();

TINYSIP_API tsip_header_P_Asserted_Identities_L_t *tsip_header_P_Asserted_Identity_parse(const char *data, tsk_size_t size);

TINYSIP_GEXTERN const tsk_object_def_t *tsip_header_P_Asserted_Identity_def_t;

TSIP_END_DECLS

#endif /* _TSIP_HEADER_P_ASSERTED_IDENTITY_H_ */

