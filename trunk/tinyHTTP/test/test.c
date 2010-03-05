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
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "tsk.h"

#include "thttp.h"
#include "tinyHTTP/auth/thttp_auth.h"

#define LOOP						1

#define RUN_TEST_ALL				0
#define RUN_TEST_AUTH				0
#define RUN_TEST_STACK				1

#include "test_auth.h"
#include "test_stack.h"


#ifdef _WIN32_WCE
int _tmain(int argc, _TCHAR* argv[])
#else
int main()
#endif
{
		thttp_global_init();

#if LOOP
	for(;;)
#endif
	{
		/* Print copyright information */
		printf("Doubango Project\nCopyright (C) 2009-2010 Mamadou Diop \n\n");

#if RUN_TEST_AUTH || RUN_TEST_ALL
		test_basic_auth();
		test_digest_auth();
#endif

#if RUN_TEST_STACK || RUN_TEST_ALL
		test_stack();
#endif

	}

	thttp_global_deinit();

	return 0;
}

