/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou@yahoo.fr>
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
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
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

#define THTTP_CHALLENGE_IS_DIGEST(self)	((self) ? tsk_striequals((self)->scheme, "Digest") : 0)
#define THTTP_CHALLENGE_IS_AKAv1(self)	((self) ? tsk_striequals((self)->algorithm, "AKAv1-MD5") : 0)
#define THTTP_CHALLENGE_IS_AKAv2(self)	((self) ? tsk_striequals((self)->algorithm, "AKAv2-MD5") : 0)

int thttp_challenge_reset_cnonce(thttp_challenge_t *self)
{
	if(self)
	{
		if(self->qop) /* client nonce is only used if qop=auth, auth-int or both */
		{
#if 0
			memcpy(self->cnonce, "f221681c1e42fb5f8f9957bf7e72eb2b", 32);
#else
			tsk_istr_t istr;
			
			tsk_strrandom(&istr);
			tsk_md5compute(istr, strlen(istr), &self->cnonce);
#endif
			self->nc = 1;
		}
	}
	return -1;
}

int thttp_challenge_get_response(thttp_challenge_t *self, const char* method, const char* uristring, const tsk_buffer_t* entity_body, tsk_md5string_t* response)
{
	if(THTTP_CHALLENGE_IS_DIGEST(self))
	{
		tsk_md5string_t ha1, ha2;
		nonce_count_t nc;

		/* ===
			Calculate HA1 = MD5(A1) = M5(username:realm:secret)
		*/
		thttp_auth_digest_HA1(self->username, self->realm, self->password, &ha1);
		
		/* ===
			HA2 
		*/
		thttp_auth_digest_HA2(method,
			uristring,
			entity_body,
			self->qop,
			&ha2);

		/* RESPONSE */
		if(self->nc){
			THTTP_NCOUNT_2_STRING(self->nc, nc);
		}
		thttp_auth_digest_response((const tsk_md5string_t *)&ha1, 
			self->nonce,
			nc,
			self->cnonce,
			self->qop,
			(const tsk_md5string_t *)&ha2,
			response);
		
		if(self->qop){
			self->nc++;
		}

		return 0;
	}
	return -1;
}

int thttp_challenge_update(thttp_challenge_t *self, const char* scheme, const char* realm, const char* nonce, const char* opaque, const char* algorithm, const char* qop)
{
	if(self)
	{
		int noncechanged = !tsk_striequals(self->nonce, nonce);

		tsk_strupdate(&self->scheme, scheme);
		tsk_strupdate(&self->realm, realm);
		tsk_strupdate(&self->nonce, nonce);
		tsk_strupdate(&self->opaque, opaque);
		tsk_strupdate(&self->algorithm, algorithm);
		if(qop){
			self->qop = tsk_strcontains(qop, strlen(qop), "auth-int") ? "auth-int" : 
					(tsk_strcontains(qop, strlen(qop), "auth") ? "auth" : 0);
		}

		if(noncechanged && self->qop){
			thttp_challenge_reset_cnonce(self);
		}
		return 0;
	}
	return -1;
}

thttp_header_t *thttp_challenge_create_header_authorization(thttp_challenge_t *self, const thttp_request_t *request)
{
	tsk_md5string_t response;
	nonce_count_t nc;
	char *uristring = 0;
	thttp_header_t *header = 0;

	if(!self || !request){
		goto bail;
	}

	/* Create the request uri string from uri object
	*/
	if(!(uristring = thttp_url_tostring(request->url))){
		goto bail;
	}

	/* We compute the nc here because @ref thttp_challenge_get_response function will increment it's value. */
	if(self->nc){
		THTTP_NCOUNT_2_STRING(self->nc, nc);
	}

	// FIXME: entity_body ==> request-content
	if(thttp_challenge_get_response(self, request->method, uristring, 0/*FIXME*/, &response))
	{
		goto bail;
	}


#define THTTP_AUTH_COPY_VALUES(hdr)															\
		hdr->username = tsk_strdup(self->username);											\
		hdr->scheme = tsk_strdup(self->scheme);												\
		hdr->realm = tsk_strdup(self->realm);												\
		hdr->nonce = tsk_strdup(self->nonce);												\
		hdr->qop = tsk_strdup(self->qop);													\
		hdr->opaque = tsk_strdup(self->opaque);												\
		hdr->algorithm = self->algorithm ? tsk_strdup(self->algorithm) : tsk_strdup("MD5");	\
		hdr->cnonce = self->nc? tsk_strdup(self->cnonce) : 0;								\
		hdr->uri = tsk_strdup(uristring);													\
		hdr->nc = self->nc? tsk_strdup(nc) : 0;												\
		hdr->response = tsk_strdup(response);												\

	if(self->isproxy){
		thttp_header_Proxy_Authorization_t *proxy_auth = THTTP_HEADER_AUTHORIZATION_CREATE(); // Very bad way to create Proxy_auth header.
		THTTP_HEADER(proxy_auth)->type = thttp_htype_Proxy_Authorization;

		THTTP_AUTH_COPY_VALUES(proxy_auth);
		header = THTTP_HEADER(proxy_auth);
	}
	else{
		thttp_header_Authorization_t *auth = THTTP_HEADER_AUTHORIZATION_CREATE();
		THTTP_AUTH_COPY_VALUES(auth);
		header = THTTP_HEADER(auth);
	}

bail:
	TSK_FREE(uristring);

	return header;

#undef THTTP_AUTH_COPY_VALUES
}
























//========================================================
//	HTTP challenge object definition
//

/**@ingroup thttp_challenge_group
*/
static void* thttp_challenge_create(void *self, va_list * app)
{
	thttp_challenge_t *challenge = self;
	if(challenge)
	{
		const char* qop;

		challenge->username = tsk_strdup(va_arg(*app, const char*));
		challenge->password = tsk_strdup(va_arg(*app, const char*));
		challenge->isproxy = va_arg(*app, unsigned);
		challenge->scheme = tsk_strdup(va_arg(*app, const char*));
		challenge->realm = tsk_strdup(va_arg(*app, const char*));
		challenge->nonce = tsk_strdup(va_arg(*app, const char*));
		challenge->opaque = tsk_strdup(va_arg(*app, const char*));
		challenge->algorithm = tsk_strdup(va_arg(*app, const char*));
		qop = va_arg(*app, const char*);
		if(qop){
			challenge->qop = tsk_strcontains(qop, strlen(qop), "auth-int") ? "auth-int" : 
					(tsk_strcontains(qop, strlen(qop), "auth") ? "auth" : 0);
		}
		
		if(challenge->qop){
			thttp_challenge_reset_cnonce(challenge);
		}
	}
	else TSK_DEBUG_ERROR("Failed to create new http challenge object.");
	
	return self;
}

/**@ingroup thttp_challenge_group
*/
static void* thttp_challenge_destroy(void *self)
{
	thttp_challenge_t *challenge = self;
	if(challenge)
	{
		TSK_FREE(challenge->username);
		TSK_FREE(challenge->password);
		TSK_FREE(challenge->scheme);
		TSK_FREE(challenge->realm);
		TSK_FREE(challenge->nonce);
		TSK_FREE(challenge->opaque);
		TSK_FREE(challenge->algorithm);
		
		//TSK_FREE(challenge->qop);
	}
	else TSK_DEBUG_ERROR("Null HTTP challenge object.");

	return self;
}

static const tsk_object_def_t thttp_challenge_def_s = 
{
	sizeof(thttp_challenge_t),
	thttp_challenge_create,
	thttp_challenge_destroy,
	0
};
const void *thttp_challenge_def_t = &thttp_challenge_def_s;
