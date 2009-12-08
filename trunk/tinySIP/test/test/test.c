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

#include "tinysip/tsip_message.h"
#include "tinysip/headers/tsip_headers.h"
#include "tinysip/parsers/tsip_parser_message.h"

#include "test_sipmessages.h"
#include "test_uri.h" /*SIP/SIPS/TEL*/


#define RUN_TEST_LOOP		1

#define RUN_TEST_ALL		0
#define RUN_TEST_MESSAGES	1
#define RUN_TEST_URI		0

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
	
#if RUN_TEST_ALL  || RUN_TEST_MESSAGES
		test_messages();
#endif

#if RUN_TEST_ALL  || RUN_TEST_URI
		test_uri();
#endif

	}

	return 0;
}

