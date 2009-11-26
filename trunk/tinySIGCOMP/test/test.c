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

#define LOOP						0

#define RUN_TEST_ALL				0
#define RUN_TEST_TORTURES			1

#include "rfc4465_torture_tests.h"

struct_torture_test tests[] = 
{
	{	//*** 0***
		"A.1.1.  Bit Manipulation", 
		RFC4465_A_1_1__Bit_Manipulation, 
		170, 
		"0x0150 0000 febf 0000", 
		22
	}
	,
	{	//*** 1***
		"A.1.2.  Arithmetic",
		RFC4465_A_1_2__Arithmetic,
		174,
		"0x0150 0000 febf 0000",
		25
	}
	,
	{
		//*** 2***
		"A.1.3.  Sorting",
		RFC4465_A_1_3__Sorting,
		223,
		"0x466f 7264 2c20 796f 7527 7265 2074 7572 6e69 6e67"
        "0x2069 6e74 6f20 6120 7065 6e67 7569 6e2e 2053 746f"
		"0x7020 6974 2e",
		371
	}
	,
	{
		//*** 3**
		"A.1.4.  SHA-1",
		RFC4465_A_1_4__SHA1,
		138,
		"0xa999 3e36 4706 816a ba3e 2571 7850 c26c 9cd0 d89d"
        "0x8498 3e44 1c3b d26e baae 4aa1 f951 29e5 e546 70f1"
        "0x12ff 347b 4f27 d69e 1f32 8e6f 4b55 73e3 666e 122f"
        "0x4f46 0452 ebb5 6393 4f46 0452 ebb5 6393 4f46 0452",
		17176
	}
	,
	{
		//*** 4**
		"A.1.5.  LOAD and MULTILOAD",
		RFC4465_A_1_5__LOAD_and_MULTILOAD,
		58,
		"0x0084 0084 0086 0086 002a 0080 002a 002a",
		36
	}
	,
	{
		//** 5**
		"A.1.6.  COPY",
		RFC4465_A_1_6__COPY,
		60,
		"0x4040 4040 4040 4040 4040 4040 4040 4040 4040 4040"
        "0x4040 4040 4040 4040 4040 4040 4141 4141 4141 4141"
        "0x4141 4141 4141 4141 4141 4141 4141 4141 4141 4141"
        "0x4141 4141 4141 4141 4141 4141 4141 4141 4141 4141"
        "0x4141 4141 4141 4141 4141 4141 4141 4141 4141 4141"
        "0x4141 4141 4141 4141 4141 4141 4141 4141 4141 5541"
        "0x4243 4443 44",
		365
	}
	,
	{
		//** 6**
		"A.1.7.  COPY-LITERAL and COPY-OFFSET",
		RFC4465_A_1_7__COPY_LITERAL_and_COPY_OFFSET,
		100,
		"0x4141 4141 0061 4141 4141 494A 4142 4344 494A 4142"
		"0x004A 004E 4748 4845 4647 4748 4546",
		216
	}
	,
	{
		//** 7**
		"A.1.8.  MEMSET",
		RFC4465_A_1_8__MEMSET,
		27,
		"0x8040 4f5e 6d7c 8b9a a9b8 c7d6 e5f4 0312",
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
		"0x0000 0002 0002 0013 0000 0003 001a 0038",
		66
	}
	,
	{
		//** 10**
		"A.1.11.  INPUT-HUFFMAN",
		RFC4465_A_1_11__INPUT_HUFFMAN,
		39,
		"0x0000 0003 0008 04d7 0002 0003 0399 30fe",
		84
	}
	,
	{
		//** 11**
		"A.1.12.  INPUT-BYTES",
		RFC4465_A_1_12__INPUT_BYTES,
		72,
		"0x0000 932e 0001 b166 d86f b100 1a2b 0003 9a97 34d8"
		"0x0007 0001 3387 4e00 08dc 9651 b5dc 9600 599d 6a",
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
		"0x0001 0102 0203 0304 0405 0506 0707 0708 0808 0909",
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
		"0x7465 7374",
		26
	}
	,
	{
		//** 26**
		"A.1.16.  STATE-ACCESS_2",
		RFC4465_A_1_16__STATE_ACCESS_2,
		408,
		"0x7465 7374",
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
		11
			
	}
	,
	{
		//** 42**
		"A.2.4.  Stream-based Transport_2",
		RFC4465_A_2_4_Stream_based_Transport_2,
		67,
		"decompression_memory_size",
		11
			
	}
	,
	{
		//** 43**
		"A.2.4.  Stream-based Transport_3",
		RFC4465_A_2_4_Stream_based_Transport_3,
		3,
		"MESSAGE_TOO_SHORT",
		0
	}
	,
	{
		//** 44**
		"A.2.4.  Stream-based Transport_4",
		RFC4465_A_2_4_Stream_based_Transport_4,
		4,
		"MESSAGE_TOO_SHORT",
		0
	}
	,
	{
		//** 45**
		"A.2.4.  Stream-based Transport_5",
		RFC4465_A_2_4_Stream_based_Transport_5,
		30,
		"MESSAGE_TOO_SHORT",
		0
	}
	,
	{
		//** 46**
		"A.2.4.  Stream-based Transport_6",
		RFC4465_A_2_4_Stream_based_Transport_6,
		29,
		"INVALID_CODE_LOCATION",
		0
	}
	,
	{
		//** 47**
		"A.2.5.  Input Past the End of a Message_1",
		RFC4465_A_2_5__Input_Past_the_End_of_a_Message_1,
		59,
		"0x6869 21",
		23
	}
	,
	{
		//** 48**
		"A.2.5.  Input Past the End of a Message_2",
		RFC4465_A_2_5__Input_Past_the_End_of_a_Message_2,
		58,
		"USER_REQUESTED",
		0
	}
	,
	{
		//** 49**
		"A.3.1.  SigComp Feedback Mechanism_1",
		RFC4465_A_3_1__SigComp_Feedback_Mechanism_1,
		84,
		"",
		52
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
		"0x5349 50",
		11
	}
	,
	{
		//** 68**
		"A.3.5.  Bytecode State Creation_1",
		RFC4465_A_3_5__Bytecode_State_Creation_1,
		74,
		"0x4f4b",
		66
	}
	,
	{
		//** 69**
		"A.3.5.  Bytecode State Creation_2",
		RFC4465_A_3_5__Bytecode_State_Creation_2,
		8,
		"0x4f4b 31",
		7
	}
	,
	{
		//** 70**
		"A.3.5.  Bytecode State Creation_3",
		RFC4465_A_3_5__Bytecode_State_Creation_3,
		13,
		"0x4f4b 32",
		5
	}
	,
	{
		//** 71**
		"A.3.5.  Bytecode State Creation_4",
		RFC4465_A_3_5__Bytecode_State_Creation_4,
		7,
		"0x0000 32",
		5
	}
	,
	{
		//** 72**
		"A.3.5.  Bytecode State Creation_5",
		RFC4465_A_3_5__Bytecode_State_Creation_5,
		7,
		"STATE_NOT_FOUND",
		0
	}
};

