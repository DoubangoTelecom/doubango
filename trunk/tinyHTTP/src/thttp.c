/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango.org>
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
/**@file thttp.c
 * @brief HTTP (RFC 2616) and HTTP basic/digest authetication (RFC 2617) implementations.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "thttp.h"

#include "tinyHTTP/thttp_action.h"
#include "tinyHTTP/thttp_event.h"

#include "tinyHTTP/thttp_message.h"
#include "tinyHTTP/parsers/thttp_parser_message.h"

#include "tinyHTTP/headers/thttp_header_Transfer_Encoding.h"

#include "tinyHTTP/thttp_dialog.h"

#include "tnet.h"

#include "tsk_runnable.h"
#include "tsk_time.h"
#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"


/** @mainpage TinyHTTP API Overview
*
* This file is an overview of <b>tinyHTTP</b> API.
*
* <b>tinyHTTP</b> is a HTTP (RFC 2616) stack.
*/

// KeepAlive : http://www.io.com/~maus/HttpKeepAlive.html


/**@defgroup thttp_stack_group HTTP/HTTPS stack
*/

/** Callback function used by the transport layer to alert the stack when new messages come.
*/
static int thttp_transport_layer_stream_cb(const tnet_transport_event_t* e)
{
	int ret = -1;
	tsk_ragel_state_t state;
	thttp_message_t *message = tsk_null;
	int endOfheaders = -1;
	const thttp_stack_t *stack = e->callback_data;
	thttp_dialog_t* dialog = tsk_null;
	thttp_session_t* session = tsk_null;
	tsk_bool_t have_all_content = tsk_false;
	
	tsk_safeobj_lock(stack);

	switch(e->type){
		case event_data: {
				break;
			}
		case event_closed:
			// alert all dialogs
			if((session = thttp_session_get_by_fd(stack->sessions, e->fd))){
				ret = thttp_session_signal_closed(session);
			}
			goto bail;

		case event_connected:
		default:{
				return 0;
			}
	}
	
	/* Gets the associated dialog */
	if((session = thttp_session_get_by_fd(stack->sessions, e->fd))){
		if(!(dialog = thttp_dialog_get_oldest(session->dialogs))){
			TSK_DEBUG_ERROR("Failed to found associated dialog.");
			ret = -5;
			goto bail;
		}
	}
	else{
		TSK_DEBUG_ERROR("Failed to found associated session.");
		ret = -4;
		goto bail;
	}	

	/* Check if buffer is too big to be valid (have we missed some chuncks?) */
	//if(TSK_BUFFER_SIZE(buf) >= THTTP_MAX_CONTENT_SIZE){
	//	tsk_buffer_cleanup(dialog->buf);
	//}

	/* Append new content. */
	tsk_buffer_append(dialog->buf, e->data, e->size);
	
	/* Check if we have all HTTP headers. */
	if((endOfheaders = tsk_strindexOf(TSK_BUFFER_DATA(dialog->buf), TSK_BUFFER_SIZE(dialog->buf), "\r\n\r\n"/*2CRLF*/)) < 0){
		TSK_DEBUG_INFO("No all HTTP headers in the TCP buffer.");
		goto bail;
	}
	
	/* If we are here this mean that we have all HTTP headers.
	*	==> Parse the HTTP message without the content.
	*/
	tsk_ragel_state_init(&state, TSK_BUFFER_DATA(dialog->buf), endOfheaders + 4/*2CRLF*/);
	if(!(ret = thttp_message_parse(&state, &message, tsk_false/* do not extract the content */)))
	{
		const thttp_header_Transfer_Encoding_t* transfer_Encoding;

		/* chunked? */
		if((transfer_Encoding = (const thttp_header_Transfer_Encoding_t*)thttp_message_get_header(message, thttp_htype_Transfer_Encoding)) && tsk_striequals(transfer_Encoding->encoding, "chunked")){
			const char* start = TSK_BUFFER_TO_U8(dialog->buf) + (endOfheaders + 4/*2CRLF*/);
			const char* end = TSK_BUFFER_TO_U8(dialog->buf) + TSK_BUFFER_SIZE(dialog->buf);
			int index;

			TSK_DEBUG_INFO("CHUNKED transfer.");
			while(start < end){
				/* RFC 2616 - 19.4.6 Introduction of Transfer-Encoding */
				// read chunk-size, chunk-extension (if any) and CRLF
				size_t chunk_size = atoi(start);
				if((index = tsk_strindexOf(start, (end-start), "\r\n")) >=0){
					start += index + 2/*CRLF*/;
				}
				else{
					TSK_DEBUG_INFO("Parsing chunked data has failed.");
					break;
				}

				if(chunk_size == 0 && ((start + 2) <= end) && *start == '\r' && *(start+ 1) == '\n'){
					have_all_content = tsk_true;
					break;
				}
					
				thttp_message_append_content(message, start, chunk_size);
				start += chunk_size + 2/*CRLF*/;
			}
		}
		else{
			size_t clen = THTTP_MESSAGE_CONTENT_LENGTH(message); /* MUST have content-length header. */
			if(clen == 0){ /* No content */
				tsk_buffer_remove(dialog->buf, 0, (endOfheaders + 4/*2CRLF*/)); /* Remove HTTP headers and CRLF ==> must never happen */
				have_all_content = tsk_true;
			}
			else{ /* There is a content */
				if((endOfheaders + 4/*2CRLF*/ + clen) > TSK_BUFFER_SIZE(dialog->buf)){ /* There is content but not all the content. */
					TSK_DEBUG_INFO("No all HTTP content in the TCP buffer.");
					goto bail;
				}
				else{
					/* Add the content to the message. */
					thttp_message_add_content(message, tsk_null, TSK_BUFFER_TO_U8(dialog->buf) + endOfheaders + 4/*2CRLF*/, clen);
					/* Remove HTTP headers, CRLF and the content. */
					tsk_buffer_remove(dialog->buf, 0, (endOfheaders + 4/*2CRLF*/ + clen));
					have_all_content = tsk_true;
				}
			}
		}
	}
	
	/* Alert the operation (FSM) */
	if(message){
		if(have_all_content){ /* only if we have all data */
			ret = thttp_dialog_fsm_act(dialog, atype_i_message, message, tsk_null);
		}
	}

bail:
	TSK_OBJECT_SAFE_FREE(dialog);
	TSK_OBJECT_SAFE_FREE(session);
	TSK_OBJECT_SAFE_FREE(message);

	tsk_safeobj_unlock(stack);

	return ret;
}

