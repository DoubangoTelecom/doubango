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

#define COMPARTMENT_ID1		"urn:uuid:2e5fdc76-00be-4314-8202-1116fa82a474"	// My first compartment id
#define COMPARTMENT_ID2		"urn:uuid:2e5fdc76-00be-4314-8202-1116fa82a475"	// My second compartment id
#define SIGCOMP_DMS			8192//4096//8192//65536
#define SIGCOMP_SMS			8192//4096//8192//65536
#define SIGCOMP_CPB			64
#define SIGCOMP_LOOP_COUNT	10

#if 0

#define DECOMP_NACK_4_TEST	0

// messages to use for tests
const char* messages[] =
{
	//{ 
		"REGISTER sip:ims-network.com SIP/2.0\r\n"
		"Via: SIP/2.0/UDP [::]:1988;comp=sigcomp;rport;branch=z9hG4bK1245420841406\r\n"
		"From: <sip:mamadou@ims-network.com>;tag=29358\r\n"
		"To: <sip:mamadou@ims-network.com>\r\n"
		"Call-ID: M-fa53180346f7f55ceb8d8670f9223dbb\r\n"
		"CSeq: 201 REGISTER\r\n"
		"Max-Forwards: 70\r\n"
		"Allow: INVITE, ACK, CANCEL, BYE, MESSAGE, OPTIONS, NOTIFY, PRACK, UPDATE, REFER\r\n"
		"Contact: <sip:mamadou@[::]:1988;comp=sigcomp;transport=udp>;expires=600000;+deviceID=\"3ca50bcb-7a67-44f1-afd0-994a55f930f4\";mobility=\"fixed\";+g.3gpp.cs-voice;+g.3gpp.app%5fref=\"urn%3Aurnxxx%3A3gpp-application.ims.iari.gsmais\";+g.oma.sip-im.large-message;+g.oma.sip-im\r\n"
		"User-Agent: IM-client/OMA1.0 doubango/v4.0.1390.0\r\n"
		"Require: pref\r\n"
		"P-Preferred-Identity: <sip:mamadou@ims-network.com>\r\n"
		"Supported: path\r\n"
		"P-Access-Network-Info: 3GPP-UTRAN-TDD;utran-cell-id-3gpp=00000000\r\n"
		"Privacy: none\r\n"
		"Supported: timer\r\n"
		"Content-Length: 0\r\n"
		"\r\n"
	//}
	,
	//{ 
		"SIP/2.0 401 Unauthorized - Challenging the UE\r\n"
		"Via: SIP/2.0/UDP [::]:1988;comp=sigcomp;received=2001:5C0:1502:1800:1D41:BF77:F1BF:BB49;rport=1988;branch=z9hG4bK1245420841406\r\n"
		"From: <sip:mamadou@ims-network.com>;tag=29358\r\n"
		"To: <sip:mamadou@ims-network.com>;tag=3241f316c9eb68efd2c34668c4fbf834-eaa0\r\n"
		"Call-ID: M-fa53180346f7f55ceb8d8670f9223dbb\r\n"
		"CSeq: 201 REGISTER\r\n"
		"Path: <sip:term@pcscf.ims-network.com:4060;lr>\r\n"
		"Service-Route: <sip:orig@scscf.ims-network.com:6060;lr>"
		"Allow: INVITE, ACK, CANCEL, OPTIONS, BYE, REFER, SUBSCRIBE, NOTIFY, PUBLISH, MESSAGE, INFO\r\n"
		"Server: Sip EXpress router (2.1.0-dev1 OpenIMSCore (i386/linux))\r\n"
		"Content-Length: 0\r\n"
		"Warning: 392 2001:5C0:1502:1800:0:0:0:226:6060 \"Noisy feedback tells:  pid=24454 req_src_ip=2001:5C0:1502:1800:0:0:0:226 req_src_port=5060 in_uri=sip:scscf.ims-network.com:6060 out_uri=sip:scscf.ims-network.com:6060 via_cnt==3\"\r\n"
		"WWW-Authenticate: Digest realm=\"ims-network.com\", nonce=\"xFBhTyFaQ0/lBgboH2ZqDe3BDmFXDwAA2Peq/bxtLQs=\", algorithm=AKAv1-MD5, qop=\"auth,auth-int\"\r\n"
		"\r\n"

	//}
		,
	//{ 
		"REGISTER sip:ims-network.com SIP/2.0\r\n"
		"Via: SIP/2.0/UDP [::]:1988;comp=sigcomp;rport;branch=z9hG4bK1245420841407\r\n"
		"From: <sip:mamadou@ims-network.com>;tag=29358\r\n"
		"To: <sip:mamadou@ims-network.com>\r\n"
		"Call-ID: M-fa53180346f7f55ceb8d8670f9223dbb\r\n"
		"CSeq: 202 REGISTER\r\n"
		"Max-Forwards: 70\r\n"
		"Allow: INVITE, ACK, CANCEL, BYE, MESSAGE, OPTIONS, NOTIFY, PRACK, UPDATE, REFER\r\n"
		"Contact: <sip:mamadou@[::]:1988;comp=sigcomp;transport=udp>;expires=600000;+deviceID=\"3ca50bcb-7a67-44f1-afd0-994a55f930f4\";mobility=\"fixed\";+g.3gpp.cs-voice;+g.3gpp.app%5fref=\"urn%3Aurnxxx%3A3gpp-application.ims.iari.gsmais\";+g.oma.sip-im.large-message;+g.oma.sip-im\r\n"
		"User-Agent: IM-client/OMA1.0 doubango/v4.0.1390.0\r\n"
		"Authorization: Digest algorithm=AKAv1-MD5,username=\"mamadou@ims-network.com\",realm=\"ims-network.com\",nonce=\"xFBhTyFaQ0/lBgboH2ZqDe3BDmFXDwAA2Peq/bxtLQs=\",uri=\"sip:ims-network.com\",response=\"c499a6b49693d5b29c431786cff32ca4\",qop=auth-int,cnonce=\"9fcc19edace2d1beaa6122b86dd11256\",nc=00000001\r\n"
		"Require: pref\r\n"
		"P-Preferred-Identity: <sip:mamadou@ims-network.com>\r\n"
		"Supported: path\r\n"
		"P-Access-Network-Info: 3GPP-UTRAN-TDD;utran-cell-id-3gpp=00000000\r\n"
		"Privacy: none\r\n"
		"Supported: timer\r\n"
		"Content-Length: 0\r\n"
		"\r\n"
	//}
		,
	//{ 
		"SIP/2.0 200 OK - SAR succesful and registrar saved\r\n"
		"Via: SIP/2.0/UDP [::]:1988;comp=sigcomp;received=2001:5C0:1502:1800:1D41:BF77:F1BF:BB49;rport=1988;branch=z9hG4bK1245420841407\r\n"
		"From: <sip:mamadou@ims-network.com>;tag=29358\r\n"
		"To: <sip:mamadou@ims-network.com>;tag=3241f316c9eb68efd2c34668c4fbf834-1b36\r\n"
		"Call-ID: M-fa53180346f7f55ceb8d8670f9223dbb\r\n"
		"CSeq: 202 REGISTER\r\n"
		"P-Associated-URI: <sip:mamadou@ims-network.com>\r\n"
		"Contact: <sip:mamadou@[::]:1988;comp=sigcomp;transport=udp>;expires=600000\r\n"
		"Path: <sip:term@pcscf.ims-network.com:4060;lr>\r\n"
		"Service-Route: <sip:orig@scscf.ims-network.com:6060;lr>"
		"Allow: INVITE, ACK, CANCEL, OPTIONS, BYE, REFER, SUBSCRIBE, NOTIFY, PUBLISH, MESSAGE, INFO\r\n"
		"P-Charging-Function-Addresses: ccf=pri_ccf_address\r\n"
		"Server: Sip EXpress router (2.1.0-dev1 OpenIMSCore (i386/linux))\r\n"
		"Content-Length: 0\r\n"
		"Warning: 392 2001:5C0:1502:1800:0:0:0:226:6060 \"Noisy feedback tells:  pid=24452 req_src_ip=2001:5C0:1502:1800:0:0:0:226 req_src_port=5060 in_uri=sip:scscf.ims-network.com:6060 out_uri=sip:scscf.ims-network.com:6060 via_cnt==3\"\r\n"
		"\r\n"
	//}
		,
	//{ 
		"SUBSCRIBE sip:mamadou@ims-network.com SIP/2.0\r\n"
		"Via: SIP/2.0/UDP [::]:1988;comp=sigcomp;rport;branch=z9hG4bK1245420841408\r\n"
		"From: <sip:mamadou@ims-network.com>;tag=5705\r\n"
		"To: <sip:mamadou@ims-network.com>\r\n"
		"Call-ID: M-dd6e227ce416f853ca7bca49ad5b676d\r\n"
		"CSeq: 301 SUBSCRIBE\r\n"
		"Max-Forwards: 70\r\n"
		"Allow: INVITE, ACK, CANCEL, BYE, MESSAGE, OPTIONS, NOTIFY, PRACK, UPDATE, REFER\r\n"
		"Contact: <sip:mamadou@[::]:1988;comp=sigcomp;transport=udp>;+g.oma.sip-im\r\n"
		"User-Agent: IM-client/OMA1.0 doubango/v4.0.1390.0\r\n"
		"Expires: 600000\r\n"
		"Event: reg\r\n"
		"Accept: application/reginfo+xml\r\n"
		"Route: <sip:[2001:5c0:1502:1800::226]:4060;lr=true;transport=udp>,<sip:orig@scscf.ims-network.com:6060;lr>\r\n"
		"P-Access-Network-Info: 3GPP-UTRAN-TDD;utran-cell-id-3gpp=00000000\r\n"
		"Privacy: none\r\n"
		"Supported: timer\r\n"
		"Allow-Events: presence, presence.winfo, conference\r\n"
		"Content-Length: 0\r\n"
		"\r\n"
	//}
		,
	//{ 
		"SIP/2.0 200 Subscription to REG saved\r\n"
		"Record-Route: <sip:mo@pcscf.ims-network.com:4060;lr>\r\n"
		"Via: SIP/2.0/UDP [::]:1988;comp=sigcomp;received=2001:5C0:1502:1800:1D41:BF77:F1BF:BB49;rport=1988;branch=z9hG4bK1245420841408\r\n"
		"From: <sip:mamadou@ims-network.com>;tag=5705\r\n"
		"To: <sip:mamadou@ims-network.com>;tag=3241f316c9eb68efd2c34668c4fbf834-5cce\r\n"
		"Call-ID: M-dd6e227ce416f853ca7bca49ad5b676d\r\n"
		"CSeq: 301 SUBSCRIBE\r\n"
		"Expires: 600000\r\n"
		"Contact: <sip:mamadou@ims-network.com;comp=sigcomp;>\r\n"
		"Server: Sip EXpress router (2.1.0-dev1 OpenIMSCore (i386/linux))\r\n"
		"Content-Length: 0\r\n"
		"Warning: 392 2001:5C0:1502:1800:0:0:0:226:6060 \"Noisy feedback tells:  pid=24454 req_src_ip=2001:5C0:1502:1800:0:0:0:226 req_src_port=4060 in_uri=sip:mamadou@ims-network.com out_uri=sip:mamadou@ims-network.com via_cnt==2\"\r\n"
		"\r\n"
	//}
		,
	//{ 
		"NOTIFY sip:mamadou@[::]:1988;transport=udp SIP/2.0\r\n"
		"Via: SIP/2.0/UDP [2001:5C0:1502:1800:0:0:0:226]:4060;comp=sigcomp;branch=z9hG4bK2b3f.38818b91.0\r\n"
		"Via: SIP/2.0/UDP [2001:5C0:1502:1800:0:0:0:226]:6060;received=2001:5C0:1502:1800:0:0:0:226;rport=6060;branch=z9hG4bK2b3f.6db77cf1.0\r\n"
		"To: <sip:mamadou@ims-network.com>;tag=5705\r\n"
		"From: <sip:mamadou@ims-network.com>;tag=3241f316c9eb68efd2c34668c4fbf834-5cce\r\n"
		"CSeq: 10 NOTIFY\r\n"
		"Call-ID: M-dd6e227ce416f853ca7bca49ad5b676d\r\n"
		"Content-Length: 379\r\n"
		"User-Agent: Sip EXpress router(2.1.0-dev1 OpenIMSCore (i386/linux))\r\n"
		"Contact: <sip:mamadou@ims-network.com; comp=sigcomp;>\r\n"
		"Event: reg\r\n"
		"Max-Forwards: 16\r\n"
		"Subscription-State: active;expires=600030\r\n"
		"Content-Type: application/reginfo+xml\r\n"
		"\r\n"
		"<?xml version=\"1.0\"?>\r\n"
		"<reginfo xmlns=\"urn:ietf:params:xml:ns:reginfo\" version=\"0\" state=\"full\">\r\n"
		"<registration aor=\"sip:mamadou@ims-network.com\" id=\"0xb5d91fcc\" state=\"active\">\r\n"
		"<contact id=\"0xb5d8fb98\" state=\"active\" event=\"registered\" expires=\"600000\">\r\n"
		"<uri>sip:mamadou@[::]:1988;transport=udp</uri>\r\n"
		"</contact>\r\n"
		"</registration>\r\n"
		"</reginfo>\r\n"
	//}
	,
	//{
		"SIP/2.0 200 OK\r\n"
		"Max-Forwards: 70\r\n"
		"User-Agent: IM-client/OMA1.0 doubango/v4.0.1390.0\r\n"
		"Via: SIP/2.0/UDP [2001:5C0:1502:1800:0:0:0:226]:4060;comp=sigcomp;branch=z9hG4bK2b3f.38818b91.0;received=2001:5c0:1502:1800::226\r\n"
		"Via: SIP/2.0/UDP [2001:5C0:1502:1800:0:0:0:226]:6060;received=2001:5C0:1502:1800:0:0:0:226;rport=6060;branch=z9hG4bK2b3f.6db77cf1.0\r\n"
		"To: <sip:mamadou@ims-network.com>;tag=5705\r\n"
		"From: <sip:mamadou@ims-network.com>;tag=3241f316c9eb68efd2c34668c4fbf834-5cce\r\n"
		"CSeq: 10 NOTIFY\r\n"
		"Call-ID: M-dd6e227ce416f853ca7bca49ad5b676d\r\n"
		"Content-Length: 0\r\n"
		"\r\n"
	//}
};

