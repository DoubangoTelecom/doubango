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
#include "tinymedia/tmedia.h"
//#include "dummy.h"

#include "tinymedia.h"

#include "test_codecs.h"
#include "test_sessions.h"
#include "test_image_attr.h"
#include "test_qos.h"
#include "test_contents.h"

#define RUN_TEST_LOOP		1

#define RUN_TEST_ALL		0
#define RUN_TEST_CODECS		0
#define RUN_TEST_SESSIONS	0
#define RUN_TEST_QOS		0
#define RUN_TEST_IMAGEATTR	1
#define RUN_TEST_CONTENTS	0


static void test_register_dummy_plugins();
static void test_register_contents_plugins();


#ifdef _WIN32_WCE
int _tmain(int argc, _TCHAR* argv[])
#else
int main()
#endif
{
    /* Register dummy plugins */
    test_register_dummy_plugins();
    /* Register content plugins */
    test_register_contents_plugins();

    do {

#if RUN_TEST_ALL  || RUN_TEST_CODECS
        test_codecs();
#endif

#if RUN_TEST_ALL  || RUN_TEST_SESSIONS
        test_sessions();
#endif

#if RUN_TEST_ALL  || RUN_TEST_QOS
        test_qos();
#endif

#if RUN_TEST_ALL  || RUN_TEST_IMAGEATTR
        test_qos_imageattr();
#endif

#if RUN_TEST_ALL  || RUN_TEST_CONTENTS
        test_contents();
#endif

    }
    while(RUN_TEST_LOOP);

    return 0;
}


void test_register_contents_plugins()
{
    tmedia_content_plugin_register("text/html", tmedia_content_dummy_plugin_def_t);
    tmedia_content_plugin_register("text/plain", tmedia_content_dummy_plugin_def_t);
    tmedia_content_plugin_register("message/CPIM", tmedia_content_cpim_plugin_def_t);
}

void test_register_dummy_plugins()
{
    int ret;

    /* === Sessions === */
    if((ret = tmedia_session_plugin_register(tmedia_session_daudio_plugin_def_t))) {
        TSK_DEBUG_ERROR("Failed to register audio plugin");
    }
    if((ret = tmedia_session_plugin_register(tmedia_session_dvideo_plugin_def_t))) {
        TSK_DEBUG_ERROR("Failed to register video plugin");
    }
    if((ret = tmedia_session_plugin_register(tmedia_session_dmsrp_plugin_def_t))) {
        TSK_DEBUG_ERROR("Failed to register msrp plugin");
    }
    if((ret = tmedia_session_plugin_register(tmedia_session_ghost_plugin_def_t))) {
        TSK_DEBUG_ERROR("Failed to register ghost plugin");
    }

    /* === Codecs === */
    if((ret = tmedia_codec_plugin_register(tmedia_codec_dpcma_plugin_def_t))) {
        TSK_DEBUG_ERROR("Failed to register G.711a plugin");
    }
    if((ret = tmedia_codec_plugin_register(tmedia_codec_dpcmu_plugin_def_t))) {
        TSK_DEBUG_ERROR("Failed to register G.711u plugin");
    }
    if((ret = tmedia_codec_plugin_register(tmedia_codec_dh263_plugin_def_t))) {
        TSK_DEBUG_ERROR("Failed to register H.263-1996 plugin");
    }
    if((ret = tmedia_codec_plugin_register(tmedia_codec_dh264_plugin_def_t))) {
        TSK_DEBUG_ERROR("Failed to register H.264 (Base profile 10) plugin");
    }
}

//#ifdef _WIN32_WCE
//int _tmain(int argc, _TCHAR* argv[])
//#else
//int main()
//#endif
//{
//	while(1)
//	{
//		tmedia_t* dummy = tsk_null;
//
//		// Register dummy media
//		tmedia_plugin_register(dummy_plugin_def_t);
//		// ...if you have another one to register
//		// ...and another
//		// ...again and again
//
//		// Create dummy media
//		if((dummy = tmedia_factory_create("dummy plugin", "127.0.0.1", tnet_socket_type_udp_ipv4))){
//
//			tmedia_get_local_offer(dummy,
//				TSDP_HEADER_A_VA_ARGS("file-disposition", "attachment"),
//
//				tsk_null
//				);
//			tmedia_get_negotiated_offer(dummy);
//			tmedia_set_remote_offer(dummy, tsk_null);
//
//			tmedia_start(dummy);
//			tmedia_pause(dummy);
//
//			tmedia_perform(dummy, tma_dummy_say_hello,
//				TSK_PARAM_VA_ARGS("to", "doubango"),
//
//				tsk_null);
//
//			tmedia_stop(dummy);
//
//			TSK_OBJECT_SAFE_FREE(dummy);
//		}
//	}
//
//	return 0;
//}

