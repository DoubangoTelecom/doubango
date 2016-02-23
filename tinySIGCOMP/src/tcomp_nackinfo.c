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

/**@file tcomp_nackinfo.c
 * @brief  RFC 4077 - A Negative Acknowledgement Mechanism for Signaling Compression (NACK).
 */
#include "tcomp_nackinfo.h"
#include "tcomp_message.h"
#include "tcomp_nack_codes.h"

#include "tsk_sha1.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

/* "OPCODE of failed instruction" is a one-byte field that includes
the opcode to which the PC was pointing when the failure occurred.
If failure occurred before the UDVM began executing any code, this
field is set to 0.
*/
#define OPCODE_UNKNOWN	0
/* "PC of failed instruction" is a two-byte field containing the
      value of the program counter when failure occurred (i.e., the
      memory address of the failed UDVM instruction).  The field is
      encoded with the most significant byte of the PC first (i.e., in
      network or big endian order).  If failure occurred before the UDVM
      began executing any code, this field is set to 0.
*/
#define PC_UNKNOWN		0

/** Creates new NACK object */
tcomp_nackinfo_t* tcomp_nackinfo_create()
{
    tcomp_nackinfo_t *nackinfo;
    if((nackinfo = tsk_object_new(tcomp_nackinfo_def_t))) {
        nackinfo->version = NACK_VERSION;
        nackinfo->details = tcomp_buffer_create_null();
    }
    return nackinfo;
}

int tcomp_nackinfo_write(tcomp_buffer_handle_t* buffer,
                         uint8_t reasonCode,
                         uint8_t opCode,
                         int16_t memory_address_of_instruction,
                         const void* sigCompMessagePtr, tsk_size_t sigCompMessageSize,
                         tcomp_buffer_handle_t* lpDetails,
                         uint16_t udvm_size,
                         uint8_t cpbValue)
{
    tsk_sha1context_t sha;
    uint8_t *nackbuffer_ptr;

    if(!buffer || !sigCompMessagePtr || !sigCompMessageSize) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tcomp_buffer_allocBuff(buffer, INDEX_NACK_SHA1 + TSK_SHA1_DIGEST_SIZE);
    if(!(nackbuffer_ptr = tcomp_buffer_getBuffer(buffer))) {
        TSK_DEBUG_ERROR("NACK buffer is null");
        return -2;
    }

    nackbuffer_ptr[INDEX_NACK_HEADER] = 0xf8;
    nackbuffer_ptr[INDEX_NACK_VERSION] = NACK_VERSION;
    nackbuffer_ptr[INDEX_NACK_REASON_CODE] = reasonCode;
    nackbuffer_ptr[INDEX_NACK_OPCODE] = opCode;
    nackbuffer_ptr[INDEX_NACK_PC] = (memory_address_of_instruction >> 8);
    nackbuffer_ptr[INDEX_NACK_PC + 1] = (memory_address_of_instruction & 0x00ff);

    // SHA-1(message) computation
    tsk_sha1reset(&sha);
    tsk_sha1input(&sha, sigCompMessagePtr, sigCompMessageSize);
    tsk_sha1result(&sha, &nackbuffer_ptr[INDEX_NACK_SHA1]);

#if 0
    {
        int i;
        TSK_DEBUG_INFO("Create NACK with id:");
        for(i = 0; i < TSK_SHA1_DIGEST_SIZE; ++i) {
            printf("%x ", nackbuffer_ptr[INDEX_NACK_SHA1 + i]);
        }
        printf("\n");
    }
#endif

    // Details
    if(lpDetails && tcomp_buffer_getSize(lpDetails)) {
        tcomp_buffer_appendBuff(buffer, tcomp_buffer_getBuffer(lpDetails), tcomp_buffer_getSize(lpDetails));
    }
    else if(reasonCode == NACK_BYTECODES_TOO_LARGE) {
        tcomp_buffer_appendBuff(buffer, &udvm_size, 2);
    }
    else if(reasonCode == NACK_CYCLES_EXHAUSTED) {
        tcomp_buffer_appendBuff(buffer, &cpbValue, 1);
    }

    return 0;

}

