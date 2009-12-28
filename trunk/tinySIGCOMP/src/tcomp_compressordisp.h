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
#ifndef TCOMP_COMPRESSORDISP_H
#define TCOMP_COMPRESSORDISP_H

#include "tinysigcomp_config.h"
#include "tcomp_statehandler.h"
#include "tcomp_buffer.h"
#include "tcomp_types.h"
#include "tcomp_result.h"
#include "tcomp_compressor.h"

#include "tsk_list.h"
#include "tsk_object.h"
#include "tsk_safeobj.h"

#include <stdint.h>

#define TCOMP_MAX_COMPRESSORS		5

#define TCOMP_COMPRESSORDISP_CREATE(statehandler)		tsk_object_new(tcomp_compressordisp_def_t, (const tcomp_statehandler_t*)statehandler)
#define TCOMP_COMPRESSORDISP_SAFE_FREE(self)			tsk_object_unref(self), self = 0

typedef struct tcomp_compressordisp_s
{
	TSK_DECLARE_OBJECT;

	tcomp_compressor_compress compressors[TCOMP_MAX_COMPRESSORS];
	const tcomp_statehandler_t* stateHandler;

	TSK_DECLARE_SAFEOBJ;
}
tcomp_compressordisp_t;


int tcomp_compressordisp_compress(tcomp_compressordisp_t *dispatcher, uint64_t compartmentId, const void *input_ptr, size_t input_size, void *output_ptr, size_t *output_size, int stream);

void tcomp_compressordisp_addCompressor(tcomp_compressordisp_t *dispatcher, tcomp_compressor_compress compressor);

TINYSIGCOMP_API const void *tcomp_compressordisp_def_t;

#endif /* TCOMP_COMPRESSORDISP_H */
