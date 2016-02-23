/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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

/**@file tsk_options.c
 * @brief Options.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tsk_options.h"
#include "tsk_memory.h"
#include "tsk_string.h"
#include "tsk_common.h"

#include <string.h>

/**@defgroup tsk_options_group Options.
*/

/** Predicate function used to find an option by id.
*/
static int pred_find_option_by_id(const tsk_list_item_t *item, const void *id)
{
    if(item && item->data) {
        tsk_option_t *option = (tsk_option_t*)item->data;
        return (option->id - *((int*)id));
    }
    return -1;
}

/**@ingroup tsk_options_group
*/
tsk_option_t* tsk_option_create(int id, const char* value)
{
    return (tsk_option_t*)tsk_object_new(TSK_OPTION_VA_ARGS(id, value));
}

/**@ingroup tsk_options_group
*/
tsk_option_t* tsk_option_create_null()
{
    return tsk_option_create(0, tsk_null);
}


/**@ingroup tsk_options_group
* Checks if the supplied list of options contains an option with this @a id.
* @param self The list of options into which to search.
* @param id The id of the option to search.
* @retval @ref tsk_true if the parameter exist and @ref tsk_false otherwise.
*/
tsk_bool_t tsk_options_have_option(const tsk_options_L_t *self, int id)
{
    if(self) {
        if(tsk_list_find_item_by_pred(self, pred_find_option_by_id, &id)) {
            return tsk_true;
        }
    }
    return tsk_false;
}

/**@ingroup tsk_options_group
* Adds an option to the list of options. If the option already exist(same id), then it's value will be updated.
* @param self The destination list.
* @param id The id of the option to add.
* @param value The value of the option to add.
* @retval Zero if succeed and -1 otherwise.
*/
int tsk_options_add_option(tsk_options_L_t **self, int id, const char* value)
{
    tsk_option_t *option;

    if(!self) {
        return -1;
    }

    if(!*self) {
        *self = tsk_list_create();
    }

    if((option = (tsk_option_t*)tsk_options_get_option_by_id(*self, id))) {
        tsk_strupdate(&option->value, value); /* Already exist ==> update the value. */
    }
    else {
        option = tsk_option_create(id, value);
        tsk_list_push_back_data(*self, (void**)&option);
    }

    return 0;
}

int tsk_options_add_option_2(tsk_options_L_t **self, const tsk_option_t* option)
{
    int ret = -1;
    if(!self || !option || !option) {
        return ret;
    }

    ret = tsk_options_add_option(self, option->id, option->value);
    return ret;
}

/**@ingroup tsk_options_group
* Removes an option from the list of options.
* @param self The source list.
* @param id The id of the option to remove.
* @retval Zero if succeed and -1 otherwise.
*/
int tsk_options_remove_option(tsk_options_L_t *self, int id)
{
    if(self) {
        tsk_list_remove_item_by_pred(self, pred_find_option_by_id, &id);
        return 0;
    }
    return -1;
}

/**@ingroup tsk_options_group
* Gets an option from the list of options by id.
* @param self The source list.
* @param id The id of the option to retrieve.
* @retval @ref tsk_option_t if succeed and NULL otherwise.
*/
const tsk_option_t *tsk_options_get_option_by_id(const tsk_options_L_t *self, int id)
{
    if(self) {
        const tsk_list_item_t *item_const = tsk_list_find_item_by_pred(self, pred_find_option_by_id, &id);
        if(item_const) {
            return (const tsk_option_t*)item_const->data;
        }
    }
    return 0;
}

/**@ingroup tsk_options_group
* Gets the value of a option.
* @param self The source list.
* @param id The id of the option to retrieve.
* @retval The value of the option if succeed and @ref tsk_null otherwise.
*/
const char *tsk_options_get_option_value(const tsk_options_L_t *self, int id)
{
    if(self) {
        const tsk_list_item_t *item_const = tsk_list_find_item_by_pred(self, pred_find_option_by_id, &id);
        if(item_const && item_const->data) {
            return ((const tsk_option_t *)item_const->data)->value;
        }
    }
    return tsk_null;
}

/**@ingroup tsk_options_group
* Gets the value of a option.
* @param self The source list.
* @param id The id of the option to retrieve.
* @retval The value of the option if succeed and -1 otherwise.
*/
int tsk_options_get_option_value_as_int(const tsk_options_L_t *self, int id)
{
    const char *value = tsk_options_get_option_value(self, id);
    return value ? atoi(value) : -1;
}






















//=================================================================================================
//	option object definition
//
static tsk_object_t* tsk_option_ctor(tsk_object_t * self, va_list * app)
{
    tsk_option_t *option = (tsk_option_t*)self;
    if(option) {
        int id = va_arg(*app, int);
        const char* value = va_arg(*app, const char *);

        option->id = id;
        if(!tsk_strnullORempty(value)) {
            option->value = tsk_strdup(value);
        }
    }

    return self;
}

static tsk_object_t* tsk_option_dtor(tsk_object_t * self)
{
    tsk_option_t *option = (tsk_option_t*)self;
    if(option) {
        TSK_FREE(option->value);
    }

    return self;
}

static const tsk_object_def_t tsk_option_def_s = {
    sizeof(tsk_option_t),
    tsk_option_ctor,
    tsk_option_dtor,
    tsk_null,
};
const tsk_object_def_t* tsk_option_def_t = &tsk_option_def_s;

