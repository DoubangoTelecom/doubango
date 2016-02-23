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
/**@file tnet_dhcp_option.c
 * @brief DHCP Options and BOOTP Vendor Extensions as per RFC 2132.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tnet_dhcp_option.h"

#include "tnet_dhcp_option_sip.h"

#include "../tnet_types.h"
#include "../tnet_endianness.h"

#include "tsk_memory.h"
#include "tsk_debug.h"

tnet_dhcp_option_t* tnet_dhcp_option_create(tnet_dhcp_option_code_t code)
{
    return tsk_object_new(tnet_dhcp_option_def_t, code);
}

tnet_dhcp_option_paramslist_t* tnet_dhcp_option_paramslist_create()
{
    return tsk_object_new(tnet_dhcp_option_paramslist_def_t);
}

tnet_dhcp_option_dns_t* tnet_dhcp_option_dns_create(const void* payload, tsk_size_t payload_size)
{
    return tsk_object_new(tnet_dhcp_option_dns_def_t, payload, payload_size);
}

/** Initializes DHCPv4 option.
 *
 * @param [in,out]	self	The option to initialize.
 * @param	code			The code of the option to initialize.
 *
 * @return	Zero if succeed and non-zero error code otherwise.
**/
int tnet_dhcp_option_init(tnet_dhcp_option_t *self, tnet_dhcp_option_code_t code)
{
    if(self) {
        if(!self->initialized) {
            self->code = code;
            //option->value = tsk_buffer_create_null();

            self->initialized = tsk_true;
            return 0;
        }
        return -2;
    }
    return -1;
}

int tnet_dhcp_option_deinit(tnet_dhcp_option_t *self)
{
    if(self) {
        if(self->initialized) {
            TSK_OBJECT_SAFE_FREE(self->value);

            self->initialized = tsk_false;
            return 0;
        }
        return -2;
    }
    return -1;
}

tnet_dhcp_option_t* tnet_dhcp_option_deserialize(const void* data, tsk_size_t size)
{
    tnet_dhcp_option_t *option = 0;
    uint8_t* dataPtr = ((uint8_t*)data);
    //uint8_t* dataEnd = (dataPtr+size);

    tnet_dhcp_option_code_t code;
    uint8_t len;

    /* Check validity */
    if(!dataPtr || size<2/*Code Len*/) {
        goto bail;
    }

    code = (tnet_dhcp_option_code_t)*dataPtr++;
    len = *dataPtr++;

    switch(code) {
    case dhcp_code_SIP_Servers_DHCP_Option: {
        option = (tnet_dhcp_option_t *)tnet_dhcp_option_sip_create(dataPtr, len);
        break;
    }

    case dhcp_code_Domain_Server: {
        option = (tnet_dhcp_option_t *)tnet_dhcp_option_dns_create(dataPtr, len);
        break;
    }

    default: {
        option = tnet_dhcp_option_create(code);
    }
    }

    /* In all case */
    if(option && !option->value && len) {
        option->value = tsk_buffer_create((((uint8_t*)data) + 2/*Code Len*/), len);
    }

bail:
    return option;
}

int tnet_dhcp_option_serialize(const tnet_dhcp_option_t* self, tsk_buffer_t *output)
{
    if(!self || !output) {
        return -1;
    }

    /* Code */
    tsk_buffer_append(output, &(self->code), 1);

    if(self->value) {
        /* Length */
        tsk_buffer_append(output, &(self->value->size), 1);

        /* Value */
        tsk_buffer_append(output, self->value->data, self->value->size);
    }
    else {
        /* Length */
        static uint8_t zero = 0x00;
        tsk_buffer_append(output, &zero, 1);
    }

    return 0;
}

int tnet_dhcp_option_serializeex(tnet_dhcp_option_code_t code, uint8_t length, const void* value, tsk_buffer_t *output)
{
    if(value && length && output) {
        tsk_buffer_append(output, &(code), 1);
        tsk_buffer_append(output, &(length), 1);
        tsk_buffer_append(output, value, length);

        return 0;
    }
    return -1;
}

//
//	[[DHCP OPTION]] object definition
//
static tsk_object_t* tnet_dhcp_option_ctor(tsk_object_t * self, va_list * app)
{
    tnet_dhcp_option_t *option = self;
    if(option) {
        tnet_dhcp_option_init(option, va_arg(*app, tnet_dhcp_option_code_t));
    }
    return self;
}

static tsk_object_t* tnet_dhcp_option_dtor(tsk_object_t * self)
{
    tnet_dhcp_option_t *option = self;
    if(option) {
        tnet_dhcp_option_deinit(option);
    }
    return self;
}

