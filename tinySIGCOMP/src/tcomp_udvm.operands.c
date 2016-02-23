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

/**@file tcomp_udvm.operands.c
 * @brief  SigComp UDVM machine (Operands).
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *

 */
#include "tcomp_udvm.h"

#include "tsk_debug.h"

#include <math.h>

/**
literal (#)<br>
<table>
<tr> <td>Bytecode</td>                       <td>Operand value</td>		<td>Range</td></tr>
<tr> <td>0nnnnnnn</td>                        <td>N</td>                  <td>0 - 127</td></tr>
<tr> <td>10nnnnnn nnnnnnnn</td>               <td>N</td>                  <td>0 - 16383</td></tr>
<tr> <td>11000000 nnnnnnnn nnnnnnnn</td>      <td>N</td>                  <td>0 - 65535</td></tr>
</table>
*/
uint32_t tcomp_udvm_opget_literal_param(tcomp_udvm_t *udvm)
{
    uint32_t result = 0;
    const uint8_t* memory_ptr = TCOMP_UDVM_GET_BUFFER_AT(udvm->executionPointer);

    switch( *memory_ptr & 0xc0) { // 2 first bits
    case 0x00: // 0nnnnnnn                        N                   0 - 127
    case 0x40: { // 0nnnnnnn                        N                   0 - 127
        result = *(memory_ptr);
        udvm->executionPointer++;
    }
    break;

    case 0x80: { // 10nnnnnn nnnnnnnn               N                   0 - 16383
        result = TSK_BINARY_GET_2BYTES(memory_ptr)&0x3fff; // All except 2 first bits
        udvm->executionPointer+=2;
    }
    break;

    case 0xc0: { // 11000000 nnnnnnnn nnnnnnnn      N                   0 - 65535
        result = TSK_BINARY_GET_2BYTES((memory_ptr+1));
        udvm->executionPointer+=3;
    }
    break;

    default: {
        TSK_DEBUG_ERROR("Invalide opcode: %u", *memory_ptr);
        tcomp_udvm_createNackInfo2(udvm, NACK_INVALID_OPERAND);
    }
    break;
    }
    return result;
}

/**
reference ($)<br>
<table>
<tr><td>Bytecode</td>                       <td>Operand value</td>      <td>Range</td></tr>
<tr><td>0nnnnnnn</td>                        <td>memory[2 * N]</td>       <td>0 - 65535</td></tr>
<tr><td>10nnnnnn nnnnnnnn </td>              <td>memory[2 * N]</td>       <td>0 - 65535</td></tr>
<tr><td>11000000 nnnnnnnn nnnnnnnn</td>      <td>memory[N]</td>           <td>0 - 65535</td></tr>
</table>
*/
uint32_t tcomp_udvm_opget_reference_param(tcomp_udvm_t *udvm)
{
    const uint8_t* memory_ptr = TCOMP_UDVM_GET_BUFFER_AT(udvm->executionPointer);
    uint32_t result = 0;

    switch( *memory_ptr & 0xc0) { // 2 first bits
    case 0x00: // 0nnnnnnn                        memory[2 * N]       0 - 65535
    case 0x40: { // 0nnnnnnn                        memory[2 * N]       0 - 65535
        uint8_t N = (*(memory_ptr) & 0x7f); // no effect first bit is already nil
        result = 2*N;
        udvm->executionPointer++;
    }
    break;

    case 0x80: { // 10nnnnnn nnnnnnnn               memory[2 * N]       0 - 65535
        uint32_t N = (TSK_BINARY_GET_2BYTES(memory_ptr) & 0x3fff);
        result = 2*N;
        udvm->executionPointer+=2;
    }
    break;

    case 0xc0: { // 11000000 nnnnnnnn nnnnnnnn      memory[N]           0 - 65535
        uint32_t N = TSK_BINARY_GET_2BYTES(memory_ptr+1);
        result = N;
        udvm->executionPointer+=3;
    }
    break;

    default: {
        TSK_DEBUG_ERROR("Invalide opcode: %u", *memory_ptr);
        tcomp_udvm_createNackInfo2(udvm, NACK_INVALID_OPERAND);
    }
    break;
    }

    return result;
}

