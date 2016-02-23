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

/**@file tdav_codec_h264_cuda.c
 * @brief H.264 codec plugin using NVIDIA CUDA for encoding/decoding
 * Env: gpucomputingsdk_4.0.17_win_32, cudatoolkit_4.0.17_win_32 and 280.26-notebook-win7-winvista-32bit-international-whql.
 * http://developer.download.nvidia.com/compute/DevZone/docs/html/C/doc/CUDA_VideoDecoder_Library.pdf
 * http://developer.download.nvidia.com/compute/DevZone/docs/html/C/doc/CUDA_VideoEncoder_Library.pdf
 * 2.0: https://developer.nvidia.com/sites/default/files/akamai/cuda/files/CUDADownloads/NVENC_VideoEncoder_API_ProgGuide.pdf
 *
 * RTP payloader/depayloader follows RFC 3984.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango(DOT)org>
 *
 */
#include "tinydav/codecs/h264/tdav_codec_h264_cuda.h"

#if HAVE_CUDA

#include "tinyrtp/rtp/trtp_rtp_packet.h"

#if defined(_MSC_VER)
#       pragma comment(lib, "nvcuvenc.lib")
#       pragma comment(lib, "nvcuvid.lib")
#       pragma comment(lib, "cuda.lib")

#       pragma comment(lib, "d3d9.lib")
#       pragma comment(lib, "d3dx9.lib")
#endif

#include "tsk_memory.h"
#include "tsk_debug.h"

#include <cutil_inline.h>
#include <cuda.h>

#define tdav_codec_h264_cuda_fmtp_set tsk_null

#if !defined(CUDA_MAX_FRM_CNT)
#	define CUDA_MAX_FRM_CNT  10
#endif

#include "tsk_semaphore.h"
tsk_semaphore_handle_t *sem = tsk_null;

#define InitH264DecoderInfo(_self) \
	memset(&_self->decoder.info, 0, sizeof(_self->decoder.info)); \
	_self->decoder.info.ulCreationFlags = cudaVideoCreate_PreferCUDA; \
	_self->decoder.info.CodecType = cudaVideoCodec_H264; \
	_self->decoder.info.ulWidth = TMEDIA_CODEC_VIDEO(_self)->in.width; \
	_self->decoder.info.ulTargetWidth = TMEDIA_CODEC_VIDEO(_self)->in.width; \
	_self->decoder.info.ulHeight = TMEDIA_CODEC_VIDEO(_self)->in.height; \
	_self->decoder.info.ulTargetHeight = TMEDIA_CODEC_VIDEO(_self)->in.height; \
	_self->decoder.info.ulNumDecodeSurfaces = CUDA_MAX_FRM_CNT; \
	_self->decoder.info.ulNumOutputSurfaces = 1; \
	_self->decoder.info.ChromaFormat = cudaVideoChromaFormat_420; \
	_self->decoder.info.OutputFormat = cudaVideoSurfaceFormat_NV12; \
	_self->decoder.info.DeinterlaceMode = cudaVideoDeinterlaceMode_Adaptive;

static int CUDAAPI _NVCallback_HandleVideoSequence(void *pvUserData, CUVIDEOFORMAT *pFormat);
static int CUDAAPI _NVCallback_HandlePictureDecode(void *pvUserData, CUVIDPICPARAMS *pPicParams);
static int CUDAAPI _NVCallback_HandlePictureDisplay(void *pvUserData, CUVIDPARSERDISPINFO *pPicParams);
static unsigned char* CUDAAPI _NVCallback_HandleAcquireBitStream(int *pBufferSize, void *pUserdata);
static void CUDAAPI _NVCallback_HandleReleaseBitStream(int nBytesInBuffer, unsigned char *cb,void *pUserdata);
static void CUDAAPI _NVCallback_HandleOnBeginFrame(const NVVE_BeginFrameInfo *pbfi, void *pUserdata);
static void CUDAAPI _NVCallback_HandleOnEndFrame(const NVVE_EndFrameInfo *pefi, void *pUserdata);

static inline void _tdav_codec_h264_cuda_encap(const tdav_codec_h264_cuda_t* h264, const uint8_t* pdata, tsk_size_t size);
static inline tsk_size_t _tdav_codec_h264_cuda_pict_layout(tdav_codec_h264_cuda_t* self, void**output, tsk_size_t *output_size);

