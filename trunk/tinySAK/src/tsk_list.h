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

/**@file tsk_list.h
 * @brief Linked list
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TINYSAK_LIST_H_
#define _TINYSAK_LIST_H_

#include "tinySAK_config.h"
#include "tsk_object.h"

/**@def TSK_LIST_CREATE
* Create and initialize a linked list.
* You MUST use @ref TSK_LIST_SAFE_FREE to free a linked list.
* @sa @ref TSK_LIST_SAFE_FREE.
*/
#define TSK_LIST_CREATE()					tsk_object_new(tsk_list_def_t)
/**@def TSK_LIST_SAFE_FREE
* Safely free a linked list previously created using @ref TSK_LIST_CREATE.
* @sa @ref TSK_LIST_CREATE.
*/
#define TSK_LIST_SAFE_FREE(self)			tsk_object_unref(self), self = 0

/**@def TSK_LIST_ITEM_CREATE
* Create and initialize an item.
* You MUST use @ref TSK_LIST_ITEM_SAFE_FREE to free an item.
* @sa @ref TSK_LIST_ITEM_SAFE_FREE.
*/
#define TSK_LIST_ITEM_CREATE()				tsk_object_new(tsk_list_item_def_t)
/**@def TSK_LIST_ITEM_SAFE_FREE
* Safely free an item previously created using @ref TSK_LIST_ITEM_CREATE.
* @sa @ref TSK_LIST_ITEM_CREATE.
*/
#define TSK_LIST_ITEM_SAFE_FREE(self)		tsk_object_unref(self), self = 0

/**@typedef tsk_list_item_func_free
* Used to define the function to call to free an @b item.
*
*@param item item to free
*/
//typedef void (*tsk_list_item_func_free)(void** item);

/** Item
*/
typedef struct tsk_list_item_s
{
	TSK_DECLARE_OBJECT;
	void* data; /**< opaque data encapsulated by the item. */
	struct tsk_list_item_s* next; /**< next item. */
}
tsk_list_item_t;

/**Linked list
*/
typedef struct tsk_list_s
{
	TSK_DECLARE_OBJECT;
	tsk_list_item_t* head; /**< head of the list. */
	struct tsk_list_item_s* tail; /**< tail item. */
}
tsk_list_t;

/**@typedef tsk_list_func_predicate
* Function predicate used to match an item.
* @retval 0 if match and <0 if first<second and >0 otherwise
*/
typedef int (*tsk_list_func_predicate)(const tsk_list_item_t* item, const void* data);


/**def tsk_list_foreach
*
* Loop through the linked list.
* @param item current item.
* @param list pointer to the list for which we want to get items.
*
*/
#define tsk_list_foreach(item, list) for(item = list?list->head:0; item; item= item->next)



TINYSAK_API void tsk_list_remove_item(tsk_list_t* list, tsk_list_item_t* item);
TINYSAK_API void tsk_list_remove_item_by_data(tsk_list_t* list, const void * tskobj);
TINYSAK_API void tsk_list_remove_item_by_pred(tsk_list_t* list, tsk_list_func_predicate predicate, const void * data);
TINYSAK_API void tsk_list_clear_items(tsk_list_t* list);

TINYSAK_API tsk_list_item_t* tsk_list_pop_first_item(tsk_list_t* list);
TINYSAK_API void tsk_list_push_item(tsk_list_t* list, tsk_list_item_t** item, int back);
#define tsk_list_push_back_item(list, item) tsk_list_push_item(list, item, 1)
#define tsk_list_push_front_item(list, item) tsk_list_push_item(list, item, 0)
TINYSAK_API void tsk_list_push_filtered_item(tsk_list_t* list, tsk_list_item_t** item, int ascending);
#define tsk_list_push_ascending_item(list, item) tsk_list_pushfiltered_item(list, item, 1)
#define tsk_list_push_descending_item(list, item) tsk_list_pushfiltered_item(list, item, 0)

TINYSAK_API void tsk_list_push_list(tsk_list_t* destination, tsk_list_t** source, int back);
#define tsk_list_pushback_list(destination, source) tsk_list_push_list(destination, source, 1)
#define tsk_list_pushfront_list(destination, source) tsk_list_push_list(destination, source, 0)

TINYSAK_API void tsk_list_push_data(tsk_list_t* list, void** data, int back);
#define tsk_list_push_back_data(list, data) tsk_list_push_data(list, data, 1)
#define tsk_list_push_front_data(list, data) tsk_list_push_data(list, data, 0)
TINYSAK_API void tsk_list_push_filtered_data(tsk_list_t* list, void** data, int ascending);
#define tsk_list_push_ascending_data(list, data) tsk_list_push_filtered_data(list, data, 1)
#define tsk_list_push_descending_data(list, data) tsk_list_push_filtered_data(list, data, 0)

TINYSAK_API const tsk_list_item_t* tsk_list_find_item_by_data(const tsk_list_t* list, const void * tskobj);
TINYSAK_API const tsk_list_item_t* tsk_list_find_item_by_pred(const tsk_list_t* list, tsk_list_func_predicate predicate, const void * data);




TINYSAK_API const void *tsk_list_def_t;
TINYSAK_API const void *tsk_list_item_def_t;

#endif /* _TINYSAK_LIST_H_ */