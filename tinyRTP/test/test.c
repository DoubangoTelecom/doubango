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
#include <string.h>
#include <stdlib.h>

#include "tinyrtp.h"

#define LOOP						1

#define RUN_TEST_ALL				0
#define RUN_TEST_PARSER				0
#define RUN_TEST_MANAGER			1

#include "test_parser.h"
#include "test_manager.h"



#ifdef _WIN32_WCE
int _tmain(int argc, _TCHAR* argv[])
#else
int main()
#endif
{
    tnet_startup();

    do {
        /* Print copyright information */
        printf("Doubango Project\nCopyright (C) 2009-2010 Mamadou Diop \n\n");

#if RUN_TEST_PARSER || RUN_TEST_ALL
        test_parser();
#endif

#if RUN_TEST_MANAGER || RUN_TEST_ALL
        test_manager();
#endif

    }
    while(LOOP);

    tnet_cleanup();

    return 0;
}

