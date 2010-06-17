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
* GNU General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/
#ifndef _TEST_SIPMESSAGES_H
#define _TEST_SIPMESSAGES_H

#define SIP_REQUEST \
	"REGISTER sip:open-ims.test SIP/2.0\r\n" \
	"Test-Header: 0\r\n" \
	"v: SIP/2.0/UDP [::]:1988;test=1234;comp=sigcomp;rport=254;ttl=457;received=192.0.2.101;branch=z9hG4bK1245420841406\r\n" \
	"f: \"Mamadou\" <sip:mamadou@open-ims.test>;tag=29358\r\n" \
	"t:    <sip:mamadou@open-ims.test>;tag= 12345\r\n" \
	"i: M-fa53180346f7f55ceb8d8670f9223dbb\r\n" \
	"CSeq: 201 REGISTER\r\n" \
	"Max-Forwards: 70\r\n" \
	"Allow: INVITE, ACK, CANCEL, BYE, MESSAGE, OPTIONS, NOTIFY, PRACK\r\n" \
	"Allow: REFER, UPDATE\r\n" \
	"u: talk, hold, conference, LocalModeStatus\r\n" \
	"m: <sip:mamadou@[::]:1988;comp=sigcomp;transport=udp>;expires=600000;+deviceID=\"3ca50bcb-7a67-44f1-afd0-994a55f930f4\";mobility=\"fixed\";+g.3gpp.cs-voice;+g.3gpp.app%5fref=\"urn%3Aurnxxx%3A3gpp-application.ims.iari.gsmais\";+g.oma.sip-im.large-message;+g.oma.sip-im\r\n" \
	"User-Agent: IM-client/OMA1.0 doubango/v0.0.0\r\n" \
	"Require: pref, path\r\n" \
	"Service-Route: <sip:orig@open-ims.test:6060;lr>,<sip:orig2@open-ims.test:6060;lr>\r\n" \
	"Path: <sip:term@open-ims.test:4060;lr>\r\n" \
	"Require: 100rel\r\n" \
	"P-Preferred-Identity: <sip:mamadou@open-ims.test>\r\n" \
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
	"To: <sip:mamadou@open-ims.test>;tag=bweyal\r\n" \
	"Via: SIP/2.0/UDP 192.168.0.11:63140;branch=z9hG4bK1261611942868;rport=63140\r\n" \
	"CSeq: 31516 REGISTER\r\n" \
	"Content-Length: 0\r\n" \
	"Call-ID: 1261611941121\r\n" \
	"Min-Expires: 30\r\n" \
	"Event: reg\r\n" \
	"From: <sip:mamadou@open-ims.test>;tag=1261611941121\r\n" \
	"Contact: <sip:mamadou@192.168.0.12:58827;transport=udp>;mobility=fixed;+deviceid=\"DD1289FA-C3D7-47bd-A40D-F1F1B2CC5FFC\";expires=300,<sip:mamadou@192.168.0.12:58828;transport=udp>;mobility=fixed;+deviceid=\"DD1289FA-C3D7-47bd-A40D-F1F1B2CC5FFC\";expires=300,<sip:mamadou@192.168.0.12:58829;transport=udp>;mobility=fixed;+deviceid=\"DD1289FA-C3D7-47bd-A40D-F1F1B2CC5FFC\";expires=300\r\n"\
	"Contact: <sip:mamadou@192.168.0.11:63140>;expires=3600;q=1.0,<sip:mamadou@192.168.0.11:56717>;expires=3600;q=1.0\r\n" \
	"Contact: <sip:mamadou@127.0.0.1:5060>;expires=3600;q=1.0\r\n" \
	"Contact: <sip:mamadou@127.0.0.1>;expires=3600;q=1.0\r\n" \
	"P-Preferred-Identity: <sip:mamadou@open-ims.test>\r\n" \
	"Service-Route: <sip:orig@open-ims.test:6060;lr><sip:orig2@open-ims.test:6060;lr>,<sip:orig3@open-ims.test:6060;lr>\r\n" \
	"Path: <sip:term@open-ims.test:4060;lr>\r\n" \
	"P-Access-Network-Info: 3GPP-UTRAN-TDD;utran-cell-id-3gpp=00000000\r\n" \
	"Authorization: Digest username=\"Alice\", realm=\"atlanta.com\",nonce=\"84a4cc6f3082121f32b42a2187831a9e\",response=\"7587245234b3434cc3412213e5f113a5432,test=123\"\r\n" \
	"Privacy: none;user;id\r\n" \
	"Proxy-Authenticate: Digest realm=\"atlanta.com\",domain=\"sip:ss1.carrier.com\",qop=\"auth,auth-int\",nonce=\"f84f1cec41e6cbe5aea9c8e88d359\",opaque=\"\", stale=FALSE, algorithm=MD5,test=124\r\n" \
	"Authorization: Digest username=\"bob\", realm=\"atlanta.example.com\",nonce=\"ea9c8e88df84f1cec4341ae6cbe5a359\", opaque=\"\",uri=\"sips:ss2.biloxi.example.com\",test=\"7854\",response=\"dfe56131d1958046689d83306477ecc\"\r\n" \
	"Proxy-Authorization: Digest username=\"Alice\", test=666,realm=\"atlanta.com\",nonce=\"c60f3082ee1212b402a21831ae\",response=\"245f23415f11432b3434341c022\"\r\n" \
	"WWW-Authenticate: Digest realm=\"atlanta.com\",domain=\"sip:boxesbybob.com\", qop=\"auth\",nonce=\"f84f1cec41e6cbe5aea9c8e88d359\",opaque=\"\",stale=FALSE,algorithm=MD5,test=\"3\"\r\n" \
	"l: 0\r\n" \
	"Subscription-State: active;reason=deactivated;expires=507099;retry-after=145;test=jk\r\n"\
	"\r\n"

	//"Via: SIP/2.0/UDP 192.168.0.11:64163;rport=4;branch=z9hG4bK1262758946486\r\n" \
	//"Via: SIP/2.0/UDP 192.168.0.11:59265;rport=59265;branch=z9hG4bK1263064096664\r\n" \

