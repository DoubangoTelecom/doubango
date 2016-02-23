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

/**@file tsk_ppfcs32.h
 * @brief PPP in HDLC-like Framing (RFC 1662).
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef _TINYSAK_PPFCS32_H_
#define _TINYSAK_PPFCS32_H_

#include "tinysak_config.h"

TSK_BEGIN_DECLS

#define TSK_PPPINITFCS32  0xffffffff   /* Initial FCS value */
#define TSK_PPPGOODFCS32  0xdebb20e3   /* Good final FCS value */

TINYSAK_API uint32_t tsk_pppfcs32(register uint32_t fcs, register const uint8_t* cp, register int32_t len);

TSK_END_DECLS

#endif /* _TINYSAK_PPFCS32_H_ */

