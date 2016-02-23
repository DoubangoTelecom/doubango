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
#ifndef _TEST_STACK_H
#define _TEST_STACK_H

#ifndef LOCAL_IP
#	ifdef ANDROID
#		define LOCAL_IP	"10.0.2.15"
#	else
#		define LOCAL_IP	TNET_SOCKET_HOST_ANY
#	endif
#endif

#define TEST_STACK_SDP \
	"v=0\r\n" \
	"o=bob 2890844730 2890844731 IN IP4 host.example.com\r\n" \
	"s=\r\n" \
	"c=IN IP4 192.168.0.12\r\n" \
	"t=0 0\r\n" \
	"m=audio 54344 RTP/AVP 0\r\n" \
	"a=rtpmap:0 PCMU/8000\r\n"

#define TEST_STACK_PIDF \
	"<?xml version=\"1.0\" encoding=\"utf-8\"?>"\
	"<presence xmlns:cp=\"urn:ietf:params:xml:ns:pidf:cipid\" xmlns:caps=\"urn:ietf:params:xml:ns:pidf:caps\" xmlns:rpid=\"urn:ietf:params:xml:ns:pidf:rpid\" xmlns:pdm=\"urn:ietf:params:xml:ns:pidf:data-model\" xmlns:p=\"urn:ietf:params:xml:ns:pidf-diff\" xmlns:op=\"urn:oma:xml:prs:pidf:oma-pres\" entity=\"sip:mamadou@"DOMAIN"\" xmlns=\"urn:ietf:params:xml:ns:pidf\">"\
	"  <pdm:person id=\"CRUVREZS\">"\
	"	<op:overriding-willingness>"\
	"	  <op:basic>open</op:basic>"\
	"	</op:overriding-willingness>"\
	"	<rpid:activities>"\
	"	  <rpid:unknown />"\
	"	</rpid:activities>"\
	"	<rpid:mood>"\
	"	  <rpid:neutral />"\
	"	</rpid:mood>"\
	"	<pdm:note>Hello world</pdm:note>"\
	"  </pdm:person>"\
	"  <pdm:device id=\"d0001\">"\
	"	<status>"\
	"	  <basic>open</basic>"\
	"	</status>"\
	"	<caps:devcaps>"\
	"	  <caps:mobility>"\
	"		<caps:supported>"\
	"		  <caps:fixed />"\
	"		</caps:supported>"\
	"	  </caps:mobility>"\
	"	</caps:devcaps>"\
	"	<op:network-availability>"\
	"	  <op:network id=\"IMS\">"\
	"		<op:active />"\
	"	  </op:network>"\
	"	</op:network-availability>"\
	"	<pdm:deviceID>urn:uuid:dd1289fa-c3d7-47bd-a40d-f1f1b2cc5ffc</pdm:deviceID>"\
	"  </pdm:device>"\
	"</presence>"

