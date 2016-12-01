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
/**@file tnet_dns_rr.c
 * @brief DNS Resource Record (RFC 1034 and 1035).
 *
 */
#include "tnet_dns_rr.h"

#include "tnet_dns_a.h"
#include "tnet_dns_aaaa.h"
#include "tnet_dns_cname.h"
#include "tnet_dns_mx.h"
#include "tnet_dns_naptr.h"
#include "tnet_dns_ns.h"
#include "tnet_dns_opt.h"
#include "tnet_dns_ptr.h"
#include "tnet_dns_soa.h"
#include "tnet_dns_srv.h"
#include "tnet_dns_txt.h"

#include "../tnet_types.h"
#include "../tnet_endianness.h"

#include "tsk_memory.h"
#include "tsk_debug.h"
#include "tsk_string.h"

#include <string.h> /* strlen ... */


/** Creates a new DNS RR.
*/

tnet_dns_rr_t* tnet_dns_rr_create()
{
    return tsk_object_new(tnet_dns_rr_def_t);
}

/** Initializes any DNS RR (either NAPTR or SRV ...).
* @param rr The DNS RR to initialize.
* @param qtype The type of the RR.
* @param qclass The class of the RR.
* @retval Zero if succeed and non-zero error code otherwise.
*/
int tnet_dns_rr_init(tnet_dns_rr_t *rr, tnet_dns_qtype_t qtype, tnet_dns_qclass_t qclass)
{
    if (rr) {
        if (!rr->initialized) {
            rr->qtype = qtype;
            rr->qclass = qclass;

            rr->initialized = tsk_true;
            return 0;
        }
        return -2;
    }
    return -1;
}

/** Deinitializes any DNS RR (either NAPTR or SRV ...).
* @param rr The DNS RR to deinitialize.
* @retval Zero if succeed and non-zero error code otherwise.
*/
int tnet_dns_rr_deinit(tnet_dns_rr_t *rr)
{
    if (rr) {
        if (rr->initialized) {
            TSK_FREE(rr->name);
            TSK_FREE(rr->rpdata);

            rr->initialized = tsk_false;
            return 0;
        }
        return -2;
    }
    return -1;
}

/** Deserialize <character-string>.
*/
int tnet_dns_rr_charstring_deserialize(const void* data, char** charstring, tsk_size_t *offset)
{
    /* RFC 1035 - 3.3. Standard RRs
    	<character-string> is a single length octet followed by that number of characters.
    	<character-string> is treated as binary information, and can be up to 256 characters in
    	length (including the length octet).
    	*/
    uint8_t* dataPtr = (((uint8_t*)data) + *offset);
    uint8_t length = *dataPtr;

    *charstring = tsk_strndup((const char*)(dataPtr + 1), length);
    *offset += (1 + length);

    return 0;
}

/** Deserializes a QName.
*/
int tnet_dns_rr_qname_deserialize(const void* data, char** name, tsk_size_t *offset)
{
    /* RFC 1035 - 4.1.4. Message compression

    	The pointer takes the form of a two octet sequence:
    	+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    	| 1  1|                OFFSET                   |
    	+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    	*/
    uint8_t* dataPtr = (((uint8_t*)data) + *offset);
    unsigned usingPtr = 0; /* Do not change. */

    while (*dataPtr) {
        usingPtr = ((*dataPtr & 0xC0) == 0xC0);

        if (usingPtr) {
            tsk_size_t ptr_offset = (*dataPtr & 0x3F);
            ptr_offset = ptr_offset << 8 | *(dataPtr + 1);

            *offset += 2;
            return tnet_dns_rr_qname_deserialize(data, name, &ptr_offset);
        }
        else {
            uint8_t length;

            if (*name) {
                tsk_strcat(name, ".");
            }

            length = *dataPtr;
            *offset += 1, dataPtr++;

            tsk_strncat(name, (const char*)dataPtr, length);
            *offset += length, dataPtr += length;
        }
    }

    *offset += 1;

    return 0;
}

