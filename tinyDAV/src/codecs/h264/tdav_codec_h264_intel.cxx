/*
* Copyright (C) 2014-2015 Mamadou DIOP.
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

/**@file tdav_codec_h264_intel.cxx
* @brief H.264 codec plugin using Intel Media SDK 2014 R2 for Clients (https://software.intel.com/en-us/media-solutions-portal) v1.1 for encoding/decoding.
* Low latency encoding/decoding: https://software.intel.com/en-us/articles/video-conferencing-features-of-intel-media-software-development-kit
*/
#include "tinydav/codecs/h264/tdav_codec_h264_intel.h"

#if HAVE_INTEL_MEDIA_SDK

#include "tinydav/codecs/h264/tdav_codec_h264_common.h"

#include "tinyrtp/rtp/trtp_rtp_packet.h"

#include "tinymedia/tmedia_codec.h"
#include "tinymedia/tmedia_params.h"
#include "tinymedia/tmedia_defaults.h"

#include "tsk_thread.h"
#include "tsk_params.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#include <mfxvideo++.h>

#if defined(_MSC_VER)
#	pragma comment(lib, "libmfx.lib")
#endif /* _MSC_VER */

#if !defined(INTEL_DX11_D3D)
#	define INTEL_DX11_D3D 1
#endif /* INTEL_DX11_D3D */

#if INTEL_DX11_D3D
#include <d3d11.h>
#include <dxgi1_2.h>
#include <atlbase.h>
#	if defined(_MSC_VER)
#		pragma comment(lib, "d3d11.lib")
#		pragma comment(lib, "dxgi.lib")
#	endif /* _MSC_VER */

#endif /* INTEL_DX11_D3D */

#define INTEL_DEBUG_INFO(FMT, ...) TSK_DEBUG_INFO("[MSDK H264 Codec] " FMT, ##__VA_ARGS__)
#define INTEL_DEBUG_WARN(FMT, ...) TSK_DEBUG_WARN("[MSDK H264 Codec] " FMT, ##__VA_ARGS__)
#define INTEL_DEBUG_ERROR(FMT, ...) TSK_DEBUG_ERROR("[MSDK H264 Codec] " FMT, ##__VA_ARGS__)
#define INTEL_DEBUG_FATAL(FMT, ...) TSK_DEBUG_FATAL("[MSDK H264 Codec] " FMT, ##__VA_ARGS__)

#define INTEL_ALIGN16(i) ((((i) + 15) >> 4) << 4)
#define INTEL_ALIGN32(i) (((mfxU32)((i)+31)) & (~ (mfxU32)31))
#define INTEL_SAFE_RELEASE(X) {if ((X)) { (X)->Release(); (X) = NULL; }}

#define INTEL_CHECK_STATUS(x) { mfxStatus __status__ = (x); if (__status__ != MFX_ERR_NONE) { INTEL_DEBUG_ERROR("Operation Failed (%d)", __status__); goto bail; } }
#define INTEL_BREAK(msg) { INTEL_DEBUG_ERROR("%s", msg); goto bail; }

#define INTEL_ENABLE_REALTIME 1

static mfxIMPL __IntelDefaultImpl = MFX_IMPL_AUTO_ANY
#if INTEL_DX11_D3D
                                    | MFX_IMPL_VIA_D3D11
#endif
                                    ;
static mfxVersion __IntelDefaultVer = { 0, 1 };

// TODO: Test against FFmpeg, CUDA, OpenH264 and Microsoft implementations
// TODO: When Bandwidth change (or any other event) Reset() fails

class IntelCodecEncoder;
class IntelCodecDecoder;

typedef struct tdav_codec_h264_intel_s {
    TDAV_DECLARE_CODEC_H264_COMMON;

    MFXVideoSession* mfxSession;

    // DX11_D3D
#if INTEL_DX11_D3D
    mfxFrameAllocResponse D3D11SavedAllocResponses[2/*Encode=0, Decode=1*/];
    mfxFrameAllocator D3D11Allocator;

    CComPtr<ID3D11Device> pD3D11Device;
    CComPtr<ID3D11DeviceContext> pD3D11Ctx;
    CComPtr<IDXGIAdapter>pAdapter;
    CComPtr<IDXGIFactory1> pDXGIFactory;
    CComPtr<IDXGIAdapter> hAdapter;
#endif

    // Encoder
    struct {
        IntelCodecEncoder *pInst;
        int64_t frame_count;
        tsk_bool_t force_idr;
        int rotation;
        int neg_width;
        int neg_height;
        int neg_fps;
        int max_bitrate_bps;
    } encoder;

    // decoder
    struct {
        IntelCodecDecoder *pInst;
    } decoder;
}
tdav_codec_h264_intel_t;

#if !defined(INTEL_H264_GOP_SIZE_IN_SECONDS)
#	define INTEL_H264_GOP_SIZE_IN_SECONDS		25
#endif

static int tdav_codec_h264_intel_init(tdav_codec_h264_intel_t* self, profile_idc_t profile);
static int tdav_codec_h264_intel_deinit(tdav_codec_h264_intel_t* self);
static int tdav_codec_h264_intel_open_encoder(tdav_codec_h264_intel_t* self);
static int tdav_codec_h264_intel_close_encoder(tdav_codec_h264_intel_t* self);
static int tdav_codec_h264_intel_open_decoder(tdav_codec_h264_intel_t* self);
static int tdav_codec_h264_intel_close_decoder(tdav_codec_h264_intel_t* self);

#if INTEL_DX11_D3D
#define D3D11_WILL_READ  0x1000
#define D3D11_WILL_WRITE 0x2000

typedef struct {
    mfxMemId	memId;
    mfxMemId	memIdStage;
    mfxU16		rw;
} CustomMemId;

const struct {
    mfxIMPL	impl;		// actual implementation
    mfxU32	adapterID;	// device adapter number
} implTypes[] = {
    { MFX_IMPL_HARDWARE, 0 },
    { MFX_IMPL_HARDWARE2, 1 },
    { MFX_IMPL_HARDWARE3, 2 },
    { MFX_IMPL_HARDWARE4, 3 }
};

static mfxStatus D3D11_CreateHWDevice(mfxHDL pthis, mfxSession session, mfxHDL* deviceHandle, HWND hWnd);
static void D3D11_CleanupHWDevice(mfxHDL pthis);
static void D3D11_SetHWDeviceContext(mfxHDL pthis, CComPtr<ID3D11DeviceContext> devCtx);

// Intel Media SDK memory allocator entrypoints....
// - A slightly different allocation procedure is used for encode, decode and VPP
static mfxStatus D3D11_SimpleAlloc(mfxHDL pthis, mfxFrameAllocRequest *request, mfxFrameAllocResponse *response);
static mfxStatus D3D11_SimpleLock(mfxHDL pthis, mfxMemId mid, mfxFrameData *ptr);
static mfxStatus D3D11_SimpleUnlock(mfxHDL pthis, mfxMemId mid, mfxFrameData *ptr);
static mfxStatus D3D11_SimpleGethdl(mfxHDL pthis, mfxMemId mid, mfxHDL *handle);
static mfxStatus D3D11_SimpleFree(mfxHDL pthis, mfxFrameAllocResponse *response);
#endif /* INTEL_DX11_D3D */

//
//	IntelCodec
//
class IntelCodec
{
protected:
    IntelCodec(MFXVideoSession* pSession)
        : m_bOpened(false)
        , m_pSession(pSession)
        , n_nNumSurfaces(0)
        , m_nSurfaceWidth(0)
        , m_nSurfaceHeight(0)
        , m_nSurfaceBitsPerPixel(0)
        , m_nSurfaceSize(0)
        , m_pSurfaceBuffers(NULL)
        , m_ppSurfacePtrs(NULL) {
        memset(&m_sBitstream, 0, sizeof(m_sBitstream));
        memset(&m_sParamReq, 0, sizeof(m_sParamReq));
        memset(&m_sParamSel, 0, sizeof(m_sParamSel));
        memset(&m_sAllocRequest, 0, sizeof(m_sAllocRequest));
    }
public:
    virtual ~IntelCodec() {
        Close();
    }
    virtual mfxStatus Open(struct tdav_codec_h264_intel_s* pWrappedCodec) = 0;

    virtual mfxStatus Close() {
        DeAllocSurfaces();
        DeAllocateBitstream();

        memset(&m_sAllocRequest, 0, sizeof(m_sAllocRequest));

        m_bOpened = false;

        return MFX_ERR_NONE;
    }

protected:
    int GetFreeSurfaceIndex() {
        if (m_ppSurfacePtrs) {
            for (mfxU16 i = 0; i < n_nNumSurfaces; i++) {
                if (0 == m_ppSurfacePtrs[i]->Data.Locked) {
                    return i;
                }
            }
        }
        return MFX_ERR_NOT_FOUND;
    }

    mfxStatus ReadPlaneData(mfxU16 w, mfxU16 h, mfxU8 *buf, mfxU8 *ptr, mfxU16 pitch, mfxU16 offset, const mfxU8* &src) {
        for (mfxU16 i = 0; i < h; i++) {
            memcpy(buf, src, w);
            src += w;

            for (mfxU16 j = 0; j < w; j++) {
                ptr[i * pitch + j * 2 + offset] = buf[j];
            }
        }
        return MFX_ERR_NONE;
    }

    mfxStatus LoadRawFrame(int nSurfaceIndex, const mfxU8* src) {
        mfxFrameSurface1* pSurface = (m_ppSurfacePtrs && nSurfaceIndex >= 0 && nSurfaceIndex < n_nNumSurfaces) ? m_ppSurfacePtrs[nSurfaceIndex] : NULL;
        if (!pSurface) {
            INTEL_DEBUG_ERROR("Failed to find surface at index=%d", nSurfaceIndex);
            return MFX_ERR_NOT_FOUND;
        }

        mfxStatus sts = MFX_ERR_NONE;
        mfxU16 w, h, i, pitch;
        mfxU8 *ptr;
        mfxFrameInfo* pInfo = &pSurface->Info;
        mfxFrameData* pData = &pSurface->Data;

        if (pInfo->CropH > 0 && pInfo->CropW > 0) {
            w = pInfo->CropW;
            h = pInfo->CropH;
        }
        else {
            w = pInfo->Width;
            h = pInfo->Height;
        }

        pitch = pData->Pitch;
        ptr = pData->Y + pInfo->CropX + pInfo->CropY * pData->Pitch;

        // read luminance plane
        for (i = 0; i < h; i++) {
            memcpy(ptr + i * pitch, src, w);
            src += w;
        }

        mfxU8 buf[2048]; // maximum supported chroma width for nv12
        w /= 2;
        h /= 2;
        ptr = pData->UV + pInfo->CropX + (pInfo->CropY / 2) * pitch;
        if (w > 2048) {
            return MFX_ERR_UNSUPPORTED;
        }

        // load U
        sts = ReadPlaneData(w, h, buf, ptr, pitch, 0, src);
        if (MFX_ERR_NONE != sts) {
            return sts;
        }
        // load V
        sts = ReadPlaneData(w, h, buf, ptr, pitch, 1, src);
        if (MFX_ERR_NONE != sts) {
            return sts;
        }

        return MFX_ERR_NONE;
    }

