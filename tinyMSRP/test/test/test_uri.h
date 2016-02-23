/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)yahoo.fr>
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
#ifndef _TEST_MSRPURI_H
#define _TEST_MSRPURI_H

#include "tinymsrp/parsers/tmsrp_parser_uri.h"

const char* uris[] = {
    "msrp://atlanta.example.com:7654/jshA7weztas;tcp;p1=1;p2=2",
    "msrps://atlanta.example.com:7654/jshA7weztas;tcp",

    // hostname, IPv4 and IPv6 (without userinfo and with port)
    "msrp://biloxi.msrp.com:12763/kjhd37s2s20w2a;sctp",
    "msrps://192.168.0.10:7654/jshA7weztas;tcp;p3=4",
    "msrps://[::127]:7654/jshA7weztas;tcp;p=0",

    // hostname, IPv4 and IPv6 (without userinfo and without port)
    "msrp://biloxi.msrp.com/kjhd37s2s20w2a;sctp",
    "msrps://192.168.0.10/jshA7weztas;tcp;p3=4",
    "msrps://[::127]/jshA7weztas;tcp;p=0",

    // hostname, IPv4 and IPv6 (with userinfo and with port)
    "msrp://bob@biloxi.msrp.com:12763/kjhd37s2s20w2a;sctp",
    "msrps://alice@192.168.0.10:7654/jshA7weztas;tcp;p3=4",
    "msrps://carol@[::127]:7654/jshA7weztas;tcp;p=0",

    // hostname, IPv4 and IPv6 (with userinfo and without port)
    "msrp://bob@biloxi.msrp.com/kjhd37s2s20w2a;sctp",
    "msrps://alice@192.168.0.10/jshA7weztas;tcp;p3=4",
    "msrps://carol@[::127]/jshA7weztas;tcp;p=0",

    // without session_id
    "msrps://[::127]:7654;tcp;p=0",
};

void test_uri_tostring(const tmsrp_uri_t *uri)
{
    char* ret = tmsrp_uri_tostring(uri);
    TSK_DEBUG_INFO("uri_to_string=%s", ret);
    TSK_FREE(ret);
}

void test_uri_parser()
{
    int i;
    tsk_list_item_t *item = 0;

    for(i=0; i<sizeof(uris)/sizeof(const char*); i++) {
        tmsrp_uri_t *uri = tmsrp_uri_parse(uris[i], strlen(uris[i]));

        printf("\n== Parsing {{ %s }} ==\n\n", uris[i]);

        if(uri) {
            printf("scheme: %s\n", uri->scheme);
            printf("userinfo: %s\n", uri->authority.userinfo);
            printf("host: %s\n", uri->authority.host);
            printf("port: %d\n", uri->authority.port);
            printf("host-type: %s\n", uri->authority.host_type == tmsrp_host_ipv4 ? "IPv4" : (uri->authority.host_type == tmsrp_host_ipv6 ? "IPv6" : (uri->authority.host_type == tmsrp_host_hostname ? "HOSTNAME" : "UNKNOWN")) );
            printf("session-id: %s\n", uri->session_id);
            printf("transport: %s\n", uri->transport);

            printf("---PARAMS---\n");

            /* dump all parameters */
            tsk_list_foreach(item, uri->params) {
                tsk_param_t* param = item->data;
                printf("-->%s=%s\n", param->name, param->value);
            }

            printf("Is-secure: %s\n", TMSRP_URI_IS_SECURE(uri) ? "YES" : "NO");

            test_uri_tostring(uri);
        }
        else {
            printf("INVALID MSRP URI.\n");
        }

        printf("\n\n");
        getchar();

        TSK_OBJECT_SAFE_FREE(uri);
    }
}


void test_uri()
{
    test_uri_parser();
}

#endif /* _TEST_MSRPURI_H */
