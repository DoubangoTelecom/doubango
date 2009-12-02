/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou@yahoo.fr>
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
* GNU Lesser General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

#include "stdafx.h"
#include "tcomp_manager.h" /* TinySIGCOMP API functions. */

#define TORTURES					0

#if TORTURES

#define STREAM_ID					1983
#define COMPARTMENT_ID				"urn:uuid:2e5fdc76-00be-4314-8202-1116fa82a473"
#define OUTPUT_BUFFER_SIZE			2000
#define DECOMPRESSION_MEMORY_SIZE	65530

#define RUN_TEST_LOOP	1

#define RUN_TEST_ALL	1
#define RUN_TEST_NO		68

#include "rfc4465_torture_tests.h"

struct_torture_test tests[] = 
{
	{	//*** 0***
		"A.1.1.  Bit Manipulation", 
		RFC4465_A_1_1__Bit_Manipulation, 
		170, 
		"\x01\x50\x00\x00\xfe\xbf\x00\x00", 
		22
	}
	,
	{	//*** 1***
		"A.1.2.  Arithmetic",
		RFC4465_A_1_2__Arithmetic,
		174,
		"\x00\x00\x00\x00\x00\x00\x00\x04",
		25
	}
	,
	{
		//*** 2***
		"A.1.3.  Sorting",
		RFC4465_A_1_3__Sorting,
		223,
		"\x46\x6f\x72\x64\x2c\x20\x79\x6f\x75\x27\x72\x65\x20\x74\x75\x72\x6e\x69\x6e\x67"
        "\x20\x69\x6e\x74\x6f\x20\x61\x20\x70\x65\x6e\x67\x75\x69\x6e\x2e\x20\x53\x74\x6f"
		"\x70\x20\x69\x74\x2e",
		371
	}
	,
	{
		//*** 3**
		"A.1.4.  SHA-1",
		RFC4465_A_1_4__SHA1,
		138,
		"\xa9\x99\x3e\x36\x47\x06\x81\x6a\xba\x3e\x25\x71\x78\x50\xc2\x6c\x9c\xd0\xd8\x9d"
		"\x84\x98\x3e\x44\x1c\x3b\xd2\x6e\xba\xae\x4a\xa1\xf9\x51\x29\xe5\xe5\x46\x70\xf1"
		"\x12\xff\x34\x7b\x4f\x27\xd6\x9e\x1f\x32\x8e\x6f\x4b\x55\x73\xe3\x66\x6e\x12\x2f"
		"\x4f\x46\x04\x52\xeb\xb5\x63\x93\x4f\x46\x04\x52\xeb\xb5\x63\x93\x4f\x46\x04\x52",
		17176
	}
	,
	{
		//*** 4**
		"A.1.5.  LOAD and MULTILOAD",
		RFC4465_A_1_5__LOAD_and_MULTILOAD,
		58,
		"\x00\x84\x00\x84\x00\x86\x00\x86\x00\x2a\x00\x80\x00\x2a\x00\x2a",
		36
	}
	,
	{
		//** 5**
		"A.1.6.  COPY",
		RFC4465_A_1_6__COPY,
		60,
		"\x40\x40\x40\x40\x40\x40\x40\x40\x40\x40\x40\x40\x40\x40\x40\x40\x40\x40\x40\x40"
		"\x40\x40\x40\x40\x40\x40\x40\x40\x40\x40\x40\x40\x41\x41\x41\x41\x41\x41\x41\x41"
		"\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
		"\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
		"\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
		"\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x55\x41"
		"\x42\x43\x44\x43\x44",
		365
	}
	,
	{
		//** 6**
		"A.1.7.  COPY-LITERAL and COPY-OFFSET",
		RFC4465_A_1_7__COPY_LITERAL_and_COPY_OFFSET,
		100,
		"\x41\x41\x41\x41\x00\x61\x41\x41\x41\x41\x49\x4a\x41\x42\x43\x44\x49\x4a\x41\x42"
		"\x00\x4a\x00\x4e\x47\x48\x48\x45\x46\x47\x47\x48\x45\x46",
		216
	}
	,
	{
		//** 7**
		"A.1.8.  MEMSET",
		RFC4465_A_1_8__MEMSET,
		27,
		"\x80\x40\x4f\x5e\x6d\x7c\x8b\x9a\xa9\xb8\xc7\xd6\xe5\xf4\x03\x12",
		166
	}
	,
	{
		//** 8**
		"A.1.9.  CRC",
		RFC4465_A_1_9__CRC,
		29,
		"",
		95
	}
	,
	{
		//** 9**
		"A.1.10.  INPUT-BITS",
		RFC4465_A_1_10__INPUT_BITS,
		28,
		"\x00\x00\x00\x02\x00\x02\x00\x13\x00\x00\x00\x03\x00\x1a\x00\x38",
		66
	}
	,
	{
		//** 10**
		"A.1.11.  INPUT-HUFFMAN",
		RFC4465_A_1_11__INPUT_HUFFMAN,
		39,
		"\x00\x00\x00\x03\x00\x08\x04\xd7\x00\x02\x00\x03\x03\x99\x30\xfe",
		84
	}
	,
	{
		//** 11**
		"A.1.12.  INPUT-BYTES",
		RFC4465_A_1_12__INPUT_BYTES,
		72,
		"\x00\x00\x93\x2e\x00\x01\xb1\x66\xd8\x6f\xb1\x00\x1a\x2b\x00\x03\x9a\x97\x34\xd8"
		"\x00\x07\x00\x01\x33\x87\x4e\x00\x08\xdc\x96\x51\xb5\xdc\x96\x00\x59\x9d\x6a",
		130
	}
	,
	{
		//** 12**
		"A.1.13.  Stack Manipulation",
			RFC4465_A_1_13__Stack_Manipulation,
			324,
			"",
			40
	}
	,
	{
		//** 13**
		"A.1.14.  Program Flow",
		RFC4465_A_1_14__Program_Flow,
		66,
		"\x00\x01\x01\x02\x02\x03\x03\x04\x04\x05\x05\x06\x07\x07\x07\x08\x08\x08\x09\x09",
		131
	}
	,
	{
		//** 14**
		"A.1.15.  State Creation_1",
		RFC4465_A_1_15__State_Creation_1,
		152,
		"",
		23
	}
	,
	{
		//** 15**
		"A.1.15.  State Creation_2",
		RFC4465_A_1_15__State_Creation_2,
		152,
		"",
		14
	}
	,
	{
		//** 16**
		"A.1.15.  State Creation_3",
		RFC4465_A_1_15__State_Creation_3,
		152,
		"",
		24
	}
	,
	{
		//** 17**
		"A.1.15.  State Creation_4", // FAIL
		RFC4465_A_1_15__State_Creation_4,
		153,
		"",
		0
	}
	,
	{
		//** 18**
		"A.1.15.  State Creation_5", // FAIL
		RFC4465_A_1_15__State_Creation_5,
		153,
		"",
		0
	}
	,
	{
		//** 19**
		"A.1.15.  State Creation_6",
		RFC4465_A_1_15__State_Creation_6,
		153,
		"",
		23
	}
	,
	{
		//** 20**
		"A.1.15.  State Creation_7",
		RFC4465_A_1_15__State_Creation_7,
		152,
		"",
		34
	}
	,
	{
		//** 21**
		"A.1.15.  State Creation_8",
		RFC4465_A_1_15__State_Creation_8,
		153,
		"",
		46
	}
	,
	{
		//** 22**
		"A.1.15.  State Creation_9",
		RFC4465_A_1_15__State_Creation_9,
		153,
		"",
		47
	}
	,
	{
		//** 23**
		"A.1.15.  State Creation_10",
		RFC4465_A_1_15__State_Creation_10,
		153,
		"",
		60
	}
	,
	{
		//** 24**
		"A.1.16.  STATE-ACCESS {Set up bytecode}",
		RFC4465_A_1_16__STATE_ACCESS__SETUP,
		403,
		"",
		17
	}
	,
	{
		//** 25**
		"A.1.16.  STATE-ACCESS_1",
		RFC4465_A_1_16__STATE_ACCESS_1,
		408,
		"\x74\x65\x73\x74",
		26
	}
	,
	{
		//** 26**
		"A.1.16.  STATE-ACCESS_2",
		RFC4465_A_1_16__STATE_ACCESS_2,
		408,
		"\x74\x65\x73\x74",
		15
	}
	,
	{
		//** 27**
		"A.1.16.  STATE-ACCESS_3",
		RFC4465_A_1_16__STATE_ACCESS_3,
		408,
		"",
		0
	}
	,
	{
		//** 28**
		"A.1.16.  STATE-ACCESS_4",
		RFC4465_A_1_16__STATE_ACCESS_4,
		408,
		"",
		0
	}
	,
	{
		//** 29**
		"A.1.16.  STATE-ACCESS_5",
		RFC4465_A_1_16__STATE_ACCESS_5,
		408,
		"",
		0
	}
	,
	{
		//** 30**
		"A.2.1.  Useful Values_1",
		RFC4465_A_2_1__Useful_Values_1,
		99,
		"",
		968
	}
	,
	{
		//** 31**
		"A.2.1.  Useful Values_2",
		RFC4465_A_2_1__Useful_Values_2,
		9,
		"",
		(16 * 1080)
	}
	,
	{
		//** 32**
		"A.2.1.  Useful Values_3",
		RFC4465_A_2_1__Useful_Values_3,
		9,
		"",
		0
	}
	,
	{
		//** 33**
		"A.2.1.  Useful Values_4",
		RFC4465_A_2_1__Useful_Values_4,
		9,
		"",
		0
	}
	,
	{
		//** 34**
		"A.2.2.  Cycles Checking",
		RFC4465_A_2_2__Cycles_Checking,
		29,
		"",
		0
	}
	,
	{
		//** 35**
		"A.2.3.  Message-based Transport_1",
		RFC4465_A_2_3_Message_based_Transport_1,
		1,
		"MESSAGE_TOO_SHORT",
		0
	}
	,
	{
		//** 36**
		"A.2.3.  Message-based Transport_2",
		RFC4465_A_2_3_Message_based_Transport_2,
		2,
		"MESSAGE_TOO_SHORT",
		0
	}
	,
	{
		//** 37**
		"A.2.3.  Message-based Transport_3",
		RFC4465_A_2_3_Message_based_Transport_3,
		17,
		"decompression_memory_size",
		5
	}
	,
	{
		//** 38**
		"A.2.3.  Message-based Transport_4",
		RFC4465_A_2_3_Message_based_Transport_4,
		17,
		"MESSAGE_TOO_SHORT",
		0
	}
	,
	{
		//** 39**
		"A.2.3.  Message-based Transport_5",
		RFC4465_A_2_3_Message_based_Transport_5,
		17,
		"INVALID_CODE_LOCATION",
		0
	}
	,
	{
		//** 40**
		"A.2.3.  Message-based Transport_6",
		RFC4465_A_2_3_Message_based_Transport_6,
		17,
		"decompression_memory_size",
		5
	}
	,
	{
		//** 41**
		"A.2.4.  Stream-based Transport_1",
		RFC4465_A_2_4_Stream_based_Transport_1,
		67,
		"decompression_memory_size",
		11,
		1
			
	}
	,
	{
		//** 42**
		"A.2.4.  Stream-based Transport_2",
		RFC4465_A_2_4_Stream_based_Transport_2,
		67,
		"decompression_memory_size",
		11,
		1,
		0
			
	}
	,
	{
		//** 43**
		"A.2.4.  Stream-based Transport_3",
		RFC4465_A_2_4_Stream_based_Transport_3,
		3,
		"MESSAGE_TOO_SHORT",
		0,
		1,
		1
	}
	,
	{
		//** 44**
		"A.2.4.  Stream-based Transport_4",
		RFC4465_A_2_4_Stream_based_Transport_4,
		4,
		"MESSAGE_TOO_SHORT",
		0,
		1,
		1
	}
	,
	{
		//** 45**
		"A.2.4.  Stream-based Transport_5",
		RFC4465_A_2_4_Stream_based_Transport_5,
		30,
		"MESSAGE_TOO_SHORT",
		0,
		1,
		0
	}
	,
	{
		//** 46**
		"A.2.4.  Stream-based Transport_6",
		RFC4465_A_2_4_Stream_based_Transport_6,
		29,
		"INVALID_CODE_LOCATION",
		0,
		1,
		0
	}
	,
	{
		//** 47**
		"A.2.5.  Input Past the End of a Message_1",
		RFC4465_A_2_5__Input_Past_the_End_of_a_Message_1,
		59,
		"\x68\x69\x21",
		23,
		0,
		0
	}
	,
	{
		//** 48**
		"A.2.5.  Input Past the End of a Message_2",
		RFC4465_A_2_5__Input_Past_the_End_of_a_Message_2,
		58,
		"USER_REQUESTED",
		0,
		0,
		1
	}
	,
	{
		//** 49**
		"A.3.1.  SigComp Feedback Mechanism_1",
		RFC4465_A_3_1__SigComp_Feedback_Mechanism_1,
		84,
		"",
		52,
		0,
		0
	}
	,
	{
		//** 50**
		"A.3.1.  SigComp Feedback Mechanism_2",
		RFC4465_A_3_1__SigComp_Feedback_Mechanism_2,
		84,
		"",
		179
	}
	,
	{
		//** 51**
		"A.3.2.  State Memory Management_1",
		RFC4465_A_3_2__State_Memory_Management_1,
		446,
		"",
		811
	}
	,
	{
		//** 52**
		"A.3.2.  State Memory Management_2",
		RFC4465_A_3_2__State_Memory_Management_2,
		446,
		"",
		2603
	}
	,
	{
		//** 53**
		"A.3.2.  State Memory Management_3",
		RFC4465_A_3_2__State_Memory_Management_3,
		446,
		"",
		811
	}
	,
	{
		//** 54**
		"A.3.2.  State Memory Management_4",
		RFC4465_A_3_2__State_Memory_Management_4,
		446,
		"",
		1805
	}
	,
	{
		//** 55**
		"A.3.2.  State Memory Management_5",
		RFC4465_A_3_2__State_Memory_Management_5,
		446,
		"STATE_NOT_FOUND",
		0
	}
	,
	{
		//** 56**
		"A.3.2.  State Memory Management_6",
		RFC4465_A_3_2__State_Memory_Management_6,
		446,
		"",
		2057
	}
	,
	{
		//** 57**
		"A.3.2.  State Memory Management_7",
		RFC4465_A_3_2__State_Memory_Management_7,
		446,
		"",
		1993
	}
	,
	{
		//** 58**
		"A.3.3.  Multiple Compartments_1",
		RFC4465_A_3_3__Multiple_Compartments_1,
		437,
		"",
		1809
	}
	,
	{
		//** 59**
		"A.3.3.  Multiple Compartments_2",
		RFC4465_A_3_3__Multiple_Compartments_2,
		437,
		"",
		1809
	}
	,
	{
		//** 60**
		"A.3.3.  Multiple Compartments_3",
		RFC4465_A_3_3__Multiple_Compartments_3,
		437,
		"",
		1809
	}
	,
	{
		//** 61**
		"A.3.3.  Multiple Compartments_4",
		RFC4465_A_3_3__Multiple_Compartments_4,
		437,
		"",
		1993
	}
	,
	{
		//** 62**
		"A.3.3.  Multiple Compartments_5",
		RFC4465_A_3_3__Multiple_Compartments_5,
		437,
		"",
		1994
	}
	,
	{
		//** 63**
		"A.3.3.  Multiple Compartments_6",
		RFC4465_A_3_3__Multiple_Compartments_6,
		437,
		"",
		1804
	}
	,
	{
		//** 64**
		"A.3.3.  Multiple Compartments_7",
		RFC4465_A_3_3__Multiple_Compartments_7,
		437,
		"STATE_NOT_FOUND",
		0
	}
	,
	{
		//** 65**
		"A.3.3.  Multiple Compartments_8",
		RFC4465_A_3_3__Multiple_Compartments_8,
		437,
		"STATE_NOT_FOUND",
		0
	}
	,
	{
		//** 66**
		"A.3.3.  Multiple Compartments_9",
		RFC4465_A_3_3__Multiple_Compartments_9,
		437,
		"STATE_NOT_FOUND",
		0
	}
	,
	{
		//** 67**
		"A.3.4.  Accessing RFC 3485 State",
		RFC4465_A_3_4__Accessing_RFC_3485_State,
		61,
		"\x53\x49\x50",
		11
	}
	,
	{
		//** 68**
		"A.3.5.  Bytecode State Creation_1",
		RFC4465_A_3_5__Bytecode_State_Creation_1,
		74,
		"\x4f\x4b",
		66
	}
	,
	{
		//** 69**
		"A.3.5.  Bytecode State Creation_2",
		RFC4465_A_3_5__Bytecode_State_Creation_2,
		8,
		"\x4f\x4b\x31",
		7
	}
	,
	{
		//** 70**
		"A.3.5.  Bytecode State Creation_3",
		RFC4465_A_3_5__Bytecode_State_Creation_3,
		13,
		"\x4f\x4b\x32",
		5
	}
	,
	{
		//** 71**
		"A.3.5.  Bytecode State Creation_4",
		RFC4465_A_3_5__Bytecode_State_Creation_4,
		7,
		"\x00\x00\x32",
		5
	}
	,
	{
		//** 72**
		"A.3.5.  Bytecode State Creation_5",
		RFC4465_A_3_5__Bytecode_State_Creation_5,
		7,
		"STATE_NOT_FOUND",
		0,
		0,
		1
	}
};

