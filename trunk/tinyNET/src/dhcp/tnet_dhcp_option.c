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
/**@file tnet_dhcp_option.c
 * @brief DHCP Options and BOOTP Vendor Extensions as per RFC 2132.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tnet_dhcp_option.h"


int tnet_dhcp_option_init(tnet_dhcp_option_t *option, tnet_dhcp_option_tag_t tag)
{
	if(option)
	{
		if(!option->initialized)
		{
			option->tag = tag;
			option->data = TSK_BUFFER_CREATE_NULL();
			
			option->initialized = 1;
			return 0;
		}
		return -2;
	}
	return -1;
}

int tnet_dhcp_option_deinit(tnet_dhcp_option_t *option)
{
	if(option)
	{
		if(option->initialized)
		{
			TSK_OBJECT_SAFE_FREE(option->data);
			
			option->initialized = 0;
			return 0;
		}
		return -2;
	}
	return -1;
}

tnet_dhcp_option_t* tnet_dhcp_option_deserialize(const void* data, size_t size)
{
	tnet_dhcp_option_t *option = 0;
	uint8_t* dataPtr = ((uint8_t*)data);
	uint8_t* dataEnd = (dataPtr+size);

	/* Check validity */
	if(!dataPtr || !size)
	{
		goto bail;
	}

bail:
	return option;
}

int tnet_dhcp_option_serialize(const tnet_dhcp_option_t* option, tsk_buffer_t *output)
{
	return 0;
}


//========================================================
//	[[DHCP OPTION]] object definition
//
static void* tnet_dhcp_option_create(void * self, va_list * app)
{
	tnet_dhcp_option_t *option = self;
	if(option)
	{
		tnet_dhcp_option_init(option, dhcp_tag_reserved);
	}
	return self;
}

static void* tnet_dhcp_option_destroy(void * self) 
{ 
	tnet_dhcp_option_t *option = self;
	if(option)
	{
		tnet_dhcp_option_deinit(option);
	}
	return self;
}

static const tsk_object_def_t tnet_dhcp_option_def_s =
{
	sizeof(tnet_dhcp_option_t),
	tnet_dhcp_option_create,
	tnet_dhcp_option_destroy,
	0,
};
const void *tnet_dhcp_option_def_t = &tnet_dhcp_option_def_s;