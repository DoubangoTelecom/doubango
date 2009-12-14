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

/**@file tnet.c
 * @brief Network utilities.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tnet.h"
#include "tnet_utils.h" 

#include "tsk_debug.h"

/**
 * @fn	int tnet_startup()
 *
 * @brief	This is probably the most important function. You MUST call this function to initialize the network stack before calling any tnet_* function. 
 *			You MUST call @ref tsip_cleanup to cleanup the network stack.
 * @author	Mamadou
 * @date	12/11/2009
 *
 * @sa @ref tnet_cleanup.
 * @return	Zero if succeed and error code otherwise. 
**/
int tnet_startup()
{
	int err = 0;

#if TNET_UNDER_WINDOWS

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
        TSK_DEBUG_INFO("The Winsock 2.2 dll was found okay\n");
	}
#endif /* TSIP_UNDER_WINDOWS */
	
	return err;
}


/**
 * @fn	int tnet_cleanup()
 *
 * @brief	Cleanup the network stack.
 *
 * @author	Mamadou
 * @date	12/11/2009
 *
 * @sa @ref tnet_cleanup.
 * @return	Zero if succeed and error code otherwise.
**/
int tnet_cleanup()
{
#if TNET_UNDER_WINDOWS
	return WSACleanup();
#endif /* #if TNET_UNDER_WINDOWS */

	return 0;
}