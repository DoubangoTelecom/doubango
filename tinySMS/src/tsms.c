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

/**@file tsms.h
 * @brief SMS over IP Networks.
 *
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 *

 */
#include "tinysms/tsms.h"

/** @mainpage tinySMS API Overview
*
* ======
*
* - @ref tsms_packing_group
* - @ref tsms_tpdu_group
* - @ref tsms_rpdu_group
*
* ======
*
*
* <h1> 18 SMS over IP</h1>
*
* SMS stands for Short Message Service or Silent Messaging Service and is a communication service standardized in the GSM mobile communication system, using standardized communications protocols allowing the interchange of short text messages between mobile telephone devices.
*
* SMS technology has been specified by the ETSI in GSM 03.40 and 03.38 documents (3GPP TS 23.040 and 3GPP TS 23.038 respectively). These documents only describe how to use SMS over mobile networks (nothing for IP networks). The support of SMS over IP networks has been defined in 3GPP TS 24.341, which is fully implemented by doubango through tinySMS.
*
* In this section we will explain how to use SMS technology over IP within the IP Multimedia (IM) Core Network (CN) subsystem based on the Session Initiation Protocol (SIP) and SIP Events as defined in 3GPP TS 24.229.
*
*
* @image html SMS_Architecture.png "Protocol layer overview for the Short Message Service"
*
* <h2>18.1 Modes</h2>
* In real world, there are two ways to receive or send SMS messages over mobile networks: Binary (PDU) and Text mode.
*
* <h3>18.1.1 Text mode</h3>
* The UE shall send the SMS as any other SIP MESSAGE request and it’s up to the IP-SM-GW to encode the payload before retransmission. This mode is out of scope because there is nothing special to do.
*
* <h3>18.1.2 Binary mode</h3>
* The UE shall implement the roles of an SM-over-IP sender and an SM-over-IP receiver, according the procedures in sections 5.3.1 and 5.3.2 in 3GPP TS 24.341.
*
* The IMS core network shall take the role of an IP-SM-GW and support the general procedures in Section 5.3.3.1 of 3GPP TS 24.341, and the functions (answering of routing information query, and transport layer interworking) according to the procedures in Sections 5.3.3.3 and 5.3.3.4 in 3GPP TS 24.341.
*
* It’s up to the UE to encode the payload in binary format before sending the SMS. The payload contains a sequence of hexadecimal octets or decimal semi-octets strings encoded in binary format. The overall PDU (Protocol Data Unit) string contains some useful information (SMSC address, Service center time stamp, sender number, message reference ...) plus the actual message.
* The message length can be up to 160 characters where each character represent 7bits [160/7bits], [140/8bits] or [70/16bits]. By default, each character represents 7bits encoded as per 3GPP TS 23.038.
*
* For IMS and LTE Networks, SMS messages shall be encapsulated in RPDUs (Relay Protocol Data Unit) data string as defined in 3GPP TS 24.011 section 7.3. The RPDU data is transferred from SM entity to SM entity using SIP MESSAGE requests. These SIP requests shall use the MIME type <b>"application/vnd.3gpp.sms"</b> for this purpose.
*
*/






