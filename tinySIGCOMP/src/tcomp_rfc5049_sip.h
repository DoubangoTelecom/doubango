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

/**@file tcomp_rfc5049_sip.h
 * @brief RFC 5049 - Applying Signaling Compression (SigComp) to the Session Initiation Protocol (SIP)
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *

 */

#ifndef _TINYSIGCOMPP_RFC5049_H_
#define _TINYSIGCOMPP_RFC5049_H_

/*****
Applying Signaling Compression (SigComp)
                to the Session Initiation Protocol (SIP)
*****/
/**  4.1.  decompression_memory_size (DMS) for SIP/SigComp*/
#define SIP_RFC5049_DECOMPRESSION_MEMORY_SIZE 8192

/** 4.2.  state_memory_size (SMS) for SIP/SigComp (per compartment) */
#define SIP_RFC5049_STATE_MEMORY_SIZE 8192

/** 4.3.  cycles_per_bit (CPB) for SIP/SigComp */
#define SIP_RFC5049_CYCLES_PER_BIT 64

/** 4.4.  SigComp_version (SV) for SIP/SigComp */
#define SIP_RFC5049_SIGCOMP_VERSION 0x02 // (at least SigComp + NACK)

// 4.5.  locally available state (LAS) for SIP/SigComp
// Minimum LAS for SIP/SigComp: the SIP/SDP static dictionary as defined
   //in [RFC3485].

#endif /* _TINYSIGCOMPP_RFC5049_H_ */
