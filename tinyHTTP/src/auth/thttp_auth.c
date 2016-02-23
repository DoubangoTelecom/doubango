/*
* Copyright (C) 2010-2015 Mamadou Diop.
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
/**@file thttp_auth.c
 * @brief HTTP basic/digest authetication (RFC 2617) implementations.
 */
#include "tinyhttp/auth/thttp_auth.h"

#include "tsk_string.h"
#include "tsk_sha1.h"
#include "tsk_base64.h"
#include "tsk_buffer.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#include <string.h>

/**@defgroup thttp_auth_group HTTP basic/digest authentication (RFC 2617)
*/

/**@ingroup thttp_auth_group
 *
 * Generates HTTP-basic response as per RFC 2617.
 *
 * @param [in,out]	userid		The user-id.
 * @param [in,out]	password	The user-password.
 * @param [in,out]	response	A pointer to the response. It will be up to the caller to free the newly allocated buffer.
 *
 * @return	The size of the response.
 **/
tsk_size_t thttp_auth_basic_response(const char* userid, const char* password, char** response)
{
    tsk_size_t ret;

    /* RFC 2617 - 2 Basic Authentication Scheme

    To receive authorization, the client sends the userid and password,
    separated by a single colon (":") character, within a base64 [7]
    encoded string in the credentials.
    */

    char *res = 0;
    tsk_sprintf(&res, "%s:%s", userid, password);
    ret = tsk_base64_encode((const uint8_t*)res, tsk_strlen(res), response);
    TSK_FREE(res);

    return ret;
}


/**@ingroup thttp_auth_group
 * Generates digest HA1 value as per RFC 2617 subclause 3.2.2.2.
 *
 *
 * @param [in,out]	username	The user's name (unquoted) in the specified @a realm.
 * @param [in,out]	realm		The realm. (unquoted)
 * @param [in,out]	password	The user's password.
 * @param [in,out]	ha1			A pointer to the result.
 *
 * @return	Zero if succeed and non-zero error code otherwise.
 **/
int thttp_auth_digest_HA1(const char* username, const char* realm, const char* password, tsk_md5string_t* ha1)
{
    int ret;

    /* RFC 2617 - 3.2.2.2 A1
    	A1       = unq(username-value) ":" unq(realm-value) ":" passwd
    	*/
    char *a1 = tsk_null;
    tsk_sprintf(&a1, "%s:%s:%s", username, realm, password);
    ret = tsk_md5compute(a1, tsk_strlen(a1), ha1);
    TSK_FREE(a1);

    return ret;
}

/**@ingroup thttp_auth_group
 *
 * Generates digest HA1 value for 'MD5-sess' algo as per RFC 2617 subclause 3.2.2.2.
 *
 *
 * @param [in,out]	username	The user's name (unquoted) in the specified @a realm.
 * @param [in,out]	realm		The realm (unquoted).
 * @param [in,out]	password	The user's password.
 * @param [in,out]	nonce		The nonce (unquoted).
 * @param [in,out]	cnonce		The client nonce (unquoted).
 * @param [in,out]	ha1sess		A pointer to the result.
 *
 * @return	Zero if succeed and non-zero error code otherwise.
 **/
int thttp_auth_digest_HA1sess(const char* username, const char* realm, const char* password, const char* nonce, const char* cnonce, tsk_md5string_t* ha1sess)
{
    int ret;

    /* RFC 2617 - 3.2.2.2 A1
    		A1       = H( unq(username-value) ":" unq(realm-value)
    		":" passwd )
    		":" unq(nonce-value) ":" unq(cnonce-value)
    		*/

    char *a1sess = tsk_null;
    tsk_sprintf(&a1sess, "%s:%s:%s:%s:%s", username, realm, password, nonce, cnonce);
    ret = tsk_md5compute(a1sess, tsk_strlen(a1sess), ha1sess);
    TSK_FREE(a1sess);

    return ret;
}

/**@ingroup thttp_auth_group
 * Generates digest HA2 value as per RFC 2617 subclause 3.2.2.3.
 *
 *
 * @param [in,out]	method		The HTTP/SIP method name.
 * @param [in,out]	url			The HTTP URL or SIP URI of the request.
 * @param [in,out]	entity_body	The entity body.
 * @param [in,out]	qop			The Quality Of Protection.
 * @param [in,out]	ha2			A pointer to the response.
 *
 * @return	Zero if succeed and non-zero error code otherwise.
 **/
