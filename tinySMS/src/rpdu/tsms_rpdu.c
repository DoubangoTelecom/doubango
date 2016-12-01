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

/**@file tsms_rpdu.c
 * @brief SMS RPDU encoder/decoder as per 3GPP TS 23.011.
 *
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 *

 */
#include "tinysms/rpdu/tsms_rpdu.h"


#include "tsk_debug.h"

#define TSMS_ERROR_TOO_SHORT(type)\
	TSK_DEBUG_ERROR("%s == Data too short.", type);\
	failed = tsk_true;\
	goto bail;

/*=== RP-DATA as per 3GPP TS 24.011 section 7.3.1. ===*/

/** internal function used to deserialize a RP-DATA message from binary content. */
tsms_rpdu_message_t* _tsms_rpdu_rpdata_deserialize(const void* data, tsk_size_t size)
{
    /* don't need to test data and size --> already done by tsms_rpdu_message_deserialize() */
    tsms_rpdu_data_t* self = tsms_rpdu_data_create(0x00, tsk_null, tsk_null, tsk_false);
    tsk_bool_t failed = tsk_false;
    const uint8_t* pdata = data;
    const uint8_t* pend = pdata + size;
    tsk_size_t smsc_len;

    /* 3GPP TS 24.011 ==> 8.2.2 Message type indicator (MTI) */
    TSMS_RPDU_MESSAGE(self)->mti = *pdata++;

    /* 3GPP TS 24.011 ==> 8.2.3 Message reference */
    TSMS_RPDU_MESSAGE(self)->mr = *pdata++;

    /* 3GPP TS 24.011 ==> 8.2.5.1 Originator address element */
    if(TSMS_RPDU_MESSAGE(self)->mti == tsms_rpdu_type_data_mt) {
        /* 7.3.1.1	RP DATA (Network to Mobile Station) */
        if((self->oa = tsms_address_deserialize(pdata, (pend-pdata), tsms_addr_smsc, &smsc_len))) {
            pdata += smsc_len;
        }
        /* 7.3.1.2	RP DATA (Mobile Station to Network) */
        pdata++; /* 1o */
    }
    else if(TSMS_RPDU_MESSAGE(self)->mti == tsms_rpdu_type_data_mo) {
        /* 7.3.1.1	RP DATA (Network to Mobile Station) */
        pdata++; /* 1o */
        /* 7.3.1.2	RP DATA (Mobile Station to Network) */
        if((self->da = tsms_address_deserialize(pdata, (pend-pdata), tsms_addr_smsc, &smsc_len))) {
            pdata += smsc_len;
        }
    }
    else {
        failed = tsk_true;
        TSK_DEBUG_ERROR("%d Not a valid RP-DATA messsage", TSMS_RPDU_MESSAGE(self)->mti);
        goto bail;
    }

    /* 3GPP TS 24.011 ==> 8.2.5.3 RP-User data element */
    if((pend-pdata) > 1) {
        tsk_size_t length = *pdata++; // length-indicator
        if((length != (pend-pdata))) {
            TSK_DEBUG_WARN("Invalid length-indicator.");
        }
        else {
            self->udata = tsk_buffer_create(pdata, length);
        }
    }
    else {
        TSK_DEBUG_WARN("RP-User data element is mandatory for RP-DATA messages.");
    }


bail:
    if(failed) {
        TSK_OBJECT_SAFE_FREE(self);
    }
    return TSMS_RPDU_MESSAGE(self);
}

/** internal function used to serialize a RP-DATA message as binary content.*/
int _tsms_rpdu_rpdata_serialize(const tsms_rpdu_message_t* _self, tsk_buffer_t* output)
{
    /* don't need to test output and self --> already done by tsms_rpdu_message_serialize() */
    const tsms_rpdu_data_t* self = (const tsms_rpdu_data_t*)_self;

    /* 3GPP TS 24.011 ==> 8.2.2 Message type indicator (MTI) */
    tsk_buffer_append(output, &_self->mti, 1);

    /* 3GPP TS 24.011 ==> 8.2.3 Message reference */
    tsk_buffer_append(output, &_self->mr, 1);

    /* 3GPP TS 24.011 ==> 8.2.5.1 Originator address element */
    tsms_address_serialize(self->oa, output);

    /* 3GPP TS 24.011 ==> 8.2.5.2 Destination address element */
    tsms_address_serialize(self->da, output);

    /* 3GPP TS 24.011 ==> 8.2.5.3 RP-User data element */
    if(self->udata) {
        tsk_buffer_append(output, &self->udata->size, 1); // length-indicator
        tsk_buffer_append(output, self->udata->data, self->udata->size);
    }
    else {
        static uint8_t _1byte = 0x00;
        tsk_buffer_append(output, &_1byte, 1); // length-indicator
    }

    return 0;
}