#define SIP_MESSAGE \
	"MESSAGE sip:mamadou@open-ims.test SIP/2.0\r\n" \
	"Via: SIP/2.0/tcp 127.0.0.1:5082;branch=z9hG4bKc16be5aee32df400d01015675ab911ba,SIP/2.0/udp 127.0.0.1:5082;branch=z9hG4bKeec53b25db240bec92ea250964b8c1fa;received_port_ext=5081;received=192.168.0.13,SIP/2.0/UDP 192.168.0.12:57121;rport=57121;branch=z9hG4bK1274980921982;received_port_ext=5081;received=192.168.0.12\r\n" \
	"From: \"Bob\"<sip:bob@open-ims.test>;tag=mercuro\r\n" \
	"To: \"Alice\"<sip:alice@open-ims.test>\r\n" \
	"m: <sip:mamadou@127.0.0.1:5060>\r\n" \
	"Call-ID: 1262767804423\r\n" \
	"CSeq: 8 MESSAGE\r\n" \
	"Refer-To: <sips:a8342043f@atlanta.example.com?Replaces=12345601%40atlanta.example.com%3Bfrom-tag%3D314159%3Bto-tag%3D1234567>\r\n" \
	"Refer-To: sip:conf44@example.com;isfocus\r\n" \
	"Referred-By: <sip:referrer@referrer.example>;cid=\"20398823.2UWQFN309shb3@referrer.example\"\r\n" \
	"Refer-Sub: false;test=45;op\r\n" \
	"Refer-Sub: true;p\r\n" \
	"RSeq: 17422\r\n" \
	"RAck: 776656 1 INVITE\r\n" \
	"Min-SE: 90;test;y=0\r\n" \
	"Session-Expires: 95;refresher=uas;y=4\r\n" \
	"x: 95;refresher=uac;o=7;k\r\n" \
	"Max-Forwards: 70\r\n" \
	"Date: Wed, 28 Apr 2010 23:42:50 GMT\r\n" \
	"Date: Sun, 2 May 2010 20:27:49 GMT\r\n" \
	"Allow: INVITE, ACK, CANCEL, BYE, MESSAGE, OPTIONS, NOTIFY, PRACK, UPDATE, REFER\r\n" \
	"User-Agent: IM-client/OMA1.0 Mercuro-Bronze/v4.0.1508.0\r\n" \
	"c: text/plain; charset=utf-8\r\n" \
	"Security-Client: ipsec-3gpp;alg=hmac-md5-96;ealg=aes-cbc;prot=esp;mod=trans;port-c=61676;port-s=61662;spi-c=4294967295;spi-s=67890,tls;q=0.2\r\n" \
	"Security-Client: ipsec-ike;q=0.1,tls;q=0.2;test=123\r\n" \
	"Security-Server: ipsec-ike;q=0.1,ipsec-3gpp;alg=hmac-md5-96;prot=esp;mod=trans;ealg=aes-cbc;spi-c=5000;spi-s=5001;port-c=78952;port-s=77854\r\n" \
	"Security-Verify: ipsec-3gpp;alg=hmac-md5-96;prot=esp;mod=trans;ealg=aes-cbc;spi-c=5000;spi-s=5001;port-c=9999;port-s=20000,ipsec-ike;q=0.1;test=458;toto\r\n" \
	"Service-Route: <sip:orig@open-ims.test:6060;lr;transport=udp>,<sip:atlanta.com>,\"Originating\" <sip:orig2@open-ims.test:6060;lr>\r\n" \
	"Path: <sip:term@open-ims.test:4060;lr>\r\n" \
	"Route: \"Prox-CSCF\" <sip:pcscf.open-ims.test:4060;lr;transport=udp>;test=1,\"Originating\" <sip:orig@scscf.open-ims.test:6060;lr>\r\n" \
	"Record-Route: <sip:mo@pcscf.ims.inexbee.com:4060;lr>,\"Originating\"<sip:pcscf.open-ims.test:4060;lr;transport=udp>;test=2\r\n" \
	"P-Preferred-Identity: <sip:bob@open-ims.test\r\n" \
	"Allow-Events: presence, presence.winfo\r\n" \
	"Event: reg\r\n" \
	"P-Associated-URI: <sip:bob@open-ims.test>, <sip:0600000001@open-ims.test>, <sip:0100000001@open-ims.test>\r\n" \
	"P-Charging-Function-Addresses: ccf=pri_ccf_address\r\n" \
	"Server: Sip EXpress router (2.0.0-dev1 OpenIMSCore (i386/linux))\r\n" \
	"Warning: 392 192.168.0.15:6060 \"Noisy feedback tells:  pid=4521 req_src_ip=192.168.0.15 req_src_port=5060 in_uri=sip:scscf.open-ims.test:6060 out_uri=sip:scscf.open-ims.test:6060 via_cnt==3\"\r\n" \
	"P-Asserted-Identity: \"Cullen Jennings\" <sip:fluffy@cisco.com>\r\n" \
	"P-Asserted-Identity: tel:+14085264000\r\n" \
	"WWW-Authenticate: Digest realm=\"ims.inexbee.com\", nonce=\"iTaxDEv2uO8sKxzVVaRy6IkU9Lra6wAA2xv4BrmCzvY=\", algorithm=AKAv1-MD5, qop=\"auth\"\r\n" \
	"Content-Length: 11\r\n" \
	"\r\n" \
	"How are you"


