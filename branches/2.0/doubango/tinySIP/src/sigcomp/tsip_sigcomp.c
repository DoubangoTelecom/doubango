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

/**@file tsip_sigcomp.c
 * @brief SigComp helper API.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/sigcomp/tsip_sigcomp.h"

#include "tcomp_manager.h" 

#include "tsk_safeobj.h"
#include "tsk_memory.h"
#include "tsk_string.h"
#include "tsk_list.h"
#include "tsk_debug.h"

#include <string.h> /* used by tsk_string.h macros */

/** SigComp compartment */
typedef struct tsip_sigcomp_compartment_s
{
	TSK_DECLARE_OBJECT;

	char* id;
	uint64_t stream_id;
	tcomp_result_t *decomp_result;
	
	/* tinySigComp library is thread-safe but I prefer to add this 
	* tcomp_manager_provideCompartmentId() is unsafe */
	TSK_DECLARE_SAFEOBJ;
}
tsip_sigcomp_compartment_t;
const tsk_object_def_t *tsip_sigcomp_compartment_def_t;
typedef tsk_list_t tsip_sigcomp_compartments_L_t;

/** SigComp handler */
typedef struct tsip_sigcomp_s
{
	TSK_DECLARE_OBJECT;

	tcomp_manager_handle_t *manager;
	tsip_sigcomp_compartments_L_t* compartments;

	TSK_DECLARE_SAFEOBJ;
}
tsip_sigcomp_t;
const tsk_object_def_t *tsip_sigcomp_def_t;

/*== Predicate function to find a compartment by id */
static int pred_find_compartment_by_id(const tsk_list_item_t *item, const void *id)
{
	if(item && item->data){
		tsip_sigcomp_compartment_t *compartment = item->data;
		return tsk_strcmp(compartment->id, (const char*)id);
	}
	return -1;
}

