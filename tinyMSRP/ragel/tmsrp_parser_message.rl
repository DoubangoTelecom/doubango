/*
* Copyright (C) 2009-2015 Mamadou DIOP.
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
/**@file tmsrp_machine_message.rl
 * @brief Ragel file.
 */
#include "tinymsrp/parsers/tmsrp_parser_message.h"

#include "tinymsrp/headers/tmsrp_header_Dummy.h"
#include "tinymsrp/headers/tmsrp_header_Expires.h"
#include "tinymsrp/headers/tmsrp_header_Max-Expires.h"
#include "tinymsrp/headers/tmsrp_header_Min-Expires.h"
#include "tinymsrp/headers/tmsrp_header_Use-Path.h"
#include "tinymsrp/headers/tmsrp_header_WWW-Authenticate.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

static tsk_bool_t parse_payload(tmsrp_message_t* msrp_msg, const char* tag_start, const char** p, const char* pe, tsk_bool_t* payload_parsed);
static void set_payload(tmsrp_message_t* msrp_msg, const void* ptr, tsk_size_t len);

#define TMSRP_MSG_PARSER_ADD_HEADER(name) \
	if((header = (tmsrp_header_t*)tmsrp_header_##name##_parse(tag_start, (p - tag_start)))){ \
		tmsrp_message_add_header(msrp_msg, header); \
		header = tsk_object_unref(header); \
	}
%%{
	machine tmsrp_machine_message;
	
	###########################################
	#	Includes
	###########################################
	include tmsrp_machine_utils "./ragel/tmsrp_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}

	###########################################
	#	Actions
	###########################################
	action parse_Authentication_Info{
		//FIXME: TMSRP_MSG_PARSER_ADD_HEADER(Authentication_Info);
		TSK_DEBUG_WARN("Authentication_Info Not implemented");
	}
	
	action parse_Authorization{
		//FIXME: TMSRP_MSG_PARSER_ADD_HEADER(Authorization);
		TSK_DEBUG_WARN("Authorization Not implemented");
	}
	
	action parse_Byte_Range{
		TMSRP_MSG_PARSER_ADD_HEADER(Byte_Range);
	}
	
	action parse_Content_Type{
		TMSRP_MSG_PARSER_ADD_HEADER(Content_Type);
	}
	
	action parse_Expires{
		TMSRP_MSG_PARSER_ADD_HEADER(Expires);
	}
	
	action parse_Failure_Report{
		TMSRP_MSG_PARSER_ADD_HEADER(Failure_Report);
	}
	
	action parse_From_Path{
		TMSRP_MSG_PARSER_ADD_HEADER(From_Path);
	}
	
	action parse_Max_Expires{
		TMSRP_MSG_PARSER_ADD_HEADER(Max_Expires);
	}
	
	action parse_Message_ID{
		TMSRP_MSG_PARSER_ADD_HEADER(Message_ID);
	}
	
	action parse_Min_Expires{
		TMSRP_MSG_PARSER_ADD_HEADER(Min_Expires);
	}
	
	action parse_Status{
		TMSRP_MSG_PARSER_ADD_HEADER(Status);
	}
	
	action parse_Success_Report{
		TMSRP_MSG_PARSER_ADD_HEADER(Success_Report);
	}
	
	action parse_To_Path{
		TMSRP_MSG_PARSER_ADD_HEADER(To_Path);
	}
	
	action parse_Use_Path{
		TMSRP_MSG_PARSER_ADD_HEADER(Use_Path);
	}
	
	action parse_WWW_Authenticate{
		TMSRP_MSG_PARSER_ADD_HEADER(WWW_Authenticate);
	}
	
	action parse_Dummy{
		TMSRP_MSG_PARSER_ADD_HEADER(Dummy);
	}
	
	action parse_tid{
		TSK_PARSER_SET_STRING(msrp_msg->tid);
	}

	action parse_method{
		if(msrp_msg->type == tmsrp_unknown){
			msrp_msg->type = tmsrp_request;
			TSK_PARSER_SET_STRING(msrp_msg->line.request.method);
			 msrp_msg->line.request.type = tmsrp_request_get_type(msrp_msg->line.request.method);
		}
		else{
			//cs = %%{ write first_final; }%%;
			cs = tmsrp_machine_message_error;
			TSK_DEBUG_ERROR("Message type already defined.");
		}
	}


	action parse_status_code{
		if(msrp_msg->type == tmsrp_unknown){
			msrp_msg->type = tmsrp_response;
			TSK_PARSER_SET_INT(msrp_msg->line.response.status);
		}
		else{
			//cs = %%{ write first_final; }%%;
			cs = tmsrp_machine_message_error;
			TSK_DEBUG_ERROR("Message type already defined.");
		}
	}
	action parse_comment{
		TSK_PARSER_SET_STRING(msrp_msg->line.response.comment);
	}

	
	action try_parse_data{
		parse_payload(msrp_msg, tag_start, &p, pe, &payload_parsed); // will update "p"
	}

	action parse_data{
		// if the msrp message contain a valid content-type, then gob it otherwise continue until we reach the endline
		int len;
		if(parse_payload(msrp_msg, tag_start, &p, pe, &payload_parsed)){ // will update "p"
			// (This space left deliberately blank)
		}
		else if((len = (int)(p  - tag_start))>0){
			set_payload(msrp_msg, tag_start, (tsk_size_t)len);
		}
	}

	action parse_endtid{
		TSK_PARSER_SET_STRING(msrp_msg->end_line.tid);
	}

	action parse_cflag{
		if(tag_start){
			msrp_msg->end_line.cflag = *tag_start;
		}
		else msrp_msg->end_line.cflag = '#';
	}

	action outside_endline{
		*msg_size = (p - (const char*)input) + 1;
	}
	
	action into_endline{
		into_endline = tsk_true;
	}
	action endtid_match{
		( into_endline || (((pe-p) >7/*seven hyphens*/) && (msrp_msg->tid) && tsk_strniequals(msrp_msg->tid, (p+7), tsk_strlen(msrp_msg->tid))) )
	}


	###########################################
	#	Headers
	###########################################

	Authentication_Info = "Authentication-Info:"i SP any* :>CRLF %parse_Authentication_Info;
	Authorization		= "Authorization:"i SP any* :>CRLF %parse_Authorization;
	Byte_Range			= "Byte-Range:"i SP any* :>CRLF %parse_Byte_Range;
	Content_Type		= "Content-Type:"i SP any* :>CRLF %parse_Content_Type;
	Expires				= "Expires:"i SP any* :>CRLF %parse_Expires;
	Failure_Report		= "Failure-Report:"i SP any* :>CRLF %parse_Failure_Report ;
	From_Path			= "From-Path:"i SP any* :>CRLF %parse_From_Path ;
	Max_Expires			= "Max-Expires:"i SP any* :>CRLF %parse_Max_Expires;
	Message_ID			= "Message-ID:"i SP any* :>CRLF %parse_Message_ID;
	Min_Expires			= "Min-Expires:"i SP any* :>CRLF %parse_Min_Expires;
	Status				= "Status:"i SP any* :>CRLF %parse_Status;
	Success_Report		= "Success-Report:"i SP any* :>CRLF %parse_Success_Report;
	To_Path				= "To-Path:"i SP any* :>CRLF %parse_To_Path;
	Use_Path			= "Use-Path:"i SP any* :>CRLF %parse_Use_Path;
	WWW_Authenticate	= "WWW-Authenticate:"i SP any* :>CRLF %parse_WWW_Authenticate;
	
	Dummy				= hname ":" SP hval :>CRLF %parse_Dummy;
	
	header = (Authentication_Info | Authorization | Byte_Range | Content_Type | Expires | Failure_Report | From_Path | Max_Expires | Message_ID | Min_Expires | Status | Success_Report | To_Path | Use_Path | WWW_Authenticate)>tag @10 | (Dummy>tag) @0;

	#headers = To_Path From_Path ( header )*;
	headers = ( header )*;

	###########################################
	#	Utils
	###########################################
	transact_id	= ident;
	method = UPALPHA*;
	status_code	= DIGIT{3};
	comment	= utf8text;
	
	continuation_flag = "+" | "$" | "#";
	end_line = "-------" transact_id>tag %parse_endtid continuation_flag>tag %parse_cflag CRLF;

	Other_Mime_header = Dummy;

	data = any*;

	###########################################
	#	Request
	###########################################
	req_start = "MSRP" SP transact_id>tag %parse_tid SP method>tag %parse_method CRLF;
	#content_stuff = (Other_Mime_header)* CRLF data>tag %parse_data :>CRLF;
	content_stuff = data>tag >try_parse_data %parse_data;
	msrp_request = req_start headers>10 (CRLF content_stuff CRLF)?>5 :>end_line when endtid_match >into_endline;

	###########################################
	#	Response
	###########################################
	resp_start = "MSRP" SP transact_id>tag %parse_tid SP status_code>tag %parse_status_code (SP comment>tag %parse_comment)? CRLF;
	msrp_response = resp_start headers end_line;

	###########################################
	#	Message
	###########################################
	msrp_req_or_resp = (msrp_request | msrp_response)>1 @outside_endline any*>0;

	###########################################
	#	Entry Point
	###########################################
	main := msrp_req_or_resp;
}%%

TSK_RAGEL_DISABLE_WARNINGS_BEGIN()
/* Ragel data */
%% write data;
TSK_RAGEL_DISABLE_WARNINGS_END()

tmsrp_message_t* tmsrp_message_parse(const void *input, tsk_size_t size)
{
	tsk_size_t msg_size;
	return tmsrp_message_parse_2(input, size, &msg_size);
}

tmsrp_message_t* tmsrp_message_parse_2(const void *input, tsk_size_t size, tsk_size_t* msg_size)
{
	tmsrp_message_t* msrp_msg = tsk_null;
	const char* tag_start = tsk_null;
	tmsrp_header_t* header = tsk_null;
	tsk_bool_t into_endline = tsk_false;
	tsk_bool_t payload_parsed = tsk_false;
	
	/* Ragel variables */
	int cs = 0;
	const char* p = input;
	const char* pe = p + size;
	const char* eof = tsk_null;

	(void)(eof);
	(void)(tmsrp_machine_message_first_final);
	(void)(tmsrp_machine_message_error);
	(void)(tmsrp_machine_message_en_main);

	*msg_size = 0;

	if(!input || !size){
		//TSK_DEBUG_ERROR("Null or empty buffer."); // --> very common case(stream): do not bother us...
		goto bail;
	}

	if(!(msrp_msg = tmsrp_message_create_null())){
		goto bail;
	}

	TSK_RAGEL_DISABLE_WARNINGS_BEGIN()
	/* Ragel init */
	%% write init;

	/* Ragel execute */
	%% write exec;
	TSK_RAGEL_DISABLE_WARNINGS_END()

	/* Check result */
	if( cs < %%{ write first_final; }%% ){
		//TSK_DEBUG_ERROR("Failed to parse MSRP message."); --> very common case(stream): do not bother us...
		TSK_OBJECT_SAFE_FREE(msrp_msg);
		goto bail;
	}
	
bail:
	return msrp_msg;
}

static tsk_bool_t parse_payload(tmsrp_message_t* msrp_msg, const char* tag_start, const char** p, const char* pe, tsk_bool_t* payload_parsed)
{
	int64_t payload_len, endline_len;
	tsk_bool_t can_parse_payload;

	if(*payload_parsed){
		TSK_DEBUG_INFO("payload already parsed");
		return tsk_true;
	}

	if(pe && p && *p && msrp_msg && (can_parse_payload = TMSRP_HEADER_BYTE_RANGE_IS_VALID(msrp_msg->ByteRange))){
		payload_len = (msrp_msg->ByteRange->end - msrp_msg->ByteRange->start) + 1;
		endline_len = 2/*CRLF*/ + 7/*hyphens*/ + tsk_strlen(msrp_msg->tid) + 2/*CRLF*/;
		can_parse_payload = (pe - tag_start) > (payload_len + endline_len);
		if(can_parse_payload){
			set_payload(msrp_msg, tag_start, (tsk_size_t)payload_len);
			*p = ((tag_start + payload_len) - 1);
			*payload_parsed = tsk_true;
			return tsk_true;
		}
	}
	return tsk_false;
}

static void set_payload(tmsrp_message_t* msrp_msg, const void* ptr, tsk_size_t len)
{
	if(msrp_msg->Content){
		tsk_buffer_cleanup(msrp_msg->Content);
		tsk_buffer_append(msrp_msg->Content, ptr, len);
	}
	else{
		msrp_msg->Content = tsk_buffer_create(ptr, len);
	}
}