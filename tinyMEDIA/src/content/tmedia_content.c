/*
* Copyright (C) 2010-2015 Mamadou Diop.
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

/**@file tmedia_content.c
 * @brief Base content object.
 */
#include "tinymedia/content/tmedia_content.h"

#include "tsk_memory.h"
#include "tsk_string.h"
#include "tsk_debug.h"

#include <string.h>

/**@defgroup tmedia_content_group Contents
*/

typedef struct tmedia_content_plugin_entry_s {
    const char* type;
    const tmedia_content_plugin_def_t* plugin;
}
tmedia_content_plugin_entry_t;

/* pointer to all registered contents */
tmedia_content_plugin_entry_t __tmedia_content_plugin_entries[TMEDIA_CONTENT_MAX_PLUGINS][1] = { tsk_null };


int tmedia_content_plugin_register(const char* type, const tmedia_content_plugin_def_t* plugin)
{
    tsk_size_t i;
    //-- int a = sizeof(__tmedia_content_plugin_entries);
    if (!plugin || !plugin) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    /* add or replace the plugin */
    for (i = 0; i < TMEDIA_CONTENT_MAX_PLUGINS; i++) {
        if (!__tmedia_content_plugin_entries[i]->plugin || (__tmedia_content_plugin_entries[i]->plugin == plugin && tsk_striequals(type, __tmedia_content_plugin_entries[i]->type))) {
            __tmedia_content_plugin_entries[i]->type = type;
            __tmedia_content_plugin_entries[i]->plugin = plugin;
            return 0;
        }
    }


    TSK_DEBUG_ERROR("There are already %d plugins.", TMEDIA_CONTENT_MAX_PLUGINS);
    return -2;
}

int tmedia_content_plugin_unregister(const char* type, const tmedia_content_plugin_def_t* plugin)
{
    tsk_size_t i;
    tsk_bool_t found = tsk_false;
    if (!plugin) {
        TSK_DEBUG_ERROR("Invalid Parameter");
        return -1;
    }

    /* find the plugin to unregister */
    for (i = 0; i < TMEDIA_CONTENT_MAX_PLUGINS && __tmedia_content_plugin_entries[i]->plugin; i++) {
        if (__tmedia_content_plugin_entries[i]->plugin == plugin && tsk_striequals(type, __tmedia_content_plugin_entries[i]->type)) {
            __tmedia_content_plugin_entries[i]->type = tsk_null,
                                                __tmedia_content_plugin_entries[i]->plugin = tsk_null;
            found = tsk_true;
            break;
        }
    }

    /* compact */
    if (found) {
        for (; i < (TMEDIA_CONTENT_MAX_PLUGINS - 1); i++) {
            if (__tmedia_content_plugin_entries[i + 1]->plugin) {
                __tmedia_content_plugin_entries[i]->type = __tmedia_content_plugin_entries[i + 1]->type,
                                                    __tmedia_content_plugin_entries[i]->plugin = __tmedia_content_plugin_entries[i + 1]->plugin;
            }
            else {
                break;
            }
        }
        __tmedia_content_plugin_entries[i]->type = tsk_null,
                                            __tmedia_content_plugin_entries[i]->plugin = tsk_null;
    }
    return (found ? 0 : -2);
}

int tmedia_content_plugin_unregister_all()
{
    tsk_size_t i;
    for (i = 0; i < TMEDIA_CONTENT_MAX_PLUGINS && __tmedia_content_plugin_entries[i]->plugin; i++) {
        __tmedia_content_plugin_entries[i]->type = tsk_null,
                                            __tmedia_content_plugin_entries[i]->plugin = tsk_null;
    }
    return 0;
}

tmedia_content_t* tmedia_content_create(const char* type)
{
    tmedia_content_t* content = tsk_null;
    const tmedia_content_plugin_entry_t* entry;
    tsk_size_t i = 0;

    while (i < TMEDIA_CONTENT_MAX_PLUGINS) {
        entry = __tmedia_content_plugin_entries[i];
        if (!entry->plugin || !entry->type) {
            break;
        }
        if (entry->plugin->objdef && tsk_striequals(entry->type, type)) {
            if ((content = tsk_object_new(entry->plugin->objdef))) {
                content->plugin = entry->plugin;
                content->type = entry->type;
                return content;
            }
        }
        ++i;
    }

    TSK_DEBUG_WARN("Failed to find content type (%s) will be added as dummy", type);
    if (tmedia_content_dummy_plugin_def_t) {
        content = tsk_object_new(tmedia_content_dummy_plugin_def_t->objdef);
        content->plugin = tmedia_content_dummy_plugin_def_t;
        content->type = type;
    }

    return content;
}

