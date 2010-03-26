/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)yahoo.fr>
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

/**@file tmsrp_sender.c
 * @brief MSRP sender.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinyMSRP/session/tmsrp_sender.h"

#include "tnet_utils.h"

#include "tsk_memory.h"
#include "tsk_string.h"
#include "tsk_debug.h"


void *run(void* self);

int tmsrp_sender_start(tmsrp_sender_t* self)
{
	int ret = -1;
	
	if(!self){
		goto bail;
	}
	
	TSK_RUNNABLE(self)->run = run;
	if((ret = tsk_runnable_start(TSK_RUNNABLE(self), tmsrp_data_out_def_t))){
		goto bail;
	}
	
bail:
	return ret;
}

int tsmrp_sender_send_data(tmsrp_sender_t* self, const void* pdata, size_t size, const char* ctype)
{
	tmsrp_data_out_t* data_out;

	if(!self){
		return -1;
	}

	if((data_out = TMSRP_DATA_OUT_CREATE(pdata, size))){
		TSK_RUNNABLE_ENQUEUE_OBJECT(self, data_out);
		return 0;
	}
	return -2;
}

int tsmrp_sender_send_file(tmsrp_sender_t* self, const char* filepath)
{
	tmsrp_data_out_t* data_out;

	if(!self || !filepath){
		return -1;
	}

	if((data_out = TMSRP_DATA_OUT_FILE_CREATE(filepath))){
		TSK_RUNNABLE_ENQUEUE_OBJECT(self, data_out);
		return 0;
	}
	return -2;
}

int tmsrp_sender_stop(tmsrp_sender_t* self)
{
	int ret = -1;

	if(!self){
		goto bail;
	}

	if((ret = tsk_runnable_stop(TSK_RUNNABLE(self)))){
		goto bail;
	}

bail:
	return ret;
}




void *run(void* self)
{
	tsk_list_item_t *curr;
	tmsrp_sender_t *sender = self;
	tmsrp_data_out_t *data_out;
	tsk_buffer_t* chunck;
	char* str;
	size_t start = 1;
	size_t end;
	tsk_istr_t tid;

	TSK_DEBUG_INFO("MSRP SENDER::run -- START");

	TSK_RUNNABLE_RUN_BEGIN(sender);

	if((curr = TSK_RUNNABLE_POP_FIRST(sender)))
	{
		if(!(data_out = (tmsrp_data_out_t*)curr->data)){
			continue;
		}
		
		while((chunck = tmsrp_data_out_get(data_out))){
			tmsrp_request_t* SEND;
			// set end
			end = start + chunck->size;
			// compute new transaction id
			tsk_strrandom(&tid);
			// create SEND request
			SEND = TMSRP_REQUEST_CREATE(tid, "SEND");
			// T-Path and From-Path (because of otherURIs)
			SEND->To = tsk_object_ref(sender->config->To_Path);
			SEND->From = tsk_object_ref(sender->config->From_Path);
			// add other headers
			tmsrp_message_add_headers(SEND,
				TMSRP_HEADER_MESSAGE_ID_VA_ARGS(TMSRP_DATA(data_out)->id),
				TMSRP_HEADER_BYTE_RANGE_VA_ARGS(start, end, -1),
				TMSRP_HEADER_FAILURE_REPORT_VA_ARGS(sender->config->Failure_Report ? freport_yes : freport_no),
				TMSRP_HEADER_SUCCESS_REPORT_VA_ARGS(sender->config->Success_Report),
				//TMSRP_HEADER_CONTENT_TYPE_VA_ARGS(TMSRP_DATA(data_out)->ctype),

				tsk_null);
			// add data
			tmsrp_message_add_content(SEND, TMSRP_DATA(data_out)->ctype, chunck->data, chunck->size);
			// serialize and send
			if((str = tmsrp_message_tostring(SEND))){
				tnet_sockfd_send(sender->fd, str, strlen(str), 0);
				TSK_FREE(str);
			}
			
			// set start
			start = end;
			// cleanup
			TSK_OBJECT_SAFE_FREE(chunck);
			TSK_OBJECT_SAFE_FREE(SEND);
		}
		

		tsk_object_unref(curr);
	}

	TSK_RUNNABLE_RUN_END(self);

	TSK_DEBUG_INFO("MSRP SENDER::run -- STOP");

	return 0;
}



//=================================================================================================
//	MSRP sender object definition
//
static void* tmsrp_sender_create(void * self, va_list *app)
{
	tmsrp_sender_t *sender = self;
	if(sender){
		sender->config = tsk_object_ref(va_arg(*app, tmsrp_config_t*));
		sender->fd = va_arg(*app, tnet_fd_t);	

		sender->outputList = TSK_LIST_CREATE();
	}
	return self;
}

static void* tmsrp_sender_destroy(void * self)
{ 
	tmsrp_sender_t *sender = self;
	if(sender){
		/* Stop */
		tmsrp_sender_stop(sender);

		TSK_OBJECT_SAFE_FREE(sender->config);
		TSK_OBJECT_SAFE_FREE(sender->outputList);
		// the FD is owned by the media ...do not close it
	}
	return self;
}

static const tsk_object_def_t tmsrp_sender_def_s = 
{
	sizeof(tmsrp_sender_t),
	tmsrp_sender_create, 
	tmsrp_sender_destroy,
	tsk_null, 
};
const tsk_object_def_t *tmsrp_sender_def_t = &tmsrp_sender_def_s;
