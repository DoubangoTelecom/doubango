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
#ifndef _TEST_SEMAPHORE_H_
#define _TEST_SEMAPHORE_H_

int sema_count = 0;

void *threadfunc_semaphore1(void *parm)
{
	tsk_semaphore_t *semaphore = (tsk_semaphore_t *)parm;
	int ret = 0;

	sema_count++;
	ret =  tsk_semaphore_decrement(semaphore);
	printf("threadfunc_semaphore1/// %d\n", ret);

	return 0;
}

void *threadfunc_semaphore2(void *parm)
{
	tsk_semaphore_t *semaphore = (tsk_semaphore_t *)parm;
	int ret = 0;

	sema_count++;
	ret =  tsk_semaphore_decrement(semaphore);
	printf("threadfunc_semaphore2/// %d\n", ret);

	return 0;
}

/* Pthread semaphore */
void test_semaphore()
{
	tsk_semaphore_t *semaphore = 0;
	void*       tid[2] = {0,0};
	int i;

	printf("test_semaphore//\n");

	TSK_SEMAPHORE_CREATE(semaphore);

	tsk_thread_create(&tid[0], threadfunc_semaphore1, semaphore);
	tsk_thread_create(&tid[1], threadfunc_semaphore2, semaphore);

	/* VERY BAD */
	while(sema_count<2);
	for(i=0;i<10000000;i++);
	
	assert(!tsk_semaphore_increment(semaphore));
	assert(!tsk_semaphore_increment(semaphore));

	tsk_thread_join(&tid[0]);
	tsk_thread_join(&tid[1]);

	TSK_SEMAPHORE_SAFE_FREE(semaphore);
}

#endif /* _TEST_SEMAPHORE_H_ */