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
* GNU Lesser General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/
#ifndef _TEST_MANAGER_H_
#define _TEST_MANAGER_H_

void test_manager()
{
	tsk_size_t i;
	trtp_manager_t* manager;

	if(!(manager = trtp_manager_create(tsk_true, "192.168.0.12", tsk_false))){
		goto bail;
	}

	trtp_manager_set_payload_type(manager, 8);

	/* Prepare: this will allow you to generate local port and ip */
	if(trtp_manager_prepare(manager)){
		goto bail;
	}

	/* set remote parameters (rtcp ip:port not mandator, could be retrieved from rtp values) */
	if(trtp_manager_set_rtp_remote(manager, "192.168.0.13", 5081)){
		goto bail;
	}
	if(trtp_manager_set_rtcp_remote(manager, "192.168.0.13", 2861)){
		goto bail;
	}

	/* start */
	if(trtp_manager_start(manager)){
		goto bail;
	}
	
	/* send data */
	for(i=0;i<2; i++){
		if(trtp_manager_send_rtp(manager, "test", tsk_strlen("test"), 160, tsk_true)){
			goto bail;
		}
	}
	
	getchar();

bail:
	/* stop and destroy */
	TSK_OBJECT_SAFE_FREE(manager);
}

#endif /* _TEST_MANAGER_H_ */

