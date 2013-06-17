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

/**@file tcomp_statehandler.c
 * @brief  SigComp state handler.
 * Entity responsible for accessing and storing state information once permission is granted by the application.
 *	
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *

 */
#include "tcomp_statehandler.h"
#include "tcomp_rfc5049_sip.h"
#include "tcomp_nack_codes.h"
#include "tcomp_dicts.h"
#include "tcomp_udvm.h"

#include "tsk_debug.h"

static int pred_find_compartment_by_id(const tsk_list_item_t *item, const void *id)
{
	if(item && item->data){
		tcomp_compartment_t *compartment = item->data;
		int64_t res = (compartment->identifier - *((int64_t*)id));
		return res > 0 ? (int)1 : (res < 0 ? (int)-1 : (int)0);
	}
	return -1;
}

/**Creates new SigComp state handler.
*/
tcomp_statehandler_t* tcomp_statehandler_create()
{
	tcomp_statehandler_t* statehandler;
	if((statehandler = tsk_object_new(tcomp_statehandler_def_t))){
		/* RFC 3320 - 3.3.  SigComp Parameters */
		statehandler->sigcomp_parameters = tcomp_params_create();
		tcomp_params_setDmsValue(statehandler->sigcomp_parameters, SIP_RFC5049_DECOMPRESSION_MEMORY_SIZE);
		tcomp_params_setSmsValue(statehandler->sigcomp_parameters, SIP_RFC5049_STATE_MEMORY_SIZE);
		tcomp_params_setCpbValue(statehandler->sigcomp_parameters, SIP_RFC5049_CYCLES_PER_BIT);

		if(!(statehandler->dictionaries = tsk_list_create())){
			TSK_OBJECT_SAFE_FREE(statehandler);
			goto bail;
		}
		if(!(statehandler->compartments = tsk_list_create())){
			TSK_OBJECT_SAFE_FREE(statehandler);
			goto bail;
		}
		statehandler->sigcomp_parameters->SigComp_version = SIP_RFC5049_SIGCOMP_VERSION;
#if TCOMP_USE_ONLY_ACKED_STATES
		statehandler->useOnlyACKedStates = tsk_true;
#endif
	}
bail:
	return statehandler;
}

int tcomp_statehandler_setUseOnlyACKedStates(tcomp_statehandler_t* self, tsk_bool_t useOnlyACKedStates)
{
	tsk_list_item_t* item;
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	self->useOnlyACKedStates = useOnlyACKedStates;

	tsk_safeobj_lock(self);
	tsk_list_foreach(item, self->compartments){
		tcomp_compartment_setUseOnlyACKedStates((tcomp_compartment_t*)item->data, self->useOnlyACKedStates);
	}
	tsk_safeobj_unlock(self);

	return 0;
}

tcomp_compartment_t *tcomp_statehandler_getCompartment(const tcomp_statehandler_t *statehandler, uint64_t id)
{
	tcomp_compartment_t *result = tsk_null;
	tcomp_compartment_t* newcomp = tsk_null;
	const tsk_list_item_t *item_const;

	if(!statehandler){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}

	tsk_safeobj_lock(statehandler);

	item_const = tsk_list_find_item_by_pred(statehandler->compartments, pred_find_compartment_by_id, &id);
	if(!item_const || !(result = item_const->data)){
		newcomp = tcomp_compartment_create(id, tcomp_params_getParameters(statehandler->sigcomp_parameters), statehandler->useOnlyACKedStates);
		result = newcomp;
		tsk_list_push_back_data(statehandler->compartments, ((void**) &newcomp));
	}

	tsk_safeobj_unlock(statehandler);

	return result;
}

void tcomp_statehandler_deleteCompartment(tcomp_statehandler_t *statehandler, uint64_t id)
{
	tcomp_compartment_t *compartment;
	const tsk_list_item_t *item_const;

	if(!statehandler){
		TSK_DEBUG_ERROR("Invalid parameter");
		return;
	}

	tsk_safeobj_lock(statehandler);

	item_const = tsk_list_find_item_by_pred(statehandler->compartments, pred_find_compartment_by_id, &id);
	if(item_const && (compartment = item_const->data)){
		TSK_DEBUG_INFO("SigComp - Delete compartment %lld", id);
		tsk_list_remove_item_by_data(statehandler->compartments, compartment);
	}

	tsk_safeobj_unlock(statehandler);
}


tsk_bool_t tcomp_statehandler_compartmentExist(tcomp_statehandler_t *statehandler, uint64_t id)
{
	tsk_bool_t exist;

	if(!statehandler){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_false;
	}

	tsk_safeobj_lock(statehandler);
	exist =  (tsk_list_find_item_by_pred(statehandler->compartments, pred_find_compartment_by_id, &id) ? 1 : 0);
	tsk_safeobj_unlock(statehandler);

	return exist;
}


