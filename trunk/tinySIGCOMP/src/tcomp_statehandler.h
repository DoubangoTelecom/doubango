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
* GNU General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

/**@file tcomp_statehandler.h
 * @brief  SigComp state handler.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TCOMP_STATE_HANDLER_H
#define TCOMP_STATE_HANDLER_H

#include "tinysigcomp_config.h"

#include "tcomp_types.h"
#include "tcomp_params.h"
#include "tcomp_nackinfo.h"
#include "tcomp_result.h"
#include "tcomp_buffer.h"
#include "tcomp_compartment.h"
#include "tcomp_state.h"

#include "tsk_safeobj.h"
#include "tsk_object.h"

TCOMP_BEGIN_DECLS

/**Creates new SigComp state handler.
*/
#define TCOMP_STATEHANDLER_CREATE()				tsk_object_new(tcomp_statehandler_def_t)

/**State handler.
*/
typedef struct tcomp_statehandler_s
{
	TSK_DECLARE_OBJECT;
	
	tcomp_compartments_L_t *compartments;
	tcomp_params_t *sigcomp_parameters;
	
	tcomp_dictionaries_L_t *dictionaries;
	int hasSipSdpDictionary;
	int hasPresenceDictionary;

	TSK_DECLARE_SAFEOBJ;
}
tcomp_statehandler_t;

tcomp_compartment_t *tcomp_statehandler_getCompartment(const tcomp_statehandler_t *statehandler, uint64_t id);
void tcomp_statehandler_deleteCompartment(tcomp_statehandler_t *statehandler, uint64_t id);
tsk_bool_t tcomp_statehandler_compartmentExist(tcomp_statehandler_t *statehandler, uint64_t id);
uint16_t tcomp_statehandler_findState(tcomp_statehandler_t *statehandler, const tcomp_buffer_handle_t *partial_identifier, tcomp_state_t** lpState);

void tcomp_statehandler_handleResult(tcomp_statehandler_t *statehandler, tcomp_result_t **lpResult);
tsk_bool_t tcomp_statehandler_handleNack(tcomp_statehandler_t *statehandler, const tcomp_nackinfo_t *);

void tcomp_statehandler_addSipSdpDictionary(tcomp_statehandler_t *statehandler);
void tcomp_statehandler_addPresenceDictionary(tcomp_statehandler_t *statehandler);

TINYSIGCOMP_GEXTERN const tsk_object_def_t *tcomp_statehandler_def_t;

TCOMP_END_DECLS

#endif /* TCOMP_STATE_HANDLER_H */