int test_stack_callback(const tsip_event_t *sipevent)
{
    TSK_DEBUG_INFO("\n====\nSTACK event: %d [%s] with opid=%lld\n=====",
                   sipevent->code, sipevent->phrase, tsip_ssession_get_id(sipevent->ss));

    // For ssessions created by the stack ==> call tsk_object_ref(sipevent->ssession);
    // to take ownership.

    switch(sipevent->type) {
        //
        // REGISTER
        //
    case tsip_event_register: {
        const tsip_register_event_t* _event;
        TSK_DEBUG_INFO("SIP event(REGISTER)");

        _event = TSIP_REGISTER_EVENT(sipevent);
        switch(_event->type) {
        case tsip_ao_register: { /* Answer to Outgoing REGISTER */
            if(TSIP_RESPONSE_IS_2XX(sipevent->sipmessage)) {
                TSK_DEBUG_INFO("Registration succeed.");
            }
            else {
                TSK_DEBUG_INFO("Registration failed.");
            }
            break;
        }
        case tsip_ao_unregister: { /* Answer to Outgoing UNREGISTER */
            if(TSIP_RESPONSE_IS_2XX(sipevent->sipmessage)) {
                TSK_DEBUG_INFO("UnRegistration succeed.");
            }
            else {
                TSK_DEBUG_INFO("UnRegistration failed.");
            }
            break;
        }
        default:
            break;
        }

        break;
    }

    //
    // INVITE
    //
    case tsip_event_invite: {
        TSK_DEBUG_INFO("SIP event(INVITE)");
        break;
    }

    //
    // MESSAGE
    //
    case tsip_event_message: {
        const tsip_message_event_t* _event;
        TSK_DEBUG_INFO("SIP event(MESSAGE)");

        _event = TSIP_MESSAGE_EVENT(sipevent);
        switch(_event->type) {
        case tsip_i_message: { /* Incoming MESSAGE */
            if(TSIP_MESSAGE_HAS_CONTENT(sipevent->sipmessage)) {
                TSK_DEBUG_INFO("MESSAGE Content: %s", TSIP_MESSAGE_CONTENT(sipevent->sipmessage));
            }
            break;
        }
        case tsip_ao_message: { /* Answer to Outgoing MESSAGE */
            if(TSIP_RESPONSE_IS_2XX(sipevent->sipmessage)) {
                TSK_DEBUG_INFO("MESSAGE successfully sent.");
            }
            else {
                TSK_DEBUG_INFO("Failed to send MESSAGE (sip code:%d).", TSIP_RESPONSE_CODE(sipevent->sipmessage));
            }
        }
        default:
            break;
        }

        break;
    }

    //
    // PUBLISH
    //
    case tsip_event_publish: {
        TSK_DEBUG_INFO("SIP event(PUBLISH)");
        break;
    }

    //
    // SUBSCRIBE
    //
    case tsip_event_subscribe: {
        const tsip_subscribe_event_t* _event;
        TSK_DEBUG_INFO("SIP event(SUBSCRIBE)");

        _event = TSIP_SUBSCRIBE_EVENT(sipevent);
        switch(_event->type) {
        case tsip_i_notify: { /* Incoming NOTIFY */
            if(TSIP_MESSAGE_CONTENT_DATA(sipevent->sipmessage)) {
                TSK_DEBUG_INFO("NOTIFY Content: %s", TSIP_MESSAGE_CONTENT(sipevent->sipmessage));
            }
            break;
        }
        case tsip_ao_subscribe: { /* Answer to Outgoing SUBSCRIBE */
            if(TSIP_RESPONSE_IS_2XX(sipevent->sipmessage)) {
                TSK_DEBUG_INFO("Subscription succeed.");
            }
            else {
                TSK_DEBUG_INFO("Subscription failed.");
            }
            break;
        }
        case tsip_ao_unsubscribe: { /* Answer to Outgoing UNSUBSCRIBE */
            if(TSIP_RESPONSE_IS_2XX(sipevent->sipmessage)) {
                TSK_DEBUG_INFO("UnSubscription succeed.");
            }
            else {
                TSK_DEBUG_INFO("UnSubscription failed.");
            }
            break;
        }
        default:
            break;
        }

        break;
    }

    default: {
        TSK_DEBUG_INFO("SIP event(UNKNOWN)");
        break;
    }
    }

    //tsk_thread_sleep(1000000);

    return 0;
}


