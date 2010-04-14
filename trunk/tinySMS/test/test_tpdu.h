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
	
	submit = tsms_tpdu_submit_create(__pdu_last_mr++, "+3310000095", "+3361234567");
	
	/* sending */
	if((buffer = tsms_pack_to_7bit(USER_DATA))){
		ret = tsms_tpdu_submit_set_userdata(submit, buffer, tsms_alpha_7bit);
		if((hex = tsms_tpdu_submit_tohexastring(submit))){
			TSK_DEBUG_INFO("SMS-SUBMIT=%s", hex);
			TSK_FREE(hex);
		}
		TSK_OBJECT_SAFE_FREE(buffer);
	}

	/* receiving */
	buffer = TSK_BUFFER_CREATE_NULL();
	tsms_tpdu_submit_serialize(submit, buffer);

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
	buffer = TSK_BUFFER_CREATE_NULL();
	tsms_tpdu_deliver_serialize(deliver, buffer);

	TSK_OBJECT_SAFE_FREE(deliver);
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
	
	report = tsms_tpdu_report_create("+27381000015", tsk_false, tsk_true);
	
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
	buffer = TSK_BUFFER_CREATE_NULL();
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
	
	command = tsms_tpdu_command_create(__pdu_last_mr++, "+33100000", "+332666", 0xF8, tsms_tpdu_cmd_delete);
		
	/*sending*/
	if((hex = tsms_tpdu_command_tohexastring(command))){
		TSK_DEBUG_INFO("SMS-COMMAND=%s", hex);
		TSK_FREE(hex);
	}

	/* receiving */
	buffer = TSK_BUFFER_CREATE_NULL();
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
	buffer = TSK_BUFFER_CREATE_NULL();
	tsms_tpdu_status_report_serialize(sreport, buffer);

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
