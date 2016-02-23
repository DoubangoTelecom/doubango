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
#ifndef _TEST_SMSRPDU_H
#define _TEST_SMSRPDU_H

void test_i_rpdata(const void* data, tsk_size_t size, tsk_bool_t MobOrig)
{
    tsms_rpdu_message_t* rp_message = tsk_null;
    tsms_tpdu_message_t* tpdu = tsk_null;

    if(!(rp_message = tsms_rpdu_message_deserialize(data, size))) {
        TSK_DEBUG_ERROR("Failed to deserialize the RP-MESSAGE");
        goto bail;
    }

    switch(rp_message->mti) {
    case tsms_rpdu_type_data_mo:
    case tsms_rpdu_type_data_mt: {
        char* ascii = tsk_null;
        tsms_rpdu_data_t* rp_data = TSMS_RPDU_DATA(rp_message);
        if((tpdu = tsms_tpdu_message_deserialize(rp_data->udata->data, rp_data->udata->size, MobOrig))) {
            if(tpdu->mti == tsms_tpdu_mti_deliver_mt || tpdu->mti == tsms_tpdu_mti_submit_mo) { /* SMS-SUBMIT or SMS-DELIVER? */
                if((ascii = tsms_tpdu_message_get_payload(tpdu))) {
                    TSK_DEBUG_INFO("ASCII message=%s", ascii);
                    TSK_FREE(ascii);
                }
            }
        }
        break;
    }
    case tsms_rpdu_type_ack_mo:
    case tsms_rpdu_type_ack_mt: {
        tsms_rpdu_ack_t* rp_ack = TSMS_RPDU_ACK(rp_message);
        // ...do whatever you want
        if(rp_ack->udata && (tpdu = tsms_tpdu_message_deserialize(rp_ack->udata->data, rp_ack->udata->size, MobOrig))) {
            // ...do whatever you want
        }
        TSK_DEBUG_INFO("RP-ACK");
        break;
    }
    case tsms_rpdu_type_error_mo:
    case tsms_rpdu_type_error_mt: {
        tsms_rpdu_error_t* rp_error = TSMS_RPDU_ERROR(rp_message);
        // ...do whatever you want
        if(rp_error->udata && (tpdu = tsms_tpdu_message_deserialize(rp_error->udata->data, rp_error->udata->size, MobOrig))) {
            // ...do whatever you want
        }
        TSK_DEBUG_INFO("RP-ERROR");
        break;
    }
    case tsms_rpdu_type_smma_mo: {
        tsms_rpdu_smma_t* rp_smma = TSMS_RPDU_SMMA(rp_message);
        // ...do whatever you want
        TSK_DEBUG_INFO("RP-SMMA");
        break;
    }
    default: {
        TSK_DEBUG_INFO("Unknown RP-Message type (%u).", rp_message->mti);
        break;
    }
    }

bail:
    TSK_OBJECT_SAFE_FREE(rp_message);
    TSK_OBJECT_SAFE_FREE(tpdu);
}

void test_o_rpdata_submit()
{
    //== Sending RP-DATA(SMS-SUBMIT) ==
    int ret;
    tsk_buffer_t* buffer = tsk_null;
    tsms_tpdu_submit_t* sms_submit = tsk_null;
    tsms_rpdu_data_t* rp_data = tsk_null;
    const char* smsc = "+331000009";
    const char* destination = "+333361234567";
    const char* short_message = "hello world";
    uint8_t mr = 0xF5;
    uint8_t message_number = 0xF8;
    char* hex;

    // create SMS-SUBMIT message
    sms_submit = tsms_tpdu_submit_create(mr, smsc, destination);
    // Set content for SMS-SUBMIT
    if((buffer = tsms_pack_to_7bit(short_message))) {
        ret = tsms_tpdu_submit_set_userdata(sms_submit, buffer, tsms_alpha_7bit);
        TSK_OBJECT_SAFE_FREE(buffer);
    }
    // create RP-DATA message and print its content (for test only)
    rp_data = tsms_rpdu_data_create_mo(mr, smsc, TSMS_TPDU_MESSAGE(sms_submit));
    if((hex = tsms_rpdu_message_tohexastring(TSMS_RPDU_MESSAGE(rp_data)))) {
        TSK_DEBUG_INFO("RP-DATA=%s", hex);
        TSK_FREE(hex);
    }


    // serialize
    buffer = tsk_buffer_create_null();
    ret = tsms_rpdu_data_serialize(rp_data, buffer);
    // send(socket, buffer->data, buffer->size);
    // print result (hex) to the console
    printhex("==RP-DATA(SMS-SUBMIT):", buffer->data, buffer->size);

    // receiving
    test_i_rpdata(buffer->data, buffer->size, tsk_true);

    TSK_OBJECT_SAFE_FREE(buffer);
    TSK_OBJECT_SAFE_FREE(sms_submit);
    TSK_OBJECT_SAFE_FREE(rp_data);
}

