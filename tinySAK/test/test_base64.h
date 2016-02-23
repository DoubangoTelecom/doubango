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
#ifndef _TEST_BASE64_H_
#define _TEST_BASE64_H_

struct b64_msg {
    const char* ascii;
    const char* base64;
};

struct b64_msg b64_msgs[] = {
    { "", "" },
    { "f", "Zg==" },
    { "fo", "Zm8=" },
    { "foo", "Zm9v" },
    { "foob", "Zm9vYg==" },
    { "fooba", "Zm9vYmE=" },
    { "foobar", "Zm9vYmFy" },
    { "Aladdin:open sesame", "QWxhZGRpbjpvcGVuIHNlc2FtZQ=="},
    { "Doubango Project", "RG91YmFuZ28gUHJvamVjdA=="},
    { "Open Source Doubango Framework", "T3BlbiBTb3VyY2UgRG91YmFuZ28gRnJhbWV3b3Jr"},
    {
        "* Copyright (C) 2009 Mamadou Diop."
        "*"
        "* Contact: Mamadou Diop <diopmamadou(at)doubango.org>"
        "*"
        "* This file is part of Open Source Doubango Framework."
        "*"
        "* DOUBANGO is free software: you can redistribute it and/or modify"
        "* it under the terms of the GNU General Public License as published by"
        "* the Free Software Foundation, either version 3 of the License, or"
        "* (at your option) any later version."
        "*"
        "* DOUBANGO is distributed in the hope that it will be useful,"
        "* but WITHOUT ANY WARRANTY; without even the implied warranty of"
        "* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the"
        "* GNU General Public License for more details."
        "*"
        "* You should have received a copy of the GNU General Public License"
        "* along with DOUBANGO."
        ,
        "KiBDb3B5cmlnaHQgKEMpIDIwMDkgTWFtYWRvdSBEaW9wLioqIENvbnRhY3Q6IE1hbW"
        "Fkb3UgRGlvcCA8ZGlvcG1hbWFkb3UoYXQpZG91YmFuZ28ub3JnPioqIFRoaXMgZmls"
        "ZSBpcyBwYXJ0IG9mIE9wZW4gU291cmNlIERvdWJhbmdvIEZyYW1ld29yay4qKiBET1"
        "VCQU5HTyBpcyBmcmVlIHNvZnR3YXJlOiB5b3UgY2FuIHJlZGlzdHJpYnV0ZSBpdCBh"
        "bmQvb3IgbW9kaWZ5KiBpdCB1bmRlciB0aGUgdGVybXMgb2YgdGhlIEdOVSBHZW5lcm"
        "FsIFB1YmxpYyBMaWNlbnNlIGFzIHB1Ymxpc2hlZCBieSogdGhlIEZyZWUgU29mdHdh"
        "cmUgRm91bmRhdGlvbiwgZWl0aGVyIHZlcnNpb24gMyBvZiB0aGUgTGljZW5zZSwgb3"
        "IqIChhdCB5b3VyIG9wdGlvbikgYW55IGxhdGVyIHZlcnNpb24uKiogRE9VQkFOR08g"
        "aXMgZGlzdHJpYnV0ZWQgaW4gdGhlIGhvcGUgdGhhdCBpdCB3aWxsIGJlIHVzZWZ1bC"
        "wqIGJ1dCBXSVRIT1VUIEFOWSBXQVJSQU5UWTsgd2l0aG91dCBldmVuIHRoZSBpbXBs"
        "aWVkIHdhcnJhbnR5IG9mKiBNRVJDSEFOVEFCSUxJVFkgb3IgRklUTkVTUyBGT1IgQS"
        "BQQVJUSUNVTEFSIFBVUlBPU0UuICBTZWUgdGhlKiBHTlUgR2VuZXJhbCBQdWJsaWMg"
        "TGljZW5zZSBmb3IgbW9yZSBkZXRhaWxzLioqIFlvdSBzaG91bGQgaGF2ZSByZWNlaX"
        "ZlZCBhIGNvcHkgb2YgdGhlIEdOVSBHZW5lcmFsIFB1YmxpYyBMaWNlbnNlKiBhbG9u"
        "ZyB3aXRoIERPVUJBTkdPLg=="
    }
};

void test_base64()
{
    size_t i, size;
    char *output_e = 0, *output_d = 0;

    for(i=0; i<sizeof(b64_msgs)/sizeof(struct b64_msg); i++) {
        /*===========
        *	Encoding
        */
        size = tsk_base64_encode((const uint8_t*)b64_msgs[i].ascii, strlen(b64_msgs[i].ascii), &output_e);
        if(tsk_striequals(b64_msgs[i].base64, output_e)) {
            TSK_DEBUG_INFO("[BASE64-%d encoding] ==> OK", i);
        }
        else {
            TSK_DEBUG_INFO("[BASE64-%d encoding] ==> NOK", i);
        }
        TSK_FREE(output_e);

        /*===========
        *	Decoding
        */
        size = tsk_base64_decode((const uint8_t*)b64_msgs[i].base64, strlen(b64_msgs[i].base64), &output_d);
        if(tsk_striequals(b64_msgs[i].ascii, output_d)) {
            TSK_DEBUG_INFO("[BASE64-%d decoding] ==> OK", i);
        }
        else {
            TSK_DEBUG_INFO("[BASE64-%d decoding] ==> NOK", i);
        }
        TSK_FREE(output_d);
    }
}

#endif /* _TEST_BASE64_H_ */
