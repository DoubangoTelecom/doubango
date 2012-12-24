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

/**@file tcomp_compressor_dummy.h
 * @brief  SigComp Dummy compressor. Used if none match. See RFC 4896 subclause 11.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *

 */

#include "tcomp_compressor_dummy.h"
#include "tcomp_buffer.h"

#include <string.h>

#define UNCOMPRESSED_BYTECODE_LENGTH				13
#define UNCOMPRESSED_BYTECODE_DESTINATION_CODE		0x01 /* 128 */
#define DUMMYCOMPRESSOR_UNCOMPRESSED_BYTECODE \
	"\xf8\x00\xa1\x1c\x01\x86\x09\x22\x86\x01\x16\xf9\x23"



////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief	Dummy compressor as per RFC 4896 subclause 11. This function is used to create uncompressed sigcomp message.
/// Used if none match. 
///
/// @param [in,out]	lpCompartment	The compartment to use. 
/// @param [in,out]	input_ptr		The input buffer containing the message to compress. 
/// @param	input_size				The size of the input buffer. 
/// @param [in,out]	output_ptr		The output buffer where to copy the compressed message. 
/// @param [in,out]	output_size		The size of the output buffer.
/// @param	stream					Indicates whether it's a stream buffer or not. 
///
/// @return	1 if succedd and 0 otherwise. 
////////////////////////////////////////////////////////////////////////////////////////////////////
tsk_bool_t tcomp_compressor_dummy_compress(tcomp_compartment_t *lpCompartment, const void *input_ptr, tsk_size_t input_size, void *output_ptr, tsk_size_t *output_size, tsk_bool_t stream)
{
	tcomp_buffer_handle_t *output_buffer = tcomp_buffer_create_null();
	tsk_size_t pointer = 0;
	uint8_t *header;
	uint32_t codeLen;

	tcomp_buffer_referenceBuff(output_buffer, (uint8_t*)output_ptr, *output_size);
	header = tcomp_buffer_getBufferAtPos(output_buffer, pointer++);

	/* SigComp Header */
	if(lpCompartment->lpReqFeedback && tcomp_buffer_getSize(lpCompartment->lpReqFeedback)){
		/* Return the requested feedback */
		*header = 0xfc; /* T=1 */
		memcpy(tcomp_buffer_getBufferAtPos(output_buffer, pointer), tcomp_buffer_getBuffer(lpCompartment->lpReqFeedback), tcomp_buffer_getSize(lpCompartment->lpReqFeedback));
		pointer += tcomp_buffer_getSize(lpCompartment->lpReqFeedback);
	}
	else{
		*header = 0xf8;
	}

	codeLen = UNCOMPRESSED_BYTECODE_LENGTH;
	/* first byte for codelen */
	*tcomp_buffer_getBufferAtPos(output_buffer, pointer++) = ((codeLen>>4)& 0x00ff);

	/* last 4 bits for codelen */
	*tcomp_buffer_getBufferAtPos(output_buffer, pointer) = ((codeLen & 0x000f)<<4); 

	/* first and last 4 bits for destination */
	*tcomp_buffer_getBufferAtPos(output_buffer, pointer++) |= UNCOMPRESSED_BYTECODE_DESTINATION_CODE;

	/*
	*	Upload UDVM bytecode
	*/
	memcpy(tcomp_buffer_getBufferAtPos(output_buffer, pointer), (uint8_t*)DUMMYCOMPRESSOR_UNCOMPRESSED_BYTECODE, codeLen);
	pointer += codeLen;

	/*
	*	Copy data uncompressed
	*/
	memcpy(tcomp_buffer_getBufferAtPos(output_buffer, pointer), input_ptr, input_size);
	pointer += input_size;
	*output_size = (pointer);

	TSK_OBJECT_SAFE_FREE(output_buffer);

	return tsk_true;
}
