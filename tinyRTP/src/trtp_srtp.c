/*
 * Copyright (C) 2012 Mamadou Diop
 * Copyright (C) 2012-2013 Doubango Telecom <http://www.doubango.org>
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
/**@file trtp_srtp.c
 */
#include "tinyrtp/trtp_srtp.h"
#include "tinyrtp/trtp_manager.h"

#if HAVE_SRTP

extern err_status_t
crypto_get_random(unsigned char *buffer, unsigned int length);

int trtp_srtp_ctx_internal_init(struct trtp_srtp_ctx_internal_xs* ctx, int32_t tag, trtp_srtp_crypto_type_t type, uint32_t ssrc)
{
    char* key_str = ctx->key_str;
    err_status_t srtp_err;
    tsk_size_t size;

    if (!ctx) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if (ctx->initialized) {
        trtp_srtp_ctx_internal_deinit(ctx);
    }

    ctx->tag = tag;
    ctx->crypto_type = type;
    if (!ctx->have_valid_key) { // use same key to avoid unseless SRTP re-negs (also fix interop-issues against buggy clients -reINVITEs-)
        if ((srtp_err = crypto_get_random((unsigned char*)ctx->key_bin, sizeof(ctx->key_bin))) != err_status_ok) {
            TSK_DEBUG_ERROR("crypto_get_random() failed");
            return -2;
        }
        size = tsk_base64_encode((const uint8_t*)ctx->key_bin, sizeof(ctx->key_bin), &key_str);
        key_str[size] = '\0';
        ctx->have_valid_key = tsk_true;
    }

    switch(ctx->crypto_type) {
    case HMAC_SHA1_80: {
        crypto_policy_set_aes_cm_128_hmac_sha1_80(&ctx->policy.rtp);
        crypto_policy_set_aes_cm_128_hmac_sha1_80(&ctx->policy.rtcp);
        break;
    }
    case HMAC_SHA1_32:
    default: {
        crypto_policy_set_aes_cm_128_hmac_sha1_32(&ctx->policy.rtp);
        crypto_policy_set_aes_cm_128_hmac_sha1_80(&ctx->policy.rtcp); // RTCP always 80
        break;
    }
    }

    ctx->policy.key = (unsigned char*)ctx->key_bin;
    ctx->policy.ssrc.type = ssrc_any_outbound;
    ctx->policy.ssrc.value = ssrc;
    ctx->policy.window_size = 2048;
    ctx->policy.allow_repeat_tx = 1;
    if ((srtp_err = srtp_create(&ctx->session, &ctx->policy)) != err_status_ok) {
        TSK_DEBUG_ERROR("srtp_create() failed");
        return -3;
    }
    ctx->initialized = tsk_true;
    return 0;
}

int trtp_srtp_ctx_internal_deinit(struct trtp_srtp_ctx_internal_xs* ctx)
{
    if(!ctx) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if(ctx->initialized) {
        /*err_status_t srtp_err =*/ srtp_dealloc(ctx->session);
        memset(&ctx->policy, 0, sizeof(ctx->policy));
        ctx->initialized = tsk_false;
    }
    return 0;
}

int trtp_srtp_ctx_init(trtp_srtp_ctx_xt* ctx, int32_t tag, trtp_srtp_crypto_type_t type, uint32_t ssrc)
{
    int ret;
    if(!ctx) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if((ret = trtp_srtp_ctx_internal_init(&ctx->rtp, tag, type, ssrc))) {
        return ret;
    }
    return trtp_srtp_ctx_internal_init(&ctx->rtcp, tag, type, ssrc);
}

int trtp_srtp_ctx_deinit(trtp_srtp_ctx_xt* ctx)
{
    int ret;
    if(!ctx) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if((ret = trtp_srtp_ctx_internal_deinit(&ctx->rtp))) {
        return ret;
    }
    return trtp_srtp_ctx_internal_deinit(&ctx->rtcp);
}

int trtp_srtp_match_line(const char* crypto_line, int32_t* tag, int32_t* crypto_type, char* key, tsk_size_t key_size)
{
    char* copyptr = tsk_strdup(crypto_line); // "strtok_r" will insert "\0" and modify the string
    char* saveptr = tsk_null;
    char* v = tsk_strtok_r(copyptr, " :|;", &saveptr);
    int32_t k = 0;
    int ret = -0xF0;
    while(v) {
        switch(k) {
        case 0: {
            if(tag) {
                *tag = atoi(v);
            }
            break;
        }
        case 1: {
            if(tsk_striequals(v, TRTP_SRTP_AES_CM_128_HMAC_SHA1_80)) {
                if(crypto_type) {
                    *crypto_type = HMAC_SHA1_80;
                }
            }
            else if(tsk_striequals(v, TRTP_SRTP_AES_CM_128_HMAC_SHA1_32)) {
                if(crypto_type) {
                    *crypto_type = HMAC_SHA1_32;
                }
            }
            else {
                ret = -0xFF;
                goto bail;
            }
            break;
        }
        case 2: {
            if(!tsk_striequals(v, "inline")) {
                ret = -0xFF;
                goto bail;
            }
            break;
        }
        case 3: {
            if(key && key_size) {
                memset(key, 0, key_size);
                memcpy(key, v, TSK_MIN(key_size, tsk_strlen(v)));
            }
            ret = 0;
            goto bail;
        }
        }
        ++k;
        v = tsk_strtok_r(tsk_null, " :|;", &saveptr);
    }
bail:
    TSK_FREE(copyptr);
    return ret;
}

