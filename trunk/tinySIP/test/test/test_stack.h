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


#ifndef LOCAL_IP
#	ifdef ANDROID
#		define LOCAL_IP	"10.0.2.15"
#	else
#		define LOCAL_IP	TNET_SOCKET_HOST_ANY
#	endif
#endif

int test_stack_callback(const tsip_event_t *sipevent)
{
	//tsip_operation_id_t opid = sipevent->opid;

	// find operation by id

	TSK_DEBUG_INFO("\n====\nSTACK event: %d [%s]\n=====", sipevent->code, sipevent->phrase);

	//tsk_thread_sleep(1000000);

	return 0;
}

int tsip_subscription_callback(const tsip_subscribe_event_t *sipevent)
{
	/* common part */
	TSK_DEBUG_INFO("\n====\nSUBSCRIBTION event: %d [%s]\n=====", TSIP_EVENT(sipevent)->code, TSIP_EVENT(sipevent)->phrase);
	/* subscribtion part */
	switch(sipevent->type)
	{
	case tsip_i_notify:
		{
			if(TSIP_MESSAGE_HAS_CONTENT(TSIP_EVENT(sipevent)->sipmessage))
			{
				TSK_DEBUG_INFO("NOTIFY Content: %s", TSIP_MESSAGE_CONTENT(TSIP_EVENT(sipevent)->sipmessage));
			}
			break;
		}
	default:
		break;
	}
	return 0;
}

int tsip_messaging_callback(const tsip_message_event_t *sipevent)
{
	/* common part */
	TSK_DEBUG_INFO("\n====\nMESSAGE event: %d [%s]\n=====", TSIP_EVENT(sipevent)->code, TSIP_EVENT(sipevent)->phrase);
	/* message part */
	switch(sipevent->type)
	{
	case tsip_i_message:
		{
			if(TSIP_MESSAGE_HAS_CONTENT(TSIP_EVENT(sipevent)->sipmessage))
			{
				TSK_DEBUG_INFO("MESSAGE Content: %s", TSIP_MESSAGE_CONTENT(TSIP_EVENT(sipevent)->sipmessage));
			}
			break;
		}
	default:
		break;
	}
	return 0;
}

int tsip_registration_callback(const tsip_register_event_t *sipevent)
{
	/* common part */
	TSK_DEBUG_INFO("\n====\nREGISTRATION event: %d [%s]\n=====", TSIP_EVENT(sipevent)->code, TSIP_EVENT(sipevent)->phrase);
	/* registration part */
	switch(sipevent->type)
	{
	case tsip_ao_register:
		{
			if(TSIP_RESPONSE_IS_2XX(TSIP_EVENT(sipevent)->sipmessage)){
				TSK_DEBUG_INFO("Registration succeed.");
			}
			else{
				TSK_DEBUG_INFO("Registration failed.");
			}
			break;
		}

	case tsip_ao_unregister:
		{
			if(TSIP_RESPONSE_IS_2XX(TSIP_EVENT(sipevent)->sipmessage)){
				TSK_DEBUG_INFO("Unregistration succeed.");
			}
			else{
				TSK_DEBUG_INFO("Unregistration failed.");
			}
			break;
		}

	default:
		break;
	}
	return 0;
}

