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
#include "plugin_win_mf_config.h"
#include "internals/mf_utils.h"
#include "internals/mf_sample_grabber.h"
#include "internals/mf_devices.h"
#include "internals/mf_display_watcher.h"
#include "internals/mf_custom_src.h"
#include "internals/mf_codec.h"

#include "tinymedia/tmedia_defaults.h"
#include "tinymedia/tmedia_producer.h"

#include "tsk_string.h"
#include "tsk_thread.h"
#include "tsk_debug.h"

#include <KS.h>
#include <Codecapi.h>
#include <assert.h>
#include <stdlib.h>     /* mbstowcs, wchar_t(C) */
#include <initguid.h>

// 0: {{[Source] -> (VideoProcessor) -> SampleGrabber}} , {{[Encoder]}} -> RTP
// 1: {{[Source] -> (VideoProcessor) -> [Encoder] -> SampleGrabber}} -> RTP
// (VideoProcessor) is optional
// "{{" and "}}" defines where the graph starts and ends respectively. For "0", [Decoder] is a stand-alone IMFTransform.
#if !defined(PLUGIN_MF_PV_BUNDLE_CODEC)
#	define PLUGIN_MF_PV_BUNDLE_CODEC 1 /* MUST be "1" when the encoder is an Async Transform (e.g. Intel Quick Sync). Use "1" to be sure is will always work. */
#endif /* PLUGIN_MF_PV_BUNDLE_CODEC */

#if !defined(PLUGIN_MF_GOP_SIZE_IN_SECONDS)
#define PLUGIN_MF_GOP_SIZE_IN_SECONDS		60
#endif /* PLUGIN_MF_GOP_SIZE_IN_SECONDS */

DEFINE_GUID(PLUGIN_MF_LOW_LATENCY,
            0x9c27891a, 0xed7a, 0x40e1, 0x88, 0xe8, 0xb2, 0x27, 0x27, 0xa0, 0x24, 0xee);

extern const tmedia_codec_plugin_def_t *mf_codec_h264_main_plugin_def_t;
extern const tmedia_codec_plugin_def_t *mf_codec_h264_base_plugin_def_t;

static void* TSK_STDCALL RunSessionThread(void *pArg);
static int _plugin_win_mf_producer_video_unprepare(struct plugin_win_mf_producer_video_s* pSelf);

//
//	plugin_win_mf_producer_video_t
//
typedef struct plugin_win_mf_producer_video_s {
    TMEDIA_DECLARE_PRODUCER;

    bool bStarted, bPrepared, bMuted;
    tsk_thread_handle_t* ppTread[1];
    HWND hWndPreview;

    int32_t bitrate_bps; // used when encoder bundled only

    DeviceListVideo* pDeviceList;

    MFCodecVideo *pEncoder;
    IMFMediaSession *pSession;
    IMFMediaSource *pSource;
    SampleGrabberCB *pCallback;
    IMFActivate *pSinkGrabber;
    IMFActivate *pSinkActivatePreview;
    DisplayWatcher* pWatcherPreview;
    IMFTopology *pTopology;
    IMFMediaType *pGrabberInputType;
}
plugin_win_mf_producer_video_t;