    virtual mfxStatus AllocSurfaces(mfxU16 nNumSurfaces, mfxU16 nSurfaceWidth, mfxU16 nSurfaceHeight, const mfxFrameInfo* pcFrameInfo) {
        mfxStatus status = MFX_ERR_UNKNOWN;

        INTEL_DEBUG_INFO("Alloc surfaces: num=%u, width=%u, height=%u", nNumSurfaces, nSurfaceWidth, nSurfaceHeight);

        DeAllocSurfaces();

        n_nNumSurfaces = nNumSurfaces;
        m_nSurfaceWidth = (mfxU16)INTEL_ALIGN32(nSurfaceWidth);
        m_nSurfaceHeight = (mfxU16)INTEL_ALIGN32(nSurfaceHeight);
        m_nSurfaceBitsPerPixel = 12;  // NV12 format is a 12 bits per pixel format
        m_nSurfaceSize = m_nSurfaceWidth * m_nSurfaceHeight * m_nSurfaceBitsPerPixel / 8;

#if !INTEL_DX11_D3D
        if (!(m_pSurfaceBuffers = (mfxU8 *)new mfxU8[m_nSurfaceSize * n_nNumSurfaces])) {
            INTEL_CHECK_STATUS(MFX_ERR_MEMORY_ALLOC);
        }
#endif

        if (!(m_ppSurfacePtrs = new mfxFrameSurface1*[n_nNumSurfaces])) {
            INTEL_CHECK_STATUS(MFX_ERR_MEMORY_ALLOC);
        }
        for (mfxU16 i = 0; i < n_nNumSurfaces; i++) {
            if (!(m_ppSurfacePtrs[i] = new mfxFrameSurface1)) {
                INTEL_CHECK_STATUS(MFX_ERR_MEMORY_ALLOC);
            }
            memset(m_ppSurfacePtrs[i], 0, sizeof(mfxFrameSurface1));
            memcpy(&(m_ppSurfacePtrs[i]->Info), pcFrameInfo, sizeof(mfxFrameInfo));
#if INTEL_DX11_D3D
            m_ppSurfacePtrs[i]->Data.MemId = m_sD3D11Response.mids[i]; // MID (memory id) represent one D3D NV12 surface
#else
            m_ppSurfacePtrs[i]->Data.Y = &m_pSurfaceBuffers[m_nSurfaceSize * i];
            m_ppSurfacePtrs[i]->Data.U = m_ppSurfacePtrs[i]->Data.Y + m_nSurfaceWidth * m_nSurfaceHeight;
            m_ppSurfacePtrs[i]->Data.V = m_ppSurfacePtrs[i]->Data.U + 1;
            m_ppSurfacePtrs[i]->Data.Pitch = m_nSurfaceWidth;
#endif
        }

        return MFX_ERR_NONE;

bail:
        DeAllocSurfaces();
        return status;
    }

    mfxStatus AllocateBitstream(mfxU32 nMaxLength) {
        DeAllocateBitstream();

        m_sBitstream.MaxLength = nMaxLength;
        if (!(m_sBitstream.Data = new mfxU8[nMaxLength])) {
            INTEL_CHECK_STATUS(MFX_ERR_MEMORY_ALLOC);
        }

        return MFX_ERR_NONE;

bail:
        return MFX_ERR_MEMORY_ALLOC;
    }
private:
    mfxStatus DeAllocSurfaces() {
        if (m_ppSurfacePtrs) {
            for (mfxU16 i = 0; i < n_nNumSurfaces; i++) {
                if (m_ppSurfacePtrs[i]) {
                    delete m_ppSurfacePtrs[i];
                }
            }
            delete[] m_ppSurfacePtrs;
            m_ppSurfacePtrs = NULL;
        }
        n_nNumSurfaces = 0;

        if (m_pSurfaceBuffers) {
            delete[] m_pSurfaceBuffers;
            m_pSurfaceBuffers = NULL;
        }

        m_nSurfaceWidth = 0;
        m_nSurfaceHeight = 0;
        m_nSurfaceBitsPerPixel = 0;
        m_nSurfaceSize = 0;

        return MFX_ERR_NONE;
    }

    mfxStatus DeAllocateBitstream() {
        if (m_sBitstream.Data) {
            delete[]m_sBitstream.Data;
        }
        memset(&m_sBitstream, 0, sizeof(m_sBitstream));

        return MFX_ERR_NONE;
    }


protected:
    bool m_bOpened;
    MFXVideoSession* m_pSession;
    mfxU16 n_nNumSurfaces;
    mfxU16 m_nSurfaceWidth;
    mfxU16 m_nSurfaceHeight;
    mfxU8  m_nSurfaceBitsPerPixel;
    mfxU32 m_nSurfaceSize;
    mfxU8* m_pSurfaceBuffers;  // mfxU8[];
    mfxFrameSurface1** m_ppSurfacePtrs; // mfxFrameSurface1[]
    mfxBitstream m_sBitstream;
    mfxVideoParam m_sParamReq; // requested params
    mfxVideoParam m_sParamSel; // selected params
    mfxFrameAllocRequest m_sAllocRequest;
#if INTEL_DX11_D3D
    mfxFrameAllocResponse m_sD3D11Response;
#endif
};


//
//	IntelCodecEncoder
//
class IntelCodecEncoder : public IntelCodec
{
public:
    IntelCodecEncoder(MFXVideoSession* pSession)
        : IntelCodec(pSession)
        , m_Inst(*pSession) {
        memset(&m_sFrameCtrl, 0, sizeof(m_sFrameCtrl));
    }
    virtual ~IntelCodecEncoder() {
        Close();
    }

    virtual mfxStatus Close() {
        m_Inst.Close();
        memset(&m_sFrameCtrl, 0, sizeof(m_sFrameCtrl));
        return IntelCodec::Close();
    }

    mfxStatus Reset() {
        if (m_bOpened) {
            return m_Inst.Reset(&m_sParamSel);
        }
        return MFX_ERR_NONE;
    }

    mfxStatus Open(struct tdav_codec_h264_intel_s* pWrappedCodec) {
        int32_t max_bw_kpbs;
        tdav_codec_h264_common_t* pWrappedCodecCommon = (tdav_codec_h264_common_t*)pWrappedCodec;
        mfxStatus status = MFX_ERR_UNKNOWN;
        mfxU16 uSelWidth, uSelHeight;

        pWrappedCodec->encoder.neg_width = (pWrappedCodec->encoder.rotation == 90 || pWrappedCodec->encoder.rotation == 270) ? TMEDIA_CODEC_VIDEO(pWrappedCodec)->out.height : TMEDIA_CODEC_VIDEO(pWrappedCodec)->out.width;
        pWrappedCodec->encoder.neg_height = (pWrappedCodec->encoder.rotation == 90 || pWrappedCodec->encoder.rotation == 270) ? TMEDIA_CODEC_VIDEO(pWrappedCodec)->out.width : TMEDIA_CODEC_VIDEO(pWrappedCodec)->out.height;
        pWrappedCodec->encoder.neg_fps = TMEDIA_CODEC_VIDEO(pWrappedCodec)->out.fps;
        max_bw_kpbs = TSK_CLAMP(
                          0,
                          tmedia_get_video_bandwidth_kbps_2(pWrappedCodec->encoder.neg_width, pWrappedCodec->encoder.neg_height, pWrappedCodec->encoder.neg_fps),
                          TMEDIA_CODEC(pWrappedCodec)->bandwidth_max_upload
                      );
        pWrappedCodec->encoder.max_bitrate_bps = (max_bw_kpbs * 1024);

        INTEL_DEBUG_INFO("neg_width=%d, neg_height=%d, neg_fps=%d, max_bitrate_bps=%d",
                         pWrappedCodec->encoder.neg_width,
                         pWrappedCodec->encoder.neg_height,
                         pWrappedCodec->encoder.neg_fps,
                         pWrappedCodec->encoder.max_bitrate_bps
                        );

        // Initialize encoder parameters
        memset(&m_sParamReq, 0, sizeof(m_sParamReq));
        m_sParamReq.mfx.CodecId = MFX_CODEC_AVC;
        m_sParamReq.mfx.CodecProfile = pWrappedCodecCommon->profile == profile_idc_main ? MFX_PROFILE_AVC_MAIN : MFX_PROFILE_AVC_BASELINE;
        m_sParamReq.mfx.CodecLevel = (mfxU16)pWrappedCodecCommon->level;
        // TODO: Update "CodecProfile" based on "common->profile_iop"
        m_sParamReq.mfx.TargetUsage = MFX_TARGETUSAGE_BALANCED;
        m_sParamReq.mfx.TargetKbps = max_bw_kpbs;
        m_sParamReq.mfx.RateControlMethod = MFX_RATECONTROL_CBR;
        m_sParamReq.mfx.IdrInterval = (pWrappedCodec->encoder.neg_fps * INTEL_H264_GOP_SIZE_IN_SECONDS);
        m_sParamReq.mfx.FrameInfo.FrameRateExtN = pWrappedCodec->encoder.neg_fps;
        m_sParamReq.mfx.FrameInfo.FrameRateExtD = 1;
        m_sParamReq.mfx.FrameInfo.FourCC = MFX_FOURCC_NV12;
        m_sParamReq.mfx.FrameInfo.ChromaFormat = MFX_CHROMAFORMAT_YUV420;
        m_sParamReq.mfx.FrameInfo.PicStruct = MFX_PICSTRUCT_PROGRESSIVE;
        m_sParamReq.mfx.FrameInfo.CropX = 0;
        m_sParamReq.mfx.FrameInfo.CropY = 0;
        m_sParamReq.mfx.FrameInfo.CropW = pWrappedCodec->encoder.neg_width;
        m_sParamReq.mfx.FrameInfo.CropH = pWrappedCodec->encoder.neg_height;
        m_sParamReq.mfx.FrameInfo.Width = INTEL_ALIGN16(pWrappedCodec->encoder.neg_width); // must be a multiple of 16
        m_sParamReq.mfx.FrameInfo.Height = INTEL_ALIGN16(pWrappedCodec->encoder.neg_height); // must be a multiple of 16
#if INTEL_DX11_D3D
        m_sParamReq.IOPattern = MFX_IOPATTERN_IN_VIDEO_MEMORY;
#else
        m_sParamReq.IOPattern = MFX_IOPATTERN_IN_SYSTEM_MEMORY;
#endif

        memset(&m_sOpt2MaxFrameSize, 0, sizeof(m_sOpt2MaxFrameSize));
        m_sOpt2MaxFrameSize.Header.BufferId = MFX_EXTBUFF_CODING_OPTION2;
        m_sOpt2MaxFrameSize.Header.BufferSz = sizeof(m_sOpt2MaxFrameSize);
        m_sOpt2MaxFrameSize.MaxSliceSize = (H264_RTP_PAYLOAD_SIZE - 100);
        m_sOpt2MaxFrameSize.RepeatPPS = MFX_CODINGOPTION_OFF;
        m_pExtendedBuffers[0] = (mfxExtBuffer*)&m_sOpt2MaxFrameSize;
#if INTEL_ENABLE_REALTIME
        m_sParamReq.AsyncDepth = 1; //  limits internal frame buffering
        m_sParamReq.mfx.GopRefDist = 1; // No B-Frames
        m_sParamReq.mfx.NumRefFrame = 1;
        memset(&m_sOptLowLatency, 0, sizeof(m_sOptLowLatency));
        m_sOptLowLatency.Header.BufferId = MFX_EXTBUFF_CODING_OPTION;
        m_sOptLowLatency.Header.BufferSz = sizeof(m_sOptLowLatency);
        m_sOptLowLatency.MaxDecFrameBuffering = 1;
        m_pExtendedBuffers[1] = (mfxExtBuffer*)&m_sOptLowLatency;
        m_sParamReq.NumExtParam = 2;
#else
        m_sParamReq.NumExtParam = 1;
#endif
        m_sParamReq.ExtParam = m_pExtendedBuffers;

        // Check parameters
        status = m_Inst.Query(&m_sParamReq, &m_sParamReq);
        if (status != MFX_ERR_NONE && status != MFX_WRN_INCOMPATIBLE_VIDEO_PARAM /* Best one will be selected by the encoder */) {
            INTEL_CHECK_STATUS(status);
        }
        if (m_sOpt2MaxFrameSize.MaxSliceSize == 0) {
            INTEL_DEBUG_INFO("The encoder doesn't support setting 'MaxSliceSize' :(");
        }

        // Query number required surfaces for encoder
        memset(&m_sAllocRequest, 0, sizeof(m_sAllocRequest));
        INTEL_CHECK_STATUS(status = m_Inst.QueryIOSurf(&m_sParamReq, &m_sAllocRequest));
        INTEL_DEBUG_INFO("nEncSurfNum = %hu", m_sAllocRequest.NumFrameSuggested);
#if INTEL_DX11_D3D
        m_sAllocRequest.Type |= D3D11_WILL_WRITE; // Hint to DX11 memory handler that application will write data to input surfaces
#endif

        // Allocate surfaces for encoder
#if INTEL_DX11_D3D
        INTEL_CHECK_STATUS(status = pWrappedCodec->D3D11Allocator.Alloc(pWrappedCodec->D3D11Allocator.pthis, &m_sAllocRequest, &m_sD3D11Response));
        if (m_sD3D11Response.NumFrameActual == 0) {
            INTEL_CHECK_STATUS(status = MFX_ERR_UNKNOWN);
        }
        INTEL_CHECK_STATUS(status = AllocSurfaces(m_sD3D11Response.NumFrameActual, m_sAllocRequest.Info.Width, m_sAllocRequest.Info.Height, &m_sParamReq.mfx.FrameInfo));
#else
        INTEL_CHECK_STATUS(status = AllocSurfaces(m_sAllocRequest.NumFrameSuggested, m_sAllocRequest.Info.Width, m_sAllocRequest.Info.Height, &m_sParamReq.mfx.FrameInfo));
#endif

        // Initialize the Media SDK encoder
        status = m_Inst.Init(&m_sParamReq);
        if (status != MFX_ERR_NONE && status != MFX_WRN_PARTIAL_ACCELERATION) {
            INTEL_CHECK_STATUS(status);
        }
        INTEL_DEBUG_INFO("Encoder->Init() returned: %d", status);

        // Retrieve video parameters selected by encoder.
        memset(&m_sParamSel, 0, sizeof(m_sParamSel));
        INTEL_CHECK_STATUS(status = m_Inst.GetVideoParam(&m_sParamSel));
        INTEL_DEBUG_INFO("sel_width=%u.crop=%u, sel_height=%u.crop=%u, sel_fps=%u/%u",
                         m_sParamSel.mfx.FrameInfo.Width, m_sParamSel.mfx.FrameInfo.CropW,
                         m_sParamSel.mfx.FrameInfo.Height, m_sParamSel.mfx.FrameInfo.CropH,
                         m_sParamReq.mfx.FrameInfo.FrameRateExtN,
                         m_sParamReq.mfx.FrameInfo.FrameRateExtD
                        );
        if (m_sParamSel.mfx.FrameInfo.CropW > 0 && m_sParamSel.mfx.FrameInfo.CropH > 0) {
            uSelWidth = m_sParamSel.mfx.FrameInfo.CropW;
            uSelHeight = m_sParamSel.mfx.FrameInfo.CropH;
        }
        else {
            uSelWidth = m_sParamSel.mfx.FrameInfo.Width;
            uSelHeight = m_sParamSel.mfx.FrameInfo.Height;
        }
        if (pWrappedCodec->encoder.neg_width != uSelWidth || pWrappedCodec->encoder.neg_height != uSelHeight) {
            INTEL_DEBUG_INFO("Encoder neg size <> sel size: %dx%d<>%dx%d", pWrappedCodec->encoder.neg_width, pWrappedCodec->encoder.neg_height, uSelWidth, uSelHeight);
            pWrappedCodec->encoder.neg_width = uSelWidth;
            pWrappedCodec->encoder.neg_height = uSelHeight;
            TMEDIA_CODEC_VIDEO(pWrappedCodec)->out.width = pWrappedCodec->encoder.neg_width;
            TMEDIA_CODEC_VIDEO(pWrappedCodec)->out.height = pWrappedCodec->encoder.neg_height;
        }

        // Allocate BitStream
        INTEL_CHECK_STATUS(status = AllocateBitstream(m_sParamSel.mfx.BufferSizeInKB * 1000));

        m_bOpened = true;
        return MFX_ERR_NONE;

bail:
        Close();
        return status;
    }

