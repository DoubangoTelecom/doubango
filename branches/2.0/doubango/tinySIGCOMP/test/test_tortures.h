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
#define OUTPUT_BUFFER_SIZE			2000
#define DMS	8192 //4096//8192//65536
#define CPB	64 // 16//32//64//128

#define RUN_TEST_LOOP	1

#define RUN_TEST_ALL	1
#define RUN_TEST_NO		68

#define IS_STREAM_YES	1
#define IS_STREAM_NO	0

#define XFAIL_YES		1
#define XFAIL_NO		0

#include "rfc4465_torture_tests.h"

struct_torture_test tests[] = 
{
	{	//*** 0***
		"A.1.1.  Bit Manipulation", 
		RFC4465_A_1_1__Bit_Manipulation, 
		170, 
		{8, "\x01\x50\x00\x00\xfe\xbf\x00\x00"}, 
		22,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{	//*** 1***
		"A.1.2(.1)  Arithmetic",
		RFC4465_A_1_2__Arithmetic_1,
		174,
		{8, "\x00\x00\x00\x00\x00\x00\x00\x04"},
		25,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{	//*** 1.1***
		"A.1.2(.2)  Arithmetic",
		RFC4465_A_1_2__Arithmetic_2,
		174,
		{11, "DIV_BY_ZERO"},
		0,
		IS_STREAM_NO,
		XFAIL_YES
	}
	,
	{	//*** 1.2***
		"A.1.2(.3)  Arithmetic",
		RFC4465_A_1_2__Arithmetic_3,
		174,
		{11, "DIV_BY_ZERO"},
		0,
		IS_STREAM_NO,
		XFAIL_YES
	}
	,
	{
		//*** 2***
		"A.1.3.  Sorting",
		RFC4465_A_1_3__Sorting,
		223,
		{45, "\x46\x6f\x72\x64\x2c\x20\x79\x6f\x75\x27\x72\x65\x20\x74\x75\x72\x6e\x69\x6e\x67"
        "\x20\x69\x6e\x74\x6f\x20\x61\x20\x70\x65\x6e\x67\x75\x69\x6e\x2e\x20\x53\x74\x6f"
		"\x70\x20\x69\x74\x2e"},
		371,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{
		//*** 3**
		"A.1.4.  SHA-1",
		RFC4465_A_1_4__SHA1,
		138,
		{40, "\xa9\x99\x3e\x36\x47\x06\x81\x6a\xba\x3e\x25\x71\x78\x50\xc2\x6c\x9c\xd0\xd8\x9d"
		"\x84\x98\x3e\x44\x1c\x3b\xd2\x6e\xba\xae\x4a\xa1\xf9\x51\x29\xe5\xe5\x46\x70\xf1"
		"\x12\xff\x34\x7b\x4f\x27\xd6\x9e\x1f\x32\x8e\x6f\x4b\x55\x73\xe3\x66\x6e\x12\x2f"
		"\x4f\x46\x04\x52\xeb\xb5\x63\x93\x4f\x46\x04\x52\xeb\xb5\x63\x93\x4f\x46\x04\x52"},
		17176,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{
		//*** 4**
		"A.1.5.(1)  LOAD and MULTILOAD",
		RFC4465_A_1_5__LOAD_and_MULTILOAD_1,
		58,
		{16, "\x00\x84\x00\x84\x00\x86\x00\x86\x00\x2a\x00\x80\x00\x2a\x00\x2a"},
		36,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{
		//*** 4**
		"A.1.5.(2)  LOAD and MULTILOAD",
		RFC4465_A_1_5__LOAD_and_MULTILOAD_2,
		58,
		{21, "MULTILOAD_OVERWRITTEN"},
		36,
		IS_STREAM_NO,
		XFAIL_YES
	}
	,
	{
		//*** 4**
		"A.1.5.(3)  LOAD and MULTILOAD",
		RFC4465_A_1_5__LOAD_and_MULTILOAD_3,
		58,
		{21, "MULTILOAD_OVERWRITTEN"},
		36,
		IS_STREAM_NO,
		XFAIL_YES
	}
	,
	{
		//** 5**
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
		XFAIL_NO
	}
	,
	{
		//** 6**
		"A.1.7.  COPY-LITERAL and COPY-OFFSET",
		RFC4465_A_1_7__COPY_LITERAL_and_COPY_OFFSET,
		100,
		{34, "\x41\x41\x41\x41\x00\x61\x41\x41\x41\x41\x49\x4a\x41\x42\x43\x44\x49\x4a\x41\x42"
		"\x00\x4a\x00\x4e\x47\x48\x48\x45\x46\x47\x47\x48\x45\x46"},
		216,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{
		//** 7**
		"A.1.8.  MEMSET",
		RFC4465_A_1_8__MEMSET,
		27,
		{16, "\x80\x40\x4f\x5e\x6d\x7c\x8b\x9a\xa9\xb8\xc7\xd6\xe5\xf4\x03\x12"},
		166,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{
		//** 8**
		"A.1.9.(1)  CRC",
		RFC4465_A_1_9__CRC_1,
		29,
		{0, ""},
		95,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{
		//** 8**
		"A.1.9.(2)  CRC",
		RFC4465_A_1_9__CRC_2,
		29,
		{14, "USER_REQUESTED"},
		95,
		IS_STREAM_NO,
		XFAIL_YES
	}
	,
	{
		//** 9**
		"A.1.10.  INPUT-BITS",
		RFC4465_A_1_10__INPUT_BITS,
		28,
		{16, "\x00\x00\x00\x02\x00\x02\x00\x13\x00\x00\x00\x03\x00\x1a\x00\x38"},
		66,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{
		//** 10**
		"A.1.11.  INPUT-HUFFMAN",
		RFC4465_A_1_11__INPUT_HUFFMAN,
		39,
		{16, "\x00\x00\x00\x03\x00\x08\x04\xd7\x00\x02\x00\x03\x03\x99\x30\xfe"},
		84,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{
		//** 11**
		"A.1.12.  INPUT-BYTES",
		RFC4465_A_1_12__INPUT_BYTES,
		72,
		{39, "\x00\x00\x93\x2e\x00\x01\xb1\x66\xd8\x6f\xb1\x00\x1a\x2b\x00\x03\x9a\x97\x34\xd8"
		"\x00\x07\x00\x01\x33\x87\x4e\x00\x08\xdc\x96\x51\xb5\xdc\x96\x00\x59\x9d\x6a"},
		130,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{
		//** 12**
		"A.1.13.  Stack Manipulation",
		RFC4465_A_1_13__Stack_Manipulation,
		324,
		{16, "\x00\x03\x00\x02\x00\x01\x00\x42\x00\x42\x00\x00\x00\x01\x00\x01"},
		40,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{
		//** 13**
		"A.1.14.  Program Flow",
		RFC4465_A_1_14__Program_Flow,
		66,
		{20, "\x00\x01\x01\x02\x02\x03\x03\x04\x04\x05\x05\x06\x07\x07\x07\x08\x08\x08\x09\x09"},
		131,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{
		//** 14**
		"A.1.15.  State Creation_1",
		RFC4465_A_1_15__State_Creation_1,
		152,
		{0, ""},
		23,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{
		//** 15**
		"A.1.15.  State Creation_2",
		RFC4465_A_1_15__State_Creation_2,
		152,
		{0, ""},
		14,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{
		//** 16**
		"A.1.15.  State Creation_3",
		RFC4465_A_1_15__State_Creation_3,
		152,
		{0, ""},
		24,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{
		//** 17**
		"A.1.15.  State Creation_4", // FAIL
		RFC4465_A_1_15__State_Creation_4,
		153,
		{0, ""},
		0,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{
		//** 18**
		"A.1.15.  State Creation_5", // FAIL
		RFC4465_A_1_15__State_Creation_5,
		153,
		{0, ""},
		0,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{
		//** 19**
		"A.1.15.  State Creation_6",
		RFC4465_A_1_15__State_Creation_6,
		153,
		{0, ""},
		23,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{
		//** 20**
		"A.1.15.  State Creation_7",
		RFC4465_A_1_15__State_Creation_7,
		152,
		{0, ""},
		34,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{
		//** 21**
		"A.1.15.  State Creation_8",
		RFC4465_A_1_15__State_Creation_8,
		153,
		{0, ""},
		46,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{
		//** 22**
		"A.1.15.  State Creation_9",
		RFC4465_A_1_15__State_Creation_9,
		153,
		{0, ""},
		47,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{
		//** 23**
		"A.1.15.  State Creation_10",
		RFC4465_A_1_15__State_Creation_10,
		153,
		{0, ""},
		60,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{
		//** 24**
		"A.1.16.  STATE-ACCESS {Set up bytecode}",
		RFC4465_A_1_16__STATE_ACCESS__SETUP,
		403,
		{0, ""},
		17,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{
		//** 25**
		"A.1.16.  STATE-ACCESS_1",
		RFC4465_A_1_16__STATE_ACCESS_1,
		408,
		{4, "\x74\x65\x73\x74"},
		26,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{
		//** 26**
		"A.1.16.  STATE-ACCESS_2",
		RFC4465_A_1_16__STATE_ACCESS_2,
		408,
		{4, "\x74\x65\x73\x74"},
		15,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{
		//** 27**
		"A.1.16.  STATE-ACCESS_3",
		RFC4465_A_1_16__STATE_ACCESS_3,
		408,
		{0, ""},
		0,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{
		//** 28**
		"A.1.16.  STATE-ACCESS_4",
		RFC4465_A_1_16__STATE_ACCESS_4,
		408,
		{0, ""},
		0,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{
		//** 29**
		"A.1.16.  STATE-ACCESS_5",
		RFC4465_A_1_16__STATE_ACCESS_5,
		408,
		{0, ""},
		0,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{
		//** 30**
		"A.2.1.(1)  Useful Values_1",
		RFC4465_A_2_1__Useful_Values_1,
		99,
		{0, ""},
		968,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{
		//** 31 (Requires previous test)**
		"A.2.1.(2)  Useful Values_2",
		RFC4465_A_2_1__Useful_Values_2,
		10,
		{0, ""},
		(CPB * 1080),
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{
		//** 32 (Requires previous test)**
		"A.2.1.(3)  Useful Values_3",
		RFC4465_A_2_1__Useful_Values_3,
		9,
		{16, "CYCLES_EXHAUSTED"},
		0,
		IS_STREAM_NO,
		XFAIL_YES
	}
	,
	{
		//** 33 (Requires previous test)**
		"A.2.1.(4)  Useful Values_4",
		RFC4465_A_2_1__Useful_Values_4,
		9,
		{8, "SEGFAULT"},
		0,
		IS_STREAM_NO,
		XFAIL_YES
	}
	,
	{
		//** 34**
		"A.2.2.  Cycles Checking",
		RFC4465_A_2_2__Cycles_Checking,
		29,
		{0, ""},
		0,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{
		//** 35**
		"A.2.3.  Message-based Transport_1",
		RFC4465_A_2_3_Message_based_Transport_1,
		1,
		{17, "MESSAGE_TOO_SHORT"},
		0,
		IS_STREAM_NO,
		XFAIL_YES
	}
	,
	{
		//** 36**
		"A.2.3.  Message-based Transport_2",
		RFC4465_A_2_3_Message_based_Transport_2,
		2,
		{17, "MESSAGE_TOO_SHORT"},
		0,
		IS_STREAM_NO,
		XFAIL_YES
	}
	,
	{
		//** 37**
		"A.2.3.  Message-based Transport_3",
		RFC4465_A_2_3_Message_based_Transport_3,
		17,
		{25, "decompression_memory_size"},
		5,
		IS_STREAM_NO,
		XFAIL_YES
	}
	,
	{
		//** 38**
		"A.2.3.  Message-based Transport_4",
		RFC4465_A_2_3_Message_based_Transport_4,
		17,
		{17, "MESSAGE_TOO_SHORT"},
		0,
		IS_STREAM_NO,
		XFAIL_YES
	}
	,
	{
		//** 39**
		"A.2.3.  Message-based Transport_5",
		RFC4465_A_2_3_Message_based_Transport_5,
		17,
		{21, "INVALID_CODE_LOCATION"},
		0,
		IS_STREAM_NO,
		XFAIL_YES
	}
	,
	{
		//** 40**
		"A.2.3.  Message-based Transport_6",
		RFC4465_A_2_3_Message_based_Transport_6,
		17,
		{25, "decompression_memory_size"},
		5,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{
		//** 41**
		"A.2.4.  Stream-based Transport_1",
		RFC4465_A_2_4_Stream_based_Transport_1,
		67,
		{25, "decompression_memory_size"},
		11,
		IS_STREAM_YES,
		XFAIL_NO
	}
	,
	{
		//** 43**
		"A.2.4.  Stream-based Transport_3",
		RFC4465_A_2_4_Stream_based_Transport_3,
		3,
		{17, "MESSAGE_TOO_SHORT"},
		0,
		IS_STREAM_YES,
		XFAIL_YES
	}
	,
	{
		//** 44**
		"A.2.4.  Stream-based Transport_4",
		RFC4465_A_2_4_Stream_based_Transport_4,
		4,
		{17, "MESSAGE_TOO_SHORT"},
		0,
		IS_STREAM_YES,
		XFAIL_YES
	}
	,
	{
		//** 45**
		"A.2.4.  Stream-based Transport_5",
		RFC4465_A_2_4_Stream_based_Transport_5,
		30,
		{17, "MESSAGE_TOO_SHORT"},
		0,
		IS_STREAM_YES,
		XFAIL_YES
	}
	,
	{
		//** 46**
		"A.2.4.  Stream-based Transport_6",
		RFC4465_A_2_4_Stream_based_Transport_6,
		29,
		{21, "INVALID_CODE_LOCATION"},
		0,
		IS_STREAM_YES,
		XFAIL_YES
	}
	,
	{
		//** 47**
		"A.2.5.  Input Past the End of a Message_1",
		RFC4465_A_2_5__Input_Past_the_End_of_a_Message_1,
		59,
		{3, "\x68\x69\x21"},
		23,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{
		//** 48**
		"A.2.5.  Input Past the End of a Message_2",
		RFC4465_A_2_5__Input_Past_the_End_of_a_Message_2,
		58,
		{14, "USER_REQUESTED"},
		0,
		IS_STREAM_NO,
		XFAIL_YES
	}
	,
	{
		//** 49**
		"A.3.1.  SigComp Feedback Mechanism_1",
		RFC4465_A_3_1__SigComp_Feedback_Mechanism_1,
		84,
		{0, ""},
		52,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{
		//** 50**
		"A.3.1.  SigComp Feedback Mechanism_2",
		RFC4465_A_3_1__SigComp_Feedback_Mechanism_2,
		84,
		{0, ""},
		179,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{
		//** 51**
		"A.3.2.  State Memory Management_1",
		RFC4465_A_3_2__State_Memory_Management_1,
		446,
		{0, ""},
		811,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{
		//** 52 (Requires previous test)**
		"A.3.2.  State Memory Management_2",
		RFC4465_A_3_2__State_Memory_Management_2,
		446,
		{0, ""},
		2603,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{
		//** 53 (Requires previous test)**
		"A.3.2.  State Memory Management_3",
		RFC4465_A_3_2__State_Memory_Management_3,
		446,
		{0, ""},
		811,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{
		//** 54 (Requires previous test)**
		"A.3.2.  State Memory Management_4",
		RFC4465_A_3_2__State_Memory_Management_4,
		446,
		{0, ""},
		1805,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{
		//** 55 (Requires previous test)**
		"A.3.2.  State Memory Management_5",
		RFC4465_A_3_2__State_Memory_Management_5,
		446,
		{15, "STATE_NOT_FOUND"},
		0,
		IS_STREAM_NO,
		XFAIL_YES
	}
	,
	{
		//** 56 (Requires previous test)**
		"A.3.2.  State Memory Management_6",
		RFC4465_A_3_2__State_Memory_Management_6,
		446,
		{0, ""},
		2057,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{
		//** 57 (Requires previous test)**
		"A.3.2.  State Memory Management_7",
		RFC4465_A_3_2__State_Memory_Management_7,
		446,
		{0, ""},
		1993,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{
		//** 58**
		"A.3.3.  Multiple Compartments_1",
		RFC4465_A_3_3__Multiple_Compartments_1,
		437,
		{0, ""},
		1809,
		IS_STREAM_NO
	}
	,
	{
		//** 59**
		"A.3.3.  Multiple Compartments_2",
		RFC4465_A_3_3__Multiple_Compartments_2,
		437,
		{0, ""},
		1809,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{
		//** 60**
		"A.3.3.  Multiple Compartments_3",
		RFC4465_A_3_3__Multiple_Compartments_3,
		437,
		{0, ""},
		1809,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{
		//** 61**
		"A.3.3.  Multiple Compartments_4",
		RFC4465_A_3_3__Multiple_Compartments_4,
		437,
		{0, ""},
		1993,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{
		//** 62**
		"A.3.3.  Multiple Compartments_5",
		RFC4465_A_3_3__Multiple_Compartments_5,
		437,
		{0, ""},
		1994,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{
		//** 63**
		"A.3.3.  Multiple Compartments_6",
		RFC4465_A_3_3__Multiple_Compartments_6,
		437,
		{0, ""},
		1804,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{
		//** 64**
		"A.3.3.  Multiple Compartments_7",
		RFC4465_A_3_3__Multiple_Compartments_7,
		437,
		{15, "STATE_NOT_FOUND"},
		0,
		IS_STREAM_NO,
		XFAIL_YES
	}
	,
	{
		//** 65**
		"A.3.3.  Multiple Compartments_8",
		RFC4465_A_3_3__Multiple_Compartments_8,
		437,
		{15, "STATE_NOT_FOUND"},
		0,
		IS_STREAM_NO,
		XFAIL_YES
	}
	,
	{
		//** 66**
		"A.3.3.  Multiple Compartments_9",
		RFC4465_A_3_3__Multiple_Compartments_9,
		437,
		{15, "STATE_NOT_FOUND"},
		0,
		IS_STREAM_NO,
		XFAIL_YES
	}
	,
	{
		//** 67**
		"A.3.4.  Accessing RFC 3485 State",
		RFC4465_A_3_4__Accessing_RFC_3485_State,
		61,
		{3, "\x53\x49\x50"},
		11,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{
		//** 68**
		"A.3.5.  Bytecode State Creation_1",
		RFC4465_A_3_5__Bytecode_State_Creation_1,
		74,
		{2, "\x4f\x4b"},
		66,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{
		//** 69**
		"A.3.5.  Bytecode State Creation_2",
		RFC4465_A_3_5__Bytecode_State_Creation_2,
		8,
		{3, "\x4f\x4b\x31"},
		7,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{
		//** 70**
		"A.3.5.  Bytecode State Creation_3",
		RFC4465_A_3_5__Bytecode_State_Creation_3,
		13,
		{3, "\x4f\x4b\x32"},
		5,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{
		//** 71**
		"A.3.5.  Bytecode State Creation_4",
		RFC4465_A_3_5__Bytecode_State_Creation_4,
		7,
		{3, "\x00\x00\x32"},
		5,
		IS_STREAM_NO,
		XFAIL_NO
	}
	,
	{
		//** 72**
		"A.3.5.  Bytecode State Creation_5",
		RFC4465_A_3_5__Bytecode_State_Creation_5,
		7,
		{15, "STATE_NOT_FOUND"},
		0,
		IS_STREAM_NO,
		XFAIL_YES
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
	if(tsk_striequals(test->xoutput.ptr, "decompression_memory_size")){
		printf("xoutput (decompression_memory_size): %s\n", (HostToNetworkShort(DMS) == *((uint16_t*)output_ptr)) ? "YES" : "NO");
	}
	else{
		printf("xoutput: %s\n", startsWith(output_ptr, output_size, test->xoutput.ptr, test->xoutput.size) ? "YES" : "NO");
	}
	printf("xcycles: %s\n", (result->consumed_cycles == test->xcycles) ? "YES" : "NO");
}

static int test_tortures()
{
	size_t i, start, end;
	size_t res_size = 0;
	char buffer[OUTPUT_BUFFER_SIZE];
	tcomp_manager_handle_t *manager = tcomp_manager_create();
	tcomp_result_t *result = tcomp_result_create();

	isBigEndian = ((*(int8_t *)&wordForEndianess) != 0x21);

	/* Add SIP dictionary. */
	tcomp_manager_addSipSdpDictionary(manager);

	/* Add Presence dictionary. */
	tcomp_manager_addPresenceDictionary(manager);

	/* Set decompression size. */
	tcomp_manager_setDecompression_Memory_Size(manager, DMS);
	/* Set Cycles Per Bit */
	tcomp_manager_setCycles_Per_Bit(manager, CPB);
	
#if RUN_TEST_LOOP
	for(;;)
#endif
	{
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
				tcomp_result_setCompartmentId(result, COMPARTMENT_ID, strlen(COMPARTMENT_ID));
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
						tcomp_result_setCompartmentId(result, COMPARTMENT_ID, strlen(COMPARTMENT_ID));
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

	}/* LOOP */

	/* Free previously allocated resources. */
	TSK_OBJECT_SAFE_FREE(result);
	TSK_OBJECT_SAFE_FREE(manager);

	return 0;
}

#endif /* TEST_TINYSIGCOMP_TORTURES_H */
