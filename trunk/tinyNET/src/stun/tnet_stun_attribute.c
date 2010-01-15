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

/**@file tnet_stun_attribute.c
 * @brief STUN2(RFC 5389) attribute parser.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tnet_stun_attribute.h"


tnet_stun_attribute_t* tnet_stun_attribute_parse(const void* data, size_t size)
{
	tnet_stun_attribute_t *attribute = 0;

	tnet_stun_attribute_type_t type = (tnet_stun_attribute_type_t)ntohs(*((uint16_t*)data));
	uint16_t length = ntohs(*((uint16_t*)dataPtr));

	switch(type)
	{
	default:
		{
			attribute = TNET_STUN_ATTRIBUTE_CREATE();
			break;
		}
	}

	
	/* Set common values. */
	attribute->type = type;
	attribute->length = length;

	return 0;
}



//========================================================
//	STUN2 ATTRIBUTE object definition
//
#define TNET_STUN_ATTRIBUTE_DEF(name) \
	static void* tnet_stun_##name##_create(void * self, va_list * app){ return self; } \
	static void* tnet_stun_##name##_destroy(void * self) { return self; } \
	static const tsk_object_def_t tnet_stun_##name##_def_s =  \
	{ \
		sizeof(tnet_stun_##name##_t), \
		tnet_stun_##name##_create,  \
		tnet_stun_##name##_destroy, \
		0,  \
	}; \
	const void *tnet_stun_##name##_def_t = &tnet_stun_##name##_def_s;


/*== RFC 5389 - 15.  STUN Attributes
 */
TNET_STUN_ATTRIBUTE_DEF(attribute)

/*== RFC 5389 - 15.1. MAPPED-ADDRESS]
 */
TNET_STUN_ATTRIBUTE_DEF(attribute_mapped_addr)