uint32_t tcomp_statehandler_findState(tcomp_statehandler_t *statehandler, const tcomp_buffer_handle_t *partial_identifier, tcomp_state_t** lpState)
{
	uint32_t count = 0;
	tsk_list_item_t *item;

	if(!statehandler){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}
	
	tsk_safeobj_lock(statehandler);

	//
	// Compartments
	//
	tsk_list_foreach(item, statehandler->compartments){
		tcomp_compartment_t *compartment = item->data;
		count += tcomp_compartment_findState(compartment, partial_identifier, lpState);
	}
	
	if(count){
		goto bail;
	}

	//
	// Dictionaries
	//
	tsk_list_foreach(item, statehandler->dictionaries){
		tcomp_dictionary_t *dictionary = item->data;
		if(tcomp_buffer_startsWith(dictionary->identifier, partial_identifier)){
			*lpState = dictionary;
			count++;
		}
	}
bail:
	tsk_safeobj_unlock(statehandler);

	return count;
}

void tcomp_statehandler_handleResult(tcomp_statehandler_t *statehandler, tcomp_result_t **lpResult)
{
	tcomp_compartment_t *lpCompartment;
	uint32_t compartment_total_size;
	uint8_t i;

	if(!statehandler){
		TSK_DEBUG_ERROR("Invalid parameter");
		return;
	}

	/*== Do not lock --> all functions are thread-safe. */
	//tsk_safeobj_lock(statehandler);

	/*
	* The compressor does not wish (or no longer wishes) to save state information?
	*/
	if((*lpResult)->ret_feedback && (*lpResult)->req_feedback->S){
		if(tcomp_statehandler_compartmentExist(statehandler, (*lpResult)->compartmentId)){
			tcomp_statehandler_deleteCompartment(statehandler, (*lpResult)->compartmentId);
		}
		return;
	}

	/*
	* Find corresponding compartment (only if !S)
	*/
	if((lpCompartment = tcomp_statehandler_getCompartment(statehandler, (*lpResult)->compartmentId))){
		compartment_total_size = lpCompartment->total_memory_size;
	}
	else{
		goto bail;
	}

//compartment_create_states:
	/*
	* Request state creation now we have the corresponding compartement.
	*/
	if(tcomp_result_getTempStatesToCreateSize(*lpResult)){
		uint8_t count;
		/* Check compartment allocated size*/
		if(!compartment_total_size){
			goto compartment_free_states;
		}
		
		count = tcomp_result_getTempStatesToCreateSize(*lpResult);

		// FIXME: lock
		for (i = 0; i < count; i++)
		{
			tcomp_state_t **lpState =  &((*lpResult)->statesToCreate[i]);
			if(!lpState || !*lpState){
				continue;
			}

			/*
			* If the state creation request needs more state memory than the
			* total state_memory_size for the compartment, the state handler
			* deletes all but the first (state_memory_size - 64) bytes from the state_value.
			*/
			if(TCOMP_GET_STATE_SIZE(*lpState) > compartment_total_size){
				tsk_size_t oldSize, newSize;
				tcomp_compartment_clearStates(lpCompartment);
				oldSize =  tcomp_buffer_getSize((*lpState)->value);
				newSize = (compartment_total_size - 64);
				tcomp_buffer_removeBuff((*lpState)->value, newSize, (oldSize-newSize));
				(*lpState)->length = newSize;

				tcomp_compartment_addState(lpCompartment, lpState);
			}

			/*
			* If the state creation request exceeds the state memory allocated
			* to the compartment, sufficient items of state created by the same
			* compartment are freed until enough memory is available to
			* accommodate the new state.
			*/
			else{
				while(lpCompartment->total_memory_left < TCOMP_GET_STATE_SIZE(*lpState)){
					tcomp_compartment_freeStateByPriority(lpCompartment);
				}
				tcomp_compartment_addState(lpCompartment, lpState);
			}
		}
	}

compartment_free_states:
	/*
	* Request state free now we have the correponding comprtement
	*/
	if(tcomp_result_getTempStatesToFreeSize((const tcomp_result_t*)*lpResult)){
		tcomp_compartment_freeStates(lpCompartment, (*lpResult)->statesToFree, tcomp_result_getTempStatesToFreeSize((const tcomp_result_t*)*lpResult));
	}

//compartment_remote_params:
	/*
	*	Set remote -compressor- parameters.
	*/
	tcomp_compartment_setRemoteParams(lpCompartment, (*lpResult)->remote_parameters);

//feedbacks:
	/*
	*	Set both Returned and Requested feedbacks.
	*/
	
	if(tcomp_buffer_getSize((*lpResult)->req_feedback->item)){
		tcomp_compartment_setReqFeedback(lpCompartment, (*lpResult)->req_feedback->item);
	}
	if(tcomp_buffer_getSize((*lpResult)->ret_feedback)){
		tcomp_compartment_setRetFeedback(lpCompartment, (*lpResult)->ret_feedback);
	}

bail: ;
	//--tsk_safeobj_unlock(lpCompartment);
	//--tsk_safeobj_unlock(statehandler);
}

