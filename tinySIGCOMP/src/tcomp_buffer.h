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

/**@file tcomp_buffer.h
 * @brief SigComp Buffer
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *

 */
#ifndef TCOMP_BUFFER_H
#define TCOMP_BUFFER_H

#include "tinysigcomp_config.h"

#include "tsk_object.h"


TCOMP_BEGIN_DECLS

#define TCOMP_P_BIT_MSB_TO_LSB 0
#define TCOMP_P_BIT_LSB_TO_MSB 1

/**Sigcomp Buffer handle
*/
typedef void tcomp_buffer_handle_t;

tcomp_buffer_handle_t* tcomp_buffer_create(const void* data, tsk_size_t len);
tcomp_buffer_handle_t* tcomp_buffer_create_null();

tsk_bool_t tcomp_buffer_equals(const tcomp_buffer_handle_t* handle1, const tcomp_buffer_handle_t* handle2);
tsk_bool_t tcomp_buffer_startsWith(const tcomp_buffer_handle_t* handle1, const tcomp_buffer_handle_t* handle2) /*const*/;

const uint8_t* tcomp_buffer_getReadOnlyBufferAtPos(const tcomp_buffer_handle_t* handle, tsk_size_t position) /*const*/;
#define tcomp_buffer_getReadOnlyBuffer(buffer) tcomp_buffer_getReadOnlyBufferAtPos(buffer, 0)

TINYSIGCOMP_API uint8_t* tcomp_buffer_getBufferAtPos(const tcomp_buffer_handle_t* handle, tsk_size_t position);
#define tcomp_buffer_getBuffer(handle) tcomp_buffer_getBufferAtPos(handle, 0)

TINYSIGCOMP_API tsk_size_t tcomp_buffer_getSize(const tcomp_buffer_handle_t* handle) /*const*/;
tsk_size_t tcomp_buffer_getRemainingBits(const tcomp_buffer_handle_t* handle) /*const*/;

uint8_t* tcomp_buffer_readBytes(tcomp_buffer_handle_t* handle, tsk_size_t size);
uint32_t tcomp_buffer_readLsbToMsb(tcomp_buffer_handle_t* handle, tsk_size_t length);
uint32_t tcomp_buffer_readMsbToLsb(tcomp_buffer_handle_t* handle, tsk_size_t length);
void tcomp_buffer_discardBits(tcomp_buffer_handle_t* handle);
void tcomp_buffer_discardLastBytes(tcomp_buffer_handle_t* handle, uint32_t count);

void tcomp_buffer_allocBuff(tcomp_buffer_handle_t* handle, tsk_size_t size);
void tcomp_buffer_referenceBuff(tcomp_buffer_handle_t* handle, uint8_t* externalBuff, tsk_size_t size);
tsk_bool_t tcomp_buffer_appendBuff(tcomp_buffer_handle_t* handle, const void* data, tsk_size_t size);
tsk_bool_t tcomp_buffer_removeBuff(tcomp_buffer_handle_t* handle, tsk_size_t pos, tsk_size_t size);
void tcomp_buffer_freeBuff(tcomp_buffer_handle_t* handle);

tsk_size_t* tcomp_buffer_getIndexBytes(const tcomp_buffer_handle_t* handle);
tsk_size_t* tcomp_buffer_getIndexBits(const tcomp_buffer_handle_t* handle);

uint8_t* tcomp_buffer_getP_BIT(const tcomp_buffer_handle_t* handle);

uint64_t tcomp_buffer_createHash(const void *data, tsk_size_t len);

void tcomp_buffer_nprint(const tcomp_buffer_handle_t* handle, tsk_ssize_t size);
#define tcomp_buffer_print(handle) tcomp_buffer_nprint(handle, -1)

void tcomp_buffer_reset(tcomp_buffer_handle_t* handle);

TINYSIGCOMP_GEXTERN const tsk_object_def_t *tcomp_buffer_def_t;

TCOMP_END_DECLS

#endif /* TCOMP_BUFFER_H */