static int test_manager()
{
	size_t i = 0;
	size_t outLen = 0;
	size_t count = (sizeof(messages)/sizeof(messages[0]));

	tcomp_manager_handle_t *manager1 = tsk_null, *manager2 = tsk_null;
	tcomp_result_t *result1 = tsk_null, *result2 = tsk_null;

	/* temporary buffers  --> will hold compression/decompression results */
	char buff1[MAX_BUFFER_SIZE]; 
	char buff2[MAX_BUFFER_SIZE];
	
	/* Managers */
	manager1 = tcomp_manager_create();
	manager2 = tcomp_manager_create();

	/* Add SIP/Presence dictionnaries */
	tcomp_manager_addSipSdpDictionary(manager1);
	tcomp_manager_addPresenceDictionary(manager1);
	tcomp_manager_addSipSdpDictionary(manager2);
	tcomp_manager_addPresenceDictionary(manager2);
	
	/* Results --> it is recomanded to use one result struct for each manager */
	result1 = tcomp_result_create();
	result2 = tcomp_result_create();

	/* Sets compartment Ids */
	tcomp_result_setCompartmentId(result1, COMPARTMENT_ID1, strlen(COMPARTMENT_ID1));
	tcomp_result_setCompartmentId(result2, COMPARTMENT_ID2, strlen(COMPARTMENT_ID2));

	/* Set DMS and SMS */
	tcomp_manager_setDecompression_Memory_Size(manager1, 32768);
	tcomp_manager_setDecompression_Memory_Size(manager2, 32768);
	tcomp_manager_setCycles_Per_Bit(manager1, 64);
	tcomp_manager_setCycles_Per_Bit(manager2, 64);
	tcomp_manager_setState_Memory_Size(manager1, 32768);
	tcomp_manager_setState_Memory_Size(manager2, 32768);
	

	for(i = 0; i< (count * SIGCOMP_LOOP_COUNT); i++)
	{
		memset(buff1, '\0', MAX_BUFFER_SIZE);
		memset(buff2, '\0', MAX_BUFFER_SIZE);

		// 
		//	Compression using manager I
		//
		outLen = tcomp_manager_compress(manager1, COMPARTMENT_ID1, strlen(COMPARTMENT_ID1), messages[i%8], strlen(messages[i%count]), buff1, MAX_BUFFER_SIZE, IS_STREAM);
		if(outLen)
		{
			//* TODO: sendto(SendSocket, buff1, outLen, 0, (SOCKADDR *) &SenderAddr, sizeof(SenderAddr));

			/*
			* Decompress the compressed message using manager II
			*/
			tcomp_result_setOutputBuffer(result2, buff2, MAX_BUFFER_SIZE, IS_STREAM, STREAM_ID); // set the output buffer where to copy decompressed message
			outLen = tcomp_manager_decompress(manager2, buff1, outLen, result2);
			if(outLen) // OK
			{
				// buff2 contains the result and outLen is result length
				TSK_DEBUG_INFO("%s\n\n", buff2);

				// provide the compartment id --> save temp states
				tcomp_manager_provideCompartmentId(manager2, result2);
			}
			else // NOK
			{
				TSK_DEBUG_ERROR("ERROR (1)");
				
				//--assert(tcomp_result_getIsNack(result2));
#if DECOMP_NACK_4_TEST
				manager1->decompress(result2.getNackInfo()->getBuffer(), result2.getNackInfo()->getSize(), &result1);
#endif
				// Decompression failed --> handle NACK (done by remote party)
				// NACK will be retourned only if SigCompVersion >= 2
				// NACK must be sent to the remote party (SIP/IMS use-case) over the network
				//* TODO: sendto(SendSocket, result2.getNackInfo()->getBuffer(), result2.getNackInfo()->getSize(), 0, (SOCKADDR *) &SenderAddr, sizeof(SenderAddr));
			}
		}
		else
		{
			//std::cout<< "ERROR (2)" << std::endl;
			//--assert(0); // MUST never happen	
		}

		// 
		//	Compression using manager II
		//
		outLen = tcomp_manager_compress(manager2, COMPARTMENT_ID2, strlen(COMPARTMENT_ID2), messages[i%8], strlen(messages[i%count]), buff2, MAX_BUFFER_SIZE, IS_STREAM);
		if(outLen)
		{
			tcomp_result_setOutputBuffer(result1, buff1, MAX_BUFFER_SIZE, IS_STREAM, STREAM_ID); // set the output buffer where to copy decompressed message
			outLen = tcomp_manager_decompress(manager1, buff2, outLen, result1);
			if(outLen)
			{
				TSK_DEBUG_INFO("%s\n\n", buff1);
				tcomp_manager_provideCompartmentId(manager1, result1);
			}
			else
			{
				TSK_DEBUG_ERROR("ERROR (3)");
				//--assert(tcomp_result_getIsNack(result2));
#if DECOMP_NACK_4_TEST
				manager2->decompress(result1.getNackInfo()->getBuffer(), result1.getNackInfo()->getSize(), &result2);
#endif
			}
		}
		else
		{
			TSK_DEBUG_ERROR("ERROR (4)");
			//--assert(0);	
		}
	}

	// Close compartments
	tcomp_manager_closeCompartment(manager1, COMPARTMENT_ID1, strlen(COMPARTMENT_ID1));
	tcomp_manager_closeCompartment(manager2, COMPARTMENT_ID2, strlen(COMPARTMENT_ID2));

	// Delete Results
	TSK_OBJECT_SAFE_FREE(result1);
	TSK_OBJECT_SAFE_FREE(result2);

	// Delete managers
	TSK_OBJECT_SAFE_FREE(manager1);
	TSK_OBJECT_SAFE_FREE(manager2);

	return 0;
}

