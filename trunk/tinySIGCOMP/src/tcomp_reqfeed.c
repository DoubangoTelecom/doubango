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

/**@file tcomp_reqfeed.c
 * @brief  SIGCOMP requested feedback item as per rfc 3320 subclause 9.4.9.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tcomp_reqfeed.h"
#include "tcomp_buffer.h"

#include "tsk_memory.h"
#include "tsk_debug.h"

/**@defgroup tcomp_reqfeed_group SigComp requested feedback item.
*/


/**@ingroup tcomp_reqfeed_group
* Reset the feedback.
* @param feedback The feedback to reset.
*/
void tcomp_reqfeed_reset(tcomp_reqfeed_t* feedback)
{
	if(feedback)
	{
		tcomp_buffer_freeBuff(feedback->item);
		tcomp_buffer_reset(feedback->item);
	
		feedback->Q = feedback->S = feedback->I = 0;
	}
	else TSK_DEBUG_ERROR("NULL feedback.");
}




//========================================================
//	Requested feedback object definition
//
/**@ingroup tcomp_reqfeed_group
* Create new feedback item. You MUST use @ref tcomp_reqfeed_destroy to free the feedback.
* @retval The new feedback item.
* @sa @ref tcomp_reqfeed_destroy.
*/
static void* tcomp_reqfeed_create(void * self, va_list * app)
{
	tcomp_reqfeed_t *feedback = self;
	if(feedback)
	{
		feedback->item = TCOMP_BUFFER_CREATE();
	}
	else TSK_DEBUG_ERROR("Failed to create new feedback.");

	return self;
}

/**@ingroup tcomp_reqfeed_group
* Free a feedback item previously created using @ref tcomp_reqfeed_create.
* @param feedback The feedback to free.
* @sa @ref tcomp_reqfeed_create.
*/
static void* tcomp_reqfeed_destroy(void* self)
{
	tcomp_reqfeed_t *feedback = self;
	if(feedback)
	{
		TSK_OBJECT_SAFE_FREE(feedback->item);
	}
	else
	{
		TSK_DEBUG_WARN("NULL feedback");
	}
	return self;
}

static const tsk_object_def_t tcomp_reqfeed_def_s = 
{
	sizeof(tcomp_reqfeed_t),
	tcomp_reqfeed_create, 
	tcomp_reqfeed_destroy,
	0
};
const void *tcomp_reqfeed_def_t = &tcomp_reqfeed_def_s;
