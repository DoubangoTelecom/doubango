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

/**@file tcomp_compartment.h
 * @brief  SIGCOMP compartment.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *

 */
#ifndef TCOMP_COMPARTMENT_H
#define TCOMP_COMPARTMENT_H

#include "tinysigcomp_config.h"

#include "tcomp_types.h"
#include "tcomp_params.h"
#include "tcomp_compressordata.h"
#include "tcomp_result.h"

#include "tsk_safeobj.h"
#include "tsk_object.h"
#include "tsk_sha1.h"

TCOMP_BEGIN_DECLS

typedef struct tcomp_compartment_s {
    TSK_DECLARE_OBJECT;

    /*
    * An identifier (in a locally chosen format) that uniquely references a compartment.
    */
    uint64_t identifier;

    tcomp_states_L_t *local_states;
    tcomp_params_t *remote_parameters;
    tcomp_params_t *local_parameters;
    uint32_t total_memory_size;
    uint32_t total_memory_left;

    tcomp_buffer_handle_t *lpReqFeedback;
    tcomp_buffer_handle_t *lpRetFeedback;

    TCOMP_DECLARE_COMPRESSORDATA;

    tcomp_buffers_L_t* nacks;
    uint8_t nacks_history_count;

    tsk_bool_t useOnlyACKedStates;

    TSK_DECLARE_SAFEOBJ;
}
tcomp_compartment_t;

tcomp_compartment_t* tcomp_compartment_create(uint64_t id, uint32_t sigCompParameters, tsk_bool_t useOnlyACKedStates);
int tcomp_compartment_setUseOnlyACKedStates(tcomp_compartment_t* self, tsk_bool_t useOnlyACKedStates);

//
//	SigComp Parameters
//
void tcomp_compartment_setRemoteParams(tcomp_compartment_t *compartment, tcomp_params_t *lpParams);
//
//	Feedbacks
//
void tcomp_compartment_setReqFeedback(tcomp_compartment_t *compartment, tcomp_buffer_handle_t *feedback);
void tcomp_compartment_setRetFeedback(tcomp_compartment_t *compartment, tcomp_buffer_handle_t *feedback);

void tcomp_compartment_clearStates(tcomp_compartment_t *compartment);
void tcomp_compartment_freeStateByPriority(tcomp_compartment_t *compartment);
void tcomp_compartment_freeStates(tcomp_compartment_t *compartment, tcomp_tempstate_to_free_t **tempStates, uint8_t size);
void tcomp_compartment_addState(tcomp_compartment_t *compartment, tcomp_state_t **lpState);
uint32_t tcomp_compartment_findState(tcomp_compartment_t *compartment, const tcomp_buffer_handle_t *partial_identifier, tcomp_state_t **lpState);
void tcomp_compartment_freeGhostState(tcomp_compartment_t *compartment);


//
//	Nacks
//
void tcomp_compartment_addNack(tcomp_compartment_t *compartment, const uint8_t nackId[TSK_SHA1_DIGEST_SIZE]);
tsk_bool_t tcomp_compartment_hasNack(tcomp_compartment_t *compartment, const tcomp_buffer_handle_t *nackId);


TINYSIGCOMP_GEXTERN const tsk_object_def_t *tcomp_compartment_def_t;

TCOMP_END_DECLS

#endif /* TCOMP_COMPARTMENT_H */
