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

/**@file tnet_endianness.h
 * @brief Byte Ordering.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TNET_ENDIANNESS_H
#define TNET_ENDIANNESS_H

#include "tinyNET_config.h"

TNET_BEGIN_DECLS

unsigned short tnet_htons(unsigned short x);
unsigned long tnet_htonl(unsigned long x);

#define tnet_ntohs(x) tnet_htons(x)
#define tnet_ntohl(x) tnet_htonl(x)

TNET_BEGIN_DECLS

#endif /*TNET_ENDIANNESS_H*/

