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

/**@file tsip_dialog_register.h
 * @brief SIP dialog REGISTER.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYSIP_DIALOG_REGISTER_H
#define TINYSIP_DIALOG_REGISTER_H

#include "tinySIP_config.h"
#include "tinySIP/dialogs/tsip_dialog.h"

#include "tsk_fsm.h"

TSIP_BEGIN_DECLS

#define TSIP_DIALOG_REGISTER_CREATE(stack, operation)		tsk_object_new(tsip_dialog_register_def_t, (tsip_stack_handle_t *)stack, (tsip_operation_handle_t*) operation)

#define TSIP_DIALOG_REGISTER(self)							((tsip_dialog_register_t*)(self))

typedef struct tsip_dialog_register
{
	TSIP_DECLARE_DIALOG;

	tsk_fsm_t *fsm;

	tsip_timer_t timerrefresh;
	tsip_timer_t timershutdown;

	unsigned unregistering:1;
}
tsip_dialog_register_t;

//void tsip_dialog_register_init(tsip_dialog_register_t *self);
int tsip_dialog_register_start(tsip_dialog_register_t *self);

TINYSIP_GEXTERN const void *tsip_dialog_register_def_t;

TSIP_END_DECLS

#endif /* TINYSIP_DIALOG_REGISTER_H */

