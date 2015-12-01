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
#include "plugin_cuda_config.h"
#include "plugin_cuda_utils.h"

#include "tinymedia/tmedia_codec.h"

#include "tsk_plugin.h"
#include "tsk_debug.h"

#include <windows.h>

#if defined(_MSC_VER)
#       pragma comment(lib, "nvcuvenc")
#       pragma comment(lib, "nvcuvid")
#       pragma comment(lib, "cuda")
#       pragma comment(lib, "cudart")

#       pragma comment(lib, "d3d9")
#       pragma comment(lib, "d3dx9")
#endif


#if !defined(PLUGIN_CUDA_H264_ENABLE)
#	define PLUGIN_CUDA_H264_ENABLE 1
#endif

extern const tmedia_codec_plugin_def_t *cuda_codec_h264_main_plugin_def_t;
extern const tmedia_codec_plugin_def_t *cuda_codec_h264_base_plugin_def_t;

PLUGIN_CUDA_BEGIN_DECLS /* BEGIN */
PLUGIN_CUDA_API int __plugin_get_def_count();
PLUGIN_CUDA_API tsk_plugin_def_type_t __plugin_get_def_type_at(int index);
PLUGIN_CUDA_API tsk_plugin_def_media_type_t __plugin_get_def_media_type_at(int index);
PLUGIN_CUDA_API tsk_plugin_def_ptr_const_t __plugin_get_def_at(int index);
PLUGIN_CUDA_END_DECLS /* END */

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
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


typedef enum PLUGIN_INDEX_E
{
#if PLUGIN_CUDA_H264_ENABLE
	PLUGIN_INDEX_CODEC_H264_MAIN,
	PLUGIN_INDEX_CODEC_H264_BASE,
#endif
	
	PLUGIN_INDEX_COUNT
}
PLUGIN_INDEX_T;


int __plugin_get_def_count()
{
	return CudaUtils::IsH264Supported() ? PLUGIN_INDEX_COUNT : 0;
}

tsk_plugin_def_type_t __plugin_get_def_type_at(int index)
{
#if PLUGIN_CUDA_H264_ENABLE
	switch(index){
		case PLUGIN_INDEX_CODEC_H264_MAIN:
		case PLUGIN_INDEX_CODEC_H264_BASE:
			{
				return CudaUtils::IsH264Supported() ? tsk_plugin_def_type_codec : tsk_plugin_def_type_none;
			}
	}
#endif
	TSK_DEBUG_ERROR("No plugin at index %d", index);
	return tsk_plugin_def_type_none;
}

tsk_plugin_def_media_type_t	__plugin_get_def_media_type_at(int index)
{
#if PLUGIN_CUDA_H264_ENABLE
	switch(index){
		case PLUGIN_INDEX_CODEC_H264_MAIN: 
		case PLUGIN_INDEX_CODEC_H264_BASE:
			{
				return CudaUtils::IsH264Supported() ? tsk_plugin_def_media_type_video : tsk_plugin_def_media_type_none;
			}
	}
#endif
	TSK_DEBUG_ERROR("No plugin at index %d", index);
	return tsk_plugin_def_media_type_none;
}

tsk_plugin_def_ptr_const_t __plugin_get_def_at(int index)
{
#if PLUGIN_CUDA_H264_ENABLE
	switch(index){
		case PLUGIN_INDEX_CODEC_H264_MAIN: 
			{
				return CudaUtils::IsH264Supported() ? cuda_codec_h264_main_plugin_def_t : tsk_null;
			}
		case PLUGIN_INDEX_CODEC_H264_BASE: 
			{
				return CudaUtils::IsH264Supported() ? cuda_codec_h264_base_plugin_def_t : tsk_null;
			}
	}
#endif
	TSK_DEBUG_ERROR("No plugin at index %d", index);
	return tsk_null;
}
