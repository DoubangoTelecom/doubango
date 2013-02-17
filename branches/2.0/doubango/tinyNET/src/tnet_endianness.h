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

/**@file tnet_endianness.h
 * @brief Byte Ordering.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef TNET_ENDIANNESS_H
#define TNET_ENDIANNESS_H

#include "tinynet_config.h"

#include "tsk_common.h" /* tsk_bool_t */

TNET_BEGIN_DECLS

#if	defined(TINYNET_IMPORTS_IGNORE)
#undef TNET_INLINE
#define TNET_INLINE
#endif

TINYNET_API TNET_INLINE unsigned short tnet_htons(unsigned short x);
TINYNET_API TNET_INLINE unsigned short tnet_htons_2(const void* px);
TINYNET_API TNET_INLINE unsigned long tnet_htonl(unsigned long x);
TINYNET_API TNET_INLINE unsigned long tnet_htonl_2(const void* px);
TINYNET_API TNET_INLINE tsk_bool_t tnet_is_BE();

#define tnet_ntohs(x) tnet_htons(x)
#define tnet_ntohs_2(px) tnet_htons_2(px)
#define tnet_ntohl(x) tnet_htonl(x)
#define tnet_ntohl_2(px) tnet_htonl_2(px)

TNET_END_DECLS

#endif /*TNET_ENDIANNESS_H*/

