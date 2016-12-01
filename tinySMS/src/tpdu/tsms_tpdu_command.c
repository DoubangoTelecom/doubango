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

/**@file tsms_tpdu_command.c
 * @brief SMS TPDU SMS-COMMAND message as per 3GPP TS 23.040 section 9.2.2.4.
 *
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 *

 */
#include "tinysms/tpdu/tsms_tpdu_command.h"

#include "tsk_debug.h"

#define TSMS_ERROR_TOO_SHORT()\
	TSK_DEBUG_ERROR("SMS-COMMAND == Data too short.");\
	failed = tsk_true;\
	goto bail;

/** internal function used to deserialse a SMS-COMMAND message from binary content. */
tsms_tpdu_message_t* _tsms_tpdu_command_deserialize(const void* data, tsk_size_t size)
{
    /* You don't need to test data and test, this is an internal function called by tsms_tpdu_message_deserialize() */
    tsms_tpdu_command_t* self = tsms_tpdu_command_create(0, tsk_null, tsk_null, 0, tsms_tpdu_cmd_rel);
    tsk_bool_t failed = tsk_false;
    tsk_size_t any_len;
    const uint8_t* pdata = data;
    const uint8_t* pend = pdata + size;

    /* SMSC address */
#if TSMS_TPDU_APPEND_SMSC
    if(!(self->smsc = tsms_address_deserialize(data, size, tsms_addr_smsc, &any_len)) || !any_len) {
        TSK_DEBUG_ERROR("SMS-COMMAND == Failed to parse SMSC address");
        failed = tsk_true;
        goto bail;
    }
    else {
        if((pdata += any_len) >= pend) {
            TSMS_ERROR_TOO_SHORT();
        }
    }
#endif

    /* SMS-COMMAND first Octect:
    	- TP-Message-Type-Indicator(2b)
    	- TP-User-Data-Header-Indicator(1b)
    	- TP-Status-Report-Request(1b)

    	+----+----+----+----+----+----+----+----+
    	|    |UDHI|SRR |	|	 |    | MTI	    |
    	+----+----+----+----+----+----+----+----+
    */
    TSMS_TPDU_MESSAGE(self)->mti = (*pdata & 0x03);
    self->srr = (*pdata & 0x20)>>5,
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

    /* 3GPP TS 23.040 ==> 9.2.3.9 TP-Protocol-Identifier (TP-PID)
    * 1o */
    TSMS_TPDU_MESSAGE(self)->pid = *pdata;
    if((++pdata) >= pend) {
        TSMS_ERROR_TOO_SHORT();
    }

    /* 3GPP TS 23.040 ==> 9.2.3.19 TP-Command-Type (TP-CT)
    * 1o */
    self->ct = *pdata;
    if((++pdata) >= pend) {
        TSMS_ERROR_TOO_SHORT();
    }

    /* 3GPP TS 23.040 ==> 9.2.3.18 TP-Message-Number (TP-MN)
    * 1o */
    self->mn = *pdata;
    if((++pdata) >= pend) {
        TSMS_ERROR_TOO_SHORT();
    }

    /* 3GPP TS 23.040 ==> 9.2.3.8 TP-Destination-Address (TP-DA) */
    if(!(self->da = tsms_address_deserialize(pdata, (pend-pdata), tsms_addr_da, &any_len)) || !any_len) {
        TSK_DEBUG_ERROR("SMS-DELIVER == Failed to parse DA address");
        failed = tsk_true;
        goto bail;
    }
    else {
        if((pdata += any_len) >= pend) {
            TSMS_ERROR_TOO_SHORT();
        }
    }

    /* 3GPP TS 23.040 ==> 9.2.3.20	TP Command Data Length (TP CDL)
    * 1o */
    TSMS_TPDU_MESSAGE(self)->udl = *pdata;
    pdata++;

    /* 3GPP TS 23.040 ==> 9.2.3.21	TP Command Data (TP CD) */
    if((pend-pdata) > 0) {
        TSMS_TPDU_MESSAGE(self)->ud = tsk_buffer_create(pdata, (pend-pdata));
    }

bail:
    if(failed) {
        TSK_OBJECT_SAFE_FREE(self);
    }
    return TSMS_TPDU_MESSAGE(self);
}

