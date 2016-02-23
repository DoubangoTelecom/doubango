/*
* Copyright (C) 2010-2015 Mamadou DIOP.
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
#ifndef TNET_PROXYDETECT_H
#define TNET_PROXYDETECT_H

#include "tinynet_config.h"
#include "tnet_types.h"
#include "tnet_socket.h"

TNET_BEGIN_DECLS

typedef struct tnet_proxyinfo_s {
    TSK_DECLARE_OBJECT;

    tnet_proxy_type_t type;
    char* autoconfig_url;
    tsk_bool_t autodetect;
    char* bypass_list;
    char* hostname;
    tnet_socket_type_t socket_type;
    tnet_port_t port;
    char* username;
    char* password;
}
tnet_proxyinfo_t;

TINYNET_API tnet_proxyinfo_t* tnet_proxyinfo_create();
TINYNET_API tnet_proxyinfo_t* tnet_proxydetect_get_info(const char* url, tnet_socket_type_t socket_type, tsk_bool_t long_operation);
#define tnet_proxydetect_get_info_fast(url, socket_type) tnet_proxydetect_get_info((url), (socket_type), tsk_false)
TINYNET_API tsk_bool_t tnet_proxyinfo_is_valid(const tnet_proxyinfo_t* self);

TNET_END_DECLS

#endif /* TNET_PROXYDETECT_H */
