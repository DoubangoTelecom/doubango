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

/**@file tsms_tpdu_status_report.c
 * @brief SMS TPDU SMS-STATUS-REPORT message as per 3GPP TS 23.040 section 9.2.2.3.
 *
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 *

 */
#include "tinysms/tpdu/tsms_tpdu_status_report.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#include <string.h> /* memcpy */

#define TSMS_ERROR_TOO_SHORT()\
	TSK_DEBUG_ERROR("SMS-STATUS-REPORT == Data too short.");\
	failed = tsk_true;\
	goto bail;

/** internal function used to deserialize a SMS-STATUS-REPORT message from a binary buffer. */
tsms_tpdu_message_t* _tsms_tpdu_status_report_deserialize(const void* data, tsk_size_t size)
{
    /* You don't need to test data and test, this is an internal function called by tsms_tpdu_message_deserialize() */
    tsms_tpdu_status_report_t* self = tsms_tpdu_status_report_create(0, tsk_null, tsk_null, tsms_tpdu_status_received, tsk_false);
    tsk_bool_t failed = tsk_false;
    tsk_size_t any_len;
    const uint8_t* pdata = data;
    const uint8_t* pend = pdata + size;

    /* SMSC address */
#if TSMS_TPDU_APPEND_SMSC
    if(!(self->smsc = tsms_address_deserialize(data, size, tsms_addr_smsc, &any_len)) || !any_len) {
        TSK_DEBUG_ERROR("SMS-STATUS-REPORT == Failed to parse SMSC address");
        failed = tsk_true;
        goto bail;
    }
    else {
        if((pdata += any_len) >= pend) {
            TSMS_ERROR_TOO_SHORT();
        }
    }
#endif

    /* SMS-STATUS-REPORT first Octect:
    	- TP-Message-Type-Indicator(2b)
    	- TP-More-Messages-to-Send(1b)
    	- TP-Loop-Prevention(1b)
    	- TP-User-Data-Header-Indicator(1b)
    	- TP-Status-Report-Qualifier(1b)
    	+----+----+----+----+----+----+----+----+
    	|    |UDHI| SRQ|	|LP	 |MMS | MTI	    |
    	+----+----+----+----+----+----+----+----+
    */
    TSMS_TPDU_MESSAGE(self)->mti = *pdata & 0x03;
    self->mms = (*pdata & 0x04)>>2,
          self->lp = (*pdata & 0x08)>>3,
                self->srq = (*pdata & 0x20)>>5,
                      self->udhi = (*pdata & 0x40)>>6;
    if((++pdata) >= pend) {
        TSMS_ERROR_TOO_SHORT();
    }

    /* 3GPP TS 23.040 ==> 9.2.3.6 TP-Message-Reference (TP-MR)
    * 1o */
    self->mr = *pdata;
    if((++pdata) >= pend) {
        TSMS_ERROR_TOO_SHORT();
    }

    /* 3GPP TS 23.040 ==> xxxx TP-Recipient-Address (TP-RA) */
    if(!(self->ra = tsms_address_deserialize(pdata, (pend-pdata), tsms_addr_da, &any_len)) || !any_len) {
        TSK_DEBUG_ERROR("SMS-STATUS-REPORT == Failed to parse RA address");
        failed = tsk_true;
        goto bail;
    }
    else {
        if((pdata += any_len) >= pend) {
            TSMS_ERROR_TOO_SHORT();
        }
    }

    /* 3GPP TS 23.040 ==> TP-Service-Centre-Time-Stamp (TP-SCTS)
    * 7o */
    if((pend - pdata)<=7) {
        TSMS_ERROR_TOO_SHORT();
    }
    memcpy(self->scts, pdata, 7);
    pdata += 7;

    /* 3GPP TS 23.040 ==> xxxx TP Discharge Time(TP-DT)
    * 7o */
    if((pend - pdata)<=7) {
        TSMS_ERROR_TOO_SHORT();
    }
    memcpy(self->dt, pdata, 7);
    pdata += 7;

    /* 3GPP TS 23.040 ==> 9.2.3.15 TP Status(TP-ST)
    * 1o */
    self->st = *pdata;
    if((++pdata) >= pend) {
        TSMS_ERROR_TOO_SHORT();
    }

    /* 3GPP TS 23.040 ==> xxxx TP-Parameter-Indicator (TP-PI)
    * 1o */
    self->pi = *pdata;
    if((++pdata) >= pend) {
        TSMS_ERROR_TOO_SHORT();
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

/* internal function used to serialize a SMS-STATUS-REPORT to binary content. */
int _tsms_tpdu_status_report_serialize(const tsms_tpdu_status_report_t* self, tsk_buffer_t* output)
{
    uint8_t _1byte;

    if(!self) {
        return -1;
    }

    /* SMSC address */
#if TSMS_TPDU_APPEND_SMSC
    tsms_address_serialize(self->smsc, output);
#endif

    /* SMS-STATUS-REPORT first Octect:
    	- TP-Message-Type-Indicator(2b)
    	- TP-More-Messages-to-Send(1b)
    	- TP-Loop-Prevention(1b)
    	- TP-User-Data-Header-Indicator(1b)
    	- TP-Status-Report-Qualifier(1b)
    	+----+----+----+----+----+----+----+----+
    	|    |UDHI| SRQ|	|LP	 |MMS | MTI	    |
    	+----+----+----+----+----+----+----+----+
    */
    _1byte = (TSMS_TPDU_MESSAGE(self)->mti & 0xF3); /*2b*/
    _1byte |= ((uint8_t)self->mms) << 2 /*1b*/
              | ((uint8_t)self->lp) << 3 /*1b*/
              /*1b (unused)*/
              | ((uint8_t)self->srq) << 5 /*1b*/
              | ((uint8_t)self->udhi) << 6; /*1b*/
    tsk_buffer_append(output, &_1byte, 1);

    /* 3GPP TS 23.040 ==> 9.2.3.6 TP-Message-Reference (TP-MR) */
    tsk_buffer_append(output, &self->mr, 1); /*0-255 ==> 1o*/

    /* 3GPP TS 23.040 ==> xxxx TP-Recipient-Address (TP-RA) */
    tsms_address_serialize(self->ra, output);

    /* 3GPP TS 23.040 ==> TP-Service-Centre-Time-Stamp (TP-SCTS) */
    tsk_buffer_append(output, self->scts, sizeof(self->scts)); /*7o*/

    /* 3GPP TS 23.040 ==> xxxx TP Discharge Time(TP-DT) */
    tsk_buffer_append(output, self->dt, sizeof(self->dt)); /*7o*/

    /* 3GPP TS 23.040 ==> 9.2.3.15 TP Status(TP-ST) */
    tsk_buffer_append(output, &self->st, 1); /*0-255 ==> 1o*/

    /* 3GPP TS 23.040 ==> 9.2.3.27 TP-Parameter-Indicator (TP-PI)
    	bit 7			bit 6		bit 5		bit 4		bit 3		bit 2	bit 1	bit 0
    	Extension bit	Reserved	Reserved	Reserved	Reserved	TP UDL	TP DCS	TP PID
    	As we are the serializer, we know which field should be added or not ==> append our own TP-PI field
    */
    _1byte = self->pi | 0x07 /* Bits 2,1 and 0 to '1' */;
    tsk_buffer_append(output, &_1byte, 1); /* 1o*/

    /* 3GPP TS 23.040 ==> 9.2.3.9 TP-Protocol-Identifier (TP-PID) */
    tsk_buffer_append(output, &TSMS_TPDU_MESSAGE(self)->pid, 1); /*1o*/

    /* 3GPP TS 23.040 ==> 9.2.3.10 TP-Data-Coding-Scheme (TP-DCS) */
    tsk_buffer_append(output, &TSMS_TPDU_MESSAGE(self)->dcs, 1); /*1o*/

    /* 3GPP TS 23.040 ==> 9.2.3.16 TP-User-Data-Length (TP-UDL) */
    tsk_buffer_append(output, &TSMS_TPDU_MESSAGE(self)->udl, 1); /*1o*/

    /* 3GPP TS 23.040 ==> 9.2.3.24 TP-User Data (TP-UD) */
    tsk_buffer_append(output, TSK_BUFFER_DATA(TSMS_TPDU_MESSAGE(self)->ud), TSK_BUFFER_SIZE(TSMS_TPDU_MESSAGE(self)->ud));

    return 0;
}

/**@ingroup tsms_tpdu_group
* Creates new @a SMS-STATUS-REPORT message.
* @a SMS-STATUS-REPORT messages are used to convey status reports from the SC (Service Center) to the MS (Mobile Station).<br>
* For more information, please refer to 3GPP TS 23.040 section 9.2.2.3.
* @param mr TP-Message-Reference (TP-MR) as per 3GPP TS 23.040 section 9.2.3.6.
* @param smsc The address of the SMSC. e.g. "+331253688".
* @param recipient The address of the recipient. e.g. "+331253688".
* @param status the status code as per 3GPP TS 23.040 v910 section 9.2.3.15 (TP-ST).
* @param submit Indicates whether the report is for a @a SMS-SUBMIT message or not (@a SMS-COMMAND).<br>
* See 3GPP TS 23.040 v910 section 9.2.3.26 (TP-SRQ).
* @retval SMS-STATUS-REPORT  message.
*
* See For more information, see @ref tsms_tpdu_group_STATUS_REPORT  "SMS-STATUS-REPORT".
*/
tsms_tpdu_status_report_t* tsms_tpdu_status_report_create(uint8_t mr, const tsms_address_string_t smsc, const tsms_address_string_t recipient, tsms_tpdu_status_type_t status, tsk_bool_t submit)
{
    tsms_tpdu_status_report_t* ret = tsk_null;

    if(!(ret = tsk_object_new(tsms_tpdu_status_report_def_t, mr, smsc, recipient, status))) {
        goto bail;
    }

bail:
    return ret;
}

//=================================================================================================
//	SMS TPDU SMS-SUMBIT object definition
//
static tsk_object_t* tsms_tpdu_status_report_ctor(tsk_object_t * self, va_list * app)
{
    tsms_tpdu_status_report_t *status_report = self;
    if(status_report) {
        const char* smsc, *recipient;
        uint8_t mr;
        tsms_tpdu_status_type_t status;
        tsk_bool_t submit;

#if defined(__GNUC__)
        mr = (uint8_t)va_arg(*app, unsigned);
#else
        mr = va_arg(*app, uint8_t);
#endif
        smsc = va_arg(*app, const char*);
        recipient = va_arg(*app, const char*);
        status = va_arg(*app, tsms_tpdu_status_type_t);
        submit = va_arg(*app, tsk_bool_t);

        /* init base*/
        tsms_tpdu_message_init(TSMS_TPDU_MESSAGE(status_report), tsms_tpdu_mti_status_report_mt);
        /* init self */
        status_report->mr = mr;
        if(smsc) {
            status_report->smsc = tsms_address_smsc_create((const uint8_t*)smsc);
        }
        if(recipient) {
            status_report->ra = tsms_address_da_create((const uint8_t*)recipient);
        }
        status_report->st = status;
        status_report->mms = TSMS_TPDU_DEFAULT_MMS;
        status_report->srq = submit?0:1/*SMS-COMAND*/;
    }
    else {
        TSK_DEBUG_ERROR("Null");
    }
    return self;
}

static tsk_object_t* tsms_tpdu_status_report_dtor(tsk_object_t * self)
{
    tsms_tpdu_status_report_t *status_report = self;
    if(status_report) {
        /*deinit base*/
        tsms_tpdu_message_deinit(TSMS_TPDU_MESSAGE(status_report));
        /*deinit self*/
        TSK_OBJECT_SAFE_FREE(status_report->smsc);
        TSK_OBJECT_SAFE_FREE(status_report->ra);
    }
    else {
        TSK_DEBUG_ERROR("Null");
    }
    return self;
}

static const tsk_object_def_t tsms_tpdu_status_report_def_s = {
    sizeof(tsms_tpdu_status_report_t),
    tsms_tpdu_status_report_ctor,
    tsms_tpdu_status_report_dtor,
    tsk_null,
};
const tsk_object_def_t *tsms_tpdu_status_report_def_t = &tsms_tpdu_status_report_def_s;
