/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou [at) doubango (DOT) org>
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
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 *

 */
#include "tmsrp.h"

#include "tsk_time.h"
#include "tsk_string.h"

#define TMSR_DEFAULT_NAMESPACE	0 // "000"

tmsrp_request_t* tmsrp_create_bodiless(const tmsrp_uri_t* To, const tmsrp_uri_t* From)
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
    tmsrp_request_t* BODILESS = tsk_null;
    tsk_istr_t tid;
    tsk_istr_t mid;

    tsk_strrandom(&tid);
    tsk_strrandom(&mid);

    if(!(BODILESS = tmsrp_request_create(tid, "SEND"))) {
        goto bail;
    }
    // To-Path
    // From-Path
    // Message-ID
    tmsrp_message_add_headers(BODILESS,
                              TMSRP_HEADER_TO_PATH_VA_ARGS(To),
                              TMSRP_HEADER_FROM_PATH_VA_ARGS(From),
                              TMSRP_HEADER_MESSAGE_ID_VA_ARGS(mid),

                              tsk_null);

bail:
    return BODILESS;
}

tmsrp_response_t* tmsrp_create_response(const tmsrp_request_t* request, short status, const char* comment)
{
    tmsrp_response_t* response = tsk_null;

    if(!request) {
        goto bail;
    }
    /* MSRP response will have the same tid ==> nothing to do */
    if(!(response = tmsrp_response_create(request->tid, status, comment))) {
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

tmsrp_request_t* tmsrp_create_report(const tmsrp_request_t* SEND, short status, const char* reason)
{
    /*	RFC 4975 - 7.1.2. Sending REPORT Requests

    * REPORT requests are similar to SEND requests, except that report
    * requests MUST NOT include Success-Report or Failure-Report header
    * fields, and MUST contain a Status header field.  REPORT requests MUST
    * contain the Message-ID header field from the original SEND request.
    */
    tmsrp_request_t* REPORT = tsk_null;
    tsk_istr_t tid;

    /*	If an MSRP element receives a REPORT for a Message-ID it does not
    	recognize, it SHOULD silently ignore the REPORT.
    */
    if(!SEND || !SEND->MessageID) {
        goto bail;
    }

    /*  Generate new tid (Report has it's own tid) */
    tsk_strrandom(&tid);

    /* MSRP response will have the same tid ==> nothing to do */
    if(!(REPORT = tmsrp_request_create(tid, "REPORT"))) {
        goto bail;
    }

    /* reverse To-Path and From-Path */
    REPORT->To = (tmsrp_header_To_Path_t*)tmsrp_header_From_Path_clone(SEND->From);
    TMSRP_HEADER(REPORT->To)->type = tmsrp_htype_To_Path; /* as it's a clone we shall change type */
    REPORT->From = (tmsrp_header_From_Path_t*)tmsrp_header_To_Path_clone(SEND->To);
    TMSRP_HEADER(REPORT->From)->type = tmsrp_htype_From_Path; /* as it's a clone we shall change type */
    /* Byte-Range */
    REPORT->ByteRange = tsk_object_ref((void*)SEND->ByteRange);

    /* Message ID */
    /* Status */
    tmsrp_message_add_headers(REPORT,
                              TMSRP_HEADER_MESSAGE_ID_VA_ARGS(SEND->MessageID->value),
                              TMSRP_HEADER_STATUS_VA_ARGS(TMSR_DEFAULT_NAMESPACE, status, reason),

                              tsk_null);
bail:
    return REPORT;
}

tsk_bool_t tmsrp_isReportRequired(const tmsrp_request_t* request, tsk_bool_t failed)
{
    if(!request) {
        return tsk_false;
    }

    /* Success Report. */
    if(request->SuccessReport && request->SuccessReport->yes) {
        return tsk_true;
    }

    /* Failure Report */
    if(!request->FailureReport || (request->FailureReport && request->FailureReport->type != freport_no)) {
        return failed;
    }

    return tsk_false;
}
