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

#define TEST_XXXX_CONTEXT_INIT(context) \
	context->user_agent = tsk_strdup2("XDM-client/OMA1.1"); \
	context->txc_root = tsk_strdup2("http://192.168.0.15:8080/services"); \
	context->password = tsk_strdup2("doubango"); \
	context->xui = tsk_strdup2("sip:doubango@micromethod.com"); \
	context->pragma = tsk_strdup2("keep-alive"); \
	context->reuse_http_connection = 0;

#define LOOP						0

#define RUN_TEST_ALL				0
#define RUN_TEST_SELECTOR			0
#define RUN_TEST_XCAP_CAPS			0
#define RUN_TEST_RLIST				0
#define RUN_TEST_RLS				0
#define RUN_TEST_IETF_PRESRULES		0
#define RUN_TEST_OMA_PRESRULES		0
#define RUN_TEST_OMA_DIRECTORY		0
#define RUN_TEST_GSMA_RCS			0
#define RUN_TEST_OMA_PRESCONT		0
#define RUN_TEST_ASYNC				1
#define RUN_TEST_HTTP				0
#define RUN_TEST_XDMC				0

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

#if RUN_TEST_IETF_PRESRULES || RUN_TEST_ALL
#include "test_ietf-pres-rules.h"
#endif

#if RUN_TEST_OMA_PRESRULES || RUN_TEST_ALL
#include "test_oma-pres-rules.h"
#endif

#if RUN_TEST_OMA_DIRECTORY || RUN_TEST_ALL
#include "test_oma-directory.h"
#endif

#if RUN_TEST_GSMA_RCS || RUN_TEST_ALL
#include "test_gsma_rcs.h"
#endif

#if RUN_TEST_OMA_PRESCONT || RUN_TEST_ALL
#include "test_oma-pres-cont.h"
#endif

#if RUN_TEST_ASYNC || RUN_TEST_ALL
#include "test_async.h"
#endif

#if RUN_TEST_HTTP || RUN_TEST_ALL
#include "test_http.h"
#endif

#if RUN_TEST_XDMC || RUN_TEST_ALL
#include "test_xdmc.h"
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

		/*ietf pres-rules*/
#if RUN_TEST_IETF_PRESRULES || RUN_TEST_ALL
		test_ietf_presrules();
#endif

		/*oma pres-rules*/
#if RUN_TEST_OMA_PRESRULES || RUN_TEST_ALL
		test_oma_presrules();
#endif

		/* oma directory */
#if RUN_TEST_OMA_DIRECTORY || RUN_TEST_ALL
		test_oma_directory(omadir_str0, strlen(omadir_str0));
#endif

		/* gsma rcs */
#if RUN_TEST_GSMA_RCS || RUN_TEST_ALL
		test_gsma_rcs();
#endif

		/* oma pres-cont (avatars) */
#if RUN_TEST_OMA_PRESCONT || RUN_TEST_ALL
		test_oma_pres_cont(omaprescont_str0, strlen(omaprescont_str0));
#endif

		/* Testing asynchronous calls(HTTP) */
#if RUN_TEST_ASYNC || RUN_TEST_ALL
		test_async();
#endif

		/* HTTP tests (sending requests to XDMS) */
#if RUN_TEST_HTTP || RUN_TEST_ALL
		test_http();
#endif

		/* testing XDMC hight level API */
#if RUN_TEST_XDMC || RUN_TEST_ALL
		test_xdmc();
#endif
		
	}
	
	getchar();

	return 0;
}

