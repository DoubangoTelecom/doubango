/*
 * Copyright (C) 2010-2015 Mamadou DIOP.
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

/**@file thttp_challenge.c
 * @brief HTTP authentication challenge.
 */
#include "tinyhttp/auth/thttp_challenge.h"

#include "thttp.h"

#include "tinyhttp/headers/thttp_header_Authorization.h"

#include "tsk_string.h"
#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_base64.h"
#include "tsk_hmac.h"

#include <string.h>

#define THTTP_CHALLENGE_IS_DIGEST(self)	((self) ? tsk_striequals((self)->scheme, "Digest") : tsk_false)
#define THTTP_CHALLENGE_IS_BASIC(self)	((self) ? tsk_striequals((self)->scheme, "Basic") : tsk_false)
#define THTTP_CHALLENGE_IS_AKAv1(self)	((self) ? tsk_striequals((self)->algorithm, "AKAv1-MD5") : tsk_false)
#define THTTP_CHALLENGE_IS_AKAv2(self)	((self) ? tsk_striequals((self)->algorithm, "AKAv2-MD5") : tsk_false)

static int _thttp_challenge_reset_cnonce(thttp_challenge_t *self);

thttp_challenge_t* thttp_challenge_create(tsk_bool_t isproxy, const char* scheme, const char* realm, const char* nonce, const char* opaque, const char* algorithm, const char* qop)
{
    thttp_challenge_t* challenge = tsk_object_new(thttp_challenge_def_t);
    if (challenge) {

        challenge->isproxy = isproxy;
        challenge->scheme = tsk_strdup(scheme);
        challenge->realm = tsk_strdup(realm);
        challenge->nonce = tsk_strdup(nonce);
        challenge->opaque = tsk_strdup(opaque);
        challenge->algorithm = tsk_strdup(algorithm);

        if (!tsk_strnullORempty(qop)) {
            challenge->qop = tsk_strcontains(qop, tsk_strlen(qop), "auth-int") ? "auth-int" :
                             (tsk_strcontains(qop, tsk_strlen(qop), "auth") ? "auth" : tsk_null);
        }

        if (challenge->qop) {
            _thttp_challenge_reset_cnonce(challenge);
        }
    }
    return challenge;
}

thttp_challenge_t* thttp_challenge_dup(const thttp_challenge_t* self)
{
    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }
    return thttp_challenge_create(self->isproxy, self->scheme, self->realm, self->nonce, self->opaque, self->algorithm, self->qop);
}

static int _thttp_challenge_reset_cnonce(thttp_challenge_t *self)
{
    if (self) {
        if (self->qop) { /* client nonce is only used if qop=auth, auth-int or both */
#if 0
            memcpy(self->cnonce, "f221681c1e42fb5f8f9957bf7e72eb2b", 32);
#else
            tsk_istr_t istr;

            tsk_strrandom(&istr);
            tsk_md5compute(istr, tsk_strlen(istr), &self->cnonce);
#endif
            self->nc = 1;
        }
    }
    return -1;
}

int thttp_challenge_get_digest_response(thttp_challenge_t *self, const char* username, const char* password, const char* method, const char* uristring, const tsk_buffer_t* entity_body, char** response)
{
    if (THTTP_CHALLENGE_IS_DIGEST(self)) {
        tsk_md5string_t ha1, ha2, md5_response;
        nonce_count_t nc;

        /* ===
         Calculate HA1 = MD5(A1) = M5(username:realm:secret)
         */
        thttp_auth_digest_HA1(username, self->realm, password, &ha1);

        /* ===
         HA2
         */
        thttp_auth_digest_HA2(method,
                              uristring,
                              entity_body,
                              self->qop,
                              &ha2);

        /* RESPONSE */
        if (self->nc) {
            THTTP_NCOUNT_2_STRING(self->nc, nc);
        }
        thttp_auth_digest_response((const tsk_md5string_t *)&ha1,
                                   self->nonce,
                                   nc,
                                   self->cnonce,
                                   self->qop,
                                   (const tsk_md5string_t *)&ha2,
                                   &md5_response);

        if (self->qop) {
            self->nc++;
        }
        if (response && !*response) {
            *response = tsk_strdup(md5_response);
        }

        return 0;
    }
    return -1;
}

int thttp_challenge_update(thttp_challenge_t *self, const char* scheme, const char* realm, const char* nonce, const char* opaque, const char* algorithm, const char* qop)
{
    if (self) {
        int noncechanged = !tsk_striequals(self->nonce, nonce);

        tsk_strupdate(&self->scheme, scheme);
        tsk_strupdate(&self->realm, realm);
        tsk_strupdate(&self->nonce, nonce);
        tsk_strupdate(&self->opaque, opaque);
        tsk_strupdate(&self->algorithm, algorithm);
        if (qop) {
            self->qop = tsk_strcontains(qop, tsk_strlen(qop), "auth-int") ? "auth-int" :
                        (tsk_strcontains(qop, tsk_strlen(qop), "auth") ? "auth" : tsk_null);
        }

        if (noncechanged && self->qop) {
            _thttp_challenge_reset_cnonce(self);
        }
        return 0;
    }
    return -1;
}

