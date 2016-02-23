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
#ifndef _TEST_CONTENTS_H_
#define _TEST_CONTENTS_H_

static void test_content_dummy();
static void test_content_text_plain();
static void test_content_cpim();

static void test_contents()
{
    test_content_dummy();
    test_content_text_plain();
    test_content_cpim();
}


static void test_content_dummy()
{
#define CONTENT_DUMMY_DATA "salut"
#define CONTENT_DUMMY_TYPE "cool/ok"
    tmedia_content_t* content = tmedia_content_parse(CONTENT_DUMMY_DATA, tsk_strlen(CONTENT_DUMMY_DATA), CONTENT_DUMMY_TYPE);
    if(content) {
        tsk_buffer_t* data = tmedia_content_get_data(content);
        TSK_DEBUG_INFO("content-type=%s\n\ncontent=%s", TMEDIA_CONTENT(content)->type, TSK_BUFFER_DATA(data));
        tsk_object_unref(data);
    }

    TSK_OBJECT_SAFE_FREE(content);
}

static void test_content_text_plain()
{
#define CONTENT_TEXT_PLAIN_DATA "salut comment tu vas?"
#define CONTENT_TEXT_PLAIN_TYPE "text/plain"
    tmedia_content_t* content = tmedia_content_parse(CONTENT_TEXT_PLAIN_DATA, tsk_strlen(CONTENT_TEXT_PLAIN_DATA), CONTENT_TEXT_PLAIN_TYPE);
    if(content) {
        tsk_buffer_t* data = tmedia_content_get_data(content);
        TSK_DEBUG_INFO("content-type=%s\n\ncontent=%s", TMEDIA_CONTENT(content)->type, TSK_BUFFER_DATA(data));
        tsk_object_unref(data);
    }

    TSK_OBJECT_SAFE_FREE(content);
}

static void test_content_cpim()
{
#define CONTENT_CPIM_DATA "To: <sip:test@doubango.org>\r\n" \
"From: <sip:test@doubango.org>\r\n" \
"DateTime: 2010-12-17T09:57:32.562Z\r\n" \
"Content-Disposition: attachment; filename=\"history.xml\"; creation-date=\"2010-12-17T09:19:56.978Z\"; size=3714\r\n" \
"\r\n" \
"Content-Type: application/octet-stream\r\n" \
"Content-ID: <1234567890@doubango.org>\r\n" \
"\r\n" \
"salut comment tu vas?\r\n"

#define CONTENT_CPIM_TYPE "message/CPIM"
    tmedia_content_t* content = tmedia_content_parse(CONTENT_CPIM_DATA, tsk_strlen(CONTENT_CPIM_DATA), CONTENT_CPIM_TYPE);
    if(content) {
        tsk_buffer_t* data = tmedia_content_get_data(content);
        TSK_DEBUG_INFO("content-type=%s\n\ncontent=%s", TMEDIA_CONTENT(content)->type, TSK_BUFFER_DATA(data));
        tsk_object_unref(data);
    }

    TSK_OBJECT_SAFE_FREE(content);
}

#endif /* _TEST_CONTENTS_H_ */

