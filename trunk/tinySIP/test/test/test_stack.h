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
	tsip_stack_handle_t *stack = tsip_stack_create(test_stack_callback, TSIP_STACK_SET_NULL());

	tsip_stack_start(stack);

	tsk_thread_sleep(1000);

	tsip_stack_stop(stack);

	tsip_stack_destroy(stack);
}


#endif /* _TEST_STACK_H */
