///*
//* Copyright (C) 2009 Mamadou Diop.
//*
//* Contact: Mamadou Diop <diopmamadou(at)doubango.org>
//*	
//* This file is part of Open Source Doubango Framework.
//*
//* DOUBANGO is free software: you can redistribute it and/or modify
//* it under the terms of the GNU General Public License as published by
//* the Free Software Foundation, either version 3 of the License, or
//* (at your option) any later version.
//*	
//* DOUBANGO is distributed in the hope that it will be useful,
//* but WITHOUT ANY WARRANTY; without even the implied warranty of
//* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//* GNU General Public License for more details.
//*	
//* You should have received a copy of the GNU General Public License
//* along with DOUBANGO.
//*
//*/
//
///**@file tsms_tpdu_ctx.c
// * @brief SMS TPDU encoder/decoder as per 3GPP TS 23.040.
// *
// * @author Mamadou Diop <diopmamadou(at)doubango.org>
// *
// * @date Created: Sat Nov 8 16:54:58 2009 mdiop
// */
//#include "tinySMS/tpdu/tsms_tpdu.h"
//
//#include "tinySMS/tpdu/tsms_tpdu_submit.h"
//#include "tinySMS/tpdu/tsms_tpdu_deliver.h"
//
//#include "tsk_string.h"
//#include "tsk_memory.h"
//#include "tsk_debug.h"
//
///**@defgroup tsms_tpdu_group SMS TPDU encoder/decoder
//*/
//
///**@ingroup tsms_tpdu_group
//* If smsc is null, then the one stored in the phone will be used.
//* phone: OA for SMS-DELIVER and DA for SMS-SUBMIT
//* mr: not used for SMS-DELIVER
//*/
//tsms_tpdu_ctx_handle_t* tsms_tpdu_ctx_create(uint8_t mr, tsms_address_string_t smsc, tsms_address_string_t phone)
//{
//	tsms_tpdu_ctx_t* ret = tsk_null;
//
//	if(!(ret = tsk_object_new(tsms_tpdu_ctx_def_t, mr, smsc, phone))){
//		goto bail;
//	}
//	
//bail:
//	return ret;
//}
//
///**@ingroup tsms_tpdu_group
//*/
//int tsms_tpdu_ctx_set_usrdata(tsms_tpdu_ctx_handle_t* handle, tsms_alphabet_t alphabet, const void* data, size_t size)
//{
//	tsms_tpdu_ctx_t* ctx = handle;
//	if(ctx && data && size){
//		ctx->alphabet = alphabet;
//		TSK_OBJECT_SAFE_FREE(ctx->usrdata);
//		ctx->usrdata = TSK_BUFFER_CREATE(data, size);
//		return 0;
//	}
//	return -1;
//}
//
///**@ingroup tsms_tpdu_group
//*/
//tsk_buffer_t* tsms_tpdu_ctx_getSUBMIT(tsms_tpdu_ctx_handle_t* handle)
//{
//	tsk_buffer_t* output = tsk_null;
//	tsms_tpdu_submit_t* submit = tsk_null;
//	
//	tsms_tpdu_ctx_t* ctx = handle;
//
//	if(!ctx){
//		TSK_DEBUG_ERROR("TPDU handle is Null.");
//		goto bail;
//	}
//	if(!ctx->usrdata){
//		TSK_DEBUG_ERROR("SMS-SUBMIT need user-data.");
//		goto bail;
//	}
//
//	if((submit = TSMS_TPDU_SUBMIT_CREATE())){
//		output = TSK_BUFFER_CREATE_NULL();
//		/*=== initialize SMS-SUBMIT ===*/
//		/* message reference */
//		submit->mr = ctx->mr;
//		/* distination address */
//		submit->da = tsk_object_ref(ctx->phone);
//		/* user-data */
//		tsms_tpdu_message_set_usrdata(TSMS_TPDU_MESSAGE(submit), ctx->usrdata->data, ctx->alphabet);
//		
//		TSMS_TPDU_MESSAGE(submit)->serialize(ctx->smsc, TSMS_TPDU_MESSAGE(submit), output);
//	}
//	else{
//		goto bail;
//	}
//
//bail:
//	TSK_OBJECT_SAFE_FREE(submit);
//	return output;
//}
//
///**@ingroup tsms_tpdu_group
//*/
//tsk_buffer_t* tsms_tpdu_ctx_getDELIVER(tsms_tpdu_ctx_handle_t* handle)
//{
//	tsk_buffer_t* output = tsk_null;
//	tsms_tpdu_deliver_t* deliver = tsk_null;
//	
//	tsms_tpdu_ctx_t* ctx = handle;
//
//	if(!ctx){
//		TSK_DEBUG_ERROR("TPDU handle is Null.");
//		goto bail;
//	}
//	if(!ctx->usrdata){
//		TSK_DEBUG_ERROR("SMS-DELIVER need user-data.");
//		goto bail;
//	}
//
//	if((deliver = TSMS_TPDU_DELIVER_CREATE())){
//		output = TSK_BUFFER_CREATE_NULL();
//		/*=== initialize SMS-SUBMIT ===*/
//		/* distination address */
//		deliver->oa = tsk_object_ref(ctx->phone);
//		/* user-data */
//		tsms_tpdu_message_set_usrdata(TSMS_TPDU_MESSAGE(deliver), ctx->usrdata->data, ctx->alphabet);
//		
//		TSMS_TPDU_MESSAGE(deliver)->serialize(ctx->smsc, TSMS_TPDU_MESSAGE(deliver), output);
//	}
//	else{
//		goto bail;
//	}
//
//bail:
//	TSK_OBJECT_SAFE_FREE(deliver);
//	return output;
//}
//
////=================================================================================================
////	SMS TPDU object definition
////
//static tsk_object_t* _tsms_tpdu_ctx_create(tsk_object_t * self, va_list * app)
//{
//	tsms_tpdu_ctx_t *ctx = self;
//	if(ctx){
//		const char* smsc, *phone;
//		uint8_t mr;
//
//#if defined(__GNUC__)
//		mr = (uint8_t)va_arg(*app, unsigned);
//#else
//		mr = va_arg(*app, uint8_t);
//#endif
//		smsc = va_arg(*app, const char*);
//		phone = va_arg(*app, const char*);
//
//		ctx->mr = mr;
//		ctx->smsc = TSMS_ADDRESS_SMSC_CREATE(smsc);
//		ctx->phone = TSMS_ADDRESS_DA_CREATE(phone);
//		ctx->alphabet = tsms_alpha_7bit;
//	}
//	return self;
//}
//
//static tsk_object_t* tsms_tpdu_ctx_destroy(tsk_object_t * self)
//{ 
//	tsms_tpdu_ctx_t *ctx = self;
//	if(ctx){
//		TSK_OBJECT_SAFE_FREE(ctx->smsc);
//		TSK_OBJECT_SAFE_FREE(ctx->phone);
//		
//		TSK_OBJECT_SAFE_FREE(ctx->usrdata);
//	}
//
//	return self;
//}
//
//static const tsk_object_def_t tsms_tpdu_ctx_def_s = 
//{
//	sizeof(tsms_tpdu_ctx_t),
//	_tsms_tpdu_ctx_create, 
//	tsms_tpdu_ctx_destroy,
//	tsk_null, 
//};
//const tsk_object_def_t *tsms_tpdu_ctx_def_t = &tsms_tpdu_ctx_def_s;
