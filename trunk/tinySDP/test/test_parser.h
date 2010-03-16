/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou@yahoo.fr>
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
#ifndef _TEST_SDPPARSER_H
#define _TEST_SDPPARSER_H

#include "tinySDP/headers/tsdp_header_Dummy.h"

#define SDP_MSG1 \
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
	"m=audio 49170 RTP/AVP 0 8 97\r\n" \
	"i=Audio line\r\n" \
	"c=IN IP4 otherhost.biloxi.example.com\r\n" \
	"k=base64:ZWFzdXJlLgdddddddddddddddddddddd==\r\n" \
	"a=rtpmap:0 PCMU/8000\r\n" \
	"a=rtpmap:8 PCMA/8000\r\n" \
	"a=rtpmap:97 iLBC/8000\r\n" \
	"m=video 51372 RTP/AVP 31 32\r\n" \
	"i=Video line\r\n" \
	"b=A-YZ:92\r\n" \
	"b=B-YZ:256\r\n" \
	"a=rtpmap:31 H261/90000\r\n" \
	"a=rtpmap:32 MPV/90000\r\n" \
	"a=sendonly\r\n"
	
#define SDP_MSG2 \
	"v=0\r\n"


#define SDP_MSG_TO_TEST SDP_MSG1

void test_parser()
{
	tsdp_message_t *message = 0;
	
	/* deserialize the message */
	if((message = tsdp_message_parse(SDP_MSG_TO_TEST, strlen(SDP_MSG_TO_TEST)))){
		tsk_buffer_t *buffer = TSK_BUFFER_CREATE_NULL();

		/* serialize the message */
		tsdp_message_tostring(message, buffer);
		TSK_DEBUG_INFO("SDP Message=\n%s", TSK_BUFFER_TO_STRING(buffer));

		TSK_OBJECT_SAFE_FREE(buffer);
	}
	else{
		TSK_DEBUG_ERROR("Failed to parse SDP message.");
	}

	TSK_OBJECT_SAFE_FREE(message);
}

#endif /* _TEST_SDPPARSER_H */
