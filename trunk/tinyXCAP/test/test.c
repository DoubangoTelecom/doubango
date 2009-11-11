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

#define RUN_TEST_ALL		0
#define RUN_TEST_SELECTOR	1

#if RUN_TEST_SELECTOR || RUN_TEST_ALL
#include "test_selector.h"
#endif


int _tmain(int argc, _TCHAR* argv[])
{
	/* Print copyright information */
	printf("Doubango Project\n Copyright (C) 2009 Mamadou Diop \n\n");

	/* XCAP selector */
#if RUN_TEST_SELECTOR || RUN_TEST_ALL
	while(1)test_selector();
#endif

	getchar();

	return 0;
}

