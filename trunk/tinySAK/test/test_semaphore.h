/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango.org>
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
#ifndef _TEST_SEMAPHORE_H_
#define _TEST_SEMAPHORE_H_

void *threadfunc_semaphore1(void *parm)
{
	tsk_semaphore_handle_t *semaphore = (tsk_semaphore_handle_t *)parm;
	int ret = 0;

	printf("threadfunc_semaphore1/// START %d\n", ret);
	ret =  tsk_semaphore_decrement(semaphore);
	printf("threadfunc_semaphore1/// STOP %d\n", ret);

	return 0;
}

void *threadfunc_semaphore2(void *parm)
{
	tsk_semaphore_handle_t *semaphore = (tsk_semaphore_handle_t *)parm;
	int ret = 0;

	printf("threadfunc_semaphore2/// START %d\n", ret);
	ret =  tsk_semaphore_decrement(semaphore);
	printf("threadfunc_semaphore2/// STOP %d\n", ret);

	return 0;
}

/* Pthread semaphore */
void test_semaphore()
{
	tsk_semaphore_handle_t *semaphore = tsk_semaphore_create();
	void*       tid[2] = {0,0};

	printf("test_semaphore//\n");

	tsk_thread_create(&tid[0], threadfunc_semaphore1, semaphore);
	tsk_thread_create(&tid[1], threadfunc_semaphore2, semaphore);

	tsk_thread_sleep(1000);
	
	assert(!tsk_semaphore_increment(semaphore));
	assert(!tsk_semaphore_increment(semaphore));

	tsk_thread_sleep(1000);

	tsk_thread_join(&tid[0]);
	tsk_thread_join(&tid[1]);

	tsk_semaphore_destroy(&semaphore);	
}

#endif /* _TEST_SEMAPHORE_H_ */
