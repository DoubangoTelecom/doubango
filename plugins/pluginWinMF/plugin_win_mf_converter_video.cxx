/* Copyright (C) 2013-2015 Mamadou DIOP
* Copyright (C) 2013-2015 Doubango Telecom <http://www.doubango.org>
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
/*
Video Processor MFT (http://msdn.microsoft.com/en-us/library/windows/desktop/hh162913(v=vs.85).aspx)
* The video processor supports GPU-accelerated video processing.
* The video processor MFT is a Microsoft Media Foundation transform (MFT) that performs :
	- colorspace conversion
	- video resizing
	- deinterlacing
	- frame rate conversion
	- rotation
	- cropping
	- spatial left and right view unpacking
	- and mirroring
*/
#include "plugin_win_mf_config.h"
#include "internals/mf_utils.h"

#include "tinymedia/tmedia_converter_video.h"

#include "tsk_memory.h"
#include "tsk_debug.h"

#include <assert.h>
#include <initguid.h>
#include <dmo.h>
#include <wmcodecdsp.h>

#ifdef _MSC_VER
#pragma comment(lib, "strmiids.lib")
#pragma comment(lib, "wmcodecdspuuid.lib")
#endif

EXTERN_C const GUID CLSID_VideoProcessorMFT; // defined in mf_utils.cxx

#if !defined(_WIN32_WINNT_WIN8)
#define _WIN32_WINNT_WIN8                   0x0602
#endif /* _WIN32_WINNT_WIN8 */

#if (WINVER < _WIN32_WINNT_WIN8)
DEFINE_GUID(MF_SA_D3D11_AWARE,
            0x206b4fc8, 0xfcf9, 0x4c51, 0xaf, 0xe3, 0x97, 0x64, 0x36, 0x9e, 0x33, 0xa0);
#endif /* MF_SA_D3D11_AWARE */

#if !defined(HAVE_IMFVideoProcessorControl)
#	if defined(__IMFVideoProcessorControl_INTERFACE_DEFINED__)
#		define HAVE_IMFVideoProcessorControl 1
#	else
#		define HAVE_IMFVideoProcessorControl 0
#	endif
#endif /* HAVE_IMFVideoProcessorControl */
#if !defined(E_BOUNDS)
#	define E_BOUNDS _HRESULT_TYPEDEF_(0x8000000BL)
#endif /* E_BOUNDS */
#if !defined(PLUGIN_MF_VC_FPS)
#define PLUGIN_MF_VC_FPS		120 // Samples requires timestamp
#endif /* PLUGIN_MF_VC_FPS */

typedef struct plugin_win_mf_converter_video_ms_s {
    TMEDIA_DECLARE_CONVERTER_VIDEO;

    GUID fmtSrc;
    tsk_size_t widthSrc;
    tsk_size_t heightSrc;

    GUID fmtDst;
    tsk_size_t widthDst;
    tsk_size_t heightDst;

    UINT32 rotation;
    UINT32 xOutputSize;
    UINT32 xInputSize;
    BOOL flip;

    IMFSample* pSampleOut;
    IMFSample* pSampleIn;

    LONGLONG rtStart;
    UINT64 rtDuration;

    IMFTransform* pMFT; // "CLSID_VideoProcessorMFT" or "CLSID_CColorConvertDMO"
#if HAVE_IMFVideoProcessorControl
    IMFVideoProcessorControl* pVPC;
#endif
    BOOL isVideoProcessor;
}
plugin_win_mf_converter_video_ms_t;

static inline const GUID& _plugin_win_mf_converter_video_ms_get_pixfmt(tmedia_chroma_t chroma);
static inline tsk_size_t _plugin_win_mf_converter_video_ms_get_size(tmedia_chroma_t chroma, tsk_size_t w, tsk_size_t h);
static inline HRESULT _plugin_win_mf_converter_video_ms_copy_rgb32_down_top(
    BYTE*       pDst,
    const BYTE* pSrc,
    INT       dwWidthInPixels,
    INT       dwHeightInPixels
);
static HRESULT _plugin_win_mf_converter_video_ms_process_input(plugin_win_mf_converter_video_ms_t* pSelf, IMFSample* pSample);
static HRESULT _plugin_win_mf_converter_video_ms_process_output(plugin_win_mf_converter_video_ms_t* pSelf, IMFSample **ppSample);
static HRESULT _plugin_win_mf_converter_video_ms_process(plugin_win_mf_converter_video_ms_t* pSelf, const void* pcInputPtr, UINT32 nInputSize, IMFSample **ppSampleOut);

