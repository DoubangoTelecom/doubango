/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango.org>
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

/**@file tnet_nat.c
 * @brief NAT Traversal helper functions using STUN, TURN and ICE.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#include "tnet_nat.h"

#include "tnet_endianness.h"

#include "tsk_string.h"
#include "tsk_memory.h"

#include "tsk_debug.h"

/**@defgroup tnet_nat_group NAT Traversal API (STUN, TURN and ICE).
*/


/**@ingroup tnet_nat_group
* Creates new NAT context.
*/
tnet_nat_context_handle_t* tnet_nat_context_create(tnet_socket_type_t socket_type, const char* username, const char* password)
{
	return tsk_object_new(tnet_nat_context_def_t, socket_type, username, password);
}

/**
 * Predicate function to find turn allocation by id.
 *
 * @param [in,out]	item	The current list item. 
 * @param [in,out]	id	A pointer to the allocation identifier. 
 *
 * @return	Zero if current list item hold an allocation with the same id and -1 otherwise. 
**/
int __pred_find_turn_allocation(const tsk_list_item_t* item, const void* id)
{
	if(item)
	{
		tnet_turn_allocation_t *allocation = item->data;
		if(allocation)
		{
			tnet_turn_allocation_id_t alloc_id = *((tnet_turn_allocation_id_t*)id);
			return (allocation->id == alloc_id) ? 0 : -1;
		}
	}
	return -1;
}

/** Predicate function to find stun binding by id. 
 *
 * @param [in,out]	item	The current list item. 
 * @param [in,out]	id		A pointer to the binding identifier. 
 *
 * @return	Zero if current list item hold a binding with the same id and -1 otherwise.  
**/
int __pred_find_stun_binding(const tsk_list_item_t* item, const void* id)
{
	if(item)
	{
		tnet_stun_binding_t *binding = item->data;
		if(binding)
		{
			tnet_stun_binding_id_t binding_id = *((tnet_stun_binding_id_t*)id);
			return (binding->id == binding_id) ? 0 : -1;
		}
	}
	return -1;
}

/** Predicate function to find TURN channel binding by id. 
 *
 * @param [in,out]	item	The current list item. 
 * @param [in,out]	id		A pointer to the TURN channel binding identifier. 
 *
 * @return	Zero if current list item hold a TURN channel binding with the same id and -1 otherwise. 
**/
int __pred_find_turn_channel_binding(const tsk_list_item_t* item, const void* id)
{
	if(item)
	{
		tnet_turn_channel_binding_t *binding = item->data;
		if(binding)
		{
			tnet_turn_channel_binding_id_t binding_id = *((tnet_turn_channel_binding_id_t*)id);
			return (binding->id == binding_id) ? 0 : -1;
		}
	}
	return -1;
}

/**
 * Formats binary IP address as string.
 *
 * @param	in_ip			The binary IP address to format (in Host byte order). 
 * @param	family			The address family. 
 * @param [in,out]	out_ip	The output string 
 *
 * @return	Zero if current list item hold a binding with the same id and -1 otherwise. 
**/
int tnet_stun_address_tostring(const uint8_t in_ip[16], tnet_stun_addr_family_t family, char** out_ip)
{
	/*if(family == stun_ipv6){
		tsk_sprintf(out_ip, "%x:%x:%x:%x:%x:%x:%x:%x",
				tnet_ntohs_2(&in_ip[0]), tnet_ntohs_2(&in_ip[2]), tnet_ntohs_2(&in_ip[4]), tnet_ntohs_2(&in_ip[6]),
				tnet_ntohs_2(&in_ip[8]), tnet_ntohs_2(&in_ip[10]), tnet_ntohs_2(&in_ip[12]), tnet_ntohs_2(&in_ip[14]));
	}
	else if(family == stun_ipv4){
		tsk_sprintf(out_ip, "%u.%u.%u.%u", (address>>24)&0xFF, (address>>16)&0xFF, (address>>8)&0xFF, (address>>0)&0xFF);
		
		return 0;
	}
	else{
		TSK_DEBUG_ERROR("Unsupported address family: %u.", family);
	}*/
	if(family == stun_ipv6){
		tsk_sprintf(out_ip, "%x:%x:%x:%x:%x:%x:%x:%x",
				TSK_TO_UINT16(&in_ip[0]), TSK_TO_UINT16(&in_ip[2]), TSK_TO_UINT16(&in_ip[4]), TSK_TO_UINT16(&in_ip[6]),
				TSK_TO_UINT16(&in_ip[8]), TSK_TO_UINT16(&in_ip[10]), TSK_TO_UINT16(&in_ip[12]), TSK_TO_UINT16(&in_ip[14]));
	}
	else if(family == stun_ipv4){
		tsk_sprintf(out_ip, "%u.%u.%u.%u", in_ip[0], in_ip[1], in_ip[2], in_ip[3]);
		
		return 0;
	}
	else{
		TSK_DEBUG_ERROR("Unsupported address family: %u.", family);
	}

	return -1;
}


