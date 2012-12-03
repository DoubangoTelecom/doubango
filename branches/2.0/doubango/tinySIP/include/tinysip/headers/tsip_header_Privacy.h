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

/**@file tsip_header_Privacy.h
 * @brief SIP header 'Privacy'.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef _TSIP_HEADER_PRIVACY_H_
#define _TSIP_HEADER_PRIVACY_H_

#include "tinysip_config.h"
#include "tinysip/headers/tsip_header.h"

TSIP_BEGIN_DECLS


////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief	SIP header 'Privacy' as per RFC 3323.
///
/// @par ABNF: Privacy = Privacy-hdr
/// Privacy-hdr	= 	"Privacy" HCOLON priv-value *(";" priv-value)
/// priv-value	= 	"header" / "session" / "user" / "none" / "critical" / "id" / "history" / token
/// 	
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsip_header_Privacy_s
{	
	TSIP_DECLARE_HEADER;

	tsk_strings_L_t *values;
}
tsip_header_Privacy_t;

TINYSIP_API tsip_header_Privacy_t* tsip_header_Privacy_create();

TINYSIP_API tsip_header_Privacy_t *tsip_header_Privacy_parse(const char *data, tsk_size_t size);

TINYSIP_GEXTERN const tsk_object_def_t *tsip_header_Privacy_def_t;

TSIP_END_DECLS

#endif /* _TSIP_HEADER_PRIVACY_H_ */

