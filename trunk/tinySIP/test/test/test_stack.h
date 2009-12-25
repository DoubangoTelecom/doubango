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
#ifndef _TEST_STACK_H
#define _TEST_STACK_H

int test_stack_callback(tsip_event_t *sipevent)
{
	tsk_object_ref(sipevent);
	
	TSK_DEBUG_INFO("STACK event: %d [%s]", sipevent->status_code, sipevent->reason_phrase);

	tsk_object_unref(sipevent);

	return 0;
}

void test_stack()
{
	tsip_stack_handle_t *stack = tsip_stack_create(test_stack_callback, 
		TSIP_STACK_SET_DISPLAY_NAME("mamadou"),
		TSIP_STACK_SET_PUBLIC_IDENTITY("sip:mamadou@micromethod.com"),
		TSIP_STACK_SET_PRIVATE_IDENTITY("mamadou"),
		TSIP_STACK_SET_REALM("sip:micromethod.com"),
		TSIP_STACK_SET_PROXY_CSCF("192.168.0.15"),
		TSIP_STACK_SET_PROXY_CSCF_PORT(5060),
		TSIP_STACK_SET_NETINFO("ADSL;utran-cell-id-3gpp=00000000"),

		TSIP_STACK_SET_NULL());


	tsip_operation_handle_t *op = TSIP_OPERATION_CREATE(stack,
		TSIP_OPERATION_SET_PARAM("expires", "600000"),

		TSIP_OPERATION_SET_NULL());

	tsip_stack_start(stack);

	tsip_stack_register(stack, op);
	tsk_thread_sleep(200000000);

	tsip_stack_stop(stack);

	TSIP_OPERATION_SAFE_FREE(op);
	tsip_stack_destroy(stack);
}


#endif /* _TEST_STACK_H */
