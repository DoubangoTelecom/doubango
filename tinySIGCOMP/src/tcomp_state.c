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

/**@file tcomp_state.c
 * @brief  SigComp state.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *

 */
#include "tcomp_state.h"
#include "tsk_memory.h"
#include "tsk_debug.h"
#include "tsk_sha1.h"

/** Creates new SigComp state.
*/
tcomp_state_t* tcomp_state_create(uint32_t length, uint32_t address, uint32_t instruction, uint32_t minimum_access_length, uint32_t retention_priority)
{
	tcomp_state_t *state;
	if((state = tsk_object_new(tcomp_state_def_t))){
		state->length = length;
		state->address = address;
		state->instruction = instruction;
		state->minimum_access_length = minimum_access_length;
		state->retention_priority = retention_priority;

		state->value = tcomp_buffer_create_null();
		state->identifier = tcomp_buffer_create_null();
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new state.");
	}
	return state;
}

/** Compares two sigomp states.
* @param state1 First state to compare.
* @param state2 Second state to compare.
* @retval 1 if the two handles are equals and 0 otherwise.
*/
int tcomp_state_equals(const tcomp_state_t *state1, const tcomp_state_t *state2)
{
	if(state1 && state2)
	{
		return tcomp_buffer_equals(state1->identifier, state2->identifier);
	}
	else if(!state1 && !state2) return 1;
	else return 0;
}

/**Computes the state identifier by calculating a 20-byte SHA-1 hash [RFC-3174] over the
*  byte string formed by concatenating the state_length, state_address,
*  state_instruction, minimum_access_length and state_value (in the order given).
* @param state The state to make valid.
*/
void tcomp_state_makeValid(tcomp_state_t* state)
{
	tsk_sha1context_t sha;

	if(!state){
		TSK_DEBUG_ERROR("Invalid parameter");
		return;
	}

	/* Lock */
	tsk_safeobj_lock(state);
	
	tcomp_buffer_allocBuff(state->identifier, TSK_SHA1_DIGEST_SIZE);

	/*=============
		* Calculates a 20-byte SHA-1 hash [RFC-3174] over the byte string formed by concatenating the state_length, state_address,
	    * state_instruction, minimum_access_length and state_value (in the order given).  This is the state_identifier. 
	*/
	{
		uint8_t i;
		int32_t err = tsk_sha1reset(&sha);
		uint8_t firstPart[8];
		
#ifdef __SYMBIAN32__
		uint32_t values[4];
		values[0] = state->length,
		values[1] = state->address,
		values[2] = state->instruction,
		values[3] = state->minimum_access_length;
		
#else
		uint32_t values[4] = { state->length, state->address, state->instruction, state->minimum_access_length };
#endif
				
		for(i=0; i<4; i++)
		{
		#if 0 /*BIG_ENDIAN*/// Do not change this (it's for my own tests)
			firstPart[i] = (values[i] & 0xff);
			firstPart[i+1] = (values[i] >> 8);
		#else
			firstPart[2*i] = (values[i] >> 8);
			firstPart[2*i+1] = (values[i]& 0xff);
		#endif
		}

		tsk_sha1input(&sha, firstPart, 8);
		tsk_sha1input(&sha, tcomp_buffer_getBuffer(state->value), tcomp_buffer_getSize(state->value));
		err = tsk_sha1result(&sha, (uint8_t*)tcomp_buffer_getBuffer(state->identifier));
	}

	/* unlock */
	tsk_safeobj_unlock(state);
}

int32_t tcomp_state_inc_usage_count(tcomp_state_t* self)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}
	return ++self->usage_count;
}

int32_t tcomp_state_dec_usage_count(tcomp_state_t* self)
{
	if(!self || self->usage_count <= 0){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}
	return --self->usage_count;
}



//========================================================
//	State object definition
//
static tsk_object_t* tcomp_state_ctor(tsk_object_t * self, va_list * app)
{
	tcomp_state_t *state = self;
	if(state){
		/* Initialize safeobject */
		tsk_safeobj_init(state);
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new state.");
	}
	return state;
}

static tsk_object_t* tcomp_state_dtor(tsk_object_t *self)
{
	tcomp_state_t *state = self;
	if(state){
		TSK_DEBUG_INFO("==SigComp - Free state with id=");
		tcomp_buffer_print(state->identifier);

		/* Deinitialize safeobject */
		tsk_safeobj_deinit(state);

		TSK_OBJECT_SAFE_FREE(state->identifier);
		TSK_OBJECT_SAFE_FREE(state->value);
	}
	else{
		TSK_DEBUG_ERROR("Null SigComp state.");
	}

	return self;
}


static int tcomp_state_cmp(const void *obj1, const void *obj2)
{
	const tcomp_state_t *state1 = obj1;
	const tcomp_state_t *state2 = obj2;

	if(state1 && state2){
		return tcomp_buffer_equals(state1->identifier, state2->identifier) ? 0 : -1;
	}
	else if(!state1 && !state2) return 0;
	else return -1;
}

static const tsk_object_def_t tcomp_state_def_s = 
{
	sizeof(tcomp_state_t),
	tcomp_state_ctor,
	tcomp_state_dtor,
	tcomp_state_cmp
};
const tsk_object_def_t *tcomp_state_def_t = &tcomp_state_def_s;

