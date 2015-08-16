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
#ifndef _TEST_SMSPACKING_H
#define _TEST_SMSPACKING_H

typedef struct test_message_7bit_s{
	const char* ascii;
	const char* _7bit;
}
test_message_7bit_t;

test_message_7bit_t test_messages_7bit[] = 
{
	"a","\x61",
	"ab","\x61\x31",
	"abc","\x61\xF1\x18",
	"abcdefg","\x61\xF1\x98\x5C\x36\x9F\x01",
	"abcdefgh","\x61\xF1\x98\x5C\x36\x9F\xD1",
	"abcdefghi","\x61\xF1\x98\x5C\x36\x9F\xD1\x69",
	"abcdefgh12345678","\x61\xF1\x98\x5C\x36\x9F\xD1\x31\xD9\x8C\x56\xB3\xDD\x70",
	"abcdefgh123456789","\x61\xF1\x98\x5C\x36\x9F\xD1\x31\xD9\x8C\x56\xB3\xDD\x70\x39",
	"This is a test sms message","\x54\x74\x7A\x0E\x4A\xCF\x41\x61\x10\xBD\x3C\xA7\x83\xE6\xED\x39\xA8\x5D\x9E\xCF\xC3\xE7\x32",
	"salut","\xF3\x30\xBB\x4E\x07",
	"hellohello","\xE8\x32\x9B\xFD\x46\x97\xD9\xEC\x37",
};

void test_7bit()
{
	tsk_size_t i;
	tsk_buffer_t* buffer;
	char* temp;

	/* To 7bit */
	for(i=0; i<sizeof(test_messages_7bit)/sizeof(test_message_7bit_t); i++){
		if((buffer = tsms_pack_to_7bit(test_messages_7bit[i].ascii))){
			/*if((temp = tohex(buffer->data, buffer->size))){
				if(!tsk_striequals(temp, test_messages_7bit[i]._7bit)){
					TSK_DEBUG_ERROR("tsms_pack_to_7bit(\"%s\") Failed.\n", test_messages_7bit[i].ascii);
				}
				TSK_FREE(temp);
			}*/
			if(!bin_equals(buffer->data, test_messages_7bit[i]._7bit, buffer->size)){
				TSK_DEBUG_ERROR("tsms_pack_to_7bit(\"%s\") Failed.\n", test_messages_7bit[i].ascii);
			}
			TSK_OBJECT_SAFE_FREE(buffer);
		}
	}

	/* From 7bit */
	for(i=0; i<sizeof(test_messages_7bit)/sizeof(test_message_7bit_t); i++){
		if((temp = tsms_pack_from_7bit(test_messages_7bit[i]._7bit, (tsk_size_t)tsk_strlen(test_messages_7bit[i]._7bit)))){
			if(!tsk_striequals(temp, test_messages_7bit[i].ascii)){
				TSK_DEBUG_ERROR("tsms_pack_from_7bit(\"%s\") Failed.\n", test_messages_7bit[i].ascii);
			}
			TSK_FREE(temp);
		}
	}
}

void test_8bit()
{
	tsk_size_t i;
	tsk_buffer_t* buffer;
	char* temp;
	
	/* To 8bit */
	for(i=0; i<sizeof(test_messages_7bit)/sizeof(test_message_7bit_t); i++){
		if((buffer = tsms_pack_to_8bit(test_messages_7bit[i].ascii))){
			if(!tsk_strequals(buffer->data, test_messages_7bit[i].ascii)){
				TSK_DEBUG_INFO("tsms_pack_to_8bit(%s) Failed", test_messages_7bit[i].ascii);
			}
			TSK_OBJECT_SAFE_FREE(buffer);
		}
	}

	/* From 8bit */
	for(i=0; i<sizeof(test_messages_7bit)/sizeof(test_message_7bit_t); i++){
		if((temp = tsms_pack_from_8bit(test_messages_7bit[i].ascii, (tsk_size_t)tsk_strlen(test_messages_7bit[i].ascii)))){
			if(!tsk_strequals(temp, test_messages_7bit[i].ascii)){
				TSK_DEBUG_INFO("tsms_pack_from_8bit(%s) Failed", test_messages_7bit[i].ascii);
			}
			TSK_FREE(temp);
		}
	}
}

void test_ucs2()
{
	tsk_size_t i;
	tsk_buffer_t* buffer;
	char* temp;
	
	/* To ucs2 */
	for(i=0; i<sizeof(test_messages_7bit)/sizeof(test_message_7bit_t); i++){
		if((buffer = tsms_pack_to_ucs2(test_messages_7bit[i].ascii))){
			if(!tsk_strequals(buffer->data, test_messages_7bit[i].ascii)){
				TSK_DEBUG_INFO("tsms_pack_to_ucs2(%s) Failed", test_messages_7bit[i].ascii);
			}
			TSK_OBJECT_SAFE_FREE(buffer);
		}
	}

	/* From ucs2 */
	for(i=0; i<sizeof(test_messages_7bit)/sizeof(test_message_7bit_t); i++){
		if((temp = tsms_pack_from_ucs2(test_messages_7bit[i].ascii, (tsk_size_t)tsk_strlen(test_messages_7bit[i].ascii)))){
			if(!tsk_strequals(temp, test_messages_7bit[i].ascii)){
				TSK_DEBUG_INFO("tsms_pack_from_ucs2(%s) Failed", test_messages_7bit[i].ascii);
			}
			TSK_FREE(temp);
		}
	}
}

void test_packing()
{
	test_ucs2();
	test_8bit();
	test_7bit();
}

#endif /* _TEST_SMSPACKING_H */