    mfxStatus UpdateBandwidth(bool bUp, mfxU16 max) {
        if (bUp) {
            m_sParamSel.mfx.TargetKbps = TSK_CLAMP(0, (mfxU16)((m_sParamSel.mfx.TargetKbps * 3) >> 1), max);
        }
        else {
            m_sParamSel.mfx.TargetKbps = TSK_CLAMP(0, (mfxU16)((m_sParamSel.mfx.TargetKbps << 1) / 3), max);
        }
        m_sParamReq.mfx.TargetKbps = m_sParamSel.mfx.TargetKbps;
        INTEL_DEBUG_INFO("Setting new target bandwidth to %ukbps", m_sParamSel.mfx.TargetKbps);
        return m_Inst.Reset(&m_sParamSel);
    }

    mfxStatus SetMaxBandwidth(mfxU16 max) {
        m_sParamSel.mfx.TargetKbps = TSK_CLAMP(0, m_sParamSel.mfx.TargetKbps, max);
        m_sParamReq.mfx.TargetKbps = m_sParamSel.mfx.TargetKbps;
        INTEL_DEBUG_INFO("Setting new target bandwidth to %ukbps", m_sParamSel.mfx.TargetKbps);
        return m_Inst.Reset(&m_sParamSel);
    }

    mfxStatus Encode(struct tmedia_codec_s* pWrappedCodec, const mfxU8* pcInDataPtr, mfxU32 nInDataSize) {
        tdav_codec_h264_intel_t* pWrappedCodecH264 = (tdav_codec_h264_intel_t*)pWrappedCodec;
        tdav_codec_h264_common_t* pWrappedCodecCommon = (tdav_codec_h264_common_t*)pWrappedCodec;
        mfxU32 nInDataXSize;
        tsk_bool_t bSendIDR;
        int nEncSurfIdx = 0;
        mfxSyncPoint syncp;
        mfxStatus status = MFX_ERR_UNKNOWN;

        if (!pWrappedCodec || !pcInDataPtr || !nInDataSize) {
            INTEL_CHECK_STATUS(MFX_ERR_NULL_PTR);
        }

        if (!m_bOpened) {
            INTEL_CHECK_STATUS(MFX_ERR_NOT_INITIALIZED);
        }

        nInDataXSize = (pWrappedCodecH264->encoder.neg_width * pWrappedCodecH264->encoder.neg_height * 3) >> 1;
        if (nInDataXSize != nInDataSize) {
            /* guard */
            INTEL_DEBUG_ERROR("Invalid size: %u<>%u", nInDataXSize, nInDataSize);
            goto bail;
        }

        bSendIDR = (pWrappedCodecH264->encoder.frame_count++ == 0 || pWrappedCodecH264->encoder.force_idr);

        nEncSurfIdx = GetFreeSurfaceIndex();
        if (MFX_ERR_NOT_FOUND == nEncSurfIdx) {
            INTEL_CHECK_STATUS(MFX_ERR_MEMORY_ALLOC);
        }

        // Surface locking required when read/write D3D surfaces

#if INTEL_DX11_D3D
        INTEL_CHECK_STATUS(status = pWrappedCodecH264->D3D11Allocator.Lock(pWrappedCodecH264->D3D11Allocator.pthis, m_ppSurfacePtrs[nEncSurfIdx]->Data.MemId, &(m_ppSurfacePtrs[nEncSurfIdx]->Data)));
#endif

        INTEL_CHECK_STATUS(status = LoadRawFrame(nEncSurfIdx, pcInDataPtr));

#if INTEL_DX11_D3D
        INTEL_CHECK_STATUS(status = pWrappedCodecH264->D3D11Allocator.Unlock(pWrappedCodecH264->D3D11Allocator.pthis, m_ppSurfacePtrs[nEncSurfIdx]->Data.MemId, &(m_ppSurfacePtrs[nEncSurfIdx]->Data)));
#endif

        m_sFrameCtrl.FrameType = bSendIDR ? (MFX_FRAMETYPE_I | MFX_FRAMETYPE_REF | MFX_FRAMETYPE_IDR) : MFX_FRAMETYPE_UNKNOWN;

        //
        // Stage 1: Main encoding loop
        //
        do {
            for (;;) {
                // Encode a frame asychronously (returns immediately)
                status = m_Inst.EncodeFrameAsync(&m_sFrameCtrl, m_ppSurfacePtrs[nEncSurfIdx], &m_sBitstream, &syncp);

                if (MFX_ERR_NONE < status && !syncp) { // Repeat the call if warning and no output
                    if (MFX_WRN_DEVICE_BUSY == status) {
                        tsk_thread_sleep(1); // Wait if device is busy, then repeat the same call
                    }
                }
                else if (MFX_ERR_NONE < status && syncp) {
                    status = MFX_ERR_NONE; // Ignore warnings if output is available
                    break;
                }
                else if (MFX_ERR_NOT_ENOUGH_BUFFER == status) {
                    // Allocate more bitstream buffer memory here if needed...
                    break;
                }
                else {
                    if (status != MFX_ERR_MORE_DATA) {
                        INTEL_CHECK_STATUS(status);
                    }
                    break;
                }
            }
            if (MFX_ERR_NONE == status) {
                INTEL_CHECK_STATUS(m_pSession->SyncOperation(syncp, 60000)); // Synchronize. Wait until encoded frame is ready
                if (m_sBitstream.DataLength > 0) {
                    tdav_codec_h264_rtp_encap(pWrappedCodecCommon, (const uint8_t*)(m_sBitstream.Data + m_sBitstream.DataOffset), (tsk_size_t)m_sBitstream.DataLength);
                    m_sBitstream.DataLength = 0;
                    pWrappedCodecH264->encoder.force_idr = tsk_false; // reset
                }
            }
        }
        while (0);

        //
        // Stage 2: Retrieve the buffered encoded frames
        //
        while (MFX_ERR_NONE <= status) {
            for (;;) {
                // Encode a frame asychronously (returns immediately)
                status = m_Inst.EncodeFrameAsync(&m_sFrameCtrl, NULL, &m_sBitstream, &syncp);

                if (MFX_ERR_NONE < status && !syncp) { // Repeat the call if warning and no output
                    if (MFX_WRN_DEVICE_BUSY == status) {
                        tsk_thread_sleep(1); // Wait if device is busy, then repeat the same call
                    }
                }
                else if (MFX_ERR_NONE < status && syncp) {
                    status = MFX_ERR_NONE; // Ignore warnings if output is available
                    break;
                }
                else {
                    break;
                }
            }

            if (MFX_ERR_NONE == status) {
                INTEL_CHECK_STATUS(m_pSession->SyncOperation(syncp, 60000)); // Synchronize. Wait until encoded frame is
                if (m_sBitstream.DataLength > 0) {
                    tdav_codec_h264_rtp_encap(pWrappedCodecCommon, (const uint8_t*)(m_sBitstream.Data + m_sBitstream.DataOffset), (tsk_size_t)m_sBitstream.DataLength);
                    m_sBitstream.DataLength = 0;
                    pWrappedCodecH264->encoder.force_idr = tsk_false; // reset
                }
            }
        }

bail:
        return MFX_ERR_NONE;
    }

private:
    MFXVideoENCODE m_Inst;
    mfxEncodeCtrl m_sFrameCtrl;
    mfxExtCodingOption m_sOptLowLatency;
    mfxExtCodingOption2 m_sOpt2MaxFrameSize;
    mfxExtBuffer* m_pExtendedBuffers[2]; // Not allocated
};


