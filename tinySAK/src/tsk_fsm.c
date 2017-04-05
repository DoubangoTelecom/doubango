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

/**@file tsk_fsm.c
 * @brief Finite-state machine (FSM) implementation.
 * @sa http://en.wikipedia.org/wiki/Finite-state_machine.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tsk_fsm.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

/**@defgroup tsk_fsm_group Finite-state machine (FSM) implementation.
*/

int tsk_fsm_exec_nothing(va_list *app)
{
    return 0/*success*/;
}
tsk_bool_t tsk_fsm_cond_always(const void* data1, const void* data2)
{
    return tsk_true;
}

/**@ingroup tsk_fsm_group
*/
tsk_fsm_t* tsk_fsm_create(tsk_fsm_state_id state_curr, tsk_fsm_state_id state_term)
{
    return (tsk_fsm_t*)tsk_object_new(tsk_fsm_def_t, state_curr, state_term);
}

/**@ingroup tsk_fsm_group
*/
tsk_fsm_entry_t* tsk_fsm_entry_create()
{
    return (tsk_fsm_entry_t*)tsk_object_new(tsk_fsm_entry_def_t);
}

/**@ingroup tsk_fsm_group
* Add entries (states) to the FSM.
* @param self The FSM.
* @param ... One of these  helper macros: @b TSK_FSM_ADD_*. MUST end with
* @b TSK_FSM_ADD_NULL.
* @retval Zero if succeed and non-zero error code otherwise.
*/
int tsk_fsm_set(tsk_fsm_t* self, ...)
{
    va_list args;
    int guard;

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    va_start(args, self);
    while((guard = va_arg(args, int)) == 1) {
        tsk_fsm_entry_t* entry;
        if((entry = tsk_fsm_entry_create())) {
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

/**@ingroup tsk_fsm_group
* Sets the @a callback function to call when the FSM enter in the final state.
* @param self The FSM.
* @param callback The callback function to call.
* @param callbackdata Opaque data (user-data) to pass to the callback function.
* @retval Zero if succeed and non-zero error code otherwise.
*/
int tsk_fsm_set_callback_terminated(tsk_fsm_t* self, tsk_fsm_onterminated_f callback, const void* callbackdata)
{
    if(self) {
        self->callback_term = callback;
        self->callback_data = callbackdata;
        return 0;
    }
    else {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
}

/**@ingroup tsk_fsm_group
* Execute an @a action. This action will probably change the current state of the FSM.
* @param self The FSM.
* @param action The id of the action to execute.
* @param cond_data1 The first opaque data to pass to the @a condition function.
* @param cond_data2 The first opaque data to pass to the @a condition function.
* @param ... Variable parameters to pass to the @a exec function.
* @retval Zero if succeed and non-zero error code otherwise.
*/
int tsk_fsm_act(tsk_fsm_t* self, tsk_fsm_action_id action, const void* cond_data1, const void* cond_data2, ...)
{
    tsk_list_item_t *item;
    va_list ap;
    tsk_bool_t found = tsk_false;
    tsk_bool_t terminates = tsk_false; /* thread-safeness -> DO NOT REMOVE THIS VARIABLE */
    int ret_exec = 0; /* success */
	tsk_fsm_entry_t* entry;

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if(tsk_fsm_terminated(self)) {
        TSK_DEBUG_WARN("The FSM is in the final state.");
        return -2;
    }

    // lock
    tsk_safeobj_lock(self);

    va_start(ap, cond_data2);
    tsk_list_foreach(item, self->entries) {
		if (!item || !item->data) {
			continue;
		}
        entry = (tsk_fsm_entry_t*)item->data;
        if(((entry->from != tsk_fsm_state_any) && (entry->from != tsk_fsm_state_current)) && (entry->from != self->current)) {
            continue;
        }

        if((entry->action != tsk_fsm_action_any) && (entry->action != action)) {
            continue;
        }

        // check condition
        if(entry->cond(cond_data1, cond_data2)) {
            // For debug information
            if(self->debug) {
                TSK_DEBUG_INFO("State machine: %s", entry->desc);
            }

            if(entry->to != tsk_fsm_state_any && entry->to != tsk_fsm_state_current) { /* Stay at the current state if destination state is Any or Current */
                self->current = entry->to;
            }

            if(entry->exec) {
                if((ret_exec = entry->exec(&ap))) {
                    TSK_DEBUG_INFO("State machine: Exec function failed. Moving to terminal state.");
                }
            }
            else { /* Nothing to execute */
                ret_exec = 0;
            }

            terminates = (ret_exec || (self->current == self->term));
            found = tsk_true;
            break;
        }
    }
    va_end(ap);

    // unlock
    tsk_safeobj_unlock(self);

    /* Only call the callback function after unlock. */
    if(terminates) {
        self->current = self->term;
        if(self->callback_term) {
            self->callback_term(self->callback_data);
        }
    }
    if(!found) {
        TSK_DEBUG_INFO("State machine: No matching state found.");
    }

    return ret_exec;
}

tsk_fsm_state_id tsk_fsm_get_current_state(tsk_fsm_t* self)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_fsm_state_any;
    }
    return self->current;
}

int tsk_fsm_set_current_state(tsk_fsm_t* self, tsk_fsm_state_id new_state)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    self->current = new_state;
    return 0;
}

tsk_bool_t tsk_fsm_terminated(tsk_fsm_t* self)
{
    if(self) {
        return (self->current == self->term);
    }
    else {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_true;
    }
}


//=================================================================================================
//	fsm object definition
//
static tsk_object_t* tsk_fsm_ctor(tsk_object_t * self, va_list * app)
{
    tsk_fsm_t *fsm = (tsk_fsm_t*)self;
    if(fsm) {
        fsm->current = va_arg(*app, tsk_fsm_state_id);
        fsm->term = va_arg(*app, tsk_fsm_state_id);

        fsm->entries = tsk_list_create();

#if defined(DEBUG) || defined(_DEBUG)
        fsm->debug = 1; /* default value, could be changed at any time */
#endif
        tsk_safeobj_init(fsm);
    }

    return self;
}

static tsk_object_t* tsk_fsm_dtor(tsk_object_t * self)
{
    tsk_fsm_t *fsm = (tsk_fsm_t*)self;
    if(fsm) {
        /* If not in the terminal state ==>do it */
        /*if(fsm->current != fsm->term){
        	tsk_safeobj_lock(fsm);
        	if(fsm->callback_term){
        		fsm->callback_term(fsm->callback_data);
        	}
        	tsk_safeobj_unlock(fsm);
        }*/
        tsk_safeobj_deinit(fsm);

        TSK_OBJECT_SAFE_FREE(fsm->entries);
    }

    return self;
}

static const tsk_object_def_t tsk_fsm_def_s = {
    sizeof(tsk_fsm_t),
    tsk_fsm_ctor,
    tsk_fsm_dtor,
    tsk_null,
};
const tsk_object_def_t *tsk_fsm_def_t = &tsk_fsm_def_s;

//=================================================================================================
//	fsm entry object definition
//
static tsk_object_t* tsk_fsm_entry_ctor(tsk_object_t * self, va_list * app)
{
    tsk_fsm_entry_t *fsm_entry = (tsk_fsm_entry_t*)self;
    if(fsm_entry) {
    }

    return self;
}

static tsk_object_t* tsk_fsm_entry_dtor(tsk_object_t * self)
{
    tsk_fsm_entry_t *fsm_entry = (tsk_fsm_entry_t*)self;
    if(fsm_entry) {
        /* desc is "const char*" => should not be deleted */
        /* TSK_FREE(fsm_entry->desc); */
    }

    return self;
}
static int tsk_fsm_entry_cmp(const tsk_object_t *_entry1, const tsk_object_t *_entry2)
{
    const tsk_fsm_entry_t* entry1 = (const tsk_fsm_entry_t*)_entry1;
    const tsk_fsm_entry_t* entry2 = (const tsk_fsm_entry_t*)_entry2;
    if(entry1 && entry2) {
        /* Put "Any" states at the bottom (Strong)*/
        if(entry1->from == tsk_fsm_state_any) {
            return -20;
        }
        else if(entry2->from == tsk_fsm_state_any) {
            return +20;
        }

        /* Put "Any" actions at the bottom (Weak)*/
        if(entry1->action == tsk_fsm_action_any) {
            return -10;
        }
        else if(entry1->action == tsk_fsm_action_any) {
            return +10;
        }
        // put conditions first
        return entry1->cond ? -1 : (entry2->cond ? 1 : 0);
    }
    return 0;
}

static const tsk_object_def_t tsk_fsm_entry_def_s = {
    sizeof(tsk_fsm_entry_t),
    tsk_fsm_entry_ctor,
    tsk_fsm_entry_dtor,
    tsk_fsm_entry_cmp,
};
const tsk_object_def_t *tsk_fsm_entry_def_t = &tsk_fsm_entry_def_s;
