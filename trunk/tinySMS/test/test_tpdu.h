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
	tsms_tpdu_submit_handle_t* submit = tsk_null;
	tsk_buffer_t* udata = tsk_null;
	char* hex;
	
	submit = tsms_tpdu_submit_create(__pdu_last_mr++, "+331000000", "+3361234567");
	
	/* set user data */
	if((udata = tsms_pack_to_7bit(USER_DATA))){
		ret = tsms_tpdu_submit_set_userdata(submit, udata, tsms_alpha_7bit);
		if((hex = tsms_tpdu_submit_tohexastring(submit))){
			TSK_DEBUG_INFO("SMS-SUBMIT=%s", hex);
			TSK_FREE(hex);
		}
		TSK_OBJECT_SAFE_FREE(udata);
	}

	TSK_OBJECT_SAFE_FREE(submit);
}

void test_deliver()
{
	int ret;
	tsms_tpdu_deliver_handle_t* deliver = tsk_null;
	tsk_buffer_t* udata = tsk_null;
	char* hex;
	
	deliver = tsms_tpdu_deliver_create("+27381000015", "+46708251358");
	
	/* set user data */
	if((udata = tsms_pack_to_7bit(USER_DATA))){
		ret = tsms_tpdu_deliver_set_userdata(deliver, udata, tsms_alpha_7bit);
		if((hex = tsms_tpdu_deliver_tohexastring(deliver))){
			TSK_DEBUG_INFO("SMS-DELIVER=%s", hex);
			TSK_FREE(hex);
		}
		TSK_OBJECT_SAFE_FREE(udata);
	}

	TSK_OBJECT_SAFE_FREE(deliver);
}


void test_report()
{
	int ret;
	tsms_tpdu_report_handle_t* report = tsk_null;
	tsk_buffer_t* udata = tsk_null;
	char* hex;
	
	report = tsms_tpdu_report_create("+27381000015", tsk_false, tsk_true);
	
	/* set user data */
	if((udata = tsms_pack_to_7bit(USER_DATA))){
		ret = tsms_tpdu_deliver_set_userdata(report, udata, tsms_alpha_7bit);
		if((hex = tsms_tpdu_deliver_tohexastring(report))){
			TSK_DEBUG_INFO("SMS-DELIVER-REPORT for RP-ERROR=%s", hex);
			TSK_FREE(hex);
		}
		TSK_OBJECT_SAFE_FREE(udata);
	}

	TSK_OBJECT_SAFE_FREE(report);
}

void test_command()
{
	tsms_tpdu_command_handle_t* command = tsk_null;
	char* hex;
	
	command = tsms_tpdu_command_create(__pdu_last_mr++, "+3310000", "+332666", 0xF8, tsms_tpdu_cmd_delete);
		
	if((hex = tsms_tpdu_command_tohexastring(command))){
		TSK_DEBUG_INFO("SMS-COMMAND=%s", hex);
		TSK_FREE(hex);
	}

	TSK_OBJECT_SAFE_FREE(command);
}

void test_sreport()
{
	tsms_tpdu_status_report_handle_t* sreport = tsk_null;
	char* hex;
	
	sreport = tsms_tpdu_status_report_create(__pdu_last_mr++, "+3310000", "+332666", tsms_tpdu_status_busy, tsk_true);
		
	if((hex = tsms_tpdu_command_tohexastring(sreport))){
		TSK_DEBUG_INFO("SMS-STATUS-REPORT=%s", hex);
		TSK_FREE(hex);
	}

	TSK_OBJECT_SAFE_FREE(sreport);
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
