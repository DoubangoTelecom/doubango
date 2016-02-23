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
#include "mf_utils.h"
#include "mf_codec.h"

#include "tinymedia/tmedia_common.h"

#include "tsk_debug.h"

#include <KS.h>/*  KS.H must be included before codecapi.H */
#include <Codecapi.h>
#include <initguid.h>
#include <wmcodecdsp.h>
#include <d3d9.h>
#include <assert.h>


#ifdef _MSC_VER
#pragma comment(lib, "strmiids.lib")
#pragma comment(lib, "wmcodecdspuuid.lib")
#pragma comment(lib, "d3d9")
#endif

#if !defined(PLUGIN_MF_DISABLE_CODECS)
// Must be "0" to use "Microsoft"/"Intel Quick Sync" MFT codecs. Testing: When set to "1", libx264 and FFmpeg will be used.
// Metropolis code (G2J.COM TelePresence client) has "PLUGIN_MF_DISABLE_CODECS=1" because of interop issues against Tandberg and Intel QuickSync H.264 implementations.
#	define PLUGIN_MF_DISABLE_CODECS 1
#endif
#if !defined(PLUGIN_MF_DISABLE_MS_H264_ENCODER)
// MS H.264 encoder produces artifacts when bundled with the producer. Disable until we found why this happens.
// What is strange is that NVIDIA CUDA H.264 decoder doesn't produce artifacts when decoding MS frames while FFmpeg and MS decoder do.
// To encode with MS and decode with CUDA:
// - Force "bMFEncoderIsRegistered" value to "FALSE" in plugin_win_mf_producer_video.cxx
// Metropolis code (G2J.COM TelePresence) has "PLUGIN_MF_DISABLE_MS_H264_ENCODER=1" beacause Microsoft H.264 not fully tested against Tandberg, Polycom, Hartallo...
#	define PLUGIN_MF_DISABLE_MS_H264_ENCODER 1
#endif

#if !defined(PLUGIN_MF_DISABLE_ASYNC_DECODERS)
// Not fully tested
#	define PLUGIN_MF_DISABLE_ASYNC_DECODERS 1
#endif

BOOL MFUtils::g_bStarted = FALSE;

DWORD MFUtils::g_dwMajorVersion = -1;
DWORD MFUtils::g_dwMinorVersion = -1;

BOOL MFUtils::g_bLowLatencyH264Checked = FALSE;
BOOL MFUtils::g_bLowLatencyH264Supported = FALSE;
BOOL MFUtils::g_bLowLatencyH264SupportsMaxSliceSize = FALSE;

BOOL MFUtils::g_bD3D9Checked = FALSE;
BOOL MFUtils::g_bD3D9Supported = FALSE;

const TOPOID MFUtils::g_ullTopoIdSinkMain = 111;
const TOPOID MFUtils::g_ullTopoIdSinkPreview = 222;
const TOPOID MFUtils::g_ullTopoIdSource = 333;
const TOPOID MFUtils::g_ullTopoIdVideoProcessor = 444;

// Preferred VideoSubTypes
static const VideoSubTypeGuidPair PreferredVideoSubTypeGuidPairs[] = {
    { tmedia_chroma_yuv420p, MFVideoFormat_I420 },
    { tmedia_chroma_nv12, MFVideoFormat_NV12 },
    { tmedia_chroma_uyvy422, MFVideoFormat_UYVY },
    { tmedia_chroma_yuyv422, MFVideoFormat_YUY2 },
    /* TODO: Add more YUV formats */
    { tmedia_chroma_rgb565le, MFVideoFormat_RGB565 },
    { tmedia_chroma_bgr24, MFVideoFormat_RGB24 },
    { tmedia_chroma_rgb32, MFVideoFormat_RGB32 },
};
static const tsk_size_t PreferredVideoSubTypeGuidPairsCount = sizeof(PreferredVideoSubTypeGuidPairs)/sizeof(PreferredVideoSubTypeGuidPairs[0]);

// Video Processor
DEFINE_GUID(CLSID_VideoProcessorMFT,
            0x88753b26, 0x5b24, 0x49bd, 0xb2, 0xe7, 0xc, 0x44, 0x5c, 0x78, 0xc9, 0x82);

// {4BE8D3C0-0515-4A37-AD55-E4BAE19AF471}
DEFINE_GUID(CLSID_MF_INTEL_H264EncFilter, // Intel Quick Sync Encoder
            0x4be8d3c0, 0x0515, 0x4a37, 0xad, 0x55, 0xe4, 0xba, 0xe1, 0x9a, 0xf4, 0x71);

// {0855C9AC-BC6F-4371-8954-671CCD4EC16F}
DEFINE_GUID(CLSID_MF_INTEL_H264DecFilter, // Intel Quick Sync Decoder
            0x0855c9ac, 0xbc6f, 0x4371, 0x89, 0x54, 0x67, 0x1c, 0xcd, 0x4e, 0xc1, 0x6f);

#if WINVER < 0x0602/* From "sdkddkver.h" and defines the SDK version not the host */
// 6ca50344-051a-4ded-9779-a43305165e35
DEFINE_GUID(CLSID_CMSH264EncoderMFT, // MS H.264 encoder
            0x6ca50344, 0x051a, 0x4ded, 0x97, 0x79, 0xa4, 0x33, 0x05, 0x16, 0x5e, 0x35);
#endif /* WINVER */

#define IsWin7_OrLater(dwMajorVersion, dwMinorVersion) ( (dwMajorVersion > 6) || ( (dwMajorVersion == 6) && (dwMinorVersion >= 1) ) )
#define IsWin8_OrLater(dwMajorVersion, dwMinorVersion) ( (dwMajorVersion > 6) || ( (dwMajorVersion == 6) && (dwMinorVersion >= 2) ) )


HRESULT MFUtils::Startup()
{
    if(!g_bStarted) {
        HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
        if(SUCCEEDED(hr) || hr == 0x80010106) { // 0x80010106 when called from managed code (e.g. Boghe) - More info: http://support.microsoft.com/kb/824480
            hr = MFStartup(MF_VERSION);
        }
        g_bStarted = SUCCEEDED(hr);

        OSVERSIONINFO osvi;
        ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
        osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
        GetVersionEx(&osvi);
        g_dwMajorVersion = osvi.dwMajorVersion;
        g_dwMinorVersion = osvi.dwMinorVersion;

        return hr;
    }
    return S_OK;
}

HRESULT MFUtils::Shutdown()
{
    if(g_bStarted) {
        g_bStarted = false;
        return S_OK;
    }
    return S_OK;
}

BOOL MFUtils::IsD3D9Supported()
{
    if (MFUtils::g_bD3D9Checked) {
        return MFUtils::g_bD3D9Supported;
    }
    MFUtils::g_bD3D9Checked = TRUE;
    HRESULT hr = S_OK;
    IDirect3D9* pD3D = NULL;
    D3DDISPLAYMODE mode = { 0 };
    D3DPRESENT_PARAMETERS pp = {0};
    IDirect3DDevice9* pDevice = NULL;

    CHECK_HR(hr = MFUtils::Startup());

    if (!(pD3D = Direct3DCreate9(D3D_SDK_VERSION))) {
        CHECK_HR(hr = E_OUTOFMEMORY);
    }

    hr = pD3D->GetAdapterDisplayMode(
             D3DADAPTER_DEFAULT,
             &mode
         );
    if (FAILED(hr)) {
        goto bail;
    }

    hr = pD3D->CheckDeviceType(
             D3DADAPTER_DEFAULT,
             D3DDEVTYPE_HAL,
             mode.Format,
             D3DFMT_X8R8G8B8,
             TRUE    // windowed
         );
    if (FAILED(hr)) {
        goto bail;
    }
    pp.BackBufferFormat = D3DFMT_X8R8G8B8;
    pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
    pp.Windowed = TRUE;
    pp.hDeviceWindow = GetDesktopWindow();
    hr = pD3D->CreateDevice(
             D3DADAPTER_DEFAULT,
             D3DDEVTYPE_HAL,
             pp.hDeviceWindow,
             D3DCREATE_HARDWARE_VERTEXPROCESSING,
             &pp,
             &pDevice
         );
    if (FAILED(hr)) {
        goto bail;
    }

    // Everythings is OK
    MFUtils::g_bD3D9Supported = TRUE;
    TSK_DEBUG_INFO("D3D9 supported");

bail:
    if (!MFUtils::g_bD3D9Supported) {
        TSK_DEBUG_WARN("D3D9 not supported");
    }
    SafeRelease(&pDevice);
    SafeRelease(&pD3D);
    return MFUtils::g_bD3D9Supported;
}

