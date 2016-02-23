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

#define SDP_RO \
	"v=0\r\n" \
	"o=alice 2890844526 2890844526 IN IP4 host.atlanta.example.com\r\n" \
	"s=\r\n" \
	"i=A Seminar on the session description protocol\r\n" \
	"u=http://www.example.com/seminars/sdp.pdf\r\n" \
	"e=j.doe@example.com (Jane Doe)\r\n" \
	"p=+1 617 555-6011\r\n" \
	"c=IN IP4 host.atlanta.example.com\r\n" \
	"b=X-YZ:128\r\n" \
	"z=2882844526 -1h 2898848070 0\r\n" \
	"k=base64:ZWFzdXJlLg==\r\n" \
	"t=3034423619 3042462419\r\n" \
    "r=7d 1h 0 25h\r\n" \
	"r=604800 3600 0 90000\r\n" \
	"w=my dummy header\r\n" \
	"m=audio 49170 RTP/AVP 8 0 97 98\r\n" \
	"i=Audio line\r\n" \
	"c=IN IP4 otherhost.biloxi.example.com\r\n" \
	"k=base64:ZWFzdXJlLgdddddddddddddddddddddd==\r\n" \
	"a=rtpmap:8 PCMA/8000\r\n" \
	"a=rtpmap:0 PCMU/8000\r\n" \
	"a=rtpmap:97 iLBC/8000\r\n" \
	"a=rtpmap:98 AMR-WB/16000\r\n" \
	"a=curr:qos local none\r\n" \
	"a=curr:qos remote none\r\n" \
	"a=des:qos mandatory local sendrecv\r\n" \
	"a=des:qos mandatory remote sendrecv\r\n" \
	"a=conf:qos remote sendrecv\r\n" \
    "a=fmtp:98 octet-align=1\r\n" \
	"m=video 51372 RTP/AVP 31 32 98\r\n" \
	"i=Video line\r\n" \
	"b=A-YZ:92\r\n" \
	"b=B-YZ:256\r\n" \
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
    tsk_bool_t canresume;
    tmedia_type_t media_type = (tmedia_audio | tmedia_video | tmedia_msrp | tmedia_t38);

    int32_t width = 176;
    int64_t height = 144LL;


    /* create manager */
    mgr = tmedia_session_mgr_create((tmedia_audio | tmedia_video | tmedia_msrp | tmedia_t38),
                                    "0.0.0.0", tsk_false, tsk_true);
    tmedia_session_mgr_set_qos(mgr, tmedia_qos_stype_segmented, tmedia_qos_strength_mandatory);

    tmedia_session_mgr_set(mgr,
                           TMEDIA_SESSION_VIDEO_SET_INT32("width", width),
                           TMEDIA_SESSION_VIDEO_SET_INT64("height", height),
                           TMEDIA_SESSION_VIDEO_SET_STR("description", "This is my session"),
                           TMEDIA_SESSION_AUDIO_SET_INT32("rate", "8000"),
                           TMEDIA_SESSION_SET_STR(tmedia_audio | tmedia_video, "hello", "world"),
                           TMEDIA_SESSION_SET_NULL());

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

    /* get lo */
    sdp_lo = tmedia_session_mgr_get_lo(mgr);
    if((temp = tsdp_message_tostring(sdp_lo))) {
        TSK_DEBUG_INFO("sdp_lo=%s", temp);
        TSK_FREE(temp);
    }

    tmedia_session_mgr_start(mgr);

    canresume = tmedia_session_mgr_canresume(mgr);

    TSK_OBJECT_SAFE_FREE(mgr);
}

