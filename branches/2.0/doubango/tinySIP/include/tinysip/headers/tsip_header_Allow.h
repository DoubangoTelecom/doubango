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

/**@file tsip_header_Allow.h
 * @brief SIP header 'Allow'.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef _TSIP_HEADER_ALLOW_H_
#define _TSIP_HEADER_ALLOW_H_

#include "tinysip_config.h"
#include "tinysip/headers/tsip_header.h"

TSIP_BEGIN_DECLS

#define TSIP_HEADER_ALLOW_DEFAULT	"ACK, BYE, CANCEL, INVITE, MESSAGE, NOTIFY, OPTIONS, PRACK, REFER, UPDATE"
#define TSIP_HEADER_STR				"Allow:" TSIP_HEADER_ALLOW_DEFAULT "\r\n"

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief	SIP header 'Allow'.
///
/// @par ABNF
/// 	
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsip_header_Allow_s
{	
	TSIP_DECLARE_HEADER;

	tsk_strings_L_t *methods;
}
tsip_header_Allow_t;

TINYSIP_API tsip_header_Allow_t* tsip_header_Allow_create();

TINYSIP_API tsip_header_Allow_t *tsip_header_Allow_parse(const char *data, tsk_size_t size);
TINYSIP_API tsk_bool_t tsip_header_Allow_allows(const tsip_header_Allow_t* self, const char* method);

TINYSIP_GEXTERN const tsk_object_def_t *tsip_header_Allow_def_t;

TSIP_END_DECLS

#endif /* _TSIP_HEADER_ALLOW_H_ */

