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
#ifndef _TEST_STACK_H_
#define _TEST_STACK_H_

int test_stack_callback(const thttp_event_t *httpevent)
{
	return 0;
}

void test_stack()
{
	thttp_operation_handle_t *op = 0;
	thttp_stack_handle_t* stack = thttp_stack_create(test_stack_callback, 0, 
		THTTP_STACK_SET_NULL());

	if(thttp_stack_start(stack)){
		goto bail;
	}

	op = THTTP_OPERATION_CREATE(stack,
		THTTP_OPERATION_SET_PARAM("method", "GET"),
		THTTP_OPERATION_SET_PARAM("URI", "http://www.google.com"),

		THTTP_OPERATION_SET_HEADER("User-Agent", "IM-client/OMA1.0 doubango/v0.0.0"),
		THTTP_OPERATION_SET_HEADER("X-XCAP-Asserted-Identity", "sip:mamadou@ericsson.com"),
		
		THTTP_OPERATION_SET_NULL());

	thttp_operation_perform(op);


	getchar();
bail:
	TSK_OBJECT_SAFE_FREE(stack);
}

#endif /* _TEST_STACK_H_ */


