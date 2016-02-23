/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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

/**@file tsk_sha1.h
 * @brief US Secure Hash Algorithm 1 (RFC 3174)
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef _TINYSAK_SHA1_H_
#define _TINYSAK_SHA1_H_

#include "tinysak_config.h"

TSK_BEGIN_DECLS

/**@ingroup tsk_sha1_group
* SHA-1 error codes.
*/
typedef enum tsk_sha1_errcode_e {
    shaSuccess = 0,		/**< Success */
    shaNull,            /**< Null pointer parameter */
    shaInputTooLong,    /**< input data too long */
    shaStateError       /**< called Input after Result */
}
tsk_sha1_errcode_t;

/**@ingroup tsk_sha1_group
*@def TSK_SHA1_DIGEST_SIZE
*/
/**@ingroup tsk_sha1_group
*@def TSK_SHA1_BLOCK_SIZE
*/
/**@ingroup tsk_sha1_group
*@def TSK_SHA1_STRING_SIZE
*/
/**@ingroup tsk_sha1_group
*@def tsk_sha1string_t
* Hexadecimal SHA-1 digest string.
*/
/**@ingroup tsk_sha1_group
*@def tsk_sha1digest_t
* SHA-1 digest bytes.
*/

#define TSK_SHA1_DIGEST_SIZE			20
#define TSK_SHA1_BLOCK_SIZE				64

#define TSK_SHA1_STRING_SIZE		(TSK_SHA1_DIGEST_SIZE*2)
typedef uint8_t tsk_sha1string_t[TSK_SHA1_STRING_SIZE+1];
typedef uint8_t tsk_sha1digest_t[TSK_SHA1_DIGEST_SIZE]; /**< SHA-1 digest bytes. */

/**@ingroup tsk_sha1_group
* Computes SHA-1 digest.
* @param input The input data.
* @param input_size The size of the input data.
* @param digest @ref tsk_sha1digest_t object conaining the sha1 digest result.
* @sa @ref tsk_sha1compute.
*/
#define TSK_SHA1_DIGEST_CALC(input, input_size, digest)			\
			{													\
				tsk_sha1context_t ctx;							\
				tsk_sha1reset(&ctx);							\
				tsk_sha1input(&ctx, (input), (input_size));		\
				tsk_sha1result(&ctx, (digest));					\
			}

/**@ingroup tsk_sha1_group
 *  This structure will hold context information for the SHA-1
 *  hashing SSESSION
 */
typedef struct tsk_sha1context_s {
    uint32_t Intermediate_Hash[TSK_SHA1_DIGEST_SIZE/4]; /* Message Digest  */

    uint32_t Length_Low;            /**< Message length in bits      */
    uint32_t Length_High;           /**< Message length in bits      */


    int_least16_t Message_Block_Index;/**< Index into message block array   */
    uint8_t Message_Block[64];      /**< 512-bit message blocks      */

    int32_t Computed;               /**< Is the digest computed?         */
    int32_t Corrupted;             /**< Is the message digest corrupted? */
}
tsk_sha1context_t;

/*
 *  Function Prototypes
 */

TINYSAK_API tsk_sha1_errcode_t tsk_sha1reset(tsk_sha1context_t *);
TINYSAK_API tsk_sha1_errcode_t tsk_sha1input(tsk_sha1context_t *, const uint8_t *, unsigned length);
TINYSAK_API tsk_sha1_errcode_t tsk_sha1result(tsk_sha1context_t *, tsk_sha1digest_t Message_Digest);
TINYSAK_API void tsk_sha1final(uint8_t *Message_Digest, tsk_sha1context_t *context);
TINYSAK_API tsk_sha1_errcode_t tsk_sha1compute(const char* input, tsk_size_t size, tsk_sha1string_t *result);

TSK_END_DECLS

#endif /* _TINYSAK_SHA1_H_ */

