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

/**@file tsms_tpdu_ctx.c
 * @brief SMS TPDU encoder/decoder as per 3GPP TS 23.040.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinySMS/tpdu/tsms_tpdu.h"

#include "tinySMS/tpdu/tsms_tpdu_submit.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

/**@defgroup tsms_pdu_group SMS TPDU encoder/decoder
*/

/**@ingroup tsms_pdu_group
*/
tsms_tpdu_ctx_handle_t* tsms_pdu_ctx_create(uint8_t mr, tsms_address_t smsc, tsms_address_t phone)
{
	tsms_tpdu_ctx_t* ret = tsk_null;

	if(!smsc){
		TSK_DEBUG_ERROR("SMSC is Null.");
		goto bail;
	}

	if(!(ret = tsk_object_new(tsms_tpdu_ctx_def_t, mr, smsc, phone))){
		goto bail;
	}
	
bail:
	return ret;
}

/**@ingroup tsms_pdu_group
*/
int tsms_pdu_ctx_set_alphabet(tsms_tpdu_ctx_handle_t* handle, tsms_alphabet_t alphabet)
{
	tsms_tpdu_ctx_t* ctx = handle;
	if(ctx){
		ctx->alphabet = alphabet;
		return 0;
	}
	return -1;
}

/**@ingroup tsms_pdu_group
*/
int tsms_pdu_ctx_set_usrdata(tsms_tpdu_ctx_handle_t* handle, const void* data, size_t size)
{
	tsms_tpdu_ctx_t* ctx = handle;
	if(ctx && data && size){
		TSK_OBJECT_SAFE_FREE(ctx->usrdata);
		ctx->usrdata = TSK_BUFFER_CREATE(data, size);
		return 0;
	}
	return -1;
}

/**@ingroup tsms_pdu_group
*/
tsk_buffer_t* tsms_pdu_ctx_getSUBMIT(tsms_tpdu_ctx_handle_t* handle)
{
	tsk_buffer_t* output = tsk_null;
	tsms_tpdu_submit_t* submit = tsk_null;
	
	tsms_tpdu_ctx_t* ctx = handle;

	if(!ctx){
		TSK_DEBUG_ERROR("TPDU handle is Null.");
		goto bail;
	}

	if((submit = TSMS_TPDU_SUBMIT_CREATE())){
		output = TSK_BUFFER_CREATE_NULL();
		TSMS_TPDU_MESSAGE(submit)->serialize(TSMS_TPDU_MESSAGE(submit), output);
	}
	else{
		goto bail;
	}


	

bail:
	TSK_OBJECT_SAFE_FREE(submit);
	return output;
}

//=================================================================================================
//	SMS TPDU object definition
//
static tsk_object_t* _tsms_tpdu_ctx_create(tsk_object_t * self, va_list * app)
{
	tsms_tpdu_ctx_t *ctx = self;
	if(ctx){
#if defined(__GNUC__)
		ctx->mr = va_arg(*app, unsigned);
#else
		ctx->mr = va_arg(*app, uint8_t);
#endif
		ctx->smsc = tsk_strdup( va_arg(*app, const char*) );
		ctx->phone = tsk_strdup( va_arg(*app, const char*) );
		ctx->alphabet = tsms_alpha_7bit;
	}
	return self;
}

static tsk_object_t* tsms_tpdu_ctx_destroy(tsk_object_t * self)
{ 
	tsms_tpdu_ctx_t *ctx = self;
	if(ctx){
		TSK_FREE(ctx->smsc);
		TSK_FREE(ctx->phone);

		TSK_OBJECT_SAFE_FREE(ctx->usrdata);
	}

	return self;
}

static const tsk_object_def_t tsms_tpdu_ctx_def_s = 
{
	sizeof(tsms_tpdu_ctx_t),
	_tsms_tpdu_ctx_create, 
	tsms_tpdu_ctx_destroy,
	tsk_null, 
};
const tsk_object_def_t *tsms_tpdu_ctx_def_t = &tsms_tpdu_ctx_def_s;
