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

/**@file tcomp_manager.c
 * @brief  SigComp Manager.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *

 */
#include "tcomp_manager.h"
#include "tsk_debug.h"

#include "tcomp_compressordisp.h"
#include "tcomp_decompressordisp.h"
#include "tcomp_statehandler.h"

#include "tsk_object.h"
#include "tsk_safeobj.h"

#define MAX_DMS	131072
#define MAX_SMS	131072
#define MAX_CPB	128

/**@defgroup tcomp_manager_group SigComp manager.
*/

/**SigComp manager.
*/
typedef struct tcomp_manager_s
{
	TSK_DECLARE_OBJECT;
	
	tcomp_compressordisp_t *dispatcher_compressor;
	tcomp_decompressordisp_t *dispatcher_decompressor;
	tcomp_statehandler_t *stateHandler;
	
	TSK_DECLARE_SAFEOBJ;
}
tcomp_manager_t;


/**Creates new SigComp manager.
*/
tcomp_manager_handle_t* tcomp_manager_create()
{
	return tsk_object_new(tcomp_manager_def_t);
}

/** Defines whether the compressor must only use ACKed states (should be false)
*/
int tcomp_manager_setUseOnlyACKedStates(tcomp_manager_handle_t* self, tsk_bool_t useOnlyACKedStates)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	return tcomp_statehandler_setUseOnlyACKedStates(((tcomp_manager_t*)self)->stateHandler, useOnlyACKedStates);
}

/**@ingroup tcomp_manager_group
*/
tsk_size_t tcomp_manager_compress(tcomp_manager_handle_t *handle, const void* compartmentId, tsk_size_t compartmentIdSize, const void* input_ptr, tsk_size_t input_size, void* output_ptr, tsk_size_t output_size, tsk_bool_t stream)
{
	tcomp_manager_t *manager = handle;
	tsk_size_t ret_size = output_size;

	if(!manager){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}
	
	if(tcomp_compressordisp_compress(manager->dispatcher_compressor, tcomp_buffer_createHash(compartmentId, compartmentIdSize), 
		input_ptr, input_size, output_ptr, &ret_size, stream))
	{
		return ret_size;
	}

	return 0;
}

/**@ingroup tcomp_manager_group
*/
tsk_size_t tcomp_manager_decompress(tcomp_manager_handle_t *handle, const void* input_ptr, tsk_size_t input_size, tcomp_result_t *lpResult)
{
	tcomp_manager_t *manager = handle;

	if(!manager){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}
	
	if(!lpResult || !lpResult->output_buffer){
		TSK_DEBUG_ERROR("You MUST initialize the sigcomp result and set a valid output buffer.");
		return 0;
	}

	/* Reset previous values */
	tcomp_result_reset(lpResult);
	
	if(tcomp_decompressordisp_decompress(manager->dispatcher_decompressor, input_ptr, input_size, lpResult)){
		return *tcomp_buffer_getIndexBytes(lpResult->output_buffer);
	}

	return 0;
}

/**@ingroup tcomp_manager_group
*/
tsk_size_t tcomp_manager_getNextStreamMessage(tcomp_manager_handle_t *handle, tcomp_result_t *lpResult)
{
	tcomp_manager_t *manager = handle;
	if(!manager){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}

	if(!lpResult || !tcomp_buffer_getSize(lpResult->output_buffer)){
		TSK_DEBUG_ERROR("Invalid result.");
		return 0;
	}

	if(!lpResult->isStreamBased){
		TSK_DEBUG_ERROR("You MUST provide stream buffer.");
		return 0;
	}

	_tcomp_result_reset(lpResult, tsk_false, tsk_false);
	
	if(tcomp_decompressordisp_getNextMessage(manager->dispatcher_decompressor, lpResult)){
		return *tcomp_buffer_getIndexBytes(lpResult->output_buffer);
	}

	return 0;
}

/**@ingroup tcomp_manager_group
*/
void tcomp_manager_provideCompartmentId(tcomp_manager_handle_t *handle, tcomp_result_t *lpResult)
{
	tcomp_manager_t *manager = handle;
	if(!manager){
		TSK_DEBUG_ERROR("Invalid parameter");
		return;
	}

	tcomp_statehandler_handleResult(manager->stateHandler, &lpResult);
}

/**@ingroup tcomp_manager_group
*/
void tcomp_manager_closeCompartment(tcomp_manager_handle_t *handle, const void *compartmentId, tsk_size_t compartmentIdSize)
{
	tcomp_manager_t *manager = handle;
	if(!manager){
		TSK_DEBUG_ERROR("Invalid parameter");
		return;
	}

	tcomp_statehandler_deleteCompartment(manager->stateHandler, tcomp_buffer_createHash(compartmentId, compartmentIdSize));
}

