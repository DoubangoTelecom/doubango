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

/**@file tsk_list.c
 * @brief Linked list.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tsk_list.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

//#include <assert.h>
#include <string.h>

// FIXME: remove asserts

/**@defgroup tsk_list_group Linked list.
* For more information about linked list you can visit http://en.wikipedia.org/wiki/Linked_list.
*/

/** tsk_list_find_by_item
*/
static int tsk_list_find_by_item(const tsk_list_item_t* item, const void* _item)
{
	return (item == (const tsk_list_item_t*)_item) ? 0 : -1;
}

/**@ingroup tsk_list_group
* Creates a linked list object.
* You MUST use @ref TSK_OBJECT_SAFE_FREE() to safely free the object.
*/
tsk_list_t* tsk_list_create()
{
	return tsk_object_new(tsk_list_def_t);
}

/**@ingroup tsk_list_group
* Create and initialize an item to be added to a linked list.
* You MUST use @ref TSK_OBJECT_SAFE_FREE() to safely free the object.
*/
tsk_list_item_t* tsk_list_item_create()
{
	return tsk_object_new(tsk_list_item_def_t);
}

/**@ingroup tsk_list_group
* Remove an item from the @a list.
* @param list the list from which to remove the @a item.
* @param item the item to remove from the @a list.
*/
void tsk_list_remove_item(tsk_list_t* list, tsk_list_item_t* item)
{
	tsk_list_remove_item_by_pred(list, tsk_list_find_by_item, (const void*)item);
}

/**@ingroup tsk_list_group
* Pops an object from the @a list.
* @param list The list from which to pop the object.
* @param tskobj Any valid object(declared using @ref TSK_DECLARE_OBJECT) to remove.
* @retval The item.
*/
tsk_list_item_t* tsk_list_pop_item_by_data(tsk_list_t* list, const tsk_object_t * tskobj)
{
	if(list){
		tsk_list_item_t *prev = tsk_null;
		tsk_list_item_t *curr = prev = list->head;

		while(curr){
			if(!tsk_object_cmp(curr->data, tskobj)){
				if(prev == curr){
					/* Found at first position. */
					if(list->head == list->tail){
						/* There was only one item */
						list->head = list->tail = tsk_null;
					}
					else{
						list->head = curr->next;
					}
				}
				else {
					if(curr == list->tail){
						/* Found at last position */
						list->tail = prev;
						list->tail->next = tsk_null;
					}
					else{
						prev->next = curr->next;
					}
				}

				return curr;
			}
			
			prev = curr;
			curr = curr->next;
		}
	}

	return 0;
}

/**@ingroup tsk_list_group
* Removes an object from the @a list.
* @param list The list from which to remove the object.
* @param tskobj Any valid object(declared using @ref TSK_DECLARE_OBJECT) to remove.
*/
void tsk_list_remove_item_by_data(tsk_list_t* list, const tsk_object_t * tskobj)
{
	tsk_list_item_t* item;
	if((item = tsk_list_pop_item_by_data(list, tskobj))){
		tsk_object_unref(item);
	}
}

/**@ingroup tsk_list_group
* Pops an item from the @a list using a predicate function.
* @param list The list from which to pop the item.
* @param predicate The predicate function used to match the item.
* @param data Arbitrary data to pass to the predicate function.
* @retval The item
*/
tsk_list_item_t* tsk_list_pop_item_by_pred(tsk_list_t* list, tsk_list_func_predicate predicate, const void * data)
{
	if(list){
		tsk_list_item_t *prev = tsk_null;
		tsk_list_item_t *curr = prev = list->head;

		while(curr){
			if(!predicate(curr, data)){
				if(prev == curr){
					/* Found at first position. */
					if(list->head == list->tail){
						/* There was only one item */
						list->head = list->tail = tsk_null;
					}
					else{
						list->head = curr->next;
					}
				}
				else {
					if(curr == list->tail){
						/* Found at last position */
						list->tail = prev;
						list->tail->next = tsk_null;
					}
					else{
						prev->next = curr->next;
					}
				}

				return curr;
			}
			
			prev = curr;
			curr = curr->next;
		}
	}

	return 0;
}

/**@ingroup tsk_list_group
* Removes an item from the @a list using a predicate function.
* @param list The list from which to remove the item.
* @param predicate The predicate function used to match the item.
* @param data Arbitrary data to pass to the predicate function.
*/
void tsk_list_remove_item_by_pred(tsk_list_t* list, tsk_list_func_predicate predicate, const tsk_object_t * data)
{
	tsk_list_item_t* item;
	if((item = tsk_list_pop_item_by_pred(list, predicate, data))){
		tsk_object_unref(item);
	}
}

