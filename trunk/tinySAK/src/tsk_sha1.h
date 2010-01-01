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
* GNU Lesser General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

/**@file tsk_sha1.h
 * @brief US Secure Hash Algorithm 1 (RFC 3174)
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TINYSAK_SHA1_H_
#define _TINYSAK_SHA1_H_

#include "tinySAK_config.h"

TSK_BEGIN_DECLS

typedef enum tsk_sha1_errcode_e
{
    shaSuccess = 0,
    shaNull,            /**< Null pointer parameter */
    shaInputTooLong,    /**< input data too long */
    shaStateError       /**< called Input after Result */
}
tsk_sha1_errcode_t;


#define TSK_SHA1_DIGEST_SIZE			20
#define TSK_SHA1_BLOCK_SIZE				64

#define TSK_SHA1_STRING_SIZE		(TSK_SHA1_DIGEST_SIZE*2)
typedef char tsk_sha1string_t[TSK_SHA1_STRING_SIZE+1]; /**< Hexadecimal SHA-1 digest string. */
typedef char tsk_sha1digest_t[TSK_SHA1_DIGEST_SIZE]; /**< SHA-1 digest bytes. */

#define TSK_SHA1_DIGEST_CALC(input, input_size, digest)			\
			{													\
				tsk_sha1context_t ctx;							\
				tsk_sha1reset(&ctx);							\
				tsk_sha1input(&ctx, (input), (input_size));		\
				tsk_sha1result(&ctx, (digest));					\
			}

/*
 *  This structure will hold context information for the SHA-1
 *  hashing operation
 */
typedef struct tsk_sha1context_s
{
    uint32_t Intermediate_Hash[TSK_SHA1_DIGEST_SIZE/4]; /* Message Digest  */

    uint32_t Length_Low;            /* Message length in bits      */
    uint32_t Length_High;           /* Message length in bits      */

                               /* Index into message block array   */
    int_least16_t Message_Block_Index;
    uint8_t Message_Block[64];      /* 512-bit message blocks      */

    int32_t Computed;               /* Is the digest computed?         */
    int32_t Corrupted;             /* Is the message digest corrupted? */
} 
tsk_sha1context_t;

/*
 *  Function Prototypes
 */

TINYSAK_API tsk_sha1_errcode_t tsk_sha1reset(tsk_sha1context_t *);
TINYSAK_API tsk_sha1_errcode_t tsk_sha1input(tsk_sha1context_t *, const uint8_t *, unsigned int32_t);
TINYSAK_API tsk_sha1_errcode_t tsk_sha1result(tsk_sha1context_t *, tsk_sha1digest_t Message_Digest);
TINYSAK_API void tsk_sha1final(uint8_t *Message_Digest, tsk_sha1context_t *context);
TINYSAK_API tsk_sha1_errcode_t tsk_sha1compute(const char* input, size_t size, tsk_sha1string_t *result);

TSK_END_DECLS

#endif /* _TINYSAK_SHA1_H_ */