/**@ingroup tcomp_manager_group
* Sets the decompression memory size (RFC 3320 section 3.3).
* @param handle The SigComp manager.
* @param dms The new decompression memory size value.
* @retval Zero if succeed and non-zero error code otherwise.
*/
int tcomp_manager_setDecompression_Memory_Size(tcomp_manager_handle_t *handle, uint32_t dms)
{
	tcomp_manager_t *manager = handle;
	if(!manager){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	
	return tcomp_params_setDmsValue(manager->stateHandler->sigcomp_parameters, (dms > MAX_DMS ? MAX_DMS : dms));
}

/**@ingroup tcomp_manager_group
* Gets the decompression memory size (RFC 3320 section 3.3).
* @param handle The SigComp manager.
* @retval The current decompression memory size value.
*/
uint32_t tcomp_manager_getDecompression_Memory_Size(tcomp_manager_handle_t *handle)
{
	tcomp_manager_t *manager = handle;
	if(!manager){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}
	return (manager->stateHandler && manager->stateHandler->sigcomp_parameters)
		? manager->stateHandler->sigcomp_parameters->dmsValue
		: 0;
}

/**@ingroup tcomp_manager_group
* Sets the state memory size (RFC 3320 section 3.3).
* @param handle The SigComp manager.
* @param sms The new state memory size value.
* @retval Zero if succeed and non-zero error code otherwise.
*/
int tcomp_manager_setState_Memory_Size(tcomp_manager_handle_t *handle, uint32_t sms)
{
	tcomp_manager_t *manager = handle;
	if(!manager){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	return tcomp_params_setSmsValue(manager->stateHandler->sigcomp_parameters, (sms > MAX_SMS ? MAX_SMS : sms));
}

/**@ingroup tcomp_manager_group
* Sets the Cycle Per Bit (RFC 3320 section 3.3).
* @param handle The SigComp manager.
* @param cpb The new cycle per bit value.
* @retval Zero if succeed and non-zero error code otherwise.
*/
int tcomp_manager_setCycles_Per_Bit(tcomp_manager_handle_t *handle, uint8_t cpb)
{
	tcomp_manager_t *manager = handle;
	if(!manager){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	return tcomp_params_setCpbValue(manager->stateHandler->sigcomp_parameters, (cpb > MAX_CPB ? MAX_CPB : cpb));
}

/**@ingroup tcomp_manager_group
* Sets the SigComp version (RFC 3320 section 3.3).
* @param handle The SigComp manager.
* @param version The SigComp version. Only 2.0 is supported.
* @retval Zero if succeed and non-zero error code otherwise.
*/
int tcomp_manager_setSigComp_Version(tcomp_manager_handle_t *handle, uint8_t version)
{
	tcomp_manager_t *manager = handle;
	if(!manager){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	manager->stateHandler->sigcomp_parameters->SigComp_version = version;
	return 0;
}

/**@ingroup tcomp_manager_group
* Adds a new compressor to the dispatcher.
* @param handle The SigComp manager holding the dispatcher.
* @param compressor The compressor to add.
* @retval Zero if succeed and non-zero error code otherwise.
*/
int tcomp_manager_addCompressor(tcomp_manager_handle_t *handle, tcomp_compressor_compress_f compressor)
{
	tcomp_manager_t *manager = handle;
	if(!manager){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	return tcomp_compressordisp_addCompressor(manager->dispatcher_compressor, compressor);
}

/**@ingroup tcomp_manager_group
* Removes the compressor from the dispatcher.
* @param handle The SigComp manager holding the dispatcher.
* @param compressor The compressor to add.
* @retval Zero if succeed and non-zero error code otherwise.
*/
int tcomp_manager_removeCompressor(tcomp_manager_handle_t *handle, tcomp_compressor_compress_f compressor)
{
	tcomp_manager_t *manager = handle;
	if(!manager){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	return tcomp_compressordisp_removeCompressor(manager->dispatcher_compressor, compressor);
}

/**@ingroup tcomp_manager_group
* Adds SIP/SDP dictionary (RFC 3485) to the state handler.
* @param handle The SigComp manager holding the state handler.
* @retval Zero if succeed and non-zero otherwise.
*/
int tcomp_manager_addSipSdpDictionary(tcomp_manager_handle_t *handle)
{
	tcomp_manager_t *manager = handle;
	if(!manager){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	return tcomp_statehandler_addSipSdpDictionary(manager->stateHandler);
}

/**@ingroup tcomp_manager_group
* Adds Presence dictionary (RFC 5112) to the state handler.
* @param handle The SigComp manager holding the state handler.
* @retval Zero if succeed and non-zero otherwise.
*/
int tcomp_manager_addPresenceDictionary(tcomp_manager_handle_t *handle)
{
	tcomp_manager_t *manager = handle;
	if(!manager){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	return tcomp_statehandler_addPresenceDictionary(manager->stateHandler);
}











//========================================================
//	SigComp manager object definition
//

static void* tcomp_manager_ctor(void * self, va_list * app)
{
	tcomp_manager_t *manager = self;
	if(manager)
	{
		manager->stateHandler = tcomp_statehandler_create();
		manager->dispatcher_compressor = tcomp_compressordisp_create(manager->stateHandler);
		manager->dispatcher_decompressor = tcomp_decompressordisp_create(manager->stateHandler);

		/* Initialize safeobject */
		tsk_safeobj_init(manager);
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new manager.");
	}

	return self;
}

static void* tcomp_manager_dtor(void *self)
{
	tcomp_manager_t *manager = self;
	if(manager){		
		TSK_OBJECT_SAFE_FREE(manager->stateHandler);
		TSK_OBJECT_SAFE_FREE(manager->dispatcher_compressor);
		TSK_OBJECT_SAFE_FREE(manager->dispatcher_decompressor);

		/* Deinitialize safeobject */
		tsk_safeobj_deinit(manager);
	}
	else{
		TSK_DEBUG_ERROR("Null manager.");
	}
	
	return self;
}

static const tsk_object_def_t tcomp_manager_def_s = 
{
	sizeof(tcomp_manager_t),
	tcomp_manager_ctor,
	tcomp_manager_dtor,
	tsk_null
};
const tsk_object_def_t *tcomp_manager_def_t = &tcomp_manager_def_s;
