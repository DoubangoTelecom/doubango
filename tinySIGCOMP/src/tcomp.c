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

/**@file tcomp.c
 * @brief SigComp (RFC 3320) Implementation for 2.5G and 3G cellular networks.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *

 */
#include "tcomp.h"

/** @mainpage tinySigComp API Overview
*
* This file is an overview of <b>tinySigComp</b> API.
*
* <b>tinySigComp</b> is a tiny but fully featured SigComp implementation for 2.5G, 3G and 4G cellular networks. This library is also used in Doubango project to provide SigComp
* support for 3GPP IMS and OMA networks.
* This API is designed to efficiently work on embedded systems whith limited memory and low computing power.
*
* As many operators have begun to commercially deploy IMS, the relevance of using SigComp to lower bandwidth usage will come quickly.
* In my own opinion I think that most operators (especially those using RCS) will question how to reduce SIP signaling (registration, billing, presence, messaging …)
* bandwidth usage (who will pay bits?).
* These questions will especially concern using SIP (or all other text-based protocols) in wireless handsets as part of 2.5G, 3G and 4G cellular networks.
*
* SigComp stands for Signaling Compression and has been defined in <a href="http://www.ietf.org/rfc/rfc3320.txt">RFC 3320</a> by the Internet Engineering Task Force (IETF) ROHC working group.
* <br> <br>
*
* @image html SigComp_Architecture.png "SigComp Architecture"
*
* Many application protocols used for multimedia communications are text-based and engineered for bandwidth rich links. As a result the messages have not been optimized in
* terms of size. For example, typical IMS/SIP messages range from a few hundred bytes up to two thousand bytes or more. For this reason, SigComp is mandatory for
* 3GPP IMS netwoks and <a href="http://en.wikipedia.org/wiki/Push_to_Talk_over_Cellular">PoC systems</a>.
*
* SigComp could also be useful for RCS (Rich Communication Suite) networks because of the size of the SIP packets (more than three thousand bytes for presence publication).
* Using SigComp in IMS/RCS context will reduce the round-trip over slow radio links.
*
* @par Supported OS
*
* - Windows XX/Vista (Visual Studio 2005/2008 or Mingw32)
* - Windows Mobile 5 and later (Visual Studio 2005/2008 or Mingw32ce/cegcc toolchain)
* - Symbian S60 (Carbide.c++ v2.0 with S60_3rd_FP2_SDK_v1.1)
* - Google Android
* - Mac OS X, iPhone (Xcode)
* - All Linux, FreeBSD, ... (GCC 4.x)
*
*
* @par FEATURES
*
* The goal of this project is to provide a SigComp framework which:
*
* - Could be used as an external API or Framework
* - Highly portable (Coded in ANSI-C without any external dependencies)
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
* @par Getting started
*
* - @ref tcomp_udp_compression_page
* - @ref tcomp_udp_decompression_page
*/

/** @page tcomp_udp_compression_page SigComp UDP compression
* It is easy to compress SIP a message and send it over UDP connection. The compression can be safely done in multithreaded appilaction because
* <a href ="http://doubango.org/API/tinySigComp/">tinySigComp</a> is thread-safe.
* You need <a href ="http://doubango.org/API/tinySAK/">tinySAK</a> in order to compile the code below.
*
* Include header files:
* @code
* #include "tsk_debug.h" // tinySAK debugging functions.
* #include "tcomp_manager.h" // tinySigComp API functions.
* @endcode
*
* Compartment Identifier: Used in SIP messages (sigomp-id) and tinySigComp to allocate/deallocate memory associated
* to a compartment.
* @code
* #define COMPARTMENT_ID		"urn:uuid:2e5fdc76-00be-4314-8202-1116fa82a475"
* @endcode
*
* Preparation:
* @code
* #define MAX_BUFFER_SIZE 0xFFFF
*
* int i = 0;
* tsk_size_t outLen = 0;
* tcomp_result_t *result = 0;
* char outputBuffer[MAX_BUFFER_SIZE];
*
* tcomp_manager_handle_t *manager = 0;

* // Create SigComp manager
* manager = TCOMP_MANAGER_CREATE();

* // Add SIP/Presence dictionnaries (not mandatory)
* tcomp_manager_addSipSdpDictionary(manager);
* tcomp_manager_addPresenceDictionary(manager);

* // Create result object and set compartment id --> It is recomanded to use one result object per manager.
* result = TCOMP_RESULT_CREATE();
* tcomp_result_setCompartmentId(result, COMPARTMENT_ID, strlen(COMPARTMENT_ID));

* // Set user parameters (not mandatory)
* tcomp_manager_setDecompression_Memory_Size(manager, 8192);
* tcomp_manager_setCycles_Per_Bit(manager, 64);
* tcomp_manager_setState_Memory_Size(manager, 8192);
*
* @endcode
* Compress one or several messages using the code below:
* @code
* // Compress the SIP message
* outLen = tcomp_manager_compress(manager,
* COMPARTMENT_ID, strlen(COMPARTMENT_ID), // Compartment
* "REGISTER ...", strlen("REGISTER ..."), // Sip message to compress and it's size
* outputBuffer, sizeof(outputBuffer), // The ouptut buffer and it's size
* FALSE // Indicates whether to compress as stream (SCTP, TCP...) message or not
* );
*
* if(outLen){
* 		// send SigComp message over UDP connection
* 		sendto(sock, outputBuffer, outLen , 0, (SOCKADDR *)address, sizeof(address));
* 	}
* 	else{
* 		// MUST never happen.
* }
* @endcode
* To safely release all resources:
* @code
* // Close the compartment
* tcomp_manager_closeCompartment(manager, COMPARTMENT_ID, strlen(COMPARTMENT_ID));
* // Delete the result object
* TSK_OBJECT_SAFE_FREE(result);
* // Delete the manager
* TSK_OBJECT_SAFE_FREE(manager);
* @endcode
*/


/** @page tcomp_udp_decompression_page SigComp UDP decompression
* Decompression is as easy as compression (thread-safe).
*/