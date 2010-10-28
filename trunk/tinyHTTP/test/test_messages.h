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

#define TEST_MSG TEST_MSG_401


void test_messages()
{
	thttp_message_t *message = tsk_null;
	tsk_ragel_state_t state;
	int ret;
	uint8_t c = 'à';
	
	/* deserialize the message */
	tsk_ragel_state_init(&state, TEST_MSG, strlen(TEST_MSG));
	if(!(ret = thttp_message_parse(&state, &message, tsk_true))){
		tsk_buffer_t *buffer = tsk_buffer_create_null();

		/* serialize the message */
		thttp_message_serialize(message, buffer);
		TSK_DEBUG_INFO("HTTP Message=\n%s", TSK_BUFFER_TO_STRING(buffer));

		TSK_OBJECT_SAFE_FREE(buffer);
	}
	else{
		TSK_DEBUG_ERROR("Failed to parse HTTP message.");
	}

	TSK_OBJECT_SAFE_FREE(message);

	getchar();
}

#endif /* _TEST_MESSAGES_H_ */

