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

/**@file tsk_list.c
 * @brief Linked list
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tsk_list.h"
#include <stdlib.h>
#include <string.h>

/**@defgroup tsk_list_group Linked list
*/

/**@page tsk_list_page Linked list Tutorial
*
* This page explain how to use linked lists in your program.
* Below you have two example, one with a basic linked list and the another with
* a complex type (structure).
*
*<H2>==Basic linked list==</H2>
*
* This kind of list contains basic items. An item is called basic if it holds a pointer to a scalar type (
* e.g. char, int , short, long ...).
*
* @code
* tsk_list_t *list = 0;
* tsk_list_item_t *item = 0;
* @endcode
*
*  create a list
* @code
* TSK_LIST_CREATE(list);
* @endcode
*
* add items to the list
* @code
* TSK_LIST_ITEM_CREATE(item);
* item->data = (void*)strdup("First item");
* tsk_list_add_item(list, &item);
* 
* TSK_LIST_ITEM_CREATE(item);
* item->data = (void*)strdup("Second item");
* tsk_list_add_item(list, &item);
* 
*
*  ... add another items ...
* @endcode
* 
* dump all items
* @code
* tsk_list_foreach(item, list)
* {
* 	char* item_data = ((char*)item->data);
* 	printf("test_basic_list/// --> [%s]\n", item_data);
* }
* @endcode
* 
* delete all items in the list
* @code
* TSK_LIST_SAFE_FREE(list);
* @endcode
*
**<H2>==Complex linked list==</H2>
*
* This kind of list contains complex items. An item is called complex if it holds a pointer to a complex type (
* e.g. struct, list, custom data, ...). You should always provided a function pointer to call to free the item.
*
*
* define the complex type to add to our linked basic list
* @code
* typedef struct person_s { char *id; char *name; } person_t;
* @endcode
*
* define the function to call to free the item
* @code
* void person_free(void** person) { 
*	person_t** pp = (person_t**)person; 
*	free((*pp)->id); 
*	free((*pp)->name);
*	free((*pp));
*	(*pp)=0;
*	printf("test_advanced_list/// [person_free]\n");
* }
* @endcode
*
*
* @code
* tsk_list_t *list = 0;
* tsk_list_item_t *item = 0;
* @endcode
*
* create list
* @code
* TSK_LIST_CREATE(list);
* @endcode
*
* add items to the complex list
* @code
* {
*	person_t *person1 = (person_t*)malloc(sizeof *person1);
*	person1->id = strdup("1");
*	person1->name = strdup("person1");
*	tsk_list_add_data(list, ((void**) &person1), person_free);
* }
* {
*	person_t *person2 = (person_t*)malloc(sizeof *person2);
*	person2->id = strdup("2");
*	person2->name = strdup("person2");
*	tsk_list_add_data(list, ((void**) &person2), person_free);
* }
... add another person to the complex list ...
* @endcode
*
* dump all items
* @code
* tsk_list_foreach(item, list)
* {
*	person_t* item_data = ((person_t*)item->data);
*	printf("test_complex_list/// --> [id=%s and name=%s]\n", item_data->id, item_data->name);
* }
* @endcode
*
* @code
* delete all items in the list
* TSK_LIST_SAFE_FREE(list);
* @endcode
*
*
**<H2>==Predicates==</H2>
* It is possible to use predicates to retrieve an item from a list.
* 
* First, define your predicate function like this:
*@code
int find_person_by_name(const tsk_list_item_t* item, const void* name)
{
	person_t* item_data = (person_t*)(item->data);
	printf("test_advanced_list/// [find_person2]\n");
	return !strcmp(item_data->name, (const char*)name);
}
*@endcode
*
* Second, use your prdicate to find an item like this:
* @code
// Find person named "person2"
{
	const tsk_list_item_t *item_const = tsk_list_find_item(list, find_person_by_name, "person2");
	{
		const person_t* item_data_const = ((const person_t*)item_const->data);
		printf("test_complex_list/// --> person with name==\"person2\" and id=\"%s\"", item_data_const->id);
	}
}
@endcode
*
*/

/* find an item */
int tsk_list_find_by_item(const tsk_list_item_t* item, const void* _item)
{
	return (item == (const tsk_list_item_t*)_item) ? 1 : 0;
}

