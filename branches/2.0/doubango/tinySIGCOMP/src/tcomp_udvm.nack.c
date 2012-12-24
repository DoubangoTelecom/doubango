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
 * @brief  SigComp UDVM machine (NACK).
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *

 */
#include "tcomp_udvm.h"

#include "tsk_sha1.h"

void tcomp_udvm_createNackInfo(tcomp_udvm_t *udvm, uint8_t reasonCode, tcomp_buffer_handle_t* lpDetails, int16_t memory_address_of_instruction)
{
#define NACK_HEADER_INDEX		0
#define NACK_VERSION_INDEX		(NACK_HEADER_INDEX+2)
#define NACK_REASON_CODE_INDEX	(NACK_VERSION_INDEX+1)
#define NACK_OPCODE_INDEX		(NACK_REASON_CODE_INDEX+1)
#define NACK_PC_INDEX			(NACK_OPCODE_INDEX+1)
#define NACK_SHA1_INDEX			(NACK_PC_INDEX+2)
#define NACK_DETAILS_INDEX		(NACK_PC_INDEX+SHA1HashSize)
	/*  0   1   2   3   4   5   6   7
        +---+---+---+---+---+---+---+---+
        | 1   1   1   1   1 | T |   0   |
        +---+---+---+---+---+---+---+---+
        |                               |
        :    returned feedback item     :
        |                               |
        +---+---+---+---+---+---+---+---+
        |         code_len = 0          |
        +---+---+---+---+---+---+---+---+
        | code_len = 0  |  version = 1  |
        +---+---+---+---+---+---+---+---+
        |          Reason Code          |
        +---+---+---+---+---+---+---+---+
        |  OPCODE of failed instruction |
        +---+---+---+---+---+---+---+---+
        |   PC of failed instruction    |
        |                               |
        +---+---+---+---+---+---+---+---+
        |                               |
        : SHA-1 Hash of failed message  :
        |                               |
        +---+---+---+---+---+---+---+---+
        |                               |
        :         Error Details         :
        |                               |
        +---+---+---+---+---+---+---+---+
	*/

	uint32_t mem_add_instruction = (memory_address_of_instruction >=0) ? memory_address_of_instruction : udvm->last_memory_address_of_instruction;
	tsk_sha1context_t sha;
	uint8_t *nackbuffer_ptr;

	tcomp_buffer_allocBuff(udvm->lpResult->nack_info, NACK_SHA1_INDEX + TSK_SHA1_DIGEST_SIZE);
	nackbuffer_ptr = tcomp_buffer_getBuffer(udvm->lpResult->nack_info);
	
	nackbuffer_ptr[NACK_HEADER_INDEX] = 0xf8;
	nackbuffer_ptr[NACK_VERSION_INDEX] = NACK_VERSION;
	nackbuffer_ptr[NACK_REASON_CODE_INDEX] = reasonCode;
	nackbuffer_ptr[NACK_OPCODE_INDEX] = *TCOMP_UDVM_GET_BUFFER_AT(mem_add_instruction);
	nackbuffer_ptr[NACK_PC_INDEX] = (mem_add_instruction >> 8);
	nackbuffer_ptr[NACK_PC_INDEX+1] = (mem_add_instruction & 0x00ff);
	
	/*
	* SHA-1 computation
	*/
	tsk_sha1reset(&sha);
	tsk_sha1input(&sha, udvm->sigCompMessage->startPtr, udvm->sigCompMessage->totalSize);
	tsk_sha1result(&sha, (uint8_t*)(nackbuffer_ptr + NACK_SHA1_INDEX));
	
	/*
	* Details
	*/
	if(lpDetails && tcomp_buffer_getSize(lpDetails))
	{
		tcomp_buffer_appendBuff(udvm->lpResult->nack_info, tcomp_buffer_getBuffer(lpDetails), tcomp_buffer_getSize(lpDetails));
	}
	else if(reasonCode == NACK_BYTECODES_TOO_LARGE)
	{
		uint32_t memorySize = TCOMP_UDVM_GET_SIZE();
		tcomp_buffer_appendBuff(udvm->lpResult->nack_info, &memorySize, 2);
	}
	else if(reasonCode == NACK_CYCLES_EXHAUSTED)
	{
		uint8_t cpb = udvm->stateHandler->sigcomp_parameters->cpbValue;
		tcomp_buffer_appendBuff(udvm->lpResult->nack_info, &cpb, 1);
	}

	udvm->lpResult->isNack = 1;
}
