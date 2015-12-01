/*
* Copyright (C) 2009 Mamadou Diop.
* Copyright (C) 2012 Doubango Telecom <http://doubango.org>.
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
#ifndef TEST_TINYSIGCOMP_TORTURES_H
#define TEST_TINYSIGCOMP_TORTURES_H

#define COMPARTMENT_ID				"urn:uuid:2e5fdc76-00be-4314-8202-1116fa82a473"
#define COMPARTMENT_ID_0			"urn:uuid:0"
#define COMPARTMENT_ID_1			"urn:uuid:1"
#define COMPARTMENT_ID_2			"urn:uuid:2"

#define OUTPUT_BUFFER_SIZE			2000
// Be careful before changing these values as not all tests accept any
#define DMS	16384 //2048//4096//8192//65536
#define SMS 2048 //2048//4096//8192//65536
#define CPB	16 // 16//32//64//128

#define RUN_TEST_LOOP	1

#define RUN_TEST_ALL	1
#define RUN_TEST_NO		68

#define IS_STREAM_YES	1
#define IS_STREAM_NO	0

#define XFAIL_YES		1
#define XFAIL_NO		0

#include "rfc4465_torture_tests.h"

#include <assert.h>

struct_torture_test tests[] = 
{
	{	//*** 0***
		"A.1.1.  Bit Manipulation", 
		RFC4465_A_1_1__Bit_Manipulation, 
		170, 
		{8, "\x01\x50\x00\x00\xfe\xbf\x00\x00"}, 
		22,
		IS_STREAM_NO,
		XFAIL_NO,
		COMPARTMENT_ID
	}
	,
	{	//*** 1***
		"A.1.2(.1)  Arithmetic",
		RFC4465_A_1_2__Arithmetic_1,
		174,
		{8, "\x00\x00\x00\x00\x00\x00\x00\x04"},
		25,
		IS_STREAM_NO,
		XFAIL_NO,
		COMPARTMENT_ID
	}
	,
	{	//*** 2***
		"A.1.2(.2)  Arithmetic",
		RFC4465_A_1_2__Arithmetic_2,
		174,
		{11, "DIV_BY_ZERO"},
		0,
		IS_STREAM_NO,
		XFAIL_YES,
		COMPARTMENT_ID
	}
	,
	{	//*** 3***
		"A.1.2(.3)  Arithmetic",
		RFC4465_A_1_2__Arithmetic_3,
		174,
		{11, "DIV_BY_ZERO"},
		0,
		IS_STREAM_NO,
		XFAIL_YES,
		COMPARTMENT_ID
	}
	,
	{
		//*** 4***
		"A.1.3.  Sorting",
		RFC4465_A_1_3__Sorting,
		223,
		{45, "\x46\x6f\x72\x64\x2c\x20\x79\x6f\x75\x27\x72\x65\x20\x74\x75\x72\x6e\x69\x6e\x67"
        "\x20\x69\x6e\x74\x6f\x20\x61\x20\x70\x65\x6e\x67\x75\x69\x6e\x2e\x20\x53\x74\x6f"
		"\x70\x20\x69\x74\x2e"},
		371,
		IS_STREAM_NO,
		XFAIL_NO,
		COMPARTMENT_ID
	}
	,
	{
		//*** 5**
		"A.1.4.  SHA-1",
		RFC4465_A_1_4__SHA1,
		138,
		{40, "\xa9\x99\x3e\x36\x47\x06\x81\x6a\xba\x3e\x25\x71\x78\x50\xc2\x6c\x9c\xd0\xd8\x9d"
		"\x84\x98\x3e\x44\x1c\x3b\xd2\x6e\xba\xae\x4a\xa1\xf9\x51\x29\xe5\xe5\x46\x70\xf1"
		"\x12\xff\x34\x7b\x4f\x27\xd6\x9e\x1f\x32\x8e\x6f\x4b\x55\x73\xe3\x66\x6e\x12\x2f"
		"\x4f\x46\x04\x52\xeb\xb5\x63\x93\x4f\x46\x04\x52\xeb\xb5\x63\x93\x4f\x46\x04\x52"},
		17176,
		IS_STREAM_NO,
		XFAIL_NO,
		COMPARTMENT_ID
	}
	,
	{
		//*** 6**
		"A.1.5.(1)  LOAD and MULTILOAD",
		RFC4465_A_1_5__LOAD_and_MULTILOAD_1,
		58,
		{16, "\x00\x84\x00\x84\x00\x86\x00\x86\x00\x2a\x00\x80\x00\x2a\x00\x2a"},
		36,
		IS_STREAM_NO,
		XFAIL_NO,
		COMPARTMENT_ID
	}
	,
	{
		//*** 7**
		"A.1.5.(2)  LOAD and MULTILOAD",
		RFC4465_A_1_5__LOAD_and_MULTILOAD_2,
		58,
		{21, "MULTILOAD_OVERWRITTEN"},
		36,
		IS_STREAM_NO,
		XFAIL_YES,
		COMPARTMENT_ID
	}
	,
	{
		//*** 8**
		"A.1.5.(3)  LOAD and MULTILOAD",
		RFC4465_A_1_5__LOAD_and_MULTILOAD_3,
		58,
		{21, "MULTILOAD_OVERWRITTEN"},
		36,
		IS_STREAM_NO,
		XFAIL_YES,
		COMPARTMENT_ID
	}
	,
	{
		//** 9**
		"A.1.6.  COPY",
		RFC4465_A_1_6__COPY,
		60,
		{125, "\x40\x40\x40\x40\x40\x40\x40\x40\x40\x40\x40\x40\x40\x40\x40\x40\x40\x40\x40\x40"
		"\x40\x40\x40\x40\x40\x40\x40\x40\x40\x40\x40\x40\x41\x41\x41\x41\x41\x41\x41\x41"
		"\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
		"\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
		"\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
		"\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x55\x41"
		"\x42\x43\x44\x43\x44"},
		365,
		IS_STREAM_NO,
		XFAIL_NO,
		COMPARTMENT_ID
	}
	,
	{
		//** 10**
		"A.1.7.  COPY-LITERAL and COPY-OFFSET",
		RFC4465_A_1_7__COPY_LITERAL_and_COPY_OFFSET,
		100,
		{34, "\x41\x41\x41\x41\x00\x61\x41\x41\x41\x41\x49\x4a\x41\x42\x43\x44\x49\x4a\x41\x42"
		"\x00\x4a\x00\x4e\x47\x48\x48\x45\x46\x47\x47\x48\x45\x46"},
		216,
		IS_STREAM_NO,
		XFAIL_NO,
		COMPARTMENT_ID
	}
	,
	{
		//** 11**
		"A.1.8.  MEMSET",
		RFC4465_A_1_8__MEMSET,
		27,
		{16, "\x80\x40\x4f\x5e\x6d\x7c\x8b\x9a\xa9\xb8\xc7\xd6\xe5\xf4\x03\x12"},
		166,
		IS_STREAM_NO,
		XFAIL_NO,
		COMPARTMENT_ID
	}
	,
	{
		//** 12**
		"A.1.9.(1)  CRC",
		RFC4465_A_1_9__CRC_1,
		29,
		{0, ""},
		95,
		IS_STREAM_NO,
		XFAIL_NO,
		COMPARTMENT_ID
	}
	,
	{
		//** 13**
		"A.1.9.(2)  CRC",
		RFC4465_A_1_9__CRC_2,
		29,
		{14, "USER_REQUESTED"},
		95,
		IS_STREAM_NO,
		XFAIL_YES,
		COMPARTMENT_ID
	}
	,
	{
		//** 14**
		"A.1.10.  INPUT-BITS",
		RFC4465_A_1_10__INPUT_BITS,
		28,
		{16, "\x00\x00\x00\x02\x00\x02\x00\x13\x00\x00\x00\x03\x00\x1a\x00\x38"},
		66,
		IS_STREAM_NO,
		XFAIL_NO,
		COMPARTMENT_ID
	}
	,
	{
		//** 15**
		"A.1.11.  INPUT-HUFFMAN",
		RFC4465_A_1_11__INPUT_HUFFMAN,
		39,
		{16, "\x00\x00\x00\x03\x00\x08\x04\xd7\x00\x02\x00\x03\x03\x99\x30\xfe"},
		84,
		IS_STREAM_NO,
		XFAIL_NO,
		COMPARTMENT_ID
	}
	,
	{
		//** 16**
		"A.1.12.  INPUT-BYTES",
		RFC4465_A_1_12__INPUT_BYTES,
		72,
		{39, "\x00\x00\x93\x2e\x00\x01\xb1\x66\xd8\x6f\xb1\x00\x1a\x2b\x00\x03\x9a\x97\x34\xd8"
		"\x00\x07\x00\x01\x33\x87\x4e\x00\x08\xdc\x96\x51\xb5\xdc\x96\x00\x59\x9d\x6a"},
		130,
		IS_STREAM_NO,
		XFAIL_NO,
		COMPARTMENT_ID
	}
	,
	{
		//** 17**
		"A.1.13.  Stack Manipulation",
		RFC4465_A_1_13__Stack_Manipulation,
		324,
		{16, "\x00\x03\x00\x02\x00\x01\x00\x42\x00\x42\x00\x00\x00\x01\x00\x01"},
		40,
		IS_STREAM_NO,
		XFAIL_NO,
		COMPARTMENT_ID
	}
	,
	{
		//** 18**
		"A.1.14.  Program Flow",
		RFC4465_A_1_14__Program_Flow,
		66,
		{20, "\x00\x01\x01\x02\x02\x03\x03\x04\x04\x05\x05\x06\x07\x07\x07\x08\x08\x08\x09\x09"},
		131,
		IS_STREAM_NO,
		XFAIL_NO,
		COMPARTMENT_ID
	}
	,
	{
		//** 19**
		"A.1.15.(1)  State Creation",
		RFC4465_A_1_15__State_Creation_1,
		152,
		{0, ""},
		23,
		IS_STREAM_NO,
		XFAIL_NO,
		COMPARTMENT_ID
	}
	,
	{
		//** 20 (requires previsous test)**
		"A.1.15.(2)  State Creation",
		RFC4465_A_1_15__State_Creation_2,
		152,
		{0, ""},
		14,
		IS_STREAM_NO,
		XFAIL_NO,
		COMPARTMENT_ID
	}
	,
	{
		//** 21 (requires previsous test)**
		"A.1.15.(3)  State Creation",
		RFC4465_A_1_15__State_Creation_3,
		152,
		{0, ""},
		24,
		IS_STREAM_NO,
		XFAIL_NO,
		COMPARTMENT_ID
	}
	,
	{
		//** 22 (requires previsous test)**
		"A.1.15.(4)  State Creation", // FAIL
		RFC4465_A_1_15__State_Creation_4,
		153,
		{23, "INVALID_STATE_ID_LENGTH"},
		0,
		IS_STREAM_NO,
		XFAIL_YES,
		COMPARTMENT_ID
	}
	,
	{
		//** 23 (requires previsous test)**
		"A.1.15.(5)  State Creation", // FAIL
		RFC4465_A_1_15__State_Creation_5,
		153,
		{23, "INVALID_STATE_ID_LENGTH"},
		0,
		IS_STREAM_NO,
		XFAIL_YES,
		COMPARTMENT_ID
	}
	,
	{
		//** 24 (requires previsous test)**
		"A.1.15.(6)  State Creation",
		RFC4465_A_1_15__State_Creation_6,
		153,
		{0, ""},
		23,
		IS_STREAM_NO,
		XFAIL_NO,
		COMPARTMENT_ID
	}
	,
	{
		//** 25 (requires previsous test)**
		"A.1.15.(7)  State Creation",
		RFC4465_A_1_15__State_Creation_7,
		152,
		{0, ""},
		34,
		IS_STREAM_NO,
		XFAIL_NO,
		COMPARTMENT_ID
	}
	,
	{
		//** 26 (requires previsous test)**
		"A.1.15.(8)  State Creation",
		RFC4465_A_1_15__State_Creation_8,
		153,
		{0, ""},
		46,
		IS_STREAM_NO,
		XFAIL_NO,
		COMPARTMENT_ID
	}
	,
	{
		//** 27 (requires previsous test)**
		"A.1.15.(9)  State Creation",
		RFC4465_A_1_15__State_Creation_9,
		153,
		{0, ""},
		47,
		IS_STREAM_NO,
		XFAIL_NO,
		COMPARTMENT_ID
	}
#if 0 // FIXME: http://code.google.com/p/doubango/issues/detail?id=181
	,
	{
		//** 23 (requires previsous test)**
		"A.1.15.(10)  State Creation",
		RFC4465_A_1_15__State_Creation_10,
		153,
		{0, ""},
		60,
		IS_STREAM_NO,
		XFAIL_NO,
		COMPARTMENT_ID
	}
#endif
	,
	{
		//** 28**
		"A.1.16.(0)  STATE-ACCESS {Set up bytecode}",
		RFC4465_A_1_16__STATE_ACCESS__SETUP,
		403,
		{0, ""},
		17,
		IS_STREAM_NO,
		XFAIL_NO,
		COMPARTMENT_ID
	}
	,
	{
		//** 29 (requires previous test)**
		"A.1.16.(1)  STATE-ACCESS",
		RFC4465_A_1_16__STATE_ACCESS_1,
		408,
		{4, "\x74\x65\x73\x74"},
		26,
		IS_STREAM_NO,
		XFAIL_NO,
		COMPARTMENT_ID
	}
	,
	{
		//** 30 (requires previous test)**
		"A.1.16.(2)  STATE-ACCESS",
		RFC4465_A_1_16__STATE_ACCESS_2,
		408,
		{4, "\x74\x65\x73\x74"},
		15,
		IS_STREAM_NO,
		XFAIL_NO,
		COMPARTMENT_ID
	}
	,
	{
		//** 31 (requires previous test)**
		"A.1.16.(3)  STATE-ACCESS",
		RFC4465_A_1_16__STATE_ACCESS_3,
		408,
		{15, "STATE_NOT_FOUND"},
		0,
		IS_STREAM_NO,
		XFAIL_YES,
		COMPARTMENT_ID
	}
	,
	{
		//** 32 (requires previous test)**
		"A.1.16.(4)  STATE-ACCESS",
		RFC4465_A_1_16__STATE_ACCESS_4,
		408,
		{15, "STATE_NOT_FOUND"},
		0,
		IS_STREAM_NO,
		XFAIL_YES,
		COMPARTMENT_ID
	}
	,
	{
		//** 33 (requires previous test)**
		"A.1.16.(5)  STATE-ACCESS",
		RFC4465_A_1_16__STATE_ACCESS_5,
		408,
		{15, "STATE_TOO_SHORT"},
		0,
		IS_STREAM_NO,
		XFAIL_YES,
		COMPARTMENT_ID
	}
	,
	{
		//** 34**
		"A.2.1.(1)  Useful Values",
		RFC4465_A_2_1__Useful_Values_1,
		93,
		{0, ""},
		966,
		IS_STREAM_NO,
		XFAIL_NO,
		COMPARTMENT_ID
	}
	,
	{
		//** 35 (Requires previous test)**
		"A.2.1.(2)  Useful Values",
		RFC4465_A_2_1__Useful_Values_2,
		9,
		{0, ""},
		(1072 << 4),
		IS_STREAM_NO,
		XFAIL_NO,
		COMPARTMENT_ID
	}
	,
	{
		//** 36 (Requires previous test)**
		"A.2.1.(3)  Useful Values",
		RFC4465_A_2_1__Useful_Values_3,
		9,
		{16, "CYCLES_EXHAUSTED"},
		0,
		IS_STREAM_NO,
		XFAIL_YES,
		COMPARTMENT_ID
	}
	,
	{
		//** 37 (Requires previous test)**
		"A.2.1.(4)  Useful Values",
		RFC4465_A_2_1__Useful_Values_4,
		9,
		{8, "SEGFAULT"},
		0,
		IS_STREAM_NO,
		XFAIL_YES,
		COMPARTMENT_ID
	}
	,
	{
		//** 38**
		"A.2.2.  Cycles Checking",
		RFC4465_A_2_2__Cycles_Checking,
		29,
		{16, "CYCLES_EXHAUSTED"},
		0,
		IS_STREAM_NO,
		XFAIL_YES,
		COMPARTMENT_ID
	}
	,
	{
		//** 39**
		"A.2.3.(1)  Message-based Transport",
		RFC4465_A_2_3_Message_based_Transport_1,
		1,
		{17, "MESSAGE_TOO_SHORT"},
		0,
		IS_STREAM_NO,
		XFAIL_YES,
		COMPARTMENT_ID
	}
	,
	{
		//** 40**
		"A.2.3.(2)  Message-based Transport",
		RFC4465_A_2_3_Message_based_Transport_2,
		2,
		{17, "MESSAGE_TOO_SHORT"},
		0,
		IS_STREAM_NO,
		XFAIL_YES,
		COMPARTMENT_ID
	}
	,
	{
		//** 41**
		"A.2.3.(3)  Message-based Transport",
		RFC4465_A_2_3_Message_based_Transport_3,
		17,
		{25, "decompression_memory_size"},
		5,
		IS_STREAM_NO,
		XFAIL_YES,
		COMPARTMENT_ID
	}
	,
	{
		//** 42**
		"A.2.3.(4)  Message-based Transport",
		RFC4465_A_2_3_Message_based_Transport_4,
		17,
		{17, "MESSAGE_TOO_SHORT"},
		0,
		IS_STREAM_NO,
		XFAIL_YES,
		COMPARTMENT_ID
	}
	,
	{
		//** 43**
		"A.2.3.(5)  Message-based Transport",
		RFC4465_A_2_3_Message_based_Transport_5,
		17,
		{21, "INVALID_CODE_LOCATION"},
		0,
		IS_STREAM_NO,
		XFAIL_YES,
		COMPARTMENT_ID
	}
	,
	{
		//** 44**
		"A.2.3.(6)  Message-based Transport",
		RFC4465_A_2_3_Message_based_Transport_6,
		17,
		{25, "decompression_memory_size"},
		5,
		IS_STREAM_NO,
		XFAIL_NO,
		COMPARTMENT_ID
	}
	,
	{
		//** 45**
		"A.2.4.(1&2)  Stream-based Transport",
		RFC4465_A_2_4_Stream_based_Transport_1,
		67,
		{25, "decompression_memory_size"},
		11,
		IS_STREAM_YES,
		XFAIL_NO,
		COMPARTMENT_ID
	}
	,
	{
		//** 46**
		"A.2.4.(3)  Stream-based Transport",
		RFC4465_A_2_4_Stream_based_Transport_3,
		3,
		{17, "MESSAGE_TOO_SHORT"},
		0,
		IS_STREAM_YES,
		XFAIL_YES,
		COMPARTMENT_ID
	}
	,
	{
		//** 47**
		"A.2.4.(4)  Stream-based Transport",
		RFC4465_A_2_4_Stream_based_Transport_4,
		4,
		{17, "MESSAGE_TOO_SHORT"},
		0,
		IS_STREAM_YES,
		XFAIL_YES,
		COMPARTMENT_ID
	}
	,
	{
		//** 48**
		"A.2.4.(5)  Stream-based Transport",
		RFC4465_A_2_4_Stream_based_Transport_5,
		30,
		{17, "MESSAGE_TOO_SHORT"},
		0,
		IS_STREAM_YES,
		XFAIL_YES,
		COMPARTMENT_ID
	}
	,
	{
		//** 49**
		"A.2.4.(5.1)  Stream-based Transport (cleanup)",
		"\xff\xff",
		2,
		{14, "INTERNAL_ERROR"},
		0,
		IS_STREAM_YES,
		XFAIL_YES,
		COMPARTMENT_ID
	}
	,
	{
		//** 50**
		"A.2.4.(6)  Stream-based Transport",
		RFC4465_A_2_4_Stream_based_Transport_6,
		29,
		{21, "INVALID_CODE_LOCATION"},
		0,
		IS_STREAM_YES,
		XFAIL_YES,
		COMPARTMENT_ID
	}
	,
	{
		//** 51**
		"A.2.4.(6.1)  Stream-based Transport (cleanup)",
		"\xff\xff",
		2,
		{14, "INTERNAL_ERROR"},
		0,
		IS_STREAM_YES,
		XFAIL_YES,
		COMPARTMENT_ID
	}
	,
	{
		//** 52**
		"A.2.5.(1)  Input Past the End of a Message",
		RFC4465_A_2_5__Input_Past_the_End_of_a_Message_1,
		59,
		{3, "\x68\x69\x21"},
		23,
		IS_STREAM_NO,
		XFAIL_NO,
		COMPARTMENT_ID
	}
	,
	{
		//** 53**
		"A.2.5.(2)  Input Past the End of a Message",
		RFC4465_A_2_5__Input_Past_the_End_of_a_Message_2,
		58,
		{14, "USER_REQUESTED"},
		0,
		IS_STREAM_NO,
		XFAIL_YES,
		COMPARTMENT_ID
	}
	,
	{
		//** 54**
		"A.3.1.(1)  SigComp Feedback Mechanism",
		RFC4465_A_3_1__SigComp_Feedback_Mechanism_1,
		84,
		{0, ""},
		52,
		IS_STREAM_NO,
		XFAIL_NO,
		COMPARTMENT_ID
	}
	,
	{
		//** 55**
		"A.3.1.(2)  SigComp Feedback Mechanism",
		RFC4465_A_3_1__SigComp_Feedback_Mechanism_2,
		84,
		{0, ""},
		179,
		IS_STREAM_NO,
		XFAIL_NO,
		COMPARTMENT_ID
	}
	,
	{
		//** 56 (SMS MUST be 2048)**
		"A.3.2.(1)  State Memory Management",
		RFC4465_A_3_2__State_Memory_Management_1,
		446,
		{0, ""},
		811,
		IS_STREAM_NO,
		XFAIL_NO,
		COMPARTMENT_ID
	}
	,
	{
		//** 57 (Requires previous test)**
		"A.3.2.(2)  State Memory Management",
		RFC4465_A_3_2__State_Memory_Management_2,
		446,
		{0, ""},
		2603,
		IS_STREAM_NO,
		XFAIL_NO,
		COMPARTMENT_ID
	}
	,
	{
		//** 58 (Requires previous test)**
		"A.3.2.(3)  State Memory Management",
		RFC4465_A_3_2__State_Memory_Management_3,
		446,
		{0, ""},
		811,
		IS_STREAM_NO,
		XFAIL_NO,
		COMPARTMENT_ID
	}
	,
	{
		//** 59 (Requires previous test)**
		"A.3.2.(4)  State Memory Management",
		RFC4465_A_3_2__State_Memory_Management_4,
		446,
		{0, ""},
		1805,
		IS_STREAM_NO,
		XFAIL_NO,
		COMPARTMENT_ID
	}
	,
	{
		//** 60 (Requires previous test)**
		"A.3.2.(5)  State Memory Management",
		RFC4465_A_3_2__State_Memory_Management_5,
		446,
		{15, "STATE_NOT_FOUND"},
		0,
		IS_STREAM_NO,
		XFAIL_YES,
		COMPARTMENT_ID
	}
	,
	{
		//** 61 (Requires previous test)**
		"A.3.2.(6)  State Memory Management",
		RFC4465_A_3_2__State_Memory_Management_6,
		446,
		{0, ""},
		2057,
		IS_STREAM_NO,
		XFAIL_NO,
		COMPARTMENT_ID
	}
	,
	{
		//** 62 (Requires previous test)**
		"A.3.2.(7)  State Memory Management",
		RFC4465_A_3_2__State_Memory_Management_7,
		446,
		{0, ""},
		1993,
		IS_STREAM_NO,
		XFAIL_NO,
		COMPARTMENT_ID
	}
	,
	{
		//** 63**
		"A.3.3.(1)  Multiple Compartments",
		RFC4465_A_3_3__Multiple_Compartments_1,
		437,
		{0, ""},
		1809,
		IS_STREAM_NO,
		XFAIL_NO,
		COMPARTMENT_ID_0
	}
	,
	{
		//** 64 (requires previsous test)**
		"A.3.3.(2)  Multiple Compartments",
		RFC4465_A_3_3__Multiple_Compartments_2,
		437,
		{0, ""},
		1809,
		IS_STREAM_NO,
		XFAIL_NO,
		COMPARTMENT_ID_1
	}
	,
	{
		//** 65 (requires previsous test)**
		"A.3.3.(3)  Multiple Compartments",
		RFC4465_A_3_3__Multiple_Compartments_3,
		437,
		{0, ""},
		1809,
		IS_STREAM_NO,
		XFAIL_NO,
		COMPARTMENT_ID_2
	}
	,
	{
		//** 66 (requires previsous test)**
		"A.3.3.(4)  Multiple Compartments",
		RFC4465_A_3_3__Multiple_Compartments_4,
		437,
		{0, ""},
		1993,
		IS_STREAM_NO,
		XFAIL_NO,
		COMPARTMENT_ID_0
	}
	,
	{
		//** 67 (requires previsous test)**
		"A.3.3.(5)  Multiple Compartments",
		RFC4465_A_3_3__Multiple_Compartments_5,
		437,
		{0, ""},
		1994,
		IS_STREAM_NO,
		XFAIL_NO,
		COMPARTMENT_ID_1
	}
	,
	{
		//** 68 (requires previsous test)**
		"A.3.3.(6)  Multiple Compartments",
		RFC4465_A_3_3__Multiple_Compartments_6,
		437,
		{0, ""},
		1804,
		IS_STREAM_NO,
		XFAIL_NO,
		COMPARTMENT_ID_2
	}
	,
	{
		//** 69 (requires previsous test)**
		"A.3.3.(7)  Multiple Compartments",
		RFC4465_A_3_3__Multiple_Compartments_7,
		437,
		{15, "STATE_NOT_FOUND"},
		0,
		IS_STREAM_NO,
		XFAIL_YES,
		COMPARTMENT_ID_0
	}
	,
	{
		//** 70 (requires previsous test)**
		"A.3.3.(8)  Multiple Compartments",
		RFC4465_A_3_3__Multiple_Compartments_8,
		437,
		{15, "STATE_NOT_FOUND"},
		0,
		IS_STREAM_NO,
		XFAIL_YES,
		COMPARTMENT_ID_1
	}
	,
	{
		//** 71 (requires previsous test)**
		"A.3.3.(9)  Multiple Compartments",
		RFC4465_A_3_3__Multiple_Compartments_9,
		437,
		{15, "STATE_NOT_FOUND"},
		0,
		IS_STREAM_NO,
		XFAIL_YES,
		COMPARTMENT_ID_2
	}
	,
	{
		//** 72**
		"A.3.4.  Accessing RFC 3485 State",
		RFC4465_A_3_4__Accessing_RFC_3485_State,
		61,
		{3, "\x53\x49\x50"},
		11,
		IS_STREAM_NO,
		XFAIL_NO,
		COMPARTMENT_ID
	}
	,
	{
		//** 73**
		"A.3.5.(1)  Bytecode State Creation",
		RFC4465_A_3_5__Bytecode_State_Creation_1,
		74,
		{2, "\x4f\x4b"},
		66,
		IS_STREAM_NO,
		XFAIL_NO,
		"Comp-id:A.3.5.  Bytecode State Creation"
	}
	,
	{
		//** 74**
		"A.3.5.(2)  Bytecode State Creation",
		RFC4465_A_3_5__Bytecode_State_Creation_2,
		8,
		{3, "\x4f\x4b\x31"},
		7,
		IS_STREAM_NO,
		XFAIL_NO,
		"Comp-id:A.3.5.  Bytecode State Creation"
	}
	,
	{
		//** 75**
		"A.3.5.(3)  Bytecode State Creation",
		RFC4465_A_3_5__Bytecode_State_Creation_3,
		13,
		{3, "\x4f\x4b\x32"},
		5,
		IS_STREAM_NO,
		XFAIL_NO,
		"Comp-id:A.3.5.  Bytecode State Creation"
	}
	,
	{
		//** 76**
		"A.3.5.(4)  Bytecode State Creation",
		RFC4465_A_3_5__Bytecode_State_Creation_4,
		7,
		{3, "\x00\x00\x32"},
		5,
		IS_STREAM_NO,
		XFAIL_NO,
		"Comp-id:A.3.5.  Bytecode State Creation"
	}
	,
	{
		//** 77**
		"A.3.5.(5)  Bytecode State Creation",
		RFC4465_A_3_5__Bytecode_State_Creation_5,
		7,
		{15, "STATE_NOT_FOUND"},
		0,
		IS_STREAM_NO,
		XFAIL_YES,
		"Comp-id:A.3.5.  Bytecode State Creation"
	}
};

int startsWith(const char* buffer1, size_t size1, const char* buffer2, size_t size2) 
{
	size_t i;

	if(!size2) return 1;
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

static const int16_t wordForEndianess = 0x4321;
static tsk_bool_t isBigEndian;

static TCOMP_INLINE uint16_t HostToNetworkShort(uint16_t x)
{
	return isBigEndian ? (x) : ((((uint16_t)(x) & 0xff00) >> 8) |
						(((uint16_t)(x) & 0x00ff) << 8));
}

static void checkResult(const struct_torture_test* test, const tcomp_result_t *result, const void* output_ptr, tsk_size_t output_size)
{
	tsk_bool_t ok_cycles = (result->consumed_cycles == test->xcycles);
	tsk_bool_t ok_output = tsk_true;

	if(tsk_striequals(test->xoutput.ptr, "decompression_memory_size")){
		printf("xoutput (decompression_memory_size): %s\n", (HostToNetworkShort(DMS) == *((uint16_t*)output_ptr)) ? "YES" : "NO");
	}
	else{
		ok_output = startsWith(output_ptr, output_size, test->xoutput.ptr, test->xoutput.size);
		printf("xoutput: %s\n", ok_output ? "YES" : "NO");
	}
	printf("xcycles: %s\n", ok_cycles ? "YES" : "NO");
	if(!ok_cycles || !ok_output){
		assert(test->xfail && result->isNack); // failure must be expected and NACK have to be present
		assert(tsk_strequals(test->xoutput.ptr, tcomp_nackinfo_get_description(result->nack_info))); // returned NACK and expected one must be equal
	}
}

static int test_tortures()
{
	size_t i, start, end;
	size_t res_size = 0;
	char buffer[OUTPUT_BUFFER_SIZE];

#if RUN_TEST_LOOP
	for(;;)
#endif
	{
		tcomp_manager_handle_t *manager = tcomp_manager_create();
		tcomp_result_t *result = tcomp_result_create();

		isBigEndian = ((*(int8_t *)&wordForEndianess) != 0x21);

		/* Add SIP dictionary. */
		tcomp_manager_addSipSdpDictionary(manager);

		/* Add Presence dictionary. */
		tcomp_manager_addPresenceDictionary(manager);

		/* Set decompression size. */
		tcomp_manager_setDecompression_Memory_Size(manager, DMS);
		/* Set state memory size. */
		tcomp_manager_setState_Memory_Size(manager, SMS);
		/* Set Cycles Per Bit */
		tcomp_manager_setCycles_Per_Bit(manager, CPB);

