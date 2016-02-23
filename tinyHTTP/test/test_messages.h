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
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/
#ifndef _TEST_MESSAGES_H_
#define _TEST_MESSAGES_H_

#define TEST_MSG_200 \
	"HTTP/1.1 200 OK\r\n" \
	"Date: Wed, 10 Mar 2010 13:37:13 GMT\r\n" \
	"Server: Apache\r\n" \
	"Content-length: 3\r\n" \
	"Expires: Sat, 07 Aug 2010 13:37:13 +0000\r\n" \
	"Cache-Control: public\r\n" \
	"X-Deezer-Cache: HIT\r\n" \
	"Pragma: \r\n" \
	"Keep-Alive: timeout=2, max=100\r\n" \
	"Connection: Keep-Alive\r\n" \
	"Content-Type: video/flv\r\n" \
	"\r\n" \
	"123"

#define TEST_MSG_401 \
	"HTTP/1.1 401 Unauthorized\r\n" \
	"Server: Apache-Coyote/1.1\r\n" \
	"WWW-Authenticate: Digest realm=\"example.com\", qop=\"auth\", nonce=\"f39d20a4dbfbd32d943de9b285d59a30\", opaque=\"c5ad02feb52eb050e464cec3740c0f2e\"\r\n" \
	"WWW-Authenticate: Basic realm=\"WallyWorld\"\r\n" \
	"Transfer-Encoding: chunked;test=21;tes7\r\n" \
	"Date: Wed, 10 Mar 2010 14:20:47 GMT\r\n" \
	"Etag: W/\"1231-3213213\"\r\n" \
	"\r\n"

#define TEST_MSG_WS \
	"Upgrade: websocket\r\n" \
	"Connection: Upgrade\r\n" \
	"Host: 192.168.0.11:5060\r\n" \
	"Origin: null\r\n" \
	"Sec-WebSocket-Key: Z8DmZQ9jZqK7vJqXbsDlYw==\r\n" \
	"Sec-WebSocket-Version: 13, 5, 6\r\n" \
	"Sec-WebSocket-Accept: s3pPLMBiTxaQ9kYGzzhZRbK+xOo=\r\n" \
	"\r\n"

#define TEST_MSG_OPTIONS \
	"OPTIONS /85 HTTP/1.1\r\n" \
	"Host: 192.168.0.37:10063\r\n" \
	"Connection: keep-alive\r\n" \
	"Access-Control-Request-Method: POST\r\n" \
	"Origin: null\r\n" \
	"User-Agent: Mozilla/5.0 (Windows NT 6.0) AppleWebKit/537.17 (KHTML, like Gecko) Chrome/24.0.1312.57 Safari/537.17\r\n" \
	"Access-Control-Request-Headers: connection, origin, content-type\r\n" \
	"Accept: */*\r\n" \
	"Accept-Encoding: gzip,deflate,sdch\r\n" \
	"Accept-Language: en-US,en;q=0.8\r\n" \
	"Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.3\r\n" \
	"\r\n"

#define TEST_MSG TEST_MSG_OPTIONS


static void test_messages()
{
    thttp_message_t *message = tsk_null;
    tsk_ragel_state_t state;
    int ret/*, idx*/;

    const char* msg_start = TEST_MSG_WS;
    const char* msg_end = msg_start + tsk_strlen(msg_start);
    //const thttp_header_Sec_WebSocket_Key_t* ws_hdr_key;
    //const thttp_header_Sec_WebSocket_Version_t* ws_hdr_version;
    //const thttp_header_Sec_WebSocket_Accept_t* ws_hdr_accept;
    message = thttp_message_create();

    /*while(msg_start < msg_end){
    	if((idx = tsk_strindexOf(msg_start, (msg_end - msg_start), "\r\n")) <= 2){
    		break;
    	}
    	idx+= 2;
    	tsk_ragel_state_init(&state, msg_start, idx);
    	if((ret = thttp_header_parse(&state, message))){
    		break;
    	}
    	msg_start += idx;
    }

    if((ws_hdr_key = (const thttp_header_Sec_WebSocket_Key_t*)thttp_message_get_header(message, thttp_htype_Sec_WebSocket_Key))){
    	TSK_DEBUG_INFO("Sec-WebSocket-Key: %s", ws_hdr_key->value);
    }

    if((ws_hdr_accept = (const thttp_header_Sec_WebSocket_Accept_t*)thttp_message_get_header(message, thttp_htype_Sec_WebSocket_Accept))){
    	TSK_DEBUG_INFO("Sec-WebSocket-Accept: %s", ws_hdr_accept->value);
    }

    if((ws_hdr_version = (const thttp_header_Sec_WebSocket_Version_t*)thttp_message_get_header(message, thttp_htype_Sec_WebSocket_Version))){
    	const tsk_list_item_t* item;
    	tsk_list_foreach(item, ws_hdr_version->values){
    		TSK_DEBUG_INFO("Sec-WebSocket-Version: %s", TSK_STRING_STR(item->data));
    	}
    }*/


    /* deserialize the message */
    tsk_ragel_state_init(&state, TEST_MSG, strlen(TEST_MSG));
    if(!(ret = thttp_message_parse(&state, &message, tsk_true))) {
        tsk_buffer_t *buffer = tsk_buffer_create_null();

        /* serialize the message */
        thttp_message_serialize(message, buffer);
        TSK_DEBUG_INFO("HTTP Message=\n%s", TSK_BUFFER_TO_STRING(buffer));

        TSK_OBJECT_SAFE_FREE(buffer);
    }
    else {
        TSK_DEBUG_ERROR("Failed to parse HTTP message.");
    }

    TSK_OBJECT_SAFE_FREE(message);

    getchar();
}

#endif /* _TEST_MESSAGES_H_ */

