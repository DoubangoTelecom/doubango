/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)yahoo.fr>
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

/**@file tmsrp_header_Content_Type.h
 * @brief MSRP header 'Content-Type'.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TMSRP_HEADER_CONTENT_TYPE_H_
#define _TMSRP_HEADER_CONTENT_TYPE_H_

#include "tinymsrp_config.h"
#include "tinymsrp/headers/tmsrp_header.h"

TMSRP_BEGIN_DECLS

/**@def TMSRP_HEADER_CONTENT_TYPE_CREATE
* Creates new msrp 'Content-Type' header.  You must call @ref TSK_OBJECT_SAFE_FREE to free the header.
* @sa TSK_OBJECT_SAFE_FREE.
*/
#define TMSRP_HEADER_CONTENT_TYPE_VA_ARGS(type)			tmsrp_header_Content_Type_def_t, (const char*)type
#define TMSRP_HEADER_CONTENT_TYPE_CREATE(type)			tsk_object_new(TMSRP_HEADER_CONTENT_TYPE_VA_ARGS(type))
#define TMSRP_HEADER_CONTENT_TYPE_CREATE_NULL()			TMSRP_HEADER_CONTENT_TYPE_CREATE(TMSRP_NULL)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @struct	
///
/// @brief	MSRP header 'Content-Type'.
/// @author	Mamadou
/// @date	12/3/2009
///
/// @par ABNF= Content-Type	=  	 "Content-Type:" SP media-type
/// media-type	= 	type "/" subtype *( ";" gen-param )
/// type	= 	token
/// subtype	= 	token
/// gen-param	= 	pname [ "=" pval ]
/// pname	= 	token
/// pval	= 	token / quoted-string 
/// 	
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tmsrp_header_Content_Type_s
{	
	TMSRP_DECLARE_HEADER;

	char* value;
	tsk_params_L_t *params;
}
tmsrp_header_Content_Type_t;


tmsrp_header_Content_Type_t *tmsrp_header_Content_Type_parse(const char *data, size_t size);

TINYMSRP_GEXTERN const void *tmsrp_header_Content_Type_def_t;


TMSRP_END_DECLS

#endif /* _TMSRP_HEADER_CONTENT_TYPE_H_ */

