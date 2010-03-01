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
* GNU General Public License for more details.
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

#include "tnet_types.h"

#include "tsk_memory.h"

tnet_dhcp6_option_t* tnet_dhcp6_option_deserialize(const void* data, size_t size)
{
	tnet_dhcp6_option_t *option = 0;
	uint8_t* dataPtr = ((uint8_t*)data);
	//uint8_t* dataEnd = (dataPtr+size);

	tnet_dhcp6_option_code_t code;
	uint16_t len;

	/* Check validity */
	if(!dataPtr || size<4/*Code Len*/){
		goto bail;
	}

	code = (tnet_dhcp6_option_code_t) ntohs(*((uint16_t*)dataPtr));
	dataPtr += 2;

	len = ntohs(*((uint16_t*)dataPtr));
	dataPtr += 2;

	switch(code)
	{
	case dhcp6_code_clientid:
	case dhcp6_code_serverid:
		{
			break;
		}

	default:
		{
			break;
		}
	}
bail:
	return option;
}

int tnet_dhcp6_option_serialize(const tnet_dhcp6_option_t* self, tsk_buffer_t *output)
{
	uint16_t _2bytes;
	int ret = -1;

	if(!self || !output){
		goto bail;
	}

	/*== Code */
	_2bytes = htons(self->code);
	tsk_buffer_append(output, &(_2bytes), 2);

	switch(self->code)
	{
	case dhcp6_code_clientid:
	case dhcp6_code_serverid:
		{
			break;
		}

	case dhcp6_code_oro:
	default:
		{
			if(self->data)
			{
				const tnet_dhcp6_option_orequest_t* opt = (const tnet_dhcp6_option_orequest_t*)self->data;
				if(opt->codes){
					/* option-len */
					_2bytes = htons(opt->codes->size);
					tsk_buffer_append(output, &(_2bytes), 2);
					/* option-data */
					ret = tsk_buffer_append(output, opt->codes->data, opt->codes->size);
				}
				
			}
			break;
		}
	}	
bail:
	return ret;
}

int tnet_dhcp6_option_serializeex(tnet_dhcp6_option_code_t code, uint8_t length, const void* value, tsk_buffer_t *output)
{
	return -1;
}

//
//	[[DHCPv6 OPTION]] object definition
//
static void* tnet_dhcp6_option_create(void * self, va_list * app)
{
	tnet_dhcp6_option_t *option = self;
	if(option)
	{
		tnet_dhcp6_option_code_t code = va_arg(*app, tnet_dhcp6_option_code_t);
		const void* payload = va_arg(*app, const void*);
		size_t payload_size = va_arg(*app, size_t);

		option->code = code;
		if(payload && payload_size){
			if((option->data = (tnet_dhcp6_option_data_t*)tsk_calloc(payload_size, sizeof(uint8_t)))){
				memcpy(option->data, payload, payload_size);
				option->len = payload_size;
			}
		}
	}
	return self;
}

static void* tnet_dhcp6_option_destroy(void * self) 
{ 
	tnet_dhcp6_option_t *option = self;
	if(option)
	{
		TSK_OBJECT_SAFE_FREE(option->data);
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

/*======================================================================================
*	RFC 3315 - 
			22.2. Client Identifier Option
			22.3. Server Identifier Option
*=======================================================================================*/
//
//	[[DHCPv6 Option Request Option]] object definition
//
static void* tnet_dhcp6_option_identifier_create(void * self, va_list * app)
{
	tnet_dhcp6_option_identifier_t *option = self;
	if(option)
	{
		tnet_dhcp6_option_code_t code = va_arg(*app, tnet_dhcp6_option_code_t);
		const void* payload = va_arg(*app, const void*);
		size_t payload_size = va_arg(*app, size_t);

		if(payload && payload_size)
		{	/* DESERIALIZATION */
		}
	}
	return self;
}

static void* tnet_dhcp6_option_identifier_destroy(void * self) 
{ 
	tnet_dhcp6_option_identifier_t *option = self;
	if(option)
	{
		TSK_OBJECT_SAFE_FREE(option->duid);
	}
	return self;
}

static const tsk_object_def_t tnet_dhcp6_option_identifier_def_s =
{
	sizeof(tnet_dhcp6_option_identifier_t),
	tnet_dhcp6_option_identifier_create,
	tnet_dhcp6_option_identifier_destroy,
	0,
};
const void *tnet_dhcp6_option_identifier_def_t = &tnet_dhcp6_option_identifier_def_s;

/*======================================================================================
*	RFC 3315 - 22.7. Option Request Option
*=======================================================================================*/

int tnet_dhcp6_option_orequest_add_code(tnet_dhcp6_option_orequest_t* self, uint16_t code)
{
	uint16_t _2bytes;
	int ret = -1;
	if(self)
	{
		if(!self->codes){
			if(!(self->codes = TSK_BUFFER_CREATE_NULL())){
				return -3;
			}
		}
		_2bytes = ntohs(code);
		if(!(ret = tsk_buffer_append(self->codes, &_2bytes, 2))){
			TNET_DHCP6_OPTION(self)->len += 2;
		}
	}
	return ret;
}

//
//	[[DHCPv6 Option Request Option]] object definition
//
static void* tnet_dhcp6_option_orequest_create(void * self, va_list * app)
{
	tnet_dhcp6_option_orequest_t *option = self;
	if(option)
	{
		const void* payload = va_arg(*app, const void*);
		size_t payload_size = va_arg(*app, size_t);

		if(payload && payload_size)
		{	/* DESERIALIZATION */
		}
	}
	return self;
}

static void* tnet_dhcp6_option_orequest_destroy(void * self) 
{ 
	tnet_dhcp6_option_orequest_t *option = self;
	if(option)
	{
		TSK_OBJECT_SAFE_FREE(option->codes);
	}
	return self;
}

static const tsk_object_def_t tnet_dhcp6_option_orequest_def_s =
{
	sizeof(tnet_dhcp6_option_orequest_t),
	tnet_dhcp6_option_orequest_create,
	tnet_dhcp6_option_orequest_destroy,
	0,
};
const void *tnet_dhcp6_option_orequest_def_t = &tnet_dhcp6_option_orequest_def_s;

/*======================================================================================
*	RFC 3315 - 22.16. Vendor Class Option
*=======================================================================================*/

//
//	[[DHCPv6 Option Request Option]] object definition
//
static void* tnet_dhcp6_option_vendorclass_create(void * self, va_list * app)
{
	tnet_dhcp6_option_vendorclass_t *option = self;
	if(option)
	{
		const void* payload = va_arg(*app, const void*);
		size_t payload_size = va_arg(*app, size_t);

		if(payload && payload_size)
		{	/* DESERIALIZATION */
		}
	}
	return self;
}

static void* tnet_dhcp6_option_vendorclass_destroy(void * self) 
{ 
	tnet_dhcp6_option_vendorclass_t *option = self;
	if(option)
	{
		TSK_OBJECT_SAFE_FREE(option->vendor_class_data);
	}
	return self;
}

static const tsk_object_def_t tnet_dhcp6_option_vendorclass_def_s =
{
	sizeof(tnet_dhcp6_option_vendorclass_t),
	tnet_dhcp6_option_vendorclass_create,
	tnet_dhcp6_option_vendorclass_destroy,
	0,
};
const void *tnet_dhcp6_option_vendorclass_def_t = &tnet_dhcp6_option_vendorclass_def_s;
