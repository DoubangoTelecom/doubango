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
* GNU General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

#include "stdafx.h"

#include "tsk.h"

#include "tsip.h"
#include "tinysip/parsers/tsip_parser_uri.h"
#include "tinysip/tsip_operation.h"
#include "tinysip/tsip_message.h"
#include "tinysip/headers/tsip_headers.h"
#include "tinysip/parsers/tsip_parser_message.h"

#include "tinysip/transactions/tsip_transac_nict.h"

#include "test_sipmessages.h"
#include "test_uri.h" /*SIP/SIPS/TEL*/
#include "test_transac.h"
#include "test_stack.h"


#define RUN_TEST_LOOP		1

#define RUN_TEST_ALL		0
#define RUN_TEST_MESSAGES	1
#define RUN_TEST_URI		0
#define RUN_TEST_TRANSAC	0
#define RUN_TEST_STACK		0

#ifdef _WIN32_WCE
int _tmain(int argc, _TCHAR* argv[])
#else
int main()
#endif
{
	tsip_global_init();

#if RUN_TEST_LOOP
	for(;;)
#endif
	{
		/* Print copyright information */
		printf("Doubango Project\nCopyright (C) 2009 Mamadou Diop \n\n");
	
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
	}

	tsip_global_deinit();

	return 0;
}

