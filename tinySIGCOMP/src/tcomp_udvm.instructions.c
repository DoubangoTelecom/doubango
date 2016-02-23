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

/**@file tcomp_udvm.instructions.c
 * @brief  SigComp UDVM machine (Instructions).
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *

 */
#include "tcomp_udvm.h"

#include "tsk_memory.h"
#include "tsk_debug.h"
#include "tsk_ppfcs16.h"

#include <string.h> /* memcpy */
#include <stdlib.h> /* qsort */
#include <math.h>	/* ceil, log ... */

/*
* IMPORTANT: MSBs are stored before LSBs in the UDVM memory --> BIG ENDIAN
*/

#define F_BIT_MSB_TO_LSB 0
#define F_BIT_LSB_TO_MSB 1
#define H_BIT_MSB_TO_LSB F_BIT_MSB_TO_LSB
#define H_BIT_LSB_TO_MSB F_BIT_LSB_TO_MSB

#define CEILLINGLOG2(x) ceil( (log((double)x)/log(2.0)) )

/*
*	Consume cycles
*/
#define CONSUME_CYCLES(cycles)										\
	udvm->consumed_cycles += (uint64_t)(cycles);					\
	if( udvm->consumed_cycles > udvm->maximum_UDVM_cycles )			\
	{																\
		TSK_DEBUG_ERROR("%s (%llu > %llu)", TCOMP_NACK_DESCRIPTIONS[NACK_CYCLES_EXHAUSTED].desc, udvm->consumed_cycles, udvm->maximum_UDVM_cycles);					\
		tcomp_udvm_createNackInfo2(udvm, NACK_CYCLES_EXHAUSTED);	\
		return tsk_false;													\
	}

#define SET_2BYTES_VAL(position, value) \
	if(((position) + 1) >= TCOMP_UDVM_GET_SIZE()) \
	{ \
		TSK_DEBUG_ERROR("%s (%u > %u)", TCOMP_NACK_DESCRIPTIONS[NACK_SEGFAULT].desc, ((position) + 1), TCOMP_UDVM_GET_SIZE()); \
		tcomp_udvm_createNackInfo2(udvm, NACK_SEGFAULT);	\
		return tsk_false;	\
	}\
	TCOMP_UDVM_SET_2BYTES_VAL(position, value);\
 
#define GET_2BYTES_VAL(position, ret_val) \
	if(((position) + 1) >= TCOMP_UDVM_GET_SIZE()) \
	{ \
		TSK_DEBUG_ERROR("%s (%u > %u)", TCOMP_NACK_DESCRIPTIONS[NACK_SEGFAULT].desc, ((position) + 1), TCOMP_UDVM_GET_SIZE()); \
		tcomp_udvm_createNackInfo2(udvm, NACK_SEGFAULT);	\
		return tsk_false;	\
	}\
	ret_val = TCOMP_UDVM_GET_2BYTES_VAL((position));


/**
	This structure is used to keep index-value pairs after sorting.
*/
typedef struct IndexValuePair_s {
    uint16_t index;
    uint16_t value;
}
IndexValuePair_t;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	Predicate to sort integers in ascending order.
///
/// @param [in,out]	a	First integer.
/// @param [in,out]	b	Second integer.
///
/// @retval	Zero if @a a == @a b; negative if @a a < @a b and positive otherwise..
////////////////////////////////////////////////////////////////////////////////////////////////////

static int SortAscendingPredicate(const void *a, const void *b)
{
    const IndexValuePair_t *el1 = a;
    const IndexValuePair_t *el2 = b;

    /* If values are  equal the original ordering of the integers must be preserved
    * ==> We cannot use normal comparaison because the ANSI C implementation of qsort could swap values even if they are equal.
    */
    return (el2->value == el1->value) ? (el1->index - el2->index) : (el1->value - el2->value);
}

/**
* Sort Descending predicate.
*/

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief	Predicate to sort integers in descending order.
/// @param [in,out]	a	First integer.
/// @param [in,out]	b	Second integer.
///
/// @retval	Zero if @a a == @a b; negative if @a a > @a b and positive otherwise.
////////////////////////////////////////////////////////////////////////////////////////////////////
static int SortDescendingPredicate(const void *a, const void *b)
{
    const IndexValuePair_t *el1 = a;
    const IndexValuePair_t *el2 = b;

    /* If values are  equal the original ordering of the integers must be preserved.
    * ==> We cannot use normal comparaison because the ANSI C implementation of qsort could swap values even if they are equal.
    */
    return (el2->value == el1->value) ? (el1->index - el2->index) : (el2->value - el1->value);
};


////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/// @brief	DECOMPRESSION-FAILURE
/// Reference:  RFC3320 Section 9.4.1
/// This instruction triggers a manual decompression failure.  This is useful if the UDVM bytecode discovers that it
/// cannot successfully decompress the message (e.g., by using the CRC instruction).