BOOL MFUtils::IsLowLatencyH264Supported()
{
    if(MFUtils::g_bLowLatencyH264Checked) {
        return MFUtils::g_bLowLatencyH264Supported;
    }

#if PLUGIN_MF_DISABLE_CODECS
    MFUtils::g_bLowLatencyH264Checked = TRUE;
    MFUtils::g_bLowLatencyH264Supported = FALSE;
#else
    Startup();

    HRESULT hr = S_OK;
    IMFTransform *pEncoderMFT = NULL;
    IMFTransform *pDecoderMFT = NULL;
    MFCodecVideoH264* pEncoderCodec = NULL;
    MFCodecVideoH264* pDecoderCodec = NULL;

    static const BOOL IsEncoderYes = TRUE;

    // Encoder
    hr = MFUtils::GetBestCodec(IsEncoderYes, MFMediaType_Video, MFVideoFormat_NV12, MFVideoFormat_H264, &pEncoderMFT);
    if(FAILED(hr)) {
        TSK_DEBUG_INFO("No low latency H.264 encoder");
        goto bail;
    }

    // Decoder
    hr = MFUtils::GetBestCodec(!IsEncoderYes, MFMediaType_Video, MFVideoFormat_H264, MFVideoFormat_NV12, &pDecoderMFT);
    if(FAILED(hr)) {
        TSK_DEBUG_INFO("No low latency H.264 decoder");
        goto bail;
    }

    // Make sure both encoder and decoder are working well. Check encoding/decoding 1080p@30 would work.

    TSK_DEBUG_INFO("Probing H.264 MFT encoder...");
    pEncoderCodec = MFCodecVideoH264::CreateCodecH264Main(MFCodecType_Encoder, pEncoderMFT);
    if(!pEncoderCodec) {
        CHECK_HR(hr = E_FAIL);
    }
    CHECK_HR(hr = pEncoderCodec->Initialize(
                      30, // FPS
                      1920, // WIDTH
                      1080, // HEIGHT
                      tmedia_get_video_bandwidth_kbps_2(1920, 1080, 30) * 1024) // BITRATE
            );
    CHECK_HR(pEncoderCodec->IsSetSliceMaxSizeInBytesSupported(MFUtils::g_bLowLatencyH264SupportsMaxSliceSize));

    TSK_DEBUG_INFO("Probing H.264 MFT decoder...");
    pDecoderCodec = MFCodecVideoH264::CreateCodecH264Main(MFCodecType_Decoder, pDecoderMFT);
    if(!pDecoderCodec) {
        CHECK_HR(hr = E_FAIL);
    }
    CHECK_HR(hr = pDecoderCodec->Initialize(
                      30, // FPS
                      1920, // WIDTH
                      1080 // HEIGHT
                  ));

bail:
    MFUtils::g_bLowLatencyH264Checked = TRUE;
    MFUtils::g_bLowLatencyH264Supported = SUCCEEDED(hr) ? TRUE : FALSE;
    SafeRelease(&pEncoderMFT);
    SafeRelease(&pEncoderCodec);
    SafeRelease(&pDecoderMFT);
    SafeRelease(&pDecoderCodec);
#endif /* PLUGIN_MF_DISABLE_CODECS */

    return MFUtils::g_bLowLatencyH264Supported;
}

BOOL MFUtils::IsLowLatencyH264SupportsMaxSliceSize()
{
    return MFUtils::IsLowLatencyH264Supported() && MFUtils::g_bLowLatencyH264SupportsMaxSliceSize;
}

HRESULT MFUtils::IsAsyncMFT(
    IMFTransform *pMFT, // The MFT to check
    BOOL* pbIsAsync // Whether the MFT is Async
)
{
    if(!pbIsAsync || !pMFT) {
        return E_POINTER;
    }

    IMFAttributes *pAttributes = NULL;
    UINT32 nIsAsync = 0;
    HRESULT hr = S_OK;

    hr = pMFT->GetAttributes(&pAttributes);
    if(SUCCEEDED(hr)) {
        hr = pAttributes->GetUINT32(MF_TRANSFORM_ASYNC, &nIsAsync);
    }

    // Never fails: just say not Async
    CHECK_HR(hr = S_OK);

    *pbIsAsync = !!nIsAsync;

bail:
    return hr;
}

HRESULT MFUtils::UnlockAsyncMFT(
    IMFTransform *pMFT // The MFT to unlock
)
{
    IMFAttributes *pAttributes = NULL;
    UINT32 nValue = 0;
    HRESULT hr = S_OK;

    hr = pMFT->GetAttributes(&pAttributes);
    if(FAILED(hr)) {
        hr = S_OK;
        goto bail;
    }

    hr = pAttributes->GetUINT32(MF_TRANSFORM_ASYNC, &nValue);
    if(FAILED(hr)) {
        hr = S_OK;
        goto bail;
    }

    if(nValue == TRUE) {
        CHECK_HR(hr = pAttributes->SetUINT32(MF_TRANSFORM_ASYNC_UNLOCK, TRUE));
    }

bail:
    SafeRelease(&pAttributes);
    return hr;
}
//-------------------------------------------------------------------
// CreatePCMAudioType
//
// Creates a media type that describes an uncompressed PCM audio
// format.
//-------------------------------------------------------------------

HRESULT MFUtils::CreatePCMAudioType(
    UINT32 sampleRate,        // Samples per second
    UINT32 bitsPerSample,     // Bits per sample
    UINT32 cChannels,         // Number of channels
    IMFMediaType **ppType     // Receives a pointer to the media type.
)
{
    HRESULT hr = S_OK;

    IMFMediaType *pType = NULL;

    // Calculate derived values.
    UINT32 blockAlign = cChannels * (bitsPerSample / 8);
    UINT32 bytesPerSecond = blockAlign * sampleRate;

    // Create the empty media type.
    hr = MFCreateMediaType(&pType);

    // Set attributes on the type.
    if (SUCCEEDED(hr)) {
        hr = pType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
    }

    if (SUCCEEDED(hr)) {
        hr = pType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
    }

    if (SUCCEEDED(hr)) {
        hr = pType->SetUINT32(MF_MT_AUDIO_NUM_CHANNELS, cChannels);
    }

    if (SUCCEEDED(hr)) {
        hr = pType->SetUINT32(MF_MT_AUDIO_SAMPLES_PER_SECOND, sampleRate);
    }

    if (SUCCEEDED(hr)) {
        hr = pType->SetUINT32(MF_MT_AUDIO_BLOCK_ALIGNMENT, blockAlign);
    }

    if (SUCCEEDED(hr)) {
        hr = pType->SetUINT32(MF_MT_AUDIO_AVG_BYTES_PER_SECOND, bytesPerSecond);
    }

    if (SUCCEEDED(hr)) {
        hr = pType->SetUINT32(MF_MT_AUDIO_BITS_PER_SAMPLE, bitsPerSample);
    }

    if (SUCCEEDED(hr)) {
        hr = pType->SetUINT32(MF_MT_ALL_SAMPLES_INDEPENDENT, TRUE);
    }

    if (SUCCEEDED(hr)) {
        // Return the type to the caller.
        *ppType = pType;
        (*ppType)->AddRef();
    }

    SafeRelease(&pType);
    return hr;
}


//-------------------------------------------------------------------
// CreateVideoType
//
// Creates a media type that describes a video subtype
// format.
//-------------------------------------------------------------------
HRESULT MFUtils::CreateVideoType(
    const GUID* subType, // video subType
    IMFMediaType **ppType,     // Receives a pointer to the media type.
    UINT32 unWidth, // Video width (0 to ignore)
    UINT32 unHeight // Video height (0 to ignore)
)
{
    HRESULT hr = S_OK;

    IMFMediaType *pType = NULL;

    CHECK_HR(hr = MFCreateMediaType(&pType));

    CHECK_HR(hr = pType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video));

    CHECK_HR(hr = pType->SetGUID(MF_MT_SUBTYPE, *subType));

    CHECK_HR(hr = pType->SetUINT32(MF_MT_ALL_SAMPLES_INDEPENDENT, TRUE)); // UnCompressed

    CHECK_HR(hr = pType->SetUINT32(MF_MT_FIXED_SIZE_SAMPLES, TRUE)); // UnCompressed

    CHECK_HR(hr = pType->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive));

    if(unWidth > 0 && unHeight > 0) {
        CHECK_HR(hr = MFSetAttributeSize(pType, MF_MT_FRAME_SIZE, unWidth, unHeight));
    }

    *ppType = pType;
    (*ppType)->AddRef();

bail:
    SafeRelease(&pType);
    return hr;
}

//-------------------------------------------------------------------
// Name: ValidateVideoFormat
// Description: Validates a media type for this sink.
//-------------------------------------------------------------------
HRESULT MFUtils::ValidateVideoFormat(IMFMediaType *pmt)
{
    GUID major_type = GUID_NULL;
    GUID subtype = GUID_NULL;
    MFVideoInterlaceMode interlace = MFVideoInterlace_Unknown;
    UINT32 val = 0;
    BOOL bFoundMatchingSubtype = FALSE;

    HRESULT hr = S_OK;

    // Major type must be video.
    CHECK_HR(hr = pmt->GetGUID(MF_MT_MAJOR_TYPE, &major_type));

    if (major_type != MFMediaType_Video) {
        CHECK_HR(hr = MF_E_INVALIDMEDIATYPE);
    }

    // Subtype must be one of the subtypes in our global list.

    // Get the subtype GUID.
    CHECK_HR(hr = pmt->GetGUID(MF_MT_SUBTYPE, &subtype));

#if 0
    // Look for the subtype in our list of accepted types.
    for (DWORD i = 0; i < g_NumVideoSubtypes; i++) {
        if (subtype == *g_VideoSubtypes[i]) {
            bFoundMatchingSubtype = TRUE;
            break;
        }
    }
    if (!bFoundMatchingSubtype) {
        CHECK_HR(hr = MF_E_INVALIDMEDIATYPE);
    }
#endif

    // Video must be progressive frames.
    CHECK_HR(hr = pmt->GetUINT32(MF_MT_INTERLACE_MODE, (UINT32*)&interlace));
    if (interlace != MFVideoInterlace_Progressive) {
        CHECK_HR(hr = MF_E_INVALIDMEDIATYPE);
    }

bail:
    return hr;
}

