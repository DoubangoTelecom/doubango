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


#ifndef _TEST_MUTEX_H_
#define _TEST_MUTEX_H_

int mutex_thread_started = 0;

int safe_func(tsk_mutex_handle_t *mutex, const char* caller)
{
	int ret = 0;

	ret = tsk_mutex_lock(mutex);
	TSK_DEBUG_INFO("threadfunc_mutex/// Start doing job [%s]... %d\n", caller, ret);
	tsk_thread_sleep(5000);
	TSK_DEBUG_INFO("threadfunc_mutex/// Stop doing job [%s]... %d\n", caller, ret);
	ret = tsk_mutex_unlock(mutex);

	return ret;
}

void *threadfunc_mutex(void *parm)
{
	tsk_mutex_handle_t *mutex = (tsk_mutex_handle_t *)parm;
	safe_func(mutex, "caller1");

	return 0;
}


/* Pthread mutex */
void test_mutex()
{
	tsk_mutex_handle_t *mutex = tsk_mutex_create();
	void*       tid[1] = {0};

	TSK_DEBUG_INFO("test_mutex//\n");
	
	//tsk_mutex_lock(mutex);
	
	tsk_thread_create(&tid[0], threadfunc_mutex, mutex);
	tsk_thread_sleep(1000);

	safe_func(mutex, "caller0");

	/*while(!mutex_thread_started) tsk_thread_sleep(1000);

	printf("test_mutex// threadfunc_mutex is locked for 1 second\n");
	tsk_thread_sleep(1000);

	printf("test_mutex// Now unlock threadfunc_mutex\n");
	tsk_mutex_unlock(mutex);*/


	tsk_thread_join(&(tid[0]));

	tsk_mutex_destroy(&mutex);
}

#endif /* _TEST_MUTEX_H_ */
