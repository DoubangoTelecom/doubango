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

/**@def TSK_LIST_CREATE
* Create and initialize a linked list.
* You MUST use @ref TSK_LIST_SAFE_FREE to free a linked list.
* @sa @ref TSK_LIST_SAFE_FREE.
*/
#define TSK_LIST_CREATE(this)				TSK_XXX_CREATE2(this, list)
/**@def TSK_LIST_SAFE_FREE
* Safely free a linked list previously created using @ref TSK_LIST_CREATE.
* @sa @ref TSK_LIST_CREATE.
*/
#define TSK_LIST_SAFE_FREE(this)			TSK_XXX_SAFE_FREE2(this, list)

/**@def TSK_LIST_ITEM_CREATE
* Create and initialize an item.
* You MUST use @ref TSK_LIST_ITEM_SAFE_FREE to free an item.
* @sa @ref TSK_LIST_ITEM_SAFE_FREE.
*/
#define TSK_LIST_ITEM_CREATE(this)		TSK_XXX_CREATE2(this, list_item)
/**@def TSK_LIST_ITEM_SAFE_FREE
* Safely free an item previously created using @ref TSK_LIST_ITEM_CREATE.
* @sa @ref TSK_LIST_ITEM_CREATE.
*/
#define TSK_LIST_ITEM_SAFE_FREE(this)		TSK_XXX_SAFE_FREE2(this, list_item)

/**@typedef tsk_list_item_func_free
* Used to define the function to call to free an @b item.
*
*@param item item to free
*/
typedef void (*tsk_list_item_func_free)(void** item);

/** Item
*/
typedef struct tsk_list_item_s
{
	void* data; /**< opaque data encapsulated by the item. */
	tsk_list_item_func_free func_free; /**< pointer to the function to call to free the item's data. */
	struct tsk_list_item_s* next; /**< next item. */
}
tsk_list_item_t;

/**Linked list
*/
typedef struct tsk_list_s
{
	tsk_list_item_t* head; /**< head of the list. */ 
	//int count; /**< number of item in the list. */ 
}
tsk_list_t;

/**@typedef tsk_list_func_predicate
* Function predicate used to match an item.
* @retval 1 if match and 0 otherwise
*/
typedef int (*tsk_list_func_predicate)(const tsk_list_item_t* item, const void* data);


/**def tsk_list_foreach
*
* Loop through the linked list
* @param item current item
* @param list pointer to the list for which we want to get items
*
*/
#define tsk_list_foreach(item, list) for(item = list?list->head:0; item; item= item->next)


TINYSAK_API void tsk_list_init(tsk_list_t* list);
TINYSAK_API void tsk_list_remove_item(tsk_list_t* list, tsk_list_item_t* item);
TINYSAK_API void tsk_list_remove_item2(tsk_list_t* list, tsk_list_func_predicate predicate, const void * data);
TINYSAK_API void tsk_list_add_item(tsk_list_t* list, tsk_list_item_t** item);
TINYSAK_API void tsk_list_add_list(tsk_list_t* destination, tsk_list_t** source);
TINYSAK_API void tsk_list_add_data(tsk_list_t* destination, void** data, tsk_list_item_func_free item_func_free);
TINYSAK_API const tsk_list_item_t* tsk_list_find_item(const tsk_list_t* list, tsk_list_func_predicate predicate, const void * data);
TINYSAK_API void tsk_list_free(tsk_list_t** list);

TINYSAK_API void tsk_list_item_init(tsk_list_item_t* item);
TINYSAK_API void tsk_list_item_free(tsk_list_item_t** item);

#endif /* _TINYSAK_LIST_H_ */