tmedia_content_t* tmedia_content_parse(const void* data, tsk_size_t size, const char* type)
{
    tmedia_content_t* content = tmedia_content_create(type);
    if (content) {
        if (content->plugin->parse) {
            int ret;
            if ((ret = content->plugin->parse(content, data, size))) {
                TSK_DEBUG_ERROR("Failed to parse the content(%d)", ret);
                TSK_OBJECT_SAFE_FREE(content);
                return tsk_null;
            }
            return content;
        }
        else {
            TSK_DEBUG_ERROR("No parser function for this content (%s)", type);
            TSK_OBJECT_SAFE_FREE(content);
            return tsk_null;
        }
    }
    else {
        TSK_DEBUG_ERROR("Failed to to find content(%s)", type);
        return tsk_null;
    }
}

int tmedia_content_init(tmedia_content_t* self)
{
    if (!self || !self->plugin) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    return 0;
}

int tmedia_content_deinit(tmedia_content_t* self)
{
    if (!self || !self->plugin) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    return 0;
}

tsk_buffer_t* tmedia_content_get_data(tmedia_content_t* self)
{
    if (!self || !self->plugin) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }
    return self->plugin->get_data(self);
}




static int tmedia_content_dummy_parse(tmedia_content_t* self, const void* in_data, tsk_size_t in_size)
{
    tmedia_content_dummy_t *dummy = TMEDIA_CONTENT_DUMMY(self);
    if (!dummy || dummy->data) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    dummy->data = tsk_buffer_create(in_data, in_size);
    return 0;
}

static tsk_buffer_t* tmedia_content_dummy_get_data(tmedia_content_t* self)
{
    return tsk_object_ref(TMEDIA_CONTENT_DUMMY(self)->data);
}






//=================================================================================================
//	object/plugin definitions
//
/* constructor */
static tsk_object_t* tmedia_content_dummy_ctor(tsk_object_t * self, va_list * app)
{
    tmedia_content_dummy_t *dummy = self;
    if (dummy) {
        /* init base: called by tmedia_content_create() */
        /* init self */
    }
    return self;
}
/* destructor */
static tsk_object_t* tmedia_content_dummy_dtor(tsk_object_t * self)
{
    tmedia_content_dummy_t *dummy = self;
    if (dummy) {
        /* deinit base */
        tmedia_content_deinit(TMEDIA_CONTENT(dummy));
        /* deinit self */
        TSK_OBJECT_SAFE_FREE(dummy->data);
    }

    return self;
}
/* object definition */
static const tsk_object_def_t tmedia_content_dummy_def_s = {
    sizeof(tmedia_content_dummy_t),
    tmedia_content_dummy_ctor,
    tmedia_content_dummy_dtor,
    tsk_null,
};
/* plugin definition*/
static const tmedia_content_plugin_def_t tmedia_content_dummy_plugin_def_s = {
    &tmedia_content_dummy_def_s,

    "dummy",
    tmedia_content_dummy_parse,
    tmedia_content_dummy_get_data
};
const tmedia_content_plugin_def_t *tmedia_content_dummy_plugin_def_t = &tmedia_content_dummy_plugin_def_s;



//=================================================================================================
//	media content header
//

tmedia_content_header_t* tmedia_content_header_create(const char* name, const char* value)
{
    tmedia_content_header_t* header = tsk_object_new(tmedia_content_header_def_t);
    const char* str;

    if (!header) {
        TSK_DEBUG_ERROR("Failed to create new header object");
        return tsk_null;
    }
    header->name = tsk_strdup(name);
    if (value && (str = strstr(value, ";"))) {
        header->value = tsk_strndup(value, (tsk_size_t)(str - value));
        header->params = tsk_params_fromstring((str + 1), ";", tsk_true);
    }
    else {
        header->value = tsk_strdup(value);
    }

    return header;
}

int tmedia_content_header_deinit(tmedia_content_header_t* self)
{
    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    TSK_FREE(self->name);
    TSK_FREE(self->value);
    TSK_OBJECT_SAFE_FREE(self->params);

    return 0;
}

char* tmedia_content_header_tostring(const tmedia_content_header_t* self)
{
    char* string = tsk_null;

    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }

    tsk_sprintf(&string, "%s: %s", self->name, self->value);
    if (self->params) {
        const tsk_list_item_t* item;
        tsk_list_foreach(item, self->params) {
            tsk_strcat_2(&string, ";%s=%s", TSK_PARAM(item->data)->name, TSK_PARAM(item->data)->value);
        }
    }

    return string;
}

/* constructor */
static tsk_object_t* tmedia_content_header_ctor(tsk_object_t * self, va_list * app)
{
    tmedia_content_header_t *header = self;
    if (header) {
    }
    return self;
}
/* destructor */
static tsk_object_t* tmedia_content_header_dtor(tsk_object_t * self)
{
    tmedia_content_header_t *header = self;
    if (header) {
        tmedia_content_header_deinit(header);
    }

    return self;
}
/* object definition */
static const tsk_object_def_t tmedia_content_header_def_s = {
    sizeof(tmedia_content_header_t),
    tmedia_content_header_ctor,
    tmedia_content_header_dtor,
    tsk_null,
};
const tsk_object_def_t *tmedia_content_header_def_t = &tmedia_content_header_def_s;