#else /* #if 0 */

#define SIGCOMP_MSG_REGISTER "REGISTER sip:doubango.org SIP/2.0\r\n" \
"Via: SIP/2.0/UDP 192.168.0.11:64549;branch=z9hG4bK357158829;rport;comp=sigcomp;sigcomp-id=\"urn:uuid:d343a50a-2961-4a25-93d8-4e0677b68e6b\"\r\n" \
"From: <sip:001@doubango.org>;tag=357151115\r\n" \
"To: <sip:001@doubango.org>\r\n" \
"Contact: <sip:001@192.168.0.11:64549;transport=udp;sigcomp-id=urn:uuid:d343a50a-2961-4a25-93d8-4e0677b68e6b>;expires=3600;+g.oma.sip-im;language=\"en,fr\";+g.3gpp.smsip;+g.oma.sip-im.large-message;audio;+g.3gpp.icsi-ref=\"urn%3Aurn-7%3A3gpp-application.ims.iari.gsma-vs\";+g.3gpp.cs-voice\r\n" \
"Call-ID: 0473e8ff-2cd6-fada-70ab-ce8c9523db8c\r\n" \
"CSeq: 5493 REGISTER\r\n" \
"Content-Length: 0\r\n" \
"Max-Forwards: 70\r\n" \
"Allow: INVITE, ACK, CANCEL, BYE, MESSAGE, OPTIONS, NOTIFY, PRACK, UPDATE, REFER\r\n" \
"Privacy: none\r\n" \
"P-Access-Network-Info: ADSL;utran-cell-id-3gpp=00000000\r\n" \
"User-Agent: IM-client/OMA1.0 Boghe/v2.0.121.757\r\n" \
"P-Preferred-Identity: <sip:001@doubango.org>\r\n" \
"Supported: path\r\n" \
"\r\n"

