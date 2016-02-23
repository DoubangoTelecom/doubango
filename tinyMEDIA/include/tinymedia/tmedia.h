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

/**@file tmedia.h
 * @brief Base media object.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef TINYMEDIA_TMEDIA_H
#define TINYMEDIA_TMEDIA_H

#include "tinymedia_config.h"

#include "tinysdp/tsdp_message.h"

#include "tnet_socket.h"

#include "tsk_params.h"

TMEDIA_BEGIN_DECLS


#if 0

#define TMEDIA_VA_ARGS(name, host, socket_type)		tmedia_def_t, (const char*) name, (const char*) host, (tnet_socket_type_t) socket_type

#define TMEDIA(self)		((tmedia_t*)(self))

typedef enum tmedia_action_e {
    // Dummy
    tma_dummy_say_hello,

    // MSRP
    tma_msrp_send_data,
    tma_msrp_send_file,

    // Audio / Video

    // T.38
}
tmedia_action_t;

typedef struct tmedia_s {
    TSK_DECLARE_OBJECT;

    const struct tmedia_plugin_def_s* plugin;

    char* name;
    uint32_t port;
    char* protocol;
}
tmedia_t;
typedef tsk_list_t tmedias_L_t;

#define TMED_DECLARE_MEDIA tmedia_t media

typedef struct tmedia_plugin_def_s {
    const tsk_object_def_t* objdef;
    const char* name;
    const char* media;

    int	(* start) (tmedia_t* );
    int	(* pause) (tmedia_t* );
    int	(* stop) (tmedia_t* );

    const tsdp_header_M_t* (* get_local_offer) (tmedia_t* , va_list* );
    const tsdp_header_M_t* (* get_negotiated_offer) (tmedia_t* );
    int (* set_remote_offer) (tmedia_t* , const tsdp_message_t* );

    int (* perform) (tmedia_t* , tmedia_action_t action, const tsk_params_L_t* );
}
tmedia_plugin_def_t;

TINYMEDIA_API int tmedia_init(tmedia_t* self, const char* name);
TINYMEDIA_API int tmedia_deinit(tmedia_t* self);

TINYMEDIA_API int tmedia_plugin_register(const tmedia_plugin_def_t* def);
TINYMEDIA_API tmedia_t* tmedia_factory_create(const char* name, const char* host, tnet_socket_type_t socket_type);

TINYMEDIA_API int tmedia_start(tmedia_t* );
TINYMEDIA_API int tmedia_pause(tmedia_t* );
TINYMEDIA_API int tmedia_stop(tmedia_t* );

TINYMEDIA_API const tsdp_header_M_t* tmedia_get_local_offer(tmedia_t* , ...);
TINYMEDIA_API const tsdp_header_M_t* tmedia_get_negotiated_offer(tmedia_t* );
TINYMEDIA_API int tmedia_set_remote_offer(tmedia_t* , const tsdp_message_t* );

TINYMEDIA_API int tmedia_perform(tmedia_t* , tmedia_action_t, ... );

TINYMEDIA_GEXTERN const void *tmedia_def_t;

#endif

TMEDIA_END_DECLS


#endif /* TINYMEDIA_TMEDIA_H */
