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
/**@file tnet_dns_srv.c
 * @brief DNS SeRVice locator record - RR - (RFC 2782).
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tnet_dns_srv.h"

#include "../tnet_types.h"
#include "../tnet_endianness.h"

#include "tsk_string.h"
#include "tsk_memory.h"

/** Creates new DNS SRV Resource Record.
*/
tnet_dns_srv_t* tnet_dns_srv_create(const char* name, tnet_dns_qclass_t qclass, uint32_t ttl, uint16_t rdlength, const void* data, tsk_size_t offset)
{
    return tsk_object_new(tnet_dns_srv_def_t, name, qclass, ttl, rdlength, data, offset);
}

//=================================================================================================
//	[[DNS SRV]] object definition
//
static tsk_object_t* tnet_dns_srv_ctor(tsk_object_t * self, va_list * app)
{
    tnet_dns_srv_t *srv = self;
    if(srv) {
        const char* name = va_arg(*app, const char*);
        tnet_dns_qclass_t qclass = va_arg(*app, tnet_dns_qclass_t);
        uint32_t ttl = va_arg(*app, uint32_t);
        uint16_t rdlength = tsk_va_arg_u16(*app);
        const void* data = va_arg(*app, const void*);
        tsk_size_t offset = va_arg(*app, tsk_size_t);

        /* init base */
        tnet_dns_rr_init(TNET_DNS_RR(srv), qtype_srv, qclass);
        TNET_DNS_RR(srv)->name = tsk_strdup(name);
        TNET_DNS_RR(srv)->rdlength = rdlength;
        TNET_DNS_RR(srv)->ttl = ttl;

        if(rdlength) {
            // ==> DESERIALIZATION
            /* Priority */
            srv->priority = tnet_ntohs_2(((uint8_t*)data) + offset),
                 offset += 2;
            /* Weight */
            srv->weight = tnet_ntohs_2(((uint8_t*)data) + offset),
                 offset += 2;
            /* Port */
            srv->port = tnet_ntohs_2(((uint8_t*)data) + offset),
                 offset += 2;
            /* Target */
            tnet_dns_rr_qname_deserialize(data, &(srv->target), &offset);
        }
    }
    return self;
}

static tsk_object_t* tnet_dns_srv_dtor(tsk_object_t * self)
{
    tnet_dns_srv_t *srv = self;
    if(srv) {
        /* deinit base */
        tnet_dns_rr_deinit(TNET_DNS_RR(srv));

        TSK_FREE(srv->target);
    }
    return self;
}

static int tnet_dns_srv_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
    const tnet_dns_rr_t* rr1 = obj1;
    const tnet_dns_rr_t* rr2 = obj2;

    if(rr1 && rr2 && (rr1->qtype==qtype_srv) && (rr2->qtype==qtype_srv)) {
        const tnet_dns_srv_t* srv1 = (tnet_dns_srv_t*)rr1;
        const tnet_dns_srv_t* srv2 = (tnet_dns_srv_t*)rr2;

        /* Compare priorities. */
        if(srv1->priority < srv2->priority) { /* Lowest priority is tried first. */
            return 1;
        }
        else if(srv1->priority > srv2->priority) {
            return -1;
        }

        /* Compare weight */
        if(srv1->weight > srv2->weight) {
            return 1;
        }
        else if(srv1->weight < srv2->weight) {
            return -1;
        }

        return 0;
    }
    else {
        return -1;
    }
}

static const tsk_object_def_t tnet_dns_srv_def_s = {
    sizeof(tnet_dns_srv_t),
    tnet_dns_srv_ctor,
    tnet_dns_srv_dtor,
    tnet_dns_srv_cmp,
};
const tsk_object_def_t *tnet_dns_srv_def_t = &tnet_dns_srv_def_s;