tsk_size_t trtp_srtp_get_local_contexts(trtp_manager_t* rtp_mgr, const struct trtp_srtp_ctx_xs ** contexts, tsk_size_t contexts_count)
{
    tsk_size_t ret = 0;
    if(!rtp_mgr || !contexts) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return 0;
    }

    if (contexts_count > ret && rtp_mgr->srtp_contexts[TRTP_SRTP_LINE_IDX_LOCAL][HMAC_SHA1_80].rtp.initialized) {
        contexts[ret++] = &rtp_mgr->srtp_contexts[TRTP_SRTP_LINE_IDX_LOCAL][HMAC_SHA1_80];
    }
    if (contexts_count > ret && rtp_mgr->srtp_contexts[TRTP_SRTP_LINE_IDX_LOCAL][HMAC_SHA1_32].rtp.initialized) {
        contexts[ret++] = &rtp_mgr->srtp_contexts[TRTP_SRTP_LINE_IDX_LOCAL][HMAC_SHA1_32];
    }
    return ret;
}

int trtp_srtp_set_crypto(struct trtp_manager_s* rtp_mgr, const char* crypto_line, int32_t idx)
{
    //e.g. 2 F8_128_HMAC_SHA1_80 inline:MTIzNDU2Nzg5QUJDREUwMTIzNDU2Nzg5QUJjZGVm|2^20|1:4;inline:QUJjZGVmMTIzNDU2Nzg5QUJDREUwMTIzNDU2Nzg5|2^20|2:4"
    trtp_srtp_ctx_xt* srtp_ctx;
    int ret;
    uint8_t *key_bin;
    err_status_t srtp_err;
    int32_t tag, crypto_type;
    char key_str[SRTP_MAX_KEY_LEN + 1];

    memset(key_str, 0, sizeof(key_str));

    if ((ret = trtp_srtp_match_line(crypto_line, &tag, &crypto_type, key_str, sizeof(key_str) - 1))) {
        return ret;
    }

    srtp_ctx = &rtp_mgr->srtp_contexts[idx][crypto_type];
    ret = trtp_srtp_ctx_deinit(srtp_ctx);

    srtp_ctx->rtp.tag = tag;
    srtp_ctx->rtp.crypto_type = (trtp_srtp_crypto_type_t)crypto_type;
    memcpy(srtp_ctx->rtp.key_str, key_str, sizeof(srtp_ctx->rtp.key_str));

    switch(srtp_ctx->rtp.crypto_type) {
    case HMAC_SHA1_80: {
        crypto_policy_set_aes_cm_128_hmac_sha1_80(&srtp_ctx->rtp.policy.rtp);
        crypto_policy_set_aes_cm_128_hmac_sha1_80(&srtp_ctx->rtp.policy.rtcp);
        if (idx == TRTP_SRTP_LINE_IDX_REMOTE) {
            trtp_srtp_ctx_deinit(&rtp_mgr->srtp_contexts[TRTP_SRTP_LINE_IDX_LOCAL][HMAC_SHA1_32]);
            rtp_mgr->srtp_contexts[TRTP_SRTP_LINE_IDX_LOCAL][HMAC_SHA1_80].rtp.tag =
                rtp_mgr->srtp_contexts[TRTP_SRTP_LINE_IDX_LOCAL][HMAC_SHA1_80].rtcp.tag = srtp_ctx->rtp.tag;
        }
        break;
    }
    case HMAC_SHA1_32: {
        crypto_policy_set_aes_cm_128_hmac_sha1_32(&srtp_ctx->rtp.policy.rtp);
        crypto_policy_set_aes_cm_128_hmac_sha1_80(&srtp_ctx->rtp.policy.rtcp); // RTCP always 80
        if (idx == TRTP_SRTP_LINE_IDX_REMOTE) {
            trtp_srtp_ctx_deinit(&rtp_mgr->srtp_contexts[TRTP_SRTP_LINE_IDX_LOCAL][HMAC_SHA1_80]);
            rtp_mgr->srtp_contexts[TRTP_SRTP_LINE_IDX_LOCAL][HMAC_SHA1_32].rtp.tag =
                rtp_mgr->srtp_contexts[TRTP_SRTP_LINE_IDX_LOCAL][HMAC_SHA1_32].rtcp.tag = srtp_ctx->rtp.tag;
        }
        break;
    }
    default:
        break;
    }

    key_bin = (unsigned char*)srtp_ctx->rtp.key_bin;
    tsk_base64_decode((const uint8_t*)srtp_ctx->rtp.key_str, (tsk_size_t)tsk_strlen(srtp_ctx->rtp.key_str), (char**)&key_bin);
    srtp_ctx->rtp.policy.key = key_bin;
    srtp_ctx->rtp.policy.ssrc.type = idx == TRTP_SRTP_LINE_IDX_REMOTE ? ssrc_any_inbound : ssrc_any_outbound;
    srtp_ctx->rtp.policy.window_size = 2048;
    srtp_ctx->rtp.policy.allow_repeat_tx = 1;
    if ((srtp_err = srtp_create(&srtp_ctx->rtp.session, &srtp_ctx->rtp.policy)) != err_status_ok) {
        TSK_DEBUG_ERROR("srtp_create() failed: %d", srtp_err);
        return -3;
    }
    srtp_ctx->rtp.initialized = tsk_true;
    return 0;
}

