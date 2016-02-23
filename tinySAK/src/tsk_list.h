/* Copyright (C) 2010-2013 Mamadou Diop.
* Copyright (C) 2013 Doubango Telecom <http://doubango.org>
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
 * @brief Linked list. For more information about linked list you can check @ref _Page_TinySAK_AnsiC_Linked_List "this page".
 */
#ifndef _TINYSAK_LIST_H_
#define _TINYSAK_LIST_H_

#include "tinysak_config.h"
#include "tsk_mutex.h"
#include "tsk_object.h"

TSK_BEGIN_DECLS

#define TSK_LIST_IS_EMPTY(list)				((list) ? (!(list)->head) : tsk_true)
/**@ingroup tsk_list_group
* @def TSK_LIST_IS_EMPTY
* Checks if the the @ref _Page_TinySAK_AnsiC_Linked_List "linked-list" is empty or not.
* @param list The @ref tsk_list_t "list" to check.
* @return @ref tsk_true if the list is empty, othersise @ref tsk_false.
*/


/**@ingroup tsk_list_group
* @def TSK_LIST_IS_FIRST
* Checks whether the @a item is the first element in the @ref _Page_TinySAK_AnsiC_Linked_List "linked-list" or not.
* @param list The @ref tsk_list_t "list" to check.
* @param item The @ref tsk_list_item_t "item" to check.
* @return @ref tsk_true if the item is the first element in the list, othersise @ref tsk_false.
*/
/**@ingroup tsk_list_group
* @def TSK_LIST_IS_LAST
* Checks whether the @a item is the last element in the @ref _Page_TinySAK_AnsiC_Linked_List "linked-list" or not.
* @param list The @ref tsk_list_t "list" to check.
* @param item The @ref tsk_list_item_t "item" to check.
* @return @ref tsk_true if the item is the last element in the list, othersise @ref tsk_false.
*/
#define TSK_LIST_IS_FIRST(list, item)		((list) ? ((list)->head == item) : tsk_false)
#define TSK_LIST_IS_LAST(list, item)		((list) ? ((list)->tail == item) : tsk_false)

/**@ingroup tsk_list_group
* @def TSK_LIST_FIRST_DATA
* Gets the first data element in the @ref _Page_TinySAK_AnsiC_Linked_List "linked-list".
* @param list The @ref tsk_list_t "list" from which to get the first data element.
* @return The first @ref tsk_object_t "data" element. The @ref tsk_object_t "data" element is a @ref _Page_TinySAK_AnsiC_Object_Programming "well-defined" object.
*/
/**@ingroup tsk_list_group
* @def TSK_LIST_LAST_DATA
* Gets the last data element in the @ref _Page_TinySAK_AnsiC_Linked_List "linked-list".
* @param list The @ref tsk_list_t "list" from which to get the last data element.
* @return The last @ref tsk_object_t "data" element. The @ref tsk_object_t "data" element is a @ref _Page_TinySAK_AnsiC_Object_Programming "well-defined" object.
*/
#define TSK_LIST_FIRST_DATA(list)			(((list) && (list)->head) ? (list)->head->data : tsk_null)
#define TSK_LIST_LAST_DATA(list)			(((list) && (list)->tail) ? (list)->tail->data : tsk_null)

/**@ingroup tsk_list_group
* Item for @ref tsk_list_t "linked list".
*/
typedef struct tsk_list_item_s {
    TSK_DECLARE_OBJECT;
    void* data; /**< Opaque data. <b>Must</b> be a @ref _Page_TinySAK_AnsiC_Object_Programming "well-defined" object. */
    struct tsk_list_item_s* next; /**< Next item. */
}
tsk_list_item_t;

/**@ingroup tsk_list_group
* A @ref _Page_TinySAK_AnsiC_Linked_List "linked-list".
*/
typedef struct tsk_list_s {
    TSK_DECLARE_OBJECT;

    tsk_list_item_t* head; /**< The head of the linked list. */
    tsk_list_item_t* tail; /**< The tail of the linked list. */
    tsk_mutex_handle_t* mutex; /**< on-demand mutex. */
}
tsk_list_t;

