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

/**@file tsms_common.c
 * @brief SMS Commons (3GPP TS 23.038 and al.).
 *
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 *

 */
#include "tinysms/tsms_common.h"

#include "tinysms/tsms_packing.h"

#include "tsk_object.h"
#include "tsk_string.h"
#include "tsk_debug.h"

#include <string.h> /* strlen() */

/* ======================== TPDU ========================
=========================================================*/

extern tsms_tpdu_message_t* _tsms_tpdu_submit_deserialize(const void* data, tsk_size_t size);
extern tsms_tpdu_message_t* _tsms_tpdu_deliver_deserialize(const void* data, tsk_size_t size);
extern tsms_tpdu_message_t* _tsms_tpdu_command_deserialize(const void* data, tsk_size_t size);
extern tsms_tpdu_message_t* _tsms_tpdu_report_deserialize(const void* data, tsk_size_t size);
extern tsms_tpdu_message_t* _tsms_tpdu_status_report_deserialize(const void* data, tsk_size_t size);

extern int _tsms_tpdu_submit_serialize(const tsms_tpdu_message_t* self, tsk_buffer_t* output);
extern int _tsms_tpdu_deliver_serialize(const tsms_tpdu_message_t* self, tsk_buffer_t* output);
extern int _tsms_tpdu_command_serialize(const tsms_tpdu_message_t* self, tsk_buffer_t* output);
extern int _tsms_tpdu_report_serialize(const tsms_tpdu_message_t* self, tsk_buffer_t* output);
extern int _tsms_tpdu_status_report_serialize(const tsms_tpdu_message_t* self, tsk_buffer_t* output);

int tsms_tpdu_message_init(tsms_tpdu_message_t* self, tsms_tpdu_mti_t mti)
{
    if(self) {
        self->MobOrig = tsk_true;
        self->mti = mti;
        self->pid = TSMS_TPDU_DEFAULT_PID;
        self->dcs = TSMS_TPDU_DEFAULT_DCS;
        return 0;
    }
    return -1;
}

/**@ingroup tsms_tpdu_group
* Serialize a @a TP-Message (@a SMS-SUBMIT, @a SMS-COMMAND, @a SMS-DELIVER, ...) as binary content.
* @param self The @a TP-Message to serialize.
* @param output A pointer to the @a output buffer.
* @param MobOrig Indicates whether the message is Mobile Originated or not.
* @retval Zero if succeed and non-zero error code otherwise.
*
* @sa @ref tsms_tpdu_submit_serialize<br>@ref tsms_tpdu_command_serialize<br>@ref tsms_tpdu_deliver_serialize<br>@ref tsms_tpdu_report_serialize<br>@ref tsms_tpdu_status_report_serialize
*/
int tsms_tpdu_message_serialize(const tsms_tpdu_message_t* self, tsk_buffer_t* output, tsk_bool_t MobOrig)
{
    if(!self) {
        return -1;
    }

    /* 3GPP TS 23.040 - 9.2.3.1	TP Message Type Indicator (TP MTI)
    bit1	bit0	Message type
    	0	0	SMS-DELIVER (in the direction SC to MS)
    	0	0	SMS-DELIVER-REPORT (in the direction MS to SC)
    	1	0	SMS-STATUS-REPORT (in the direction SC to MS)
    	1	0	SMS-COMMAND (in the direction MS to SC)
    	0	1	SMS-SUBMIT (in the direction MS to SC)
    	0	1	SMS-SUBMIT-REPORT (in the direction SC to MS)
    	1	1	Reserved
    */
    if(MobOrig) { /* MO */
        switch(self->mti) {
        case tsms_tpdu_mti_deliver_report_mo:
            return _tsms_tpdu_report_serialize(self, output); /* SMS-DELIVER-REPORT */
        case tsms_tpdu_mti_command_mo:
            return _tsms_tpdu_command_serialize(self, output); /* SMS-COMMAND */
        case tsms_tpdu_mti_submit_mo:
            return _tsms_tpdu_submit_serialize(self, output); /* SMS-SUBMIT */
        default:
            return -2;
        }
    }
    else { /* MT */
        switch(self->mti) {
        case tsms_tpdu_mti_deliver_mt:
            return _tsms_tpdu_deliver_serialize(self, output); /* SMS-DELIVER */
        case tsms_tpdu_mti_status_report_mt:
            return _tsms_tpdu_status_report_serialize(self, output); /* SMS-STATUS-REPORT */
        case tsms_tpdu_mti_submit_report_mt:
            return _tsms_tpdu_report_serialize(self, output); /* SMS-SUBMIT-REPORT */
        default:
            return -2;
        }
    }
}

