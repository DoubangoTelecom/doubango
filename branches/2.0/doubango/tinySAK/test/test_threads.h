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


#ifndef _TEST_THREADS_H_
#define _TEST_THREADS_H_


void* MyThreadFunction(void *arg)
{
	printf("arg=%d", *((int*)arg));
	return tsk_null;
}

void test_threads()
{
	void* tid[1] = {tsk_null}; // thread id
	int arg = 112; // arg to pass to the function

	printf("test_threads//\n");
	
	// create the thread
	tsk_thread_create(&tid[0], MyThreadFunction, &arg);

	// join the thread
	tsk_thread_join(&(tid[0]));
}

#endif /* _TEST_THREADS_H_ */
