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
#ifndef TNET_TEST_IFACES_H
#define TNET_TEST_IFACES_H

void test_faces_bestsource()
{
	tnet_ip_t source;

    // IPv6
	if(!tnet_getbestsource("fe80::fe4c:3ea1", 5060, tnet_socket_type_udp_ipv6, &source)){
		TSK_DEBUG_INFO("Best IPv6 source is [%s]", source);
	}
	else{
		TSK_DEBUG_ERROR("Failed to get best IPv6 source.");
	}
    // IPv6
	if(!tnet_getbestsource("2a01:e35:8b32:7050:212:f0ff:fe4c:3ea1", 5060, tnet_socket_type_udp_ipv6, &source)){
		TSK_DEBUG_INFO("Best IPv6 source is [%s]", source);
	}
	else{
		TSK_DEBUG_ERROR("Failed to get best IPv6 source.");
	}
	// IPv4
	if(!tnet_getbestsource("192.168.0.11", 5060, tnet_socket_type_udp_ipv4, &source)){
		TSK_DEBUG_INFO("Best IPv4 source is [%s]", source);
	}
	else{
		TSK_DEBUG_ERROR("Failed to get best IPv4 source.");
	}
}

void test_ifaces_dump_ifaces()
{
	tnet_interfaces_L_t* ifaces = tnet_get_interfaces();
	tsk_list_item_t *item;

	tsk_list_foreach(item, ifaces)
	{
		const tnet_interface_t *iface = item->data;
		TSK_DEBUG_INFO("Interface: %s", iface->description);
	}

	TSK_OBJECT_SAFE_FREE(ifaces);
}

void test_ifaces_dump_addresses()
{
	tnet_addresses_L_t* addresses = tnet_get_addresses_all();
	tsk_list_item_t *item;

	tsk_list_foreach(item, addresses)
	{
		const tnet_address_t *address = item->data;
		if(address->anycast)
		{
			TSK_DEBUG_INFO("ANYCAST address: %s", address->ip);
		}
		else if(address->unicast)
		{
			TSK_DEBUG_INFO("UNICAST address: %s", address->ip);
		}
		else if(address->multicast)
		{
			TSK_DEBUG_INFO("MULTICAST address: %s", address->ip);
		}
		else if(address->dnsserver)
		{
			TSK_DEBUG_INFO("DNSSERVER address: %s", address->ip);
		}
	}

	TSK_OBJECT_SAFE_FREE(addresses);
}

void test_ifaces()
{
	test_faces_bestsource();
	test_ifaces_dump_ifaces();
	test_ifaces_dump_addresses();
}

#endif /* TNET_TEST_IFACES_H */