/**@ingroup tsms_rpdu_group
* Creates a @a RP-DATA message.
* @a RP-DATA messages could be sent from the Network to Mobile Station (MT) or from the Mobile Station to the Network (MO).
* For more information, please refer to 3GPP TS 24.011 section 7.3.1.
* @param mr @a RP-Message Reference as per 3GPP TS 24.011 section 8.2.3.
* @param smsc The address of the SMSC. e.g. "+331253688".
* @param tpdu A Pointer to the @a TP-Message to relay.
* @param MobOrig Indicates whether to create a Mobile Originated @a RP-DATA message or not.
* @retval Mobile Originated @a RP-DATA message.
* @sa @ref tsms_rpdu_data_create_mt<br>@ref tsms_rpdu_data_create_mo
*
* See For more information, see @ref tsms_rpdu_group_DATA "RP-DATA".
*/
tsms_rpdu_data_t* tsms_rpdu_data_create(uint8_t mr, const tsms_address_string_t smsc, const tsms_tpdu_message_t* tpdu, tsk_bool_t MobOrig)
{
    tsms_rpdu_data_t* ret = tsk_null;

    if(!(ret = tsk_object_new(tsms_rpdu_data_def_t))) {
        goto bail;
    }

    /* 3GPP TS 24.011 ==> 8.2.2 Message type indicator (MTI) */
    TSMS_RPDU_MESSAGE(ret)->mti = MobOrig? tsms_rpdu_type_data_mo : tsms_rpdu_type_data_mt;

    /* 3GPP TS 24.011 ==> 8.2.3 Message reference */
    TSMS_RPDU_MESSAGE(ret)->mr = mr;

    /* 3GPP TS 24.341 section B5 (Moblie Originated RP-DATA message)
    	The payload includes an RP-DATA message (see 3GPP TS 24.011 [8]). It includes:
    	-	Address of the originating UE: this field includes the length indicator only;
    	-	Address of the destination SC, which is configured in the UE; and
    	-	RP-User-Data (see 3GPP TS 23.040 [3]), which includes SMS-SUBMIT as type indicator.
    */


    /* 3GPP TS 24.011 ==> 8.2.5.1 Originator address element */
    if(smsc) {
        if(TSMS_RPDU_MESSAGE(ret)->mti == tsms_rpdu_type_data_mt) {
            /* 7.3.1.1	RP DATA (Network to Mobile Station) */
            ret->oa = tsms_address_smsc_create(smsc);
        }
        else {
            /* 7.3.1.2	RP DATA (Mobile Station to Network) */
            ret->da = tsms_address_smsc_create(smsc);
        }
    }

    /* 3GPP TS 24.011 ==> 8.2.5.3 RP-User data element */
    if(tpdu && (ret->udata = tsk_buffer_create_null())) {
        tsms_tpdu_message_serialize(tpdu, ret->udata, MobOrig);
    }

bail:
    return ret;
}

//
//	RP-DATA object definition
//
static tsk_object_t* tsms_rpdu_data_ctor(tsk_object_t * self, va_list * app)
{
    tsms_rpdu_data_t *rpdata = self;
    if(rpdata) {
        TSMS_RPDU_MESSAGE(rpdata)->mti = tsms_rpdu_type_data_mo;
    }
    else {
        TSK_DEBUG_ERROR("Null");
    }
    return self;
}