//
//	IntelCodecDecoder
//
class IntelCodecDecoder : public IntelCodec
{
public:
    IntelCodecDecoder(MFXVideoSession* pSession)
        : IntelCodec(pSession)
        , m_Inst(*pSession)
        , m_pAccumulatorPtr(NULL)
        , m_nAccumulatorSize(0)
        , m_nAccumulatorPos(0)
        , m_bInit(false) {

    }
    virtual ~IntelCodecDecoder() {
        Close();
    }

    virtual mfxStatus Close() {
        m_Inst.Close();
        TSK_FREE(m_pAccumulatorPtr);
        m_nAccumulatorSize = 0;
        m_nAccumulatorPos = 0;
        m_bInit = false;
        return IntelCodec::Close();
    }

    mfxStatus Open(struct tdav_codec_h264_intel_s* pWrappedCodec) {
        tdav_codec_h264_common_t* pWrappedCodecCommon = (tdav_codec_h264_common_t*)pWrappedCodec;

        INTEL_DEBUG_INFO("Decoder.Open width=%d, height=%d, fps=%d",
                         TMEDIA_CODEC_VIDEO(pWrappedCodec)->in.width,
                         TMEDIA_CODEC_VIDEO(pWrappedCodec)->in.height,
                         TMEDIA_CODEC_VIDEO(pWrappedCodec)->in.fps
                        );

        // Allocation will be done each time we decode the SPS:PPS header

        m_bOpened = true;
        return MFX_ERR_NONE;
    }

    mfxU32 Decode(struct tmedia_codec_s* pWrappedCodec, const mfxU8* pcInDataPtr, mfxU32 nInDataSize, void **ppOutDataPtr, tsk_size_t *pOutDataMaxSize, const trtp_rtp_header_t* pcRtpHdr) {
        mfxU32 nRetSize = 0, nOutXSize;
        mfxStatus status = MFX_ERR_NONE;
        tsk_bool_t append_scp, end_of_unit;
        tsk_bool_t sps_or_pps;
        const uint8_t* pay_ptr = tsk_null;
        tsk_size_t pay_size = 0, size_to_copy = 0;
        bool bGotFrame = false;
        mfxFrameSurface1* pmfxOutSurface = NULL;
        static const tsk_size_t xmax_size = (3840 * 2160 * 3) >> 3; // >>3 instead of >>1 (not an error)
        static tsk_size_t start_code_prefix_size = sizeof(H264_START_CODE_PREFIX);

        tdav_codec_h264_intel_t* pWrappedCodecH264 = (tdav_codec_h264_intel_t*)pWrappedCodec;
        tdav_codec_h264_common_t* pWrappedCodecCommon = (tdav_codec_h264_common_t*)pWrappedCodec;

        if (!pWrappedCodec || !pcInDataPtr || !nInDataSize || !ppOutDataPtr) {
            INTEL_CHECK_STATUS(MFX_ERR_NULL_PTR);
        }
        //INTEL_DEBUG_INFO("Size=%u", nInDataSize);
        if (!m_bOpened) {
            INTEL_CHECK_STATUS(MFX_ERR_NOT_INITIALIZED);
        }


        /* 5.3. NAL Unit Octet Usage
        +---------------+
        |0|1|2|3|4|5|6|7|
        +-+-+-+-+-+-+-+-+
        |F|NRI|  Type   |
        +---------------+
        */
        if (pcInDataPtr[0] & 0x80) { // F ?== 1
            /* reset accumulator */
            m_nAccumulatorPos = 0;
            INTEL_CHECK_STATUS(status = MFX_ERR_UNDEFINED_BEHAVIOR);
        }

        // New frame?
        if (m_nLastRtpTimestamp != pcRtpHdr->timestamp) {
            m_nAccumulatorPos = 0;
            m_nLastRtpTimestamp = pcRtpHdr->timestamp;
        }

        /* get payload */
        if ((tdav_codec_h264_get_pay(pcInDataPtr, nInDataSize, (const void**)&pay_ptr, &pay_size, &append_scp, &end_of_unit) != 0) || !pay_ptr || !pay_size) {
            INTEL_BREAK("Depayloader failed to get H.264 content");
        }
#if 1 // TODO: MSDK cannot decode slices
        end_of_unit = pcRtpHdr->marker;
#endif
        //append_scp = tsk_true;
        size_to_copy = pay_size + (append_scp ? start_code_prefix_size : 0);
        // whether it's SPS or PPS (append_scp is false for subsequent FUA chuncks)
        sps_or_pps = append_scp && pay_ptr && ((pay_ptr[0] & 0x1F) == 7 || (pay_ptr[0] & 0x1F) == 8);

        // start-accumulator
        if (!m_pAccumulatorPtr) {
            if (size_to_copy > xmax_size) {
                INTEL_DEBUG_ERROR("%u too big to contain valid encoded data. xmax_size=%u", size_to_copy, xmax_size);
                m_nAccumulatorPos = 0;
                return 0;
            }
            if (!(m_pAccumulatorPtr = (mfxU8*)tsk_calloc(size_to_copy, sizeof(mfxU8)))) {
                INTEL_DEBUG_ERROR("Failed to allocated new buffer");
                m_nAccumulatorPos = 0;
                return 0;
            }
            m_nAccumulatorSize = (mfxU32)size_to_copy;
        }
        if ((m_nAccumulatorPos + size_to_copy) >= xmax_size) {
            INTEL_DEBUG_ERROR("BufferOverflow");
            m_nAccumulatorPos = 0;
            return 0;
        }
        if ((m_nAccumulatorPos + size_to_copy) > m_nAccumulatorSize) {
            if (!(m_pAccumulatorPtr = (mfxU8*)tsk_realloc(m_pAccumulatorPtr, (m_nAccumulatorPos + size_to_copy)))) {
                INTEL_DEBUG_ERROR("Failed to reallocated new buffer");
                m_nAccumulatorPos = 0;
                m_nAccumulatorSize = 0;
                return 0;
            }
            m_nAccumulatorSize = (mfxU32)(m_nAccumulatorPos + size_to_copy);
        }

        if (append_scp) {
            memcpy(&m_pAccumulatorPtr[m_nAccumulatorPos], H264_START_CODE_PREFIX, start_code_prefix_size);
            m_nAccumulatorPos += (mfxU32)start_code_prefix_size;
        }
        memcpy(&m_pAccumulatorPtr[m_nAccumulatorPos], pay_ptr, pay_size);
        m_nAccumulatorPos += (mfxU32)pay_size;
        // end-accumulator

        if (/*rtp_hdr->marker*/end_of_unit) {
            /* decode the picture */
            mfxU32 nOutWidth, nOutHeight;

            // Decode a Unit
            status = DecodeFrame(pWrappedCodecH264, m_pAccumulatorPtr, m_nAccumulatorPos, !!sps_or_pps, &pmfxOutSurface, bGotFrame);
            if (status != MFX_ERR_NONE) {
                INTEL_DEBUG_WARN("DecodeFrame failed: %d", status);
                goto bail;
            }

            // Do we have a complete frame?
            if (!bGotFrame || !pmfxOutSurface) {
                goto bail;
            }

#if INTEL_DX11_D3D
            INTEL_CHECK_STATUS(status = pWrappedCodecH264->D3D11Allocator.Lock(pWrappedCodecH264->D3D11Allocator.pthis, pmfxOutSurface->Data.MemId, &(pmfxOutSurface->Data)));
#endif
            if (!pmfxOutSurface->Data.Y || !pmfxOutSurface->Data.U || !pmfxOutSurface->Data.V) {
#if INTEL_DX11_D3D
                INTEL_CHECK_STATUS(status = pWrappedCodecH264->D3D11Allocator.Unlock(pWrappedCodecH264->D3D11Allocator.pthis, pmfxOutSurface->Data.MemId, &(pmfxOutSurface->Data)));
#endif
                goto bail;
            }

            if (pmfxOutSurface->Info.CropW > 0 && pmfxOutSurface->Info.CropH > 0) {
                nOutWidth = pmfxOutSurface->Info.CropW;
                nOutHeight = pmfxOutSurface->Info.CropH;
            }
            else {
                nOutWidth = pmfxOutSurface->Info.Width;
                nOutHeight = pmfxOutSurface->Info.Height;
            }

            nOutXSize = (nOutWidth * nOutHeight * 3) >> 1; // I420
            /* IDR ? */
            if (((pay_ptr[0] & 0x1F) == 0x05) && TMEDIA_CODEC_VIDEO(pWrappedCodec)->in.callback) {
                INTEL_DEBUG_INFO("Decoded H.264 IDR");
                TMEDIA_CODEC_VIDEO(pWrappedCodec)->in.result.type = tmedia_video_decode_result_type_idr;
                TMEDIA_CODEC_VIDEO(pWrappedCodec)->in.result.proto_hdr = pcRtpHdr;
                TMEDIA_CODEC_VIDEO(pWrappedCodec)->in.callback(&TMEDIA_CODEC_VIDEO(pWrappedCodec)->in.result);
            }
            /* fill out */
            if (*pOutDataMaxSize < nOutXSize) {
                if ((*ppOutDataPtr = tsk_realloc(*ppOutDataPtr, nOutXSize))) {
                    *pOutDataMaxSize = nOutXSize;
                }
                else {
                    *pOutDataMaxSize = 0;
                    return 0;
                }
            }
            TMEDIA_CODEC_VIDEO(pWrappedCodec)->in.width = nOutWidth;
            TMEDIA_CODEC_VIDEO(pWrappedCodec)->in.height = nOutHeight;

            /* layout picture */
            INTEL_CHECK_STATUS(status = IntelCodecDecoder::LayoutPicture(pmfxOutSurface, (mfxU8 *)*ppOutDataPtr));
            nRetSize = nOutXSize;
#if INTEL_DX11_D3D
            INTEL_CHECK_STATUS(status = pWrappedCodecH264->D3D11Allocator.Unlock(pWrappedCodecH264->D3D11Allocator.pthis, pmfxOutSurface->Data.MemId, &(pmfxOutSurface->Data)));
#endif
        } // else if(rtp_hdr->marker)

bail:
        if (end_of_unit) {
            /* reset accumulator */
            m_nAccumulatorPos = 0;
        }

        if (status != MFX_ERR_NONE) {
            INTEL_DEBUG_INFO("Failed to decode the buffer with error code =%d, size=%u, append=%s", status, m_nAccumulatorPos, append_scp ? "yes" : "no");
            if (TMEDIA_CODEC_VIDEO(pWrappedCodec)->in.callback) {
                TMEDIA_CODEC_VIDEO(pWrappedCodec)->in.result.type = tmedia_video_decode_result_type_error;
                TMEDIA_CODEC_VIDEO(pWrappedCodec)->in.result.proto_hdr = pcRtpHdr;
                TMEDIA_CODEC_VIDEO(pWrappedCodec)->in.callback(&TMEDIA_CODEC_VIDEO(pWrappedCodec)->in.result);
            }
        }
        return nRetSize;
    }

private:
#if 0
    static mfxStatus WriteSection(mfxU8* plane, mfxU16 factor, mfxU16 chunksize, mfxFrameInfo *pInfo, mfxFrameData *pData, mfxU32 i, mfxU32 j, mfxU8 *pDstPtr) {
        memcpy(pDstPtr, plane + (pInfo->CropY * pData->Pitch / factor + pInfo->CropX) + i * pData->Pitch + j, chunksize);
        return MFX_ERR_NONE;
    }
#else
#define WriteSection(_plane, _factor, _chunksize, _pInfo, _pData, _i, _j, _pDstPtr) \
		memcpy((_pDstPtr), (_plane) + ((_pInfo)->CropY * (_pData)->Pitch / (_factor) + (_pInfo)->CropX) + (_i) * (_pData)->Pitch + (_j), (_chunksize))
#define WriteSection1(_plane, _factor, _pInfo, _pData, _i, _j, _pDstPtr) \
		*(_pDstPtr) = *((_plane) + ((_pInfo)->CropY * (_pData)->Pitch / (_factor) + (_pInfo)->CropX) + (_i) * (_pData)->Pitch + (_j));
#endif

