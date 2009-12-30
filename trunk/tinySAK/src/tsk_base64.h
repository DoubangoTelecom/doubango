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

/**@file tsk_base64.h
 * @brief Base64 encoder and decoder as per RFC 4648.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYSAK_BASE64_H
#define TINYSAK_BASE64_H

#include "tinySAK_config.h"

#define TSK_BASE64_ENCODE_LEN(IN_LEN)		((2 + (IN_LEN) - (((IN_LEN) + 2) % 3)) * 4 / 3)
#define TSK_BASE64_DECODE_LEN(IN_LEN)		(((IN_LEN * 3)/4) + 2)

TINYSAK_API size_t tsk_base64_encode(const uint8_t* input, size_t input_size, char **output);
TINYSAK_API size_t tsk_base64_decode(const uint8_t* input, size_t input_size, char **output);

#endif /* TINYSAK_BASE64_H */