#define SIGCOMP_MSG_200REGISTER "SIP/2.0 200 OK\r\n" \
"Via: SIP/2.0/UDP 192.168.0.11:64549;branch=z9hG4bK357158829;rport=64549;comp=sigcomp;sigcomp-id=\"urn:uuid:d343a50a-2961-4a25-93d8-4e0677b68e6b\"\r\n" \
"Contact: <sip:001@192.168.0.11:54027;transport=udp;sigcomp-id=\"urn:uuid:7556d001-9ca7-41a3-80b0-5b7725a91cc4\">;expires=2761;language=\"en,fr\";+g.oma.sip-im;+g.3gpp.smsip;+g.oma.sip-im.large-message;audio;+g.3gpp.icsi-ref=\"urn%3Aurn-7%3A3gpp-application.ims.iari.gsma-vs\";+g.3gpp.cs-voice\r\n" \
"Contact: <sip:001@192.168.0.11:50417;transport=udp;sigcomp-id=\"urn:uuid:6a8b493e-7dd8-45fb-b104-085723cbfb7b\">;expires=3025;language=\"en,fr\";+g.oma.sip-im;+g.3gpp.smsip;+g.oma.sip-im.large-message;audio;+g.3gpp.icsi-ref=\"urn%3Aurn-7%3A3gpp-application.ims.iari.gsma-vs\";+g.3gpp.cs-voice\r\n" \
"Contact: <sip:001@192.168.0.11:50464;transport=udp;sigcomp-id=\"urn:uuid:c5534343-0fb1-41a8-b4bc-612f04913d2a\">;expires=3409;language=\"en,fr\";+g.oma.sip-im;+g.3gpp.smsip;+g.oma.sip-im.large-message;audio;+g.3gpp.icsi-ref=\"urn%3Aurn-7%3A3gpp-application.ims.iari.gsma-vs\";+g.3gpp.cs-voice\r\n" \
"Contact: <sip:001@192.168.0.11:64547;transport=udp;sigcomp-id=\"urn:uuid:40ce6b6c-90b3-487d-8768-ae180a8153b1\">;expires=3487;language=\"en,fr\";+g.oma.sip-im;+g.3gpp.smsip;+g.oma.sip-im.large-message;audio;+g.3gpp.icsi-ref=\"urn%3Aurn-7%3A3gpp-application.ims.iari.gsma-vs\";+g.3gpp.cs-voice\r\n" \
"Contact: <sip:001@192.168.0.11:64549;transport=udp;sigcomp-id=\"urn:uuid:d343a50a-2961-4a25-93d8-4e0677b68e6b\">;expires=3600;language=\"en,fr\";+g.oma.sip-im;+g.3gpp.smsip;+g.oma.sip-im.large-message;audio;+g.3gpp.icsi-ref=\"urn%3Aurn-7%3A3gpp-application.ims.iari.gsma-vs\";+g.3gpp.cs-voice\r\n" \
"To: <sip:001@doubango.org>;tag=4345c04e\r\n" \
"From: <sip:001@doubango.org>;tag=357151115\r\n" \
"Call-ID: 0473e8ff-2cd6-fada-70ab-ce8c9523db8c\r\n" \
"CSeq: 5493 REGISTER\r\n" \
"Content-Length: 0\r\n" \
"\r\n"