/**@ingroup tsms_tpdu_group
* Deserialize a @a TP-Message (@a SMS-SUBMIT, @a SMS-COMMAND, @a SMS-DELIVER, ...) from binary content.
* @param data A pointer to the binary data.
* @param size The size of the buffer holding the binary data.
* @param MobOrig Indicates whether the binaray content is Mobile Originated (Sent by the MS) or not.
* @retval @ref tsms_tpdu_message_t if succeed and @a Null otherwise.
*/
tsms_tpdu_message_t* tsms_tpdu_message_deserialize(const void* data, tsk_size_t size,  tsk_bool_t MobOrig)
{
    tsms_tpdu_mti_t mti;
    uint8_t smsc_len = 0;
    tsms_tpdu_message_t* ret = tsk_null;

    if(!data || size<=1) {
        TSK_DEBUG_ERROR("Invalid parameter.");
        return tsk_null;
    }

#if TSMS_TPDU_APPEND_SMSC
    smsc_len = *((uint8_t*)data) + 1/* len itself*/;
    if(smsc_len>=size) {
        TSK_DEBUG_ERROR("Too short.");
        return tsk_null;
    }
#endif

    mti = (*(((uint8_t*)data)+smsc_len) & 0x03);

    /* 3GPP TS 23.040 - 9.2.3.1	TP Message Type Indicator (TP MTI)
    bit1	bit0	Message type
    	0	0	SMS-DELIVER (in the direction SC to MS)
    	0	0	SMS-DELIVER-REPORT (in the direction MS to SC)
    	1	0	SMS-STATUS-REPORT (in the direction SC to MS)
    	1	0	SMS-COMMAND (in the direction MS to SC)
    	0	1	SMS-SUBMIT (in the direction MS to SC)
    	0	1	SMS-SUBMIT-REPORT (in the direction SC to MS)
    	1	1	Reserved
    */
    if(MobOrig) { /* MO */
        switch(mti) {
        case tsms_tpdu_mti_deliver_report_mo:
            ret = _tsms_tpdu_report_deserialize(data, size);
            break; /* SMS-DELIVER-REPORT */
        case tsms_tpdu_mti_command_mo:
            ret = _tsms_tpdu_command_deserialize(data, size);
            break; /* SMS-COMMAND */
        case tsms_tpdu_mti_submit_mo:
            ret = _tsms_tpdu_submit_deserialize(data, size);
            break; /* SMS-SUBMIT */
        default:
            return tsk_null;
        }
    }
    else { /* MT */
        switch(mti) {
        case tsms_tpdu_mti_deliver_mt:
            ret = _tsms_tpdu_deliver_deserialize(data, size);
            break;/* SMS-DELIVER */
        case tsms_tpdu_mti_status_report_mt:
            ret = _tsms_tpdu_status_report_deserialize(data, size);
            break;/* SMS-STATUS-REPORT */
        case tsms_tpdu_mti_submit_report_mt:
            ret = _tsms_tpdu_report_deserialize(data, size);
            break;/* SMS-SUBMIT-REPORT */
        default:
            return tsk_null;
        }
    }

    if(ret) {
        ret->MobOrig = MobOrig;
    }
    return ret;
}

/**@ingroup tsms_tpdu_group
* For debugging.
*/
char* tsms_tpdu_message_tostring(const tsms_tpdu_message_t* self, tsk_bool_t MobOrig)
{
    char* ret = tsk_null;
    tsk_buffer_t* output = tsk_null;
    if(!self) {
        goto bail;
    }

    if((output = tsk_buffer_create_null())) {
        if(!tsms_tpdu_message_serialize(self, output, MobOrig)) {
            ret = tsk_strndup(output->data, output->size);
        }
        TSK_OBJECT_SAFE_FREE(output);
    }

bail:
    return ret;
}

/**@ingroup tsms_tpdu_group
* Serializes any  @a TP-Message(@a SMS-SUBMIT, @a SMS-DELIVER, @a SMS-COMMAND ...) as hexa-string content.
* @param self A pointer to the @a TP-Message to serialize.
* @param MobOrig Indicates whether its an Mobile originated message or not.
* @retval A pointer to the hexa-string if succeed and Null otherwise.
*
* @sa @ref tsms_tpdu_submit_tohexastring<br>@ref tsms_tpdu_command_tohexastring<br>@ref tsms_tpdu_deliver_tohexastring<br>@ref tsms_tpdu_report_tohexastring<br>@ref tsms_tpdu_status_report_tohexastring
*/
char* tsms_tpdu_message_tohexastring(const tsms_tpdu_message_t* self, tsk_bool_t MobOrig)
{
    char* ret = tsk_null;
    tsk_buffer_t* output = tsk_null;
    if(!self) {
        goto bail;
    }

    if((output = tsk_buffer_create_null())) {
        if(!tsms_tpdu_message_serialize(self, output, MobOrig)) {
            tsk_size_t i;
            for(i=0; i<output->size; i++) {
                tsk_strcat_2(&ret, "%.2X", *(TSK_BUFFER_TO_U8(output)+i));
            }
        }
        TSK_OBJECT_SAFE_FREE(output);
    }

bail:
    return ret;
}

