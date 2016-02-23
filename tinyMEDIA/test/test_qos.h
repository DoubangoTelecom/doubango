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
#ifndef _TEST_QOS_H_
#define _TEST_QOS_H_

char* test_qos_tostring(const tmedia_qos_tline_t* tline)
{
    char* ret = tsk_null;
    tsdp_header_M_t* M;

    if(!tline) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }

    M = tsdp_header_M_create("audio", 20000, "RTP/AVP");
    tmedia_qos_tline_to_sdp(tline, M);

    ret = tsdp_header_tostring(TSDP_HEADER(M));
    TSK_OBJECT_SAFE_FREE(M);

    return ret;
}

void test_qos_parser()
{
    tsdp_header_M_t* M;
    tmedia_qos_tline_e2e_t* e2e;
    tmedia_qos_tline_segmented_t* segmented;
    char* temp = tsk_null;
    tmedia_qos_stype_t type;
    tsk_bool_t canresume;

    /* test E2E */
    M = tsdp_header_M_create("audio", 20000, "RTP/AVP");
    e2e = tmedia_qos_tline_e2e_create(tmedia_qos_strength_mandatory);
    // to_sdp
    tmedia_qos_tline_e2e_to_sdp(e2e, M);
    if((type = tmedia_qos_get_type(M)) != tmedia_qos_stype_e2e) {
        TSK_DEBUG_ERROR("Invalid type");
    }
    if((temp = tsdp_header_tostring(TSDP_HEADER(M)))) {
        TSK_DEBUG_INFO("E2E to_sdp: %s", temp);
        TSK_FREE(temp);
    }
    // from_sdp
    TSK_OBJECT_SAFE_FREE(e2e);
    e2e = tmedia_qos_tline_e2e_from_sdp(M);
    canresume = tmedia_qos_tline_e2e_canresume(e2e);
    tmedia_qos_tline_e2e_to_sdp(e2e, M);
    if((temp = tsdp_header_tostring(TSDP_HEADER(M)))) {
        TSK_DEBUG_INFO("e2e from_sdp: %s", temp);
        TSK_FREE(temp);
    }


    TSK_OBJECT_SAFE_FREE(e2e);
    TSK_OBJECT_SAFE_FREE(M);

    /* test Segmented */
    M = tsdp_header_M_create("video", 20002, "RTP/AVP");
    segmented = tmedia_qos_tline_segmented_create(tmedia_qos_strength_none);
    segmented->remote_send.strength = tmedia_qos_strength_optional;
    // to_sdp
    tmedia_qos_tline_segmented_to_sdp(segmented, M);
    if((type = tmedia_qos_get_type(M)) != tmedia_qos_stype_segmented) {
        TSK_DEBUG_ERROR("Invalid type");
    }
    if((temp = tsdp_header_tostring(TSDP_HEADER(M)))) {
        TSK_DEBUG_INFO("Segmented to_sdp: %s", temp);
        TSK_FREE(temp);
    }
    // from_sdp
    TSK_OBJECT_SAFE_FREE(segmented);
    segmented = tmedia_qos_tline_segmented_from_sdp(M);
    canresume = tmedia_qos_tline_segmented_canresume(segmented);
    tmedia_qos_tline_segmented_to_sdp(segmented, M);
    if((temp = tsdp_header_tostring(TSDP_HEADER(M)))) {
        TSK_DEBUG_INFO("Segmented from_sdp: %s", temp);
        TSK_FREE(temp);
    }



    TSK_OBJECT_SAFE_FREE(segmented);
    TSK_OBJECT_SAFE_FREE(M);
}


