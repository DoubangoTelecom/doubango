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

/**@file tcomp_nackinfo.c
 * @brief  RFC 4077 - A Negative Acknowledgement Mechanism for Signaling Compression (NACK).
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tcomp_nackinfo.h"
#include "tsk_memory.h"
#include "tsk_debug.h"


//========================================================
//	NackInfo object definition
//

/*
* Creates a nack info message. You MUST use @ref tcomp_nackinfo_destroy to free the nackinfo.
* @retval The NACK info message.
* @sa @ref tcomp_nackinfo_destroy.
*/
static tsk_object_t* tcomp_nackinfo_create(tsk_object_t *self, va_list* app)
{
	tcomp_nackinfo_t *nackinfo = self;
	if(nackinfo){
		nackinfo->version = NACK_VERSION;
		nackinfo->details = TCOMP_BUFFER_CREATE();
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new nackinfo.");
	}
	return self;
}

/*
* Destroy a nackinfo message previously created using @ref tcomp_nackinfo_create.
* @param nackinfo The NACK info message to free.
* @sa @ref tcomp_nackinfo_create.
*/
static tsk_object_t* tcomp_nackinfo_destroy(tsk_object_t* self)
{
	tcomp_nackinfo_t *nackinfo = self;
	if(nackinfo){
		TSK_OBJECT_SAFE_FREE(nackinfo->details);
	}
	return self;
}


static const tsk_object_def_t tcomp_nackinfo_def_s = 
{
	sizeof(tcomp_nackinfo_t),
	tcomp_nackinfo_create, 
	tcomp_nackinfo_destroy,
	tsk_null
};
const tsk_object_def_t* tcomp_nackinfo_def_t = &tcomp_nackinfo_def_s;

