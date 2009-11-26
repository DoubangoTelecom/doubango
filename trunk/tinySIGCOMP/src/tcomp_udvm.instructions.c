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

/**@file tcomp_udvm.instructions.c
 * @brief  The machine architecture described in this document.  The UDVM is used to decompress SigComp messages.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tcomp_udvm.h"

#include "tsk_memory.h"

#include <stdlib.h> /* qsort */
#include <math.h>	/* ceil, log ... */

/**@defgroup tcomp_udvm_group SIGCOMP UDVM machine.
* The machine architecture described in this document.  The UDVM is used to decompress SigComp messages.
*/

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
		tcomp_udvm_createNackInfo2(udvm, NACK_CYCLES_EXHAUSTED);	\
		return 0;													\
	}

/**
	This structure is used to keep index-value pairs after sorting.
*/
typedef struct IndexValuePair_s
{
	uint16_t index;
	uint16_t value;
}
IndexValuePair_t;

/**
* Sort Ascending predicate.
*/
int SortAscendingPredicate(const void *a, const void *b)
{
	const IndexValuePair_t *el1 = a;
	const IndexValuePair_t *el2 = b;
	
	return (el1->value - el2->value);
}

/**
* Sort Descending predicate.
*/
int SortDescendingPredicate(const void *a, const void *b)
{
	const IndexValuePair_t *el1 = a;
	const IndexValuePair_t *el2 = b;
	
	return (el2->value - el1->value);
};



int TCOMP_UDVM_EXEC_INST__DECOMPRESSION_FAILURE(tcomp_udvm_t *udvm)
{
	return 0;
}

/**
* AND ($operand_1, %operand_2)
* Reference:  RFC3320 Section 9.1.1
* Formula: [operand_1 := operand_1 & operand_2]
* @param operand_1 2-byte value encoded by the operand. After the operation is complete, the 2-byte word at the memory address specified by
*    this operand is overwritten with the result
* @param operand_2 Second operand
* @retval true if succeed, otherwise return false
*/
int TCOMP_UDVM_EXEC_INST__AND(tcomp_udvm_t *udvm, uint16_t operand_1, uint16_t operand_2)
{
	CONSUME_CYCLES(1);
	
	TCOMP_UDVM_SET_2BYTES_VAL( operand_1, 
		(TCOMP_UDVM_GET_2BYTES_VAL(operand_1) & operand_2) );
	
	return 1; 
}

/**
* OR ($operand_1, %operand_2)
* Reference:  RFC3320 Section 9.1.1
* Formula: [operand_1 := operand_1 | operand_2]
* @param operand_1 2-byte value encoded by the operand. After the operation is complete, the 2-byte word at the memory address specified by
*    this operand is overwritten with the result
* @param operand_2 Second operand
* @retval true if succeed, otherwise return false
*/
int TCOMP_UDVM_EXEC_INST__OR(tcomp_udvm_t *udvm, uint16_t operand_1, uint16_t operand_2)
{
	CONSUME_CYCLES(1);

	TCOMP_UDVM_SET_2BYTES_VAL( operand_1, 
		(TCOMP_UDVM_GET_2BYTES_VAL(operand_1) | operand_2) );

	return 1; 
}

/**
* NOT ($operand_1)
* Reference:  RFC3320 Section 9.1.1
* Formula: [operand_1 := ~operand_1]
* @param operand_1 2-byte value encoded by the operand. After the operation is complete, the 2-byte word at the memory address specified by
*    this operand is overwritten with the result
* @retval true if succeed, otherwise return false
*/
int TCOMP_UDVM_EXEC_INST__NOT(tcomp_udvm_t *udvm, uint16_t operand_1)
{
	CONSUME_CYCLES(1);

	TCOMP_UDVM_SET_2BYTES_VAL( operand_1, 
		~( TCOMP_UDVM_GET_2BYTES_VAL(operand_1) ) );

	return 1;
}

