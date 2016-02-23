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

/**@file tsk_base64.h
 * @brief Base64 encoder and decoder as per RFC 4648.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef TINYSAK_BASE64_H
#define TINYSAK_BASE64_H

#include "tinysak_config.h"

TSK_BEGIN_DECLS

/**@ingroup tsk_base64_group
* Guess the output(encoded) size.
* @param IN_LEN The input size.
*/
#define TSK_BASE64_ENCODE_LEN(IN_LEN)		((2 + (IN_LEN) - (((IN_LEN) + 2) % 3)) * 4 / 3)
/**@ingroup tsk_base64_group
* Guess the output(decoded) size.
* @param IN_LEN The input size.
*/
#define TSK_BASE64_DECODE_LEN(IN_LEN)		(((IN_LEN * 3)/4) + 2)

TINYSAK_API tsk_size_t tsk_base64_encode(const uint8_t* input, tsk_size_t input_size, char **output);
TINYSAK_API tsk_size_t tsk_base64_decode(const uint8_t* input, tsk_size_t input_size, char **output);

TSK_END_DECLS

#endif /* TINYSAK_BASE64_H */