#include "tcomp_manager.h"

#define OUTPUT_BUFFER_SIZE 2000

#define RUN_TEST_LOOP	0

#define RUN_TEST_ALL	0
#define RUN_TEST_NO		2

int _tmain(int argc, _TCHAR* argv[])
{
	size_t i, start, end;
	char buffer[OUTPUT_BUFFER_SIZE];
	tcomp_manager_handle_t *manager = TCOMP_MANAGER_CREATE();
	tcomp_result_t *result = TCOMP_RESULT_CREATE();

	/* Add SIP dictionary. */
	tcomp_manager_addSipSdpDictionary(manager);

	/* Add Presence dictionary. */
	tcomp_manager_addPresenceDictionary(manager);
	
#if RUN_TEST_LOOP
	for(;;;)
#endif
	{
#if RUN_TEST_ALL
		start = 0, end = 72;
#else
		start = RUN_TEST_NO, end = RUN_TEST_NO + 1;
#endif

		for(i=start; i<end; i++)
		{
			printf("Testing %s\n", tests[i].section_name);

			memset(buffer, 0, OUTPUT_BUFFER_SIZE);
			tcomp_result_setOutputUDPBuffer(result, buffer, OUTPUT_BUFFER_SIZE);

			/* Performs decompression */
			tcomp_manager_decompress(manager, tests[i].bytecode, tests[i].bytecode_size, result);
		}

	}/* LOOP */

	/* Free previously allocated resources. */
	TCOMP_RESULT_SAFE_FREE(result);
	TCOMP_MANAGER_SAFE_FREE(manager);

	return 0;
}