/// @param [in,out]	udvm	The udvm state machine entity.
///
/// @retval	@a tsk_true if succeed, otherwise returns @a tsk_false.
////////////////////////////////////////////////////////////////////////////////////////////////////
tsk_bool_t TCOMP_UDVM_EXEC_INST__DECOMPRESSION_FAILURE(tcomp_udvm_t *udvm)
{
    TSK_DEBUG_ERROR("%s", TCOMP_NACK_DESCRIPTIONS[NACK_USER_REQUESTED].desc);
    tcomp_udvm_createNackInfo2(udvm, NACK_USER_REQUESTED);
    return tsk_false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/// @brief	AND ($operand_1, %operand_2)
/// Reference:  RFC3320 Section 9.1.1
/// Formula: [operand_1 := operand_1 & operand_2].

/// @param [in,out]	udvm	The udvm state machine entity.
/// @param	operand_1		2-byte value encoded by the operand. After the operation is complete, the 2-byte word at the memory address specified by
///   this operand is overwritten with the result.
/// @param	operand_2		The second operand.
///
/// @retval	@a tsk_true if succeed, otherwise returns @a tsk_false.
////////////////////////////////////////////////////////////////////////////////////////////////////

tsk_bool_t TCOMP_UDVM_EXEC_INST__AND(tcomp_udvm_t *udvm, uint32_t operand_1, uint32_t operand_2)
{
    uint16_t _2bytes;

    CONSUME_CYCLES(1);

    GET_2BYTES_VAL(operand_1, _2bytes);
    SET_2BYTES_VAL( operand_1, (_2bytes & operand_2) );

    return tsk_true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/// @brief	OR ($operand_1, %operand_2)
/// Reference:  RFC3320 Section 9.1.1
/// Formula: [operand_1 := operand_1 | operand_2].

/// @param [in,out]	udvm	The udvm state machine entity.
/// @param	operand_1		2-byte value encoded by the operand. After the operation is complete, the 2-byte word at the memory address specified by
///    this operand is overwritten with the result.
/// @param	operand_2		The second operand.
///
/// @retval	@a tsk_true if succeed, otherwise returns @a tsk_false.
////////////////////////////////////////////////////////////////////////////////////////////////////

tsk_bool_t TCOMP_UDVM_EXEC_INST__OR(tcomp_udvm_t *udvm, uint32_t operand_1, uint32_t operand_2)
{
    uint16_t _2bytes;

    CONSUME_CYCLES(1);

    GET_2BYTES_VAL(operand_1, _2bytes);
    SET_2BYTES_VAL( operand_1, (_2bytes | operand_2) );

    return tsk_true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/// @brief	<i>NOT ($operand_1)</i><br><br>
///  Reference:  RFC3320 Section 9.1.1<br>
///  Formula: [operand_1 := ~operand_1]. <br>

/// @param [in,out]	udvm	The udvm state machine entity.
/// @param	operand_1		2-byte value encoded by the operand. After the operation is complete, the 2-byte word at the memory address specified by
///   this operand is overwritten with the result.
///
/// @retval	@a tsk_true if succeed, otherwise returns @a tsk_false.
////////////////////////////////////////////////////////////////////////////////////////////////////

tsk_bool_t TCOMP_UDVM_EXEC_INST__NOT(tcomp_udvm_t *udvm, uint32_t operand_1)
{
    uint16_t _2bytes;

    CONSUME_CYCLES(1);

    GET_2BYTES_VAL(operand_1, _2bytes);
    SET_2BYTES_VAL( operand_1, ~( _2bytes ) );

    return tsk_true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/// @brief	<i>LSHIFT ($operand_1, %operand_2)</i><br><br>
/// Reference:  RFC3320 Section 9.1.1<br>
/// Formula: [LSHIFT (m, n) := m * 2^n (modulo 2^16)]. <br>

/// @param [in,out]	udvm	The udvm state machine entity.
/// @param	operand_1		2-byte value encoded by the operand. After the operation is complete, the 2-byte word at the memory address specified by
///  this operand is overwritten with the result.
/// @param	operand_2		2-byte value encoded by the operand.
///
/// @retval	@a tsk_true if succeed, otherwise returns @a tsk_false.
////////////////////////////////////////////////////////////////////////////////////////////////////

tsk_bool_t TCOMP_UDVM_EXEC_INST__LSHIFT(tcomp_udvm_t *udvm, uint32_t operand_1, uint32_t operand_2)
{
    uint16_t _2bytes;

    CONSUME_CYCLES(1);

    // (m * 2^n)  == (m<<n)
    // (2^16) === 65536

    GET_2BYTES_VAL(operand_1, _2bytes);
    SET_2BYTES_VAL( operand_1, (_2bytes << operand_2) );

    return tsk_true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/// @brief	<i>RSHIFT ($operand_1, %operand_2)</i><br><br>
/// Reference:  RFC3320 Section 9.1.1<br>
/// Formula: [RSHIFT (m, n) := floor(m / 2^n)]. <br>

/// @param [in,out]	udvm	The udvm state machine entity.
/// @param	operand_1		2-byte value encoded by the operand. After the operation is complete, the 2-byte word at the memory address specified by
//     this operand is overwritten with the result.
/// @param	operand_2		2-byte value encoded by the operand.
///
/// @retval	@a tsk_true if succeed, otherwise returns @a tsk_false.
////////////////////////////////////////////////////////////////////////////////////////////////////

tsk_bool_t TCOMP_UDVM_EXEC_INST__RSHIFT(tcomp_udvm_t *udvm, uint32_t operand_1, uint32_t operand_2)
{
    uint16_t _2bytes;

    CONSUME_CYCLES(1);

    // floor(m / 2^n) == (m>>n)
    GET_2BYTES_VAL(operand_1, _2bytes);
    SET_2BYTES_VAL(operand_1, (_2bytes >> operand_2) );

    return tsk_true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/// @brief	<i>ADD ($operand_1, %operand_2)</i><br><br>
/// Reference:  RFC3320 Section 9.1.2<br>
/// Formula: [ADD (m, n) := m + n (modulo 2^16)]<br>
///
///
/// @param [in,out]	udvm	The udvm state machine entity.
/// @param	operand_1		2-byte value encoded by the operand. After the operation is complete, the 2-byte word at the memory address specified by
///   this operand is overwritten with the result.
/// @param	operand_2		2-byte value encoded by the operand.
///
/// @retval	True if succeed, otherwise return false.
////////////////////////////////////////////////////////////////////////////////////////////////////
tsk_bool_t TCOMP_UDVM_EXEC_INST__ADD(tcomp_udvm_t *udvm, uint32_t operand_1, uint32_t operand_2)
{
    uint16_t _2bytes;

    CONSUME_CYCLES(1);

    GET_2BYTES_VAL(operand_1, _2bytes);
    SET_2BYTES_VAL(operand_1, (_2bytes + operand_2) );

    return tsk_true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/// @brief	<i>SUBTRACT ($operand_1, %operand_2)</i><br><br>
/// Reference:  RFC3320 Section 9.1.2<br>
/// Formula: [SUBTRACT (m, n)  := m - n (modulo 2^16)]<br>
///
/// @param [in,out]	udvm	The udvm state machine entity.
/// @param	operand_1		2-byte value encoded by the operand. After the operation is complete, the 2-byte word at the memory address specified by
///   this operand is overwritten with the result.
/// @param	operand_2		2-byte value encoded by the operand.
///
/// @retval	1 if succeed, otherwise returns 0.
////////////////////////////////////////////////////////////////////////////////////////////////////
tsk_bool_t TCOMP_UDVM_EXEC_INST__SUBTRACT(tcomp_udvm_t *udvm, uint32_t operand_1, uint32_t operand_2)
{
    uint16_t _2bytes;

    CONSUME_CYCLES(1);

    GET_2BYTES_VAL(operand_1, _2bytes);
    SET_2BYTES_VAL(operand_1, (_2bytes - operand_2) );

    return tsk_true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/// @brief	<i>MULTIPLY ($operand_1, %operand_2)</i><br><br>
/// Reference:  RFC3320 Section 9.1.2<br>
/// Formula: [MULTIPLY (m, n)  := m * n (modulo 2^16)]<br>
///
/// @param [in,out]	udvm	The udvm state machine entity.
/// @param	operand_1		2-byte value encoded by the operand. After the operation is complete, the 2-byte word at the memory address specified by
///  this operand is overwritten with the result.
/// @param	operand_2		2-byte value encoded by the operand.
///
/// @retval	@a tsk_true if succeed, otherwise returns @a tsk_false.
////////////////////////////////////////////////////////////////////////////////////////////////////
tsk_bool_t TCOMP_UDVM_EXEC_INST__MULTIPLY(tcomp_udvm_t *udvm, uint32_t operand_1, uint32_t operand_2)
{
    uint16_t _2bytes;

    CONSUME_CYCLES(1);

    GET_2BYTES_VAL(operand_1, _2bytes);
    SET_2BYTES_VAL(operand_1, (_2bytes * operand_2) );

    return tsk_true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/// @brief	<i>DIVIDE ($operand_1, %operand_2)</i><br><br>
/// Reference:  RFC3320 Section 9.1.2<br>
/// Formula: [DIVIDE (m, n) := floor(m / n)]<br>
///
///
/// @param [in,out]	udvm	The udvm state machine entity.
/// @param	operand_1		2-byte value encoded by the operand. After the operation is complete, the 2-byte word at the memory address specified by
///   this operand is overwritten with the result.
/// @param	operand_2		2-byte value encoded by the operand. Decompression failure occurs if this operand is zero.
///
/// @retval	@a tsk_true if succeed, otherwise returns @a tsk_false.
////////////////////////////////////////////////////////////////////////////////////////////////////
tsk_bool_t TCOMP_UDVM_EXEC_INST__DIVIDE(tcomp_udvm_t *udvm, uint32_t operand_1, uint32_t operand_2)
{
    uint16_t _2bytes;

    CONSUME_CYCLES(1);

    if(!operand_2) {
        TSK_DEBUG_ERROR("%s", TCOMP_NACK_DESCRIPTIONS[NACK_DIV_BY_ZERO].desc);
        tcomp_udvm_createNackInfo2(udvm, NACK_DIV_BY_ZERO);
        return tsk_false;
    }

    GET_2BYTES_VAL(operand_1, _2bytes);
    SET_2BYTES_VAL(operand_1, (_2bytes / operand_2) );

    return tsk_true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/// @brief	<i>REMAINDER ($operand_1, %operand_2)</i><br><br>
/// Reference:  RFC3320 Section 9.1.2<br>
/// Formula: [REMAINDER (m, n) := m - n * floor(m / n)]<br>
///
///
/// @param [in,out]	udvm	The udvm state machine entity.
/// @param	operand_1		2-byte value encoded by the operand. After the operation is complete, the 2-byte word at the memory address specified by
///   this operand is overwritten with the result.
/// @param	operand_2		2-byte value encoded by the operand. Decompression failure occurs if this operand is zero.
///
/// @retval	@a tsk_true if succeed, otherwise returns @a tsk_false.
////////////////////////////////////////////////////////////////////////////////////////////////////
tsk_bool_t TCOMP_UDVM_EXEC_INST__REMAINDER(tcomp_udvm_t *udvm, uint32_t operand_1, uint32_t operand_2)
{
    uint16_t _2bytes;
    CONSUME_CYCLES(1);

    if(!operand_2) {
        TSK_DEBUG_ERROR("%s", TCOMP_NACK_DESCRIPTIONS[NACK_DIV_BY_ZERO].desc);
        tcomp_udvm_createNackInfo2(udvm, NACK_DIV_BY_ZERO);
        return tsk_false;
    }

    GET_2BYTES_VAL(operand_1, _2bytes);
    SET_2BYTES_VAL(operand_1, (_2bytes % operand_2) );

    return tsk_true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/// @brief	<i>SORT-ASCENDING (%start, %n, %k)</i><br><br>
/// Reference:  RFC3320 Section 9.1.3<br>
///
/// This instruction sort lists of 2-byte words in ascending order.
///
/// @param [in,out]	udvm	The udvm state machine entity.
/// @param	start			The starting memory address of the block of data to be sorted.
/// @param	n				Number of lists.
/// @param	k				Lists length (2-byte words).
///
/// @retval	True if succeed, otherwise return false.
////////////////////////////////////////////////////////////////////////////////////////////////////
tsk_bool_t TCOMP_UDVM_EXEC_INST__SORT_ASCENDING(tcomp_udvm_t *udvm, uint32_t start, uint32_t n, uint32_t k)
{
    tsk_bool_t segfault = tsk_false;
    uint16_t* list_temp = tsk_null;
    IndexValuePair_t *list1_values = tsk_null;
    uint32_t list_index, list_el;
    uint32_t j, pos;

    CONSUME_CYCLES(( 1 + k *(CEILLINGLOG2(k) + n) )); /* 1 + k * (ceiling(log2(k)) + n) */

    if(TCOMP_UDVM_GET_SIZE() <= (tsk_size_t)(start+(n*k*2)) ) {
        segfault = tsk_true;
        goto __SEGFAULT;
    };

    //
    // Create FirstList with key-value pairs
    //
    list1_values = (IndexValuePair_t*)tsk_calloc(k, sizeof(IndexValuePair_t));
    if(!list1_values) {
        segfault = tsk_true;
        goto __SEGFAULT;
    };
    for(j=0, pos=0; pos<k; j+=2,pos++) {
        list1_values[pos].index = pos;
        GET_2BYTES_VAL((start+j), list1_values[pos].value);
    }

    /*
    * Sort Fisrt List Values
    */
    qsort(list1_values, k, sizeof(IndexValuePair_t), SortAscendingPredicate);

    /* Sort all lists */
    list_temp = tsk_calloc(k, sizeof(uint32_t));
    if(!list1_values) {
        segfault = tsk_true;
        goto __SEGFAULT;
    };
    for(list_index = 0; list_index < n; list_index++) {
        uint16_t* list_start = (uint16_t*)TCOMP_UDVM_GET_BUFFER_AT( start + (list_index*k*2) );
        memcpy(list_temp, list_start, k*2);
        for(list_el=0; list_el<k; list_el++) {
            list_start[(list_el)] = list_temp[ list1_values[list_el].index ];
        }
    }

__SEGFAULT:
    TSK_FREE(list_temp);
    TSK_FREE(list1_values);

    if(segfault) {
        TSK_DEBUG_ERROR("%s", TCOMP_NACK_DESCRIPTIONS[NACK_SEGFAULT].desc);
        tcomp_udvm_createNackInfo2(udvm, NACK_SEGFAULT);
        return tsk_false;
    }

    return tsk_true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/// @brief	<i>SORT-DESCENDING (%start, %n, %k)</i><br><br>
/// Reference:  RFC3320 Section 9.1.3<br>
///
/// This instruction sort lists of 2-byte words in descending order.
///
/// @param [in,out]	udvm	The udvm state machine entity.
/// @param	start			The starting memory address of the block of data to be sorted.
/// @param	n				Number of lists.
/// @param	k				Lists length (2-byte words).
///
/// @retval	True if succeed, otherwise return false.
////////////////////////////////////////////////////////////////////////////////////////////////////
tsk_bool_t TCOMP_UDVM_EXEC_INST__SORT_DESCENDING(tcomp_udvm_t *udvm, uint32_t start, uint32_t n, uint32_t k)
{
    tsk_bool_t segfault = tsk_false;
    uint16_t* list_temp = tsk_null;
    IndexValuePair_t *list1_values = tsk_null;
    uint32_t list_index, list_el;
    uint32_t j, pos;

    CONSUME_CYCLES(( 1 + k *(CEILLINGLOG2(k) + n) )); // 1 + k * (ceiling(log2(k)) + n)

    if(TCOMP_UDVM_GET_SIZE() <= (tsk_size_t)(start+(n*k*2)) ) {
        segfault = tsk_true;
        goto __SEGFAULT;
    };

    //
    // Create FirstList with key-value pairs.
    //
    list1_values = (IndexValuePair_t*)tsk_calloc(k, sizeof(IndexValuePair_t));
    if(!list1_values) {
        segfault = tsk_true;
        goto __SEGFAULT;
    };
    for(j=0, pos=0; pos<k; j+=2,pos++) {
        list1_values[pos].index = pos;
        GET_2BYTES_VAL((start+j), list1_values[pos].value);
    }

    // Sort Fisrt List Values.
    qsort(list1_values, k, sizeof(IndexValuePair_t), SortDescendingPredicate);


    // Sort all lists
    list_temp = tsk_calloc(k, sizeof(uint16_t));
    if(!list1_values) {
        segfault = tsk_true;
        goto __SEGFAULT;
    };
    for(list_index = 0; list_index < n; list_index++) {
        uint16_t* list_start = (uint16_t*)TCOMP_UDVM_GET_BUFFER_AT(start + (list_index*k*2));
        memcpy(list_temp, list_start, k*2);
        for(list_el=0; list_el<k; list_el++) {
            list_start[(list_el)] = list_temp[ list1_values[list_el].index ];
        }
    }

__SEGFAULT:
    TSK_FREE(list_temp);
    TSK_FREE(list1_values);

    if(segfault) {
        TSK_DEBUG_ERROR("%s", TCOMP_NACK_DESCRIPTIONS[NACK_SEGFAULT].desc);
        tcomp_udvm_createNackInfo2(udvm, NACK_SEGFAULT);
        return tsk_false;
    }

    return tsk_true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/// @brief	<i>SHA-1 (%position, %length, %destination)</i><br><br>
/// Reference:  RFC3320 Section 9.1.4<br>
/// This instruction calculates a 20-byte SHA-1 hash [RFC-3174] over the specified area of UDVM memory.
///
/// @param [in,out]	udvm	The udvm state machine entity.
/// @param	position		The starting memory address.
/// @param	length			The length of the byte string over which the SHA-1 hash is calculated.
/// @param	destination		The starting address to which the resulting 20-byte hash will be copied.
///
/// @retval	True if succeed, otherwise return false.
////////////////////////////////////////////////////////////////////////////////////////////////////
tsk_bool_t TCOMP_UDVM_EXEC_INST__SHA_1(tcomp_udvm_t *udvm, uint32_t position, uint32_t length, uint32_t destination)
{
    tsk_bool_t ok = tsk_false;
    tsk_sha1context_t sha;
    int32_t err;
    uint8_t Message_Digest[TSK_SHA1_DIGEST_SIZE];

    // only check length
    // (destination + length) could be > sizeof(udvm_memory) as copying is done byte by byte and could wrap
    if(!length) {
        TSK_DEBUG_ERROR("%s", TCOMP_NACK_DESCRIPTIONS[NACK_SEGFAULT].desc);
        tcomp_udvm_createNackInfo2(udvm, NACK_SEGFAULT);
        goto bail;
    }

    CONSUME_CYCLES(1 + length);

    // The SHA-1 instruction calculates a 20-byte SHA-1 hash [RFC-3174] over  the specified area of UDVM memory
    if(udvm->tmp_buff.size < length) {
        if(!(udvm->tmp_buff.ptr = tsk_realloc(udvm->tmp_buff.ptr, length))) {
            udvm->tmp_buff.size = 0;
            goto bail;
        }
        udvm->tmp_buff.size = length;
    }

    if(!(ok = tcomp_udvm_bytecopy_from(udvm, udvm->tmp_buff.ptr, position, length))) {
        goto bail;
    }

    // Compute SHA-1
    if(!(ok = ((err = tsk_sha1reset(&sha)) == 0))) {
        TSK_DEBUG_ERROR("%s: %d", TCOMP_NACK_DESCRIPTIONS[NACK_INTERNAL_ERROR].desc, err);
        tcomp_udvm_createNackInfo2(udvm, NACK_INTERNAL_ERROR);
        goto bail;
    }
    if(!(ok = ((err = tsk_sha1input(&sha, udvm->tmp_buff.ptr, length)) == 0))) {
        TSK_DEBUG_ERROR("%s : %d", TCOMP_NACK_DESCRIPTIONS[NACK_INTERNAL_ERROR].desc, err);
        tcomp_udvm_createNackInfo2(udvm, NACK_INTERNAL_ERROR);
        goto bail;
    }
    if(!(ok = ((err = tsk_sha1result(&sha, (uint8_t*)Message_Digest)) == 0))) {
        TSK_DEBUG_ERROR("%s : %d", TCOMP_NACK_DESCRIPTIONS[NACK_INTERNAL_ERROR].desc, ok);
        tcomp_udvm_createNackInfo2(udvm, NACK_INTERNAL_ERROR);
        goto bail;
    }

    //Copy sha1 result to udvm memory
    ok &= tcomp_udvm_bytecopy_to(udvm, destination, Message_Digest, TSK_SHA1_DIGEST_SIZE);

bail:
    return ok;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/// @brief	<i>LOAD(%address, %value)</i><br><br>
/// Reference:  RFC3320 Section 9.2.1<br>
/// This instruction sets a 2-byte word to a certain specified value
/// As usual, MSBs are stored before LSBs in the UDVM memory.
///
/// @param [in,out]	udvm	The udvm state machine entity.
/// @param	address			Specifies the starting address of a 2-byte word.
/// @param	value			Specifies the value to be loaded into this word.
///
/// @retval	@a tsk_true if succeed, otherwise returns @a tsk_false.
////////////////////////////////////////////////////////////////////////////////////////////////////
tsk_bool_t TCOMP_UDVM_EXEC_INST__LOAD(tcomp_udvm_t *udvm, uint32_t address, uint32_t value)
{
    CONSUME_CYCLES(1);

    if( address >= TCOMP_UDVM_GET_SIZE() ) {
        TSK_DEBUG_ERROR("%s", TCOMP_NACK_DESCRIPTIONS[NACK_SEGFAULT].desc);
        tcomp_udvm_createNackInfo2(udvm, NACK_SEGFAULT);
        return tsk_false;
    }

    SET_2BYTES_VAL(address, value);

    return tsk_true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/// @brief	<i>MULTILOAD(%address, \#n, %value_0, ..., %value_n-1)</i><br><br>
/// Reference:  RFC3320 Section 9.2.2<br>
/// This instruction sets a contiguous block of 2-byte words in the UDVM memory to specified values.
/// value_0 through to value_n-1 specify the values to load into these words (in the same order as
/// 	they appear in the instruction).
///
/// @param [in,out]	udvm	The udvm state machine entity.
/// @param	address			Starting address of the contiguous 2-byte words.
/// @param	n				Number of 2-bytes values to load.
///
/// @retval	@a tsk_true if succeed, otherwise returns @a tsk_false.
////////////////////////////////////////////////////////////////////////////////////////////////////
tsk_bool_t TCOMP_UDVM_EXEC_INST__MULTILOAD(tcomp_udvm_t *udvm, uint32_t address, uint32_t n)
{
    uint32_t index, _address;
    uint32_t overlap_start = udvm->last_memory_address_of_instruction;
#define overlap_end  udvm->executionPointer
    uint32_t write_start = address;
    uint32_t write_end = (address + (n << 1));

    CONSUME_CYCLES(1 + n);

#define CHECK_MULTILOAD_OVERWRITTEN(__start, __address, __end) \
		if(( (__start) <= (__address) && (__address) <= (__end) ) || ( (__start) <= ((__address) + 1) && ((__address) + 1) <= (__end) )){ \
			TSK_DEBUG_ERROR("%s", TCOMP_NACK_DESCRIPTIONS[NACK_MULTILOAD_OVERWRITTEN].desc); \
			tcomp_udvm_createNackInfo2(udvm, NACK_MULTILOAD_OVERWRITTEN); \
			return tsk_false; \
		}

    // tcomp_udvm_opget_multitype_param() will move the execPtr => make the test before the for loop
    CHECK_MULTILOAD_OVERWRITTEN(overlap_start, address, overlap_end);
    CHECK_MULTILOAD_OVERWRITTEN(write_start, udvm->executionPointer, write_end);

    for(index = 0, _address = address; index < n; index++ , _address += 2) {
        uint32_t value_n = tcomp_udvm_opget_multitype_param(udvm);
        CHECK_MULTILOAD_OVERWRITTEN(overlap_start, _address, overlap_end);
        CHECK_MULTILOAD_OVERWRITTEN(write_start, udvm->executionPointer, write_end);
        SET_2BYTES_VAL(_address, value_n);
    }

    return tsk_true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/// @brief	<i>PUSH (%value)</i><br><br>
/// Reference:  RFC3320 Section 9.2.3<br>
/// This instruction pushes the value specified by its operand on the stack..
///

/// @date	11/27/2009
///
/// @param [in,out]	udvm	The udvm state machine entity.
/// @param	value			2-byte word to push.
///
/// @retval	@a tsk_true if succeed, otherwise returns @a tsk_false.
////////////////////////////////////////////////////////////////////////////////////////////////////
tsk_bool_t TCOMP_UDVM_EXEC_INST__PUSH(tcomp_udvm_t *udvm, int16_t value)
{
    tsk_bool_t callback = (value>=0);
    uint32_t stack_location, stack_fill;
    if(!callback) {
        value = tcomp_udvm_opget_multitype_param(udvm);
    }

    CONSUME_CYCLES(callback ? 0  : 1);



    GET_2BYTES_VAL(TCOMP_UDVM_HEADER_STACK_LOCATION_INDEX, stack_location);
    GET_2BYTES_VAL(stack_location, stack_fill);

    /*
    * copying the value to stack[stack_fill]
    * stack[n] = stack_location+2*n+2
    */
    SET_2BYTES_VAL((stack_location+(2*stack_fill)+2), value);

    /* increasing stack_fill by 1*/
    SET_2BYTES_VAL(stack_location, (stack_fill+1));

    return tsk_true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/// @brief	<i>POP (%address)</i><br><br>
/// Reference:  RFC3320 Section 9.2.3<br>
/// This instruction pops a value from the stack and then copies the value to the specified memory address..
///
/// @param [in,out]	udvm	The udvm state machine entity.
/// @param [in,out]	value	2-byte word to pop from the stack.
///
/// @retval	@a tsk_true if succeed, otherwise returns @a tsk_false.
////////////////////////////////////////////////////////////////////////////////////////////////////
tsk_bool_t TCOMP_UDVM_EXEC_INST__POP(tcomp_udvm_t *udvm, uint32_t* value)
{
    uint32_t address;
    uint32_t stack_location, stack_fill, _value = 0;

    tsk_bool_t callback = (value != 0);

    CONSUME_CYCLES(callback?0:1);

    address = callback ? 0 : tcomp_udvm_opget_multitype_param(udvm);

    GET_2BYTES_VAL(TCOMP_UDVM_HEADER_STACK_LOCATION_INDEX, stack_location);
    GET_2BYTES_VAL(stack_location, stack_fill);

    /*
    * Decompression failure occurs if stack_fill is
    * zero at the commencement of a popping operation.  POP and RETURN pop
    * values from the stack.
    */
    if(stack_fill == 0) {
        TSK_DEBUG_ERROR("%s", TCOMP_NACK_DESCRIPTIONS[NACK_SEGFAULT].desc);
        tcomp_udvm_createNackInfo2(udvm, NACK_SEGFAULT);
        _value = 0;
        goto end;
    }

    /*
    * "Popping" a value from the stack is an abbreviation for decreasing
    * stack_fill by 1, and then using the value stored in stack[stack_fill].
    */
    --stack_fill;
    SET_2BYTES_VAL(stack_location, stack_fill);
    /* stack[n] = stack_location+2*n+2 */
    GET_2BYTES_VAL((stack_location + (2*stack_fill) + 2), _value);

end:
    if(callback) {
        *value = _value;
    }
    else {
        SET_2BYTES_VAL(address, _value);
    }

    return tsk_true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/// @brief	<i>COPY(%position, %length, %destination)</i><br><br>
/// Reference:  RFC3320 Section 9.2.4<br>
/// This instruction is used to copy a string of bytes from one part of the UDVM memory to another.
///
/// @param [in,out]	udvm	The udvm state machine entity.
/// @param	position			Specifies the memory address of the first byte in the string to be copied.
/// @param	length			Specifies the number of bytes to be copied.
/// @param	destination		Gives the address to which the first byte in the string will be copied.
///
/// @retval	@a tsk_true if succeed, otherwise returns @a tsk_false.
////////////////////////////////////////////////////////////////////////////////////////////////////
tsk_bool_t TCOMP_UDVM_EXEC_INST__COPY(tcomp_udvm_t *udvm, uint32_t position, uint32_t length, uint32_t destination)
{
    tsk_bool_t ok = tsk_true;

    CONSUME_CYCLES(1+length);

    if( (position + length) > (int32_t)TCOMP_UDVM_GET_SIZE() || (destination + length) > (int32_t)TCOMP_UDVM_GET_SIZE() ) {
        TSK_DEBUG_ERROR("%s", TCOMP_NACK_DESCRIPTIONS[NACK_SEGFAULT].desc);
        tcomp_udvm_createNackInfo2(udvm, NACK_SEGFAULT);
        return tsk_false;
    }

    /*
    * The COPY instruction is used to copy a string of bytes from one part
    * of the UDVM memory to another.
    */
    ok &= tcomp_udvm_bytecopy_self(udvm, &destination, position, length);

    return ok;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/// @brief	<i>COPY-LITERAL(%position, %length, $destination)</i><br><br>
/// Reference:  RFC3320 Section 9.2.5<br>
/// The COPY-LITERAL instruction behaves as a COPY instruction except
/// that after copying is completed, the value of the destination operand
/// is replaced by the address to which the next byte of data would be copied..
///
/// @param [in,out]	udvm	The udvm state machine entity.
/// @param	position		Specifies the memory address of the first byte in the string to be copied.
/// @param	length			Specifies the number of bytes to be copied.
/// @param	destination		Gives the address to which the first byte in the string will be copied.
///
/// @retval	@a tsk_true if succeed, otherwise returns @a tsk_false.
////////////////////////////////////////////////////////////////////////////////////////////////////
tsk_bool_t TCOMP_UDVM_EXEC_INST__COPY_LITERAL(tcomp_udvm_t *udvm, uint32_t position, uint32_t length, uint32_t destination)
{
    tsk_bool_t ok;
    uint32_t destination_index;

    CONSUME_CYCLES(1+length);

    GET_2BYTES_VAL(destination, destination_index);
    ok = tcomp_udvm_bytecopy_self(udvm, &destination_index, position, length);
    if(ok) {
        /* set next byte */
        SET_2BYTES_VAL(destination, destination_index);
    }

    return ok;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/// @brief	<i>COPY-OFFSET(%offset, %length, $destination)</i><br><br>
/// Reference:  RFC3320 Section 9.2.6<br>
/// This instruction behaves as a COPY-LITERAL instruction
/// except that an offset operand is given instead of a position operand..
///
/// @param [in,out]	udvm	The udvm state machine entity.
/// @param	offset			The offset value.
/// @param	length			Specifies the number of bytes to be copied.
/// @param	destination		Gives the address to which the first byte in the string will be copied.
///
/// @retval	@a tsk_true if succeed, otherwise returns @a tsk_false.
////////////////////////////////////////////////////////////////////////////////////////////////////
tsk_bool_t TCOMP_UDVM_EXEC_INST__COPY_OFFSET(tcomp_udvm_t *udvm, uint32_t offset, uint32_t length, uint32_t destination)
{
    uint32_t DEST, LEFT, RIGTH;
    int32_t position = -1;
    uint32_t destination_index;

    int32_t D, T;
    int32_t O;

    CONSUME_CYCLES(1+length);

    GET_2BYTES_VAL(destination, DEST);
    GET_2BYTES_VAL(TCOMP_UDVM_HEADER_BYTE_COPY_LEFT_INDEX, LEFT);
    GET_2BYTES_VAL(TCOMP_UDVM_HEADER_BYTE_COPY_RIGHT_INDEX, RIGTH);

    /*
    DEST: ses
    D: distance between LEFT and DEST
    O: offset
    T: total size between LEFT and RIGTH

    [*****
    case 1:
    -----LEFT--------DEST------------RIGTH----
    <-----D---->
    <--O->
    <---------------T------------>
    ****]
    [*****
    case 2:
    -----LEFT--------DEST------------RIGTH----
    <-----D---->
    <--------O-------->
    <---------------T------------>
    ****]
    [*****
    case 3:
    -------DEST-----LEFT-------------RIGTH----
    ****]
    */
    D = (DEST - LEFT);
    T = (RIGTH - LEFT);
    O = offset;

    if( D>=0 && O<=D ) {
        /* case 1: easy case */
        position = (DEST-O);
    }
    else if( D>=0 && O>D ) {
        /* case 2: */
        double PAD = (D + ((ceil(((double)O-(double)D)/(double)T))*T))-O;
        position = LEFT+(int32_t)PAD;
    }
    else if( D<0 ) {
        /* case 3: */
        position = DEST-O;
    }

    /* Check position */
    if(position<0) {
        TSK_DEBUG_ERROR("%s", TCOMP_NACK_DESCRIPTIONS[NACK_SEGFAULT].desc);
        tcomp_udvm_createNackInfo2(udvm, NACK_SEGFAULT);
        return tsk_false;
    }

    /* EXEC_INST__COPY_LITERAL */
    GET_2BYTES_VAL(destination, destination_index);
    if(tcomp_udvm_bytecopy_self(udvm, &destination_index, position, length) == tsk_true) {
        SET_2BYTES_VAL(destination, destination_index);
    }
    else {
        return tsk_false;
    }

    return tsk_true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/// @brief	<i>MEMSET(%address, %length, %start_value, %offset)</i><br><br>
/// Reference:  RFC3320 Section 9.2.7<br>
/// Formula: Seq[n] := (start_value + n * offset) modulo 256<br>
/// This instruction initializes an area of UDVM memory to a specified sequence of values.
///

/// @date	11/27/2009
///
/// @param [in,out]	udvm	The udvm state machine entity.
/// @param	address			The destination address.
/// @param	length			The number of 1-byte values to set.
/// @param	start_value		The starting value.
/// @param	offset			The offset used for computation.
///
/// @retval	@a tsk_true if succeed, otherwise returns @a tsk_false.
////////////////////////////////////////////////////////////////////////////////////////////////////
tsk_bool_t TCOMP_UDVM_EXEC_INST__MEMSET(tcomp_udvm_t *udvm, uint32_t address, uint32_t length, uint32_t start_value, uint32_t offset)
{
    uint8_t* seqs_temp;
    uint32_t n;
    tsk_bool_t ok;

    CONSUME_CYCLES(1+length);

    /*
    * The values Seq[0] to Seq[length - 1] inclusive are each interpreted
    * as a single byte, and then concatenated to form a byte string where
    * the first byte has value Seq[0], the second byte has value Seq[1] and
    * so on up to the last byte which has value Seq[length - 1].
    */
    seqs_temp = tsk_calloc(length, sizeof(uint8_t));
    if(!seqs_temp) {
        return tsk_false;
    }

    for(n=0; n < length; n++) {
        seqs_temp[n] = (start_value + n * offset)%256;
    }
    /*
    * The string is then byte copied into the UDVM memory beginning at the
    * memory address specified as an operand to the MEMSET instruction,
    * obeying the rules of Section 8.4.
    */
    ok = tcomp_udvm_bytecopy_to(udvm, address, seqs_temp, length);

    TSK_FREE(seqs_temp);

    return ok;
}

/**
* @brief <i>JUMP (\@address)</i><br><br>
* Reference:  RFC3320 Section 9.3.1<br>
* This instruction moves program execution to the specified memory address.
* Decompression failure occurs if the value of the address operand lies
*    beyond the overall UDVM memory size.
* @param [in,out]	udvm	The udvm state machine entity.
* @param address defines the address to jump to
* @retval 1 if succeed, otherwise returns 0
*/
tsk_bool_t TCOMP_UDVM_EXEC_INST__JUMP(tcomp_udvm_t *udvm, int16_t address)
{
    int callback = (address >=0 );
    CONSUME_CYCLES(callback?0:1);

    if(!callback) {
        address = tcomp_udvm_opget_address_param(udvm, udvm->last_memory_address_of_instruction);
    }

    if(address > (int32_t)TCOMP_UDVM_GET_SIZE()) {
        TSK_DEBUG_ERROR("%s", TCOMP_NACK_DESCRIPTIONS[NACK_SEGFAULT].desc);
        tcomp_udvm_createNackInfo2(udvm, NACK_SEGFAULT);
        return tsk_false;
    }
    udvm->executionPointer = address;

    return tsk_true;
}


/**
 *
 * @brief	<i>COMPARE(%value_1, %value_2, \@address_1, \@address_2, \@address_3)</i><br><br>
 * 			Reference:  RFC3320 Section 9.3.2<br>
 *
 * 			This instruction compares two operands and then jumps to one of three specified
 * 			memory addresses depending on the result.<br>
 * 			if(value_1 < value_2) --> address_1<br>
 * 			elif(value_1 = value_2) --> address_2<br>
 * 			elif(value_1 > value_2) --> address_3. <br>
 *
 *
 * @param [in,out]	udvm	The udvm state machine entity.
 * @param	value_1			The first value to compare.
 * @param	value_2			The second value to compare.
 * @param	address_1		The address to jump to if (value_1 < value_2).
 * @param	address_2		The address to jump to if (value_1 = value_2).
 * @param	address_3		address to jump to if (value_1 > value_2).
 * @retval	1	if succeed, otherwise returns 0.
**/
tsk_bool_t TCOMP_UDVM_EXEC_INST__COMPARE(tcomp_udvm_t *udvm, uint32_t value_1, uint32_t value_2, uint32_t address_1, uint32_t address_2, uint32_t address_3)
{
    tsk_bool_t ok = 1;

    CONSUME_CYCLES(1);

    if(value_1 < value_2) {
        ok &= TCOMP_UDVM_EXEC_INST__JUMP(udvm, address_1);
        goto end;
    }

    if(value_1 == value_2) {
        ok &= TCOMP_UDVM_EXEC_INST__JUMP(udvm, address_2);
        goto end;
    }

    if(value_1 > value_2) {
        ok &= TCOMP_UDVM_EXEC_INST__JUMP(udvm, address_3);
        goto end;
    }

end:
    return ok;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/// @brief	<i>CALL(\@address)</i><br><br>
/// Reference:  RFC3320 Section 9.3.3<br>
/// This instruction finds the memory address of the instruction immediately following
/// the CALL instruction and pushes this 2-byte value on the stack, ready for later retrieval.
/// It then continues instruction execution at the memory address specified by the address operand..
///
/// @param [in,out]	udvm	The udvm state machine entity.
/// @param	address			The next address.
///
/// @retval	@a tsk_true if succeed, otherwise returns @a tsk_false.
////////////////////////////////////////////////////////////////////////////////////////////////////
tsk_bool_t TCOMP_UDVM_EXEC_INST__CALL(tcomp_udvm_t *udvm, uint32_t address)
{
    CONSUME_CYCLES(1);

    return TCOMP_UDVM_EXEC_INST__PUSH(udvm, udvm->executionPointer)
           && TCOMP_UDVM_EXEC_INST__JUMP(udvm, address);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/// @brief	<i>RETURN</i><br><br>
/// Reference:  RFC3320 Section 9.3.3<br>
/// This instruction pops a value from the stack and then continues instruction
/// execution at the memory address just popped..
///
/// @param [in,out]	udvm	The udvm state machine entity.
///
/// @retval	True if succeed, otherwise return false .
////////////////////////////////////////////////////////////////////////////////////////////////////
tsk_bool_t TCOMP_UDVM_EXEC_INST__RETURN(tcomp_udvm_t *udvm)
{
    uint32_t value = 0;
    tsk_bool_t ok = tsk_true;

    CONSUME_CYCLES(1);

    if( (ok = TCOMP_UDVM_EXEC_INST__POP(udvm, &value)) ) {
        ok &= TCOMP_UDVM_EXEC_INST__JUMP(udvm, value);
    }

    return ok;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/// @brief	<i>SWITCH(\#n, %j, \@address_0, \@address_1, ... , \@address_n-1)</i><br><br>
/// Reference:  RFC3320 Section 9.3.4<br>
/// This instruction performs a conditional jump based on the value of one of its operands.
/// Decompression failure occurs if j specifies a value of n or more, or
///    if the address lies beyond the overall UDVM memory size..

/// @param [in,out]	udvm	The udvm state machine entity.
/// @param	n				The number of possibilities.
/// @param	j				The possibility.
///
/// @retval	@a tsk_true if succeed, otherwise returns @a tsk_false.
////////////////////////////////////////////////////////////////////////////////////////////////////
tsk_bool_t TCOMP_UDVM_EXEC_INST__SWITCH(tcomp_udvm_t *udvm, uint32_t n, uint32_t j)
{
    uint32_t next = 0;
    tsk_bool_t ok = tsk_true;

    CONSUME_CYCLES(1+n);

    /* Decompression failure occurs if j specifies a value of n or more. */
    if(j >= n) {
        TSK_DEBUG_ERROR("%s", TCOMP_NACK_DESCRIPTIONS[NACK_SWITCH_VALUE_TOO_HIGH].desc);
        tcomp_udvm_createNackInfo2(udvm, NACK_SWITCH_VALUE_TOO_HIGH);
        ok = tsk_false;
        goto end;
    }

    do {
        next = tcomp_udvm_opget_address_param(udvm, udvm->last_memory_address_of_instruction);
    }
    while(j--);

end:
    if(ok) {
        ok = TCOMP_UDVM_EXEC_INST__JUMP(udvm, next);
    }

    return ok;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/// @brief	<i>CRC(%value, %position, %length, \@address)</i><br><br>
/// Reference:  RFC3320 Section 9.3.5<br>
/// This instruction verifies a string of bytes using a 2-byte CRC.
/// The CRC value is computed exactly as defined for the 16-bit FCS calculation in [RFC-1662]..
///
/// @param [in,out]	udvm	The udvm state machine entity.
/// @param	value			Contains the expected integer value of the 2-byte CRC.
/// @param	position		Defines the position of the string of bytes over which the CRC is evaluated.
/// @param	length			Defines the length of the string of bytes over which the CRC is evaluated.
/// @param	address			The address to jump to if the calculated CRC value do not match.
///
/// @retval	True if succeed, otherwise return false.
////////////////////////////////////////////////////////////////////////////////////////////////////
tsk_bool_t TCOMP_UDVM_EXEC_INST__CRC(tcomp_udvm_t *udvm, uint32_t value, uint32_t position, uint32_t length, uint32_t address)
{
    uint32_t crc_value;

    CONSUME_CYCLES(1 + length);

    if(udvm->tmp_buff.size < length) {
        if(!(udvm->tmp_buff.ptr = tsk_realloc(udvm->tmp_buff.ptr, length))) {
            udvm->tmp_buff.size = 0;
            tcomp_udvm_createNackInfo2(udvm, NACK_INTERNAL_ERROR);
            TSK_DEBUG_ERROR("%s", TCOMP_NACK_DESCRIPTIONS[NACK_INTERNAL_ERROR].desc);
            return tsk_false;
        }
        udvm->tmp_buff.size = length;
    }

    /* copy data */
    if(!tcomp_udvm_bytecopy_from(udvm, udvm->tmp_buff.ptr, position, length)) {
        return tsk_false;
    }

    /*
    * The CRC value is computed exactly as defined for the 16-bit FCS
    * calculation in [RFC-1662]
    */
    crc_value = tsk_pppfcs16(TSK_PPPINITFCS16, udvm->tmp_buff.ptr, length);

    /*
    * If the calculated CRC matches the expected value then the UDVM
    * continues instruction execution at the following instruction.
    * Otherwise the UDVM jumps to the memory address specified by the
    * address operand.
    */
    if(value != crc_value) {
        TCOMP_UDVM_EXEC_INST__JUMP(udvm, address);
    }

    return tsk_true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/// @brief	<i>INPUT-BYTES (%length, %destination, \@address)</i><br><br>
/// Reference:  RFC3320 Section 9.4.2<br>
/// This instruction requests a certain number of bytes of compressed data from the decompressor dispatcher.
///
/// @param [in,out]	udvm	The udvm state machine entity.
/// @param	length			Indicates the requested number of bytes of compressed data.
/// @param	destination		Specifies the starting memory address to which they should be copied.
/// @param	address			Defines the address to jump to if the instruction requests data that lies beyond the end of the SigComp message.
///
/// @retval	True if succeed, otherwise return false.
////////////////////////////////////////////////////////////////////////////////////////////////////
tsk_bool_t TCOMP_UDVM_EXEC_INST__INPUT_BYTES(tcomp_udvm_t *udvm, uint32_t length, uint32_t destination, uint32_t address)
{
    tsk_bool_t ok = tsk_true;
    const uint8_t* compressedDataAddress;
    uint8_t* destinationAddress;

    CONSUME_CYCLES(1+length);

    /*
    * If the INPUT-BYTES is encountered after an INPUT-BITS or an INPUT-
    * HUFFMAN instruction has been used, and the dispatcher currently holds
    * a fraction of a byte, then the fraction MUST be discarded before any
    * data is passed to the UDVM.  The first byte to be passed is the byte
    * immediately following the discarded data.
    */
    tcomp_buffer_discardBits(udvm->sigCompMessage->remaining_sigcomp_buffer);

    compressedDataAddress = tcomp_buffer_readBytes(udvm->sigCompMessage->remaining_sigcomp_buffer, length);
    destinationAddress  = TCOMP_UDVM_GET_BUFFER_AT(destination);
    if(compressedDataAddress) {
        ok &= tcomp_udvm_bytecopy_to(udvm, destination, compressedDataAddress, length);
        if(ok) {
            /* FIXME: (8 * n + 1000) * cycles_per_bit */
            udvm->maximum_UDVM_cycles += ((8 * length /*+ 1000*/) * udvm->stateHandler->sigcomp_parameters->cpbValue);
        }
    }
    else {
        /*
        * If the instruction requests data that lies beyond the end of the
        * SigComp message, no data is returned.  Instead the UDVM moves program
        * execution to the address specified by the address operand.
        */
        ok &= TCOMP_UDVM_EXEC_INST__JUMP(udvm, address);
    }

    return ok;
}

/**
 *
 * @brief	<i>INPUT-BITS (%length, %destination, \@address)</i><br><br>
 * Reference:  RFC3320 Section 9.4.3<br>
 * The INPUT-BITS instruction requests a certain number of bits of
 *  compressed data from the decompressor dispatcher.
 *
 *
 * @param [in,out]	udvm	The udvm state machine entity.
 * @param	length			The length operand indicates the requested number of bits.
   Decompression failure occurs if this operand does not lie between 0
   and 16 inclusive.
 * @param	destination		The destination operand specifies the memory address to which the
   compressed data should be copied.  Note that the requested bits are
   interpreted as a 2-byte integer ranging from 0 to 2^length - 1, as
   explained in Section 8.2.

 * @param	address			The address of the destination.
 *
 * @retval	1 if succeed, otherwise returns 0.
**/
tsk_bool_t TCOMP_UDVM_EXEC_INST__INPUT_BITS(tcomp_udvm_t *udvm, uint32_t length, uint32_t destination, uint32_t address)
{
    tsk_bool_t ok = tsk_true;
    uint32_t input_bit_order, reserved;
    uint8_t F_BIT, P_BIT;
    uint8_t* old_P_BIT;

    /*
    The input_bit_order register contains the following three flags:
    0             7 8            15
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |         reserved        |F|H|P|  68 - 69
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    */

    CONSUME_CYCLES(1);

    GET_2BYTES_VAL(TCOMP_UDVM_HEADER_INPUT_BIT_ORDER_INDEX, input_bit_order);
    reserved = (input_bit_order & 0xf8);
    /*
    * Decompression failure occurs if an INPUT-BITS or an INPUT-HUFFMAN
    * instruction is encountered and the input_bit_order register does not
    * lie between 0 and 7 inclusive.
    */
    if(reserved) {
        /* MUST BE ZEROS --> Only 3bits --> [0-7] */
        TSK_DEBUG_ERROR("%s", TCOMP_NACK_DESCRIPTIONS[NACK_BAD_INPUT_BITORDER].desc);
        tcomp_udvm_createNackInfo2(udvm, NACK_BAD_INPUT_BITORDER);
        return tsk_false;
    }

    /* F and P BITS */
    F_BIT = (input_bit_order & 0x0004) ? 1 : 0;
    P_BIT = (input_bit_order & 0x0001);

    /*
    * Decompression failure occurs if this operand (length) does not lie between 0
    * and 16 inclusive.
    */
    if(/*length<0 ||*/ length>16) {
        TSK_DEBUG_ERROR("%s", TCOMP_NACK_DESCRIPTIONS[NACK_INVALID_OPERAND].desc);
        tcomp_udvm_createNackInfo2(udvm, NACK_INVALID_OPERAND);
        return tsk_false;
    }

    /*
    * P:The P-bit controls the order in which bits are passed from the
    * dispatcher to the INPUT instructions
    * P=0 --> MSB_TO_LSB
    * P=1 --> LSB_TO_MSB
    */
    old_P_BIT = tcomp_buffer_getP_BIT(udvm->sigCompMessage->remaining_sigcomp_buffer);
    if(*old_P_BIT != P_BIT) {
        /*
        * If the P-bit has changed since the last INPUT instruction, any fraction of a
        * byte still held by the dispatcher MUST be discarded (even if the
        * INPUT instruction requests zero bits)
        */
        tcomp_buffer_discardBits(udvm->sigCompMessage->remaining_sigcomp_buffer);
        *old_P_BIT = P_BIT;
    }

    /*
    * If the instruction requests data that lies beyond the end of the
    * SigComp message, no data is returned.  Instead the UDVM moves program
    * execution to the address specified by the address operand.
    */
    if( (length) > tcomp_buffer_getRemainingBits(udvm->sigCompMessage->remaining_sigcomp_buffer) ) {
        ok &= TCOMP_UDVM_EXEC_INST__JUMP(udvm, address);
        goto end;
    }

    /*
    * If the F-bit is set to 0, the INPUT-BITS instruction interprets the
    * received bits as arriving MSBs first, and if it is set to 1, it interprets the bits as arriving LSBs first.
    * F=0 --> MSB_TO_LSB
    * F=1 --> LSB_TO_MSB
    */
    if(P_BIT == TCOMP_P_BIT_MSB_TO_LSB) {
        /* MSB_TO_LSB */
        uint32_t value = tcomp_buffer_readMsbToLsb(udvm->sigCompMessage->remaining_sigcomp_buffer, length);
        if(F_BIT == F_BIT_LSB_TO_MSB) {
            value = (TSK_BINARY_REVERSE_2BYTE(value)>>(16-length));
        }
        SET_2BYTES_VAL(destination, value);
    }
    else {
        /* LSB_TO_MSB */
        uint32_t value = tcomp_buffer_readLsbToMsb(udvm->sigCompMessage->remaining_sigcomp_buffer, length);
        if(F_BIT == F_BIT_LSB_TO_MSB) {
            value = (TSK_BINARY_REVERSE_2BYTE(value)>>(16-length));
        }
        SET_2BYTES_VAL(destination, value);
    }

end:

    udvm->maximum_UDVM_cycles += (length * udvm->stateHandler->sigcomp_parameters->cpbValue);
    return ok;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/// @brief	<i>INPUT-HUFFMAN(%destination, \@address, \#n, %bits_1, %lower_bound_1, %upper_bound_1, %uncompressed_1, ... , %bits_n, %lower_bound_n, %upper_bound_n, %uncompressed_n)</i><br><br>
/// Reference:  RFC3320 Section 9.4.4<br>
///
/// This instruction requests a variable number of bits of compressed data from the decompressor dispatcher.  The instruction
/// initially requests a small number of bits and compares the result against a certain criterion; if the criterion is not met, then
/// additional bits are requested until the criterion is achieved.
///
/// @param [in,out]	udvm	The udvm state machine entity.
/// @param	destination		The udvm destination address.
/// @param	address			Address to jump to if data is requested that lies beyond the end of the SigComp message.
/// @param	n				Additional sets of operands count.
///
/// @retval	 True if succeed, otherwise return false.
////////////////////////////////////////////////////////////////////////////////////////////////////
tsk_bool_t TCOMP_UDVM_EXEC_INST__INPUT_HUFFMAN(tcomp_udvm_t *udvm, uint32_t destination, uint32_t address, uint32_t n)
{
    /*
    The input_bit_order register contains the following three flags:
    0             7 8            15
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |         reserved        |F|H|P|  68 - 69
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    */
    tsk_bool_t ok = tsk_true;
    uint32_t input_bit_order, reserved;
    uint8_t H_BIT, P_BIT, *old_P_BIT;

    uint32_t bits_j, lower_bound_j, upper_bound_j, uncompressed_j;
    uint32_t bits_total = 0, k = 0, H, J;
    tsk_bool_t criterion_ok = tsk_false;

    CONSUME_CYCLES(1+n);

    /*Note that if n = 0 then the INPUT-HUFFMAN instruction is ignored and
    program execution resumes at the following instruction.*/
    if(n == 0) {
        //goto end;
        return ok;
    }

    GET_2BYTES_VAL(TCOMP_UDVM_HEADER_INPUT_BIT_ORDER_INDEX, input_bit_order);
    reserved = (input_bit_order & 0xf8);
    /*
    * Decompression failure occurs if an INPUT-BITS or an INPUT-HUFFMAN
    * instruction is encountered and the input_bit_order register does not
    * lie between 0 and 7 inclusive.
    */
    if(reserved) {
        /* MUST BE ZEROS --> Only 3bits --> [0-7] */
        TSK_DEBUG_ERROR("%s", TCOMP_NACK_DESCRIPTIONS[NACK_BAD_INPUT_BITORDER].desc);
        tcomp_udvm_createNackInfo2(udvm, NACK_BAD_INPUT_BITORDER);
        return tsk_false;
    }

    /* H and P */
    H_BIT = (input_bit_order & 0x0002)?1:0;
    P_BIT = (input_bit_order & 0x0001);

    /*
    * P:The P-bit controls the order in which bits are passed from the
    * dispatcher to the INPUT instructions
    * P=0 --> MSB_TO_LSB
    * P=1 --> LSB_TO_MSB
    */
    old_P_BIT = tcomp_buffer_getP_BIT(udvm->sigCompMessage->remaining_sigcomp_buffer);
    if( *old_P_BIT != P_BIT ) {
        /*
        * If the P-bit has changed since the last INPUT instruction, any fraction of a
        * byte still held by the dispatcher MUST be discarded (even if the
        * INPUT instruction requests zero bits)
        */
        tcomp_buffer_discardBits(udvm->sigCompMessage->remaining_sigcomp_buffer);
        *old_P_BIT = P_BIT;
    }

    /*
    *	HUFFMAN COMPUTATION
    */

    /* 1. Set j := 1 and set H := 0. */
    for(J = 1, H = 0; J<=n; J++) {
        /*
        * Request bits_j compressed bits.  Interpret the returned bits as an
        * integer k from 0 to 2^bits_j - 1, as explained in Section 8.2.
        */
        bits_j = tcomp_udvm_opget_multitype_param(udvm);
        lower_bound_j = tcomp_udvm_opget_multitype_param(udvm);
        upper_bound_j = tcomp_udvm_opget_multitype_param(udvm);
        uncompressed_j = tcomp_udvm_opget_multitype_param(udvm);
        bits_total += bits_j;

        /*Decompression failure occurs if (bits_1 + ... + bits_n) > 16.*/
        if(bits_total > 16) {
            ok = tsk_false;
            // FIXME: DECOMPRESSION failure TOO_MANY_BITS_REQUESTED
            goto end;
        }

        if(criterion_ok) {
            continue;
        }

//==step_4:
        /*
        * 4.If data is requested that lies beyond the end of the SigComp
        * message, terminate the INPUT-HUFFMAN instruction and move program
        * execution to the memory address specified by the address operand.
        */
        if( (bits_j) > tcomp_buffer_getRemainingBits(udvm->sigCompMessage->remaining_sigcomp_buffer) ) {
            ok &= TCOMP_UDVM_EXEC_INST__JUMP(udvm, address);
            goto end;
        }

//==step_2:
        /*
        * 2. Request bits_j compressed bits.  Interpret the returned bits as an
        * integer k from 0 to 2^bits_j - 1, as explained in Section 8.2.
        */
        if(P_BIT == TCOMP_P_BIT_MSB_TO_LSB) {
            k = tcomp_buffer_readMsbToLsb(udvm->sigCompMessage->remaining_sigcomp_buffer, bits_j);
            if(H_BIT == H_BIT_LSB_TO_MSB) {
                k = (TSK_BINARY_REVERSE_2BYTE(k)>>(16-bits_j));
            }
        }
        else {
            k = tcomp_buffer_readLsbToMsb(udvm->sigCompMessage->remaining_sigcomp_buffer, bits_j);
            if(H_BIT == H_BIT_LSB_TO_MSB) {
                k = (TSK_BINARY_REVERSE_2BYTE(k)>>(16-bits_j));
            }
        }
//==step_3:
        /* 3. Set H := H * 2^bits_j + k */
        H = H * (uint32_t)pow(2.0, bits_j) + k;

//==step_5:
        /*
        * 5. If (H < lower_bound_j) or (H > upper_bound_j) then set j := j + 1.
        * Then go back to Step 2, unless j > n in which case decompression
        * failure occurs.
        */
        if( (H < lower_bound_j) || (H > upper_bound_j) ) {
            continue;
            //goto step_2;
        }
        else {
            /*
            * Copy (H + uncompressed_j - lower_bound_j) modulo 2^16 to the
            * memory address specified by the destination operand.
            */
            H = (H + uncompressed_j - lower_bound_j) % 65536;
            criterion_ok = 1;
        }
    }

    if(!criterion_ok) {
        TSK_DEBUG_ERROR("%s", TCOMP_NACK_DESCRIPTIONS[NACK_HUFFMAN_NO_MATCH].desc);
        tcomp_udvm_createNackInfo2(udvm, NACK_HUFFMAN_NO_MATCH);
        ok = tsk_false;
        goto end;
    }
    else if(ok) {
        SET_2BYTES_VAL(destination, H);
        udvm->maximum_UDVM_cycles += (bits_total * udvm->stateHandler->sigcomp_parameters->cpbValue);
    }
end:
    return ok;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/// @brief	<i>STATE-ACCESS(%partial_identifier_start, %partial_identifier_length, %state_begin, %state_length, %state_address, %state_instruction)</i><br><br>
/// Reference:  RFC3320 Section 9.4.5<br>
/// This instruction retrieves some previously stored state information..

/// @param [in,out]	udvm				If non-null, the udvm.
/// @param	partial_identifier_start	Specifies the location of the partial state identifier used to retrieve the state information.
/// @param	partial_identifier_length	Specifies the length of the partial state identifier used to retrieve the state information.
/// @param	state_begin					Defines the starting byte to copy from the state_value contained in the returned item of state.
/// @param	state_length				Defines the number of bytes to copy from the state_value contained in the returned item of state.
/// @param	state_address				Contains a UDVM memory address.
/// @param	state_instruction			Next instruction to jump to.
///
/// @retval	@a tsk_true if succeed, otherwise returns @a tsk_false.
////////////////////////////////////////////////////////////////////////////////////////////////////
tsk_bool_t TCOMP_UDVM_EXEC_INST__STATE_ACCESS(tcomp_udvm_t *udvm, uint32_t partial_identifier_start, uint32_t partial_identifier_length, uint32_t state_begin, uint32_t state_length, uint32_t state_address, uint32_t state_instruction)
{
    tcomp_state_t* lpState = NULL;
    tcomp_buffer_handle_t* partial_id;
    uint32_t match_count;

    /*
    * Decompression failure occurs if partial_identifier_length does not
    * lie between 6 and 20 inclusive.
    */
    if(partial_identifier_length<6 || partial_identifier_length>20) {
        TSK_DEBUG_ERROR("%s", TCOMP_NACK_DESCRIPTIONS[NACK_INVALID_STATE_ID_LENGTH].desc);
        tcomp_udvm_createNackInfo2(udvm, NACK_INVALID_STATE_ID_LENGTH);
        return tsk_false;
    }
    /*
    * decompression failure will always occur if the state_length operand
    * is set to 0 but the state_begin operand is non-zero.
    */
    if(!state_length && state_begin) {
        TSK_DEBUG_ERROR("%s", TCOMP_NACK_DESCRIPTIONS[NACK_INVALID_STATE_PROBE].desc);
        tcomp_udvm_createNackInfo2(udvm, NACK_INVALID_STATE_PROBE);
        return tsk_false;
    }

    /*
    * Find matching state
    */
    partial_id = tcomp_buffer_create_null();
    tcomp_buffer_referenceBuff(partial_id, TCOMP_UDVM_GET_BUFFER_AT(partial_identifier_start), partial_identifier_length);
    match_count = tcomp_statehandler_findState(udvm->stateHandler, partial_id, &lpState);

    /*
    * Decompression failure occurs if no state item matching the partial state identifier can be found, if
    * more than one state item matches the partial identifier.
    */
    if(!lpState || match_count != 1) {
        int32_t nack_code = (match_count > 1) ? NACK_ID_NOT_UNIQUE : NACK_STATE_NOT_FOUND;
        TSK_DEBUG_ERROR("%s", TCOMP_NACK_DESCRIPTIONS[nack_code].desc);
        tcomp_udvm_createNackInfo3(udvm, nack_code, partial_id);
        TSK_OBJECT_SAFE_FREE(partial_id);
        return tsk_false;
    }
    else if(partial_identifier_length < lpState->minimum_access_length) {
        /*
        * Decompression failure occurs if partial_identifier_length is less than the minimum_access_length of
        * the matched state item.
        */
        TSK_DEBUG_ERROR("%s", TCOMP_NACK_DESCRIPTIONS[NACK_STATE_NOT_FOUND].desc);
        tcomp_udvm_createNackInfo3(udvm, NACK_STATE_NOT_FOUND, partial_id);
        TSK_OBJECT_SAFE_FREE(partial_id);
        return tsk_false;
    }
    TSK_OBJECT_SAFE_FREE(partial_id);

    /*
    * If any of the operands state_address, state_instruction or
    * state_length is set to 0 then its value is taken from the returned
    * item of state instead.
    */
    if(!state_address) {
        state_address = lpState->address;
    }
    if(!state_instruction) {
        state_instruction = lpState->instruction;
    }
    if(!state_length) {
        state_length = lpState->length;
    }

    CONSUME_CYCLES(1+state_length);

    /* Decompression failure occurs if bytes are copied from beyond the end of the state_value. */
    if((tsk_size_t)(state_begin + state_length) > tcomp_buffer_getSize(lpState->value)) {
        TSK_DEBUG_ERROR("%s", TCOMP_NACK_DESCRIPTIONS[NACK_STATE_TOO_SHORT].desc);
        tcomp_udvm_createNackInfo3(udvm, NACK_STATE_TOO_SHORT, partial_id);
        return tsk_false;
    }

    /*
    * The state_address operand contains a UDVM memory address.  The
    * requested portion of the state_value is byte copied to this memory
    * address using the rules of Section 8.4.
    */
    if(tcomp_udvm_bytecopy_to(udvm, state_address, tcomp_buffer_getBufferAtPos(lpState->value, state_begin), state_length) != tsk_true) {
        return tsk_false;
    }

    /*
    * Program execution then resumes at the memory address specified by
    * state_instruction, unless this address is 0 in which case program
    * execution resumes at the next instruction following the STATE-ACCESS
    * instruction.
    */
    if(state_instruction) {
        if(!TCOMP_UDVM_EXEC_INST__JUMP(udvm, state_instruction)) {
            return tsk_false;
        }
    }

    return tsk_true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/// @brief	<i>STATE-CREATE (%state_length, %state_address, %state_instruction, %minimum_access_length, %state_retention_priority)</i><br><br>
/// Reference:  RFC3320 Section 9.4.6<br>
/// This instruction requests the creation of a state item at the receiving endpoint..

/// @param [in,out]	udvm				If non-null, the udvm.
/// @param	state_length				Defines the length of the state to create.
/// @param	state_address				Defines the udvm address of the state to create.
/// @param	state_instruction			Defines the state instruction.
/// @param	minimum_access_length		Defines the minimun access length.
/// @param	state_retention_priority	Defines the state retenion priority.
///
/// @retval	@a tsk_true if succeed, otherwise returns @a tsk_false.
////////////////////////////////////////////////////////////////////////////////////////////////////
tsk_bool_t TCOMP_UDVM_EXEC_INST__STATE_CREATE(tcomp_udvm_t *udvm, uint32_t state_length, uint32_t state_address, uint32_t state_instruction, uint32_t minimum_access_length, uint32_t state_retention_priority)
{
    CONSUME_CYCLES(1 + state_length);

    /*
    *	Create temporary state?
    */
    if(!tcomp_udvm_createTempState(udvm, state_length, state_address, state_instruction,
                                   minimum_access_length, state_retention_priority, 0)) {
        return tsk_false;
    }

    return tsk_true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/// @brief	<i>STATE-FREE(%partial_identifier_start, %partial_identifier_length)</i><br><br>
/// Reference:  RFC3320 Section 9.4.7<br>
/// This instruction informs the receiving endpoint that the sender no longer wishes to use a particular state item..

/// @param [in,out]	udvm				If non-null, the udvm.
/// @param	partial_identifier_start	Defines the first byte address of partial start identifier.
/// @param	partial_identifier_length	Defines the partial identifier length.
///
/// @retval	True if succeed, otherwise return false .
////////////////////////////////////////////////////////////////////////////////////////////////////
tsk_bool_t TCOMP_UDVM_EXEC_INST__STATE_FREE(tcomp_udvm_t *udvm, uint32_t partial_identifier_start, uint32_t partial_identifier_length)
{
    tcomp_tempstate_to_free_t *lpDesc;

    CONSUME_CYCLES(1);

    /*
    * Decompression failure MUST occur if more than four state free
    * requests are made before the END-MESSAGE instruction is encountered.
    */
    if(tcomp_result_getTempStatesToFreeSize(udvm->lpResult) >=4) {
        TSK_DEBUG_ERROR("%s", TCOMP_NACK_DESCRIPTIONS[NACK_TOO_MANY_STATE_REQUESTS].desc);
        tcomp_udvm_createNackInfo2(udvm, NACK_TOO_MANY_STATE_REQUESTS);
        return tsk_false;
    }

    /*
    * Decompression failure also occurs if partial_identifier_length does
    * not lie between 6 and 20 inclusive.
    */
    if(partial_identifier_length<6 || partial_identifier_length>20) {
        TSK_DEBUG_ERROR("%s", TCOMP_NACK_DESCRIPTIONS[NACK_INVALID_STATE_ID_LENGTH].desc);
        tcomp_udvm_createNackInfo2(udvm, NACK_INVALID_STATE_ID_LENGTH);
        return tsk_false;
    }

    lpDesc = tcomp_tempstate_to_free_create();
    lpDesc->partial_identifier_length = partial_identifier_length;
    lpDesc->partial_identifier_start = partial_identifier_start;
    tcomp_result_addTempStateToFree(udvm->lpResult, lpDesc);

    /*** Do not ByteCopy data, wait for END_MESSAGE --> see RFC 3320 subclause 9.4.9 **/

    return tsk_true;
}

/**
* @brief <i>OUTPUT (%output_start, %output_length)</i><br><br>
* Reference:  RFC3320 Section 9.4.8<br>
* This instruction provides successfully decompressed data to the dispatcher.
* @param [in,out]	udvm	The udvm state machine entity.
* @param output_start defines the starting memory address of the byte string to be provided to the dispatcher
* @param output_length defines the length of the byte string to be provided to the dispatcher
* @retval 1 if succeed, otherwise returns 0
*/
tsk_bool_t TCOMP_UDVM_EXEC_INST__OUTPUT(tcomp_udvm_t *udvm, uint32_t output_start, uint32_t output_length)
{
    tsk_bool_t ok;
    tsk_size_t *outputbuffer_size;

    CONSUME_CYCLES(1+output_length);

    outputbuffer_size = tcomp_buffer_getIndexBytes(udvm->lpResult->output_buffer);
    if( (*outputbuffer_size + output_length) > 65536 ) {
        /*
        * Decompression failure occurs if the cumulative number of bytes
        * provided to the dispatcher exceeds 65536 bytes.
        */
        TSK_DEBUG_ERROR("%s", TCOMP_NACK_DESCRIPTIONS[NACK_OUTPUT_OVERFLOW].desc);
        tcomp_udvm_createNackInfo2(udvm, NACK_OUTPUT_OVERFLOW);
        return tsk_false;
    }

    // FIXME: do it once?
    if((ok = tcomp_udvm_bytecopy_from(udvm, tcomp_buffer_getBufferAtPos(udvm->lpResult->output_buffer, *outputbuffer_size), output_start,  output_length))) {
        *outputbuffer_size += output_length;
    }

#if DEBUG || _DEBUG
    //tcomp_buffer_nprint(udvm->lpResult->output_buffer, *outputbuffer_size);
#endif
    return ok;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/// @brief	<i> END-MESSAGE (%requested_feedback_location, %returned_parameters_location, %state_length, %state_address, %state_instruction, %minimum_access_length, %state_retention_priority)</i><br><br>
/// Reference:  RFC3320 Section 9.4.9<br>
/// This instruction successfully terminates the UDVM and forwards the state creation and state free requests to the state
/// handler together with any supplied feedback data.
///
/// @param [in,out]	udvm					Defines the requested feedback location.
/// @param	requested_feedback_location		The requested feedback location.
/// @param	returned_parameters_location	Defines the returned parameters location which contains remote party capabilities.
/// @param	state_length					Length of the state to create.
/// @param	state_address					UDVM memory address of the state to create.
/// @param	state_instruction				Defines the state instruction.
/// @param	minimum_access_length			Defines the state's minimum access length.
/// @param	state_retention_priority		Determines the order in which state will be deleted when the compartment exceeds its allocated state memory.
///
/// @retval	@a tsk_true if succeed, otherwise returns @a tsk_false.
////////////////////////////////////////////////////////////////////////////////////////////////////

tsk_bool_t TCOMP_UDVM_EXEC_INST__END_MESSAGE(tcomp_udvm_t *udvm, uint32_t requested_feedback_location, uint32_t returned_parameters_location, uint32_t state_length, uint32_t state_address, uint32_t state_instruction, uint32_t minimum_access_length, uint32_t state_retention_priority)
{
    tsk_size_t udvm_size;

    CONSUME_CYCLES(1+state_length);

    udvm_size = TCOMP_UDVM_GET_SIZE();

    /*
    *	Create temporary state provided by END_MESSAGE?
    */
    if(!tcomp_udvm_createTempState(udvm, state_length, state_address, state_instruction, minimum_access_length, state_retention_priority, 1)) {
        return tsk_false;
    }

    /*
    * Byte copy all waiting STATE-FREE/STATE-CREATE/END-MESSAGE states
    */
    if(!tcomp_udvm_byteCopy_TempStates(udvm)) {
        TSK_DEBUG_ERROR("%s", TCOMP_NACK_DESCRIPTIONS[NACK_INTERNAL_ERROR].desc);
        tcomp_udvm_createNackInfo2(udvm, NACK_INTERNAL_ERROR);
        return tsk_false;
    }

    /*
    *	Feedback has been requested?
    */
    if(requested_feedback_location) {
        uint8_t r_f_l;
        if(requested_feedback_location >= udvm_size) {
            TSK_DEBUG_ERROR("%s", TCOMP_NACK_DESCRIPTIONS[NACK_SEGFAULT].desc);
            tcomp_udvm_createNackInfo2(udvm, NACK_SEGFAULT);
            return tsk_false;
        }
        /*
        0   1   2   3   4   5   6   7
        +---+---+---+---+---+---+---+---+
        |     reserved      | Q | S | I |  requested_feedback_location
        +---+---+---+---+---+---+---+---+
        |                               |
        :    requested feedback item    :  if Q = 1
        |                               |
        +---+---+---+---+---+---+---+---+
        */
        r_f_l = *TCOMP_UDVM_GET_BUFFER_AT(requested_feedback_location);
        udvm->lpResult->req_feedback->I = (r_f_l & 0x01);
        udvm->lpResult->req_feedback->S = (r_f_l & 0x02) ? 1 : 0;
        udvm->lpResult->req_feedback->Q = (r_f_l & 0x04) ? 1 : 0;

        requested_feedback_location++; /* skip 1-byte header */
        if(udvm->lpResult->req_feedback->Q) {
            /* we have a requested feedback item */
            uint8_t r_f_i = *TCOMP_UDVM_GET_BUFFER_AT(requested_feedback_location);
            uint8_t length = 1; /* [1-128] */
            if(r_f_i & 0x80) {
                /* case 2 */
                length += (r_f_i & 0x7f); /* seven last bits */
            }

            if(requested_feedback_location >= TCOMP_UDVM_GET_SIZE()) {
                TSK_DEBUG_ERROR("%s", TCOMP_NACK_DESCRIPTIONS[NACK_SEGFAULT].desc);
                tcomp_udvm_createNackInfo2(udvm, NACK_SEGFAULT);
                return tsk_false;
            }
            /* copy from udvm */
            // FIXME: use realloc
            tcomp_buffer_freeBuff(udvm->lpResult->req_feedback->item);
            tcomp_buffer_allocBuff(udvm->lpResult->req_feedback->item, length);
            if(!tcomp_udvm_bytecopy_from(udvm, tcomp_buffer_getBuffer(udvm->lpResult->req_feedback->item), requested_feedback_location, length)) {
                return tsk_false;
            }
        }
    }

    //
    //	SigComp parameters have been returned?
    //
    if(returned_parameters_location) {
        uint8_t r_p_l, SigComp_version;
        uint32_t index;
        tcomp_buffer_handle_t *partial_id;

        /*
        0   1   2   3   4   5   6   7
        +---+---+---+---+---+---+---+---+
        |  cpb  |    dms    |    sms    |  returned_parameters_location
        +---+---+---+---+---+---+---+---+
        |        SigComp_version        |
        +---+---+---+---+---+---+---+---+
        | length_of_partial_state_ID_1  |
        +---+---+---+---+---+---+---+---+
        |                               |
        :  partial_state_identifier_1   :
        |                               |
        +---+---+---+---+---+---+---+---+
        :               :
        +---+---+---+---+---+---+---+---+
        | length_of_partial_state_ID_n  |
        +---+---+---+---+---+---+---+---+
        |                               |
        :  partial_state_identifier_n   :
        |                               |
        +---+---+---+---+---+---+---+---+
        */

        if(returned_parameters_location >= udvm_size) {
            TSK_DEBUG_ERROR("%s", TCOMP_NACK_DESCRIPTIONS[NACK_SEGFAULT].desc);
            tcomp_udvm_createNackInfo2(udvm, NACK_SEGFAULT);
            return tsk_false;
        }

        /* cpb+dms+sms */
        r_p_l = *TCOMP_UDVM_GET_BUFFER_AT(returned_parameters_location);
        returned_parameters_location++;
        if(r_p_l) {
            tcomp_params_setCpbCode(udvm->lpResult->remote_parameters, ((r_p_l & 0xc0)>>6));
            tcomp_params_setDmsCode(udvm->lpResult->remote_parameters, ((r_p_l & 0x38)>>3));
            tcomp_params_setSmsCode(udvm->lpResult->remote_parameters, (r_p_l & 0x07));
        }
        /* sigcomp version */
        SigComp_version = *TCOMP_UDVM_GET_BUFFER_AT(returned_parameters_location);
        returned_parameters_location++;
        if(SigComp_version) {
            udvm->lpResult->remote_parameters->SigComp_version = SigComp_version;
        }
        /* state items */
        for(index = returned_parameters_location; index <(udvm_size-1); ) {
            uint8_t length, *length_ptr = TCOMP_UDVM_GET_BUFFER_AT(index);
            if(!length_ptr) {
                return tsk_false;
            }
            length = *length_ptr; // 1-byte
            if(length<6 || length>20) {
                break;
            }
            index++;
            if((index+length) >= (uint32_t)udvm_size) {
                TSK_DEBUG_ERROR("%s", TCOMP_NACK_DESCRIPTIONS[NACK_SEGFAULT].desc);
                tcomp_udvm_createNackInfo2(udvm, NACK_SEGFAULT);
                return tsk_false;
            }
            partial_id = tcomp_buffer_create_null();
            tcomp_buffer_allocBuff(partial_id, length);
            if(!tcomp_udvm_bytecopy_from(udvm, tcomp_buffer_getBuffer(partial_id), index, length)) {
                return tsk_false;
            }
            if(!udvm->lpResult->remote_parameters->returnedStates) {
                udvm->lpResult->remote_parameters->returnedStates = tsk_list_create();
            }
            tsk_list_push_back_data(udvm->lpResult->remote_parameters->returnedStates, (void**)&partial_id);
            index += length;
        }
    }

    return tsk_true;
}

