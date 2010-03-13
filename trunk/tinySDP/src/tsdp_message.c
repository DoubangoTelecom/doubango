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

/**@file tsdp_message.c
 * @brief SDP message.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */

#include "tinySDP/tsdp_message.h"


/*== Predicate function to find tsdp_header_t object by type. */
static int pred_find_header_by_type(const tsk_list_item_t *item, const void *tsdp_htype)
{
	if(item && item->data)
	{
		tsdp_header_t *header = item->data;
		tsdp_header_type_t htype = *((tsdp_header_type_t*)tsdp_htype);
		return (header->type - htype);
	}
	return -1;
}

/*== Predicate function to find tsdp_header_t object by name. */
static int pred_find_header_by_name(const tsk_list_item_t *item, const void *name)
{
	if(item && item->data && name)
	{
		tsdp_header_t *header = item->data;
		return tsdp_header_get_nameex(header) - *((const char*)name);
	}
	return -1;
}


int tsdp_message_add_header(tsdp_message_t *self, const tsdp_header_t *hdr)
{
	if(self && hdr)
	{
		tsdp_header_t *header = tsk_object_ref((void*)hdr);
		tsk_list_push_ascending_data(self->headers, (void**)&header); // Very important: Headers MUST appear in a fixed order (see ranks def).

		return 0;
	}
	return -1;
}

int tsdp_message_add_headers(tsdp_message_t *self, const tsdp_headers_L_t *headers)
{
	tsk_list_item_t *item = 0;
	if(self && headers)
	{
		tsk_list_foreach(item, headers){
			tsdp_message_add_header(self, item->data);
		}
		return 0;
	}
	return -1;
}

const tsdp_header_t *tsdp_message_get_headerAt(const tsdp_message_t *self, tsdp_header_type_t type, size_t index)
{
	size_t pos = 0;
	const tsk_list_item_t *item;
	const tsdp_header_t *hdr;

	if(!self || !self->headers){
		return TSDP_NULL;
	}

	tsk_list_foreach(item, self->headers)
	{
		hdr = item->data;
		if(hdr->type == type){
			if(pos++ >= index){
				return hdr;
			}
		}
	}
	
	return TSDP_NULL;
}

const tsdp_header_t *tsdp_message_get_header(const tsdp_message_t *self, tsdp_header_type_t type)
{
	return tsdp_message_get_headerAt(self, type, 0);
}

const tsdp_header_t *tsdp_message_get_headerByName(const tsdp_message_t *self, char name)
{
	if(self && self->headers){
		const tsk_list_item_t* item;
		if((item = tsk_list_find_item_by_pred(self->headers, pred_find_header_by_name, &name))){
			return item->data;
		}
	}
	return TSDP_NULL;
}

int tsdp_message_tostring(const tsdp_message_t *self, tsk_buffer_t *output)
{
	const tsk_list_item_t* item;

	if(!self || !output){
		return -1;
	}
	
	tsk_list_foreach(item, self->headers)
	{
		if(tsdp_header_tostring(TSDP_HEADER(item->data), output)){
			// Abort?
		}
	}
	
	return 0;
}






























//=================================================================================================
//	SDP object definition
//
static void* tsdp_message_create(void * self, va_list * app)
{
	tsdp_message_t *message = self;
	if(message){
		message->headers = TSK_LIST_CREATE();
	}
	return self;
}

static void* tsdp_message_destroy(void * self)
{ 
	tsdp_message_t *message = self;
	if(message){
		TSK_OBJECT_SAFE_FREE(message->headers);
	}
	return self;
}

static int tsdp_message_cmp(const void *obj1, const void *obj2)
{
	return -1;
}

static const tsk_object_def_t tsdp_message_def_s = 
{
	sizeof(tsdp_message_t),
	tsdp_message_create, 
	tsdp_message_destroy,
	tsdp_message_cmp, 
};
const void *tsdp_message_def_t = &tsdp_message_def_s;