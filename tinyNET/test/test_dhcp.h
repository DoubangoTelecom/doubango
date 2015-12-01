/* Copyright (C) 2014 Mamadou DIOP.
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
#ifndef TNET_TEST_DHCP_H
#define TNET_TEST_DHCP_H



void test_dhcp_discover(tnet_dhcp_ctx_t *ctx)
{
}

void test_dhcp_offer(tnet_dhcp_ctx_t *ctx)
{
}

void test_dhcp_request(tnet_dhcp_ctx_t *ctx)
{
}

void test_dhcp_inform(tnet_dhcp_ctx_t *ctx)
{
	tnet_dhcp_params_t *params = tsk_null;
	tnet_dhcp_reply_t *reply = tsk_null;

	params = tnet_dhcp_params_create();
	tnet_dhcp_params_add_code(params, dhcp_code_SIP_Servers_DHCP_Option); /* SIP Servers */
	tnet_dhcp_params_add_code(params, dhcp_code_Domain_Server); /* DNS Server */
	
	reply = tnet_dhcp_query_inform(ctx, params);

	if(reply && !TNET_DHCP_MESSAGE_IS_REPLY(reply)){
		TSK_DEBUG_ERROR("DHCP request is not expected in response to a request.");
		goto bail;
	}

	if(reply){
		switch(reply->type)
		{
		case dhcp_type_ack:
			{
				tsk_list_item_t *item;
				TSK_DEBUG_INFO("DHCP response type ==> ACK.");

				tsk_list_foreach(item, reply->options)
				{
					const tnet_dhcp_option_t *option = item->data;

					/* SIP SERVERS */
					if(option->code == dhcp_code_SIP_Servers_DHCP_Option)
					{
						tsk_list_item_t *item2;
						const tnet_dhcp_option_sip_t *option_sip4 = (const tnet_dhcp_option_sip_t*)option;;
						tsk_list_foreach(item2, option_sip4->servers)
						{
							const tsk_string_t *str = item2->data;
							TSK_DEBUG_INFO("DHCP-SIP_SERVER ==>%s", str->value);
						}
					}

					/* DNS SERVERS */
					if(option->code == dhcp_code_Domain_Server)
					{
						tsk_list_item_t *item2;
						const tnet_dhcp_option_dns_t *option_dns = (const tnet_dhcp_option_dns_t*)option;;
						tsk_list_foreach(item2, option_dns->servers)
						{
							const tsk_string_t *str = item2->data;
							TSK_DEBUG_INFO("DHCP-DNS_SERVER ==>%s", str->value);
						}
					}
				}
				break;
			}

		default:
			{
				break;
			}
		}
	}
	else
	{
		TSK_DEBUG_ERROR("DHCP reply is NULL.");
		goto bail;
	}
	
bail:
	TSK_OBJECT_SAFE_FREE(reply);
	TSK_OBJECT_SAFE_FREE(params);

	//tsk_thread_sleep(1000);
}

void test_dhcp()
{
	tnet_dhcp_ctx_t *ctx = tnet_dhcp_ctx_create();
	test_dhcp_inform(ctx);

	TSK_OBJECT_SAFE_FREE(ctx);
}

#endif /* TNET_TEST_DHCP_H */
