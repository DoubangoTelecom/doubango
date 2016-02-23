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
/**@file trtp_srtp.h
 */
#ifndef TINYRTP_SRTP_H
#define TINYRTP_SRTP_H

#include "tinyrtp_config.h"

#if HAVE_SRTP
#	include "tsk_common.h"
#	include <srtp/srtp.h>

struct trtp_manager_s;

typedef enum trtp_srtp_dtls_event_type_e {
    trtp_srtp_dtls_event_type_handshake_failed,
    trtp_srtp_dtls_event_type_handshake_succeed,
    trtp_srtp_dtls_event_type_fatal_error,
    trtp_srtp_dtls_event_type_started
}
trtp_srtp_dtls_event_type_t;

typedef enum trtp_srtp_crypto_type_e {
    NONE = -1,
    HMAC_SHA1_80,
    HMAC_SHA1_32,

    SRTP_CRYPTO_TYPES_MAX
}
trtp_srtp_crypto_type_t;

typedef enum trtp_srtp_state_e {
    trtp_srtp_state_none,
    /* at this state we're able to generated DTLS "fingerprints" and SDES "crypro" attributes
     but neither encrypt() nor decrypt() is possible.
     it's possible to move backward and disable SRTP (e.g. because of negotiation error)
     it's required to move to this state in order to be able to negotiate SRTP when mode is "optional" or "mandatory"
     */
    trtp_srtp_state_enabled,
    /* at this state both required parameters (e.g. "crypto" attributes) have been successfuly proceeded
     it's not possible to move backward and disable SRTP
     if type="SDES": start()ing the engine means we'll be imediately able to encrypt()/decrypt() data
     if type="DTLS": start()ing the engine doesn't mean we will be able to encrypt()/decrypt() data unless handshaking process successfuly completed
     */
    trtp_srtp_state_activated,
    /* at this state we're able to encrypt()/decrypt() SRTP data
     */
    trtp_srtp_state_started
}
trtp_srtp_state_t;

typedef int (*trtp_srtp_dtls_cb_f)(const void* usrdata,  enum trtp_srtp_dtls_event_type_e type, const char* reason);

#define TRTP_SRTP_AES_CM_128_HMAC_SHA1_80 "AES_CM_128_HMAC_SHA1_80"
#define TRTP_SRTP_AES_CM_128_HMAC_SHA1_32 "AES_CM_128_HMAC_SHA1_32"

#define TRTP_SRTP_LINE_IDX_LOCAL	0
#define TRTP_SRTP_LINE_IDX_REMOTE	1

static const char* trtp_srtp_crypto_type_strings[2] = {
    TRTP_SRTP_AES_CM_128_HMAC_SHA1_80, TRTP_SRTP_AES_CM_128_HMAC_SHA1_32
};


typedef struct trtp_srtp_ctx_internal_xs {
    int32_t tag;
    trtp_srtp_crypto_type_t crypto_type;
    char key_str[SRTP_MAX_KEY_LEN];
    char key_bin[SRTP_MASTER_KEY_LEN];
    tsk_bool_t have_valid_key;

    srtp_t session;
    srtp_policy_t policy;
    tsk_bool_t initialized;
}
trtp_srtp_ctx_internal_xt;

typedef struct trtp_srtp_ctx_xs {
    // (rtp == rtcp) for SDES but different for DTLS
    struct trtp_srtp_ctx_internal_xs rtp;
    struct trtp_srtp_ctx_internal_xs rtcp;
}
trtp_srtp_ctx_xt;

int trtp_srtp_ctx_internal_init(struct trtp_srtp_ctx_internal_xs* ctx, int32_t tag, trtp_srtp_crypto_type_t type, uint32_t ssrc);
int trtp_srtp_ctx_internal_deinit(struct trtp_srtp_ctx_internal_xs* ctx);
int trtp_srtp_ctx_init(struct trtp_srtp_ctx_xs* ctx, int32_t tag, trtp_srtp_crypto_type_t type, uint32_t ssrc);
int trtp_srtp_ctx_deinit(struct trtp_srtp_ctx_xs* ctx);
TINYRTP_API int trtp_srtp_match_line(const char* crypto_line, int32_t* tag, int32_t* crypto_type, char* key, tsk_size_t key_size);

TINYRTP_API int trtp_srtp_set_crypto(struct trtp_manager_s* rtp_mgr, const char* crypto_line, int32_t idx);
#define trtp_srtp_set_crypto_local(rtp_mgr, crypto_line) trtp_srtp_set_crypto((rtp_mgr), (crypto_line), TRTP_SRTP_LINE_IDX_LOCAL)
#define trtp_srtp_set_crypto_remote(rtp_mgr, crypto_line) trtp_srtp_set_crypto((rtp_mgr), (crypto_line), TRTP_SRTP_LINE_IDX_REMOTE)
TINYRTP_API int trtp_srtp_set_key_and_salt(struct trtp_manager_s* rtp_mgr, trtp_srtp_crypto_type_t crypto_type, const void* key, tsk_size_t key_size, const void* salt, tsk_size_t salt_size, int32_t idx, tsk_bool_t rtp);
#define trtp_srtp_set_key_and_salt_local(rtp_mgr, crypto_type, key, key_size, salt, salt_size, is_rtp) trtp_srtp_set_key_and_salt((rtp_mgr), (crypto_type), (key), (key_size), (salt), (salt_size), TRTP_SRTP_LINE_IDX_LOCAL, (is_rtp))
#define trtp_srtp_set_key_and_salt_remote(rtp_mgr, crypto_type, key, key_size, salt, salt_size, is_rtp) trtp_srtp_set_key_and_salt((rtp_mgr), (crypto_type), (key), (key_size), (salt), (salt_size), TRTP_SRTP_LINE_IDX_REMOTE, (is_rtp))
TINYRTP_API tsk_size_t trtp_srtp_get_local_contexts(struct trtp_manager_s* rtp_mgr, const struct trtp_srtp_ctx_xs ** contexts, tsk_size_t contexts_count);
TINYRTP_API tsk_bool_t trtp_srtp_is_initialized(struct trtp_manager_s* rtp_mgr);
TINYRTP_API tsk_bool_t trtp_srtp_is_started(struct trtp_manager_s* rtp_mgr);

#endif /* HAVE_SRTP */




#endif /* TINYRTP_SRTP_H */
