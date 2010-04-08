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
/**@file txcap.h
 * @brief RFC 4825 (XCAP) implementation.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "txcap.h"

#include "tinyHTTP/thttp_url.h"

/**@defgroup txcap_stack_group XCAP stack
*/

/** Internal function used to set options.
*/
int __txcap_stack_set(txcap_stack_t* self, va_list *app)
{
	txcap_stack_param_type_t curr;
	tsk_bool_t cred_updated = tsk_false;

	if(!self || !self->http_session){
		return -1;
	}

	while((curr = va_arg(*app, txcap_stack_param_type_t)) != xcapp_null){
		switch(curr){
			case xcapp_option:
				{	/* (txcap_stack_option_t)ID_INT, (const char*)VALUE_STR */
					txcap_stack_option_t ID_IN = va_arg(*app, txcap_stack_option_t);
					const char* VALUE_STR = va_arg(*app, const char*);
					switch(ID_IN){
						/* PASSWORD and XUI are not used as options in the HTTP/HTTPS stack */
						case TXCAP_STACK_OPTION_PASSWORD:
							{
								tsk_strupdate(&self->password, VALUE_STR);
								cred_updated = tsk_true;
								break;
							}
						case TXCAP_STACK_OPTION_XUI:
							{
								tsk_strupdate(&self->xui, VALUE_STR);
								cred_updated = tsk_true;
								break;
							}
						default:
							{
								tsk_options_add_option(&((thttp_session_t*)self->http_session)->options, ID_IN, VALUE_STR);
								break;
							}
					}
					break;
				}
			
			case xcapp_header:
				{	/* (const char*)NAME_STR, (const char*)VALUE_STR */
					const char* NAME_STR = va_arg(*app, const char*);
					const char* VALUE_STR = va_arg(*app, const char*);
					tsk_params_add_param(&((thttp_session_t*)self->http_session)->headers, NAME_STR, VALUE_STR);
					break;
				}
			
			case xcapp_context:
				{	/* (const void*)CTX_PTR */
					const void* CTX_PTR = va_arg(*app, const void*);
					((thttp_session_t*)self->http_session)->context = CTX_PTR;
					break;
				}

			case xcapp_auid:
				{	/* (const char*)ID_STR, (const char*)MIME_TYPE_STR, (const char*)NS_STR, (const char*)DOC_NAME_STR, (tsk_bool_t)IS_GLOBAL_BOOL */
					const char* ID_STR = va_arg(*app, const char*);
					const char* MIME_TYPE_STR = va_arg(*app, const char*);
					const char* NS_STR = va_arg(*app, const char*);
					const char* DOC_NAME_STR = va_arg(*app, const char*);
					tsk_bool_t IS_GLOBAL_BOOL = va_arg(*app, tsk_bool_t);
					
					if(txcap_auid_register(self->auids, ID_STR, MIME_TYPE_STR, NS_STR, DOC_NAME_STR, IS_GLOBAL_BOOL)){
						// do nothing
					}

					break;
				}

			default:
				{
					TSK_DEBUG_ERROR("NOT SUPPORTED.");
					goto bail;
				}
		} /* switch */
	} /* while */

	if(cred_updated && self->http_session){
		/* credentials */
		thttp_session_set(self->http_session,
			THTTP_SESSION_SET_CRED(self->xui, self->password),
			THTTP_SESSION_SET_NULL());
	}
	return 0;

bail:
	return -2;
}

/**@ingroup txcap_stack_group
* Creates new XCAP stack.
* @param callback Poiner to the callback function to call when new messages come to the transport layer.
* Can be set to Null if you don't want to be alerted.
* @param xui user's id as per RFC 4825 subclause 4. Also used to fill @b "X-3GPP-Intended-Identity" header.
* This paramter is mandatory and must not be null. If for any reason you'd like to update the user's id, then use @ref TXCAP_STACK_SET_XUI().
* @param password user's password used to authenticate to the XDMS.
* This parameter is not mandatory. If for any reason you'd like to update the password, then use @ref TXCAP_STACK_SET_PASSWORD().
* @param xcap_root xcap-root URI as per RFC 4825 subclause 6.1, used to build all request-uris. 
* This parameter is not mandatory and must be a valid HTPP/HTTPS URL.
* @param ... User configuration. You must use @a TXCAP_STACK_SET_*() macros to set these options.
* The list of options must always and with @ref TXCAP_STACK_SET_NULL() even if these is no option to pass to the stack.
* @retval A Pointer to the newly created stack if succeed and @a Null otherwise.
* A stack is a well-defined object.
*
* @code
*
* @endcode
*
* @sa @ref txcap_stack_set
*/
txcap_stack_handle_t* txcap_stack_create(thttp_stack_callback callback, const char* xui, const char* password, const char* xcap_root, ...)
{
	txcap_stack_t* ret = tsk_null;

	if(!xui || !xcap_root){
		TSK_DEBUG_ERROR("Both xui and xcap_root are mandatory and should be non-null");
		goto bail;
	}

	/* check url validity */
	if(!thttp_url_isok(xcap_root)){
		TSK_DEBUG_ERROR("%s is not a valid HTTP/HTTPS url", xcap_root);
		goto bail;
	}

	if(!(ret = tsk_object_new(txcap_stack_def_t, callback, xui, password, xcap_root))){
		TSK_DEBUG_FATAL("Failed to create the XCAP stack");
		goto bail;
	}
	else{
		/* set parameters */
		va_list ap;
		va_start(ap, xcap_root);
		__txcap_stack_set(ret, &ap);
		va_end(ap);
		/* credendials */
		tsk_strupdate(&ret->xui, xui);
		tsk_strupdate(&ret->password, password);
		if(ret->http_session){
			thttp_session_set(ret->http_session,
				THTTP_SESSION_SET_CRED(ret->xui, ret->password),
				THTTP_SESSION_SET_NULL());
		}
	}

bail:
	return ret;
}

