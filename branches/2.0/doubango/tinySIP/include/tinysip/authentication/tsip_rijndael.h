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

/**@file tsip_rijndael.h
 * @brief Rijndael Implementation.
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
 *                      Rijndael Implementation
 *-------------------------------------------------------------------
 *
 *  A sample 32-bit orientated implementation of Rijndael, the
 *  suggested kernel for the example 3GPP authentication and key
 *  agreement functions.
 *
 *  This implementation draws on the description in section 5.2 of
 *  the AES proposal and also on the implementation by
 *  Dr B. R. Gladman <brg@gladman.uk.net> 9th October 2000.
 *  It uses a number of large (4k) lookup tables to implement the
 *  algorithm in an efficient manner.
 *
 *  Note: in this implementation the State is stored in four 32-bit
 *  words, one per column of the State, with the top byte of the
 *  column being the _least_ significant byte of the word.
 *
 *-----------------------------------------------------------------
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef TINYSIP_AUTHENTICATION_RIJNDAEL_H
#define TINYSIP_AUTHENTICATION_RIJNDAEL_H

#include "tinysip_config.h"

TSIP_BEGIN_DECLS

void RijndaelKeySchedule( uint8_t key[16] );
void RijndaelEncrypt( uint8_t in[16], uint8_t out[16] );

TSIP_END_DECLS

#endif /*TINYSIP_AUTHENTICATION_RIJNDAEL_H*/
