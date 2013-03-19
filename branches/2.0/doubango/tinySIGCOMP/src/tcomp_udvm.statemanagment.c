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

/**@file tcomp_udvm.statemanagment.c
 * @brief  SigComp UDVM machine (State managment)
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *

 */
#include "tcomp_udvm.h"


int tcomp_udvm_byteCopy_TempStates(tcomp_udvm_t *udvm)
{
	int ok = 1;
	uint8_t i;
	uint8_t tocreate_size = tcomp_result_getTempStatesToCreateSize(udvm->lpResult);
	uint8_t tofree_size = tcomp_result_getTempStatesToFreeSize(udvm->lpResult);
	
	
	/*
	* State Create
	*/
	for(i = 0; i < tocreate_size && ok; i++)
	{
		/*
		* The UDVM byte copies a string of state_length bytes from the UDVM
		* memory beginning at state_address (obeying the rules of Section 8.4).
		* This is the state_value.
		*/
		tcomp_state_t* lpState = udvm->lpResult->statesToCreate[i];
		if(lpState->length){
			tcomp_buffer_allocBuff(lpState->value, lpState->length);
		}

		ok &= tcomp_udvm_bytecopy_from(udvm, tcomp_buffer_getBuffer(lpState->value), lpState->address, lpState->length);
	}
	
	/*
	* State Free
	*/
	for(i = 0; i<tofree_size && ok; i++){
		tcomp_tempstate_to_free_t *lpDesc = udvm->lpResult->statesToFree[i];
		tcomp_buffer_allocBuff(lpDesc->identifier, lpDesc->partial_identifier_length);
		ok &= tcomp_udvm_bytecopy_from(udvm, tcomp_buffer_getBuffer(lpDesc->identifier), lpDesc->partial_identifier_start, lpDesc->partial_identifier_length);
	}
	return ok;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief	Creates temporary state.
///
/// @param [in,out]	udvm				If non-null, the udvm. 
/// @param	state_length				Length of the state. 
/// @param	state_address				The state address. 
/// @param	state_instruction			The state instruction. 
/// @param	minimum_access_length		Length of the minimum access. 
/// @param	state_retention_priority	The state retention priority. 
/// @param	end_msg						Message describing the end. 
///
/// @return	1 if succeed an zero otherwise.
////////////////////////////////////////////////////////////////////////////////////////////////////

int tcomp_udvm_createTempState(tcomp_udvm_t *udvm, uint32_t state_length, uint32_t state_address, uint32_t state_instruction, 
	uint32_t minimum_access_length, uint32_t state_retention_priority, int end_msg)
{
	/*
	* If the specified minimum_access_length does not lie between 6 and 20 inclusive, or if
	* the state_retention_priority is 65535 then the END-MESSAGE
	* instruction fails to make a state creation request of its own
	* (however decompression failure does not occur and the state creation
	* requests made by the STATE-CREATE instruction are still valid).
	*/
	int is_ok = ( (6<=minimum_access_length && minimum_access_length<=20) && state_retention_priority!=65535 );

	// if not ok and not END_MESSAGE --> decompression failure MUST occurs
	if(!is_ok)
	{
		if(end_msg) return 1;

		if(state_retention_priority == 65535)
		{
			tcomp_udvm_createNackInfo2(udvm, NACK_INVALID_STATE_PRIORITY);
		}
		else
		{
			tcomp_udvm_createNackInfo2(udvm, NACK_INVALID_STATE_ID_LENGTH);
		}
		return 0;
	}

	/*
	* decompression failure occurs if the END-MESSAGE instruction makes a state creation request and four
	* instances of the STATE-CREATE instruction have already been encountered.
	*/
	if(tcomp_result_getTempStatesToCreateSize(udvm->lpResult) >= MAX_TEMP_SATES)
	{
		tcomp_udvm_createNackInfo2(udvm, NACK_TOO_MANY_STATE_REQUESTS);
		return 0;
	}

	/*
	*	Is there a state to create?
	*/
	if(is_ok)
	{
		// no byte copy ()
		tcomp_state_t *lpState =  tcomp_state_create(state_length, state_address, state_instruction, minimum_access_length, state_retention_priority);
		tcomp_result_addTempStateToCreate(udvm->lpResult, lpState);
	}

	return 1;
}
