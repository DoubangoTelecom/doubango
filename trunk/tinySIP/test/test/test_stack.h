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
#ifndef _TEST_STACK_H
#define _TEST_STACK_H

#include "tinysip/api/tsip_api.h"

#ifndef LOCAL_IP
#	ifdef ANDROID
#		define LOCAL_IP	"10.0.2.15"
#	else
#		define LOCAL_IP	TNET_SOCKET_HOST_ANY
#	endif
#endif

#define TEST_STACK_PIDF \
	"<?xml version=\"1.0\" encoding=\"utf-8\"?>"\
	"<presence xmlns:cp=\"urn:ietf:params:xml:ns:pidf:cipid\" xmlns:caps=\"urn:ietf:params:xml:ns:pidf:caps\" xmlns:rpid=\"urn:ietf:params:xml:ns:pidf:rpid\" xmlns:pdm=\"urn:ietf:params:xml:ns:pidf:data-model\" xmlns:p=\"urn:ietf:params:xml:ns:pidf-diff\" xmlns:op=\"urn:oma:xml:prs:pidf:oma-pres\" entity=\"sip:mamadou@ericsson.com\" xmlns=\"urn:ietf:params:xml:ns:pidf\">"\
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
		sipevent->code, sipevent->phrase, sipevent->opid);

	switch(sipevent->type)
	{
		//
		// REGISTER 
		//
		case tsip_event_register:
			{
				const tsip_register_event_t* _event;
				TSK_DEBUG_INFO("SIP event(REGISTER)");
				
				_event = TSIP_REGISTER_EVENT(sipevent);
				switch(_event->type){
					case tsip_ao_register: /* Answer to Outgoing REGISTER */
						{
							if(TSIP_RESPONSE_IS_2XX(sipevent->sipmessage)){
								TSK_DEBUG_INFO("Registration succeed.");
							}
							else{
								TSK_DEBUG_INFO("Registration failed.");
							}
							break;
						}
					case tsip_ao_unregister: /* Answer to Outgoing UNREGISTER */
						{
							if(TSIP_RESPONSE_IS_2XX(sipevent->sipmessage)){
								TSK_DEBUG_INFO("UnRegistration succeed.");
							}
							else{
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
		case tsip_event_invite:
			{
				TSK_DEBUG_INFO("SIP event(INVITE)");
				break;
			}

		// 
		// MESSAGE
		//
		case tsip_event_message:
			{
				const tsip_message_event_t* _event;
				TSK_DEBUG_INFO("SIP event(MESSAGE)");
				
				_event = TSIP_MESSAGE_EVENT(sipevent);
				switch(_event->type)
				{
					case tsip_i_message: /* Incoming MESSAGE */
						{
							if(TSIP_MESSAGE_HAS_CONTENT(sipevent->sipmessage)){
								TSK_DEBUG_INFO("MESSAGE Content: %s", TSIP_MESSAGE_CONTENT(sipevent->sipmessage));
							}
							break;
						}
					case tsip_ao_message: /* Answer to Outgoing MESSAGE */
						{
							if(TSIP_RESPONSE_IS_2XX(sipevent->sipmessage)){
								TSK_DEBUG_INFO("MESSAGE successfully sent.");
							}
							else{
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
		case tsip_event_publish:
			{
				TSK_DEBUG_INFO("SIP event(PUBLISH)");
				break;
			}
		
		//
		// SUBSCRIBE
		//
		case tsip_event_subscribe:
			{
				const tsip_subscribe_event_t* _event;
				TSK_DEBUG_INFO("SIP event(SUBSCRIBE)");
				
				_event = TSIP_SUBSCRIBE_EVENT(sipevent);
				switch(_event->type)
				{
				case tsip_i_notify: /* Incoming NOTIFY */
					{
						if(TSIP_MESSAGE_HAS_CONTENT(sipevent->sipmessage)){
							TSK_DEBUG_INFO("NOTIFY Content: %s", TSIP_MESSAGE_CONTENT(sipevent->sipmessage));
						}
						break;
					}
				case tsip_ao_subscribe: /* Answer to Outgoing SUBSCRIBE */
					{
						if(TSIP_RESPONSE_IS_2XX(sipevent->sipmessage)){
							TSK_DEBUG_INFO("Subscription succeed.");
						}
						else{
							TSK_DEBUG_INFO("Subscription failed.");
						}
						break;
					}
				case tsip_ao_unsubscribe: /* Answer to Outgoing UNSUBSCRIBE */
					{
						if(TSIP_RESPONSE_IS_2XX(sipevent->sipmessage)){
							TSK_DEBUG_INFO("UnSubscription succeed.");
						}
						else{
							TSK_DEBUG_INFO("UnSubscription failed.");
						}
						break;
					}
				default:
					break;
				}

				break;
			}

		default:
			{
				TSK_DEBUG_INFO("SIP event(UNKNOWN)");
				break;
			}
	}

	//tsk_thread_sleep(1000000);

	return 0;
}


void test_stack()
{
/*
	tsip_stack_handle_t *stack = tsip_stack_create(test_stack_callback, 
		TSIP_STACK_SET_DISPLAY_NAME("2233392625"),
		TSIP_STACK_SET_PUBLIC_IDENTITY("sip:2233392625@sip2sip.info"),
		TSIP_STACK_SET_PRIVATE_IDENTITY("2233392625"),
		TSIP_STACK_SET_PASSWORD("d3sb7j4fb8"),
		TSIP_STACK_SET_REALM("sip:sip2sip.info"), // FIXME: without sip:
		TSIP_STACK_SET_LOCAL_IP(LOCAL_IP),
		//TSIP_STACK_SET_DISCOVERY_NAPTR(1),
		TSIP_STACK_SET_PROXY_CSCF("proxy.sipthor.net", "udp", 0),
		//TSIP_STACK_SET_PROXY_CSCF("192.168.0.15", "udp", 0),
		TSIP_STACK_SET_PROXY_CSCF_PORT(5060),
		TSIP_STACK_SET_MOBILITY("fixed"),
		TSIP_STACK_SET_DEVICE_ID("dd1289fa-c3d7-47bd-a40d-f1f1b2cc5ffc"),
		TSIP_STACK_SET_NETINFO("ADSL;utran-cell-id-3gpp=00000000"),
		TSIP_STACK_SET_PRIVACY("header;id"),
*/

	tsip_stack_handle_t *stack = tsip_stack_create(test_stack_callback, 
		TSIP_STACK_SET_DISPLAY_NAME("Mamadou"),
		TSIP_STACK_SET_PUBLIC_IDENTITY("sip:mamadou@ericsson.com"),
		TSIP_STACK_SET_PRIVATE_IDENTITY("mamadou@ericsson.com"),
		TSIP_STACK_SET_PASSWORD("mamadou"),
		TSIP_STACK_SET_REALM("sip:ericsson.com"), // FIXME: without sip:
		TSIP_STACK_SET_LOCAL_IP(LOCAL_IP),
		//TSIP_STACK_SET_DISCOVERY_NAPTR(1),
		TSIP_STACK_SET_PROXY_CSCF("192.168.0.11", "udp", 0),
		//TSIP_STACK_SET_PROXY_CSCF("192.168.0.15", "udp", 0),
		TSIP_STACK_SET_PROXY_CSCF_PORT(5081),
		//TSIP_STACK_SET_SECAGREE_IPSEC("hmac-md5-96", "null", "trans", "esp"),
		TSIP_STACK_SET_MOBILITY("fixed"),
		TSIP_STACK_SET_DEVICE_ID("dd1289fa-c3d7-47bd-a40d-f1f1b2cc5ffc"),
		TSIP_STACK_SET_NETINFO("ADSL;utran-cell-id-3gpp=00000000"),
		TSIP_STACK_SET_PRIVACY("header;id"),

/*
	tsip_stack_handle_t *stack = tsip_stack_create(test_stack_callback, 
		TSIP_STACK_SET_DISPLAY_NAME("Mamadou"),
		TSIP_STACK_SET_PUBLIC_IDENTITY("sip:mamadou@ims.inexbee.com"),
		TSIP_STACK_SET_PRIVATE_IDENTITY("mamadou@ims.inexbee.com"),
		TSIP_STACK_SET_PASSWORD("mamadou"),
		TSIP_STACK_SET_REALM("sip:ims.inexbee.com"), // FIXME: without sip:
		TSIP_STACK_SET_LOCAL_IP(LOCAL_IP),
		//TSIP_STACK_SET_DISCOVERY_NAPTR(1),
		TSIP_STACK_SET_PROXY_CSCF("pcscf.ims.inexbee.com", "udp", 1),
		//TSIP_STACK_SET_PROXY_CSCF("192.168.0.15", "udp", 0),
		TSIP_STACK_SET_PROXY_CSCF_PORT(4060),
		//TSIP_STACK_SET_SECAGREE_IPSEC("hmac-md5-96", "null", "trans", "esp"),
		TSIP_STACK_SET_MOBILITY("fixed"),
		TSIP_STACK_SET_DEVICE_ID("dd1289fa-c3d7-47bd-a40d-f1f1b2cc5ffc"),
		TSIP_STACK_SET_NETINFO("ADSL;utran-cell-id-3gpp=00000000"),
		TSIP_STACK_SET_PRIVACY("header;id"),
*/

		TSIP_STACK_SET_NULL());

	tsip_operation_handle_t *op = TSIP_OPERATION_CREATE(stack,
		TSIP_OPERATION_SET_PARAM("expires", "30"),
		
		TSIP_OPERATION_SET_CAPS("language", "\"en,fr\""),
		TSIP_OPERATION_SET_CAPS("+audio", ""),
		TSIP_OPERATION_SET_CAPS("+g.oma.sip-im", ""),
		

		TSIP_OPERATION_SET_NULL());

	tsip_operation_id_t opid = tsip_operation_get_id(op);

	tsip_stack_start(stack);

	tsip_register(stack, op);

	tsk_thread_sleep(2000);

	///* SUBSCRIBE */
	//{
	//	tsip_operation_handle_t *op2 = TSIP_OPERATION_CREATE(stack,
	//	TSIP_OPERATION_SET_PARAM("to", "sip:mamadou@ims.inexbee.com"),
	//	TSIP_OPERATION_SET_PARAM("expires", "30"),
	//	TSIP_OPERATION_SET_HEADER("Event", "reg"),
	//	TSIP_OPERATION_SET_HEADER("Accept", "application/reginfo+xml"),
	//	TSIP_OPERATION_SET_HEADER("Allow-Events", "refer, presence, presence.winfo, xcap-diff"),
	//	TSIP_OPERATION_SET_HEADER("Allow", "INVITE, ACK, CANCEL, BYE, MESSAGE, OPTIONS, NOTIFY, PRACK, UPDATE, REFER"),
	//	
	//	TSIP_OPERATION_SET_NULL());
	//	tsip_subscribe(stack, op2);
	//}
	
	///* MESSAGE */
	//{
	//	tsip_operation_handle_t *op3 = TSIP_OPERATION_CREATE(stack,
	//	TSIP_OPERATION_SET_PARAM("to", "sip:laurent@ims.inexbee.com"),
	//	TSIP_OPERATION_SET_HEADER("Accept-Contact", "*;+g.oma.sip-im"),
	//	TSIP_OPERATION_SET_HEADER("Content-Type", "text/plain"),

	//	TSIP_OPERATION_SET_PARAM("content", "test"),
	//	
	//	TSIP_OPERATION_SET_NULL());
	//	tsip_message(stack, op3);
	//}

	/* PUBLISH */
	{
		tsip_operation_handle_t *op4 = TSIP_OPERATION_CREATE(stack,
		TSIP_OPERATION_SET_PARAM("to", "sip:mamadou@ericsson.com"),
		TSIP_OPERATION_SET_HEADER("Content-Type", "application/pidf+xml"),

		TSIP_OPERATION_SET_PARAM("content", TEST_STACK_PIDF),
		
		TSIP_OPERATION_SET_NULL());
		tsip_publish(stack, op4);
	}

	//while(1);//tsk_thread_sleep(500);
	//while(1)
		//tsk_thread_sleep(50000);
	getchar();

	tsip_stack_stop(stack);

	TSK_OBJECT_SAFE_FREE(op);
	tsip_stack_destroy(stack);
}


#endif /* _TEST_STACK_H */
