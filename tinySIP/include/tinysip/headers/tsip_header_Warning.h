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

/**@file tsip_header_Warning.h
 * @brief SIP header 'Warning.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef _TSIP_HEADER_WARNING_H_
#define _TSIP_HEADER_WARNING_H_

#include "tinysip_config.h"
#include "tinysip/headers/tsip_header.h"

TSIP_BEGIN_DECLS


////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief	SIP header 'Warning'.
///
/// @par ABNF: Warning	= 	"Warning" HCOLON warning-value *(COMMA warning-value)
/// warning-value	= 	warn-code SP warn-agent SP warn-text
/// warn-code	= 	3DIGIT
/// warn-agent	= 	hostport / pseudonym
/// warn-text	= 	quoted-string
/// pseudonym	= 	token
///
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsip_header_Warning_s {
    TSIP_DECLARE_HEADER;

    int32_t code;
    char* agent;
    char* text;
}
tsip_header_Warning_t;

typedef tsk_list_t tsip_header_Warnings_L_t;

tsip_header_Warning_t* tsip_header_Warning_create();

tsip_header_Warnings_L_t *tsip_header_Warning_parse(const char *data, tsk_size_t size);

TINYSIP_GEXTERN const tsk_object_def_t *tsip_header_Warning_def_t;

TSIP_END_DECLS

#endif /* _TSIP_HEADER_WARNING_H_ */

