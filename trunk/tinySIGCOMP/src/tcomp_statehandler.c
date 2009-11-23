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

/**@file tcomp_statehandler.c
 * @brief  SIGCOMP state handler.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tcomp_statehandler.h"
#include "tcomp_rfc5049_sip.h"

#include "tsk_debug.h"

/**@defgroup tcomp_statehandler_group SIGCOMP state handler.
* Entity responsible for accessing and storing state information once permission is granted by the application.
*/

/**@ingroup tcomp_statehandler_group
*/
static int pred_find_compartment_by_id(const tsk_list_item_t *item, const void *id)
{
	if(item && item->data)
	{
		tcomp_compartment_t *compartment = item->data;
		return (compartment->identifier == *((uint64_t*)id));
	}
	return 0;
}

/**@ingroup tcomp_statehandler_group
*/
tcomp_compartment_t *tcomp_statehandler_getCompartment(tcomp_statehandler_t *statehandler, uint64_t id)
{
	tcomp_compartment_t *result;
	tcomp_compartment_t* newcomp;

	if(!statehandler)
	{
		TSK_DEBUG_ERROR("NULL SigComp state handler.");
		return 0;
	}

	tsk_safeobj_lock(statehandler);

	if(!(result = (tcomp_compartment_t *)tsk_list_find_item_by_pred(statehandler->compartments, pred_find_compartment_by_id, &id)))
	{
		newcomp = TCOMP_COMPARTMENT_CREATE(id, tcomp_params_getParameters(statehandler->sigcomp_parameters));
		result = newcomp;
		tsk_list_add_data(statehandler->compartments, ((void**) &newcomp));
	}

	tsk_safeobj_unlock(statehandler);

	return result;
}

/**@ingroup tcomp_statehandler_group
*/
void tcomp_statehandler_deleteCompartment(tcomp_statehandler_t *statehandler, uint64_t id)
{
	if(!statehandler)
	{
		TSK_DEBUG_ERROR("NULL SigComp state handler.");
		return;
	}
}

/**@ingroup tcomp_statehandler_group
*/
int tcomp_statehandler_compartmentExist(tcomp_statehandler_t *statehandler, uint64_t id)
{
	if(!statehandler)
	{
		TSK_DEBUG_ERROR("NULL SigComp state handler.");
		return 0;
	}
}

/**@ingroup tcomp_statehandler_group
*/
uint16_t tcomp_statehandler_findState(tcomp_statehandler_t *statehandler, const tcomp_buffer_handle_t *partial_identifier, tcomp_state_t** lpState)
{
	if(!statehandler)
	{
		TSK_DEBUG_ERROR("NULL SigComp state handler.");
		return 0;
	}
}

/**@ingroup tcomp_statehandler_group
*/
void tcomp_statehandler_handleResult(tcomp_statehandler_t *statehandler, tcomp_result_t **lpResult)
{
	if(!statehandler)
	{
		TSK_DEBUG_ERROR("NULL SigComp state handler.");
		return;
	}
}

/**@ingroup tcomp_statehandler_group
*/
int tcomp_statehandler_handleNack(tcomp_statehandler_t *statehandler, const tcomp_nackinfo_t * nackinfo)
{
	if(!statehandler)
	{
		TSK_DEBUG_ERROR("NULL SigComp state handler.");
		return 0;
	}
}

/**@ingroup tcomp_statehandler_group
*/
void tcomp_statehandler_addSipSdpDictionary(tcomp_statehandler_t *statehandler)
{
	if(!statehandler)
	{
		TSK_DEBUG_ERROR("NULL SigComp state handler.");
		return;
	}
}

/**@ingroup tcomp_statehandler_group
*/
void tcomp_statehandler_addPresenceDictionary(tcomp_statehandler_t *statehandler)
{
	if(!statehandler)
	{
		TSK_DEBUG_ERROR("NULL SigComp state handler.");
		return;
	}
}







//========================================================
//	State hanlder object definition
//

/**@ingroup tcomp_statehandler_group
*/
static void* tcomp_statehandler_create(void * self, va_list * app)
{
	tcomp_statehandler_t *statehandler = self;
	if(statehandler)
	{
		/* Initialize safeobject */
		tsk_safeobj_init(statehandler);
		
		/* RFC 3320 - 3.3.  SigComp Parameters */
		tcomp_params_setDmsValue(statehandler->sigcomp_parameters, SIP_RFC5049_DECOMPRESSION_MEMORY_SIZE);
		tcomp_params_setSmsValue(statehandler->sigcomp_parameters, SIP_RFC5049_STATE_MEMORY_SIZE);
		tcomp_params_setCpbValue(statehandler->sigcomp_parameters, SIP_RFC5049_CYCLES_PER_BIT);

		statehandler->sigcomp_parameters->SigComp_version = SIP_RFC5049_SIGCOMP_VERSION;
	}
	else TSK_DEBUG_ERROR("Null SigComp state handler.");

	return self;
}

/**@ingroup tcomp_statehandler_group
*/
static void* tcomp_statehandler_destroy(void *self)
{
	tcomp_statehandler_t *statehandler = self;
	if(statehandler)
	{
		/* Deinitialize safeobject */
		tsk_safeobj_deinit(statehandler);

		/* Delete all compartments */
		TSK_LIST_SAFE_FREE(statehandler->compartments);

		/* Delete all dictionaries */
		TSK_LIST_SAFE_FREE(statehandler->dictionaries);
	}
	else TSK_DEBUG_ERROR("Null SigComp state handler.");

	return self;
}

static const tsk_object_def_t tsk_statehandler_def_s = 
{
	sizeof(tcomp_statehandler_t),
	tcomp_statehandler_create,
	tcomp_statehandler_destroy,
	0,
	0,
	0
};
const void *tcomp_statehandler_def_t = &tsk_statehandler_def_s;