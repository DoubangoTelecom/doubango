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
#include "internals/mf_custom_src.h"
#include "internals/mf_display_watcher.h"
#include "internals/mf_codec.h"

#include "tinymedia/tmedia_consumer.h"

#include "tsk_string.h"
#include "tsk_thread.h"
#include "tsk_debug.h"

#include <KS.h>
#include <Codecapi.h>
#include <assert.h>
#include <initguid.h>

// 0: {{[Source] -> (VideoProcessor) -> SampleGrabber}} , {{[Decoder]}} -> RTP
// 1: {{[Source] -> (VideoProcessor) -> [Decoder] -> SampleGrabber}} -> RTP
// (VideoProcessor) is optional
// "{{" and "}}" defines where the graph starts and ends respectively. For "0", [Decoder] is a stand-alone IMFTransform.
#if !defined(PLUGIN_MF_CV_BUNDLE_CODEC)
#	define PLUGIN_MF_CV_BUNDLE_CODEC 0
#endif

// Uncompressed video frame will come from Doubango core and it's up to the converter to match the requested chroma.
// Supported values: NV12, I420, RGB32 and RGB24. (RGB formats are not recommended because of performance issues)
// To avoid chroma conversion (performance issues) we use NV12 when the codec is bundled as MediaFoundation codecs most likely only support this format.
// NV12 is the native format for media foundation codecs (e.g. Intel Quick Sync) and the GPU.
// I420 is the native format for FFmpeg, libvpx and libtheora.
const GUID kDefaultUncompressedType 
#if PLUGIN_MF_CV_BUNDLE_CODEC
= MFVideoFormat_NV12;
#else
= MFVideoFormat_I420;
#endif

DEFINE_GUID(PLUGIN_MF_LOW_LATENCY,
0x9c27891a, 0xed7a, 0x40e1, 0x88, 0xe8, 0xb2, 0x27, 0x27, 0xa0, 0x24, 0xee);

static void* TSK_STDCALL RunSessionThread(void *pArg);
static int _plugin_win_mf_consumer_video_unprepare(struct plugin_win_mf_consumer_video_s* pSelf);

typedef struct plugin_win_mf_consumer_video_s
{
	TMEDIA_DECLARE_CONSUMER;
	
	bool bStarted, bPrepared;
	HWND hWindow;
	tsk_thread_handle_t* ppTread[1];

	UINT32 nNegWidth;
	UINT32 nNegHeight;
	UINT32 nNegFps;

	MFCodecVideo *pDecoder;
    IMFMediaSession *pSession;
    CMFSource *pSource;
    IMFActivate *pSinkActivate;
	DisplayWatcher* pDisplayWatcher;
    IMFTopology *pTopologyFull;
	IMFTopology *pTopologyPartial;
	IMFMediaType *pOutType;
}
plugin_win_mf_consumer_video_t;



