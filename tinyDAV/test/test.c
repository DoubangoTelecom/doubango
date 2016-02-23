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

#include "tinydav.h"

#include "test_sessions.h"

#define LOOP						0

#define RUN_TEST_ALL				0
#define RUN_TEST_SESSIONS			1

// Codecs : http://www.itu.int/rec/T-REC-G.191-200509-S/en

#ifdef _WIN32_WCE
int _tmain(int argc, _TCHAR* argv[])
#else
int main()
#endif
{
    tnet_startup();
    tdav_init();

    do {
        /* Print copyright information */
        printf("Doubango Project\nCopyright (C) 2009-2010 Mamadou Diop \n\n");

#if RUN_TEST_SESSIONS || RUN_TEST_ALL
        test_sessions();
#endif

    }
    while(LOOP);

    tdav_deinit();
    tnet_cleanup();

    return 0;
}