static int tdav_codec_h264_cuda_open(tmedia_codec_t* self)
{
    int ret = 0, i;
    int bestGPU = 0, gpuPerf = 0, adapterCount;
    static int low_latency = 1;
    HRESULT hr;
    CUresult cuResult;
    D3DPRESENT_PARAMETERS d3dpp;
    tdav_codec_h264_cuda_t* h264 = (tdav_codec_h264_cuda_t*)self;

    if(!h264) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    //
    // encoder
    //
    memset(&h264->encoder.clb_params, 0, sizeof(h264->encoder.clb_params));
    memset(&h264->encoder.ctx_params, 0, sizeof(h264->encoder.ctx_params));

    h264->encoder.ctx_params.iInputSize[0] = TMEDIA_CODEC_VIDEO(h264)->out.width;
    h264->encoder.ctx_params.iInputSize[1] = TMEDIA_CODEC_VIDEO(h264)->out.height;
    h264->encoder.ctx_params.iOutputSize[0] = TMEDIA_CODEC_VIDEO(h264)->out.width;
    h264->encoder.ctx_params.iOutputSize[1] = TMEDIA_CODEC_VIDEO(h264)->out.height;
    h264->encoder.ctx_params.GPUOffloadLevel= NVVE_GPU_OFFLOAD_DEFAULT;
    h264->encoder.ctx_params.iSurfaceFormat = (int)IYUV;
    h264->encoder.ctx_params.iPictureType   = (int)FRAME_PICTURE;
    h264->encoder.ctx_params.Fieldmode = MODE_FRAME;
    h264->encoder.ctx_params.Presets = (NVVE_PRESETS_TARGET)-1;//Should be iPod, Zune ...
    h264->encoder.ctx_params.iP_Interval = 1;
    h264->encoder.ctx_params.iAspectRatio[0] = 4;
    h264->encoder.ctx_params.iAspectRatio[1] = 3;
    h264->encoder.ctx_params.iAspectRatio[2] = 0;
    h264->encoder.ctx_params.iIDR_Period = TMEDIA_CODEC_VIDEO(h264)->out.fps * 3;
    h264->encoder.ctx_params.iUseDeviceMem = 0;
    h264->encoder.ctx_params.iDynamicGOP = 0;
    h264->encoder.ctx_params.RCType = RC_VBR;
    h264->encoder.ctx_params.iAvgBitrate = 400000;
    h264->encoder.ctx_params.iPeakBitrate = 800000;
    h264->encoder.ctx_params.iQP_Level_Intra = 25;
    h264->encoder.ctx_params.iQP_Level_InterP = 28;
    h264->encoder.ctx_params.iQP_Level_InterB = 31;
    h264->encoder.ctx_params.iFrameRate[0] = TMEDIA_CODEC_VIDEO(h264)->out.fps * 1000;
    h264->encoder.ctx_params.iFrameRate[1] = 1000;
    h264->encoder.ctx_params.iDeblockMode = 1;
    h264->encoder.ctx_params.iForceIntra = 0;
    h264->encoder.ctx_params.iForceIDR = 0;
    h264->encoder.ctx_params.iClearStat = 0;
    h264->encoder.ctx_params.DIMode = DI_MEDIAN;
    h264->encoder.ctx_params.iDisableSPSPPS = 1; // Do not include SPS/PPS frames
    h264->encoder.ctx_params.iNaluFramingType = 0; // StartCodes
    h264->encoder.ctx_params.iMultiGPU = 1;
    switch(TDAV_CODEC_H264_COMMON(h264)->profile) {
    case tdav_codec_h264_bp10:
        h264->encoder.ctx_params.iDisableCabac = 1;
        h264->encoder.ctx_params.iProfileLevel = 0xff42;
        break;
    case tdav_codec_h264_bp20:
        h264->encoder.ctx_params.iDisableCabac = 1;
        h264->encoder.ctx_params.iProfileLevel = 0xff42;
        break;
    case tdav_codec_h264_bp30:
        h264->encoder.ctx_params.iDisableCabac = 1;
        h264->encoder.ctx_params.iProfileLevel = 0xff42;
        break;
    default:
        break;
    }

    hr = NVCreateEncoder(&h264->encoder.context);
    if(FAILED(hr)) {
        TSK_DEBUG_ERROR("NVCreateEncoder failed with error code = %08x", hr);
        return -2;
    }

    hr = NVSetCodec(h264->encoder.context, NV_CODEC_TYPE_H264);
    if(FAILED(hr)) {
        TSK_DEBUG_ERROR("NVSetCodec failed with error code = %08x", hr);
        return -2;
    }

    hr = NVSetDefaultParam(h264->encoder.context);
    if(FAILED(hr)) {
        TSK_DEBUG_ERROR("NVSetDefaultParam() failed with error code = %08x", hr);
        return -2;
    }

    hr = NVGetParamValue(h264->encoder.context, NVVE_GET_GPU_COUNT, &h264->encoder.ctx_params.GPU_count);
    if(SUCCEEDED(hr)) {
        int temp = 0, deviceCount;
        for (deviceCount=0; deviceCount < h264->encoder.ctx_params.GPU_count; deviceCount++) {
            NVVE_GPUAttributes GPUAttributes = {0};

            GPUAttributes.iGpuOrdinal = deviceCount;
            hr = NVGetParamValue(h264->encoder.context, NVVE_GET_GPU_ATTRIBUTES,  &GPUAttributes);
            if(FAILED(hr)) {
                TSK_DEBUG_ERROR("NVGetParamValue(NVVE_GET_GPU_ATTRIBUTES) failed with error code = %08x", hr);
                continue;
            }

            temp = GPUAttributes.iClockRate * GPUAttributes.iMultiProcessorCount;
            temp = temp * _ConvertSMVer2Cores(GPUAttributes.iMajor, GPUAttributes.iMinor);

            if(temp > gpuPerf) {
                gpuPerf = temp;
                bestGPU = deviceCount;
            }
        }
    }
    else {
        TSK_DEBUG_ERROR("NVGetParamValue(NVVE_GET_GPU_COUNT) failed with error code = %08x", hr);
        return -2;
    }

    h264->encoder.ctx_params.iForcedGPU = bestGPU;
    hr = NVSetParamValue(h264->encoder.context, NVVE_FORCE_GPU_SELECTION, &h264->encoder.ctx_params.iForcedGPU);
    if(FAILED(hr)) {
        TSK_DEBUG_WARN("NVSetParamValue(NVVE_FORCE_GPU_SELECTION) failed with error code = %08x", hr);
    }

    hr = NVSetParamValue(h264->encoder.context, NVVE_DEVICE_MEMORY_INPUT, &(h264->encoder.ctx_params.iUseDeviceMem));
    if(FAILED(hr)) {
        TSK_DEBUG_ERROR("NVSetParamValue(NVVE_OUT_SIZE) failed with error code = %08x", hr);
        return -2;
    }

    h264->encoder.buffer_size = (h264->encoder.ctx_params.iOutputSize[1] * h264->encoder.ctx_params.iOutputSize[0] * 3) >> 4;
    if(!h264->encoder.buffer && !(h264->encoder.buffer = tsk_realloc(h264->encoder.buffer, h264->encoder.buffer_size))) {
        TSK_DEBUG_ERROR("Failed to allocate buffer with size=%u", h264->encoder.buffer_size);
        h264->encoder.buffer_size = 0;
        return -2;
    }

    hr = NVSetParamValue(h264->encoder.context,NVVE_OUT_SIZE,		&(h264->encoder.ctx_params.iOutputSize));
    if (hr!=S_OK) {
        TSK_DEBUG_WARN("NVSetParamValue(NVVE_OUT_SIZE) failed with error code = %08x", hr);
    }
    hr = NVSetParamValue(h264->encoder.context,NVVE_IN_SIZE,		&(h264->encoder.ctx_params.iInputSize));
    if (hr!=S_OK) {
        TSK_DEBUG_WARN("NVSetParamValue(NVVE_IN_SIZE) failed with error code = %08x", hr);
    }
    hr = NVSetParamValue(h264->encoder.context,NVVE_MULTI_GPU,	&(h264->encoder.ctx_params.iMultiGPU));
    if (hr!=S_OK) {
        TSK_DEBUG_WARN("NVSetParamValue(NVVE_MULTI_GPU) failed with error code = %08x", hr);
    }
    hr = NVSetParamValue(h264->encoder.context,NVVE_ASPECT_RATIO,	&(h264->encoder.ctx_params.iAspectRatio));
    if (hr!=S_OK) {
        TSK_DEBUG_WARN("NVSetParamValue(NVVE_ASPECT_RATIO) failed with error code = %08x", hr);
    }
    hr = NVSetParamValue(h264->encoder.context,NVVE_FIELD_ENC_MODE,	&(h264->encoder.ctx_params.Fieldmode));
    if (hr!=S_OK) {
        TSK_DEBUG_WARN("NVSetParamValue(NVVE_FIELD_ENC_MODE) failed with error code = %08x", hr);
    }
    hr = NVSetParamValue(h264->encoder.context,NVVE_P_INTERVAL,		&(h264->encoder.ctx_params.iP_Interval));
    if (hr!=S_OK) {
        TSK_DEBUG_WARN("NVSetParamValue(NVVE_P_INTERVAL) failed with error code = %08x", hr);
    }
    hr = NVSetParamValue(h264->encoder.context,NVVE_IDR_PERIOD,		&(h264->encoder.ctx_params.iIDR_Period));
    if (hr!=S_OK) {
        TSK_DEBUG_WARN("NVSetParamValue(NVVE_IDR_PERIOD) failed with error code = %08x", hr);
    }
    hr = NVSetParamValue(h264->encoder.context,NVVE_DYNAMIC_GOP,	&(h264->encoder.ctx_params.iDynamicGOP));
    if (hr!=S_OK) {
        TSK_DEBUG_WARN("NVSetParamValue(NVVE_DYNAMIC_GOP) failed with error code = %08x", hr);
    }
    hr = NVSetParamValue(h264->encoder.context,NVVE_RC_TYPE,		&(h264->encoder.ctx_params.RCType));
    if (hr!=S_OK) {
        TSK_DEBUG_WARN("NVSetParamValue(NVVE_RC_TYPE) failed with error code = %08x", hr);
    }
    hr = NVSetParamValue(h264->encoder.context,NVVE_AVG_BITRATE,	&(h264->encoder.ctx_params.iAvgBitrate));
    if (hr!=S_OK) {
        TSK_DEBUG_WARN("NVSetParamValue(NVVE_AVG_BITRATE) failed with error code = %08x", hr);
    }
    hr = NVSetParamValue(h264->encoder.context,NVVE_PEAK_BITRATE,	&(h264->encoder.ctx_params.iPeakBitrate));
    if (hr!=S_OK) {
        TSK_DEBUG_WARN("NVSetParamValue(NVVE_PEAK_BITRATE) failed with error code = %08x", hr);
    }
    hr = NVSetParamValue(h264->encoder.context,NVVE_QP_LEVEL_INTRA,	&(h264->encoder.ctx_params.iQP_Level_Intra));
    if (hr!=S_OK) {
        TSK_DEBUG_WARN("NVSetParamValue(NVVE_OUT_SIZE) failed with error code = %08x", hr);
    }
    hr = NVSetParamValue(h264->encoder.context,NVVE_QP_LEVEL_INTER_P,&(h264->encoder.ctx_params.iQP_Level_InterP));
    if (hr!=S_OK) {
        TSK_DEBUG_WARN("NVSetParamValue(NVVE_QP_LEVEL_INTER_P) failed with error code = %08x", hr);
    }
    hr = NVSetParamValue(h264->encoder.context,NVVE_QP_LEVEL_INTER_B,&(h264->encoder.ctx_params.iQP_Level_InterB));
    if (hr!=S_OK) {
        TSK_DEBUG_WARN("NVSetParamValue(NVVE_QP_LEVEL_INTER_B) failed with error code = %08x", hr);
    }
    hr = NVSetParamValue(h264->encoder.context,NVVE_FRAME_RATE,		&(h264->encoder.ctx_params.iFrameRate));
    if (hr!=S_OK) {
        TSK_DEBUG_WARN("NVSetParamValue(NVVE_FRAME_RATE) failed with error code = %08x", hr);
    }
    hr = NVSetParamValue(h264->encoder.context,NVVE_DEBLOCK_MODE,	&(h264->encoder.ctx_params.iDeblockMode));
    if (hr!=S_OK) {
        TSK_DEBUG_WARN("NVSetParamValue(NVVE_DEBLOCK_MODE) failed with error code = %08x", hr);
    }
    hr = NVSetParamValue(h264->encoder.context,NVVE_PROFILE_LEVEL,	&(h264->encoder.ctx_params.iProfileLevel));
    if (hr!=S_OK) {
        TSK_DEBUG_WARN("NVSetParamValue(NVVE_PROFILE_LEVEL) failed with error code = %08x", hr);
    }
    hr = NVSetParamValue(h264->encoder.context,NVVE_FORCE_INTRA,	&(h264->encoder.ctx_params.iForceIntra));
    if (hr!=S_OK) {
        TSK_DEBUG_WARN("NVSetParamValue(NVVE_FORCE_INTRA) failed with error code = %08x", hr);
    }
    hr = NVSetParamValue(h264->encoder.context,NVVE_FORCE_IDR,		&(h264->encoder.ctx_params.iForceIDR));
    if (hr!=S_OK) {
        TSK_DEBUG_WARN("NVSetParamValue(NVVE_FORCE_IDR) failed with error code = %08x", hr);
    }
    hr = NVSetParamValue(h264->encoder.context,NVVE_CLEAR_STAT,		&(h264->encoder.ctx_params.iClearStat));
    if (hr!=S_OK) {
        TSK_DEBUG_WARN("NVSetParamValue(NVVE_CLEAR_STAT) failed with error code = %08x", hr);
    }
    hr = NVSetParamValue(h264->encoder.context,NVVE_SET_DEINTERLACE,&(h264->encoder.ctx_params.DIMode));
    if (hr!=S_OK) {
        TSK_DEBUG_WARN("NVSetParamValue(NVVE_SET_DEINTERLACE) failed with error code = %08x", hr);
    }
    if (h264->encoder.ctx_params.Presets != -1) {
        hr = NVSetParamValue(h264->encoder.context,NVVE_PRESETS,	&(h264->encoder.ctx_params.Presets));
        if (hr!=S_OK) {
            TSK_DEBUG_WARN("NVSetParamValue(NVVE_PRESETS) failed with error code = %08x", hr);
        }
    }
    hr = NVSetParamValue(h264->encoder.context,NVVE_DISABLE_CABAC,	&(h264->encoder.ctx_params.iDisableCabac));
    if (hr!=S_OK) {
        TSK_DEBUG_WARN("NVSetParamValue(NVVE_DISABLE_CABAC) failed with error code = %08x", hr);
    }
    hr = NVSetParamValue(h264->encoder.context,NVVE_CONFIGURE_NALU_FRAMING_TYPE, &(h264->encoder.ctx_params.iNaluFramingType));
    if (hr!=S_OK) {
        TSK_DEBUG_WARN("NVSetParamValue(NVVE_CONFIGURE_NALU_FRAMING_TYPE) failed with error code = %08x", hr);
    }
    hr = NVSetParamValue(h264->encoder.context,NVVE_DISABLE_SPS_PPS,&(h264->encoder.ctx_params.iDisableSPSPPS));
    if (hr!=S_OK) {
        TSK_DEBUG_WARN("NVSetParamValue(NVVE_DISABLE_SPS_PPS) failed with error code = %08x", hr);
    }
    hr = NVSetParamValue(h264->encoder.context,NVVE_LOW_LATENCY,&low_latency);
    if (hr!=S_OK) {
        TSK_DEBUG_WARN("NVSetParamValue(NVVE_LOW_LATENCY) failed with error code = %08x", hr);
    }

    h264->encoder.clb_params.pfnacquirebitstream = _NVCallback_HandleAcquireBitStream;
    h264->encoder.clb_params.pfnonbeginframe     = _NVCallback_HandleOnBeginFrame;
    h264->encoder.clb_params.pfnonendframe       = _NVCallback_HandleOnEndFrame;
    h264->encoder.clb_params.pfnreleasebitstream = _NVCallback_HandleReleaseBitStream;
    NVRegisterCB(h264->encoder.context, h264->encoder.clb_params, h264);


    hr = NVCreateHWEncoder(h264->encoder.context);
    if(FAILED(hr)) {
        TSK_DEBUG_ERROR("NVCreateHWEncoder failed with error code = %08x", hr);
        return -2;
    }



    //
    // decoder
    //
    if((cuResult = cuInit(0)) != CUDA_SUCCESS) {
        TSK_DEBUG_ERROR("cuInit(0) failed with error code = %d", (int)cuResult);
        return -3;
    }

    InitH264DecoderInfo(h264);

    h264->decoder.cu_device = cutilDrvGetMaxGflopsGraphicsDeviceId();

#if _DEBUG || DEBUG
    {
        int major, minor;
        size_t totalGlobalMem;
        char deviceName[256];
        cuDeviceComputeCapability(&major, &minor, h264->decoder.cu_device);
        cuDeviceGetName(deviceName, sizeof(deviceName), h264->decoder.cu_device);
        TSK_DEBUG_INFO("Using GPU Device %d: %s has SM %d.%d compute capability", h264->decoder.cu_device, deviceName, major, minor);

        cutilDrvSafeCallNoSync(cuDeviceTotalMem(&totalGlobalMem, h264->decoder.cu_device) );
        TSK_DEBUG_INFO("Total amount of global memory in GPU device: %4.4f MB", (float)totalGlobalMem/(1024*1024));
    }
#endif

    // create Direct3D instance
    h264->decoder.dx_d3d = Direct3DCreate9(D3D_SDK_VERSION);
    if(!h264->decoder.dx_d3d) {
        TSK_DEBUG_ERROR("Direct3DCreate9 failed");
        return -3;
    }
    adapterCount = h264->decoder.dx_d3d->GetAdapterCount();
    for(i=0; i<adapterCount; ++i) {
        ZeroMemory(&d3dpp, sizeof(d3dpp));
        d3dpp.Windowed = TRUE;
        d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
        d3dpp.BackBufferWidth = h264->decoder.info.ulTargetWidth;
        d3dpp.BackBufferHeight = h264->decoder.info.ulTargetHeight;
        d3dpp.BackBufferCount = 1;
        d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
        d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
        d3dpp.Flags = D3DPRESENTFLAG_VIDEO;//D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
        hr = h264->decoder.dx_d3d->CreateDevice(i,
                                                D3DDEVTYPE_HAL,
                                                GetDesktopWindow(),
                                                D3DCREATE_FPU_PRESERVE | D3DCREATE_MULTITHREADED | D3DCREATE_HARDWARE_VERTEXPROCESSING,
                                                &d3dpp,
                                                &h264->decoder.dx_d3ddevice);
        if(hr == S_OK) {
            cuResult = cuD3D9CtxCreate(&h264->decoder.cu_context, &h264->decoder.cu_device, 0, h264->decoder.dx_d3ddevice);
            if(cuResult == CUDA_SUCCESS) {
                break;
            }
            if(h264->decoder.dx_d3ddevice) {
                h264->decoder.dx_d3ddevice->Release();
                h264->decoder.dx_d3ddevice = NULL;
            }
        }
    }

    memset(&h264->decoder.cu_paser_params, 0, sizeof(h264->decoder.cu_paser_params));
    h264->decoder.cu_paser_params.CodecType = cudaVideoCodec_H264;
    h264->decoder.cu_paser_params.ulMaxNumDecodeSurfaces = CUDA_MAX_FRM_CNT;
    h264->decoder.cu_paser_params.pUserData = h264;
    h264->decoder.cu_paser_params.pfnSequenceCallback = _NVCallback_HandleVideoSequence;
    h264->decoder.cu_paser_params.pfnDecodePicture = _NVCallback_HandlePictureDecode;
    h264->decoder.cu_paser_params.pfnDisplayPicture = _NVCallback_HandlePictureDisplay;
    cuResult = cuvidCreateVideoParser(&h264->decoder.cu_parser, &h264->decoder.cu_paser_params);
    if(cuResult != CUDA_SUCCESS) {
        TSK_DEBUG_ERROR("cuvidCreateVideoParser(0) failed with error code = %d", (int)cuResult);
        return -3;
    }

    cuResult = cuvidCreateDecoder(&h264->decoder.context, &h264->decoder.info);
    if(CUDA_SUCCESS != cuResult) {
        TSK_DEBUG_ERROR("cuvidCreateDecoder failed with error code=%d", (int)cuResult);
        return -3;
    }

    return ret;
}

