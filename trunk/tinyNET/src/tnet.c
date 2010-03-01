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
* GNU General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

/**@file tnet.c
 * @brief Network stack.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tnet.h"
#include "tnet_utils.h" 

#include "tsk_debug.h"

/** @mainpage TinyNET API Overview
*
* This file is an overview of TinyNET API.
*
* <b>tinyNET</b>(a.k.a tiny <b>NET</b>working or tiny <b>NET</b>work) is one of the most important API provided by <a href="doubango.org">doubango</a>.<br>
* 
* It implements protocol agnostic sockets (@ref tnet_socket_group) used by higher Layers (SIP, XCAP, MSRP, DNS, DHCP ...) to send and receive data.<br>
*  
*
* @par Highlights
* 
* - <b>STUN2</b>: Session Traversal Utilities for NAT (STUN) is a protocol that serves
*   as a tool for other protocols in dealing with Network Address Translator (NAT) traversal. <br>
*   It can be used by an endpoint to determine the IP address and port allocated to it by a NAT.  <br>
*   It can also be used to check connectivity between two endpoints, and as a
*   keep-alive protocol to maintain NAT bindings.  STUN works with many existing NATs, and does not require any special behavior from them.<br>
*	<b>tinyNET</b> fully support STUN2(RFC 5389) and partialy implement STUN1 (RFC 3489) which is obsolete.
* 
* - <b>TURN</b>: 
* - <b>ICE</b>: 
* - <b>DHCPv4</b>: 
* - <b>DHCPv6</b>: 
* - <b>DNS</b>: A, AAAA, CNAME, MX, NAPTR, NS, OPT, PTR, SOA, SRV and TXT DNS Resource Records.
*
* @par Getting Started
*
* - @ref tnet_socket_group
* - @ref tnet_utils_group
* - @ref tnet_dns_group
* - @ref tnet_dhcp_group
* - @ref tnet_dhcp6_group
* - @ref tnet_nat_group (@ref tnet_stun_group, @ref tnet_turn_group and ICE)
* 
* @par IMPORTANT
*
* In order to use <b>tnet_*</b> functions the stack MUST be initialized by calling @ref tnet_startup.<br>
* @ref tnet_cleanup is used to uninitialize the stack.
*/
static int __tnet_started = 0;

/**
 * This is probably the most important function. You MUST call this function to initialize the network stack before calling any <b>tnet_*</b> function. 
 *			You MUST call @ref tnet_cleanup to cleanup the network stack.
 *
 * @sa @ref tnet_cleanup.
 * @return	Zero if succeed and error code otherwise. 
**/
int tnet_startup()
{
	int err = 0;
	if(__tnet_started) goto bail;

#if TNET_UNDER_WINDOWS
	{
		WORD wVersionRequested;
		WSADATA wsaData;

		wVersionRequested = MAKEWORD(2, 2);

		err = WSAStartup(wVersionRequested, &wsaData);
		if (err != 0) 
		{
			TSK_DEBUG_FATAL("WSAStartup failed with error: %d\n", err);
			return 1;
		}

		if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
		{
			TSK_DEBUG_FATAL("Could not find a usable version of Winsock.dll\n");
			tnet_cleanup();
			return 1;
		}
		else
		{
			__tnet_started = 1;
			TSK_DEBUG_INFO("The Winsock 2.2 dll was found okay\n");
		}
	}
#else
	__tnet_started = 1;
#endif /* TSIP_UNDER_WINDOWS */
	
bail:
	return err;
}


/**
 * Cleanup the network stack.
 *
 * @sa @ref tnet_startup.
 * @retval	Zero if succeed and non-zero error code otherwise.
**/
int tnet_cleanup()
{
	if(!__tnet_started) goto bail;

#if TNET_UNDER_WINDOWS
	__tnet_started = 0;
	return WSACleanup();
#else
	__tnet_started = 0;
#endif

bail:
	return 0;
}