/* ============ Video MF Producer Interface ================= */
static int plugin_win_mf_producer_video_set(tmedia_producer_t *self, const tmedia_param_t* param)
{
    int ret = 0;
    HRESULT hr = S_OK;
    plugin_win_mf_producer_video_t* pSelf = (plugin_win_mf_producer_video_t*)self;

    if (!pSelf || !param) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if (tsk_striequals(param->key, "action")) {
        tmedia_codec_action_t action = (tmedia_codec_action_t)TSK_TO_INT32((uint8_t*)param->value);
        HRESULT hr = S_OK;
        switch (action) {
        case tmedia_codec_action_encode_idr: {
            if (pSelf->pEncoder) {
                CHECK_HR(hr = pSelf->pEncoder->RequestKeyFrame());
            }
            break;
        }
        case tmedia_codec_action_bw_down: {
            pSelf->bitrate_bps = TSK_CLAMP(0, (int32_t)((pSelf->bitrate_bps << 1) / 3), TMEDIA_CODEC(pSelf)->bandwidth_max_upload);
            TSK_DEBUG_INFO("New target bitrate = %d kbps", pSelf->bitrate_bps);
            if (pSelf->pEncoder) {
                CHECK_HR(hr = pSelf->pEncoder->SetBitRate(pSelf->bitrate_bps));
            }
            break;
        }
        case tmedia_codec_action_bw_up: {
            pSelf->bitrate_bps = TSK_CLAMP(0, (int32_t)((pSelf->bitrate_bps * 3) >> 1), TMEDIA_CODEC(pSelf)->bandwidth_max_upload);
            TSK_DEBUG_INFO("New target bitrate = %d kbps", pSelf->bitrate_bps);
            if (pSelf->pEncoder) {
                CHECK_HR(hr = pSelf->pEncoder->SetBitRate(pSelf->bitrate_bps));
            }
            break;
        }
        }
    }
    else if (param->value_type == tmedia_pvt_int64) {
        if (tsk_striequals(param->key, "local-hwnd")) {
            HWND hWnd = reinterpret_cast<HWND>((INT64)*((int64_t*)param->value));
            if (hWnd != pSelf->hWndPreview) {
                pSelf->hWndPreview = hWnd;
                if (pSelf->pWatcherPreview) {
                    CHECK_HR(hr = pSelf->pWatcherPreview->SetHwnd(hWnd));
                }
            }
        }
    }
    else if (param->value_type == tmedia_pvt_int32) {
        if (tsk_striequals(param->key, "mute")) {
            pSelf->bMuted = (TSK_TO_INT32((uint8_t*)param->value) != 0);
            if (pSelf->pCallback) {
                pSelf->pCallback->SetMute(pSelf->bMuted);
            }
#if 0
            if (pSelf->bStarted && pSelf->pSession) {
                if (pSelf->bMuted) {
                    pSelf->pSession->Pause();
                }
                else {
                    CHECK_HR(hr = MFUtils::RunSession(pSelf->pSession, pSelf->pTopology));
                }
            }
#endif
        }
        else if (tsk_striequals(param->key, "create-on-current-thead")) {
            //producer->create_on_ui_thread = *((int32_t*)param->value) ? tsk_false : tsk_true;
        }
        else if (tsk_striequals(param->key, "plugin-firefox")) {
            //producer->plugin_firefox = (*((int32_t*)param->value) != 0);
            //if(producer->grabber){
            //	producer->grabber->setPluginFirefox((producer->plugin_firefox == tsk_true));
            //}
        }
    }

bail:
    return SUCCEEDED(hr) ? 0 : -1;
}