    static mfxStatus LayoutPicture(mfxFrameSurface1 *pSurface, mfxU8 *pDstPtr) {
#if 1 // ->YUV420
        mfxFrameInfo *pInfo = &pSurface->Info;
        mfxFrameData *pData = &pSurface->Data;
        mfxU32 i, j, h, w;

        if (pSurface->Info.CropW > 0 && pSurface->Info.CropH > 0) {
            w = pSurface->Info.CropW;
            h = pSurface->Info.CropH;
        }
        else {
            w = pSurface->Info.Width;
            h = pSurface->Info.Height;
        }

        for (i = 0; i < h; i++) {
            WriteSection(pData->Y, 1, w, pInfo, pData, i, 0, pDstPtr);
            pDstPtr += w;
        }

        h >>= 1;
        for (i = 0; i < h; i++) {
            for (j = 0; j < w; j += 2) {
                WriteSection1(pData->UV, 2, pInfo, pData, i, j, pDstPtr);
                pDstPtr += 1;
            }
        }

        for (i = 0; i < h; i++) {
            for (j = 1; j < w; j += 2) {
                WriteSection1(pData->UV, 2, pInfo, pData, i, j, pDstPtr);
                pDstPtr += 1;
            }
        }
#elif 1 // ->NV12
        mfxFrameInfo *pInfo = &pSurface->Info;
        mfxFrameData *pData = &pSurface->Data;
        mfxU32 i, j, h, w;

        if (pSurface->Info.CropW > 0 && pSurface->Info.CropH > 0) {
            w = pSurface->Info.CropW;
            h = pSurface->Info.CropH;
        }
        else {
            w = pSurface->Info.Width;
            h = pSurface->Info.Height;
        }

        for (i = 0; i < h; i++) {
            WriteSection(pData->Y, 1, w, pInfo, pData, i, 0, pDstPtr);
            pDstPtr += w;
        }

        h >>= 1;
        for (i = 0; i < h; i++) {
            for (j = 0; j < w; j += 2) {
                WriteSection1(pData->UV, 2, pInfo, pData, i, j, &pDstPtr[0]);
                WriteSection1(pData->UV, 2, pInfo, pData, i, j + 1, &pDstPtr[1]);
                pDstPtr += 2;
            }
        }
#endif

        return MFX_ERR_NONE;
    }

    mfxStatus DecodeFrame(struct tdav_codec_h264_intel_s* pWrappedCodec, const mfxU8* pcInDataPtr, mfxU32 nInDataSize, bool bSpsOrPps, mfxFrameSurface1** ppmfxOutSurface, bool &bGotFrame) {
        mfxStatus status = MFX_ERR_NONE;
        int nSurfaceIndex;
        mfxSyncPoint syncp;
        bGotFrame = false;
        *ppmfxOutSurface = NULL;
        mfxFrameSurface1* pmfxOutSurface = NULL;

#if 0
        if (!bSpsOrPps && !m_bInit) {
            INTEL_CHECK_STATUS(status = MFX_ERR_NOT_INITIALIZED);
        }
#endif

        if (m_sBitstream.DataLength < nInDataSize) {
            INTEL_CHECK_STATUS(status = AllocateBitstream(nInDataSize));
        }
        memcpy(m_sBitstream.Data, pcInDataPtr, nInDataSize);
        m_sBitstream.DataOffset = 0;
        m_sBitstream.DataLength = nInDataSize;
        m_sBitstream.DataFlag = MFX_BITSTREAM_COMPLETE_FRAME;

        if (bSpsOrPps || !m_bInit) {
            memset(&m_sParamReq, 0, sizeof(m_sParamReq));
            m_sParamReq.mfx.CodecId = MFX_CODEC_AVC;
            m_sParamReq.mfx.FrameInfo.FourCC = MFX_FOURCC_NV12;
            m_sParamReq.mfx.FrameInfo.ChromaFormat = MFX_CHROMAFORMAT_YUV420;
#if INTEL_DX11_D3D
            m_sParamReq.IOPattern = MFX_IOPATTERN_OUT_VIDEO_MEMORY;
#else
            m_sParamReq.IOPattern = MFX_IOPATTERN_OUT_SYSTEM_MEMORY;
#endif
            m_sParamReq.AsyncDepth = 1; // Low latency: limits internal frame buffering

            status = m_Inst.DecodeHeader(&m_sBitstream, &m_sParamReq);
            if (status == MFX_WRN_PARTIAL_ACCELERATION) {
                status = MFX_ERR_NONE;
            }
            INTEL_CHECK_STATUS(status);

            memcpy(&m_sParamSel, &m_sParamReq, sizeof(m_sParamSel));

            // Check parameters
            status = m_Inst.Query(&m_sParamReq, &m_sParamReq);
            if (status != MFX_ERR_NONE && status != MFX_WRN_INCOMPATIBLE_VIDEO_PARAM /* Best one will be selected by the encoder */) {
                INTEL_CHECK_STATUS(status);
            }

            // Query number required surfaces for encoder
            memset(&m_sAllocRequest, 0, sizeof(m_sAllocRequest));
            INTEL_CHECK_STATUS(status = m_Inst.QueryIOSurf(&m_sParamReq, &m_sAllocRequest));
#if INTEL_DX11_D3D
            m_sAllocRequest.Type |= D3D11_WILL_READ; // Hint to DX11 memory handler that application will read data from output surfaces
#endif

            // Allocate surfaces for decoder
#if INTEL_DX11_D3D
            INTEL_CHECK_STATUS(status = pWrappedCodec->D3D11Allocator.Alloc(pWrappedCodec->D3D11Allocator.pthis, &m_sAllocRequest, &m_sD3D11Response));
            if (m_sD3D11Response.NumFrameActual == 0) {
                INTEL_CHECK_STATUS(status = MFX_ERR_UNKNOWN);
            }
            INTEL_DEBUG_INFO("nEncSurfNum = %hu", m_sD3D11Response.NumFrameActual);
            INTEL_CHECK_STATUS(status = AllocSurfaces(m_sD3D11Response.NumFrameActual, m_sAllocRequest.Info.Width, m_sAllocRequest.Info.Height, &m_sParamReq.mfx.FrameInfo));
#else
            INTEL_DEBUG_INFO("nEncSurfNum = %hu", m_sAllocRequest.NumFrameSuggested);
            INTEL_CHECK_STATUS(status = AllocSurfaces(m_sAllocRequest.NumFrameSuggested, m_sAllocRequest.Info.Width, m_sAllocRequest.Info.Height, &m_sParamReq.mfx.FrameInfo));
#endif
            // Initialize the Media SDK decoder
            status = m_Inst.Init(&m_sParamReq);
            if (status != MFX_ERR_NONE && status != MFX_WRN_PARTIAL_ACCELERATION) {
                INTEL_CHECK_STATUS(status);
            }
            INTEL_DEBUG_INFO("Decoder->Init() returned: %d", status);

            m_bInit = true;
        }

        //
        // Stage 1: Main decoding loop
        //
        while (MFX_ERR_NONE <= status || MFX_ERR_MORE_DATA == status || MFX_ERR_MORE_SURFACE == status) {
            if (MFX_WRN_DEVICE_BUSY == status) {
                tsk_thread_sleep(1); // Wait if device is busy, then repeat the same call to DecodeFrameAsync
            }

            if (MFX_ERR_MORE_DATA == status) {
                return MFX_ERR_NONE;
            }

            if (MFX_ERR_MORE_SURFACE == status || MFX_ERR_NONE == status) {
                nSurfaceIndex = GetFreeSurfaceIndex(); // Find free frame surface
                if (MFX_ERR_NOT_FOUND == nSurfaceIndex) {
                    INTEL_CHECK_STATUS((status = MFX_ERR_MEMORY_ALLOC));
                }
            }

            // Decode a frame asychronously (returns immediately)
            //  - If input bitstream contains multiple frames DecodeFrameAsync will start decoding multiple frames, and remove them from bitstream
            status = m_Inst.DecodeFrameAsync(&m_sBitstream, m_ppSurfacePtrs[nSurfaceIndex], &pmfxOutSurface, &syncp);

            // Ignore warnings if output is available,
            // if no output and no action required just repeat the DecodeFrameAsync call
            if (MFX_ERR_NONE < status && syncp) {
                status = MFX_ERR_NONE;
            }

            if (MFX_ERR_NONE == status) {
                status = m_pSession->SyncOperation(syncp, 60000); // Synchronize. Wait until decoded frame is ready
            }

            if (MFX_ERR_NONE == status) {
                bGotFrame = true;
                if (pmfxOutSurface) {
                    *ppmfxOutSurface = pmfxOutSurface;
                }
            }
        }

        //
        // Stage 2: Retrieve the buffered decoded frames
        //
        while (MFX_ERR_NONE <= status || MFX_ERR_MORE_SURFACE == status) {
            if (MFX_WRN_DEVICE_BUSY == status) {
                tsk_thread_sleep(1); // Wait if device is busy, then repeat the same call to DecodeFrameAsync
            }

            nSurfaceIndex = GetFreeSurfaceIndex(); // Find free frame surface
            if (MFX_ERR_NOT_FOUND == nSurfaceIndex) {
                INTEL_CHECK_STATUS((status = MFX_ERR_MEMORY_ALLOC));
            }

            // Decode a frame asychronously (returns immediately)
            status = m_Inst.DecodeFrameAsync(NULL, m_ppSurfacePtrs[nSurfaceIndex], ppmfxOutSurface, &syncp);

            // Ignore warnings if output is available,
            // if no output and no action required just repeat the DecodeFrameAsync call
            if (MFX_ERR_NONE < status && syncp) {
                status = MFX_ERR_NONE;
            }

            if (MFX_ERR_NONE == status) {
                status = m_pSession->SyncOperation(syncp, 60000); // Synchronize. Waits until decoded frame is ready
            }

            if (MFX_ERR_NONE == status) {
                bGotFrame = true;
                if (pmfxOutSurface) {
                    *ppmfxOutSurface = pmfxOutSurface;
                }
            }
        }

        status = MFX_ERR_NONE;

bail:
        return status;
    }
private:
    mfxU8 *m_pAccumulatorPtr;
    mfxU32 m_nAccumulatorSize;
    mfxU32 m_nAccumulatorPos;
    mfxU32 m_nLastRtpTimestamp;
    MFXVideoDECODE m_Inst;
    bool m_bInit;
};

