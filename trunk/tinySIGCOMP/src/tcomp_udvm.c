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

/**@file tcomp_udvm.c
 * @brief  The machine architecture described in this document.  The UDVM is used to decompress SigComp messages.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tcomp_udvm.h"
#include "tsk_debug.h"

#include <string.h>

/**@defgroup tcomp_udvm_group SIGCOMP UDVM machine.
* The machine architecture described in this document.  The UDVM is used to decompress SigComp messages.
*/

#define TCOMP_UDVM_HEADER_RESERVED_SIZE 22






















//========================================================
//	UDVM machine definition
//
/**@ingroup tcomp_udvm_group
*/
static void* tcomp_udvm_create(void * self, va_list * app)
{
	tcomp_udvm_t *udvm = self;
	if(udvm)
	{
		/* RFC 3320 - 7.  SigComp Message Format */
		udvm->sigCompMessage = va_arg(*app, tcomp_message_t *);
		udvm->stateHandler = va_arg(*app, tcomp_statehandler_t *);
		udvm->lpResult = va_arg(*app, tcomp_result_t *);
		
		udvm->isOK = 1;
		udvm->maximum_UDVM_cycles = 0; /* RFC 3320 subclause 8.6 */
		udvm->consumed_cycles = 0;

		/* Alloc UDVM memory */
		if(udvm->sigCompMessage->stream_based)
		{
			/*
			* If the transport is stream-based however, then a fixed-size input buffer is required to accommodate the stream, independently of the
			* size of each SigComp message. So, for simplicity, the UDVM memory size is set to (decompression_memory_size / 2).
			*/

			tcomp_buffer_allocBuff(udvm->memory, udvm->stateHandler->sigcomp_parameters->dmsValue/2);
		}
		else
		{
			/*
			* If the transport is message-based then sufficient memory must be available
			* to buffer the entire SigComp message before it is passed to the UDVM. So if the message is n bytes long, then the UDVM memory size is set
			* to (decompression_memory_size - n), up to a maximum of 65536 bytes.
			*/
			tcomp_buffer_allocBuff(udvm->memory, (udvm->stateHandler->sigcomp_parameters->dmsValue-udvm->sigCompMessage->totalSize));
		}

		/*
		* Has feedback with my state id?
		*/
		if(tcomp_buffer_getSize(udvm->sigCompMessage->ret_feedback_buffer))
		{
			size_t size = tcomp_buffer_getSize(udvm->sigCompMessage->ret_feedback_buffer);
			tcomp_buffer_allocBuff(udvm->lpResult->ret_feedback, size);
			memmove(tcomp_buffer_getBuffer(udvm->lpResult->ret_feedback), tcomp_buffer_getBuffer(udvm->sigCompMessage->ret_feedback_buffer), size);
		}

		/*
		*	Has state?
		*/
		if(tcomp_buffer_getSize(udvm->sigCompMessage->stateId))
		{
			/* Find the provided state */
			tcomp_state_t* lpState = NULL;
			uint16_t match_count = tcomp_statehandler_findState(udvm->stateHandler, udvm->sigCompMessage->stateId, &lpState);
			if( (!match_count || match_count>1 || !lpState)
				|| (lpState->minimum_access_length > tcomp_buffer_getSize(udvm->sigCompMessage->stateId))
				|| ((size_t)(lpState->address + lpState->length) > tcomp_buffer_getSize(udvm->memory)) )
			{
				tcomp_udvm_createNackInfo(udvm, NACK_STATE_NOT_FOUND, udvm->sigCompMessage->stateId, 0);
				udvm->isOK = 0;
				return self;
			}
			//this->sigCompMessage->stateId.print();//FIXME
			/*
			* Copy bytecodes to UDVM memory
			*/
			if( (size_t)(lpState->address + lpState->length) >= tcomp_buffer_getSize(udvm->memory) )
			{
				tcomp_udvm_createNackInfo2(udvm, NACK_SEGFAULT);
				udvm->isOK = 0;
				return self;
			}
			memmove( tcomp_buffer_getBufferAtPos(udvm, lpState->address), 
				tcomp_buffer_getBuffer(lpState->value), 
				tcomp_buffer_getSize(lpState->value) );
			
			//RFC 3320 - 7.2.  Accessing Stored State (Useful values)
			TCOMP_UDVM_SET_2BYTES_VAL(TCOMP_UDVM_HEADER_PARTIAL_STATE_ID_LENGTH_INDEX, tcomp_buffer_getSize(udvm->sigCompMessage->stateId));
			TCOMP_UDVM_SET_2BYTES_VAL(TCOMP_UDVM_HEADER_STATE_LENGTH_INDEX, tcomp_buffer_getSize(lpState->value));
			
			udvm->executionPointer = lpState->instruction;
		}
		else // DON'T HAVE STATE
		{
			//
			// Copy bytecodes to UDVM memory
			//
			size_t bytecodes_destination = udvm->sigCompMessage->bytecodes_destination;
			if( (bytecodes_destination + tcomp_buffer_getSize(udvm->sigCompMessage->uploaded_UDVM_buffer)) >= tcomp_buffer_getSize(udvm->memory) )
			{
				tcomp_udvm_createNackInfo2(udvm, NACK_BYTECODES_TOO_LARGE);
				udvm->isOK = 0;
				return self;
			}
			memmove( tcomp_buffer_getBufferAtPos(udvm, bytecodes_destination),
				tcomp_buffer_getBuffer(udvm->sigCompMessage->uploaded_UDVM_buffer),
				tcomp_buffer_getSize(udvm->sigCompMessage->uploaded_UDVM_buffer));

			// Set pointer indicating execution index
			udvm->executionPointer = bytecodes_destination;
		}

		/* RFC 3320-Section_8.6.  UDVM Cycles
		//To ensure that a SigComp message cannot consume excessive processing
		resources, SigComp limits the number of "UDVM cycles" allocated to
		each message.  The number of available UDVM cycles is initialized to
		1000 plus the number of bits in the SigComp header (as described in
		Section 7); this sum is then multiplied by cycles_per_bit.*/
		/*maximum_UDVM_cycles = (8 * n + 1000) * cycles_per_bit*///FIXME:header_size
		udvm->maximum_UDVM_cycles = ( (8 * udvm->sigCompMessage->header_size + 1000)* udvm->stateHandler->sigcomp_parameters->cpbValue );
		
		//
		//	RFC 3320 - 7.2.  Useful values
		//
		TCOMP_UDVM_SET_2BYTES_VAL(TCOMP_UDVM_HEADER_UDVM_MEMORY_SIZE_INDEX, tcomp_buffer_getSize(udvm->memory));
		TCOMP_UDVM_SET_2BYTES_VAL(TCOMP_UDVM_HEADER_CYCLES_PER_BIT_INDEX, udvm->stateHandler->sigcomp_parameters->cpbValue);
		TCOMP_UDVM_SET_2BYTES_VAL(TCOMP_UDVM_HEADER_SIGCOMP_VERSION_INDEX, udvm->stateHandler->sigcomp_parameters->SigComp_version);
		memset(tcomp_buffer_getBufferAtPos(udvm, TCOMP_UDVM_HEADER_RESERVED_INDEX), 0, TCOMP_UDVM_HEADER_RESERVED_SIZE);
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new udvm machine.");
	}
	return self;
}

/**@ingroup tcomp_udvm_group
*/
static void* tcomp_udvm_destroy(void *self)
{
	tcomp_udvm_t *udvm = self;
	if(udvm)
	{
		TCOMP_BUFFER_SAFE_FREE(udvm->memory);
	}
	else TSK_DEBUG_ERROR("Null udvm machine.");

	return self;
}

static const tsk_object_def_t tcomp_udvm_def_s = 
{
	sizeof(tcomp_udvm_t),
	tcomp_udvm_create,
	tcomp_udvm_destroy,
	0,
	0,
	0
};
const void *tcomp_udvm_def_t = &tcomp_udvm_def_s;