static int plugin_win_mf_producer_video_prepare(tmedia_producer_t* self, const tmedia_codec_t* codec)
{
    plugin_win_mf_producer_video_t* pSelf = (plugin_win_mf_producer_video_t*)self;

    if (!pSelf || !codec && codec->plugin) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if (pSelf->bPrepared) {
        TSK_DEBUG_WARN("MF video producer already prepared");
        return -1;
    }

    // FIXME: DirectShow requires flipping but not MF
    // The Core library always tries to flip when OSType==Win32. Must be changed
    TMEDIA_CODEC_VIDEO(codec)->out.flip = tsk_false;

    TMEDIA_PRODUCER(pSelf)->video.fps = TMEDIA_CODEC_VIDEO(codec)->out.fps;
    TMEDIA_PRODUCER(pSelf)->video.width = TMEDIA_CODEC_VIDEO(codec)->out.width;
    TMEDIA_PRODUCER(pSelf)->video.height = TMEDIA_CODEC_VIDEO(codec)->out.height;
    TMEDIA_PRODUCER(pSelf)->encoder.codec_id = tmedia_codec_id_none; // means RAW frames as input

    TSK_DEBUG_INFO("MF video producer: fps=%d, width=%d, height=%d",
                   TMEDIA_PRODUCER(pSelf)->video.fps,
                   TMEDIA_PRODUCER(pSelf)->video.width,
                   TMEDIA_PRODUCER(pSelf)->video.height);

    HRESULT hr = S_OK;
    IMFAttributes* pSessionAttributes = NULL;
    IMFTopology *pTopology = NULL;
    IMFMediaSink* pEvr = NULL;
    IMFMediaType* pEncoderInputType = NULL;
    IMFTopologyNode *pNodeGrabber = NULL;
    IMFMediaType* pGrabberNegotiatedInputMedia = NULL;
    BOOL bVideoProcessorIsSupported = FALSE;
    const VideoSubTypeGuidPair *pcPreferredSubTypeGuidPair = NULL;

    // create device list object
    if (!pSelf->pDeviceList && !(pSelf->pDeviceList = new DeviceListVideo())) {
        TSK_DEBUG_ERROR("Failed to create device list");
        hr = E_OUTOFMEMORY;
        goto  bail;
    }
    // enumerate devices
    hr = pSelf->pDeviceList->EnumerateDevices();
    if (!SUCCEEDED(hr)) {
        goto bail;
    }

    // check if we have at least one MF video source connected to the PC
    if (pSelf->pDeviceList->Count() == 0) {
        TSK_DEBUG_WARN("No MF video source could be found...no video will be sent");
        // do not break the negotiation as one-way video connection is a valid use-case
    }
    else {
        // Get best MF video source
        IMFActivate* pActivate = NULL;
        const char* pczSrcFriendlyName = tmedia_producer_get_friendly_name(tmedia_video);
        if (!tsk_strnullORempty(pczSrcFriendlyName)) {
            TSK_DEBUG_INFO("MF pref. video source = %s", pczSrcFriendlyName);
            wchar_t pczwSrcFriendlyName[MAX_PATH] = { 0 };
            mbstowcs(pczwSrcFriendlyName, pczSrcFriendlyName, sizeof(pczwSrcFriendlyName) / sizeof(pczwSrcFriendlyName[0]));
            hr = pSelf->pDeviceList->GetDeviceBest(&pActivate, pczwSrcFriendlyName);
        }
        else {
            hr = pSelf->pDeviceList->GetDeviceBest(&pActivate);
        }
        if (!SUCCEEDED(hr) || !pActivate) {
            TSK_DEBUG_ERROR("Failed to get best MF video source");
            if (!pActivate) {
                hr = E_OUTOFMEMORY;
            }
            goto bail;
        }

        // Create the media source for the device.
        hr = pActivate->ActivateObject(
                 __uuidof(IMFMediaSource),
                 (void**)&pSelf->pSource
             );
        SafeRelease(&pActivate);
        if (!SUCCEEDED(hr)) {
            TSK_DEBUG_ERROR("ActivateObject(MF video source) failed");
            goto bail;
        }

        // Check whether video processor (http://msdn.microsoft.com/en-us/library/windows/desktop/hh162913(v=vs.85).aspx) is supported
        CHECK_HR(hr = MFUtils::IsVideoProcessorSupported(&bVideoProcessorIsSupported));

        // Must not be set because not supported by Frame Rate Converter DSP (http://msdn.microsoft.com/en-us/library/windows/desktop/ff819100(v=vs.85).aspx).aspx) because of color (neither I420 nor NV12)
        // Video Processor (http://msdn.microsoft.com/en-us/library/windows/desktop/hh162913(v=vs.85).aspx) supports both NV12 and I420
        if (!bVideoProcessorIsSupported) {
            UINT32 nWidth, nHeight, nFps;
            hr = MFUtils::GetBestFormat(
                     pSelf->pSource,
                     &MFVideoFormat_I420,
                     (UINT32)TMEDIA_PRODUCER(pSelf)->video.width,
                     (UINT32)TMEDIA_PRODUCER(pSelf)->video.height,
                     (UINT32)TMEDIA_PRODUCER(pSelf)->video.fps,
                     &nWidth,
                     &nHeight,
                     &nFps,
                     &pcPreferredSubTypeGuidPair
                 );
            if (SUCCEEDED(hr)) {
                TSK_DEBUG_INFO("Video processor not supported...using source fps=%u, width=%u, height=%u", nFps, nWidth, nHeight);
                TMEDIA_PRODUCER(pSelf)->video.width = nWidth;
                TMEDIA_PRODUCER(pSelf)->video.height = nHeight;
                TMEDIA_PRODUCER(pSelf)->video.fps = nFps;
            }
        }

        // If H.264 is negotiated for this session then, try to find hardware encoder
        // If no HW encoder is found will fallback to SW implementation from x264
#if PLUGIN_MF_PV_BUNDLE_CODEC
        // Before embedding a H.264 encoder we have to be sure that:
        // - Low latency is supported
        // - The user decided to use MF encoder (Microsoft, Intel Quick Sync or any other)
        if ((codec->id == tmedia_codec_id_h264_bp || codec->id == tmedia_codec_id_h264_mp) && MFUtils::IsLowLatencyH264Supported()) {
            BOOL bMFEncoderIsRegistered =
                (codec->id == tmedia_codec_id_h264_mp && codec->plugin == mf_codec_h264_main_plugin_def_t)
                || (codec->id == tmedia_codec_id_h264_bp && codec->plugin == mf_codec_h264_base_plugin_def_t);
            if (bMFEncoderIsRegistered) {
                // both Microsoft and Intel encoders support NV12 only as input
                // static const BOOL kIsEncoder = TRUE;
                // hr = MFUtils::GetBestCodec(kIsEncoder, MFMediaType_Video, MFVideoFormat_NV12, MFVideoFormat_H264, &pSelf->pEncoder);
                pSelf->pEncoder = (codec->id == tmedia_codec_id_h264_bp) ? MFCodecVideoH264::CreateCodecH264Base(MFCodecType_Encoder) : MFCodecVideoH264::CreateCodecH264Main(MFCodecType_Encoder);
                if (pSelf->pEncoder) {
                    pSelf->pEncoder->setBundled(TRUE);
                    int32_t avg_bitrate_kbps = tmedia_get_video_bandwidth_kbps_2((unsigned int)TMEDIA_PRODUCER(pSelf)->video.width, (unsigned int)TMEDIA_PRODUCER(pSelf)->video.height, TMEDIA_PRODUCER(pSelf)->video.fps);
                    TSK_DEBUG_INFO("MF_MT_AVG_BITRATE defined with value = %d kbps", avg_bitrate_kbps);
                    pSelf->bitrate_bps = (avg_bitrate_kbps * 1024);

                    hr = pSelf->pEncoder->Initialize(
                             (UINT32)TMEDIA_PRODUCER(pSelf)->video.fps,
                             (UINT32)TMEDIA_PRODUCER(pSelf)->video.width,
                             (UINT32)TMEDIA_PRODUCER(pSelf)->video.height,
                             (UINT32)pSelf->bitrate_bps);
                    if (SUCCEEDED(hr)) {
                        /*hr =*/ pSelf->pEncoder->SetGOPSize((PLUGIN_MF_GOP_SIZE_IN_SECONDS * TMEDIA_PRODUCER(pSelf)->video.fps));
                    }
                    if (FAILED(hr)) {
                        SafeRelease(&pSelf->pEncoder);
                        hr = S_OK;
                    }
                }
                if (SUCCEEDED(hr) && pSelf->pEncoder) {
                    TMEDIA_PRODUCER(pSelf)->encoder.codec_id = codec->id; // means encoded frames as input
                }
                else {
                    SafeRelease(&pSelf->pEncoder);
                    TSK_DEBUG_WARN("Failed to find H.264 HW encoder...fallback to SW implementation");
                }
            }
            else { /* if(!bMFEncoderIsRegistered) */
                TSK_DEBUG_INFO("Not bundling MF H.264 encoder even if low latency is supported because another implementation is registered: %s", codec->plugin->desc);
            }
        }
#endif

        // Set session attributes
        CHECK_HR(hr = MFCreateAttributes(&pSessionAttributes, 1));
        CHECK_HR(hr = pSessionAttributes->SetUINT32(PLUGIN_MF_LOW_LATENCY, 1));

        // Configure the media type that the Sample Grabber will receive.
        // Setting the major and subtype is usually enough for the topology loader
        // to resolve the topology.

        CHECK_HR(hr = MFCreateMediaType(&pSelf->pGrabberInputType));
        CHECK_HR(hr = pSelf->pGrabberInputType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video));
        CHECK_HR(hr = pSelf->pGrabberInputType->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive));

        CHECK_HR(hr = MFSetAttributeSize(pSelf->pGrabberInputType, MF_MT_FRAME_SIZE, (UINT32)TMEDIA_PRODUCER(pSelf)->video.width, (UINT32)TMEDIA_PRODUCER(pSelf)->video.height));
        CHECK_HR(hr = MFSetAttributeRatio(pSelf->pGrabberInputType, MF_MT_FRAME_RATE, TMEDIA_PRODUCER(pSelf)->video.fps, 1));

        CHECK_HR(hr = MFSetAttributeRatio(pSelf->pGrabberInputType, MF_MT_PIXEL_ASPECT_RATIO, 1, 1));
        CHECK_HR(hr = pSelf->pGrabberInputType->SetUINT32(MF_MT_ALL_SAMPLES_INDEPENDENT, pSelf->pEncoder ? FALSE : TRUE));
        CHECK_HR(hr = pSelf->pGrabberInputType->SetUINT32(MF_MT_FIXED_SIZE_SAMPLES, pSelf->pEncoder ? FALSE : TRUE));
        if (pSelf->pEncoder) {
            switch (codec->id) {
            case tmedia_codec_id_h264_bp:
            case tmedia_codec_id_h264_mp: {
                CHECK_HR(hr = pSelf->pGrabberInputType->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_H264));
                CHECK_HR(hr = pSelf->pGrabberInputType->SetUINT32(MF_MT_MPEG2_PROFILE, (codec->id == tmedia_codec_id_h264_bp) ? eAVEncH264VProfile_Base : eAVEncH264VProfile_Main));
                CHECK_HR(hr = pSelf->pGrabberInputType->SetUINT32(MF_MT_AVG_BITRATE, pSelf->bitrate_bps));
                break;
            }
            default: {
                TSK_DEBUG_ERROR("HW encoder with id = %d not expected", codec->id);
                assert(false);
            }
            }
            TMEDIA_PRODUCER(pSelf)->video.chroma = tmedia_chroma_nv12;
            TSK_DEBUG_INFO("MF video producer chroma = NV12 (because of HW encoder)");
        }
        else {
            // Video Processors will be inserted in the topology if the source cannot produce I420 frames
            // IMPORTANT: Must not be NV12 because not supported by Video Resizer DSP (http://msdn.microsoft.com/en-us/library/windows/desktop/ff819491(v=vs.85).aspx)
            CHECK_HR(hr = pSelf->pGrabberInputType->SetGUID(MF_MT_SUBTYPE, pcPreferredSubTypeGuidPair ? pcPreferredSubTypeGuidPair->fourcc : MFVideoFormat_I420));
            TMEDIA_PRODUCER(pSelf)->video.chroma = pcPreferredSubTypeGuidPair ? pcPreferredSubTypeGuidPair->chroma : tmedia_chroma_yuv420p;
            TSK_DEBUG_INFO("MF video producer chroma = %d", TMEDIA_PRODUCER(pSelf)->video.chroma);
        }

        if (pSelf->pEncoder) {
            // Unlock the encoder
            //BOOL bIsAsyncMFT = FALSE;
            //CHECK_HR(hr = MFUtils::IsAsyncMFT(pSelf->pEncoder->GetMFT(), &bIsAsyncMFT));
            //if(bIsAsyncMFT)
            //{
            //	CHECK_HR(hr = MFUtils::UnlockAsyncMFT(pSelf->pEncoderpSelf->pEncoder->GetMFT()));
            //}
            // Apply Encoder output type (must be called before SetInputType)
            //CHECK_HR(hr = pSelf->pEncoder->GetMFT()->SetOutputType(0, pSelf->pGrabberInputType, 0/*MFT_SET_TYPE_TEST_ONLY*/));

            // HW encoders support only NV12
            //CHECK_HR(hr = MFUtils::ConvertVideoTypeToUncompressedType(pSelf->pGrabberInputType, MFVideoFormat_NV12, &pEncoderInputType));
            //CHECK_HR(hr = pSelf->pEncoder->GetMFT()->SetInputType(0, pEncoderInputType, 0/*MFT_SET_TYPE_TEST_ONLY*/));
        }
        // Create the sample grabber sink.
        CHECK_HR(hr = SampleGrabberCB::CreateInstance(TMEDIA_PRODUCER(pSelf), &pSelf->pCallback));
        CHECK_HR(hr = MFCreateSampleGrabberSinkActivate(pSelf->pGrabberInputType, pSelf->pCallback, &pSelf->pSinkGrabber));

        // To run as fast as possible, set this attribute (requires Windows 7):
        CHECK_HR(hr = pSelf->pSinkGrabber->SetUINT32(MF_SAMPLEGRABBERSINK_IGNORE_CLOCK, TRUE));

        // Create the Media Session.
        CHECK_HR(hr = MFCreateMediaSession(pSessionAttributes, &pSelf->pSession));

        // Create the EVR activation object for the preview.
        CHECK_HR(hr = MFCreateVideoRendererActivate(pSelf->hWndPreview, &pSelf->pSinkActivatePreview));

        // Create the topology.
        CHECK_HR(hr = MFUtils::CreateTopology(
                          pSelf->pSource,
                          pSelf->pEncoder ? pSelf->pEncoder->GetMFT() : NULL,
                          pSelf->pSinkGrabber,
                          pSelf->pSinkActivatePreview,
                          pSelf->pGrabberInputType,
                          &pTopology));
        // Resolve topology (adds video processors if needed).
        CHECK_HR(hr = MFUtils::ResolveTopology(pTopology, &pSelf->pTopology));

        // Find EVR for the preview.
        CHECK_HR(hr = MFUtils::FindNodeObject(pSelf->pTopology, MFUtils::g_ullTopoIdSinkPreview, (void**)&pEvr));

        // Find negotiated media and update producer
        UINT32 nNegWidth = (UINT32)TMEDIA_PRODUCER(pSelf)->video.width, nNegHeight = (UINT32)TMEDIA_PRODUCER(pSelf)->video.height, nNegNumeratorFps = (UINT32)TMEDIA_PRODUCER(pSelf)->video.fps, nNegDenominatorFps = 1;
        CHECK_HR(hr = pSelf->pTopology->GetNodeByID(MFUtils::g_ullTopoIdSinkMain, &pNodeGrabber));
        CHECK_HR(hr = pNodeGrabber->GetInputPrefType(0, &pGrabberNegotiatedInputMedia));
        hr = MFGetAttributeSize(pGrabberNegotiatedInputMedia, MF_MT_FRAME_SIZE, &nNegWidth, &nNegHeight);
        if (SUCCEEDED(hr)) {
            TSK_DEBUG_INFO("MF video producer topology vs sdp parameters: width(%u/%u), height(%u/%u)",
                           TMEDIA_PRODUCER(pSelf)->video.width, nNegWidth,
                           TMEDIA_PRODUCER(pSelf)->video.height, nNegHeight
                          );
            TMEDIA_PRODUCER(pSelf)->video.width = nNegWidth;
            TMEDIA_PRODUCER(pSelf)->video.height = nNegHeight;
        }
        hr = MFGetAttributeRatio(pGrabberNegotiatedInputMedia, MF_MT_FRAME_RATE, &nNegNumeratorFps, &nNegDenominatorFps);
        if (SUCCEEDED(hr)) {
            TSK_DEBUG_INFO("MF video producer topology vs sdp parameters: fps(%u/%u)",
                           TMEDIA_PRODUCER(pSelf)->video.fps, (nNegNumeratorFps / nNegDenominatorFps)
                          );
            TMEDIA_PRODUCER(pSelf)->video.fps = (nNegNumeratorFps / nNegDenominatorFps);
        }

        // Create EVR watcher for the preview.
        pSelf->pWatcherPreview = new DisplayWatcher(pSelf->hWndPreview, pEvr, hr);
        CHECK_HR(hr);
    }

