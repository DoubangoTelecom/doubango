/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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
#ifndef _TEST_IMS_AKA_H
#define _TEST_IMS_AKA_H

#include "tinysip/authentication/tsip_challenge.h" /* Not part of the API */

#define TEST_IMS_AKA_REQUEST "REGISTER sip:ims.inexbee.com SIP/2.0\r\n" \
"v: SIP/2.0/TCP 192.168.16.82:2851;branch=z9hG4bK1272986926192;rport\r\n" \
"f: <sip:bob@ims.inexbee.com>;tag=1272986909384\r\n" \
"t: <sip:bob@ims.inexbee.com>\r\n" \
"m: <sip:bob@192.168.16.82:2851;transport=tcp>;expires=30;+g.oma.sip-im;+g.3gpp.smsip;language=\"en,fr\"\r\n" \
"i: ef566354-2051-df74-b888-1d30e1753213\r\n" \
"CSeq: 30149 REGISTER\r\n" \
"l: 0\r\n" \
"Max-Forwards: 70\r\n" \
"Authorization: Digest username=\"bob@ims.inexbee.com\",realm=\"ims.inexbee.com\",nonce=\"\",uri=\"sip:ims.inexbee.com\",response=\"\"\r\n" \
"Privacy: header;id\r\n" \
"Allow: INVITE, ACK, CANCEL, BYE, MESSAGE, OPTIONS, NOTIFY, PRACK, UPDATE, REFER\r\n" \
"P-Access-Network-Info: ADSL;utran-cell-id-3gpp=00000000\r\n" \
"User-Agent: IM-client/OMA1.0 doubango/v1.0.0\r\n" \
"P-Preferred-Identity: <sip:bob@ims.inexbee.com>\r\n" \
"k: path\r\n" \
"Action-Header: Myheader-value\r\n" \
"\r\n"



void test_imsaka()
{
    tsip_challenge_t* challenge;
    tsip_stack_handle_t *stack;
    tsip_request_t *request = tsk_null;
    tsk_ragel_state_t state;
    tsip_header_Authorization_t* hdr_Auth;

    /*  create the SIP stack */
    stack = tsip_stack_create(tsk_null, "sip:ims.inexbee.com", "bob@ims.inexbee.com", "sip:bob@ims.inexbee.com",
                              TSIP_STACK_SET_PASSWORD("bob"),
                              TSIP_STACK_SET_NULL());

    /* create the chalenge */
    challenge = tsip_challenge_create(stack,
                                      tsk_false,
                                      "Digest", /* scheme */
                                      "ims.inexbee.com", /* realm */
                                      "RqhHfrN+ciXaM4mt8k/0Lyx7bgshVgAA5TcTpiTtqsY=", /* nonce */
                                      tsk_null, /* opaque */
                                      "AKAv1-MD5", /* algorithm */
                                      "auth" /* qop */
                                     );

    /* Parse SIP request */
    tsk_ragel_state_init(&state, TEST_IMS_AKA_REQUEST, tsk_strlen(TEST_IMS_AKA_REQUEST));
    if(!tsip_message_parse(&state, &request, tsk_true)) {
        goto bail;
    }

    /* Gets auth header */
    if((hdr_Auth = (tsip_header_Authorization_t*)tsip_challenge_create_header_authorization(challenge, request))) {
        TSK_DEBUG_INFO("Response=[%s]", hdr_Auth->response);
    }


bail:
    TSK_OBJECT_SAFE_FREE(request);
    TSK_OBJECT_SAFE_FREE(challenge);
    TSK_OBJECT_SAFE_FREE(stack);

    getchar();
}

#endif /* _TEST_IMS_AKA_H */