/**
* LSHIFT ($operand_1, %operand_2)
* Reference:  RFC3320 Section 9.1.1
* Formula: [LSHIFT (m, n) := m * 2^n (modulo 2^16)]
* @param operand_1 2-byte value encoded by the operand. After the operation is complete, the 2-byte word at the memory address specified by
*    this operand is overwritten with the result
* @param operand_2 2-byte value encoded by the operand
* @retval true if succeed, otherwise return false
*/
int TCOMP_UDVM_EXEC_INST__LSHIFT(tcomp_udvm_t *udvm, uint16_t operand_1, uint16_t operand_2)
{
	CONSUME_CYCLES(1);

	// (m * 2^n)  == (m<<n)
	// (2^16) === 65536
	TCOMP_UDVM_SET_2BYTES_VAL( operand_1, 
		(TCOMP_UDVM_GET_2BYTES_VAL(operand_1) << operand_2) );

	return 1; 
}

/**
* RSHIFT ($operand_1, %operand_2)
* Reference:  RFC3320 Section 9.1.1
* Formula: [RSHIFT (m, n) := floor(m / 2^n)]
* @param operand_1 2-byte value encoded by the operand. After the operation is complete, the 2-byte word at the memory address specified by
*    this operand is overwritten with the result
* @param operand_2 2-byte value encoded by the operand
* @returns true if succeed, otherwise return false
*/
int TCOMP_UDVM_EXEC_INST__RSHIFT(tcomp_udvm_t *udvm, uint16_t operand_1, uint16_t operand_2)
{
	CONSUME_CYCLES(1);
	
	// floor(m / 2^n) == (m>>n)
	TCOMP_UDVM_SET_2BYTES_VAL(operand_1,
		(TCOMP_UDVM_GET_2BYTES_VAL(operand_1) >> operand_2) );

	return 1; 
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @fn	int TCOMP_UDVM_EXEC_INST__ADD(tcomp_udvm_t *udvm, uint16_t operand_1, uint16_t operand_2)
///
/// @brief	ADD ($operand_1, %operand_2)
/// Reference:  RFC3320 Section 9.1.2
/// Formula: [ADD (m, n) := m + n (modulo 2^16)]
///
/// @author	Mamadou
/// @date	11/26/2009
///
/// @param [in,out]	udvm	If non-null, the udvm. 
/// @param	operand_1		2-byte value encoded by the operand. After the operation is complete, the 2-byte word at the memory address specified by
///   this operand is overwritten with the result. 
/// @param	operand_2		2-byte value encoded by the operand. 
///
/// @return	True if succeed, otherwise return false.
////////////////////////////////////////////////////////////////////////////////////////////////////

int TCOMP_UDVM_EXEC_INST__ADD(tcomp_udvm_t *udvm, uint16_t operand_1, uint16_t operand_2)
{
	CONSUME_CYCLES(1);

	TCOMP_UDVM_SET_2BYTES_VAL(operand_1, 
		(TCOMP_UDVM_GET_2BYTES_VAL(operand_1) + operand_2) );
	
	return 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @fn	int TCOMP_UDVM_EXEC_INST__SUBTRACT(tcomp_udvm_t *udvm, uint16_t operand_1,
/// 	uint16_t operand_2)
///
/// @brief	SUBTRACT ($operand_1, %operand_2)
/// Reference:  RFC3320 Section 9.1.2
/// Formula: [SUBTRACT (m, n)  := m - n (modulo 2^16)]
///
/// @author	Mamadou
/// @date	11/26/2009
///
/// @param [in,out]	udvm	If non-null, the udvm. 
/// @param	operand_1		2-byte value encoded by the operand. After the operation is complete, the 2-byte word at the memory address specified by
///   this operand is overwritten with the result.
/// @param	operand_2		2-byte value encoded by the operand.
///
/// @return	true if succeed, otherwise return false.
////////////////////////////////////////////////////////////////////////////////////////////////////
int TCOMP_UDVM_EXEC_INST__SUBTRACT(tcomp_udvm_t *udvm, uint16_t operand_1, uint16_t operand_2)
{
	CONSUME_CYCLES(1);
	
	TCOMP_UDVM_SET_2BYTES_VAL(operand_1, 
		(TCOMP_UDVM_GET_2BYTES_VAL(operand_1) - operand_2) );
	
	return 1; 
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @fn	int TCOMP_UDVM_EXEC_INST__MULTIPLY(tcomp_udvm_t *udvm, uint16_t operand_1,
/// 	uint16_t operand_2)
///
/// @brief	MULTIPLY ($operand_1, %operand_2)
/// Reference:  RFC3320 Section 9.1.2
/// Formula: [MULTIPLY (m, n)  := m * n (modulo 2^16)]
///
/// @author	Mamadou
/// @date	11/26/2009
///
/// @param [in,out]	udvm	If non-null, the udvm. 
/// @param	operand_1		2-byte value encoded by the operand. After the operation is complete, the 2-byte word at the memory address specified by
///  this operand is overwritten with the result. 
/// @param	operand_2		2-byte value encoded by the operand. 
///
/// @return	True if succeed, otherwise return false. 
////////////////////////////////////////////////////////////////////////////////////////////////////
int TCOMP_UDVM_EXEC_INST__MULTIPLY(tcomp_udvm_t *udvm, uint16_t operand_1, uint16_t operand_2)
{
	CONSUME_CYCLES(1);
	
	TCOMP_UDVM_SET_2BYTES_VAL(operand_1, 
		(TCOMP_UDVM_GET_2BYTES_VAL(operand_1) * operand_2) );
	
	return 1; 
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @fn	int TCOMP_UDVM_EXEC_INST__DIVIDE(tcomp_udvm_t *udvm, uint16_t operand_1,
/// 	uint16_t operand_2)
///
/// @brief	DIVIDE ($operand_1, %operand_2)
/// Reference:  RFC3320 Section 9.1.2
/// Formula: [DIVIDE (m, n) := floor(m / n)]
///
/// @author	Mamadou
/// @date	11/26/2009
///
/// @param [in,out]	udvm	If non-null, the udvm. 
/// @param	operand_1		2-byte value encoded by the operand. After the operation is complete, the 2-byte word at the memory address specified by
///   this operand is overwritten with the result.
/// @param	operand_2		2-byte value encoded by the operand. Decompression failure occurs if this operand is zero. 
///
/// @return	True if succeed, otherwise return false. 
////////////////////////////////////////////////////////////////////////////////////////////////////
int TCOMP_UDVM_EXEC_INST__DIVIDE(tcomp_udvm_t *udvm, uint16_t operand_1, uint16_t operand_2)
{
	CONSUME_CYCLES(1);

	if(!operand_2)
	{
		tcomp_udvm_createNackInfo2(udvm, NACK_DIV_BY_ZERO);
		return 0;
	}

	TCOMP_UDVM_SET_2BYTES_VAL(operand_1, 
		(TCOMP_UDVM_GET_2BYTES_VAL(operand_1) / operand_2) );

	return 1; 
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @fn	int TCOMP_UDVM_EXEC_INST__REMAINDER(tcomp_udvm_t *udvm, uint16_t operand_1,
/// 	uint16_t operand_2)
///
/// @brief	REMAINDER ($operand_1, %operand_2)
/// Reference:  RFC3320 Section 9.1.2
/// Formula: [REMAINDER (m, n) := m - n * floor(m / n)]
///
/// @author	Mamadou
/// @date	11/26/2009
///
/// @param [in,out]	udvm	If non-null, the udvm. 
/// @param	operand_1		2-byte value encoded by the operand. After the operation is complete, the 2-byte word at the memory address specified by
///   this operand is overwritten with the result. 
/// @param	operand_2		2-byte value encoded by the operand. Decompression failure occurs if this operand is zero.
///
/// @return	True if succeed, otherwise return false. 
////////////////////////////////////////////////////////////////////////////////////////////////////
int TCOMP_UDVM_EXEC_INST__REMAINDER(tcomp_udvm_t *udvm, uint16_t operand_1, uint16_t operand_2)
{
	CONSUME_CYCLES(1);

	if(!operand_2)
	{
		tcomp_udvm_createNackInfo2(udvm, NACK_DIV_BY_ZERO);
		return 0;
	}

	TCOMP_UDVM_SET_2BYTES_VAL(operand_1, 
		(TCOMP_UDVM_GET_2BYTES_VAL(operand_1) % operand_2) );

	return 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @fn	int TCOMP_UDVM_EXEC_INST__SORT_ASCENDING(tcomp_udvm_t *udvm, uint16_t start, uint16_t n,
/// 	uint16_t k)
///
/// @brief	SORT-ASCENDING (%start, %n, %k)
/// Reference:  RFC3320 Section 9.1.3
///
/// This instruction sort lists of 2-byte words in ascending order.
///
/// @author	Mamadou
/// @date	11/26/2009
///
/// @param [in,out]	udvm	If non-null, the udvm. 
/// @param	start			The starting memory address of the block of data to be sorted. 
/// @param	n				Number of lists. 
/// @param	k				Lists length (2-byte words). 
///
/// @return	True if succeed, otherwise return false.
////////////////////////////////////////////////////////////////////////////////////////////////////
int TCOMP_UDVM_EXEC_INST__SORT_ASCENDING(tcomp_udvm_t *udvm, uint16_t start, uint16_t n, uint16_t k)
{
	int segfault = 0;
	uint16_t* list_temp = 0;
	IndexValuePair_t *list1_values = 0;
	uint16_t list_index, list_el;
	int j, pos;
	
	CONSUME_CYCLES(( 1 + k *(CEILLINGLOG2(k) + n) )); /* 1 + k * (ceiling(log2(k)) + n) */

	if(TCOMP_UDVM_GET_SIZE() <= (size_t)(start+(n*k*2)) ){ segfault = 1; goto __SEGFAULT; };

	//
	// Create FirstList with key-value pairs
	//
	list1_values = (IndexValuePair_t*)tsk_calloc(sizeof(IndexValuePair_t), k);
	if(!list1_values) { segfault = 1; goto __SEGFAULT; };
	for(j=0, pos=0; pos<k; j+=2,pos++)
	{
		list1_values[pos].index = pos;
		list1_values[pos].value = *((uint16_t*)TCOMP_UDVM_GET_BUFFER_AT(start+j));
	}

	/*
	* Sort Fisrt List Values
	*/
	qsort(list1_values, k, sizeof(IndexValuePair_t), SortAscendingPredicate); 

	/* Sort all lists */
	list_temp = tsk_calloc(sizeof(uint16_t), k);
	if(!list1_values) { segfault = 1; goto __SEGFAULT; };
	for(list_index = 0; list_index < n; list_index++)
	{
		uint16_t* list_start = (uint16_t*)TCOMP_UDVM_GET_BUFFER_AT( start + (list_index*k*2) );
		memmove(list_temp, list_start, k*2);
		for(list_el=0; list_el<k; list_el++)
		{
			list_start[(list_el)] = list_temp[ list1_values[list_el].index ];
		}
	}

	TSK_FREE(list_temp);
	TSK_FREE(list1_values);

__SEGFAULT:
	if(segfault)
	{
		tcomp_udvm_createNackInfo2(udvm, NACK_SEGFAULT);
		return 0;
	}

	return 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @fn	int TCOMP_UDVM_EXEC_INST__SORT_ASCENDING(tcomp_udvm_t *udvm, uint16_t start, uint16_t n,
/// 	uint16_t k)
///
/// @brief	SORT-DESCENDING (%start, %n, %k)
/// Reference:  RFC3320 Section 9.1.3
///
/// This instruction sort lists of 2-byte words in descending order.
///
/// @author	Mamadou
/// @date	11/26/2009
///
/// @param [in,out]	udvm	If non-null, the udvm. 
/// @param	start			The starting memory address of the block of data to be sorted. 
/// @param	n				Number of lists. 
/// @param	k				Lists length (2-byte words). 
///
/// @return	True if succeed, otherwise return false.
////////////////////////////////////////////////////////////////////////////////////////////////////
int TCOMP_UDVM_EXEC_INST__SORT_DESCENDING(tcomp_udvm_t *udvm, uint16_t start, uint16_t n, uint16_t k)
{
	int segfault = 0;
	uint16_t* list_temp = 0;
	IndexValuePair_t *list1_values = 0;
	uint16_t list_index, list_el;
	int j, pos;
	
	CONSUME_CYCLES(( 1 + k *(CEILLINGLOG2(k) + n) )); /* 1 + k * (ceiling(log2(k)) + n) */

	if(TCOMP_UDVM_GET_SIZE() <= (size_t)(start+(n*k*2)) ){ segfault = 1; goto __SEGFAULT; };

	//
	// Create FirstList with key-value pairs
	//
	list1_values = (IndexValuePair_t*)tsk_calloc(sizeof(IndexValuePair_t), k);
	if(!list1_values) { segfault = 1; goto __SEGFAULT; };
	for(j=0, pos=0; pos<k; j+=2,pos++)
	{
		list1_values[pos].index = pos;
		list1_values[pos].value = *((uint16_t*)TCOMP_UDVM_GET_BUFFER_AT(start+j));
	}

	/*
	* Sort Fisrt List Values
	*/
	qsort(list1_values, k, sizeof(IndexValuePair_t), SortDescendingPredicate); 

	/* Sort all lists */
	list_temp = tsk_calloc(sizeof(uint16_t), k);
	if(!list1_values) { segfault = 1; goto __SEGFAULT; };
	for(list_index = 0; list_index < n; list_index++)
	{
		uint16_t* list_start = (uint16_t*)TCOMP_UDVM_GET_BUFFER_AT( start + (list_index*k*2) );
		memmove(list_temp, list_start, k*2);
		for(list_el=0; list_el<k; list_el++)
		{
			list_start[(list_el)] = list_temp[ list1_values[list_el].index ];
		}
	}

	TSK_FREE(list_temp);
	TSK_FREE(list1_values);

__SEGFAULT:
	if(segfault)
	{
		tcomp_udvm_createNackInfo2(udvm, NACK_SEGFAULT);
		return 0;
	}

	return 1;
}

int TCOMP_UDVM_EXEC_INST__SHA_1(tcomp_udvm_t *udvm, uint16_t position, uint16_t length, uint16_t destination)
{
	return 0;
}

int TCOMP_UDVM_EXEC_INST__LOAD(tcomp_udvm_t *udvm, uint16_t address, uint16_t value)
{
	return 0;
}

int TCOMP_UDVM_EXEC_INST__MULTILOAD(tcomp_udvm_t *udvm, uint16_t address, uint16_t n)
{
	return 0;
}

int TCOMP_UDVM_EXEC_INST__PUSH(tcomp_udvm_t *udvm, int16_t value)
{
	return 0;
}


int TCOMP_UDVM_EXEC_INST__POP(tcomp_udvm_t *udvm, uint16_t* value)
{
	return 0;
}


int TCOMP_UDVM_EXEC_INST__COPY(tcomp_udvm_t *udvm, uint16_t position, uint16_t length, uint16_t destination)
{
	return 0;
}

int TCOMP_UDVM_EXEC_INST__COPY_LITERAL(tcomp_udvm_t *udvm, uint16_t position, uint16_t length, uint16_t destination)
{
	return 0;
}

int TCOMP_UDVM_EXEC_INST__COPY_OFFSET(tcomp_udvm_t *udvm, uint16_t offset, uint16_t length, uint16_t destination)
{
	return 0;
}

int TCOMP_UDVM_EXEC_INST__MEMSET(tcomp_udvm_t *udvm, uint16_t address, uint16_t length, uint16_t start_value, uint16_t offset)
{
	return 0;
}

/**
* JUMP (@address)
* Reference:  RFC3320 Section 9.3.1
* This instruction moves program execution to the specified memory address.
* Decompression failure occurs if the value of the address operand lies
*    beyond the overall UDVM memory size.
* @param address defines the address to jump to
* @returns true if succeed, otherwise return false
*/
int TCOMP_UDVM_EXEC_INST__JUMP(tcomp_udvm_t *udvm, int16_t address)
{
	int callback = (address >=0 );
	CONSUME_CYCLES(callback?0:1);

	if(!callback)
	{
		address = tcomp_udvm_opget_address_param(udvm, udvm->last_memory_address_of_instruction);
	}

	if(address > (int16_t)TCOMP_UDVM_GET_SIZE())
	{
		tcomp_udvm_createNackInfo2(udvm, NACK_SEGFAULT);
		return 0;
	}
	udvm->executionPointer = address;

	return 1; 
}


int TCOMP_UDVM_EXEC_INST__COMPARE(tcomp_udvm_t *udvm, uint16_t value_1, uint16_t value_2, uint16_t address_1, uint16_t address_2, uint16_t address_3)
{
	return 0;
}

int TCOMP_UDVM_EXEC_INST__CALL(tcomp_udvm_t *udvm, uint16_t address)
{
	return 0;
}

int TCOMP_UDVM_EXEC_INST__RETURN(tcomp_udvm_t *udvm)
{
	return 0;
}

int TCOMP_UDVM_EXEC_INST__SWITCH(tcomp_udvm_t *udvm, uint16_t n, uint16_t j)
{
	return 0;
}

int TCOMP_UDVM_EXEC_INST__CRC(tcomp_udvm_t *udvm, uint16_t value, uint16_t position, uint16_t length, uint16_t address)
{
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @fn	int TCOMP_UDVM_EXEC_INST__INPUT_BYTES(tcomp_udvm_t *udvm, uint16_t length,
/// 	uint16_t destination, uint16_t address)
///
/// @brief	INPUT-BITS (%length, %destination, @address)
/// Reference:  RFC3320 Section 9.4.2
/// This instruction requests a certain number of bytes of compressed data from the decompressor dispatcher. 
///
/// @author	Mamadou
/// @date	11/26/2009
///
/// @param [in,out]	udvm	If non-null, the udvm. 
/// @param	length			Indicates the requested number of bytes of compressed data. 
/// @param	destination		Specifies the starting memory address to which they should be copied. 
/// @param	address			Defines the address to jump to if the instruction requests data that lies beyond the end of the SigComp message. 
///
/// @return	True if succeed, otherwise return false.
////////////////////////////////////////////////////////////////////////////////////////////////////
int TCOMP_UDVM_EXEC_INST__INPUT_BYTES(tcomp_udvm_t *udvm, uint16_t length, uint16_t destination, uint16_t address)
{
	int ok = 1;
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
	if(compressedDataAddress)
	{
		ok &= tcomp_udvm_bytecopy_to(udvm, destination, compressedDataAddress, length);
		if(ok)
		{
			/* (8 * n + 1000) * cycles_per_bit */
			udvm->maximum_UDVM_cycles += ((8*length+1000) * udvm->stateHandler->sigcomp_parameters->cpbCode);
		}
	}
	else
	{
		/*
		* If the instruction requests data that lies beyond the end of the
		* SigComp message, no data is returned.  Instead the UDVM moves program
		* execution to the address specified by the address operand.
		*/
		ok &= TCOMP_UDVM_EXEC_INST__JUMP(udvm, address);
	}

	return ok;
}

int TCOMP_UDVM_EXEC_INST__INPUT_BITS(tcomp_udvm_t *udvm, uint16_t length, uint16_t destination, uint16_t address)
{
	return 0;
}

int TCOMP_UDVM_EXEC_INST__INPUT_HUFFMAN(tcomp_udvm_t *udvm, uint16_t destination, uint16_t address, uint16_t n)
{
	return 0;
}

int TCOMP_UDVM_EXEC_INST__STATE_ACCESS(tcomp_udvm_t *udvm, uint16_t partial_identifier_start, uint16_t partial_identifier_length, uint16_t state_begin, uint16_t state_length, uint16_t state_address, uint16_t state_instruction)
{
	return 0;
}

int TCOMP_UDVM_EXEC_INST__STATE_CREATE(tcomp_udvm_t *udvm, uint16_t state_length, uint16_t state_address, uint16_t state_instruction, uint16_t minimum_access_length, uint16_t state_retention_priority)
{
	return 0;
}

int TCOMP_UDVM_EXEC_INST__STATE_FREE(tcomp_udvm_t *udvm, uint16_t partial_identifier_start, uint16_t partial_identifier_length)
{
	return 0;
}

/**
* OUTPUT (%output_start, %output_length)
* Reference:  RFC3320 Section 9.4.8
* This instruction provides successfully decompressed data to the dispatcher.
* @param output_start defines the starting memory address of the byte string to be provided to the dispatcher
* @param output_length defines the length of the byte string to be provided to the dispatcher
* @retval true if succeed, otherwise return false
*/
int TCOMP_UDVM_EXEC_INST__OUTPUT(tcomp_udvm_t *udvm, uint16_t output_start, uint16_t output_length)
{
	int ok;
	size_t *outputbuffer_size;

	CONSUME_CYCLES(1+output_length);

	outputbuffer_size = tcomp_buffer_getIndexBytes(udvm->lpResult->output_buffer);
	if( (*outputbuffer_size + output_length) > 65536 )
	{
		/*
		* Decompression failure occurs if the cumulative number of bytes
		* provided to the dispatcher exceeds 65536 bytes.
		*/
		tcomp_udvm_createNackInfo2(udvm, NACK_OUTPUT_OVERFLOW);
		return 0;
	}

	// FIXME: do it once?
	ok = tcomp_udvm_bytecopy_from(udvm, tcomp_buffer_getBufferAtPos(udvm->lpResult->output_buffer, *outputbuffer_size), output_start,  output_length);
	*outputbuffer_size += output_length;

#if DEBUG || _DEBUG
	tcomp_buffer_nprint(udvm->lpResult->output_buffer, *outputbuffer_size);
#endif
	return ok;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @fn	int TCOMP_UDVM_EXEC_INST__END_MESSAGE(tcomp_udvm_t *udvm,
/// 	uint16_t requested_feedback_location, uint16_t returned_parameters_location,
/// 	uint16_t state_length, uint16_t state_address, uint16_t state_instruction,
/// 	uint16_t minimum_access_length, uint16_t state_retention_priority)
///
/// @brief	This instruction successfully terminates the UDVM and forwards the state creation and state free requests to the state
/// handler together with any supplied feedback data.
/// END-MESSAGE (%requested_feedback_location, %returned_parameters_location, %state_length, %state_address, %state_instruction, %minimum_access_length, %state_retention_priority)
/// Reference:  RFC3320 Section 9.4.9
///
/// @author	Mamadou
/// @date	11/26/2009
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
/// @return	True if succeed, otherwise return false. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int TCOMP_UDVM_EXEC_INST__END_MESSAGE(tcomp_udvm_t *udvm, uint16_t requested_feedback_location, uint16_t returned_parameters_location, uint16_t state_length, uint16_t state_address, uint16_t state_instruction, uint16_t minimum_access_length, uint16_t state_retention_priority)
{
	size_t udvm_size;

	CONSUME_CYCLES(1+state_length);

	udvm_size = TCOMP_UDVM_GET_SIZE();

	/*
	*	Create temporary state provided by END_MESSAGE?
	*/
	if(!tcomp_udvm_createTempState(udvm, state_length, state_address, state_instruction, minimum_access_length, state_retention_priority, 1))
	{
		return 0;
	}

	/*
	* Byte copy all waiting STATE-FREE/STATE-CREATE/END-MESSAGE states
	*/
	if(!tcomp_udvm_byteCopy_TempStates(udvm))
	{
		tcomp_udvm_createNackInfo2(udvm, NACK_INTERNAL_ERROR);
		return 0;
	}

	/*
	*	Feedback has been requested?
	*/
	if(requested_feedback_location)
	{
		uint8_t r_f_l;
		if(requested_feedback_location >= udvm_size)
		{
			tcomp_udvm_createNackInfo2(udvm, NACK_SEGFAULT);
			return 0;
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
		if(udvm->lpResult->req_feedback->Q)
		{
			/* we have a requested feedback item */
			uint8_t r_f_i = *TCOMP_UDVM_GET_BUFFER_AT(requested_feedback_location);
			uint8_t length = 1; /* [1-128] */
			if(r_f_i & 0x80)
			{
				/* case 2 */
				length += (r_f_i & 0x7f); /* seven last bits */
			}

			if(requested_feedback_location >= TCOMP_UDVM_GET_SIZE())
			{
				tcomp_udvm_createNackInfo2(udvm, NACK_SEGFAULT);
				return 0;
			}
			/* copy from udvm */
			// FIXME: use realloc
			tcomp_buffer_freeBuff(udvm->lpResult->req_feedback->item);
			tcomp_buffer_allocBuff(udvm->lpResult->req_feedback->item, length);
			tcomp_udvm_bytecopy_from(udvm, tcomp_buffer_getBuffer(udvm->lpResult->req_feedback->item), requested_feedback_location, length);
		}
	}

	//
	//	SigComp parameters have been returned?
	//
	if(returned_parameters_location)
	{
		uint8_t r_p_l, SigComp_version;
		uint16_t index;
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

		if(returned_parameters_location >= udvm_size)
		{
			tcomp_udvm_createNackInfo2(udvm, NACK_SEGFAULT);
			return 0;
		}

		/* cpb+dms+sms */
		r_p_l = *TCOMP_UDVM_GET_BUFFER_AT(returned_parameters_location);
		returned_parameters_location++;
		if(r_p_l)
		{
			tcomp_params_setCpbCode(udvm->lpResult->remote_parameters, ((r_p_l & 0xc0)>>6));
			tcomp_params_setDmsCode(udvm->lpResult->remote_parameters, ((r_p_l & 0x38)>>3));
			tcomp_params_setSmsCode(udvm->lpResult->remote_parameters, (r_p_l & 0x07));
		}
		/* sigcomp version */
		SigComp_version = *TCOMP_UDVM_GET_BUFFER_AT(returned_parameters_location);
		returned_parameters_location++;
		if(SigComp_version)
		{
			udvm->lpResult->remote_parameters->SigComp_version = SigComp_version;
		}
		/* state items */
		for(index = returned_parameters_location; index <(udvm_size-1); )
		{
			uint8_t length = *TCOMP_UDVM_GET_BUFFER_AT(index); // 1-byte
			if(length<6 || length>20) break;
			index++;
			if((index+length) >= (uint16_t)udvm_size)
			{
				tcomp_udvm_createNackInfo2(udvm, NACK_SEGFAULT);
				return 0;
			}
			partial_id = TCOMP_BUFFER_CREATE();
			tcomp_buffer_allocBuff(partial_id, length);
			tcomp_udvm_bytecopy_from(udvm, tcomp_buffer_getBuffer(partial_id), index, length);
			tsk_list_add_data(udvm->lpResult->remote_parameters->returnedStates, (void**)&partial_id);
			index += length;
		}
	}

	return 1; 
}
