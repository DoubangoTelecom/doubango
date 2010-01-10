/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou@yahoo.fr>
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
* GNU Lesser General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

/**@file tcomp_compressor.h
 * @brief  Deflate compressor.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tcomp_compressor_deflate.h"
#include "tcomp_deflatedata.h"

#include "tsk_debug.h"

#include <string.h>

#define TCOMP_MIN(a, b) (a < b ? a : b)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @fn	int tcomp_compressor_deflate_compress(tcomp_compartment_t *lpCompartment,
/// 	const void *input_ptr, size_t input_size, void *output_ptr, size_t *output_size,
/// 	int stream)
///
/// @brief	Compress SIgComp message using deflate algorithm. 
///
/// @author	Mamadou
/// @date	11/29/2009
///
/// @param [in,out]	lpCompartment	If non-null, the pointer to a compartment. 
/// @param [in,out]	input_ptr		If non-null, the input pointer. 
/// @param	input_size				Size of the input. 
/// @param [in,out]	output_ptr		If non-null, the output pointer. 
/// @param [in,out]	output_size		If non-null, size of the output. 
/// @param	stream					The stream. 
///
/// @return	. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int tcomp_compressor_deflate_compress(tcomp_compartment_t *lpCompartment, const void *input_ptr, size_t input_size, void *output_ptr, size_t *output_size, int stream)
{
#define GET_OUTPUT_BUFFER_AT(position) (((uint8_t*)output_ptr) + position)

	int result = 1, stateChanged, stateful, windowBits, zret;
	tcomp_deflatedata_t *deflatedata = 0;
	size_t pointer = 0, state_len_index = 0, compressedDataLen;
	uint8_t smsCode, *header;

	tsk_safeobj_lock(lpCompartment);
	
	/* Compression Data */
	if(!lpCompartment->compressorData)
	{
		lpCompartment->compressorData = TCOMP_DEFLATEDATA_CREATE(stream);
		if(!lpCompartment->compressorData)
		{
			TSK_DEBUG_ERROR("Failed to create deflate compressor data.");
			result = 0;
			goto bail;
		}
		else
		{
			lpCompartment->ackGhost = tcomp_deflatedata_ackGhost;
			lpCompartment->freeGhostState = tcomp_deflatedata_freeGhostState;
			lpCompartment->compressorData_isStream  = stream;
		}
	}
		
	deflatedata = lpCompartment->compressorData;

	/* State memory size code */
	smsCode = TCOMP_MIN(lpCompartment->remote_parameters->smsCode, lpCompartment->remote_parameters->dmsCode);
#if USE_ONLY_ACKED_STATES
	stateful = (deflatedata->ghostState && tcomp_deflatedata_isStateful(deflatedata));
#else
	stateful = (deflatedata->ghostState != 0);
