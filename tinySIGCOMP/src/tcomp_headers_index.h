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

/**@file tcomp_headers_index.h
 * @brief SigComp headers index.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *

 */

#ifndef TCOMP_UDVM_HEADERS_INDEX_H
#define TCOMP_UDVM_HEADERS_INDEX_H

/*
 0             7 8            15
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |       UDVM_memory_size        |  0 - 1
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |        cycles_per_bit         |  2 - 3
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |        SigComp_version        |  4 - 5
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |    partial_state_ID_length    |  6 - 7
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |         state_length          |  8 - 9
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |                               |
 :           reserved            :  10 - 31
 |                               |
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
#define TCOMP_UDVM_HEADER_UDVM_MEMORY_SIZE_INDEX				0
#define TCOMP_UDVM_HEADER_CYCLES_PER_BIT_INDEX				2
#define TCOMP_UDVM_HEADER_SIGCOMP_VERSION_INDEX				4
#define TCOMP_UDVM_HEADER_PARTIAL_STATE_ID_LENGTH_INDEX		6
#define TCOMP_UDVM_HEADER_STATE_LENGTH_INDEX					8
#define TCOMP_UDVM_HEADER_RESERVED_INDEX						10


/*
0             7 8            15
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|        byte_copy_left         |  64 - 65
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|        byte_copy_right        |  66 - 67
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|        input_bit_order        |  68 - 69
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|        stack_location         |  70 - 71
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

0             7 8            15
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|         reserved        |F|H|P|  68 - 69
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*/
#define TCOMP_UDVM_HEADER_BYTE_COPY_LEFT_INDEX				64
#define TCOMP_UDVM_HEADER_BYTE_COPY_RIGHT_INDEX				66
#define TCOMP_UDVM_HEADER_INPUT_BIT_ORDER_INDEX				68
#define TCOMP_UDVM_HEADER_STACK_LOCATION_INDEX				70

#endif /* TCOMP_UDVM_HEADERS_INDEX_H */
