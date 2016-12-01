/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou [at) doubango (DOT) org>
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

/**@file tsms_packing.h
 * @brief SMS Packing (3GPP TS 23.038 subclause 6.1.2).
 *
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 *

 */
#ifndef TINYSMS_TSMS_PACKING_H
#define TINYSMS_TSMS_PACKING_H

#include "tinysms_config.h"

#include "tsk_buffer.h"

TSMS_BEGIN_DECLS

TINYSMS_API tsk_buffer_t* tsms_pack_to_7bit(const char* ascii);
TINYSMS_API tsk_buffer_t* tsms_pack_to_ucs2(const char* ascii);
TINYSMS_API tsk_buffer_t* tsms_pack_to_8bit(const char* ascii);

TINYSMS_API char* tsms_pack_from_7bit(const void* gsm7bit, tsk_size_t size);
TINYSMS_API char* tsms_pack_from_ucs2(const void* ucs2, tsk_size_t size);
TINYSMS_API char* tsms_pack_from_8bit(const void* gsm8bit, tsk_size_t size);

TSMS_END_DECLS

#endif /* TINYSMS_TSMS_PACKING_H */
