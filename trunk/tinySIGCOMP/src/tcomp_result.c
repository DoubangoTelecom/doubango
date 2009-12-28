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

/**@file tcomp_result.c
 * @brief  SIGCOMP decompresion result.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tcomp_result.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

/**@defgroup tcomp_result_group SIGCOMP decompresion result.
*/

/**@ingroup tcomp_result_group
*/
void _tcomp_result_reset(tcomp_result_t *result, int isDestructor, int isResetOutput)
{
	if(result)
	{
		uint8_t i;
		for(i = 0; i < result->statesToCreateIndex; i++)
		{
			result->statesToCreate[i] = TCOMP_STATE_SAFE_FREE(result->statesToCreate[i]);
		}

		for(i = 0; i < result->statesToFreeIndex; i++)
		{
			result->statesToFree[i] = TCOMP_TEMPSTATE_TO_FREE_SAFE_FREE(result->statesToFree[i]);
		}

		if(!isDestructor)
		{
			result->statesToCreateIndex = 0;
			result->statesToFreeIndex = 0;
			result->consumed_cycles = 0;

			tcomp_params_reset(result->remote_parameters);
			
			if(isResetOutput)
			{
				tcomp_buffer_reset(result->output_buffer);
			}
			
			tcomp_reqfeed_reset(result->req_feedback);
			tcomp_buffer_freeBuff(result->ret_feedback);

			result->isNack = 0;
			tcomp_buffer_freeBuff(result->nack_info);
		}
	}
	else
	{
		TSK_DEBUG_ERROR("NULL SigComp result.");
	}
}

/**@ingroup tcomp_result_group
*/
void tcomp_result_setOutputBuffer(tcomp_result_t *result, void *output_ptr, size_t output_size, int isStream, uint64_t streamId)
{
	if(result)
	{
		tcomp_buffer_referenceBuff(result->output_buffer, (uint8_t*)output_ptr, output_size);
		result->isStreamBased = isStream;
		result->streamId = streamId;
	}
	else
	{
		TSK_DEBUG_ERROR("NULL SigComp result.");
	}
}

/**@ingroup tcomp_result_group
*/
void tcomp_result_setCompartmentId(tcomp_result_t *result, const void *id, size_t len)
{
	if(result)
	{
		result->compartmentId = tcomp_buffer_createHash(id, len);
	}
	else
	{
		TSK_DEBUG_ERROR("NULL SigComp result.");
	}
}

/**@ingroup tcomp_result_group
*/
void tcomp_result_addTempStateToCreate(tcomp_result_t *result, tcomp_state_t* lpState)
{
	if(result)
	{
		/*
		* Note that there is a maximum limit of four state creation requestsper instance of the UDVM.
		*/
		if(result->statesToCreateIndex >= MAX_TEMP_SATES) 
		{
			return;
		}
					
		// Add state
		result->statesToCreate[result->statesToCreateIndex++] = lpState;
	}
	else
	{
		TSK_DEBUG_ERROR("NULL SigComp result.");
	}
}

/**@ingroup tcomp_result_group
*/
uint8_t tcomp_result_getTempStatesToCreateSize(const tcomp_result_t *result)
{
	if(result)
	{
		return result->statesToCreateIndex;
	}
	else
	{
		TSK_DEBUG_ERROR("NULL SigComp result.");
	}
	return 0;
}

/**@ingroup tcomp_result_group
*/
void tcomp_result_addTempStateToFree(tcomp_result_t *result, tcomp_tempstate_to_free_t* lpDesc)
{
	if(result)
	{
		/*
		* Decompression failure MUST occur if more than four state free
		* requests are made before the END-MESSAGE instruction is encountered.
		*/
		if(result->statesToFreeIndex >= MAX_TEMP_SATES) 
		{
			return;
		}
		result->statesToFree[result->statesToFreeIndex++] = lpDesc;
	}
	else
	{
		TSK_DEBUG_ERROR("NULL SigComp result.");
	}
}

/**@ingroup tcomp_result_group
*/
uint8_t tcomp_result_getTempStatesToFreeSize(const tcomp_result_t *result)
{
	if(result)
	{
		return result->statesToFreeIndex;
	}
	else
	{
		TSK_DEBUG_ERROR("NULL SigComp result.");
	}
	return 0;
}





//========================================================
//	SigComp result object definition
//

/**@ingroup tcomp_result_group
*/
static void* tcomp_result_create(void *self, va_list * app)
{
	tcomp_result_t* result = self;

	if(result)
	{
		result->output_buffer = TCOMP_BUFFER_CREATE();
		result->ret_feedback = TCOMP_BUFFER_CREATE();
		result->nack_info = TCOMP_BUFFER_CREATE();

		result->remote_parameters = TCOMP_PARAMS_CREATE();

		result->req_feedback = TCOMP_REQFEED_CREATE();
		
	}

	return self;
}

static void* tcomp_result_destroy(void * self)
{
	tcomp_result_t* result = self;

	if(result)
	{
		_tcomp_result_reset(result, 1, 1);
		TCOMP_BUFFER_SAFE_FREE(result->output_buffer);
		TCOMP_BUFFER_SAFE_FREE(result->ret_feedback);
		TCOMP_BUFFER_SAFE_FREE(result->nack_info);

		TCOMP_PARAMS_SAFE_FREE(result->remote_parameters);

		TCOMP_REQFEED_SAFE_FREE(result->req_feedback);
	}

	return self;
}

static const tsk_object_def_t tcomp_result_def_s = 
{
	sizeof(tcomp_result_t),
	tcomp_result_create, 
	tcomp_result_destroy,
	0
};
const void *tcomp_result_def_t = &tcomp_result_def_s;


//========================================================
//	SigComp temporary state object definition
//

/**@ingroup tcomp_result_group
*/
static void* tcomp_tempstate_to_free_create(void *self, va_list * app)
{
	tcomp_tempstate_to_free_t* tempstate_to_free = self;

	if(tempstate_to_free)
	{
		
		tempstate_to_free->identifier = TCOMP_BUFFER_CREATE();
	}

	return self;
}

static void* tcomp_tempstate_to_free_destroy(void * self)
{
	tcomp_tempstate_to_free_t* tempstate_to_free = self;

	if(tempstate_to_free)
	{
		TCOMP_BUFFER_SAFE_FREE(tempstate_to_free->identifier);
	}

	return self;
}

static const tsk_object_def_t tcomp_tempstate_to_free_def_s = 
{
	sizeof(tcomp_tempstate_to_free_t),
	tcomp_tempstate_to_free_create, 
	tcomp_tempstate_to_free_destroy,
	0
};
const void *tcomp_tempstate_to_free_def_t = &tcomp_tempstate_to_free_def_s;
