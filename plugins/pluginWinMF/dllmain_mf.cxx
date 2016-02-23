/* Copyright (C) 2013 Mamadou DIOP
* Copyright (C) 2013 Doubango Telecom <http://www.doubango.org>
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
*/
#include "plugin_win_mf_config.h"
#include "internals/mf_utils.h"

#include "tinymedia/tmedia_producer.h"
#include "tinymedia/tmedia_consumer.h"
#include "tinymedia/tmedia_converter_video.h"

#include "tsk_plugin.h"
#include "tsk_debug.h"

#include <windows.h>

#if defined(_MSC_VER)
#       pragma comment(lib, "mfplat")
#       pragma comment(lib, "mf")
#       pragma comment(lib, "mfuuid")
#       pragma comment(lib, "shlwapi")
#		pragma comment(lib, "Strmiids")
#endif

#if !defined(PLUGIN_MF_ENABLE_AUDIO_IO)
#	define PLUGIN_MF_ENABLE_AUDIO_IO 0 /* audio not good as DirectSound */
#endif
#if !defined(PLUGIN_MF_ENABLE_VIDEO_CONVERTER)
#	define PLUGIN_MF_ENABLE_VIDEO_CONVERTER 1
#endif
#if !defined(PLUGIN_MF_ENABLE_VIDEO_IO)
#	define PLUGIN_MF_ENABLE_VIDEO_IO 1
#endif

extern const tmedia_codec_plugin_def_t *mf_codec_h264_main_plugin_def_t;
extern const tmedia_codec_plugin_def_t *mf_codec_h264_base_plugin_def_t;

#if PLUGIN_MF_ENABLE_VIDEO_CONVERTER
extern const tmedia_converter_video_plugin_def_t *plugin_win_mf_converter_video_ms_plugin_def_t;
#endif
#if PLUGIN_MF_ENABLE_VIDEO_IO
extern const tmedia_producer_plugin_def_t *plugin_win_mf_producer_video_plugin_def_t;
extern const tmedia_consumer_plugin_def_t *plugin_win_mf_consumer_video_plugin_def_t;
#endif
#if PLUGIN_MF_ENABLE_AUDIO_IO
extern const tmedia_producer_plugin_def_t *plugin_win_mf_producer_audio_plugin_def_t;
extern const tmedia_consumer_plugin_def_t *plugin_win_mf_consumer_audio_plugin_def_t;
#endif

PLUGIN_WIN_MF_BEGIN_DECLS /* BEGIN */
PLUGIN_WIN_MFP_API int __plugin_get_def_count();
PLUGIN_WIN_MFP_API tsk_plugin_def_type_t __plugin_get_def_type_at(int index);
PLUGIN_WIN_MFP_API tsk_plugin_def_media_type_t __plugin_get_def_media_type_at(int index);
PLUGIN_WIN_MFP_API tsk_plugin_def_ptr_const_t __plugin_get_def_at(int index);
PLUGIN_WIN_MF_END_DECLS /* END */

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}


typedef enum PLUGIN_INDEX_E {
#if PLUGIN_MF_ENABLE_AUDIO_IO
    PLUGIN_INDEX_AUDIO_CONSUMER,
    PLUGIN_INDEX_AUDIO_PRODUCER,
#endif
#if PLUGIN_MF_ENABLE_VIDEO_IO
    PLUGIN_INDEX_VIDEO_PRODUCER,
    PLUGIN_INDEX_VIDEO_CONSUMER,
#endif
#if PLUGIN_MF_ENABLE_VIDEO_CONVERTER
    PLUGIN_INDEX_VIDEO_CONVERTER,
#endif

    PLUGIN_INDEX_CODEC_H264_MAIN,
    PLUGIN_INDEX_CODEC_H264_BASE,

    PLUGIN_INDEX_COUNT
}
PLUGIN_INDEX_T;


int __plugin_get_def_count()
{
    int count = PLUGIN_INDEX_COUNT;
    if(!MFUtils::IsLowLatencyH264Supported()) {
        count -= 2;
    }
    return count;
}