HRESULT MFUtils::ConvertVideoTypeToUncompressedType(
    IMFMediaType *pType,    // Pointer to an encoded video type.
    const GUID& subtype,    // Uncompressed subtype (eg, RGB-32, AYUV)
    IMFMediaType **ppType   // Receives a matching uncompressed video type.
)
{
    IMFMediaType *pTypeUncomp = NULL;

    HRESULT hr = S_OK;
    GUID majortype = { 0 };
    MFRatio par = { 0 };

    hr = pType->GetMajorType(&majortype);

    if (majortype != MFMediaType_Video) {
        return MF_E_INVALIDMEDIATYPE;
    }

    // Create a new media type and copy over all of the items.
    // This ensures that extended color information is retained.

    if (SUCCEEDED(hr)) {
        hr = MFCreateMediaType(&pTypeUncomp);
    }

    if (SUCCEEDED(hr)) {
        hr = pType->CopyAllItems(pTypeUncomp);
    }

    // Set the subtype.
    if (SUCCEEDED(hr)) {
        hr = pTypeUncomp->SetGUID(MF_MT_SUBTYPE, subtype);
    }

    // Uncompressed means all samples are independent.
    if (SUCCEEDED(hr)) {
        hr = pTypeUncomp->SetUINT32(MF_MT_ALL_SAMPLES_INDEPENDENT, TRUE);
    }

    // Fix up PAR if not set on the original type.
    if (SUCCEEDED(hr)) {
        hr = MFGetAttributeRatio(
                 pTypeUncomp,
                 MF_MT_PIXEL_ASPECT_RATIO,
                 (UINT32*)&par.Numerator,
                 (UINT32*)&par.Denominator
             );

        // Default to square pixels.
        if (FAILED(hr)) {
            hr = MFSetAttributeRatio(
                     pTypeUncomp,
                     MF_MT_PIXEL_ASPECT_RATIO,
                     1, 1
                 );
        }
    }

    if (SUCCEEDED(hr)) {
        *ppType = pTypeUncomp;
        (*ppType)->AddRef();
    }

    SafeRelease(&pTypeUncomp);
    return hr;
}

HRESULT MFUtils::CreateMediaSample(
    DWORD cbData, // Maximum buffer size
    IMFSample **ppSample // Receives the sample
)
{
    assert(ppSample);

    HRESULT hr = S_OK;

    IMFSample *pSample = NULL;
    IMFMediaBuffer *pBuffer = NULL;

    CHECK_HR(hr = MFCreateSample(&pSample));
    CHECK_HR(hr = MFCreateMemoryBuffer(cbData, &pBuffer));
    CHECK_HR(hr = pSample->AddBuffer(pBuffer));

    *ppSample = pSample;
    (*ppSample)->AddRef();

bail:
    SafeRelease(&pSample);
    SafeRelease(&pBuffer);
    return hr;
}

// Gets the best encoder and decoder. Up to the caller to release the returned pointer
HRESULT MFUtils::GetBestCodec(
    BOOL bEncoder, // Whether we request an encoder or not (TRUE=encoder, FALSE=decoder)
    const GUID& mediaType, // The MediaType
    const GUID& inputFormat, // The input MediaFormat (e.g. MFVideoFormat_NV12)
    const GUID& outputFormat, // The output MediaFormat (e.g. MFVideoFormat_H264)
    IMFTransform **ppMFT // Receives the decoder/encoder transform
)
{
    assert(ppMFT);
    assert(mediaType == MFMediaType_Video || mediaType == MFMediaType_Audio); // only audio and video codecs are support for now

    *ppMFT = NULL;

    HRESULT hr = S_OK;

    if(outputFormat == MFVideoFormat_H264 || inputFormat == MFVideoFormat_H264) {
        if(bEncoder) {
            // Force using Intel Quick Sync Encoder
            hr = CoCreateInstance(CLSID_MF_INTEL_H264EncFilter, NULL,
                                  CLSCTX_INPROC_SERVER, IID_PPV_ARGS(ppMFT));
            if(SUCCEEDED(hr) && *ppMFT) {
                TSK_DEBUG_INFO("Using Intel Quick Sync encoder :)");
                return hr;
            }
            TSK_DEBUG_INFO("Not using Intel Quick Sync encoder :(");
        }
        else {
#if !PLUGIN_MF_DISABLE_ASYNC_DECODERS // Intel Quick Sync decoder is asynchronous
            // Force using Intel Quick Sync Decoder
            hr = CoCreateInstance(CLSID_MF_INTEL_H264DecFilter, NULL,
                                  CLSCTX_INPROC_SERVER, IID_PPV_ARGS(ppMFT));
#endif
            if(SUCCEEDED(hr) && *ppMFT) {
                TSK_DEBUG_INFO("Using Intel Quick Sync decoder :)");
                return hr;
            }
            TSK_DEBUG_INFO("Not using Intel Quick Sync decoder :(");
        }
    }

    UINT32 count = 0;
    BOOL bAsync = FALSE;
    GUID guidActivateCLSID = GUID_NULL;

    IMFActivate **ppActivate = NULL;

    MFT_REGISTER_TYPE_INFO infoInput = { mediaType, inputFormat };
    MFT_REGISTER_TYPE_INFO infoOutput = { mediaType, outputFormat };

    UINT32 unFlags = MFT_ENUM_FLAG_HARDWARE |
                     MFT_ENUM_FLAG_SYNCMFT  |
                     MFT_ENUM_FLAG_ASYNCMFT |
                     MFT_ENUM_FLAG_LOCALMFT |
                     MFT_ENUM_FLAG_TRANSCODE_ONLY | // Otherwise Intel Quick Sync will not be listed
                     MFT_ENUM_FLAG_SORTANDFILTER;

    hr = MFTEnumEx(
             (mediaType == MFMediaType_Video) ? (bEncoder ? MFT_CATEGORY_VIDEO_ENCODER : MFT_CATEGORY_VIDEO_DECODER) : (bEncoder ? MFT_CATEGORY_AUDIO_ENCODER : MFT_CATEGORY_AUDIO_DECODER),
                 unFlags,
                 (inputFormat == GUID_NULL) ? NULL : &infoInput,      // Input type
                 (outputFormat == GUID_NULL) ? NULL : &infoOutput,       // Output type
                 &ppActivate,
                 &count
             );

    for(UINT32 i = 0; i < count; ++i) {
        SafeRelease(ppMFT);
        hr = ppActivate[i]->GetGUID(MFT_TRANSFORM_CLSID_Attribute, &guidActivateCLSID);
        if(FAILED(hr)) {
            continue;
        }

        if(bEncoder) {
            // Encoder
            if(guidActivateCLSID == CLSID_CMSH264EncoderMFT) { // MS H.264 encoder ?
                if(PLUGIN_MF_DISABLE_MS_H264_ENCODER) {
                    // Microsoft H.264 encoder is disabled
                    TSK_DEBUG_INFO("MS H.264 encoder is disabled...skipping");
                    continue;
                }
                if(!IsWin8_OrLater(g_dwMajorVersion, g_dwMinorVersion)) {
                    // Microsoft H.264 encoder doesn't support low latency on Win7.
                    TSK_DEBUG_INFO("MS H.264 encoder doesn't support low delay on (%ld, %ld)...skipping", g_dwMajorVersion, g_dwMinorVersion);
                    continue;
                }
            }
        }
        else {
            // Decoder
            if(guidActivateCLSID == CLSID_CMSH264DecoderMFT) { // MS H.264 decoder ?
                if(!IsWin8_OrLater(g_dwMajorVersion, g_dwMinorVersion)) {
                    // Microsoft H.264 decoder doesn't support low latency on Win7.
                    TSK_DEBUG_INFO("MS H.264 decoder doesn't support low delay on (%ld, %ld)...skipping", g_dwMajorVersion, g_dwMinorVersion);
                    continue;
                }
            }
        }

        hr = ppActivate[i]->ActivateObject(IID_PPV_ARGS(ppMFT));
        if(SUCCEEDED(hr) && *ppMFT) { // For now we just get the first one. FIXME: Give HW encoders/decoders higher priority.
            if(bEncoder) {
                // Encoder

            }
            else {
                // Decoder
#if PLUGIN_MF_DISABLE_ASYNC_DECODERS
                hr = IsAsyncMFT(*ppMFT, &bAsync);
                if(bAsync) {
                    TSK_DEBUG_INFO("Skipping async decoder because not supported yet");
                    continue; // Async decoders not supported yet
                }
#endif
            }
            break;
        }
    }

    for (UINT32 i = 0; i < count; i++) {
        ppActivate[i]->Release();
    }
    CoTaskMemFree(ppActivate);

    return *ppMFT ? S_OK : MF_E_NOT_FOUND;
}

HRESULT MFUtils::IsVideoProcessorSupported(BOOL *pbSupported)
{
    HRESULT hr = S_OK;
    IMFTransform *pTransform = NULL;

    if(!pbSupported) {
        CHECK_HR(hr = E_POINTER);
    }

    hr = CoCreateInstance(CLSID_VideoProcessorMFT, NULL,
                          CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pTransform));
    *pbSupported = SUCCEEDED(hr);
    if(FAILED(hr)) {
        hr = S_OK; // not an error
    }

bail:
    SafeRelease(&pTransform);
    return hr;
}

