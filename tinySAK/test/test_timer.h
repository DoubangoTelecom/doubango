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
#ifndef _TEST_TIMER_H_
#define _TEST_TIMER_H_
typedef struct test_timer_s {
    tsk_timer_id_t id;
    uint64_t timeout;
    const char *arg;
}
test_timer_t;

test_timer_t timers[] = {
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

static int test_timer_callback(const void* arg, tsk_timer_id_t timer_id)
{
    // Do quick job
    printf("test_timer - id=%llu and arg=%s//\n", timer_id, arg);
    return 0;
}

void test_global_timer()
{
    size_t i;
    tsk_timer_mgr_global_ref();

    // for test: start it two times
    tsk_timer_mgr_global_start();
    tsk_timer_mgr_global_start();

    for(i=0; i<sizeof(timers)/sizeof(test_timer_t); ++i) {
        timers[i].id = tsk_timer_mgr_global_schedule(timers[i].timeout, test_timer_callback, timers[i].arg);
    }

    tsk_timer_mgr_global_cancel(timers[6].id);
    tsk_timer_mgr_global_cancel(timers[2].id);

    tsk_thread_sleep(4000);

    // for test: stop it only one time
    tsk_timer_mgr_global_stop();
    tsk_timer_mgr_global_stop();

    tsk_timer_mgr_global_unref();
}

void test_single_timer()
{
    size_t i;
    tsk_timer_manager_handle_t *handle = tsk_timer_manager_create();
    printf("test_timer//\n");

    tsk_timer_manager_start(handle);

    for(i=0; i<sizeof(timers)/sizeof(test_timer_t); ++i) {
        timers[i].id = tsk_timer_manager_schedule(handle, timers[i].timeout, test_timer_callback, timers[i].arg);
    }

    tsk_timer_manager_cancel(handle, timers[6].id);
    tsk_timer_manager_cancel(handle, timers[2].id);

    tsk_thread_sleep(4000);

    /* Stops and frees the timer manager */
    TSK_OBJECT_SAFE_FREE(handle);
}

void test_timer()
{
    //test_single_timer();
    test_global_timer();
}

#endif /* _TEST_TIMER_H_ */