int tcomp_nackinfo_write_2(tcomp_buffer_handle_t* buffer,
                           uint8_t reasonCode,
                           uint8_t opCode,
                           int16_t memory_address_of_instruction,
                           const tcomp_message_t* sigCompMessage,
                           tcomp_buffer_handle_t* lpDetails,
                           uint16_t udvm_size,
                           uint8_t cpbValue)
{
    return tcomp_nackinfo_write(buffer,
                                reasonCode,
                                opCode,
                                memory_address_of_instruction,
                                sigCompMessage->startPtr, sigCompMessage->totalSize,
                                lpDetails,
                                udvm_size,
                                cpbValue);
}

int tcomp_nackinfo_write_3(tcomp_buffer_handle_t* buffer,
                           uint8_t reasonCode,
                           const void* sigCompMessagePtr, tsk_size_t sigCompMessageSize)
{
    return tcomp_nackinfo_write(buffer,
                                reasonCode,
                                OPCODE_UNKNOWN,
                                PC_UNKNOWN,
                                sigCompMessagePtr, sigCompMessageSize,
                                tsk_null,
                                0,
                                0);
}

int tcomp_nackinfo_write_4(tcomp_buffer_handle_t* buffer,
                           uint8_t reasonCode,
                           const tcomp_message_t* sigCompMessage)
{
    return tcomp_nackinfo_write_2(buffer,
                                  reasonCode,
                                  OPCODE_UNKNOWN,
                                  PC_UNKNOWN,
                                  sigCompMessage,
                                  tsk_null,
                                  0,
                                  0);
}

const char* tcomp_nackinfo_get_description(const tcomp_buffer_handle_t* buffer)
{
    uint8_t reasonCode;
    if(!buffer) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }
    if(tcomp_buffer_getSize(buffer) < 3) {
        TSK_DEBUG_ERROR("Too short");
        return tsk_null;
    }
    reasonCode = *((const uint8_t*)tcomp_buffer_getBufferAtPos(buffer, 3));
    if(reasonCode >= (sizeof(TCOMP_NACK_DESCRIPTIONS)/sizeof(TCOMP_NACK_DESCRIPTIONS[0]))) {
        TSK_DEBUG_ERROR("%d not valid as reasonCode", (int32_t)reasonCode);
        return tsk_null;
    }
    return TCOMP_NACK_DESCRIPTIONS[reasonCode].desc;
}

//========================================================
//	NackInfo object definition
//

/*
* Creates a nack info message. You MUST use @ref tcomp_nackinfo_destroy to free the nackinfo.
* @retval The NACK info message.
* @sa @ref tcomp_nackinfo_destroy.
*/
static tsk_object_t* tcomp_nackinfo_ctor(tsk_object_t *self, va_list* app)
{
    tcomp_nackinfo_t *nackinfo = self;
    if(nackinfo) {
    }
    return self;
}

/*
* Destroy a nackinfo message previously created using @ref tcomp_nackinfo_create.
* @param nackinfo The NACK info message to free.
* @sa @ref tcomp_nackinfo_create.
*/
static tsk_object_t* tcomp_nackinfo_dtor(tsk_object_t* self)
{
    tcomp_nackinfo_t *nackinfo = self;
    if(nackinfo) {
        TSK_OBJECT_SAFE_FREE(nackinfo->details);
    }
    return self;
}


static const tsk_object_def_t tcomp_nackinfo_def_s = {
    sizeof(tcomp_nackinfo_t),
    tcomp_nackinfo_ctor,
    tcomp_nackinfo_dtor,
    tsk_null
};
const tsk_object_def_t* tcomp_nackinfo_def_t = &tcomp_nackinfo_def_s;

