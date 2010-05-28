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

#include "tinyhttp/thttp_action.h"
#include "tinyhttp/thttp_event.h"

#include "tinyhttp/thttp_message.h"
#include "tinyhttp/parsers/thttp_parser_message.h"

#include "tinyhttp/headers/thttp_header_Transfer_Encoding.h"

#include "tinyhttp/thttp_dialog.h"

#include "tnet.h"

#include "tsk_runnable.h"
#include "tsk_time.h"
#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"


/** @mainpage TinyHTTP API Overview
*
* <h1>15 HTTP/HTTPS</h1>
* <p>
* The HTTP/HTTPS stack is a basic thread-safe client API and is used in conjunction with the XCAP protocol. 
* Almost all HTTP methods such as OPTIONS, HEAD, GET, DELETE, POST, CONNET, TRACE or PUT … are supported for outgoing requests. Only response messages will be correctly handled by the stack. Requests will be passed to the application layer “as is” and no new connection will be opened.
* </p>
* <p>
* Both IPv4 and IPv6 are supported. If the host name (FQDN) resolution leads to both IPv4 and IPv6 results, then IPv4 will be used by default.
* </p>
* <p>
* When HTTPS is used, then both end-user and mutual authentication modes are supported. For mutual authentication the TLS/SSL certificate files MUST be sets by using @ref THTTP_STACK_SET_TLS_CERTS(CA_FILE_STR, PUB_FILE_STR, PRIV_FILE_STR).
* </p>
*
*
*
* 
* <h2>15.1	Initialization</h2>
* <p>
* As the HTTP/HTTPS stack depends on the network library (tinyNET), you MUST call tnet_startup() before using any HTTP/Network function (thttp_*). tnet_cleanup() is used to terminate use of network functions.
* </p>
* <p>
* The example below demonstrates how to create and start a HTTP/HTTPS stack. The callback function shows the most common events.
* @code
int ret;

int test_stack_callback(const thttp_event_t *httpevent)
{
	switch(httpevent->type){
		case thttp_event_message: // New HTTP message
			{
				break;
			}
		case thttp_event_auth_failed: // Authentication failed
			{
				break;
			}


		case thttp_event_closed: // HTTP connection closed (informational)
			{
				break;
			}
	}
	
	return 0;
}

thttp_stack_handle_t* stack = thttp_stack_create(test_stack_callback,
	// TLS certificates (not mandatory) to show how parameters are passed to the stack
	THTTP_STACK_SET_TLS_CERTS("C:\\tls\\ca.pki-crt.pem", "C:\\tls\\pub-crt.pem", "C:\\tls\\pub-key.pem"),
	THTTP_STACK_SET_NULL());// MUST always be present

if((ret = thttp_stack_start(stack))){
	TSK_DEBUG_ERROR("Failed to start the HTTP/HTTPS stack.");
	goto bail;
}
* @endcode
*
* A stack is a well-defined object and must be destroyed by using @a TSK_OBJECT_SAFE_FREE() macro.
*
*
*
*<h2>15.2	Sessions</h2>
* <p>
* A session could be seen as a peer2peer persistent connection and will be maintained by the stack as long as you wish to keep the network connection opened (not explicitly destroyed). <br>
* If the connection is closed by the remote peer, then the stack will automatically reopen it when you try to send a new HTTP/HTTP request. <br>
* The network connection will be definitely closed when the session is destroyed.
* </p>
* <p>
* As the connection is persistent, then you can send multiple requests without waiting for each response. This mode is called “Pipelining” and is defined as per RFC 2616 section 8.1.2.2.
* </p>
* <p>
* You should not pipeline requests using non-idempotent methods or non-idempotent sequences of methods. This means that you can safely pipeline GET or HEAD methods but should not with PUT or POST requests. Only HTTP version 1.1(or later) requests should be pipelined.<br>
* </p>
* <p>
* To avoid pipelining, you must use a session object (@ref thttp_session_handle_t*) only once to send a single request.
* </p>
* <p>
* The example below shows how to create and configure a session.
* </p>
*
* @code

// creates session
thttp_session_handle_t * session = thttp_session_create(stack,
	// session-level credentials
THTTP_SESSION_SET_CRED("ali baba", "open sesame"),

	// session-level options
	THTTP_SESSION_SET_OPTION(THTTP_SESSION_OPTION_TIMEOUT, "6000"),

	// session-level headers
	THTTP_SESSION_SET_HEADER("Pragma", "No-Cache"),
	THTTP_SESSION_SET_HEADER("Connection", "Keep-Alive"),
	THTTP_SESSION_SET_HEADER("User-Agent", "doubango 1.0"),
			
	THTTP_SESSION_SET_NULL());// MUST always be present
	
* @endcode
*
* A session is a well-defined object and must be destroyed by using @a TSK_OBJECT_SAFE_FREE() macro.
*
*
*
* <h3>15.2.1 Credentials</h3>
* <p>
* Both HTTP digest and Basic authentication (RFC 2617) are supported in this version (1.0). The credentials (username and password) should be set when the session is created (@ref thttp_session_create()) or later, by using @ref thttp_session_set(). As credentials are configured at session level, you can use one stack to authenticate against multiple HTTP servers with different domains.
* </p>
* <p>
* The stack will automatically add authorizations (as per RFC 2617) when it is challenged (401/407 responses), this is why you should set credentials before sending any requests which is susceptible to be challenged. 
* </p>
* 
* 
* 
* <h3>15.2.2 Options</h3>
* <p>
* All options shall be set by using @ref THTTP_SESSION_SET_OPTION(id, value) macro. Session-level options will be applied to all underlying requests, unless the request redefines this option.
* </p>
*
* 
* 
* <h3>15.2.3 Headers</h3>
* <p>
* All headers shall be set by using @ref THTTP_SESSION_SET_HEADER(name, value) macro. Session-level headers will be added to all underlying requests even if a request redefines this header. This means that if both the request and the session have the same header, then it will be duplicated.<br>
* <i>Host</i> and <i>Content-Length</i> headers are automatically added by the stack.
* </p>
* <p>
* @ref THTTP_SESSION_UNSET_HEADER() and @ref THTTP_SESSION_SET_HEADER() macros are used to remove or update a previously added session-level header, respectively.
* </p>
* 
* <h2>15.3 Requests</h2>
* <p>
* A HTTP request is referred to as an “action” and is always associated to a session. There are nine well-know actions you can perform: <b>CONNET</b>, <b>DELETE</b>, <b>GET</b>, <b>HEAD</b>, <b>OPTIONS</b>, <b>PATCH</b>, <b>POST</b>, <b>PUT</b> and <b>TRACE</b>. You can use @ref thttp_action_perform(session, url, method, …) function to send a custom request. All <i>thttp_action_*()</i> functions are non-blocking.<br>
* </p>
* <p>
* All requests are sent in an asynchronous manner and the result (HTTP messages, errors, time out …) will be passed to the callback function.
* </p>
*
* The code below shows how to send HTTP <b>PUT</b> request.
*
* @code
int ret = thttp_action_PUT(session, "http://www.doubango.org",
		// action-level options
		THTTP_ACTION_SET_OPTION(THTTP_ACTION_OPTION_TIMEOUT, "2500"),
		
		// payload
		THTTP_ACTION_SET_PAYLOAD("Comment allez-vous?", tsk_strlen("Comment allez-vous?")),

		// action-level headers
		THTTP_ACTION_SET_HEADER("Content-Type", "text/plain"),
		THTTP_ACTION_SET_HEADER("Expect", "100-continue"),
		
		THTTP_ACTION_SET_NULL());

* @endcode

* The code below uses HTTP <b>GET</b> request to connect to an IPv6 website
*
* @code
int ret = thttp_action_GET(session, "http://ipv6.google.com",
		// action-level options
		THTTP_ACTION_SET_OPTION(THTTP_ACTION_OPTION_TIMEOUT, "4500"),
		
		THTTP_ACTION_SET_NULL());

* @endcode
*
* You can notice that, there is nothing special to do in order to connect to an IPv6 website.
*
*
*
* <h3>15.3.1	Options</h3>
* <p>
* All options shall be set by using @ref THTTP_ACTION_SET_OPTION(id, value) macro. 
* Action-level options and headers are only applied to the current request. 
* In the code, the timeout previously defined by the session has been redefined (from 6000ms to 2500ms).
* </p>
*
* For more information, please visit the website.
*
*
*
* <h3>15.3.2 Headers</h3>
* <p>
* All headers shall be set by using @ref THTTP_ACTION_SET_HEADER(name, value) macro. 
* Action-level headers will only be added to the current request. <i>Host</i> and <i>Content-Length</i> headers are automatically added by the stack.
* </p>

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

		case event_error:
			// alert all dialogs
			if((session = thttp_session_get_by_fd(stack->sessions, e->fd))){
				ret = thttp_session_signal_error(session);
			}
			goto bail;
		case event_connected:
		default:{
				tsk_safeobj_unlock(stack);
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
			const char* start = (const char*)(TSK_BUFFER_TO_U8(dialog->buf) + (endOfheaders + 4/*2CRLF*/));
			const char* end = (const char*)(TSK_BUFFER_TO_U8(dialog->buf) + TSK_BUFFER_SIZE(dialog->buf));
			int index;

			TSK_DEBUG_INFO("CHUNKED transfer.");
			while(start < end){
				/* RFC 2616 - 19.4.6 Introduction of Transfer-Encoding */
				// read chunk-size, chunk-extension (if any) and CRLF
				tsk_size_t chunk_size = (tsk_size_t)tsk_atox(start);
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
			tsk_size_t clen = THTTP_MESSAGE_CONTENT_LENGTH(message); /* MUST have content-length header. */
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
			ret = thttp_dialog_fsm_act(dialog, thttp_atype_i_message, message, tsk_null);
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
int __thttp_stack_set(thttp_stack_t *self, va_list* app)
{
	thttp_stack_param_type_t curr;

	while((curr = va_arg(*app, thttp_stack_param_type_t)) != thttp_pname_null)
	{
		switch(curr)
		{
			//
			// Network
			//
		case thttp_pname_local_ip:
			{ /* STR */
				tsk_strupdate(&self->local_ip, va_arg(*app, const char*));
				break;
			}
		case thttp_pname_local_port:
			{	/* INT */
				self->local_port = va_arg(*app, int);
				break;
			}
		
			//
			// TLS
			//
		case thttp_pname_tls_certs:
			{	/* A_FILE_STR, PUB_FILE_STR, PRIV_FILE_STR */
				tsk_strupdate(&self->tls.ca, va_arg(*app, const char*));
				tsk_strupdate(&self->tls.pbk, va_arg(*app, const char*));
				tsk_strupdate(&self->tls.pvk, va_arg(*app, const char*));
				break;
			}

			//
			// Userdata
			//
		case thttp_pname_userdata:
			{ /* (const void*)USERDATA_PTR */
				self->userdata = va_arg(*app, const void*);
				break;
			}

		default:
			{
				TSK_DEBUG_WARN("Found unknown pname.");
				goto bail;
			}

		}/* switch */
	}/* while */
	return 0;

bail:
	return -2;
}

/**@ingroup thttp_stack_group
* Creates new HTTP/HTTPS stack.
* @param callback Pointer to the callback function to call when new messages come to the transport layer.
* Can be set to Null if you don't want to be alerted.
* @param ... Configuration parameters. You must use @a THTTP_STACK_SET_*() macros to set these parameters.
* The list of parameters must end with @ref THTTP_STACK_SET_NULL() even if there is no parameter to pass to the stack.<br>
* @retval A Pointer to the newly created stack if succeed and @a Null otherwise.
* A stack is a well-defined object.
*
* @code
* thttp_stack_create(callback,
*	THTTP_STACK_SET_*(),
*	THTTP_STACK_SET_NULL());
* @endcode
*
* @sa @ref thttp_stack_set
*/
thttp_stack_handle_t *thttp_stack_create(thttp_stack_callback_f callback, ...)
{
	thttp_stack_t* stack = tsk_null;
	va_list ap;

	if(!(stack = tsk_object_new(thttp_stack_def_t))){ /* should never happen */
		TSK_DEBUG_ERROR("Failed to create new HTTP/HTTPS stack.");
		return tsk_null;
	}
	stack->local_ip = TNET_SOCKET_HOST_ANY;
	stack->local_port = TNET_SOCKET_PORT_ANY;

	stack->callback = callback;
	va_start(ap, callback);
	if(__thttp_stack_set(stack, &ap)){
		TSK_DEBUG_ERROR("Failed to set user's parameters.");
		TSK_OBJECT_SAFE_FREE(stack);
	}
	va_end(ap);

	return stack;
}

/**@ingroup thttp_stack_group
* Starts the stack.
* @param self A pointer to the stack to start. The stack shall be created using @ref thttp_stack_create().
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

	stack->transport = tnet_transport_create(stack->local_ip, stack->local_port, tnet_socket_type_tcp_ipv46, "HTTP/HTTPS transport");
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
		
		va_list ap;
		va_start(ap, self);
		ret = __thttp_stack_set(stack, &ap);
		va_end(ap);
		return ret;
	}
	else{
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
}

/**@ingroup thttp_stack_group
*/
const void* thttp_stack_get_userdata(thttp_stack_handle_t *self)
{
	thttp_stack_t *stack = self;
	if(stack){
		return stack->userdata;
	}
	else{
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}
}

/**@ingroup thttp_stack_group
* Stops the stack. The stack must already be started.
* @param self A pointer to the stack to stop. The stack shall be created using @ref thttp_stack_create.
* @retval Zero if succeed and non-zero error code otherwise.
* @sa @ref thttp_stack_start
*/
int thttp_stack_stop(thttp_stack_handle_t *self)
{
	thttp_stack_t *stack = self;

	if(!stack){
		TSK_DEBUG_ERROR("Invalid parameter");
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
static tsk_object_t* _thttp_stack_create(tsk_object_t * self, va_list * app)
{
	thttp_stack_t *stack = self;
	if(stack){
		tsk_safeobj_init(stack);

		stack->sessions = tsk_list_create();
	}
	return self;
}

static tsk_object_t* thttp_stack_destroy(tsk_object_t * self)
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
	tsk_null, 
};
const tsk_object_def_t *thttp_stack_def_t = &thttp_stack_def_s;
