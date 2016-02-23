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

/**@file tsk_list.c
 * @brief Linked list.
 *
 */
#include "tsk_list.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#include <string.h>

/**@defgroup tsk_list_group Linked list.
* @brief Linked list. For more information about linked list you can check @ref _Page_TinySAK_AnsiC_Linked_List "this page".
*/

/* Predicate function to find item using memory address comparison.
*/
static int tsk_list_find_by_item(const tsk_list_item_t* item, const void* _item)
{
    return (item == (const tsk_list_item_t*)_item) ? 0 : -1;
}

/**@ingroup tsk_list_group
* Creates a @ref _Page_TinySAK_AnsiC_Linked_List "linked-list"  object. <br />
* You <b>MUST</b> use @ref TSK_OBJECT_SAFE_FREE() to safely free the returned @ref _Page_TinySAK_AnsiC_Object_Programming "well-defined" object.
* @return The newly created list object.
*/
tsk_list_t* tsk_list_create()
{
    return (tsk_list_t*)tsk_object_new(tsk_list_def_t);
}

/**@ingroup tsk_list_group
* Create and initialize an item to be added to a @ref tsk_list_t "linked list".
* You should not need to call this function by yourself. See @ref _Anchor_TinySAK_Linked_List_Add_Remove "here" for more information on how to add items.<br />
* You <b>MUST</b> use @ref TSK_OBJECT_SAFE_FREE() to safely free the returned @ref _Page_TinySAK_AnsiC_Object_Programming "well-defined" object. <br />
* @return The newly created @ref tsk_list_item_t "item" object.
*/
tsk_list_item_t* tsk_list_item_create()
{
    return (tsk_list_item_t*)tsk_object_new(tsk_list_item_def_t);
}

