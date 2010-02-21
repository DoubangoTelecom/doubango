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
#include "tnet_nat.h"
#include "tnet_socket.h"
#include "tnet_transport.h"
#include "stun/tnet_stun.h"
#include "dns/tnet_dns.h"
#include "dhcp/tnet_dhcp.h"
#include "dhcp6/tnet_dhcp6.h"

#include "test_sockets.h"
#include "test_transport.h"
#include "test_auth.h"
#include "test_stun.h"
#include "test_nat.h"
#include "test_ifaces.h"
#include "test_dns.h"
#include "test_dhcp.h"
#include "test_dhcp6.h"

#define RUN_TEST_LOOP		1

#define RUN_TEST_ALL		0
#define RUN_TEST_SOCKETS	0 /* FIXME: Android */
#define RUN_TEST_TRANSPORT	0
#define RUN_TEST_AUTH		0
#define RUN_TEST_STUN		0
#define RUN_TEST_NAT		0
#define RUN_TEST_IFACES		1
#define RUN_TEST_DNS		0
#define RUN_TEST_DHCP		0
#define RUN_TEST_DHCP6		0

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

#if RUN_TEST_ALL || RUN_TEST_AUTH
		test_auth();
#endif

#if RUN_TEST_ALL || RUN_TEST_STUN
		test_stun();
#endif

#if RUN_TEST_ALL || RUN_TEST_NAT
		test_nat();
#endif

#if RUN_TEST_ALL || RUN_TEST_IFACES
		test_ifaces();
#endif

#if RUN_TEST_ALL || RUN_TEST_DNS
		test_dns();
#endif

#if RUN_TEST_ALL || RUN_TEST_DHCP
		test_dhcp();
#endif

#if RUN_TEST_ALL || RUN_TEST_DHCP6
		test_dhcp6();
#endif

	}	

	/* Cleanup the network stack */
	tnet_cleanup();

	return 0;
}