int thttp_auth_digest_HA2(const char* method, const char* url, const tsk_buffer_t* entity_body, const char* qop, tsk_md5string_t* ha2)
{
    int ret;
    /* RFC 2617 - 3.2.2.3 A2

    If the "qop" directive's value is "auth" or is unspecified, then A2
    is:
    A2       = Method ":" digest-url-value

    If the "qop" value is "auth-int", then A2 is:
    A2       = Method ":" digest-url-value ":" H(entity-body)
    */

    char *a2 = tsk_null;

    if (!qop || tsk_strempty(qop) || tsk_striequals(qop, "auth")) {
        tsk_sprintf(&a2, "%s:%s", method, url);
    }
    else if (tsk_striequals(qop, "auth-int")) {
        if (entity_body && entity_body->data && entity_body->size) {
            tsk_md5string_t hEntity;
            if ((ret = tsk_md5compute(entity_body->data, entity_body->size, &hEntity))) {
                goto bail;
            }
            tsk_sprintf(&a2, "%s:%s:%s", method, url, hEntity);
        }
        else {
            tsk_sprintf(&a2, "%s:%s:%s", method, url, TSK_MD5_EMPTY);
        }
    }

    ret = tsk_md5compute(a2, tsk_strlen(a2), ha2);

bail:
    TSK_FREE(a2);

    return ret;
}


/**@ingroup thttp_auth_group
 *
 * Generates HTTP digest response as per RFC 2617 subclause 3.2.2.1.
 *
 * @param [in,out]	ha1			HA1 string generated using  @ref thttp_auth_digest_HA1 or @ref thttp_auth_digest_HA1sess.
 * @param [in,out]	nonce		The nonce value.
 * @param [in,out]	noncecount	The nonce count.
 * @param [in,out]	cnonce		The client nounce (unquoted).
 * @param [in,out]	qop			The Quality Of Protection (unquoted).
 * @param [in,out]	ha2			HA2 string generated using @ref thttp_auth_digest_HA2.
 * @param [in,out]	response	A pointer to the response.
 *
 * @return	Zero if succeed and non-zero error code otherwise.
 **/
int thttp_auth_digest_response(const tsk_md5string_t *ha1, const char* nonce, const nonce_count_t noncecount, const char* cnonce,
                               const char* qop, const tsk_md5string_t* ha2, tsk_md5string_t* response)
{
    int ret;

    /* RFC 2617 3.2.2.1 Request-Digest

    ============ CASE 1 ============
    If the "qop" value is "auth" or "auth-int":
    request-digest  = <"> < KD ( H(A1),     unq(nonce-value)
    ":" nc-value
    ":" unq(cnonce-value)
    ":" unq(qop-value)
    ":" H(A2)
    ) <">
    ============ CASE 2 ============
    If the "qop" directive is not present (this construction is for
    compatibility with RFC 2069):
    request-digest  =
    <"> < KD ( H(A1), unq(nonce-value) ":" H(A2) ) >
    <">
    */

    char *res = tsk_null;

    if (tsk_striequals(qop, "auth") || tsk_striequals(qop, "auth-int")) {
        /* CASE 1 */
        tsk_sprintf(&res, "%s:%s:%s:%s:%s:%s", *ha1, nonce, noncecount, cnonce, qop, *ha2);
    }
    else {
        /* CASE 2 */
        tsk_sprintf(&res, "%s:%s:%s", *ha1, nonce, *ha2);
    }

    ret = tsk_md5compute(res, tsk_strlen(res), response);
    TSK_FREE(res);

    return ret;
}

/**@ingroup thttp_auth_group
 *
 * Generates WebSocket Accept key.
 * @param [in]	key		The value of the key received from the client ("Sec-WebSocket-Key" header). Must be null-terminated.
 * @param [in,out]	response		The response ("Sec-WebSocket-Value" header).
 *
 * @return	The size of the response. Zero if error.
 */
tsk_size_t thttp_auth_ws_response(const char* key, thttp_auth_ws_keystring_t* response)
{
    if (!key || !response) {
        TSK_DEBUG_ERROR("invalid parameter");
        return 0;
    }
    else {
        tsk_sha1string_t sha1result;
        char* tmp = tsk_null;
        long ret;
        tsk_size_t size, i;
        uint8_t result[21] = { 0 };

        tsk_strcat_2(&tmp, "%s258EAFA5-E914-47DA-95CA-C5AB0DC85B11", key);

        tsk_sha1compute(tmp, tsk_strlen(tmp), &sha1result);
        size = tsk_strlen((char*)sha1result);
        for (i = 0; i < size; i += 2) {
            if (sscanf((const char*)&sha1result[i], "%2x", (unsigned int*)&ret) != EOF) {
                ;
                result[i >> 1] = (char)ret;
            }
        }
        TSK_FREE(tmp);
        return tsk_base64_encode(result, (size >> 1), (char**)&response);
    }
}
