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

/**@file tcomp_nack_codes.h
 * @brief List of NACK codes as per RFC 4077 subclause 3.2.
 *
 */

#ifndef TCOMP_NACKCODES_H
#define TCOMP_NACKCODES_H

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
#define INDEX_NACK_HEADER		0
#define INDEX_NACK_VERSION		(INDEX_NACK_HEADER+ 2)
#define INDEX_NACK_REASON_CODE	(INDEX_NACK_VERSION + 1)
#define INDEX_NACK_OPCODE		(INDEX_NACK_REASON_CODE + 1)
#define INDEX_NACK_PC			(INDEX_NACK_OPCODE + 1)
#define INDEX_NACK_SHA1			(INDEX_NACK_PC + 2)
#define INDEX_NACK_DETAILS		(INDEX_NACK_PC + SHA1HashSize)

/************************************************************************************
* Error								       code							details
*************************************************************************************/
#define NACK_NONE							0				// For internal use only
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


typedef struct tcomp_nack_desc_s
{
	int32_t code;
	const char* desc;
}
tcomp_nack_desc_t;

static const tcomp_nack_desc_t TCOMP_NACK_DESCRIPTIONS[] =
{
	{ NACK_NONE, "NONE" },
	{ NACK_STATE_NOT_FOUND, "STATE_NOT_FOUND" },
	{ NACK_CYCLES_EXHAUSTED, "CYCLES_EXHAUSTED" },
	{ NACK_USER_REQUESTED, "USER_REQUESTED" },
	{ NACK_SEGFAULT, "SEGFAULT" },
	{ NACK_TOO_MANY_STATE_REQUESTS, "TOO_MANY_STATE_REQUESTS" },
	{ NACK_INVALID_STATE_ID_LENGTH, "INVALID_STATE_ID_LENGTH" },
	{ NACK_INVALID_STATE_PRIORITY, "INVALID_STATE_PRIORITY" },
	{ NACK_OUTPUT_OVERFLOW, "OUTPUT_OVERFLOW" },
	{ NACK_STACK_UNDERFLOW, "STACK_UNDERFLOW" },
	{ NACK_BAD_INPUT_BITORDER, "BAD_INPUT_BITORDER" },
	{ NACK_DIV_BY_ZERO, "DIV_BY_ZERO" },
	{ NACK_SWITCH_VALUE_TOO_HIGH, "SWITCH_VALUE_TOO_HIGH" },
	{ NACK_TOO_MANY_BITS_REQUESTED, "TOO_MANY_BITS_REQUESTED" },
	{ NACK_INVALID_OPERAND, "INVALID_OPERAND" },
	{ NACK_HUFFMAN_NO_MATCH, "HUFFMAN_NO_MATCH" },
	{ NACK_MESSAGE_TOO_SHORT, "MESSAGE_TOO_SHORT" },
	{ NACK_INVALID_CODE_LOCATION, "INVALID_CODE_LOCATION" },
	{ NACK_BYTECODES_TOO_LARGE, "BYTECODES_TOO_LARGE" },
	{ NACK_INVALID_OPCODE, "INVALID_OPCODE" },
	{ NACK_INVALID_STATE_PROBE, "INVALID_STATE_PROBE" },
	{ NACK_ID_NOT_UNIQUE, "ID_NOT_UNIQUE" },
	{ NACK_MULTILOAD_OVERWRITTEN, "MULTILOAD_OVERWRITTEN" },
	{ NACK_STATE_TOO_SHORT, "STATE_TOO_SHORT" },
	{ NACK_INTERNAL_ERROR, "INTERNAL_ERROR" },
	{ NACK_FRAMING_ERROR, "FRAMING_ERROR" }
};

#endif /* TCOMP_NACKCODES_H */
