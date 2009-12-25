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
* GNU Lesser General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/
#ifndef _TEST_SIPMESSAGES_H
#define _TEST_SIPMESSAGES_H

#define SIP_REQUEST \
	"REGISTER sip:ims-network.com SIP/2.0\r\n" \
	"Test-Header: 0\r\n" \
	"v: SIP/2.0/UDP [::]:1988;test=1234;comp=sigcomp;rport=254;ttl=457;received=192.0.2.101;branch=z9hG4bK1245420841406\r\n" \
	"f: \"Mamadou\" <sip:mamadou@ims-network.com>;tag=29358\r\n" \
	"t:    <sip:mamadou@ims-network.com>;tag= 12345\r\n" \
	"i: M-fa53180346f7f55ceb8d8670f9223dbb\r\n" \
	"CSeq: 201 REGISTER\r\n" \
	"Max-Forwards: 70\r\n" \
	"Allow: INVITE, ACK, CANCEL, BYE, MESSAGE, OPTIONS, NOTIFY, PRACK\r\n" \
	"Allow: REFER, UPDATE\r\n" \
	"u: talk, hold, conference, LocalModeStatus\r\n" \
	"m: <sip:mamadou@[::]:1988;comp=sigcomp;transport=udp>;expires=600000;+deviceID=\"3ca50bcb-7a67-44f1-afd0-994a55f930f4\";mobility=\"fixed\";+g.3gpp.cs-voice;+g.3gpp.app%5fref=\"urn%3Aurnxxx%3A3gpp-application.ims.iari.gsmais\";+g.oma.sip-im.large-message;+g.oma.sip-im\r\n" \
	"User-Agent: IM-client/OMA1.0 doubango/v0.0.0\r\n" \
	"Require: pref, path\r\n" \
	"Require: 100rel\r\n" \
	"P-Preferred-Identity: <sip:mamadou@ims-network.com>\r\n" \
	"k: path\r\n" \
	"k: gruu, outbound, timer\r\n" \
	"P-Access-Network-Info: 3GPP-UTRAN-TDD;utran-cell-id-3gpp=00000000\r\n" \
	"Privacy: none;user;id\r\n" \
	"Supported: gruu, outbound, path, timer\r\n" \
	"Expires12: 1983\r\n" \
	"l: 180\r\n" \
	"\r\n"

#define SIP_RESPONSE \
	"SIP/2.0 200 This is my reason phrase\r\n" \
	"To: <sip:mamadou@micromethod.com>;tag=bweyal\r\n" \
	"Via: SIP/2.0/UDP 192.168.0.11:63140;branch=z9hG4bK1261611942868;rport=63140\r\n" \
	"CSeq: 31516 REGISTER\r\n" \
	"Content-Length: 0\r\n" \
	"Call-ID: 1261611941121\r\n" \
	"Min-Expires: 30\r\n" \
	"From: <sip:mamadou@micromethod.com>;tag=1261611941121\r\n" \
	"Contact: <sip:mamadou@192.168.0.11:63140>;expires=3600;q=1.0\r\n" \
	"Contact: <sip:mamadou@192.168.0.11:56717>;expires=3600;q=1.0\r\n" \
	"Contact: <sip:mamadou@127.0.0.1:5060>;expires=3600;q=1.0\r\n" \
	"Contact: <sip:mamadou@127.0.0.1>;expires=3600;q=1.0\r\n" \
	"\r\n"



void test_parser()
{
	tsip_ragel_state_t state;
	tsip_message_t *message = TSIP_MESSAGE_CREATE();
	tsk_buffer_t *buffer = TSK_BUFFER_CREATE_NULL();
	TSIP_BOOLEAN enabled;
	int32_t expires;
	uint32_t clength;

	tsip_ragel_state_init(&state, SIP_RESPONSE, strlen(SIP_RESPONSE));
	tsip_message_parse(&state, message);

	enabled = tsip_message_allowed(message, "REFER");
	enabled = tsip_message_allowed(message, "MESSAGE");
	enabled = tsip_message_allowed(message, "MSG");

	enabled = tsip_message_required(message, "path");
	enabled = tsip_message_required(message, "100rel");
	enabled = tsip_message_required(message, "200rel");

	enabled = tsip_message_supported(message, "outbound");
	enabled = tsip_message_supported(message, "path");
	enabled = tsip_message_supported(message, "inbound");

	expires = tsip_message_getExpires(message);
	clength = TSIP_MESSAGE_CONTENT_LENGTH(message);
	
	tsip_message_tostring(message, buffer);
	TSK_DEBUG_INFO("Buffer=\n%s", TSK_BUFFER_TO_STRING(buffer));

	TSIP_MESSAGE_SAFE_FREE(message);
	TSK_BUFFER_SAFE_FREE(buffer);
}


void test_requests()
{
	tsk_buffer_t *buffer = TSK_BUFFER_CREATE_NULL();
	tsip_uri_t *from = tsip_uri_parse("sip:mamadou@micromethod.com", strlen("sip:mamadou@micromethod.com"));
	tsip_uri_t *request_uri = tsip_uri_parse("sip:micromethod.com", strlen("sip:micromethod.com"));
	

	tsip_request_t *request = tsip_request_new("REGISTER", request_uri, from, from, "ABCDEFGHIJKLMOPQRSTUVWXYZ", 4521);
	
	tsip_message_tostring(request, buffer);
	TSK_DEBUG_INFO("Buffer=\n%s", TSK_BUFFER_TO_STRING(buffer));
	

	TSIP_URI_SAFE_FREE(from);
	TSIP_REQUEST_SAFE_FREE(request_uri);
	TSIP_REQUEST_SAFE_FREE(request);
	TSK_BUFFER_SAFE_FREE(buffer);
}

void test_responses()
{

}

void test_messages()
{
	test_parser();
	//test_requests();
}


#endif /* _TEST_SIPMESSAGES_H */