/* ============ Media Consumer Interface ================= */
static int plugin_win_mf_consumer_video_set(tmedia_consumer_t *self, const tmedia_param_t* param)
{
	int ret = 0;
	HRESULT hr = S_OK;
	plugin_win_mf_consumer_video_t* pSelf = (plugin_win_mf_consumer_video_t*)self;

	if(!self || !param){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(param->value_type == tmedia_pvt_int64){
		if(tsk_striequals(param->key, "remote-hwnd")){
			HWND hWnd = reinterpret_cast<HWND>((INT64)*((int64_t*)param->value));
			if(hWnd != pSelf->hWindow)
			{
				pSelf->hWindow = hWnd;
				if(pSelf->pDisplayWatcher)
				{
					CHECK_HR(hr = pSelf->pDisplayWatcher->SetHwnd(hWnd));
				}
			}
		}
	}
	else if(param->value_type == tmedia_pvt_int32){
		if(tsk_striequals(param->key, "fullscreen")){
			if(pSelf->pDisplayWatcher)
			{
				CHECK_HR(hr = pSelf->pDisplayWatcher->SetFullscreen(!!*((int32_t*)param->value)));
			}
		}
		else if(tsk_striequals(param->key, "create-on-current-thead")){
			// DSCONSUMER(self)->create_on_ui_thread = *((int32_t*)param->value) ? tsk_false : tsk_true;
		}
		else if(tsk_striequals(param->key, "plugin-firefox")){
			/*DSCONSUMER(self)->plugin_firefox = (*((int32_t*)param->value) != 0);
			if(DSCONSUMER(self)->display){
				DSCONSUMER(self)->display->setPluginFirefox((DSCONSUMER(self)->plugin_firefox == tsk_true));
			}*/
		}
	}

bail:
	return SUCCEEDED(hr) ?  0 : -1;
}


static int plugin_win_mf_consumer_video_prepare(tmedia_consumer_t* self, const tmedia_codec_t* codec)
{
	plugin_win_mf_consumer_video_t* pSelf = (plugin_win_mf_consumer_video_t*)self;

	if(!pSelf || !codec && codec->plugin){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	if(pSelf->bPrepared){
		TSK_DEBUG_WARN("MF video consumer already prepared");
		return -1;
	}

	// FIXME: DirectShow requires flipping but not MF
	// The Core library always tries to flip when OSType==Win32. Must be changed
	TMEDIA_CODEC_VIDEO(codec)->in.flip = tsk_false;

	HRESULT hr = S_OK;
	
	TMEDIA_CONSUMER(pSelf)->video.fps = TMEDIA_CODEC_VIDEO(codec)->in.fps;
	TMEDIA_CONSUMER(pSelf)->video.in.width = TMEDIA_CODEC_VIDEO(codec)->in.width;
	TMEDIA_CONSUMER(pSelf)->video.in.height = TMEDIA_CODEC_VIDEO(codec)->in.height;

	if(!TMEDIA_CONSUMER(pSelf)->video.display.width){
		TMEDIA_CONSUMER(pSelf)->video.display.width = TMEDIA_CONSUMER(pSelf)->video.in.width;
	}
	if(!TMEDIA_CONSUMER(pSelf)->video.display.height){
		TMEDIA_CONSUMER(pSelf)->video.display.height = TMEDIA_CONSUMER(pSelf)->video.in.height;
	}
	
	pSelf->nNegFps = TMEDIA_CONSUMER(pSelf)->video.fps;
	pSelf->nNegWidth = TMEDIA_CONSUMER(pSelf)->video.display.width;
	pSelf->nNegHeight = TMEDIA_CONSUMER(pSelf)->video.display.height;

	TSK_DEBUG_INFO("MF video consumer: fps=%d, width=%d, height=%d", 
		pSelf->nNegFps, 
		pSelf->nNegWidth, 
		pSelf->nNegHeight);

	if(kDefaultUncompressedType == MFVideoFormat_NV12) {
		TMEDIA_CONSUMER(pSelf)->video.display.chroma = tmedia_chroma_nv12;
	}
	else if(kDefaultUncompressedType == MFVideoFormat_I420) {
		TMEDIA_CONSUMER(pSelf)->video.display.chroma = tmedia_chroma_yuv420p;
	}
	else if(kDefaultUncompressedType == MFVideoFormat_RGB32) {
		TMEDIA_CONSUMER(pSelf)->video.display.chroma = tmedia_chroma_rgb32;
	}
	else if(kDefaultUncompressedType == MFVideoFormat_RGB24) {
		TMEDIA_CONSUMER(pSelf)->video.display.chroma = tmedia_chroma_rgb24;
	}
	else {
		CHECK_HR(hr = E_NOTIMPL);
	}
	TMEDIA_CONSUMER(pSelf)->decoder.codec_id = tmedia_codec_id_none; // means accept RAW fames
	
	IMFMediaSink* pMediaSink = NULL;
	IMFAttributes* pSessionAttributes = NULL;

	// Set session attributes
	CHECK_HR(hr = MFCreateAttributes(&pSessionAttributes, 1));
	CHECK_HR(hr = pSessionAttributes->SetUINT32(PLUGIN_MF_LOW_LATENCY, 1));

	CHECK_HR(hr = MFCreateMediaType(&pSelf->pOutType));
	CHECK_HR(hr = pSelf->pOutType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video));

#if PLUGIN_MF_CV_BUNDLE_CODEC
	if((codec->id == tmedia_codec_id_h264_bp || codec->id == tmedia_codec_id_h264_mp) && MFUtils::IsLowLatencyH264Supported()) {
		// both Microsoft and Intel encoders support NV12 only as input
		// static const BOOL kIsEncoder = FALSE;
		// hr = MFUtils::GetBestCodec(kIsEncoder, MFMediaType_Video, MFVideoFormat_H264, MFVideoFormat_NV12, &pSelf->pDecoder);
		pSelf->pDecoder = (codec->id == tmedia_codec_id_h264_bp) ? MFCodecVideoH264::CreateCodecH264Base(MFCodecType_Decoder) : MFCodecVideoH264::CreateCodecH264Main(MFCodecType_Decoder);
		if(pSelf->pDecoder)
		{
			hr = pSelf->pDecoder->Initialize(
				pSelf->nNegFps,
				pSelf->nNegWidth,
				pSelf->nNegHeight);
			
			if(FAILED(hr))
			{
				SafeRelease(&pSelf->pDecoder);
				hr = S_OK;
			}
		}
		if(SUCCEEDED(hr) && pSelf->pDecoder) {
			TMEDIA_CONSUMER(pSelf)->decoder.codec_id = codec->id; // means accept ENCODED fames
			CHECK_HR(hr = pSelf->pOutType->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_H264));
		}
		else {
			SafeRelease(&pSelf->pDecoder);
			TSK_DEBUG_WARN("Failed to find H.264 HW encoder...fallback to SW implementation");
		}
	}
#endif

	if(!pSelf->pDecoder){
		CHECK_HR(hr = pSelf->pOutType->SetGUID(MF_MT_SUBTYPE, kDefaultUncompressedType));
		TMEDIA_CONSUMER(pSelf)->video.display.chroma = kDefaultUncompressedType == MFVideoFormat_NV12 ? tmedia_chroma_nv12 : tmedia_chroma_yuv420p;
	}
    CHECK_HR(hr = pSelf->pOutType->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive));
	CHECK_HR(hr = pSelf->pOutType->SetUINT32(MF_MT_ALL_SAMPLES_INDEPENDENT, TRUE));
    CHECK_HR(hr = MFSetAttributeSize(pSelf->pOutType, MF_MT_FRAME_SIZE, pSelf->nNegWidth, pSelf->nNegHeight));
    CHECK_HR(hr = MFSetAttributeRatio(pSelf->pOutType, MF_MT_FRAME_RATE, pSelf->nNegFps, 1));     
    CHECK_HR(hr = MFSetAttributeRatio(pSelf->pOutType, MF_MT_PIXEL_ASPECT_RATIO, 1, 1));

	CHECK_HR(hr = CMFSource::CreateInstanceEx(IID_IMFMediaSource, (void**)&pSelf->pSource, pSelf->pOutType));

	// Apply Encoder output type (must be called before SetInputType)
	//if(pSelf->pDecoder) {
	//	CHECK_HR(hr = pSelf->pDecoder->SetOutputType(0, pSelf->pOutType, 0/*MFT_SET_TYPE_TEST_ONLY*/));
	//}

	// Create the Media Session.
	CHECK_HR(hr = MFCreateMediaSession(pSessionAttributes, &pSelf->pSession));

	// Create the EVR activation object.
	CHECK_HR(hr = MFCreateVideoRendererActivate(pSelf->hWindow, &pSelf->pSinkActivate));

	// Create the topology.
	CHECK_HR(hr = MFUtils::CreateTopology(
		pSelf->pSource, 
		pSelf->pDecoder ? pSelf->pDecoder->GetMFT() : NULL, 
		pSelf->pSinkActivate, 
		NULL/*Preview*/, 
		MFMediaType_Video, 
		&pSelf->pTopologyPartial));
	// Resolve topology (adds video processors if needed).
	CHECK_HR(hr = MFUtils::ResolveTopology(pSelf->pTopologyPartial, &pSelf->pTopologyFull));

	// Find EVR
	CHECK_HR(hr = MFUtils::FindNodeObject(pSelf->pTopologyFull, MFUtils::g_ullTopoIdSinkMain, (void**)&pMediaSink));

	// Create EVR watcher
	pSelf->pDisplayWatcher = new DisplayWatcher(pSelf->hWindow, pMediaSink, hr);
	CHECK_HR(hr);

