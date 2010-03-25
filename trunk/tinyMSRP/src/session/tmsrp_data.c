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

/**@file tmsrp_data.c
 * @brief MSRP input/output data.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinyMSRP/session/tmsrp_data.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#include <stdio.h> /* fopen, fclose ... */

/* =========================== Common ============================= */

int tmsrp_data_init(tmsrp_data_t* self, tsk_bool_t outgoing, const void* pdata, size_t size, tsk_bool_t isfilepath, const char* ctype)
{
	if(!self || !pdata || !size){
		return -1;
	}
	
	self->start = 1;
	
	if(isfilepath){
		if(self->file){
			fclose(self->file);
		}
		if(outgoing){
			if(!(self->file = fopen((const char*)pdata, "rb"))){
				TSK_DEBUG_ERROR("Failed to open(rb) this file:%s", pdata);
				return -2;
			}
		}
		else{
			if(!(self->file = fopen((const char*)pdata, "wb"))){
				TSK_DEBUG_ERROR("Failed to open(wb) this file:%s", pdata);
				return -3;
			}
		}
	}
	else{
		self->buffer = TSK_BUFFER_CREATE(pdata, size);
	}
	tsk_strupdate(&self->ctype, ctype);

	return 0;
}

int tmsrp_data_deinit(tmsrp_data_t* self)
{
	if(!self){
		return -1;
	}

	TSK_FREE(self->id);
	TSK_FREE(self->ctype);
	TSK_OBJECT_SAFE_FREE(self->buffer);
	
	if(self->file){
		fclose(self->file);
		self->file = tsk_null;
	}

	return 0;
}






/* =========================== Incoming ============================= */

int tmsrp_data_in_put(tmsrp_data_in_t* self, const void* pdata, size_t size)
{
	if(!self || !pdata || !size){
		return -1;
	}

	return 0;
}

tmsrp_request_t* tmsrp_data_in_get(tmsrp_data_in_t* self)
{
	tmsrp_request_t* ret = tsk_null;

	if(!self){
		return tsk_null;
	}

	return ret;
}


/* =========================== Outgoing ============================= */

tsk_buffer_t* tmsrp_data_out_get(tmsrp_data_out_t* self)
{
	tsk_buffer_t* ret = tsk_null;

	if(!self){
		return tsk_null;
	}

	return ret;
}








//=================================================================================================
//	MSRP incoming data object definition
//
static void* tmsrp_data_in_create(void * self, va_list * app)
{
	tmsrp_data_in_t *data_in = self;
	if(data_in){
		const void* pdata = va_arg(*app, const void*);
		size_t size = va_arg(*app, size_t);
		tsk_bool_t isfilepath = va_arg(*app, tsk_bool_t);

		if(tmsrp_data_init(TMSRP_DATA(data_in), tsk_false, pdata, size, isfilepath, "text/plain")){
			TMSRP_DATA(data_in)->isOK = tsk_false;
		}
		else{
			TMSRP_DATA(data_in)->isOK = tsk_true;
		}
	}
	return self;
}

static void* tmsrp_data_in_destroy(void * self)
{ 
	tmsrp_data_in_t *data_in = self;
	if(data_in){
		tmsrp_data_deinit(TMSRP_DATA(data_in));
	}

	return self;
}

static const tsk_object_def_t tmsrp_data_in_def_s = 
{
	sizeof(tmsrp_data_in_t),
	tmsrp_data_in_create, 
	tmsrp_data_in_destroy,
	tsk_null, 
};
const tsk_object_def_t *tmsrp_data_in_def_t = &tmsrp_data_in_def_s;

//=================================================================================================
//	MSRP outgoing data object definition
//
static void* tmsrp_data_out_create(void * self, va_list * app)
{
	tmsrp_data_out_t *data_out = self;
	if(data_out){
		const void* pdata = va_arg(*app, const void*);
		size_t size = va_arg(*app, size_t);
		tsk_bool_t isfilepath = va_arg(*app, tsk_bool_t);

		if(tmsrp_data_init(TMSRP_DATA(data_out), tsk_true, pdata, size, isfilepath, "text/plain")){
			TMSRP_DATA(data_out)->isOK = tsk_false;
		}
		else{
			TMSRP_DATA(data_out)->isOK = tsk_true;
		}
	}
	return self;
}

static void* tmsrp_data_out_destroy(void * self)
{ 
	tmsrp_data_out_t *data_out = self;
	if(data_out){
		tmsrp_data_deinit(TMSRP_DATA(data_out));
	}

	return self;
}

static const tsk_object_def_t tmsrp_data_out_def_s = 
{
	sizeof(tmsrp_data_out_t),
	tmsrp_data_out_create, 
	tmsrp_data_out_destroy,
	tsk_null, 
};
const tsk_object_def_t *tmsrp_data_out_def_t = &tmsrp_data_out_def_s;