HRESULT MFUtils::GetBestVideoProcessor(
    const GUID& inputFormat, // The input MediaFormat (e.g. MFVideoFormat_I420)
    const GUID& outputFormat, // The output MediaFormat (e.g. MFVideoFormat_NV12)
    IMFTransform **ppProcessor // Receives the video processor
)
{
    assert(ppProcessor);

    *ppProcessor = NULL;

    HRESULT hr = S_OK;
    UINT32 count = 0;

    IMFActivate **ppActivate = NULL;

    MFT_REGISTER_TYPE_INFO infoInput = { MFMediaType_Video, inputFormat };
    MFT_REGISTER_TYPE_INFO infoOutput = { MFMediaType_Video, outputFormat };

    UINT32 unFlags = MFT_ENUM_FLAG_HARDWARE |
                     MFT_ENUM_FLAG_SYNCMFT  |
                     MFT_ENUM_FLAG_LOCALMFT |
                     MFT_ENUM_FLAG_SORTANDFILTER;

    hr = MFTEnumEx(
             MFT_CATEGORY_VIDEO_PROCESSOR,
             unFlags,
             &infoInput,      // Input type
             &infoOutput,       // Output type
             &ppActivate,
             &count
         );

    for(UINT32 i = 0; i < count; ++i) {
        hr = ppActivate[i]->ActivateObject(IID_PPV_ARGS(ppProcessor));
        if(SUCCEEDED(hr) && *ppProcessor) {
            break;
        }
        SafeRelease(ppProcessor);
    }

    for (UINT32 i = 0; i < count; i++) {
        ppActivate[i]->Release();
    }
    CoTaskMemFree(ppActivate);

    return *ppProcessor ? S_OK : MF_E_NOT_FOUND;
}

// Add an transform node to a topology.
HRESULT MFUtils::AddTransformNode(
    IMFTopology *pTopology,     // Topology.
    IMFTransform *pMFT,     // MFT.
    DWORD dwId,                 // Identifier of the stream sink.
    IMFTopologyNode **ppNode   // Receives the node pointer.
)
{
    *ppNode = NULL;

    IMFTopologyNode *pNode = NULL;
    HRESULT hr = S_OK;

    // Create the node.
    CHECK_HR(hr = MFCreateTopologyNode(MF_TOPOLOGY_TRANSFORM_NODE, &pNode));
    // Set the object pointer.
    CHECK_HR(hr = pNode->SetObject(pMFT));
    CHECK_HR(hr = pNode->SetUINT32(MF_TOPONODE_STREAMID, dwId));
    CHECK_HR(hr = pNode->SetUINT32(MF_TOPONODE_NOSHUTDOWN_ON_REMOVE, FALSE));
    // Add the node to the topology.
    CHECK_HR(hr = pTopology->AddNode(pNode));

    // Return the pointer to the caller.
    *ppNode = pNode;
    (*ppNode)->AddRef();

bail:
    SafeRelease(&pNode);
    return hr;
}

// Sets the IMFStreamSink pointer on an output node.
HRESULT MFUtils::BindOutputNode(
    IMFTopologyNode *pNode // The Node
)
{
    assert(pNode);

    HRESULT hr = S_OK;
    IUnknown *pNodeObject = NULL;
    IMFActivate *pActivate = NULL;
    IMFStreamSink *pStream = NULL;
    IMFMediaSink *pSink = NULL;

    // Get the node's object pointer.
    CHECK_HR(hr = pNode->GetObject(&pNodeObject));

    // The object pointer should be one of the following:
    // 1. An activation object for the media sink.
    // 2. The stream sink.

    // If it's #2, then we're already done.

    // First, check if it's an activation object.
    CHECK_HR(hr = pNodeObject->QueryInterface(IID_PPV_ARGS(&pActivate)));

    if (SUCCEEDED(hr)) {
        DWORD dwStreamID = 0;

        // The object pointer is an activation object.

        // Try to create the media sink.
        hr = pActivate->ActivateObject(IID_PPV_ARGS(&pSink));

        // Look up the stream ID. (Default to zero.)

        if (SUCCEEDED(hr)) {
            dwStreamID = MFGetAttributeUINT32(pNode, MF_TOPONODE_STREAMID, 0);
        }

        // Now try to get or create the stream sink.

        // Check if the media sink already has a stream sink with the requested ID.

        if (SUCCEEDED(hr)) {
            hr = pSink->GetStreamSinkById(dwStreamID, &pStream);
            if (FAILED(hr)) {
                // Try to add a new stream sink.
                hr = pSink->AddStreamSink(dwStreamID, NULL, &pStream);
            }
        }

        // Replace the node's object pointer with the stream sink.
        if (SUCCEEDED(hr)) {
            hr = pNode->SetObject(pStream);
        }
    }
    else {
        // Not an activation object. Is it a stream sink?
        hr = pNodeObject->QueryInterface(IID_PPV_ARGS(&pStream));
    }

bail:
    SafeRelease(&pNodeObject);
    SafeRelease(&pActivate);
    SafeRelease(&pStream);
    SafeRelease(&pSink);
    return hr;
}

// Add an output node to a topology.
HRESULT MFUtils::AddOutputNode(
    IMFTopology *pTopology,     // Topology.
    IMFActivate *pActivate,     // Media sink activation object.
    DWORD dwId,                 // Identifier of the stream sink.
    IMFTopologyNode **ppNode)   // Receives the node pointer
{
    IMFTopologyNode *pNode = NULL;

    HRESULT hr = S_OK;
    CHECK_HR(hr = MFCreateTopologyNode(MF_TOPOLOGY_OUTPUT_NODE, &pNode));
    CHECK_HR(hr = pNode->SetObject(pActivate));
    CHECK_HR(hr = pNode->SetUINT32(MF_TOPONODE_STREAMID, dwId));
    CHECK_HR(hr = pNode->SetUINT32(MF_TOPONODE_NOSHUTDOWN_ON_REMOVE, FALSE));
    CHECK_HR(hr = pTopology->AddNode(pNode));

    // Return the pointer to the caller.
    *ppNode = pNode;
    (*ppNode)->AddRef();

bail:
    SafeRelease(&pNode);
    return hr;
}

// Add a source node to a topology
HRESULT MFUtils::AddSourceNode(
    IMFTopology *pTopology,           // Topology.
    IMFMediaSource *pSource,          // Media source.
    IMFPresentationDescriptor *pPD,   // Presentation descriptor.
    IMFStreamDescriptor *pSD,         // Stream descriptor.
    IMFTopologyNode **ppNode          // Receives the node pointer.
)
{
    IMFTopologyNode *pNode = NULL;

    HRESULT hr = S_OK;
    CHECK_HR(hr = MFCreateTopologyNode(MF_TOPOLOGY_SOURCESTREAM_NODE, &pNode));
    CHECK_HR(hr = pNode->SetUnknown(MF_TOPONODE_SOURCE, pSource));
    CHECK_HR(hr = pNode->SetUnknown(MF_TOPONODE_PRESENTATION_DESCRIPTOR, pPD));
    CHECK_HR(hr = pNode->SetUnknown(MF_TOPONODE_STREAM_DESCRIPTOR, pSD));
    CHECK_HR(hr = pTopology->AddNode(pNode));

    // Return the pointer to the caller.
    *ppNode = pNode;
    (*ppNode)->AddRef();

bail:
    SafeRelease(&pNode);
    return hr;
}