/** internal function used to serialize a SMS-COMMAND message as binary content. */
int _tsms_tpdu_command_serialize(const tsms_tpdu_command_t* self, tsk_buffer_t* output)
{
    uint8_t _1byte;

    if(!self) {
        return -1;
    }

    /* SMSC address */
#if TSMS_TPDU_APPEND_SMSC
    tsms_address_serialize(self->smsc, output);
#endif

    /* SMS-COMMAND first Octect:
    	- TP-Message-Type-Indicator(2b)
    	- TP-User-Data-Header-Indicator(1b)
    	- TP-Status-Report-Request(1b)

    	+----+----+----+----+----+----+----+----+
    	|    |UDHI|SRR |	|	 |    | MTI	    |
    	+----+----+----+----+----+----+----+----+
    */
    _1byte = (TSMS_TPDU_MESSAGE(self)->mti & 0xF3); /*2b*/
    _1byte |=
        /*1b*/
        /*2b*/
        ((uint8_t)self->srr) << 5 /*1b*/
        | ((uint8_t)self->udhi) << 6; /*1b*/
    tsk_buffer_append(output, &_1byte, 1);

    /* 3GPP TS 23.040 ==> 9.2.3.6 TP-Message-Reference (TP-MR) */
    tsk_buffer_append(output, &self->mr, 1); /*0-255 ==> 1o*/

    /* 3GPP TS 23.040 ==> 9.2.3.9 TP-Protocol-Identifier (TP-PID) */
    tsk_buffer_append(output, &TSMS_TPDU_MESSAGE(self)->pid, 1); /*1o*/

    /* 3GPP TS 23.040 ==> 9.2.3.19 TP-Command-Type (TP-CT) */
    tsk_buffer_append(output, &self->ct, 1); /*1o*/

    /* 3GPP TS 23.040 ==> 9.2.3.18 TP-Message-Number (TP-MN) */
    tsk_buffer_append(output, &self->mn, 1); /*1o*/

    /* 3GPP TS 23.040 ==> 9.2.3.8 TP-Destination-Address (TP-DA) */
    tsms_address_serialize(self->da, output);

    /* 3GPP TS 23.040 ==> 9.2.3.20	TP Command Data Length (TP CDL) */
    tsk_buffer_append(output, &TSMS_TPDU_MESSAGE(self)->udl, 1); /*1o*/

    /* 3GPP TS 23.040 ==> 9.2.3.21	TP Command Data (TP CD) */
    tsk_buffer_append(output, TSK_BUFFER_DATA(TSMS_TPDU_MESSAGE(self)->ud), TSK_BUFFER_SIZE(TSMS_TPDU_MESSAGE(self)->ud));

    return 0;
}

/**@ingroup tsms_tpdu_group
* Creates new @a SMS-COMMAND message.
* @a SMS-COMMAND messages are used to convey commands from the MS (Mobile Station) to the SC (Service Center).<br>
* For more information, please refer to 3GPP TS 23.040 section 9.2.2.4.
* @param mr TP-Message-Reference (TP-MR) as per 3GPP TS 23.040 section 9.2.3.6.
* @param smsc The address of the SMSC. e.g. "+331253688".
* @param dest The address of the destination. e.g. "+331253688".
* @param msg_num The message number. For more information, please refer to 3GPP TS 23.040 section 9.2.3.18 (TP-MN).
* @param cmd The command type as per 3GPP TS 23.040 v910 section 9.2.3.19 (TP-CT).
* @retval SMS-COMMAND  message.
*
* See For more information, see @ref tsms_tpdu_group_COMMAND  "SMS-COMMAND".
*/
tsms_tpdu_command_t* tsms_tpdu_command_create(uint8_t mr, const tsms_address_string_t smsc, const tsms_address_string_t dest, uint8_t msg_num, tsms_tpdu_cmd_t cmd)
{
    return tsk_object_new(tsms_tpdu_command_def_t, mr, smsc, dest, msg_num, cmd);
}



//=================================================================================================
//	SMS TPDU SMS-COMMAND object definition
//
static tsk_object_t* tsms_tpdu_command_ctor(tsk_object_t * self, va_list * app)
{
    tsms_tpdu_command_t *command = self;
    if(command) {
        const char* smsc, *dest;
        uint8_t mr, msg_num;
        tsms_tpdu_cmd_t cmd;

#if defined(__GNUC__)
        mr = (uint8_t)va_arg(*app, unsigned);
#else
        mr = va_arg(*app, uint8_t);
#endif

        smsc = va_arg(*app, const char*);
        dest = va_arg(*app, const char*);

#if defined(__GNUC__)
        msg_num = (uint8_t)va_arg(*app, unsigned);
#else
        msg_num = va_arg(*app, uint8_t);
#endif
        cmd = va_arg(*app, tsms_tpdu_cmd_t);

        /* init base*/
        tsms_tpdu_message_init(TSMS_TPDU_MESSAGE(command), tsms_tpdu_mti_command_mo);
        /* init self */
        command->mr = mr;
        if(smsc) {
            command->smsc = tsms_address_smsc_create((const uint8_t*)smsc);
        }
        if(dest) {
            command->da = tsms_address_da_create((const uint8_t*)dest);
        }
        command->mn = msg_num;
        command->ct = cmd;
    }
    return self;
}

static tsk_object_t* tsms_tpdu_command_dtor(tsk_object_t * self)
{
    tsms_tpdu_command_t *command = self;
    if(command) {
        /*deinit base*/
        tsms_tpdu_message_deinit(TSMS_TPDU_MESSAGE(command));
        /*deinit self*/
        TSK_OBJECT_SAFE_FREE(command->smsc);
        TSK_OBJECT_SAFE_FREE(command->da);
    }

    return self;
}

static const tsk_object_def_t tsms_tpdu_command_def_s = {
    sizeof(tsms_tpdu_command_t),
    tsms_tpdu_command_ctor,
    tsms_tpdu_command_dtor,
    tsk_null,
};
const tsk_object_def_t *tsms_tpdu_command_def_t = &tsms_tpdu_command_def_s;