/**@ingroup tsk_list_group
* Function predicate used to match an item. <br />
* For example, see @ref _Anchor_TinySAK_Linked_List_Find_Items "here" for more information on how to use this function to find items.
* @param item The current item to match.
* @param data Arbitrary data holding the data to compare.
* @return 0 if match, <0 if first<second, >0 otherwise.
*/
typedef int (*tsk_list_func_predicate)(const tsk_list_item_t* item, const void* data);


/**@ingroup tsk_list_group
* @def tsk_list_foreach
* Loop through the @ref _Page_TinySAK_AnsiC_Linked_List "linked-list". <br />
* For example, see @ref _Anchor_TinySAK_Linked_List_Dump_Items "here" for more information on how to use this function.
* @param item The current tsk_list_item_t "item".
* @param list Pointer to the @ref tsk_list_t "list" for which we want to get items.
*/
#define tsk_list_foreach(item, list) for(item = list ? list->head : tsk_null; item; item = item->next)

TINYSAK_API tsk_list_t* tsk_list_create();
TINYSAK_API tsk_list_item_t* tsk_list_item_create();

TINYSAK_API int tsk_list_lock(tsk_list_t* list);
TINYSAK_API int tsk_list_unlock(tsk_list_t* list);


/**@ingroup tsk_list_group
* @def tsk_list_remove_first_item
* Removes the first tsk_list_item_t "item" from a @ref _Page_TinySAK_AnsiC_Linked_List "linked-list". @ref tsk_object_unref() will be called on the data held by the item.
* @param list The @ref tsk_list_t "list" from which to remove the item.
*/
/**@ingroup tsk_list_group
* @def tsk_list_remove_last_item
* Removes the last tsk_list_item_t "item" from a @ref _Page_TinySAK_AnsiC_Linked_List "linked-list". @ref tsk_object_unref() will be called on the data held by the item.
* @param list The @ref tsk_list_t "list" from which to remove the item.
*/
TINYSAK_API tsk_bool_t tsk_list_remove_item(tsk_list_t* list, tsk_list_item_t* item);
#define tsk_list_remove_first_item(list) tsk_list_remove_item((list), (list) ? (list)->head : tsk_null)
#define tsk_list_remove_last_item(list) tsk_list_remove_item((list), (list) ? (list)->tail : tsk_null)
TINYSAK_API tsk_list_item_t* tsk_list_pop_item_by_data(tsk_list_t* list, const tsk_object_t * tskobj);
TINYSAK_API tsk_bool_t tsk_list_remove_item_by_data(tsk_list_t* list, const tsk_object_t * tskobj);
TINYSAK_API tsk_bool_t tsk_list_remove_item_by_pred(tsk_list_t* list, tsk_list_func_predicate predicate, const void * data);
TINYSAK_API tsk_list_item_t* tsk_list_pop_item_by_pred(tsk_list_t* list, tsk_list_func_predicate predicate, const void * data);
TINYSAK_API void tsk_list_clear_items(tsk_list_t* list);


