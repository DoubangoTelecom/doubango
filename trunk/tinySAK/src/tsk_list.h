/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)yahoo.fr>
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

/**@file tsk_list.h
 * @brief Linked list. For more information about linked list you can visit http://en.wikipedia.org/wiki/Linked_list.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TINYSAK_LIST_H_
#define _TINYSAK_LIST_H_

#include "tinySAK_config.h"
#include "tsk_object.h"

TSK_BEGIN_DECLS

/**@ingroup tsk_list_group
* Creates a linked list object.
* You MUST use @ref TSK_OBJECT_SAFE_FREE to safely free the object.
*/
#define TSK_LIST_CREATE()					tsk_object_new(tsk_list_def_t)

/**@ingroup tsk_list_group
* Create and initialize an item to be added to a linked list.
* You MUST use @ref TSK_OBJECT_SAFE_FREE to safely free the object.
*/
#define TSK_LIST_ITEM_CREATE()				tsk_object_new(tsk_list_item_def_t)

/**@ingroup tsk_list_group
* Check if the the linked list is empty or not.
*	This function will fail if the list is NULL.
*/
#define TSK_LIST_IS_EMPTY(self)				(self ? (!self->head) : 1)

#define TSK_LIST_IS_FIRST(self, item)		(self ? (self->head == item) : 0)
#define TSK_LIST_IS_LAST(self, item)		(self ? (self->tail == item) : 0)

/**@ingroup tsk_list_group
* Item for linked list. 
*/
typedef struct tsk_list_item_s
{
	TSK_DECLARE_OBJECT;
	void* data; /**< Opaque data. */
	struct tsk_list_item_s* next; /**< Next item. */
}
tsk_list_item_t;

/**@ingroup tsk_list_group
* Linked list.
*/
typedef struct tsk_list_s
{
	TSK_DECLARE_OBJECT;
	tsk_list_item_t* head; /**< The head of the linked list. */
	tsk_list_item_t* tail; /**< The tail of the linked list. */
}
tsk_list_t;

/**@ingroup tsk_list_group 
* Function predicate used to match an item.
* @param item The current item to match.
* @param data Arbitrary data holding the object to compare.
* @retval 0 if match and <0 if first<second and >0 otherwise
*/
typedef int (*tsk_list_func_predicate)(const tsk_list_item_t* item, const void* data);


/**@ingroup tsk_list_group
* Loop through the linked list.
* @param item The current item.
* @param list Pointer to the list for which we want to get items.
*
*/
#define tsk_list_foreach(item, list) for(item = list?list->head:0; item; item= item->next)

TINYSAK_API void tsk_list_remove_item(tsk_list_t* list, tsk_list_item_t* item);
TINYSAK_API tsk_list_item_t* tsk_list_pop_item_by_data(tsk_list_t* list, const tsk_object_t * tskobj);
TINYSAK_API void tsk_list_remove_item_by_data(tsk_list_t* list, const tsk_object_t * tskobj);
TINYSAK_API void tsk_list_remove_item_by_pred(tsk_list_t* list, tsk_list_func_predicate predicate, const tsk_object_t * data);
TINYSAK_API tsk_list_item_t* tsk_list_pop_item_by_pred(tsk_list_t* list, tsk_list_func_predicate predicate, const tsk_object_t * data);
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

TINYSAK_API const tsk_list_item_t* tsk_list_find_item_by_data(const tsk_list_t* list, const tsk_object_t * tskobj);
TINYSAK_API const tsk_list_item_t* tsk_list_find_item_by_pred(const tsk_list_t* list, tsk_list_func_predicate predicate, const void * data);

TINYSAK_GEXTERN const void *tsk_list_def_t;
TINYSAK_GEXTERN const void *tsk_list_item_def_t;


TSK_END_DECLS

#endif /* _TINYSAK_LIST_H_ */

