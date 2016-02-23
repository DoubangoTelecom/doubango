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
#ifndef TNET_TEST_DHCP6_H
#define TNET_TEST_DHCP6_H



void test_dhcp6_requestinfo(tnet_dhcp6_ctx_t *ctx)
{
    tnet_dhcp6_option_orequest_t *orequest = tnet_dhcp6_option_orequest_create_null();
    tnet_dhcp6_reply_t* reply = 0;

    tnet_dhcp6_option_orequest_add_code(orequest, 24);
    tnet_dhcp6_option_orequest_add_code(orequest, 23);
    tnet_dhcp6_option_orequest_add_code(orequest, 21); /* SIP Servers Domain Name List */
    tnet_dhcp6_option_orequest_add_code(orequest, 22); /* SIP Servers IPv6 Address List */

    reply = tnet_dhcp6_requestinfo(ctx, orequest);

    TSK_OBJECT_SAFE_FREE(orequest);
    TSK_OBJECT_SAFE_FREE(reply);
}

void test_dhcp6()
{
    tnet_dhcp6_ctx_t *ctx = tnet_dhcp6_ctx_create();
    test_dhcp6_requestinfo(ctx);

    TSK_OBJECT_SAFE_FREE(ctx);
}

#endif /* TNET_TEST_DHCP6_H */