#if RUN_TEST_ALL
		start = 0, end = sizeof(tests)/sizeof(tests[0]);
#else
		start = RUN_TEST_NO, end = RUN_TEST_NO + 1;
#endif

		for(i = start; i<end; i++){
			printf("=== Testing %s ===\n\n", tests[i].section_name);

			if(tests[i].stream){
				tcomp_result_setOutputTCPBuffer(result, buffer, OUTPUT_BUFFER_SIZE, STREAM_ID);
			}
			else{
				tcomp_result_setOutputUDPBuffer(result, buffer, OUTPUT_BUFFER_SIZE);
			}

			/* Performs decompression */
			res_size = tcomp_manager_decompress(manager, tests[i].bytecode, tests[i].bytecode_size, result);
			if(result->isNack){
				printf("\n==WE GOT A NACK\n\n");
				//sendto(tcomp_buffer_getBuffer(result->nack_info), tcomp_buffer_getSize(result->nack_info));
			}
			else{
				tcomp_result_setCompartmentId(result, tests[i].comp_id, strlen(tests[i].comp_id));
				tcomp_manager_provideCompartmentId(manager, result);
			}

			/* Checks result */
			checkResult(&tests[i], result, buffer, sizeof(buffer));

			/* Get sub-sequent stream messages */
			if(tests[i].stream && (res_size || result->isNack)){
				uint64_t consumed_cycles = result->consumed_cycles; // save "consumed_cycles" (shared by all sub-sequent messages)
				for(;;){
					res_size = tcomp_manager_getNextStreamMessage(manager, result);
					if(!res_size && !result->isNack){
						break;
					}
					else if(res_size){
						tcomp_result_setCompartmentId(result, tests[i].comp_id, strlen(tests[i].comp_id));
						tcomp_manager_provideCompartmentId(manager, result);
					}
					else{
						printf("\n==WE GOT A NACK\n\n");
						//sendto(tcomp_buffer_getBuffer(result->nack_info), tcomp_buffer_getSize(result->nack_info));
					}
					result->consumed_cycles = consumed_cycles; // restore cycles just for checking. Had been cleared by "tcomp_manager_getNextStreamMessage()"
					checkResult(&tests[i], result, buffer, sizeof(buffer));
				}
			}
		}

		/* Free previously allocated resources. */
		TSK_OBJECT_SAFE_FREE(result);
		TSK_OBJECT_SAFE_FREE(manager);

	}/* LOOP */

	return 0;
}

#endif /* TEST_TINYSIGCOMP_TORTURES_H */