/* ============ H.264 Base/Main Profile X.X Plugin interface functions ================= */

static int tdav_codec_h264_intel_set(tmedia_codec_t* self, const tmedia_param_t* param)
{
    tdav_codec_h264_intel_t* h264 = (tdav_codec_h264_intel_t*)self;
    if (param->value_type == tmedia_pvt_int32) {
        if (tsk_striequals(param->key, "action")) {
            tmedia_codec_action_t action = (tmedia_codec_action_t)TSK_TO_INT32((uint8_t*)param->value);
            switch (action) {
            case tmedia_codec_action_encode_idr: {
                h264->encoder.force_idr = tsk_true;
                return 0;
            }
            case tmedia_codec_action_bw_up:
            case tmedia_codec_action_bw_down: {
                if (self->opened) {
                    INTEL_CHECK_STATUS(h264->encoder.pInst->UpdateBandwidth(action == tmedia_codec_action_bw_up, TMEDIA_CODEC(h264)->bandwidth_max_upload));
                }
                break;
            }
            }
        }
        else if (tsk_striequals(param->key, "bw_kbps")) { // both up and down (from the SDP)
            int32_t max_bw_userdefine = tmedia_defaults_get_bandwidth_video_upload_max();
            int32_t max_bw_new = *((int32_t*)param->value);
            if (max_bw_userdefine > 0) {
                // do not use more than what the user defined in it's configuration
                TMEDIA_CODEC(h264)->bandwidth_max_upload = TSK_MIN(max_bw_new, max_bw_userdefine);
            }
            else {
                TMEDIA_CODEC(h264)->bandwidth_max_upload = max_bw_new;
            }
            INTEL_DEBUG_INFO("bandwidth-max-upload=%d", TMEDIA_CODEC(h264)->bandwidth_max_upload);
            if (self->opened) {
                INTEL_CHECK_STATUS(h264->encoder.pInst->SetMaxBandwidth(TMEDIA_CODEC(h264)->bandwidth_max_upload));
            }
            return 0;
        }
        else if (tsk_striequals(param->key, "bandwidth-max-upload")) {
            int32_t bw_max_upload = *((int32_t*)param->value);
            TSK_DEBUG_INFO("OpenH264 codec: bandwidth-max-upload=%d", bw_max_upload);
            TMEDIA_CODEC(h264)->bandwidth_max_upload = bw_max_upload;
            if (self->opened) {
                INTEL_CHECK_STATUS(h264->encoder.pInst->SetMaxBandwidth(TMEDIA_CODEC(h264)->bandwidth_max_upload));
            }
            return 0;
        }
        else if (tsk_striequals(param->key, "rotation")) {
            int rotation = *((int32_t*)param->value);
            if (h264->encoder.rotation != rotation) {
                if (self->opened) {
                    int ret;
                    h264->encoder.rotation = rotation;
                    if ((ret = tdav_codec_h264_intel_close_encoder(h264))) {
                        return ret;
                    }
                    if ((ret = tdav_codec_h264_intel_open_encoder(h264))) {
                        return ret;
                    }
                }
            }
            return 0;
        }
    }
bail:

    return -1;
}


static int tdav_codec_h264_intel_open(tmedia_codec_t* self)
{
    int ret;
    tdav_codec_h264_intel_t* h264 = (tdav_codec_h264_intel_t*)self;

    if (!h264) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    /* the caller (base class) already checked that the codec is not opened */

    //	Encoder
    if ((ret = tdav_codec_h264_intel_open_encoder(h264))) {
        return ret;
    }

    //	Decoder
    if ((ret = tdav_codec_h264_intel_open_decoder(h264))) {
        return ret;
    }

    return 0;
}

static int tdav_codec_h264_intel_close(tmedia_codec_t* self)
{
    tdav_codec_h264_intel_t* h264 = (tdav_codec_h264_intel_t*)self;

    if (!h264) {
        INTEL_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    /* the caller (base class) alreasy checked that the codec is opened */

    //	Encoder
    tdav_codec_h264_intel_close_encoder(h264);

    //	Decoder
    tdav_codec_h264_intel_close_decoder(h264);

    return 0;
}

static tsk_size_t tdav_codec_h264_intel_encode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size)
{
    tdav_codec_h264_intel_t* h264 = (tdav_codec_h264_intel_t*)self;
    (void)(out_data);
    (void)(out_max_size);

    if (!h264 || !h264->encoder.pInst) {
        INTEL_CHECK_STATUS(MFX_ERR_NULL_PTR);
    }
    INTEL_CHECK_STATUS(h264->encoder.pInst->Encode(self, (const mfxU8*)in_data, (mfxU32)in_size));
bail:
    return 0;
}

static tsk_size_t tdav_codec_h264_intel_decode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size, const tsk_object_t* proto_hdr)
{
    tdav_codec_h264_intel_t* h264 = (tdav_codec_h264_intel_t*)self;

    if (!h264 || !h264->decoder.pInst) {
        INTEL_CHECK_STATUS(MFX_ERR_NULL_PTR);
    }
    return (tsk_size_t)h264->decoder.pInst->Decode(self, (const mfxU8*)in_data, (mfxU32)in_size, out_data, out_max_size, (const trtp_rtp_header_t*)proto_hdr);
bail:
    return 0;
}

static tsk_bool_t tdav_codec_h264_intel_sdp_att_match(const tmedia_codec_t* self, const char* att_name, const char* att_value)
{
    return tdav_codec_h264_common_sdp_att_match((tdav_codec_h264_common_t*)self, att_name, att_value);
}

static char* tdav_codec_h264_intel_sdp_att_get(const tmedia_codec_t* self, const char* att_name)
{
    char* att = tdav_codec_h264_common_sdp_att_get((const tdav_codec_h264_common_t*)self, att_name);
    if (att && tsk_striequals(att_name, "fmtp")) {
        tsk_strcat(&att, "; impl=intel");
    }
    return att;
}

/* ============ H.264 Base Profile Plugin interface ================= */

