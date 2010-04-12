/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango.org>
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

/**@file tcomp_instructions.h
 * @brief List of all supported UDVM instructions as per RFC 3320 subclause 9.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */

#ifndef TCOMP_INSTRUCTIONS_H
#define TCOMP_INSTRUCTIONS_H

/************************************************************************************
* Instruction								Bytecode value		Cost in UDVM cycles	*
*************************************************************************************/
#define TCOMP_UDVM_INST__DECOMPRESSION_FAILURE		0					//1
#define TCOMP_UDVM_INST__AND						1					//1
#define TCOMP_UDVM_INST__OR							2					//1
#define TCOMP_UDVM_INST__NOT						3					//1
#define TCOMP_UDVM_INST__LSHIFT						4					//1
#define TCOMP_UDVM_INST__RSHIFT						5					//1
#define TCOMP_UDVM_INST__ADD						6					//1
#define TCOMP_UDVM_INST__SUBTRACT					7					//1
#define TCOMP_UDVM_INST__MULTIPLY					8					//1
#define TCOMP_UDVM_INST__DIVIDE						9					//1
#define TCOMP_UDVM_INST__REMAINDER					10					//1
#define TCOMP_UDVM_INST__SORT_ASCENDING				11					//1 + k * (ceiling(log2(k)) + n)
#define TCOMP_UDVM_INST__SORT_DESCENDING			12					//1 + k * (ceiling(log2(k)) + n)
#define TCOMP_UDVM_INST__SHA_1						13					//1 + length
#define TCOMP_UDVM_INST__LOAD						14					//1
#define TCOMP_UDVM_INST__MULTILOAD					15					//1 + n
#define TCOMP_UDVM_INST__PUSH						16					//1
#define TCOMP_UDVM_INST__POP						17					//1
#define TCOMP_UDVM_INST__COPY						18					//1 + length
#define TCOMP_UDVM_INST__COPY_LITERAL				19					//1 + length
#define TCOMP_UDVM_INST__COPY_OFFSET				20					//1 + length
#define TCOMP_UDVM_INST__MEMSET						21					//1 + length
#define TCOMP_UDVM_INST__JUMP						22					//1
#define TCOMP_UDVM_INST__COMPARE					23					//1
#define TCOMP_UDVM_INST__CALL						24					//1
#define TCOMP_UDVM_INST__RETURN						25					//1
#define TCOMP_UDVM_INST__SWITCH						26					//1 + n
#define TCOMP_UDVM_INST__CRC						27					//1 + length
#define TCOMP_UDVM_INST__INPUT_BYTES				28					//1 + length
#define TCOMP_UDVM_INST__INPUT_BITS					29					//1
#define TCOMP_UDVM_INST__INPUT_HUFFMAN				30					//1 + n
#define TCOMP_UDVM_INST__STATE_ACCESS				31					//1 + state_length
#define TCOMP_UDVM_INST__STATE_CREATE				32					//1 + state_length
#define TCOMP_UDVM_INST__STATE_FREE					33					//1
#define TCOMP_UDVM_INST__OUTPUT						34					//1 + output_length
#define TCOMP_UDVM_INST__END_MESSAGE				35					//1 + state_length

#endif /* TCOMP_INSTRUCTIONS_H */
