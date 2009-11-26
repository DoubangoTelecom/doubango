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

/**@file tcomp_result.h
 * @brief  SIGCOMP decompresion result.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TCOMP_RESULT_H
#define TCOMP_RESULT_H

#include "tinysigcomp_config.h"
#include "tcomp_state.h"
#include "tcomp_reqfeed.h"
#include "tcomp_params.h"
#include "tcomp_buffer.h"

#include "tsk_object.h"

#define MAX_TEMP_SATES	4

#define TCOMP_RESULT_CREATE()						tsk_object_new(tcomp_result_def_t)
#define TCOMP_RESULT_SAFE_FREE(self)				tsk_object_unref(self)

#define TCOMP_TEMPSTATE_TO_FREE_CREATE()			tsk_object_new(tcomp_tempstate_to_free_def_t)
#define TCOMP_TEMPSTATE_TO_FREE_SAFE_FREE(self)		tsk_object_unref(self)

typedef struct tcomp_tempstate_to_free_s
{
	TSK_DECLARE_OBJECT;

	// Operands
	uint16_t partial_identifier_start;
	uint16_t partial_identifier_length;
	// identifier
	tcomp_buffer_handle_t *identifier;
}
tcomp_tempstate_to_free_t;

TINYSIGCOMP_API const void *tcomp_tempstate_to_free_def_t;

/**
* SigComp decompression result.
*/
typedef struct tcomp_result_s
{
	TSK_DECLARE_OBJECT;

	uint64_t					compartmentId;
	tcomp_state_t*				statesToCreate[MAX_TEMP_SATES];
	uint8_t						statesToCreateIndex;

	tcomp_tempstate_to_free_t*	statesToFree[MAX_TEMP_SATES];
	uint8_t						statesToFreeIndex;

	uint64_t					streamId;
	unsigned					isStreamBased:1;
	tcomp_buffer_handle_t		*output_buffer;

	tcomp_reqfeed_t				*req_feedback;
	tcomp_params_t				*remote_parameters;
	tcomp_buffer_handle_t		*ret_feedback;

	unsigned					isNack:1;
	tcomp_buffer_handle_t		*nack_info;
}
tcomp_result_t;


void _tcomp_result_reset(tcomp_result_t *result, int isDestructor, int isResetOutput);
#define tcomp_result_reset(result) _tcomp_result_reset((tcomp_result_t *)result, 0, 1)

TINYSIGCOMP_API void tcomp_result_setOutputBuffer(tcomp_result_t *result, void *output_ptr, size_t output_size, int isStream, uint64_t streamId);
#define tcomp_result_setOutputUDPBuffer(result, output_ptr, output_size) tcomp_result_setOutputBuffer((tcomp_result_t *)result, (void *)output_ptr, (size_t) output_size, 0, 0)
#define tcomp_result_setOutputTCPBuffer(result, output_ptr, output_size, streamId) tcomp_result_setOutputBuffer((tcomp_result_t *)result, (void *)output_ptr, (size_t) output_size, 1, (uint64_t)streamId)
#define tcomp_result_setOutputSCTPBuffer(result, output_ptr, output_size) tcomp_result_setOutputTCPBuffer

TINYSIGCOMP_API void tcomp_result_setCompartmentId(tcomp_result_t *result, const void *id, size_t len);

void tcomp_result_addTempStateToCreate(tcomp_result_t *result, tcomp_state_t* lpState);
uint8_t tcomp_result_getTempStatesToCreateSize(const tcomp_result_t *result);

void tcomp_result_addTempStateToFree(tcomp_result_t *result, tcomp_tempstate_to_free_t* lpDesc);
uint8_t tcomp_result_getTempStatesToFreeSize(const tcomp_result_t *result);

TINYSIGCOMP_API const void *tcomp_result_def_t;

#endif /* TCOMP_RESULT_H */