/* constructor */
static tsk_object_t* tdav_codec_h264_intel_base_ctor(tsk_object_t * self, va_list * app)
{
    tdav_codec_h264_intel_t *h264 = (tdav_codec_h264_intel_t*)self;
    if (h264) {
        /* init base: called by tmedia_codec_create() */
        /* init self */
        if (tdav_codec_h264_intel_init(h264, profile_idc_baseline) != 0) {
            return tsk_null;
        }
    }
    return self;
}
/* destructor */
static tsk_object_t* tdav_codec_h264_intel_base_dtor(tsk_object_t * self)
{
    tdav_codec_h264_intel_t *h264 = (tdav_codec_h264_intel_t*)self;
    if (h264) {
        /* deinit base */
        tdav_codec_h264_common_deinit(TDAV_CODEC_H264_COMMON(self));
        /* deinit self */
        tdav_codec_h264_intel_deinit(h264);
    }

    return self;
}
/* object definition */
static const tsk_object_def_t tdav_codec_h264_intel_base_def_s = {
    sizeof(tdav_codec_h264_intel_t),
    tdav_codec_h264_intel_base_ctor,
    tdav_codec_h264_intel_base_dtor,
    tmedia_codec_cmp,
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t tdav_codec_h264_intel_base_plugin_def_s = {
    &tdav_codec_h264_intel_base_def_s,

    tmedia_video,
    tmedia_codec_id_h264_bp,
    "H264",
    "H264 Base Profile (Intel Media SDK)",
    TMEDIA_CODEC_FORMAT_H264_BP,
    tsk_true,
    90000, // rate

    /* audio */
    { 0 },

    /* video (width, height, fps) */
    { 176, 144, 0 }, // fps is @deprecated

    tdav_codec_h264_intel_set,
    tdav_codec_h264_intel_open,
    tdav_codec_h264_intel_close,
    tdav_codec_h264_intel_encode,
    tdav_codec_h264_intel_decode,
    tdav_codec_h264_intel_sdp_att_match,
    tdav_codec_h264_intel_sdp_att_get
};
const tmedia_codec_plugin_def_t *tdav_codec_h264_intel_base_plugin_def_t = &tdav_codec_h264_intel_base_plugin_def_s;


/* ============ H.264 Main Profile Plugin interface ================= */

/* constructor */
static tsk_object_t* tdav_codec_h264_intel_main_ctor(tsk_object_t * self, va_list * app)
{
    tdav_codec_h264_intel_t *h264 = (tdav_codec_h264_intel_t*)self;
    if (h264) {
        /* init main: called by tmedia_codec_create() */
        /* init self */
        if (tdav_codec_h264_intel_init(h264, profile_idc_main) != 0) {
            return tsk_null;
        }
    }
    return self;
}
/* destructor */
static tsk_object_t* tdav_codec_h264_intel_main_dtor(tsk_object_t * self)
{
    tdav_codec_h264_intel_t *h264 = (tdav_codec_h264_intel_t*)self;
    if (h264) {
        /* deinit main */
        tdav_codec_h264_common_deinit(TDAV_CODEC_H264_COMMON(self));
        /* deinit self */
        tdav_codec_h264_intel_deinit(h264);
    }

    return self;
}
/* object definition */
static const tsk_object_def_t tdav_codec_h264_intel_main_def_s = {
    sizeof(tdav_codec_h264_intel_t),
    tdav_codec_h264_intel_main_ctor,
    tdav_codec_h264_intel_main_dtor,
    tmedia_codec_cmp,
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t tdav_codec_h264_intel_main_plugin_def_s = {
    &tdav_codec_h264_intel_main_def_s,

    tmedia_video,
    tmedia_codec_id_h264_mp,
    "H264",
    "H264 main Profile (Intel Media SDK)",
    TMEDIA_CODEC_FORMAT_H264_MP,
    tsk_true,
    90000, // rate

    /* audio */
    { 0 },

    /* video (width, height, fps) */
    { 176, 144, 0 }, // fps is @deprecated

    tdav_codec_h264_intel_set,
    tdav_codec_h264_intel_open,
    tdav_codec_h264_intel_close,
    tdav_codec_h264_intel_encode,
    tdav_codec_h264_intel_decode,
    tdav_codec_h264_intel_sdp_att_match,
    tdav_codec_h264_intel_sdp_att_get
};
const tmedia_codec_plugin_def_t *tdav_codec_h264_intel_main_plugin_def_t = &tdav_codec_h264_intel_main_plugin_def_s;

/* ============ Common To all H264 profiles ================= */

static int tdav_codec_h264_intel_open_encoder(tdav_codec_h264_intel_t* self)
{
    if (self->encoder.pInst) {
        INTEL_BREAK("Already initialized");
    }

    if (!(self->encoder.pInst = new IntelCodecEncoder(self->mfxSession))) {
        INTEL_CHECK_STATUS(MFX_ERR_MEMORY_ALLOC);
    }
    INTEL_CHECK_STATUS(self->encoder.pInst->Open(self));

    return 0;

bail:
    if (self->encoder.pInst) {
        delete self->encoder.pInst, self->encoder.pInst = NULL;
    }
    return -1;
}

static int tdav_codec_h264_intel_close_encoder(tdav_codec_h264_intel_t* self)
{
    if (self) {
        if (self->encoder.pInst) {
            delete self->encoder.pInst;
            self->encoder.pInst = NULL;
        }
        self->encoder.frame_count = 0;
    }
    return 0;
}

int tdav_codec_h264_intel_open_decoder(tdav_codec_h264_intel_t* self)
{
    if (self->decoder.pInst) {
        INTEL_BREAK("Already initialized");
    }

    if (!(self->decoder.pInst = new IntelCodecDecoder(self->mfxSession))) {
        INTEL_CHECK_STATUS(MFX_ERR_MEMORY_ALLOC);
    }
    INTEL_CHECK_STATUS(self->decoder.pInst->Open(self));

    return 0;

bail:
    if (self->decoder.pInst) {
        delete self->decoder.pInst, self->decoder.pInst = NULL;
    }
    return -1;
}

static int tdav_codec_h264_intel_close_decoder(tdav_codec_h264_intel_t* self)
{
    if (self) {
        if (self->decoder.pInst) {
            delete self->decoder.pInst;
            self->decoder.pInst = NULL;
        }
    }
    return 0;
}

static int tdav_codec_h264_intel_init(tdav_codec_h264_intel_t* self, profile_idc_t profile)
{
    int ret = -1;
    level_idc_t level;
    tdav_codec_h264_common_t* common = (tdav_codec_h264_common_t*)self;
#if INTEL_DX11_D3D
    mfxHDL deviceHandle = NULL;
#endif

    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        goto bail;
    }

    if ((ret = tdav_codec_h264_common_init(common))) {
        TSK_DEBUG_ERROR("tdav_codec_h264_intel_common_init() faile with error code=%d", ret);
        goto bail;
    }

    if ((ret = tdav_codec_h264_common_level_from_size(TMEDIA_CODEC_VIDEO(self)->out.width, TMEDIA_CODEC_VIDEO(self)->out.height, &level))) {
        TSK_DEBUG_ERROR("Failed to find level for size=[%u, %u]", TMEDIA_CODEC_VIDEO(self)->out.width, TMEDIA_CODEC_VIDEO(self)->out.height);
        goto bail;
    }

    if (/*MFUtils::IsLowLatencyH264SupportsMaxSliceSize()*/0) { // TODO: MSDK doesn't support PM=0. Neg. PM=1 but try to do the best to produce SingleNalUnits
        common->pack_mode_local = H264_PACKETIZATION_MODE;
    }
    else {
        common->pack_mode_local = Non_Interleaved_Mode;
    }
    common->profile = profile;
    common->level = level;
#if 0
    // A.2.1.1 Constrained Baseline profile
    // Conformance of a bitstream to the Constrained Baseline profile is indicated by profile_idc being equal to 66 with
    // constraint_set1_flag being equal to 1.
    common->profile_iop = 0xe0; // "constraint_set0_flag=1 and constraint_set1_flag=1" -> Constrained Baseline profile
#endif
    TMEDIA_CODEC_VIDEO(self)->in.max_mbps = TMEDIA_CODEC_VIDEO(self)->out.max_mbps = H264_MAX_MBPS * 1000;
    TMEDIA_CODEC_VIDEO(self)->in.max_br = TMEDIA_CODEC_VIDEO(self)->out.max_br = H264_MAX_BR * 1000;

    TMEDIA_CODEC_VIDEO(self)->in.chroma = tmedia_chroma_yuv420p; // decoded
    TMEDIA_CODEC_VIDEO(self)->out.chroma = tmedia_chroma_yuv420p; // encoded

    if (!self->mfxSession && !(self->mfxSession = new MFXVideoSession())) {
        INTEL_CHECK_STATUS(MFX_ERR_MEMORY_ALLOC);
    }
    INTEL_CHECK_STATUS(self->mfxSession->Init(__IntelDefaultImpl, &__IntelDefaultVer));

#if INTEL_DX11_D3D
    // Create DirectX device context
    INTEL_CHECK_STATUS(D3D11_CreateHWDevice(self, *self->mfxSession, &deviceHandle, NULL));

    // Provide device manager to Media SDK
    INTEL_CHECK_STATUS(self->mfxSession->SetHandle(MFX_HANDLE_D3D11_DEVICE, deviceHandle));

    self->D3D11Allocator.Alloc = D3D11_SimpleAlloc;
    self->D3D11Allocator.Free = D3D11_SimpleFree;
    self->D3D11Allocator.Lock = D3D11_SimpleLock;
    self->D3D11Allocator.Unlock = D3D11_SimpleUnlock;
    self->D3D11Allocator.GetHDL = D3D11_SimpleGethdl;
    self->D3D11Allocator.pthis = self;

    // Since we are using video memory we must provide Media SDK with an external allocator
    INTEL_CHECK_STATUS(self->mfxSession->SetFrameAllocator(&self->D3D11Allocator));
#endif /* INTEL_DX11_D3D */

    ret = 0;

bail:
    return ret;
}

static int tdav_codec_h264_intel_deinit(tdav_codec_h264_intel_t* self)
{
    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tdav_codec_h264_intel_close((tmedia_codec_t*)self);

    if (self->mfxSession) {
        delete self->mfxSession, self->mfxSession = NULL;
    }

#if INTEL_DX11_D3D
    D3D11_CleanupHWDevice(self);
#endif /* INTEL_DX11_D3D */

    return 0;
}


#if INTEL_DX11_D3D

static IDXGIAdapter* D3D11_GetIntelDeviceAdapterHandle(mfxHDL _pthis, mfxSession session)
{
    mfxU32	adapterNum = 0;
    mfxIMPL impl;
    tdav_codec_h264_intel_t* pthis = (tdav_codec_h264_intel_t*)_pthis;

    MFXQueryIMPL(session, &impl);

    mfxIMPL baseImpl = MFX_IMPL_BASETYPE(impl); // Extract Media SDK base implementation type

    // get corresponding adapter number
    for (mfxU8 i = 0; i < sizeof(implTypes) / sizeof(implTypes[0]); i++) {
        if (implTypes[i].impl == baseImpl) {
            adapterNum = implTypes[i].adapterID;
            break;
        }
    }

    HRESULT hres = CreateDXGIFactory(__uuidof(IDXGIFactory2), (void**)(&pthis->pDXGIFactory));
    if (FAILED(hres)) {
        INTEL_DEBUG_ERROR("CreateDXGIFactory failed: %ld", hres);
        return NULL;
    }

    IDXGIAdapter* adapter = NULL;
    hres = pthis->pDXGIFactory->EnumAdapters(adapterNum, &adapter);
    if (FAILED(hres)) {
        INTEL_DEBUG_ERROR("EnumAdapters failed: %ld", hres);
        return NULL;
    }

    return adapter;
}

// Create DirectX 11 device context
// - Required when using D3D surfaces.
// - D3D Device created and handed to Intel Media SDK
// - Intel graphics device adapter will be determined automatically (does not have to be primary),
//   but with the following caveats:
//     - Device must be active (but monitor does NOT have to be attached)
//     - Device must be enabled in BIOS. Required for the case when used together with a discrete graphics card
//     - For switchable graphics solutions (mobile) make sure that Intel device is the active device
static mfxStatus D3D11_CreateHWDevice(mfxHDL _pthis, mfxSession session, mfxHDL* deviceHandle, HWND hWnd)
{
    hWnd; // Window handle not required by DX11 since we do not showcase rendering.

    HRESULT hres = S_OK;
    tdav_codec_h264_intel_t* pthis = (tdav_codec_h264_intel_t*)_pthis;

    static D3D_FEATURE_LEVEL FeatureLevels[] = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0
    };
    D3D_FEATURE_LEVEL pFeatureLevelsOut;

    pthis->hAdapter = D3D11_GetIntelDeviceAdapterHandle(_pthis, session);
    if (NULL == pthis->hAdapter) {
        INTEL_DEBUG_ERROR("D3D11_GetIntelDeviceAdapterHandle failed");
        return MFX_ERR_DEVICE_FAILED;
    }

    hres = D3D11CreateDevice(pthis->hAdapter,
                             D3D_DRIVER_TYPE_UNKNOWN,
                             NULL,
                             0,
                             FeatureLevels,
                             (sizeof(FeatureLevels) / sizeof(FeatureLevels[0])),
                             D3D11_SDK_VERSION,
                             &pthis->pD3D11Device,
                             &pFeatureLevelsOut,
                             &pthis->pD3D11Ctx);
    if (FAILED(hres)) {
        INTEL_DEBUG_ERROR("D3D11CreateDevice failed: %ld", hres);
        return MFX_ERR_DEVICE_FAILED;
    }

    // turn on multithreading for the DX11 cntext
    CComQIPtr<ID3D10Multithread> p_mt(pthis->pD3D11Ctx);
    if (p_mt) {
        p_mt->SetMultithreadProtected(true);
    }
    else {
        INTEL_DEBUG_ERROR("Failed to create ID3D10Multithread object");
        return MFX_ERR_DEVICE_FAILED;
    }

    *deviceHandle = (mfxHDL)pthis->pD3D11Device;
    return MFX_ERR_NONE;
}

static void D3D11_CleanupHWDevice(mfxHDL _pthis)
{
    tdav_codec_h264_intel_t* pthis = (tdav_codec_h264_intel_t*)_pthis;
    pthis->pD3D11Device = NULL;
    pthis->pDXGIFactory = NULL;
    pthis->pD3D11Ctx = NULL;
    pthis->hAdapter = NULL;
    pthis->pAdapter = NULL;
}

static void D3D11_SetHWDeviceContext(mfxHDL _pthis, CComPtr<ID3D11DeviceContext> devCtx)
{
    tdav_codec_h264_intel_t* pthis = (tdav_codec_h264_intel_t*)_pthis;
    pthis->pD3D11Ctx = devCtx;
    devCtx->GetDevice(&pthis->pD3D11Device);
}