/**@ingroup tnet_nat_group
 *
 * Sets the address of the STUN/TURN server.
 *
 * @param [in,out]	self			The NAT context. 
 * @param [in,out]	server_address	The address of server. 
 *
 * @return	Zero if succeed and non zero error code otherwise. 
**/
int tnet_nat_set_server_address(tnet_nat_context_handle_t* self, const char* server_address)
{
	tnet_nat_context_t* context = self;

	if(context){
		tsk_strupdate(&(context->server_address), server_address);
		return 0;
	}
	return -1;
}

/**@ingroup tnet_nat_group
 *
 * Sets the address and port of the STUN/TURN server.
 *
 * @param [in,out]	self			The NAT context. 
 * @param [in,out]	server_address	The address of server. 
 * @param	server_port				The server port. 
 *
 * @return	Zero if succeed and non zero error code otherwise.  
**/
int tnet_nat_set_server(tnet_nat_context_handle_t* self, const char* server_address,  tnet_port_t server_port)
{
	tnet_nat_context_t* context = self;

	if(context){
		tsk_strupdate(&(context->server_address), server_address);
		context->server_port = server_port;

		return 0;
	}
	return -1;
}

/**@ingroup tnet_nat_group
 *
 * Creates and sends a STUN2 binding request to the STUN/TURN server in order to get the server reflexive
 *			address associated to this file descriptor (or socket). The caller should call @ref tnet_nat_stun_unbind to destroy the binding.
 *
 * @param [in,out]	self	The NAT context.  
 * @param	localFD			The local file descriptor (or socket) for which to get the reflexive server address.
 *
 * @return	A valid binding id if succeed and @ref TNET_STUN_INVALID_BINDING_ID otherwise. If the returned id is valid then
 *			the newly created binding will contain the server-reflexive address associated to the local file descriptor.
 *
 * @sa @ref tnet_nat_stun_unbind.
**/
tnet_stun_binding_id_t tnet_nat_stun_bind(const tnet_nat_context_handle_t* self, const tnet_fd_t localFD)
{
	const tnet_nat_context_t* context = self;
	if(context){
		return tnet_stun_bind(context, localFD);
	}
	return TNET_STUN_INVALID_BINDING_ID;
}