/**@ingroup tsms_tpdu_group
* Gets the payload (ASCII string) of the @a TP-Message(@a SMS-SUBMIT, @a SMS-DELIVER, @a SMS-COMMAND ...).
* @param self Any @a TP-Message which holds the user content. Should be @a SMS-SUBMIT or @a SMS-DELIVER.
* @retval A pointer to the ascii string if succeed and non-zero error code otherwise.
*/
char* tsms_tpdu_message_get_payload(const tsms_tpdu_message_t* self)
{
    if(!self || !self->ud || !self->ud->data || !self->ud->size) {
        TSK_DEBUG_WARN("No content.");
        return tsk_null;
    }

    switch(TSMS_ALPHA_FROM_DCS(self->dcs)) {
    case tsms_alpha_7bit:
        return tsms_pack_from_7bit(self->ud->data, self->ud->size);
    case tsms_alpha_8bit:
        return tsms_pack_from_8bit(self->ud->data, self->ud->size);
    case tsms_alpha_ucs2:
        return tsms_pack_from_ucs2(self->ud->data, self->ud->size);
    case tsms_alpha_reserved:
    default: {
        TSK_DEBUG_ERROR("%d alpha not suported", TSMS_ALPHA_FROM_DCS(self->dcs));
        return tsk_null;
    }
    }
}

/**@ingroup tsms_tpdu_group
* Sets the content of any @a TP-Message(@a SMS-SUBMIT, @a SMS-DELIVER, @a SMS-COMMAND ...).
* @param self A pointer to the message.
* @param udata A pointer the content.
* @param alpha The alphabet used to encode the content.
* @retval Zero if succeed and non-zero error code otherwise.
* @sa @ref tsms_tpdu_submit_set_userdata<br>@ref tsms_tpdu_command_set_cmddata<br>@ref tsms_tpdu_report_set_userdata<br>@ref tsms_tpdu_status_report_set_userdata
*/
int tsms_tpdu_message_set_userdata(tsms_tpdu_message_t* self, const tsk_buffer_t* udata, tsms_alphabet_t alpha)
{
    if(!self || !udata) {
        return -1;
    }
    /* SMS alphabet values as per 3GPP TS 23.038 v911 section 4.
    * Part of TP-DCS (SMS Data Coding Scheme).
    */
    self->dcs = ((self->dcs & 0xF3) | (alpha << 2)); /* Bit3 and Bit2 */

    TSK_OBJECT_SAFE_FREE(self->ud);

    /* 3GPP TS 23.040 ==> 9.2.3.16 TP-User-Data-Length (TP-UDL)
    * (alpha = SMS_ALPHA_7bit) ==> number of septets.
    * ((alpha == SMS_ALPHA_8bit) || (alpha == SMS_ALPHA_UCS2)) ==> number of octes.
    */

    /* 3GPP TS 23.040 ==> 9.2.3.24 TP-User Data (TP-UD) */
    switch(alpha) {
    case tsms_alpha_7bit: {
        self->udl = (udata->size) + (udata->size/7);
        self->ud = tsk_buffer_create(udata->data, udata->size);
    }
    break;
    case tsms_alpha_8bit:
    case tsms_alpha_ucs2: {
        self->udl = udata->size;
        self->ud = tsk_buffer_create(udata->data, udata->size);
    }
    break;

    default: {
        TSK_DEBUG_ERROR("Invalid Alphabet.");
        return -2;
    }
    }
    return 0;
}

/** internal function used to deinit a TP-Message.*/
int tsms_tpdu_message_deinit(tsms_tpdu_message_t* self)
{
    if(self) {
        TSK_OBJECT_SAFE_FREE(self->ud);
        return 0;
    }
    return -1;
}



/* ======================== RPDU ========================
=========================================================*/

