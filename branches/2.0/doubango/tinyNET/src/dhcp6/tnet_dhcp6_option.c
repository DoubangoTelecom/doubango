/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tnet_dhcp6_option.h"

#include "../tnet_types.h"
#include "../tnet_endianness.h"

#include "tsk_memory.h"

#include <string.h>

tnet_dhcp6_option_t* tnet_dhcp6_option_create(tnet_dhcp6_option_code_t code, const void* payload, tsk_size_t payload_size)
{
	return tsk_object_new(tnet_dhcp6_option_def_t, code, payload, payload_size);
}

tnet_dhcp6_option_identifier_t* tnet_dhcp6_option_indentifer_create(tnet_dhcp6_option_code_t code, const void* payload, tsk_size_t payload_size)
{
	return tsk_object_new(tnet_dhcp6_option_identifier_def_t, code, payload, payload_size);
}

tnet_dhcp6_option_identifier_t* tnet_dhcp6_option_clientid_create(const void* payload, tsk_size_t payload_size)
{
	return tnet_dhcp6_option_indentifer_create(dhcp6_code_clientid, payload, payload_size);
}

tnet_dhcp6_option_identifier_t* tnet_dhcp6_option_serverid_create(const void* payload, tsk_size_t payload_size)
{
	return tnet_dhcp6_option_indentifer_create(dhcp6_code_serverid, payload, payload_size);
}

tnet_dhcp6_option_orequest_t* tnet_dhcp6_option_orequest_create(const void* payload, tsk_size_t payload_size)
{
	return tsk_object_new(tnet_dhcp6_option_orequest_def_t, payload, payload_size);
}

tnet_dhcp6_option_orequest_t* tnet_dhcp6_option_orequest_create_null()
{
	return tnet_dhcp6_option_orequest_create(tsk_null, 0);
}

tnet_dhcp6_option_vendorclass_t* tnet_dhcp6_option_vendorclass_create(const void* payload, tsk_size_t payload_size)
{
	return tsk_object_new(tnet_dhcp6_option_vendorclass_def_t, payload, payload_size);
}

tnet_dhcp6_option_vendorclass_t* tnet_dhcp6_option_vendorclass_create_null()
{
	return tnet_dhcp6_option_vendorclass_create(tsk_null, 0);
}

