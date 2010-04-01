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

/**@file tnet_endianness.c
 * @brief Byte Ordering.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tnet_endianness.h"

#include "tnet.h"

extern tsk_bool_t tnet_isBigEndian;

/** Converts a 16-bit value from host to TCP/IP network byte order (big-endian).
* @param x The 16-bit (in host byte order) value to convert.
* @retval @a x in TCP/IP network byte order.
*/
unsigned short tnet_htons(unsigned short x)
{
	if(tnet_isBigEndian){
		return x;
	}
	else{
		return ((((uint16_t)(x) & 0xff00) >> 8)		|
						(((uint16_t)(x) & 0x00ff) << 8));
	}
}

/** Converts a 32-bit value from host to TCP/IP network byte order (big-endian).
* @param x The 32-bit (in host byte order) value to convert.
* @retval @a x in TCP/IP network byte order.
*/
unsigned long tnet_htonl(unsigned long x)
{
	if(tnet_isBigEndian){
		return x;
	}
	else{
		return ((((uint32_t)(x) & 0xff000000) >> 24)	| \
						(((uint32_t)(x) & 0x00ff0000) >> 8)		| \
						(((uint32_t)(x) & 0x0000ff00) << 8)		| \
						(((uint32_t)(x) & 0x000000ff) << 24));
	}
}
