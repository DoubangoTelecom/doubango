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
/**@file tnet_dns_naptr.c
 * @brief DNS Naming Authority Pointer - RR - (RFC 3403).
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tnet_dns_naptr.h"


#include "../tnet_types.h"
#include "./tnet_endianness.h"

#include "tsk_memory.h"
#include "tsk_string.h"

/** Creates new DNS NAPTR Resource Record.
*/
tnet_dns_naptr_t* tnet_dns_naptr_create(const char* name, tnet_dns_qclass_t qclass, uint32_t ttl, uint16_t rdlength, const void* data, tsk_size_t offset)
{
    return tsk_object_new(tnet_dns_naptr_def_t, name, qclass, ttl, rdlength, data, offset);
}



//=================================================================================================
//	[[DNS NAPTR]] object definition
//
static tsk_object_t* tnet_dns_naptr_ctor(tsk_object_t * self, va_list * app)
{
    tnet_dns_naptr_t *naptr = self;
    if(naptr) {
        const char* name = va_arg(*app, const char*);
        tnet_dns_qclass_t qclass = va_arg(*app, tnet_dns_qclass_t);
        uint32_t ttl = va_arg(*app, uint32_t);
        uint16_t rdlength = tsk_va_arg_u16(*app);
        const void* data = va_arg(*app, const void*);
        tsk_size_t offset = va_arg(*app, tsk_size_t);

        /* init base */
        tnet_dns_rr_init(TNET_DNS_RR(naptr), qtype_naptr, qclass);
        TNET_DNS_RR(naptr)->name = tsk_strdup(name);
        TNET_DNS_RR(naptr)->rdlength = rdlength;
        TNET_DNS_RR(naptr)->ttl = ttl;

        if(rdlength) {
            // ==> DESERIALIZATION
            /* ORDER */
            naptr->order = tnet_ntohs_2(((uint8_t*)data) + offset);
            offset += 2;
            /* PREFERENCE */
            naptr->preference = tnet_ntohs_2(((uint8_t*)data) + offset);
            offset += 2;
            /* FLAGS */
            tnet_dns_rr_charstring_deserialize(data, &(naptr->flags), &offset);
            /* SERVICES */
            tnet_dns_rr_charstring_deserialize(data, &(naptr->services), &offset);
            /* REGEXP */
            tnet_dns_rr_charstring_deserialize(data, &(naptr->regexp), &offset);
            /* REPLACEMENT */
            tnet_dns_rr_qname_deserialize(data, &(naptr->replacement), &offset);
        }
    }
    return self;
}

static tsk_object_t* tnet_dns_naptr_dtor(tsk_object_t * self)
{
    tnet_dns_naptr_t *naptr = self;
    if(naptr) {
        /* deinit base */
        tnet_dns_rr_deinit(TNET_DNS_RR(naptr));

        TSK_FREE(naptr->flags);
        TSK_FREE(naptr->services);
        TSK_FREE(naptr->regexp);
        TSK_FREE(naptr->replacement);
    }
    return self;
}

static int tnet_dns_naptr_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
    const tnet_dns_rr_t* rr1 = obj1;
    const tnet_dns_rr_t* rr2 = obj2;

    if(rr1 && rr2 && (rr1->qtype==qtype_naptr) && (rr2->qtype==qtype_naptr)) {
        const tnet_dns_naptr_t* naptr1 = (tnet_dns_naptr_t*)rr1;
        const tnet_dns_naptr_t* naptr2 = (tnet_dns_naptr_t*)rr2;

        /* Compare orders. */
        if(naptr1->order < naptr2->order) { /* Lowest order is tried first. */
            return 1;
        }
        else if(naptr1->order > naptr2->order) {
            return -1;
        }

        /* Compare preference */
        if(naptr1->order < naptr2->order) { /* Lowest preference is tried first. */
            return 1;
        }
        else if(naptr1->order > naptr2->order) {
            return -1;
        }

        return 0;
    }
    else {
        return -1;
    }
}

static const tsk_object_def_t tnet_dns_naptr_def_s = {
    sizeof(tnet_dns_naptr_t),
    tnet_dns_naptr_ctor,
    tnet_dns_naptr_dtor,
    tnet_dns_naptr_cmp,
};
const tsk_object_def_t *tnet_dns_naptr_def_t = &tnet_dns_naptr_def_s;
