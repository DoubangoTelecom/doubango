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
#include "internals/mf_sample_grabber.h"
#include "internals/mf_devices.h"
#include "internals/mf_display_watcher.h"

#include "tinymedia/tmedia_defaults.h"
#include "tinymedia/tmedia_producer.h"

#include "tsk_string.h"
#include "tsk_thread.h"
#include "tsk_debug.h"

#include <Codecapi.h>
#include <assert.h>
#include <stdlib.h>     /* mbstowcs, wchar_t(C) */

// 0: {{[Source] -> (VideoProcessor) -> SampleGrabber}} , {{[Encoder]}} -> RTP
// 1: {{[Source] -> (VideoProcessor) -> [Encoder] -> SampleGrabber}} -> RTP
// (VideoProcessor) is optional
// "{{" and "}}" defines where the graph starts and ends respectively. For "0", [Decoder] is a stand-alone IMFTransform.
#if !defined(PLUGIN_MF_PV_BUNDLE_CODEC)
#	define PLUGIN_MF_PV_BUNDLE_CODEC 0
#endif

static void* TSK_STDCALL RunSessionThread(void *pArg);
static int _plugin_win_mf_producer_video_unprepare(struct plugin_win_mf_producer_video_s* pSelf);

//
//	plugin_win_mf_producer_video_t
//
typedef struct plugin_win_mf_producer_video_s
{
	TMEDIA_DECLARE_PRODUCER;
	
	bool bStarted, bPrepared;
	tsk_thread_handle_t* ppTread[1];
	HWND hWndPreview;

    DeviceListVideo* pDeviceList;

	IMFTransform *pEncoder;
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

	if(!pSelf || !param){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(param->value_type == tmedia_pvt_int64){
		if(tsk_striequals(param->key, "local-hwnd")){
			HWND hWnd = reinterpret_cast<HWND>((INT64)*((int64_t*)param->value));
			if(hWnd != pSelf->hWndPreview)
			{
				pSelf->hWndPreview = hWnd;
				if(pSelf->pWatcherPreview)
				{
					CHECK_HR(hr = pSelf->pWatcherPreview->SetHwnd(hWnd));
				}
			}
		}
	}
	else if(param->value_type == tmedia_pvt_int32){
		if(tsk_striequals(param->key, "mute")){
			//producer->mute = (TSK_TO_INT32((uint8_t*)param->value) != 0);
			//if(producer->started){
			//	if(producer->mute){
			//		producer->grabber->pause();
			//	}
			//	else{
			//		producer->grabber->start();
			//	}
			//}
		}
		else if(tsk_striequals(param->key, "create-on-current-thead")){
			//producer->create_on_ui_thread = *((int32_t*)param->value) ? tsk_false : tsk_true;
		}
		else if(tsk_striequals(param->key, "plugin-firefox")){
			//producer->plugin_firefox = (*((int32_t*)param->value) != 0);
			//if(producer->grabber){
			//	producer->grabber->setPluginFirefox((producer->plugin_firefox == tsk_true));
			//}
		}
	}

bail:
	return SUCCEEDED(hr) ?  0 : -1;
}

static int plugin_win_mf_producer_video_prepare(tmedia_producer_t* self, const tmedia_codec_t* codec)
{
	plugin_win_mf_producer_video_t* pSelf = (plugin_win_mf_producer_video_t*)self;

	if(!pSelf || !codec && codec->plugin){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	if(pSelf->bPrepared){
		TSK_DEBUG_WARN("MF video producer already prepared");
		return -1;
	}

	TMEDIA_PRODUCER(pSelf)->video.fps = TMEDIA_CODEC_VIDEO(codec)->out.fps;
	TMEDIA_PRODUCER(pSelf)->video.width = TMEDIA_CODEC_VIDEO(codec)->out.width;
	TMEDIA_PRODUCER(pSelf)->video.height = TMEDIA_CODEC_VIDEO(codec)->out.height;

	TSK_DEBUG_INFO("MF video producer: fps=%d, width=%d, height=%d", 
		TMEDIA_PRODUCER(pSelf)->video.fps, 
		TMEDIA_PRODUCER(pSelf)->video.width, 
		TMEDIA_PRODUCER(pSelf)->video.height);

	HRESULT hr = S_OK;
	IMFAttributes* pSessionAttributes = NULL;
	IMFTopology *pTopology = NULL;
	IMFMediaSink* pEvr = NULL;

	// create device list object
	if(!pSelf->pDeviceList && !(pSelf->pDeviceList = new DeviceListVideo())){
		TSK_DEBUG_ERROR("Failed to create device list");
		hr = E_OUTOFMEMORY;
		goto  bail;
	}
	// enumerate devices
	hr = pSelf->pDeviceList->EnumerateDevices();
	if(!SUCCEEDED(hr)){
		goto bail;
	}

	// check if we have at least one MF video source connected to the PC
	if(pSelf->pDeviceList->Count() == 0){
		TSK_DEBUG_WARN("No MF video source could be found...no video will be sent");
		// do not break the negotiation as one-way video connection is a valid use-case
	}
	else{
		// Get best MF video source
		IMFActivate* pActivate = NULL;
		const char* pczSrcFriendlyName = tmedia_producer_get_friendly_name(tmedia_video);
		if(!tsk_strnullORempty(pczSrcFriendlyName)) {
			TSK_DEBUG_INFO("MF pref. video source = %s", pczSrcFriendlyName);
			wchar_t pczwSrcFriendlyName[MAX_PATH] = { 0 };
			mbstowcs(pczwSrcFriendlyName, pczSrcFriendlyName, sizeof(pczwSrcFriendlyName)/sizeof(pczwSrcFriendlyName[0]));
			hr = pSelf->pDeviceList->GetDeviceBest(&pActivate, pczwSrcFriendlyName);
		}
		else {
			hr = pSelf->pDeviceList->GetDeviceBest(&pActivate);
		}
		if(!SUCCEEDED(hr) || !pActivate){
			TSK_DEBUG_ERROR("Failed to get best MF video source");
			if(!pActivate){
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
		if(!SUCCEEDED(hr)){
			TSK_DEBUG_ERROR("ActivateObject(MF video source) failed");
			goto bail;
		}

		// If H.264 is negotiated for this session then, try to find hardware encoder
		// If no HW encoder is found will fallback to SW implementation from x264
#if PLUGIN_MF_PV_BUNDLE_CODEC
		if((codec->id == tmedia_codec_id_h264_bp || codec->id == tmedia_codec_id_h264_mp)) {
			// both Microsoft and Intel encoders support NV12 only as input
			static const bool kIsEncoder = true;
			hr = MFUtils::GetBestCodec(kIsEncoder, MFMediaType_Video, MFVideoFormat_NV12, MFVideoFormat_H264, &pSelf->pEncoder);
			if(SUCCEEDED(hr) && pSelf->pEncoder) {
				TMEDIA_PRODUCER(pSelf)->encoder.codec_id = codec->id;
			}
			else {
				SafeRelease(&pSelf->pEncoder);
				TSK_DEBUG_WARN("Failed to find H.264 HW encoder...fallback to SW implementation");
				TMEDIA_PRODUCER(pSelf)->encoder.codec_id = tmedia_codec_id_none; // means RAW
			}
		}
#endif

		// Set session attributes
		CHECK_HR(hr = MFCreateAttributes(&pSessionAttributes, 1));
		CHECK_HR(hr = pSessionAttributes->SetUINT32(MF_LOW_LATENCY, 1)); //FIXME: CodecAPI should also use http://msdn.microsoft.com/en-us/library/dd317656(v=vs.85).aspx

		// Configure the media type that the Sample Grabber will receive.
		// Setting the major and subtype is usually enough for the topology loader
		// to resolve the topology.

		CHECK_HR(hr = MFCreateMediaType(&pSelf->pGrabberInputType));
		CHECK_HR(hr = pSelf->pGrabberInputType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video));
		CHECK_HR(hr = pSelf->pGrabberInputType->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive));
		CHECK_HR(hr = MFSetAttributeSize(pSelf->pGrabberInputType, MF_MT_FRAME_SIZE, TMEDIA_PRODUCER(pSelf)->video.width, TMEDIA_PRODUCER(pSelf)->video.height));
		if(pSelf->pEncoder){ // FIXME: MF_E_NO_SAMPLE_TIMESTAMP
			CHECK_HR(hr = MFSetAttributeRatio(pSelf->pGrabberInputType, MF_MT_FRAME_RATE, TMEDIA_PRODUCER(pSelf)->video.fps, 1));
		}
		CHECK_HR(hr = MFSetAttributeRatio(pSelf->pGrabberInputType, MF_MT_PIXEL_ASPECT_RATIO, 1, 1));
		CHECK_HR(hr = pSelf->pGrabberInputType->SetUINT32(MF_MT_ALL_SAMPLES_INDEPENDENT, pSelf->pEncoder ? FALSE : TRUE));
		CHECK_HR(hr = pSelf->pGrabberInputType->SetUINT32(MF_MT_FIXED_SIZE_SAMPLES, pSelf->pEncoder ? FALSE : TRUE));
		if(pSelf->pEncoder) {
			int32_t avg_bitrate = tmedia_get_video_bandwidth_kbps_2(TMEDIA_PRODUCER(pSelf)->video.width, TMEDIA_PRODUCER(pSelf)->video.height, TMEDIA_PRODUCER(pSelf)->video.fps);
			TSK_DEBUG_INFO("MF_MT_AVG_BITRATE defined with value = %d kbps", avg_bitrate);
			switch(codec->id){
				case tmedia_codec_id_h264_bp: case tmedia_codec_id_h264_mp: 
					{
						CHECK_HR(hr = pSelf->pGrabberInputType->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_H264));
						CHECK_HR(hr = pSelf->pGrabberInputType->SetUINT32(MF_MT_MPEG2_PROFILE, (codec->id == tmedia_codec_id_h264_bp) ? eAVEncH264VProfile_Base : eAVEncH264VProfile_Main));
						CHECK_HR(hr = pSelf->pGrabberInputType->SetUINT32(MF_MT_AVG_BITRATE, (avg_bitrate * 1024)));
						break;
					}
				default:
					{
						TSK_DEBUG_ERROR("HW encoder with id = %d not expected", codec->id);
						assert(false);
					}
			}
			TMEDIA_PRODUCER(pSelf)->video.chroma = tmedia_chroma_nv12;
			TSK_DEBUG_INFO("MF video producer chroma = NV12 (because of HW encoder)");
		}
		else {
			// Video Processors will be inserted in the topology if the source cannot produce I420 frames
			CHECK_HR(hr = pSelf->pGrabberInputType->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_I420));
			TMEDIA_PRODUCER(pSelf)->video.chroma = tmedia_chroma_yuv420p;
			TSK_DEBUG_INFO("MF video producer chroma = %d", TMEDIA_PRODUCER(pSelf)->video.chroma);
		}

		// Apply Encoder output type (must be called before SetInputType)
		if(pSelf->pEncoder) {
			CHECK_HR(hr = pSelf->pEncoder->SetOutputType(0, pSelf->pGrabberInputType, 0/*MFT_SET_TYPE_TEST_ONLY*/));
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
		CHECK_HR(hr = MFUtils::CreateTopology(pSelf->pSource, pSelf->pEncoder, pSelf->pSinkGrabber, pSelf->pSinkActivatePreview, MFMediaType_Video, &pTopology));
		// Resolve topology (adds video processors if needed).
		CHECK_HR(hr = MFUtils::ResolveTopology(pTopology, &pSelf->pTopology));

		// Find EVR for the preview.
		CHECK_HR(hr = MFUtils::FindNodeObject(pSelf->pTopology, MFUtils::g_ullTopoIdSinkPreview, (void**)&pEvr));

		// Create EVR watcher for the preview.
		pSelf->pWatcherPreview = new DisplayWatcher(pSelf->hWndPreview, pEvr, hr);
		CHECK_HR(hr);
	}

bail:
	SafeRelease(&pSessionAttributes);
	SafeRelease(&pTopology);
	SafeRelease(&pEvr);
	
	pSelf->bPrepared = SUCCEEDED(hr);
	return pSelf->bPrepared ? 0 : -1;
}

static int plugin_win_mf_producer_video_start(tmedia_producer_t* self)
{
	plugin_win_mf_producer_video_t* pSelf = (plugin_win_mf_producer_video_t*)self;

	if(!pSelf){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(pSelf->bStarted){
		TSK_DEBUG_INFO("MF video producer already started");
		return 0;
	}
	if(!pSelf->bPrepared){
		TSK_DEBUG_ERROR("MF video producer not prepared");
		return -1;
	}

	HRESULT hr = S_OK;

	// Run preview watcher
	if(pSelf->pWatcherPreview) {
		CHECK_HR(hr = pSelf->pWatcherPreview->Start());
	}

	// Run the media session.
	CHECK_HR(hr = MFUtils::RunSession(pSelf->pSession, pSelf->pTopology));

	// Start asynchronous watcher thread
	pSelf->bStarted = true;
	int ret = tsk_thread_create(&pSelf->ppTread[0], RunSessionThread, pSelf);
	if(ret != 0) {
		TSK_DEBUG_ERROR("Failed to create thread");
		hr = E_FAIL;
		pSelf->bStarted = false;
		if(pSelf->ppTread[0]){
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

	if(!pSelf){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	if(!pSelf->bStarted)
	{
		TSK_DEBUG_INFO("MF video producer not started");
		return 0;
	}

	HRESULT hr = MFUtils::PauseSession(pSelf->pSession);

	return SUCCEEDED(hr) ? 0 : -1;
}

static int plugin_win_mf_producer_video_stop(tmedia_producer_t* self)
{
	plugin_win_mf_producer_video_t* pSelf = (plugin_win_mf_producer_video_t*)self;

	if(!pSelf){
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    HRESULT hr = S_OK;

	if(pSelf->pWatcherPreview){
		hr = pSelf->pWatcherPreview->Stop();
	}

    // for the thread
    pSelf->bStarted = false;
    hr = MFUtils::ShutdownSession(pSelf->pSession, NULL); // stop session to wakeup the asynchronous thread
    if(pSelf->ppTread[0]){
        tsk_thread_join(&pSelf->ppTread[0]);
    }
    hr = MFUtils::ShutdownSession(NULL, pSelf->pSource); // stop source to release the camera

	// next start() will be called after prepare()
	return _plugin_win_mf_producer_video_unprepare(pSelf);
}

static int _plugin_win_mf_producer_video_unprepare(plugin_win_mf_producer_video_t* pSelf)
{
	if(!pSelf){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(pSelf->bStarted) {
		// plugin_win_mf_producer_video_stop(TMEDIA_PRODUCER(pSelf));
		TSK_DEBUG_ERROR("Producer must be stopped before calling unprepare");
	}
	if(pSelf->pDeviceList){
		delete pSelf->pDeviceList, pSelf->pDeviceList = NULL;
    }
	if(pSelf->pWatcherPreview){
		pSelf->pWatcherPreview->Stop();
	}
    if(pSelf->pSource){
		pSelf->pSource->Shutdown();
		pSelf->pSource = NULL;
    }
    if(pSelf->pSession){
        pSelf->pSession->Shutdown();
		pSelf->pSession = NULL;
    }

	SafeRelease(&pSelf->pEncoder);
    SafeRelease(&pSelf->pSession);
    SafeRelease(&pSelf->pSource);
	SafeRelease(&pSelf->pSinkActivatePreview);
    SafeRelease(&pSelf->pCallback);
    SafeRelease(&pSelf->pSinkGrabber);
    SafeRelease(&pSelf->pTopology);
	SafeRelease(&pSelf->pGrabberInputType);

	if(pSelf->pWatcherPreview){
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
	if(pSelf){
		/* init base */
		tmedia_producer_init(TMEDIA_PRODUCER(pSelf));

		/* init self with default values*/
		TMEDIA_PRODUCER(pSelf)->video.chroma = tmedia_chroma_bgr24;
		TMEDIA_PRODUCER(pSelf)->video.fps = 15;
		TMEDIA_PRODUCER(pSelf)->video.width = 352;
		TMEDIA_PRODUCER(pSelf)->video.height = 288;
	}
	return self;
}
/* destructor */
static tsk_object_t* plugin_win_mf_producer_video_dtor(tsk_object_t * self)
{ 
	plugin_win_mf_producer_video_t *pSelf = (plugin_win_mf_producer_video_t *)self;
	if(pSelf){
		/* stop */
		if(pSelf->bStarted){
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
static const tsk_object_def_t plugin_win_mf_producer_video_def_s = 
{
	sizeof(plugin_win_mf_producer_video_t),
	plugin_win_mf_producer_video_ctor, 
	plugin_win_mf_producer_video_dtor,
	tsk_null, 
};
/* plugin definition*/
static const tmedia_producer_plugin_def_t plugin_win_mf_producer_video_plugin_def_s = 
{
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

	while(pSelf->bStarted){
		CHECK_HR(hr = pSelf->pSession->GetEvent(0, &pEvent));
		CHECK_HR(hr = pEvent->GetStatus(&hrStatus));
		CHECK_HR(hr = pEvent->GetType(&met));
		
		if (FAILED(hrStatus) /*&& hrStatus != MF_E_NO_SAMPLE_TIMESTAMP*/)
		{
			TSK_DEBUG_ERROR("Session error: 0x%x (event id: %d)\n", hrStatus, met);
			hr = hrStatus;
			goto bail;
		}
		if (met == MESessionEnded)
		{
			break;
		}
		SafeRelease(&pEvent);
	}

bail:
	TSK_DEBUG_INFO("RunSessionThread (MF video producer) - EXIT");

	return NULL;
}