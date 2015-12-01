/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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

/**@file tsip_api_common.c
 * @brief Public common functions.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/api/tsip_api_common.h"

#include "tinysip/tsip_action.h"

#include "tsk_runnable.h"
#include "tsk_debug.h"

/* Internal functions */
extern tsip_action_t* _tsip_action_create(tsip_action_type_t type, va_list* app);
/* Local functions */
int _tsip_api_common_any(const tsip_ssession_handle_t *ss, tsip_action_type_t type, va_list* app);

/* internal function used to execute any user action 
* can only handle session with dialogs */
int _tsip_api_common_any(const tsip_ssession_handle_t *ss, tsip_action_type_t type, va_list* app)
{
	int ret = -1;
	tsip_action_t* action;
	const tsip_ssession_t* _ss;

	/* Checks for validity */
	if(!(_ss = ss) || !_ss->stack){
		TSK_DEBUG_ERROR("Invalid parameter.");
		return ret;
	}
	
	/* Checks if the stack has been started */
	if(!TSK_RUNNABLE(_ss->stack)->started){
		TSK_DEBUG_ERROR("Stack not started.");
		return -2;
	}

	/* execute action */
	if((action = _tsip_action_create(type, app))){
		ret = tsip_ssession_handle(_ss, action);
		TSK_OBJECT_SAFE_FREE(action);
	}
	return ret;
}

/**@ingroup tsip_action_group
* Rejects an incoming request.
* @param ss The SIP Session managing the dialog on which the request has been received.
* @param ... Any TSIP_ACTION_SET_*() macros. e.g. @ref TSIP_ACTION_SET_HEADER(). 
* MUST always ends with @ref TSIP_ACTION_SET_NULL().
* @retval Zero if succeed and non-zero error code otherwise.
*/
int tsip_api_common_reject(const tsip_ssession_handle_t *ss, ...)
{
	int ret = -1;
	va_list ap;

	va_start(ap, ss);
	if((ret = _tsip_api_common_any(ss, tsip_atype_reject, &ap))){
		TSK_DEBUG_ERROR("Reject() failed.");
	}
	va_end(ap);

	return ret;
}

/**@ingroup tsip_action_group
* Hangs up a session.
* @param ss The SIP Session to hang-up. Will send an unREGISTER or unSUBSCRIBE or unPUBLISH or
* BYE etc depending on the type of the SIP dialog managed by the session. 
* @param ... Any TSIP_ACTION_SET_*() macros. e.g. @ref TSIP_ACTION_SET_HEADER(). 
* MUST always ends with @ref TSIP_ACTION_SET_NULL().
* @retval Zero if succeed and non-zero error code otherwise.
*/
int tsip_api_common_hangup(const tsip_ssession_handle_t *ss, ...)
{
	int ret = -1;
	va_list ap;

	va_start(ap, ss);
	if((ret = _tsip_api_common_any(ss, tsip_atype_hangup, &ap))){
		TSK_DEBUG_ERROR("Hang-up() failed.");
	}
	va_end(ap);

	return ret;
}

/**@ingroup tsip_action_group
* Accepts an incoming request.
* @param ss The SIP Session managing the dialog on which the request has been received.
* @param ... Any TSIP_ACTION_SET_*() macros. e.g. @ref TSIP_ACTION_SET_HEADER(). 
* MUST always ends with @ref TSIP_ACTION_SET_NULL().
* @retval Zero if succeed and non-zero error code otherwise.
*/
int tsip_api_common_accept(const tsip_ssession_handle_t *ss, ...)
{
	int ret = -1;
	va_list ap;

	va_start(ap, ss);
	if((ret = _tsip_api_common_any(ss, tsip_atype_accept, &ap))){
		TSK_DEBUG_ERROR("Accept() failed.");
	}
	va_end(ap);

	return ret;
}


