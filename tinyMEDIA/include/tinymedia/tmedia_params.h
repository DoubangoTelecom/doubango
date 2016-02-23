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

/**@file tmedia_params.h
 * @brief Media parameters used to configure any session or plugin.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef TINYMEDIA_PARAMS_H
#define TINYMEDIA_PARAMS_H

#include "tinymedia_config.h"

#include "tinymedia/tmedia_common.h"

#include "tsk_list.h"

TMEDIA_BEGIN_DECLS

#define TMEDIA_PARAM(self) ((tmedia_param_t*)(self))

typedef enum tmedia_param_access_type_e {
    tmedia_pat_get,
    tmedia_pat_set
}
tmedia_param_access_type_t;

typedef enum tmedia_param_plugin_type_e {
    tmedia_ppt_consumer,
    tmedia_ppt_producer,
    tmedia_ppt_codec,
    tmedia_ppt_session,
    tmedia_ppt_manager
}
tmedia_param_plugin_type_t;

typedef enum tmedia_param_value_type_e {
    tmedia_pvt_int32,
    tmedia_pvt_bool = tmedia_pvt_int32,
    tmedia_pvt_pobject,
    tmedia_pvt_pchar,
    tmedia_pvt_int64,
}
tmedia_param_value_type_t;

#define TMEDIA_PARAM_VALUE_TYPE_IS_PTR(self) ((self) == tmedia_pvt_pobject || (self) == tmedia_pvt_pchar)

typedef struct tmedia_param_s {
    TSK_DECLARE_OBJECT;

    tmedia_param_access_type_t access_type;
    tmedia_type_t media_type;
    tmedia_param_plugin_type_t plugin_type;
    tmedia_param_value_type_t value_type;

    char* key;
    /* Because setting parameters could be deferred
    * ==> MUST copy the value for later use.
    * e.g. TMEDIA_SESSION_MANAGER_SET_INT32("width", 1234); 1234 will be lost when we exit the block code
    */
    void* value;
}
tmedia_param_t;

typedef tsk_list_t tmedia_params_L_t; /**< List of @ref tsk_param_t elements. */

#define tmedia_params_create() tsk_list_create()

TINYMEDIA_API tmedia_param_t* tmedia_param_create(tmedia_param_access_type_t access_type,
        tmedia_type_t media_type,
        tmedia_param_plugin_type_t plugin_type,
        tmedia_param_value_type_t value_type,
        const char* key,
        void* value);
#define tmedia_param_create_get(media_type, plugin_type, value_type, key, value) tmedia_param_create(tmedia_pat_get, (media_type), (plugin_type), (value_type), (key), (value))
#define tmedia_param_create_get_session(media_type, value_type, key, value) tmedia_param_create_get((media_type), tmedia_ppt_session, (value_type), (key), (value))
#define tmedia_param_create_get_codec(media_type, value_type, key, value) tmedia_param_create_get((media_type), tmedia_ppt_codec, (value_type), (key), (value))
#define tmedia_param_create_set(media_type, plugin_type, value_type, key, value) tmedia_param_create(tmedia_pat_set, (media_type), (plugin_type), (value_type), (value))

TINYMEDIA_API tmedia_params_L_t* tmedia_params_create_2(va_list *app);

TINYMEDIA_API int tmedia_params_add_param(tmedia_params_L_t **self,
        tmedia_param_access_type_t access_type,
        tmedia_type_t media_type,
        tmedia_param_plugin_type_t plugin_type,
        tmedia_param_value_type_t value_type,
        const char* key,
        void* value);

TINYMEDIA_GEXTERN const tsk_object_def_t *tmedia_param_def_t;

TMEDIA_END_DECLS

#endif /* TINYMEDIA_PARAMS_H */