static tsk_object_t* tsms_rpdu_data_dtor(tsk_object_t * self)
{
    tsms_rpdu_data_t *rpdata = self;
    if(rpdata) {
        TSK_OBJECT_SAFE_FREE(rpdata->oa);
        TSK_OBJECT_SAFE_FREE(rpdata->da);
        TSK_OBJECT_SAFE_FREE(rpdata->udata);
    }
    else {
        TSK_DEBUG_ERROR("Null");
    }

    return self;
}

static const tsk_object_def_t tsms_rpdu_data_def_s = {
    sizeof(tsms_rpdu_data_t),
    tsms_rpdu_data_ctor,
    tsms_rpdu_data_dtor,
    tsk_null,
};
const tsk_object_def_t *tsms_rpdu_data_def_t = &tsms_rpdu_data_def_s;

/*=== RP-SMMA as per 3GPP TS 24.011 section 7.3.2. ===*/

/** internal function used to deserialize a RP-SMMA message from binary content. */
tsms_rpdu_message_t* _tsms_rpdu_rpsmma_deserialize(const void* data, tsk_size_t size)
{
    /* don't need to test data and size --> already done by tsms_rpdu_message_deserialize() */
    tsms_rpdu_smma_t* self = tsms_rpdu_smma_create(0);
    const uint8_t* pdata = data;

    /* 3GPP TS 24.011 ==> 8.2.2 Message type indicator (MTI) */
    TSMS_RPDU_MESSAGE(self)->mti = *pdata++;

    /* 3GPP TS 24.011 ==> 8.2.3 Message reference */
    TSMS_RPDU_MESSAGE(self)->mr = *pdata++;

    return TSMS_RPDU_MESSAGE(self);
}

/** internal function used to serialize a RP-SMMA message as binary content. */
int _tsms_rpdu_rpsmma_serialize(const tsms_rpdu_message_t* _self, tsk_buffer_t* output)
{
    /* don't need to test output and self --> already done by tsms_rpdu_message_serialize() */
    //const tsms_rpdu_smma_t* self = (const tsms_rpdu_smma_t*)_self;

    /* 3GPP TS 24.011 ==> 8.2.2 Message type indicator (MTI) */
    tsk_buffer_append(output, &_self->mti, 1);

    /* 3GPP TS 24.011 ==> 8.2.3 Message reference */
    tsk_buffer_append(output, &_self->mr, 1);

    return 0;
}

/**@ingroup tsms_rpdu_group
* Creates a @a RP-SMMA message.
* This message is sent by the mobile station to relay a notification to the network that the mobile has memory available to receive one or more short messages.
* For more information, please refer to 3GPP TS 24.011 section 7.3.2.
* @param mr @a RP-Message Reference as per 3GPP TS 24.011 section 8.2.3.
* @retval Mobile Originated @a RP-SMMA message.
*
* See For more information, see @ref tsms_rpdu_group_SMMA "RP-SMMA".
*/
tsms_rpdu_smma_t* tsms_rpdu_smma_create(uint8_t mr)
{
    tsms_rpdu_smma_t* ret = tsk_null;

    if((ret = tsk_object_new(tsms_rpdu_smma_def_t))) {
        TSMS_RPDU_MESSAGE(ret)->mr = mr;
    }
    return ret;
}

//
//	RP-SMMA object definition
//
static tsk_object_t* tsms_rpdu_smma_ctor(tsk_object_t * self, va_list * app)
{
    tsms_rpdu_smma_t *smma = self;
    if(smma) {
        TSMS_RPDU_MESSAGE(smma)->mti = tsms_rpdu_type_smma_mo;
    }
    else {
        TSK_DEBUG_ERROR("Null");
    }
    return self;
}

static tsk_object_t* tsms_rpdu_smma_dtor(tsk_object_t * self)
{
    tsms_rpdu_smma_t *smma = self;
    if(smma) {
    }
    else {
        TSK_DEBUG_ERROR("Null");
    }

    return self;
}

static const tsk_object_def_t tsms_rpdu_smma_def_s = {
    sizeof(tsms_rpdu_smma_t),
    tsms_rpdu_smma_ctor,
    tsms_rpdu_smma_dtor,
    tsk_null,
};
const tsk_object_def_t *tsms_rpdu_smma_def_t = &tsms_rpdu_smma_def_s;