// Create the topology
//
// [source] -> (Transform) -> [SinkMain]
//         \-> (SinkPreview)
//
HRESULT MFUtils::CreateTopology(
    IMFMediaSource *pSource, // Media source
    IMFTransform *pTransform, // Transform filter (e.g. encoder or decoder) to insert between the source and Sink. NULL is valid.
    IMFActivate *pSinkActivateMain, // Main sink (e.g. sample grabber or EVR).
    IMFActivate *pSinkActivatePreview, // Preview sink. Optional. Could be NULL.
    IMFMediaType *pIputTypeMain, // Main sink input MediaType
    IMFTopology **ppTopo // Receives the newly created topology
)
{
    IMFTopology *pTopology = NULL;
    IMFPresentationDescriptor *pPD = NULL;
    IMFStreamDescriptor *pSD = NULL;
    IMFMediaTypeHandler *pHandler = NULL;
    IMFTopologyNode *pNodeSource = NULL;
    IMFTopologyNode *pNodeSinkMain = NULL;
    IMFTopologyNode *pNodeSinkPreview = NULL;
    IMFTopologyNode *pNodeTransform = NULL;
    IMFTopologyNode *pNodeTee = NULL;
    IMFMediaType *pMediaType = NULL;
    IMFTransform *pVideoProcessor = NULL;
    IMFTopologyNode *pNodeVideoProcessor = NULL;
    IMFTransform *pConvFrameRate = NULL;
    IMFTransform *pConvSize = NULL;
    IMFTransform *pConvColor = NULL;
    IMFTopologyNode *pNodeConvFrameRate = NULL;
    IMFTopologyNode *pNodeConvSize = NULL;
    IMFTopologyNode *pNodeConvColor = NULL;
    IMFMediaType *pTransformInputType = NULL;
    IMFMediaType *pSinkMainInputType = NULL;
    const IMFTopologyNode *pcNodeBeforeSinkMain = NULL;

    HRESULT hr = S_OK;
    DWORD cStreams = 0;
    BOOL bSourceFound = FALSE;
    BOOL bSupportedSize = FALSE;
    BOOL bSupportedFps = FALSE;
    BOOL bSupportedFormat = FALSE;
    BOOL bVideoProcessorSupported = FALSE;
    GUID inputMajorType, inputSubType;

    CHECK_HR(hr = IsVideoProcessorSupported(&bVideoProcessorSupported));
    CHECK_HR(hr = pIputTypeMain->GetMajorType(&inputMajorType));

    CHECK_HR(hr = MFCreateTopology(&pTopology));
    CHECK_HR(hr = pSource->CreatePresentationDescriptor(&pPD));
    CHECK_HR(hr = pPD->GetStreamDescriptorCount(&cStreams));

    for (DWORD i = 0; i < cStreams; i++) {
        BOOL fSelected = FALSE;
        GUID majorType;

        CHECK_HR(hr = pPD->GetStreamDescriptorByIndex(i, &fSelected, &pSD));
        CHECK_HR(hr = pSD->GetMediaTypeHandler(&pHandler));
        CHECK_HR(hr = pHandler->GetMajorType(&majorType));

        if (majorType == inputMajorType && fSelected) {
            CHECK_HR(hr = AddSourceNode(pTopology, pSource, pPD, pSD, &pNodeSource));
            CHECK_HR(hr = pNodeSource->SetTopoNodeID(MFUtils::g_ullTopoIdSource));
            CHECK_HR(hr = AddOutputNode(pTopology, pSinkActivateMain, 0, &pNodeSinkMain));
            CHECK_HR(hr = pNodeSinkMain->SetTopoNodeID(MFUtils::g_ullTopoIdSinkMain));
            CHECK_HR(hr = MFUtils::BindOutputNode(pNodeSinkMain)); // To avoid MF_E_TOPO_SINK_ACTIVATES_UNSUPPORTED

            //
            // Create preview
            //

            if(pSinkActivatePreview) {
                CHECK_HR(hr = AddOutputNode(pTopology, pSinkActivatePreview, 0, &pNodeSinkPreview));
                CHECK_HR(hr = pNodeSinkPreview->SetTopoNodeID(MFUtils::g_ullTopoIdSinkPreview));
                CHECK_HR(hr = MFUtils::BindOutputNode(pNodeSinkPreview));

                CHECK_HR(hr = MFCreateTopologyNode(MF_TOPOLOGY_TEE_NODE, &pNodeTee));
                CHECK_HR(hr = pTopology->AddNode(pNodeTee));
            }

            //
            // Create converters
            //

            if(majorType == MFMediaType_Video) {
                // Even when size matches the topology could add a resizer which doesn't keep ratio when resizing while video processor does.
                if(!bVideoProcessorSupported) {
                    hr = IsSupported(
                             pPD,
                             i,
                             pIputTypeMain,
                             &bSupportedSize,
                             &bSupportedFps,
                             &bSupportedFormat);
                }

                CHECK_HR(hr = pIputTypeMain->GetGUID(MF_MT_SUBTYPE, &inputSubType));

                if(!bSupportedSize || !bSupportedFps || !bSupportedFormat) {
                    // Use video processor single MFT or 3 different MFTs
                    if(!pVideoProcessor) {
                        hr = CoCreateInstance(CLSID_VideoProcessorMFT, NULL,
                                              CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pVideoProcessor));
                    }
                    if(!pVideoProcessor) {
                        // Video Resizer DSP(http://msdn.microsoft.com/en-us/library/windows/desktop/ff819491(v=vs.85).aspx) supports I420 only
                        if(!bSupportedSize && !pConvSize && inputSubType == MFVideoFormat_I420) {
                            hr = CoCreateInstance(CLSID_CResizerDMO, NULL,
                                                  CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pConvSize));
                        }
                        // Frame Rate Converter DSP(http://msdn.microsoft.com/en-us/library/windows/desktop/ff819100(v=vs.85).aspx) supports neither NV12 nor I420
                        /*if(!bSupportedFps && !pConvFrameRate)
                        {
                        	hr = CoCreateInstance(CLSID_CFrameRateConvertDmo, NULL,
                        		CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pConvFrameRate));
                        }*/
                        // Color Converter DSP (http://msdn.microsoft.com/en-us/library/windows/desktop/ff819079(v=vs.85).aspx) supports both NV12 and I420
                        if(!bSupportedFormat && !pConvColor) {
                            hr = CoCreateInstance(CLSID_CColorConvertDMO, NULL,
                                                  CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pConvColor));
                        }
                    }
                }
                else {
                    // MediaType supported
                    CHECK_HR(hr = pHandler->SetCurrentMediaType(pIputTypeMain));
                }

                if(pVideoProcessor && !pNodeVideoProcessor) {
                    CHECK_HR(hr = AddTransformNode(pTopology, pVideoProcessor, 0, &pNodeVideoProcessor));
                    CHECK_HR(hr = pNodeVideoProcessor->SetTopoNodeID(MFUtils::g_ullTopoIdVideoProcessor));
                }
                if(pConvColor && !pNodeConvColor) {
                    CHECK_HR(hr = AddTransformNode(pTopology, pConvColor, 0, &pNodeConvColor));
                }
                if(pConvFrameRate && !pNodeConvFrameRate) {
                    CHECK_HR(hr = AddTransformNode(pTopology, pConvFrameRate, 0, &pNodeConvFrameRate));
                }
                if(pConvSize && !pNodeConvSize) {
                    CHECK_HR(hr = AddTransformNode(pTopology, pConvSize, 0, &pNodeConvSize));
                }
            } // if(majorType == MFMediaType_Video)


            //
            // Set media type
            //

            if(pTransform) {
                CHECK_HR(hr = AddTransformNode(pTopology, pTransform, 0, &pNodeTransform));
                hr = pTransform->GetInputCurrentType(0, &pTransformInputType);
                if(FAILED(hr)) {
                    pTransformInputType = pIputTypeMain;
                    pTransformInputType->AddRef();
                    hr = S_OK;
                }
                if(pVideoProcessor) {
                    CHECK_HR(hr = pVideoProcessor->SetOutputType(0, pTransformInputType, 0));
                }
                else {
                    if(pConvColor) {
                        /*CHECK_HR*/(hr = pConvColor->SetOutputType(0, pTransformInputType, 0));
                    }
                    if(pConvFrameRate) {
                        /*CHECK_HR*/(hr = pConvFrameRate->SetOutputType(0, pTransformInputType, 0));
                    }
                    if(pConvSize) {
                        // Transform requires NV12
                        //Video Resizer DSP(http://msdn.microsoft.com/en-us/library/windows/desktop/ff819491(v=vs.85).aspx) doesn't support NV12
                        //*CHECK_HR*/(hr = pConvSize->SetOutputType(0, pTransformInputType, 0));
                    }
                }
            }
            else {
                hr = pNodeSinkMain->GetInputPrefType(0, &pSinkMainInputType);
                if(FAILED(hr)) {
                    pSinkMainInputType = pIputTypeMain;
                    pSinkMainInputType->AddRef();
                    hr = S_OK;
                }
                if(SUCCEEDED(hr)) {
                    if(pVideoProcessor) {
                        CHECK_HR(hr = pVideoProcessor->SetOutputType(0, pSinkMainInputType, 0));
                    }
                    else {
                        //!\ MUST NOT SET OUTPUT TYPE
                        if(pConvColor) {
                            //*CHECK_HR*/(hr = pConvColor->SetOutputType(0, pSinkMainInputType, 0));
                        }
                        if(pConvFrameRate) {
                            //*CHECK_HR*/(hr = pConvFrameRate->SetOutputType(0, pSinkMainInputType, 0));
                        }
                        if(pConvSize) {
                            //*CHECK_HR*/(hr = pConvSize->SetOutputType(0, pSinkMainInputType, 0));
                        }
                    }
                }
            }

            //
            // Connect
            //

            if(pNodeTee) {
                // Connect(Source -> Tee)
                CHECK_HR(hr = pNodeSource->ConnectOutput(0, pNodeTee, 0));

                // Connect(Tee -> SinkPreview)
                CHECK_HR(hr = pNodeTee->ConnectOutput(1, pNodeSinkPreview, 0));

                // Connect(Tee ->(Processors)
                if(pVideoProcessor) {
                    CHECK_HR(hr = pNodeTee->ConnectOutput(0, pNodeVideoProcessor, 0));
                    pcNodeBeforeSinkMain = pNodeVideoProcessor;
                }
                else if(pNodeConvFrameRate || pNodeConvSize || pNodeConvColor) {
                    CHECK_HR(hr = ConnectConverters(
                                      pNodeTee,
                                      0,
                                      pNodeConvFrameRate,
                                      pNodeConvColor,
                                      pNodeConvSize
                                  ));
                    pcNodeBeforeSinkMain = pNodeConvSize ? pNodeConvSize : (pNodeConvColor ? pNodeConvColor : pNodeConvFrameRate);
                }
                else {
                    pcNodeBeforeSinkMain = pNodeTee;
                }
            }
            else {
                // Connect(Source -> (Processors))
                if(pVideoProcessor) {
                    CHECK_HR(hr = pNodeSource->ConnectOutput(0, pNodeVideoProcessor, 0));
                    pcNodeBeforeSinkMain = pNodeVideoProcessor;
                }
                else if(pNodeConvFrameRate || pNodeConvFrameRate || pNodeConvColor) {
                    CHECK_HR(hr = ConnectConverters(
                                      pNodeSource,
                                      0,
                                      pNodeConvFrameRate,
                                      pNodeConvSize,
                                      pNodeConvColor
                                  ));
                    pcNodeBeforeSinkMain = pNodeConvSize ? pNodeConvSize : (pNodeConvColor ? pNodeConvColor : pNodeConvFrameRate);
                }
                else {
                    pcNodeBeforeSinkMain = pNodeSource;
                }
            }


            if(pNodeTransform) {
                // Connect(X->Transform)
                CHECK_HR(hr = ((IMFTopologyNode *)pcNodeBeforeSinkMain)->ConnectOutput(0, pNodeTransform, 0));
                pcNodeBeforeSinkMain = pNodeTransform;
            }

            // Connect(X -> SinkMain)
            CHECK_HR(hr = ((IMFTopologyNode *)pcNodeBeforeSinkMain)->ConnectOutput(0, pNodeSinkMain, 0));

            bSourceFound = TRUE;
            break;
        }
        else {
            CHECK_HR(hr = pPD->DeselectStream(i));
        }
        SafeRelease(&pSD);
        SafeRelease(&pHandler);
    }

    *ppTopo = pTopology;
    (*ppTopo)->AddRef();

