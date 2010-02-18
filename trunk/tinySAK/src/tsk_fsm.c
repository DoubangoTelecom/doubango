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

/**@file tsk_fsm.c
 * @brief Finite-state machine (FSM) implementation.
 * @sa http://en.wikipedia.org/wiki/Finite-state_machine.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tsk_fsm.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

int tsk_fsm_exec_nothing(va_list *app){ return 1; }
int tsk_fsm_cond_always(const void* data1, const void* data2) { return 1; }

int tsk_fsm_set(tsk_fsm_t* self, ...)
{
	va_list args;
	int guard;
	
	if(!self){
		return -1;
	}
	
	va_start(args, self);
	while((guard=va_arg(args, int)))
	{		
		tsk_fsm_entry_t* entry;
		if((entry = TSK_FSM_ENTRY_CREATE()))
		{
			entry->from = va_arg(args, tsk_fsm_state_id);
			entry->action = va_arg(args, tsk_fsm_action_id);
			entry->cond = va_arg(args, tsk_fsm_cond);
			entry->to = va_arg(args, tsk_fsm_state_id);
			entry->exec = va_arg(args, tsk_fsm_exec);
			entry->desc = va_arg(args, const char*);
			
			tsk_list_push_descending_data(self->entries, (void**)&entry);
		}
	}
	va_end(args);
	
	return 0;
}

int tsk_fsm_set_callback_terminated(tsk_fsm_t* self, tsk_fsm_onterminated callback, const void* callbackdata)
{
	if(self)
	{
		self->callback_term = callback;
		self->callback_data = callbackdata;
		return 0;
	}
	return -1;
}

int tsk_fsm_act(tsk_fsm_t* self, tsk_fsm_action_id action, const void* cond_data1, const void* cond_data2, ...)
{
	tsk_list_item_t *item;
	va_list ap;
	int found = 0;
	int ret_exec = 0;
	
	if(!self){
		TSK_DEBUG_ERROR("Null FSM.");
		return -1;
	}
	if(self->current == self->term){
		TSK_DEBUG_WARN("The FSM is in the final state.");
		return -2;
	}
	
	// lock
	tsk_safeobj_lock(self);
	
	va_start(ap, cond_data2);
	tsk_list_foreach(item, self->entries)
	{
		tsk_fsm_entry_t* entry = item->data;
		if((entry->from != tsk_fsm_state_any) && (entry->from != self->current)){
			continue;
		}

		if((entry->action != tsk_fsm_action_any) && (entry->action != action)){
			continue;
		}
		
		// check condition
		if(entry->cond(cond_data1, cond_data2)){
			// For debug information
			if(self->debug){
				TSK_DEBUG_INFO("State machine: %s", entry->desc);
			}
			self->current = entry->to;
			if(entry->exec){
				if((ret_exec = entry->exec(&ap))){
					TSK_DEBUG_INFO("State machine: Exec function failed. Moving to terminal state.");
					self->current = self->term;
				}
			}

			found = 1;
			break;
		}
	}
	va_end(ap);
	
	// unlock
	tsk_safeobj_unlock(self);

	/* Only call the callback function after unlock. */
	if(self->current == self->term){
		if(self->callback_term){
			self->callback_term(self->callback_data);
		}
	}
	if(!found){
		TSK_DEBUG_WARN("State machine: No matching state found.");
	}
	
	return 0;
}




//=================================================================================================
//	fsm object definition
//
static void* tsk_fsm_create(void * self, va_list * app)
{
	tsk_fsm_t *fsm = self;
	if(fsm)
	{
		fsm->current = va_arg(*app, tsk_fsm_state_id);
		fsm->term = va_arg(*app, tsk_fsm_state_id);

		fsm->entries = TSK_LIST_CREATE();

#if defined(DEBUG) || defined(_DEBUG)
		fsm->debug = 1;
#endif
		tsk_safeobj_init(fsm);
	}

	return self;
}

static void* tsk_fsm_destroy(void * self)
{ 
	tsk_fsm_t *fsm = self;
	if(fsm)
	{
		tsk_safeobj_deinit(fsm);

		TSK_OBJECT_SAFE_FREE(fsm->entries);
	}

	return self;
}

static const tsk_object_def_t tsk_fsm_def_s = 
{
	sizeof(tsk_fsm_t),
	tsk_fsm_create, 
	tsk_fsm_destroy,
	0, 
};
const void *tsk_fsm_def_t = &tsk_fsm_def_s;

//=================================================================================================
//	fsm entry object definition
//
static void* tsk_fsm_entry_create(void * self, va_list * app)
{
	tsk_fsm_entry_t *fsm_entry = self;
	if(fsm_entry)
	{
	}

	return self;
}

static void* tsk_fsm_entry_destroy(void * self)
{ 
	tsk_fsm_entry_t *fsm_entry = self;
	if(fsm_entry)
	{
		//TSK_FREE(fsm_entry->desc);
	}

	return self;
}
static int tsk_fsm_entry_cmp(const void *obj1, const void *obj2)
{
	const tsk_fsm_entry_t* entry1 = obj1;
	const tsk_fsm_entry_t* entry2 = obj2;
	if(entry1 && entry2)
	{
		/* Put "Any" states at the bottom. (Strong)*/
		if(entry1->from == tsk_fsm_state_any){
			return -20;
		}
		else if(entry2->from == tsk_fsm_state_any){
			return +20;
		}

		/* Put "Any" actions at the bottom. (Weak)*/
		if(entry1->action == tsk_fsm_action_any){
			return -10;
		}
		else if(entry1->action == tsk_fsm_action_any){
			return +10;
		}
	}
	return 0;
}

static const tsk_object_def_t tsk_fsm_entry_def_s = 
{
	sizeof(tsk_fsm_entry_t),
	tsk_fsm_entry_create, 
	tsk_fsm_entry_destroy,
	tsk_fsm_entry_cmp, 
};
const void *tsk_fsm_entry_def_t = &tsk_fsm_entry_def_s;