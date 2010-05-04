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

/**@file tmsrp_receiver.c
 * @brief MSRP receiver.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinymsrp/session/tmsrp_receiver.h"

#include "tmsrp.h"

#include "tnet_utils.h"

#include "tsk_memory.h"
#include "tsk_string.h"
#include "tsk_debug.h"

int tmsrp_receiver_start(tmsrp_receiver_t* self)
{
	return 0;
}

int tmsrp_receiver_stop(tmsrp_receiver_t* self)
{
	return 0;
}

int tmsrp_transport_layer_stream_cb(const tnet_transport_event_t* e)
{
	const tmsrp_receiver_t *receiver = e->callback_data;
	tmsrp_message_t* message;

	switch(e->type){
		case event_data: {
				break;
			}
		case event_closed:
		case event_connected:
		default:{
				return 0;
			}
	}

	// put the data
	tmsrp_data_in_put(receiver->data_in, e->data, e->size);
	// get msrp messages
	while((message = tmsrp_data_in_get(receiver->data_in))){
				
		//
		//	SEND
		//
		if(TMSRP_REQUEST_IS_SEND(message)){
			tmsrp_response_t* r2xx;
			tmsrp_request_t* REPORT;
			char* str;

			// send 200 OK
			if((r2xx = tmsrp_create_response(message, 200, "I got it"))){
				if((str = tmsrp_message_tostring(r2xx))){
					tnet_sockfd_send(receiver->fd, str, strlen(str), 0);
					TSK_FREE(str);
				}
				TSK_OBJECT_SAFE_FREE(r2xx);
			}
			// send REPORT
			if(tmsrp_isReportRequired(message, tsk_false)){
				if((REPORT = tmsrp_create_report(message, 200, "I got it"))){
					if((str = tmsrp_message_tostring(REPORT))){
						tnet_sockfd_send(receiver->fd, str, strlen(str), 0);
						TSK_FREE(str);
					}
					TSK_OBJECT_SAFE_FREE(REPORT);
				}
			}
		}

		// alert user layer

		TSK_OBJECT_SAFE_FREE(message);
	}

	return 0;
}



//=================================================================================================
//	MSRP receiver object definition
//
static void* tmsrp_receiver_create(void * self, va_list *app)
{
	tmsrp_receiver_t *receiver = self;
	if(receiver){
		receiver->config = tsk_object_ref(va_arg(*app, tmsrp_config_t*));
		receiver->fd = va_arg(*app, tnet_fd_t);	

		receiver->data_in = TMSRP_DATA_IN_CREATE();
	}
	return self;
}

static void* tmsrp_receiver_destroy(void * self)
{ 
	tmsrp_receiver_t *receiver = self;
	if(receiver){
		/* Stop */
		tmsrp_receiver_stop(receiver);

		TSK_OBJECT_SAFE_FREE(receiver->config);
		TSK_OBJECT_SAFE_FREE(receiver->data_in);
		// the FD is owned by the transport ...do not close it
	}
	return self;
}

static const tsk_object_def_t tmsrp_receiver_def_s = 
{
	sizeof(tmsrp_receiver_t),
	tmsrp_receiver_create, 
	tmsrp_receiver_destroy,
	tsk_null, 
};
const tsk_object_def_t *tmsrp_receiver_def_t = &tmsrp_receiver_def_s;

