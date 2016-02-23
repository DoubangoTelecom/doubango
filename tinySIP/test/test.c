/*
* Copyright (C) 2009 Mamadou Diop.
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

#include "stdafx.h"

#include "tinysip.h"

#include "test_sipmessages.h"
#include "test_uri.h" /*SIP/SIPS/TEL*/
#include "test_transac.h"
#include "test_stack.h"
#include "test_imsaka.h"


#define RUN_TEST_LOOP		1

#define RUN_TEST_ALL		0
#define RUN_TEST_MESSAGES	1
#define RUN_TEST_URI		0
#define RUN_TEST_TRANSAC	0
#define RUN_TEST_STACK		0
#define RUN_TEST_IMS_AKA	0

#ifdef _WIN32_WCE
int _tmain(int argc, _TCHAR* argv[])
#else
int main()
#endif
{
    tnet_startup();

#if RUN_TEST_LOOP
    for(;;)
#endif
    {
        /* Print copyright information */
        printf("Doubango Project\nCopyright (C) 2009 - 2010 Mamadou Diop \n\n");



#if RUN_TEST_ALL  || RUN_TEST_MESSAGES
        test_messages();
#endif

#if RUN_TEST_ALL  || RUN_TEST_URI
        test_uri();
#endif

#if RUN_TEST_ALL ||RUN_TEST_TRANSAC
        test_transac();
#endif

#if RUN_TEST_ALL || RUN_TEST_STACK
        test_stack();
#endif

#if RUN_TEST_ALL || RUN_TEST_IMS_AKA
        test_imsaka();
#endif
    }

    tnet_cleanup();

    return 0;
}