/**@ingroup tsk_list_group
* Locks the @ref tsk_list_t "list" to avoid concurrent access. The @ref tsk_list_t "list" must be unlocked using @ref tsk_list_unlock(). <br />
* For more information about thread-safety see @ref _Anchor_TinySAK_Linked_List_Thread_Safety "here".
* @param list The list to lock.
* @return 0 if succeed and non-zero error code otherwise.
* @sa @ref tsk_list_unlock
*/
int tsk_list_lock(tsk_list_t* list)
{
    if(list) {
        if(!list->mutex) {
            list->mutex = tsk_mutex_create();
        }
        return tsk_mutex_lock(list->mutex);
    }
    else {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
}

/**@ingroup tsk_list_group
* UnLocks a previously locked @ref tsk_list_t "list". The @ref tsk_list_t "list" must be previously locked using @ref tsk_list_lock().<br />
* For more information about thread-safety see @ref _Anchor_TinySAK_Linked_List_Thread_Safety "here".
* @param list The @ref tsk_list_t "list" to unlock. <br />
* @return 0 if succeed and non-zero error code otherwise.
* @sa @ref tsk_list_lock
*/
int tsk_list_unlock(tsk_list_t* list)
{
    if(list && list->mutex) {
        return tsk_mutex_unlock(list->mutex);
    }
    else {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
}

/**@ingroup tsk_list_group
* Removes an item from the @ref tsk_list_t "list". The reference counter value of the object held by the item will be decremented. <br />
* You should not need to call this function by yourself. See @ref _Anchor_TinySAK_Linked_List_Add_Remove "here" for more information on how to remove items.<br />
* @param list the @ref tsk_list_t "list" from which to remove the @a item.
* @param item the @ref tsk_list_item_t "item" to remove from the @a list.
* @return @ref tsk_true if @a item exists and have been removed; @ref tsk_false otherwise.
*/
tsk_bool_t tsk_list_remove_item(tsk_list_t* list, tsk_list_item_t* item)
{
    if(item) {
        return tsk_list_remove_item_by_pred(list, tsk_list_find_by_item, (const void*)item);
    }
    return tsk_false;
}

/**@ingroup tsk_list_group
* Pops an object from the @ref tsk_list_t "list". The item will be removed from the list but the refrence counter will not change.
* @param list The @ref tsk_list_t "list" from which to pop the item.
* @param tskobj Any @ref _Page_TinySAK_AnsiC_Object_Programming "well-defined" object to pop.
* @return The item holding @ref tskobj or any equivalent object. @ref tsk_object_cmp(item->data, tskobj) == 0. <br />
* @ref tsk_null will be returned if no matching object could be found.
*/
tsk_list_item_t* tsk_list_pop_item_by_data(tsk_list_t* list, const tsk_object_t *tskobj)
{
    if(list) {
        tsk_list_item_t *prev = tsk_null;
        tsk_list_item_t *curr = prev = list->head;

        while(curr) {
            if(!tsk_object_cmp(curr->data, tskobj)) {
                if(prev == curr) {
                    /* Found at first position. */
                    if(list->head == list->tail) {
                        /* There was only one item */
                        list->head = list->tail = tsk_null;
                    }
                    else {
                        list->head = curr->next;
                    }
                }
                else {
                    if(curr == list->tail) {
                        /* Found at last position */
                        list->tail = prev;
                        list->tail->next = tsk_null;
                    }
                    else {
                        prev->next = curr->next;
                    }
                }

                return curr;
            }

            prev = curr;
            curr = curr->next;
        }
    }

    return tsk_null;
}

/**@ingroup tsk_list_group
* Removes an object from the @ref tsk_list_t "list". The reference counter value of the @ref _Page_TinySAK_AnsiC_Object_Programming "well-defined" object held by the item will be decremented.
* @param list The @ref tsk_list_t "list" from which to remove the object.
* @param tskobj Any valid @ref _Page_TinySAK_AnsiC_Object_Programming "well-defined" object to remove.
* @return @ref tsk_true if an item have been removed; @ref tsk_false otherwise.
*/
tsk_bool_t tsk_list_remove_item_by_data(tsk_list_t* list, const tsk_object_t * tskobj)
{
    tsk_list_item_t* item;
    if((item = tsk_list_pop_item_by_data(list, tskobj))) {
        tsk_object_unref(item);
        return tsk_true;
    }
    return tsk_false;
}

/**@ingroup tsk_list_group
* Pops an item from the @ref tsk_list_t "list" using a @ref _Anchor_TinySAK_Linked_List_Predicates "predicate". The item will be completely removed from the list but the reference counter value will <b>not</b> be decremented.<br />
* See @ref _Anchor_TinySAK_Linked_List_Add_Remove "here" for more information on how to use this function.
* @param list The list from which to pop the item.
* @param predicate The @ref _Anchor_TinySAK_Linked_List_Predicates "predicate" used to match the item.
* @param data Arbitrary data to pass to the predicate function.
* @return An @ref tsk_list_item_t "item" matching the @ref _Anchor_TinySAK_Linked_List_Predicates "predicate" or @ref tsk_null if none match. <br />
* You <b>must</b> free the returned item using @ref TSK_OBJECT_SAFE_FREE.
*/
tsk_list_item_t* tsk_list_pop_item_by_pred(tsk_list_t* list, tsk_list_func_predicate predicate, const void * data)
{
    if(list) {
        tsk_list_item_t *prev = tsk_null;
        tsk_list_item_t *curr = prev = list->head;

        while(curr) {
            if(!predicate(curr, data)) {
                if(prev == curr) {
                    /* Found at first position. */
                    if(list->head == list->tail) {
                        /* There was only one item */
                        list->head = list->tail = tsk_null;
                    }
                    else {
                        list->head = curr->next;
                    }
                }
                else {
                    if(curr == list->tail) {
                        /* Found at last position */
                        list->tail = prev;
                        list->tail->next = tsk_null;
                    }
                    else {
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
* Removes an item from the @ref tsk_list_t "list" using a @ref _Anchor_TinySAK_Linked_List_Predicates "predicate".
* @param list The list from which to remove the @ref tsk_list_item_t "item".
* @param predicate The @ref _Anchor_TinySAK_Linked_List_Predicates "predicate" used to match the item to remove.
* @param data Arbitrary data to pass to the @ref _Anchor_TinySAK_Linked_List_Predicates "predicate".
* @return @ref tsk_true if an item have been removed and @ref tsk_false otherwise.
*/
tsk_bool_t tsk_list_remove_item_by_pred(tsk_list_t* list, tsk_list_func_predicate predicate, const void * data)
{
    tsk_list_item_t* item;
    if((item = tsk_list_pop_item_by_pred(list, predicate, data))) {
        tsk_object_unref(item);
        return tsk_true;
    }
    return tsk_false;
}

/**@ingroup tsk_list_group
* Cleans up and remove all items from the @ref tsk_list_t "list". The reference counter for each item will be decremented. An item will be freed only if its new reference counter value is equal to 0.
* @param list The list to clean up.
*/
void tsk_list_clear_items(tsk_list_t* list)
{
    if(list) {
        tsk_list_item_t* next = tsk_null;
        tsk_list_item_t* curr = list->head;

        while(curr) {
            next = curr->next;
            tsk_object_unref(curr);
            curr = next;
        }
        list->head = tsk_null;
        list->tail = tsk_null;
    }
}

/**@ingroup tsk_list_group
* Pops the first item from the @ref tsk_list_t "list". The item will be definitely removed from the list but its reference counter will not be decremented.
* @param list The list from which to pop the item.
* @return The first item from the lit. It's up to you to free the returned item using @ref TSK_OBJECT_SAFE_FREE.
*/
tsk_list_item_t* tsk_list_pop_first_item(tsk_list_t* list)
{
    tsk_list_item_t* item = tsk_null;
    if(list) {
        item = list->head;
        if(list->head) {
            if(list->head->next) {
                list->head = list->head->next;
            }
            else {
                list->head = list->tail = tsk_null;
            }
        }
    }

    return item;
}

/**@ingroup tsk_list_group
* Adds an item to the @ref tsk_list_t "list". You should using @ref tsk_list_push_data instead of this function.
* @param list The destination @a list.
* @param item The @a item to add. The reference to the item will be stolen(set to @ref tsk_null) instead of incrementing its reference counter.
* @param back Indicates whether to put the item back or not (first).
*/
void tsk_list_push_item(tsk_list_t* list, tsk_list_item_t** item, tsk_bool_t back)
{
    // do not test
    tsk_bool_t first = !list->head;

    if(back && list->tail) {
        list->tail->next = *item, list->tail = *item, (*item)->next = tsk_null;
    }
    else {
        (*item)->next = list->head, list->head = *item;
    }

    if(first) {
        list->tail = list->head = *item, list->tail->next = tsk_null;
    }
    (*item) = tsk_null;
}

/**@ingroup tsk_list_group
* Add an item to the list in ascending or descending order. <br />
* See @ref _Anchor_TinySAK_Linked_List_Sort_Items "here" for more information.
* @param list The destination @a list.
* @param item The  @a item to add. The reference to the @a item will be stolen(set to @ref tsk_null) instead of incrementing its reference counter.
* @param ascending Indicates whether to put the @a item in ascending order or not.
*/
void tsk_list_push_filtered_item(tsk_list_t* list, tsk_list_item_t** item, tsk_bool_t ascending)
{
    if (list && item && *item) {
        tsk_list_item_t *prev = tsk_null;
        tsk_list_item_t *curr = prev = list->head;
        int diff;

        while (curr) {
            diff = tsk_object_cmp((*item), curr);
            if (diff == 0 || (diff </*=*/ 0 && ascending) || (diff >/*=*/0 && !ascending)) {
                int after = (diff == 0); // push_after(match) -> backward compatibility
                int back = (after && (curr == list->tail));
                int front = !back && (curr == list->head);

                if (back) {
                    tsk_list_push_back_item(list, item);
                }
                else if (front) {
                    tsk_list_push_front_item(list, item);
                }
                else {
                    if (after) {
                        // push_after(match)
                        (*item)->next = curr->next;
                        curr->next = (*item);
                    }
                    else {
                        // push_before(match)
                        (*item)->next = curr;
                        prev->next = (*item);
                    }
                }
                (*item) = tsk_null;
                return;
            }
            prev = curr;
            curr = curr->next;
        }
        tsk_list_push_back_item(list, item);
    }
}

/**@ingroup tsk_list_group
* Add all items in @a src into @a dest. Each item will have its reference counter incremented before being added.
* @param dest The destination list.
* @param src The source list.
* @param back Indicates whether to put the list back or not.
* @return 0 if succeed and non-zero error code otherwise.
**/
int tsk_list_push_list(tsk_list_t* dest, const tsk_list_t* src, tsk_bool_t back)
{
    const tsk_list_item_t* curr = (src)->head;
    tsk_object_t* copy;

    if(!dest || !src) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    while(curr) {
        copy = tsk_object_ref(curr->data);
        tsk_list_push_data(dest, (void**)&copy, back);

        curr = curr->next;
    }
    return 0;
}

/**@ingroup tsk_list_group
* Adds a @ref _Page_TinySAK_AnsiC_Object_Programming "well-defined" object to the @a list.
* The reference to the @a object will be stolen(set to @ref tsk_null) instead of incrementing its reference counter.
* @param list The destination @a list.
* @param tskobj The @a object to add.
* @param back Indicates whether to put the item back or not.
* @return 0 if succeed and non-zero error code otherwise.
* @sa @ref tsk_list_push_back_data @ref tsk_list_push_front_data
*/
int tsk_list_push_data(tsk_list_t* list, tsk_object_t** tskobj, tsk_bool_t back)
{
    if(list && tskobj && *tskobj) {
        tsk_list_item_t *item = tsk_list_item_create();
        item->data = *tskobj; // stolen

        tsk_list_push_item(list, &item, back);
        (*tskobj) = tsk_null;

        return 0;
    }
    else {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
}

/**@ingroup tsk_list_group
* Adds a @ref _Page_TinySAK_AnsiC_Object_Programming "well-defined" object to the @a list in ascending or descending order.
* The reference to the @a object will be stolen(set to @ref tsk_null) instead of incrementing its reference counter. <br />
* See @ref @ref _Anchor_TinySAK_Linked_List_Sort_Items "here" for more information on how to use this function.
* @param list The destination @a list.
* @param tskobj The @a tskobj to add.
* @param ascending Indicates whether to put the object in ascending order or not.
*/
int tsk_list_push_filtered_data(tsk_list_t* list, tsk_object_t** tskobj, tsk_bool_t ascending)
{
    if(list && tskobj && *tskobj) {
        tsk_list_item_t *item = tsk_list_item_create();
        item->data = *tskobj;

        tsk_list_push_filtered_item(list, &item, ascending);
        (*tskobj) = tsk_null;

        return 0;
    }
    else {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
}

/**@ingroup tsk_list_group
* Finds an item helding same object as @a tskobj from a list. A matching item will be: <i>tsk_object_cmp(item->data, tskobj) == 0</i> <br />
* See @ref _Anchor_TinySAK_Linked_List_Find_Items "here" for more information on how to use this function.
* @param list The @a list holding the item to find.
* @param tskobj The @a object to use for comparison to find the item.
* @return A @ref tsk_list_item_t "item" if found and @ref tsk_null otherwize. <br />
* Because the returned object is a constant, you <b>must</b> not try to free or decrement its reference counter.
* @sa @ref tsk_list_find_item_by_pred
*/
const tsk_list_item_t* tsk_list_find_item_by_data(const tsk_list_t* list, const tsk_object_t* tskobj)
{
    if(list && tskobj) {
        tsk_list_item_t *item;
        tsk_list_foreach(item, list) {
            if(!tsk_object_cmp(item->data, tskobj)) {
                return item;
            }
        }
    }

    return 0;
}

/**@ingroup tsk_list_group
* Finds an item from a list using a @ref _Anchor_TinySAK_Linked_List_Predicates "predicate".
* See @ref _Anchor_TinySAK_Linked_List_Find_Items "here" for more information on how to use this function.
* @param list the list from which to find the item.
* @param predicate the @ref _Anchor_TinySAK_Linked_List_Predicates "predicate" against which to test each item
* @param data data passed to the predicate function for comparison
* @return the item which match the criteria and @ref tsk_null otherwise. <br />
* Because the returned object is a constant, you <b>must</b> not try to free or decrement its reference counter.
* @sa @ref tsk_list_find_item_by_data
*/
const tsk_list_item_t* tsk_list_find_item_by_pred(const tsk_list_t* list, tsk_list_func_predicate predicate, const void* data)
{
    if(predicate) {
        const tsk_list_item_t *item;
        tsk_list_foreach(item, list) {
            if(predicate(item, data) == 0) {
                return item;
            }
        }
    }
    else {
        TSK_DEBUG_WARN("Cannot use a null predicate function");
    }
    return tsk_null;
}

/**@ingroup tsk_list_group
* Finds a @ref _Page_TinySAK_AnsiC_Object_Programming "well-defined" object (held by an item) from a list using a @ref _Anchor_TinySAK_Linked_List_Predicates "predicate".
* @param list the list from which to find the object.
* @param predicate the @ref _Anchor_TinySAK_Linked_List_Predicates "predicate" against which to test each item
* @param data data passed to the predicate function for comparaison
* @return The @ref _Page_TinySAK_AnsiC_Object_Programming "well-defined" object held by the item which match the criteria and @ref tsk_null otherwise. <br />
* Because the returned object is a constant, you <b>must</b> not try to free or decrement its reference counter.
* @sa @ref tsk_list_find_item_by_pred
*/
const tsk_object_t* tsk_list_find_object_by_pred(const tsk_list_t* list, tsk_list_func_predicate predicate, const void* data)
{
    return tsk_list_find_object_by_pred_at_index(list, predicate, data, 0);
}

const tsk_object_t* tsk_list_find_object_by_data(const tsk_list_t* list, const tsk_object_t* tskobj)
{
    const tsk_list_item_t* item = tsk_list_find_item_by_data(list, tskobj);
    if(item) {
        return (const tsk_object_t*)item->data;
    }
    return tsk_null;
}

/**@ingroup tsk_list_group */
const tsk_object_t* tsk_list_find_object_by_pred_at_index(const tsk_list_t* list, tsk_list_func_predicate predicate, const void* data, tsk_size_t index)
{
    tsk_size_t pos = 0;
    const tsk_list_item_t *item;

    tsk_list_foreach(item, list) {
        if((!predicate || predicate(item, data) == 0) && pos++ >= index) {
            return item->data;
        }
    }

    return tsk_null;
}

/**@ingroup tsk_list_group */
int tsk_list_find_index_by_pred(const tsk_list_t* list, tsk_list_func_predicate predicate, const void* data)
{
    if(list && predicate) {
        int index = 0;
        const tsk_list_item_t *item;
        tsk_list_foreach(item, list) {
            if(predicate(item, data) == 0) {
                return index;
            }
            ++index;
        }
    }
    return -1;
}

/**@ingroup tsk_list_group
* Counts the number of item matching the predicate.
* @param list The list containing the items to count
* @param predicate The predicate to use to match the items. Set to null to count all items
* @param data Data passed to the predicate function for comparaison
* @return The number of item matching the predicate
*/
tsk_size_t tsk_list_count(const tsk_list_t* list, tsk_list_func_predicate predicate, const void* data)
{
    tsk_size_t count = 0;
    if(list) {
        const tsk_list_item_t *item;
        tsk_list_foreach(item, list) {
            if(!predicate || (predicate(item, data) == 0)) {
                ++count;
            }
        }
    }
    else {
        TSK_DEBUG_ERROR("Invalid parameter");
    }

    return count;
}

/**@ingroup tsk_list_group
*/
tsk_list_t* tsk_list_clone(const tsk_list_t* list)
{
    if (list) {
        tsk_list_t*clone = tsk_list_create();
        if (clone) {
            tsk_list_pushback_list(clone, list);
        }
        return clone;
    }
    return tsk_null;
}









//=================================================================================================
//	Item object definition
//
static tsk_object_t* tsk_list_item_ctor(tsk_object_t * self, va_list * app)
{
    tsk_list_item_t *item = (tsk_list_item_t*)self;
    if(item) {
    }
    return self;
}

static tsk_object_t* tsk_list_item_dtor(tsk_object_t *self)
{
    tsk_list_item_t *item = (tsk_list_item_t*)self;
    if(item) {
        item->data = tsk_object_unref(item->data);
    }
    else {
        TSK_DEBUG_WARN("Cannot free an uninitialized item");
    }
    return item;
}

static int tsk_list_item_cmp(const tsk_object_t *_item1, const tsk_object_t *_item2)
{
    const tsk_list_item_t* item1 = (const tsk_list_item_t*)_item1;
    const tsk_list_item_t* item2 = (const tsk_list_item_t*)_item2;

    if(item1 && item2) {
        return tsk_object_cmp(item1->data, item2->data);
    }
    else {
        return -1;
    }
}

static const tsk_object_def_t tsk_list_item_def_s = {
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
    tsk_list_t *list = (tsk_list_t *)self;
    if(list) {
    }

    return self;
}

static tsk_object_t* tsk_list_dtor(tsk_object_t *self)
{
    tsk_list_t *list = (tsk_list_t *)self;
    if(list) {
#if 0
        /* Not thread-safe */
        tsk_list_item_t* next = tsk_null;
        tsk_list_item_t* curr = list->head;

        while(curr) {
            next = curr->next;
            /*curr =*/ tsk_object_unref(curr);
            curr = next;
        }
#else
        /* Thread-safe method */
        tsk_list_item_t* item;
        while((item = tsk_list_pop_first_item(list))) {
            tsk_object_unref(item);
        }
#endif

        /* destroy the on-demand mutex */
        if(list->mutex) {
            tsk_mutex_destroy(&list->mutex);
        }
    }
    else {
        TSK_DEBUG_WARN("Cannot free an uninitialized list");
    }
    return list;
}

static const tsk_object_def_t tsk_list_def_s = {
    sizeof(tsk_list_t),
    tsk_list_ctor,
    tsk_list_dtor,
    tsk_null,
};
const tsk_object_def_t *tsk_list_def_t = &tsk_list_def_s;



/**@page _Page_TinySAK_AnsiC_Linked_List Linked list
*
* - @ref _Anchor_TinySAK_Linked_List_Well_Defined_Object "Our \"well-defined\" object"
* - @ref _Anchor_TinySAK_Linked_List_Predicates "Predicates"
* - @ref _Anchor_TinySAK_Linked_List_Create_Destroy "Create and destroy the list"
* - @ref _Anchor_TinySAK_Linked_List_Thread_Safety "Thread safety"
* - @ref _Anchor_TinySAK_Linked_List_Add_Remove "Add and remove items"
* - @ref _Anchor_TinySAK_Linked_List_Dump_Items "Dump items"
* - @ref _Anchor_TinySAK_Linked_List_Sort_Items "Sort items"
* - @ref _Anchor_TinySAK_Linked_List_Find_Items "Find items"
* - @ref _Anchor_TinySAK_Linked_List_Public_Functions "Public functions"
* - @ref _Anchor_TinySAK_Linked_List_Helper_Macros "Helper macros"
*
* <hr />
*
* A linked-list is a @ref _Page_TinySAK_AnsiC_Object_Programming "well defined" object and must contain only @ref _Page_TinySAK_AnsiC_Object_Programming "well defined" objects. <br />
* A linked-list is a @ref tsk_list_t object and contains @ref tsk_list_item_t objects which holds @ref _Anchor_TinySAK_Linked_List_Well_Defined_Object "our well defined object" in its <i>data</i> field. <br />
*
* <h2>@anchor _Anchor_TinySAK_Linked_List_Well_Defined_Object Our "well-defined" object</h2>
* Our linked-list will hold a "well defined" object defined like this:
*
*@code
// (well-defined object declaration)
typedef struct student_s
{
	TSK_DECLARE_OBJECT;

	char *id;
	char *name;
}
student_t;

// (constructor)
static tsk_object_t* student_ctor(tsk_object_t* self, va_list * app)
{
	student_t* student = (tsk_object_t*)self;
	if(student){
	}
	return self;
}

// (destructor)
static tsk_object_t* student_dtor(tsk_object_t* self)
{
	student_t* student = (tsk_object_t*)self;
	TSK_FREE(student->id);
	TSK_FREE(student->name);
	return self;
}

// (case insensitive comparator)
static int student_icmp(const tsk_object_t *self, const tsk_object_t *object)
{
	const student_t* student1 = (const tsk_object_t*)self;
	const student_t* student2 = (const tsk_object_t*)object;

	if(!student1 || !student2){
		return 0;// must never happen
	}

	return tsk_stricmp(student1->id, student2->id);
}

// (well-defined object declaration)
static const tsk_object_def_t student_def_s =
{
	sizeof(student_t),
	student_ctor,
	student_dtor,
	student_icmp
};

// create a student object
static student_t* student_create(const char* id, const char* name)
{
	student_t* student;
	if((student = (student_t*)tsk_object_new(&student_def_s))){
		student->id = tsk_strdup(id);
		student->name = tsk_strdup(name);
	}
	return student;
}
*@endcode
*
*
* <h2>@anchor _Anchor_TinySAK_Linked_List_Predicates Predicates</h2>
* A predicate is a function pointer. <br />
* In the next sections we will need some predicates defined like this:
*@code
// predicate to find a student by name
static int pred_find_student_by_name(const tsk_list_item_t *item, const void *name)
{
	if(item && item->data){
		const student_t *student = (const student_t *)item->data;
		return tsk_striequals(student->name, name);
	}
	return -1;
}
// predicate to find a student by id
static int pred_find_student_by_id(const tsk_list_item_t *item, const void *id)
{
	if(item && item->data){
		const student_t *student = (const student_t *)item->data;
		return tsk_striequals(student->id, id);
	}
	return -1;
}
*@endcode
*
*
* <h2>@anchor _Anchor_TinySAK_Linked_List_Create_Destroy Create and destroy the list</h2>
* To create the list:
* @code
* tsk_list_t *list = tsk_list_create();
* @endcode
* To destroy the list:
* @code
* TSK_OBJECT_SAFE_FREE(list);
* @endcode
* To increment the reference counting:
* @code
* tsk_object_ref(list);
* @endcode
* To decrement the reference counting:
* @code
* tsk_object_unref(list);
* @endcode
*
** <h2>@anchor _Anchor_TinySAK_Linked_List_Thread_Safety Thread safety</h2>
* A linked-list is not thread-safe. It's up to you to use @ref tsk_list_lock() and @ref tsk_list_unlock() to make it thread-safe. <br />
* @ref tsk_list_lock() and @ref tsk_list_unlock() internally use a <a target=_blank href="http://en.wikipedia.org/wiki/Reentrant_mutex">reentrant mutex</a>.
*
* <h2>@anchor _Anchor_TinySAK_Linked_List_Add_Remove Add and remove items</h2>
* There are two ways to add items to the list: <br />
* First way:
* @code
* // create the well-defined object
* student_t *student = student_create("0", "student-0");
* // push the object at the end of the list
* tsk_list_push_back_data(list, (void**)&student);
* @endcode
* Second way:
* @code
* // create an item
* tsk_list_item_t* item = tsk_list_item_create();
* // fill the data field of the item
* item->data = (void*)student_create("0", "student-0");
* // push the item at the end of the list
* tsk_list_push_back_item(list, (void**)&item);
* @endcode
* You can also add a list to another list using @ref tsk_list_pushback_list() or @ref tsk_list_pushfront_list(). All items will be retained (reference counting incremented).<br />
* There are many ways to remove items. <br />
* Remove item using default comparison function:
* @code
* tsk_list_remove_item_by_data(list, student);
* @endcode
* Remove item using predicate:
* @code
* tsk_list_remove_item_by_pred(list, pred_find_student_by_id, "2"); // remove student with id equal to "2"
* @endcode
*
* Checks @ref _Anchor_TinySAK_Linked_List_Public_Functions "the public functions" for more ways to add or remove items.
*
* <h2>@anchor _Anchor_TinySAK_Linked_List_Dump_Items Dump items</h2>
* To dump the content of all items:
* @code
* const tsk_list_item_t* item;
* const student_t *student;
* tsk_list_foreach(item, list) {
*	student = (const student_t *)item->data;
*	TSK_DEBUG_INFO("id = %s, name = %s", student->id, student->name);
* }
* @endcode
*
* <h2>@anchor _Anchor_TinySAK_Linked_List_Sort_Items Sort items</h2>
* You can sort the list in ascending or descending order. The default function used for comparison is the one defined in the well-defined object (see @ref _Anchor_TinySAK_Object_Comparator "here" for more information). <br />
* An item is sorted when it's being added to the list. It's not possible to sort items later once they are added. <br />
* Sorting items using default comparison function:
* @code
// ascending order
* tsk_list_push_ascending_data(list, (void**)&student); // comparision function will be student_icmp() defined above.
// or descending order
* tsk_list_push_descending_data(list, (void**)&student); // comparision function will be student_icmp() defined above.
* @endcode
*
* Checks @ref _Anchor_TinySAK_Linked_List_Public_Functions "the public functions" for more ways to sort items.
*
* <h2>@anchor _Anchor_TinySAK_Linked_List_Find_Items Find items</h2>
* @code
* // find student with same id
* const student_t* _student = tsk_list_find_object_by_data(list, student);// comparision function will be student_icmp() defined above.
* // find an item containing a student with name equal to "student-2"
* const tsk_list_item_t *item = tsk_list_find_item_by_pred(list, pred_find_student_by_name, "student-2");
* // find a student with id equal to "2"
* const student_t *student = tsk_list_find_object_by_pred(list, pred_find_student_by_id, "2");
* @endcode
*
* Checks @ref _Anchor_TinySAK_Linked_List_Public_Functions "the public functions" for more ways to find items.
*
* <h2>@anchor _Anchor_TinySAK_Linked_List_Public_Functions Public functions </h2>
* - @ref tsk_list_count
* - @ref tsk_list_create
* - @ref tsk_list_find_index_by_pred
* - @ref tsk_list_find_item_by_data
* - @ref tsk_list_find_item_by_pred
* - @ref tsk_list_find_object_by_pred
* - @ref tsk_list_find_object_by_pred_at_index
* - @ref tsk_list_foreach
* - @ref tsk_list_item_create
* - @ref tsk_list_lock
* - @ref tsk_list_pop_first_item
* - @ref tsk_list_pop_item_by_data
* - @ref tsk_list_pop_item_by_pred
* - @ref tsk_list_push_ascending_data
* - @ref tsk_list_push_ascending_item
* - @ref tsk_list_push_back_data
* - @ref tsk_list_push_back_item
* - @ref tsk_list_push_data
* - @ref tsk_list_push_descending_data
* - @ref tsk_list_push_descending_item
* - @ref tsk_list_push_filtered_data
* - @ref tsk_list_push_filtered_item
* - @ref tsk_list_push_front_data
* - @ref tsk_list_push_front_item
* - @ref tsk_list_push_item
* - @ref tsk_list_push_list
* - @ref tsk_list_pushback_list
* - @ref tsk_list_pushfront_list
* - @ref tsk_list_remove_first_item
* - @ref tsk_list_remove_item
* - @ref tsk_list_remove_item_by_data
* - @ref tsk_list_remove_item_by_pred
* - @ref tsk_list_remove_last_item
* - @ref tsk_list_unlock
*
* <h2>@anchor _Anchor_TinySAK_Linked_List_Helper_Macros Helper macros </h2>
* - @ref TSK_LIST_IS_EMPTY
* - @ref TSK_LIST_IS_FIRST
* - @ref TSK_LIST_IS_LAST
* - @ref TSK_LIST_FIRST_DATA
* - @ref TSK_LIST_LAST_DATA
*/