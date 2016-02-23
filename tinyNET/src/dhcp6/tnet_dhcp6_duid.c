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
/**@file tnet_dhcp6_duid.c
 * @brief DHCPv6 (RFC 3315) DHCP Unique Identifier (DUID) as defined in subclause 9.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tnet_dhcp6_duid.h"

#include "../tnet_types.h"
#include "../tnet_endianness.h"

int tnet_dhcp6_duid_llt_serialize(const tnet_dhcp6_duid_llt_t* self, tsk_buffer_t *output);
int tnet_dhcp6_duid_en_serialize(const tnet_dhcp6_duid_en_t* self, tsk_buffer_t *output);
int tnet_dhcp6_duid_ll_serialize(const tnet_dhcp6_duid_ll_t* self, tsk_buffer_t *output);


tnet_dhcp6_duid_llt_t* tnet_dhcp6_duid_llt_create(const void* payload, tsk_size_t payload_size)
{
    return tsk_object_new(tnet_dhcp6_duid_llt_def_t, payload, payload_size);
}

tnet_dhcp6_duid_en_t* tnet_dhcp6_duid_en_create(const void* payload, tsk_size_t payload_size)
{
    return tsk_object_new(tnet_dhcp6_duid_en_def_t, payload, payload_size);
}

tnet_dhcp6_duid_ll_t* tnet_dhcp6_duid_ll_create(const void* payload, tsk_size_t payload_size)
{
    return tsk_object_new(tnet_dhcp6_duid_ll_def_t, payload, payload_size);
}

int tnet_dhcp6_duid_init(tnet_dhcp6_duid_t *self, tnet_dhcp6_duid_type_t type)
{
    if(self) {
        if(!self->initialized) {
            self->type = type;
            self->initialized = tsk_true;
            return 0;
        }
        return -2;
    }
    return -1;
}

int tnet_dhcp6_duid_deinit(tnet_dhcp6_duid_t *self)
{
    if(self) {
        if(self->initialized) {
            self->initialized = tsk_true;
            return 0;
        }
        return -2;
    }
    return -1;
}

tnet_dhcp6_duid_t* tnet_dhcp6_duid_deserialize(const void* data, tsk_size_t size)
{
    tnet_dhcp6_duid_t *duid = 0;
    uint8_t* dataPtr = ((uint8_t*)data);
    //uint8_t* dataEnd = (dataPtr+size);

    tnet_dhcp6_duid_type_t type;
    //uint8_t len = 0;

    /* Check validity */
    if(!dataPtr || size<2/*Type*/) {
        goto bail;
    }

    type = (tnet_dhcp6_duid_type_t) tnet_ntohs_2(dataPtr);
    dataPtr += 2;

bail:
    return duid;
}

int tnet_dhcp6_duid_serialize(const tnet_dhcp6_duid_t* self, tsk_buffer_t *output)
{
    int ret = -1;

    if(!self || !output) {
        return ret;
    }

    switch(self->type) {
    case dhcp6_duid_linklayer_plus_time: {
        ret = tnet_dhcp6_duid_llt_serialize(TNET_DHCP6_DUID_LLT(self), output);
        break;
    }

    case dhcp6_duid_Vendor_assigned_id: {
        ret = tnet_dhcp6_duid_en_serialize(TNET_DHCP6_DUID_EN(self), output);
        break;
    }

    case dhcp6_duid_linklayer: {
        ret = tnet_dhcp6_duid_ll_serialize(TNET_DHCP6_DUID_LL(self), output);
        break;
    }

    default: {
        ret = -2;
        goto bail;
    }
    }

bail:
    return ret;
}

/*=======================================================================================
*	RFC 3315 - 9.2. DUID Based on Link-layer Address Plus Time [DUID-LLT]
*=======================================================================================*/

int tnet_dhcp6_duid_llt_serialize(const tnet_dhcp6_duid_llt_t* self, tsk_buffer_t *output)
{
    return -1;
}

//
//	[[DHCPv6 DUID-LLT]] object definition
//
static tsk_object_t* tnet_dhcp6_duid_llt_ctor(tsk_object_t * self, va_list * app)
{
    tnet_dhcp6_duid_llt_t *duid = self;
    if(duid) {
        const void* payload = va_arg(*app, const void*);
        tsk_size_t payload_size = va_arg(*app, tsk_size_t);

        /* init base */
        tnet_dhcp6_duid_init(TNET_DHCP6_DUID(duid), dhcp6_duid_linklayer_plus_time);

        if(payload && payload_size) {
            /* DESERIALIZATION */
        }
    }
    return self;
}