bail:
	SafeRelease(&pMediaSink);
	SafeRelease(&pSessionAttributes);
	
	pSelf->bPrepared = SUCCEEDED(hr);
	return pSelf->bPrepared ? 0 : -1;
}

static int plugin_win_mf_consumer_video_start(tmedia_consumer_t* self)
{
	plugin_win_mf_consumer_video_t* pSelf = (plugin_win_mf_consumer_video_t*)self;

	if(!pSelf){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(pSelf->bStarted){
		TSK_DEBUG_INFO("MF video consumer already started");
		return 0;
	}
	if(!pSelf->bPrepared){
		TSK_DEBUG_ERROR("MF video consumer not prepared");
		return -1;
	}

	HRESULT hr = S_OK;

	// Run EVR watcher
	if(pSelf->pDisplayWatcher) {
		CHECK_HR(hr = pSelf->pDisplayWatcher->Start());
	}

	// Run the media session.
	CHECK_HR(hr = MFUtils::RunSession(pSelf->pSession, pSelf->pTopologyFull));

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
		CHECK_HR(hr = E_FAIL);
	}

bail:
	return SUCCEEDED(hr) ? 0 : -1;
}

static int plugin_win_mf_consumer_video_consume(tmedia_consumer_t* self, const void* buffer, tsk_size_t size, const tsk_object_t* proto_hdr)
{
	plugin_win_mf_consumer_video_t* pSelf = (plugin_win_mf_consumer_video_t*)self;

	HRESULT hr = S_OK;

	if(!pSelf || !buffer || !size) {
		TSK_DEBUG_ERROR("Invalid parameter");
		CHECK_HR(hr = E_INVALIDARG);
	}

	if(!pSelf->bStarted) {
		TSK_DEBUG_INFO("MF video consumer not started");
		CHECK_HR(hr = E_FAIL);
	}
	if(!pSelf->pSource) {
		TSK_DEBUG_ERROR("No video custom source");
		CHECK_HR(hr = E_FAIL);
	}

	if(pSelf->nNegWidth != TMEDIA_CONSUMER(pSelf)->video.in.width || pSelf->nNegHeight != TMEDIA_CONSUMER(pSelf)->video.in.height){
		TSK_DEBUG_INFO("Negotiated and input video sizes are different:%d#%d or %d#%d",
			pSelf->nNegWidth, TMEDIA_CONSUMER(pSelf)->video.in.width,
			pSelf->nNegHeight, TMEDIA_CONSUMER(pSelf)->video.in.height);
		// Update media type
		CHECK_HR(hr = MFSetAttributeSize(pSelf->pOutType, MF_MT_FRAME_SIZE, TMEDIA_CONSUMER(pSelf)->video.in.width, TMEDIA_CONSUMER(pSelf)->video.in.height));
		CHECK_HR(hr = MFSetAttributeRatio(pSelf->pOutType, MF_MT_FRAME_RATE, TMEDIA_CONSUMER(pSelf)->video.fps, 1));

		CHECK_HR(hr = pSelf->pSession->ClearTopologies());

		//
		// FIXME: Using same EVR when the size is just swapped (e.g. [640, 480] -> [480, 640]) doesn't work while other changes does (e.g. [352, 288] -> [640, 480])
		// /!\This look like a bug in Media Foundation
		//
		if(pSelf->nNegWidth == TMEDIA_CONSUMER(pSelf)->video.in.height && pSelf->nNegHeight == TMEDIA_CONSUMER(pSelf)->video.in.width)  // swapped?
		{
			TSK_DEBUG_INFO("/!\\ Size swapped");

			IMFActivate* pSinkActivate = NULL;
			IMFTopology* pTopologyPartial = NULL;
			hr = MFCreateVideoRendererActivate(pSelf->hWindow, &pSinkActivate);
			if(FAILED(hr)) goto end_of_swapping;
			hr = MFUtils::CreateTopology(
				pSelf->pSource, 
				pSelf->pDecoder ? pSelf->pDecoder->GetMFT() : NULL, 
				pSinkActivate, 
				NULL/*Preview*/, 
				MFMediaType_Video, 
				&pTopologyPartial);
			if(FAILED(hr)) goto end_of_swapping;

			if(SUCCEEDED(hr)) {
				SafeRelease(&pSelf->pSinkActivate);
				SafeRelease(&pSelf->pTopologyPartial);
				pSelf->pSinkActivate = pSinkActivate; pSinkActivate = NULL;
				pSelf->pTopologyPartial = pTopologyPartial; pTopologyPartial = NULL;
				
			}
			
end_of_swapping:
			SafeRelease(&pSinkActivate);
			SafeRelease(&pTopologyPartial);
			CHECK_HR(hr);
		}

		// Set media type again (not required but who know)
		CHECK_HR(hr = MFUtils::SetMediaType(pSelf->pSource, pSelf->pOutType));

		// Rebuild topology using the partial one
		IMFTopology* pTopologyFull = NULL;
		hr = MFUtils::ResolveTopology(pSelf->pTopologyPartial, &pTopologyFull);
		if(SUCCEEDED(hr)){
			SafeRelease(&pSelf->pTopologyFull);
			pSelf->pTopologyFull = pTopologyFull; pTopologyFull = NULL;
		}
		SafeRelease(&pTopologyFull);
		CHECK_HR(hr);

		// Find Main Sink
		IMFMediaSink* pMediaSink = NULL;
		hr = MFUtils::FindNodeObject(pSelf->pTopologyFull, MFUtils::g_ullTopoIdSinkMain, (void**)&pMediaSink);
		if(SUCCEEDED(hr)) {
			if(pSelf->pDisplayWatcher){
				delete pSelf->pDisplayWatcher, pSelf->pDisplayWatcher = NULL;
			}
			pSelf->pDisplayWatcher = new DisplayWatcher(pSelf->hWindow, pMediaSink, hr);
			if(SUCCEEDED(hr) && pSelf->bStarted) {
				hr = pSelf->pDisplayWatcher->Start();
			}
		}
		SafeRelease(&pMediaSink);		
		CHECK_HR(hr);

		// Update the topology associated to the media session
		CHECK_HR(hr = pSelf->pSession->SetTopology(MFSESSION_SETTOPOLOGY_IMMEDIATE, pSelf->pTopologyFull));

		// Update negotiated width and height
		pSelf->nNegWidth = TMEDIA_CONSUMER(pSelf)->video.in.width;
		pSelf->nNegHeight = TMEDIA_CONSUMER(pSelf)->video.in.height;		
	}
	
	// Deliver buffer
	CHECK_HR(hr = pSelf->pSource->CopyVideoBuffer(pSelf->nNegWidth, pSelf->nNegHeight, buffer, size));

bail:
	return SUCCEEDED(hr) ?  0 : -1;
}