bail:
    SafeRelease(&pTopology);
    SafeRelease(&pNodeSource);
    SafeRelease(&pNodeSinkMain);
    SafeRelease(&pNodeSinkPreview);
    SafeRelease(&pNodeTransform);
    SafeRelease(&pNodeTee);
    SafeRelease(&pPD);
    SafeRelease(&pSD);
    SafeRelease(&pHandler);
    SafeRelease(&pMediaType);
    SafeRelease(&pTransformInputType);
    SafeRelease(&pSinkMainInputType);

    SafeRelease(&pVideoProcessor);
    SafeRelease(&pNodeVideoProcessor);
    SafeRelease(&pConvFrameRate);
    SafeRelease(&pConvSize);
    SafeRelease(&pConvColor);
    SafeRelease(&pNodeConvFrameRate);
    SafeRelease(&pNodeConvSize);
    SafeRelease(&pNodeConvColor);

    if(!bSourceFound) {
        TSK_DEBUG_ERROR("No source node found");
        return E_NOT_SET;
    }

    return hr;
}

// Creates a fully loaded topology from the input partial topology.
HRESULT MFUtils::ResolveTopology(
    IMFTopology *pInputTopo, // A pointer to the IMFTopology interface of the partial topology to be resolved.
    IMFTopology **ppOutputTopo, // Receives a pointer to the IMFTopology interface of the completed topology. The caller must release the interface.
    IMFTopology *pCurrentTopo /*= NULL*/ // A pointer to the IMFTopology interface of the previous full topology. The topology loader can re-use objects from this topology in the new topology. This parameter can be NULL.
)
{
    assert(ppOutputTopo && pInputTopo);

    HRESULT hr = S_OK;
    IMFTopoLoader* pTopoLoader = NULL;

    *ppOutputTopo = NULL;

    CHECK_HR(hr = MFCreateTopoLoader(&pTopoLoader));
    CHECK_HR(hr = pTopoLoader->Load(pInputTopo, ppOutputTopo, pCurrentTopo));

bail:
    SafeRelease(&pTopoLoader);
    return hr;
}

HRESULT MFUtils::FindNodeObject(
    IMFTopology *pInputTopo, // The Topology containing the node to find
    TOPOID qwTopoNodeID, //The identifier for the node
    void** ppObject // Receives the Object
)
{
    assert(pInputTopo && ppObject);

    *ppObject = NULL;

    IMFTopologyNode *pNode = NULL;
    HRESULT hr = S_OK;

    CHECK_HR(hr = pInputTopo->GetNodeByID(qwTopoNodeID, &pNode));
    CHECK_HR(hr = pNode->GetObject((IUnknown**)ppObject));

bail:
    SafeRelease(&pNode);
    return hr;
}

//  Create an activation object for a renderer, based on the stream media type.
HRESULT MFUtils::CreateMediaSinkActivate(
    IMFStreamDescriptor *pSourceSD,     // Pointer to the stream descriptor.
    HWND hVideoWindow,                  // Handle to the video clipping window.
    IMFActivate **ppActivate
)
{
    HRESULT hr = S_OK;
    IMFMediaTypeHandler *pHandler = NULL;
    IMFActivate *pActivate = NULL;

    // Get the media type handler for the stream.
    CHECK_HR(hr = pSourceSD->GetMediaTypeHandler(&pHandler));
    // Get the major media type.
    GUID guidMajorType;
    CHECK_HR(hr = pHandler->GetMajorType(&guidMajorType));

    // Create an IMFActivate object for the renderer, based on the media type.
    if (MFMediaType_Audio == guidMajorType) {
        // Create the audio renderer.
        CHECK_HR(hr = MFCreateAudioRendererActivate(&pActivate));
    }
    else if (MFMediaType_Video == guidMajorType) {
        // Create the video renderer.
        CHECK_HR(hr = MFCreateVideoRendererActivate(hVideoWindow, &pActivate));
    }
    else {
        // Unknown stream type.
        hr = E_FAIL;
        // Optionally, you could deselect this stream instead of failing.
    }
    if (FAILED(hr)) {
        goto bail;
    }

    // Return IMFActivate pointer to caller.
    *ppActivate = pActivate;
    (*ppActivate)->AddRef();

bail:
    SafeRelease(&pHandler);
    SafeRelease(&pActivate);
    return hr;
}

// Set source output media type
HRESULT MFUtils::SetMediaType(
    IMFMediaSource *pSource,        // Media source.
    IMFMediaType* pMediaType // Media Type.
)
{
    assert(pSource && pMediaType);

    IMFPresentationDescriptor *pPD = NULL;
    IMFStreamDescriptor *pSD = NULL;
    IMFMediaTypeHandler *pHandler = NULL;

    HRESULT hr = S_OK;
    DWORD cStreams = 0;
    GUID inputMajorType;

    CHECK_HR(hr = pSource->CreatePresentationDescriptor(&pPD));
    CHECK_HR(hr = pPD->GetStreamDescriptorCount(&cStreams));
    CHECK_HR(hr = pMediaType->GetMajorType(&inputMajorType));

    for (DWORD i = 0; i < cStreams; i++) {
        BOOL fSelected = FALSE;
        GUID majorType;

        CHECK_HR(hr = pPD->GetStreamDescriptorByIndex(i, &fSelected, &pSD));
        CHECK_HR(hr = pSD->GetMediaTypeHandler(&pHandler));
        CHECK_HR(hr = pHandler->GetMajorType(&majorType));

        if (majorType == inputMajorType && fSelected) {
            CHECK_HR(hr = pHandler->SetCurrentMediaType(pMediaType));
        }
        else {
            CHECK_HR(hr = pPD->DeselectStream(i));
        }
        SafeRelease(&pSD);
        SafeRelease(&pHandler);
    }


bail:
    SafeRelease(&pPD);
    SafeRelease(&pSD);
    SafeRelease(&pHandler);

    return hr;
}

HRESULT MFUtils::SetVideoWindow(
    IMFTopology *pTopology,         // Topology.
    IMFMediaSource *pSource,        // Media source.
    HWND hVideoWnd                 // Window for video playback.
)
{
    HRESULT hr = S_OK;
    IMFStreamDescriptor *pSD = NULL;
    IMFPresentationDescriptor *pPD = NULL;
    IMFActivate         *pSinkActivate = NULL;
    IMFTopologyNode     *pSourceNode = NULL;
    IMFTopologyNode     *pOutputNode = NULL;
    DWORD cStreams = 0, iStream;

    CHECK_HR(hr = pSource->CreatePresentationDescriptor(&pPD));
    CHECK_HR(hr = pPD->GetStreamDescriptorCount(&cStreams));

    for(iStream = 0; iStream < cStreams; ++iStream) {
        BOOL fSelected = FALSE;

        CHECK_HR(hr = pPD->GetStreamDescriptorByIndex(iStream, &fSelected, &pSD));

        if (fSelected) {
            // Create the media sink activation object.
            CHECK_HR(hr = CreateMediaSinkActivate(pSD, hVideoWnd, &pSinkActivate));
            // Add a source node for this stream.
            CHECK_HR(hr = AddSourceNode(pTopology, pSource, pPD, pSD, &pSourceNode));
            // Create the output node for the renderer.
            CHECK_HR(hr = AddOutputNode(pTopology, pSinkActivate, 0, &pOutputNode));
            // Connect the source node to the output node.
            CHECK_HR(hr = pSourceNode->ConnectOutput(0, pOutputNode, 0));
        }
        // else: If not selected, don't add the branch.
    }

bail:
    SafeRelease(&pPD);
    SafeRelease(&pSD);
    SafeRelease(&pSinkActivate);
    SafeRelease(&pSourceNode);
    SafeRelease(&pOutputNode);
    return hr;
}

// Run the session
HRESULT MFUtils::RunSession(
    IMFMediaSession *pSession, // Session to run
    IMFTopology *pTopology // The toppology
)
{
    assert(pSession && pTopology);

    IMFMediaEvent *pEvent = NULL;

    PROPVARIANT var;
    PropVariantInit(&var);

    MediaEventType met;
    HRESULT hrStatus = S_OK;
    HRESULT hr = S_OK;
    CHECK_HR(hr = pSession->SetTopology(MFSESSION_SETTOPOLOGY_IMMEDIATE, pTopology)); // MFSESSION_SETTOPOLOGY_IMMEDIATE required to update (reload) topology when media type change
    CHECK_HR(hr = pSession->Start(&GUID_NULL, &var));

    // Check first event
    hr = pSession->GetEvent(MF_EVENT_FLAG_NO_WAIT, &pEvent);
    if(hr == MF_E_NO_EVENTS_AVAILABLE || hr == MF_E_MULTIPLE_SUBSCRIBERS) { // MF_E_MULTIPLE_SUBSCRIBERS means already listening
        hr = S_OK;
        goto bail;
    }
    if(pEvent) {
        CHECK_HR(hr = pEvent->GetStatus(&hrStatus));
    }
    else {
        hrStatus = hr;
    }
    if (FAILED(hrStatus)) {
        CHECK_HR(hr = pEvent->GetType(&met));
        TSK_DEBUG_ERROR("Session error: 0x%x (event id: %d)\n", hrStatus, met);
        hr = hrStatus;
        goto bail;
    }

bail:
    SafeRelease(&pEvent);
    return hr;
}