static int plugin_win_mf_converter_video_ms_init(tmedia_converter_video_t* self, tsk_size_t srcWidth, tsk_size_t srcHeight, tmedia_chroma_t srcChroma, tsk_size_t dstWidth, tsk_size_t dstHeight, tmedia_chroma_t dstChroma)
{
    plugin_win_mf_converter_video_ms_t* pSelf = (plugin_win_mf_converter_video_ms_t*)self;
    TSK_DEBUG_INFO("Initializing new MF Video Converter src=(%dx%d@%d) dst=(%dx%d@%d)", srcWidth, srcHeight, srcChroma, dstWidth, dstHeight, dstChroma);

    if((pSelf->fmtSrc = _plugin_win_mf_converter_video_ms_get_pixfmt(srcChroma)) == GUID_NULL) {
        TSK_DEBUG_ERROR("Invalid source chroma");
        return -2;
    }
    if((pSelf->fmtDst = _plugin_win_mf_converter_video_ms_get_pixfmt(dstChroma)) == GUID_NULL) {
        TSK_DEBUG_ERROR("Invalid destination chroma");
        return -3;
    }

    pSelf->rtStart = 0;

    pSelf->widthSrc = srcWidth;
    pSelf->heightSrc = srcHeight;
    pSelf->widthDst = dstWidth;
    pSelf->heightDst = dstHeight;
    pSelf->rotation = 0;
    pSelf->xOutputSize = (UINT32)_plugin_win_mf_converter_video_ms_get_size(dstChroma, dstWidth, dstHeight);
    pSelf->xInputSize = (UINT32)_plugin_win_mf_converter_video_ms_get_size(srcChroma, srcWidth, srcHeight);

    SafeRelease(&pSelf->pSampleOut);
    SafeRelease(&pSelf->pSampleIn);
    SafeRelease(&pSelf->pMFT);
#if HAVE_IMFVideoProcessorControl
    SafeRelease(&pSelf->pVPC);
#endif

    HRESULT hr = S_OK;

    IMFMediaType* pTypeSrc = NULL;
    IMFMediaType* pTypeDst = NULL;

    // Get video processor or Color convertor
    hr = CoCreateInstance(CLSID_VideoProcessorMFT, NULL,
                          CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pSelf->pMFT));
    pSelf->isVideoProcessor = SUCCEEDED(hr);
    if(FAILED(hr)) {
        TSK_DEBUG_INFO("CoCreateInstance(CLSID_VideoProcessorMFT) failed");
        if(pSelf->widthSrc == pSelf->widthDst && pSelf->heightSrc == pSelf->heightDst) {
            TSK_DEBUG_INFO("No video scaling is required...perform CoCreateInstance(CLSID_CColorConvertDMO)");
            CHECK_HR(hr = CoCreateInstance(CLSID_CColorConvertDMO, NULL,
                                           CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pSelf->pMFT)));
        }
        else {
            CHECK_HR(hr);
        }
    }



    if(pSelf->isVideoProcessor) {
        IMFAttributes* pAttributes = NULL;
        UINT32 GPU = 0;
        hr = pSelf->pMFT->GetAttributes(&pAttributes);
        if (SUCCEEDED(hr)) {
            hr = pAttributes->GetUINT32(MF_SA_D3D11_AWARE, &GPU);
        }
        SafeRelease(&pAttributes);
        TSK_DEBUG_INFO("MF_SA_D3D11_AWARE = %d", GPU);
#if HAVE_IMFVideoProcessorControl
        CHECK_HR(hr = pSelf->pMFT->QueryInterface(IID_PPV_ARGS(&pSelf->pVPC)));
#endif
    }


    CHECK_HR(hr = MFUtils::CreateVideoType(&pSelf->fmtSrc, &pTypeSrc, (UINT32)pSelf->widthSrc, (UINT32)pSelf->heightSrc));
    CHECK_HR(hr = MFUtils::CreateVideoType(&pSelf->fmtDst, &pTypeDst, (UINT32)pSelf->widthDst, (UINT32)pSelf->heightDst));

    CHECK_HR(hr = pSelf->pMFT->SetInputType(0, pTypeSrc, 0));
    CHECK_HR(hr = pSelf->pMFT->SetOutputType(0, pTypeDst, 0));