/** Internal function used to set values.
*/
int __thttp_stack_set(thttp_stack_t *self, va_list values)
{
	thttp_stack_param_type_t curr;

	while((curr=va_arg(values, thttp_stack_param_type_t)) != pname_null)
	{
		switch(curr)
		{
			//
			// Network
			//
		case pname_local_ip:
			{ /* STR */
				tsk_strupdate(&self->local_ip, va_arg(values, const char*));
				break;
			}
		case pname_local_port:
			{	/* INT */
				self->local_port = va_arg(values, int);
				break;
			}
		
			//
			// TLS
			//
		case pname_tls_certs:
			{	/* A_FILE_STR, PUB_FILE_STR, PRIV_FILE_STR */
				tsk_strupdate(&self->tls.ca, va_arg(values, const char*));
				tsk_strupdate(&self->tls.pbk, va_arg(values, const char*));
				tsk_strupdate(&self->tls.pvk, va_arg(values, const char*));
				break;
			}


		default:
			{
				TSK_DEBUG_WARN("Found unknown pname.");
				goto bail;
			}

		}/* switch */
	}/* while */

bail:
	return 0;
}

/**@ingroup thttp_stack_group
* Creates new HTTP/HTTPS stack.
* @param callback Pointer to the callback function to call when new messages come to the transport layer.
* Can be set to Null if you don't want to be alerted.
* @param ... Configuration parameters. You must use @a THTTP_STACK_SET_* macros to set these parameters.
* The list of parameters must end with @ref THTTP_STACK_SET_NULL() even if there is no parameter.<br>
* @retval A Pointer to the newly created stack if succeed and @a Null otherwise.
* A session is a well-defined object.
*
* @code
* thttp_stack_create(callback,
*	THTTP_STACK_SET_*(),
*	THTTP_STACK_SET_NULL());
* @endcode
*
* @sa @ref thttp_stack_set
*/
thttp_stack_handle_t *thttp_stack_create(thttp_stack_callback callback, ...)
{
	thttp_stack_t* stack = tsk_object_new(thttp_stack_def_t);
	va_list params;

	if(!stack){
		return 0;
	}
	stack->local_ip = TNET_SOCKET_HOST_ANY;
	stack->local_port = TNET_SOCKET_PORT_ANY;

	stack->callback = callback;
	va_start(params, callback);
	if(__thttp_stack_set(stack, params)){
		// Delete the stack?
	}
	va_end(params);

	return stack;
}

