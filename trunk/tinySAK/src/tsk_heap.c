///*
//* Copyright (C) 2009 Mamadou Diop.
//*
//* Contact: Mamadou Diop <diopmamadou@yahoo.fr>
//*	
//* This file is part of Open Source Doubango Framework.
//*
//* DOUBANGO is free software: you can redistribute it and/or modify
//* it under the terms of the GNU General Public License as published by
//* the Free Software Foundation, either version 3 of the License, or
//* (at your option) any later version.
//*	
//* DOUBANGO is distributed in the hope that it will be useful,
//* but WITHOUT ANY WARRANTY; without even the implied warranty of
//* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//* GNU Lesser General Public License for more details.
//*	
//* You should have received a copy of the GNU General Public License
//* along with DOUBANGO.
//*
//*/
//
///**@file tsk_heap.c
// * @brief Useful string functions to manipulate strings.
// *
// * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
// *
// * @date Created: Sat Nov 8 16:54:58 2009 mdiop
// */
//#include "tsk_heap.h"
//#include "tsk_memory.h"
//#include "tsk_debug.h"
//#include "tsk_macros.h"
//#include <stdlib.h>
//
///**@defgroup tsk_heap_group Memory heap
//*/
//
///**@page tsk_heap_page Heap Management Tutorial
//*/
//
///**@ingroup tsk_heap_group
//* Predicate function used to retrive an address by ref
//*/
//static int tsk_heap_find_by_address(const tsk_heap_address_t* item, const void* address)
//{
//	return (item->data == address) ? 1 : 0;
//}
//
///**@ingroup tsk_heap_group
//* Initialize a memory heap. You MUST call this method before using the your heap memory.
//* @param heap the memory heap to initialize
//*/
///*void tsk_heap_init(tsk_heap_t *heap)
//{
//	tsk_list_init(&heap->pool);
//}*/
//
///**@ingroup tsk_heap_group
//* Push new variable address into the heap
//* @param heap the heap into which to push the address
//* @param address the address to insert into the heap
//*/
//void tsk_heap_push(tsk_heap_t *heap, void * address)
//{
//	tsk_heap_address_t *item;
//
//	TSK_LIST_ITEM_CREATE(item);
//	item->data = address;
//	item->func_free = 0;
//	tsk_list_add_item(&heap->pool, &item);
//}
//
///**@ingroup tsk_heap_group
//* Pop an address from the heap and free the corresponding memory
//* @param heap the heap from which to pop the address
//* @param address the address to pop from the heap
//*/
//void tsk_heap_pop(tsk_heap_t *heap, void * address)
//{
//	tsk_list_remove_item2(&heap->pool, tsk_heap_find_by_address, (const void*)address);
//}
//
///**@ingroup tsk_heap_group
//* Free all addresses from the heap
//*/
//void tsk_heap_cleanup(tsk_heap_t *heap)
//{
//	if(heap)
//	{
//		tsk_list_item_t* next = 0;
//		tsk_heap_pool_t *pool = &heap->pool;
//		tsk_list_item_t* curr = pool->head;
//
//		while(curr)
//		{
//			next = curr->next;
//			tsk_list_item_free(&curr);
//			curr = next;
//		}
//		pool->head = 0;
//	}
//	else
//	{
//		TSK_DEBUG_WARN("Cannot cleanup an uninitialized heap object");
//	}
//}