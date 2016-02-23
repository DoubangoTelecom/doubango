/*
 * Copyright (C) 2010-2015 Mamadou DIOP.
 * Copyright (C) 2015 Doubango Telecom.
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
#if !defined(TNET_PROXY_PLUGIN_H)
#define TNET_PROXY_PLUGIN_H

#include "tinynet_config.h"
#include "tnet_types.h"
#include "tnet_socket.h"

#include "tsk_list.h"

#if TNET_UNDER_APPLE
#   import <CFNetwork/CFNetwork.h>
#endif /* TNET_UNDER_APPLE */

TNET_BEGIN_DECLS

typedef struct tnet_proxy_node_s {
    TSK_DECLARE_OBJECT;

    enum tnet_proxy_type_e type;
    tsk_bool_t ipv6;
    char* dst_host;
    tnet_port_t dst_port;
    char* proxy_host;
    tnet_port_t proxy_port;
    char* login;
    char* password;
    struct {
        tnet_fd_t fd;
        tnet_socket_type_t type;
    } socket;
#if TNET_UNDER_APPLE
    CFReadStreamRef cf_read_stream;
    CFWriteStreamRef cf_write_stream;
#endif /* TNET_UNDER_APPLE */

    const struct tnet_proxy_node_plugin_def_s* plugin;
}
tnet_proxy_node_t;

#define TNET_PROXY_NODE(self) ((tnet_proxy_node_t*)(self))
#define TNET_DECLARE_PROXY_NONE tnet_proxy_node_t __node__

typedef enum tnet_proxy_node_param_type_e {
    tnet_proxy_node_param_type_null = 0,
    tnet_proxy_node_param_type_destination_address,
    tnet_proxy_node_param_type_proxy_address,
    tnet_proxy_node_param_type_ipv6,
    tnet_proxy_node_param_type_credentials,
    tnet_proxy_node_param_type_socket,
#if TNET_UNDER_APPLE
    tnet_proxy_node_param_type_cfstreams,
#endif
}
tnet_proxy_node_param_type_t;
#define TNET_PROXY_NODE_SET_NULL()    tnet_proxy_node_param_type_null
#define TNET_PROXY_SET_DEST_ADDRESS(HOST_STR, PORT_INT)    tnet_proxy_node_param_type_destination_address, (const char*)(HOST_STR), (int)(PORT_INT)
#define TNET_PROXY_SET_PROXY_ADDRESS(HOST_STR, PORT_INT)    tnet_proxy_node_param_type_proxy_address, (const char*)(HOST_STR), (int)(PORT_INT)
#define TNET_PROXY_NODE_SET_IPV6(IPV6_BOOL) tnet_proxy_node_param_type_ipv6, (tsk_bool_t)(IPV6_BOOL)
#define TNET_PROXY_SET_CREDENTIALS(LOGIN_STR, PASSWORD_STR) tnet_proxy_node_param_type_credentials, (const char*)(LOGIN_STR), (const char*)(PASSWORD_STR)
#define TNET_PROXY_SET_SOCKET(FD_FD, type) tnet_proxy_node_param_type_socket, (tnet_fd_t)(FD_FD), (enum tnet_socket_type_e)(type)

#if TNET_UNDER_APPLE
#   define TNET_PROXY_SET_CFSTREAM(READ_CFSTREAM, WRITE_CFSTREAM) tnet_proxy_node_param_type_cfstreams, (CFReadStreamRef)(READ_CFSTREAM), (CFWriteStreamRef)(WRITE_CFSTREAM)
#endif /* TNET_UNDER_APPLE */

/** Virtual table used to define a proxy node plugin */
typedef struct tnet_proxy_node_plugin_def_s {
    //! object definition used to create an instance of the plugin
    const tsk_object_def_t* objdef;

    //! plugin type
    enum tnet_proxy_type_e type;

    //! full description (usefull for debugging)
    const char* desc;

    int (* configure) (tnet_proxy_node_t* self, ...);
    int (* start_handshaking) (tnet_proxy_node_t* self);
    int (* set_handshaking_data) (tnet_proxy_node_t* self, const void* data_ptr, tsk_size_t data_size);
    int (* get_handshaking_pending_data) (tnet_proxy_node_t* self, void** data_pptr, tsk_size_t* data_psize);
    int (* get_handshaking_completed) (tnet_proxy_node_t* self, tsk_bool_t* completed);
}
tnet_proxy_node_plugin_def_t;

TINYNET_API tsk_bool_t tnet_proxy_node_is_nettransport_supported(enum tnet_proxy_type_e proxy_type, enum tnet_socket_type_e socket_type);
TINYNET_API int tnet_proxy_node_init(tnet_proxy_node_t* self);
TINYNET_API int tnet_proxy_node_configure(tnet_proxy_node_t* self, ...);
TINYNET_API int tnet_proxy_node_configure_2(tnet_proxy_node_t* self, va_list* app);
TINYNET_API int tnet_proxy_node_start_handshaking(tnet_proxy_node_t* self);
TINYNET_API int tnet_proxy_node_set_handshaking_data(tnet_proxy_node_t* self, const void* data_ptr, tsk_size_t data_size);
TINYNET_API int tnet_proxy_node_get_handshaking_pending_data(tnet_proxy_node_t* self, void** data_pptr, tsk_size_t* data_psize);
TINYNET_API int tnet_proxy_node_get_handshaking_completed(tnet_proxy_node_t* self, tsk_bool_t* completed);
TINYNET_API int tnet_proxy_node_deinit(tnet_proxy_node_t* self);

TINYNET_API int tnet_proxy_node_plugin_register(const tnet_proxy_node_plugin_def_t* plugin);
TINYNET_API int tnet_proxy_node_plugin_unregister(const tnet_proxy_node_plugin_def_t* plugin);
TINYNET_API tsk_size_t tnet_proxy_node_plugin_registry_count();
TINYNET_API tnet_proxy_node_t* tnet_proxy_node_create(enum tnet_proxy_type_e type);

TNET_END_DECLS

#endif /* TNET_PROXY_PLUGIN_H */

