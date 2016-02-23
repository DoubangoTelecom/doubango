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
#include "plugin_audio_dsp_utils.h"

#include "tinymedia/tmedia_resampler.h"
#include "tinymedia/tmedia_denoise.h"

#include "tsk_plugin.h"
#include "tsk_debug.h"

#include <windows.h>

#if defined(_MSC_VER)
#	pragma comment(lib, "wmcodecdspuuid")
#	pragma comment(lib, "Msdmo")
#	pragma comment(lib, "Dmoguids")
#	pragma comment(lib, "amstrmid")
#	pragma comment(lib, "Mfplat")
#	pragma comment(lib, "mfuuid")
#endif

#if !defined(PLUGIN_AUDIO_DSP_RESAMPLER_ENABLE)
#	define PLUGIN_AUDIO_DSP_RESAMPLER_ENABLE 1
#endif
#if !defined(PLUGIN_AUDIO_DSP_DENOISER_ENABLE)
#	define PLUGIN_AUDIO_DSP_DENOISER_ENABLE 0 /* Filter mode doesn't support AEC */
#endif

extern const tmedia_resampler_plugin_def_t *plugin_audio_dsp_resampler_plugin_def_t;
extern const tmedia_denoise_plugin_def_t *plugin_audio_dsp_denoise_plugin_def_t;

PLUGIN_AUDIO_DSP_BEGIN_DECLS /* BEGIN */
PLUGIN_AUDIO_DSP_API int __plugin_get_def_count();
PLUGIN_AUDIO_DSP_API tsk_plugin_def_type_t __plugin_get_def_type_at(int index);
PLUGIN_AUDIO_DSP_API tsk_plugin_def_media_type_t __plugin_get_def_media_type_at(int index);
PLUGIN_AUDIO_DSP_API tsk_plugin_def_ptr_const_t __plugin_get_def_at(int index);
PLUGIN_AUDIO_DSP_END_DECLS /* END */

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
#if PLUGIN_AUDIO_DSP_RESAMPLER_ENABLE
    PLUGIN_INDEX_RESAMPLER,
#endif
#if PLUGIN_AUDIO_DSP_DENOISER_ENABLE
    PLUGIN_INDEX_DENOISER,
#endif

    PLUGIN_INDEX_COUNT
}
PLUGIN_INDEX_T;


int __plugin_get_def_count()
{
    return PLUGIN_INDEX_COUNT;
}

tsk_plugin_def_type_t __plugin_get_def_type_at(int index)
{
    switch(index) {
#if PLUGIN_AUDIO_DSP_RESAMPLER_ENABLE
    case PLUGIN_INDEX_RESAMPLER: {
        return tsk_plugin_def_type_resampler;
    }
#endif
#if PLUGIN_AUDIO_DSP_DENOISER_ENABLE
    case PLUGIN_INDEX_DENOISER: {
        return tsk_plugin_def_type_denoiser;
    }
#endif
    }
    TSK_DEBUG_ERROR("No plugin at index %d", index);
    return tsk_plugin_def_type_none;
}

tsk_plugin_def_media_type_t	__plugin_get_def_media_type_at(int index)
{
    switch(index) {
#if PLUGIN_AUDIO_DSP_RESAMPLER_ENABLE
    case PLUGIN_INDEX_RESAMPLER: {
        return tsk_plugin_def_media_type_audio;
    }
#endif
#if PLUGIN_AUDIO_DSP_DENOISER_ENABLE
    case PLUGIN_INDEX_DENOISER: {
        return tsk_plugin_def_media_type_audio;
    }
#endif
    }

    TSK_DEBUG_ERROR("No plugin at index %d", index);
    return tsk_plugin_def_media_type_none;
}

tsk_plugin_def_ptr_const_t __plugin_get_def_at(int index)
{
    switch(index) {
#if PLUGIN_AUDIO_DSP_RESAMPLER_ENABLE
    case PLUGIN_INDEX_RESAMPLER: {
        return plugin_audio_dsp_resampler_plugin_def_t;
    }
#endif
#if PLUGIN_AUDIO_DSP_DENOISER_ENABLE
    case PLUGIN_INDEX_DENOISER: {
        return plugin_audio_dsp_denoise_plugin_def_t;
    }
#endif
    }

    TSK_DEBUG_ERROR("No plugin at index %d", index);
    return tsk_null;
}