bail:
    SafeRelease(&pSessionAttributes);
    SafeRelease(&pTopology);
    SafeRelease(&pEvr);
    SafeRelease(&pEncoderInputType);
    SafeRelease(&pNodeGrabber);
    SafeRelease(&pGrabberNegotiatedInputMedia);

    pSelf->bPrepared = SUCCEEDED(hr);
    return pSelf->bPrepared ? 0 : -1;
}

static int plugin_win_mf_producer_video_start(tmedia_producer_t* self)
{
    plugin_win_mf_producer_video_t* pSelf = (plugin_win_mf_producer_video_t*)self;

    if (!pSelf) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if (pSelf->bStarted) {
        TSK_DEBUG_INFO("MF video producer already started");
        return 0;
    }
    if (!pSelf->bPrepared) {
        TSK_DEBUG_ERROR("MF video producer not prepared");
        return -1;
    }

    HRESULT hr = S_OK;

    // Run preview watcher
    if (pSelf->pWatcherPreview) {
        CHECK_HR(hr = pSelf->pWatcherPreview->Start());
    }

    // Run the media session.
    CHECK_HR(hr = MFUtils::RunSession(pSelf->pSession, pSelf->pTopology));

    // Start asynchronous watcher thread
    pSelf->bStarted = true;
    int ret = tsk_thread_create(&pSelf->ppTread[0], RunSessionThread, pSelf);
    if (ret != 0) {
        TSK_DEBUG_ERROR("Failed to create thread");
        hr = E_FAIL;
        pSelf->bStarted = false;
        if (pSelf->ppTread[0]) {
            tsk_thread_join(&pSelf->ppTread[0]);
        }
        MFUtils::ShutdownSession(pSelf->pSession, pSelf->pSource);
        goto bail;
    }

bail:
    return SUCCEEDED(hr) ? 0 : -1;
}