int startsWith(const char* buffer1, size_t size1, const char* buffer2, size_t size2) 
{
	size_t i;

	if(size1 < size2) return 0;

	for(i = 0; i< size2; i++)
	{
		if(buffer1[i] != buffer2[i])
		{
			return 0;
		}
	}
	return 1;
}

#ifdef _WIN32_WCE
int _tmain(int argc, _TCHAR* argv[])
#else
int main()
#endif
{
	size_t i, start, end;
	size_t res_size = 0;
	char buffer[OUTPUT_BUFFER_SIZE];
	tcomp_manager_handle_t *manager = TCOMP_MANAGER_CREATE();
	tcomp_result_t *result = TCOMP_RESULT_CREATE();

	/* Add SIP dictionary. */
	tcomp_manager_addSipSdpDictionary(manager);

	/* Add Presence dictionary. */
	tcomp_manager_addPresenceDictionary(manager);

	/* Set decompression size. */
	tcomp_manager_setDecompression_Memory_Size(manager, DECOMPRESSION_MEMORY_SIZE);
	
#if RUN_TEST_LOOP
	for(;;)
#endif
	{
#if RUN_TEST_ALL
		start = 0, end = 72;
#else
		start = RUN_TEST_NO, end = RUN_TEST_NO + 1;
#endif

		for(i=start; i<end; i++)
		{
			printf("=== Testing %s ===\n\n", tests[i].section_name);

			if(tests[i].stream)
			{
				tcomp_result_setOutputTCPBuffer(result, buffer, OUTPUT_BUFFER_SIZE, STREAM_ID);
			}
			else
			{
				tcomp_result_setOutputUDPBuffer(result, buffer, OUTPUT_BUFFER_SIZE);
			}

			/* Performs decompression */
			res_size = tcomp_manager_decompress(manager, tests[i].bytecode, tests[i].bytecode_size, result);
			if(result->isNack)
			{
				printf("\n==WE GOT A NACK\n\n");
				//sendto(tcomp_buffer_getBuffer(result->nack_info), tcomp_buffer_getSize(result->nack_info));
			}
			else
			{
				tcomp_result_setCompartmentId(result, COMPARTMENT_ID, strlen(COMPARTMENT_ID));
				tcomp_manager_provideCompartmentId(manager, result);
			}
			/* == Stream 
			*/
			if(tests[i].stream && (res_size || result->isNack))
			{
				for(;;)
				{
					res_size = tcomp_manager_getNextStreamMessage(manager, result);
					if(!res_size && !result->isNack)
					{
						break;
					}
					else if(res_size)
					{
						tcomp_result_setCompartmentId(result, COMPARTMENT_ID, strlen(COMPARTMENT_ID));
						tcomp_manager_provideCompartmentId(manager, result);
					}
					else
					{
						printf("\n==WE GOT A NACK\n\n");
						//sendto(tcomp_buffer_getBuffer(result->nack_info), tcomp_buffer_getSize(result->nack_info));
					}
				}
			}

			printf("xoutpout: %s\n", startsWith(buffer, OUTPUT_BUFFER_SIZE, tests[i].xoutput, strlen(tests[i].xoutput)) ? "YES" : "NO");
			printf("xcycles: %s\n", (result->consumed_cycles == tests[i].xcycles) ? "YES" : "NO");
			printf("output size: %u\n", res_size);
		}

	}/* LOOP */

	/* Free previously allocated resources. */
	TCOMP_RESULT_SAFE_FREE(result);
	TCOMP_MANAGER_SAFE_FREE(manager);

	return 0;
}


