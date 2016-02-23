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

/**@file tcomp_udvm.nack.c
 * @brief  SigComp UDVM machine (NACK).
 *
 */
#include "tcomp_udvm.h"

#include "tsk_debug.h"

int tcomp_udvm_createNackInfo(tcomp_udvm_t *udvm, uint8_t reasonCode, tcomp_buffer_handle_t* lpDetails, int16_t memory_address_of_instruction)
{
    uint32_t mem_add_instruction;
    int ret;

    if(!udvm) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tcomp_buffer_allocBuff(udvm->lpResult->nack_info, INDEX_NACK_SHA1 + TSK_SHA1_DIGEST_SIZE);
    mem_add_instruction = (memory_address_of_instruction >=0) ? memory_address_of_instruction : udvm->last_memory_address_of_instruction;

    if((ret = tcomp_nackinfo_write_2(udvm->lpResult->nack_info,
                                     reasonCode,
                                     *TCOMP_UDVM_GET_BUFFER_AT(mem_add_instruction),
                                     mem_add_instruction,
                                     udvm->sigCompMessage,
                                     lpDetails,
                                     TCOMP_UDVM_GET_SIZE(),
                                     udvm->stateHandler->sigcomp_parameters->cpbValue)) == 0) {
        udvm->lpResult->isNack = 1;
    }

    return ret;
}