thttp_header_t *thttp_challenge_create_header_authorization(thttp_challenge_t *self, const char* username, const char* password, const thttp_request_t *request)
{
    char *uristring = tsk_null;
    thttp_header_t *header = tsk_null;

    if (!self || !request || !request->line.request.url) {
        TSK_DEBUG_ERROR("Invalid parameter");
        goto bail;
    }

    /* Sets URI: hpath do not start with / ==> append a '/'*/
    tsk_sprintf(&uristring, "/%s", request->line.request.url->hpath ? request->line.request.url->hpath : "");
    header = thttp_challenge_create_header_authorization_2(self, username, password, request->line.request.method, uristring, request->Content);
bail:
    TSK_FREE(uristring);
    return header;
}

thttp_header_t *thttp_challenge_create_header_authorization_2(thttp_challenge_t *self, const char* username, const char* password, const char* method, const char *uristring, const tsk_buffer_t* entity_body)
{
    char* response = tsk_null;
    tsk_size_t response_size = 0;
    nonce_count_t nc;
    thttp_header_t *header = tsk_null;

    if (!self || tsk_strnullORempty(uristring)) {
        TSK_DEBUG_ERROR("Invalid parameter");
        goto bail;
    }

    /* We compute the nc here because @ref thttp_challenge_get_response function will increment it's value. */
    if (self->nc) {
        THTTP_NCOUNT_2_STRING(self->nc, nc);
    }

    /* Computes the response (Basic and Digest)*/
    if (THTTP_CHALLENGE_IS_DIGEST(self)) {
        if (thttp_challenge_get_digest_response(self, username, password, method, uristring, entity_body, &response)) {
            goto bail;
        }
        response_size = (TSK_MD5_DIGEST_SIZE * 2);
    }
    else if (THTTP_CHALLENGE_IS_BASIC(self)) {
        response_size = thttp_auth_basic_response(username, password, &response);
    }
    else {
        TSK_DEBUG_ERROR("%s not supported as scheme.", self->scheme);
        goto bail;
    }


#define THTTP_AUTH_COPY_VALUES(hdr)															\
hdr->username = tsk_strdup(username);												\
hdr->scheme = tsk_strdup(self->scheme);												\
hdr->realm = tsk_strdup(self->realm);												\
hdr->nonce = tsk_strdup(self->nonce);												\
hdr->qop = tsk_strdup(self->qop);													\
hdr->opaque = tsk_strdup(self->opaque);												\
hdr->algorithm = self->algorithm ? tsk_strdup(self->algorithm) : tsk_strdup("MD5");	\
hdr->cnonce = self->nc? tsk_strdup(self->cnonce) : 0;								\
hdr->uri = tsk_strdup(uristring);													\
hdr->nc = self->nc? tsk_strdup(nc) : 0;												\
hdr->response = tsk_strndup(response, response_size);								\
 
    if (self->isproxy) {
        thttp_header_Proxy_Authorization_t *proxy_auth = thttp_header_authorization_create(); // Very bad way to create Proxy_auth header.
        THTTP_HEADER(proxy_auth)->type = thttp_htype_Proxy_Authorization;

        THTTP_AUTH_COPY_VALUES(proxy_auth);
        header = THTTP_HEADER(proxy_auth);
    }
    else {
        thttp_header_Authorization_t *auth = thttp_header_authorization_create();
        THTTP_AUTH_COPY_VALUES(auth);
        header = THTTP_HEADER(auth);
    }

bail:
    TSK_FREE(response);

    return header;

#undef THTTP_AUTH_COPY_VALUES
}























//========================================================
//	HTTP challenge object definition
//

/**@ingroup thttp_challenge_group
 */
static tsk_object_t* thttp_challenge_ctor(tsk_object_t *self, va_list * app)
{
    thttp_challenge_t *challenge = self;
    if (challenge) {

    }

    return self;
}

/**@ingroup thttp_challenge_group
 */
static tsk_object_t* thttp_challenge_dtor(tsk_object_t *self)
{
    thttp_challenge_t *challenge = self;
    if (challenge) {
        TSK_FREE(challenge->scheme);
        TSK_FREE(challenge->realm);
        TSK_FREE(challenge->nonce);
        TSK_FREE(challenge->opaque);
        TSK_FREE(challenge->algorithm);

        //TSK_FREE(challenge->qop);
    }
    else {
        TSK_DEBUG_ERROR("Null HTTP challenge object.");
    }

    return self;
}

static const tsk_object_def_t thttp_challenge_def_s = {
    sizeof(thttp_challenge_t),
    thttp_challenge_ctor,
    thttp_challenge_dtor,
    tsk_null
};
const tsk_object_def_t *thttp_challenge_def_t = &thttp_challenge_def_s;
