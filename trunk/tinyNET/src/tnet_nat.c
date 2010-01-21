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

/**@file tnet_nat.c
 * @brief NAT Traversal helper functions using STUN, TURN and ICE.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tnet_nat.h"

#include "tsk_string.h"
#include "tsk_memory.h"


int __pred_find_turn_allocation(const tsk_list_item_t* item, const void* data)
{
	if(item)
	{
		tnet_turn_allocation_t *allocation = item->data;
		tnet_turn_allocation_id_t alloc_id = *((tnet_turn_allocation_id_t*)data);
		if(allocation)
		{
			return (allocation->id == alloc_id) ? 0 : -1;
		}
	}
	return -1;
}

int tnet_nat_set_server_address(tnet_nat_context_handle_t* self, const char* server_address)
{
	tnet_nat_context_t* context = self;

	if(context)
	{
		tsk_strupdate(&(context->server_address), server_address);
		return 0;
	}
	return -1;
}

int tnet_nat_set_server(tnet_nat_context_handle_t* self, const char* server_address,  tnet_port_t server_port)
{
	tnet_nat_context_t* context = self;

	if(context)
	{
		tsk_strupdate(&(context->server_address), server_address);
		context->server_port = server_port;

		return 0;
	}
	return -1;
}

/**
 * @fn	int tnet_nat_stun_bind(const tnet_nat_context_handle_t* self, const tnet_fd_t localFD,
 * 		char** mapped_address, tnet_port_t *mapped_port)
 *
 * @brief	Performs STUN2 binding request to retrieve information about the mapped address. 
 *
 * @author	Mamadou
 * @date	1/16/2010
 *
 * @param [in,out]	self			If non-null, the self. 
 * @param	localFD					The local fd. 
 * @param [in,out]	mapped_address	The mapped address. You must pass a NULL pointer and it is up
 * 									to you to free the returned string. 
 * @param [in,out]	mapped_port		The mapped port. 
 *
 * @return	Zero if succeed and non-zero error code otherwise. 
 *
 * ### param [in,out]	context	The NAT context. 
**/
int tnet_nat_stun_bind(const tnet_nat_context_handle_t* self, const tnet_fd_t localFD, char** mapped_address, tnet_port_t *mapped_port)
{
	const tnet_nat_context_t* context = self;
	int ret = -1;

	if(self)
	{
		tnet_stun_context_t* stun_context = TNET_STUN_CONTEXT_CREATE(localFD, context->socket_type, context->server_address, context->server_port, context->username, context->password);
		if(stun_context)
		{
			ret = tnet_stun_bind(stun_context, mapped_address, mapped_port);
		}
		TSK_OBJECT_SAFE_FREE(stun_context);
	}
	return ret;
}



tnet_turn_allocation_id_t tnet_nat_turn_allocate(const tnet_nat_context_handle_t* self, const tnet_fd_t localFD)
{
	const tnet_nat_context_t* context = self;

	if(self)
	{
		return tnet_turn_allocate(self, localFD, context->socket_type);
	}

	return TNET_TURN_INVALID_ALLOCATION_ID;
}

int tnet_nat_turn_unallocate(const tnet_nat_context_handle_t* self, tnet_turn_allocation_id_t id)
{
	const tnet_nat_context_t* context = self;

	if(self)
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




//========================================================
//	NAT CONTEXT object definition
//
static void* tnet_nat_context_create(void * self, va_list * app)
{
	tnet_nat_context_t *context = self;
	if(context)
	{
		context->socket_type = va_arg(*app, tnet_socket_type_t);
		
		context->username = tsk_strdup(va_arg(*app, const char*));
		context->password = tsk_strdup(va_arg(*app, const char*));

		context->server_port = TNET_NAT_TCP_UDP_DEFAULT_PORT;
		
		context->RTO = TNET_NAT_DEFAULT_RTO;
		context->Rc = TNET_NAT_DEFAULT_RC;
		
		context->software = tsk_strdup(TNET_SOFTWARE);
		//context->enable_evenport = 1;
		//context->enable_fingerprint = 1;
		context->enable_integrity = 0;
		context->enable_dontfrag = 0;//TNET_SOCKET_TYPE_IS_DGRAM(context->socket_type) ? 1 : 0;

		context->allocations = TSK_LIST_CREATE();
	}
	return self;
}

static void* tnet_nat_context_destroy(void * self)
{ 
	tnet_nat_context_t *context = self;
	if(context)
	{
		TSK_FREE(context->username);
		TSK_FREE(context->password);
		TSK_FREE(context->software);
		TSK_FREE(context->server_address);

		TSK_OBJECT_SAFE_FREE(context->allocations);
	}

	return self;
}

static const tsk_object_def_t tnet_nat_context_def_s = 
{
	sizeof(tnet_nat_context_t),
	tnet_nat_context_create, 
	tnet_nat_context_destroy,
	0, 
};
const void *tnet_nat_context_def_t = &tnet_nat_context_def_s;