/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)yahoo.fr>
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

#include "tnet.h"

#include "tsk.h"

#include "tinymsrp.h"


#include "test_parser.h"
#include "test_uri.h"
//#include "test_session.h"


#define RUN_TEST_LOOP		1

#define RUN_TEST_ALL		0
#define RUN_TEST_URI		0
#define RUN_TEST_PARSER		1
#define RUN_TEST_SESSION	0

#ifdef _WIN32_WCE
int _tmain(int argc, _TCHAR* argv[])
#else
int main()
#endif
{
#if RUN_TEST_LOOP
    for(;;)
#endif
    {
        tnet_startup();

        /* Print copyright information */
        printf("Doubango Project\nCopyright (C) 2009 - 2010 Mamadou Diop \n\n");

#if RUN_TEST_ALL  || RUN_TEST_URI
        test_uri();
#endif

#if RUN_TEST_ALL  || RUN_TEST_PARSER
        test_parser();
#endif

#if RUN_TEST_ALL  || RUN_TEST_SESSION
        test_session();
#endif

        tnet_cleanup();
    }
}