#define SIGCOMP_MSG_INVITE "INVITE sip:13131313@sip2sip.info SIP/2.0\r\n" \
"Via: SIP/2.0/UDP 192.168.0.11:64549;branch=z9hG4bK357179240;rport;comp=sigcomp;sigcomp-id=\"urn:uuid:d343a50a-2961-4a25-93d8-4e0677b68e6b\"\r\n" \
"From: <sip:001@doubango.org>;tag=357203612\r\n" \
"To: <sip:13131313@sip2sip.info>\r\n" \
"Contact: <sip:001@192.168.0.11:64549;transport=udp;sigcomp-id=urn:uuid:d343a50a-2961-4a25-93d8-4e0677b68e6b>;+g.oma.sip-im;language=\"en,fr\";+g.3gpp.icsi-ref=\"urn%3Aurn-7%3A3gpp-service.ims.icsi.mmtel\"\r\n" \
"Call-ID: da91debf-0033-dd9e-9c8d-e9018f825c8f\r\n" \
"CSeq: 14727 INVITE\r\n" \
"Content-Type: application/sdp\r\n" \
"Content-Length: 989\r\n" \
"Max-Forwards: 70\r\n" \
"Accept-Contact: *;+g.3gpp.icsi-ref=\"urn%3Aurn-7%3A3gpp-service.ims.icsi.mmtel\"\r\n" \
"P-Preferred-Service: urn:urn-7:3gpp-service.ims.icsi.mmtel\r\n" \
"Allow: INVITE, ACK, CANCEL, BYE, MESSAGE, OPTIONS, NOTIFY, PRACK, UPDATE, REFER\r\n" \
"Privacy: none\r\n" \
"P-Access-Network-Info: ADSL;utran-cell-id-3gpp=00000000\r\n" \
"User-Agent: IM-client/OMA1.0 Boghe/v2.0.121.757\r\n" \
"Supported: 100rel\r\n" \
"\r\n" \
"v=0\r\n" \
"o=doubango 1983 678901 IN IP4 192.168.0.11\r\n" \
"s=-\r\n" \
"c=IN IP4 192.168.0.11\r\n" \
"t=0 0\r\n" \
"m=audio 64550 RTP/SAVPF 0 8 97 101\r\n" \
"c=IN IP4 192.168.0.11\r\n" \
"a=ptime:20\r\n" \
"a=silenceSupp:off - - - -\r\n" \
"a=rtpmap:0 PCMU/8000/1\r\n" \
"a=rtpmap:8 PCMA/8000/1\r\n" \
"a=rtpmap:97 SPEEX/8000/1\r\n" \
"a=rtpmap:101 telephone-event/8000/1\r\n" \
"a=fmtp:101 0-16\r\n" \
"a=sendrecv\r\n" \
"a=crypto:1 AES_CM_128_HMAC_SHA1_80 inline:JaJaTgwhqSd2A7yQ842YPXASomulb95aX/Wa96T7\r\n" \
"a=crypto:2 AES_CM_128_HMAC_SHA1_32 inline:T3yTUA2dVXTr6v0jb+E1TR3Btj4xP3mPeaAtQbeC\r\n" \
"a=rtcp-mux\r\n" \
"a=ssrc:3301897658 cname:ldjWoB60jbyQlR6e\r\n" \
"a=ssrc:3301897658 mslabel:6994f7d1-6ce9-4fbd-acfd-84e5131ca2e2\r\n" \
"a=ssrc:3301897658 label:Doubango\r\n" \
"a=ice-ufrag:zNevtNKAGk6h7gJ\r\n" \
"a=ice-pwd:YaOV3FlWUkBmifZ87SP51\r\n" \
"a=mid:audio\r\n" \
"a=candidate:CuNeg0Keh 1 udp 2130706431 192.168.0.11 64550 typ host\r\n" \
"a=candidate:CuNeg0Keh 2 udp 2130706430 192.168.0.11 64551 typ host\r\n" \
"a=candidate:srflxCuNe 1 udp 1694498815 88.179.39.5 64550 typ srflx\r\n" \
"a=candidate:srflxCuNe 2 udp 1694498814 88.179.39.5 64551 typ srflx\r\n" \
"\r\n"

