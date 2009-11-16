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
#ifndef _TEST_MUTEX_H_
#define _TEST_MUTEX_H_

int mutex_count = 0;

void *threadfunc_mutex1(void *parm)
{
	tsk_mutex_t *mutex = (tsk_mutex_t *)parm;
	int ret = 0;

	mutex_count++;
	ret =  tsk_mutex_lock(mutex);
	printf("threadfunc_mutex1/// %d\n", ret);

	return 0;
}

void *threadfunc_mutex2(void *parm)
{
	tsk_mutex_t *mutex = (tsk_mutex_t *)parm;
	int ret = 0;

	mutex_count++;
	ret =  tsk_mutex_lock(mutex);
	printf("threadfunc_mutex2/// %d\n", ret);

	return 0;
}

/* Pthread mutex */
void test_mutex()
{
	tsk_mutex_t *mutex = 0;
	void*       tid[2] = {0, 0};
	int i;

	printf("test_mutex//\n");

	TSK_MUTEX_CREATE(mutex);

	tsk_thread_create(&tid[0], threadfunc_mutex1, mutex);
	tsk_thread_create(&tid[1], threadfunc_mutex2, mutex);

	/* VERY BAD */
	while(mutex_count<2);
	for(i=0;i<10000000;i++);
	
	assert(!tsk_mutex_unlock(mutex));

	tsk_thread_join(&tid[0]);
	tsk_thread_join(&tid[1]);

	TSK_MUTEX_SAFE_FREE(mutex);
}

#endif /* _TEST_MUTEX_H_ */