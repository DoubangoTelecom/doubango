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

#include "tinymsrp.h"

#define SDP_RO \
	"v=0\r\n" \
	"o=alice 2890844526 2890844526 IN IP4 host.atlanta.example.com\r\n" \
	"s=\r\n" \
	"i=A Seminar on the session description protocol\r\n" \
	"u=http://www.example.com/seminars/sdp.pdf\r\n" \
	"e=j.doe@example.com (Jane Doe)\r\n" \
	"p=+1 617 555-6011\r\n" \
	"c=IN IP4 192.168.0.13\r\n" \
	"b=X-YZ:128\r\n" \
	"z=2882844526 -1h 2898848070 0\r\n" \
	"k=base64:ZWFzdXJlLg==\r\n" \
	"t=3034423619 3042462419\r\n" \
    "r=7d 1h 0 25h\r\n" \
	"r=604800 3600 0 90000\r\n" \
	"w=my dummy header\r\n" \
	"m=audio 6061 RTP/AVP 18 0 115 97 96 3 102 103 8\r\n" \
	"i=Audio line\r\n" \
	"c=IN IP4 192.168.0.13\r\n" \
	"k=base64:ZWFzdXJlLgdddddddddddddddddddddd==\r\n" \
	"a=rtpmap:96 iLBC/8000\r\n" \
	"a=rtpmap:3 GSM/8000\r\n" \
	"a=rtpmap:8 PCMA/8000\r\n" \
	"a=rtpmap:0 PCMU/8000\r\n" \
	"a=rtpmap:103 AMR/8000\r\n" \
	"a=rtpmap:102 AMR/8000\r\n" \
	"a=rtpmap:97 SPEEX/8000\r\n" \
	"a=rtpmap:115 BV16/8000\r\n" \
	"a=fmtp:102 octet-align=0; mode-set=0,1,2,3,4,5,6,7; mode-change-period=1; mode-change-capability=2; mode-change-neighbor=0\r\n" \
    "a=fmtp:103 octet-align=1; mode-set=0,1,2,3,4,5,6,7; mode-change-period=1; mode-change-capability=2; mode-change-neighbor=0\r\n" \
	"m=video 6060 RTP/AVP 125 98 111 121 31 126 34 32\r\n" \
	"i=Video line\r\n" \
	"b=A-YZ:92\r\n" \
	"b=B-YZ:256\r\n" \
	"a=rtpmap:125 VP8/90000\r\n" \
	"a=fmtp:125 QCIF=2;CIF=2 MaxBR=4520\r\n" \
	"a=rtpmap:121 MP4V-ES/90000\r\n" \
	"a=fmtp:121 profile-level-id=1\r\n" \
	"a=rtpmap:126 theora/90000\r\n" \
	"a=rtpmap:34 H263/90000\r\n" \
	"a=fmtp:34 QCIF=1 MaxBR=4520\r\n" \
	"a=rtpmap:111 H263-1998/90000\r\n" \
	"a=rtpmap:31 H261/90000\r\n" \
	"a=rtpmap:32 MPV/90000\r\n" \
	"a=rtpmap:98 H264/90000\r\n" \
    "a=fmtp:98 profile-level-id=42e01e; packetization-mode=1; max-br=452; max-mbps=11880\r\n" \
	"a=recvonly\r\n" \
	"m=toto 51372 RTP/AVP 31 32\r\n" \
	"i=Video line\r\n" \
	"b=A-YZ:92\r\n" \
	"b=B-YZ:256\r\n" \
	"a=rtpmap:31 H261/90000\r\n" \
	"a=rtpmap:32 MPV/90000\r\n" \
	"a=recvonly\r\n" \
	"m=message 2000 TCP/MSRP *\r\n" \
	"c=IN IP4 192.168.0.14\r\n" \
	"a=path:msrp://192.168.0.12:2000/fdxfircvscx;tcp\r\n" \
	"a=accept-types:message/CPIM\r\n" \
	"a=setup:passive\r\n" \
	"a=file-transfer-id:wcysyycqpevikeffmznimkkasvwsrenz\r\n" \
	"a=file-selector:name:\"test.zip\" type:application/octet-stream size:11376 hash:sha-1:8D:55:24:2B:F4:F9:9B:A2:54:A3:5B:91:00:15:9E:A3:D4:48:D7:DF\r\n" \
 
