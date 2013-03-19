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

/**@file tcomp_udvm.nack.c
 * @brief  SigComp UDVM machine (byte copying).
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *

 */
#include "tcomp_udvm.h"

#include "tsk_debug.h"


#define TCOMP_UDVM_MEMORY_REGISTERS_PTR TCOMP_UDVM_GET_BUFFER_AT(UDVM_REGISTERS_START)


/**RFC3320-Setction_8.4.  Byte copying
From UDVM to UDVM
*/
tsk_bool_t tcomp_udvm_bytecopy_self(tcomp_udvm_t *udvm, uint32_t *destination, uint32_t source, uint32_t tsk_size_tocopy)
{
	uint32_t byte_copy_left, byte_copy_right;
	uint8_t* destination_ptr;

	//if (*destination == TCOMP_UDVM_GET_SIZE() || source == TCOMP_UDVM_GET_SIZE())
	if (*destination >= TCOMP_UDVM_GET_SIZE() || source >= TCOMP_UDVM_GET_SIZE())
	{
		/* SEGFAULT */
		tcomp_udvm_createNackInfo2(udvm, NACK_SEGFAULT);
		return tsk_false;
	}

	/*
	* The string of bytes is copied in ascending order of memory address,
	* respecting the bounds set by byte_copy_left and byte_copy_right.
	*/	
	byte_copy_left = TCOMP_UDVM_GET_2BYTES_VAL(TCOMP_UDVM_HEADER_BYTE_COPY_LEFT_INDEX);
	byte_copy_right = TCOMP_UDVM_GET_2BYTES_VAL(TCOMP_UDVM_HEADER_BYTE_COPY_RIGHT_INDEX);
	
	// string of bytes is copied one byte at a time
	while((tsk_size_tocopy--))
	{
		if(!(destination_ptr = TCOMP_UDVM_GET_BUFFER_AT((*destination)++))){
			tcomp_udvm_createNackInfo2(udvm, NACK_SEGFAULT);
			return tsk_false;
		}
		*destination_ptr = *TCOMP_UDVM_GET_BUFFER_AT(source++);
		*destination = (*destination == byte_copy_right)? byte_copy_left : *destination;
		source = (source == byte_copy_right)? byte_copy_left : source;
	}

	return tsk_true;
}

/**RFC3320-Setction_8.4.  Byte copying
From EXTERNAL to UDVM
*/
tsk_bool_t tcomp_udvm_bytecopy_to(tcomp_udvm_t *udvm, uint32_t destination, const uint8_t* source, uint32_t tsk_size_tocopy)
{
	uint32_t byte_copy_left, byte_copy_right;
	uint8_t* destination_ptr;

	if(destination == TCOMP_UDVM_GET_SIZE())
	{
		/* SEGFAULT */
		tcomp_udvm_createNackInfo2(udvm, NACK_SEGFAULT);
		return tsk_false;
	}

	/*
	* The string of bytes is copied in ascending order of memory address,
	* respecting the bounds set by byte_copy_left and byte_copy_right.
	*/
	byte_copy_left = TCOMP_UDVM_GET_2BYTES_VAL(TCOMP_UDVM_HEADER_BYTE_COPY_LEFT_INDEX);
	byte_copy_right = TCOMP_UDVM_GET_2BYTES_VAL(TCOMP_UDVM_HEADER_BYTE_COPY_RIGHT_INDEX);

	// string of bytes is copied one byte at a time
	while((tsk_size_tocopy--))
	{
		if(!(destination_ptr = TCOMP_UDVM_GET_BUFFER_AT(destination++))){
			tcomp_udvm_createNackInfo2(udvm, NACK_SEGFAULT);
			return tsk_false;
		}
		*destination_ptr = *(source++);
		destination = (destination == byte_copy_right)? byte_copy_left : destination;
	}

	return tsk_true;
}

/**RFC3320-Setction_8.4.  Byte copying
From UDVM to EXTERNAL
*/
tsk_bool_t tcomp_udvm_bytecopy_from(tcomp_udvm_t *udvm, uint8_t* destination, uint32_t source, uint32_t tsk_size_tocopy)
{
	uint32_t byte_copy_left, byte_copy_right;
	uint8_t* source_ptr;

	if(source >= TCOMP_UDVM_GET_SIZE()){
		TSK_DEBUG_ERROR("SEGFAULT");
		tcomp_udvm_createNackInfo2(udvm, NACK_SEGFAULT);
		return tsk_false;
	}

	/*
	* The string of bytes is copied in ascending order of memory address,
	* respecting the bounds set by byte_copy_left and byte_copy_right.
	*/
	byte_copy_left = TCOMP_UDVM_GET_2BYTES_VAL(TCOMP_UDVM_HEADER_BYTE_COPY_LEFT_INDEX);
	byte_copy_right = TCOMP_UDVM_GET_2BYTES_VAL(TCOMP_UDVM_HEADER_BYTE_COPY_RIGHT_INDEX);


	// string of bytes is copied one byte at a time
	while((tsk_size_tocopy--)){
		if(!(source_ptr = TCOMP_UDVM_GET_BUFFER_AT(source++))){
			tcomp_udvm_createNackInfo2(udvm, NACK_SEGFAULT);
			return tsk_false;
		}
		*(destination++) = *source_ptr;
		source = (source == byte_copy_right)? byte_copy_left : source;
	}

	return tsk_true;
}