// Intel Media SDK memory allocator entrypoints....
// - A slightly different allocation procedure is used for encode, decode and VPP
static mfxStatus _D3D11_SimpleAlloc(mfxHDL _pthis, mfxFrameAllocRequest *request, mfxFrameAllocResponse *response)
{
    HRESULT hRes;
    tdav_codec_h264_intel_t* pthis = (tdav_codec_h264_intel_t*)_pthis;

    // Determine surface format (current simple implementation only supports NV12 and RGB4(32))
    DXGI_FORMAT format;
    if (MFX_FOURCC_NV12 == request->Info.FourCC) {
        format = DXGI_FORMAT_NV12;
    }
    else if (MFX_FOURCC_RGB4 == request->Info.FourCC) {
        format = DXGI_FORMAT_B8G8R8A8_UNORM;
    }
    else {
        format = DXGI_FORMAT_P8;
    }

    // Allocate custom container to keep texture and stage buffers for each surface
    // Container also stores the intended read and/or write operation.
    CustomMemId** mids = new CustomMemId *[request->NumFrameSuggested];
    if (!mids) {
        return MFX_ERR_MEMORY_ALLOC;
    }
    for (int i = 0; i < request->NumFrameSuggested; i++) {
        mids[i] = new CustomMemId;
        memset(mids[i], 0, sizeof(CustomMemId));
        mids[i]->rw = request->Type & 0xF000; // Set intended read/write operation
    }

    request->Type = request->Type & 0x0FFF;

    // because P8 data (bitstream) for h264 encoder should be allocated by CreateBuffer()
    // but P8 data (MBData) for MPEG2 encoder should be allocated by CreateTexture2D()
    if (request->Info.FourCC == MFX_FOURCC_P8) {
        D3D11_BUFFER_DESC desc = { 0 };

        desc.ByteWidth = request->Info.Width * request->Info.Height;
        desc.Usage = D3D11_USAGE_STAGING;
        desc.BindFlags = 0;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;

        ID3D11Buffer * buffer = 0;
        hRes = pthis->pD3D11Device->CreateBuffer(&desc, 0, &buffer);
        if (FAILED(hRes)) {
            INTEL_DEBUG_ERROR("CreateBuffer failed:%ld", hRes);
            return MFX_ERR_MEMORY_ALLOC;
        }

        mids[0]->memId = reinterpret_cast<ID3D11Texture2D *>(buffer);
    }
    else {
        D3D11_TEXTURE2D_DESC desc = { 0 };

        desc.Width = request->Info.Width;
        desc.Height = request->Info.Height;
        desc.MipLevels = 1;
        desc.ArraySize = 1; // number of subresources is 1 in this case
        desc.Format = format;
        desc.SampleDesc.Count = 1;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_DECODER;
        desc.MiscFlags = 0;

        if ((MFX_MEMTYPE_FROM_VPPIN & request->Type) &&
                (DXGI_FORMAT_B8G8R8A8_UNORM == desc.Format)) {
            desc.BindFlags = D3D11_BIND_RENDER_TARGET;
            if (desc.ArraySize > 2) {
                return MFX_ERR_MEMORY_ALLOC;
            }
        }

        if ((MFX_MEMTYPE_FROM_VPPOUT & request->Type) ||
                (MFX_MEMTYPE_VIDEO_MEMORY_PROCESSOR_TARGET & request->Type)) {
            desc.BindFlags = D3D11_BIND_RENDER_TARGET;
            if (desc.ArraySize > 2) {
                return MFX_ERR_MEMORY_ALLOC;
            }
        }

        if (DXGI_FORMAT_P8 == desc.Format) {
            desc.BindFlags = 0;
        }

        ID3D11Texture2D* pTexture2D;

        // Create surface textures
        for (size_t i = 0; i < request->NumFrameSuggested / desc.ArraySize; i++) {
            hRes = pthis->pD3D11Device->CreateTexture2D(&desc, NULL, &pTexture2D);

            if (FAILED(hRes)) {
                INTEL_DEBUG_ERROR("CreateTexture2D failed:%ld", hRes);
                return MFX_ERR_MEMORY_ALLOC;
            }

            mids[i]->memId = pTexture2D;
        }

        desc.ArraySize = 1;
        desc.Usage = D3D11_USAGE_STAGING;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ /*| D3D11_CPU_ACCESS_WRITE*/;
        desc.BindFlags = 0;
        desc.MiscFlags = 0;

        // Create surface staging textures
        for (size_t i = 0; i < request->NumFrameSuggested; i++) {
            hRes = pthis->pD3D11Device->CreateTexture2D(&desc, NULL, &pTexture2D);

            if (FAILED(hRes)) {
                INTEL_DEBUG_ERROR("CreateTexture2D failed:%ld", hRes);
                return MFX_ERR_MEMORY_ALLOC;
            }

            mids[i]->memIdStage = pTexture2D;
        }
    }

    response->mids = (mfxMemId*)mids;
    response->NumFrameActual = request->NumFrameSuggested;

    return MFX_ERR_NONE;
}

static mfxStatus D3D11_SimpleAlloc(mfxHDL _pthis, mfxFrameAllocRequest *request, mfxFrameAllocResponse *response)
{
    tdav_codec_h264_intel_t* pthis = (tdav_codec_h264_intel_t*)_pthis;
    mfxStatus sts = MFX_ERR_NONE;
    int idx = (MFX_MEMTYPE_FROM_DECODE & request->Type) ? 1 : 0;

    if (request->NumFrameSuggested <= pthis->D3D11SavedAllocResponses[idx].NumFrameActual &&
            MFX_MEMTYPE_EXTERNAL_FRAME & request->Type &&
            MFX_MEMTYPE_FROM_DECODE & request->Type &&
            pthis->D3D11SavedAllocResponses[idx].NumFrameActual != 0) {
        // Memory for this request was already allocated during manual allocation stage. Return saved response
        //   When decode acceleration device (DXVA) is created it requires a list of d3d surfaces to be passed.
        //   Therefore Media SDK will ask for the surface info/mids again at Init() stage, thus requiring us to return the saved response
        //   (No such restriction applies to Encode or VPP)

        *response = pthis->D3D11SavedAllocResponses[idx];
    }
    else {
        sts = _D3D11_SimpleAlloc(_pthis, request, response);
        pthis->D3D11SavedAllocResponses[idx] = *response;
    }

    return sts;
}

static mfxStatus D3D11_SimpleLock(mfxHDL _pthis, mfxMemId mid, mfxFrameData *ptr)
{
    HRESULT hRes = S_OK;
    tdav_codec_h264_intel_t* pthis = (tdav_codec_h264_intel_t*)_pthis;

    D3D11_TEXTURE2D_DESC		desc = { 0 };
    D3D11_MAPPED_SUBRESOURCE	lockedRect = { 0 };

    CustomMemId*		memId = (CustomMemId*)mid;
    ID3D11Texture2D*	pSurface = (ID3D11Texture2D *)memId->memId;
    ID3D11Texture2D*	pStage = (ID3D11Texture2D *)memId->memIdStage;

    D3D11_MAP	mapType = D3D11_MAP_READ;
    UINT		mapFlags = D3D11_MAP_FLAG_DO_NOT_WAIT;

    if (NULL == pStage) {
        hRes = pthis->pD3D11Ctx->Map(pSurface, 0, mapType, mapFlags, &lockedRect);
        desc.Format = DXGI_FORMAT_P8;
    }
    else {
        pSurface->GetDesc(&desc);

        // copy data only in case of user wants o read from stored surface
        if (memId->rw & D3D11_WILL_READ) {
            pthis->pD3D11Ctx->CopySubresourceRegion(pStage, 0, 0, 0, 0, pSurface, 0, NULL);
        }

        do {
            hRes = pthis->pD3D11Ctx->Map(pStage, 0, mapType, mapFlags, &lockedRect);
            if (S_OK != hRes && DXGI_ERROR_WAS_STILL_DRAWING != hRes) {
                return MFX_ERR_LOCK_MEMORY;
            }
        }
        while (DXGI_ERROR_WAS_STILL_DRAWING == hRes);
    }

    if (FAILED(hRes)) {
        return MFX_ERR_LOCK_MEMORY;
    }

    switch (desc.Format) {
    case DXGI_FORMAT_NV12:
        ptr->Pitch = (mfxU16)lockedRect.RowPitch;
        ptr->Y = (mfxU8 *)lockedRect.pData;
        ptr->U = (mfxU8 *)lockedRect.pData + desc.Height * lockedRect.RowPitch;
        ptr->V = ptr->U + 1;
        break;
    case DXGI_FORMAT_B8G8R8A8_UNORM:
        ptr->Pitch = (mfxU16)lockedRect.RowPitch;
        ptr->B = (mfxU8 *)lockedRect.pData;
        ptr->G = ptr->B + 1;
        ptr->R = ptr->B + 2;
        ptr->A = ptr->B + 3;
        break;
    case DXGI_FORMAT_P8:
        ptr->Pitch = (mfxU16)lockedRect.RowPitch;
        ptr->Y = (mfxU8 *)lockedRect.pData;
        ptr->U = 0;
        ptr->V = 0;
        break;
    default:
        return MFX_ERR_LOCK_MEMORY;
    }

    return MFX_ERR_NONE;
}

static mfxStatus D3D11_SimpleUnlock(mfxHDL _pthis, mfxMemId mid, mfxFrameData *ptr)
{
    tdav_codec_h264_intel_t* pthis = (tdav_codec_h264_intel_t*)_pthis;

    CustomMemId*		memId = (CustomMemId*)mid;
    ID3D11Texture2D*	pSurface = (ID3D11Texture2D *)memId->memId;
    ID3D11Texture2D*	pStage = (ID3D11Texture2D *)memId->memIdStage;

    if (NULL == pStage) {
        pthis->pD3D11Ctx->Unmap(pSurface, 0);
    }
    else {
        pthis->pD3D11Ctx->Unmap(pStage, 0);
        // copy data only in case of user wants to write to stored surface
        if (memId->rw & D3D11_WILL_WRITE) {
            pthis->pD3D11Ctx->CopySubresourceRegion(pSurface, 0, 0, 0, 0, pStage, 0, NULL);
        }
    }

    if (ptr) {
        ptr->Pitch = 0;
        ptr->U = ptr->V = ptr->Y = 0;
        ptr->A = ptr->R = ptr->G = ptr->B = 0;
    }

    return MFX_ERR_NONE;
}

static mfxStatus D3D11_SimpleGethdl(mfxHDL _pthis, mfxMemId mid, mfxHDL *handle)
{
    _pthis;
    if (NULL == handle) {
        return MFX_ERR_INVALID_HANDLE;
    }

    mfxHDLPair*		pPair = (mfxHDLPair*)handle;
    CustomMemId*	memId = (CustomMemId*)mid;

    pPair->first = memId->memId; // surface texture
    pPair->second = 0;

    return MFX_ERR_NONE;
}

static mfxStatus D3D11_SimpleFree(mfxHDL _pthis, mfxFrameAllocResponse *response)
{
    _pthis;
    if (NULL == response) {
        return MFX_ERR_NULL_PTR;
    }

    if (response->mids) {
        for (mfxU32 i = 0; i < response->NumFrameActual; i++) {
            if (response->mids[i]) {
                CustomMemId*		mid = (CustomMemId*)response->mids[i];
                ID3D11Texture2D*	pSurface = (ID3D11Texture2D *)mid->memId;
                ID3D11Texture2D*	pStage = (ID3D11Texture2D *)mid->memIdStage;
                if (pSurface) {
                    pSurface->Release();
                }
                if (pStage) {
                    pStage->Release();
                }
                delete mid;
            }
        }
    }

    delete[] response->mids;
    response->mids = 0;

    return MFX_ERR_NONE;
}

#endif /* INTEL_DX11_D3D */

#endif /* HAVE_INTEL_MEDIA_SDK */