/**@ingroup txcap_stack_group
* Starts the stack.
* @param self A pointer to the stack to stark. The stack shall be created using @ref txcap_stack_create().
* @retval Zero if succeed and non-zero error code otherwise.
* @sa @ref txcap_stack_stop
*/
int txcap_stack_start(txcap_stack_handle_t* self)
{
	int ret = -1;
	txcap_stack_t* stack = self;

	if(!stack){
		goto bail;
	}

	ret = thttp_stack_start(stack->http_stack);

bail:
	return ret;
}

/**@ingroup txcap_stack_group
* Updates the stack configuration.
* @param self  The XCAP stack to update. The stack shall be created using @ref txcap_stack_create().
* @param ... Any @a TXCAP_STACK_SET_*() macros. MUST ends with @ref TXCAP_STACK_SET_NULL().
* @retval Zero if succeed and non-zero error code otherwise.
*
* @code
*
* @endcode
*
* @sa @ref txcap_stack_create
*/
int txcap_stack_set(txcap_stack_handle_t* self, ...)
{
	int ret = -1;
	va_list ap;

	if(!self){
		goto bail;
	}
	
	va_start(ap, self);
	ret = __txcap_stack_set(self, &ap);
	va_end(ap);

bail:
	return ret;
}

/**@ingroup txcap_stack_group
* Stops the stack. The stack must already be started.
* @param self A pointer to the stack to stop. The stack shall be created using @ref txcap_stack_create().
* @retval Zero if succeed and non-zero error code otherwise.
* @sa @ref txcap_stack_create
*/
int txcap_stack_stop(txcap_stack_handle_t* self)
{
	int ret = -1;
	txcap_stack_t* stack = self;

	if(!stack){
		goto bail;
	}
	else{
		ret = thttp_stack_stop(stack->http_stack);
	}

bail:
	return ret;
}









//========================================================
//	XCAP stack object definition
//
static tsk_object_t* _txcap_stack_create(tsk_object_t * self, va_list * app)
{
	txcap_stack_t *stack = self;
	if(stack){
		thttp_stack_callback callback;
		tsk_safeobj_init(stack);

		callback = va_arg(*app, thttp_stack_callback);
		stack->xui = tsk_strdup( va_arg(*app, const char*) );
		stack->password = tsk_strdup( va_arg(*app, const char*) );
		stack->xcap_root = tsk_strdup( va_arg(*app, const char*) );
		
		/* HTTP/HTTPS resources */
		stack->http_stack = thttp_stack_create(callback,
			THTTP_STACK_SET_NULL());
		stack->http_session = thttp_session_create(stack->http_stack ,
			THTTP_SESSION_SET_NULL());
		
		/* Options */
		stack->options = TSK_LIST_CREATE();

		/* AUIDs */
		txcap_auids_init(&stack->auids);
	}
	return self;
}

static tsk_object_t* txcap_stack_destroy(tsk_object_t* self)
{ 
	txcap_stack_t *stack = self;
	if(stack){	
		/* vars */
		TSK_FREE(stack->xui);
		TSK_FREE(stack->password);
		TSK_FREE(stack->xcap_root);
		
		/* HTTP/HTTPS resources */
		TSK_OBJECT_SAFE_FREE(stack->http_session);
		TSK_OBJECT_SAFE_FREE(stack->http_stack);

		/* Options */
		TSK_OBJECT_SAFE_FREE(stack->options);

		/* AUIDs */
		TSK_OBJECT_SAFE_FREE(stack->auids);
		
		tsk_safeobj_deinit(stack);
	}
	return self;
}

static const tsk_object_def_t txcap_stack_def_s = 
{
	sizeof(txcap_stack_t),
	_txcap_stack_create, 
	txcap_stack_destroy,
	tsk_null, 
};
const tsk_object_def_t *txcap_stack_def_t = &txcap_stack_def_s;