/**
multitype(%)<br>
<table>
<tr><td>Bytecode</td>               <td>Operand value</td>     <td>Range</td></tr>
<tr><td>00nnnnnn</td>                        <td>N</td>					<td>0 - 63</td></tr>
<tr><td>01nnnnnn</td>                        <td>memory[2 * N]</td>       <td>0 - 65535</td></tr>
<tr><td>1000011n</td>                        <td>2 ^ (N + 6)</td>			<td>64 , 128</td></tr>
<tr><td>10001nnn</td>                        <td>2 ^ (N + 8)</td>			<td>256 , ... , 32768</td></tr>
<tr><td>111nnnnn</td>						<td>N + 65504</td>			<td>65504 - 65535</td></tr>
<tr><td>1001nnnn nnnnnnnn</td>               <td>N + 61440</td>			<td>61440 - 65535</td></tr>
<tr><td>101nnnnn nnnnnnnn</td>              <td>N</td>					<td>0 - 8191</td></tr>
<tr><td>110nnnnn nnnnnnnn</td>				<td>memory[N]</td>           <td>0 - 65535</td></tr>
<tr><td>10000000 nnnnnnnn nnnnnnnn</td>      <td>N</td>                   <td>0 - 65535</td></tr>
<tr><td>10000001 nnnnnnnn nnnnnnnn</td>      <td>memory[N]</td>           <td>0 - 65535</td></tr>
</table>
*/
uint32_t tcomp_udvm_opget_multitype_param(tcomp_udvm_t *udvm)
{
    const uint8_t* memory_ptr = TCOMP_UDVM_GET_BUFFER_AT(udvm->executionPointer);
    int8_t index = operand_multitype_indexes[*memory_ptr];
    uint32_t result = 0;

    switch(index) {
    case 1: { // 00nnnnnn                        N                   0 - 63
        result = *(memory_ptr);
        udvm->executionPointer++;
    }
    break;

    case 2: { // 01nnnnnn                        memory[2 * N]       0 - 65535
        uint8_t N = (*(memory_ptr) & 0x3f);
        result = TSK_BINARY_GET_2BYTES( TCOMP_UDVM_GET_BUFFER_AT(2*N) );
        udvm->executionPointer++;
    }
    break;

    case 3: { // 1000011n                        2 ^ (N + 6)        64 , 128
        uint8_t N = (*(memory_ptr) & 0x01);
        result = (uint32_t)pow( (double)2, (N + 6) );
        udvm->executionPointer++;
    }
    break;

    case 4: { // 10001nnn                        2 ^ (N + 8)    256 , ... , 32768
        uint8_t N = (*(memory_ptr) & 0x07);
        result = (uint32_t)pow( (double)2, (N + 8) );
        udvm->executionPointer++;
    }
    break;

    case 5: { // 111nnnnn                        N + 65504       65504 - 65535
        result = ((*(memory_ptr) & 0x1f) + 65504 );
        udvm->executionPointer++;
    }
    break;

    case 6: { // 1001nnnn nnnnnnnn               N + 61440       61440 - 65535
        result = (TSK_BINARY_GET_2BYTES(memory_ptr) & 0x0fff) + 61440;
        udvm->executionPointer+=2;
    }
    break;

    case 7: { // 101nnnnn nnnnnnnn               N                   0 - 8191
        result = (TSK_BINARY_GET_2BYTES(memory_ptr) & 0x1fff);
        udvm->executionPointer+=2;
    }
    break;

    case 8: { // 110nnnnn nnnnnnnn               memory[N]           0 - 65535
        uint32_t N = TSK_BINARY_GET_2BYTES(memory_ptr) & 0x1fff;
        result = TSK_BINARY_GET_2BYTES( TCOMP_UDVM_GET_BUFFER_AT(N) );
        udvm->executionPointer+=2;
    }
    break;

    case 9: { // 10000000 nnnnnnnn nnnnnnnn      N                   0 - 65535
        result = TSK_BINARY_GET_2BYTES(memory_ptr+1);
        udvm->executionPointer+=3;
    }
    break;

    case 10: { // 10000001 nnnnnnnn nnnnnnnn      memory[N]           0 - 65535
        uint32_t N = TSK_BINARY_GET_2BYTES(memory_ptr+1);
        result = TSK_BINARY_GET_2BYTES( TCOMP_UDVM_GET_BUFFER_AT(N) );
        udvm->executionPointer+=3;
    }
    break;

    default: { // -1
        TSK_DEBUG_ERROR("Invalide opcode: %u", *memory_ptr);
        tcomp_udvm_createNackInfo2(udvm, NACK_INVALID_OPERAND);
    }
    break;
    }

    return result;
}

/**
address(@)
This operand is decoded as a multitype operand followed by a further step: the memory address
of the UDVM instruction containing the address operand is added to
obtain the correct operand value.  So if the operand value from
Figure 10 is D then the actual operand value of an address is
calculated as follows:

   operand_value = (memory_address_of_instruction + D) modulo 2^16
*/
uint32_t tcomp_udvm_opget_address_param(tcomp_udvm_t *udvm, uint32_t memory_address_of_instruction)
{
    uint32_t D = tcomp_udvm_opget_multitype_param(udvm);
    // (2^16) => 65536;
    return ( (memory_address_of_instruction + D)%65536 );
}
