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
#include "stdafx.h"

#include "tsk.h"

#include "tnet.h"
#include "tnet_socket.h"
#include "tnet_transport.h"

#include "test_sockets.h"
#include "test_transport.h"



#define RUN_TEST_LOOP		1

#define RUN_TEST_ALL		0
#define RUN_TEST_SOCKETS	0
#define RUN_TEST_TRANSPORT	1

#ifdef _WIN32_WCE
int _tmain(int argc, _TCHAR* argv[])
#else
int main()
#endif
{
	/* Startup the network stack. */
	if(tnet_startup())
	{
		return -1;
	}

#if RUN_TEST_LOOP
	for(;;)
#endif
	{
	
#if RUN_TEST_ALL  || RUN_TEST_SOCKETS
		test_sockets();
#endif

#if RUN_TEST_ALL  || RUN_TEST_TRANSPORT
		test_transport();
#endif		
	}	

	/* Cleanup the network stack */
	tnet_cleanup();

	return 0;
}

