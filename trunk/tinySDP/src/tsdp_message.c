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

#include "tinySDP/headers/tsdp_header_O.h"
#include "tinySDP/headers/tsdp_header_S.h"
#include "tinySDP/headers/tsdp_header_T.h"
#include "tinySDP/headers/tsdp_header_V.h"

#define TSDP_LINE_S_VALUE_DEFAULT "-"	/* as per RFC 3264 subclause 5 */

#define TSDP_LINE_O_USERNAME_DEFAULT	"doubango"
#define TSDP_LINE_O_SESSION_VER_DEFAULT	2301
#define TSDP_LINE_O_SESSION_ID_DEFAULT	1983

/*== Predicate function to find tsdp_header_t object by type. */
int __pred_find_header_by_type(const tsk_list_item_t *item, const void *tsdp_htype)
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
int __pred_find_header_by_name(const tsk_list_item_t *item, const void *name)
{
	if(item && item->data && name)
	{
		tsdp_header_t *header = item->data;
		return tsdp_header_get_nameex(header) - *((const char*)name);
	}
	return -1;
}

/*== Predicate function to find media object by name. */
int __pred_find_media_by_name(const tsk_list_item_t *item, const void *name)
{
	if(item && item->data && name)
	{
		tsdp_header_t *header = item->data;
		if(header->type == tsdp_htype_M){
			return tsk_stricmp(((tsdp_header_M_t*)header)->media, (const char*)name);
		}
	}
	return -1;
}