/**@ingroup tnet_nat_group
 * Gets the server reflexive address associated to this STUN2 binding.
 *
 *
 * @param [in,out]	self		The NAT context. 
 * @param	id					The id of the STUN2 binding conetxt (obtained using @ref tnet_nat_stun_bind) holding the server-reflexive address. 
 * @param [in,out]	ipaddress	The reflexive IP address. It is up the the caller to free the returned string 
 * @param [in,out]	port		The reflexive port. 
 *
 * @return	Zero if succeed and non zero error code otherwise. 
**/
int tnet_nat_stun_get_reflexive_address(const tnet_nat_context_handle_t* self, tnet_stun_binding_id_t id, char** ipaddress, tnet_port_t *port)
{
	const tnet_nat_context_t* context = self;
	if(context){
		const tsk_list_item_t* item = tsk_list_find_item_by_pred(context->stun_bindings, __pred_find_stun_binding, &id);
		if(item && item->data){
			tnet_stun_binding_t *binding = item->data;
			/*STUN2: XOR-MAPPED-ADDRESS */
			if(binding->xmaddr){
				int ret = 0;
				if(ipaddress){
					ret = tnet_stun_address_tostring(binding->xmaddr->xaddress, binding->xmaddr->family, ipaddress);
				}
				if(port){
					*port = /*tnet_ntohs*/(binding->xmaddr->xport);
				}
				return ret;
			}

			/*STUN1: MAPPED-ADDRESS*/
			if(binding->maddr){
				int ret = 0;
				if(ipaddress){
					ret = tnet_stun_address_tostring(binding->maddr->address, binding->maddr->family, ipaddress);
				}
				if(port){
					*port = /*tnet_ntohs*/(binding->maddr->port);
				}
				return ret;
			}
		}
	}
	return -1;
}

/**@ingroup tnet_nat_group
 *
 * Removes a STUN2 binding from the NAT context.
 *
 * @param [in,out]	self	The NAT context from which to remove the STUN2 binding. 
 * @param	id				The id of the STUN2 binding to remove. 
 *
 * @return	Zero if succeed and non zero error code otherwise.
 *
 *
 * @sa @ref tnet_nat_stun_bind.
**/
int tnet_nat_stun_unbind(const tnet_nat_context_handle_t* self, tnet_stun_binding_id_t id)
{
	const tnet_nat_context_t* context = self;
	if(context)
	{
		tsk_list_remove_item_by_pred(context->stun_bindings, __pred_find_stun_binding, &id);
		return 0;
	}
	return -1;
}


















/**@ingroup tnet_nat_group
 *
 * Creates TURN allocation as per draft-ietf-behave-turn-16 subclause 6. This function  will also 
 *			send an allocation request to the server (subclause 6.1).
 *
 * @param [in,out]	self	The NAT context. 
 * @param	localFD			The local file descriptor. 
 *
 * @return	A valid TURN allocation id if succeed and @ref TNET_TURN_INVALID_ALLOCATION_ID otherwise.
 *
 * @sa @ref tnet_nat_turn_unallocate.
**/
tnet_turn_allocation_id_t tnet_nat_turn_allocate(const tnet_nat_context_handle_t* self, const tnet_fd_t localFD)
{
	const tnet_nat_context_t* context = self;

	if(self)
	{
		return tnet_turn_allocate(self, localFD, context->socket_type);
	}

	return TNET_TURN_INVALID_ALLOCATION_ID;
}

/**@ingroup tnet_nat_group
 * Gets the STUN server-refelexive IP address and port associated to this TURN allocation.
 *
 * @param [in,out]	self		The NAT context. 
 * @param	id					The id of the TURN allocation for which to to get server-reflexive IP address and port. 
 * @param [in,out]	ipaddress	The server-reflexive IP address. 
 * @param [in,out]	port		The server-reflexive port. 
 *
 * @return	Zero if succeed and non zero error code otherwise. 
**/
int tnet_nat_turn_get_reflexive_address(const tnet_nat_context_handle_t* self, tnet_turn_allocation_id_t id, char** ipaddress, tnet_port_t *port)
{
	const tnet_nat_context_t* context = self;
	if(context)
	{
		const tsk_list_item_t* item = tsk_list_find_item_by_pred(context->allocations, __pred_find_turn_allocation, &id);
		if(item && item->data)
		{
			tnet_turn_allocation_t *allocation = item->data;
			/*STUN2: XOR-MAPPED-ADDRESS */
			if(allocation->xmaddr)
			{
				int ret = tnet_stun_address_tostring(allocation->xmaddr->xaddress, allocation->xmaddr->family, ipaddress);
				*port = /*tnet_ntohs*/(allocation->xmaddr->xport);
				return ret;
			}

			/*STUN1: MAPPED-ADDRESS*/
			if(allocation->maddr)
			{
				int ret = tnet_stun_address_tostring(allocation->maddr->address, allocation->maddr->family, ipaddress);
				*port = /*tnet_ntohs*/(allocation->maddr->port);
				return ret;
			}
		}
	}
	return -1;
}