tnet_dhcp6_option_t* tnet_dhcp6_option_deserialize(const void* data, tsk_size_t size)
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

	code = (tnet_dhcp6_option_code_t) tnet_ntohs_2(dataPtr);
	dataPtr += 2;

	len = tnet_ntohs_2(dataPtr);
	dataPtr += 2;

	switch(code){
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
	_2bytes = tnet_htons(self->code);
	tsk_buffer_append(output, &(_2bytes), 2);

	switch(self->code){
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
					_2bytes = tnet_htons(opt->codes->size);
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
static tsk_object_t* tnet_dhcp6_option_ctor(tsk_object_t * self, va_list * app)
{
	tnet_dhcp6_option_t *option = self;
	if(option){
		tnet_dhcp6_option_code_t code = va_arg(*app, tnet_dhcp6_option_code_t);
		const void* payload = va_arg(*app, const void*);
		tsk_size_t payload_size = va_arg(*app, tsk_size_t);

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

static tsk_object_t* tnet_dhcp6_option_dtor(tsk_object_t * self) 
{ 
	tnet_dhcp6_option_t *option = self;
	if(option){
		TSK_OBJECT_SAFE_FREE(option->data);
	}
	return self;
}

static const tsk_object_def_t tnet_dhcp6_option_def_s =
{
	sizeof(tnet_dhcp6_option_t),
	tnet_dhcp6_option_ctor,
	tnet_dhcp6_option_dtor,
	tsk_null,
};
const tsk_object_def_t *tnet_dhcp6_option_def_t = &tnet_dhcp6_option_def_s;

/*======================================================================================
*	RFC 3315 - 
			22.2. Client Identifier Option
			22.3. Server Identifier Option
*=======================================================================================*/
//
//	[[DHCPv6 Option Request Option]] object definition
//
static tsk_object_t* tnet_dhcp6_option_identifier_ctor(tsk_object_t * self, va_list * app)
{
	tnet_dhcp6_option_identifier_t *option = self;
	if(option){
		//tnet_dhcp6_option_code_t code = va_arg(*app, tnet_dhcp6_option_code_t);
		const void* payload = va_arg(*app, const void*);
		tsk_size_t payload_size = va_arg(*app, tsk_size_t);

		if(payload && payload_size){
			/* DESERIALIZATION */
		}
	}
	return self;
}

static tsk_object_t* tnet_dhcp6_option_identifier_dtor(tsk_object_t * self) 
{ 
	tnet_dhcp6_option_identifier_t *option = self;
	if(option){
		TSK_OBJECT_SAFE_FREE(option->duid);
	}
	return self;
}

static const tsk_object_def_t tnet_dhcp6_option_identifier_def_s =
{
	sizeof(tnet_dhcp6_option_identifier_t),
	tnet_dhcp6_option_identifier_ctor,
	tnet_dhcp6_option_identifier_dtor,
	tsk_null,
};
const tsk_object_def_t *tnet_dhcp6_option_identifier_def_t = &tnet_dhcp6_option_identifier_def_s;

/*======================================================================================
*	RFC 3315 - 22.7. Option Request Option
*=======================================================================================*/

int tnet_dhcp6_option_orequest_add_code(tnet_dhcp6_option_orequest_t* self, uint16_t code)
{
	uint16_t _2bytes;
	int ret = -1;
	if(self){
		if(!self->codes){
			if(!(self->codes = tsk_buffer_create_null())){
				return -3;
			}
		}
		_2bytes = tnet_ntohs(code);
		if(!(ret = tsk_buffer_append(self->codes, &_2bytes, 2))){
			TNET_DHCP6_OPTION(self)->len += 2;
		}
	}
	return ret;
}

//
//	[[DHCPv6 Option Request Option]] object definition
//
static tsk_object_t* tnet_dhcp6_option_orequest_ctor(tsk_object_t * self, va_list * app)
{
	tnet_dhcp6_option_orequest_t *option = self;
	if(option){
		const void* payload = va_arg(*app, const void*);
		tsk_size_t payload_size = va_arg(*app, tsk_size_t);

		if(payload && payload_size)
		{	/* DESERIALIZATION */
		}
	}
	return self;
}

static tsk_object_t* tnet_dhcp6_option_orequest_dtor(tsk_object_t * self) 
{ 
	tnet_dhcp6_option_orequest_t *option = self;
	if(option){
		TSK_OBJECT_SAFE_FREE(option->codes);
	}
	return self;
}

static const tsk_object_def_t tnet_dhcp6_option_orequest_def_s =
{
	sizeof(tnet_dhcp6_option_orequest_t),
	tnet_dhcp6_option_orequest_ctor,
	tnet_dhcp6_option_orequest_dtor,
	tsk_null,
};
const tsk_object_def_t *tnet_dhcp6_option_orequest_def_t = &tnet_dhcp6_option_orequest_def_s;

/*======================================================================================
*	RFC 3315 - 22.16. Vendor Class Option
*=======================================================================================*/

//
//	[[DHCPv6 Option Request Option]] object definition
//
static tsk_object_t* tnet_dhcp6_option_vendorclass_ctor(tsk_object_t * self, va_list * app)
{
	tnet_dhcp6_option_vendorclass_t *option = self;
	if(option){
		const void* payload = va_arg(*app, const void*);
		tsk_size_t payload_size = va_arg(*app, tsk_size_t);

		if(payload && payload_size){
			/* DESERIALIZATION */
		}
	}
	return self;
}

static tsk_object_t* tnet_dhcp6_option_vendorclass_dtor(tsk_object_t * self) 
{ 
	tnet_dhcp6_option_vendorclass_t *option = self;
	if(option){
		TSK_OBJECT_SAFE_FREE(option->vendor_class_data);
	}
	return self;
}

static const tsk_object_def_t tnet_dhcp6_option_vendorclass_def_s =
{
	sizeof(tnet_dhcp6_option_vendorclass_t),
	tnet_dhcp6_option_vendorclass_ctor,
	tnet_dhcp6_option_vendorclass_dtor,
	tsk_null,
};
const tsk_object_def_t *tnet_dhcp6_option_vendorclass_def_t = &tnet_dhcp6_option_vendorclass_def_s;
