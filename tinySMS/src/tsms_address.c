/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango.org>
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

/**@file tsms_address.c
 * @brief SMS address encoder/decoder.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#include "tinysms/tsms_address.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#include <string.h>

/** swaps the address from 'abcd' to 'badc'
*/
char* tsms_address_swap(const char* in, tsk_size_t in_len)
{
    tsk_size_t i;
    char* ret = tsk_null;
    if(tsk_strnullORempty(in)) {
        goto bail;
    }
    ret = tsk_calloc(in_len + 2/*\0 and trainling F*/, sizeof(uint8_t));

    if(in_len>=2) {
        for(i=0; i<in_len; i+=2) {
            ret[i] = in[i+1];
            ret[i+1] = in[i];
        }
    }

    if(in_len & 0x01) { /* odd number? */
        ret[i-2] = 'F';
        ret[i-1] = in[i-2];
    }

bail:
    return ret;
}

tsms_address_t* tsms_address_create(const tsms_address_string_t digits, tsms_address_type_t type)
{
    return tsk_object_new(tsms_address_def_t, digits, type);
}

tsms_address_t* tsms_address_oa_create(const tsms_address_string_t digits)
{
    return tsms_address_create(digits, tsms_addr_oa);
}

tsms_address_t* tsms_address_da_create(const tsms_address_string_t digits)
{
    return tsms_address_create(digits, tsms_addr_da);
}

tsms_address_t* tsms_address_smsc_create(const tsms_address_string_t digits)
{
    return tsms_address_create(digits, tsms_addr_smsc);
}

/** Serialize the address as per 3GPP TS 23.040 v910 section 9.1.2.5. */
int tsms_address_serialize(const tsms_address_t* address, tsk_buffer_t* output)
{
    char* number = tsk_null;
    tsk_size_t i, num_len/*real len*/;
    uint8_t type_of_address;
    if(!output) {
        return -1;
    }

    /*	For more information see 3GPP TS 23.040 v910 section 9.1.2.5

    	1 - Address-Length
    	2 - Type-of-Address
    	+----+----+----+----+----+----+----+----+
    	| 1  |     TON      |		 NPI        |
    	+----+----+----+----+----+----+----+----+
    	3 - Phone number in semi octets
    */
    number = tsms_address_swap(address ? address->digits : tsk_null, address ? strlen(address->digits) : 0);

    if(number) {
        tsk_size_t len =  (address->type == tsms_addr_smsc) ?
                          ((strlen(number)/2) + 1) /* Number of octets plus 1. */
                          : strlen(address->digits); /* Number of BCD digits */
        /* 1 - Address-Length */
        tsk_buffer_append(output, &len, 1);
    }
    else {
        /* 1 - Address-Length */
        static uint8_t _1bytes = 0x00;
        tsk_buffer_append(output, &_1bytes, 1);
        goto bail; /* neither 2 nor 3 will be executed */
    }

    /* 2 - Type-of-Address */
    type_of_address = (address->npi | (((address->ton << 4) | 0x80)));
    tsk_buffer_append(output, &type_of_address, 1);
    /* 3 - Phone number in semi octets (BCD digits) */
    if(((num_len = strlen(number)))) {
        unsigned _1bytes; /* do not use neither int8_t nor uint8_t */
        /* as number comes from swap ==> num_len not odd */
        for(i=0; i<num_len; i+=2) {
            if(sscanf(&number[i], "%2x", &_1bytes)) { /*do not use tsk_atox(str), because str should end with '\0'.*/
                tsk_buffer_append(output, &_1bytes, 1);
            }
        }
    }

bail:
    TSK_FREE(number);
    return 0;
}


tsms_address_t* tsms_address_deserialize(const void* data, tsk_size_t size, tsms_address_type_t xtype, tsk_size_t *length)
{
    tsms_address_t* address = tsk_null;
    uint8_t addr_len, _1byte, i;
    const uint8_t* pdata = data;
    *length = 0;

    if(!pdata || size<=2) {
        TSK_DEBUG_ERROR("Invalid Parameter.");
        goto bail;
    }

    /* 1 - Address-Length */
    addr_len = *pdata;
    pdata++;

    /*== len=0 ==*/
    if(!addr_len) {
        address = tsms_address_create(tsk_null, xtype);
        *length = 1;
        goto bail;
    }
    /*== check validity for non-zero-length address ==*/
    addr_len = (xtype == tsms_addr_smsc) ?
               (addr_len - 1) /* Number of octets plus 1. */
               : ((addr_len/2) + (addr_len%2)); /* Number of BCD digits */
    if((tsk_size_t)(1 /*Address-Length*/ + 1 /*Type-of-Address*/ + addr_len /* digits */) >= size) {
        TSK_DEBUG_ERROR("Too short to contain an address.");
        goto bail;
    }
    else {
        address = tsms_address_create(tsk_null, xtype);
        *length = 1 /*Address-Length*/ + 1 /*Type-of-Address*/ + addr_len /* digits */;
    }

    /*	2 - Type-of-Address
    	+----+----+----+----+----+----+----+----+
    	| 1  |     TON      |		 NPI        |
    	+----+----+----+----+----+----+----+----+
    */
    address->ton = ((*pdata &0x70) >> 4);
    address->npi = (*pdata &0x0F);
    pdata++;

    /* 3 - Phone number in semi octets (BCD digits) */
    //address->digits = tsk_calloc((addr_len/2) + 1, sizeof(uint8_t));
    for(i=0; i<addr_len; i++, pdata++) {
        _1byte = ((*pdata << 4) | (*pdata >> 4));
        if((_1byte & 0x0F) == 0x0F) { /* ends with 'F'? */
            _1byte = ((_1byte & 0xF0) >> 4);
            tsk_strcat_2(&address->digits, "%.1x", _1byte);
        }
        else {
            tsk_strcat_2(&address->digits, "%.2x", _1byte);
        }
    }

bail:
    return address;
}

//=================================================================================================
//	SMS Address object definition
//
static tsk_object_t* tsms_address_ctor(tsk_object_t * self, va_list * app)
{
    tsms_address_t *address = self;
    if(address) {
        const char* digits = va_arg(*app, const char*);
        address->type = va_arg(*app, tsms_address_type_t);

        if(!tsk_strnullORempty(digits)) {
            if(*digits == '+') {
                address->npi = tsms_addr_npi_isdn;
                address->ton = tsms_addr_ton_international;
                address->digits = tsk_strdup((digits+1));
            }
            else {
                address->npi = tsms_addr_npi_national;
                address->ton = tsms_addr_ton_national;
                address->digits = tsk_strdup((digits));
            }
        }
        else {
            address->npi = tsms_addr_npi_unknown;
            address->ton = tsms_addr_ton_unknown;
        }

    }
    return self;
}

static tsk_object_t* tsms_address_dtor(tsk_object_t * self)
{
    tsms_address_t *address = self;
    if(address) {
        TSK_FREE(address->digits);
    }

    return self;
}

static const tsk_object_def_t tsms_address_def_s = {
    sizeof(tsms_address_t),
    tsms_address_ctor,
    tsms_address_dtor,
    tsk_null,
};
const tsk_object_def_t *tsms_address_def_t = &tsms_address_def_s;
