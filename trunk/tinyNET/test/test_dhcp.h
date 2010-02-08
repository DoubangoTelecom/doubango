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
	tnet_dhcp_params_t *params = 0;
	tnet_dhcp_reply_t *reply = 0;

	params = TNET_DHCP_PARAMS_CREATE();
	tnet_dhcp_params_add_code(params, dhcp_code_SIP_Servers_DHCP_Option);
	
	reply = tnet_dhcp_query_inform(ctx, params);
	
	TSK_OBJECT_SAFE_FREE(reply);
	TSK_OBJECT_SAFE_FREE(params);
}

void test_dhcp()
{
	tnet_dhcp_ctx_t *ctx = TNET_DHCP_CTX_CREATE();
	test_dhcp_inform(ctx);

	TSK_OBJECT_SAFE_FREE(ctx);
}

#endif /* TNET_TEST_DHCP_H */