/**@ingroup tnet_nat_group
 *
 * Refresh a TURN allocation previously created using @ref tnet_nat_turn_allocate.
 *
 * @param [in,out]	self	The NAT context.
 * @param	id				The id of the TURN allocation to refresh. 
 *
 * @return	Zero if succeed and non zero error code otherwise. 
**/
int tnet_nat_turn_allocation_refresh(const tnet_nat_context_handle_t* self, tnet_turn_allocation_id_t id)
{
	const tnet_nat_context_t* context = self;
	
	if(context)
	{
		const tsk_list_item_t* item = tsk_list_find_item_by_pred(context->allocations, __pred_find_turn_allocation, &id);
		if(item && item->data)
		{
			tnet_turn_allocation_t *allocation = item->data;
			return tnet_turn_allocation_refresh(self, allocation);
		}
	}
	return -1;
}

/**@ingroup tnet_nat_group
 *
 * Unallocate/remove a TURN allocation from the server.
 *
 * @param [in,out]	self	The NAT context from which to remove the allocation. 
 * @param	id				The id of the TURN allocation to remove. 
 *
 * @return	Zero if succeed and non zero error code otherwise.
 *
 * @sa @ref tnet_nat_turn_allocate.
**/
int tnet_nat_turn_unallocate(const tnet_nat_context_handle_t* self, tnet_turn_allocation_id_t id)
{
	const tnet_nat_context_t* context = self;

	if(context)
	{
		const tsk_list_item_t* item = tsk_list_find_item_by_pred(context->allocations, __pred_find_turn_allocation, &id);
		if(item && item->data)
		{
			tnet_turn_allocation_t *allocation = item->data;
			return tnet_turn_unallocate(self, allocation);
		}
	}
	return -1;
}

/**@ingroup tnet_nat_group
 * Creates TURN channel binding as per draft-ietf-behave-turn-16 sublause 11 and send it to the
 *			server as per subclause 11.1.
 *
 *
 * @param [in,out]	self	The NAT context. 
 * @param	id				The id of the TURN allocation associated to this binding. 
 * @param [in,out]	peer	The XOR remote peer for the channel binding. 
 *
 * @return	A valid TURN channel binding id if succeed and @ref TNET_TURN_INVALID_CHANNEL_BINDING_ID otherwise.
**/
tnet_turn_channel_binding_id_t tnet_nat_turn_channel_bind(const tnet_nat_context_handle_t* self, tnet_turn_allocation_id_t id, struct sockaddr_storage *peer)
{
	const tnet_nat_context_t* context = self;

	if(context)
	{
		const tsk_list_item_t* item = tsk_list_find_item_by_pred(context->allocations, __pred_find_turn_allocation, &id);
		if(item && item->data)
		{
			tnet_turn_allocation_t *allocation = item->data;
			return tnet_turn_channel_bind(self, allocation, peer);
		}
	}
	return TNET_TURN_INVALID_CHANNEL_BINDING_ID;
}

/**@ingroup tnet_nat_group
*/
int tnet_nat_turn_channel_refresh(const tnet_nat_context_handle_t* self, tnet_turn_channel_binding_id_t id)
{
	const tnet_nat_context_t* context = self;
	
	if(context)
	{
		tsk_list_item_t* curr;
		tsk_list_foreach(curr, context->allocations)
		{
			const tsk_list_item_t* item = tsk_list_find_item_by_pred(((tnet_turn_allocation_t *)curr->data)->channel_bindings, __pred_find_turn_channel_binding, &id);
			if(item && item->data)
			{
				return tnet_turn_channel_refresh(context, (tnet_turn_channel_binding_t *)item->data);
			}
		}
	}
	return -1;
}