/**@ingroup tsk_list_group
* Initialize an already allocated list
* @param list list to initialize
*/
void tsk_list_init(tsk_list_t* list)
{
	memset(list, 0, sizeof(tsk_list_t));
}

/**@ingroup tsk_list_group
* Remove an item from the @a list
* @param list the list from which to remove the @a item
* @param item the item to remove from the @a list
*/
void tsk_list_remove_item(tsk_list_t* list, tsk_list_item_t* item)
{
	tsk_list_remove_item2(list, tsk_list_find_by_item, (const void*)item);
}

/**@ingroup tsk_list_group
* Remove an item from the @a list using predicate function
* @param list the list from which to remove an item
* @param predicate the predicate function
* @param data the data to pass to the predicate function
*/
void tsk_list_remove_item2(const tsk_list_t* list, tsk_list_func_predicate predicate, const void * data)
{
	if(list)
	{
		tsk_list_item_t *prev = 0;
		tsk_list_item_t *curr = prev = list->head;

		while(curr)
		{
			if(predicate(curr, data))
			{
				prev->next = curr->next;
				tsk_list_item_free(&curr);
				break;
			}
			
			prev = curr;
			curr = curr->next;
		}
	}
}


/**@ingroup tsk_list_group
* Add an item to the list
* @param list list in which we will add the item
* @param item item to add
*/
void tsk_list_add_item(tsk_list_t* list, tsk_list_item_t** item)
{
	(*item)->next  = list->head;
    list->head = *item;
	
	(*item) = 0;
}

/**@ingroup tsk_list_group
* Add all items in @a source into @a destination
* @param dest destination list
* @param src source list
**/
void tsk_list_add_list(tsk_list_t* dest, tsk_list_t** src)
{
	tsk_list_item_t* next = 0;
	tsk_list_item_t* curr = (*src)->head;

	while(curr)
	{
		next = curr->next;
		tsk_list_add_item(dest, &curr);
		curr = next->next;
	}
	(*src) = 0;
}

/**@ingroup tsk_list_group
* Add an opaque data to the list
* @param list destination list
* @param data opaque data to add into @a list
* @param item_func_free pointer to the function to call to free the opaque @a data
*/
void tsk_list_add_data(tsk_list_t* list, void** data, tsk_list_item_func_free item_func_free)
{
	if(data)
	{
		tsk_list_item_t *item = 0;
		TSK_LIST_ITEM_CREATE(item);
		item->data = *data;
		item->func_free = item_func_free;

		tsk_list_add_item(list, &item);
		(*data) = 0;
	}
}

/**@ingroup tsk_list_group
* Find first item matching criteria defined by the @a predicate.
* @param list the list to query
* @param predicate the predicate against which to test each item
* @param data data passed to the predicate function for comparaison
* @retval the item which match the criteria and NULL otherwise
*/
const tsk_list_item_t* tsk_list_find_item(const tsk_list_t* list, tsk_list_func_predicate predicate, const void * data)
{
	if(predicate)
	{
		tsk_list_item_t *item;
		tsk_list_foreach(item, list)
		{
			if(predicate(item, data))
			{
				return item;
			}
		}
	}
	return 0;
}

/**@ingroup tsk_list_group
* Free all items in the list. If function pointers have been defined for some items then
* they will be called before deallocating the item pointer.
* @param list list to free
*/
void tsk_list_free(tsk_list_t** list)
{
	if(*list)
	{
		tsk_list_item_t* next = 0;
		tsk_list_item_t* curr = (*list)->head;

		while(curr)
		{
			next = curr->next;
			tsk_list_item_free(&curr);
			curr = next;
		}
		free(*list);
		(*list) = 0;
	}
}

/**@ingroup tsk_list_group
* Initialize an already allocated item 
* @param item item to initialize
*/
void tsk_list_item_init(tsk_list_item_t* item)
{
	memset(item, 0, sizeof(tsk_list_item_t));
}

/**@ingroup tsk_list_group
* Free a previously allocated item.
* @param item item to free
*/
void tsk_list_item_free(tsk_list_item_t** item)
{
	if(*item)
	{
		if((*item)->func_free)
		{
			(*item)->func_free(&((*item)->data));
		}
		else if((*item)->data)
		{
			free((*item)->data);
		}

		free(*item);
		(*item) = 0;
	}
}