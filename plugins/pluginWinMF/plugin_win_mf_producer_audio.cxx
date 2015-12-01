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

#include "tinydav/audio/tdav_producer_audio.h"

#include "tsk_thread.h"
#include "tsk_debug.h"

static void* TSK_STDCALL RunSessionThread(void *pArg);

typedef struct plugin_win_mf_producer_audio_s
{
	TDAV_DECLARE_PRODUCER_AUDIO;

	bool bStarted;
	tsk_thread_handle_t* ppTread[1];

    DeviceListAudio* pDeviceList;

    IMFMediaSession *pSession;
    IMFMediaSource *pSource;
    SampleGrabberCB *pCallback;
    IMFActivate *pSinkActivate;
    IMFTopology *pTopology;
    IMFMediaType *pType;
}
plugin_win_mf_producer_audio_t;

/* ============ Media Producer Interface ================= */
static int plugin_win_mf_producer_audio_set(tmedia_producer_t* self, const tmedia_param_t* param)
{	
	plugin_win_mf_producer_audio_t* pSelf = (plugin_win_mf_producer_audio_t*)self;
	if(param->plugin_type == tmedia_ppt_producer){
	}
	return tdav_producer_audio_set(TDAV_PRODUCER_AUDIO(pSelf), param);
}

static int plugin_win_mf_producer_audio_prepare(tmedia_producer_t* self, const tmedia_codec_t* codec)
{
	plugin_win_mf_producer_audio_t* pSelf = (plugin_win_mf_producer_audio_t*)self;

	if(!pSelf || !codec){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	TMEDIA_PRODUCER(pSelf)->audio.channels = TMEDIA_CODEC_CHANNELS_AUDIO_ENCODING(codec);
	TMEDIA_PRODUCER(pSelf)->audio.rate = TMEDIA_CODEC_RATE_ENCODING(codec);
	TMEDIA_PRODUCER(pSelf)->audio.ptime = TMEDIA_CODEC_PTIME_AUDIO_ENCODING(codec);

	TSK_DEBUG_INFO("MF audio producer: channels=%d, rate=%d, ptime=%d",
		TMEDIA_PRODUCER(pSelf)->audio.channels,
		TMEDIA_PRODUCER(pSelf)->audio.rate,
		TMEDIA_PRODUCER(pSelf)->audio.ptime
		);

	HRESULT hr = S_OK;

	// create device list object
	if(!pSelf->pDeviceList && !(pSelf->pDeviceList = new DeviceListAudio())){
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
		IMFActivate* pActivate = NULL;
		// Get best MF audio source
		hr = pSelf->pDeviceList->GetDeviceBest(&pActivate);
		if(!SUCCEEDED(hr) || !pActivate){
			TSK_DEBUG_ERROR("Failed to get best MF audio source");
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
			TSK_DEBUG_ERROR("ActivateObject(MF audio source) failed");
			goto bail;
		}

		// Create and configure the media type
		CHECK_HR(hr = MFCreateMediaType(&pSelf->pType));
		CHECK_HR(hr = pSelf->pType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio));
		CHECK_HR(hr = pSelf->pType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM));
		CHECK_HR(hr = pSelf->pType->SetUINT32(MF_MT_AUDIO_NUM_CHANNELS, TMEDIA_PRODUCER(pSelf)->audio.channels));
		CHECK_HR(hr = pSelf->pType->SetUINT32(MF_MT_AUDIO_SAMPLES_PER_SECOND, TMEDIA_PRODUCER(pSelf)->audio.rate));
		CHECK_HR(hr = pSelf->pType->SetUINT32(MF_MT_AUDIO_BITS_PER_SAMPLE, TMEDIA_PRODUCER(pSelf)->audio.bits_per_sample));
		CHECK_HR(hr = pSelf->pType->SetUINT32(MF_MT_ALL_SAMPLES_INDEPENDENT, TRUE)); // because uncompressed media type
		CHECK_HR(hr = pSelf->pType->SetUINT32(MF_MT_FIXED_SIZE_SAMPLES, TRUE));
		UINT32 nBlockAlign = TMEDIA_PRODUCER(pSelf)->audio.channels * (TMEDIA_PRODUCER(pSelf)->audio.bits_per_sample >> 3);
		UINT32 nAvgBytesPerSec = (nBlockAlign * TMEDIA_PRODUCER(pSelf)->audio.rate);
		CHECK_HR(hr = pSelf->pType->SetUINT32(MF_MT_AUDIO_BLOCK_ALIGNMENT, nBlockAlign));
		CHECK_HR(hr = pSelf->pType->SetUINT32(MF_MT_AUDIO_AVG_BYTES_PER_SECOND, nAvgBytesPerSec));
		
		// Create the sample grabber sink.
		CHECK_HR(hr = SampleGrabberCB::CreateInstance(TMEDIA_PRODUCER(pSelf), &pSelf->pCallback));
		CHECK_HR(hr = MFCreateSampleGrabberSinkActivate(pSelf->pType, pSelf->pCallback, &pSelf->pSinkActivate));
		
		// To run as fast as possible, set this attribute (requires Windows 7):
		CHECK_HR(hr = pSelf->pSinkActivate->SetUINT32(MF_SAMPLEGRABBERSINK_IGNORE_CLOCK, TRUE));
		
		// Create the Media Session.
		CHECK_HR(hr = MFCreateMediaSession(NULL, &pSelf->pSession));
		
		// Create the topology.
		CHECK_HR(hr = MFUtils::CreateTopology(pSelf->pSource, NULL/*NO ENCODER*/, pSelf->pSinkActivate, NULL/*Preview*/, pSelf->pType, &pSelf->pTopology));
	}

