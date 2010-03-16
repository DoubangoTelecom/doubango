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
* GNU General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

/**@file tsk_list.c
 * @brief Linked list.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
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
tsk_list_item_t* tsk_list_pop_item_by_data(tsk_list_t* list, const void * tskobj)
{
	if(list)
	{
		tsk_list_item_t *prev = 0;
		tsk_list_item_t *curr = prev = list->head;

		while(curr)
		{
			if(!tsk_object_cmp(curr->data, tskobj))
			{
				if(prev == curr){
					/* Found at first position. */
					if(list->head == list->tail){
						/* There was only one item */
						list->head = list->tail = 0;
					}
					else{
						list->head = curr->next;
					}
				}
				else 
				{
					if(curr == list->tail){
						/* Found at last position */
						list->tail = prev;
						list->tail->next = 0;
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
void tsk_list_remove_item_by_data(tsk_list_t* list, const void * tskobj)
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
	if(list)
	{
		tsk_list_item_t *prev = 0;
		tsk_list_item_t *curr = prev = list->head;

		while(curr)
		{
			if(!predicate(curr, data))
			{
				if(prev == curr){
					/* Found at first position. */
					if(list->head == list->tail){
						/* There was only one item */
						list->head = list->tail = 0;
					}
					else{
						list->head = curr->next;
					}
				}
				else 
				{
					if(curr == list->tail){
						/* Found at last position */
						list->tail = prev;
						list->tail->next = 0;
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
void tsk_list_remove_item_by_pred(tsk_list_t* list, tsk_list_func_predicate predicate, const void * data)
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
	if(list)
	{
		tsk_list_item_t* next = 0;
		tsk_list_item_t* curr = list->head;

		while(curr){
			next = curr->next;
			tsk_object_unref(curr);
			curr = next;
		}
		list->head = 0;
	}
}

/**@ingroup tsk_list_group
* Pops first item from the @a list. The item will be definitely removed from the list.
* @param list The list from which to pop the item.
* @retval The first item. It is up to you to free the returned item (@ref TSK_OBJECT_SAFE_FREE(item)).
*/
tsk_list_item_t* tsk_list_pop_first_item(tsk_list_t* list)
{
	tsk_list_item_t* item = 0;
	if(list)
	{
		item = list->head;
		if(list->head)
		{
			if(list->head->next){
				list->head = list->head->next;
			}
			else{
				list->head = list->tail = 0;
			}
		}
	}

	return item;
}

/**@ingroup tsk_list_group
* Add an item to the @a list.
* @param list The destination @a list.
* @param item The @a item to add.
* @param back Indicates whether to put the item back (1=yes and 0=no).
*/
void tsk_list_push_item(tsk_list_t* list, tsk_list_item_t** item, int back)
{
	int first = !list->head;

	if(back && list->tail){
		list->tail->next = *item, list->tail = *item;
	}
	else {
		(*item)->next = list->head, list->head = *item;
	}
	
	if(first){
		list->tail = list->head = *item;
	}
	(*item) = 0;
}

/**@ingroup tsk_list_group
* Add an item to the list in ascending or descending order.
* @param list The destination @a list.
* @param item The  @a item to add.
* @param ascending Indicates whether to put the @a item in ascending order(1=yes and 0=no).
*/
void tsk_list_push_filtered_item(tsk_list_t* list, tsk_list_item_t** item, int ascending)
{
	if(list)
	{
		tsk_list_item_t *prev = 0;
		tsk_list_item_t *curr = prev = list->head;
		
		while(curr)
		{
			int diff = tsk_object_cmp((*item), curr);
			if((diff </*=*/ 0 && ascending) || (diff >/*=*/0 && !ascending))
			{
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
		
		//assert((list && list->tail) ? !list->tail->next : 1);
	}
}

/**@ingroup tsk_list_group
* Add all items in @a src into @a dest.
* @param dest The destination list.
* @param src The source list.
* @param back Indicates whether to put the list back (1=yes and 0=no).
**/
void tsk_list_push_list(tsk_list_t* dest, tsk_list_t** src, int back)
{
	tsk_list_item_t* next = 0;
	tsk_list_item_t* curr = (*src)->head;

	while(curr){
		next = curr->next;
		tsk_list_push_item(dest, &curr, back);
		curr = next->next;
	}
	(*src) = 0;
}

/**@ingroup tsk_list_group
* Add an opaque data to the @a list.
* @param list The destination @a list.
* @param data The @a data to add.
* @param back Indicates whether to put the item back (1=yes and 0=no).
*/
void tsk_list_push_data(tsk_list_t* list, void** data, int back)
{
	if(data)
	{
		tsk_list_item_t *item = TSK_LIST_ITEM_CREATE();
		item->data = *data;
		
		tsk_list_push_item(list, &item, back);
		(*data) = 0;
	}
	else{
		TSK_DEBUG_WARN("Cannot add an uninitialized data to the list");
	}
}

/**@ingroup tsk_list_group
* Add an opaque data to the list in ascending or descending order.
* @param list The destination @a list.
* @param data The @a data to add.
* @param ascending Indicates whether to put the @a data in ascending order(1=yes and 0=no).
*/
void tsk_list_push_filtered_data(tsk_list_t* list, void** data, int ascending)
{
	if(data)
	{
		tsk_list_item_t *item = TSK_LIST_ITEM_CREATE();
		item->data = *data;
		
		tsk_list_push_filtered_item(list, &item, ascending);
		(*data) = 0;
	}
	else
	{
		TSK_DEBUG_WARN("Cannot add an uninitialized data to the list");
	}
}

/**@ingroup tsk_list_group
* Find an item from a list.
* @param list The @a list holding the item.
* @param tskobj The @a object to find.
* @retval A @ref tsk_list_item_t item if found and NULL otherwize.
*/
const tsk_list_item_t* tsk_list_find_item_by_data(const tsk_list_t* list, const void * tskobj)
{
	if(list && tskobj)
	{
		tsk_list_item_t *item;
		tsk_list_foreach(item, list)
		{
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
* @param data data passed to the predicate function for comparaison
* @retval the item which match the criteria and NULL otherwise
*/
const tsk_list_item_t* tsk_list_find_item_by_pred(const tsk_list_t* list, tsk_list_func_predicate predicate, const void * data)
{
	if(predicate)
	{
		tsk_list_item_t *item;
		tsk_list_foreach(item, list)
		{
			if(!predicate(item, data)){
				return item;
			}
		}
	}
	else{
		TSK_DEBUG_WARN("Cannot use an uninitialized predicate function");
	}
	return 0;
}












//=================================================================================================
//	Item object definition
//
static void* tsk_list_item_create(void * self, va_list * app)
{
	return self;
}

static void* tsk_list_item_destroy(void *self)
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

static int tsk_list_item_cmp(const void *self, const void *object)
{	
	const tsk_list_item_t* item1 = self;
	const tsk_list_item_t* item2 = object;

	return tsk_object_cmp(item1->data, item2->data);
}

//TSK_DECLARE_DEF(tsk, list_item);
static const tsk_object_def_t tsk_list_item_def_s =
{
	sizeof(tsk_list_item_t),	
	tsk_list_item_create,
	tsk_list_item_destroy,
	tsk_list_item_cmp,
};
const void *tsk_list_item_def_t = &tsk_list_item_def_s;

//=================================================================================================
//	List object definition
//
static void* tsk_list_create(void *self, va_list *app)
{
	//tsk_list_t *list = self;

	return self;
}

static void* tsk_list_destroy(void *self)
{ 
	tsk_list_t *list = self;
	if(list)
	{
		tsk_list_item_t* next = 0;
		tsk_list_item_t* curr = list->head;

		while(curr){
			next = curr->next;
			/*curr =*/ tsk_object_unref(curr);
			curr = next;
		}
	}
	else
	{
		TSK_DEBUG_WARN("Cannot free an uninitialized list");
	}
	return list;
}

//TSK_DECLARE_DEF(tsk, list);
static const tsk_object_def_t tsk_list_def_s =
{
	sizeof(tsk_list_t),
	tsk_list_create,
	tsk_list_destroy,
	0,
};
const void *tsk_list_def_t = &tsk_list_def_s;

