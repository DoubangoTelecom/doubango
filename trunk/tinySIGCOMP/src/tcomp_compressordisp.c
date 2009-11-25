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

/**@file tcomp_compressordisp.h
 * @brief  Entity that receives application messages, invokes a compressor,and forwards the resulting SigComp compressed messages to a remote
 * endpoint.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tcomp_compressordisp.h"
#include "tsk_debug.h"

/**@defgroup tcomp_compressordisp_group SIGCOMP decompressor dispatcher.
* Entity that receives application messages, invokes a compressor,and forwards the resulting SigComp compressed messages to a remote
* endpoint.
*/













//========================================================
//	SigComp compressor dispatcher object definition
//

/**@ingroup tcomp_compressordisp_group
*/
static void* tcomp_compressordisp_create(void * self, va_list * app)
{
	tcomp_compressordisp_t *compressordisp = self;
	if(compressordisp)
	{
		compressordisp->stateHandler = va_arg(*app, const tcomp_statehandler_t*);

		/* Initialize safeobject */
		tsk_safeobj_init(compressordisp);
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new compressor dispatcher.");
	}

	return self;
}

/**@ingroup tcomp_compressordisp_group
*/
static void* tcomp_compressordisp_destroy(void *self)
{
	tcomp_compressordisp_t *compressordisp = self;
	if(compressordisp)
	{
		/* Deinitialize safeobject */
		tsk_safeobj_deinit(compressordisp);

		// FIXME: clear compressors
	}
	else TSK_DEBUG_ERROR("Null dispatcher.");
	
	return self;
}

static const tsk_object_def_t tcomp_compressordisp_def_s = 
{
	sizeof(tcomp_compressordisp_t),
	tcomp_compressordisp_create,
	tcomp_compressordisp_destroy,
	0,
	0,
	0
};
const void *tcomp_compressordisp_def_t = &tcomp_compressordisp_def_s;