static int plugin_win_mf_producer_video_pause(tmedia_producer_t* self)
{
    plugin_win_mf_producer_video_t* pSelf = (plugin_win_mf_producer_video_t*)self;

    if (!pSelf) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if (!pSelf->bStarted) {
        TSK_DEBUG_INFO("MF video producer not started");
        return 0;
    }

    HRESULT hr = MFUtils::PauseSession(pSelf->pSession);

    return SUCCEEDED(hr) ? 0 : -1;
}

static int plugin_win_mf_producer_video_stop(tmedia_producer_t* self)
{
    plugin_win_mf_producer_video_t* pSelf = (plugin_win_mf_producer_video_t*)self;

    if (!pSelf) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    HRESULT hr = S_OK;

    if (pSelf->pWatcherPreview) {
        hr = pSelf->pWatcherPreview->Stop();
    }

    // for the thread
    pSelf->bStarted = false;
    hr = MFUtils::ShutdownSession(pSelf->pSession, NULL); // stop session to wakeup the asynchronous thread
    if (pSelf->ppTread[0]) {
        tsk_thread_join(&pSelf->ppTread[0]);
    }
    hr = MFUtils::ShutdownSession(NULL, pSelf->pSource); // stop source to release the camera

    // next start() will be called after prepare()
    return _plugin_win_mf_producer_video_unprepare(pSelf);
}

