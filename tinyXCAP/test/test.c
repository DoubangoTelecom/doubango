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
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/
#include "stdafx.h"

#include "tinyxcap.h"

#define LOOP						1

#define RUN_TEST_ALL				0
#define RUN_TEST_SELECTOR			0
#define RUN_TEST_STACK				1


#define XUI "sip:mamadou@micromethod.com"
#define PASSWORD "mysecret"
#define XCAP_ROOT "http://192.168.0.10:8080/services"


#if RUN_TEST_SELECTOR || RUN_TEST_ALL
#include "test_selector.h"
#endif

#if RUN_TEST_STACK || RUN_TEST_ALL
#include "test_stack.h"
#endif


#ifdef _WIN32_WCE
int _tmain(int argc, _TCHAR* argv[])
#else
int main()
#endif
{
    tnet_startup();

    /* Print copyright information */
    printf("Doubango Project\nCopyright (C) 2009 Mamadou Diop \n\n");

#if LOOP
    while(1)
#endif
    {
        /* XCAP document/node selector */
#if RUN_TEST_SELECTOR || RUN_TEST_ALL
        test_selector();
#endif

        /* XCAP stack */
#if RUN_TEST_STACK || RUN_TEST_ALL
        test_stack();
#endif


    }

    tnet_cleanup();

    return 0;
}