//int tnet_dns_rr_qname_deserialize(const void* data, tsk_size_t size, char** name, tsk_size_t *offset)
//{
//	/* RFC 1035 - 4.1.4. Message compression
//
//		The pointer takes the form of a two octet sequence:
//		+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
//		| 1  1|                OFFSET                   |
//		+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
//	*/
//	uint8_t* dataPtr = (((uint8_t*)data) + *offset);
//	uint8_t* dataEnd = (dataPtr + size);
//	unsigned usingPtr = 0; /* Do not change. */
//	unsigned islast = 0;
//
//	while(!islast)
//	{
//		usingPtr = ((*dataPtr & 0xC0) == 0xC0);
//
//		if(usingPtr)
//		{
//			uint8_t *Ptr;
//			uint16_t ptr_offset = (*dataPtr & 0x3F);
//			ptr_offset = ptr_offset << 8 | *(dataPtr+1);
//			Ptr = ((uint8_t*)data) + ptr_offset;
//
//			tnet_qname_label_parse(Ptr, (dataEnd - Ptr), name, &islast);
//			*offset += 2, dataPtr += 2;
//		}
//		else
//		{
//			tsk_size_t length = tnet_qname_label_parse(dataPtr, size, name, &islast);
//			*offset += length, dataPtr += length;
//		}
//	}
//
//	*offset += usingPtr ? 0 : 1;
//
//	return 0;
//}

/** Serializes a QName.
*/
int tnet_dns_rr_qname_serialize(const char* qname, tsk_buffer_t* output)
{
    /*
    	QNAME       a domain name represented as a sequence of labels, where
    	each label consists of a length octet followed by that
    	number of octets.  The domain name terminates with the
    	zero length octet for the null label of the root.  Note
    	that this field may be an odd number of octets; no
    	padding is used.

    	Example: "doubango.com" ==> 8doubango3comNULL
    	*/
    static uint8_t null = 0;

    if (qname) {
        char* saveptr;
        char* _qname = tsk_strdup(qname);
        char* label = tsk_strtok_r(_qname, ".", &saveptr);

        while (label) {
            uint8_t length = (uint8_t)tsk_strlen(label);
            tsk_buffer_append(output, &length, 1);
            tsk_buffer_append(output, label, tsk_strlen(label));

            label = tsk_strtok_r(tsk_null, ".", &saveptr);
        }

        TSK_FREE(_qname);
    }

    /* terminates domain name */
    tsk_buffer_append(output, &null, 1);

    return 0;
}

/** Deserializes a DNS RR.
*/
tnet_dns_rr_t* tnet_dns_rr_deserialize(const void* data, tsk_size_t size, tsk_size_t* offset)
{
    tnet_dns_rr_t *rr = tsk_null;
    uint8_t* dataStart = (uint8_t*)data;
    uint8_t* dataPtr = (dataStart + *offset);
    //uint8_t* dataEnd = (dataPtr+size);
    tnet_dns_qtype_t qtype;
    tnet_dns_qclass_t qclass;
    uint32_t ttl;
    uint16_t rdlength = 0;
    char* qname = tsk_null;

    /* Check validity */
    if (!dataPtr || !size) {
        goto bail;
    }

    /* == Parse QNAME == */
    tnet_dns_rr_qname_deserialize(dataStart, &qname, offset);
    dataPtr = (dataStart + *offset);
    /* == Parse QTYPE == */
    qtype = (tnet_dns_qtype_t)tnet_ntohs_2(dataPtr);
    dataPtr += 2, *offset += 2;
    /* == Parse QCLASS == */
    qclass = (tnet_dns_qclass_t)tnet_ntohs_2(dataPtr);
    dataPtr += 2, *offset += 2;
    /* == Parse TTL == */
    ttl = (uint32_t)tnet_htonl_2(dataPtr);
    dataPtr += 4, *offset += 4;
    /* == Parse RDLENGTH == */
    rdlength = tnet_ntohs_2(dataPtr);
    dataPtr += 2, *offset += 2;

    switch (qtype) {
    case qtype_a: {
        rr = (tnet_dns_rr_t *)tnet_dns_a_create(qname, qclass, ttl, rdlength, dataStart, *offset);
        break;
    }

    case qtype_aaaa: {
        rr = (tnet_dns_rr_t *)tnet_dns_aaaa_create(qname, qclass, ttl, rdlength, dataStart, *offset);
        break;
    }

    case qtype_cname: {
        rr = (tnet_dns_rr_t *)tnet_dns_cname_create(qname, qclass, ttl, rdlength, dataStart, *offset);
        break;
    }

    case qtype_mx: {
        rr = (tnet_dns_rr_t *)tnet_dns_mx_create(qname, qclass, ttl, rdlength, dataStart, *offset);
        break;
    }

    case qtype_naptr: {
        rr = (tnet_dns_rr_t *)tnet_dns_naptr_create(qname, qclass, ttl, rdlength, dataStart, *offset);
        break;
    }

    case qtype_ns: {
        rr = (tnet_dns_rr_t *)tnet_dns_ns_create(qname, qclass, ttl, rdlength, dataStart, *offset);
        break;
    }

    case qtype_opt: {
        unsigned payload_size = qclass;
        rr = (tnet_dns_rr_t *)tnet_dns_opt_create(payload_size);
        break;
    }

    case qtype_ptr: {
        rr = (tnet_dns_rr_t *)tnet_dns_ptr_create(qname, qclass, ttl, rdlength, dataStart, *offset);
        break;
    }

    case qtype_soa: {
        rr = (tnet_dns_rr_t *)tnet_dns_soa_create(qname, qclass, ttl, rdlength, dataStart, *offset);
        break;
    }

    case qtype_srv: {
        rr = (tnet_dns_rr_t *)tnet_dns_srv_create(qname, qclass, ttl, rdlength, dataStart, *offset);
        break;
    }

    case qtype_txt: {
        rr = (tnet_dns_rr_t *)tnet_dns_txt_create(qname, qclass, ttl, rdlength, dataStart, *offset);
        break;
    }

    default: {
        TSK_DEBUG_ERROR("NOT IMPLEMENTED");
        break;
    }
    }

bail:
    TSK_FREE(qname);

    *offset += rdlength;
    return rr;
}