void test_o_rpdata_deliver()
{
    //== Sending RP-DATA(SMS-DELIVER) ==
    int ret;
    tsk_buffer_t* buffer = tsk_null;
    tsms_tpdu_deliver_t* sms_deliver = tsk_null;
    tsms_rpdu_data_t* rp_data = tsk_null;
    const char* smsc = "+331000000";
    const char* originator = "+3361234567";
    const char* content = "hello world!";

    // create SMS-DELIVER message
    sms_deliver = tsms_tpdu_deliver_create(smsc, originator);
    // Set content for SMS-DELIVER
    if((buffer = tsms_pack_to_7bit(content))) {
        ret = tsms_tpdu_deliver_set_userdata(sms_deliver, buffer, tsms_alpha_7bit);
        TSK_OBJECT_SAFE_FREE(buffer);
    }
    // create RP-DATA message
    rp_data = tsms_rpdu_data_create_mt(0x01, smsc, TSMS_TPDU_MESSAGE(sms_deliver));
    // serialize
    buffer = tsk_buffer_create_null();
    ret = tsms_rpdu_data_serialize(rp_data, buffer);
    // send(socket, buffer->data, buffer->size);
    // print result (hex) to the console
    printhex("==RP-DATA(SMS-DELIVER):", buffer->data, buffer->size);

    // receiving
    test_i_rpdata(buffer->data, buffer->size, tsk_false);

    TSK_OBJECT_SAFE_FREE(buffer);
    TSK_OBJECT_SAFE_FREE(sms_deliver);
    TSK_OBJECT_SAFE_FREE(rp_data);
}

void test_o_rpdata_smma()
{
    //== Sending RP-SMMA ==
    int ret;
    tsk_buffer_t* buffer = tsk_null;
    tsms_rpdu_smma_t* rp_smma = tsk_null;
    uint8_t mr = 0xF5;

    // create RP-SMMA message
    rp_smma = tsms_rpdu_smma_create(mr);
    // serialize
    buffer = tsk_buffer_create_null();
    ret = tsms_rpdu_data_serialize(rp_smma, buffer);
    // send(socket, buffer->data, buffer->size);
    // print result (hex) to the console
    printhex("==RP-SMMA:", buffer->data, buffer->size);

    // receiving
    test_i_rpdata(buffer->data, buffer->size, tsk_true);

    TSK_OBJECT_SAFE_FREE(buffer);
    TSK_OBJECT_SAFE_FREE(rp_smma);
}

void test_o_rpdata_ack()
{
    //== Sending RP-ACK(SMS-DELIVER-REPORT) ==
    int ret;
    tsk_buffer_t* buffer = tsk_null;
    tsms_tpdu_report_t* sms_report = tsk_null;
    tsms_rpdu_ack_t* rp_ack= tsk_null;
    const char* smsc = "+331000000";
    tsk_bool_t isSUBMIT = tsk_false; /* isDELIVER */
    tsk_bool_t isERROR = tsk_false;
    uint8_t mr = 0xF5;

    // create SMS-DELIVER-REPORT message
    sms_report = tsms_tpdu_report_create(smsc, isSUBMIT, isERROR);
    // create RP-ACK message (From MS to SC)
    rp_ack = tsms_rpdu_ack_create_mo(mr, TSMS_TPDU_MESSAGE(sms_report));
    // serialize
    buffer = tsk_buffer_create_null();
    if(!(ret = tsms_rpdu_data_serialize(rp_ack, buffer))) {
        // send(socket, buffer->data, buffer->size);
        // print result (hex) to the console
        printhex("==RP-ACK(SMS-DELIVER-REPORT):", buffer->data, buffer->size);
    }

    // receiving
    test_i_rpdata(buffer->data, buffer->size, tsk_true);

    TSK_OBJECT_SAFE_FREE(buffer);
    TSK_OBJECT_SAFE_FREE(sms_report);
    TSK_OBJECT_SAFE_FREE(rp_ack);
}

void test_o_rpdata_error()
{
    //== Sending RP-ERROR(SMS-DELIVER-REPORT) ==
    int ret;
    tsk_buffer_t* buffer = tsk_null;
    tsms_tpdu_report_t* sms_report = tsk_null;
    tsms_rpdu_error_t* rp_error= tsk_null;
    tsk_bool_t isSUBMIT = tsk_false; /* isDELIVER */
    tsk_bool_t isERROR = tsk_true;
    const char* smsc = "+331000000";
    uint8_t mr = 0xF5;

    // create SMS-DELIVER-REPORT message
    sms_report = tsms_tpdu_report_create(smsc, isSUBMIT, isERROR);
    // create RP-ERROR message
    rp_error = tsms_rpdu_error_create_mo(mr, TSMS_TPDU_MESSAGE(sms_report), 0x0A/*call barred*/);
    // serialize
    buffer = tsk_buffer_create_null();
    if(!(ret = tsms_rpdu_data_serialize(rp_error, buffer))) {
        // send(socket, buffer->data, buffer->size);
        // print result (hex) to the console
        printhex("==RP-ERROR(SMS-DELIVER-REPORT):", buffer->data, buffer->size);
    }

    // receiving
    test_i_rpdata(buffer->data, buffer->size, tsk_true);

    TSK_OBJECT_SAFE_FREE(buffer);
    TSK_OBJECT_SAFE_FREE(sms_report);
    TSK_OBJECT_SAFE_FREE(rp_error);
}


void test_rpdu()
{
    test_o_rpdata_submit();
    //test_o_rpdata_deliver();
    //test_o_rpdata_smma();
    //test_o_rpdata_ack();
    //test_o_rpdata_error();

    //const char* data = "\x03\x01\x41\x09\x01\x00\x01\x80\x01\x32\x42\x00\x69";
    //test_i_rpdata(data, 13, tsk_false);
}

#endif /* _TEST_SMSRPDU_H */