static int plugin_win_mf_consumer_video_pause(tmedia_consumer_t* self)
{
	plugin_win_mf_consumer_video_t* pSelf = (plugin_win_mf_consumer_video_t*)self;

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

static int plugin_win_mf_consumer_video_stop(tmedia_consumer_t* self)
{
	plugin_win_mf_consumer_video_t* pSelf = (plugin_win_mf_consumer_video_t*)self;

	if(!pSelf){
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    HRESULT hr = S_OK;

	// stop EVR watcher
	if(pSelf->pDisplayWatcher) {
		hr = pSelf->pDisplayWatcher->Stop();
	}

    // for the thread
    pSelf->bStarted = false;
    hr = MFUtils::ShutdownSession(pSelf->pSession, NULL); // stop session to wakeup the asynchronous thread
    if(pSelf->ppTread[0]){
        tsk_thread_join(&pSelf->ppTread[0]);
    }
    hr = MFUtils::ShutdownSession(NULL, pSelf->pSource); // stop source to release the camera

	// next start() will be called after prepare()
	return _plugin_win_mf_consumer_video_unprepare(pSelf);
}

static int _plugin_win_mf_consumer_video_unprepare(plugin_win_mf_consumer_video_t* pSelf)
{
	if(!pSelf){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(pSelf->bStarted) {
		// plugin_win_mf_producer_video_stop(TMEDIA_PRODUCER(pSelf));
		TSK_DEBUG_ERROR("Consumer must be stopped before calling unprepare");
	}

	if(pSelf->pDisplayWatcher) {
		pSelf->pDisplayWatcher->Stop();
	}
    if(pSelf->pSource){
		pSelf->pSource->Shutdown();
		pSelf->pSource = NULL;
    }
    if(pSelf->pSession){
        pSelf->pSession->Shutdown();
		pSelf->pSession = NULL;
    }

	SafeRelease(&pSelf->pDecoder);
    SafeRelease(&pSelf->pSession);
    SafeRelease(&pSelf->pSource);
    SafeRelease(&pSelf->pSinkActivate);
    SafeRelease(&pSelf->pTopologyFull);
	SafeRelease(&pSelf->pTopologyPartial);
	SafeRelease(&pSelf->pOutType);

	if(pSelf->pDisplayWatcher) {
		delete pSelf->pDisplayWatcher;
		pSelf->pDisplayWatcher = NULL;
	}

	pSelf->bPrepared = false;

	return 0;
}


//
//	Media Foundation video consumer object definition
//
/* constructor */
static tsk_object_t* plugin_win_mf_consumer_video_ctor(tsk_object_t * self, va_list * app)
{
	MFUtils::Startup();

	plugin_win_mf_consumer_video_t *pSelf = (plugin_win_mf_consumer_video_t *)self;
	if(pSelf){
		/* init base */
		tmedia_consumer_init(TMEDIA_CONSUMER(pSelf));
		TMEDIA_CONSUMER(pSelf)->video.display.chroma = tmedia_chroma_yuv420p;
		TMEDIA_CONSUMER(pSelf)->decoder.codec_id = tmedia_codec_id_none; // means accept RAW fames

		/* init self */
		// consumer->create_on_ui_thread = tsk_true;
		TMEDIA_CONSUMER(pSelf)->video.fps = 15;
		TMEDIA_CONSUMER(pSelf)->video.display.width = 0; // use codec value
		TMEDIA_CONSUMER(pSelf)->video.display.height = 0; // use codec value
		TMEDIA_CONSUMER(pSelf)->video.display.auto_resize = tsk_true;
	}
	return self;
}
/* destructor */
static tsk_object_t* plugin_win_mf_consumer_video_dtor(tsk_object_t * self)
{ 
	plugin_win_mf_consumer_video_t *pSelf = (plugin_win_mf_consumer_video_t *)self;
	if(pSelf){
		/* stop */
		if(pSelf->bStarted){
			plugin_win_mf_consumer_video_stop(TMEDIA_CONSUMER(pSelf));
		}

		/* deinit base */
		tmedia_consumer_deinit(TMEDIA_CONSUMER(pSelf));
		/* deinit self */
		_plugin_win_mf_consumer_video_unprepare(pSelf);
	}

	return self;
}
/* object definition */
static const tsk_object_def_t plugin_win_mf_consumer_video_def_s = 
{
	sizeof(plugin_win_mf_consumer_video_t),
	plugin_win_mf_consumer_video_ctor, 
	plugin_win_mf_consumer_video_dtor,
	tsk_null, 
};
/* plugin definition*/
static const tmedia_consumer_plugin_def_t plugin_win_mf_consumer_video_plugin_def_s = 
{
	&plugin_win_mf_consumer_video_def_s,
	
	tmedia_video,
	"Media Foundation video consumer",
	
	plugin_win_mf_consumer_video_set,
	plugin_win_mf_consumer_video_prepare,
	plugin_win_mf_consumer_video_start,
	plugin_win_mf_consumer_video_consume,
	plugin_win_mf_consumer_video_pause,
	plugin_win_mf_consumer_video_stop
};
const tmedia_consumer_plugin_def_t *plugin_win_mf_consumer_video_plugin_def_t = &plugin_win_mf_consumer_video_plugin_def_s;

// Run session async thread
static void* TSK_STDCALL RunSessionThread(void *pArg)
{
	plugin_win_mf_consumer_video_t *pSelf = (plugin_win_mf_consumer_video_t *)pArg;
	HRESULT hrStatus = S_OK;
	HRESULT hr = S_OK;
	IMFMediaEvent *pEvent = NULL;
	MediaEventType met;

	TSK_DEBUG_INFO("RunSessionThread (MF video consumer) - ENTER");

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
	TSK_DEBUG_INFO("RunSessionThread (MF video consumer) - EXIT");

	return NULL;
}