void test_qos_e2e_neg()
{
    tmedia_qos_tline_e2e_t *e2eA = tsk_null, *e2eB = tsk_null;
    char* temp = tsk_null;

    /* SDP1: A includes end-to-end quality of service preconditions in the
    	initial offer.

    	m=audio 20000 RTP/AVP 0
        c=IN IP4 192.0.2.1
        a=curr:qos e2e none
        a=des:qos mandatory e2e sendrecv
    */
    e2eA = tmedia_qos_tline_e2e_create(tmedia_qos_strength_mandatory);
    if((temp = test_qos_tostring((const tmedia_qos_tline_t*)e2eA))) {
        TSK_DEBUG_INFO("SDP1=\n%s", temp);
        TSK_FREE(temp);
    }

    /*	SDP2: Since B uses RSVP, it can know when resources in its "send"
    	direction are available, because it will receive RESV messages from
    	the network.  However, it does not know the status of the
    	reservations in the other direction.  B requests confirmation for
    	resource reservations in its "recv" direction to the peer user agent
    	A in its answer.

    	m=audio 30000 RTP/AVP 0
        c=IN IP4 192.0.2.4
        a=curr:qos e2e none
        a=des:qos mandatory e2e sendrecv
        a=conf:qos e2e recv
    */
    e2eB = tmedia_qos_tline_e2e_create(tmedia_qos_strength_mandatory);
    tmedia_qos_tline_e2e_set_ro(e2eB, e2eA);
    if((temp = test_qos_tostring((const tmedia_qos_tline_t*)e2eB))) {
        TSK_DEBUG_INFO("SDP2=\n%s", temp);
        TSK_FREE(temp);
    }

    /* SDP3: When A receives RESV messages, it sends an updated offer (5) to B:

    	m=audio 20000 RTP/AVP 0
    	c=IN IP4 192.0.2.1
    	a=curr:qos e2e send
    	a=des:qos mandatory e2e sendrecv
    */
    tmedia_qos_tline_e2e_set_ro(e2eA, e2eB);
    if((temp = test_qos_tostring((const tmedia_qos_tline_t*)e2eA))) {
        TSK_DEBUG_INFO("SDP3=\n%s", temp);
        TSK_FREE(temp);
    }

    /* SDP4: B responds with an answer (6) which contains the current status
    	of the resource reservation (i.e., sendrecv):

    	m=audio 30000 RTP/AVP 0
    	c=IN IP4 192.0.2.4
    	a=curr:qos e2e sendrecv
    	a=des:qos mandatory e2e sendrecv
    */
    tmedia_qos_tline_e2e_set_ro(e2eB, e2eA);
    if((temp = test_qos_tostring((const tmedia_qos_tline_t*)e2eB))) {
        TSK_DEBUG_INFO("SDP4=\n%s", temp);
        TSK_FREE(temp);
    }

    /* A receive B's response */
    tmedia_qos_tline_e2e_set_ro(e2eA, e2eB);
    if((temp = test_qos_tostring((const tmedia_qos_tline_t*)e2eA))) {
        TSK_DEBUG_INFO("SDP5=\n%s", temp);
        TSK_FREE(temp);
    }

    if(tmedia_qos_tline_e2e_canresume(e2eA)) {
        TSK_DEBUG_INFO("A can resume");
    }
    else {
        TSK_DEBUG_ERROR("A can't resume");
    }

    if(tmedia_qos_tline_e2e_canresume(e2eB)) {
        TSK_DEBUG_INFO("B can resume");
    }
    else {
        TSK_DEBUG_ERROR("B can't resume");
    }

    TSK_OBJECT_SAFE_FREE(e2eB);
    TSK_OBJECT_SAFE_FREE(e2eA);
}


void test_qos_segmented_neg()
{
    tmedia_qos_tline_segmented_t *segA = tsk_null, *segB = tsk_null;
    char* temp = tsk_null;

    /*	INVITE
    	a=curr:qos local none
    	a=curr:qos remote none
    	a=des:qos mandatory local sendrecv
    	a=des:qos mandatory remote sendrecv
    */
    segA = tmedia_qos_tline_segmented_create(tmedia_qos_strength_mandatory);
    if((temp = test_qos_tostring((const tmedia_qos_tline_t*)segA))) {
        TSK_DEBUG_INFO("INVITE=\n%s", temp);
        TSK_FREE(temp);
    }

    /*	183 Sesson progress
    	a=curr:qos local none
    	a=curr:qos remote none
    	a=des:qos mandatory local sendrecv
    	a=des:qos mandatory remote sendrecv
    	a=conf:qos remote sendrecv
    */
    segB = tmedia_qos_tline_segmented_create(tmedia_qos_strength_mandatory);
    tmedia_qos_tline_segmented_set_ro(segB, segA);
    if((temp = test_qos_tostring((const tmedia_qos_tline_t*)segB))) {
        TSK_DEBUG_INFO("183=\n%s", temp);
        TSK_FREE(temp);
    }

    /*	UPDATE
    	a=curr:qos local sendrecv
    	a=curr:qos remote none
    	a=des:qos mandatory local sendrecv
    	a=des:qos mandatory remote sendrecv
    */
    tmedia_qos_tline_segmented_set_ro(segA, segB);
    if((temp = test_qos_tostring((const tmedia_qos_tline_t*)segA))) {
        TSK_DEBUG_INFO("UPDATE=\n%s", temp);
        TSK_FREE(temp);
    }

    /* 200 OK
    	a=curr:qos local sendrecv
    	a=curr:qos remote sendrecv
    	a=des:qos mandatory local sendrecv
    	a=des:qos mandatory remote sendrecv
    */
    tmedia_qos_tline_segmented_set_ro(segB, segA);
    if((temp = test_qos_tostring((const tmedia_qos_tline_t*)segB))) {
        TSK_DEBUG_INFO("200OK=\n%s", temp);
        TSK_FREE(temp);
    }

    TSK_OBJECT_SAFE_FREE(segA);
    TSK_OBJECT_SAFE_FREE(segB);
}

void test_qos()
{
    //test_qos_parser();
    //test_qos_e2e_neg();
    test_qos_segmented_neg();
}

#endif /* _TEST_QOS_H_ */