int trtp_srtp_set_key_and_salt(trtp_manager_t* rtp_mgr, trtp_srtp_crypto_type_t crypto_type, const void* key, tsk_size_t key_size, const void* salt, tsk_size_t salt_size, int32_t idx, tsk_bool_t is_rtp)
{
    int ret;
    trtp_srtp_ctx_internal_xt* srtp_ctx;
    err_status_t srtp_err;
    if (!rtp_mgr || !key || !key_size || !salt || !salt_size) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    srtp_ctx = is_rtp ? &rtp_mgr->srtp_contexts[idx][crypto_type].rtp : &rtp_mgr->srtp_contexts[idx][crypto_type].rtcp;
    if ((ret = trtp_srtp_ctx_internal_deinit(srtp_ctx))) {
        return ret;
    }

    switch ((srtp_ctx->crypto_type = crypto_type)) {
    case HMAC_SHA1_80:
    default: {
        crypto_policy_set_aes_cm_128_hmac_sha1_80(&srtp_ctx->policy.rtp);
        crypto_policy_set_aes_cm_128_hmac_sha1_80(&srtp_ctx->policy.rtcp);
        break;
    }
    case HMAC_SHA1_32: {
        crypto_policy_set_aes_cm_128_hmac_sha1_32(&srtp_ctx->policy.rtp);
        crypto_policy_set_aes_cm_128_hmac_sha1_80(&srtp_ctx->policy.rtcp); // always 80
        break;
    }
    }

    memcpy(srtp_ctx->key_bin, key, key_size);
#if HAVE_APPEND_SALT_TO_KEY
    append_salt_to_key(srtp_ctx->key_bin, key_size, (void*)salt, salt_size);
#else
    memcpy(&srtp_ctx->key_bin[key_size], salt, salt_size);
#endif

    srtp_ctx->policy.key = (unsigned char *)srtp_ctx->key_bin;
    srtp_ctx->policy.ssrc.type = idx == TRTP_SRTP_LINE_IDX_REMOTE ? ssrc_any_inbound : ssrc_any_outbound;
    srtp_ctx->policy.window_size = 2048;
    srtp_ctx->policy.allow_repeat_tx = 1;
    if((srtp_err = srtp_create(&srtp_ctx->session, &srtp_ctx->policy)) != err_status_ok) {
        TSK_DEBUG_ERROR("srtp_create() failed: %d", srtp_err);
        return -3;
    }
    srtp_ctx->initialized = tsk_true;
    return 0;
}

tsk_bool_t trtp_srtp_is_initialized(trtp_manager_t* rtp_mgr)
{
    if (!rtp_mgr) {
        return tsk_false;
    }
    return ((rtp_mgr->srtp_contexts[TRTP_SRTP_LINE_IDX_LOCAL][0].rtp.initialized || rtp_mgr->srtp_contexts[TRTP_SRTP_LINE_IDX_LOCAL][1].rtp.initialized)
            && rtp_mgr->srtp_contexts[TRTP_SRTP_LINE_IDX_REMOTE][0].rtp.initialized);
}

tsk_bool_t trtp_srtp_is_started(trtp_manager_t* rtp_mgr)
{
    if (!rtp_mgr) {
        TSK_DEBUG_ERROR("Invalid argument");
        return tsk_false;
    }
    return (rtp_mgr ->srtp_ctx_neg_remote && rtp_mgr ->srtp_ctx_neg_local);
}

#endif /* HAVE_SRTP */