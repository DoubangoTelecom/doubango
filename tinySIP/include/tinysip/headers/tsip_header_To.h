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

/**@file tsip_header_To.h
 * @brief SIP header 'To'.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef _TSIP_HEADER_TO_H_
#define _TSIP_HEADER_TO_H_

#include "tinysip_config.h"
#include "tinysip/tsip_uri.h"
#include "tinysip/headers/tsip_header.h"

TSIP_BEGIN_DECLS


#define TSIP_HEADER_TO_VA_ARGS(display_name, uri, tag)			tsip_header_To_def_t, (const char*)display_name, (const tsip_uri_t*)uri, (const char*)tag

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief	SIP header 'To' .
///
/// @par ABNF: To	= 	To	= 	( "To" / "t" ) HCOLON ( name-addr / addr-spec ) *( SEMI to-param )
/// to-param	= 	tag-param / generic-param
///
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsip_header_To_s {
    TSIP_DECLARE_HEADER;

    char *display_name;
    tsip_uri_t *uri;
    char *tag;
}
tsip_header_To_t;

TINYSIP_API tsip_header_To_t* tsip_header_To_create(const char* display_name, const tsip_uri_t* uri, const char* tag);
TINYSIP_API tsip_header_To_t* tsip_header_To_create_null();

TINYSIP_API tsip_header_To_t *tsip_header_To_parse(const char *data, tsk_size_t size);

TINYSIP_GEXTERN const tsk_object_def_t *tsip_header_To_def_t;

TSIP_END_DECLS

#endif /* _TSIP_HEADER_TO_H_ */