static int tdav_codec_h264_cuda_close(tmedia_codec_t* self)
{
    tdav_codec_h264_cuda_t* h264 = (tdav_codec_h264_cuda_t*)self;

    if(!h264) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if(h264->encoder.context) {
        NVDestroyEncoder(h264->encoder.context);
        h264->encoder.context = NULL;
    }
    return 0;
}

static tsk_size_t tdav_codec_h264_cuda_encode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size)
{
    NVVE_EncodeFrameParams efparams;
    int ret = 0;
    unsigned long flags = 0;
    HRESULT hr;

    tdav_codec_h264_cuda_t* h264 = (tdav_codec_h264_cuda_t*)self;

    if(!self || !in_data || !in_size || !out_data) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return 0;
    }

    if(!self->opened) {
        TSK_DEBUG_ERROR("Codec not opened");
        return 0;
    }

    if((h264->encoder.ctx_params.iOutputSize[1] * h264->encoder.ctx_params.iOutputSize[0] * 3)>>1 != in_size) {
        /* guard */
        TSK_DEBUG_ERROR("Invalid size");
        return 0;
    }

    //return 0;

    efparams.Width = h264->encoder.ctx_params.iOutputSize[0];
    efparams.Height = h264->encoder.ctx_params.iOutputSize[1];
    efparams.Pitch = (h264->encoder.ctx_params.nDeviceMemPitch ? h264->encoder.ctx_params.nDeviceMemPitch : h264->encoder.ctx_params.iOutputSize[0]);
    efparams.PictureStruc = (NVVE_PicStruct)h264->encoder.ctx_params.iPictureType;
    efparams.SurfFmt = (NVVE_SurfaceFormat)h264->encoder.ctx_params.iSurfaceFormat;
    efparams.progressiveFrame = (h264->encoder.ctx_params.iSurfaceFormat == 3) ? 1 : 0;
    efparams.repeatFirstField = 0;
    efparams.topfieldfirst = (h264->encoder.ctx_params.iSurfaceFormat == 1) ? 1 : 0;
    efparams.picBuf = (unsigned char *)in_data;
    efparams.bLast = 0;

    // send keyframe for:
    //	- the first frame
    //	- every second within the first 4seconds
    //  - every 7 seconds after the first 4seconds
    if(h264->encoder.frame_count++ == 0
            ||
            ( (h264->encoder.frame_count < (int)TMEDIA_CODEC_VIDEO(h264)->out.fps * 4) && ((h264->encoder.frame_count % TMEDIA_CODEC_VIDEO(h264)->out.fps)==0) )
            ||
            ( (h264->encoder.frame_count % (TMEDIA_CODEC_VIDEO(h264)->out.fps * 5))==0 )
      ) {
        flags |= 0x04;  // FORCE IDR
        if(h264->encoder.ctx_params.iDisableSPSPPS) {
            unsigned char SPSPPSBuff[1024];
            int SPSPPSBuffSize = sizeof(SPSPPSBuff);
            hr = NVGetSPSPPS(h264->encoder.context, SPSPPSBuff, SPSPPSBuffSize, &SPSPPSBuffSize);
            if(SUCCEEDED(hr)) {
                int size = 0;
                while(size < SPSPPSBuffSize - 2) {
                    int16_t next_size = ((int16_t)SPSPPSBuff[size])<<1 | ((int16_t)SPSPPSBuff[size + 1]);
                    _tdav_codec_h264_cuda_encap(h264, &SPSPPSBuff[size + 2], next_size);
                    size+=next_size + 2;
                }
            }
            else {
                TSK_DEBUG_ERROR("NVGetSPSPPS failed with error code = %08x", hr)
            }
        }
    }

    hr = NVEncodeFrame(h264->encoder.context, &efparams, flags, NULL);

    return 0;
}

