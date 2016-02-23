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

/**@file tnet_endianness.c
 * @brief Byte Ordering.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

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
    if(tnet_is_BE()) {
        return x;
    }
    else {
        return ((((uint16_t)(x) & 0xff00) >> 8)		|
                (((uint16_t)(x) & 0x00ff) << 8));
    }
}

/* Memory alignment hack */
unsigned short tnet_htons_2(const void* px)
{
    unsigned short y = TSK_TO_UINT16((const uint8_t*)px);
    return tnet_htons(y);
}

/** Converts a 32-bit value from host to TCP/IP network byte order (big-endian).
* @param x The 32-bit (in host byte order) value to convert.
* @retval @a x in TCP/IP network byte order.
*/
unsigned long tnet_htonl(unsigned long x)
{
    if(tnet_is_BE()) {
        return x;
    }
    else {
        return ((((uint32_t)(x) & 0xff000000) >> 24)	| \
                (((uint32_t)(x) & 0x00ff0000) >> 8)		| \
                (((uint32_t)(x) & 0x0000ff00) << 8)		| \
                (((uint32_t)(x) & 0x000000ff) << 24));
    }
}

/* Memory alignment hack */
unsigned long tnet_htonl_2(const void* px)
{
    unsigned long y = TSK_TO_UINT32((const uint8_t*)px);
    return tnet_htonl(y);
}

/** Indicates whether we are on a Big Endian host or not.<br>
* <b>IMPORTANT</b>: Before calling this function, you should initialize the network stack by using
* @ref tnet_startup().
* @retval @a tsk_true if the program is runnin on a Big Endian host and @a tsk_false otherwise.
*/
tsk_bool_t tnet_is_BE()
{
    /* If LITTLE_ENDIAN or BIG_ENDIAN macros have been defined in config.h ==> use them
    * otherwise ==> dyn retrieve the endianness
    */
#if LITTLE_ENDIAN
    return tsk_false;
#elif BIG_ENDIAN
    return tsk_true;
#else
    return tnet_isBigEndian;
#endif
}
