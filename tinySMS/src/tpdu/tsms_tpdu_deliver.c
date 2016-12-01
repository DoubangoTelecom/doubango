/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou [at) doubango (DOT) org>
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

/**@file tsms_tpdu_deliver.c
 * @brief SMS TPDU SMS-DELIVER message as per 3GPP TS 23.040 section 9.2.2.1.
 *
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 *

 */
#include "tinysms/tpdu/tsms_tpdu_deliver.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#include <string.h> /* memcpy */

#define TSMS_ERROR_TOO_SHORT()\
	TSK_DEBUG_ERROR("SMS-DELIVER == Data too short.");\
	failed = tsk_true;\
	goto bail;

/** internal function used to deserialize a buffer containing a SMS-DELIVER message. */
tsms_tpdu_message_t* _tsms_tpdu_deliver_deserialize(const void* data, tsk_size_t size)
{
    /* You don't need to test data and test, this is an internal function called by tsms_tpdu_message_deserialize() */
    tsms_tpdu_deliver_t* self = tsms_tpdu_deliver_create(tsk_null, tsk_null);
    tsk_bool_t failed = tsk_false;
    tsk_size_t any_len;
    const uint8_t* pdata = data;
    const uint8_t* pend = pdata + size;

    /* SMSC address */
#if TSMS_TPDU_APPEND_SMSC
    if(!(self->smsc = tsms_address_deserialize(data, size, tsms_addr_smsc, &any_len)) || !any_len) {
        TSK_DEBUG_ERROR("SMS-DELIVER == Failed to parse SMSC address");
        failed = tsk_true;
        goto bail;
    }
    else {
        if((pdata += any_len) >= pend) {
            TSMS_ERROR_TOO_SHORT();
        }
    }
#endif

    /* SMS-DELIVER first Octect:
    	- TP-Message-Type-Indicator(2b)
    	- TP-More-Messages-to-Send(1b)
    	- TP-Loop-Prevention(1b)
    	- TP-Reply-Path(1b)
    	- TP-User-Data-Header-Indicator(1b)
    	- TP-Status-Report-Indication(1b)
    	+----+----+----+----+----+----+----+----+
    	|  RP|UDHI|SRI |	|LP	 |MMS | MTI	    |
    	+----+----+----+----+----+----+----+----+
    */
    TSMS_TPDU_MESSAGE(self)->mti = (*pdata & 0x03);
    self->mms = (*pdata & 0x04)>>2,
          self->lp = (*pdata & 0x08)>>3,
                self->sri = (*pdata & 0x20)>>5,
                      self->udhi = (*pdata & 0x40)>>6,
                            self->rp = (*pdata & 0x80)>>7;
    if((++pdata) >= pend) {
        TSMS_ERROR_TOO_SHORT();
    }

    /* 3GPP TS 23.040 ==> TP-Originating-Address (TP-OA)*/
    if(!(self->oa = tsms_address_deserialize(pdata, (pend-pdata), tsms_addr_oa, &any_len)) || !any_len) {
        TSK_DEBUG_ERROR("SMS-DELIVER == Failed to parse OA address");
        failed = tsk_true;
        goto bail;
    }
    else {
        if((pdata += any_len) >= pend) {
            TSMS_ERROR_TOO_SHORT();
        }
    }

    /* 3GPP TS 23.040 ==> 9.2.3.9 TP-Protocol-Identifier (TP-PID)
    * 1o */
    TSMS_TPDU_MESSAGE(self)->pid = *pdata;
    if((++pdata) >= pend) {
        TSMS_ERROR_TOO_SHORT();
    }

    /* 3GPP TS 23.040 ==> 9.2.3.10 TP-Data-Coding-Scheme (TP-DCS)
    * 1o */
    TSMS_TPDU_MESSAGE(self)->dcs = *pdata;
    if((++pdata) >= pend) {
        TSMS_ERROR_TOO_SHORT();
    }

    /* 3GPP TS 23.040 ==> TP-Service-Centre-Time-Stamp  (TP-SCTS)
    * 7o */
    if((pend - pdata)<=7) {
        TSMS_ERROR_TOO_SHORT();
    }
    memcpy(self->scts, pdata, 7);
    pdata += 7;


    /* 3GPP TS 23.040 ==> 9.2.3.16 TP-User-Data-Length (TP-UDL)
    * 1o */
    TSMS_TPDU_MESSAGE(self)->udl = *pdata;
    pdata++;

    /* 3GPP TS 23.040 ==> 9.2.3.24 TP-User Data (TP-UD) */
    if((pend-pdata) > 0) {
        TSMS_TPDU_MESSAGE(self)->ud = tsk_buffer_create(pdata, (pend-pdata));
    }

bail:
    if(failed) {
        TSK_OBJECT_SAFE_FREE(self);
    }
    return TSMS_TPDU_MESSAGE(self);
}