static tsk_size_t tdav_codec_h264_cuda_decode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size, const tsk_object_t* proto_hdr)
{
    tdav_codec_h264_cuda_t* h264 = (tdav_codec_h264_cuda_t*)self;
    const trtp_rtp_header_t* rtp_hdr = (const trtp_rtp_header_t*)proto_hdr;
    const uint8_t* pay_ptr = tsk_null;
    tsk_size_t pay_size = 0, retsize = 0, size_to_copy = 0;
    int ret = 0;
    tsk_bool_t append_scp = tsk_false;
    static tsk_size_t xmax_size = (1920 * 1080 * 3) >> 3;
    static tsk_size_t start_code_prefix_size = sizeof(H264_START_CODE_PREFIX);

    // Packet lost?
    if(h264->decoder.last_seq != (rtp_hdr->seq_num - 1) && h264->decoder.last_seq) {
        if(h264->decoder.last_seq == rtp_hdr->seq_num) {
            // Could happen on some stupid emulators
            TSK_DEBUG_INFO("Packet duplicated, seq_num=%d", rtp_hdr->seq_num);
            return 0;
        }
        TSK_DEBUG_INFO("Packet lost, seq_num=%d", rtp_hdr->seq_num);
    }
    h264->decoder.last_seq = rtp_hdr->seq_num;

    /* 5.3. NAL Unit Octet Usage
      +---------------+
      |0|1|2|3|4|5|6|7|
      +-+-+-+-+-+-+-+-+
      |F|NRI|  Type   |
      +---------------+
    */
    if(*((uint8_t*)in_data) >> 7) {
        TSK_DEBUG_WARN("F=1");
        /* reset accumulator */
        h264->decoder.accumulator_pos = 0;
        goto bail;
    }

    // get payload
    if((ret = tdav_codec_h264_get_pay(in_data, in_size, (const void**)&pay_ptr, &pay_size, &append_scp)) || !pay_ptr || !pay_size) {
        TSK_DEBUG_ERROR("Depayloader failed to get H.264 content");
        goto bail;
    }
    //append_scp = tsk_true;
    size_to_copy = pay_size + (append_scp ? start_code_prefix_size : 0);

    // start-accumulator
    if(!h264->decoder.accumulator) {
        if(size_to_copy > xmax_size) {
            TSK_DEBUG_ERROR("%u too big to contain valid encoded data. xmax_size=%u", size_to_copy, xmax_size);
            goto bail;
        }
        if(!(h264->decoder.accumulator = tsk_calloc(size_to_copy, sizeof(uint8_t)))) {
            TSK_DEBUG_ERROR("Failed to allocated new buffer");
            goto bail;
        }
        h264->decoder.accumulator_size = size_to_copy;
    }
    if((h264->decoder.accumulator_pos + size_to_copy) >= xmax_size) {
        TSK_DEBUG_ERROR("BufferOverflow");
        h264->decoder.accumulator_pos = 0;
        goto bail;
    }
    if((h264->decoder.accumulator_pos + size_to_copy) > h264->decoder.accumulator_size) {
        if(!(h264->decoder.accumulator = tsk_realloc(h264->decoder.accumulator, (h264->decoder.accumulator_pos + size_to_copy)))) {
            TSK_DEBUG_ERROR("Failed to reallocated new buffer");
            h264->decoder.accumulator_pos = 0;
            h264->decoder.accumulator_size = 0;
            goto bail;
        }
        h264->decoder.accumulator_size = (h264->decoder.accumulator_pos + size_to_copy);
    }

    if(append_scp) {
        memcpy(&((uint8_t*)h264->decoder.accumulator)[h264->decoder.accumulator_pos], H264_START_CODE_PREFIX, start_code_prefix_size);
        h264->decoder.accumulator_pos += start_code_prefix_size;
    }
    memcpy(&((uint8_t*)h264->decoder.accumulator)[h264->decoder.accumulator_pos], pay_ptr, pay_size);
    h264->decoder.accumulator_pos += pay_size;
    // end-accumulator

    if(rtp_hdr->marker) {
        CUVIDSOURCEDATAPACKET pkt;
        CUresult cuResult;
        pkt.flags = 0;
        pkt.payload_size = (unsigned long) h264->decoder.accumulator_pos;
        pkt.payload = (unsigned char *)h264->decoder.accumulator;
        pkt.timestamp = 0;

        // reset accumulator
        h264->decoder.accumulator_pos = 0;
        cuResult = cuvidParseVideoData(h264->decoder.cu_parser, &pkt);
        if(cuResult != CUDA_SUCCESS) {
            TSK_DEBUG_ERROR("cuvidParseVideoData() failed with error code = %d", (int)cuResult);
            goto bail;
        }

        if(h264->decoder.cu_buffer_avail) {
            h264->decoder.cu_buffer_avail = tsk_false;
            if((retsize = _tdav_codec_h264_cuda_pict_layout(h264, out_data, out_max_size)) == 0) {
                TSK_DEBUG_ERROR("_tdav_codec_h264_cuda_pict_layout failed");
                goto bail;
            }
        }
    }

bail:
    return retsize;
}

