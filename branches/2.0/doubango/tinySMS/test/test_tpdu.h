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
#ifndef _TEST_SMSTPDU_H
#define _TEST_SMSTPDU_H

#define USER_DATA "hellohello"

static uint8_t __pdu_last_mr = 0;

void test_submit()
{
	int ret;
	tsms_tpdu_submit_t* submit = tsk_null;
	tsk_buffer_t* buffer = tsk_null;
	char* hex;
	const char* smsc = "+331000009";
	const char* destination = "+333361234567";
	const char* short_message = "hello world";
	uint8_t mr = 0xF5;
	
	submit = tsms_tpdu_submit_create(mr, smsc, destination);
	
	/* encode the user data to GSM 7-bit alphabet */
	if((buffer = tsms_pack_to_7bit(short_message))){
		ret = tsms_tpdu_submit_set_userdata(submit, buffer, tsms_alpha_7bit);
		if((hex = tsms_tpdu_submit_tohexastring(submit))){
			TSK_DEBUG_INFO("SMS-SUBMIT=%s", hex);
			TSK_FREE(hex);
		}
		TSK_OBJECT_SAFE_FREE(buffer);
	}

	/* receiving */
	buffer = tsk_buffer_create_null();
	ret = tsms_tpdu_submit_serialize(submit, buffer);
	// send(socket, buffer->data, buffer->size);

	TSK_OBJECT_SAFE_FREE(submit);
	submit = (tsms_tpdu_submit_t*)tsms_tpdu_message_deserialize_mo(buffer->data, buffer->size);
	if((hex = tsms_tpdu_submit_tohexastring(submit))){
		TSK_DEBUG_INFO("SMS-SUBMIT=%s", hex);
		TSK_FREE(hex);
	}

	TSK_OBJECT_SAFE_FREE(submit);
	TSK_OBJECT_SAFE_FREE(buffer);

	printf("==========\n");
}

void test_deliver()
{
	int ret;
	tsms_tpdu_deliver_t* deliver = tsk_null;
	tsk_buffer_t* buffer = tsk_null;
	char* hex;
	
	deliver = tsms_tpdu_deliver_create("+27381000015", "+46708251358");
	
	/* sending */
	if((buffer = tsms_pack_to_7bit(USER_DATA))){
		ret = tsms_tpdu_deliver_set_userdata(deliver, buffer, tsms_alpha_7bit);
		if((hex = tsms_tpdu_deliver_tohexastring(deliver))){
			TSK_DEBUG_INFO("SMS-DELIVER=%s", hex);
			TSK_FREE(hex);
		}
		TSK_OBJECT_SAFE_FREE(buffer);
	}

	/* receiving */
	buffer = tsk_buffer_create_null();
	tsms_tpdu_deliver_serialize(deliver, buffer);

	TSK_OBJECT_SAFE_FREE(deliver);
	
	//{
	//	tsms_tpdu_message_t* sms_any = tsms_tpdu_message_deserialize_mt(buffer->data, buffer->size);
	//	if(sms_any && sms_any->mti == tsms_tpdu_mti_deliver_mt){
	//		//tsms_tpdu_deliver_t* sms_deliver = TSMS_TPDU_DELIVER(sms_any); ==> Yes we can !
	//		char* ascii;
	//		if((ascii = tsms_tpdu_message_get_payload(sms_any))){
	//			TSK_DEBUG_INFO("Message=%s", ascii);
	//			TSK_FREE(ascii);
	//		}
	//	}
	//	TSK_OBJECT_SAFE_FREE(sms_any);
	//}

	deliver = (tsms_tpdu_deliver_t*)tsms_tpdu_message_deserialize_mt(buffer->data, buffer->size);
	if((hex = tsms_tpdu_deliver_tohexastring(deliver))){
		TSK_DEBUG_INFO("SMS-DELIVER=%s", hex);
		TSK_FREE(hex);
	}

	TSK_OBJECT_SAFE_FREE(deliver);
	TSK_OBJECT_SAFE_FREE(buffer);

	printf("==========\n");
}


