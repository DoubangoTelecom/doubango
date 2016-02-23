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
#ifndef _TEST_RUNNABLE_H_
#define _TEST_RUNNABLE_H_

typedef struct test_runnable_timer_s {
    tsk_timer_id_t id;
    uint64_t timeout;
    const char *arg;
}
test_runnable_timer_t;

test_runnable_timer_t runnable_timers[] = {
    {0, 2000,	"3"},
    {1, 2500,	"4"},
    {2, 500,	"1"},
    {3, 1000,	"2"},
    {4, 1000,	"2"},
    {5, 0,		"0"},
    {6, 10000,	"6"},
    {7, 3000,	"5"},
    {8, 2500,	"4"},
};

typedef struct tsk_obj_s {
    TSK_DECLARE_OBJECT;

    tsk_timer_id_t timer_id;
}
tsk_obj_t;

static void* tsk_obj_ctor(void * self, va_list * app)
{
    tsk_obj_t *obj = self;
    if(obj) {
        obj->timer_id = va_arg(*app, tsk_timer_id_t);
    }
    return self;
}
static void* tsk_obj_dtor(void * self)
{
    return self;
}
static int tsk_obj_cmp(const void *obj1, const void *obj2)
{
    return 0;
}
static const tsk_object_def_t tsk_obj_def_s = {
    sizeof(tsk_obj_t),
    tsk_obj_ctor,
    tsk_obj_dtor,
    tsk_obj_cmp,
};
const tsk_object_def_t *tsk_obj_def_t = &tsk_obj_def_s;

void *run(void* self)
{
    int i = 0;
    tsk_list_item_t *curr;

    TSK_RUNNABLE_RUN_BEGIN(self);

    if(curr = TSK_RUNNABLE_POP_FIRST(self)) {
        const tsk_obj_t *obj = (const tsk_obj_t*)curr->data;
        printf("\n\nRunnable event-id===>[%llu]\n\n", obj->timer_id);
        tsk_object_unref(curr);
    }

    TSK_RUNNABLE_RUN_END(self);

    return 0;
}

static int test_runnable_timer_callback(const void* arg, tsk_timer_id_t timer_id)
{
    const tsk_runnable_t* runnable = arg;
    if(runnable) {
        TSK_RUNNABLE_ENQUEUE(runnable, timer_id);
        return 0;
    }
    return -1;
}

void test_runnable()
{
    size_t i;
    tsk_timer_manager_handle_t *timer_mgr = tsk_timer_manager_create();
    tsk_runnable_t* runnable = tsk_runnable_create();
    runnable->run = run;
    printf("test_runnable//\n");

    tsk_timer_manager_start(timer_mgr);
    tsk_runnable_start(runnable, tsk_obj_def_t);

    for(i=0; i<sizeof(runnable_timers)/sizeof(test_runnable_timer_t); ++i) {
        runnable_timers[i].id = tsk_timer_manager_schedule(timer_mgr, runnable_timers[i].timeout, test_runnable_timer_callback, runnable);
    }

    tsk_thread_sleep(4000);

    /* Stops and frees both timer manager and runnable object */
    TSK_OBJECT_SAFE_FREE(runnable);
    TSK_OBJECT_SAFE_FREE(timer_mgr);
}

#endif /* _TEST_RUNNABLE_H_ */