/*=== RP-ACK as per 3GPP TS 24.011 section 7.3.3. ===*/

/** internal function used to deserialize an RP-ACK message from binary content. */
tsms_rpdu_message_t* _tsms_rpdu_rpack_deserialize(const void* data, tsk_size_t size)
{
    /* don't need to test data and size --> already done by tsms_rpdu_message_deserialize() */
    tsms_rpdu_ack_t* self = tsms_rpdu_ack_create(0, tsk_null, tsk_false);
    const uint8_t* pdata = data;
    const uint8_t* pend = pdata + size;

    /* 3GPP TS 24.011 ==> 8.2.2 Message type indicator (MTI) */
    TSMS_RPDU_MESSAGE(self)->mti = *pdata++;

    /* 3GPP TS 24.011 ==> 8.2.3 Message reference */
    TSMS_RPDU_MESSAGE(self)->mr = *pdata++;

    /* 3GPP TS 24.011 ==> 8.2.5.3 RP-User data element */
    if((pend-pdata) > 1) {
        tsk_size_t length;
        /*uint8_t iei =*/ (void)(*pdata++); // IEI MUST be equal to 0x41
        length = *pdata++; // length-indicator
        if((length != (pend-pdata))) {
            TSK_DEBUG_WARN("Invalid length-indicator.");
        }
        else {
            self->udata = tsk_buffer_create(pdata, length);
        }
    }

    return TSMS_RPDU_MESSAGE(self);
}

/** internal function used to serialize a RP-ACK message as binary content. */
int _tsms_rpdu_rpack_serialize(const tsms_rpdu_message_t* _self, tsk_buffer_t* output)
{
    /* don't need to test output and self --> already done by tsms_rpdu_message_serialize() */
    const tsms_rpdu_ack_t* self = (const tsms_rpdu_ack_t*)_self;

    /* 3GPP TS 24.011 ==> 8.2.2 Message type indicator (MTI) */
    tsk_buffer_append(output, &_self->mti, 1);

    /* 3GPP TS 24.011 ==> 8.2.3 Message reference */
    tsk_buffer_append(output, &_self->mr, 1);

    /* 3GPP TS 24.011 ==> 8.2.5.3 RP-User data element */
    if(self->udata) {
        static uint8_t iei = 0x41;
        tsk_buffer_append(output, &iei, 1); // IEI
        tsk_buffer_append(output, &self->udata->size, 1); // length-indicator
        tsk_buffer_append(output, self->udata->data, self->udata->size);
    }
    else {
        static uint8_t _1byte = 0x00;
        tsk_buffer_append(output, &_1byte, 1); // length-indicator
    }

    return 0;
}

/**@ingroup tsms_rpdu_group
* Creates a @a RP-ACK message.
* @a RP-ACK is sent between the MSC and the mobile station in both directions and used to relay the acknowledgement of a RP-DATA or RP-SMMA message reception.
* For more information, please refer to 3GPP TS 24.011 section 7.3.2.3
* @param mr @a RP-Message Reference as per 3GPP TS 24.011 section 8.2.3.
* @param tpdu A Pointer to the @a TP-Message to include as RP-User data.
* @param MobOrig Indicates whether to create a Mobile Originated @a RP-ACK message or not.
* @retval Mobile Originated @a RP-ACK message.
*
* See For more information, see @ref tsms_rpdu_group_ACK "RP-ACK".
*/
tsms_rpdu_ack_t* tsms_rpdu_ack_create(uint8_t mr, const tsms_tpdu_message_t* tpdu, tsk_bool_t MobOrig)
{
    tsms_rpdu_ack_t* ret = tsk_null;

    if(!tpdu) {
        TSK_DEBUG_WARN("TPDU for RP-ACK message is Null.");
    }

    if(!(ret = tsk_object_new(tsms_rpdu_ack_def_t))) {
        goto bail;
    }

    /* 3GPP TS 24.011 ==> 8.2.2 Message type indicator (MTI) */
    TSMS_RPDU_MESSAGE(ret)->mti = MobOrig? tsms_rpdu_type_ack_mo : tsms_rpdu_type_ack_mt;

    /* 3GPP TS 24.011 ==> 8.2.3 Message reference */
    TSMS_RPDU_MESSAGE(ret)->mr = mr;

    /* 3GPP TS 24.011 ==> 8.2.5.3 RP-User data element */
    if(tpdu && (ret->udata = tsk_buffer_create_null())) {
        tsms_tpdu_message_serialize(tpdu, ret->udata, MobOrig);
    }

bail:
    return ret;
}

