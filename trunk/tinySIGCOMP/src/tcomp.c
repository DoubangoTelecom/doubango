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

/**@file tcomp.c
 * @brief SIGCOMP (RFC 3320) Implementation for 2.5G and 3G cellular networks.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tcomp.h"

/** @mainpage TinySIGCOMP API Overview
*
* This file is an overview of TinySIGCOMP API.
*
* TinySIGCOMP is a tiny but fully featured SIGCOMP implementation for 2.5G, 3G and 4G cellular networks. This library is also used in Doubango project to provide SIGCOMP
* support for 3GPP IMS and OMA networks.
* This API is designed to efficiently work on embedded systems whith limited memory and low computing power.
*
* As many operators have begun to commercially deploy IMS, the relevance of using SigComp to lower bandwidth usage will come quickly. 
* In my own opinion I think that most operators (especially those using RCS) will question how to reduce SIP signaling (registration, billing, presence, messaging …) 
* bandwidth usage (who will pay bits?). 
* These questions will especially concern using SIP (or all other text-based protocols) in wireless handsets as part of 2.5G, 3G and 4G cellular networks. 
*
* SigComp stands for Signaling Compression and has been defined in <a href="http://www.ietf.org/rfc/rfc3320.txt">RFC 3320</a> by the Internet Engineering Task Force (IETF) ROHC working group.
*
* @image html SigComp_Architecture.png "SigComp Architecture (From wikimedia)"
*
* Many application protocols used for multimedia communications are text-based and engineered for bandwidth rich links. As a result the messages have not been optimized in 
* terms of size. For example, typical IMS/SIP messages range from a few hundred bytes up to two thousand bytes or more. For this reason, SIGCOMP is mandatory for 
* 3GPP IMS netwoks and <a href="http://en.wikipedia.org/wiki/Push_to_Talk_over_Cellular">PoC systems</a>. 
*
* SIGCOMP could also be useful for RCS (Rich Communication Suite) networks because of the size of the SIP packets (more than three thousand bytes for presence publication). 
* Using SIGCOMP in IMS/RCS context will reduce the round-trip over slow radio links.
*
* @par Supported OS
*
* Windows Xp/Vista (Visual Studio 2005 or Mingw32) 
* Windows Mobile 5 and later (Visual Studio 2005 or Mingw32ce/cegcc toolchain) 
* Symbian S60 (Carbide.c++ v2.0 with S60_3rd_FP2_SDK_v1.1)
* Google Android
* All Linux, FreeBSD, ... (GCC 4.x) 
*
*
* @par FEATURES
*
* The goal of this project is to provide a SigComp framework which: 
*
* - Could be used as an external API or Framework
* - Highly portable (Coded in C/C++ without any external dependencies)
* - Easily configurable (memory usage, priorities in static dictionaries, stateful/stateless modes, dynamic/static/shared compression types …)
* - Easy to integrate with any existing SIP/IMS stack, Proxy-CSCF, PoC client …
* - Allow to easily plug your own compressor (DEFLATE – RFC 1951- will be the default)
* -
* - Robust
* - Efficiently run on mobile handsets (small footprint)
* - Use small memory (UDVM decompression)
* - Run fast without high CPU usage
* - Supports both TCP and UDP compression modes
* - Thread-safe
*
* @par COMPLIANCE
*
* - <a href="http://www.ietf.org/rfc/rfc3320.txt">RFC 3320</a>: Signaling Compression (SigComp)
* - <a href="http://www.ietf.org/rfc/rfc3321.txt">RFC 3321</a>: Signaling Compression (SigComp) - Extended Operations
* - <a href="http://www.ietf.org/rfc/rfc3485.txt">RFC 3485</a>: The Session Initiation Protocol (SIP) and Session Description Protocol (SDP) Static Dictionary for Signaling Compression (SigComp)
* - <a href="http://www.ietf.org/rfc/rfc4077.txt">RFC 4077</a>: A Negative Acknowledgement Mechanism for Signaling Compression
* - <a href="http://www.ietf.org/rfc/rfc4464.txt">RFC 4464</a>: Signaling Compression (SigComp) Users' Guide
* - <a href="http://www.ietf.org/rfc/rfc4465.txt">RFC 4465</a>: Signaling Compression (SigComp) Torture Tests
* - <a href="http://www.ietf.org/rfc/rfc4896.txt">RFC 4896</a>: Signaling Compression (SigComp) Corrections and Clarifications
* - <a href="http://www.ietf.org/rfc/rfc5049.txt">RFC 5049</a>: Applying Signaling Compression (SigComp) to the Session Initiation Protocol (SIP)
* - <a href="http://www.ietf.org/rfc/rfc5112.txt">RFC 5112</a>: The Presence-Specific Static Dictionary for Signaling Compression (Sigcomp)
* - <a href="http://www.ietf.org/rfc/rfc1662.txt">RFC 1662</a>: PPP in HDLC-like Framing
* - <a href="http://www.ietf.org/rfc/rfc1951.txt">RFC 1951</a>: DEFLATE Compressed Data Format Specification version
* - <a href="http://www.ietf.org/rfc/rfc3174.txt">RFC 3174</a>: US Secure Hash Algorithm 1 (SHA1)
* - 3GPP TR23.979 Annex C: Required SigComp performance
*
* @par Getting Started
*
*/

/**@defgroup tcomp_group SIGCOMP
*/