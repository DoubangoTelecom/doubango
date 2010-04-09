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

/**@file tcomp_compressor.h
 * @brief  SigComp Deflate compressor(Compressor data).
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tcomp_deflatedata.h"

#include "tsk_debug.h"


tsk_bool_t tcomp_deflatedata_isStateful(tcomp_deflatedata_t *deflatedata)
{
	if(!deflatedata){
		TSK_DEBUG_ERROR("NULL defalte data.");
		return tsk_false;
	}

	return deflatedata->stream_acked.stateful; 
}








//========================================================
//	Deflate compressor data object definition
//

static void* tcomp_deflatedata_create(void * self, va_list * app)
{
	tcomp_deflatedata_t *deflatedata = self;
	if(deflatedata)
	{
		/* Initialize safeobject */
		tsk_safeobj_init(deflatedata);
		
		deflatedata->stream = va_arg(*app, int);
		deflatedata->zLevel = va_arg(*app, int);
		deflatedata->zWindowBits = va_arg(*app, int);;
	}
	else TSK_DEBUG_ERROR("Null SigComp defalte data.");

	return self;
}

static void* tcomp_deflatedata_destroy(void *self)
{
	tcomp_deflatedata_t *deflatedata = self;
	if(deflatedata)
	{
		/* Deinitialize safeobject */
		tsk_safeobj_deinit(deflatedata);
		
		TSK_OBJECT_SAFE_FREE(deflatedata->ghostState);

		tcomp_deflatedata_zUnInit(deflatedata);
	}
	else TSK_DEBUG_ERROR("Null SigComp defalte data.");

	return self;
}

static const tsk_object_def_t tsk_deflatedata_def_s = 
{
	sizeof(tcomp_deflatedata_t),
	tcomp_deflatedata_create,
	tcomp_deflatedata_destroy,
	0,
};
const void *tcomp_deflatedata_def_t = &tsk_deflatedata_def_s;