#define SIGCOMP_MSG_100INVITE "SIP/2.0 100 Trying\r\n" \
"Via: SIP/2.0/UDP 192.168.0.11:64549;branch=z9hG4bK357179240;rport=64549;comp=sigcomp;sigcomp-id=\"urn:uuid:d343a50a-2961-4a25-93d8-4e0677b68e6b\"\r\n" \
"To: <sip:13131313@sip2sip.info>\r\n" \
"From: <sip:001@doubango.org>;tag=357203612\r\n" \
"Call-ID: da91debf-0033-dd9e-9c8d-e9018f825c8f\r\n" \
"CSeq: 14727 INVITE\r\n" \
"Content-Length: 0\r\n" \
"\r\n"

#define SIGCOMP_MSG_488INVITE "SIP/2.0 488 Not acceptable here\r\n" \
"Via: SIP/2.0/UDP 192.168.0.11:64549;branch=z9hG4bK357179240;rport=64549;comp=sigcomp;sigcomp-id=\"urn:uuid:d343a50a-2961-4a25-93d8-4e0677b68e6b\"\r\n" \
"To: <sip:13131313@sip2sip.info>;tag=as6edf9ce2\r\n" \
"From: <sip:001@doubango.org>;tag=357203612\r\n" \
"Call-ID: da91debf-0033-dd9e-9c8d-e9018f825c8f\r\n" \
"CSeq: 14727 INVITE\r\n" \
"Allow: INVITE, ACK, CANCEL, OPTIONS, BYE, REFER, SUBSCRIBE, NOTIFY, INFO, PUBLISH\r\n" \
"Server: Asterisk PBX\r\n" \
"Supported: replaces, timer\r\n" \
"Content-Length: 0\r\n" \
"\r\n"

