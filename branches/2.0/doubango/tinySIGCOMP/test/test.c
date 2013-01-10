/*
* Copyright (C) 2009 Mamadou Diop.
* Copyright (C) 2012 Doubango Telecom <http://doubango.org>.
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
#include "tsk_string.h"
#include "tsk_debug.h"

#include "tcomp_manager.h" /* TinySIGCOMP API functions. */

#define STREAM_ID					1983
#define IS_STREAM			0	// Using reliable transport
#define MAX_BUFFER_SIZE		0xfff0

#include "test_manager.h"
#include "test_osc.h"
#include "test_tortures.h"

#define TEST_TORTURES	1
#define TEST_MANAGER	0
#define TEST_OSC		0

#ifdef _WIN32_WCE
int _tmain(int argc, _TCHAR* argv[])
#else
int main()
#endif
{
#if TEST_TORTURES
	test_tortures();
#endif

#if TEST_MANAGER
	test_manager();
#endif

#if TEST_OSC
	test_osc();
#endif
	
	getchar();

	return 0;
}