bail:
    SafeRelease(&pTypeSrc);
    SafeRelease(&pTypeDst);

    if(FAILED(hr)) {
        SafeRelease(&pSelf->pMFT);
#if HAVE_IMFVideoProcessorControl
        SafeRelease(&pSelf->pVPC);
#endif
        return -4;
    }

    return 0;
}

static tsk_size_t plugin_win_mf_converter_video_ms_process(tmedia_converter_video_t* _self, const void* buffer, tsk_size_t buffer_size, void** output, tsk_size_t* output_max_size)
{
    plugin_win_mf_converter_video_ms_t* pSelf = (plugin_win_mf_converter_video_ms_t*)_self;

    HRESULT hr = S_OK;

    IMFSample *pSampleOut = NULL;
    IMFMediaBuffer* pBufferOut = NULL;

    if(!pSelf || !buffer || !output || !output_max_size) {
        CHECK_HR(hr = E_POINTER);
    }

    if(!pSelf->pMFT) {
        TSK_DEBUG_ERROR("Not initialized");
        CHECK_HR(hr = E_FAIL);
    }
#if HAVE_IMFVideoProcessorControl
    if(!pSelf->pVPC && pSelf->isVideoProcessor) {
        TSK_DEBUG_ERROR("Not initialized");
        CHECK_HR(hr = E_FAIL);
    }
#endif

    if(*output_max_size < pSelf->xOutputSize) {
        if(!(*output = tsk_realloc(*output, pSelf->xOutputSize))) {
            *output_max_size = 0;
            TSK_DEBUG_ERROR("Failed to allocate buffer with size = %u", pSelf->xOutputSize);
            CHECK_HR(hr = E_OUTOFMEMORY);
        }
        *output_max_size = pSelf->xOutputSize;
    }
#if HAVE_IMFVideoProcessorControl
    if(pSelf->pVPC && !!_self->flip != !!pSelf->flip) {
        pSelf->flip = !!_self->flip;
        CHECK_HR(hr = pSelf->pVPC->SetMirror(pSelf->flip ? MIRROR_NONE : MIRROR_VERTICAL));
    }
    if(pSelf->pVPC && _self->rotation != pSelf->rotation) {
        _self->rotation = pSelf->rotation;
        CHECK_HR(hr = pSelf->pVPC->SetRotation(pSelf->rotation == 0 ? ROTATION_NONE : ROTATION_NORMAL));

    }
#endif

    CHECK_HR(hr = _plugin_win_mf_converter_video_ms_process(
                      pSelf, buffer, pSelf->xInputSize, &pSampleOut));

    if(pSampleOut) {
        CHECK_HR(hr = pSampleOut->GetBufferByIndex(0, &pBufferOut));

        BYTE* pBufferPtr = NULL;
        DWORD dwDataLength = 0;
        CHECK_HR(hr = pBufferOut->GetCurrentLength(&dwDataLength));
        if(dwDataLength > 0) {
            if(dwDataLength != pSelf->xOutputSize) {
                TSK_DEBUG_ERROR("Output size mismatch");
                CHECK_HR(hr = E_BOUNDS);
            }
            CHECK_HR(hr = pBufferOut->Lock(&pBufferPtr, NULL, NULL));

            // MFCopyImage() is optimized: MMX, SSE, or SSE2
            switch(_self->dstChroma) {
                // Don't waste your time guessing which parameter to use: The consumer will always request RGB32. If not used for consumer then, just memcpy()
            case tmedia_chroma_rgb32: {
                if(pSelf->isVideoProcessor) {
                    hr = _plugin_win_mf_converter_video_ms_copy_rgb32_down_top(
                             (BYTE*)*output,
                             (const BYTE*)pBufferPtr,
                             (INT)pSelf->widthDst,
                             (INT)pSelf->heightDst
                         );
                }
                else {
                    hr = MFCopyImage(
                             (BYTE*)*output,
                             (LONG)(pSelf->widthDst << 2),
                             (BYTE*)pBufferPtr,
                             (LONG)(pSelf->widthDst << 2),
                             (DWORD)(pSelf->widthDst << 2),
                             (DWORD)pSelf->heightDst
                         );
                }


                if(FAILED(hr)) {
                    // unlock() before leaving
                    pBufferOut->Unlock();
                    CHECK_HR(hr);
                }
                break;
            }
            default: {
                memcpy(*output, pBufferPtr, dwDataLength);
            }
            }
            CHECK_HR(hr = pBufferOut->Unlock());
        }
    }

    pSelf->rtStart += pSelf->rtDuration;

bail:
    SafeRelease(&pSampleOut);
    SafeRelease(&pBufferOut);

    return SUCCEEDED(hr) ? pSelf->xOutputSize : 0;
}