void test_stack()
{
	
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
		TSIP_STACK_SET_SEC_AGREE_MECH("ipsec-3gpp"),
		TSIP_STACK_SET_MOBILITY("fixed"),
		TSIP_STACK_SET_DEVICE_ID("DD1289FA-C3D7-47bd-A40D-F1F1B2CC5FFC"),
		TSIP_STACK_SET_NETINFO("ADSL;utran-cell-id-3gpp=00000000"),
	
/*
	tsip_stack_handle_t *stack = tsip_stack_create(test_stack_callback, 
		TSIP_STACK_SET_DISPLAY_NAME("Mamadou"),
		TSIP_STACK_SET_PUBLIC_IDENTITY("sip:mamadou@ericsson.com"),
		TSIP_STACK_SET_PRIVATE_IDENTITY("mamadou@ericsson.com"),
		TSIP_STACK_SET_PASSWORD("mamadou"),
		TSIP_STACK_SET_REALM("sip:ericsson.com"), // FIXME: without sip:
		TSIP_STACK_SET_LOCAL_IP(LOCAL_IP),
		//TSIP_STACK_SET_DISCOVERY_NAPTR(1),
		TSIP_STACK_SET_PROXY_CSCF("192.168.0.11", "tcp", 0),
		//TSIP_STACK_SET_PROXY_CSCF("192.168.0.15", "udp", 0),
		TSIP_STACK_SET_PROXY_CSCF_PORT(5081),
		TSIP_STACK_SET_SEC_AGREE_MECH("ipsec-3gpp"),
		TSIP_STACK_SET_MOBILITY("fixed"),
		TSIP_STACK_SET_DEVICE_ID("DD1289FA-C3D7-47bd-A40D-F1F1B2CC5FFC"),
		TSIP_STACK_SET_NETINFO("ADSL;utran-cell-id-3gpp=00000000"),
*/
	/*
	tsip_stack_handle_t *stack = tsip_stack_create(test_stack_callback, 
		TSIP_STACK_SET_DISPLAY_NAME("Mamadou"),
		TSIP_STACK_SET_PUBLIC_IDENTITY("sip:mamadou@ims.inexbee.com"),
		TSIP_STACK_SET_PRIVATE_IDENTITY("mamadou@ims.inexbee.com"),
		TSIP_STACK_SET_PASSWORD("mamadou"),
		TSIP_STACK_SET_REALM("sip:ims.inexbee.com"), // FIXME: without sip:
		TSIP_STACK_SET_LOCAL_IP(LOCAL_IP),
		//TSIP_STACK_SET_DISCOVERY_NAPTR(1),
		TSIP_STACK_SET_PROXY_CSCF("192.168.16.225", "tcp", 0),
		//TSIP_STACK_SET_PROXY_CSCF("192.168.0.15", "udp", 0),
		TSIP_STACK_SET_PROXY_CSCF_PORT(4060),
		TSIP_STACK_SET_SEC_AGREE_MECH("ipsec-3gpp"),
		TSIP_STACK_SET_MOBILITY("fixed"),
		TSIP_STACK_SET_DEVICE_ID("DD1289FA-C3D7-47bd-A40D-F1F1B2CC5FFC"),
		TSIP_STACK_SET_NETINFO("ADSL;utran-cell-id-3gpp=00000000"),
*/
		TSIP_STACK_SET_NULL());

	tsip_operation_handle_t *op = TSIP_OPERATION_CREATE(stack,
		TSIP_OPERATION_SET_PARAM("expires", "30"),

		TSIP_OPERATION_SET_NULL());

	tsip_operation_id_t opid = tsip_operation_get_id(op);

	/* Callbacks */
	tsip_stack_set_callback_register(stack, tsip_registration_callback);
	tsip_stack_set_callback_subscribe(stack, tsip_subscription_callback);
	tsip_stack_set_callback_message(stack, tsip_messaging_callback);

	tsip_stack_start(stack);

	tsip_register(stack, op);

	tsk_thread_sleep(2000);

	{
		tsip_operation_handle_t *op2 = TSIP_OPERATION_CREATE(stack,
		TSIP_OPERATION_SET_PARAM("expires", "30"),
		TSIP_OPERATION_SET_PARAM("package", "reg"),
		TSIP_OPERATION_SET_PARAM("accept", "application/reginfo+xml"),
		TSIP_OPERATION_SET_PARAM("to", "sip:mamadou@sip2sip.info"),
		
		TSIP_OPERATION_SET_NULL());
		tsip_subscribe(stack, op2);
	}

	//{
	//	tsip_operation_handle_t *op3 = TSIP_OPERATION_CREATE(stack,
	//	TSIP_OPERATION_SET_PARAM("to", "sip:laurent@ims.inexbee.com"),
	//	TSIP_OPERATION_SET_PARAM("content", "test"),
	//	TSIP_OPERATION_SET_PARAM("content-type", "text/plain"),
	//	
	//	TSIP_OPERATION_SET_NULL());
	//	tsip_message(stack, op3);
	//}

	//while(1);//tsk_thread_sleep(500);
	//while(1)
		//tsk_thread_sleep(50000);
	getchar();

	tsip_stack_stop(stack);

	TSK_OBJECT_SAFE_FREE(op);
	tsip_stack_destroy(stack);
}


#endif /* _TEST_STACK_H */
