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

/**@file tsk_binaryutils.h
 * @brief Binary utils.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef _TINYSAK_BINARYUTILS_H_
#define _TINYSAK_BINARYUTILS_H_

#include "tinysak_config.h"

#include <stdio.h>

TSK_BEGIN_DECLS

/**@ingroup tsk_binaryutils_group
* Reverse (bit per bit) a 2-byte value.
* @param value The 2-byte value to reverse.
*/
#define TSK_BINARY_REVERSE_2BYTE(value) ((Tsk_BitReverseTable256[value & 0xff] << 8) | (Tsk_BitReverseTable256[(value >> 8)]))

/**@ingroup tsk_binaryutils_group
* Converts to uint8_t pointer.
*/
#define TSK_TO_U8(buffer) ((uint8_t*)buffer)
/**@ingroup tsk_binaryutils_group
* Gets the value of the fist byte.
*/
#define TSK_BINARY_GET_1BYTE(buffer) *TSK_TO_U8(buffer)// 1-byte

TSK_GCC_DISABLE_WARNINGS_BEGIN("-Wunused-function")
/**@ingroup tsk_binaryutils_group
* Converts from Little to Big endian.
*/
static uint16_t TSK_LSB_2_MSB( void const * buffer )
{
    const uint8_t* dummy = (const uint8_t*)buffer;
    return ( ((uint16_t)dummy[0] << 8) | dummy[1] );
}
TSK_GCC_DISABLE_WARNINGS_END()

/**@ingroup tsk_binaryutils_group
* @def TSK_BINARY_GET_2BYTES
* Gets the first 2-bytes value.
*/
/**@ingroup tsk_binaryutils_group
* @def TSK_BINARY_SET_2BYTES
* Sets the first 2-bytes value.
*/

#if 0 /*BIG_ENDIAN*/// Do not change this (it's for my own tests)
#	define	TSK_BINARY_GET_2BYTES(buffer) *((uint16_t*)buffer)
#	define  TSK_BINARY_SET_2BYTES(buffer, value) *((uint16_t*)buffer)=value
#else
#	define	TSK_BINARY_GET_2BYTES(buffer) TSK_LSB_2_MSB(buffer)
#	define  TSK_BINARY_SET_2BYTES(buffer, value)							\
				{uint16_t __value__ = value;							\
				uint8_t*__buffer__ = buffer;							\
				(__buffer__[0]) = (__value__ >> 8);					\
				(__buffer__[1]) = (__value__ & 0xff);}
#endif

/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////

//	RFC 3320 - 7.3.  Uploading UDVM bytecode
static const uint16_t sigcomp_encoding_destination[]
    = { 0, 128, 192, 256, 320, 384, 448, 512, 576, 640, 704, 768, 832, 896, 960, 1024 };

//	RFC 3320 - 7.2.  Accessing Stored State
static const uint8_t sigcomp_encoding_partial_id_length[]
    = { 0, 6, 9, 12 };

//	RFC 3320 - 3.3.1.  Memory Size and UDVM Cycles
static const uint8_t sigcomp_encoding_cycles_per_bit[]
    = { 16, 32, 64, 128 };
static const uint32_t sigcomp_encoding_state_memory_size[]
    = { 0, 2048, 4096, 8192, 16384, 32768, 65536, 131072 };
static const uint32_t sigcomp_encoding_decompression_memory_size[]
    = { 0, 2048, 4096, 8192, 16384, 32768, 65536, 131072 }; // the bit pattern 000 cannot be used.

//	3.3.1.  Memory Size and UDVM Cycles
//	3.3.  SigComp Parameters
static const uint8_t sigcomp_encoding_cpb[]
    = { 16, 32, 64, 128 };
static const uint32_t sigcomp_encoding_sms[]
    = { 0, 2048, 4096, 8192, 16384, 32768, 65536, 131072 };
static const uint32_t sigcomp_encoding_dms[]
    = { 0, 2048, 4096, 8192, 16384, 32768, 65536, 131072 }; // the bit pattern 000 cannot be used.


//	RFC 3320: Figure 10: Bytecode for a multitype (%) operand
static const int8_t operand_multitype_indexes [256] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    9,
    10,
    -1, -1, -1, -1,
    3, 3,
    4, 4, 4, 4, 4, 4, 4, 4,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5
};

//	Used LSB<->MSB bits reverse/swap
/**@ingroup tsk_binaryutils_group
* Lookup table for bit reversing.
*/
static const unsigned char Tsk_BitReverseTable256[] = {
    0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0,
    0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8,
    0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4,
    0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC,
    0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2,
    0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
    0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6,
    0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
    0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
    0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9,
    0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
    0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
    0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3,
    0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
    0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7,
    0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
};

TSK_END_DECLS

#endif /* _TINYSAK_BINARYUTILS_H_ */

