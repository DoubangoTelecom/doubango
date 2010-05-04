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

/**@file tsip_challenge.c
 * @brief SIP authentication challenge.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/authentication/tsip_challenge.h"

#include "tinysip/headers/tsip_header_Authorization.h"
#include "tinysip/headers/tsip_header_Proxy_Authorization.h"

#include "tsk_string.h"
#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_base64.h"
#include "tsk_hmac.h"

#include <string.h>

#define TSIP_CHALLENGE_IS_DIGEST(self)	((self) ? tsk_striequals((self)->scheme, "Digest") : 0)
#define TSIP_CHALLENGE_IS_AKAv1(self)	((self) ? tsk_striequals((self)->algorithm, "AKAv1-MD5") : 0)
#define TSIP_CHALLENGE_IS_AKAv2(self)	((self) ? tsk_striequals((self)->algorithm, "AKAv2-MD5") : 0)

#define TSIP_CHALLENGE_STACK(self)		(TSIP_STACK((self)->stack))
#define TSIP_CHALLENGE_USERNAME(self)	TSIP_CHALLENGE_STACK(self)->identity.impi
#define TSIP_CHALLENGE_PASSWORD(self)	TSIP_CHALLENGE_STACK(self)->identity.password


/** Creates new challenge object. */
tsip_challenge_t* tsip_challenge_create(tsip_stack_t* stack, tsk_bool_t isproxy, const char* scheme, const char* realm, const char* nonce, const char* opaque, const char* algorithm, const char* qop)
{
	return tsk_object_new(tsip_challenge_def_t, stack, isproxy,scheme, realm, nonce, opaque, algorithm, qop);
}

/** Creates new challenge object (with default values). */
tsip_challenge_t* tsip_challenge_create_null(tsip_stack_t* stack)
{
	return tsip_challenge_create(stack, tsk_false, tsk_null, tsk_null, tsk_null, tsk_null, tsk_null, tsk_null);
}