/**@ingroup tsk_list_group
* Clean up and remove all items from the @a list.
* @param list The list ro clean up.
*/
void tsk_list_clear_items(tsk_list_t* list)
{
	if(list){
		tsk_list_item_t* next = tsk_null;
		tsk_list_item_t* curr = list->head;

		while(curr){
			next = curr->next;
			tsk_object_unref(curr);
			curr = next;
		}
		list->head = tsk_null;
		list->tail = tsk_null;
	}
}

/**@ingroup tsk_list_group
* Pops first item from the @a list. The item will be definitely removed from the list.
* @param list The list from which to pop the item.
* @retval The first item. It is up to you to free the returned item (@ref TSK_OBJECT_SAFE_FREE(item)).
*/
tsk_list_item_t* tsk_list_pop_first_item(tsk_list_t* list)
{
	tsk_list_item_t* item = tsk_null;
	if(list){
		item = list->head;
		if(list->head){
			if(list->head->next){
				list->head = list->head->next;
			}
			else{
				list->head = list->tail = tsk_null;
			}
		}
	}

	return item;
}

/**@ingroup tsk_list_group
* Add an item to the @a list.
* @param list The destination @a list.
* @param item The @a item to add.
* @param back Indicates whether to put the item back or not.
*/
void tsk_list_push_item(tsk_list_t* list, tsk_list_item_t** item, tsk_bool_t back)
{
	// do not test
	tsk_bool_t first = !list->head;
	
	if(back && list->tail){
		list->tail->next = *item, list->tail = *item, (*item)->next = tsk_null;
	}
	else {
		(*item)->next = list->head, list->head = *item;
	}
	
	if(first){
		list->tail = list->head = *item, list->tail->next = tsk_null;
	}
	(*item) = tsk_null;
}

/**@ingroup tsk_list_group
* Add an item to the list in ascending or descending order.
* @param list The destination @a list.
* @param item The  @a item to add.
* @param ascending Indicates whether to put the @a item in ascending order or not.
*/
void tsk_list_push_filtered_item(tsk_list_t* list, tsk_list_item_t** item, tsk_bool_t ascending)
{
	if(list)
	{
		tsk_list_item_t *prev = tsk_null;
		tsk_list_item_t *curr = prev = list->head;
		
		while(curr)
		{
			int diff = tsk_object_cmp((*item), curr);
			if((diff </*=*/ 0 && ascending) || (diff >/*=*/0 && !ascending)){
				if(curr == list->head){
					tsk_list_push_front_item(list, item);
				}
				else{
					(*item)->next = curr;
					prev->next = (*item);
				}

				return;
			}
			
			prev = curr;
			curr = curr->next;
		}

		tsk_list_push_back_item(list, item);
	}
}

