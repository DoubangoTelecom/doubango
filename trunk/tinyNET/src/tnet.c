/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango.org>
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
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tnet.h"
#include "tnet_utils.h" 

#include "tsk_time.h"
#include "tsk_debug.h"

#include <stdlib.h> /* srand */

/** @mainpage tinyNET API Overview
*
* <h1>10 Sockets and Network Functions</h1>
*
* All network functions are part of tinyNET projects.<br>
* You MUST call @ref tnet_startup() before using any network function (tnet_*). tnet_cleanup() is used to terminate use of network functions. <br>
* The startup function will determine whether the host is a ìlittle-endianî machine or not (at runtime).
*
* ======
*
* - @ref tnet_socket_group 
* - @ref tnet_utils_group
* - @ref tnet_dhcp_group
* - @ref tnet_dhcp6_group
* - @ref tnet_dns_group
* - @ref tnet_nat_group (@ref tnet_stun_group, @ref tnet_turn_group, ICE)
*
* ======
*
*/
static tsk_bool_t __tnet_started = tsk_false;
tsk_bool_t tnet_isBigEndian = tsk_false;

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
	short word = 0x4321;

	if(__tnet_started){
		goto bail;
	}

	// rand()
	srand((unsigned int) tsk_time_epoch());

	// endianness
	tnet_isBigEndian = ((*(int8_t *)&word) != 0x21);
#if TNET_UNDER_WINDOWS
	if(tnet_isBigEndian){
		TSK_DEBUG_ERROR("Big endian on Windows machine. Is it right?");
	}
#endif

#if TNET_UNDER_WINDOWS
	{
		WORD wVersionRequested;
		WSADATA wsaData;

		wVersionRequested = MAKEWORD(2, 2);

		err = WSAStartup(wVersionRequested, &wsaData);
		if (err != 0) {
			TSK_DEBUG_FATAL("WSAStartup failed with error: %d\n", err);
			return -1;
		}

		if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2){
			TSK_DEBUG_FATAL("Could not find a usable version of Winsock.dll\n");
			tnet_cleanup();
			return -2;
		}
		else{
			__tnet_started = tsk_true;
			TSK_DEBUG_INFO("The Winsock 2.2 dll was found okay\n");
		}
	}
#else
	__tnet_started = tsk_true;
#endif /* TNET_UNDER_WINDOWS */
	
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
	if(!__tnet_started){
		goto bail;
	}

#if TNET_UNDER_WINDOWS
	__tnet_started = tsk_false;
	return WSACleanup();
#else
	__tnet_started = tsk_false;
#endif

bail:
	return 0;
}

