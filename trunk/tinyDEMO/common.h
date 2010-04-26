/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango.org>
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
#if !defined(TINYDEMO_COMMON_H)
#define TINYDEMO_COMMON_H

#include "demo_config.h"

#include "cmd.h"

#include "tinysip.h" /* 3GPP IMS/LTE API */

_BEGIN_DECLS

typedef tsk_list_t ssessions_L_t;

typedef struct stack_s
{
	TSK_DECLARE_OBJECT;
	
	/* IMS/LTE stack */
	tsip_stack_handle_t * ims_stack;

	/* user's preference */
	char* impi;
	char* impu;
	char* password;
	char* realm;
	char* display_name;
	char* amf;
	char* operator_id;
}
stack_t;

typedef struct context_s
{
	TSK_DECLARE_OBJECT;

	stack_t* stack;

	ssessions_L_t* sessions;

	TSK_DECLARE_SAFEOBJ; /* For thread-safeness */
}
context_t;

int stack_config(tsk_options_L_t* options);
int stack_run(tsk_options_L_t* options);

int pred_find_session_by_id(const tsk_list_item_t *item, const void* id);

const tsk_object_def_t *stack_def_t;
const tsk_object_def_t *context_def_t;

_END_DECLS

#endif /* TINYDEMO_COMMON_H */