static int _plugin_win_mf_producer_video_unprepare(plugin_win_mf_producer_video_t* pSelf)
{
    if (!pSelf) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if (pSelf->bStarted) {
        // plugin_win_mf_producer_video_stop(TMEDIA_PRODUCER(pSelf));
        TSK_DEBUG_ERROR("Producer must be stopped before calling unprepare");
    }
    if (pSelf->pDeviceList) {
        delete pSelf->pDeviceList, pSelf->pDeviceList = NULL;
    }
    if (pSelf->pWatcherPreview) {
        pSelf->pWatcherPreview->Stop();
    }
    if (pSelf->pSource) {
        pSelf->pSource->Shutdown();
    }
    if (pSelf->pSession) {
        pSelf->pSession->Shutdown();
    }

    SafeRelease(&pSelf->pEncoder);
    SafeRelease(&pSelf->pSession);
    SafeRelease(&pSelf->pSource);
    SafeRelease(&pSelf->pSinkActivatePreview);
    SafeRelease(&pSelf->pCallback);
    SafeRelease(&pSelf->pSinkGrabber);
    SafeRelease(&pSelf->pTopology);
    SafeRelease(&pSelf->pGrabberInputType);

    if (pSelf->pWatcherPreview) {
        delete pSelf->pWatcherPreview;
        pSelf->pWatcherPreview = NULL;
    }

    pSelf->bPrepared = false;

    return 0;
}

