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

/**@file tsk_fsm.h
 * @brief Finite-state machine (FSM) implementation.
 * @sa http://en.wikipedia.org/wiki/Finite-state_machine.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TINYSAK_FSM_H_
#define _TINYSAK_FSM_H_

#include "tinySAK_config.h"
#include "tsk_list.h"
#include "tsk_safeobj.h"

TSK_BEGIN_DECLS

#define TSK_FSM_CREATE(state_curr, state_term)		tsk_object_new(tsk_fsm_def_t, (tsk_fsm_state_id)state_curr, (tsk_fsm_state_id)state_term)
#define TSK_FSM_ENTRY_CREATE()						tsk_object_new(tsk_fsm_entry_def_t)


/*==============
*	from (stateA) ---> [fsm_actionB {fsm_cond_B or fsm_cond_always}] ---> to (stateB) ==> execute(fsm_functionB)
*				  ---> [fsm_actionC {fsm_cond_C or fsm_cond_always}] ---> to (stateC) ==> execute(fsm_functionC)
*
*	The starting state is "stateA". If after executing "fsm_actionB" action, "fsm_cond_B" or "fsm_cond_always" condition is true
*	then we can move to "stateB". At "stateB" we execute "fsm_functionB" function.
*/

#define tsk_fsm_state_any -0xFFFF
#define tsk_fsm_state_default -0xFFF0
#define tsk_fsm_state_none -0xFF00
#define tsk_fsm_state_final -0xF000

#define tsk_fsm_action_any -0xFFFF

typedef int tsk_fsm_state_id;
typedef int tsk_fsm_action_id;
typedef int (*tsk_fsm_cond)(void*, void*);
typedef int (*tsk_fsm_exec)(va_list *app);
typedef int (*tsk_fsm_onterminated)(const void*);

#define TSK_FSM_ADD(from, action, cond, to, exec, desc)\
	1,\
	(tsk_fsm_state_id)from, \
	(tsk_fsm_action_id)action, \
	(tsk_fsm_cond)cond, \
	(tsk_fsm_state_id)to, \
	(tsk_fsm_exec)exec, \
	(const char*)desc
#define TSK_FSM_ADD_ALWAYS(from, action, to, exec, desc) TSK_FSM_ADD(from, action, tsk_fsm_cond_always, to, exec, desc)
#define TSK_FSM_ADD_NOTING(from, action, cond, desc) TSK_FSM_ADD(from, action, cond, from, tsk_fsm_exec_nothing, desc)
#define TSK_FSM_ADD_ALWAYS_NOTHING(from, desc)	TSK_FSM_ADD(from, tsk_fsm_action_any, tsk_fsm_cond_always, from, tsk_fsm_exec_nothing, desc)
#define TSK_FSM_ADD_DEFAULT()
#define TSK_FSM_ADD_NULL()\
	0

typedef struct tsk_fsm_entry_s
{
	TSK_DECLARE_OBJECT;

	tsk_fsm_state_id from;
	tsk_fsm_action_id action;
	tsk_fsm_cond cond;
	tsk_fsm_state_id to;
	tsk_fsm_exec exec;
	const char* desc;
}
tsk_fsm_entry_t;
typedef tsk_list_t tsk_fsm_entries_L_t;

typedef struct tsk_fsm_s
{
	TSK_DECLARE_OBJECT;

	unsigned debug:1;
	tsk_fsm_state_id current;
	tsk_fsm_state_id term;
	tsk_fsm_entries_L_t* entries;

	tsk_fsm_onterminated callback_term;
	const void* callback_data;

	TSK_DECLARE_SAFEOBJ;
}
tsk_fsm_t;

TINYSAK_API int tsk_fsm_exec_nothing(va_list *app);
TINYSAK_API int tsk_fsm_cond_always(void*, void*);
TINYSAK_API int tsk_fsm_set(tsk_fsm_t* self, ...);
TINYSAK_API int tsk_fsm_set_callback_terminated(tsk_fsm_t* self, tsk_fsm_onterminated callback, const void* callbackdata);
TINYSAK_API int tsk_fsm_act(tsk_fsm_t* self, tsk_fsm_action_id action, void* cond_data1, void* cond_data2, ...);


TINYSAK_GEXTERN const void *tsk_fsm_def_t;
TINYSAK_GEXTERN const void *tsk_fsm_entry_def_t;

TSK_END_DECLS

#endif /* _TINYSAK_FSM_H_ */
