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

/**@file tcomp_nakinfo.c
 * @brief  RFC 4077 - A Negative Acknowledgement Mechanism for Signaling Compression (NACK)
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tcomp_nakinfo.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

/**@defgroup tcomp_nackinfo_group SigComp NACK information.
*/

/**@ingroup tcomp_nackinfo_group
* Creates a nack info message. You MUST use @ref tcomp_nackinfo_destroy to free the nackinfo.
* @retval The NACK info message.
* @sa @ref tcomp_nackinfo_destroy.
*/
tcomp_nackinfo_t* tcomp_nackinfo_create()
{
	tcomp_nackinfo_t *nackinfo = (tcomp_nackinfo_t *)tsk_calloc2(1, sizeof(tcomp_nackinfo_t));
	if(nackinfo)
	{
		nackinfo->version = NACK_VERSION;
		nackinfo->details = tcomp_buffer_create();
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new nackinfo.");
	}
	return nackinfo;
}

/**@ingroup tcomp_nackinfo_group
* Destroy a nackinfo message previously created using @ref tcomp_nackinfo_create.
* @param nackinfo The NACK info message to free.
* @sa @ref tcomp_nackinfo_create.
*/
void tcomp_nackinfo_destroy(tcomp_nackinfo_t **nackinfo)
{
	if(nackinfo && *nackinfo)
	{
		tcomp_buffer_destroy(&((*nackinfo)->details));
		tsk_free2(nackinfo);
	}
}