static tsk_bool_t tdav_codec_h264_cuda_sdp_att_match(const tmedia_codec_t* codec, const char* att_name, const char* att_value)
{
    tdav_codec_h264_cuda_t* h264 = (tdav_codec_h264_cuda_t*)codec;
    profile_idc_t profile;

    if(!h264) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_false;
    }

    /* Check whether the profile match (If the profile is missing, then we consider that it's ok) */
    if(((profile = tdav_codec_h264_common_get_profile(fmtp)) != tdav_codec_h264_bp99) && (profile != TDAV_CODEC_H264_COMMON(h264)->profile)) {
        TSK_DEBUG_INFO("Profile not matching");
        return tsk_false;
    }

    TMEDIA_CODEC_VIDEO(h264)->in.width = 800, TMEDIA_CODEC_VIDEO(h264)->in.height = 640;
    TMEDIA_CODEC_VIDEO(h264)->out.width = 800, TMEDIA_CODEC_VIDEO(h264)->out.height = 640;
    //TMEDIA_CODEC_VIDEO(h264)->in.width = 352, TMEDIA_CODEC_VIDEO(h264)->in.height = 288;
    //TMEDIA_CODEC_VIDEO(h264)->out.width = 352, TMEDIA_CODEC_VIDEO(h264)->out.height = 288;

    return tsk_true;
}