//
//	RP-ACK object definition
//
static tsk_object_t* tsms_rpdu_ack_ctor(tsk_object_t * self, va_list * app)
{
    tsms_rpdu_ack_t *ack = self;
    if(ack) {
        TSMS_RPDU_MESSAGE(ack)->mti = tsms_rpdu_type_ack_mo;
    }
    else {
        TSK_DEBUG_ERROR("Null");
    }
    return self;
}

static tsk_object_t* tsms_rpdu_ack_dtor(tsk_object_t * self)
{
    tsms_rpdu_ack_t *ack = self;
    if(ack) {
        TSK_OBJECT_SAFE_FREE(ack->udata);
    }
    else {
        TSK_DEBUG_ERROR("Null");
    }

    return self;
}

static const tsk_object_def_t tsms_rpdu_ack_def_s = {
    sizeof(tsms_rpdu_ack_t),
    tsms_rpdu_ack_ctor,
    tsms_rpdu_ack_dtor,
    tsk_null,
};
const tsk_object_def_t *tsms_rpdu_ack_def_t = &tsms_rpdu_ack_def_s;

/*=== RP-ERROR as per 3GPP TS 24.011 section 7.3.4. ===*/

/** internal function used to deserialize a RP-ERROR message from binary content. */
tsms_rpdu_message_t* _tsms_rpdu_rperror_deserialize(const void* data, tsk_size_t size)
{
    /* don't need to test data and size --> already done by tsms_rpdu_message_deserialize() */
    tsms_rpdu_error_t* self = tsms_rpdu_error_create(0, tsk_null, 0, tsk_false);
    const uint8_t* pdata = data;
    const uint8_t* pend = pdata + size;
    tsk_bool_t failed = tsk_false;
    uint8_t cause_count;

    /* 3GPP TS 24.011 ==> 8.2.2 Message type indicator (MTI) */
    TSMS_RPDU_MESSAGE(self)->mti = *pdata++;

    /* 3GPP TS 24.011 ==> 8.2.3 Message reference */
    TSMS_RPDU_MESSAGE(self)->mr = *pdata++;

    /* 3GPP TS 24.011 ==> 8.2.5.4 RP Cause
    * Only 1 cause (LV)*/
    if((cause_count = *pdata++)) {
        self->cause[0] = cause_count;
        self->cause[1] = *pdata++;
        if((pdata += (cause_count-1)) >= pend) {
            TSMS_ERROR_TOO_SHORT("RP-ERROR");
        }
    }

    /* 3GPP TS 24.011 ==> 8.2.5.3 RP-User data element */
    if((pend-pdata) > 2) {
        tsk_size_t length;
        /*uint8_t iei =*/ (void)(*pdata++); // IEI MUST be equal to 0x41
        length = *pdata++; // length-indicator
        if((length != (pend-pdata))) {
            TSK_DEBUG_WARN("Invalid length-indicator.");
        }
        else {
            self->udata = tsk_buffer_create(pdata, length);
        }
    }


bail:
    if(failed) {
        TSK_OBJECT_SAFE_FREE(self);
    }
    return TSMS_RPDU_MESSAGE(self);
}

