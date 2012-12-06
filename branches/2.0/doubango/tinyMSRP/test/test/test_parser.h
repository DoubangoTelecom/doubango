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
#ifndef _TEST_MSRPPARSER_H
#define _TEST_MSRPPARSER_H

#include "tinymsrp/headers/tmsrp_header_Dummy.h"

#define MSRP_MSG_REQUEST \
	"MSRP a786hjs2 SEND\r\n" \
	"To-Path: msrp://biloxi.example.com:12763/kjhd37s2s20w2a;tcp\r\n" \
	"From-Path: msrp://atlanta.example.com:7654/jshA7weztas;tcp\r\n" \
	"Message-ID: 87652491\r\n" \
	"Byte-Dummy: 1-25/25\r\n" \
	"Byte-Range: 1-5/3\r\n" \
	"Byte-Range: 1-25/*\r\n" \
	"Byte-Range: 1-*/*\r\n" \
	"Failure-Report: partial\r\n" \
    "Success-Report: yes\r\n" \
	"Content-Type: text/plain;charset=utf8\r\n" \
	"\r\n" \
	"ab\r\nc\r\n" \
	"-------a786hjs2$\r\nMSRP"

#define MSRP_MSG_RESPONSE \
	"MSRP a786hjs2 200 OK\r\n" \
	"To-Path: msrp://atlanta.example.com:7654/jshA7weztas;tcp\r\n" \
	"From-Path: msrp://biloxi.example.com:12763/kjhd37s2s20w2a;tcp\r\n" \
	"-------a786hjs2$\r\nMSRP 000"

#define MSRP_MSG_REPORT \
	"MSRP dkei38sd REPORT\r\n" \
	"To-Path: msrp://alicepc.example.com:7777/iau39soe2843z;tcp\r\n" \
	"From-Path: msrp://bob.example.com:8888/9di4eae923wzd;tcp\r\n" \
	"Message-ID: 12339sdqwer\r\n" \
	"Byte-Range: 1-106/106\r\n" \
	"Status: 000 200 OK\r\n" \
	"-------dkei38sd$\r\nMSRP 000"

#define MSRP_MSG_TO_TEST MSRP_MSG_REQUEST

void test_parser()
{
	tmsrp_message_t *message;
	const tmsrp_header_Dummy_t* header;
	size_t msg_size;
	char* str;
	
	//
	//	Serialization / Deserialization
	//
	/* deserialize the message */
	if((message = tmsrp_message_parse_2(MSRP_MSG_TO_TEST, strlen(MSRP_MSG_TO_TEST), &msg_size))){
		
		tmsrp_message_add_headers(message,
			TMSRP_HEADER_DUMMY_VA_ARGS("NS", "imdn <urn:ietf:params:imdn>"),
			TMSRP_HEADER_DUMMY_VA_ARGS("imdn.Message-ID", "MsgiQqFZqTYAA"),
			TMSRP_HEADER_DUMMY_VA_ARGS("DateTime", "2012-10-25T18:02:08.000Z"),
			TMSRP_HEADER_DUMMY_VA_ARGS("imdn.Disposition-Notification", "positive-delivery, display"),
			
			tsk_null);

		if((header = (const tmsrp_header_Dummy_t*)tmsrp_message_get_headerByName(message, "NS"))){
			TSK_DEBUG_INFO("NS=%s\n", header->value);
		}
		if((header = (const tmsrp_header_Dummy_t*)tmsrp_message_get_headerByName(message, "imdn.Message-ID"))){
			TSK_DEBUG_INFO("imdn.Message-ID=%s\n", header->value);
		}

		/* serialize the message */
		if((str = tmsrp_message_tostring(message))){
			TSK_DEBUG_INFO("\nMSRP Message=\n%s\n\n", str);
			TSK_FREE(str);
		}
	}
	else{
		TSK_DEBUG_ERROR("Failed to parse MSRP message(1).");
	}
	TSK_OBJECT_SAFE_FREE(message);

	//
	//	Create Response from Request
	//
	if((message = tmsrp_message_parse(MSRP_MSG_REQUEST, strlen(MSRP_MSG_REQUEST)))){
		tmsrp_response_t* response = tmsrp_create_response(message, 202, "Accepted");

		if((str = tmsrp_message_tostring(response))){
			TSK_DEBUG_INFO("\nMSRP Response=\n%s\n\n", str);
			TSK_FREE(str);
		}
		
		TSK_OBJECT_SAFE_FREE(response);
	}
	else{
		TSK_DEBUG_ERROR("Failed to parse MSRP message(2).");
	}
	TSK_OBJECT_SAFE_FREE(message);

	//
	//	Create Report from Request
	//
	if((message = tmsrp_message_parse(MSRP_MSG_REQUEST, strlen(MSRP_MSG_REQUEST)))){
		tmsrp_request_t* report = tmsrp_create_report(message, 403, "Stop-sending-message");

		if((str = tmsrp_message_tostring(report))){
			TSK_DEBUG_INFO("\nMSRP Response=\n%s\n\n", str);
			TSK_FREE(str);
		}
		
		TSK_OBJECT_SAFE_FREE(report);
	}
	else{
		TSK_DEBUG_ERROR("Failed to parse MSRP message(2).");
	}
	TSK_OBJECT_SAFE_FREE(message);

	//
	// Create bodiless Request
	//
	{
		tmsrp_request_t* bodiless = tmsrp_create_bodiless(tsk_null, tsk_null);

		if((str = tmsrp_message_tostring(bodiless))){
			TSK_DEBUG_INFO("\nMSRP Bodiless=\n%s\n\n", str);
			TSK_FREE(str);
		}

		TSK_OBJECT_SAFE_FREE(bodiless);
	}
}

#endif /* _TEST_MSRPPARSER_H */