void test_stack()
{
#define DOMAIN "ericsson.com"
//#define DOMAIN "micromethod.com"
//#define DOMAIN "ims.inexbee.com"
//#define DOMAIN "sip2sip.info"

    const void* usr_context = tsk_null;
    int ret;
    uint16_t AMF = 0x0001;
    /*
    	tsip_stack_handle_t *stack = tsip_stack_create(test_stack_callback,
    		TSIP_STACK_SET_DISPLAY_NAME("2233392625"),
    		TSIP_STACK_SET_PUBLIC_IDENTITY("sip:2233392625@"DOMAIN),
    		TSIP_STACK_SET_PRIVATE_IDENTITY("2233392625"),
    		TSIP_STACK_SET_PASSWORD("d3sb7j4fb8"),
    		TSIP_STACK_SET_REALM("sip:"DOMAIN), // FIXME: without sip:
    		TSIP_STACK_SET_LOCAL_IP(LOCAL_IP),
    		//TSIP_STACK_SET_DISCOVERY_NAPTR(1),
    		//TSIP_STACK_SET_PROXY_CSCF("proxy.sipthor.net", "udp", 0),
    		//TSIP_STACK_SET_PROXY_CSCF("192.168.0.15", "udp", 0),
    		TSIP_STACK_SET_PROXY_CSCF_PORT(5060),
    		TSIP_STACK_SET_MOBILITY("fixed"),
    		TSIP_STACK_SET_DEVICE_ID("dd1289fa-c3d7-47bd-a40d-f1f1b2cc5ffc"),
    		TSIP_STACK_SET_NETINFO("ADSL;utran-cell-id-3gpp=00000000"),
    		TSIP_STACK_SET_PRIVACY("header;id"),
    */

    /*
    tsip_stack_handle_t *stack = tsip_stack_create(test_stack_callback,
    	TSIP_STACK_SET_DISPLAY_NAME("Mamadou"),
    	TSIP_STACK_SET_PUBLIC_IDENTITY("sip:mamadou@"DOMAIN),
    	TSIP_STACK_SET_PRIVATE_IDENTITY("mamadou@"DOMAIN),
    	TSIP_STACK_SET_PASSWORD("mamadou"),
    	TSIP_STACK_SET_REALM("sip:"DOMAIN), // FIXME: without sip:
    	TSIP_STACK_SET_LOCAL_IP(LOCAL_IP),
    	//TSIP_STACK_SET_DISCOVERY_NAPTR(1),
    	TSIP_STACK_SET_PROXY_CSCF("192.168.0.15", "udp", 0),
    	//TSIP_STACK_SET_PROXY_CSCF_PORT(5081),
    	TSIP_STACK_SET_PROXY_CSCF_PORT(5060),
    	//TSIP_STACK_SET_SECAGREE_IPSEC("hmac-md5-96", "null", "trans", "esp"),
    	TSIP_STACK_SET_MOBILITY("fixed"),
    	TSIP_STACK_SET_DEVICE_ID("dd1289fa-c3d7-47bd-a40d-f1f1b2cc5ffc"),
    	TSIP_STACK_SET_NETINFO("ADSL;utran-cell-id-3gpp=00000000"),
    	TSIP_STACK_SET_PRIVACY("header;id"),
    */



    int32_t port = 4060;
    /*
    	tsip_stack_handle_t *stack = tsip_stack_create(test_stack_callback, "sip:"DOMAIN, "mamadou@"DOMAIN, "sip:mamadou@"DOMAIN,
    		TSIP_STACK_SET_DISPLAY_NAME("Mamadou"),
    		TSIP_STACK_SET_PASSWORD("mamadou"),
    		TSIP_STACK_SET_IMS_AKA_AMF(AMF),
    		TSIP_STACK_SET_IMS_AKA_OPERATOR_ID("0xff08"),
    		TSIP_STACK_SET_HEADER("User-Agent", "IM-client/OMA1.0 doubango/v1.0.0"),

    		TSIP_STACK_SET_PROXY_CSCF("192.168.16.225", port, "udp", "ipv4"),

    		TSIP_STACK_SET_EARLY_IMS(tsk_true),
    */

    tsip_stack_handle_t *stack = tsip_stack_create(test_stack_callback, "sip:"DOMAIN, "mamadou@"DOMAIN, "sip:mamadou@"DOMAIN,
                                 TSIP_STACK_SET_DISPLAY_NAME("Mamadou"),
                                 TSIP_STACK_SET_PASSWORD("mamadou"),
                                 TSIP_STACK_SET_IMS_AKA_AMF(AMF),
                                 TSIP_STACK_SET_IMS_AKA_OPERATOR_ID("0xff08"),
                                 TSIP_STACK_SET_HEADER("User-Agent", "IM-client/OMA1.0 doubango/v1.0.0"),

                                 TSIP_STACK_SET_PROXY_CSCF("192.168.0.13", 5081, "tcp", "ipv4"),
                                 TSIP_STACK_SET_LOCAL_IP(LOCAL_IP),

                                 TSIP_STACK_SET_EARLY_IMS(tsk_true),


                                 /*
                                 	tsip_stack_handle_t *stack = tsip_stack_create(test_stack_callback, "sip:"DOMAIN, "2233392625", "sip:2233392625@"DOMAIN,
                                 		TSIP_STACK_SET_DISPLAY_NAME("Mamadou"),
                                 		TSIP_STACK_SET_PASSWORD("d3sb7j4fb8"),

                                 		TSIP_STACK_SET_EARLY_IMS(tsk_true),

                                 		TSIP_STACK_SET_HEADER("User-Agent", "IM-client/OMA1.0 doubango/v1.0.0"),
                                 */
                                 /*
                                 		tsip_stack_handle_t *stack = tsip_stack_create(test_stack_callback,
                                 		TSIP_STACK_SET_DISPLAY_NAME("Mamadou"),
                                 		TSIP_STACK_SET_PUBLIC_IDENTITY("sip:mamadou@"DOMAIN),
                                 		TSIP_STACK_SET_PRIVATE_IDENTITY("mamadou@"DOMAIN),
                                 		TSIP_STACK_SET_PASSWORD("mamadou"),
                                 		TSIP_STACK_SET_REALM("sip:"DOMAIN), // FIXME: without sip:
                                 		TSIP_STACK_SET_LOCAL_IP(LOCAL_IP),
                                 		//TSIP_STACK_SET_DISCOVERY_NAPTR(1),
                                 		TSIP_STACK_SET_PROXY_CSCF("192.168.16.104", "udp", 0),
                                 		//TSIP_STACK_SET_PROXY_CSCF("192.168.0.15", "udp", 0),
                                 		TSIP_STACK_SET_PROXY_CSCF_PORT(5060),
                                 		//TSIP_STACK_SET_SECAGREE_IPSEC("hmac-md5-96", "null", "trans", "esp"),
                                 		TSIP_STACK_SET_MOBILITY("fixed"),
                                 		TSIP_STACK_SET_DEVICE_ID("dd1289fa-c3d7-47bd-a40d-f1f1b2cc5ffc"),
                                 		TSIP_STACK_SET_NETINFO("ADSL;utran-cell-id-3gpp=00000000"),
                                 		TSIP_STACK_SET_PRIVACY("header;id"),
                                 */

                                 TSIP_STACK_SET_NULL());




    //tsip_ssession_handle_t *op = tsip_ssession_create(stack,
    //	TSIP_SSESSION_SET_CONTEXT(usr_context),
    //	TSIP_SSESSION_SET_HEADER("expires", "30"),
    //	TSIP_SSESSION_SET_HEADER("Persistence", "session"),
    //
    //	TSIP_SSESSION_SET_CAPS("language", "\"en,fr\""),
    //	TSIP_SSESSION_SET_CAPS("+audio", ""),
    //	TSIP_SSESSION_SET_CAPS("+g.oma.sip-im", ""),
    //
    //	tsk_null);


    tsip_ssession_handle_t *op = tsip_ssession_create(stack,
                                 TSIP_SSESSION_SET_EXPIRES(30),

                                 TSIP_SSESSION_SET_CAPS("+g.oma.sip-im", ""),
                                 TSIP_SSESSION_SET_CAPS("+audio", ""),
                                 TSIP_SSESSION_SET_CAPS("automata", ""),
                                 TSIP_SSESSION_SET_CAPS("language", "\"en,fr\""),

                                 TSIP_SSESSION_SET_HEADER("Supported", "ssl"),
                                 TSIP_SSESSION_SET_HEADER("P-Access-Network-Info", "3GPP-UTRAN-TDD;utran-cell-id-3gpp=AAAAA0000BBBB"),

                                 TSIP_SSESSION_UNSET_HEADER("P-Access-Network-Info"),

                                 TSIP_SSESSION_SET_NULL());

    //tsip_ssession_id_t opid = tsip_ssession_get_id(op);

    if((ret = tsip_stack_start(stack))) {
        goto bail;
    }

    tsip_action_REGISTER(op,
                         TSIP_ACTION_SET_HEADER("My-Header-1", "My-Value-1"),
                         TSIP_ACTION_SET_HEADER("My-Header-2", "My-Value-1"),

                         TSIP_ACTION_SET_NULL());

    getchar();
    //tsk_thread_sleep(2000);

    tsip_action_UNREGISTER(op,
                           TSIP_ACTION_SET_NULL()
                          );

    getchar();
    tsk_thread_sleep(2000);

    //tsip_unregister(op,
    //	TSIP_ACTION_SET_HEADER("Persistence2", "action"),
    //
    //	tsk_null);
    /*
    	tsip_subscribe(op,
    		TSIP_ACTION_SET_HEADER("Persistence", "action"),

    		tsk_null);

    	tsk_thread_sleep(5000);

    	tsip_unsubscribe(op,
    		TSIP_ACTION_SET_HEADER("Persistence2", "action"),

    		tsk_null);
    	*/

    /* INVITE */
    /*
    {
    	tsip_ssession_handle_t *call = TSIP_SSESSION_CREATE(stack,
    		TSIP_SSESSION_SET_HEADER("to", "sip:samba@"DOMAIN),
    		TSIP_SSESSION_SET_CAPS("+g.3gpp.icsi-ref", "\"urn%3Aurn-7%3gpp-service.ims.icsi.mmtel\""),
    		TSIP_SSESSION_SET_HEADER("Supported", "timer, norefersub, precondition"),
    		TSIP_SSESSION_SET_HEADER("Require", "100rel"),

    		TSIP_SSESSION_SET_PARAM("content", TEST_STACK_SDP),

    	TSIP_SSESSION_SET_NULL());
    	tsip_invite(stack, call);
    }
    */

    /* SUBSCRIBE */
    //{
    //	tsip_ssession_handle_t *ss2 = tsip_ssession_create(stack,
    //		TSIP_SSESSION_SET_CONTEXT(usr_context),
    //		TSIP_SSESSION_SET_HEADER("expires", "35"),
    //		TSIP_SSESSION_SET_HEADER("Event", "reg"),
    //		TSIP_SSESSION_SET_HEADER("Accept", "application/reginfo+xml"),
    //		TSIP_SSESSION_SET_HEADER("Allow-Events", "refer, presence, presence.winfo, xcap-diff"),
    //		TSIP_SSESSION_SET_HEADER("Allow", "INVITE, ACK, CANCEL, BYE, MESSAGE, OPTIONS, NOTIFY, PRACK, UPDATE, REFER"),
    //
    //		tsk_null);

    //	tsip_subscribe(ss2,
    //		TSIP_ACTION_SET_HEADER("Description", "subscribing"),

    //		tsk_null);

    //	getchar();

    //	tsip_unsubscribe(ss2,
    //		TSIP_ACTION_SET_HEADER("Test", "unsubscribing"),
    //
    //		tsk_null);
    //}

    /* MESSAGE */
    //{
    //	tsip_ssession_handle_t *ss3 = TSIP_SSESSION_CREATE(stack,
    //	TSIP_SSESSION_SET_HEADER("to", "sip:alice@"DOMAIN),
    //	TSIP_SSESSION_SET_HEADER("Accept-Contact", "*;+g.oma.sip-im"),
    //
    //	tsk_null);
    //	tsip_message(ss3,
    //		TSIP_ACTION_SET_HEADER("Content-Type", "text/plain"),
    //		TSIP_ACTION_SET_PAYLOAD("Hello world!", tsk_strlen("Hello world!")),

    //		tsk_null
    //		);
    //}

    /* PUBLISH */
    /*{
    	tsip_ssession_handle_t *ss4 = TSIP_SSESSION_CREATE(stack,
    		TSIP_SSESSION_SET_HEADER("expires", "30"),
    		TSIP_SSESSION_SET_HEADER("to", "sip:mamadou@"DOMAIN),

    		TSIP_SSESSION_SET_HEADER("Accept-Contact", "*;+g.oma.sip-im"),
    		TSIP_SSESSION_SET_HEADER("Event", "presence"),

    		tsk_null);

    	tsip_publish(ss4,
    		TSIP_ACTION_SET_HEADER("Description", "publish"),
    		TSIP_ACTION_SET_HEADER("Content-Type", "application/pidf+xml"),
    		TSIP_ACTION_SET_PAYLOAD(TEST_STACK_PIDF, tsk_strlen(TEST_STACK_PIDF)),

    		tsk_null);

    	getchar();

    	tsip_unpublish(ss4,
    		TSIP_ACTION_SET_HEADER("Description", "unpublish"),

    		tsk_null);
    }
    */
    //	/*getchar();
    //	tsip_ssession_set(op4,
    //		TSIP_SSESSION_SET_PARAM("content", TEST_STACK_PIDF),

    //		TSIP_SSESSION_SET_NULL());
    //	tsip_publish(stack, op4);*/
    //
    //	getchar();
    //	tsip_ssession_hangup(op4);
    //
    //}
bail:
    //while(1);//tsk_thread_sleep(500);
    //while(1)
    //tsk_thread_sleep(50000);
    getchar();

    tsip_stack_stop(stack);

    TSK_OBJECT_SAFE_FREE(op);
    TSK_OBJECT_SAFE_FREE(stack);

    TSK_DEBUG_INFO("==================");
    TSK_DEBUG_INFO("==================");
    TSK_DEBUG_INFO("==================");
    TSK_DEBUG_INFO("==================");
}


#endif /* _TEST_STACK_H */
