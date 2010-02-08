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
#ifndef _TEST_STACK_H
#define _TEST_STACK_H


#if defined(ANDROID) /* FIXME */
#	define LOCAL_IP	"10.0.2.15"
#else
#	define LOCAL_IP	TNET_SOCKET_HOST_ANY
#endif

int test_stack_callback(const tsip_event_t *sipevent)
{
	//tsip_operation_id_t opid = sipevent->opid;

	// find operation by id

	TSK_DEBUG_INFO("\n====\nSTACK event: %d [%s]\n=====", sipevent->code, sipevent->phrase);

	//tsk_thread_sleep(1000000);

	return 0;
}

int tsip_registration_callback(const tsip_register_event_t *sipevent)
{
	/* common part */
	TSK_DEBUG_INFO("\n====\nREGISTRATION event: %d [%s]\n=====", TSIP_EVENT(sipevent)->code, TSIP_EVENT(sipevent)->phrase);
	/* registration part */
	switch(sipevent->type)
	{
	case tsip_register_ok:
		{
			TSK_DEBUG_INFO("Registration succeed.");
			break;
		}
	case tsip_register_nok:
	case tsip_register_cancelled:
	case tsip_register_auth_nok:
	case tsip_register_secagree_nok:
	case tsip_register_transporterr:
		{
			TSK_DEBUG_INFO("Registration failed.");
			break;
		}
	case tsip_register_terminated:
		{
			TSK_DEBUG_INFO("Registration terminated.");
			break;
		}
	}
	
	return 0;
}

void test_stack()
{
	tsip_stack_handle_t *stack = tsip_stack_create(test_stack_callback, 
		TSIP_STACK_SET_DISPLAY_NAME("2233392625"),
		TSIP_STACK_SET_PUBLIC_IDENTITY("sip:2233392625@sip2sip.info"),
		TSIP_STACK_SET_PRIVATE_IDENTITY("2233392625"),
		TSIP_STACK_SET_PASSWORD("d3sb7j4fb80"),
		TSIP_STACK_SET_REALM("sip:sip2sip.info"),
		TSIP_STACK_SET_LOCAL_IP(LOCAL_IP),
		TSIP_STACK_SET_DISCOVERY_NAPTR(1),
		//TSIP_STACK_SET_PROXY_CSCF("proxy.sipthor.net", "udp", 0),
		//TSIP_STACK_SET_PROXY_CSCF_PORT(5060),
		TSIP_STACK_SET_SEC_AGREE_MECH("ipsec-3gpp"),
		TSIP_STACK_SET_MOBILITY("fixed"),
		TSIP_STACK_SET_DEVICE_ID("DD1289FA-C3D7-47bd-A40D-F1F1B2CC5FFC"),
		TSIP_STACK_SET_NETINFO("ADSL;utran-cell-id-3gpp=00000000"),

		TSIP_STACK_SET_NULL());

	tsip_operation_handle_t *op = TSIP_OPERATION_CREATE(stack,
		TSIP_OPERATION_SET_PARAM("expires", "600000"),

		TSIP_OPERATION_SET_NULL());

	tsip_operation_id_t opid = tsip_operation_get_id(op);

	/* Callbacks */
	tsip_stack_set_callback_register(stack, tsip_registration_callback);

	tsip_stack_start(stack);

	tsip_register(stack, op);

	//while(1);//tsk_thread_sleep(500);
	//while(1)
		//tsk_thread_sleep(50000);
	getchar();

	tsip_stack_stop(stack);

	TSK_OBJECT_SAFE_FREE(op);
	tsip_stack_destroy(stack);
}


#endif /* _TEST_STACK_H */