#else /* !TORTURES */

#define COMPARTMENT_ID1		"urn:uuid:2e5fdc76-00be-4314-8202-1116fa82a474"	// My first compartment id
#define COMPARTMENT_ID2		"urn:uuid:2e5fdc76-00be-4314-8202-1116fa82a475"	// My second compartment id

#define IS_STREAM			0	// Using reliable transport
#define STREAM_ID			678		// stream identifier

#define MAX_BUFFER_SIZE		0xfff0

#define LOOP_COUNT			100

#define DECOMP_NACK_4_TEST	0

#define PRINTLN printf("\r\n");

// messages to use for tests
const char* messages[] =
{
	//{ 
		"REGISTER sip:ims-network.com SIP/2.0\r\n"
		"Via: SIP/2.0/UDP [::]:1988;comp=sigcomp;rport;branch=z9hG4bK1245420841406\r\n"
		"From: <sip:mamadou@ims-network.com>;tag=29358\r\n"
		"To: <sip:mamadou@ims-network.com>\r\n"
		"Call-ID: M-fa53180346f7f55ceb8d8670f9223dbb\r\n"
		"CSeq: 201 REGISTER\r\n"
		"Max-Forwards: 70\r\n"
		"Allow: INVITE, ACK, CANCEL, BYE, MESSAGE, OPTIONS, NOTIFY, PRACK, UPDATE, REFER\r\n"
		"Contact: <sip:mamadou@[::]:1988;comp=sigcomp;transport=udp>;expires=600000;+deviceID=\"3ca50bcb-7a67-44f1-afd0-994a55f930f4\";mobility=\"fixed\";+g.3gpp.cs-voice;+g.3gpp.app%5fref=\"urn%3Aurnxxx%3A3gpp-application.ims.iari.gsmais\";+g.oma.sip-im.large-message;+g.oma.sip-im\r\n"
		"User-Agent: IM-client/OMA1.0 doubango/v4.0.1390.0\r\n"
		"Require: pref\r\n"
		"P-Preferred-Identity: <sip:mamadou@ims-network.com>\r\n"
		"Supported: path\r\n"
		"P-Access-Network-Info: 3GPP-UTRAN-TDD;utran-cell-id-3gpp=00000000\r\n"
		"Privacy: none\r\n"
		"Supported: timer\r\n"
		"Content-Length: 0\r\n"
		"\r\n"
	//}
	,
	//{ 
		"SIP/2.0 401 Unauthorized - Challenging the UE\r\n"
		"Via: SIP/2.0/UDP [::]:1988;comp=sigcomp;received=2001:5C0:1502:1800:1D41:BF77:F1BF:BB49;rport=1988;branch=z9hG4bK1245420841406\r\n"
		"From: <sip:mamadou@ims-network.com>;tag=29358\r\n"
		"To: <sip:mamadou@ims-network.com>;tag=3241f316c9eb68efd2c34668c4fbf834-eaa0\r\n"
		"Call-ID: M-fa53180346f7f55ceb8d8670f9223dbb\r\n"
		"CSeq: 201 REGISTER\r\n"
		"Path: <sip:term@pcscf.ims-network.com:4060;lr>\r\n"
		"Service-Route: <sip:orig@scscf.ims-network.com:6060;lr>"
		"Allow: INVITE, ACK, CANCEL, OPTIONS, BYE, REFER, SUBSCRIBE, NOTIFY, PUBLISH, MESSAGE, INFO\r\n"
		"Server: Sip EXpress router (2.1.0-dev1 OpenIMSCore (i386/linux))\r\n"
		"Content-Length: 0\r\n"
		"Warning: 392 2001:5C0:1502:1800:0:0:0:226:6060 \"Noisy feedback tells:  pid=24454 req_src_ip=2001:5C0:1502:1800:0:0:0:226 req_src_port=5060 in_uri=sip:scscf.ims-network.com:6060 out_uri=sip:scscf.ims-network.com:6060 via_cnt==3\"\r\n"
		"WWW-Authenticate: Digest realm=\"ims-network.com\", nonce=\"xFBhTyFaQ0/lBgboH2ZqDe3BDmFXDwAA2Peq/bxtLQs=\", algorithm=AKAv1-MD5, qop=\"auth,auth-int\"\r\n"
		"\r\n"

	//}
		,
	//{ 
		"REGISTER sip:ims-network.com SIP/2.0\r\n"
		"Via: SIP/2.0/UDP [::]:1988;comp=sigcomp;rport;branch=z9hG4bK1245420841407\r\n"
		"From: <sip:mamadou@ims-network.com>;tag=29358\r\n"
		"To: <sip:mamadou@ims-network.com>\r\n"
		"Call-ID: M-fa53180346f7f55ceb8d8670f9223dbb\r\n"
		"CSeq: 202 REGISTER\r\n"
		"Max-Forwards: 70\r\n"
		"Allow: INVITE, ACK, CANCEL, BYE, MESSAGE, OPTIONS, NOTIFY, PRACK, UPDATE, REFER\r\n"
		"Contact: <sip:mamadou@[::]:1988;comp=sigcomp;transport=udp>;expires=600000;+deviceID=\"3ca50bcb-7a67-44f1-afd0-994a55f930f4\";mobility=\"fixed\";+g.3gpp.cs-voice;+g.3gpp.app%5fref=\"urn%3Aurnxxx%3A3gpp-application.ims.iari.gsmais\";+g.oma.sip-im.large-message;+g.oma.sip-im\r\n"
		"User-Agent: IM-client/OMA1.0 doubango/v4.0.1390.0\r\n"
		"Authorization: Digest algorithm=AKAv1-MD5,username=\"mamadou@ims-network.com\",realm=\"ims-network.com\",nonce=\"xFBhTyFaQ0/lBgboH2ZqDe3BDmFXDwAA2Peq/bxtLQs=\",uri=\"sip:ims-network.com\",response=\"c499a6b49693d5b29c431786cff32ca4\",qop=auth-int,cnonce=\"9fcc19edace2d1beaa6122b86dd11256\",nc=00000001\r\n"
		"Require: pref\r\n"
		"P-Preferred-Identity: <sip:mamadou@ims-network.com>\r\n"
		"Supported: path\r\n"
		"P-Access-Network-Info: 3GPP-UTRAN-TDD;utran-cell-id-3gpp=00000000\r\n"
		"Privacy: none\r\n"
		"Supported: timer\r\n"
		"Content-Length: 0\r\n"
		"\r\n"
	//}
		,
	//{ 
		"SIP/2.0 200 OK - SAR succesful and registrar saved\r\n"
		"Via: SIP/2.0/UDP [::]:1988;comp=sigcomp;received=2001:5C0:1502:1800:1D41:BF77:F1BF:BB49;rport=1988;branch=z9hG4bK1245420841407\r\n"
		"From: <sip:mamadou@ims-network.com>;tag=29358\r\n"
		"To: <sip:mamadou@ims-network.com>;tag=3241f316c9eb68efd2c34668c4fbf834-1b36\r\n"
		"Call-ID: M-fa53180346f7f55ceb8d8670f9223dbb\r\n"
		"CSeq: 202 REGISTER\r\n"
		"P-Associated-URI: <sip:mamadou@ims-network.com>\r\n"
		"Contact: <sip:mamadou@[::]:1988;comp=sigcomp;transport=udp>;expires=600000\r\n"
		"Path: <sip:term@pcscf.ims-network.com:4060;lr>\r\n"
		"Service-Route: <sip:orig@scscf.ims-network.com:6060;lr>"
		"Allow: INVITE, ACK, CANCEL, OPTIONS, BYE, REFER, SUBSCRIBE, NOTIFY, PUBLISH, MESSAGE, INFO\r\n"
		"P-Charging-Function-Addresses: ccf=pri_ccf_address\r\n"
		"Server: Sip EXpress router (2.1.0-dev1 OpenIMSCore (i386/linux))\r\n"
		"Content-Length: 0\r\n"
		"Warning: 392 2001:5C0:1502:1800:0:0:0:226:6060 \"Noisy feedback tells:  pid=24452 req_src_ip=2001:5C0:1502:1800:0:0:0:226 req_src_port=5060 in_uri=sip:scscf.ims-network.com:6060 out_uri=sip:scscf.ims-network.com:6060 via_cnt==3\"\r\n"
		"\r\n"
	//}
		,
	//{ 
		"SUBSCRIBE sip:mamadou@ims-network.com SIP/2.0\r\n"
		"Via: SIP/2.0/UDP [::]:1988;comp=sigcomp;rport;branch=z9hG4bK1245420841408\r\n"
		"From: <sip:mamadou@ims-network.com>;tag=5705\r\n"
		"To: <sip:mamadou@ims-network.com>\r\n"
		"Call-ID: M-dd6e227ce416f853ca7bca49ad5b676d\r\n"
		"CSeq: 301 SUBSCRIBE\r\n"
		"Max-Forwards: 70\r\n"
		"Allow: INVITE, ACK, CANCEL, BYE, MESSAGE, OPTIONS, NOTIFY, PRACK, UPDATE, REFER\r\n"
		"Contact: <sip:mamadou@[::]:1988;comp=sigcomp;transport=udp>;+g.oma.sip-im\r\n"
		"User-Agent: IM-client/OMA1.0 doubango/v4.0.1390.0\r\n"
		"Expires: 600000\r\n"
		"Event: reg\r\n"
		"Accept: application/reginfo+xml\r\n"
		"Route: <sip:[2001:5c0:1502:1800::226]:4060;lr=true;transport=udp>,<sip:orig@scscf.ims-network.com:6060;lr>\r\n"
		"P-Access-Network-Info: 3GPP-UTRAN-TDD;utran-cell-id-3gpp=00000000\r\n"
		"Privacy: none\r\n"
		"Supported: timer\r\n"
		"Allow-Events: presence, presence.winfo, conference\r\n"
		"Content-Length: 0\r\n"
		"\r\n"
	//}
		,
	//{ 
		"SIP/2.0 200 Subscription to REG saved\r\n"
		"Record-Route: <sip:mo@pcscf.ims-network.com:4060;lr>\r\n"
		"Via: SIP/2.0/UDP [::]:1988;comp=sigcomp;received=2001:5C0:1502:1800:1D41:BF77:F1BF:BB49;rport=1988;branch=z9hG4bK1245420841408\r\n"
		"From: <sip:mamadou@ims-network.com>;tag=5705\r\n"
		"To: <sip:mamadou@ims-network.com>;tag=3241f316c9eb68efd2c34668c4fbf834-5cce\r\n"
		"Call-ID: M-dd6e227ce416f853ca7bca49ad5b676d\r\n"
		"CSeq: 301 SUBSCRIBE\r\n"
		"Expires: 600000\r\n"
		"Contact: <sip:mamadou@ims-network.com;comp=sigcomp;>\r\n"
		"Server: Sip EXpress router (2.1.0-dev1 OpenIMSCore (i386/linux))\r\n"
		"Content-Length: 0\r\n"
		"Warning: 392 2001:5C0:1502:1800:0:0:0:226:6060 \"Noisy feedback tells:  pid=24454 req_src_ip=2001:5C0:1502:1800:0:0:0:226 req_src_port=4060 in_uri=sip:mamadou@ims-network.com out_uri=sip:mamadou@ims-network.com via_cnt==2\"\r\n"
		"\r\n"
	//}
		,
	//{ 
		"NOTIFY sip:mamadou@[::]:1988;transport=udp SIP/2.0\r\n"
		"Via: SIP/2.0/UDP [2001:5C0:1502:1800:0:0:0:226]:4060;comp=sigcomp;branch=z9hG4bK2b3f.38818b91.0\r\n"
		"Via: SIP/2.0/UDP [2001:5C0:1502:1800:0:0:0:226]:6060;received=2001:5C0:1502:1800:0:0:0:226;rport=6060;branch=z9hG4bK2b3f.6db77cf1.0\r\n"
		"To: <sip:mamadou@ims-network.com>;tag=5705\r\n"
		"From: <sip:mamadou@ims-network.com>;tag=3241f316c9eb68efd2c34668c4fbf834-5cce\r\n"
		"CSeq: 10 NOTIFY\r\n"
		"Call-ID: M-dd6e227ce416f853ca7bca49ad5b676d\r\n"
		"Content-Length: 379\r\n"
		"User-Agent: Sip EXpress router(2.1.0-dev1 OpenIMSCore (i386/linux))\r\n"
		"Contact: <sip:mamadou@ims-network.com; comp=sigcomp;>\r\n"
		"Event: reg\r\n"
		"Max-Forwards: 16\r\n"
		"Subscription-State: active;expires=600030\r\n"
		"Content-Type: application/reginfo+xml\r\n"
		"\r\n"
		"<?xml version=\"1.0\"?>\r\n"
		"<reginfo xmlns=\"urn:ietf:params:xml:ns:reginfo\" version=\"0\" state=\"full\">\r\n"
		"<registration aor=\"sip:mamadou@ims-network.com\" id=\"0xb5d91fcc\" state=\"active\">\r\n"
		"<contact id=\"0xb5d8fb98\" state=\"active\" event=\"registered\" expires=\"600000\">\r\n"
		"<uri>sip:mamadou@[::]:1988;transport=udp</uri>\r\n"
		"</contact>\r\n"
		"</registration>\r\n"
		"</reginfo>\r\n"
	//}
	,
	//{
		"SIP/2.0 200 OK\r\n"
		"Max-Forwards: 70\r\n"
		"User-Agent: IM-client/OMA1.0 doubango/v4.0.1390.0\r\n"
		"Via: SIP/2.0/UDP [2001:5C0:1502:1800:0:0:0:226]:4060;comp=sigcomp;branch=z9hG4bK2b3f.38818b91.0;received=2001:5c0:1502:1800::226\r\n"
		"Via: SIP/2.0/UDP [2001:5C0:1502:1800:0:0:0:226]:6060;received=2001:5C0:1502:1800:0:0:0:226;rport=6060;branch=z9hG4bK2b3f.6db77cf1.0\r\n"
		"To: <sip:mamadou@ims-network.com>;tag=5705\r\n"
		"From: <sip:mamadou@ims-network.com>;tag=3241f316c9eb68efd2c34668c4fbf834-5cce\r\n"
		"CSeq: 10 NOTIFY\r\n"
		"Call-ID: M-dd6e227ce416f853ca7bca49ad5b676d\r\n"
		"Content-Length: 0\r\n"
		"\r\n"
	//}
};


