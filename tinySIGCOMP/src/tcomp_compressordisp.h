/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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

/**@file tcomp_compressordisp.h
 * @brief  Entity that receives application messages, invokes a compressor,and forwards the resulting SigComp compressed messages to a remote
 * endpoint.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *

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

TCOMP_BEGIN_DECLS


#define TCOMP_MAX_COMPRESSORS		5

/**Compressor dispatcher.
*/
typedef struct tcomp_compressordisp_s {
    TSK_DECLARE_OBJECT;

    tcomp_compressor_compress_f compressors[TCOMP_MAX_COMPRESSORS];
    const tcomp_statehandler_t* stateHandler;

    TSK_DECLARE_SAFEOBJ;
}
tcomp_compressordisp_t;

tcomp_compressordisp_t* tcomp_compressordisp_create(const tcomp_statehandler_t* statehandler);

tsk_bool_t tcomp_compressordisp_compress(tcomp_compressordisp_t *dispatcher, uint64_t compartmentId, const void *input_ptr, tsk_size_t input_size, void *output_ptr, tsk_size_t *output_size, tsk_bool_t stream);

int tcomp_compressordisp_addCompressor(tcomp_compressordisp_t *dispatcher, tcomp_compressor_compress_f compressor);
int tcomp_compressordisp_removeCompressor(tcomp_compressordisp_t *dispatcher, tcomp_compressor_compress_f compressor);

TINYSIGCOMP_GEXTERN const tsk_object_def_t *tcomp_compressordisp_def_t;

TCOMP_END_DECLS

#endif /* TCOMP_COMPRESSORDISP_H */