tsk_bool_t tcomp_statehandler_handleNack(tcomp_statehandler_t *statehandler, const tcomp_nackinfo_t * nackinfo)
{
	tcomp_buffer_handle_t* sha_id;
	tsk_list_item_t *item;
	tsk_bool_t found = tsk_false;
	if(!statehandler){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_false;
	}

	if(!(sha_id = tcomp_buffer_create_null())){
		TSK_DEBUG_ERROR("Failed to create buffer handle");
		return tsk_false;
	}

	tcomp_buffer_referenceBuff(sha_id, ((tcomp_nackinfo_t*)nackinfo)->sha1, TSK_SHA1_DIGEST_SIZE);

	tsk_list_foreach(item, statehandler->compartments)
	{
		tcomp_compartment_t* lpCompartement = item->data;
		if(tcomp_compartment_hasNack(lpCompartement, sha_id))
		{
			// this compartment is responsible for this nack
			switch(nackinfo->reasonCode)
			{
				case NACK_STATE_NOT_FOUND:
					{
						// Next commented because in this version remote state ids are never saved.
						// Only the ghost has information on last partial state id to use --> reset the ghost
						//SigCompState* lpState = NULL;
						//lpCompartement->findState(&nack_info->details, &lpState);
						//if(lpState)
						//{
						//	lpCompartement->freeState(lpState);
						//}
						tcomp_compartment_freeGhostState(lpCompartement);
					}
					break;

				default:
					{
						tcomp_compartment_freeGhostState(lpCompartement);
						tcomp_compartment_clearStates(lpCompartement);
					}
					break;
			}
			TSK_DEBUG_INFO("Compartment has NACK :)");
			tcomp_buffer_print(sha_id);
			found = tsk_true;
		}
	}

	if(!found)
	{
		TSK_DEBUG_ERROR("Compartments do not have NACK with id=");
		tcomp_buffer_print(sha_id);
	}

	TSK_OBJECT_SAFE_FREE(sha_id);

	return found;
}

int tcomp_statehandler_addSipSdpDictionary(tcomp_statehandler_t *statehandler)
{
	if(!statehandler){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	
	tsk_safeobj_lock(statehandler);
	
	if(!statehandler->hasSipSdpDictionary){
		tcomp_dictionary_t* sip_dict = tcomp_dicts_create_sip_dict();
		tsk_list_push_back_data(statehandler->dictionaries, ((void**) &sip_dict));
		statehandler->hasSipSdpDictionary = 1;
	}

	tsk_safeobj_unlock(statehandler);
	return 0;
}

int tcomp_statehandler_addPresenceDictionary(tcomp_statehandler_t *statehandler)
{
	if(!statehandler){
		TSK_DEBUG_ERROR("NULL SigComp state handler.");
		return -1;
	}

	tsk_safeobj_lock(statehandler);

	if(!statehandler->hasPresenceDictionary){
		tcomp_dictionary_t* pres_dict = tcomp_dicts_create_presence_dict();
		tsk_list_push_back_data(statehandler->dictionaries, ((void**) &pres_dict));
		statehandler->hasPresenceDictionary = 1;
	}

	tsk_safeobj_unlock(statehandler);
	return 0;
}







//========================================================
//	State hanlder object definition
//

static tsk_object_t* tcomp_statehandler_ctor(tsk_object_t * self, va_list * app)
{
	tcomp_statehandler_t *statehandler = self;
	if(statehandler){
		/* Initialize safeobject */
		tsk_safeobj_init(statehandler);
	}
	else{
		TSK_DEBUG_ERROR("Null SigComp state handler.");
	}

	return self;
}

static void* tcomp_statehandler_dtor(tsk_object_t *self)
{
	tcomp_statehandler_t *statehandler = self;
	if(statehandler){
		/* Deinitialize safeobject */
		tsk_safeobj_deinit(statehandler);

		TSK_OBJECT_SAFE_FREE(statehandler->sigcomp_parameters);

		TSK_OBJECT_SAFE_FREE(statehandler->dictionaries);
		TSK_OBJECT_SAFE_FREE(statehandler->compartments);		
	}
	else{
		TSK_DEBUG_ERROR("Null SigComp state handler.");
	}

	return self;
}

static const tsk_object_def_t tsk_statehandler_def_s = 
{
	sizeof(tcomp_statehandler_t),
	tcomp_statehandler_ctor,
	tcomp_statehandler_dtor,
	tsk_null
};
const tsk_object_def_t *tcomp_statehandler_def_t = &tsk_statehandler_def_s;
