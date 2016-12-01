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

/**@file tsms_tpdu_submit.c
 * @brief SMS TPDU SMS-SUBMIT (MO) message as per 3GPP TS 23.040 section 9.2.2.2.
 *
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 *

 */
#include "tinysms/tpdu/tsms_tpdu_submit.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#include <string.h> /* memcpy */

#define TSMS_ERROR_TOO_SHORT()\
	TSK_DEBUG_ERROR("SMS-SUBMIT == Data too short.");\
	failed = tsk_true;\
	goto bail;

/** internal function used to deserialize SMS-SUBMIT function.
*/
tsms_tpdu_message_t* _tsms_tpdu_submit_deserialize(const void* data, tsk_size_t size)
{
    /* You don't need to test data and test, this is an internal function called by tsms_tpdu_message_deserialize() */
    tsms_tpdu_submit_t* self = tsms_tpdu_submit_create(0, tsk_null, tsk_null);
    tsk_bool_t failed = tsk_false;
    tsk_size_t any_len;
    const uint8_t* pdata = data;
    const uint8_t* pend = pdata + size;

    /* SMSC address */
#if TSMS_TPDU_APPEND_SMSC
    if(!(self->smsc = tsms_address_deserialize(data, size, tsms_addr_smsc, &any_len)) || !any_len) {
        TSK_DEBUG_ERROR("SMS-SUBMIT == Failed to parse SMSC address");
        failed = tsk_true;
        goto bail;
    }
    else {
        if((pdata += any_len) >= pend) {
            TSMS_ERROR_TOO_SHORT();
        }
    }
#endif

    /* SMS-SUBMIT first Octect:
    	- TP-Message-Type-Indicator(2b)
    	- TP-Reject-Duplicates(1b)
    	- TP-Validity-Period-Format(1b)
    	- TP-Reply-Path(1b)
    	- TP-User-Data-Header-Indicator(1b)
    	- TP-Status-Report-Request(1b)

    	+----+----+----+----+----+----+----+----+
    	|RP  |UDHI|SRR |VPF		 | RD | MTI	    |
    	+----+----+----+----+----+----+----+----+
    */
    TSMS_TPDU_MESSAGE(self)->mti = *pdata & 0x03;
    self->rd = (*pdata & 0x04)>>2,
          self->vpf = (*pdata & 0x18)>>3,
                self->srr = (*pdata & 0x20)>>5,
                      self->udhi = (*pdata & 0x40)>>6,
                            self->rp = (*pdata & 0x80)>>7;
    if((++pdata) >= pend) {
        TSMS_ERROR_TOO_SHORT();
    }

    /* 3GPP TS 23.040 ==> 9.2.3.6 TP-Message-Reference (TP-MR)
    * 1o */
    self->mr = *pdata;
    if((++pdata) >= pend) {
        TSMS_ERROR_TOO_SHORT();
    }

    /* 3GPP TS 23.040 ==> 9.2.3.8 TP-Destination-Address (TP-DA) */
    if(!(self->da = tsms_address_deserialize(pdata, (pend-pdata), tsms_addr_da, &any_len)) || !any_len) {
        TSK_DEBUG_ERROR("SMS-SUBMIT == Failed to parse DA address");
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

    /* 3GPP TS 23.040 ==> 9.2.3.12 TP-Validity-Period
    * 1o for Relative format (9.2.3.12.1)
    * 7o for Absolute format (9.2.3.12.2)
    * 7o for Enhanced format (9.2.3.12.3)*/
    switch(self->vpf) {
    case tsms_tpdu_vpf_relative:
        any_len = 1;
        break;
    case tsms_tpdu_vpf_enhanced:
    case tsms_tpdu_vpf_absolute:
        any_len = 7;
        break;
    default:
    case tsms_tpdu_vpf_not_present:
        any_len = 0;
        break;
    }
    if((tsk_size_t)(pend-pdata) <= any_len) {
        TSMS_ERROR_TOO_SHORT();
    }
    memcpy(self->vp, pdata, any_len);
    pdata += any_len;

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


/** internal function used to serialize a SMS-SUBMIT message
*/
int _tsms_tpdu_submit_serialize(const tsms_tpdu_submit_t* self, tsk_buffer_t* output)
{
    uint8_t _1byte;

    if(!self) {
        return -1;
    }

    /* SMSC address */
#if TSMS_TPDU_APPEND_SMSC
    tsms_address_serialize(self->smsc, output);
#endif

    /* SMS-SUBMIT first Octect:
    	- TP-Message-Type-Indicator(2b)
    	- TP-Reject-Duplicates(1b)
    	- TP-Validity-Period-Format(1b)
    	- TP-Reply-Path(1b)
    	- TP-User-Data-Header-Indicator(1b)
    	- TP-Status-Report-Request(1b)

    	+----+----+----+----+----+----+----+----+
    	|RP  |UDHI|SRR |VPF		 | RD | MTI	    |
    	+----+----+----+----+----+----+----+----+
    */
    _1byte = (TSMS_TPDU_MESSAGE(self)->mti & 0xF3); /*2b*/
    _1byte |= ((uint8_t)self->rd) << 2 /*1b*/
              | ((uint8_t)self->vpf) << 3 /*2b*/
              | ((uint8_t)self->srr) << 5 /*1b*/
              | ((uint8_t)self->udhi) << 6; /*1b*/
    tsk_buffer_append(output, &_1byte, 1);

    /* 3GPP TS 23.040 ==> 9.2.3.6 TP-Message-Reference (TP-MR) */
    tsk_buffer_append(output, &self->mr, 1); /*0-255 ==> 1o*/

    /* 3GPP TS 23.040 ==> 9.2.3.8 TP-Destination-Address (TP-DA) */
    tsms_address_serialize(self->da, output);

    /* 3GPP TS 23.040 ==> 9.2.3.9 TP-Protocol-Identifier (TP-PID) */
    tsk_buffer_append(output, &TSMS_TPDU_MESSAGE(self)->pid, 1); /*1o*/

    /* 3GPP TS 23.040 ==> 9.2.3.10 TP-Data-Coding-Scheme (TP-DCS) */
    tsk_buffer_append(output, &TSMS_TPDU_MESSAGE(self)->dcs, 1); /*1o*/

    /* 3GPP TS 23.040 ==> 9.2.3.12 TP-Validity-Period
    * 1o for Relative format (9.2.3.12.1)
    * 7o for Absolute format (9.2.3.12.2)
    * 7o for Enhanced format (9.2.3.12.3)*/
    switch(self->vpf) {
    case tsms_tpdu_vpf_relative:
        tsk_buffer_append(output, &self->vp, 1);
        break;
    case tsms_tpdu_vpf_enhanced:
    case tsms_tpdu_vpf_absolute:
        tsk_buffer_append(output, &self->vp, 7);
        break;
    default:
    case tsms_tpdu_vpf_not_present:
        break;
    }

    /* 3GPP TS 23.040 ==> 9.2.3.16 TP-User-Data-Length (TP-UDL) */
    tsk_buffer_append(output, &TSMS_TPDU_MESSAGE(self)->udl, 1); /*1o*/

    /* 3GPP TS 23.040 ==> 9.2.3.24 TP-User Data (TP-UD) */
    tsk_buffer_append(output, TSK_BUFFER_DATA(TSMS_TPDU_MESSAGE(self)->ud), TSK_BUFFER_SIZE(TSMS_TPDU_MESSAGE(self)->ud));

    return 0;
}

/**@ingroup tsms_tpdu_group
* Creates new @a SMS-SUBMIT message.
* @a SMS-SUBMIT messages are used to convey short messages from the MS (Mobile Station) to the SC (Service Center).<br>
* For more information, please refer to 3GPP TS 23.040 section 9.2.2.2.
* @param mr TP-Message-Reference (TP-MR) as per 3GPP TS 23.040 section 9.2.3.6.
* @param smsc The address of the SMS. e.g. "+331253688".
* @param dest The destination address. e.g. "+331253688".
* @retval SMS-SUBMIT message.
*
* See For more information, see @ref tsms_tpdu_group_SUBMIT "SMS-SUBMIT".
*/
tsms_tpdu_submit_t* tsms_tpdu_submit_create(uint8_t mr, const tsms_address_string_t smsc, const tsms_address_string_t dest)
{
    tsms_tpdu_submit_t* ret = tsk_null;

    if(!(ret = tsk_object_new(tsms_tpdu_submit_def_t, mr, smsc, dest))) {
        goto bail;
    }

bail:
    return ret;
}

//=================================================================================================
//	SMS TPDU SMS-SUMBIT object definition
//
static tsk_object_t* tsms_tpdu_submit_ctor(tsk_object_t * self, va_list * app)
{
    tsms_tpdu_submit_t *submit = self;
    if(submit) {
        const char* smsc, *dest;
        uint8_t mr;

#if defined(__GNUC__)
        mr = (uint8_t)va_arg(*app, unsigned);
#else
        mr = va_arg(*app, uint8_t);
#endif
        smsc = va_arg(*app, const char*);
        dest = va_arg(*app, const char*);

        /* init base*/
        tsms_tpdu_message_init(TSMS_TPDU_MESSAGE(submit), tsms_tpdu_mti_submit_mo);
        /* init self */
        submit->mr = mr;
        if(smsc) {
            submit->smsc = tsms_address_smsc_create((const uint8_t*)smsc);
        }
        if(dest) {
            submit->da = tsms_address_da_create((const uint8_t*)dest);
        }

        submit->vp[0] = TSMS_TPDU_DEFAULT_VP;
        submit->vpf = TSMS_TPDU_DEFAULT_VPF;
    }
    else {
        TSK_DEBUG_ERROR("Null");
    }
    return self;
}

static tsk_object_t* tsms_tpdu_submit_dtor(tsk_object_t * self)
{
    tsms_tpdu_submit_t *submit = self;
    if(submit) {
        /*deinit base*/
        tsms_tpdu_message_deinit(TSMS_TPDU_MESSAGE(submit));
        /*deinit self*/
        TSK_OBJECT_SAFE_FREE(submit->smsc);
        TSK_OBJECT_SAFE_FREE(submit->da);
    }
    else {
        TSK_DEBUG_ERROR("Null");
    }
    return self;
}

static const tsk_object_def_t tsms_tpdu_submit_def_s = {
    sizeof(tsms_tpdu_submit_t),
    tsms_tpdu_submit_ctor,
    tsms_tpdu_submit_dtor,
    tsk_null,
};
const tsk_object_def_t *tsms_tpdu_submit_def_t = &tsms_tpdu_submit_def_s;