int tsip_challenge_reset_cnonce(tsip_challenge_t *self)
{
	if(self){
		if(self->qop) /* client nonce is only used if qop=auth, auth-int or both */
		{
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

//3GPP TS 35.205/6/7/8/9 and RFC 3310
int tsip_challenge_get_akares(tsip_challenge_t *self, char const *password, char** result)
{
#define SQN_XOR_AK() (AUTN + 0)
#define SERVER_DATA() (nonce + AKA_RAND_SIZE + AKA_AUTN_SIZE)

	// § ==> XOR
	// || ==> append

	AKA_RES_T akares;

	int ret = -1;
	size_t n;
	char *nonce = 0;

	AKA_XXX_DECLARE(RAND);
	AKA_XXX_DECLARE(AK);
	AKA_XXX_DECLARE(AMF);
	AKA_XXX_DECLARE(CK);
	AKA_XXX_DECLARE(IK);
	AKA_XXX_DECLARE(K);
	AKA_XXX_DECLARE(SQN);
	AKA_XXX_DECLARE(MAC_A);
	AKA_XXX_DECLARE(AUTN);

	AKA_XXX_BZERO(RAND);
	AKA_XXX_BZERO(AK);
	AKA_XXX_BZERO(AMF);
	AKA_XXX_BZERO(CK);
	AKA_XXX_BZERO(IK);
	AKA_XXX_BZERO(K);
	AKA_XXX_BZERO(SQN);
	AKA_XXX_BZERO(MAC_A);
	AKA_XXX_BZERO(AUTN);

	/* RFC 3310 subclause 3.2: nonce = base64(RAND || AUTN || SERV_DATA) */
	n = tsk_base64_decode((const uint8_t*)self->nonce, tsk_strlen(self->nonce), &nonce);	
	if(n > TSK_MD5_STRING_SIZE){
		TSK_DEBUG_ERROR("The IMS CORE returned an invalid nonce.");
		goto bail;
	}
	if(n < AKA_RAND_SIZE + AKA_AUTN_SIZE){
		TSK_DEBUG_ERROR("The nonce returned by the IMS CORE is too short to contain both [RAND] and [AUTHN]");
		goto bail;
	}
	else{
		/* Get RAND and AUTN */
		memcpy(RAND, nonce, AKA_RAND_SIZE);
		memcpy(AUTN, (nonce + AKA_RAND_SIZE), AKA_AUTN_SIZE);
	}

	/* Secret key */
	memcpy(K, password, (tsk_strlen(password) > AKA_K_SIZE ? AKA_K_SIZE : tsk_strlen(password)));

	/* 3GPP TS 35.205: AUTN = SQN[§AK] || AMF || MAC-A */
	memcpy(AMF, (AUTN + AKA_SQN_SIZE), AKA_AMF_SIZE);
	memcpy(MAC_A, (AUTN + AKA_SQN_SIZE + AKA_AMF_SIZE), AKA_MAC_A_SIZE);

	/* compute OP */
	ComputeOP(TSIP_CHALLENGE_STACK(self)->security.operator_id);

	/* Checks that we hold the same AMF */
	for(n=0; n<AKA_AMF_SIZE; n++){
		if(AMF[n] != TSIP_CHALLENGE_STACK(self)->security.amf[n]){
			TSK_DEBUG_ERROR("IMS-AKA error: AMF <> XAMF");
			goto bail;
		}
	}
	
	/* Calculate CK, IK and AK */
	f2345(K, RAND, akares, CK, IK, AK);

	/* Calculate SQN from SQN_XOR_AK */
	for(n=0; n<AKA_SQN_SIZE; n++){
		SQN[n] = (uint8_t) (SQN_XOR_AK()[n] ^ AK[n]);
	}

	/* Calculate XMAC_A */
	{
		AKA_MAC_A_T XMAC_A;
		f1(K, RAND, SQN, AMF, XMAC_A);
		if(!tsk_strnequals(MAC_A, XMAC_A, AKA_MAC_A_SIZE)){
			TSK_DEBUG_ERROR("IMS-AKA error: XMAC_A <> MAC_A");
			goto bail;
		}
	}

	/* RFC 4169 subclause 3
		The HTTP Digest password is derived from base64 encoded PRF(RES || IK||CK, "http-digest-akav2-password") 
		or 
		PRF(XRES||IK||CK, "http-digest-akav2-password") instead of (RES) or (XRES) respectively.
		Where PRF ==> HMAC_MD5 function.
	*/
	if(TSIP_CHALLENGE_IS_AKAv2(self)){
		uint8_t res_ik_ck[AKA_RES_SIZE + AKA_IK_SIZE + AKA_CK_SIZE];
		tsk_md5digest_t md5_digest;

		memcpy(res_ik_ck, akares, AKA_RES_SIZE);
		memcpy((res_ik_ck + AKA_RES_SIZE), IK, AKA_IK_SIZE);
		memcpy((res_ik_ck + AKA_RES_SIZE + AKA_IK_SIZE), CK, AKA_CK_SIZE);

		if((ret = hmac_md5digest_compute("http-digest-akav2-password", 26, res_ik_ck, sizeof(res_ik_ck), md5_digest))){/* PRF(RES||IK||CK, ...) */
			TSK_DEBUG_ERROR("hmac_md5digest_compute() failed. AKAv2 response will be invalid.");

			ret = -3;
			goto bail;
		}
		else{/* b64(PRF(...)) */
			if(!tsk_base64_encode(md5_digest, sizeof(md5_digest), result)){
				TSK_DEBUG_ERROR("tsk_base64_encode() failed. AKAv2 response will be invalid.");

				ret = -4;
				goto bail;
			}
		}
	}
	else{
		*result = tsk_calloc(1, AKA_RES_SIZE + 1);
		memcpy(*result, akares, AKA_RES_SIZE);

		ret = 0;
	}

	/* Copy CK and IK */
	memcpy(self->ck, CK, AKA_CK_SIZE);
	memcpy(self->ik, IK, AKA_IK_SIZE);

bail:
	TSK_FREE(nonce);
	return ret;

#undef SQN_XOR_AK
#undef SERVER_DATA
}

int tsip_challenge_get_response(tsip_challenge_t *self, const char* method, const char* uristring, const tsk_buffer_t* entity_body, tsk_md5string_t* response)
{
	if(TSIP_CHALLENGE_IS_DIGEST(self) && self->stack)
	{
		tsk_md5string_t ha1, ha2;
		nonce_count_t nc;

		/* ===
			Calculate HA1 = MD5(A1) = M5(username:realm:secret)
			In case of AKAv1-MD5 and AKAv2-MD5 the secret must be computed as per RFC 3310 + 3GPP TS 206/7/8/9.
			The resulting AKA RES parameter is treated as a "password"/"secret" when calculating the response directive of RFC 2617.
		*/
		if(TSIP_CHALLENGE_IS_AKAv1(self) || TSIP_CHALLENGE_IS_AKAv2(self)){
			char* akaresult = 0;
			tsip_challenge_get_akares(self, TSIP_CHALLENGE_STACK(self)->identity.password, &akaresult);
			if(thttp_auth_digest_HA1(TSIP_CHALLENGE_USERNAME(self), self->realm, akaresult, &ha1)){
				// return -1;
			}
			TSK_FREE(akaresult);
		}
		else{
			thttp_auth_digest_HA1(TSIP_CHALLENGE_USERNAME(self), self->realm, TSIP_CHALLENGE_STACK(self)->identity.password, &ha1);
		}

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

int tsip_challenge_update(tsip_challenge_t *self, const char* scheme, const char* realm, const char* nonce, const char* opaque, const char* algorithm, const char* qop)
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
			self->qop = tsk_strcontains(qop, tsk_strlen(qop), "auth-int") ? "auth-int" : 
					(tsk_strcontains(qop, tsk_strlen(qop), "auth") ? "auth" : tsk_null);
		}

		if(noncechanged && self->qop){
			tsip_challenge_reset_cnonce(self);
		}
		return 0;
	}
	return -1;
}

tsip_header_t *tsip_challenge_create_header_authorization(tsip_challenge_t *self, const tsip_request_t *request)
{
	tsk_md5string_t response;
	nonce_count_t nc;
	char *uristring = 0;
	tsip_header_t *header = 0;

	if(!self || !self->stack || !request)
	{
		goto bail;
	}

	/* Create the request uri string from uri object
	*/
	{
		tsk_istr_t port;

		if(request->uri->port) tsk_itoa(request->uri->port, &port);
		tsk_sprintf(&uristring, "%s%s%s%s%s%s%s%s%s", 

		request->uri->scheme ? request->uri->scheme : "",
		request->uri->scheme ? ":" : "",

		request->uri->user_name ? request->uri->user_name : "",

		request->uri->password ? ":" : "",
		request->uri->password ? request->uri->password : "",

		request->uri->host ? (request->uri->user_name ? "@" : "") : "",
		request->uri->host ? request->uri->host : "",

		request->uri->port ? ":" : "",
		request->uri->port ? port : ""
		);
	}

	/* We compute the nc here because @ref tsip_challenge_get_response function will increment it's value. */
	if(self->nc){
		THTTP_NCOUNT_2_STRING(self->nc, nc);
	}

	/* entity_body ==> request-content */
	if(tsip_challenge_get_response(self, request->method, uristring, request->Content, &response)){
		goto bail;
	}
	

#define TSIP_AUTH_COPY_VALUES(hdr)															\
		hdr->username = tsk_strdup(TSIP_CHALLENGE_USERNAME(self));							\
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
		tsip_header_Proxy_Authorization_t *proxy_auth = tsip_header_Proxy_Authorization_create();
		TSIP_AUTH_COPY_VALUES(proxy_auth);
		header = TSIP_HEADER(proxy_auth);
	}
	else{
		tsip_header_Authorization_t *auth = tsip_header_Authorization_create();
		TSIP_AUTH_COPY_VALUES(auth);
		header = TSIP_HEADER(auth);
	}

bail:
	TSK_FREE(uristring);

	return header;

#undef TSIP_AUTH_COPY_VALUES
}

tsip_header_t *tsip_challenge_create_empty_header_authorization(const char* username, const char* realm, const char* uristring)
{
	tsip_header_Authorization_t *header = tsip_header_Authorization_create();

	if(header)
	{
		header->scheme = tsk_strdup("Digest");
		header->username = tsk_strdup(username);
		header->realm = tsk_strdup(realm);
		header->nonce = tsk_strdup("");
		header->response = tsk_strdup("");
		header->uri = tsk_strdup(uristring);
	}

	return TSIP_HEADER(header);
}

























//========================================================
//	SIP challenge object definition
//

/**@ingroup tsip_challenge_group
*/
static tsk_object_t* tsip_challenge_ctor(tsk_object_t *self, va_list * app)
{
	tsip_challenge_t *challenge = self;
	if(challenge){
		const char* qop;

		challenge->stack = va_arg(*app, const tsip_stack_handle_t *);
		challenge->isproxy = va_arg(*app, tsk_bool_t);
		challenge->scheme = tsk_strdup(va_arg(*app, const char*));
		challenge->realm = tsk_strdup(va_arg(*app, const char*));
		challenge->nonce = tsk_strdup(va_arg(*app, const char*));
		challenge->opaque = tsk_strdup(va_arg(*app, const char*));
		challenge->algorithm = tsk_strdup(va_arg(*app, const char*));
		qop = va_arg(*app, const char*);
		if(qop){
			challenge->qop = tsk_strcontains(qop, tsk_strlen(qop), "auth-int") ? "auth-int" : 
					(tsk_strcontains(qop, tsk_strlen(qop), "auth") ? "auth" : tsk_null);
		}
		
		if(challenge->qop){
			tsip_challenge_reset_cnonce(challenge);
		}
	}
	else TSK_DEBUG_ERROR("Failed to create new sip challenge object.");
	
	return self;
}

/**@ingroup tsip_challenge_group
*/
static tsk_object_t* tsip_challenge_dtor(tsk_object_t *self)
{
	tsip_challenge_t *challenge = self;
	if(challenge){
		TSK_FREE(challenge->scheme);
		TSK_FREE(challenge->realm);
		TSK_FREE(challenge->nonce);
		TSK_FREE(challenge->opaque);
		TSK_FREE(challenge->algorithm);
		
		//TSK_FREE(challenge->qop);
	}
	else{
		TSK_DEBUG_ERROR("Null SIP challenge object.");
	}

	return self;
}

static const tsk_object_def_t tsip_challenge_def_s = 
{
	sizeof(tsip_challenge_t),
	tsip_challenge_ctor,
	tsip_challenge_dtor,
	tsk_null
};
const tsk_object_def_t *tsip_challenge_def_t = &tsip_challenge_def_s;
