/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango.org>
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
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TNET_ENDIANNESS_H
#define TNET_ENDIANNESS_H

#include "tinynet_config.h"

#include "tsk_common.h" /* tsk_bool_t */

TNET_BEGIN_DECLS

TINYNET_API __inline unsigned short tnet_htons(unsigned short x);
TINYNET_API __inline unsigned long tnet_htonl(unsigned long x);
TINYNET_API __inline tsk_bool_t tnet_is_BE();

#define tnet_ntohs(x) tnet_htons(x)
#define tnet_ntohl(x) tnet_htonl(x)

TNET_BEGIN_DECLS

#endif /*TNET_ENDIANNESS_H*/

