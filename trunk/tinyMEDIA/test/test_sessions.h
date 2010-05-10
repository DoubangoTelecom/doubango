/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)yahoo.fr>
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
#ifndef _TEST_SESSIONS_H_
#define _TEST_SESSIONS_H_

void test_sessions()
{
	tmedia_session_mgr_t* mgr;
	const tsdp_message_t* sdp_lo;
	tsdp_message_t* sdp_ro;
	char* temp;
	
	/* create manager */
	mgr = tmedia_session_mgr_create((tmedia_audio | tmedia_video | tmedia_msrp | tmedia_t38),
		"192.168.16.82", tsk_false);

	/* get local sdp */
	sdp_lo = tmedia_session_mgr_get_lo(mgr);
	if((temp = tsdp_message_tostring(sdp_lo))){
		TSK_DEBUG_INFO("sdp_lo=%s", temp);
		TSK_FREE(temp);
	}
	
	TSK_OBJECT_SAFE_FREE(mgr);
}

#endif /* _TEST_SESSIONS_H_ */
