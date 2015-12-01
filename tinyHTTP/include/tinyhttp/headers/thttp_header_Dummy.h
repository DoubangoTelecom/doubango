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

/**@file thttp_header_Dummy.h
 * @brief HTTP dummy header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef _THTTP_HEADER_DUMMY_H_
#define _THTTP_HEADER_DUMMY_H_

#include "tinyhttp_config.h"
#include "tinyhttp/headers/thttp_header.h"

THTTP_BEGIN_DECLS

#define THTTP_HEADER_DUMMY_VA_ARGS(name, value)		thttp_header_Dummy_def_t, (const char*)name, (const char*)value

////////////////////////////////////////////////////////////////////////////////////////////////////
/// HTTP Dummy header.
///
/// @par ABNF : token SP* HCOLON SP*<: any*
/// 	
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct thttp_header_Dummy_s
{	
	THTTP_DECLARE_HEADER;

	char *name;
	char *value;
}
thttp_header_Dummy_t;

thttp_header_Dummy_t *thttp_header_Dummy_parse(const char *data, tsk_size_t size);

TINYHTTP_GEXTERN const tsk_object_def_t *thttp_header_Dummy_def_t;

THTTP_END_DECLS

#endif /* _THTTP_HEADER_DUMMY_H_ */