void test_report()
{
	int ret;
	tsms_tpdu_report_t* report = tsk_null;
	tsk_buffer_t* buffer = tsk_null;
	char* hex;
	const char* smsc = "+331000009";
	tsk_bool_t isSUBMIT = tsk_false;
	tsk_bool_t isERROR = tsk_false;
	
	report = tsms_tpdu_report_create(smsc, isSUBMIT, isERROR);
	
	/* sending */
	if((buffer = tsms_pack_to_7bit(USER_DATA))){
		ret = tsms_tpdu_deliver_set_userdata(report, buffer, tsms_alpha_7bit);
		if((hex = tsms_tpdu_deliver_tohexastring(report))){
			TSK_DEBUG_INFO("SMS-DELIVER-REPORT for RP-ERROR=%s", hex);
			TSK_FREE(hex);
		}
		TSK_OBJECT_SAFE_FREE(buffer);
	}

	/* receiving */
	buffer = tsk_buffer_create_null();
	tsms_tpdu_report_serialize(report, buffer);

	TSK_OBJECT_SAFE_FREE(report);
	report = (tsms_tpdu_report_t*)tsms_tpdu_message_deserialize_mo(buffer->data, buffer->size);
	if((hex = tsms_tpdu_report_tohexastring(report))){
		TSK_DEBUG_INFO("SMS-DELIVER-REPORT for RP-ERROR=%s", hex);
		TSK_FREE(hex);
	}

	TSK_OBJECT_SAFE_FREE(report);
	TSK_OBJECT_SAFE_FREE(buffer);

	printf("==========\n");
}

void test_command()
{
	tsms_tpdu_command_t* command = tsk_null;
	char* hex;
	tsk_buffer_t* buffer = tsk_null;
	const char* smsc = "+331000009";
	const char* destination = "+333361234567";
	uint8_t mr = 0xF5;
	uint8_t message_number = 0xF8;

	command = tsms_tpdu_command_create(mr, smsc, destination, message_number, tsms_tpdu_cmd_delete);
		
	/*sending*/
	if((hex = tsms_tpdu_command_tohexastring(command))){
		TSK_DEBUG_INFO("SMS-COMMAND=%s", hex);
		TSK_FREE(hex);
	}

	/* receiving */
	buffer = tsk_buffer_create_null();
	tsms_tpdu_command_serialize(command, buffer);

	TSK_OBJECT_SAFE_FREE(command);
	command = (tsms_tpdu_command_t*)tsms_tpdu_message_deserialize_mo(buffer->data, buffer->size);
	if((hex = tsms_tpdu_command_tohexastring(command))){
		TSK_DEBUG_INFO("SMS-COMMAND=%s", hex);
		TSK_FREE(hex);
	}

	TSK_OBJECT_SAFE_FREE(command);
	TSK_OBJECT_SAFE_FREE(buffer);

	printf("==========\n");
}

void test_sreport()
{
	tsms_tpdu_status_report_t* sreport = tsk_null;
	char* hex;
	tsk_buffer_t* buffer = tsk_null;
	
	sreport = tsms_tpdu_status_report_create(__pdu_last_mr++, "+3310000", "+332666", tsms_tpdu_status_busy, tsk_true);
		
	if((hex = tsms_tpdu_report_tohexastring(sreport))){
		TSK_DEBUG_INFO("SMS-STATUS-REPORT=%s", hex);
		TSK_FREE(hex);
	}

	/* receiving */
	buffer = tsk_buffer_create_null();
	tsms_tpdu_status_report_serialize(sreport, buffer);

	
//tsms_tpdu_message_t* sms_any = tsms_tpdu_message_deserialize_mt(buffer->data, buffer->size);
//if(sms_any && sms_any->mti == tsms_tpdu_mti_status_report_mt){
//	tsms_tpdu_status_report_t* sms_status_report = TSMS_TPDU_STATUS_REPORT(sms_any);
//	switch(sms_status_report->st){
//		case tsms_tpdu_status_received:
//		case tsms_tpdu_status_forwarded:
//		case tsms_tpdu_status_replaced:
//		// ...
//		default:
//			break;
//	}
//}
//TSK_OBJECT_SAFE_FREE(sms_any);
	

	TSK_OBJECT_SAFE_FREE(sreport);
	sreport = (tsms_tpdu_status_report_t*)tsms_tpdu_message_deserialize_mt(buffer->data, buffer->size);
	if((hex = tsms_tpdu_status_report_tohexastring(sreport))){
		TSK_DEBUG_INFO("SMS-STATUS-REPORT=%s", hex);
		TSK_FREE(hex);
	}

	TSK_OBJECT_SAFE_FREE(sreport);
	TSK_OBJECT_SAFE_FREE(buffer);

	printf("==========\n");
}

void test_tpdu()
{
	test_submit();
	test_deliver();
	test_report();
	test_command();
	test_sreport();
}

#endif /* _TEST_SMSTPDU_H */
