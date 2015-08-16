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

/**@file tsip_milenage.h
 * @brief 3GPP authentication and key agreement functions f1, f1*, f2, f3, f4, f5 and f5*.
 *
 * @section DESCRIPTION
 *
 * @sa 3G Security
 * <a href="http://www.3gpp.org/ftp/Specs/html-info/35205.htm"> 3GPP TS 35.205 </a>
 * <a href="http://www.3gpp.org/ftp/Specs/html-info/35206.htm"> 3GPP TS 35.206 </a>
 * <a href="http://www.3gpp.org/ftp/Specs/html-info/35207.htm"> 3GPP TS 35.207 </a>
 * <a href="http://www.3gpp.org/ftp/Specs/html-info/35208.htm"> 3GPP TS 35.208 </a>
 * <a href="http://www.3gpp.org/ftp/Specs/html-info/35909.htm"> 3GPP TS 35.909 </a>
 *-------------------------------------------------------------------
 *          Example algorithms f1, f1*, f2, f3, f4, f5, f5*
 *-------------------------------------------------------------------
 *
 *  A sample implementation of the example 3GPP authentication and
 *  key agreement functions f1, f1*, f2, f3, f4, f5 and f5*.  This is
 *  a byte-oriented implementation of the functions, and of the block
 *  cipher kernel function Rijndael.
 *
 *  This has been coded for clarity, not necessarily for efficiency.
 *
 *  The functions f2, f3, f4 and f5 share the same inputs and have 
 *  been coded together as a single function.  f1, f1* and f5* are
 *  all coded separately.
 *
 *-----------------------------------------------------------------
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */

#ifndef TINYSIP_AUTHENTICATION_MILENAGE_H
#define TINYSIP_AUTHENTICATION_MILENAGE_H

#include "tinysip_config.h"

/**@def AKA_AK_SIZE
 * a 48-bit anonymity key that is the output of either of the functions f5 and f5*. 
 * Defined in 3GPP TS 35.206 subclause 2.3.
 */

TSIP_BEGIN_DECLS

#define AKA_AK_SIZE 6
typedef uint8_t AKA_AK_T[AKA_AK_SIZE + 1];

/**@def AKA_AMF_SIZE
 * 16-bit authentication management field that is an input to the functions f1 and f1*.
 * Defined in 3GPP TS 35.206 subclause 2.3.
 */
#define AKA_AMF_SIZE 2
typedef uint8_t AKA_AMF_T[AKA_AMF_SIZE + 1];

/**@def AKA_CK_SIZE
 * a 128-bit confidentiality key that is the output of the function f3.
 * Defined in 3GPP TS 35.206 subclause 2.3.
 */
#define AKA_CK_SIZE 16
typedef uint8_t AKA_CK_T[AKA_CK_SIZE + 1];

/**@def AKA_IK_SIZE
 * a 128-bit integrity key that is the output of the function f4.
 * Defined in 3GPP TS 35.206 subclause 2.3.
 */
#define AKA_IK_SIZE 16
typedef uint8_t AKA_IK_T[AKA_IK_SIZE + 1];

/**@def AKA_K_SIZE
 * a 128-bit subscriber key that is an input to the functions f1, f1*, f2, f3, f4, f5 and f5*.
 * Defined in 3GPP TS 35.206 subclause 2.3.
 */
#define AKA_K_SIZE 16
typedef uint8_t AKA_K_T[AKA_K_SIZE + 1];

/**@def AKA_MAC_A_SIZE
 * a 64-bit network authentication code that is the output of the function f1. 
 * Defined in 3GPP TS 35.206 subclause 2.3.
 */
#define AKA_MAC_A_SIZE 8
typedef uint8_t AKA_MAC_A_T[AKA_MAC_A_SIZE + 1];

/**@def AKA_MAC_S_SIZE
 * a 64-bit network authentication code that is the output of the function f1*.
 * Defined in 3GPP TS 35.206 subclause 2.3.
 */
