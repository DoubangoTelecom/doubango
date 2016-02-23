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
#ifndef _TEST_SAFEOBJ_H_
#define _TEST_SAFEOBJ_H_

int so_mutex_count = 0;

typedef struct mysafeobj_s {
    TSK_DECLARE_SAFEOBJ;

    int test1;
    int test2;
}
mysafeobj_t;

void *threadfunc_safeobj1(void *parm)
{
    mysafeobj_t *safeobj = (mysafeobj_t *)parm;
    int ret = 0;

    so_mutex_count++;
    ret =  tsk_safeobj_lock(safeobj);
    printf("threadfunc_safeobj1/// %d\n", ret);

    return 0;
}

void *threadfunc_safeobj2(void *parm)
{
    mysafeobj_t *safeobj = (mysafeobj_t *)parm;
    int ret = 0;

    so_mutex_count++;
    ret =  tsk_safeobj_lock(safeobj);
    printf("threadfunc_safeobj2/// %d\n", ret);

    return 0;
}

/* test safeobject */
void test_safeobject()
{
    mysafeobj_t* obj = calloc(1, sizeof(mysafeobj_t));
    void*       tid[2] = {0, 0};
    int i;

    printf("test_safeobject//\n");

    tsk_safeobj_init(obj);

    //assert(!tsk_safeobj_lock(obj));

    tsk_thread_create(&tid[0], threadfunc_safeobj1, obj);
    tsk_thread_create(&tid[1], threadfunc_safeobj2, obj);

    /* VERY BAD */
    while(so_mutex_count<2);
    for(i=0; i<10000000; i++);

    /*assert(!*/tsk_safeobj_unlock(obj)/*)*/;
    /*assert(!*/tsk_safeobj_unlock(obj)/*)*/;

    tsk_thread_join(&tid[0]);
    tsk_thread_join(&tid[1]);

    tsk_safeobj_deinit(obj);
    tsk_free((void**)&obj);
}

#endif /* _TEST_SAFEOBJ_H_ */
