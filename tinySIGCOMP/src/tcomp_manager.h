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

/**@file tcomp_manager.h
 * @brief  SigComp Manager.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *

 */
#ifndef TCOMP_MANAGER_H
#define TCOMP_MANAGER_H

#include "tinysigcomp_config.h"

#include "tcomp_result.h"
#include "tcomp_compressor.h"
#include "tcomp_nackinfo.h"

TCOMP_BEGIN_DECLS

typedef void tcomp_manager_handle_t;

TINYSIGCOMP_API tcomp_manager_handle_t* tcomp_manager_create();
TINYSIGCOMP_API int tcomp_manager_setUseOnlyACKedStates(tcomp_manager_handle_t* self, tsk_bool_t useOnlyACKedStates);

//
//	Compression / Decompression
//
TINYSIGCOMP_API tsk_size_t tcomp_manager_compress(tcomp_manager_handle_t *handle, const void* compartmentId, tsk_size_t compartmentIdSize, const void* input_ptr, tsk_size_t input_size, void* output_ptr, tsk_size_t output_size, tsk_bool_t stream);
#define tcomp_manager_compressUDP(handle, compartmentId, compartmentIdSize, input_ptr, input_size, output_ptr, output_size) tcomp_manager_compress((tcomp_manager_handle_t *)handle, (const void*) compartmentId, (tsk_size_t) compartmentIdSize, (const void*) input_ptr, (tsk_size_t) input_size, (void*) output_ptr, (tsk_size_t) output_size, tsk_false)
#define tcomp_manager_compressTCP(handle, compartmentId, compartmentIdSize, input_ptr, input_size, output_ptr, output_size) tcomp_manager_compress((tcomp_manager_handle_t *)handle, (const void*) compartmentId, (tsk_size_t) compartmentIdSize, (const void*) input_ptr, (tsk_size_t) input_size, (void*) output_ptr, (tsk_size_t) output_size, tsk_true) 
#define tcomp_manager_compressSCTP compressTCP

TINYSIGCOMP_API tsk_size_t tcomp_manager_decompress(tcomp_manager_handle_t *handle, const void* input_ptr, tsk_size_t input_size, tcomp_result_t *lpResult);
TINYSIGCOMP_API tsk_size_t tcomp_manager_getNextStreamMessage(tcomp_manager_handle_t *handle, tcomp_result_t *lpResult);

/*
*	Compartment management
*/
TINYSIGCOMP_API void tcomp_manager_provideCompartmentId(tcomp_manager_handle_t *handle, tcomp_result_t *lpResult);
TINYSIGCOMP_API void tcomp_manager_closeCompartment(tcomp_manager_handle_t *handle, const void *compartmentId, tsk_size_t compartmentIdSize);

/*
*	SigComp Parameters
*/
TINYSIGCOMP_API int tcomp_manager_setDecompression_Memory_Size(tcomp_manager_handle_t *handle, uint32_t dms);
TINYSIGCOMP_API uint32_t tcomp_manager_getDecompression_Memory_Size(tcomp_manager_handle_t *handle);
TINYSIGCOMP_API int tcomp_manager_setState_Memory_Size(tcomp_manager_handle_t *handle, uint32_t sms);
TINYSIGCOMP_API int tcomp_manager_setCycles_Per_Bit(tcomp_manager_handle_t *handle, uint8_t cpb);
TINYSIGCOMP_API int tcomp_manager_setSigComp_Version(tcomp_manager_handle_t *handle, uint8_t version);

/*
*	Compressors
*/
TINYSIGCOMP_API int tcomp_manager_addCompressor(tcomp_manager_handle_t *handle, tcomp_compressor_compress_f compressor);
TINYSIGCOMP_API int tcomp_manager_removeCompressor(tcomp_manager_handle_t *handle, tcomp_compressor_compress_f compressor);

/*
*	Dictionnaries
*/
TINYSIGCOMP_API int tcomp_manager_addSipSdpDictionary(tcomp_manager_handle_t *handle);
TINYSIGCOMP_API int tcomp_manager_addPresenceDictionary(tcomp_manager_handle_t *handle);

TINYSIGCOMP_GEXTERN const tsk_object_def_t *tcomp_manager_def_t;

TCOMP_END_DECLS

#endif /* TCOMP_MANAGER_H */