/** Serializes a DNS RR.
*/
int tnet_dns_rr_serialize(const tnet_dns_rr_t* rr, tsk_buffer_t *output)
{
    if (!rr || !output) {
        return -1;
    }

    /*=== NAME ===*/
    {
        tnet_dns_rr_qname_serialize(rr->name, output);
    }

    /*=== TYPE ===*/
    {
        uint16_t qtype = tnet_htons(rr->qtype);
        tsk_buffer_append(output, &(qtype), 2);
    }

    /*=== CLASS ===*/
    {
        uint16_t qclass = tnet_htons(rr->qclass);
        tsk_buffer_append(output, &(qclass), 2);
    }

    /*=== TTL ===*/
    {
        uint32_t ttl = (uint32_t)tnet_htonl(rr->ttl);
        tsk_buffer_append(output, &(ttl), 4);
    }

    /*=== RDLENGTH ===*/
    {
        uint16_t length = tnet_htons(rr->rdlength);
        tsk_buffer_append(output, &(length), 2);
    }

    /*===  RDATA : Request never contains data
    ===*/
    if (!rr->rpdata) {
        goto done;
    }

    switch (rr->qtype) {
    case qtype_a:
    case qtype_aaaa:
    case qtype_cname:
    case qtype_mx:
    case qtype_naptr:
    case qtype_ns:
    case qtype_opt:
    case qtype_ptr:
    case qtype_soa:
    case qtype_srv:
    case qtype_txt:
    default: {
        TSK_DEBUG_WARN("DNS Request should not contains RDATA (not supported).");
        break;
    }
    }

done:
    return 0;
}


//=================================================================================================
//	[[DNS RR]] object definition
//
static tsk_object_t* tnet_dns_rr_ctor(tsk_object_t * self, va_list * app)
{
    tnet_dns_rr_t *rr = self;
    if (rr) {
        tnet_dns_rr_init(rr, qtype_any, qclass_any);
    }
    return self;
}

static tsk_object_t* tnet_dns_rr_dtor(tsk_object_t * self)
{
    tnet_dns_rr_t *rr = self;
    if (rr) {
        tnet_dns_rr_deinit(rr);
    }
    return self;
}

static const tsk_object_def_t tnet_dns_rr_def_s = {
    sizeof(tnet_dns_rr_t),
    tnet_dns_rr_ctor,
    tnet_dns_rr_dtor,
    tsk_null,
};
const tsk_object_def_t *tnet_dns_rr_def_t = &tnet_dns_rr_def_s;