//
//      Windows Media Foundation video producer object definition
//
/* constructor */
static tsk_object_t* plugin_win_mf_producer_video_ctor(tsk_object_t * self, va_list * app)
{
    MFUtils::Startup();

    plugin_win_mf_producer_video_t *pSelf = (plugin_win_mf_producer_video_t *)self;
    if (pSelf) {
        /* init base */
        tmedia_producer_init(TMEDIA_PRODUCER(pSelf));

        /* init self with default values*/
        TMEDIA_PRODUCER(pSelf)->encoder.codec_id = tmedia_codec_id_none; // means RAW frames as input
        TMEDIA_PRODUCER(pSelf)->video.chroma = tmedia_chroma_nv12;
        TMEDIA_PRODUCER(pSelf)->video.fps = 15;
        TMEDIA_PRODUCER(pSelf)->video.width = 352;
        TMEDIA_PRODUCER(pSelf)->video.height = 288;

        TSK_DEBUG_INFO("Create WinMF video producer");
    }
    return self;
}
/* destructor */
static tsk_object_t* plugin_win_mf_producer_video_dtor(tsk_object_t * self)
{
    plugin_win_mf_producer_video_t *pSelf = (plugin_win_mf_producer_video_t *)self;
    if (pSelf) {
        /* stop */
        if (pSelf->bStarted) {
            plugin_win_mf_producer_video_stop(TMEDIA_PRODUCER(pSelf));
        }

        /* deinit base */
        tmedia_producer_deinit(TMEDIA_PRODUCER(pSelf));
        /* deinit self */
        _plugin_win_mf_producer_video_unprepare(pSelf);
    }

    return self;
}
/* object definition */
static const tsk_object_def_t plugin_win_mf_producer_video_def_s = {
    sizeof(plugin_win_mf_producer_video_t),
    plugin_win_mf_producer_video_ctor,
    plugin_win_mf_producer_video_dtor,
    tsk_null,
};
/* plugin definition*/
static const tmedia_producer_plugin_def_t plugin_win_mf_producer_video_plugin_def_s = {
    &plugin_win_mf_producer_video_def_s,

    tmedia_video,
    "Microsoft Windows Media Foundation producer (Video)",

    plugin_win_mf_producer_video_set,
    plugin_win_mf_producer_video_prepare,
    plugin_win_mf_producer_video_start,
    plugin_win_mf_producer_video_pause,
    plugin_win_mf_producer_video_stop
};
const tmedia_producer_plugin_def_t *plugin_win_mf_producer_video_plugin_def_t = &plugin_win_mf_producer_video_plugin_def_s;