static tsk_object_t* plugin_win_mf_converter_video_ms_ctor(tsk_object_t * self, va_list * app)
{
    plugin_win_mf_converter_video_ms_t *pSelf = (plugin_win_mf_converter_video_ms_t *)self;
    if(pSelf) {
        HRESULT hr = MFFrameRateToAverageTimePerFrame(PLUGIN_MF_VC_FPS, 1, &pSelf->rtDuration);
        if(FAILED(hr)) {
            pSelf->rtDuration = 83333; // 120 FPS
        }
    }
    return self;
}
static tsk_object_t* plugin_win_mf_converter_video_ms_dtor(tsk_object_t * self)
{
    plugin_win_mf_converter_video_ms_t *pSelf = (plugin_win_mf_converter_video_ms_t *)self;
    if(pSelf) {
        SafeRelease(&pSelf->pSampleOut);
        SafeRelease(&pSelf->pSampleIn);
        SafeRelease(&pSelf->pMFT);
#if HAVE_IMFVideoProcessorControl
        SafeRelease(&pSelf->pVPC);
#endif
    }

    return self;
}
static const tsk_object_def_t plugin_win_mf_converter_video_ms_def_s = {
    sizeof(plugin_win_mf_converter_video_ms_t),
    plugin_win_mf_converter_video_ms_ctor,
    plugin_win_mf_converter_video_ms_dtor,
    tsk_null,
};
const tsk_object_def_t *plugin_win_mf_converter_video_ms_def_t = &plugin_win_mf_converter_video_ms_def_s;
static const tmedia_converter_video_plugin_def_t plugin_win_mf_converter_video_ms_plugin_def_s = {
    &plugin_win_mf_converter_video_ms_def_s,

    plugin_win_mf_converter_video_ms_init,
    plugin_win_mf_converter_video_ms_process
};
const tmedia_converter_video_plugin_def_t *plugin_win_mf_converter_video_ms_plugin_def_t = &plugin_win_mf_converter_video_ms_plugin_def_s;


static inline tsk_size_t _plugin_win_mf_converter_video_ms_get_size(tmedia_chroma_t chroma, tsk_size_t w, tsk_size_t h)
{
    switch(chroma) {
    case tmedia_chroma_rgb24:
    case tmedia_chroma_bgr24:
        return (w * h * 3);
    case tmedia_chroma_rgb565le:
        return ((w * h) << 1);
    case tmedia_chroma_rgb32:
        return ((w * h) << 2);
    case tmedia_chroma_nv21:
        return ((w * h * 3) >> 1);
    case tmedia_chroma_nv12:
        return ((w * h * 3) >> 1);
    case tmedia_chroma_yuv422p:
        return ((w * h) << 1);
    case tmedia_chroma_uyvy422:
    case tmedia_chroma_yuyv422:
        return ((w * h) << 1);
    case tmedia_chroma_yuv420p:
        return ((w * h * 3) >> 1);
    default:
        TSK_DEBUG_ERROR("Invalid chroma %d", (int)chroma);
        return 0;
    }
}

