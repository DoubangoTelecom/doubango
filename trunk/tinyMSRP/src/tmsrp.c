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

/**@file tmsrp.c
 * @brief MSRP API.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tmsrp.h"

#include "tsk_time.h"
#include "tsk_string.h"

#include <stdlib.h> /* rand() */

#define TMSR_DEFAULT_NAMESPACE	0 // "000"

tmsrp_request_t* tmsrp_create_bodiless()
{
	/* RFC 4975 - 7.1. Constructing Requests
		Requests with no bodies are useful when a client wishes to send
		"traffic", but does not wish to send content to be rendered to the
		peer user.  For example, the active endpoint sends a SEND request
		immediately upon establishing a connection.  If it has nothing to
		say at the moment, it can send a request with no body.  Bodiless
		requests may also be used in certain applications to keep Network
		Address Translation (NAT) bindings alive, etc.

		Bodiless requests are distinct from requests with empty bodies.  A
		request with an empty body will have a Content-Type header field
		value and will generally be rendered to the recipient according to
		the rules for that type.
	*/
	tmsrp_request_t* bodiless = TMSRP_NULL;
	tsk_istr_t tid;
	tsk_istr_t mid;

	tsk_strrandom(&tid);
	tsk_strrandom(&mid);

	if(!(bodiless = TMSRP_REQUEST_CREATE(tid, "SEND"))){
		goto bail;
	}
	//FIXME: To-Path
	//FIXME: From-Path

	/* Message-ID */
	TMSRP_MESSAGE_ADD_HEADER(bodiless, TMSRP_HEADER_MESSAGE_ID_VA_ARGS(mid));

bail:
	return bodiless;
}

tmsrp_response_t* tmsrp_create_response(const tmsrp_request_t* request, short status, const char* comment)
{
	tmsrp_response_t* response = TMSRP_NULL;

	if(!request){
		goto bail;
	}
	/* MSRP response will have the same tid ==> nothing to do */
	if(!(response = TMSRP_RESPONSE_CREATE(request->tid, status, comment))){
		goto bail;
	}
	/* reverse To-Path and From-Path */
	response->To = (tmsrp_header_To_Path_t*)tmsrp_header_From_Path_clone(request->From);
	TMSRP_HEADER(response->To)->type = tmsrp_htype_To_Path; /* as it's a clone we shall change type */
	response->From = (tmsrp_header_From_Path_t*)tmsrp_header_To_Path_clone(request->To);
	TMSRP_HEADER(response->From)->type = tmsrp_htype_From_Path; /* as it's a clone we shall change type */
	/* Byte-Range */
	response->ByteRange = tsk_object_ref((void*)request->ByteRange);
	
bail:
	return response;
}

tmsrp_request_t* tmsrp_create_report(const tmsrp_request_t* request, short status, const char* reason)
{
	/*	RFC 4975 - 7.1.2. Sending REPORT Requests
	
	* REPORT requests are similar to SEND requests, except that report
	* requests MUST NOT include Success-Report or Failure-Report header
	* fields, and MUST contain a Status header field.  REPORT requests MUST
	* contain the Message-ID header field from the original SEND request.
	*/
	tmsrp_request_t* report = TMSRP_NULL;
	tsk_istr_t tid;
	
	if(!request || !request->MessageID){
		goto bail;
	}

	/*  Generate new tid (Report has it's own tid) */
	tsk_strrandom(&tid);

	/* MSRP response will have the same tid ==> nothing to do */
	if(!(report = TMSRP_REQUEST_CREATE(tid, "REPORT"))){
		goto bail;
	}

	/* reverse To-Path and From-Path */
	report->To = (tmsrp_header_To_Path_t*)tmsrp_header_From_Path_clone(request->From);
	TMSRP_HEADER(report->To)->type = tmsrp_htype_To_Path; /* as it's a clone we shall change type */
	report->From = (tmsrp_header_From_Path_t*)tmsrp_header_To_Path_clone(request->To);
	TMSRP_HEADER(report->From)->type = tmsrp_htype_From_Path; /* as it's a clone we shall change type */
	/* Message ID */
	TMSRP_MESSAGE_ADD_HEADER(report, TMSRP_HEADER_MESSAGE_ID_VA_ARGS(request->MessageID->value));
	/* Byte-Range */
	report->ByteRange = tsk_object_ref((void*)request->ByteRange);
	/* Status */
	TMSRP_MESSAGE_ADD_HEADER(report, TMSRP_HEADER_STATUS_VA_ARGS(TMSR_DEFAULT_NAMESPACE, status, reason));
	
bail:
	return report;
}

int tmsrp_isReportRequired(const tmsrp_request_t* request)
{
	if(!request){
		return 0;
	}

	/* Success Report. */
	if(request->SuccessReport && request->SuccessReport->yes){
		if(request->Status && (request->Status->code>199 && request->Status->code<300)){
			return 1;
		}
	}

	/* Failure Report */
	if(!request->FailureReport || (request->FailureReport && request->FailureReport->type != freport_no)){
		if(request->Status && (request->Status->code<=199 && request->Status->code>=300)){
			return 1;
		}
	}

	return 0;
}
