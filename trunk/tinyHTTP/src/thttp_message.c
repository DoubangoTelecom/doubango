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

/**@file thttp_message.c
 * @brief Represents a HTTP message. A HTTP message is either a request from a client to a server, or a
 * response from a server to a client.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinyHTTP/thttp_message.h"


int	thttp_message_add_header(thttp_message_t *self, const thttp_header_t *hdr)
{
	return -1;
}

uint32_t thttp_message_getContent_length(const thttp_message_t *message)
{
	return 0;
}














//=================================================================================================
//	THTTP mesage object definition
//
static void* thttp_message_create(void * self, va_list * app)
{
	thttp_message_t *message = self;
	if(message)
	{
	}
	return self;
}

static void* thttp_message_destroy(void * self)
{ 
	thttp_message_t *message = self;
	if(message)
	{
	}

	return self;
}

static int thttp_message_cmp(const void *obj1, const void *obj2)
{
	return -1;
}

static const tsk_object_def_t thttp_message_def_s = 
{
	sizeof(thttp_message_t),
	thttp_message_create, 
	thttp_message_destroy,
	thttp_message_cmp, 
};
const void *thttp_message_def_t = &thttp_message_def_s;
