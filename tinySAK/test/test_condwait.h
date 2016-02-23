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
#ifndef _TEST_CONDWAIT_H_
#define _TEST_CONDWAIT_H_

void *threadfunc_timed(void *parm)
{
    tsk_condwait_handle_t *condwait = (tsk_condwait_handle_t *)parm;
    int ret = 0;

    ret =  tsk_condwait_timedwait(condwait, 10);
    printf("threadfunc_timed/// %d\n", ret);

    return 0;
}

void *threadfunc_infinite(void *parm)
{
    tsk_condwait_handle_t *condwait = (tsk_condwait_handle_t *)parm;
    int ret = 0;

    ret =  tsk_condwait_wait(condwait);
    printf("threadfunc_infinite/// %d\n", ret);

    return 0;
}

void *threadfunc_onemore(void *parm)
{
    tsk_condwait_handle_t *condwait = (tsk_condwait_handle_t *)parm;
    int ret = 0;

    ret =  tsk_condwait_wait(condwait);
    printf("threadfunc_onemore/// %d\n", ret);

    return 0;
}



/* Pthread condwait */
void test_condwait()
{
    tsk_condwait_handle_t *condwait = tsk_condwait_create();
    int ret;
    void*             tid[3] = {0,0,0};

    printf("test_condwait//\n");

    tsk_thread_create(&tid[0], threadfunc_timed, condwait);
    tsk_thread_create(&tid[1], threadfunc_infinite, condwait);
    tsk_thread_create(&tid[2], threadfunc_onemore, condwait);

    tsk_condwait_timedwait(condwait, 100); /* give the threads the time to start and 'threadfunc_timed' function to timeout */

    ret = tsk_condwait_signal(condwait); /* Release one */
    ret = tsk_condwait_broadcast(condwait); /* Release all */


    tsk_thread_join(&tid[0]);
    tsk_thread_join(&tid[1]);
    tsk_thread_join(&tid[2]);

    tsk_condwait_destroy(&condwait);
}

#endif /* _TEST_CONDWAIT_H_ */
