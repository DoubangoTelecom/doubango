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
#ifndef TINYRTP_SRTP_H
#define TINYRTP_SRTP_H

#include "tinyrtp_config.h"

#if HAVE_SRTP
#	include "tsk_common.h" 
#	include <srtp/srtp.h>

struct trtp_manager_s;

#define TRTP_SRTP_AES_CM_128_HMAC_SHA1_80 "AES_CM_128_HMAC_SHA1_80"
#define TRTP_SRTP_AES_CM_128_HMAC_SHA1_32 "AES_CM_128_HMAC_SHA1_32"

#define TRTP_SRTP_LINE_IDX_LOCAL	0
#define TRTP_SRTP_LINE_IDX_REMOTE	1

static const char* trtp_srtp_crypto_type_strings[2] =
{
	TRTP_SRTP_AES_CM_128_HMAC_SHA1_80, TRTP_SRTP_AES_CM_128_HMAC_SHA1_32
};

typedef enum trtp_srtp_crypto_type_e
{
	NONE = -1,
	HMAC_SHA1_80,
	HMAC_SHA1_32
}
trtp_srtp_crypto_type_t;

typedef struct trtp_srtp_ctx_xs
{
	int32_t tag;
	trtp_srtp_crypto_type_t crypto_type;
	char key_str[SRTP_MAX_KEY_LEN];
	char key_bin[30];

	srtp_t session;
	srtp_policy_t policy;
	tsk_bool_t initialized;
}
trtp_srtp_ctx_xt;

int trtp_srtp_ctx_init(struct trtp_srtp_ctx_xs* ctx, int32_t tag, trtp_srtp_crypto_type_t type, uint32_t ssrc);
int trtp_srtp_ctx_deinit(struct trtp_srtp_ctx_xs* ctx);
TINYRTP_API int trtp_srtp_match_line(const char* crypto_line, int32_t* tag, int32_t* crypto_type, char* key, tsk_size_t key_size);
TINYRTP_API int trtp_srtp_set_remote(struct trtp_manager_s* rtp_mgr, const char* crypto_line);
TINYRTP_API int trtp_srtp_get_ctx_local(struct trtp_manager_s* rtp_mgr, const struct trtp_srtp_ctx_xs** ctx, tsk_size_t *count);
TINYRTP_API tsk_bool_t trtp_srtp_is_initialized(struct trtp_manager_s* rtp_mgr);
TINYRTP_API tsk_bool_t trtp_srtp_is_active(struct trtp_manager_s* rtp_mgr);

#endif /* HAVE_SRTP */




#endif /* TINYRTP_SRTP_H */
