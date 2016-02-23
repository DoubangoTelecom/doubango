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
#include "tnet_proxy_plugin.h"

#include "tsk_memory.h"
#include "tsk_string.h"
#include "tsk_debug.h"

#if !defined(TNET_PROXY_NODE_MAX_PLUGINS)
#   define TNET_PROXY_NODE_MAX_PLUGINS 10
#endif

const tnet_proxy_node_plugin_def_t* __tnet_proxy_node_plugins[TNET_PROXY_NODE_MAX_PLUGINS] = {0};


tsk_bool_t tnet_proxy_node_is_nettransport_supported(enum tnet_proxy_type_e proxy_type, enum tnet_socket_type_e socket_type)
{
    switch (proxy_type) {
    case tnet_proxy_type_http:
        case tnet_proxy_type_https:
                return TNET_SOCKET_TYPE_IS_STREAM(socket_type);

    case tnet_proxy_type_socks4:
    case tnet_proxy_type_socks4a:
        return TNET_SOCKET_TYPE_IS_STREAM(socket_type) && TNET_SOCKET_TYPE_IS_IPV4(socket_type);
    case tnet_proxy_type_socks5: // SOCKS5 adds support for UDP and IPv6
        return TNET_SOCKET_TYPE_IS_STREAM(socket_type) || TNET_SOCKET_TYPE_IS_DGRAM(socket_type);// for now we don't support socks for UDP (just like a browser)
    default:
        return tsk_false;
    }
}

int tnet_proxy_node_init(tnet_proxy_node_t* self)
{
    if (self) {
        self->socket.fd = TNET_INVALID_FD;
        self->socket.type = tnet_socket_type_invalid;
    }
    return 0;
}

int tnet_proxy_node_configure(tnet_proxy_node_t* self, ...)
{
    va_list ap;
    int ret = 0;

    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    va_start(ap, self);
    ret = tnet_proxy_node_configure_2(self, &ap);
    va_end(ap);
    return ret;
}

int tnet_proxy_node_configure_2(tnet_proxy_node_t* self, va_list* app)
{
    int ret = 0;
    tnet_proxy_node_param_type_t ptype;

    if (!self || !app) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    while ((ptype = va_arg(*app, tnet_proxy_node_param_type_t)) != tnet_proxy_node_param_type_null) {
        switch (ptype) {
        case tnet_proxy_node_param_type_destination_address:
        case tnet_proxy_node_param_type_proxy_address: {
            // (const char*)(HOST_STR), (int)(PORT_INT)
            const char* HOST_STR = va_arg(*app, const char*);
            int PORT_INT = va_arg(*app, int);
            if (PORT_INT < 1 || PORT_INT > 0xFFF) {
                TSK_DEBUG_ERROR("Invalid value for port number: %d", PORT_INT);
                ret = -3;
                goto bail;
            }
            if (ptype == tnet_proxy_node_param_type_destination_address) {
                tsk_strupdate(&self->dst_host, HOST_STR);
                self->dst_port = (tnet_port_t)PORT_INT;
            }
            else {
                tsk_strupdate(&self->proxy_host, HOST_STR);
                self->proxy_port = (tnet_port_t)PORT_INT;
            }
            break;
        }
        case tnet_proxy_node_param_type_ipv6: {
            /* (tsk_bool_t)(IPV6_BOOL) */
            self->ipv6 = va_arg(*app, tsk_bool_t);
            break;
        }
        case tnet_proxy_node_param_type_credentials: {
            /* (const char*)(LOGIN_STR), (const char*)(PASSWORD_STR) */
            const char* LOGIN_STR = va_arg(*app, const char*);
            const char* PASSWORD_STR = va_arg(*app, const char*);
            tsk_strupdate(&self->login, LOGIN_STR);
            tsk_strupdate(&self->password, PASSWORD_STR);
            break;
        }
        case tnet_proxy_node_param_type_socket: {
            /* (tnet_fd_t)(FD_FD), (enum tnet_socket_type_e)(type) */
            self->socket.fd = va_arg(*app, tnet_fd_t);
            self->socket.type = va_arg(*app, enum tnet_socket_type_e);
            break;
        }
#if TNET_UNDER_APPLE
        case tnet_proxy_node_param_type_cfstreams: {
            /* (CFReadStreamRef)(READ_CFSTREAM), (CFWriteStreamRef)(WRITE_CFSTREAM) */
            CFReadStreamRef READ_CFSTREAM = va_arg(*app, CFReadStreamRef);
            CFWriteStreamRef WRITE_CFSTREAM = va_arg(*app, CFWriteStreamRef);
            if (self->cf_read_stream) {
                CFRelease(self->cf_read_stream), self->cf_read_stream = tsk_null;
            }
            if (self->cf_write_stream) {
                CFRelease(self->cf_write_stream), self->cf_write_stream = tsk_null;
            }
            if (READ_CFSTREAM) {
                self->cf_read_stream = (CFReadStreamRef)CFRetain(READ_CFSTREAM);
            }
            if (WRITE_CFSTREAM) {
                self->cf_write_stream = (CFWriteStreamRef)CFRetain(WRITE_CFSTREAM);
            }
            break;
        }
#endif /* TNET_UNDER_APPLE */
        default: {
            TSK_DEBUG_ERROR("%d not valid param type", ptype);
            ret = -2;
            goto bail;
        }
        }
    }

bail:
    return ret;
}

