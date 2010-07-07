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
#ifndef _TINYDEV_TEST_SESSIONS_H
#define _TINYDEV_TEST_SESSIONS_H

#define SDP_RO \
	"v=0\r\n" \
	"o=alice 2890844526 2890844526 IN IP4 host.atlanta.example.com\r\n" \
	"s=\r\n" \
	"i=A Seminar on the session description protocol\r\n" \
	"u=http://www.example.com/seminars/sdp.pdf\r\n" \
	"e=j.doe@example.com (Jane Doe)\r\n" \
	"p=+1 617 555-6011\r\n" \
	"c=IN IP4 192.168.0.16\r\n" \
	"b=X-YZ:128\r\n" \
	"z=2882844526 -1h 2898848070 0\r\n" \
	"k=base64:ZWFzdXJlLg==\r\n" \
	"t=3034423619 3042462419\r\n" \
    "r=7d 1h 0 25h\r\n" \
	"r=604800 3600 0 90000\r\n" \
	"w=my dummy header\r\n" \
	"m=audio 49170 RTP/AVP 103 102 96 3 8 0\r\n" \
	"i=Audio line\r\n" \
	"c=IN IP4 192.168.0.15\r\n" \
	"k=base64:ZWFzdXJlLgdddddddddddddddddddddd==\r\n" \
	"a=rtpmap:96 iLBC/8000\r\n" \
	"a=rtpmap:3 GSM/8000\r\n" \
	"a=rtpmap:8 PCMA/8000\r\n" \
	"a=rtpmap:0 PCMU/8000\r\n" \
	"a=rtpmap:103 AMR/8000\r\n" \
	"a=rtpmap:102 AMR/8000\r\n" \
	"a=fmtp:102 octet-align=0; mode-set=0,1,2,3,4,5,6,7; mode-change-period=1; mode-change-capability=2; mode-change-neighbor=0\r\n" \
    "a=fmtp:103 octet-align=1; mode-set=0,1,2,3,4,5,6,7; mode-change-period=1; mode-change-capability=2; mode-change-neighbor=0\r\n" \
	"m=video 51372 RTP/AVP 31 111 34 32 98\r\n" \
	"i=Video line\r\n" \
	"b=A-YZ:92\r\n" \
	"b=B-YZ:256\r\n" \
	"a=rtpmap:34 H263/90000\r\n" \
	"a=fmtp:34 QCIF=1 MaxBR=4520\r\n" \
	"a=rtpmap:111 H263-1998/90000\r\n" \
	"a=rtpmap:31 H261/90000\r\n" \
	"a=rtpmap:32 MPV/90000\r\n" \
	"a=rtpmap:98 H264/90000\r\n" \
    "a=fmtp:98 profile-level-id=42A01E\r\n" \
	"a=recvonly\r\n" \
	"m=toto 51372 RTP/AVP 31 32\r\n" \
	"i=Video line\r\n" \
	"b=A-YZ:92\r\n" \
	"b=B-YZ:256\r\n" \
	"a=rtpmap:31 H261/90000\r\n" \
	"a=rtpmap:32 MPV/90000\r\n" \
	"a=recvonly\r\n"

void test_sessions_client()
{
	tmedia_session_mgr_t* mgr;
	const tsdp_message_t* sdp_lo;
	tsdp_message_t* sdp_ro;
	char* temp;
	tmedia_type_t type = tmedia_audio /*| tmedia_video*/;

	mgr = tmedia_session_mgr_create(type,
		"0.0.0.0", tsk_false, tsk_true/* offerer */);

	/* get lo */
	sdp_lo = tmedia_session_mgr_get_lo(mgr);
	if((temp = tsdp_message_tostring(sdp_lo))){
		TSK_DEBUG_INFO("sdp_lo=%s", temp);
		TSK_FREE(temp);
	}

	/* set ro */
	if((sdp_ro = tsdp_message_parse(SDP_RO, tsk_strlen(SDP_RO)))){
		tmedia_session_mgr_set_ro(mgr, sdp_ro);
		TSK_OBJECT_SAFE_FREE(sdp_ro);
	}

	/* start() */
	tmedia_session_mgr_start(mgr);

	//getchar();

	/* for fun, send DTMF */
	//tmedia_session_mgr_send_dtmf(mgr, 1);
	//tmedia_session_mgr_send_dtmf(mgr, 10);
	//tmedia_session_mgr_send_dtmf(mgr, 11);

	getchar();

	/* stop() */
	//tmedia_session_mgr_stop(mgr);

	//getchar();

	TSK_OBJECT_SAFE_FREE(mgr);
}

void test_sessions_server()
{
	tmedia_session_mgr_t* mgr;
	const tsdp_message_t* sdp_lo;
	tsdp_message_t* sdp_ro;
	char* temp;
	tmedia_type_t type;

	/* get ro (INVITE) */
	if((sdp_ro = tsdp_message_parse(SDP_RO, tsk_strlen(SDP_RO)))){
		//type = tmedia_type_from_sdp(sdp_ro);
		type = tmedia_audio;
		mgr = tmedia_session_mgr_create(type,
			"0.0.0.0", tsk_false, tsk_false/* answerer */);
		tmedia_session_mgr_set_ro(mgr, sdp_ro);
		TSK_OBJECT_SAFE_FREE(sdp_ro);
	}
	else{
		TSK_DEBUG_ERROR("Failed to deserialize remote sdp");
		return;
	}

	/* get lo (200 OK) */
	sdp_lo = tmedia_session_mgr_get_lo(mgr);
	if((temp = tsdp_message_tostring(sdp_lo))){
		TSK_DEBUG_INFO("sdp_lo=%s", temp);
		TSK_FREE(temp);
	}

	/* ACK */

	/* start() */
	tmedia_session_mgr_start(mgr);

	getchar();

	/* stop() */
	//tmedia_session_mgr_stop(mgr);

	//getchar();

	TSK_OBJECT_SAFE_FREE(mgr);
}


void test_sessions()
{	
	//test_sessions_client();
	test_sessions_server();



}


#endif /* _TINYDEV_TEST_SESSIONS_H */