/** internal function used to serialize a SMS-DELIVER message. */
int _tsms_tpdu_deliver_serialize(const tsms_tpdu_deliver_t* self, tsk_buffer_t* output)
{
    uint8_t _1byte;

    if(!self) {
        return -1;
    }

    /* SMSC address */
#if TSMS_TPDU_APPEND_SMSC
    tsms_address_serialize(self->smsc, output);
#endif

    /* SMS-DELIVER first Octect:
    	- TP-Message-Type-Indicator(2b)
    	- TP-More-Messages-to-Send(1b)
    	- TP-Loop-Prevention(1b)
    	- TP-Reply-Path(1b)
    	- TP-User-Data-Header-Indicator(1b)
    	- TP-Status-Report-Indication(1b)
    	+----+----+----+----+----+----+----+----+
    	|  RP|UDHI|SRI |	|LP	 |MMS | MTI	    |
    	+----+----+----+----+----+----+----+----+
    */
    _1byte = (TSMS_TPDU_MESSAGE(self)->mti & 0xF3); /*2b*/
    _1byte |= ((uint8_t)self->mms) << 2 /*1b*/
              | ((uint8_t)self->lp) << 3 /*1b*/
              /*1b (unused)*/
              | ((uint8_t)self->sri) << 5 /*1b*/
              | ((uint8_t)self->udhi) << 6 /*1b*/
              | ((uint8_t)self->rp) << 7; /*1b*/
    tsk_buffer_append(output, &_1byte, 1);

    /* 3GPP TS 23.040 ==> TP-Originating-Address (TP-OA)*/
    tsms_address_serialize(self->oa, output);

    /* 3GPP TS 23.040 ==> 9.2.3.9 TP-Protocol-Identifier (TP-PID)*/
    tsk_buffer_append(output, &TSMS_TPDU_MESSAGE(self)->pid, 1); /*1o*/

    /* 3GPP TS 23.040 ==> 9.2.3.10 TP-Data-Coding-Scheme (TP-DCS)*/
    tsk_buffer_append(output, &TSMS_TPDU_MESSAGE(self)->dcs, 1); /*1o*/

    /* 3GPP TS 23.040 ==> TP-Service-Centre-Time-Stamp  (TP-SCTS)*/
    tsk_buffer_append(output, self->scts, sizeof(self->scts)); /*7o*/

    /* 3GPP TS 23.040 ==> 9.2.3.16 TP-User-Data-Length (TP-UDL) */
    tsk_buffer_append(output, &TSMS_TPDU_MESSAGE(self)->udl, 1); /*1o*/

    /* 3GPP TS 23.040 ==> 9.2.3.24 TP-User Data (TP-UD) */
    tsk_buffer_append(output, TSK_BUFFER_DATA(TSMS_TPDU_MESSAGE(self)->ud), TSK_BUFFER_SIZE(TSMS_TPDU_MESSAGE(self)->ud));

    return 0;
}

/**@ingroup tsms_tpdu_group
* Creates new @a SMS-DELIVER message.
* @a SMS-DELIVER messages are used to convey short messages from the SC (Service Center) to the MS (Mobile Station).<br>
* For more information, please refer to 3GPP TS 23.040 section 9.2.2.1.
* @param smsc SMSC address. e.g. "+331253688".
* @param orig The Originator address. e.g. "+331253688".
* @retval SMS-DELIVER message.
*
* See For more information, see @ref tsms_tpdu_group_DELIVER "SMS-DELIVER".
*/
tsms_tpdu_deliver_t* tsms_tpdu_deliver_create(const tsms_address_string_t smsc, const tsms_address_string_t orig)
{
    tsms_tpdu_deliver_t* ret = tsk_null;

    if(!(ret = tsk_object_new(tsms_tpdu_deliver_def_t, smsc, orig))) {
        goto bail;
    }

bail:
    return ret;
}

//=================================================================================================
//	SMS TPDU SMS-DELIVER object definition
//
static tsk_object_t* tsms_tpdu_deliver_ctor(tsk_object_t * self, va_list * app)
{
    tsms_tpdu_deliver_t *deliver = self;
    if(deliver) {
        const char* smsc, *orig;

        smsc = va_arg(*app, const char*);
        orig = va_arg(*app, const char*);

        /* init base*/
        tsms_tpdu_message_init(TSMS_TPDU_MESSAGE(deliver), tsms_tpdu_mti_deliver_mt);
        /* init self */
        deliver->mms = TSMS_TPDU_DEFAULT_MMS;
        if(smsc) {
            deliver->smsc = tsms_address_smsc_create((const uint8_t*)smsc);
        }
        if(orig) {
            deliver->oa = tsms_address_oa_create((const uint8_t*)orig);
        }
    }
    return self;
}

static tsk_object_t* tsms_tpdu_deliver_dtor(tsk_object_t * self)
{
    tsms_tpdu_deliver_t *deliver = self;
    if(deliver) {
        /*deinit base*/
        tsms_tpdu_message_deinit(TSMS_TPDU_MESSAGE(deliver));
        /*deinit self*/
        TSK_OBJECT_SAFE_FREE(deliver->oa);
        TSK_OBJECT_SAFE_FREE(deliver->smsc);
    }
    return self;
}

static const tsk_object_def_t tsms_tpdu_deliver_def_s = {
    sizeof(tsms_tpdu_deliver_t),
    tsms_tpdu_deliver_ctor,
    tsms_tpdu_deliver_dtor,
    tsk_null,
};
const tsk_object_def_t *tsms_tpdu_deliver_def_t = &tsms_tpdu_deliver_def_s;