static int test_session_msrp_cb(const tmsrp_event_t* _event);

void test_sessions_client()
{
    tmedia_session_mgr_t* mgr;
    const tsdp_message_t* sdp_lo;
    tsdp_message_t* sdp_ro;
    char* temp;
    tmedia_type_t type = tmedia_video/*tmedia_msrp | tmedia_audio*//*| tmedia_video tmedia_msrp*/;

    tdav_codec_set_priority(tdav_codec_id_vp8, 0);

    mgr = tmedia_session_mgr_create(type,
                                    "192.168.0.13", tsk_false, tsk_true/* offerer */);

    /* set MSRP callback */
    //tmedia_session_mgr_set_msrp_cb(mgr, tsk_null, test_session_msrp_cb);

    /* MSRP File Transfer */
    /*tmedia_session_mgr_set(mgr,
    	TMEDIA_SESSION_MSRP_SET_STR("file-path", "C:\\avatar.png"),
    	TMEDIA_SESSION_MSRP_SET_STR("accept-types", "message/CPIM application/octet-stream"),
    	TMEDIA_SESSION_MSRP_SET_STR("accept-wrapped-types", "application/octet-stream"),
    	TMEDIA_SESSION_MSRP_SET_STR("accept-wrapped-types", "application/octet-stream"),
    	TMEDIA_SESSION_MSRP_SET_STR("file-selector", "name:\"test.zip\" type:application/octet-stream size:20312 hash:sha-1:27:D0:AE:39:48:77:37:1D:FD:39:7E:2D:78:2F:BC:7B:94:48:29:81"),
    	TMEDIA_SESSION_MSRP_SET_STR("file-disposition", "attachment"),
    	TMEDIA_SESSION_MSRP_SET_STR("file-date", "creation:2010-02-13T17:50:31.763Z"),
    	TMEDIA_SESSION_MSRP_SET_STR("file-icon", "cid:test@doubango.org"),

    	TMEDIA_SESSION_SET_NULL());*/


    /* get lo */
    sdp_lo = tmedia_session_mgr_get_lo(mgr);
    if((temp = tsdp_message_tostring(sdp_lo))) {
        TSK_DEBUG_INFO("sdp_lo=%s", temp);
        TSK_FREE(temp);
    }

    /* set ro */
    if((sdp_ro = tsdp_message_parse(SDP_RO, tsk_strlen(SDP_RO)))) {
        tmedia_session_mgr_set_ro(mgr, sdp_ro);
        TSK_OBJECT_SAFE_FREE(sdp_ro);
    }

    /* start() */
    tmedia_session_mgr_start(mgr);

    /* send file */
    //tmedia_session_mgr_send_file(mgr, "C:\\avatar.png", TMEDIA_SESSION_SET_NULL());

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
    if((sdp_ro = tsdp_message_parse(SDP_RO, tsk_strlen(SDP_RO)))) {
        //type = tmedia_type_from_sdp(sdp_ro);
        type = tmedia_video;
        mgr = tmedia_session_mgr_create(type,
                                        "192.168.0.13", tsk_false, tsk_false/* answerer */);
        tmedia_session_mgr_set_ro(mgr, sdp_ro);
        TSK_OBJECT_SAFE_FREE(sdp_ro);
    }
    else {
        TSK_DEBUG_ERROR("Failed to deserialize remote sdp");
        return;
    }

    /* get lo (200 OK) */
    sdp_lo = tmedia_session_mgr_get_lo(mgr);
    if((temp = tsdp_message_tostring(sdp_lo))) {
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
    test_sessions_client();
    //test_sessions_server();
}


int test_session_msrp_cb(const tmsrp_event_t* _event)
{
    if(TMSRP_MESSAGE_IS_REQUEST(_event->message)) {
        TSK_DEBUG_INFO("Received MSRP request");
    }
    else {
        TSK_DEBUG_INFO("Received MSRP response");
    }

    return 0;
}

#endif /* _TINYDEV_TEST_SESSIONS_H */