// Run session async thread
static void* TSK_STDCALL RunSessionThread(void *pArg)
{
    plugin_win_mf_producer_video_t *pSelf = (plugin_win_mf_producer_video_t *)pArg;
    HRESULT hrStatus = S_OK;
    HRESULT hr = S_OK;
    IMFMediaEvent *pEvent = NULL;
    MediaEventType met;

    TSK_DEBUG_INFO("RunSessionThread (MF video producer) - ENTER");

    while (pSelf->bStarted) {
        hr = pSelf->pSession->GetEvent(0, &pEvent);
        if (hr == MF_E_SHUTDOWN) {
            if (pSelf->bStarted) {
                CHECK_HR(hr); // Shutdown called but "bStarted" not equal to false
            }
            break; // Shutdown called and "bStarted" is equal to false => break the loop
        }
        CHECK_HR(hr = pEvent->GetStatus(&hrStatus));
        CHECK_HR(hr = pEvent->GetType(&met));

        if (FAILED(hrStatus) /*&& hrStatus != MF_E_NO_SAMPLE_TIMESTAMP*/) {
            TSK_DEBUG_ERROR("Session error: 0x%x (event id: %d)\n", hrStatus, met);
            hr = hrStatus;
            goto bail;
        }
        if (met == MESessionEnded) {
            break;
        }
        SafeRelease(&pEvent);
    }

bail:
    TSK_DEBUG_INFO("RunSessionThread (MF video producer) - EXIT");

    return NULL;
}