/** internal function used to serialize a RP-ERROR message as binary content. */
int _tsms_rpdu_rperror_serialize(const tsms_rpdu_message_t* _self, tsk_buffer_t* output)
{
    /* don't need to test output and self --> already done by tsms_rpdu_message_serialize() */
    const tsms_rpdu_error_t* self = (const tsms_rpdu_error_t*)_self;

    /* 3GPP TS 24.011 ==> 8.2.2 Message type indicator (MTI) */
    tsk_buffer_append(output, &_self->mti, 1);

    /* 3GPP TS 24.011 ==> 8.2.3 Message reference */
    tsk_buffer_append(output, &_self->mr, 1);

    /* 3GPP TS 24.011 ==> 8.2.5.4 RP Cause
    * Only 1 cause (LV)*/
    tsk_buffer_append(output, self->cause, 2);

    /* 3GPP TS 24.011 ==> 8.2.5.3 RP-User data element */
    if(self->udata) {
        static uint8_t iei = 0x41;
        tsk_buffer_append(output, &iei, 1); // IEI
        tsk_buffer_append(output, &self->udata->size, 1); // length-indicator
        tsk_buffer_append(output, self->udata->data, self->udata->size);
    }
    else {
        static uint8_t _1byte = 0x00;
        tsk_buffer_append(output, &_1byte, 1); // length-indicator
    }

    return 0;
}

/**@ingroup tsms_rpdu_group
* Creates a @a RP-ERROR message.
* @a RP-ERROR is sent between the MSC and the mobile station in both directions and used to relay an error cause from an erroneous short message or notification transfer attempt.
* For more information, please refer to 3GPP TS 24.011 section 7.3.2.4.
* @param mr @a RP-Message Reference as per 3GPP TS 24.011 section 8.2.3.
* @param tpdu A Pointer to the @a TP-Message to include as RP-User data.
* @param cause RP-Cause value as per 3GPP TS 24.011 section 8.2.5.4.
* @param MobOrig Indicates whether to create a Mobile Originated @a RP-ERROR message or not.
* @retval Mobile Originated @a RP-ERROR message.
*
* See For more information, see @ref tsms_rpdu_group_ERROR "RP-ERROR".
*/
tsms_rpdu_error_t* tsms_rpdu_error_create(uint8_t mr, const tsms_tpdu_message_t* tpdu, uint8_t cause, tsk_bool_t MobOrig)
{
    tsms_rpdu_error_t* ret = tsk_null;

    /*if(!tpdu){
    	TSK_DEBUG_WARN("TPDU for RP-ERROR message is Null.");
    }*/

    if(!(ret = tsk_object_new(tsms_rpdu_error_def_t))) {
        goto bail;
    }

    /* 3GPP TS 24.011 ==> 8.2.2 Message type indicator (MTI) */
    TSMS_RPDU_MESSAGE(ret)->mti = MobOrig? tsms_rpdu_type_error_mo : tsms_rpdu_type_error_mt;

    /* 3GPP TS 24.011 ==> 8.2.3 Message reference */
    TSMS_RPDU_MESSAGE(ret)->mr = mr;

    /* 3GPP TS 24.011 ==> 8.2.5.4 RP Cause */
    ret->cause[0] = 1 /*len*/;
    ret->cause[1] = cause;

    /* 3GPP TS 24.011 ==> 8.2.5.3 RP-User data element */
    if(tpdu && (ret->udata = tsk_buffer_create_null())) {
        tsms_tpdu_message_serialize(tpdu, ret->udata, MobOrig);
    }

bail:
    return ret;
}

//
//	RP-ERROR object definition
//
static tsk_object_t* tsms_rpdu_error_ctor(tsk_object_t * self, va_list * app)
{
    tsms_rpdu_error_t *error = self;
    if(error) {
        TSMS_RPDU_MESSAGE(error)->mti = tsms_rpdu_type_error_mo;
    }
    else {
        TSK_DEBUG_ERROR("Null");
    }
    return self;
}

static tsk_object_t* tsms_rpdu_error_dtor(tsk_object_t * self)
{
    tsms_rpdu_error_t *error = self;
    if(error) {
        TSK_OBJECT_SAFE_FREE(error->udata);
    }
    else {
        TSK_DEBUG_ERROR("Null");
    }

    return self;
}

static const tsk_object_def_t tsms_rpdu_error_def_s = {
    sizeof(tsms_rpdu_error_t),
    tsms_rpdu_error_ctor,
    tsms_rpdu_error_dtor,
    tsk_null,
};
const tsk_object_def_t *tsms_rpdu_error_def_t = &tsms_rpdu_error_def_s;

