/*
* Copyright (C) 2009 Mamadou Diop.
* Copyright (C) 2012 Doubango Telecom <http://www.doubango.org>
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
#ifndef _TEST_SOA_H
#define _TEST_SOA_H

#define SDP \
	"v=0\r\n" \
	"o=carol 28908764872 28908764872 IN IP4 100.3.6.6\r\n" \
	"s=-\r\n" \
	"t=0 0\r\n" \
	"c=IN IP4 192.0.2.4\r\n" \
	"m=video 0 RTP/AVP 31 34\r\n" \
	"a=rtpmap:31 H261/90000\r\n" \
	"a=rtpmap:34 H263/90000\r\n"


void test_create_sdp()
{
	tsdp_ctx_handle_t* ctx = TSDP_CTX_CREATE();
	const tsdp_message_t* sdp;
	char* str;
	
	// Create local SDP from string
	tsdp_ctx_local_create_sdp_2(ctx, SDP, strlen(SDP));
	if((sdp = tsdp_ctx_local_get_sdp(ctx))){
		if((str = tsdp_message_tostring(sdp))){
			TSK_DEBUG_INFO("Local SDP (2)=\n%s", str);
			TSK_FREE(str);
		}
	}
	
	// Create local SDP from object
	tsdp_ctx_local_create_sdp(ctx, sdp);
	if((sdp = tsdp_ctx_local_get_sdp(ctx))){
		if((str = tsdp_message_tostring(sdp))){
			TSK_DEBUG_INFO("Local SDP (1)=\n%s", str);
			TSK_FREE(str);
		}
	}

	// Add media to the local sdp
	tsdp_ctx_local_add_media_2(ctx, "audio", 0, "RTP/AVP",
			TSDP_HEADER_I_VA_ARGS("this is the (audio)information line"),
			
			// PCMU
			TSDP_FMT_VA_ARGS("0"),
			TSDP_HEADER_A_VA_ARGS("rtpmap", "0 PCMU/8000"),

			// 1016
			TSDP_FMT_VA_ARGS("1"),
			TSDP_HEADER_A_VA_ARGS("rtpmap", "1 1016/8000"),

			// GSM
			TSDP_FMT_VA_ARGS("3"),
			TSDP_HEADER_A_VA_ARGS("rtpmap", "3 GSM/8000"),

			tsk_null);
	if((str = tsdp_message_tostring(sdp))){
			TSK_DEBUG_INFO("Local SDP (audio)=\n%s", str);
			TSK_FREE(str);
		}

	// Add headers to the local sdp
	tsdp_ctx_local_add_headers(ctx,
			TSDP_HEADER_E_VA_ARGS("j.doe@example.com (Jane Doe)"),
			TSDP_HEADER_P_VA_ARGS("+44 (123)456789"),

			tsk_null);
	if((str = tsdp_message_tostring(sdp))){
			TSK_DEBUG_INFO("Local SDP (headers)=\n%s", str);
			TSK_FREE(str);
		}
	

	TSK_OBJECT_SAFE_FREE(ctx);
}

void test_soa()
{
	test_create_sdp();
}









#endif /* _TEST_SOA_H */
