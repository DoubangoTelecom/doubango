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
///**@file tsk_heap.h
//* @brief Useful string functions to manipulate strings.
//*
//* @author Mamadou Diop <diopmamadou(at)yahoo.fr>
//*
//* @date Created: Sat Nov 8 16:54:58 2009 mdiop
//*/
//#ifndef _TINYSAK_HEAP_H_
//#define _TINYSAK_HEAP_H_
//
//#include "tinySAK_config.h"
//#include "tsk_list.h"
//#include "tsk_object.h"
//
///**@typedef tsk_heap_pool_t
//* Memory heap pool containing all aloocated variables.
//*/
//typedef tsk_list_t tsk_heap_pool_t;
//
///**@typedef tsk_heap_address_t
//* The address of one variable allocated on the heap memory.
//*/
//typedef tsk_list_item_t tsk_heap_address_t;
//
///** Defines the memory on which all variables will be allocated.
//* You must only allocate scalar variables on the heap memory otherwise they will not be
//* correctly freed.
//*/
//typedef struct tsk_heap_s
//{
//	TSK_DECLARE_OBJECT;
//	tsk_heap_pool_t pool; /**< Memory pool holding the address of all allocated variables */ 
//}
//tsk_heap_t;
//
///** PUSH new address to the heap pool
//*/
//#define HEAP_PUSH(heap, address) if(heap) tsk_heap_push(heap, (void*)address)
///** POP and free an address from the heap pool
//*/
//#define HEAP_POP(heap, address) if(heap) tsk_heap_pop(heap, (void*)address); else free(address)
//
////TINYSAK_API void tsk_heap_init(tsk_heap_t *heap);
//TINYSAK_API void tsk_heap_push(tsk_heap_t *heap, void * address);
//TINYSAK_API void tsk_heap_pop(tsk_heap_t *heap, void * address);
//TINYSAK_API void tsk_heap_cleanup(tsk_heap_t *heap);
//
//#endif /* _TINYSAK_HEAP_H_ */