#define AKA_MAC_S_SIZE 8
typedef uint8_t AKA_MAC_S_T[AKA_MAC_S_SIZE + 1];

/**@def AKA_OP_SIZE
 * a 128-bit Operator Variant Algorithm Configuration Field that is a component of the functions f1, f1*, f2, f3, f4, f5 and f5*.
 * Defined in 3GPP TS 35.206 subclause 2.3.
 */
#define AKA_OP_SIZE 16
typedef uint8_t AKA_OP_T[AKA_OP_SIZE + 1];

/**@def AKA_OPC_SIZE
 * a 128-bit value derived from OP and K and used within the computation of the functions.
 * Defined in 3GPP TS 35.206 subclause 2.3.
 */
#define AKA_OPC_SIZE 16
typedef uint8_t AKA_OPC_T[AKA_OPC_SIZE + 1];

/**@def AKA_RAND_SIZE
 * a 128-bit random challenge that is an input to the functions f1, f1*, f2, f3, f4, f5 and f5*.
 * Defined in 3GPP TS 35.206 subclause 2.3.
 */
#define AKA_RAND_SIZE 16
typedef uint8_t AKA_RAND_T[AKA_RAND_SIZE + 1];

/**@def AKA_RES_SIZE
 * a 64-bit signed response that is the output of the function f2.
 * Defined in 3GPP TS 35.206 subclause 2.3.
 */
#define AKA_RES_SIZE 8
typedef uint8_t AKA_RES_T[AKA_RES_SIZE + 1];
	
/**@def AKA_SQN_SIZE
 * a 48-bit sequence number that is an input to either of the functions f1 and f1*.  (For f1* this input is more precisely called SQNMS.)
 * Defined in 3GPP TS 35.206 subclause 2.3.
 */
#define AKA_SQN_SIZE 6
typedef uint8_t AKA_SQN_T[AKA_SQN_SIZE + 1];

/**@def AKA_TEMP_SIZE
 * a 128-bit value used within the computation of the functions.
 * Defined in 3GPP TS 35.206 subclause 2.3.
 */
#define AKA_TEMP_SIZE 16
typedef uint8_t AKA_TEMP_T[AKA_TEMP_SIZE + 1];

/**@def AKA_AUTS_SIZE
* Authentication Token.  A 112 bit value generated by the client
      upon experiencing an SQN synchronization failure.
* Defined in RFC 3310 subclause 3.4
*/
#define AKA_AUTS_SIZE 14
typedef uint8_t AKA_AUTS_T[AKA_AUTS_SIZE + 1];

/**@def AKA_AUTN_SIZE
* Authentication Token.  A 128 bit value generated by the AuC, which
      together with the RAND parameter authenticates the server to the client.
* Defined in RFC 3310 subclause 3.4
*/
#define AKA_AUTN_SIZE 16
typedef uint8_t AKA_AUTN_T[AKA_AUTN_SIZE + 1];

#define AKA_XXX_DECLARE(name) AKA_##name##_T name
#define AKA_XXX_BZERO(name)	memset(name, '\0', AKA_##name##_SIZE + 1)

void f1    ( uint8_t k[16], uint8_t rand[16], uint8_t sqn[6], uint8_t amf[2],
             uint8_t mac_a[8] );
void f2345 ( uint8_t k[16], uint8_t rand[16],
             uint8_t res[8], uint8_t ck[16], uint8_t ik[16], uint8_t ak[6] );
void f1star( uint8_t k[16], uint8_t rand[16], uint8_t sqn[6], uint8_t amf[2], 
             uint8_t mac_s[8] );
void f5star( uint8_t k[16], uint8_t rand[16],
             uint8_t ak[6] );
void ComputeOPc( uint8_t op_c[16] );
void ComputeOP( uint8_t op[16] );


TSIP_END_DECLS

#endif /* TINYSIP_AUTHENTICATION_MILENAGE_H */