void test_sessions_server()
{
    tmedia_session_mgr_t* mgr = tsk_null;
    const tsdp_message_t* sdp_lo;
    tsdp_message_t* sdp_ro = tsk_null;
    char* temp;
    tmedia_type_t type;

    /* parse ro */
    if(!(sdp_ro = tsdp_message_parse(SDP_RO, tsk_strlen(SDP_RO)))) {
        TSK_DEBUG_ERROR("Failed to parse ro");
        return;
    }
    else {
        /* get ro media type */
        type = tmedia_type_from_sdp(sdp_ro);
    }

    /* create manager */
    mgr = tmedia_session_mgr_create(type, "192.168.16.82", tsk_false, tsk_false);

    /* set ro */
    tmedia_session_mgr_set_ro(mgr, sdp_ro);

    /* get lo */
    sdp_lo = tmedia_session_mgr_get_lo(mgr);
    if((temp = tsdp_message_tostring(sdp_lo))) {
        TSK_DEBUG_INFO("sdp_lo=%s", temp);
        TSK_FREE(temp);
    }

    TSK_OBJECT_SAFE_FREE(sdp_ro);
    TSK_OBJECT_SAFE_FREE(mgr);
}

void test_sessions_hold_resume()
{
    tmedia_session_mgr_t* mgr;
    const tsdp_message_t* sdp_lo;
    char* temp;
    tmedia_type_t type = tmedia_audio | tmedia_video | tmedia_msrp | tmedia_t38;

    /* create manager */
    mgr = tmedia_session_mgr_create(type, "192.168.16.82", tsk_false, tsk_true);

    /* get lo */
    sdp_lo = tmedia_session_mgr_get_lo(mgr);
    if((temp = tsdp_message_tostring(sdp_lo))) {
        TSK_DEBUG_INFO("sdp_lo=%s", temp);
        TSK_FREE(temp);
    }

    /* hold */
    tmedia_session_mgr_hold(mgr, type);
    sdp_lo = tmedia_session_mgr_get_lo(mgr);
    if((temp = tsdp_message_tostring(sdp_lo))) {
        TSK_DEBUG_INFO("sdp_lo(hold)=%s", temp);
        TSK_FREE(temp);
    }
    TSK_DEBUG_INFO("Hold local=%s and remote=%s",
                   tmedia_session_mgr_is_held(mgr, type, tsk_true) ? "yes" : "no",
                   tmedia_session_mgr_is_held(mgr, type, tsk_false) ? "yes" : "no"
                  );

    /* resume */
    tmedia_session_mgr_resume(mgr, type, tsk_true);
    sdp_lo = tmedia_session_mgr_get_lo(mgr);
    if((temp = tsdp_message_tostring(sdp_lo))) {
        TSK_DEBUG_INFO("sdp_lo(resume)=%s", temp);
        TSK_FREE(temp);
    }
    TSK_DEBUG_INFO("Hold local=%s and remote=%s",
                   tmedia_session_mgr_is_held(mgr, type, tsk_true) ? "yes" : "no",
                   tmedia_session_mgr_is_held(mgr, type, tsk_false) ? "yes" : "no"
                  );

    TSK_OBJECT_SAFE_FREE(mgr);
}

void test_sessions_add_remove()
{
    tmedia_session_mgr_t* mgr = tsk_null;
    const tsdp_message_t* sdp_lo;
    tsdp_message_t* sdp_ro = tsk_null;
    char* temp;
    tmedia_type_t type;

    /* parse ro */
    if(!(sdp_ro = tsdp_message_parse(SDP_RO, tsk_strlen(SDP_RO)))) {
        TSK_DEBUG_ERROR("Failed to parse ro");
        return;
    }
    else {
        /* get ro media type */
        type = tmedia_type_from_sdp(sdp_ro);
    }

    /* create manager */
    mgr = tmedia_session_mgr_create(type, "192.168.16.82", tsk_false, tsk_false);

    /* set ro */
    tmedia_session_mgr_set_ro(mgr, sdp_ro);

    /* get lo */
    sdp_lo = tmedia_session_mgr_get_lo(mgr);
    if((temp = tsdp_message_tostring(sdp_lo))) {
        TSK_DEBUG_INFO("sdp_lo=%s", temp);
        TSK_FREE(temp);
    }

    TSK_OBJECT_SAFE_FREE(sdp_ro);
    TSK_OBJECT_SAFE_FREE(mgr);
}

void test_sessions()
{
    test_sessions_client();
    //test_sessions_server();
    //test_sessions_hold_resume();
}

#endif /* _TEST_SESSIONS_H_ */
