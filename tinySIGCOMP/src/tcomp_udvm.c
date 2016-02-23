/*
* Copyright (C) 2010-2011 Mamadou Diop
* Copyright (C) 2011-2013 Doubango Telecom <http://www.doubango.org>
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

/**@file tcomp_udvm.c
 * @brief  SigComp UDVM machine.
 */
#include "tcomp_udvm.h"

#include "tsk_memory.h"
#include "tsk_debug.h"

#include <string.h>

#define TCOMP_UDVM_HEADER_RESERVED_SIZE 22

tcomp_udvm_t* tcomp_udvm_create(tcomp_message_t* _sigCompMessage, tcomp_statehandler_t*  stateHandler, tcomp_result_t* lpResult)
{
    tcomp_udvm_t *udvm;
    if((udvm = tsk_object_new(tcomp_udvm_def_t))) {
        /* RFC 3320 - 7.  SigComp Message Format */
        udvm->sigCompMessage = tsk_object_ref(_sigCompMessage);
        udvm->stateHandler = tsk_object_ref(stateHandler);
        udvm->lpResult = tsk_object_ref(lpResult);

        udvm->isOK = tsk_true;
        udvm->maximum_UDVM_cycles = 0; /* RFC 3320 subclause 8.6 */
        udvm->consumed_cycles = 0;
        udvm->memory = tcomp_buffer_create_null();

        /* Alloc UDVM memory */
        if(udvm->sigCompMessage->stream_based) {
            /*
            * If the transport is stream-based however, then a fixed-size input buffer is required to accommodate the stream, independently of the
            * size of each SigComp message. So, for simplicity, the UDVM memory size is set to (decompression_memory_size / 2).
            */

            tcomp_buffer_allocBuff(udvm->memory, udvm->stateHandler->sigcomp_parameters->dmsValue/2);
        }
        else {
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
        if(tcomp_buffer_getSize(udvm->sigCompMessage->ret_feedback_buffer)) {
            tsk_size_t size = tcomp_buffer_getSize(udvm->sigCompMessage->ret_feedback_buffer);
            tcomp_buffer_allocBuff(udvm->lpResult->ret_feedback, size);
            memcpy(tcomp_buffer_getBuffer(udvm->lpResult->ret_feedback), tcomp_buffer_getBuffer(udvm->sigCompMessage->ret_feedback_buffer), size);
        }

        /*
        *	Has state?
        */
        if(tcomp_buffer_getSize(udvm->sigCompMessage->stateId)) {
            /* Find the provided state */
            tcomp_state_t* lpState = NULL;
            uint32_t match_count = tcomp_statehandler_findState(udvm->stateHandler, udvm->sigCompMessage->stateId, &lpState);
            if( (match_count != 1 || !lpState)
                    || (lpState->minimum_access_length > tcomp_buffer_getSize(udvm->sigCompMessage->stateId))
                    || ((tsk_size_t)(lpState->address + lpState->length) > TCOMP_UDVM_GET_SIZE()) ) {
                TSK_DEBUG_ERROR("NACK_STATE_NOT_FOUND for id = ");
                tcomp_buffer_print(udvm->sigCompMessage->stateId);
                tcomp_udvm_createNackInfo(udvm, NACK_STATE_NOT_FOUND, udvm->sigCompMessage->stateId, 0);
                udvm->isOK = tsk_false;
                goto bail;
            }
            /*
            * Copy bytecodes to UDVM memory
            */
            if( (tsk_size_t)(lpState->address + lpState->length) >= TCOMP_UDVM_GET_SIZE() ) {
                tcomp_udvm_createNackInfo2(udvm, NACK_SEGFAULT);
                udvm->isOK = tsk_false;
                goto bail;
            }
            memcpy( TCOMP_UDVM_GET_BUFFER_AT(lpState->address),
                    tcomp_buffer_getBuffer(lpState->value),
                    tcomp_buffer_getSize(lpState->value) );

            //RFC 3320 - 7.2.  Accessing Stored State (Useful values)
            TCOMP_UDVM_SET_2BYTES_VAL(TCOMP_UDVM_HEADER_PARTIAL_STATE_ID_LENGTH_INDEX, tcomp_buffer_getSize(udvm->sigCompMessage->stateId));
            TCOMP_UDVM_SET_2BYTES_VAL(TCOMP_UDVM_HEADER_STATE_LENGTH_INDEX, tcomp_buffer_getSize(lpState->value));

            udvm->executionPointer = lpState->instruction;
        }
        else { // DON'T HAVE STATE
            /*
            * Copy bytecodes to UDVM memory
            */
            tsk_size_t bytecodes_destination = udvm->sigCompMessage->bytecodes_destination;
            if( (bytecodes_destination + tcomp_buffer_getSize(udvm->sigCompMessage->uploaded_UDVM_buffer)) >= TCOMP_UDVM_GET_SIZE() ) {
                tcomp_udvm_createNackInfo2(udvm, NACK_BYTECODES_TOO_LARGE);
                udvm->isOK = tsk_false;
                goto bail;
            }
            memcpy( TCOMP_UDVM_GET_BUFFER_AT(bytecodes_destination),
                    tcomp_buffer_getBuffer(udvm->sigCompMessage->uploaded_UDVM_buffer),
                    tcomp_buffer_getSize(udvm->sigCompMessage->uploaded_UDVM_buffer));

            // Set pointer indicating execution index
            udvm->executionPointer = bytecodes_destination;
        }

        /* RFC 3320-Section_8.6.  UDVM Cycles
        *
        * the maximum number of UDVM cycles available for processing an n-byte SigComp message is given by the formula
        * maximum_UDVM_cycles = (8 * n + 1000) * cycles_per_bit
        */
        udvm->maximum_UDVM_cycles = ( (8 * udvm->sigCompMessage->header_size + 1000) * udvm->stateHandler->sigcomp_parameters->cpbValue );

        //
        //	RFC 3320 - 7.2.  Useful values
        //
        TCOMP_UDVM_SET_2BYTES_VAL(TCOMP_UDVM_HEADER_UDVM_MEMORY_SIZE_INDEX, TCOMP_UDVM_GET_SIZE());
        TCOMP_UDVM_SET_2BYTES_VAL(TCOMP_UDVM_HEADER_CYCLES_PER_BIT_INDEX, udvm->stateHandler->sigcomp_parameters->cpbValue);
        TCOMP_UDVM_SET_2BYTES_VAL(TCOMP_UDVM_HEADER_SIGCOMP_VERSION_INDEX, udvm->stateHandler->sigcomp_parameters->SigComp_version);
        memset(TCOMP_UDVM_GET_BUFFER_AT(TCOMP_UDVM_HEADER_RESERVED_INDEX), 0, TCOMP_UDVM_HEADER_RESERVED_SIZE);
    }
    else {
        TSK_DEBUG_ERROR("Failed to create new udvm machine.");
    }

bail:
    return udvm;
}

/**Executes the bytecode.
*/
static tsk_bool_t tcomp_udvm_runByteCode(tcomp_udvm_t *udvm)
{
    uint32_t operand_1, operand_2, operand_3, operand_4, operand_5, operand_6, operand_7;
    tsk_bool_t excution_failed = tsk_false, end_message = tsk_false;
    if(!udvm->isOK) {
        TSK_DEBUG_ERROR("Cannot run()/execute() invalid bytecode");
        return tsk_false;
    }

    // LOOP - EXCUTE all bytecode
    while( !excution_failed && !end_message ) {
        uint8_t udvm_instruction = * (TCOMP_UDVM_GET_BUFFER_AT(udvm->executionPointer));
        udvm->last_memory_address_of_instruction = udvm->executionPointer;
        udvm->executionPointer++; /* Skip the 1-byte [INSTRUCTION]. */

        switch(udvm_instruction) {
        case TCOMP_UDVM_INST__DECOMPRESSION_FAILURE: {
            TCOMP_UDVM_EXEC_INST__DECOMPRESSION_FAILURE(udvm);
            excution_failed = tsk_true;
            break;
        }

        case TCOMP_UDVM_INST__AND: {
            operand_1 = tcomp_udvm_opget_reference_param(udvm);
            operand_2 = tcomp_udvm_opget_multitype_param(udvm);
            excution_failed = !TCOMP_UDVM_EXEC_INST__AND(udvm, operand_1, operand_2);
            break;
        }

        case TCOMP_UDVM_INST__OR: {
            operand_1 = tcomp_udvm_opget_reference_param(udvm);
            operand_2 = tcomp_udvm_opget_multitype_param(udvm);
            excution_failed = !TCOMP_UDVM_EXEC_INST__OR(udvm, operand_1, operand_2);
            break;
        }

        case TCOMP_UDVM_INST__NOT: {
            operand_1 = tcomp_udvm_opget_reference_param(udvm);
            excution_failed = !TCOMP_UDVM_EXEC_INST__NOT(udvm, operand_1);
            break;
        }

        case TCOMP_UDVM_INST__LSHIFT: {
            operand_1 = tcomp_udvm_opget_reference_param(udvm);
            operand_2 = tcomp_udvm_opget_multitype_param(udvm);
            excution_failed = !TCOMP_UDVM_EXEC_INST__LSHIFT(udvm, operand_1, operand_2);
            break;
        }

        case TCOMP_UDVM_INST__RSHIFT: {
            operand_1 = tcomp_udvm_opget_reference_param(udvm);
            operand_2 = tcomp_udvm_opget_multitype_param(udvm);
            excution_failed = !TCOMP_UDVM_EXEC_INST__RSHIFT(udvm, operand_1, operand_2);
            break;
        }

        case TCOMP_UDVM_INST__ADD: {
            operand_1 = tcomp_udvm_opget_reference_param(udvm);
            operand_2 = tcomp_udvm_opget_multitype_param(udvm);
            excution_failed = !TCOMP_UDVM_EXEC_INST__ADD(udvm, operand_1, operand_2);
            break;
        }

        case TCOMP_UDVM_INST__SUBTRACT: {
            operand_1 = tcomp_udvm_opget_reference_param(udvm);
            operand_2 = tcomp_udvm_opget_multitype_param(udvm);
            excution_failed = !TCOMP_UDVM_EXEC_INST__SUBTRACT(udvm, operand_1, operand_2);
            break;
        }

        case TCOMP_UDVM_INST__MULTIPLY: {
            operand_1 = tcomp_udvm_opget_reference_param(udvm);
            operand_2 = tcomp_udvm_opget_multitype_param(udvm);
            excution_failed = !TCOMP_UDVM_EXEC_INST__MULTIPLY(udvm, operand_1, operand_2);
            break;
        }

        case TCOMP_UDVM_INST__DIVIDE: {
            operand_1 = tcomp_udvm_opget_reference_param(udvm);
            operand_2 = tcomp_udvm_opget_multitype_param(udvm);
            excution_failed = !TCOMP_UDVM_EXEC_INST__DIVIDE(udvm, operand_1, operand_2);
            break;
        }

        case TCOMP_UDVM_INST__REMAINDER: {
            operand_1 = tcomp_udvm_opget_reference_param(udvm);
            operand_2 = tcomp_udvm_opget_multitype_param(udvm);
            excution_failed = !TCOMP_UDVM_EXEC_INST__REMAINDER(udvm, operand_1, operand_2);
            break;
        }

        case TCOMP_UDVM_INST__SORT_ASCENDING: {
            operand_1 = tcomp_udvm_opget_multitype_param(udvm);
            operand_2 = tcomp_udvm_opget_multitype_param(udvm);
            operand_3 = tcomp_udvm_opget_multitype_param(udvm);
            excution_failed = !TCOMP_UDVM_EXEC_INST__SORT_ASCENDING(udvm, operand_1, operand_2, operand_3);
            break;
        }

        case TCOMP_UDVM_INST__SORT_DESCENDING: {
            operand_1 = tcomp_udvm_opget_multitype_param(udvm);
            operand_2 = tcomp_udvm_opget_multitype_param(udvm);
            operand_3 = tcomp_udvm_opget_multitype_param(udvm);
            excution_failed = !TCOMP_UDVM_EXEC_INST__SORT_DESCENDING(udvm, operand_1, operand_2, operand_3);
            break;
        }

        case TCOMP_UDVM_INST__SHA_1: {
            operand_1 = tcomp_udvm_opget_multitype_param(udvm);
            operand_2 = tcomp_udvm_opget_multitype_param(udvm);
            operand_3 = tcomp_udvm_opget_multitype_param(udvm);
            excution_failed = !TCOMP_UDVM_EXEC_INST__SHA_1(udvm, operand_1, operand_2, operand_3);
            break;
        }

        case TCOMP_UDVM_INST__LOAD: {
            operand_1 = tcomp_udvm_opget_multitype_param(udvm);
            operand_2 = tcomp_udvm_opget_multitype_param(udvm);
            excution_failed = !TCOMP_UDVM_EXEC_INST__LOAD(udvm, operand_1, operand_2);
            break;
        }

        case TCOMP_UDVM_INST__MULTILOAD: {
            operand_1 = tcomp_udvm_opget_multitype_param(udvm);
            operand_2 = tcomp_udvm_opget_literal_param(udvm);
            excution_failed = !TCOMP_UDVM_EXEC_INST__MULTILOAD(udvm, operand_1, operand_2);
            break;
        }

        case TCOMP_UDVM_INST__PUSH: {
            excution_failed = !TCOMP_UDVM_EXEC_INST__PUSH2(udvm);
            break;
        }

        case TCOMP_UDVM_INST__POP: {
            excution_failed = !TCOMP_UDVM_EXEC_INST__POP2(udvm);
            break;
        }

        case TCOMP_UDVM_INST__COPY: {
            operand_1 = tcomp_udvm_opget_multitype_param(udvm);
            operand_2 = tcomp_udvm_opget_multitype_param(udvm);
            operand_3 = tcomp_udvm_opget_multitype_param(udvm);
            excution_failed = !TCOMP_UDVM_EXEC_INST__COPY(udvm, operand_1, operand_2, operand_3);
            break;
        }

        case TCOMP_UDVM_INST__COPY_LITERAL: {
            operand_1 = tcomp_udvm_opget_multitype_param(udvm);
            operand_2 = tcomp_udvm_opget_multitype_param(udvm);
            operand_3 = tcomp_udvm_opget_reference_param(udvm);
            excution_failed = !TCOMP_UDVM_EXEC_INST__COPY_LITERAL(udvm, operand_1, operand_2, operand_3);
            break;
        }

        case TCOMP_UDVM_INST__COPY_OFFSET: {
            operand_1 = tcomp_udvm_opget_multitype_param(udvm);
            operand_2 = tcomp_udvm_opget_multitype_param(udvm);
            operand_3 = tcomp_udvm_opget_reference_param(udvm);
            excution_failed = !TCOMP_UDVM_EXEC_INST__COPY_OFFSET(udvm, operand_1, operand_2, operand_3);
            break;
        }

        case TCOMP_UDVM_INST__MEMSET: {
            operand_1 = tcomp_udvm_opget_multitype_param(udvm);
            operand_2 = tcomp_udvm_opget_multitype_param(udvm);
            operand_3 = tcomp_udvm_opget_multitype_param(udvm);
            operand_4 = tcomp_udvm_opget_multitype_param(udvm);
            excution_failed = !TCOMP_UDVM_EXEC_INST__MEMSET(udvm, operand_1, operand_2, operand_3, operand_4);
            break;
        }

        case TCOMP_UDVM_INST__JUMP: {
            excution_failed = !TCOMP_UDVM_EXEC_INST__JUMP2(udvm);
            break;
        }

        case TCOMP_UDVM_INST__COMPARE: {
            operand_1 = tcomp_udvm_opget_multitype_param(udvm);
            operand_2 = tcomp_udvm_opget_multitype_param(udvm);
            operand_3 = tcomp_udvm_opget_address_param(udvm, udvm->last_memory_address_of_instruction);
            operand_4 = tcomp_udvm_opget_address_param(udvm, udvm->last_memory_address_of_instruction);
            operand_5 = tcomp_udvm_opget_address_param(udvm, udvm->last_memory_address_of_instruction);
            excution_failed = !TCOMP_UDVM_EXEC_INST__COMPARE(udvm, operand_1, operand_2, operand_3, operand_4, operand_5);
            break;
        }

        case TCOMP_UDVM_INST__CALL: {
            operand_1 = tcomp_udvm_opget_address_param(udvm, udvm->last_memory_address_of_instruction);
            excution_failed = !TCOMP_UDVM_EXEC_INST__CALL(udvm, operand_1);
            break;
        }

        case TCOMP_UDVM_INST__RETURN: {
            excution_failed = !TCOMP_UDVM_EXEC_INST__RETURN(udvm);
            break;
        }

        case TCOMP_UDVM_INST__SWITCH: {
            operand_1 = tcomp_udvm_opget_literal_param(udvm);
            operand_2 = tcomp_udvm_opget_multitype_param(udvm);
            excution_failed = !TCOMP_UDVM_EXEC_INST__SWITCH(udvm, operand_1, operand_2);
            break;
        }

        case TCOMP_UDVM_INST__CRC: {
            operand_1 = tcomp_udvm_opget_multitype_param(udvm);
            operand_2 = tcomp_udvm_opget_multitype_param(udvm);
            operand_3 = tcomp_udvm_opget_multitype_param(udvm);
            operand_4 = tcomp_udvm_opget_reference_param(udvm);
            excution_failed = !TCOMP_UDVM_EXEC_INST__CRC(udvm, operand_1, operand_2, operand_3, operand_4);
            break;
        }

        case TCOMP_UDVM_INST__INPUT_BYTES: {
            operand_1 = tcomp_udvm_opget_multitype_param(udvm);
            operand_2 = tcomp_udvm_opget_multitype_param(udvm);
            operand_3 = tcomp_udvm_opget_address_param(udvm, udvm->last_memory_address_of_instruction);
            excution_failed = !TCOMP_UDVM_EXEC_INST__INPUT_BYTES(udvm, operand_1, operand_2, operand_3);
            break;
        }

        case TCOMP_UDVM_INST__INPUT_BITS: {
            operand_1 = tcomp_udvm_opget_multitype_param(udvm);
            operand_2 = tcomp_udvm_opget_multitype_param(udvm);
            operand_3 = tcomp_udvm_opget_address_param(udvm, udvm->last_memory_address_of_instruction);
            excution_failed = !TCOMP_UDVM_EXEC_INST__INPUT_BITS(udvm, operand_1, operand_2, operand_3);
            break;
        }

        case TCOMP_UDVM_INST__INPUT_HUFFMAN: {
            operand_1 = tcomp_udvm_opget_multitype_param(udvm);
            operand_2 = tcomp_udvm_opget_address_param(udvm, udvm->last_memory_address_of_instruction);
            operand_3 = tcomp_udvm_opget_literal_param(udvm);
            excution_failed = !TCOMP_UDVM_EXEC_INST__INPUT_HUFFMAN(udvm, operand_1, operand_2, operand_3);
            break;
        }

        case TCOMP_UDVM_INST__STATE_ACCESS: {
            operand_1 = tcomp_udvm_opget_multitype_param(udvm);
            operand_2 = tcomp_udvm_opget_multitype_param(udvm);
            operand_3 = tcomp_udvm_opget_multitype_param(udvm);
            operand_4 = tcomp_udvm_opget_multitype_param(udvm);
            operand_5 = tcomp_udvm_opget_multitype_param(udvm);
            operand_6 = tcomp_udvm_opget_multitype_param(udvm);
            excution_failed = !TCOMP_UDVM_EXEC_INST__STATE_ACCESS(udvm, operand_1, operand_2, operand_3, operand_4, operand_5, operand_6);
            break;
        }

        case TCOMP_UDVM_INST__STATE_CREATE: {
            operand_1 = tcomp_udvm_opget_multitype_param(udvm);
            operand_2 = tcomp_udvm_opget_multitype_param(udvm);
            operand_3 = tcomp_udvm_opget_multitype_param(udvm);
            operand_4 = tcomp_udvm_opget_multitype_param(udvm);
            operand_5 = tcomp_udvm_opget_multitype_param(udvm);
            excution_failed = !TCOMP_UDVM_EXEC_INST__STATE_CREATE(udvm, operand_1, operand_2, operand_3, operand_4, operand_5);
            break;
        }

        case TCOMP_UDVM_INST__STATE_FREE: {
            operand_1 = tcomp_udvm_opget_multitype_param(udvm);
            operand_2 = tcomp_udvm_opget_multitype_param(udvm);
            excution_failed = !TCOMP_UDVM_EXEC_INST__STATE_FREE(udvm, operand_1, operand_2);
            break;
        }

        case TCOMP_UDVM_INST__OUTPUT: {
            operand_1 = tcomp_udvm_opget_multitype_param(udvm);
            operand_2 = tcomp_udvm_opget_multitype_param(udvm);
            excution_failed = !TCOMP_UDVM_EXEC_INST__OUTPUT(udvm, operand_1, operand_2);
            break;
        }

        case TCOMP_UDVM_INST__END_MESSAGE: {
            operand_1 = tcomp_udvm_opget_multitype_param(udvm);
            operand_2 = tcomp_udvm_opget_multitype_param(udvm);
            operand_3 = tcomp_udvm_opget_multitype_param(udvm);
            operand_4 = tcomp_udvm_opget_multitype_param(udvm);
            operand_5 = tcomp_udvm_opget_multitype_param(udvm);
            operand_6 = tcomp_udvm_opget_multitype_param(udvm);
            operand_7 = tcomp_udvm_opget_multitype_param(udvm);
            excution_failed = !TCOMP_UDVM_EXEC_INST__END_MESSAGE(udvm, operand_1, operand_2, operand_3, operand_4, operand_5, operand_6, operand_7);
            end_message = 1;
            break;
        }

        default:
            tcomp_udvm_createNackInfo2(udvm, NACK_INVALID_OPCODE);
            goto bail;
        }

        if(excution_failed) {
            TSK_DEBUG_ERROR("Execution failed for instruction = %s", TCOMP_INST_DESCRIPTIONS[udvm_instruction].desc);
        }
        //TCOMP_UDVM_DEBUG_PRINT(200);
    }

bail:
    udvm->lpResult->consumed_cycles = udvm->consumed_cycles;
    return (!excution_failed);
}

/**Decompress the bytecode.
*/
tsk_bool_t tcomp_udvm_decompress(tcomp_udvm_t *udvm)
{
    return tcomp_udvm_runByteCode(udvm);
}




















//========================================================
//	UDVM machine definition
//
static tsk_object_t* tcomp_udvm_ctor(tsk_object_t * self, va_list * app)
{
    tcomp_udvm_t *udvm = self;
    if(udvm) {

    }
    return self;
}

static tsk_object_t* tcomp_udvm_dtor(tsk_object_t *self)
{
    tcomp_udvm_t *udvm = self;
    if(udvm) {
        TSK_OBJECT_SAFE_FREE(udvm->memory);
        TSK_OBJECT_SAFE_FREE(udvm->sigCompMessage);
        TSK_OBJECT_SAFE_FREE(udvm->stateHandler);
        TSK_OBJECT_SAFE_FREE(udvm->lpResult);

        TSK_FREE(udvm->tmp_buff.ptr);
    }
    else {
        TSK_DEBUG_ERROR("Null udvm machine.");
    }

    return self;
}

static const tsk_object_def_t tcomp_udvm_def_s = {
    sizeof(tcomp_udvm_t),
    tcomp_udvm_ctor,
    tcomp_udvm_dtor,
    tsk_null
};
const tsk_object_def_t *tcomp_udvm_def_t = &tcomp_udvm_def_s;
