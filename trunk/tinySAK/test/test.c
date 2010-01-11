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

#if defined(WIN32) || defined(_WIN32) || defined(_WIN32_WCE)
#	define strdup _strdup
#endif


#define LOOP						1

#define RUN_TEST_ALL				0
#define RUN_TEST_LISTS				0
#define RUN_TEST_HEAP				0
#define RUN_TEST_STRINGS			0
#define RUN_TEST_URL				0
#define RUN_TEST_MUTEX				0
#define RUN_TEST_CONDWAIT			0
#define RUN_TEST_SEMAPHORE			0
#define RUN_TEST_SAFEOBJECT			0
#define RUN_TEST_OBJECT				0
#define RUN_TEST_PARAMS				0
#define RUN_TEST_TIMER				0
#define RUN_TEST_RUNNABLE			1
#define RUN_TEST_BUFFER				0
#define RUN_TEST_MD5				0
#define RUN_TEST_SHA1				0
#define RUN_TEST_BASE64				0

#if RUN_TEST_LISTS || RUN_TEST_ALL
#include "test_lists.h"
#endif

#if RUN_TEST_HEAP || RUN_TEST_ALL
#include "test_heap.h"
#endif

#if RUN_TEST_STRINGS || RUN_TEST_ALL
#include "test_strings.h"
#endif

#if RUN_TEST_URL || RUN_TEST_ALL
#include "test_url.h"
#endif

#if RUN_TEST_MUTEX || RUN_TEST_ALL
#include "test_mutex.h"
#endif

#if RUN_TEST_CONDWAIT || RUN_TEST_ALL
#include "test_condwait.h"
#endif

#if RUN_TEST_SEMAPHORE || RUN_TEST_ALL
#include "test_semaphore.h"
#endif

#if RUN_TEST_SAFEOBJECT || RUN_TEST_ALL
//#include "test_safeobject.h"
#endif

#if RUN_TEST_OBJECT || RUN_TEST_ALL
#include "test_object.h"
#endif

#if RUN_TEST_PARAMS || RUN_TEST_ALL
#include "test_params.h"
#endif

#if RUN_TEST_TIMER || RUN_TEST_ALL
#include "test_timer.h"
#endif

#if RUN_TEST_RUNNABLE || RUN_TEST_ALL
#include "test_runnable.h"
#endif

#if RUN_TEST_BUFFER || RUN_TEST_ALL
#include "test_buffer.h"
#endif

#if RUN_TEST_MD5 || RUN_TEST_ALL
#include "test_md5.h"
#endif

#if RUN_TEST_SHA1 || RUN_TEST_ALL
#include "test_sha1.h"
#endif

#if RUN_TEST_BASE64 || RUN_TEST_ALL
#include "test_base64.h"
#endif




#ifdef _WIN32_WCE
int _tmain(int argc, _TCHAR* argv[])
#else
int main()
#endif
{
#if LOOP
	while(1)
#endif
	{
		/* Print copyright information */
		printf("Doubango Project\nCopyright (C) 2009 Mamadou Diop \n\n");

#if RUN_TEST_LISTS || RUN_TEST_ALL
		/* linked lists */
		test_basic_list();
		printf("\n\n");
		test_complex_list();
		printf("\n\n");
		test_filtered_list();
		printf("\n\n");
#endif

#if RUN_TEST_HEAP || RUN_TEST_ALL
		/* heap */
		test_heap();
		printf("\n\n");
#endif

#if RUN_TEST_STRINGS || RUN_TEST_ALL
		/* strings */
		test_strings();
		printf("\n\n");
#endif

#if RUN_TEST_URL || RUN_TEST_ALL
		/* url */
		test_url();
		printf("\n\n");
#endif

#if RUN_TEST_MUTEX || RUN_TEST_ALL
		/* mutex */
		test_mutex();
		printf("\n\n");
#endif

#if RUN_TEST_CONDWAIT || RUN_TEST_ALL
		/* condwait */
		test_condwait();
		printf("\n\n");
#endif

#if RUN_TEST_SEMAPHORE || RUN_TEST_ALL
		/* semaphore */
		test_semaphore();
		printf("\n\n");
#endif

#if RUN_TEST_SAFEOBJECT || RUN_TEST_ALL
	/* safe object */
		//test_safeobject();
		printf("\n\n");
#endif

#if RUN_TEST_OBJECT || RUN_TEST_ALL
	/* object */
		test_object();
		printf("\n\n");
#endif

#if RUN_TEST_PARAMS || RUN_TEST_ALL
		/* parameters */
		test_params();
		printf("\n\n");
#endif

#if RUN_TEST_TIMER || RUN_TEST_ALL
		/* timer */
		test_timer();
		printf("\n\n");
#endif
		
#if RUN_TEST_RUNNABLE || RUN_TEST_ALL
		/* test runnable. */
		test_runnable();
		printf("\n\n");
#endif


#if RUN_TEST_BUFFER || RUN_TEST_ALL
		/* test buffer */
		test_buffer();
#endif

#if RUN_TEST_MD5 || RUN_TEST_ALL
		/* test md5 and hmac_md5 */
		test_md5();
		test_hmac_md5();
#endif

#if RUN_TEST_SHA1 || RUN_TEST_ALL
		/* test sha1 and hmac_sha-1 */
		test_sha1();
		test_hmac_sha1();
#endif

#if RUN_TEST_BASE64 || RUN_TEST_ALL
		/* test base64 encoding/decoding */
		test_base64();
#endif
	}

	getchar();

	return 0;
}
