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

/**@file tcomp_result.c
 * @brief  SigComp decompresion result.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *

 */
#include "tcomp_result.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

/** Creates new SigComp result object.
*/
tcomp_result_t* tcomp_result_create()
{
    return tsk_object_new(tcomp_result_def_t);
}

tcomp_tempstate_to_free_t* tcomp_tempstate_to_free_create()
{
    return tsk_object_new(tcomp_tempstate_to_free_def_t);
}

/**Resets the result.
*/
void _tcomp_result_reset(tcomp_result_t *result, tsk_bool_t isDestructor, tsk_bool_t isResetOutput)
{
    if(result) {
        uint8_t i;
        for(i = 0; i < result->statesToCreateIndex; i++) {
            TSK_OBJECT_SAFE_FREE(result->statesToCreate[i]);
        }

        for(i = 0; i < result->statesToFreeIndex; i++) {
            TSK_OBJECT_SAFE_FREE(result->statesToFree[i]);
        }

        if(!isDestructor) {
            result->statesToCreateIndex = 0;
            result->statesToFreeIndex = 0;
            result->consumed_cycles = 0;

            tcomp_params_reset(result->remote_parameters);

            if(isResetOutput) {
                tcomp_buffer_reset(result->output_buffer);
            }

            tcomp_reqfeed_reset(result->req_feedback);
            tcomp_buffer_freeBuff(result->ret_feedback);

            result->isNack = 0;
            tcomp_buffer_freeBuff(result->nack_info);
        }
    }
    else {
        TSK_DEBUG_ERROR("NULL SigComp result.");
    }
}

/**Sets the output buffer.
*/
void tcomp_result_setOutputBuffer(tcomp_result_t *result, void *output_ptr, tsk_size_t output_size, tsk_bool_t isStream, uint64_t streamId)
{
    if(result) {
        tcomp_buffer_referenceBuff(result->output_buffer, (uint8_t*)output_ptr, output_size);
        result->isStreamBased = isStream;
        result->streamId = streamId;
    }
    else {
        TSK_DEBUG_ERROR("NULL SigComp result.");
    }
}

/**Sets the compartment identifier.
*/
void tcomp_result_setCompartmentId(tcomp_result_t *result, const void *id, tsk_size_t len)
{
    if(result) {
        result->compartmentId = tcomp_buffer_createHash(id, len);
    }
    else {
        TSK_DEBUG_ERROR("NULL SigComp result.");
    }
}

/**Adds temporary state.
*/
void tcomp_result_addTempStateToCreate(tcomp_result_t *result, tcomp_state_t* lpState)
{
    if(result) {
        /*
        * Note that there is a maximum limit of four state creation requests per instance of the UDVM.
        */
        if(result->statesToCreateIndex >= MAX_TEMP_SATES) {
            TSK_DEBUG_ERROR("Maximum limit for %d state creation requests reached", MAX_TEMP_SATES);
            return;
        }

        // Add state
        result->statesToCreate[result->statesToCreateIndex++] = lpState;
    }
    else {
        TSK_DEBUG_ERROR("NULL SigComp result.");
    }
}

/**Gets the number of temporary state (to be created).
*/
uint8_t tcomp_result_getTempStatesToCreateSize(const tcomp_result_t *result)
{
    if(result) {
        return result->statesToCreateIndex;
    }
    else {
        TSK_DEBUG_ERROR("NULL SigComp result.");
    }
    return 0;
}

/**Adds temporary state (to be freed).
*/
void tcomp_result_addTempStateToFree(tcomp_result_t *result, tcomp_tempstate_to_free_t* lpDesc)
{
    if(result) {
        /*
        * Decompression failure MUST occur if more than four state free
        * requests are made before the END-MESSAGE instruction is encountered.
        */
        if(result->statesToFreeIndex >= MAX_TEMP_SATES) {
            return;
        }
        result->statesToFree[result->statesToFreeIndex++] = lpDesc;
    }
    else {
        TSK_DEBUG_ERROR("NULL SigComp result.");
    }
}

/**Gets the number of temporary state (to be be freed).
*/
uint8_t tcomp_result_getTempStatesToFreeSize(const tcomp_result_t *result)
{
    if(result) {
        return result->statesToFreeIndex;
    }
    else {
        TSK_DEBUG_ERROR("NULL SigComp result.");
    }
    return 0;
}





//========================================================
//	SigComp result object definition
//

static tsk_object_t* tcomp_result_ctor(tsk_object_t *self, va_list * app)
{
    tcomp_result_t* result = self;

    if(result) {
        result->output_buffer = tcomp_buffer_create_null();
        result->ret_feedback = tcomp_buffer_create_null();
        result->nack_info = tcomp_buffer_create_null();

        result->remote_parameters = tcomp_params_create();

        result->req_feedback = tcomp_reqfeed_create();
    }
    else {
        TSK_DEBUG_ERROR("Null result object");
    }

    return self;
}

static tsk_object_t* tcomp_result_dtor(tsk_object_t * self)
{
    tcomp_result_t* result = self;

    if(result) {
        _tcomp_result_reset(result, tsk_true, tsk_true);
        TSK_OBJECT_SAFE_FREE(result->output_buffer);
        TSK_OBJECT_SAFE_FREE(result->ret_feedback);
        TSK_OBJECT_SAFE_FREE(result->nack_info);

        TSK_OBJECT_SAFE_FREE(result->remote_parameters);

        TSK_OBJECT_SAFE_FREE(result->req_feedback);
    }
    else {
        TSK_DEBUG_ERROR("Null result object");
    }

    return self;
}

static const tsk_object_def_t tcomp_result_def_s = {
    sizeof(tcomp_result_t),
    tcomp_result_ctor,
    tcomp_result_dtor,
    tsk_null
};
const tsk_object_def_t *tcomp_result_def_t = &tcomp_result_def_s;


//========================================================
//	SigComp temporary state object definition
//

static tsk_object_t* tcomp_tempstate_to_free_ctor(tsk_object_t* self, va_list * app)
{
    tcomp_tempstate_to_free_t* tempstate_to_free = self;

    if(tempstate_to_free) {
        tempstate_to_free->identifier = tcomp_buffer_create_null();
    }
    else {
        TSK_DEBUG_ERROR("Null object");
    }

    return self;
}

static tsk_object_t* tcomp_tempstate_to_free_dtor(tsk_object_t* self)
{
    tcomp_tempstate_to_free_t* tempstate_to_free = self;

    if(tempstate_to_free) {
        TSK_OBJECT_SAFE_FREE(tempstate_to_free->identifier);
    }
    else {
        TSK_DEBUG_ERROR("Null object");
    }

    return self;
}

static const tsk_object_def_t tcomp_tempstate_to_free_def_s = {
    sizeof(tcomp_tempstate_to_free_t),
    tcomp_tempstate_to_free_ctor,
    tcomp_tempstate_to_free_dtor,
    tsk_null
};
const tsk_object_def_t* tcomp_tempstate_to_free_def_t = &tcomp_tempstate_to_free_def_s;