/**@ingroup tsk_list_group
* @def tsk_list_push_back_item
* Adds a new tsk_list_item_t "item" at the end of the @ref _Page_TinySAK_AnsiC_Linked_List "linked-list".
* @param list The @ref tsk_list_t "list" into which to add the item.
* @param item A reference (@ref tsk_list_item_t **) to the tsk_list_item_t "item" to add. The reference will be stolen (set to @ref tsk_null). You must increment the reference counter using @ref tsk_object_ref() if you want to continue to use the item.
*/
/**@ingroup tsk_list_group
* @def tsk_list_push_front_item
* Adds a new tsk_list_item_t "item" at the beginning of the @ref _Page_TinySAK_AnsiC_Linked_List "linked-list".
* @param list The @ref _Page_TinySAK_AnsiC_Linked_List "linked-list" into which to add the item. Must be a @ref tsk_list_t object.
* @param item A reference (@ref tsk_list_item_t **) to the item to add. The reference will be stolen (set to @ref tsk_null). You must increment the reference counter using @ref tsk_object_ref() if you want to continue to use the item.
*/
/**@ingroup tsk_list_group
* @def tsk_list_push_ascending_item
* Inserts a new item in a @ref _Page_TinySAK_AnsiC_Linked_List "linked-list" while keeping the list in ascending order. Note that this function will not sort the list if it's not already sorted. <br />
* For more information on how to sort items check @ref _Anchor_TinySAK_Linked_List_Sort_Items "here".
* @param list The @ref tsk_list_t "list" into which to insert the item.
* @param item The @ref tsk_list_item_t "item" to insert.
* @sa @ref tsk_list_push_descending_item()
*/
/**@ingroup tsk_list_group
* @def tsk_list_push_descending_item
* Inserts a new item in a @ref _Page_TinySAK_AnsiC_Linked_List "linked-list" while keeping the list in descending order. Note that this function will not sort the list if it's not already sorted. <br />
* For more information on how to sort items check @ref _Anchor_TinySAK_Linked_List_Sort_Items "here".
* @param list The @ref tsk_list_t "list" into which to insert the item.
* @param item The @ref tsk_list_item_t "item" to insert.
* @sa @ref tsk_list_push_ascending_item()
*/
TINYSAK_API tsk_list_item_t* tsk_list_pop_first_item(tsk_list_t* list);
TINYSAK_API void tsk_list_push_item(tsk_list_t* list, tsk_list_item_t** item, tsk_bool_t back);
#define tsk_list_push_back_item(list, item) tsk_list_push_item(list, item, tsk_true)
#define tsk_list_push_front_item(list, item) tsk_list_push_item(list, item, tsk_false)
TINYSAK_API void tsk_list_push_filtered_item(tsk_list_t* list, tsk_list_item_t** item, tsk_bool_t ascending);
#define tsk_list_push_ascending_item(list, item) tsk_list_pushfiltered_item(list, item, tsk_true)
#define tsk_list_push_descending_item(list, item) tsk_list_pushfiltered_item(list, item, tsk_false)

/**@ingroup tsk_list_group
* @def tsk_list_pushback_list
* Inserts all items from one @ref _Page_TinySAK_AnsiC_Linked_List "linked-list" at the end of another one. The reference counter for all items to be added will be incremented (+1). <br />
* For more information on how to add items check @ref _Anchor_TinySAK_Linked_List_Add_Remove "here".
* @param destination The destination @ref tsk_list_t "list" into which to insert the items.
* @param source The source @ref tsk_list_t "list" from which to get the items to be inserted in @a destination.
* @sa @ref tsk_list_pushfront_list()
*/
/**@ingroup tsk_list_group
* @def tsk_list_pushfront_list
* Inserts all items from one @ref _Page_TinySAK_AnsiC_Linked_List "linked-list" at the beginning of another one. The reference counter for all items to be added will be incremented (+1). <br />
* For more information on how to add items check @ref _Anchor_TinySAK_Linked_List_Add_Remove "here".
* @param destination The destination @ref tsk_list_t "list" into which to insert the items.
* @param source The source @ref tsk_list_t "list" from which to get the items to be inserted in @a destination.
* @sa @ref tsk_list_pushback_list()
*/
TINYSAK_API int tsk_list_push_list(tsk_list_t* destination, const tsk_list_t* source, tsk_bool_t back);
#define tsk_list_pushback_list(destination, source) tsk_list_push_list(destination, source, tsk_true)
#define tsk_list_pushfront_list(destination, source) tsk_list_push_list(destination, source, tsk_false)