bail:
	return SUCCEEDED(hr) ? 0 : -1;
}

static int plugin_win_mf_producer_audio_start(tmedia_producer_t* self)
{
	plugin_win_mf_producer_audio_t* pSelf = (plugin_win_mf_producer_audio_t*)self;

	if(!pSelf){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(pSelf->bStarted){
		TSK_DEBUG_INFO("MF audio producer already started");
		return 0;
	}

	HRESULT hr = S_OK;

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

static int plugin_win_mf_producer_audio_pause(tmedia_producer_t* self)
{
	plugin_win_mf_producer_audio_t* pSelf = (plugin_win_mf_producer_audio_t*)self;

	if(!pSelf){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	HRESULT hr = MFUtils::PauseSession(pSelf->pSession);

	return SUCCEEDED(hr) ? 0 : -1;
}

static int plugin_win_mf_producer_audio_stop(tmedia_producer_t* self)
{
	plugin_win_mf_producer_audio_t* pSelf = (plugin_win_mf_producer_audio_t*)self;
	
    if(!pSelf){
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    HRESULT hr = S_OK;

    // for the thread
    pSelf->bStarted = false;
    hr = MFUtils::ShutdownSession(pSelf->pSession, NULL); // stop session to wakeup the asynchronous thread
    if(pSelf->ppTread[0]){
        tsk_thread_join(&pSelf->ppTread[0]);
    }
    hr = MFUtils::ShutdownSession(NULL, pSelf->pSource); // stop source to release the camera

    return 0;
}


//
//	WaveAPI producer object definition
//
/* constructor */
static tsk_object_t* plugin_win_mf_producer_audio_ctor(tsk_object_t * self, va_list * app)
{
	MFUtils::Startup();

	plugin_win_mf_producer_audio_t *pSelf = (plugin_win_mf_producer_audio_t*)self;
	if(pSelf){
		/* init base */
		tdav_producer_audio_init(TDAV_PRODUCER_AUDIO(pSelf));
		/* init self */
		
	}
	return self;
}
/* destructor */
static tsk_object_t* plugin_win_mf_producer_audio_dtor(tsk_object_t * self)
{ 
	plugin_win_mf_producer_audio_t *pSelf = (plugin_win_mf_producer_audio_t *)self;
	if(pSelf){
		/* stop */
		if(pSelf->bStarted){
			plugin_win_mf_producer_audio_stop(TMEDIA_PRODUCER(pSelf));
		}

		/* deinit base */
		tdav_producer_audio_deinit(TDAV_PRODUCER_AUDIO(pSelf));
		/* deinit self */
		if(pSelf->pDeviceList){
			delete pSelf->pDeviceList, pSelf->pDeviceList = NULL;
        }
        if(pSelf->pSource){
			pSelf->pSource->Shutdown();
        }
        if(pSelf->pSession){
            pSelf->pSession->Shutdown();
        }

        SafeRelease(&pSelf->pSession);
        SafeRelease(&pSelf->pSource);
        SafeRelease(&pSelf->pCallback);
        SafeRelease(&pSelf->pSinkActivate);
        SafeRelease(&pSelf->pTopology);
        SafeRelease(&pSelf->pType);
	}

	return self;
}
/* object definition */
static const tsk_object_def_t plugin_win_mf_producer_audio_def_s = 
{
	sizeof(plugin_win_mf_producer_audio_t),
	plugin_win_mf_producer_audio_ctor, 
	plugin_win_mf_producer_audio_dtor,
	tdav_producer_audio_cmp, 
};
/* plugin definition*/
static const tmedia_producer_plugin_def_t plugin_win_mf_producer_audio_plugin_def_s = 
{
	&plugin_win_mf_producer_audio_def_s,
	
	tmedia_audio,
	"Media Foundation audio producer",
	
	plugin_win_mf_producer_audio_set,
	plugin_win_mf_producer_audio_prepare,
	plugin_win_mf_producer_audio_start,
	plugin_win_mf_producer_audio_pause,
	plugin_win_mf_producer_audio_stop
};
const tmedia_producer_plugin_def_t *plugin_win_mf_producer_audio_plugin_def_t = &plugin_win_mf_producer_audio_plugin_def_s;


// Run session async thread
static void* TSK_STDCALL RunSessionThread(void *pArg)
{
	plugin_win_mf_producer_audio_t *pSelf = (plugin_win_mf_producer_audio_t *)pArg;
	HRESULT hrStatus = S_OK;
	HRESULT hr = S_OK;
	IMFMediaEvent *pEvent = NULL;
	MediaEventType met;

	TSK_DEBUG_INFO("RunSessionThread (audio) - ENTER");

	while(pSelf->bStarted){
		CHECK_HR(hr = pSelf->pSession->GetEvent(0, &pEvent));
		CHECK_HR(hr = pEvent->GetStatus(&hrStatus));
		CHECK_HR(hr = pEvent->GetType(&met));

		if (FAILED(hrStatus))
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
	TSK_DEBUG_INFO("RunSessionThread (audio) - EXIT");

	return NULL;
}