static char* tdav_codec_h264_cuda_sdp_att_get(const tmedia_codec_t* self, const char* att_name)
{
    char* fmtp = tsk_null;
    tdav_codec_h264_cuda_t* h264 = (tdav_codec_h264_cuda_t*)self;

    switch(TDAV_CODEC_H264_COMMON(h264)->profile) {
    case tdav_codec_h264_bp10:
        fmtp = tsk_strdup("profile-level-id=42e00a");
        break;
    case tdav_codec_h264_bp20:
        fmtp = tsk_strdup("profile-level-id=42e014");
        break;
    case tdav_codec_h264_bp30:
        fmtp = tsk_strdup("profile-level-id=42e01e");
        break;
    }

    //1080p(1920 x 1080), 720p(1280 x 720), SVGA(800 x 600), VGA(640 x 480), 4CIF(704 x 576), CIF(352 x 288), QCIF(176 x 144), SQCIF(128 x 96)
    return fmtp;
}

tsk_bool_t tdav_codec_h264_cuda_is_supported()
{
    static tsk_bool_t __already_checked = tsk_false;
    static tsk_bool_t __is_supported = tsk_false;
    if(!__already_checked) {
        HRESULT hr;
        __already_checked = tsk_true;
        hr = NVGetHWEncodeCaps();
        if(SUCCEEDED(hr)) {
            NVEncoder encoder;
            hr = NVCreateEncoder(&encoder);
            if(SUCCEEDED(hr)) {
                hr = NVIsSupportedCodec(encoder, NV_CODEC_TYPE_H264);
                __is_supported = SUCCEEDED(hr);
            }
            else {
                TSK_DEBUG_ERROR("NVCreateEncoder() failed with error code = %08x", hr);
            }
            if(encoder) {
                NVDestroyEncoder(encoder);
                encoder = NULL;
            }
        }
    }
    return __is_supported;
}

static int tdav_codec_h264_cuda_init(tdav_codec_h264_cuda_t* self, profile_idc_t profile)
{
    int ret = 0;

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if((ret = tdav_codec_h264_common_init(TDAV_CODEC_H264_COMMON(self)))) {
        TSK_DEBUG_ERROR("tdav_codec_h264_common_init() faile with error code=%d", ret);
        return ret;
    }

    if(!self->decoder.mutex && !(self->decoder.mutex = tsk_mutex_create_2(tsk_false))) {
        TSK_DEBUG_ERROR("Failed to create mutex");
        return -2;
    }

    sem = tsk_semaphore_create_2(1);

    TDAV_CODEC_H264_COMMON(self)->pack_mode_local = H264_PACKETIZATION_MODE;
    TDAV_CODEC_H264_COMMON(self)->profile = profile;
    TMEDIA_CODEC_VIDEO(self)->in.max_mbps = TMEDIA_CODEC_VIDEO(self)->out.max_mbps = H264_MAX_MBPS*1000;
    TMEDIA_CODEC_VIDEO(self)->in.max_br = TMEDIA_CODEC_VIDEO(self)->out.max_br = H264_MAX_BR*1000;

    // At this time self->plugin is Null
    TMEDIA_CODEC_VIDEO(self)->in.width = TMEDIA_CODEC_VIDEO(self)->out.width = 176;
    TMEDIA_CODEC_VIDEO(self)->in.height = TMEDIA_CODEC_VIDEO(self)->out.height = 144;
    TMEDIA_CODEC_VIDEO(self)->in.fps = TMEDIA_CODEC_VIDEO(self)->out.fps = 15;
    TMEDIA_CODEC_VIDEO(self)->in.chroma = tmedia_chroma_yuv420p;// no choice

    return 0;
}

static int tdav_codec_h264_cuda_deinit(tdav_codec_h264_cuda_t* self)
{
    tdav_codec_h264_cuda_t* h264 = (tdav_codec_h264_cuda_t*)self;

    if(!h264) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if(h264->encoder.context) {
        NVDestroyEncoder(h264->encoder.context);
        h264->encoder.context = NULL;
    }
    TSK_FREE(h264->encoder.buffer);
    h264->encoder.buffer_size = 0;

    if(h264->decoder.context) {
        cuvidDestroyDecoder(h264->decoder.context);
        h264->decoder.context = NULL;
    }
    if(h264->decoder.cu_context) {
        cuCtxDestroy(h264->decoder.cu_context);
        h264->decoder.cu_context = NULL;
    }
    if (h264->decoder.dx_d3ddevice) {
        h264->decoder.dx_d3ddevice->Release();
        h264->decoder.dx_d3ddevice = NULL;
    }
    if (h264->decoder.dx_d3d) {
        h264->decoder.dx_d3d->Release();
        h264->decoder.dx_d3d = NULL;
    }
    if(h264->decoder.cu_parser) {
        cuvidDestroyVideoParser(h264->decoder.cu_parser);
        h264->decoder.cu_parser = NULL;
    }
    if(h264->decoder.cu_buffer) {
        cuMemFreeHost(h264->decoder.cu_buffer);
        h264->decoder.cu_buffer = NULL;
    }
    h264->decoder.cu_buffer_size = 0;
    if(self->decoder.mutex) {
        tsk_mutex_destroy(&self->decoder.mutex);
    }

    TSK_FREE(h264->decoder.accumulator);
    h264->decoder.accumulator_pos = 0;
    h264->decoder.accumulator_size = 0;

    return 0;
}

static inline void _tdav_codec_h264_cuda_encap(const tdav_codec_h264_cuda_t* h264, const uint8_t* pdata, tsk_size_t size)
{
    register int32_t i;
    int32_t last_scp, prev_scp;
    static int32_t size_of_scp = sizeof(H264_START_CODE_PREFIX); /* we know it's equal to 4 ..but */

    if(!pdata || !size) {
        return;
    }

    last_scp = 0, prev_scp = 0;

    for(i = size_of_scp; i<(int32_t)(size - size_of_scp); i++) {
        if(pdata[i] == H264_START_CODE_PREFIX[0] && pdata[i+1] == H264_START_CODE_PREFIX[1] && pdata[i+2] == H264_START_CODE_PREFIX[2] && pdata[i+3] == H264_START_CODE_PREFIX[3]) { /* Found Start Code Prefix */
            prev_scp = last_scp;
            if((i - last_scp) >= H264_RTP_PAYLOAD_SIZE || 1) {
                tdav_codec_h264_rtp_callback(TDAV_CODEC_H264_COMMON(h264), pdata + prev_scp,
                                             (i - prev_scp), (prev_scp == size));
            }
            last_scp = i;
        }
    }
    if(last_scp < (int32_t)size) {
        tdav_codec_h264_rtp_callback(TDAV_CODEC_H264_COMMON(h264), pdata + last_scp,
                                     (size - last_scp), tsk_true);
    }
}

