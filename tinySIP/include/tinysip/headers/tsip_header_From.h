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

/**@file tsip_header_From.h
 * @brief SIP header 'From'.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef _TSIP_HEADER_FROM_H_
#define _TSIP_HEADER_FROM_H_

#include "tinysip_config.h"
#include "tinysip/tsip_uri.h"
#include "tinysip/headers/tsip_header.h"

TSIP_BEGIN_DECLS

#define TSIP_HEADER_FROM_VA_ARGS(display_name, uri, tag)	tsip_header_From_def_t, (const char*)display_name, (const tsip_uri_t*)uri, (const char*)tag

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief	SIP header 'From' .
///
/// @par ABNF: From	= 	( "From" / "f" ) HCOLON from-spec
/// from-spec	= 	( name-addr / addr-spec ) *( SEMI from-param )
/// from-param	= 	tag-param / generic-param
/// tag-param	= 	"tag" EQUAL token
///
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsip_header_From_s {
    TSIP_DECLARE_HEADER;

    char *display_name;
    tsip_uri_t *uri;
    char *tag;
}
tsip_header_From_t;

TINYSIP_API tsip_header_From_t* tsip_header_From_create(const char* display_name, const tsip_uri_t* uri, const char* tag);

TINYSIP_API tsip_header_From_t *tsip_header_From_parse(const char *data, tsk_size_t size);

TINYSIP_GEXTERN const tsk_object_def_t *tsip_header_From_def_t;

TSIP_END_DECLS

#endif /* _TSIP_HEADER_FROM_H_ */