#endif

	/*
	*	Init zLIB
	*/
	windowBits = ( smsCode - (stream?2:1) ) + 10;
	windowBits = (windowBits < 8) ? 8 : ( (windowBits > 15 ? 15 : windowBits) ); /* Because of zlib limitation (windowsize MUST be between 8 and 15) */
	if(windowBits != deflatedata->zWindowBits)
	{
		/* Window size changed */
		tcomp_deflatedata_freeGhostState(deflatedata);
		tcomp_deflatedata_zSetWindowBits(deflatedata, windowBits);

		if( !(result = tcomp_deflatedata_zReset(deflatedata)) ) 
		{
			goto bail;
		}
	}
	else if(!deflatedata->ghostState)
	{
		/* No ghost --> reset zlib */
		deflatedata->ghost_copy_offset = 0;
		if( !(result = tcomp_deflatedata_zReset(deflatedata)) ) 
		{
			goto bail;
		}
	}

	/*
	*	SigComp headers
	*/
	header = GET_OUTPUT_BUFFER_AT(pointer++);

	
	/* SigComp Header */
	if(lpCompartment->lpReqFeedback && tcomp_buffer_getSize(lpCompartment->lpReqFeedback))
	{
		/* Return the requested feedback */
		*header = 0xfc; /* T=1 */
		memcpy(GET_OUTPUT_BUFFER_AT(pointer), tcomp_buffer_getBuffer(lpCompartment->lpReqFeedback), tcomp_buffer_getSize(lpCompartment->lpReqFeedback));
		pointer += tcomp_buffer_getSize(lpCompartment->lpReqFeedback);
	}
	else
	{
		*header = 0xf8;
	}

	/*
	* Stateless or stateful?
	*/
	if(stateful)
	{
		memcpy(GET_OUTPUT_BUFFER_AT(pointer), tcomp_buffer_getBuffer(deflatedata->ghostState->identifier), TCOMP_PARTIAL_ID_LEN_VALUE);

		pointer += TCOMP_PARTIAL_ID_LEN_VALUE; 
		*header |= TCOMP_PARTIAL_ID_LEN_CODE;

		TSK_DEBUG_INFO("Compressing stateful message.");
	}
	else
	{
		uint16_t codeLen = DEFLATE_BYTECODE_LEN;
		/* first byte for codelen */
		*GET_OUTPUT_BUFFER_AT(pointer++) = ((codeLen>>4)& 0x00ff);
		/* last 4 bits for codelen */
		*GET_OUTPUT_BUFFER_AT(pointer) = ((codeLen & 0x000f)<<4); 
		/* first and last 4 bits for destination */
		*GET_OUTPUT_BUFFER_AT(pointer++) |= DEFLATE_BYTECODE_DESTINATION_CODE;

		/*
		*	Upload UDVM bytecode
		*/
		memcpy(GET_OUTPUT_BUFFER_AT(pointer), (const uint8_t*)DEFLATEDATA_DEFLATE_BYTECODE, codeLen);
		pointer += codeLen;

		//////////////////////////////////////////////////
		// FIXME: check for overflow and >=320
		//
		// [DMS]+[Req. Fed. Loc.]+[state_len]+[cpb||dms||sms]+[Sigcomp_version]+[states]
		//*output_buffer.getBuffer(pointer++) = 0x04; //reserved=0, Q=1, S=0, I=0
		//*output_buffer.getBuffer(pointer++) = (this->req_feedback_item++); //requested feedback item
		
		state_len_index  = pointer;
		*GET_OUTPUT_BUFFER_AT(pointer) = 0x00, pointer += 4; /* [hash_len]+[state_len] */
		*GET_OUTPUT_BUFFER_AT(pointer++) = (tcomp_params_getParameters(lpCompartment->local_parameters)>>8); // [cpb||dms||sms]
		*GET_OUTPUT_BUFFER_AT(pointer++) = (tcomp_params_getParameters(lpCompartment->local_parameters)&0x00ff); // [Sigcomp_version]
#if USE_DICTS_FOR_COMPRESSION
		*output_buffer.getBuffer(pointer++) = 0x00; // First dict byte	// FIXME
		*output_buffer.getBuffer(pointer++) = DEFLATE_FIXME_DICT; // FIXME: also change ghost
#endif
		TSK_DEBUG_INFO("Compressing stateless message.");
	}

	/*
	*	Compress data using ZLIB
	*/
	compressedDataLen = (*output_size - pointer);
	zret = tcomp_deflatedata_zCompress(deflatedata, input_ptr, input_size, GET_OUTPUT_BUFFER_AT(pointer), &compressedDataLen, &stateChanged);
	if(!zret)
	{
		result = 0;
		goto bail;
	}
	pointer += compressedDataLen;
	*output_size = (pointer);

	/*
	* Update state length
	*/
	if(!stateful)
	{	
		uint16_t state_len = ( (1<<(deflatedata->zWindowBits)) + DEFLATE_UDVM_CIRCULAR_START_INDEX - 64 );
		uint32_t hash_len = (state_len + 8);
		
		// FIXME: 131072  could not go in 2-bytes
		*GET_OUTPUT_BUFFER_AT(state_len_index) = (hash_len >> 8);
		*GET_OUTPUT_BUFFER_AT(state_len_index+1) = (hash_len & 0x00ff);
		*GET_OUTPUT_BUFFER_AT(state_len_index+2) = (state_len >> 8);
		*GET_OUTPUT_BUFFER_AT(state_len_index+3) = (state_len & 0x00ff);

		/*	First time or synchronize failure (NACK reason=STATE_NOT_FOUND) */
		if(!deflatedata->ghostState)
		{
			tcomp_deflatedata_createGhost(deflatedata, state_len, lpCompartment->local_parameters);
		}
	}
#if USE_ONLY_ACKED_STATES
	if(stateChanged)
#endif
	{
		tcomp_deflatedata_updateGhost(deflatedata, (const uint8_t*)input_ptr, input_size);
	}

	//output_buffer.print(2000);
	
bail:
	tsk_safeobj_unlock(lpCompartment);

	return result;

#undef GET_OUTPUT_BUFFER_AT
}
