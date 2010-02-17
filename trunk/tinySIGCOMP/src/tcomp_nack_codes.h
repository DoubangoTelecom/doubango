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
* GNU General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

/**@file tcomp_nack_codes.h
 * @brief List of NACK codes as per RFC 4077 subclause 3.2.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */

#ifndef TCOMP_NACKCODES_H
#define TCOMP_NACKCODES_H

/************************************************************************************
* Error								       code							details
*************************************************************************************/
#define NACK_STATE_NOT_FOUND				1				// State ID (6 - 20 bytes)
#define NACK_CYCLES_EXHAUSTED			    2				// Cycles Per Bit (1 byte)
#define NACK_USER_REQUESTED					3
#define NACK_SEGFAULT						4
#define NACK_TOO_MANY_STATE_REQUESTS		5
#define NACK_INVALID_STATE_ID_LENGTH		6
#define NACK_INVALID_STATE_PRIORITY			7
#define NACK_OUTPUT_OVERFLOW				8
#define NACK_STACK_UNDERFLOW				9
#define NACK_BAD_INPUT_BITORDER				10
#define NACK_DIV_BY_ZERO					11
#define NACK_SWITCH_VALUE_TOO_HIGH			12
#define NACK_TOO_MANY_BITS_REQUESTED		13
#define NACK_INVALID_OPERAND				14
#define NACK_HUFFMAN_NO_MATCH				15
#define NACK_MESSAGE_TOO_SHORT				16
#define NACK_INVALID_CODE_LOCATION			17
#define NACK_BYTECODES_TOO_LARGE			18				// Memory size (2 bytes)
#define NACK_INVALID_OPCODE					19
#define NACK_INVALID_STATE_PROBE			20
#define NACK_ID_NOT_UNIQUE					21				// State ID (6 - 20 bytes)
#define NACK_MULTILOAD_OVERWRITTEN			22
#define NACK_STATE_TOO_SHORT				23				// State ID (6 - 20 bytes)
#define NACK_INTERNAL_ERROR					24
#define NACK_FRAMING_ERROR					25

#endif /* TCOMP_NACKCODES_H */