int tnet_proxy_node_start_handshaking(tnet_proxy_node_t* self)
{
    if (!self || !self->plugin || !self->plugin->start_handshaking) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    return self->plugin->start_handshaking(self);
}

int tnet_proxy_node_set_handshaking_data(tnet_proxy_node_t* self, const void* data_ptr, tsk_size_t data_size)
{
    if (!self || !data_ptr || !data_size || !self->plugin || !self->plugin->set_handshaking_data) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    return self->plugin->set_handshaking_data(self, data_ptr, data_size);
}

int tnet_proxy_node_get_handshaking_pending_data(tnet_proxy_node_t* self, void** data_pptr, tsk_size_t* data_psize)
{
    if (!self || !data_pptr || !data_psize || !self->plugin || !self->plugin->get_handshaking_pending_data) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    return self->plugin->get_handshaking_pending_data(self, data_pptr, data_psize);
}

int tnet_proxy_node_get_handshaking_completed(tnet_proxy_node_t* self, tsk_bool_t* completed)
{
    if (!self || !completed || !self->plugin || !self->plugin->get_handshaking_completed) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    return self->plugin->get_handshaking_completed(self, completed);
}

int tnet_proxy_node_deinit(tnet_proxy_node_t* self)
{
    if (self) {
        TSK_FREE(self->dst_host);
        TSK_FREE(self->proxy_host);
        TSK_FREE(self->login);
        TSK_FREE(self->password);
#if TNET_UNDER_APPLE
        if (self->cf_read_stream) {
            CFRelease(self->cf_read_stream), self->cf_read_stream = tsk_null;
        }
        if (self->cf_write_stream) {
            CFRelease(self->cf_write_stream), self->cf_write_stream = tsk_null;
        }
#endif
    }
    return 0;
}

int tnet_proxy_node_plugin_register(const tnet_proxy_node_plugin_def_t* plugin)
{
    tsk_size_t i;
    if (!plugin || tsk_strnullORempty(plugin->desc)) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    /* add or replace the plugin */
    for (i = 0; i<TNET_PROXY_NODE_MAX_PLUGINS; i++) {
        if (!__tnet_proxy_node_plugins[i] || (__tnet_proxy_node_plugins[i] == plugin)) {
            __tnet_proxy_node_plugins[i] = plugin;
            TSK_DEBUG_INFO("Register network proxy node plugin: %s", plugin->desc);
            return 0;
        }
    }

    TSK_DEBUG_ERROR("There are already %d network proxy node plugins.", TNET_PROXY_NODE_MAX_PLUGINS);
    return -2;
}

int tnet_proxy_node_plugin_unregister(const tnet_proxy_node_plugin_def_t* plugin)
{
    tsk_size_t i;
    tsk_bool_t found = tsk_false;
    if (!plugin) {
        TSK_DEBUG_ERROR("Invalid Parameter");
        return -1;
    }

    /* find the plugin to unregister */
    for (i = 0; i<TNET_PROXY_NODE_MAX_PLUGINS && __tnet_proxy_node_plugins[i]; i++) {
        if (__tnet_proxy_node_plugins[i] == plugin) {
            TSK_DEBUG_INFO("UnRegister network proxy node plugin: %s", plugin->desc);
            __tnet_proxy_node_plugins[i] = tsk_null;
            found = tsk_true;
            break;
        }
    }

    /* compact */
    if (found) {
        for (; i<(TNET_PROXY_NODE_MAX_PLUGINS - 1); i++) {
            if (__tnet_proxy_node_plugins[i+1]) {
                __tnet_proxy_node_plugins[i] = __tnet_proxy_node_plugins[i+1];
            }
            else {
                break;
            }
        }
        __tnet_proxy_node_plugins[i] = tsk_null;
    }
    return (found ? 0 : -2);
}

tsk_size_t tnet_proxy_node_plugin_registry_count()
{
    tsk_size_t count;
    for(count = 0;
            count < TNET_PROXY_NODE_MAX_PLUGINS && __tnet_proxy_node_plugins[count];
            ++count) ;
    return count;
}

tnet_proxy_node_t* tnet_proxy_node_create(enum tnet_proxy_type_e type)
{
    tsk_size_t i;
    tnet_proxy_node_t* node = tsk_null;
    for (i = 0; i<TNET_PROXY_NODE_MAX_PLUGINS && __tnet_proxy_node_plugins[i]; i++) {
        if ((__tnet_proxy_node_plugins[i]->type & type) == type) {
            if ((node = tsk_object_new(__tnet_proxy_node_plugins[i]->objdef))) {
                node->type = type;
                node->plugin = __tnet_proxy_node_plugins[i];
                break;
            }
        }
    }
    return node;
}