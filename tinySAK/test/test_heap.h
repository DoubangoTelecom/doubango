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
#ifndef _TEST_HEAP_H_
#define _TEST_HEAP_H_


/* test heap*/
void test_heap()
{
    //int i = 0;
    //tsk_heap_t heap;

    ///* initialize our memory heap */
    //tsk_heap_init(&heap);

    ///* heap(1): Ten strings are allocated an all freed when we cleanup the heap  */
    //for(i=0; i<10;i++)
    //{
    //	char* test = tsk_strdup(&heap, "testing the heap (1)");
    //}
    //tsk_heap_cleanup(&heap);

    ///* heap(2): pop memory from the heap*/
    //{
//		char* test = tsk_strdup(&heap, "testing the heap (2)");
    //	tsk_free(&heap, (void**)&test);

    //	test = tsk_calloc(&heap, 10, 1);
    //	tsk_free(&heap, (void**)&test);

    //	test = tsk_malloc(&heap, 10);
    //	tsk_free(&heap, (void**)&test);

    //	test = tsk_malloc(&heap, 10);
    //	test = tsk_realloc(&heap, test, 100);
    //	tsk_free(&heap, (void**)&test);
    //}
    //
    ///* heap(3): pop a NULL pointer */
    //{
    //	tsk_free(&heap, 0);
    //}

    ///* heap(4): allocate and pop from NULL heap */
    //{
    //	char* test = tsk_calloc(0, 10, 1);
    //	tsk_free(0, (void**)&test);

    //	test = tsk_malloc(0, 10);
    //	tsk_free(0, (void**)&test);

    //	test = tsk_malloc(0, 10);
    //	test = tsk_realloc(0, test, 100);
    //	tsk_free(0, (void**)&test);
    //}
}

#endif /* _TEST_HEAP_H_ */
