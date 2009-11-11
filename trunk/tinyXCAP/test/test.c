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

#define LOOP				0

#define RUN_TEST_ALL		0
#define RUN_TEST_SELECTOR	0
#define RUN_TEST_XCAP_CAPS	0
#define RUN_TEST_RLIST		0
#define RUN_TEST_RLS		1

#if RUN_TEST_SELECTOR || RUN_TEST_ALL
#include "test_selector.h"
#endif

#if RUN_TEST_XCAP_CAPS || RUN_TEST_ALL
#include "test_xcap-caps.h"
#endif

#if RUN_TEST_RLIST || RUN_TEST_ALL
#include "test_rlist.h"
#endif

#if RUN_TEST_RLS || RUN_TEST_ALL
#include "test_rls.h"
#endif

int _tmain(int argc, _TCHAR* argv[])
{
#if LOOP
	while(1)
#endif
	{
		/* Print copyright information */
		printf("Doubango Project\nCopyright (C) 2009 Mamadou Diop \n\n");

		/* XCAP document/node selector */
#if RUN_TEST_SELECTOR || RUN_TEST_ALL
		test_selector();
#endif

		/* XCAP-CAPS parsing */
#if RUN_TEST_XCAP_CAPS || RUN_TEST_ALL
		test_xcap_caps(xcap_caps_str0, strlen(xcap_caps_str0));
#endif

		/* resource-lists */
#if RUN_TEST_RLIST || RUN_TEST_ALL
		test_rlist(rlist_str0, strlen(rlist_str0));
		test_rlist2(rlist_str0, strlen(rlist_str0));
#endif

		/* rls-services */
#if RUN_TEST_RLS || RUN_TEST_ALL
		test_rls_services(rls_str0, strlen(rls_str0));
#endif

	}
	getchar();

	return 0;
}