/** Creates new SigComp handler
* @param cpb Cycles Per Bit (16, 32, 64 or 128)
* @param dms Decompression Memory Size (should be: 0, 2048, 4096, 8192, 16384, 32768, 65536 or 131072)
* @param sms State Memory Size (should be: 0, 2048, 4096, 8192, 16384, 32768, 65536 or 131072)
* @retval A pointer to a SigComp handler if succeed and Null otherwise.
*/
tsip_sigcomp_handle_t* tsip_sigcomp_handler_create(uint8_t cpb, uint32_t dms, uint32_t sms)
{
	tsip_sigcomp_t* sigcomp = tsk_object_new(tsip_sigcomp_def_t);

	/* create SigComp handler */
	if(!sigcomp){
		TSK_DEBUG_ERROR("Failed to create new SigComp handler");
		return tsk_null;
	}

	/* create SigComp manager */
	if(!(sigcomp->manager = tcomp_manager_create()) || !(sigcomp->compartments = tsk_list_create())){
		TSK_DEBUG_ERROR("Failed to create new SigComp manager");
		TSK_OBJECT_SAFE_FREE(sigcomp);
		return tsk_null;
	}

	tcomp_manager_setCycles_Per_Bit(sigcomp->manager, cpb);
	tcomp_manager_setDecompression_Memory_Size(sigcomp->manager, dms);
	tcomp_manager_setState_Memory_Size(sigcomp->manager, sms);

	return sigcomp;
}
/** Creates new SigComp handler
* Adds/Removes dictionaries. These dictionaries will be used both for compression and decompression.
* And this will apply to all compartments holded by the handler.
* @param self The SigComp manager holding the dictionaries to add/remove.
* @param sip_n_sdp Whether to add the SIP/SDP dictionary (RFC 3485). If @a tsk_false_t this dictionary will be added,
* otherwise it will be removed.
* @param pres Whether to add the Presence dictionary (RFC 5112). If @a tsk_false_t this dictionary will be added,
* otherwise it will be removed.
*/
int tsip_sigcomp_handler_set_dicts(tsip_sigcomp_handle_t* self, tsk_bool_t sip_n_sdp, tsk_bool_t pres)
{
	tsip_sigcomp_t* sigcomp = self;

	if(!sigcomp || !sigcomp->manager){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	tsk_safeobj_lock(sigcomp);

	/* SIP/SDP (RFC 3485) */
	if(sip_n_sdp){
		tcomp_manager_addSipSdpDictionary(sigcomp->manager);
	}
	else{
		// FIXME: To be implemented in tinySigComp
		/* tcomp_manager_removeSipSdpDictionary(sigcomp->manager); */
	}

	/* SIP/SDP (RFC 5112) */
	if(pres){
		tcomp_manager_addPresenceDictionary(sigcomp->manager);
	}
	else{
		// FIXME: To be implemented in tinySigComp
		/* tcomp_manager_removePresenceDictionary(sigcomp->manager); */
	}

	tsk_safeobj_unlock(sigcomp);

	return 0;
}

/** Adds new SigComp compartement to the handler
* @param self The SigComp handler
* @param comp_id The id of the new compartment to add
* @retval Zero if succeed and non-zero error code otherwise.
*/
int tsip_sigcomp_handler_add_compartment(tsip_sigcomp_handle_t* self, const char* comp_id)
{
	tsip_sigcomp_compartment_t* compartment;
	tsip_sigcomp_t* sigcomp = self;
	int ret = -1;

	if(!sigcomp || !comp_id){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	tsk_safeobj_lock(sigcomp);

	/* check if we already have a compartment with the same id */
	if(tsk_list_find_object_by_pred(sigcomp->compartments, pred_find_compartment_by_id, comp_id)){
		TSK_DEBUG_ERROR("Failed to add new SigComp compartment. %s already exist.", comp_id);
		ret = -2;
		goto bail;
	}

	if((compartment = tsk_object_new(tsip_sigcomp_compartment_def_t))){
		compartment->id = tsk_strdup(comp_id);
		tcomp_result_setCompartmentId(compartment->decomp_result, compartment->id, tsk_strlen(compartment->id));

		tsk_list_push_back_data(sigcomp->compartments, (void**)&compartment);
		ret = 0;
		goto bail;
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new SigComp compartment");
		ret = -3;
		goto bail;
	}

bail:
	tsk_safeobj_unlock(sigcomp);

	return ret;
}

/** Removes a SigComp compartement from the handler
* @param self The SigComp handler
* @param comp_id The id of the new compartment to remove
* @retval Zero if succeed and non-zero error code otherwise.
*/
int tsip_sigcomp_handler_remove_compartment(tsip_sigcomp_handle_t* self, const char* comp_id)
{
	tsip_sigcomp_t* sigcomp = self;

	if(!sigcomp || !comp_id){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	tsk_safeobj_lock(sigcomp);
	tsk_list_remove_item_by_pred(sigcomp->compartments, pred_find_compartment_by_id, comp_id);
	tsk_safeobj_unlock(sigcomp);

	return 0;
}

// FIXME
const char* tsip_sigcomp_handler_fixme_getcompid(const tsip_sigcomp_handle_t* self)
{
	const tsip_sigcomp_t* sigcomp = self;
	const char* comp_id = tsk_null;

	if(!sigcomp){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}

	tsk_safeobj_lock(sigcomp);
	if(!TSK_LIST_IS_EMPTY(sigcomp->compartments)){
		comp_id = ((tsip_sigcomp_compartment_t*)TSK_LIST_FIRST_DATA(sigcomp->compartments))->id;
	}
	tsk_safeobj_unlock(sigcomp);
	return comp_id;
}

/** Close all SigComp compartments
* @param self The SigComp handler
*/
int tsip_sigcomp_close_all(tsip_sigcomp_handle_t* self)
{
	tsip_sigcomp_t* sigcomp = self;
	const tsk_list_item_t* item;
	const char* comp_id;

	if(!sigcomp){
		return -1;
	}

	tsk_safeobj_lock(sigcomp);

	tsk_list_foreach(item, sigcomp->compartments){
		comp_id = ((tsip_sigcomp_compartment_t*)item->data)->id;
		tcomp_manager_closeCompartment(sigcomp->manager, comp_id, tsk_strlen(comp_id));
	}

	tsk_safeobj_unlock(sigcomp);

	return 0;
}

/** Compress a Sip message
* @param self The SigComp handler
* @param comp_id The id of the compartement to use for compression. This compartment should be previously added using
* @ref tsip_sigcomp_handler_add_compartment()
* @param is_stream Indicates whether we are about to compress a stream message or not
* @param in_data Sip input data to compress
* @param in_size The size of the Sip input data
* @param out_data A pointer to the output data where the compressed message will be copied. It's up to you to allocate
* this buffer before calling this function.
* @param out_maxsize The maximum size of the @a out_data. Used as guard to avoid buffer overflow.
* @retval Size of the compressed data.
*/
tsk_size_t tsip_sigcomp_handler_compress(tsip_sigcomp_handle_t* self, const char* comp_id, tsk_bool_t is_stream, const void* in_data, tsk_size_t in_size, void* out_data, tsk_size_t out_maxsize)
{
	tsk_size_t out_size = 0;
	tsip_sigcomp_compartment_t* compartment;
	tsip_sigcomp_t* sigcomp = self;

	if(!sigcomp || !in_data || !in_size || !out_data || !out_maxsize){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}
	
	/* find the compartment */
	if(!(compartment = (tsip_sigcomp_compartment_t*)tsk_list_find_object_by_pred(sigcomp->compartments, pred_find_compartment_by_id, comp_id))){
		TSK_DEBUG_ERROR("%s not a valid compartment Id", comp_id);
		return 0;
	}
	
	/* take ownership and lock() */
	compartment = tsk_object_ref(compartment); /* take ownership instead of locking the handler(which will lock all compartments) */
	tsk_safeobj_lock(compartment);
	
	/* compress the message */
	out_size = tcomp_manager_compress(sigcomp->manager, compartment->id, tsk_strlen(compartment->id), in_data, in_size, out_data, out_maxsize, is_stream);
	
	/* release ownership and unlock() */
	tsk_safeobj_unlock(compartment);
	tsk_object_unref(compartment);

	return out_size;
}

/** UnCompress a SigComp message
* @param self The SigComp handler
* @param comp_id The compartment id to use after successful decompression.
* @param is_stream Indicates whether we are about to uncompress a stream message or not
* @param in_data A pointer to the SigComp buffer to decompress
* @param in_size The size of the SigComp buffer
* @param out_data A pointer to the output data where the uncompressed message will be copied. It's up to you to allocate
* this buffer before calling this function.
* @param out_maxsize The maximum size of the @a out_data. Used as guard to avoid buffer overflow.
* @param is_nack Used to signal whether the uncompressed (result) message is a NACK or a Sip Message.
* @retval Size of the uncompressed data.
*/
tsk_size_t tsip_sigcomp_handler_uncompress(tsip_sigcomp_handle_t* self, const char* comp_id, tsk_bool_t is_stream, const void* in_data, tsk_size_t in_size, void* out_data, tsk_size_t out_maxsize, tsk_bool_t* is_nack)
{
	tsk_size_t out_size = 0;
	tsip_sigcomp_compartment_t* compartment;
	tsip_sigcomp_t* sigcomp = self;

	if(!sigcomp || !in_data || !in_size || !out_data || !out_maxsize || !is_nack){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}
	
	/* find the compartment */
	if(!(compartment = (tsip_sigcomp_compartment_t*)tsk_list_find_object_by_pred(sigcomp->compartments, pred_find_compartment_by_id, comp_id))){
		TSK_DEBUG_ERROR("%s not a valid compartment Id", comp_id);
		return 0;
	}

	*is_nack = tsk_false;

	/* take ownership and lock() */
	compartment = tsk_object_ref(compartment); /* take ownership instead of locking the handler(which will lock all compartments) */
	tsk_safeobj_lock(compartment);
	
	/* uncompress the message */
	tcomp_result_setOutputBuffer(compartment->decomp_result, out_data, out_maxsize, is_stream, compartment->stream_id); // set the output buffer where to copy uncompressed message
	out_size = tcomp_manager_decompress(sigcomp->manager, in_data, in_size, compartment->decomp_result);
	if(out_size){
		// provide the compartment id --> save temp states
		tcomp_manager_provideCompartmentId(sigcomp->manager, compartment->decomp_result);
	}
	else{
		if((*is_nack = compartment->decomp_result->isNack)){
			tsk_size_t nack_info_size;
			if(compartment->decomp_result->nack_info && (nack_info_size = tcomp_buffer_getSize(compartment->decomp_result->nack_info))){
				out_size = (nack_info_size > out_maxsize) ? out_maxsize : nack_info_size;
				memcpy(out_data, tcomp_buffer_getBuffer(compartment->decomp_result->nack_info), out_size);
				TSK_DEBUG_INFO("We got a NACK to send()");
			}
			else{
				TSK_DEBUG_INFO("We got a NACK from the remote party");
			}
		}
		else{
			/* Should never happen */
			TSK_DEBUG_ERROR("SigComp decompression failed");
		}
	}

	/* release ownership and unlock() */
	tsk_safeobj_unlock(compartment);
	tsk_object_unref(compartment);

	return out_size;
}

/** Try to unCompress the next stream chunck. Must only be used with stream compartments.
* When you are dealing with stream stream transports, then you should call this function several times until
* it returns zero.
* @param self The SigComp handler
* @param comp_id The compartment id to use after successful decompression
* @param nack_data Pointer to the NACL message. We be filled only if @a is_nack is equal to @a tsk_true
* @param is_nack Used to signal whether the uncompressed (result) message is a NACK or a Sip Message.
* @retval Size of the uncompressed data plus size of all previously uncompressed chuncks except NACKs.
* If the uncompressed message is a NACK, then the returned size will be equal to the length of the NACK
* to send to the remote party.
*/
tsk_size_t tsip_sigcomp_handler_uncompress_next(tsip_sigcomp_handle_t* self, const char* comp_id, void** nack_data, tsk_bool_t* is_nack)
{
	tsk_size_t out_size = 0;
	tsip_sigcomp_compartment_t* compartment;
	tsip_sigcomp_t* sigcomp = self;

	if(!sigcomp || !is_nack || !nack_data){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}
	
	/* find the compartment */
	if(!(compartment = (tsip_sigcomp_compartment_t*)tsk_list_find_object_by_pred(sigcomp->compartments, pred_find_compartment_by_id, comp_id))){
		TSK_DEBUG_ERROR("%s not a valid compartment Id", comp_id);
		return 0;
	}

	/* take ownership and lock() */
	compartment = tsk_object_ref(compartment); /* take ownership instead of locking the handler(which will lock all compartments) */
	tsk_safeobj_lock(compartment);

	/* uncompress the next chunk
	* the returned size the the total size which includes previous chuncks uncompressed
	* using tsip_sigcomp_handler_uncompress()
	*/
	out_size = tcomp_manager_getNextStreamMessage(sigcomp->manager, compartment->decomp_result);

	if(out_size){
		// provide the compartment id --> save temp states
		tcomp_manager_provideCompartmentId(sigcomp->manager, compartment->decomp_result);
	}
	else{
		if((*is_nack = compartment->decomp_result->isNack)){
			tsk_size_t nack_info_size;
			if(compartment->decomp_result->nack_info && (nack_info_size = tcomp_buffer_getSize(compartment->decomp_result->nack_info))){
				if((*nack_data = tsk_calloc(nack_info_size, sizeof(uint8_t)))){
					memcpy(*nack_data, tcomp_buffer_getBuffer(compartment->decomp_result->nack_info), nack_info_size);
				}
				TSK_DEBUG_INFO("We got a NACK to send()");
			}
			else{
				TSK_DEBUG_INFO("We got a NACK from the remote party");
			}
		}
	}

	/* release ownership and unlock() */
	tsk_safeobj_unlock(compartment);
	tsk_object_unref(compartment);

	return out_size;
}

//===========================================================================
//	SIP SigComp Handler object definition
//
static tsk_object_t* tsip_sigcomp_ctor(tsk_object_t * self, va_list * app)
{
	tsip_sigcomp_t *sigcomp = self;
	if(sigcomp){
		/* Done by tsip_sigcomp_create()
		sigcomp->manager = tcomp_manager_create();
		sigcomp->compartments = tsk_list_create();
		*/

		tsk_safeobj_init(sigcomp);
	}
	return self;
}

static tsk_object_t* tsip_sigcomp_dtor(tsk_object_t * self)
{ 
	tsip_sigcomp_t *sigcomp = self;
	if(sigcomp){
		TSK_OBJECT_SAFE_FREE(sigcomp->manager);
		TSK_OBJECT_SAFE_FREE(sigcomp->compartments);

		tsk_safeobj_deinit(sigcomp);
	}

	return self;
}

static const tsk_object_def_t tsip_sigcomp_def_s = 
{
	sizeof(tsip_sigcomp_t),
	tsip_sigcomp_ctor, 
	tsip_sigcomp_dtor,
	tsk_null, 
};
const tsk_object_def_t *tsip_sigcomp_def_t = &tsip_sigcomp_def_s;


//===========================================================================
//	SIP SigComp Compartment object definition
//
static tsk_object_t* tsip_sigcomp_compartment_ctor(tsk_object_t * self, va_list * app)
{
	static uint64_t __unique_stream_id = 0;

	tsip_sigcomp_compartment_t *compartment = self;
	if(compartment){
		compartment->decomp_result = tcomp_result_create();
		compartment->stream_id = ++(__unique_stream_id);
		tsk_safeobj_init(compartment);
	}
	return self;
}

static tsk_object_t* tsip_sigcomp_compartment_dtor(tsk_object_t * self)
{ 
	tsip_sigcomp_compartment_t *compartment = self;
	if(compartment){
		TSK_FREE(compartment->id);
		TSK_OBJECT_SAFE_FREE(compartment->decomp_result);

		tsk_safeobj_deinit(compartment);
	}

	return self;
}
int	tsip_sigcomp_compartment_cmp(const tsk_object_t * _c1, const tsk_object_t * _c2)
{
	const tsip_sigcomp_compartment_t *c1 = _c1;
	const tsip_sigcomp_compartment_t *c2 = _c2;

	if(c1 && c2){
		return tsk_strcmp(c1->id, c2->id);
	}
	else{
		return (c1 - c2);
	}
}

static const tsk_object_def_t tsip_sigcomp_compartment_def_s = 
{
	sizeof(tsip_sigcomp_compartment_t),
	tsip_sigcomp_compartment_ctor, 
	tsip_sigcomp_compartment_dtor,
	tsip_sigcomp_compartment_cmp, 
};
const tsk_object_def_t *tsip_sigcomp_compartment_def_t = &tsip_sigcomp_compartment_def_s;