// Stop session
HRESULT MFUtils::ShutdownSession(
    IMFMediaSession *pSession, // The Session
    IMFMediaSource *pSource // Source to shutdown (optional)
)
{
    // MUST be source then session
    if(pSource) {
        pSource->Stop();
        pSource->Shutdown();
    }
    if(pSession) {
        pSession->Shutdown();
    }
    return S_OK;
}

// Pause session
HRESULT MFUtils::PauseSession(
    IMFMediaSession *pSession, // The session
    IMFMediaSource *pSource // Source to pause (optional)
)
{
    if(!pSession) {
        return E_INVALIDARG;
    }
    if(pSource) {
        pSource->Pause();
    }
    return pSession->Pause();
}

// Returns -1 if none is supported
INT MFUtils::GetSupportedSubTypeIndex(
    IMFMediaSource *pSource, // The source
    const GUID& mediaType, // The MediaType
    const VideoSubTypeGuidPair* subTypes, UINT subTypesCount // List of preferred subtypes (in ascending order)
)
{
    assert(pSource);

    IMFPresentationDescriptor *pPD = NULL;
    IMFStreamDescriptor *pSD = NULL;
    IMFMediaTypeHandler *pHandler = NULL;
    IMFMediaType *pMediaType = NULL;

    INT nIndex = -1;
    HRESULT hr = S_OK;
    DWORD cStreams = 0, cMediaTypesCount;
    GUID majorType, subType;
    BOOL fSelected;

    CHECK_HR(hr = pSource->CreatePresentationDescriptor(&pPD));
    CHECK_HR(hr = pPD->GetStreamDescriptorCount(&cStreams));

    for (UINT subTypesIndex = 0; subTypesIndex < subTypesCount && nIndex == -1; ++subTypesIndex) {
        for (DWORD cStreamIndex = 0; cStreamIndex < cStreams && nIndex == -1; ++cStreamIndex) {
            fSelected = FALSE;
            CHECK_HR(hr = pPD->GetStreamDescriptorByIndex(cStreamIndex, &fSelected, &pSD));
            if(fSelected) {
                CHECK_HR(hr = pSD->GetMediaTypeHandler(&pHandler));
                CHECK_HR(hr = pHandler->GetMajorType(&majorType));
                if(majorType == mediaType) {
                    CHECK_HR(hr = pHandler->GetMediaTypeCount(&cMediaTypesCount));
                    for(DWORD cMediaTypesIndex = 0; cMediaTypesIndex < cMediaTypesCount && nIndex == -1; ++cMediaTypesIndex) {
                        CHECK_HR(hr = pHandler->GetMediaTypeByIndex(cMediaTypesIndex, &pMediaType));
                        CHECK_HR(hr = pMediaType->GetGUID(MF_MT_SUBTYPE, &subType));
                        if (subTypes[subTypesIndex].fourcc == subType) {
                            nIndex = subTypesIndex;
                            break;
                        }
                        SafeRelease(&pMediaType);
                    }
                }
            }

            SafeRelease(&pSD);
            SafeRelease(&pHandler);
        }
    }

bail:
    SafeRelease(&pMediaType);
    SafeRelease(&pPD);
    SafeRelease(&pSD);
    SafeRelease(&pHandler);

    return nIndex;
}

HRESULT MFUtils::IsSupported(
    IMFPresentationDescriptor *pPD,
    DWORD cStreamIndex,
    UINT32 nWidth,
    UINT32 nHeight,
    UINT32 nFps,
    const GUID& guidFormat,
    BOOL* pbSupportedSize,
    BOOL* pbSupportedFps,
    BOOL* pbSupportedFormat
)
{
    HRESULT hr = S_OK;

    BOOL fSelected = FALSE;
    IMFStreamDescriptor *pSD = NULL;
    IMFMediaTypeHandler *pHandler = NULL;
    IMFMediaType *pMediaType = NULL;
    UINT32 _nWidth = 0, _nHeight = 0, numeratorFps = 0, denominatorFps = 0;
    GUID subType;
    DWORD cMediaTypesCount;

    if(!pPD || !pbSupportedSize || !pbSupportedFps || !pbSupportedFormat) {
        CHECK_HR(hr = E_POINTER);
    }

    *pbSupportedSize = FALSE;
    *pbSupportedFps = FALSE;
    *pbSupportedFormat = FALSE;

    CHECK_HR(hr = pPD->GetStreamDescriptorByIndex(cStreamIndex, &fSelected, &pSD));
    if(fSelected) {
        CHECK_HR(hr = pSD->GetMediaTypeHandler(&pHandler));
        CHECK_HR(hr = pHandler->GetMediaTypeCount(&cMediaTypesCount));
        for(DWORD cMediaTypesIndex = 0; cMediaTypesIndex < cMediaTypesCount; ++cMediaTypesIndex) {
            CHECK_HR(hr = pHandler->GetMediaTypeByIndex(cMediaTypesIndex, &pMediaType));
            CHECK_HR(hr = MFGetAttributeSize(pMediaType, MF_MT_FRAME_SIZE, &_nWidth, &_nHeight));
            CHECK_HR(hr = pMediaType->GetGUID(MF_MT_SUBTYPE, &subType));
            if(FAILED(hr = MFGetAttributeRatio(pMediaType, MF_MT_FRAME_RATE, &numeratorFps, &denominatorFps))) {
                numeratorFps = 30;
                denominatorFps = 1;
            }

            // all must match for the same stream
            if(_nWidth == nWidth && _nHeight == nHeight && subType == guidFormat && (numeratorFps/denominatorFps) == nFps) {
                *pbSupportedSize = TRUE;
                *pbSupportedFormat = TRUE;
                *pbSupportedFps = TRUE;
                break;
            }

            SafeRelease(&pMediaType);
        }
        SafeRelease(&pHandler);
    }

bail:
    SafeRelease(&pSD);
    SafeRelease(&pHandler);
    SafeRelease(&pMediaType);

    return hr;
}

HRESULT MFUtils::IsSupported(
    IMFPresentationDescriptor *pPD,
    DWORD cStreamIndex,
    IMFMediaType* pMediaType,
    BOOL* pbSupportedSize,
    BOOL* pbSupportedFps,
    BOOL* pbSupportedFormat
)
{
    HRESULT hr = S_OK;

    UINT32 nWidth = 0, nHeight = 0, nFps = 0, numeratorFps = 30, denominatorFps = 1;
    GUID subType;

    if(!pPD || !pMediaType || !pbSupportedSize || !pbSupportedFps || !pbSupportedFormat) {
        CHECK_HR(hr = E_POINTER);
    }

    CHECK_HR(hr = MFGetAttributeSize(pMediaType, MF_MT_FRAME_SIZE, &nWidth, &nHeight));
    CHECK_HR(hr = pMediaType->GetGUID(MF_MT_SUBTYPE, &subType));
    if(FAILED(hr = MFGetAttributeRatio(pMediaType, MF_MT_FRAME_RATE, &numeratorFps, &denominatorFps))) {
        numeratorFps = 30;
        denominatorFps = 1;
    }

    CHECK_HR(hr = IsSupported(
                      pPD,
                      cStreamIndex,
                      nWidth,
                      nHeight,
                      (numeratorFps / denominatorFps),
                      subType,
                      pbSupportedSize,
                      pbSupportedFps,
                      pbSupportedFormat
                  ));
bail:
    return hr;
}

HRESULT MFUtils::IsSupportedByInput(
    IMFPresentationDescriptor *pPD,
    DWORD cStreamIndex,
    IMFTopologyNode *pNode,
    BOOL* pbSupportedSize,
    BOOL* pbSupportedFps,
    BOOL* pbSupportedFormat
)
{
    HRESULT hr = S_OK;

    IMFMediaType *pMediaType = NULL;
    IUnknown* pObject = NULL;
    IMFActivate *pActivate = NULL;
    IMFMediaSink *pMediaSink = NULL;
    IMFTransform *pTransform = NULL;
    IMFStreamSink *pStreamSink = NULL;
    IMFMediaTypeHandler *pHandler = NULL;

    if(!pPD || !pNode || !pbSupportedSize || !pbSupportedFps || !pbSupportedFormat) {
        CHECK_HR(hr = E_POINTER);
    }

    CHECK_HR(hr = pNode->GetObject(&pObject));
    hr = pObject->QueryInterface(IID_PPV_ARGS(&pActivate));
    if(SUCCEEDED(hr)) {
        SafeRelease(&pObject);
        hr = pActivate->ActivateObject(IID_IMFMediaSink, (void**)&pObject);
        if(FAILED(hr)) {
            hr = pActivate->ActivateObject(IID_IMFTransform, (void**)&pObject);
        }
    }

    if(!pObject) {
        CHECK_HR(hr = E_NOINTERFACE);
    }

    hr = pObject->QueryInterface(IID_PPV_ARGS(&pMediaSink));
    if(FAILED(hr)) {
        hr = pObject->QueryInterface(IID_PPV_ARGS(&pTransform));
    }



    if(pMediaSink) {
        CHECK_HR(hr = pMediaSink->GetStreamSinkByIndex(0, &pStreamSink));
        CHECK_HR(hr = pStreamSink->GetMediaTypeHandler(&pHandler));
        CHECK_HR(hr = pHandler->GetCurrentMediaType(&pMediaType));

    }
    else if(pTransform) {
        CHECK_HR(hr = pTransform->GetInputCurrentType(0, &pMediaType));
    }
    else {
        CHECK_HR(hr = pNode->GetInputPrefType(0, &pMediaType));
    }

    CHECK_HR(hr = IsSupported(
                      pPD,
                      cStreamIndex,
                      pMediaType,
                      pbSupportedSize,
                      pbSupportedFps,
                      pbSupportedFormat
                  ));

bail:
    SafeRelease(&pObject);
    SafeRelease(&pActivate);
    SafeRelease(&pMediaType);
    SafeRelease(&pStreamSink);
    SafeRelease(&pHandler);
    return hr;
}