/**@defgroup tsms_tpdu_group Service provided by the SM-TL (Transport)
* The Short Message Transfer Layer (SM TL) provides a service to the Short Message Application Layer (SM AL).
*
* This service enables the SM AL to transfer short messages to its peer entity, receive short messages from its
* peer entity and receive reports about earlier requests for short messages to be transferred.
*
* ======
*
* - @ref tsms_tpdu_group_SUBMIT "18.2.1 SMS-SUBMIT"
* - @ref tsms_tpdu_group_DELIVER "18.2.2 SMS-DELIVER"
* - @ref tsms_tpdu_group_STATUS_REPORT "18.2.3 SMS-STATUS-REPORT"
* - @ref tsms_tpdu_group_COMMAND "18.2.4 SMS-COMMAND"
*
* ======
*
* <h2> 18.2.1 SMS-SUBMIT @anchor tsms_tpdu_group_SUBMIT </h2>
* <i>SMS-SUBMIT</i> messages are used to convey short messages from the MS (Mobile Station) to the SC (Service Center).<br>
* <i>SMS-SUBMIT-REPORT</i> messages are used for positive or negative acknowledgement to an <i>SMS-DELIVER</i> or <i>SMS-STATUS-REPORT</i>.
*
* For more information, please refer to 3GPP TS 23.040 section 9.2.2.2.
*
* The code below shows how to send a <i>SMS-SUBMIT</i> message from the MS to the SC (MO).
*
* @code
#include "tsk.h"
#include "tinysms.h"

int ret;
tsms_tpdu_submit_t* submit = tsk_null;
tsk_buffer_t* buffer = tsk_null;
char* hex;
const char* smsc = "+331000009";
const char* destination = "+333361234567";
const char* short_message = "hello world";
uint8_t mr = 0xF5;

submit = tsms_tpdu_submit_create(mr, smsc, destination);

// encode the user data to GSM 7-bit alphabet
if((buffer = tsms_pack_to_7bit(short_message))){
	ret = tsms_tpdu_submit_set_userdata(submit, buffer, tsms_alpha_7bit);
	if((hex = tsms_tpdu_submit_tohexastring(submit))){
		TSK_DEBUG_INFO("SMS-SUBMIT=%s", hex);
		TSK_FREE(hex);
	}
	TSK_OBJECT_SAFE_FREE(buffer);
}
TSK_OBJECT_SAFE_FREE(submit);
* @endcode
*
* The message should be sent over the network as binary content. For <i>SMS-SUBMIT</i> messages, binary serialization is performed by @ref tsms_tpdu_submit_serialize().<br>
* The code below shows how to serialize a <i>SMS-SUBMIT</i> message as binary content.
*
*
*
* <h2>18.2.2 SMS-DELIVER @anchor tsms_tpdu_group_DELIVER</h2>
* <i>SMS-DELIVER</i> messages are used to convey short messages from the SC (Service Center) to the MS (Mobile Station). <i>SMS-DELIVER-REPORT</i> messages are used for positive or negative acknowledgement to an <i>SMS-DELIVER</i> or <i>SMS-STATUS-REPORT</i>.<br>
* For more information, please refer to 3GPP TS 23.040 section 9.2.2.1.<br>
* The code below shows how to receive a <i>SMS-DELIVER</i> message, sent from the SC to the MS (MT).<br>
*
* @code
#include "tsk.h"
#include "tinysms.h"

tsms_tpdu_message_t* sms_any = tsms_tpdu_message_deserialize_mt(data, size);
if(sms_any && sms_any->mti == tsms_tpdu_mti_deliver_mt){
	//tsms_tpdu_deliver_t* sms_deliver = TSMS_TPDU_DELIVER(sms_any); ==> Yes we can !
	char* ascii;
	if((ascii = tsms_tpdu_message_get_payload(sms_any))){
		TSK_DEBUG_INFO("Message=%s", ascii);
		TSK_FREE(ascii);
	}
}
TSK_OBJECT_SAFE_FREE(sms_any);
* @endcode
*
* When the MS receive a SMS-DELIVER message, it should send back a <i>SMS-DELIVER-REPORT</i> message. The code bellow shows how to send this kind of message.
*
* @code
#include "tsk.h"
#include "tinysms.h"

int ret;
tsms_tpdu_report_t* report = tsk_null;
tsk_buffer_t* buffer = tsk_null;
const char* smsc = "+331000009";
tsk_bool_t isSUBMIT = tsk_false;
tsk_bool_t isERROR = tsk_false;
report = tsms_tpdu_report_create(smsc, isSUBMIT, isERROR);
buffer = tsk_buffer_create_null();
if(!(ret = tsms_tpdu_report_serialize(report, buffer))){
//send(buffer->data, buffer->size);
}
TSK_OBJECT_SAFE_FREE(report);
TSK_OBJECT_SAFE_FREE(buffer);
* @endcode
*
*
*
* <h2>18.2.3 SMS-STATUS-REPORT @anchor tsms_tpdu_group_STATUS_REPORT</h2>
* <i>SMS-STATUS-REPORT</i> messages are used to convey status reports from the SC (Service Center) to the MS (Mobile Station).<br>
* For more information, please refer to 3GPP TS 23.040 section 9.2.2.3.<br>
* The code below shows how to receive a <i>SMS-STATUS-REPORT</i> message, sent from the SC to the MS (MT).<br>
*
* @code
#include "tsk.h"
#include "tinysms.h"

tsms_tpdu_message_t* sms_any = tsms_tpdu_message_deserialize_mt(buffer->data, buffer->size);
if(sms_any && sms_any->mti == tsms_tpdu_mti_status_report_mt){
	tsms_tpdu_status_report_t* sms_status_report = TSMS_TPDU_STATUS_REPORT(sms_any);
	switch(sms_status_report->st){
		case tsms_tpdu_status_received:
		case tsms_tpdu_status_forwarded:
		case tsms_tpdu_status_replaced:
		// ...
		default:
			break;
	}
}
* @endcode
*
*
*
* <h2>18.2.4 SMS-COMMAND @anchor tsms_tpdu_group_COMMAND</h2>
* <i>SMS-COMMAND</i> messages are used to convey commands from the MS (Mobile Station) to the SC (Service Center).<br>
* For more information, please refer to 3GPP TS 23.040 section 9.2.2.4.<br>
* The code below shows how to send a <i>SMS-COMMAND</i> (DELETE) from the MS to the SC.<br>
*
* @code
#include "tsk.h"
#include "tinysms.h"

tsms_tpdu_command_t* command = tsk_null;
char* hex;
tsk_buffer_t* buffer = tsk_null;
const char* smsc = "+331000009";
const char* destination = "+333361234567";
uint8_t mr = 0xF5;
uint8_t message_number = 0xF8;

command = tsms_tpdu_command_create(mr, smsc, destination, message_number, tsms_tpdu_cmd_delete);

if((hex = tsms_tpdu_command_tohexastring(command))){
	TSK_DEBUG_INFO("SMS-COMMAND=%s", hex);
	TSK_FREE(hex);
}
TSK_OBJECT_SAFE_FREE(command);
* @endcode
*
* The message should be sent over the network as binary content. For SMS-COMMAND messages, binary serialization is performed by @ref tsms_tpdu_command_serialize().
* The code below shows how to serialize a SMS-COMMAND message as binary content.
* @code
#include "tsk.h"
#include "tinysms.h"

if ((buffer = tsk_buffer_create_null())){
ret = tsms_tpdu_command_serialize(command, buffer);
// ret = send(socket, buffer->data, buffer->size);
TSK_OBJECT_SAFE_FREE(buffer);
}
* @endcode
* A SC receiving binary <i>SMS-COMMAND</i> message (or any other SMS-*) over the network should use @ref tsms_tpdu_message_deserialize_mo() function to deserialize the content.
*/