/**@ingroup tnet_nat_group
*/
int tnet_nat_turn_channel_send(const tnet_nat_context_handle_t* self, tnet_turn_channel_binding_id_t id, const void* data, tsk_size_t size, int indication)
{
	const tnet_nat_context_t* context = self;
	
	if(context && data && size)
	{
		tsk_list_item_t* curr;
		tsk_list_foreach(curr, context->allocations)
		{
			const tsk_list_item_t* item = tsk_list_find_item_by_pred(((tnet_turn_allocation_t *)curr->data)->channel_bindings, __pred_find_turn_channel_binding, &id);
			if(item && item->data)
			{
				return tnet_turn_channel_senddata(context, (tnet_turn_channel_binding_t *)item->data, data, size, indication);
			}
		}
	}
	return -1;
}

/**@ingroup tnet_nat_group
*/
int tnet_nat_turn_add_permission(const tnet_nat_context_handle_t* self, tnet_turn_allocation_id_t id, const char* ipaddress, uint32_t timeout)
{
	const tnet_nat_context_t* context = self;

	if(self)
	{
		const tsk_list_item_t* item = tsk_list_find_item_by_pred(context->allocations, __pred_find_turn_allocation, &id);
		if(item && item->data)
		{
			tnet_turn_allocation_t *allocation = item->data;
			return tnet_turn_add_permission(self, allocation, ipaddress, timeout);
		}
	}
	return -1;
}


//=================================================================================================
//	NAT CONTEXT object definition
//
static tsk_object_t* tnet_nat_context_ctor(tsk_object_t * self, va_list * app)
{
	tnet_nat_context_t *context = self;
	if(context){
		context->socket_type = va_arg(*app, tnet_socket_type_t);
		
		context->username = tsk_strdup(va_arg(*app, const char*));
		context->password = tsk_strdup(va_arg(*app, const char*));

		context->server_port = TNET_NAT_TCP_UDP_DEFAULT_PORT;
		
		/*	7.2.1.  Sending over UDP
			In fixed-line access links, a value of 500 ms is RECOMMENDED.
		*/
		context->RTO = TNET_NAT_DEFAULT_RTO;

		/*	7.2.1.  Sending over UDP
			Rc SHOULD be configurable and SHOULD have a default of 7.
		*/
		context->Rc = TNET_NAT_DEFAULT_RC;
		
		context->software = tsk_strdup(TNET_SOFTWARE);
		//context->enable_evenport = 1;
		//context->enable_fingerprint = 1;
		context->enable_integrity = 0;
		context->enable_dontfrag = 0;//TNET_SOCKET_TYPE_IS_DGRAM(context->socket_type) ? 1 : 0;

		context->allocations = tsk_list_create();
		context->stun_bindings = tsk_list_create();
	}
	return self;
}

static tsk_object_t* tnet_nat_context_dtor(tsk_object_t * self)
{ 
	tnet_nat_context_t *context = self;
	if(context){
		TSK_FREE(context->username);
		TSK_FREE(context->password);
		TSK_FREE(context->software);
		TSK_FREE(context->server_address);

		TSK_OBJECT_SAFE_FREE(context->allocations);
		TSK_OBJECT_SAFE_FREE(context->stun_bindings);
	}

	return self;
}

static const tsk_object_def_t tnet_nat_context_def_s = 
{
	sizeof(tnet_nat_context_t),
	tnet_nat_context_ctor, 
	tnet_nat_context_dtor,
	tsk_null, 
};
const tsk_object_def_t *tnet_nat_context_def_t = &tnet_nat_context_def_s;