HRESULT MFUtils::ConnectConverters(
    IMFTopologyNode *pNode,
    DWORD dwOutputIndex,
    IMFTopologyNode *pNodeConvFrameRate,
    IMFTopologyNode *pNodeConvColor,
    IMFTopologyNode *pNodeConvSize
)
{
    HRESULT hr = S_OK;

    if(!pNode) {
        CHECK_HR(hr = E_POINTER);
    }

    if(pNodeConvFrameRate) {
        CHECK_HR(hr = pNode->ConnectOutput(dwOutputIndex, pNodeConvFrameRate, 0));
        if(pNodeConvSize) {
            CHECK_HR(hr = pNodeConvFrameRate->ConnectOutput(0, pNodeConvSize, 0));
            if(pNodeConvColor) {
                CHECK_HR(hr = pNodeConvSize->ConnectOutput(0, pNodeConvColor, 0));
            }
        }
        else {
            if(pNodeConvColor) {
                CHECK_HR(hr = pNodeConvFrameRate->ConnectOutput(0, pNodeConvColor, 0));
            }
        }
    }
    else {
        if(pNodeConvSize) {
            CHECK_HR(hr = pNode->ConnectOutput(dwOutputIndex, pNodeConvSize, 0));
            if(pNodeConvColor) {
                CHECK_HR(hr = pNodeConvSize->ConnectOutput(0, pNodeConvColor, 0));
            }
        }
        else {
            if(pNodeConvColor) {
                CHECK_HR(hr = pNode->ConnectOutput(dwOutputIndex, pNodeConvColor, 0));
            }
        }
    }

bail:
    return hr;
}

// This function should be called only if VideoProcessor is not supported
HRESULT MFUtils::GetBestFormat(
    IMFMediaSource *pSource,
    const GUID *pSubType,
    UINT32 nWidth,
    UINT32 nHeight,
    UINT32 nFps,
    UINT32 *pnWidth,
    UINT32 *pnHeight,
    UINT32 *pnFps,
    const VideoSubTypeGuidPair **ppSubTypeGuidPair
)
{

#define _FindPairByGuid(_guid, _index) { \
	int _i; _index = -1; \
	for (_i = 0; _i < PreferredVideoSubTypeGuidPairsCount; ++_i) { \
		if (PreferredVideoSubTypeGuidPairs[_i].fourcc == _guid) { \
			_index = _i; break; \
		} \
	} \
}
#if 0
    *pnWidth = 640;
    *pnHeight = 480;
    *pnFps = 30;
    return S_OK;
#else
    HRESULT hr = S_OK;
    IMFPresentationDescriptor *pPD = NULL;
    IMFStreamDescriptor *pSD = NULL;
    IMFMediaTypeHandler *pHandler = NULL;
    IMFMediaType *pMediaType = NULL;
    DWORD cStreams = 0, cMediaTypesCount;
    GUID majorType, subType, _BestSubType;
    BOOL bFound = FALSE, fSelected;
    UINT32 _nWidth, _nHeight, numeratorFps, denominatorFps, _nFps, _nScore, _nBestScore;
    int PreferredVideoSubTypeGuidPairIndex;
    static const UINT32 kSubTypeMismatchPad = _UI32_MAX >> 4;
    static const UINT32 kFpsMismatchPad = _UI32_MAX >> 2;

    if (!ppSubTypeGuidPair || !pSubType) {
        CHECK_HR(hr = E_INVALIDARG);
    }
    _FindPairByGuid(*pSubType, PreferredVideoSubTypeGuidPairIndex);
    if (PreferredVideoSubTypeGuidPairIndex == -1) {
        CHECK_HR(hr = E_INVALIDARG);
    }
    *ppSubTypeGuidPair = &PreferredVideoSubTypeGuidPairs[PreferredVideoSubTypeGuidPairIndex];

    _nBestScore = _UI32_MAX;
    CHECK_HR(hr = pSource->CreatePresentationDescriptor(&pPD));
    CHECK_HR(hr = pPD->GetStreamDescriptorCount(&cStreams));


    for (DWORD i = 0; i < cStreams; i++) {
        fSelected = FALSE;

        CHECK_HR(hr = pPD->GetStreamDescriptorByIndex(i, &fSelected, &pSD));

        if (fSelected) {
            CHECK_HR(hr = pSD->GetMediaTypeHandler(&pHandler));

            CHECK_HR(hr = pHandler->GetMajorType(&majorType));

            if(majorType == MFMediaType_Video) {
                CHECK_HR(hr = pHandler->GetMediaTypeCount(&cMediaTypesCount));

                for(DWORD cMediaTypesIndex = 0; cMediaTypesIndex < cMediaTypesCount; ++cMediaTypesIndex) {
                    CHECK_HR(hr = pHandler->GetMediaTypeByIndex(cMediaTypesIndex, &pMediaType));

                    CHECK_HR(hr = pMediaType->GetGUID(MF_MT_SUBTYPE, &subType));
                    // if(subType == *pSubType)
                    {
                        CHECK_HR(hr = MFGetAttributeSize(pMediaType, MF_MT_FRAME_SIZE, &_nWidth, &_nHeight));
                        CHECK_HR(hr = MFGetAttributeRatio(pMediaType, MF_MT_FRAME_RATE, &numeratorFps, &denominatorFps));
                        _nFps = (numeratorFps / denominatorFps);

                        if (subType == *pSubType) {
                            _nScore = 0;
                        }
                        else {
                            _FindPairByGuid(subType, PreferredVideoSubTypeGuidPairIndex);
                            if (PreferredVideoSubTypeGuidPairIndex == -1) {
                                _nScore = kSubTypeMismatchPad; // Not a must but important: If(!VideoProcess) then CLSID_CColorConvertDMO
                            }
                            else {
                                _nScore = kSubTypeMismatchPad >> (PreferredVideoSubTypeGuidPairsCount - PreferredVideoSubTypeGuidPairIndex);
                            }
                        }
                        _nScore += abs((int)(_nWidth - nWidth)); // Not a must: If(!VideoProcess) then CLSID_CResizerDMO
                        _nScore += abs((int)(_nHeight - nHeight)); // Not a must: If(!VideoProcess) then CLSID_CResizerDMO
                        _nScore +=  (_nFps == nFps) ? 0 : kFpsMismatchPad; // Fps is a must because without video processor no alternative exist (CLSID_CFrameRateConvertDmo doesn't support I420)

                        if (_nScore <= _nBestScore || !bFound) {
                            *pnWidth = _nWidth;
                            *pnHeight = _nHeight;
                            *pnFps = _nFps;
                            bFound = TRUE;
                            _BestSubType = subType;
                            _nBestScore = _nScore;
                        }
                    }

                    SafeRelease(&pMediaType);
                }
            }
        }

        SafeRelease(&pHandler);
        SafeRelease(&pSD);
    }

bail:
    SafeRelease(&pPD);
    SafeRelease(&pSD);
    SafeRelease(&pHandler);
    SafeRelease(&pMediaType);

    _FindPairByGuid(_BestSubType, PreferredVideoSubTypeGuidPairIndex);
    if (PreferredVideoSubTypeGuidPairIndex != -1) {
        *ppSubTypeGuidPair = &PreferredVideoSubTypeGuidPairs[PreferredVideoSubTypeGuidPairIndex];
    }
    else { /*if (_nBestScore > kSubTypeMismatchPad)*/
        *pnWidth = 640;
        *pnHeight = 480;
        *pnFps = 30;
        TSK_DEBUG_WARN("Failed to math subtype...using VGA@30fps");
    }

    return SUCCEEDED(hr) ? (bFound ? S_OK : E_NOT_SET): hr;
#endif

}

HWND MFUtils::GetConsoleHwnd(void)
{
#define MY_BUFSIZE 1024 // Buffer size for console window titles.
    HWND hwndFound;         // This is what is returned to the caller.
    TCHAR pszNewWindowTitle[MY_BUFSIZE]; // Contains fabricated
    // WindowTitle.
    TCHAR pszOldWindowTitle[MY_BUFSIZE]; // Contains original
    // WindowTitle.

    // Fetch current window title.
    GetConsoleTitle(pszOldWindowTitle, MY_BUFSIZE);

    // Format a "unique" NewWindowTitle.
    wsprintf(pszNewWindowTitle,TEXT("%d/%d"),
             GetTickCount(),
             GetCurrentProcessId());

    // Change current window title.
    SetConsoleTitle(pszNewWindowTitle);

    // Ensure window title has been updated.
    Sleep(40);

    // Look for NewWindowTitle.
    hwndFound=FindWindow(NULL, pszNewWindowTitle);

    // Restore original window title.
    SetConsoleTitle(pszOldWindowTitle);

    return(hwndFound);
}