static inline tsk_size_t _tdav_codec_h264_cuda_pict_layout(tdav_codec_h264_cuda_t* self, void**output, tsk_size_t *output_size)
{
    if(self && self->decoder.cu_buffer && self->decoder.cu_buffer_size) {
        unsigned int w = TMEDIA_CODEC_VIDEO(self)->in.width;
        unsigned int h = TMEDIA_CODEC_VIDEO(self)->in.height;
        unsigned int pitch = self->decoder.cu_buffer_pitch;
        tsk_size_t xsize = (w * h * 3) >> 1;
        // resize if too short
        if(*output_size<xsize) {
            if((*output = tsk_realloc(*output, xsize))) {
                *output_size = xsize;
            }
            else {
                *output_size = 0;
                return 0;
            }
        }

        tsk_semaphore_decrement(sem);

        register unsigned int y;
        register unsigned int x, x2;
        const unsigned char *p = (const unsigned char *)self->decoder.cu_buffer;
        register unsigned char *iyuv = (unsigned char *)*output, *i = iyuv, *j;
        // copy luma
        for (y=0; y<h; y++) {
            memcpy(i+y*w, p+y*pitch, w);
        }
        // de-interleave chroma (NV12 stored as U,V,U,V,...)
        p += h * pitch;
        i += h * w;
        j = i + (h/2)*(w/2);
        for (y=0; y<h/2; y++) {
            for (x=0, x2=0; x<w/2; x++, x2+=2) {
                i[x] = p[x2], j[x] = p[x2+1];
            }
            p += pitch, i += w/2, j += w/2;
        }

        tsk_semaphore_increment(sem);

        return xsize;
    }
    return 0;
}

static int CUDAAPI _NVCallback_HandleVideoSequence(void *pvUserData, CUVIDEOFORMAT *pFormat)
{
    tdav_codec_h264_cuda_t* h264 = (tdav_codec_h264_cuda_t*)pvUserData;
    if(!h264 || !pFormat) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return 0;//error
    }

    int ret = 1;

    if(pFormat->coded_width != TMEDIA_CODEC_VIDEO(h264)->in.width || pFormat->coded_height != TMEDIA_CODEC_VIDEO(h264)->in.height) {
        tsk_mutex_lock(h264->decoder.mutex);

        TMEDIA_CODEC_VIDEO(h264)->in.width = pFormat->coded_width;
        TMEDIA_CODEC_VIDEO(h264)->in.height = pFormat->coded_height;

        InitH264DecoderInfo(h264);
        CUresult cuResult;
        if(h264->decoder.context) {
            cuResult = cuvidDestroyDecoder(h264->decoder.context);
            if(CUDA_SUCCESS != cuResult) {
                TSK_DEBUG_ERROR("cuvidDestroyDecoder failed with error code=%d", (int)cuResult);
                ret = 0;
            }
            h264->decoder.context = NULL;
        }
        cuResult = cuvidCreateDecoder(&h264->decoder.context, &h264->decoder.info);
        if(CUDA_SUCCESS != cuResult) {
            TSK_DEBUG_ERROR("cuvidCreateDecoder failed with error code=%d", (int)cuResult);
            ret = 0;
        }

        tsk_mutex_unlock(h264->decoder.mutex);
    }



    return ret;//success
}

static int CUDAAPI _NVCallback_HandlePictureDecode(void *pvUserData, CUVIDPICPARAMS *pPicParams)
{
    tdav_codec_h264_cuda_t* h264 = (tdav_codec_h264_cuda_t*)pvUserData;
    if(!h264 || !pPicParams) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return 0;//error
    }

    tsk_mutex_lock(h264->decoder.mutex);
    CUresult cuResult = cuvidDecodePicture(h264->decoder.context, pPicParams);
    tsk_mutex_unlock(h264->decoder.mutex);

    if(cuResult != CUDA_SUCCESS) {
        TSK_DEBUG_ERROR("cuvidDecodePicture failed with error code= %d", cuResult);
        return 0;//error
    }

    return 1;//success
}

static int CUDAAPI _NVCallback_HandlePictureDisplay(void *pvUserData, CUVIDPARSERDISPINFO *pPicParams)
{
    tdav_codec_h264_cuda_t* h264 = (tdav_codec_h264_cuda_t*)pvUserData;
    CUVIDPROCPARAMS vpp;
    CUdeviceptr devPtr;
    CUresult cuResult;
    tsk_size_t nv12_size;
    tsk_bool_t mapped = tsk_false;
    int ret = 1;//success

    if(!h264 || !pPicParams) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return 0;//error
    }
    tsk_semaphore_decrement(sem);
    cuResult = cuCtxPushCurrent(h264->decoder.cu_context);
    if(cuResult != CUDA_SUCCESS) {
        TSK_DEBUG_ERROR("cuCtxPushCurrent failed with error code = %d", (int)cuResult);
        ret = 0;//error
        goto bail;
    }

    memset(&vpp, 0, sizeof(vpp));
    vpp.progressive_frame = pPicParams->progressive_frame;
    vpp.top_field_first = pPicParams->top_field_first;
    cuResult = cuvidMapVideoFrame(h264->decoder.context, pPicParams->picture_index, &devPtr, &h264->decoder.cu_buffer_pitch, &vpp);

    if(cuResult != CUDA_SUCCESS) {
        TSK_DEBUG_ERROR("cuvidMapVideoFrame failed with error code = %d", (int)cuResult);
        ret = 0;//error
        goto bail;
    }
    mapped = tsk_true;
    nv12_size = h264->decoder.cu_buffer_pitch * (h264->decoder.info.ulTargetHeight + h264->decoder.info.ulTargetHeight/2);  // 12bpp
    //nv12_size = (w * h * 3) >> 1;
    if ((!h264->decoder.cu_buffer) || (nv12_size > h264->decoder.cu_buffer_size)) {
        h264->decoder.cu_buffer_size = 0;
        if (h264->decoder.cu_buffer) {
            cuResult = cuMemFreeHost(h264->decoder.cu_buffer);
            h264->decoder.cu_buffer = NULL;
        }
        cuResult = cuMemAllocHost((void**)&h264->decoder.cu_buffer, nv12_size);
        if (cuResult != CUDA_SUCCESS) {
            TSK_DEBUG_ERROR("cuMemAllocHost failed to allocate %d bytes (error code=%d)", nv12_size, (int)cuResult);
            h264->decoder.cu_buffer = 0;
            h264->decoder.cu_buffer_size = 0;
            ret = 0;//error
        }
        else {
            h264->decoder.cu_buffer_size = nv12_size;
        }
    }
    if(h264->decoder.cu_buffer) {
        cuResult = cuMemcpyDtoH(h264->decoder.cu_buffer, devPtr, nv12_size);
    }

bail:
    if(mapped) {
        cuResult = cuvidUnmapVideoFrame(h264->decoder.context, devPtr);
    }
    cuResult = cuCtxPopCurrent(NULL);
    tsk_semaphore_increment(sem);
    h264->decoder.cu_buffer_avail = (ret == 1);
    return ret;
}

static unsigned char* CUDAAPI _NVCallback_HandleAcquireBitStream(int *pBufferSize, void *pUserdata)
{
    tdav_codec_h264_cuda_t* h264 = (tdav_codec_h264_cuda_t*)pUserdata;
    if(!h264 || !pBufferSize) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }

    *pBufferSize = (int)h264->encoder.buffer_size;
    return (unsigned char*)h264->encoder.buffer;
}

static void CUDAAPI _NVCallback_HandleReleaseBitStream(int nBytesInBuffer, unsigned char *cb,void *pUserdata)
{
    tdav_codec_h264_cuda_t* h264 = (tdav_codec_h264_cuda_t*)pUserdata;
    if(!h264 || !cb || !nBytesInBuffer) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return;
    }
    _tdav_codec_h264_cuda_encap(h264, cb, (tsk_size_t)nBytesInBuffer);

    return;
}

