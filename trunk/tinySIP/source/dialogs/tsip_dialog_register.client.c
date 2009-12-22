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

/**@file tsip_dialog_register.client.c
 * @brief SIP dialog register (Client side).
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/dialogs/tsip_dialog_register.h"

#include "tsk_memory.h"

#define DEBUG_STATE_MACHINE				1

void tsip_dialog_register_init(tsip_dialog_register_t *self)
{
	/* Initialize the state machine.
	*/
	tsip_dialog_registerContext_Init(&self->_fsm, self);

#if defined(_DEBUG) || defined(DEBUG)
	 setDebugFlag(&(self->_fsm), DEBUG_STATE_MACHINE);
#endif
}

int tsip_dialog_register_start(tsip_dialog_register_t *self, const tsip_operation_handle_t* operation)
{
	int ret = -1;
	if(self && operation)
	{
		tsip_dialog_registerContext_sm_send(&self->_fsm);
		return ret;
	}
	return -1;
}

void tsip_dialog_register_Started_2_Trying_X_send(tsip_dialog_register_t *self)
{
	//ret = tsip_dialog_send(TSIP_TRANSAC(self), (const tsip_message_t *)request);
}

void tsip_dialog_register_Trying_2_Trying_X_1xx(tsip_dialog_register_t *self)
{

}

void tsip_dialog_register_Trying_2_Connected_X_2xx(tsip_dialog_register_t *self)
{

}

void tsip_dialog_register_Trying_2_Terminated_X_2xx(tsip_dialog_register_t *self)
{

}

void tsip_dialog_register_Trying_2_Trying_X_401_407_421_494(tsip_dialog_register_t *self)
{

}

void tsip_dialog_register_Trying_2_Terminated_X_401_407_421_494(tsip_dialog_register_t *self)
{

}

void tsip_dialog_register_Trying_2_Trying_X_423(tsip_dialog_register_t *self)
{

}

void tsip_dialog_register_Trying_2_Terminated_X_300_to_699(tsip_dialog_register_t *self)
{

}

void tsip_dialog_register_Trying_2_Terminated_X_cancel(tsip_dialog_register_t *self)
{
}

void tsip_dialog_register_Connected_2_Trying_X_unregister(tsip_dialog_register_t *self)
{

}

void tsip_dialog_register_Connected_2_Trying_X_refresh(tsip_dialog_register_t *self)
{

}

void tsip_dialog_register_Any_2_Terminated_X_transportError(tsip_dialog_register_t *self)
{

}


















//========================================================
//	SIP dialog REGISTER object definition
//
static void* tsip_dialog_register_create(void * self, va_list * app)
{
	tsip_dialog_register_t *dialog = self;
	if(dialog)
	{
		const tsip_stack_handle_t *stack = va_arg(*app, const tsip_stack_handle_t *);

		/* Initialize base class */
		tsip_dialog_init(TSIP_DIALOG(self), stack);
	}
	return self;
}

static void* tsip_dialog_register_destroy(void * self)
{ 
	tsip_dialog_register_t *dialog = self;
	if(dialog)
	{
		/* DeInitialize base class */
		tsip_dialog_deinit(TSIP_DIALOG(self));
	}
	return self;
}

static int tsip_dialog_register_cmp(const void *obj1, const void *obj2)
{
	return -1;
}

static const tsk_object_def_t tsip_dialog_register_def_s = 
{
	sizeof(tsip_dialog_register_t),
	tsip_dialog_register_create, 
	tsip_dialog_register_destroy,
	tsip_dialog_register_cmp, 
};
const void *tsip_dialog_register_def_t = &tsip_dialog_register_def_s;