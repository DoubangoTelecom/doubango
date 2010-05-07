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
	tsdp_message_t* sdp_lo;
	
	/* create manager */
	mgr = tmedia_session_mgr_create((tmed_sess_type_audio | tmed_sess_type_video | tmed_sess_type_msrp | tmed_sess_type_t38));

	/* get local sdp */
	sdp_lo = tmedia_session_mgr_get_lo(mgr);
	
	TSK_OBJECT_SAFE_FREE(sdp_lo);
	TSK_OBJECT_SAFE_FREE(mgr);
}

#endif /* _TEST_SESSIONS_H_ */