static tsk_object_t* tnet_dhcp6_duid_llt_dtor(tsk_object_t * self)
{
    tnet_dhcp6_duid_llt_t *duid = self;
    if(duid) {
        /* deinit base */
        tnet_dhcp6_duid_deinit(TNET_DHCP6_DUID(duid));

        TSK_OBJECT_SAFE_FREE(duid->address);
    }
    return self;
}

static const tsk_object_def_t tnet_dhcp6_duid_llt_def_s = {
    sizeof(tnet_dhcp6_duid_llt_t),
    tnet_dhcp6_duid_llt_ctor,
    tnet_dhcp6_duid_llt_dtor,
    tsk_null,
};
const tsk_object_def_t *tnet_dhcp6_duid_llt_def_t = &tnet_dhcp6_duid_llt_def_s;


/*=======================================================================================
*	RFC 3315 - 9.3. DUID Assigned by Vendor Based on Enterprise Number [DUID-EN]
*=======================================================================================*/

int tnet_dhcp6_duid_en_serialize(const tnet_dhcp6_duid_en_t* self, tsk_buffer_t *output)
{
    return -1;
}

//
//	[[DHCPv6 DUID-EN]] object definition
//
static tsk_object_t* tnet_dhcp6_duid_en_ctor(tsk_object_t * self, va_list * app)
{
    tnet_dhcp6_duid_en_t *duid = self;
    if(duid) {
        const void* payload = va_arg(*app, const void*);
        tsk_size_t payload_size = va_arg(*app, tsk_size_t);

        /* init base */
        tnet_dhcp6_duid_init(TNET_DHCP6_DUID(duid), dhcp6_duid_Vendor_assigned_id);

        if(payload && payload_size) {
            /* DESERIALIZATION */
        }
    }
    return self;
}

static tsk_object_t* tnet_dhcp6_duid_en_dtor(tsk_object_t * self)
{
    tnet_dhcp6_duid_en_t *duid = self;
    if(duid) {
        /* deinit base */
        tnet_dhcp6_duid_deinit(TNET_DHCP6_DUID(duid));

        TSK_OBJECT_SAFE_FREE(duid->indentifier);
    }
    return self;
}

static const tsk_object_def_t tnet_dhcp6_duid_en_def_s = {
    sizeof(tnet_dhcp6_duid_en_t),
    tnet_dhcp6_duid_en_ctor,
    tnet_dhcp6_duid_en_dtor,
    tsk_null,
};
const tsk_object_def_t *tnet_dhcp6_duid_en_def_t = &tnet_dhcp6_duid_en_def_s;


/*=======================================================================================
*	RFC 3315 - 9.4. DUID Based on Link-layer Address [DUID-LL]
*=======================================================================================*/

int tnet_dhcp6_duid_ll_serialize(const tnet_dhcp6_duid_ll_t* self, tsk_buffer_t *output)
{
    return -1;
}

//
//	[[DHCPv6 DUID-LL]] object definition
//
static tsk_object_t* tnet_dhcp6_duid_ll_ctor(tsk_object_t * self, va_list * app)
{
    tnet_dhcp6_duid_ll_t *duid = self;
    if(duid) {
        const void* payload = va_arg(*app, const void*);
        tsk_size_t payload_size = va_arg(*app, tsk_size_t);

        /* init base */
        tnet_dhcp6_duid_init(TNET_DHCP6_DUID(duid), dhcp6_duid_linklayer);

        if(payload && payload_size) {
            /* DESERIALIZATION */
        }
    }
    return self;
}

static tsk_object_t* tnet_dhcp6_duid_ll_dtor(tsk_object_t * self)
{
    tnet_dhcp6_duid_ll_t *duid = self;
    if(duid) {
        /* deinit base */
        tnet_dhcp6_duid_deinit(TNET_DHCP6_DUID(duid));

        TSK_OBJECT_SAFE_FREE(duid->address);
    }
    return self;
}

static const tsk_object_def_t tnet_dhcp6_duid_ll_def_s = {
    sizeof(tnet_dhcp6_duid_ll_t),
    tnet_dhcp6_duid_ll_ctor,
    tnet_dhcp6_duid_ll_dtor,
    tsk_null,
};
const tsk_object_def_t *tnet_dhcp6_duid_ll_def_t = &tnet_dhcp6_duid_ll_def_s;
