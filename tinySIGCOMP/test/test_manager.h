/*
* Copyright (C) 2009 Mamadou Diop.
* Copyright (C) 2012 Doubango Telecom <http://doubango.org>.
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
#ifndef TEST_TINYSIGCOMP_MANAGER_H
#define TEST_TINYSIGCOMP_MANAGER_H

#define SIGCOMP_TESTS				SIGCOMP_TESTS_CALL

#define COMPARTMENT_ID_CLIENT		"urn:uuid:2e5fdc76-00be-4314-8202-1116fa82a474"
#define COMPARTMENT_ID_SERVER		"urn:uuid:2e5fdc76-00be-4314-8202-1116fa82a475"
#define SIGCOMP_DMS					8192//4096//8192//65536
#define SIGCOMP_SMS					8192//4096//8192//65536
#define SIGCOMP_CPB					64
#define SIGCOMP_LOOP_COUNT			10
#define USE_ONLY_ACKED_STATES		tsk_false // Should be false unless you know what you're doing


typedef struct sigcomp_test_s
{
	const char* description;
	const char* msg;
	tsk_bool_t is_client;
	tsk_bool_t tobe_lost;
}
sigcomp_test_t;

#define IS_CLIENT_YES			tsk_true
#define IS_CLIENT_NO			tsk_false
#define TOBE_LOST_YES			tsk_true
#define TOBE_LOST_NO			tsk_false

static const sigcomp_test_t SIGCOMP_TESTS_CALL[]=
{
	{
		"REGISTER (client -> server)",
		"REGISTER sip:doubango.org SIP/2.0\r\n"
		"Via: SIP/2.0/UDP 192.168.0.11:64549;branch=z9hG4bK357158829;rport;comp=sigcomp;sigcomp-id=\"urn:uuid:d343a50a-2961-4a25-93d8-4e0677b68e6b\"\r\n"
		"From: <sip:001@doubango.org>;tag=357151115\r\n"
		"To: <sip:001@doubango.org>\r\n"
		"Contact: <sip:001@192.168.0.11:64549;transport=udp;sigcomp-id=urn:uuid:d343a50a-2961-4a25-93d8-4e0677b68e6b>;expires=3600;+g.oma.sip-im;language=\"en,fr\";+g.3gpp.smsip;+g.oma.sip-im.large-message;audio;+g.3gpp.icsi-ref=\"urn%3Aurn-7%3A3gpp-application.ims.iari.gsma-vs\";+g.3gpp.cs-voice\r\n"
		"Call-ID: 0473e8ff-2cd6-fada-70ab-ce8c9523db8c\r\n"
		"CSeq: 5493 REGISTER\r\n"
		"Content-Length: 0\r\n"
		"Max-Forwards: 70\r\n"
		"Allow: INVITE, ACK, CANCEL, BYE, MESSAGE, OPTIONS, NOTIFY, PRACK, UPDATE, REFER\r\n"
		"Privacy: none\r\n"
		"P-Access-Network-Info: ADSL;utran-cell-id-3gpp=00000000\r\n"
		"User-Agent: IM-client/OMA1.0 Boghe/v2.0.121.757\r\n"
		"P-Preferred-Identity: <sip:001@doubango.org>\r\n"
		"Supported: path\r\n"
		"\r\n",
		IS_CLIENT_YES,
		TOBE_LOST_NO
	},
	{
		"200 REGISTER (server -> client)",
		"SIP/2.0 200 OK\r\n"
		"Via: SIP/2.0/UDP 192.168.0.11:64549;branch=z9hG4bK357158829;rport=64549;comp=sigcomp;sigcomp-id=\"urn:uuid:d343a50a-2961-4a25-93d8-4e0677b68e6b\"\r\n"
		"Contact: <sip:001@192.168.0.11:54027;transport=udp;sigcomp-id=\"urn:uuid:7556d001-9ca7-41a3-80b0-5b7725a91cc4\">;expires=2761;language=\"en,fr\";+g.oma.sip-im;+g.3gpp.smsip;+g.oma.sip-im.large-message;audio;+g.3gpp.icsi-ref=\"urn%3Aurn-7%3A3gpp-application.ims.iari.gsma-vs\";+g.3gpp.cs-voice\r\n"
		"Contact: <sip:001@192.168.0.11:50417;transport=udp;sigcomp-id=\"urn:uuid:6a8b493e-7dd8-45fb-b104-085723cbfb7b\">;expires=3025;language=\"en,fr\";+g.oma.sip-im;+g.3gpp.smsip;+g.oma.sip-im.large-message;audio;+g.3gpp.icsi-ref=\"urn%3Aurn-7%3A3gpp-application.ims.iari.gsma-vs\";+g.3gpp.cs-voice\r\n"
		"Contact: <sip:001@192.168.0.11:50464;transport=udp;sigcomp-id=\"urn:uuid:c5534343-0fb1-41a8-b4bc-612f04913d2a\">;expires=3409;language=\"en,fr\";+g.oma.sip-im;+g.3gpp.smsip;+g.oma.sip-im.large-message;audio;+g.3gpp.icsi-ref=\"urn%3Aurn-7%3A3gpp-application.ims.iari.gsma-vs\";+g.3gpp.cs-voice\r\n"
		"Contact: <sip:001@192.168.0.11:64547;transport=udp;sigcomp-id=\"urn:uuid:40ce6b6c-90b3-487d-8768-ae180a8153b1\">;expires=3487;language=\"en,fr\";+g.oma.sip-im;+g.3gpp.smsip;+g.oma.sip-im.large-message;audio;+g.3gpp.icsi-ref=\"urn%3Aurn-7%3A3gpp-application.ims.iari.gsma-vs\";+g.3gpp.cs-voice\r\n"
		"Contact: <sip:001@192.168.0.11:64549;transport=udp;sigcomp-id=\"urn:uuid:d343a50a-2961-4a25-93d8-4e0677b68e6b\">;expires=3600;language=\"en,fr\";+g.oma.sip-im;+g.3gpp.smsip;+g.oma.sip-im.large-message;audio;+g.3gpp.icsi-ref=\"urn%3Aurn-7%3A3gpp-application.ims.iari.gsma-vs\";+g.3gpp.cs-voice\r\n"
		"To: <sip:001@doubango.org>;tag=4345c04e\r\n"
		"From: <sip:001@doubango.org>;tag=357151115\r\n"
		"Call-ID: 0473e8ff-2cd6-fada-70ab-ce8c9523db8c\r\n"
		"CSeq: 5493 REGISTER\r\n"
		"Content-Length: 0\r\n"
		"\r\n",
		IS_CLIENT_NO,
		TOBE_LOST_NO
	},
	{
		"INVITE (client -> server)",
		"INVITE sip:13131313@sip2sip.info SIP/2.0\r\n"
		"Via: SIP/2.0/UDP 192.168.0.11:64549;branch=z9hG4bK357179240;rport;comp=sigcomp;sigcomp-id=\"urn:uuid:d343a50a-2961-4a25-93d8-4e0677b68e6b\"\r\n"
		"From: <sip:001@doubango.org>;tag=357203612\r\n"
		"To: <sip:13131313@sip2sip.info>\r\n"
		"Contact: <sip:001@192.168.0.11:64549;transport=udp;sigcomp-id=urn:uuid:d343a50a-2961-4a25-93d8-4e0677b68e6b>;+g.oma.sip-im;language=\"en,fr\";+g.3gpp.icsi-ref=\"urn%3Aurn-7%3A3gpp-service.ims.icsi.mmtel\"\r\n"
		"Call-ID: da91debf-0033-dd9e-9c8d-e9018f825c8f\r\n"
		"CSeq: 14727 INVITE\r\n"
		"Content-Type: application/sdp\r\n"
		"Content-Length: 989\r\n"
		"Max-Forwards: 70\r\n"
		"Accept-Contact: *;+g.3gpp.icsi-ref=\"urn%3Aurn-7%3A3gpp-service.ims.icsi.mmtel\"\r\n"
		"P-Preferred-Service: urn:urn-7:3gpp-service.ims.icsi.mmtel\r\n"
		"Allow: INVITE, ACK, CANCEL, BYE, MESSAGE, OPTIONS, NOTIFY, PRACK, UPDATE, REFER\r\n"
		"Privacy: none\r\n"
		"P-Access-Network-Info: ADSL;utran-cell-id-3gpp=00000000\r\n"
		"User-Agent: IM-client/OMA1.0 Boghe/v2.0.121.757\r\n"
		"Supported: 100rel\r\n"
		"\r\n"
		"v=0\r\n"
		"o=doubango 1983 678901 IN IP4 192.168.0.11\r\n"
		"s=-\r\n"
		"c=IN IP4 192.168.0.11\r\n"
		"t=0 0\r\n"
		"m=audio 64550 RTP/SAVPF 0 8 97 101\r\n"
		"c=IN IP4 192.168.0.11\r\n"
		"a=ptime:20\r\n"
		"a=silenceSupp:off - - - -\r\n"
		"a=rtpmap:0 PCMU/8000/1\r\n"
		"a=rtpmap:8 PCMA/8000/1\r\n"
		"a=rtpmap:97 SPEEX/8000/1\r\n"
		"a=rtpmap:101 telephone-event/8000/1\r\n"
		"a=fmtp:101 0-16\r\n"
		"a=sendrecv\r\n"
		"a=crypto:1 AES_CM_128_HMAC_SHA1_80 inline:JaJaTgwhqSd2A7yQ842YPXASomulb95aX/Wa96T7\r\n"
		"a=crypto:2 AES_CM_128_HMAC_SHA1_32 inline:T3yTUA2dVXTr6v0jb+E1TR3Btj4xP3mPeaAtQbeC\r\n"
		"a=rtcp-mux\r\n"
		"a=ssrc:3301897658 cname:ldjWoB60jbyQlR6e\r\n"
		"a=ssrc:3301897658 mslabel:6994f7d1-6ce9-4fbd-acfd-84e5131ca2e2\r\n"
		"a=ssrc:3301897658 label:Doubango\r\n"
		"a=ice-ufrag:zNevtNKAGk6h7gJ\r\n"
		"a=ice-pwd:YaOV3FlWUkBmifZ87SP51\r\n"
		"a=mid:audio\r\n"
		"a=candidate:CuNeg0Keh 1 udp 2130706431 192.168.0.11 64550 typ host\r\n"
		"a=candidate:CuNeg0Keh 2 udp 2130706430 192.168.0.11 64551 typ host\r\n"
		"a=candidate:srflxCuNe 1 udp 1694498815 88.179.39.5 64550 typ srflx\r\n"
		"a=candidate:srflxCuNe 2 udp 1694498814 88.179.39.5 64551 typ srflx\r\n"
		"\r\n",
		IS_CLIENT_YES,
		TOBE_LOST_NO
	},
	{
		"100 TRYING (server -> client)",
		"SIP/2.0 100 Trying\r\n"
		"Via: SIP/2.0/UDP 192.168.0.11:64549;branch=z9hG4bK357179240;rport=64549;comp=sigcomp;sigcomp-id=\"urn:uuid:d343a50a-2961-4a25-93d8-4e0677b68e6b\"\r\n"
		"To: <sip:13131313@sip2sip.info>\r\n"
		"From: <sip:001@doubango.org>;tag=357203612\r\n"
		"Call-ID: da91debf-0033-dd9e-9c8d-e9018f825c8f\r\n"
		"CSeq: 14727 INVITE\r\n"
		"Content-Length: 0\r\n"
		"\r\n",
		IS_CLIENT_NO,
		TOBE_LOST_NO
	},
	{
		"488 Not acceptable (server -> client)",
		"SIP/2.0 488 Not acceptable here\r\n"
		"Via: SIP/2.0/UDP 192.168.0.11:64549;branch=z9hG4bK357179240;rport=64549;comp=sigcomp;sigcomp-id=\"urn:uuid:d343a50a-2961-4a25-93d8-4e0677b68e6b\"\r\n"
		"To: <sip:13131313@sip2sip.info>;tag=as6edf9ce2\r\n"
		"From: <sip:001@doubango.org>;tag=357203612\r\n"
		"Call-ID: da91debf-0033-dd9e-9c8d-e9018f825c8f\r\n"
		"CSeq: 14727 INVITE\r\n"
		"Allow: INVITE, ACK, CANCEL, OPTIONS, BYE, REFER, SUBSCRIBE, NOTIFY, INFO, PUBLISH\r\n"
		"Server: Asterisk PBX\r\n"
		"Supported: replaces, timer\r\n"
		"Content-Length: 0\r\n"
		"\r\n",
		IS_CLIENT_NO,
		TOBE_LOST_NO
	},
	{
		"ACK (client -> server)",
		"ACK sip:13131313@sip2sip.info SIP/2.0\r\n"
		"Via: SIP/2.0/UDP 192.168.0.11:64549;branch=z9hG4bK357179240;rport;comp=sigcomp;sigcomp-id=\"urn:uuid:d343a50a-2961-4a25-93d8-4e0677b68e6b\"\r\n"
		"From: <sip:001@doubango.org>;tag=357203612\r\n"
		"To: <sip:13131313@sip2sip.info>;tag=as6edf9ce2\r\n"
		"Call-ID: da91debf-0033-dd9e-9c8d-e9018f825c8f\r\n"
		"CSeq: 14727 ACK\r\n"
		"Content-Length: 0\r\n"
		"Max-Forwards: 70\r\n"
		"\r\n",
		IS_CLIENT_YES,
		TOBE_LOST_NO
	}
};

static const sigcomp_test_t SIGCOMP_TESTS_SUBSCRIBE[]=
{
	{
		"REGISTER (client -> server)",
		"REGISTER sip:test.3gpp.com SIP/2.0\r\n"
		"Via: SIP/2.0/TCP 156.141.115.182:62493;branch=z9hG4bK1403432184;rport;comp=sigcomp;sigcomp-id=\"urn:uuid:41147e2b-d2b9-47c1-a698-c732f6f036f4\"\r\n"
		"From: <sip:johndoe@test.3gpp.com>;tag=1403421024\r\n"
		"To: <sip:johndoe@test.3gpp.com>\r\n"
		"Contact: <sip:johndoe@156.141.115.182:62493;transport=tcp;sigcomp-id=urn:uuid:41147e2b-d2b9-47c1-a698-c732f6f036f4>;expires=600000;+g.oma.sip-im;language=\"en,fr\";+g.3gpp.smsip;+g.oma.sip-im.large-message;audio;+g.3gpp.icsi-ref=\"urn%3Aurn-7%3A3gpp-application.ims.iari.gsma-vs\";+g.3gpp.cs-voice\r\n"
		"Call-ID: afac1d20-b998-2720-6407-7bbbc77ace9b\r\n"
		"CSeq: 27335 REGISTER\r\n"
		"Content-Length: 0\r\n"
		"Max-Forwards: 70\r\n"
		"Authorization: Digest username=\"johndoe@test.3gpp.com\",realm=\"test.3gpp.com\",nonce=\"\",uri=\"sip:test.3gpp.com\",response=\"\"\r\n"
		"Allow: INVITE, ACK, CANCEL, BYE, MESSAGE, OPTIONS, NOTIFY, PRACK, UPDATE, REFER\r\n"
		"Privacy: none\r\n"
		"P-Access-Network-Info: ADSL;utran-cell-id-3gpp=00000000\r\n"
		"User-Agent: IM-client/OMA1.0 Boghe/v2.0.132.808\r\n"
		"P-Preferred-Identity: <sip:johndoe@test.3gpp.com>\r\n"
		"Supported: path\r\n"
		"\r\n",
		IS_CLIENT_YES,
		TOBE_LOST_NO
	},
	{
		"200 REGISTER (server -> client)",
		"SIP/2.0 200 OK\r\n"
		"Via: SIP/2.0/TCP 156.141.115.182:62493;branch=z9hG4bK1403432184;rport;comp=sigcomp;sigcomp-id=\"urn:uuid:41147e2b-d2b9-47c1-a698-c732f6f036f4\"\r\n"
		"From: <sip:johndoe@test.3gpp.com>;tag=1403421024\r\n"
		"To: <sip:johndoe@test.3gpp.com>;tag=T91612B6\r\n"
		"Call-ID: afac1d20-b998-2720-6407-7bbbc77ace9b\r\n"
		"CSeq: 27335 REGISTER\r\n"
		"Contact: <sip:johndoe@156.141.115.182:62493;transport=tcp;sigcomp-id=urn:uuid:41147e2b-d2b9-47c1-a698-c732f6f036f4>;q=0.500;expires = 600000;+g.oma.sip-im;language=\"en,fr\";+g.3gpp.smsip;+g.oma.sip-im.large-message;audio;+g.3gpp.icsi-ref=\"urn%3Aurn-7%3A3gpp-application.ims.iari.gsma-vs\";+g.3gpp.cs-voice\r\n"
		"Content-Length: 0\r\n"
		"Date: Fri, 25 Jan 2013 09:20:31 GMT\r\n"
		"P-Associated-URI: <sip:johndoe@test.3gpp.com>\r\n"
		"Server: Agilent_IMS-SIP_Server/4.11.0.0\r\n"
		"\r\n",
		IS_CLIENT_NO,
		TOBE_LOST_NO
	},
	{
		"SUBSCRIBE-1 (client -> server)",
		"SUBSCRIBE sip:johndoe@test.3gpp.com SIP/2.0\r\n"
		"Via: SIP/2.0/TCP 156.141.115.182:62493;branch=z9hG4bK1403431204;rport;comp=sigcomp;sigcomp-id=\"urn:uuid:41147e2b-d2b9-47c1-a698-c732f6f036f4\"\r\n"
		"From: <sip:johndoe@test.3gpp.com>;tag=1403442750\r\n"
		"To: <sip:johndoe@test.3gpp.com>\r\n"
		"Contact: <sip:johndoe@156.141.115.182:62493;transport=tcp;sigcomp-id=urn:uuid:41147e2b-d2b9-47c1-a698-c732f6f036f4>;+g.oma.sip-im;language=\"en,fr\"\r\n"
		"Call-ID: 8c1a2260-c6e7-d415-2492-88493471d6fd\r\n"
		"CSeq: 24466 SUBSCRIBE\r\n"
		"Expires: 600000\r\n"
		"Content-Length: 0\r\n"
		"Max-Forwards: 70\r\n"
		"Event: reg\r\n"
		"Accept: application/reginfo+xml\r\n"
		"Allow-Events: refer, presence, presence.winfo, xcap-diff, conference\r\n"
		"Allow: INVITE, ACK, CANCEL, BYE, MESSAGE, OPTIONS, NOTIFY, PRACK, UPDATE, REFER\r\n"
		"Privacy: none\r\n"
		"P-Access-Network-Info: ADSL;utran-cell-id-3gpp=00000000\r\n"
		"User-Agent: IM-client/OMA1.0 Boghe/v2.0.132.808\r\n"
		"P-Preferred-Identity: <sip:johndoe@test.3gpp.com>\r\n"
		"\r\n",
		IS_CLIENT_YES,
		TOBE_LOST_NO
	},
	{
		"SUBSCRIBE-2 (client -> server)",
		"SUBSCRIBE sip:johndoe@test.3gpp.com SIP/2.0\r\n"
		"Via: SIP/2.0/TCP 156.141.115.182:62493;branch=z9hG4bK1403422374;rport;comp=sigcomp;sigcomp-id=\"urn:uuid:41147e2b-d2b9-47c1-a698-c732f6f036f4\"\r\n"
		"From: <sip:johndoe@test.3gpp.com>;tag=1403431575\r\n"
		"To: <sip:johndoe@test.3gpp.com>\r\n"
		"Contact: <sip:johndoe@156.141.115.182:62493;transport=tcp;sigcomp-id=urn:uuid:41147e2b-d2b9-47c1-a698-c732f6f036f4>;+g.oma.sip-im;language=\"en,fr\"\r\n"
		"Call-ID: 05952487-8f39-ddbd-ebac-72a3f33eaf8d\r\n"
		"CSeq: 32393 SUBSCRIBE\r\n"
		"Expires: 600000\r\n"
		"Content-Length: 0\r\n"
		"Max-Forwards: 70\r\n"
		"Event: message-summary\r\n"
		"Accept: application/simple-message-summary\r\n"
		"Allow-Events: refer, presence, presence.winfo, xcap-diff, conference\r\n"
		"Allow: INVITE, ACK, CANCEL, BYE, MESSAGE, OPTIONS, NOTIFY, PRACK, UPDATE, REFER\r\n"
		"Privacy: none\r\n"
		"P-Access-Network-Info: ADSL;utran-cell-id-3gpp=00000000\r\n"
		"User-Agent: IM-client/OMA1.0 Boghe/v2.0.132.808\r\n"
		"P-Preferred-Identity: <sip:johndoe@test.3gpp.com>\r\n"
		"\r\n",
		IS_CLIENT_YES,
		TOBE_LOST_NO
	},
};




static int handle_nack(tcomp_result_t *nack, tcomp_manager_handle_t* manager)
{
	if(nack->isNack){
		tsk_size_t nack_info_size;
		if(nack->nack_info && (nack_info_size = tcomp_buffer_getSize(nack->nack_info))){
			char buff[MAX_BUFFER_SIZE];
			tcomp_result_t* result = tcomp_result_create();
			tcomp_result_setOutputBuffer(result, buff, sizeof(buff), IS_STREAM, STREAM_ID);
			tcomp_manager_decompress(manager, tcomp_buffer_getBuffer(nack->nack_info), nack_info_size, result);
			TSK_OBJECT_SAFE_FREE(result);
			return 0;
		}
	}
	return -1;
}

static int test_manager()
{
	size_t i, j;
	size_t outLen;
	const sigcomp_test_t* test;

	tcomp_manager_handle_t *client = tsk_null, *server = tsk_null;
	tcomp_result_t *result_client = tsk_null, *result_server = tsk_null;

	/* temporary buffers  --> will hold compression/decompression results */
	char buff_client[MAX_BUFFER_SIZE]; 
	char buff_server[MAX_BUFFER_SIZE];
	
	/* Managers */
	client = tcomp_manager_create();
	server = tcomp_manager_create();

	/* Add SIP/Presence dictionnaries */
	tcomp_manager_addSipSdpDictionary(client);
	tcomp_manager_addPresenceDictionary(client);
	tcomp_manager_addSipSdpDictionary(server);
	tcomp_manager_addPresenceDictionary(server);
	
	/* Results --> it is recomanded to use one result struct for each manager */
	result_client = tcomp_result_create();
	result_server = tcomp_result_create();

	/* Sets compartment Ids */
	tcomp_result_setCompartmentId(result_client, COMPARTMENT_ID_CLIENT, strlen(COMPARTMENT_ID_CLIENT));
	tcomp_result_setCompartmentId(result_server, COMPARTMENT_ID_SERVER, strlen(COMPARTMENT_ID_SERVER));

	/* Set DMS and SMS */
	tcomp_manager_setDecompression_Memory_Size(client, SIGCOMP_DMS);
	tcomp_manager_setDecompression_Memory_Size(server, SIGCOMP_DMS);
	tcomp_manager_setCycles_Per_Bit(client, SIGCOMP_CPB);
	tcomp_manager_setCycles_Per_Bit(server, SIGCOMP_CPB);
	tcomp_manager_setState_Memory_Size(client, SIGCOMP_SMS);
	tcomp_manager_setState_Memory_Size(server, SIGCOMP_SMS);

	/* Set wehther to use only ACKed states (should be false) */
	tcomp_manager_setUseOnlyACKedStates(client, USE_ONLY_ACKED_STATES);
	tcomp_manager_setUseOnlyACKedStates(server, USE_ONLY_ACKED_STATES);

	tcomp_result_setOutputBuffer(result_client, buff_client, sizeof(buff_client), IS_STREAM, STREAM_ID);
	tcomp_result_setOutputBuffer(result_server, buff_server, sizeof(buff_server), IS_STREAM, STREAM_ID);
	
	for(j = 0; j < SIGCOMP_LOOP_COUNT; ++j)
	{
		for(i = 0; i < sizeof(SIGCOMP_TESTS)/sizeof(SIGCOMP_TESTS[0]); ++i)
		{
			test = &SIGCOMP_TESTS[i];

			// compress
			outLen = tcomp_manager_compress(
				test->is_client ? client : server, 
				test->is_client ? COMPARTMENT_ID_CLIENT : COMPARTMENT_ID_SERVER, 
				tsk_strlen((test->is_client ? COMPARTMENT_ID_CLIENT : COMPARTMENT_ID_SERVER)), 
				test->msg, 
				tsk_strlen(test->msg), 
				test->is_client ? buff_client : buff_server, 
				test->is_client ? sizeof(buff_client) : sizeof(buff_server), 
				IS_STREAM);
			if(!outLen)
			{
				TSK_DEBUG_ERROR("Failed to compress %s message", test->description);
				goto bail;
			}
			TSK_DEBUG_INFO("==%s compression ratio = %f%%==", test->description, ((float)tsk_strlen(test->msg) / (float)outLen) * 100.f);

			// decompress
			if(!test->tobe_lost)
			{
				tsk_bool_t server_decompress = test->is_client; // decompress using server manager if compressed using client mgr
				memset(server_decompress ? buff_server : buff_client, '\0', server_decompress ? sizeof(buff_server) : sizeof(buff_client));
				outLen = tcomp_manager_decompress(server_decompress ? server : client, server_decompress ? buff_client : buff_server, outLen, server_decompress ? result_server : result_client);
				if(!outLen)
				{
					TSK_DEBUG_ERROR("Failed to decompress %s message", test->description);
					// pass NACK to remote peer (sendto())
					if(handle_nack(server_decompress ? result_server : result_client, server_decompress ? client : server) != 0){
						goto bail;
					}
				}
				tcomp_manager_provideCompartmentId(server_decompress ? server : client, server_decompress ? result_server : result_client); // save states
				TSK_DEBUG_INFO("Decompressed message:\n%s = %s", test->description, server_decompress ? buff_server : buff_client);
			}
		}// for (i < count(SIGCOMP_TESTS))
	} // for (j < SIGCOMP_LOOP_COUNT)

bail:
	// Close compartments
	tcomp_manager_closeCompartment(client, COMPARTMENT_ID_CLIENT, tsk_strlen(COMPARTMENT_ID_CLIENT));
	tcomp_manager_closeCompartment(server, COMPARTMENT_ID_SERVER, tsk_strlen(COMPARTMENT_ID_SERVER));

	// Delete Results
	TSK_OBJECT_SAFE_FREE(result_client);
	TSK_OBJECT_SAFE_FREE(result_server);

	// Delete managers
	TSK_OBJECT_SAFE_FREE(client);
	TSK_OBJECT_SAFE_FREE(server);

	return 0;
}

#endif /* TEST_TINYSIGCOMP_MANAGER_H */