/**@ingroup tsk_list_group
* @def tsk_list_push_back_data
* Adds a new @ref _Page_TinySAK_AnsiC_Object_Programming "well-defined" object at the end of the @ref _Page_TinySAK_AnsiC_Linked_List "linked-list".
* @param list The @ref tsk_list_t "list" into which to add the object.
* @param tskobj A reference (@ref tsk_object_t **) to the @ref tsk_object_t "object" to add. The reference will be stolen (set to @ref tsk_null). You must increment the reference counter using @ref tsk_object_ref() if you want to continue to use the object.
*/
/**@ingroup tsk_list_group
* @def tsk_list_push_front_data
* Adds a new @ref _Page_TinySAK_AnsiC_Object_Programming "well-defined" object at the beginning of the @ref _Page_TinySAK_AnsiC_Linked_List "linked-list".
* @param list The @ref tsk_list_t "list" into which to add the object.
* @param tskobj A reference (@ref tsk_object_t **) to the @ref tsk_object_t "object" to add. The reference will be stolen (set to @ref tsk_null). You must increment the reference counter using @ref tsk_object_ref() if you want to continue to use the object.
*/
/**@ingroup tsk_list_group
* @def tsk_list_push_ascending_data
* Inserts a new @ref _Page_TinySAK_AnsiC_Object_Programming "well-defined" object in a @ref _Page_TinySAK_AnsiC_Linked_List "linked-list" while keeping the list in ascending order. Note that this function will not sort the list if it's not already sorted. <br />
* For more information on how to insert items check @ref _Anchor_TinySAK_Linked_List_Add_Remove "here".
* @param list The @ref tsk_list_t "list" into which to insert the item.
* @param tskobj A reference (@ref tsk_object_t **) to the @ref tsk_object_t "object" to add. The reference will be stolen (set to @ref tsk_null). You must increment the reference counter using @ref tsk_object_ref() if you want to continue to use the object.
* @sa @ref tsk_list_push_descending_data()
*/
/**@ingroup tsk_list_group
* @def tsk_list_push_descending_data
* Inserts a new @ref _Page_TinySAK_AnsiC_Object_Programming "well-defined" in a @ref _Page_TinySAK_AnsiC_Linked_List "linked-list" while keeping the list in descending order. Note that this function will not sort the list if it's not already sorted. <br />
* For more information on how to insert items check @ref _Anchor_TinySAK_Linked_List_Add_Remove "here".
* @param list The @ref tsk_list_t "list" into which to insert the item.
* @param tskobj A reference (@ref tsk_object_t **) to the @ref tsk_object_t "object" to add. The reference will be stolen (set to @ref tsk_null). You must increment the reference counter using @ref tsk_object_ref() if you want to continue to use the object.
* @sa @ref tsk_list_push_ascending_data()
*/

TINYSAK_API int tsk_list_push_data(tsk_list_t* list, tsk_object_t** tskobj, tsk_bool_t back);
#define tsk_list_push_back_data(list, data) tsk_list_push_data(list, data, tsk_true)
#define tsk_list_push_front_data(list, data) tsk_list_push_data(list, data, tsk_false)
TINYSAK_API int tsk_list_push_filtered_data(tsk_list_t* list, tsk_object_t** tskobj, tsk_bool_t ascending);
#define tsk_list_push_ascending_data(list, tskobj) tsk_list_push_filtered_data(list, tskobj, tsk_true)
#define tsk_list_push_descending_data(list, tskobj) tsk_list_push_filtered_data(list, tskobj, tsk_false)

TINYSAK_API const tsk_list_item_t* tsk_list_find_item_by_data(const tsk_list_t* list, const tsk_object_t * tskobj);
TINYSAK_API const tsk_list_item_t* tsk_list_find_item_by_pred(const tsk_list_t* list, tsk_list_func_predicate predicate, const void* data);
TINYSAK_API const tsk_object_t* tsk_list_find_object_by_data(const tsk_list_t* list, const tsk_object_t* tskobj);
TINYSAK_API const tsk_object_t* tsk_list_find_object_by_pred(const tsk_list_t* list, tsk_list_func_predicate predicate, const void* data);
TINYSAK_API const tsk_object_t* tsk_list_find_object_by_pred_at_index(const tsk_list_t* list, tsk_list_func_predicate predicate, const void* data, tsk_size_t index);
TINYSAK_API int tsk_list_find_index_by_pred(const tsk_list_t* list, tsk_list_func_predicate predicate, const void* data);

TINYSAK_API tsk_list_t* tsk_list_clone(const tsk_list_t* list);

TINYSAK_API tsk_size_t tsk_list_count(const tsk_list_t* list, tsk_list_func_predicate predicate, const void* data);
#define tsk_list_count_all(list) tsk_list_count((list), tsk_null, tsk_null)

TINYSAK_GEXTERN const tsk_object_def_t *tsk_list_def_t;
TINYSAK_GEXTERN const tsk_object_def_t *tsk_list_item_def_t;


TSK_END_DECLS

#endif /* _TINYSAK_LIST_H_ */

