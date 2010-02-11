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
/**@file tnet_dhcp6_option.c
 * @brief DHCPv6 Options as per RFC 3315 subclause 22.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tnet_dhcp6_option.h"

int tnet_dhcp6_option_init(tnet_dhcp6_option_t *self, tnet_dhcp6_option_code_t code)
{
	if(self)
	{
		if(!self->initialized)
		{
			self->code = code;
			//option->value = TSK_BUFFER_CREATE_NULL();
			
			self->initialized = 1;
			return 0;
		}
		return -2;
	}
	return -1;
}

int tnet_dhcp6_option_deinit(tnet_dhcp6_option_t *self)
{
	if(self)
	{
		if(self->initialized)
		{
			TSK_OBJECT_SAFE_FREE(self->data);
			
			self->initialized = 0;
			return 0;
		}
		return -2;
	}
	return -1;
}


tnet_dhcp6_option_t* tnet_dhcp6_option_deserialize(const void* data, size_t size)
{
	return 0;
}

int tnet_dhcp6_option_serialize(const tnet_dhcp6_option_t* self, tsk_buffer_t *output)
{
	return -1;
}

int tnet_dhcp6_option_serializeex(tnet_dhcp6_option_code_t code, uint8_t length, const void* value, tsk_buffer_t *output)
{
	return -1;
}

//
//	[[DHCP OPTION]] object definition
//
static void* tnet_dhcp6_option_create(void * self, va_list * app)
{
	tnet_dhcp6_option_t *option = self;
	if(option)
	{
		tnet_dhcp6_option_init(option, va_arg(*app, tnet_dhcp6_option_code_t));
	}
	return self;
}

static void* tnet_dhcp6_option_destroy(void * self) 
{ 
	tnet_dhcp6_option_t *option = self;
	if(option)
	{
		tnet_dhcp6_option_deinit(option);
	}
	return self;
}

static const tsk_object_def_t tnet_dhcp6_option_def_s =
{
	sizeof(tnet_dhcp6_option_t),
	tnet_dhcp6_option_create,
	tnet_dhcp6_option_destroy,
	0,
};
const void *tnet_dhcp6_option_def_t = &tnet_dhcp6_option_def_s;