#define SIGCOMP_MSG_ACK "ACK sip:13131313@sip2sip.info SIP/2.0\r\n" \
"Via: SIP/2.0/UDP 192.168.0.11:64549;branch=z9hG4bK357179240;rport;comp=sigcomp;sigcomp-id=\"urn:uuid:d343a50a-2961-4a25-93d8-4e0677b68e6b\"\r\n" \
"From: <sip:001@doubango.org>;tag=357203612\r\n" \
"To: <sip:13131313@sip2sip.info>;tag=as6edf9ce2\r\n" \
"Call-ID: da91debf-0033-dd9e-9c8d-e9018f825c8f\r\n" \
"CSeq: 14727 ACK\r\n" \
"Content-Length: 0\r\n" \
"Max-Forwards: 70\r\n" \
"\r\n"

static int handle_nack(tcomp_result_t *nack, tcomp_manager_handle_t* manager, tcomp_result_t *result)
{
	if(nack->isNack){
		tsk_size_t nack_info_size;
		if(nack->nack_info && (nack_info_size = tcomp_buffer_getSize(nack->nack_info))){
			tcomp_manager_decompress(manager, tcomp_buffer_getBuffer(nack->nack_info), nack_info_size, result);
			return 0;
		}
	}
	return -1;
}

static int test_manager()
{
	size_t i = 0;
	size_t outLen;

	tcomp_manager_handle_t *client = tsk_null, *server = tsk_null;
	tcomp_result_t *result1 = tsk_null, *result2 = tsk_null;

	/* temporary buffers  --> will hold compression/decompression results */
	char buff1[MAX_BUFFER_SIZE]; 
	char buff2[MAX_BUFFER_SIZE];
	
	/* Managers */
	client = tcomp_manager_create();
	server = tcomp_manager_create();

	/* Add SIP/Presence dictionnaries */
	tcomp_manager_addSipSdpDictionary(client);
	tcomp_manager_addPresenceDictionary(client);
	tcomp_manager_addSipSdpDictionary(server);
	tcomp_manager_addPresenceDictionary(server);
	
	/* Results --> it is recomanded to use one result struct for each manager */
	result1 = tcomp_result_create();
	result2 = tcomp_result_create();

	/* Sets compartment Ids */
	tcomp_result_setCompartmentId(result1, COMPARTMENT_ID1, strlen(COMPARTMENT_ID1));
	tcomp_result_setCompartmentId(result2, COMPARTMENT_ID2, strlen(COMPARTMENT_ID2));

	/* Set DMS and SMS */
	tcomp_manager_setDecompression_Memory_Size(client, SIGCOMP_DMS);
	tcomp_manager_setDecompression_Memory_Size(server, SIGCOMP_DMS);
	tcomp_manager_setCycles_Per_Bit(client, SIGCOMP_CPB);
	tcomp_manager_setCycles_Per_Bit(server, SIGCOMP_CPB);
	tcomp_manager_setState_Memory_Size(client, SIGCOMP_SMS);
	tcomp_manager_setState_Memory_Size(server, SIGCOMP_SMS);

	tcomp_result_setOutputBuffer(result1, buff1, MAX_BUFFER_SIZE, IS_STREAM, STREAM_ID);
	tcomp_result_setOutputBuffer(result2, buff2, MAX_BUFFER_SIZE, IS_STREAM, STREAM_ID);
	
	for(i = 0; i < SIGCOMP_LOOP_COUNT; ++i)
	{
		// Compress REGISTER
		outLen = tcomp_manager_compress(client, COMPARTMENT_ID1, tsk_strlen(COMPARTMENT_ID1), SIGCOMP_MSG_REGISTER, tsk_strlen(SIGCOMP_MSG_REGISTER), buff1, MAX_BUFFER_SIZE, IS_STREAM);
		if(!outLen)
		{
			TSK_DEBUG_ERROR("Failed to compress REGISTER request");
			goto bail;
		}
		TSK_DEBUG_INFO("==REGISTER compression ratio = %f%%==", ((float)tsk_strlen(SIGCOMP_MSG_REGISTER) / (float)outLen) * 100.f);

		// Decompress REGISTER
		memset(buff2, '\0', MAX_BUFFER_SIZE);
		outLen = tcomp_manager_decompress(server, buff1, outLen, result2);
		if(!outLen)
		{
			TSK_DEBUG_ERROR("Failed to decompress REGISTER request");
			goto bail;
		}
		tcomp_manager_provideCompartmentId(server, result2); // save states
		//--TSK_DEBUG_INFO("Decompressed message:\nREGISTER = %s", buff2);

		// Compress 200 REGISTER
		outLen = tcomp_manager_compress(server, COMPARTMENT_ID2, tsk_strlen(COMPARTMENT_ID2), SIGCOMP_MSG_200REGISTER, tsk_strlen(SIGCOMP_MSG_200REGISTER), buff2, MAX_BUFFER_SIZE, IS_STREAM);
		if(!outLen)
		{
			TSK_DEBUG_ERROR("Failed to compress 200 REGISTER response");
			goto bail;
		}
		TSK_DEBUG_INFO("==200 REGISTER compression ratio = %f%%==", ((float)tsk_strlen(SIGCOMP_MSG_200REGISTER) / (float)outLen) * 100.f);

		// Decompress 200 REGISTER
		memset(buff1, '\0', MAX_BUFFER_SIZE);
		outLen = tcomp_manager_decompress(client, buff2, outLen, result1);
		if(!outLen)
		{
			TSK_DEBUG_ERROR("Failed to decompress REGISTER request");
			goto bail;
		}
		tcomp_manager_provideCompartmentId(client, result1); // save states
		//--TSK_DEBUG_INFO("Decompressed message:\n200 REGISTER = %s", buff1);


		// Compress INVITE
COMPRESS_INVITE:
		outLen = tcomp_manager_compress(client, COMPARTMENT_ID1, tsk_strlen(COMPARTMENT_ID1), SIGCOMP_MSG_INVITE, tsk_strlen(SIGCOMP_MSG_INVITE), buff1, MAX_BUFFER_SIZE, IS_STREAM);
		if(!outLen)
		{
			TSK_DEBUG_ERROR("Failed to compress INVITE request");
			goto bail;
		}
		TSK_DEBUG_INFO("==INVITE compression ratio = %f%%==", ((float)tsk_strlen(SIGCOMP_MSG_INVITE) / (float)outLen) * 100.f);

		// Decompress INVITE
		memset(buff2, '\0', MAX_BUFFER_SIZE);
		outLen = tcomp_manager_decompress(server, buff1, outLen, result2);
		if(!outLen)
		{
			TSK_DEBUG_ERROR("Failed to decompress INVITE request");
			if(handle_nack(result2, client, result1) == 0){
				goto COMPRESS_INVITE;
			}
			goto bail;
		}
		tcomp_manager_provideCompartmentId(server, result2); // save states
		//--TSK_DEBUG_INFO("Decompressed message:\nINVITE = %s", buff2);


		// Compress 488 INVITE
		outLen = tcomp_manager_compress(server, COMPARTMENT_ID2, tsk_strlen(COMPARTMENT_ID2), SIGCOMP_MSG_488INVITE, tsk_strlen(SIGCOMP_MSG_488INVITE), buff2, MAX_BUFFER_SIZE, IS_STREAM);
		if(!outLen)
		{
			TSK_DEBUG_ERROR("Failed to compress 488 INVITE response");
			goto bail;
		}
		TSK_DEBUG_INFO("==488 INVITE compression ratio = %f%%==", ((float)tsk_strlen(SIGCOMP_MSG_488INVITE) / (float)outLen) * 100.f);

		// Decompress 488 INVITE
		memset(buff1, '\0', MAX_BUFFER_SIZE);
		outLen = tcomp_manager_decompress(client, buff2, outLen, result1);
		if(!outLen)
		{
			TSK_DEBUG_ERROR("Failed to decompress 488 INVITE response");
			goto bail;
		}
		tcomp_manager_provideCompartmentId(client, result1); // save states
		//--TSK_DEBUG_INFO("Decompressed message:\n488 INVITE = %s", buff1);


		// Compress ACK
		outLen = tcomp_manager_compress(client, COMPARTMENT_ID1, tsk_strlen(COMPARTMENT_ID1), SIGCOMP_MSG_ACK, tsk_strlen(SIGCOMP_MSG_ACK), buff1, MAX_BUFFER_SIZE, IS_STREAM);
		if(!outLen)
		{
			TSK_DEBUG_ERROR("Failed to compress ACK request");
			goto bail;
		}
		TSK_DEBUG_INFO("==ACK compression ratio = %f%%==", ((float)tsk_strlen(SIGCOMP_MSG_ACK) / (float)outLen) * 100.f);

		// Decompress ACK
		memset(buff2, '\0', MAX_BUFFER_SIZE);
		outLen = tcomp_manager_decompress(server, buff1, outLen, result2);
		if(!outLen)
		{
			TSK_DEBUG_ERROR("Failed to decompress ACK request");
			goto bail;
		}
		tcomp_manager_provideCompartmentId(server, result2); // save states
		//--TSK_DEBUG_INFO("Decompressed message:\nACK = %s", buff2);

	} // for (i < SIGCOMP_LOOP_COUNT)

bail:
	// Close compartments
	tcomp_manager_closeCompartment(client, COMPARTMENT_ID1, tsk_strlen(COMPARTMENT_ID1));
	tcomp_manager_closeCompartment(server, COMPARTMENT_ID2, tsk_strlen(COMPARTMENT_ID2));

	// Delete Results
	TSK_OBJECT_SAFE_FREE(result1);
	TSK_OBJECT_SAFE_FREE(result2);

	// Delete managers
	TSK_OBJECT_SAFE_FREE(client);
	TSK_OBJECT_SAFE_FREE(server);

	return 0;
}

#endif /* #if 0 */

#endif /* TEST_TINYSIGCOMP_MANAGER_H */
