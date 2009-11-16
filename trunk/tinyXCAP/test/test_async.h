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

typedef const char* URLS_T[4]; 

static URLS_T urls = 
{
	"http://192.168.0.15:8080/xcap-caps/global/index",
	"http://192.168.0.15:8080/xcap-caps/global/index",
	"http://192.168.0.15:8080/xcap-caps/global/index",
	"http://192.168.0.15:8080/xcap-caps/global/index"
};

/* Testing asynchronous call */
int test_async()
{
	txc_request_t* request = 0;
	int ret = 0, error = 0, i;
	txc_context_t* context = 0;

	printf("---\nTEST_ASYNC\n---\n");

	TXC_CONTEXT_CREATE(context);
	TEST_XXXX_CONTEXT_INIT(context);

	for(i=0; i<100; i++)
	{
		TXC_REQUEST_CREATE(request);

		/*== get xcap-caps document an update avalable auids ==*/
		request->url = TXC_DOC_GET_SEL(context, ietf_xcap_caps);
		request->auid = tsk_strdup2(context->auids[ietf_xcap_caps].name);
		
		if((error = txc_xcap_send(context, request, op_fetch, sl_document)))
		{
			printf("test_async/// failed:%d status:%d\n", error, request->status);
			break;
		}
	}

	getchar();

	TXC_CONTEXT_SAFE_FREE(context);

	return ret;
}

#endif