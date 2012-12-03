/*
* Copyright (C) 2011 Doubango Telecom <http://www.doubango.org>.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango(DOT)org>
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

/**@file tdav_codec_h264_cuda.h
 * @brief H.264 codec plugin using NVIDIA CUDA for encoding/decoding.
 * Env: gpucomputingsdk_4.0.17_win_32, cudatoolkit_4.0.17_win_32 and 280.26-notebook-win7-winvista-32bit-international-whql.
 * http://developer.download.nvidia.com/compute/DevZone/docs/html/C/doc/CUDA_VideoDecoder_Library.pdf
 * http://developer.download.nvidia.com/compute/DevZone/docs/html/C/doc/CUDA_VideoEncoder_Library.pdf
 * RTP payloader/depayloader follows RFC 3984.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango(DOT)org>
 *
 */
#ifndef TINYDAV_CODEC_H264_CUDA_H
#define TINYDAV_CODEC_H264_CUDA_H

#include "tinydav_config.h"

#if HAVE_CUDA

#include "tinydav/codecs/h264/tdav_codec_h264_common.h"

// I really don't want to use C++ code :(
#if !defined(__cplusplus)
typedef enum NVVE_FrameRate NVVE_FrameRate;
typedef enum NVVE_GPUOffloadLevel NVVE_GPUOffloadLevel;
typedef enum NVVE_ASPECT_RATIO_TYPE NVVE_ASPECT_RATIO_TYPE;
typedef enum NVVE_SurfaceFormat NVVE_SurfaceFormat;
typedef enum NVVE_PicStruct NVVE_PicStruct;
typedef enum NVVE_FIELD_MODE NVVE_FIELD_MODE;
typedef enum NVVE_RateCtrlType NVVE_RateCtrlType;
typedef enum NVVE_DI_MODE NVVE_DI_MODE;
typedef enum NVVE_PRESETS_TARGET NVVE_PRESETS_TARGET;
typedef enum NVVE_DI_MODE NVVE_DI_MODE;

typedef struct NVEncoderParams NVEncoderParams;
#endif /* __cplusplus */

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <unknwn.h>
#include <nvcuvid.h>
#include <cuviddec.h>
#include <NVEncoderAPI.h>
#include <NVEncodeDataTypes.h>
#include <d3d9.h>
#include <cudad3d9.h>
#include <cuda/types.h>

#include "tsk_mutex.h"

TDAV_BEGIN_DECLS

typedef struct tdav_codec_h264_cuda_s
{
	TDAV_DECLARE_CODEC_H264_COMMON;

	struct {
		NVEncoder context;
		NVEncoderParams ctx_params;
		NVVE_CallbackParams clb_params;
		void *buffer;
		tsk_size_t buffer_size;
		int64_t frame_count;
	} encoder;

	struct {
		tsk_mutex_handle_t *mutex;
		CUvideodecoder context;
		CUVIDDECODECREATEINFO info;
		CUvideoparser cu_parser;
		CUVIDPARSERPARAMS cu_paser_params;
		CUdevice cu_device;
		IDirect3D9 *dx_d3d;
		IDirect3DDevice9 *dx_d3ddevice;
		CUcontext cu_context;
		void* accumulator;
		tsk_size_t accumulator_pos;
		tsk_size_t accumulator_size;
		void *cu_buffer;
		tsk_size_t cu_buffer_size;
		tsk_size_t cu_buffer_pitch;
		tsk_bool_t cu_buffer_avail;
		uint16_t last_seq;
	} decoder;
}
tdav_codec_h264_cuda_t;

TINYDAV_GEXTERN const tmedia_codec_plugin_def_t *tdav_codec_h264_cuda_bp10_plugin_def_t;
TINYDAV_GEXTERN const tmedia_codec_plugin_def_t *tdav_codec_h264_cuda_bp20_plugin_def_t;
TINYDAV_GEXTERN const tmedia_codec_plugin_def_t *tdav_codec_h264_cuda_bp30_plugin_def_t;

tsk_bool_t tdav_codec_h264_cuda_is_supported();
static inline tsk_bool_t tdav_codec_h264_is_cuda_plugin(const tmedia_codec_plugin_def_t *plugin)
{
	if(plugin && (plugin == tdav_codec_h264_cuda_bp10_plugin_def_t || plugin == tdav_codec_h264_cuda_bp20_plugin_def_t || plugin == tdav_codec_h264_cuda_bp30_plugin_def_t)){
		return tsk_true;
	}
	return tsk_false;
}

TDAV_END_DECLS

#endif /* HAVE_CUDA */

#endif /* TINYDAV_CODEC_H264_CUDA_H */
