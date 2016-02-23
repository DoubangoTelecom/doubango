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
#ifndef _TEST_MD5_H_
#define _TEST_MD5_H_

struct md5_result {
    const char* msg;
    const char* xres;
};

struct md5_result msgs_md5[] = {
    { "x3JJHMbDL1EzLkh9GBhXDw==258EAFA5-E914-47DA-95CA-C5AB0DC85B11", "f3d129cf0d52088287dee3e6057fa18d" },
    { "", "d41d8cd98f00b204e9800998ecf8427e" },
    { "a", "0cc175b9c0f1b6a831c399e269772661" },
    { "abc", "900150983cd24fb0d6963f7d28e17f72" },
    { "message digest", "f96b697d7cb7938d525a2f31aaf161d0" },
    { "abcdefghijklmnopqrstuvwxyz", "c3fcd3d76192e4007dfb496cca67e13b" },
    { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", "d174ab98d277d9f5a5611c2c9f419d9f" },
    { "12345678901234567890123456789012345678901234567890123456789012345678901234567890", "57edf4a22be3c955ac49da2e2107b67a" },
    { "The quick brown fox jumps over the lazy dog", "9e107d9d372bb6826bd81d3542a419d6" },
    { "The quick brown fox jumps over the lazy dog.", "e4d909c290d0fb1ca068ffaddf22cbd0" },
};

void test_md5()
{
    size_t i;
    tsk_md5string_t md5result;

    for(i=0; i< sizeof(msgs_md5)/sizeof(struct md5_result); i++) {
        tsk_md5compute(msgs_md5[i].msg, strlen(msgs_md5[i].msg), &md5result);
        if(tsk_striequals(msgs_md5[i].xres, md5result)) {
            TSK_DEBUG_INFO("[MD5-%d] ==> OK", i);
        }
        else {
            TSK_DEBUG_INFO("[MD5-%d] ==> NOK", i);
        }
    }
}

struct hmac_md5_result {
    const char* msg;
    const char* key;
    const char* xres;
};
struct hmac_md5_result msgs_hmac_md5[] = {
    { "Hi There", "\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b", "9294727a3638bb1c13f48ef8158bfc9d" },
    { "what do ya want for nothing?", "Jefe", "750c783e6ab0b503eaa86e310a5db738" },
    { "\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd", "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa", "56be34521d144c88dbb8c733f0e8b3f6" },
    { "\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd", "\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19", "697eaf0aca3a3aea3a75164746ffaa79" },
    { "Test With Truncation", "\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c", "56461ef2342edc00f9bab995690efd4c" },
    { "Test Using Larger Than Block-Size Key - Hash Key First", "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa", "6b1ab7fe4bd7bf8f0b62e6ce61b9d0cd" },
    { "Test Using Larger Than Block-Size Key and Larger Than One Block-Size Data", "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa", "6f630fad67cda0ee1fb1f562db3aa53e" },
};

void test_hmac_md5()
{
    size_t i;
    tsk_md5string_t md5result;

    for(i=0; i< sizeof(msgs_hmac_md5)/sizeof(struct hmac_md5_result); i++) {
        hmac_md5_compute((const uint8_t*)msgs_hmac_md5[i].msg, strlen(msgs_hmac_md5[i].msg), msgs_hmac_md5[i].key, strlen(msgs_hmac_md5[i].key), &md5result);
        if(tsk_striequals(msgs_hmac_md5[i].xres, md5result)) {
            TSK_DEBUG_INFO("[HMAC-MD5-%d] ==> OK", i);
        }
        else {
            TSK_DEBUG_INFO("[HMAC-MD5-%d] ==> NOK", i);
        }
    }
}


#endif /* _TEST_MD5_H_ */