static inline const GUID& _plugin_win_mf_converter_video_ms_get_pixfmt(tmedia_chroma_t chroma)
{
    switch(chroma) {
    case tmedia_chroma_rgb24:
    case tmedia_chroma_bgr24:
        return MFVideoFormat_RGB24;
    case tmedia_chroma_rgb565le:
        return MFVideoFormat_RGB565;
    case tmedia_chroma_rgb32:
        return MFVideoFormat_RGB32;
    case tmedia_chroma_nv12:
        return MFVideoFormat_NV12;
    case tmedia_chroma_yuv420p:
        return MFVideoFormat_I420;
    case tmedia_chroma_yuyv422:
        return MFVideoFormat_YUY2;
    case tmedia_chroma_uyvy422:
        return MFVideoFormat_UYVY;
    default:
        TSK_DEBUG_ERROR("Invalid chroma %d", (int)chroma);
        return GUID_NULL;
    }
}

// For RGB32:
// Direct3D -> Top-Down
// Video Processor -> Down-Top
static inline HRESULT _plugin_win_mf_converter_video_ms_copy_rgb32_down_top(
    BYTE*       pDst,
    const BYTE* pSrc,
    INT       dwWidthInPixels,
    INT       dwHeightInPixels
)
{
    RGBQUAD *pSrcPixel = &((RGBQUAD*)pSrc)[(dwWidthInPixels * dwHeightInPixels) - dwWidthInPixels];
    RGBQUAD *pDestPixel = &((RGBQUAD*)pDst)[0];

    register INT x;
    register INT y;

    for (y = dwHeightInPixels; y > 0 ; --y) {
        for (x = 0; x < dwWidthInPixels; ++x) {
            pDestPixel[x] = pSrcPixel[x];
        }
        pDestPixel += dwWidthInPixels;
        pSrcPixel -= dwWidthInPixels;
    }
    return S_OK;
}

static HRESULT _plugin_win_mf_converter_video_ms_process_input(plugin_win_mf_converter_video_ms_t* pSelf, IMFSample* pSample)
{
    return pSelf->pMFT->ProcessInput(0, pSample, 0);
}

static HRESULT _plugin_win_mf_converter_video_ms_process_output(plugin_win_mf_converter_video_ms_t* pSelf, IMFSample **ppSample)
{
    *ppSample = NULL;

    IMFMediaBuffer* pBufferOut = NULL;

    DWORD dwStatus;

    HRESULT hr = S_OK;

    MFT_OUTPUT_STREAM_INFO mftStreamInfo = { 0 };
    MFT_OUTPUT_DATA_BUFFER mftOutputData = { 0 };

    CHECK_HR(hr = pSelf->pMFT->GetOutputStreamInfo(0, &mftStreamInfo));

    if(!pSelf->pSampleOut) {
        CHECK_HR(hr = MFUtils::CreateMediaSample(mftStreamInfo.cbSize, &pSelf->pSampleOut));
        hr = pSelf->pSampleOut->GetBufferByIndex(0, &pBufferOut);
        if(FAILED(hr)) {
            SafeRelease(&pSelf->pSampleOut);
            CHECK_HR(hr);
        }
    }
    else {
        DWORD dwMaxLength = 0;
        CHECK_HR(hr = pSelf->pSampleOut->GetBufferByIndex(0, &pBufferOut));
        CHECK_HR(hr = pBufferOut->GetMaxLength(&dwMaxLength));
        if(dwMaxLength < mftStreamInfo.cbSize) {
            CHECK_HR(hr = pSelf->pSampleOut->RemoveAllBuffers());
            SafeRelease(&pBufferOut);
            CHECK_HR(hr = MFCreateMemoryBuffer(mftStreamInfo.cbSize, &pBufferOut));
            CHECK_HR(hr = pSelf->pSampleOut->AddBuffer(pBufferOut));
        }
    }

    CHECK_HR(hr = pBufferOut->SetCurrentLength(0));

    //Set the output sample
    mftOutputData.pSample = pSelf->pSampleOut;
    //Set the output id
    mftOutputData.dwStreamID = 0;

    //Generate the output sample
    CHECK_HR(hr = pSelf->pMFT->ProcessOutput(0, 1, &mftOutputData, &dwStatus));
    /*if (hr == MF_E_TRANSFORM_NEED_MORE_INPUT)
    {
        hr = S_OK;
        goto bail;
    }*/

    // TODO: Handle MF_E_TRANSFORM_STREAM_CHANGE

    *ppSample = pSelf->pSampleOut;
    (*ppSample)->AddRef();

bail:
    SafeRelease(&pBufferOut);
    return hr;
}

