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


int tnet_dhcp_option_init(tnet_dhcp_option_t *option, tnet_dhcp_option_code_t code)
{
	if(option)
	{
		if(!option->initialized)
		{
			option->code = code;
			option->value = TSK_BUFFER_CREATE_NULL();
			
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
			TSK_OBJECT_SAFE_FREE(option->value);
			
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
	if(!option || !output)
	{
		return -1;
	}
	
	/* Code */
	tsk_buffer_append(output, &(option->code), 1);
	
	/* Length */
	tsk_buffer_append(output, &(option->value->size), 1);
	
	/* Value */
	tsk_buffer_append(output, option->value->data, option->value->size);
	
	return 0;
}

int tnet_dhcp_option_serializeex(tnet_dhcp_option_code_t code, uint8_t length, const void* value, tsk_buffer_t *output)
{
	if(value && length && output)
	{
		tsk_buffer_append(output, &(code), 1);
		tsk_buffer_append(output, &(length), 1);
		tsk_buffer_append(output, value, length);
		
		return 0;
	}
	return -1;
}

//
//	[[DHCP OPTION]] object definition
//
static void* tnet_dhcp_option_create(void * self, va_list * app)
{
	tnet_dhcp_option_t *option = self;
	if(option)
	{
		tnet_dhcp_option_init(option, dhcp_code_null);
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



/*=============================================
*	RFC 2132 - 9.8. Parameter Request List
*==============================================*/
int tnet_dhcp_option_paramslist_add_code(tnet_dhcp_option_paramslist_t* self, tnet_dhcp_option_code_t code)
{
	if(self){
		return tsk_buffer_append(TNET_DHCP_OPTION(self)->value, &code, 1);
	}
	return -1;
}

//
//	[[DHCP OPTION - RFC 2132 9.8. Parameter Request List]] object definition
//
static void* tnet_dhcp_option_paramslist_create(void * self, va_list * app)
{
	tnet_dhcp_option_paramslist_t *option = self;
	if(option)
	{
		/* init base */
		tnet_dhcp_option_init(TNET_DHCP_OPTION(option), dhcp_code_Parameter_List);
	}
	return self;
}

static void* tnet_dhcp_option_paramslist_destroy(void * self) 
{ 
	tnet_dhcp_option_paramslist_t *option = self;
	if(option)
	{
		/* deinit base */
		tnet_dhcp_option_deinit(TNET_DHCP_OPTION(option));
	}
	return self;
}

static const tsk_object_def_t tnet_dhcp_option_paramslist_def_s =
{
	sizeof(tnet_dhcp_option_paramslist_t),
	tnet_dhcp_option_paramslist_create,
	tnet_dhcp_option_paramslist_destroy,
	0,
};
const void *tnet_dhcp_option_paramslist_def_t = &tnet_dhcp_option_paramslist_def_s;
