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

/**@file tnet.c
 * @brief Network stack.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tnet.h"
#include "tnet_utils.h" 

#include "tsk_time.h"
#include "tsk_debug.h"

#include <stdlib.h> /* srand */

#if HAVE_OPENSSL
#	include <openssl/ssl.h>
#endif

/** @mainpage tinyNET API Overview
*
* <h1>10 Sockets and Network Functions</h1>
*
* All network functions are part of tinyNET projects.<br>
* You MUST call @ref tnet_startup() before using any network function (tnet_*). tnet_cleanup() is used to terminate use of network functions. <br>
* The startup function will determine whether the host is a little-endian machine or not (at runtime).
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
* <table>
* <tr> <td><b>LDFLAGS</b></td> <td>-ltinySAK</td> </tr>
* <tr> <td><b>CFLAGS</b></td> <td>-Isrc -ItinySAK/src</td> </tr>
* </table>
*
*/
static tsk_bool_t __tnet_started = tsk_false;
tsk_bool_t tnet_isBigEndian = tsk_false;

/**
 * @fn tnet_startup
 * This is probably the most important function. You MUST call this function to initialize the network stack before calling any <b>tnet_*</b> function. <br />
 * On windows, this function will call <a target=_blank href="http://msdn.microsoft.com/en-us/library/windows/desktop/ms742213(v=vs.85).aspx">WSAStartup</a>. <br />
 * You MUST call @ref tnet_cleanup to cleanup the network stack if you no longer need to use networking function.
 *
 * @sa @ref tnet_cleanup.
 * @return 0 if succeed and error code otherwise. 
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
			fprintf(stderr, "WSAStartup failed with error: %d\n", err);
			return -1;
		}

		if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2){
			fprintf(stderr, "Could not find a usable version of Winsock.dll\n");
			tnet_cleanup();
			return -2;
		}
		else{
			fprintf(stdout, "The Winsock 2.2 dll was found okay\n");
		}
	}
#endif /* TNET_UNDER_WINDOWS */

#if HAVE_OPENSSL
	fprintf(stdout, "SSL is enabled :)\n");
	SSL_library_init();
	OpenSSL_add_all_algorithms();
	SSL_load_error_strings();

	fprintf(stdout, "DTLS supported: %s\n", tnet_dtls_is_supported() ? "yes" : "no");
	fprintf(stdout, "DTLS-SRTP supported: %s\n", tnet_dtls_is_srtp_supported() ? "yes" : "no");
#else
	fprintf(stderr, "SSL is disabled :(\n");
#endif
	
	__tnet_started = tsk_true;

bail:
	return err;
}


/**
 * Cleanup the network stack. <br />
 * On windows, this function will call <a target=_blank href="http://msdn.microsoft.com/en-us/library/windows/desktop/ms741549(v=vs.85).aspx">WSACleanup</a>. <br />
 * @sa @ref tnet_startup.
 * @return 0 if succeed and non-zero error code otherwise.
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

