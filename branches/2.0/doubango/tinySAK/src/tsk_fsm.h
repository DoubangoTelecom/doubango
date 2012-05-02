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

/**@file tsk_fsm.h
 * @brief Finite-state machine (FSM) implementation.
 * @sa http://en.wikipedia.org/wiki/Finite-state_machine.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef _TINYSAK_FSM_H_
#define _TINYSAK_FSM_H_

#include "tinysak_config.h"
#include "tsk_list.h"
#include "tsk_safeobj.h"

/**@ingroup tsk_fsm_group
* @def TSK_FSM_ONTERMINATED
*/

TSK_BEGIN_DECLS

#define TSK_FSM_ONTERMINATED_F(self)				(tsk_fsm_onterminated_f)(self)

/**@ingroup tsk_fsm_group
* @def tsk_fsm_state_any
*/
/**@ingroup tsk_fsm_group
* @def tsk_fsm_state_default
*/
/**@ingroup tsk_fsm_group
* @def tsk_fsm_state_none
*/
/**@ingroup tsk_fsm_group
* @def tsk_fsm_state_final
*/
#define tsk_fsm_state_any -0xFFFF
#define tsk_fsm_state_current -0xFFF0
#define tsk_fsm_state_none -0xFF00
#define tsk_fsm_state_final -0xF000

/**@ingroup tsk_fsm_group
* @def tsk_fsm_action_any
*/
#define tsk_fsm_action_any -0xFFFF

/**@ingroup tsk_fsm_group
* @def tsk_fsm_state_id_t
*/
/**@ingroup tsk_fsm_group
* @def tsk_fsm_action_id_t
*/
/**@ingroup tsk_fsm_group
* @def tsk_fsm_cond
*/
/**@ingroup tsk_fsm_group
* @def tsk_fsm_exec
*/
/**@ingroup tsk_fsm_group
* @def tsk_fsm_onterminated
*/

typedef int tsk_fsm_state_id;
typedef int tsk_fsm_action_id;
typedef tsk_bool_t (*tsk_fsm_cond)(const void*, const void*);
typedef int (*tsk_fsm_exec)(va_list *app);
typedef int (*tsk_fsm_onterminated_f)(const void*);


/**@ingroup tsk_fsm_group
* @def TSK_FSM_ADD
*/
/**@ingroup tsk_fsm_group
* @def TSK_FSM_ADD_ALWAYS
*/
/**@ingroup tsk_fsm_group
* @def TSK_FSM_ADD_NOTHING
*/
/**@ingroup tsk_fsm_group
* @def TSK_FSM_ADD_ALWAYS_NOTHING
*/
/**@ingroup tsk_fsm_group
* @def TSK_FSM_ADD_DEFAULT
*/
/**@ingroup tsk_fsm_group
* @def TSK_FSM_ADD_NULL
*/
#define TSK_FSM_ADD(from, action, cond, to, exec, desc)\
	1,\
	(tsk_fsm_state_id)from, \
	(tsk_fsm_action_id)action, \
	(tsk_fsm_cond)cond, \
	(tsk_fsm_state_id)to, \
	(tsk_fsm_exec)exec, \
	(const char*)desc
#define TSK_FSM_ADD_ALWAYS(from, action, to, exec, desc) TSK_FSM_ADD(from, action, tsk_fsm_cond_always, to, exec, desc)
#define TSK_FSM_ADD_NOTHING(from, action, cond, desc) TSK_FSM_ADD(from, action, cond, from, tsk_fsm_exec_nothing, desc)
#define TSK_FSM_ADD_ALWAYS_NOTHING(from, desc)	TSK_FSM_ADD(from, tsk_fsm_action_any, tsk_fsm_cond_always, from, tsk_fsm_exec_nothing, desc)
#define TSK_FSM_ADD_DEFAULT()
#define TSK_FSM_ADD_NULL()\
	tsk_null

/**@ingroup tsk_fsm_group
* FSM entry.
*/
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

/**@ingroup tsk_fsm_group
* List of @ref tsk_fsm_entry_t elements. 
*/
typedef tsk_list_t tsk_fsm_entries_L_t;

/**@ingroup tsk_fsm_group
* FSM.
*/
typedef struct tsk_fsm_s
{
	TSK_DECLARE_OBJECT;

	unsigned debug:1;
	tsk_fsm_state_id current;
	tsk_fsm_state_id term;
	tsk_fsm_entries_L_t* entries;

	tsk_fsm_onterminated_f callback_term;
	const void* callback_data;

	TSK_DECLARE_SAFEOBJ;
}
tsk_fsm_t;

TINYSAK_API tsk_fsm_t* tsk_fsm_create(tsk_fsm_state_id state_curr, tsk_fsm_state_id state_term);

TINYSAK_API int tsk_fsm_exec_nothing(va_list *app);
TINYSAK_API tsk_bool_t tsk_fsm_cond_always(const void*, const void*);
TINYSAK_API int tsk_fsm_set(tsk_fsm_t* self, ...);
TINYSAK_API int tsk_fsm_set_callback_terminated(tsk_fsm_t* self, tsk_fsm_onterminated_f callback, const void* callbackdata);
TINYSAK_API int tsk_fsm_act(tsk_fsm_t* self, tsk_fsm_action_id action, const void* cond_data1, const void* cond_data2, ...);
TINYSAK_API tsk_fsm_state_id tsk_fsm_get_current_state(tsk_fsm_t* self);
TINYSAK_API int tsk_fsm_set_current_state(tsk_fsm_t* self, tsk_fsm_state_id new_state);
TINYSAK_API tsk_bool_t tsk_fsm_terminated(tsk_fsm_t* self);

TINYSAK_GEXTERN const tsk_object_def_t *tsk_fsm_def_t;
TINYSAK_GEXTERN const tsk_object_def_t *tsk_fsm_entry_def_t;

TSK_END_DECLS

#endif /* _TINYSAK_FSM_H_ */