/*== Add headers/fmt to the media line */
int __add_headers(tsdp_header_M_t* m, va_list *ap)
{
	const tsk_object_def_t* objdef;
	tsdp_header_t *header;
	tsdp_fmt_t* fmt;
	
	if(!m){
		return -1;
	}
	
	while((objdef = va_arg(*ap, const tsk_object_def_t*))){
		if(objdef == tsdp_fmt_def_t){
			if((fmt = tsk_object_new2(objdef, ap))){
				tsk_list_push_back_data(m->FMTs, (void**)&fmt);
			}
		}
		else{
			if((header = tsk_object_new2(objdef, ap))){
				tsdp_header_M_add(m, header);
				TSK_OBJECT_SAFE_FREE(header);
			}
		}
	}
	return 0;
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

int tsdp_message_add_headers(tsdp_message_t *self, ...)
{
	const tsk_object_def_t* objdef;
	tsdp_header_t *header;
	va_list ap;

	if(!self){
		return -1;
	}

	va_start(ap, self);
	while((objdef = va_arg(ap, const tsk_object_def_t*))){
		if((header = tsk_object_new2(objdef, &ap))){
			tsdp_message_add_header(self, header);
			TSK_OBJECT_SAFE_FREE(header);
		}
	}
	va_end(ap);

	return 0;
}

const tsdp_header_t *tsdp_message_get_headerAt(const tsdp_message_t *self, tsdp_header_type_t type, size_t index)
{
	size_t pos = 0;
	const tsk_list_item_t *item;
	const tsdp_header_t *hdr;

	if(!self || !self->headers){
		return tsk_null;
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
	
	return tsk_null;
}

const tsdp_header_t *tsdp_message_get_header(const tsdp_message_t *self, tsdp_header_type_t type)
{
	return tsdp_message_get_headerAt(self, type, 0);
}

const tsdp_header_t *tsdp_message_get_headerByName(const tsdp_message_t *self, char name)
{
	if(self && self->headers){
		const tsk_list_item_t* item;
		if((item = tsk_list_find_item_by_pred(self->headers, __pred_find_header_by_name, &name))){
			return item->data;
		}
	}
	return tsk_null;
}

int tsdp_message_serialize(const tsdp_message_t *self, tsk_buffer_t *output)
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

char* tsdp_message_tostring(const tsdp_message_t *self)
{
	tsk_buffer_t* output = TSK_BUFFER_CREATE_NULL();
	char* ret = tsk_null;

	if(!tsdp_message_serialize(self, output)){
		ret = tsk_strndup(TSK_BUFFER_DATA(output), TSK_BUFFER_SIZE(output));
	}

	TSK_OBJECT_SAFE_FREE(output);
	return ret;
}

tsdp_message_t* tsdp_message_create_empty(const char* addr, tsk_bool_t ipv6)
{
	tsdp_message_t* ret = 0;

	if(!(ret = TSDP_MESSAGE_CREATE())){
		return tsk_null;
	}

	/*	RFC 3264 - 5 Generating the Initial Offer
		The numeric value of the session id and version in the o line MUST be 
		representable with a 64 bit signed integer.  The initial value of the version MUST be less than
	   (2**62)-1, to avoid rollovers.
	*/
	TSDP_MESSAGE_ADD_HEADER(ret, TSDP_HEADER_V_VA_ARGS(0));
	TSDP_MESSAGE_ADD_HEADER(ret, TSDP_HEADER_O_VA_ARGS(
		TSDP_LINE_O_USERNAME_DEFAULT,
		TSDP_LINE_O_SESSION_ID_DEFAULT,
		TSDP_LINE_O_SESSION_VER_DEFAULT,
		"IN",
		ipv6 ? "IP6" : "IP4",
		addr));

	/*	RFC 3264 - 5 Generating the Initial Offer
		The SDP "s=" line conveys the subject of the session, which is
		reasonably defined for multicast, but ill defined for unicast.  For
		unicast sessions, it is RECOMMENDED that it consist of a single space
		character (0x20) or a dash (-).

		Unfortunately, SDP does not allow the "s=" line to be empty.
	*/
	TSDP_MESSAGE_ADD_HEADER(ret, TSDP_HEADER_S_VA_ARGS(TSDP_LINE_S_VALUE_DEFAULT));

	/*	RFC 3264 - 5 Generating the Initial Offer
		The SDP "t=" line conveys the time of the session.  Generally,
		streams for unicast sessions are created and destroyed through
		external signaling means, such as SIP.  In that case, the "t=" line
		SHOULD have a value of "0 0".
	*/
	TSDP_MESSAGE_ADD_HEADER(ret, TSDP_HEADER_T_VA_ARGS(0, 0));
	
	return ret;
}

tsdp_message_t* tsdp_message_clone(const tsdp_message_t *self)
{
	tsdp_message_t* clone = tsk_null;
	tsk_list_item_t* item;
	tsdp_header_t* header;

	if(!self){
		goto bail;
	}

	if((clone =  TSDP_MESSAGE_CREATE())){
		tsk_list_foreach(item, self->headers){
			if((header = tsdp_header_clone(TSDP_HEADER(item->data)))){
				tsk_list_push_back_data(clone->headers, (void**)&header);
			}
		}
	}


bail:
	return clone;
}

int tsdp_message_add_media(tsdp_message_t *self, const char* media, uint32_t port, const char* proto, ...)
{
	va_list ap;
	int ret;
		
	va_start(ap, proto);
	ret = tsdp_message_add_media_2(self, media, port, proto, &ap);
	va_end(ap);

	return ret;
}

int tsdp_message_add_media_2(tsdp_message_t *self, const char* media, uint32_t port, const char* proto, va_list *ap)
{
	int ret = -1;
	tsdp_header_M_t* m;

	if(!self){
		return -1;
	}

	if((m = TSDP_HEADER_M_CREATE(media, port, proto))){
		__add_headers(m, ap);
		
		ret = tsdp_message_add_header(self, TSDP_HEADER(m));
		TSK_OBJECT_SAFE_FREE(m);
	}
	
	return ret;
}

int tsdp_message_remove_media(tsdp_message_t *self, const char* media)
{
	int ret = -1;

	if(!self){
		goto bail;
	}

	tsk_list_remove_item_by_pred(self->headers, __pred_find_media_by_name, media);

bail:
	return ret;
}



/* ================= 3GPP TS 34.610 :: Communication HOLD (HOLD) using IP Multimedia (IM) Core ================*/
int tsdp_message_hold(tsdp_message_t* self, const char* media)
{
	tsdp_header_M_t* m;
	const tsdp_header_A_t* a;
	const tsk_list_item_t* item;

	if(!self){
		return -1;
	}
	// 3GPP TS 34.610-900 - 4.5.2.1	Actions at the invoking UE
	if((item = tsk_list_find_item_by_pred(self->headers, __pred_find_media_by_name, media))){
		m = TSDP_HEADER_M(item->data);
		if((a = tsdp_header_M_findA(m, "recvonly"))){
			// an "inactive" SDP attribute if the stream was previously set to "recvonly" media stream
			tsk_strupdate(&(TSDP_HEADER_A(a)->field), "inactive");
		}
		else if((a = tsdp_header_M_findA(m, "sendrecv"))){
			// a "sendonly" SDP attribute if the stream was previously set to "sendrecv" media stream
			tsk_strupdate(&(TSDP_HEADER_A(a)->field), "sendonly");
		}
		else{
			// default value is sendrecv. hold on default --> sendonly
			if(!(a = tsdp_header_M_findA(m, "sendonly")) && !(a = tsdp_header_M_findA(m, "inactive"))){
				tsdp_header_A_t* newA;
				if((newA = TSDP_HEADER_A_CREATE("sendonly", tsk_null))){
					tsdp_header_M_add(m, TSDP_HEADER(newA));
					TSK_OBJECT_SAFE_FREE(newA);
				}
			}
		}
	}

	return 0;
}

int tsdp_message_resume(tsdp_message_t* self, const char* media)
{
	tsdp_header_M_t* m;
	const tsdp_header_A_t* a;
	const tsk_list_item_t* item;

	if(!self){
		return -1;
	}
	// 3GPP TS 34.610-900 - 4.5.2.1	Actions at the invoking UE
	if((item = tsk_list_find_item_by_pred(self->headers, __pred_find_media_by_name, media))){
		m = TSDP_HEADER_M(item->data);
		if((a = tsdp_header_M_findA(m, "inactive"))){
			// a "recvonly" SDP attribute if the stream was previously an inactive media stream
			tsk_strupdate(&(TSDP_HEADER_A(a)->field), "recvonly");
		}
		else if((a = tsdp_header_M_findA(m, "sendonly"))){
			// a "sendrecv" SDP attribute if the stream was previously a sendonly media stream, or the attribute may be omitted, since sendrecv is the default
			tsk_strupdate(&(TSDP_HEADER_A(a)->field), "sendrecv");
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
const tsk_object_def_t *tsdp_message_def_t = &tsdp_message_def_s;