static void CUDAAPI _NVCallback_HandleOnBeginFrame(const NVVE_BeginFrameInfo *pbfi, void *pUserdata)
{
    return;
}

static void CUDAAPI _NVCallback_HandleOnEndFrame(const NVVE_EndFrameInfo *pefi, void *pUserdata)
{
    return;
}

/* ============ H.264 Base Profile 1.0 Plugin interface ================= */

/* constructor */
static tsk_object_t* tdav_codec_h264_cuda_bp10_ctor(tsk_object_t * self, va_list * app)
{
    tdav_codec_h264_cuda_t *h264 = (tdav_codec_h264_cuda_t *)self;
    if(h264) {
        /* init base: called by tmedia_codec_create() */
        /* init self */
        tdav_codec_h264_cuda_init(h264, tdav_codec_h264_bp10);
    }
    return self;
}
/* destructor */
static tsk_object_t* tdav_codec_h264_cuda_bp10_dtor(tsk_object_t * self)
{
    tdav_codec_h264_cuda_t *h264 = (tdav_codec_h264_cuda_t *)self;
    if(h264) {
        /* deinit base */
        tdav_codec_h264_common_deinit(TDAV_CODEC_H264_COMMON(self));
        /* deinit self */
        tdav_codec_h264_cuda_deinit(h264);
    }

    return self;
}
/* object definition */
static const tsk_object_def_t tdav_codec_h264_cuda_bp10_def_s = {
    sizeof(tdav_codec_h264_cuda_t),
    tdav_codec_h264_cuda_bp10_ctor,
    tdav_codec_h264_cuda_bp10_dtor,
    tmedia_codec_cmp,
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t tdav_codec_h264_cuda_bp10_plugin_def_s = {
    &tdav_codec_h264_cuda_bp10_def_s,

    tmedia_video,
    "H264",
    "H264 Base Profile 1.0 using CUDA",
    TMEDIA_CODEC_FORMAT_H264_BP10,
    tsk_true,
    90000, // rate

    /* audio */
    { 0 },

    /* video */
    {176, 144, 15},

    tdav_codec_h264_cuda_open,
    tdav_codec_h264_cuda_close,
    tdav_codec_h264_cuda_encode,
    tdav_codec_h264_cuda_decode,
    tdav_codec_h264_cuda_sdp_att_match,
    tdav_codec_h264_cuda_sdp_att_get,
    tdav_codec_h264_cuda_fmtp_set
};
extern const tmedia_codec_plugin_def_t *tdav_codec_h264_cuda_bp10_plugin_def_t = &tdav_codec_h264_cuda_bp10_plugin_def_s;


/* ============ H.264 Base Profile 2.0 Plugin interface ================= */

/* constructor */
static tsk_object_t* tdav_codec_h264_cuda_bp20_ctor(tsk_object_t * self, va_list * app)
{
    tdav_codec_h264_cuda_t *h264 = (tdav_codec_h264_cuda_t *)self;
    if(h264) {
        /* init base: called by tmedia_codec_create() */
        /* init self */
        tdav_codec_h264_cuda_init(h264, tdav_codec_h264_bp20);
    }
    return self;
}
/* destructor */
static tsk_object_t* tdav_codec_h264_cuda_bp20_dtor(tsk_object_t * self)
{
    tdav_codec_h264_cuda_t *h264 = (tdav_codec_h264_cuda_t *)self;
    if(h264) {
        /* deinit base */
        tdav_codec_h264_common_deinit(TDAV_CODEC_H264_COMMON(self));
        /* deinit self */
        tdav_codec_h264_cuda_deinit(h264);

    }

    return self;
}
/* object definition */
static const tsk_object_def_t tdav_codec_h264_cuda_bp20_def_s = {
    sizeof(tdav_codec_h264_cuda_t),
    tdav_codec_h264_cuda_bp20_ctor,
    tdav_codec_h264_cuda_bp20_dtor,
    tmedia_codec_cmp,
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t tdav_codec_h264_cuda_bp20_plugin_def_s = {
    &tdav_codec_h264_cuda_bp20_def_s,

    tmedia_video,
    "H264",
    "H264 Base Profile 2.0 using CUDA",
    TMEDIA_CODEC_FORMAT_H264_BP20,
    tsk_true,
    90000, // rate

    /* audio */
    { 0 },

    /* video */
    {352, 288, 15},

    tdav_codec_h264_cuda_open,
    tdav_codec_h264_cuda_close,
    tdav_codec_h264_cuda_encode,
    tdav_codec_h264_cuda_decode,
    tdav_codec_h264_cuda_sdp_att_match,
    tdav_codec_h264_cuda_sdp_att_get,
    tdav_codec_h264_cuda_fmtp_set
};
extern const tmedia_codec_plugin_def_t *tdav_codec_h264_cuda_bp20_plugin_def_t = &tdav_codec_h264_cuda_bp20_plugin_def_s;


/* ============ H.264 Base Profile 3.0 Plugin interface ================= */

/* constructor */
static tsk_object_t* tdav_codec_h264_cuda_bp30_ctor(tsk_object_t * self, va_list * app)
{
    tdav_codec_h264_cuda_t *h264 = (tdav_codec_h264_cuda_t *)self;
    if(h264) {
        /* init base: called by tmedia_codec_create() */
        /* init self */
        tdav_codec_h264_cuda_init(h264, tdav_codec_h264_bp30);
    }
    return self;
}
/* destructor */
static tsk_object_t* tdav_codec_h264_cuda_bp30_dtor(tsk_object_t * self)
{
    tdav_codec_h264_cuda_t *h264 = (tdav_codec_h264_cuda_t *)self;
    if(h264) {
        /* deinit base */
        tdav_codec_h264_common_deinit(TDAV_CODEC_H264_COMMON(self));
        /* deinit self */
        tdav_codec_h264_cuda_deinit(h264);

    }

    return self;
}
/* object definition */
static const tsk_object_def_t tdav_codec_h264_cuda_bp30_def_s = {
    sizeof(tdav_codec_h264_cuda_t),
    tdav_codec_h264_cuda_bp30_ctor,
    tdav_codec_h264_cuda_bp30_dtor,
    tmedia_codec_cmp,
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t tdav_codec_h264_cuda_bp30_plugin_def_s = {
    &tdav_codec_h264_cuda_bp30_def_s,

    tmedia_video,
    "H264",
    "H264 Base Profile 3.0 using CUDA",
    TMEDIA_CODEC_FORMAT_H264_BP30,
    tsk_true,
    90000, // rate

    /* audio */
    { 0 },

    /* video */
    {352, 288, 15},

    tdav_codec_h264_cuda_open,
    tdav_codec_h264_cuda_close,
    tdav_codec_h264_cuda_encode,
    tdav_codec_h264_cuda_decode,
    tdav_codec_h264_cuda_sdp_att_match,
    tdav_codec_h264_cuda_sdp_att_get,
    tdav_codec_h264_cuda_fmtp_set
};
extern const tmedia_codec_plugin_def_t *tdav_codec_h264_cuda_bp30_plugin_def_t = &tdav_codec_h264_cuda_bp30_plugin_def_s;


#endif /* HAVE_CUDA */