tsk_plugin_def_type_t __plugin_get_def_type_at(int index)
{
    switch(index) {
#if PLUGIN_MF_ENABLE_AUDIO_IO
    case PLUGIN_INDEX_AUDIO_CONSUMER:
    case PLUGIN_INDEX_AUDIO_PRODUCER: {
        return (index == PLUGIN_INDEX_AUDIO_CONSUMER) ? tsk_plugin_def_type_consumer : tsk_plugin_def_type_producer;
    }
#endif
#if PLUGIN_MF_ENABLE_VIDEO_IO
    case PLUGIN_INDEX_VIDEO_CONSUMER: {
        return MFUtils::IsD3D9Supported() ? tsk_plugin_def_type_consumer : tsk_plugin_def_type_none;
    }
    case PLUGIN_INDEX_VIDEO_PRODUCER: {
        return tsk_plugin_def_type_producer;
    }
#endif
#if PLUGIN_MF_ENABLE_VIDEO_CONVERTER
    case PLUGIN_INDEX_VIDEO_CONVERTER: {
        return tsk_plugin_def_type_converter;
    }
#endif
    case PLUGIN_INDEX_CODEC_H264_MAIN:
    case PLUGIN_INDEX_CODEC_H264_BASE: {
        return MFUtils::IsLowLatencyH264Supported() ? tsk_plugin_def_type_codec : tsk_plugin_def_type_none;
    }
    default: {
        TSK_DEBUG_ERROR("No plugin at index %d", index);
        return tsk_plugin_def_type_none;
    }
    }
}

tsk_plugin_def_media_type_t	__plugin_get_def_media_type_at(int index)
{
    switch(index) {
#if PLUGIN_MF_ENABLE_AUDIO_IO
    case PLUGIN_INDEX_AUDIO_CONSUMER:
    case PLUGIN_INDEX_AUDIO_PRODUCER: {
        return tsk_plugin_def_media_type_audio;
    }
#endif
#if PLUGIN_MF_ENABLE_VIDEO_IO
    case PLUGIN_INDEX_VIDEO_CONSUMER: {
        return MFUtils::IsD3D9Supported() ? tsk_plugin_def_media_type_video : tsk_plugin_def_media_type_none;
    }
    case PLUGIN_INDEX_VIDEO_PRODUCER: {
        return tsk_plugin_def_media_type_video;
    }
#endif
#if PLUGIN_MF_ENABLE_VIDEO_CONVERTER
    case PLUGIN_INDEX_VIDEO_CONVERTER: {
        return tsk_plugin_def_media_type_video;
    }
#endif
    case PLUGIN_INDEX_CODEC_H264_MAIN:
    case PLUGIN_INDEX_CODEC_H264_BASE: {
        return MFUtils::IsLowLatencyH264Supported() ? tsk_plugin_def_media_type_video : tsk_plugin_def_media_type_none;
    }
    default: {
        TSK_DEBUG_ERROR("No plugin at index %d", index);
        return tsk_plugin_def_media_type_none;
    }
    }
}

tsk_plugin_def_ptr_const_t __plugin_get_def_at(int index)
{
    switch(index) {
#if PLUGIN_MF_ENABLE_VIDEO_IO
    case PLUGIN_INDEX_VIDEO_PRODUCER: {
        return plugin_win_mf_producer_video_plugin_def_t;
    }
    case PLUGIN_INDEX_VIDEO_CONSUMER: {
        return MFUtils::IsD3D9Supported() ? plugin_win_mf_consumer_video_plugin_def_t : tsk_null;
    }
#endif
#if PLUGIN_MF_ENABLE_AUDIO_IO
    case PLUGIN_INDEX_AUDIO_PRODUCER: {
        return plugin_win_mf_producer_audio_plugin_def_t;
    }
    case PLUGIN_INDEX_AUDIO_CONSUMER: {
        return plugin_win_mf_consumer_audio_plugin_def_t;
    }
#endif
#if PLUGIN_MF_ENABLE_VIDEO_CONVERTER
    case PLUGIN_INDEX_VIDEO_CONVERTER: {
        return plugin_win_mf_converter_video_ms_plugin_def_t;
    }
#endif
    case PLUGIN_INDEX_CODEC_H264_MAIN: {
        return MFUtils::IsLowLatencyH264Supported() ? mf_codec_h264_main_plugin_def_t : tsk_null;
    }
    case PLUGIN_INDEX_CODEC_H264_BASE: {
        return MFUtils::IsLowLatencyH264Supported() ? mf_codec_h264_base_plugin_def_t : tsk_null;
    }
    default: {
        TSK_DEBUG_ERROR("No plugin at index %d", index);
        return tsk_null;
    }
    }
}
