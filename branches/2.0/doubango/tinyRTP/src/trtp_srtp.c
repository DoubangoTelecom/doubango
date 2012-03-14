/*
* Copyright (C) 2012 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango(dot)org>
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
/**@file trtp_srtp.h
 *
 * @author Mamadou Diop <diopmamadou(at)doubango(dot)org>
 */
#include "tinyrtp/trtp_srtp.h"
#include "tinyrtp/trtp_manager.h"

#if HAVE_SRTP

int trtp_srtp_ctx_init(trtp_srtp_ctx_xt* ctx, int32_t tag, trtp_srtp_crypto_type_t type, uint32_t ssrc){

	char* key_str = ctx->key_str;
	err_status_t srtp_err;
	tsk_size_t size;
	
	if(!ctx){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(ctx->initialized){
		trtp_srtp_ctx_deinit(ctx);
	}

	ctx->tag = tag;
	ctx->crypto_type = type;
	if((srtp_err = crypto_get_random((unsigned char*)ctx->key_bin, sizeof(ctx->key_bin))) != err_status_ok){
		TSK_DEBUG_ERROR("crypto_get_random() failed");
		return -2;
	}
	size = tsk_base64_encode((const uint8_t*)ctx->key_bin, sizeof(ctx->key_bin), &key_str);
	key_str[size] = '\0';

	switch(ctx->crypto_type){
		case HMAC_SHA1_80:
			{
				crypto_policy_set_aes_cm_128_hmac_sha1_80(&ctx->policy.rtp);
				crypto_policy_set_aes_cm_128_hmac_sha1_80(&ctx->policy.rtcp);
				break;
			}
		case HMAC_SHA1_32:
		default:
			{
				crypto_policy_set_aes_cm_128_hmac_sha1_32(&ctx->policy.rtp);
				crypto_policy_set_aes_cm_128_hmac_sha1_32(&ctx->policy.rtcp);
				break;
			}
	}
	
	ctx->policy.key = (unsigned char*)ctx->key_bin;
	ctx->policy.ssrc.type = ssrc_any_inbound;
	ctx->policy.ssrc.value = ssrc;
	if((srtp_err = srtp_create(&ctx->session, &ctx->policy)) != err_status_ok){
		TSK_DEBUG_ERROR("srtp_create() failed");
		return -3;
	}
	ctx->initialized = tsk_true;
	return 0;
}

int trtp_srtp_ctx_deinit(trtp_srtp_ctx_xt* ctx){	
	if(!ctx){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	if(ctx->initialized){
		err_status_t srtp_err = srtp_dealloc(ctx->session);
		ctx->initialized = tsk_false;
	}
	return 0;
}

int trtp_srtp_match_line(const char* crypto_line, int32_t* tag, int32_t* crypto_type, char* key, tsk_size_t key_size)
{
	char* v = strtok((char*)crypto_line, " :|;");
	int32_t k = 0;
	while(v){
		switch(k){
			case 0:
				{
					if(tag){
						*tag = atoi(v);
					}
					break;
				}
			case 1:
				{
					if(tsk_striequals(v, TRTP_SRTP_AES_CM_128_HMAC_SHA1_80)){
						if(crypto_type){
							*crypto_type = HMAC_SHA1_80;
						}
					}
					else if(tsk_striequals(v, TRTP_SRTP_AES_CM_128_HMAC_SHA1_32)){
						if(crypto_type){
							*crypto_type = HMAC_SHA1_32;
						}
					}
					else {
						return -0xFF; 
					}
					break;
				}
			case 2:
				{
					if(!tsk_striequals(v, "inline")){
						return -0xFF;
					}
					break;
				}
			case 3:
				{
					if(key && key_size){
						memset(key, 0, key_size);
						memcpy(key, v, TSK_MIN(key_size, tsk_strlen(v)));
					}
					return 0;
				}
		}
		++k;
		v = strtok(tsk_null, " :|;");
	}

	return -0xF0;
}


int trtp_srtp_get_ctx_local(trtp_manager_t* rtp_mgr, const trtp_srtp_ctx_xt** ctx, tsk_size_t *count)
{
	trtp_srtp_ctx_xt* ctx_local;
	const trtp_srtp_ctx_xt* ctx_remote;

	if(!rtp_mgr || !ctx || !count){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	
	*count = 0;
	*ctx = tsk_null;

	ctx_remote = &rtp_mgr->srtp_contexts[TRTP_SRTP_LINE_IDX_REMOTE][0];
	ctx_local = &rtp_mgr->srtp_contexts[TRTP_SRTP_LINE_IDX_LOCAL][0];
	
	if(ctx_remote->initialized){
		int index = -1;
		if(ctx_local[0].crypto_type == ctx_remote->crypto_type){
			index = 0;
		}
		else if(ctx_local[1].crypto_type == ctx_remote->crypto_type){
			index = 1;
		}
		
		if(index != -1){
			*count = 1;
			*ctx = &ctx_local[index];
			ctx_local[index].tag = ctx_remote->tag;
		}
	}
	else{
		*count = 2;
		*ctx = &ctx_local[0];
	}	

	return 0;
}

int trtp_srtp_set_remote(trtp_manager_t* rtp_mgr, const char* crypto_line)
{
	//e.g. 2 F8_128_HMAC_SHA1_80 inline:MTIzNDU2Nzg5QUJDREUwMTIzNDU2Nzg5QUJjZGVm|2^20|1:4;inline:QUJjZGVmMTIzNDU2Nzg5QUJDREUwMTIzNDU2Nzg5|2^20|2:4"
	tsk_bool_t matched = tsk_false;
	trtp_srtp_ctx_xt* srtp_ctx = &rtp_mgr->srtp_contexts[TRTP_SRTP_LINE_IDX_REMOTE][0];
	int ret;
	uint8_t *key_bin;
	err_status_t srtp_err;
	
	ret = trtp_srtp_ctx_deinit(srtp_ctx);
	ret = trtp_srtp_match_line(crypto_line, &srtp_ctx->tag, (int32_t*)&srtp_ctx->crypto_type, srtp_ctx->key_str, (sizeof(srtp_ctx->key_str) - 1));
	if(ret != 0){
		return ret;
	}

	switch(srtp_ctx->crypto_type){
		case HMAC_SHA1_80:
			{
				crypto_policy_set_aes_cm_128_hmac_sha1_80(&srtp_ctx->policy.rtp);
				crypto_policy_set_aes_cm_128_hmac_sha1_80(&srtp_ctx->policy.rtcp);
				break;
			}
		case HMAC_SHA1_32:
		default:
			{
				crypto_policy_set_aes_cm_128_hmac_sha1_32(&srtp_ctx->policy.rtp);
				crypto_policy_set_aes_cm_128_hmac_sha1_32(&srtp_ctx->policy.rtcp);
				break;
			}
	}

	key_bin = (unsigned char*)srtp_ctx->key_bin;
	tsk_base64_decode((const uint8_t*)srtp_ctx->key_str, tsk_strlen(srtp_ctx->key_str), (char**)&key_bin);
	srtp_ctx->policy.key = key_bin;
	srtp_ctx->policy.ssrc.type = ssrc_any_inbound;
	if((srtp_err = srtp_create(&srtp_ctx->session, &srtp_ctx->policy)) != err_status_ok){
		TSK_DEBUG_ERROR("srtp_create() failed");
		return -3;
	}
	srtp_ctx->initialized = tsk_true;
	return 0;
}

tsk_bool_t trtp_srtp_is_initialized(trtp_manager_t* rtp_mgr)
{
	if(!rtp_mgr){
		TSK_DEBUG_ERROR("Invalid argument");
		return tsk_false;
	}
	return ((rtp_mgr->srtp_contexts[TRTP_SRTP_LINE_IDX_LOCAL][0].initialized || rtp_mgr->srtp_contexts[TRTP_SRTP_LINE_IDX_LOCAL][1].initialized) 
		&& rtp_mgr->srtp_contexts[TRTP_SRTP_LINE_IDX_REMOTE][0].initialized);
}

tsk_bool_t trtp_srtp_is_active(trtp_manager_t* rtp_mgr)
{
	if(!rtp_mgr){
		TSK_DEBUG_ERROR("Invalid argument");
		return tsk_false;
	}
	return (rtp_mgr ->srtp_ctx_neg_remote && rtp_mgr ->srtp_ctx_neg_local);
}

#endif /* HAVE_SRTP */