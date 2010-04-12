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


void test_tpdu()
{
	static uint8_t __pdu_last_mr = 1;

	tsms_tpdu_ctx_handle_t* handle;
	tsk_buffer_t* buffer = tsk_null;
	
	handle = tsms_pdu_ctx_create(__pdu_last_mr++, "+331000000", "+3361234567");
	
	if((buffer = tsms_pdu_ctx_getSUBMIT(handle))){
		TSK_OBJECT_SAFE_FREE(buffer);
	}
	
	TSK_OBJECT_SAFE_FREE(handle);
}

#endif /* _TEST_SMSTPDU_H */