#define SIP_MSG_2_TEST SIP_MESSAGE

void test_parser()
{
	tsk_ragel_state_t state;
	tsip_message_t *message = tsk_null;
	tsk_buffer_t *buffer = tsk_buffer_create_null();
	tsk_bool_t enabled;
	int64_t expires;
	uint32_t clength;

	tsk_ragel_state_init(&state, SIP_MSG_2_TEST, tsk_strlen(SIP_MSG_2_TEST));
	tsip_message_parse(&state, &message, tsk_true);

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

	/* Add new headers */
	TSIP_MESSAGE_ADD_HEADER(message, TSIP_HEADER_DUMMY_VA_ARGS("MyHeader1", "Value1; test=123;m"));
	TSIP_MESSAGE_ADD_HEADER(message, TSIP_HEADER_DUMMY_VA_ARGS("MyHeader2", "Value2"));
	
	tsip_message_tostring(message, buffer);
	TSK_DEBUG_INFO("Buffer=\n%s", TSK_BUFFER_TO_STRING(buffer));

	TSK_OBJECT_SAFE_FREE(message);
	TSK_OBJECT_SAFE_FREE(buffer);

	//getchar();
}


void test_requests()
{
	tsk_buffer_t *buffer = tsk_buffer_create_null();
	tsip_uri_t *from = tsip_uri_parse("sip:mamadou@open-ims.test", tsk_strlen("sip:mamadou@open-ims.test"));
	tsip_uri_t *request_uri = tsip_uri_parse("sip:open-ims.test", tsk_strlen("sip:open-ims.test"));
	

	tsip_request_t *request = tsip_request_new("REGISTER", request_uri, from, from, "ABCDEFGHIJKLMOPQRSTUVWXYZ", 4521);
	
	tsip_message_tostring(request, buffer);
	TSK_DEBUG_INFO("Buffer=\n%s", TSK_BUFFER_TO_STRING(buffer));
	

	TSK_OBJECT_SAFE_FREE(from);
	TSK_OBJECT_SAFE_FREE(request_uri);
	TSK_OBJECT_SAFE_FREE(request);
	TSK_OBJECT_SAFE_FREE(buffer);
}

void test_responses()
{
	tsk_ragel_state_t state;
	tsip_request_t *request = 0;
	tsip_request_t *response = 0;

	tsk_ragel_state_init(&state, SIP_MESSAGE, tsk_strlen(SIP_MESSAGE));
	tsip_message_parse(&state, &request, tsk_true);

	/* Create the response and destroy the request */
	response = tsip_response_new(200, "OK you can move forward", request);
	TSK_OBJECT_SAFE_FREE(request);

	{
		/* DUMP the response */
		tsk_buffer_t *buffer = tsk_buffer_create_null();

		tsip_message_tostring(response, buffer);
		TSK_DEBUG_INFO("Response=\n%s", TSK_BUFFER_TO_STRING(buffer));

		TSK_OBJECT_SAFE_FREE(buffer);
	}
	

	TSK_OBJECT_SAFE_FREE(response);
}

void test_messages()
{
	test_parser();
	test_requests();
	test_responses();
}


#endif /* _TEST_SIPMESSAGES_H */
