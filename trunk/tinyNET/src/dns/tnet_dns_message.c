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
/**@file tnet_dns_message.h
 * @brief DNS Message holding RRs (RFC 1035).
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tnet_dns_message.h"

#include "tsk_memory.h"






//========================================================
//	[[DNS MESSAGE]] object definition
//
static void* tnet_dns_message_create(void * self, va_list * app)
{
	tnet_dns_message_t *message = self;
	if(message)
	{
		
	}
	return self;
}

static void* tnet_dns_message_destroy(void * self) 
{ 
	tnet_dns_message_t *message = self;
	if(message)
	{
		TSK_FREE(message->Question.QNAME);
		
		TSK_OBJECT_SAFE_FREE(message->Answers);
		TSK_OBJECT_SAFE_FREE(message->Authoritys);
		TSK_OBJECT_SAFE_FREE(message->Additionals);
	}
	return self;
}

static const tsk_object_def_t tnet_dns_message_def_s =
{
	sizeof(tnet_dns_message_t),
	tnet_dns_message_create,
	tnet_dns_message_destroy,
	0,
};
const void *tnet_dns_message_def_t = &tnet_dns_message_def_s;