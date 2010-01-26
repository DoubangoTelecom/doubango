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
#ifndef TNET_TEST_DNS_H
#define TNET_TEST_DNS_H

void test_dns_query()
{
	tnet_dns_t *ctx = TNET_DNS_CREATE();
	tnet_dns_response_t *response = 0;

	response = tnet_dns_resolve(ctx, "ims.inexbee.com", qclass_in, qtype_naptr);

	TSK_OBJECT_SAFE_FREE(response);
	TSK_OBJECT_SAFE_FREE(ctx);
}

void test_dns()
{
	test_dns_query();
}


#endif /* TNET_TEST_DNS_H */