static HRESULT _plugin_win_mf_converter_video_ms_process(plugin_win_mf_converter_video_ms_t* pSelf, const void* pcInputPtr, UINT32 nInputSize, IMFSample **ppSampleOut)
{
    if(!pcInputPtr || !nInputSize || !ppSampleOut) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return E_INVALIDARG;
    }

    *ppSampleOut = NULL;

    HRESULT hr = S_OK;

    IMFMediaBuffer* pBufferIn = NULL;
    BYTE* pBufferPtr = NULL;

    if(!pSelf->pSampleIn) {
        CHECK_HR(hr = MFUtils::CreateMediaSample(nInputSize, &pSelf->pSampleIn));
        hr = pSelf->pSampleIn->GetBufferByIndex(0, &pBufferIn);
        if(FAILED(hr)) {
            SafeRelease(&pSelf->pSampleIn);
            CHECK_HR(hr);
        }
    }
    else {
        DWORD dwMaxLength = 0;
        CHECK_HR(hr = pSelf->pSampleIn->GetBufferByIndex(0, &pBufferIn));
        CHECK_HR(hr = pBufferIn->GetMaxLength(&dwMaxLength));
        if(dwMaxLength < nInputSize) {
            CHECK_HR(hr = pSelf->pSampleIn->RemoveAllBuffers());
            SafeRelease(&pBufferIn);
            CHECK_HR(hr = MFCreateMemoryBuffer(nInputSize, &pBufferIn));
            CHECK_HR(hr = pSelf->pSampleIn->AddBuffer(pBufferIn));
        }
    }

    CHECK_HR(hr = pBufferIn->Lock(&pBufferPtr, NULL, NULL));
    memcpy(pBufferPtr, pcInputPtr, nInputSize);
    CHECK_HR(hr = pBufferIn->Unlock());
    CHECK_HR(hr = pBufferIn->SetCurrentLength(nInputSize));

    CHECK_HR(hr = pSelf->pSampleIn->SetSampleDuration(pSelf->rtDuration));
    CHECK_HR(hr = pSelf->pSampleIn->SetSampleTime(pSelf->rtStart));

    hr = _plugin_win_mf_converter_video_ms_process_input(pSelf, pSelf->pSampleIn);
    while(hr == MF_E_NOTACCEPTING) {
        TSK_DEBUG_INFO("MF_E_NOTACCEPTING");
        IMFSample* pSample = NULL;
        hr = _plugin_win_mf_converter_video_ms_process_output(pSelf, &pSample);
        if(SUCCEEDED(hr) && pSample) {
            SafeRelease(ppSampleOut);
            *ppSampleOut = pSample, pSample = NULL;

            hr = pSelf->pSampleIn->SetUINT32(MFSampleExtension_Discontinuity, TRUE);
            hr = _plugin_win_mf_converter_video_ms_process_input(pSelf, pSelf->pSampleIn);
        }
    }
    if(!*ppSampleOut) {
        CHECK_HR(hr = _plugin_win_mf_converter_video_ms_process_output(pSelf, ppSampleOut));
    }

bail:
    SafeRelease(&pBufferIn);
    return hr;
}