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

#define SIP_MESSAGE \
	"REGISTER sip:ims-network.com SIP/2.0\r\n" \
	"Test-Header: 0\r\n" \
	"v: SIP/2.0/UDP [::]:1988;comp=sigcomp;rport=254;ttl=457;received=192.0.2.101;branch=z9hG4bK1245420841406\r\n" \
	"From: <sip:mamadou@ims-network.com>;tag=29358\r\n" \
	"To: <sip:mamadou@ims-network.com>\r\n" \
	"Call-ID: M-fa53180346f7f55ceb8d8670f9223dbb\r\n" \
	"CSeq: 201 REGISTER\r\n" \
	"Max-Forwards: 70\r\n" \
	"Allow: INVITE, ACK, CANCEL, BYE, MESSAGE, OPTIONS, NOTIFY, PRACK, UPDATE, REFER\r\n" \
	"m: <sip:mamadou@[::]:1988;comp=sigcomp;transport=udp>;expires=600000;+deviceID=\"3ca50bcb-7a67-44f1-afd0-994a55f930f4\";mobility=\"fixed\";+g.3gpp.cs-voice;+g.3gpp.app%5fref=\"urn%3Aurnxxx%3A3gpp-application.ims.iari.gsmais\";+g.oma.sip-im.large-message;+g.oma.sip-im\r\n" \
	"User-Agent: IM-client/OMA1.0 doubango/v4.0.1390.0\r\n" \
	"Require: pref\r\n" \
	"P-Preferred-Identity: <sip:mamadou@ims-network.com>\r\n" \
	"Supported: path\r\n" \
	"P-Access-Network-Info: 3GPP-UTRAN-TDD;utran-cell-id-3gpp=00000000\r\n" \
	"Privacy: none\r\n" \
	"Supported: timer\r\n" \
	"Content-Length: 0\r\n" \
	"\r\n"

void test_messages()
{
	tsip_ragel_state_t state;
	tsip_message_t *message = TSIP_MESSAGE_CREATE();

	tsip_ragel_state_init(&state, SIP_MESSAGE, strlen(SIP_MESSAGE));
	tsip_message_parse(&state, message);

	TSIP_MESSAGE_SAFE_FREE(message);
}



#endif /* _TEST_SIPMESSAGES_H */