/**@ingroup thttp_stack_group
* Starts the stack.
* @param self A pointer to the stack to start. The stack must be create using @ref thttp_stack_create.
* @retval Zero if succeed and non-zero error code otherwise.
* @sa @ref thttp_stack_stop
*/
int thttp_stack_start(thttp_stack_handle_t *self)
{
	int ret = -1;
	thttp_stack_t *stack = self;

	if(!stack || stack->transport){
		return ret;
	}

	stack->transport = TNET_TRANSPORT_CREATE(stack->local_ip, stack->local_port, tnet_socket_type_tcp_ipv46, "HTTP/HTTPS transport");
	tnet_transport_set_callback(stack->transport, TNET_TRANSPORT_CB_F(thttp_transport_layer_stream_cb), self);

	if(!(ret = tnet_transport_start(stack->transport))){
		// Sets TLS certificates
		if(stack->tls.ca){
			tsk_strupdate(&stack->transport->tls.ca, stack->tls.ca);
			tsk_strupdate(&stack->transport->tls.pvk, stack->tls.pvk);
			tsk_strupdate(&stack->transport->tls.pbk, stack->tls.pbk);
		}
	}
	return ret;
}

/**@ingroup thttp_stack_group
* Sets the configuration parameters.
* @param self A pointer to the stack to start. The stack must be create using @ref thttp_stack_create.
* @param ... Configuration parameters. You must use @a THTTP_STACK_SET_* macros to set these parameters.
* The list of parameters must end with @ref THTTP_STACK_SET_NULL() even if there is no parameter.<br>
* @retval A Pointer to the newly created stack if succeed and @a Null otherwise.
*
* @code
* thttp_stack_set(stack, 
*	THTTP_STACK_SET_*(),
*	THTTP_STACK_SET_NULL());
* @endcode
*/
int thttp_stack_set(thttp_stack_handle_t *self, ...)
{
	if(self){
		int ret;
		thttp_stack_t *stack = self;
		
		va_list params;
		va_start(params, self);
		ret = __thttp_stack_set(stack, params);
		va_end(params);
		return ret;
	}

	return -1;
}

/**@ingroup thttp_stack_group
* Stops the stack. The stack must be already started.
* @param self A pointer to the stack to stop. The stack must be create using @ref thttp_stack_create.
* @retval Zero if succeed and non-zero error code otherwise.
* @sa @ref thttp_stack_start
*/
int thttp_stack_stop(thttp_stack_handle_t *self)
{
	thttp_stack_t *stack = self;

	if(!stack){
		return -1;
	}
	return tnet_transport_shutdown(stack->transport);
}

/** Alerts the user.
*/
int thttp_stack_alert(const thttp_stack_t *self, const thttp_event_t* e)
{
	if(!self || !e){
		return -1;
	}

	if(self->callback){
		return self->callback(e);
	}
	else{
		return 0;
	}
}

























//========================================================
//	HTTP stack object definition
//
static void* _thttp_stack_create(void * self, va_list * app)
{
	thttp_stack_t *stack = self;
	if(stack){
		tsk_safeobj_init(stack);

		stack->sessions = TSK_LIST_CREATE();
	}
	return self;
}

static void* thttp_stack_destroy(void * self)
{ 
	thttp_stack_t *stack = self;
	if(stack){	
		/* TLS */
		TSK_FREE(stack->tls.ca);
		TSK_FREE(stack->tls.pbk);
		TSK_FREE(stack->tls.pvk);

		/* Sessions */
		tsk_safeobj_lock(stack);
		TSK_OBJECT_SAFE_FREE(stack->sessions);
		tsk_safeobj_unlock(stack);

		/* Network */
		TSK_FREE(stack->local_ip);
		TSK_OBJECT_SAFE_FREE(stack->transport);

		tsk_safeobj_deinit(stack);
	}
	return self;
}

static const tsk_object_def_t thttp_stack_def_s = 
{
	sizeof(thttp_stack_t),
	_thttp_stack_create, 
	thttp_stack_destroy,
	0, 
};
const void *thttp_stack_def_t = &thttp_stack_def_s;