/**@defgroup tsms_rpdu_group Service provided by the SM-RL (Relay)
*
* The Short Message Relay Layer (SM-RL) provides a service to the Short Message Transfer Layer (SM-TL). This service enables the SM TL to send Transfer Protocol Data Units (TPDUs) to its peer entity, receive TPDUs from its peer entity and receive reports about earlier requests for TPDUs to be transferred.<br>
*
* For more information about how doubango provides SM-TL services, please refer to @ref tsms_tpdu_group "SM-TL".
*
* ======
*
* - @ref tsms_rpdu_group_DATA "18.3.1 RP-DATA"
* - @ref tsms_rpdu_group_SMMA "18.3.2 RP-SMMA"
* - @ref tsms_rpdu_group_ACK "18.3.3 RP-ACK"
* - @ref tsms_rpdu_group_ERROR "18.3.4 RP-ERROR"
*
* ======
*
* <h2> 18.3.1 RP-DATA @anchor tsms_rpdu_group_DATA</h2>
* <i>RP-DATA</i> messages are use to relay the TPDUs. These messages could be sent from the Network to Mobile Station (MT) or from the Mobile Station to the Network (MO).<br>
* For more information, please refer to 3GPP TS 24.011 section 7.3.1.<br>
* The code below shows how to send a RP-DATA message with a RP-User-Data (see 3GPP TS 23.011 section 8.2.5.3) information element which includes <i>SMS-SUBMIT</i> as type indicator (this use case comes from 3GPP TS 24.341 chapter B.5). <br>
*
* @code
#include "tsk.h"
#include "tinysms.h"

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
if((buffer = tsms_pack_to_7bit(short_message))){
	ret = tsms_tpdu_submit_set_userdata(sms_submit, buffer, tsms_alpha_7bit);
	TSK_OBJECT_SAFE_FREE(buffer);
}
// create RP-DATA(SMS-SUBMIT) message and print its content (for test only)
rp_data = tsms_rpdu_data_create_mo(mr, smsc, TSMS_TPDU_MESSAGE(sms_submit));
if((hex = tsms_rpdu_message_tohexastring(TSMS_RPDU_MESSAGE(rp_data)))){
	TSK_DEBUG_INFO("RP-DATA=%s", hex);
	TSK_FREE(hex);
}

TSK_OBJECT_SAFE_FREE(buffer);
TSK_OBJECT_SAFE_FREE(sms_submit);
TSK_OBJECT_SAFE_FREE(rp_data);
* @endcode
*
* The message should be sent over the network as binary content. For <i>RP-DATA</i> messages, binary serialization is performed by @ref tsms_rpdu_data_serialize().<br>
* The code below shows how to serialize a <i>RP-DATA</i> message as binary content.<br>
* @code
#include "tsk.h"
#include "tinysms.h"

if ((buffer = tsk_buffer_create_null())){
ret = tsms_rpdu_data_serialize(rp_data, buffer);
// ret = send(socket, buffer->data, buffer->size);
TSK_OBJECT_SAFE_FREE(buffer);
}
* @endcode
*
* A SC receiving binary <i>RP-DATA</i> message (or any other <i>RP-*</i>) from the network should use tsms_rpdu_message_deserialize() function to deserialize the content.<br>
* The code below shows how to receive a <i>RP-DATA</i> message with a <i>RP-User-Data</i> (see 3GPP TS 23.011 section 8.2.5.3) information element which includes <i>SMS-DELIVER</i> as type indicator (this use case comes from 3GPP TS 24.341 chapter B.6). <br>
*
* @code
#include "tsk.h"
#include "tinysms.h"

* tsms_rpdu_message_t* rp_message = tsk_null;
* tsms_tpdu_message_t* tpdu = tsk_null;

if(!(rp_message = tsms_rpdu_message_deserialize(data, size))){ // Deserialize RP-* Message
	TSK_DEBUG_ERROR("Failed to deserialize the RP-MESSAGE");
	goto bail;
}

if(rp_message->mti == tsms_rpdu_type_data_mt){ // RP-DATA from SC to MS
	char* ascii = tsk_null;
	tsms_rpdu_data_t* rp_data = TSMS_RPDU_DATA(rp_message);
	if((tpdu = tsms_tpdu_message_deserialize_mt(rp_data->udata->data, rp_data->udata->size))){ //Deserialize SMS-* Message (From SC to MS)
		if(tpdu->mti == tsms_tpdu_mti_deliver_mt){ // SMS-DELIVER from SC to MS
			if((ascii = tsms_tpdu_message_get_payload(tpdu))){
				TSK_DEBUG_INFO("ASCII message=%s", ascii);
				TSK_FREE(ascii);
			}
		}
	}
}

bail:
TSK_OBJECT_SAFE_FREE(rp_message);
TSK_OBJECT_SAFE_FREE(tpdu);
* @endcode
*
*
*
* <h2> 18.3.2 RP-SMMA @anchor tsms_rpdu_group_SMMA</h2>
* This message is sent by the mobile station to relay a notification to the network that the mobile has memory available to receive one or more short messages.<br>
* For more information, please refer to 3GPP TS 24.011 section 7.3.2.<br>
* The code below shows how to send a <i>RP-SMMA</i> message.<br>
*
* @code
#include "tsk.h"
#include "tinysms.h"

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

TSK_OBJECT_SAFE_FREE(buffer);
TSK_OBJECT_SAFE_FREE(rp_smma);
* @endcode
*
*
*
* <h2> 18.3.3 RP-ACK @anchor tsms_rpdu_group_ACK</h2>
* This message is sent between the MSC and the mobile station in both directions and used to relay the acknowledgement of a <i>RP-DATA</i> or <i>RP-SMMA</i> message reception.<br>
* For more information, please refer to 3GPP TS 24.011 section 7.3.2.3.<br>
* The code below shows how to send a <i>RP-ACK</i> message with a <i>RP-User-Data</i> (see 3GPP TS 23.011 section 8.2.5.3) information element which includes <i>SMS-DELIVER-REPORT</i> as type indicator (this use case comes from 3GPP TS 24.341 chapter B.6 section 8). <br>
*
* @code
#include "tsk.h"
#include "tinysms.h"

int ret;
tsk_buffer_t* buffer = tsk_null;
tsms_tpdu_report_t* sms_report = tsk_null;
tsms_rpdu_ack_t* rp_ack= tsk_null;
const char* smsc = "+331000000";
tsk_bool_t isSUBMIT = tsk_false; // is SMS-DELIVER-REPORT
tsk_bool_t isERROR = tsk_false;
uint8_t mr = 0xF5;

// create SMS-DELIVER-REPORT message
sms_report = tsms_tpdu_report_create(smsc, isSUBMIT, isERROR);
// create RP-ACK message (From MS to SC)
rp_ack = tsms_rpdu_ack_create_mo(mr, TSMS_TPDU_MESSAGE(sms_report));
// serialize
buffer = tsk_buffer_create_null();
if(!(ret = tsms_rpdu_data_serialize(rp_ack, buffer))){
// send(socket, buffer->data, buffer->size);
}
TSK_OBJECT_SAFE_FREE(buffer);
TSK_OBJECT_SAFE_FREE(sms_report);
TSK_OBJECT_SAFE_FREE(rp_ack);

* @endcode
*
*
*
*
* <h2> 18.3.4 RP-ERROR @anchor tsms_rpdu_group_ERROR</h2>
* This message is sent between the MSC and the mobile station in both directions and used to relay an error cause from an erroneous short message or notification transfer attempt.<br>
* For more information, please refer to 3GPP TS 24.011 section7.3.2.4.<br>
* The code below shows how to send a <i>RP-ERROR</i> message with a <i>RP-User-Data</i> (see 3GPP TS 23.011 section 8.2.5.3) information element which includes <i>SMS-DELIVER-REPORT</i> as type indicator. In this example, the error message is sent because the “call is barred”. For more information about the cause values that may be contained in an <i>RP-ERROR</i> message, please refer to 3GPP TS 24.011 section 8.2.5.4.<br>
*
* @code
#include "tsk.h"
#include "tinysms.h"

int ret;
tsk_buffer_t* buffer = tsk_null;
tsms_tpdu_report_t* sms_report = tsk_null;
tsms_rpdu_error_t* rp_error= tsk_null;
tsk_bool_t isSUBMIT = tsk_false; // is SMS-DELIVER-REPORT
tsk_bool_t isERROR = tsk_true;
const char* smsc = "+331000000";
uint8_t mr = 0xF5;

// create SMS-DELIVER-REPORT message
sms_report = tsms_tpdu_report_create(smsc, isSUBMIT, isERROR);
// create RP-ERROR message
rp_error = tsms_rpdu_error_create_mo(mr, TSMS_TPDU_MESSAGE(sms_report),
0x0A//call barred
);
// serialize
buffer = tsk_buffer_create_null();
if(!(ret = tsms_rpdu_data_serialize(rp_error, buffer))){
	// send(socket, buffer->data, buffer->size);
}

TSK_OBJECT_SAFE_FREE(buffer);
TSK_OBJECT_SAFE_FREE(sms_report);
TSK_OBJECT_SAFE_FREE(rp_error);

* @endcode
*/
