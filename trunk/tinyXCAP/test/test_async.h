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
#ifndef TEST_ASYNC_H
#define TEST_ASYNC_H

#include "txc_api.h"

int count = 0;

/* HTTP callback to handle requests received from the XDMS */
static void http_callback(txc_request_t *request)
{
	if(request->content)
	{
		TSK_DEBUG_INFO("%d) USER received response from the XDMS\n content:%s \n content-type:%s\n", ++count, request->content->data, request->content->type);
	}
	
	TXC_REQUEST_SAFE_FREE(request);
}

/* Testing asynchronous call */
int test_async()
{
	txc_request_t* request = 0;
	int ret = 0, error = 0, i;
	txc_context_t* context = 0;
	tsk_condwait_t *condwait;

	printf("---\nTEST_ASYNC\n---\n");

	TXC_CONTEXT_CREATE(context);
	TSK_CONDWAIT_CREATE(condwait);

	TEST_XXXX_CONTEXT_INIT(context);
	context->http_callback = http_callback;

	for(i=0; i<1; i++)
	{
		TXC_REQUEST_CREATE(request);

		/*== get xcap-caps document an update avalable auids ==*/
		request->url = TXC_DOC_GET_SEL(context, ietf_xcap_caps);
		if((error = txc_xcap_send(context, &request, op_fetch, sl_document)))
		{
			printf("test_async/// failed:%d\n", error);
			break;
		}
#if !defined (__SYMBIAN32__)
		tsk_condwait_timedwait(condwait, 50);
#endif
	}
	
	//getchar();
#if defined(_WIN32_WCE) || defined (__SYMBIAN32__)
	tsk_condwait_timedwait(condwait, 20000);
#endif

	TSK_CONDWAIT_SAFE_FREE(condwait);
	TXC_CONTEXT_SAFE_FREE(context);

	return ret;
}

#endif