/**@ingroup tsk_list_group
* Add all items in @a src into @a dest.
* @param dest The destination list.
* @param src The source list.
* @param back Indicates whether to put the list back or not.
**/
int tsk_list_push_list(tsk_list_t* dest, const tsk_list_t* src, tsk_bool_t back)
{
	const tsk_list_item_t* curr = (src)->head;
	tsk_object_t* copy;
	
	if(!dest || !src){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	while(curr){
		copy = tsk_object_ref(curr->data);
		tsk_list_push_data(dest, (void**)&copy, back);
				
		curr = curr->next;
	}
	return 0;
}

/**@ingroup tsk_list_group
* Add an opaque data to the @a list.
* @param list The destination @a list.
* @param data The @a data to add.
* @param back Indicates whether to put the item back or not.
*/
int tsk_list_push_data(tsk_list_t* list, void** data, tsk_bool_t back)
{
	if(list && data && *data){
		tsk_list_item_t *item = tsk_list_item_create();
		item->data = *data;
		
		tsk_list_push_item(list, &item, back);
		(*data) = tsk_null;

		return 0;
	}
	else{
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
}

/**@ingroup tsk_list_group
* Add an opaque data to the list in ascending or descending order.
* @param list The destination @a list.
* @param data The @a data to add.
* @param ascending Indicates whether to put the @a data in ascending order or not.
*/
int tsk_list_push_filtered_data(tsk_list_t* list, void** data, tsk_bool_t ascending)
{
	if(list && data && *data){
		tsk_list_item_t *item = tsk_list_item_create();
		item->data = *data;
		
		tsk_list_push_filtered_item(list, &item, ascending);
		(*data) = tsk_null;

		return 0;
	}
	else{
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
}

/**@ingroup tsk_list_group
* Find an item from a list.
* @param list The @a list holding the item.
* @param tskobj The @a object to find.
* @retval A @ref tsk_list_item_t item if found and NULL otherwize.
*/
const tsk_list_item_t* tsk_list_find_item_by_data(const tsk_list_t* list, const tsk_object_t* tskobj)
{
	if(list && tskobj){
		tsk_list_item_t *item;
		tsk_list_foreach(item, list){
			if(!tsk_object_cmp(item->data, tskobj)){
				return item;
			}
		}
	}
	
	return 0;
}

/**@ingroup tsk_list_group
* Find first item matching criteria defined by the @a predicate.
* @param list the list to query
* @param predicate the predicate against which to test each item
* @param Data data passed to the predicate function for comparaison
* @retval the item which match the criteria and NULL otherwise
*/
const tsk_list_item_t* tsk_list_find_item_by_pred(const tsk_list_t* list, tsk_list_func_predicate predicate, const void* data)
{
	if(predicate){
		const tsk_list_item_t *item;
		tsk_list_foreach(item, list){
			if(!predicate(item, data)){
				return item;
			}
		}
	}
	else{
		TSK_DEBUG_WARN("Cannot use an uninitialized predicate function");
	}
	return tsk_null;
}

/**@ingroup tsk_list_group
* Counts the number of item matching the predicate.
* @param list The list containing the items to count
* @predicate The predicate to use to match the items
* @param data Data passed to the predicate function for comparaison
* @retval The number of item matching the predicate
*/
size_t tsk_list_count(const tsk_list_t* list, tsk_list_func_predicate predicate, const void* data)
{
	size_t count = 0;
	if(predicate && list){
		const tsk_list_item_t *item;
		tsk_list_foreach(item, list){
			if(!predicate(item, data)){
				++count;
			}
		}
	}
	else{
		TSK_DEBUG_ERROR("Invalid parameter");
	}

	return count;
}











//=================================================================================================
//	Item object definition
//
static tsk_object_t* tsk_list_item_ctor(tsk_object_t * self, va_list * app)
{
	tsk_list_item_t *item = self;
	if(item){
	}
	return self;
}

static tsk_object_t* tsk_list_item_dtor(tsk_object_t *self)
{
	tsk_list_item_t *item = self;
	if(item){
		item->data = tsk_object_unref(item->data);
	}
	else{
		TSK_DEBUG_WARN("Cannot free an uninitialized item");
	}
	return item;
}

static int tsk_list_item_cmp(const tsk_object_t *_item1, const tsk_object_t *_item2)
{	
	const tsk_list_item_t* item1 = _item1;
	const tsk_list_item_t* item2 = _item2;
	
	if(item1 && item2){
		return tsk_object_cmp(item1->data, item2->data);
	}
	else return -1;
}

static const tsk_object_def_t tsk_list_item_def_s =
{
	sizeof(tsk_list_item_t),	
	tsk_list_item_ctor,
	tsk_list_item_dtor,
	tsk_list_item_cmp,
};
const tsk_object_def_t *tsk_list_item_def_t = &tsk_list_item_def_s;

//=================================================================================================
//	List object definition
//
static tsk_object_t* tsk_list_ctor(tsk_object_t *self, va_list *app)
{
	tsk_list_t *list = self;
	if(list){
	}

	return self;
}

static tsk_object_t* tsk_list_dtor(tsk_object_t *self)
{ 
	tsk_list_t *list = self;
	if(list){
#if 0
		/* Not thread-safe */
		tsk_list_item_t* next = tsk_null;
		tsk_list_item_t* curr = list->head;

		while(curr){
			next = curr->next;
			/*curr =*/ tsk_object_unref(curr);
			curr = next;
		}
#else
		/* Thread-safe method */
		tsk_list_item_t* item;
		while((item = tsk_list_pop_first_item(list))){
			tsk_object_unref(item);
		}
#endif
	}
	else{
		TSK_DEBUG_WARN("Cannot free an uninitialized list");
	}
	return list;
}

static const tsk_object_def_t tsk_list_def_s =
{
	sizeof(tsk_list_t),
	tsk_list_ctor,
	tsk_list_dtor,
	tsk_null,
};
const tsk_object_def_t *tsk_list_def_t = &tsk_list_def_s;

