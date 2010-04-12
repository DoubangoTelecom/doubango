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

/**@file tsms_tpdu_command.c
 * @brief SMS TPDU SMS-COMMAND message as per 3GPP TS 23.040 section 9.2.2.4.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinySMS/tpdu/tsms_tpdu_command.h"








//=================================================================================================
//	SMS TPDU SMS-COMMAND object definition
//
static tsk_object_t* tsms_tpdu_command_create(tsk_object_t * self, va_list * app)
{
	tsms_tpdu_command_t *command = self;
	if(command){
		//tsk_bool_t orig = va_arg(*app, tsk_bool_t);
		TSMS_TPDU_MESSAGE(command)->mti = tsms_tpdu_mti_command_mo;
	}
	return self;
}

static tsk_object_t* tsms_tpdu_command_destroy(tsk_object_t * self)
{ 
	tsms_tpdu_command_t *command = self;
	if(command){
		TSK_OBJECT_SAFE_FREE(command->da);
	}

	return self;
}

static const tsk_object_def_t tsms_tpdu_command_def_s = 
{
	sizeof(tsms_tpdu_command_t),
	tsms_tpdu_command_create, 
	tsms_tpdu_command_destroy,
	tsk_null, 
};
const tsk_object_def_t *tsms_tpdu_command_def_t = &tsms_tpdu_command_def_s;