extern int _tsms_rpdu_rpdata_serialize(const tsms_rpdu_message_t* self, tsk_buffer_t* output);
extern int _tsms_rpdu_rpsmma_serialize(const tsms_rpdu_message_t* self, tsk_buffer_t* output);
extern int _tsms_rpdu_rpack_serialize(const tsms_rpdu_message_t* self, tsk_buffer_t* output);
extern int _tsms_rpdu_rperror_serialize(const tsms_rpdu_message_t* self, tsk_buffer_t* output);


extern tsms_rpdu_message_t* _tsms_rpdu_rpdata_deserialize(const void* data, tsk_size_t size);
extern tsms_rpdu_message_t* _tsms_rpdu_rpsmma_deserialize(const void* data, tsk_size_t size);
extern tsms_rpdu_message_t* _tsms_rpdu_rpack_deserialize(const void* data, tsk_size_t size);
extern tsms_rpdu_message_t* _tsms_rpdu_rperror_deserialize(const void* data, tsk_size_t size);


/**@ingroup tsms_rpdu_group
* Serialize a @a RP-Message (@a RP-DATA, @a RP-SMMA, @a SMS-ACK, ...) as binary content.
* @param self A pointer to the @a RP-Message to serialize.
* @param output A pointer to the @a output buffer.
* @retval Zero if succeed and non-zero error code otherwise.
*
* @sa @ref tsms_rpdu_data_serialize<br>@ref tsms_rpdu_smma_serialize<br>@ref tsms_rpdu_ack_serialize<br>@ref tsms_rpdu_error_serialize
*/
int tsms_rpdu_message_serialize(const tsms_rpdu_message_t* self, tsk_buffer_t* output)
{
    if(!self || !output) {
        TSK_DEBUG_ERROR("Invalid Parameter");
        return -1;
    }

    switch(self->mti) {
    case tsms_rpdu_type_data_mo:
    case tsms_rpdu_type_data_mt:
        return _tsms_rpdu_rpdata_serialize(self, output);
    case tsms_rpdu_type_ack_mo:
    case tsms_rpdu_type_ack_mt:
        return _tsms_rpdu_rpack_serialize(self, output);
    case tsms_rpdu_type_error_mo:
    case tsms_rpdu_type_error_mt:
        return _tsms_rpdu_rperror_serialize(self, output);
    case tsms_rpdu_type_smma_mo:
        return _tsms_rpdu_rpsmma_serialize(self, output);
    }

    return -2;
}

/**@ingroup tsms_rpdu_group
* Deserialize a @a RP-Message (@a RP-DATA, @a RP-SMMA, @a SMS-ACK, ...) from binary content.
* @param data A pointer to the binary data.
* @param size The size of the buffer holding the binary data.
* @retval @ref tsms_rpdu_message_t if succeed and @a Null otherwise.
*/
tsms_rpdu_message_t* tsms_rpdu_message_deserialize(const void* data, tsk_size_t size)
{
    tsms_rpdu_type_t mti;

    if(!data || size<2 /* MTI and MR*/) {
        TSK_DEBUG_ERROR("Invalid parameter.");
        return tsk_null;
    }

    mti = (*((uint8_t*)data) & 0x07);

    switch(mti) {
    case tsms_rpdu_type_data_mo:
    case tsms_rpdu_type_data_mt:
        return _tsms_rpdu_rpdata_deserialize(data, size);
    case tsms_rpdu_type_ack_mo:
    case tsms_rpdu_type_ack_mt:
        return _tsms_rpdu_rpack_deserialize(data, size);
    case tsms_rpdu_type_error_mo:
    case tsms_rpdu_type_error_mt:
        return _tsms_rpdu_rperror_deserialize(data, size);
    case tsms_rpdu_type_smma_mo:
        return _tsms_rpdu_rpsmma_deserialize(data, size);
    }

    return tsk_null;
}

/**@ingroup tsms_rpdu_group
* Serializes any  @a RP-Message (@a RP-DATA, @a RP-SMMA, @a SMS-ACK, ...) as hexa-string content.
* @param self A pointer to the @a RP-Message to serialize.
* @retval A pointer to the hexa-string if succeed and Null otherwise.
*/
char* tsms_rpdu_message_tohexastring(const tsms_rpdu_message_t* self)
{
    char* ret = tsk_null;
    tsk_buffer_t* output = tsk_null;
    if(!self) {
        goto bail;
    }

    if((output = tsk_buffer_create_null())) {
        if(!tsms_rpdu_message_serialize(self, output)) {
            tsk_size_t i;
            for(i=0; i<output->size; i++) {
                tsk_strcat_2(&ret, "%.2X", *(TSK_BUFFER_TO_U8(output)+i));
            }
        }
        TSK_OBJECT_SAFE_FREE(output);
    }

bail:
    return ret;
}
