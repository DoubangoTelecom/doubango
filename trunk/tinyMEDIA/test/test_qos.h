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

void test_qos()
{
	tsdp_header_M_t* M;
	tmedia_qos_tline_e2e_t* e2e;
	tmedia_qos_tline_segmented_t* segmented;
	char* temp = tsk_null;
	tmedia_qos_stype_t type;

	/* test E2E */
	M = tsdp_header_M_create("audio", 20000, "RTP/AVP");
	e2e = tmedia_qos_tline_e2e_create(tmedia_qos_strength_mandatory);
	// to_sdp
	tmedia_qos_tline_e2e_to_sdp(e2e, M);
	if((type = tmedia_qos_get_type(M)) != tmedia_qos_stype_e2e){
		TSK_DEBUG_ERROR("Invalid type");
	}
	if((temp = tsdp_header_tostring(TSDP_HEADER(M)))){
		TSK_DEBUG_INFO("E2E to_sdp: %s", temp);
		TSK_FREE(temp);
	}
	// from_sdp
	TSK_OBJECT_SAFE_FREE(e2e);
	e2e = tmedia_qos_tline_e2e_from_sdp(M);
	tmedia_qos_tline_e2e_to_sdp(e2e, M);
	if((temp = tsdp_header_tostring(TSDP_HEADER(M)))){
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
	if((type = tmedia_qos_get_type(M)) != tmedia_qos_stype_segmented){
		TSK_DEBUG_ERROR("Invalid type");
	}
	if((temp = tsdp_header_tostring(TSDP_HEADER(M)))){
		TSK_DEBUG_INFO("Segmented to_sdp: %s", temp);
		TSK_FREE(temp);
	}
	// from_sdp
	TSK_OBJECT_SAFE_FREE(segmented);
	segmented = tmedia_qos_tline_segmented_from_sdp(M);
	tmedia_qos_tline_segmented_to_sdp(segmented, M);
	if((temp = tsdp_header_tostring(TSDP_HEADER(M)))){
		TSK_DEBUG_INFO("Segmented from_sdp: %s", temp);
		TSK_FREE(temp);
	}


	
	TSK_OBJECT_SAFE_FREE(segmented);
	TSK_OBJECT_SAFE_FREE(M);
}

#endif /* _TEST_QOS_H_ */