static const tsk_object_def_t tnet_dhcp_option_def_s = {
    sizeof(tnet_dhcp_option_t),
    tnet_dhcp_option_ctor,
    tnet_dhcp_option_dtor,
    tsk_null,
};
const tsk_object_def_t *tnet_dhcp_option_def_t = &tnet_dhcp_option_def_s;



/*=======================================================================================
*	RFC 2132 - 9.8. Parameter Request List
*=======================================================================================*/
int tnet_dhcp_option_paramslist_add_code(tnet_dhcp_option_paramslist_t* self, tnet_dhcp_option_code_t code)
{
    if(self) {
        if(!TNET_DHCP_OPTION(self)->value) {
            TNET_DHCP_OPTION(self)->value = tsk_buffer_create_null();
        }
        return tsk_buffer_append(TNET_DHCP_OPTION(self)->value, &code, 1);
    }
    return -1;
}

//
//	[[DHCP OPTION - RFC 2132 9.8. Parameter Request List]] object definition
//
static tsk_object_t* tnet_dhcp_option_paramslist_ctor(tsk_object_t * self, va_list * app)
{
    tnet_dhcp_option_paramslist_t *option = self;
    if(option) {
        /* init base */
        tnet_dhcp_option_init(TNET_DHCP_OPTION(option), dhcp_code_Parameter_List);
    }
    return self;
}

static tsk_object_t* tnet_dhcp_option_paramslist_dtor(tsk_object_t * self)
{
    tnet_dhcp_option_paramslist_t *option = self;
    if(option) {
        /* deinit base */
        tnet_dhcp_option_deinit(TNET_DHCP_OPTION(option));
    }
    return self;
}

static const tsk_object_def_t tnet_dhcp_option_paramslist_def_s = {
    sizeof(tnet_dhcp_option_paramslist_t),
    tnet_dhcp_option_paramslist_ctor,
    tnet_dhcp_option_paramslist_dtor,
    tsk_null,
};
const tsk_object_def_t *tnet_dhcp_option_paramslist_def_t = &tnet_dhcp_option_paramslist_def_s;


/*=======================================================================================
*	RFC 2132 - 3.8. Domain Name Server Option
*=======================================================================================*/
//
//	[[DHCP OPTION - RFC 2132 3.8. Domain Name Server Option]] object definition
//
static tsk_object_t* tnet_dhcp_option_dns_ctor(tsk_object_t * self, va_list * app)
{
    tnet_dhcp_option_dns_t *option = self;
    if(option) {
        const void* payload = va_arg(*app, const void*);
        tsk_size_t payload_size = va_arg(*app, tsk_size_t);

        const uint8_t* payloadPtr = (const uint8_t*)payload;
        const uint8_t* payloadEnd = (payloadPtr + payload_size);

        /* init base */
        tnet_dhcp_option_init(TNET_DHCP_OPTION(option), dhcp_code_Domain_Server);

        option->servers = tsk_list_create();

        if(payload_size<4 || payload_size%4) {
            TSK_DEBUG_ERROR("DHCP - The minimum length for this option is 4 octets, and the length MUST always be a multiple of 4.");
        }
        else {
            tsk_size_t i;
            char* ip4 = 0;
            uint32_t address;
            tsk_string_t* addrstring;

            for(i=0; i<payload_size && (payloadPtr< payloadEnd); i+=4) {
                /*
                Code   Len         Address 1               Address 2
                +-----+-----+-----+-----+-----+-----+-----+-----+--
                |  6  |  n  |  a1 |  a2 |  a3 |  a4 |  a1 |  a2 |  ...
                +-----+-----+-----+-----+-----+-----+-----+-----+--
                */
                address = (uint32_t)tnet_htonl_2(payloadPtr);
                tsk_sprintf(&ip4, "%u.%u.%u.%u", (address>>24)&0xFF, (address>>16)&0xFF, (address>>8)&0xFF, (address>>0)&0xFF);

                addrstring = tsk_string_create(ip4);
                tsk_list_push_back_data(option->servers, (void*)&addrstring);

                TSK_FREE(ip4);
                payloadPtr+= 4;
            }
        }
    }
    return self;
}

static tsk_object_t* tnet_dhcp_option_dns_dtor(tsk_object_t * self)
{
    tnet_dhcp_option_dns_t *option = self;
    if(option) {
        /* deinit base */
        tnet_dhcp_option_deinit(TNET_DHCP_OPTION(option));

        TSK_OBJECT_SAFE_FREE(option->servers);
    }
    return self;
}

static const tsk_object_def_t tnet_dhcp_option_dns_def_s = {
    sizeof(tnet_dhcp_option_dns_t),
    tnet_dhcp_option_dns_ctor,
    tnet_dhcp_option_dns_dtor,
    tsk_null,
};
const tsk_object_def_t *tnet_dhcp_option_dns_def_t = &tnet_dhcp_option_dns_def_s;