#ifdef _WIN32_WCE
int _tmain(int argc, _TCHAR* argv[])
#else
int main()
#endif
{
	int i = 0;
	size_t outLen = 0;

	tcomp_manager_handle_t *manager1 = 0, *manager2 = 0;
	tcomp_result_t *result1 = 0, *result2 = 0;

	/* temporary buffers  --> will hold compression/decompression results */
	char buff1[MAX_BUFFER_SIZE]; 
	char buff2[MAX_BUFFER_SIZE];
	
	/* Managers */
	manager1 = TCOMP_MANAGER_CREATE();
	manager2 = TCOMP_MANAGER_CREATE();

	/* Add SIP/Presence dictionnaries */
	tcomp_manager_addSipSdpDictionary(manager1);
	tcomp_manager_addPresenceDictionary(manager1);
	tcomp_manager_addSipSdpDictionary(manager2);
	tcomp_manager_addPresenceDictionary(manager2);
	
	/* Results --> it is recomanded to use one result struct for each manager */
	result1 = TCOMP_RESULT_CREATE();
	result2 = TCOMP_RESULT_CREATE();

	/* Sets compartment Ids */
	tcomp_result_setCompartmentId(result1, COMPARTMENT_ID1, strlen(COMPARTMENT_ID1));
	tcomp_result_setCompartmentId(result2, COMPARTMENT_ID2, strlen(COMPARTMENT_ID2));

	/* Set DMS and SMS */
	tcomp_manager_setDecompression_Memory_Size(manager1, 8192);
	tcomp_manager_setDecompression_Memory_Size(manager2, 8192);
	tcomp_manager_setCycles_Per_Bit(manager1, 64);
	tcomp_manager_setCycles_Per_Bit(manager2, 64);
	tcomp_manager_setState_Memory_Size(manager1, 8192);
	tcomp_manager_setState_Memory_Size(manager2, 8192);

	for(i = 0; i< (8*LOOP_COUNT); i++)
	{
		memset(buff1, '\0', MAX_BUFFER_SIZE);
		memset(buff2, '\0', MAX_BUFFER_SIZE);

		// 
		//	Compression using manager I
		//
		outLen = tcomp_manager_compress(manager1, COMPARTMENT_ID1, strlen(COMPARTMENT_ID1), messages[i%8], strlen(messages[i%8]), buff1, MAX_BUFFER_SIZE, IS_STREAM);
		if(outLen)
		{
			//* TODO: sendto(SendSocket, buff1, outLen, 0, (SOCKADDR *) &SenderAddr, sizeof(SenderAddr));

			/*
			* Decompress the compressed message using manager II
			*/
			tcomp_result_setOutputBuffer(result2, buff2, MAX_BUFFER_SIZE, IS_STREAM, STREAM_ID); // set the output buffer where to copy decompressed message
			outLen = tcomp_manager_decompress(manager2, buff1, outLen, result2);
			if(outLen) // OK
			{
				// buff2 contains the result and outLen is result length
				printf("%s\n\n", buff2);
				PRINTLN;

				// provide the compartment id --> save temp states
				tcomp_manager_provideCompartmentId(manager2, result2);
			}
			else // NOK
			{
				printf("ERROR (1)");
				PRINTLN;
				//--assert(tcomp_result_getIsNack(result2));
#if DECOMP_NACK_4_TEST
				manager1->decompress(result2.getNackInfo()->getBuffer(), result2.getNackInfo()->getSize(), &result1);
#endif
				// Decompression failed --> handle NACK (done by remote party)
				// NACK will be retourned only if SigCompVersion >= 2
				// NACK must be sent to the remote party (SIP/IMS use-case) over the network
				//* TODO: sendto(SendSocket, result2.getNackInfo()->getBuffer(), result2.getNackInfo()->getSize(), 0, (SOCKADDR *) &SenderAddr, sizeof(SenderAddr));
			}
		}
		else
		{
			//std::cout<< "ERROR (2)" << std::endl;
			//--assert(0); // MUST never happen	
		}


		// 
		//	Compression using manager II
		//
		outLen = tcomp_manager_compress(manager2, COMPARTMENT_ID2, strlen(COMPARTMENT_ID2), messages[i%8], strlen(messages[i%8]), buff2, MAX_BUFFER_SIZE, IS_STREAM);
		if(outLen)
		{
			tcomp_result_setOutputBuffer(result1, buff1, MAX_BUFFER_SIZE, IS_STREAM, STREAM_ID); // set the output buffer where to copy decompressed message
			outLen = tcomp_manager_decompress(manager1, buff2, outLen, result1);
			if(outLen)
			{
				printf(buff1);
				PRINTLN;
				tcomp_manager_provideCompartmentId(manager1, result1);
			}
			else
			{
				printf("ERROR (3)");
				//--assert(tcomp_result_getIsNack(result2));
#if DECOMP_NACK_4_TEST
				manager2->decompress(result1.getNackInfo()->getBuffer(), result1.getNackInfo()->getSize(), &result2);
#endif
			}
		}
		else
		{
			printf("ERROR (4)");
			//--assert(0);	
		}
	}

	// Close compartments
	tcomp_manager_closeCompartment(manager1, COMPARTMENT_ID1, strlen(COMPARTMENT_ID1));
	tcomp_manager_closeCompartment(manager2, COMPARTMENT_ID2, strlen(COMPARTMENT_ID2));

	// Delete Results
	TCOMP_RESULT_SAFE_FREE(result1);
	TCOMP_RESULT_SAFE_FREE(result2);

	// Delete managers
	TCOMP_MANAGER_SAFE_FREE(manager1);
	TCOMP_MANAGER_SAFE_FREE(manager2);
}


#endif /* TORTURES */
