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
#include "mf_codec_topology.h"
#include "mf_utils.h"

#include "tsk_debug.h"

//
//	 MFCodecTopologySampleGrabberCB
//

class MFCodecTopologySampleGrabberCB : public IMFSampleGrabberSinkCallback 
{
    long m_cRef;
    MFCodecTopology *m_pCodecTopology;

    MFCodecTopologySampleGrabberCB(MFCodecTopology *pCodecTopology)
		: m_cRef(1)
	{
		m_pCodecTopology = pCodecTopology;
		m_pCodecTopology->AddRef();
	}
	virtual ~MFCodecTopologySampleGrabberCB()
	{
		SafeRelease(&m_pCodecTopology);
	}

public:
    // Create a new instance of the object.
	static HRESULT MFCodecTopologySampleGrabberCB::CreateInstance(MFCodecTopology *pCodecTopology, MFCodecTopologySampleGrabberCB **ppCB)
	{
		*ppCB = new (std::nothrow) MFCodecTopologySampleGrabberCB(pCodecTopology);

		if (ppCB == NULL)
		{
			return E_OUTOFMEMORY;
		}
		return S_OK;
	}

	STDMETHODIMP MFCodecTopologySampleGrabberCB::QueryInterface(REFIID riid, void** ppv)
	{
		static const QITAB qit[] = 
		{
			QITABENT(MFCodecTopologySampleGrabberCB, IMFSampleGrabberSinkCallback),
			QITABENT(MFCodecTopologySampleGrabberCB, IMFClockStateSink),
			{ 0 }
		};
		return QISearch(this, qit, riid, ppv);
	}

	STDMETHODIMP_(ULONG) MFCodecTopologySampleGrabberCB::AddRef()
	{
		return InterlockedIncrement(&m_cRef);
	}

	STDMETHODIMP_(ULONG) MFCodecTopologySampleGrabberCB::Release()
	{
		ULONG cRef = InterlockedDecrement(&m_cRef);
		if (cRef == 0)
		{
			delete this;
		}
		return cRef;

	}

	// IMFClockStateSink methods

	STDMETHODIMP MFCodecTopologySampleGrabberCB::OnClockStart(MFTIME hnsSystemTime, LONGLONG llClockStartOffset)
	{
		TSK_DEBUG_INFO("MFCodecTopologySampleGrabberCB::OnClockStart(%lld, %lld)", hnsSystemTime, llClockStartOffset);
		return S_OK;
	}

	STDMETHODIMP MFCodecTopologySampleGrabberCB::OnClockStop(MFTIME hnsSystemTime)
	{
		TSK_DEBUG_INFO("MFCodecTopologySampleGrabberCB::OnClockStop(%lld)", hnsSystemTime);
		return S_OK;
	}

	STDMETHODIMP MFCodecTopologySampleGrabberCB::OnClockPause(MFTIME hnsSystemTime)
	{
		TSK_DEBUG_INFO("MFCodecTopologySampleGrabberCB::OnClockPause(%lld)", hnsSystemTime);
		return S_OK;
	}

	STDMETHODIMP MFCodecTopologySampleGrabberCB::OnClockRestart(MFTIME hnsSystemTime)
	{
		TSK_DEBUG_INFO("MFCodecTopologySampleGrabberCB::OnClockRestart(%lld)", hnsSystemTime);
		return S_OK;
	}

	STDMETHODIMP MFCodecTopologySampleGrabberCB::OnClockSetRate(MFTIME hnsSystemTime, float flRate)
	{
		TSK_DEBUG_INFO("MFCodecTopologySampleGrabberCB::OnClockSetRate(%lld, %f)", hnsSystemTime, flRate);
		return S_OK;
	}

	// IMFSampleGrabberSink methods.

	STDMETHODIMP MFCodecTopologySampleGrabberCB::OnSetPresentationClock(IMFPresentationClock* pClock)
	{
		TSK_DEBUG_INFO("MFCodecTopologySampleGrabberCB::OnSetPresentationClock");
		return S_OK;
	}

	STDMETHODIMP MFCodecTopologySampleGrabberCB::OnProcessSample(
		REFGUID guidMajorMediaType, DWORD dwSampleFlags,
		LONGLONG llSampleTime, LONGLONG llSampleDuration, const BYTE * pSampleBuffer,
		DWORD dwSampleSize)
	{
		HRESULT hr = S_OK;
		IMFSample *pSample = NULL;
		IMFMediaBuffer* pMediaBuffer = NULL;
		BYTE* _pcBufferPtr = NULL;

		CHECK_HR(hr = MFUtils::CreateMediaSample(dwSampleSize, &pSample));
		CHECK_HR(hr = pSample->SetSampleTime(llSampleTime));
		CHECK_HR(hr = pSample->SetSampleDuration(llSampleDuration));
		CHECK_HR(hr = pSample->GetBufferByIndex(0, &pMediaBuffer));
		CHECK_HR(hr = pMediaBuffer->Lock(&_pcBufferPtr, NULL, NULL));
		memcpy(_pcBufferPtr, pSampleBuffer, dwSampleSize);
		CHECK_HR(hr = pMediaBuffer->SetCurrentLength(dwSampleSize));
		CHECK_HR(hr = pMediaBuffer->Unlock());

		m_pCodecTopology->m_SampleQueue.Queue(pSample); // thread-safe
		
bail:
		SafeRelease(&pSample);
		SafeRelease(&pMediaBuffer);
		return hr;
	}

	STDMETHODIMP MFCodecTopologySampleGrabberCB::OnShutdown()
	{
		TSK_DEBUG_INFO("MFCodecTopologySampleGrabberCB::OnShutdown");
		return S_OK;
	}
};

//
//	 MFCodecTopology
//


MFCodecTopology::MFCodecTopology(MFCodec* pCodec, HRESULT &hr)
: m_nRefCount(1)
, m_bInitialized(FALSE)
, m_bStarted(FALSE)
, m_pCodec(NULL)
, m_pSource(NULL)
, m_pSession(NULL)
, m_pTopologyFull(NULL)
, m_pTopologyPartial(NULL)
, m_pOutputType(NULL)
, m_pInputType(NULL)
, m_pGrabberCallback(NULL)
, m_pGrabberActivate(NULL)
, m_pTread(NULL)
{
	hr = S_OK;

	if(!pCodec)
	{
		CHECK_HR(hr = E_POINTER);
	}

	m_pCodec = pCodec;
	m_pCodec->AddRef();

bail: ;
}

MFCodecTopology::~MFCodecTopology()
{
	DeInitialize();
}

ULONG MFCodecTopology::AddRef()
{
    return InterlockedIncrement(&m_nRefCount);
}

ULONG  MFCodecTopology::Release()
{
    ULONG uCount = InterlockedDecrement(&m_nRefCount);
    if (uCount == 0)
    {
        delete this;
    }
    // For thread safety, return a temporary variable.
    return uCount;
}

HRESULT MFCodecTopology::QueryInterface(REFIID iid, void** ppv)
{
	return E_NOTIMPL;
}

HRESULT MFCodecTopology::Start()
{
	HRESULT hr = S_OK;

	if(m_bStarted)
	{
		return S_OK;
	}

	if(!m_bInitialized)
	{
		CHECK_HR(hr = E_FAIL);
	}

	CHECK_HR(hr = MFUtils::RunSession(m_pSession, m_pTopologyFull));

	// Start asynchronous watcher thread
	m_bStarted = TRUE;
	int ret = tsk_thread_create(&m_pTread, MFCodecTopology::RunSessionThread, this);
	if(ret != 0)
	{
		TSK_DEBUG_ERROR("Failed to create thread");
		m_bStarted = FALSE;
		if(m_pTread)
		{
			tsk_thread_join(&m_pTread);
		}
		MFUtils::ShutdownSession(m_pSession, m_pSource);
		CHECK_HR(hr = E_FAIL);
	}

	// FIXME
	Sleep(2000);

bail:
	return hr;
}

HRESULT MFCodecTopology::Stop()
{
	HRESULT hr = S_OK;

	if(!m_bStarted)
	{
		return S_OK;
	}

	m_bStarted = FALSE;
    hr = MFUtils::ShutdownSession(m_pSession, NULL); // stop session to wakeup the asynchronous thread
    if(m_pTread)
	{
        tsk_thread_join(&m_pTread);
    }
    hr = MFUtils::ShutdownSession(NULL, m_pSource);
	
	return hr;
}

HRESULT MFCodecTopology::Initialize()
{
	HRESULT hr = S_OK;
	IMFAttributes* pSessionAttributes = NULL;

	if(m_bInitialized)
	{
		CHECK_HR(hr = E_FAIL);
	}

	// Set session attributes
	CHECK_HR(hr = MFCreateAttributes(&pSessionAttributes, 1));
	CHECK_HR(hr = pSessionAttributes->SetUINT32(PLUGIN_MF_LOW_LATENCY, 1));

	// Get input and output type
	CHECK_HR(hr = m_pCodec->GetInputType(&m_pInputType));
	CHECK_HR(hr = m_pCodec->GetOutputType(&m_pOutputType));

	// Create custom source
	CHECK_HR(hr = CMFSource::CreateInstanceEx(IID_IMFMediaSource, (void**)&m_pSource, m_pInputType));

	// Create the sample grabber sink.
	CHECK_HR(hr = MFCodecTopologySampleGrabberCB::CreateInstance(this, &m_pGrabberCallback));
	CHECK_HR(hr = MFCreateSampleGrabberSinkActivate(m_pOutputType, m_pGrabberCallback, &m_pGrabberActivate));

	// To run as fast as possible, set this attribute (requires Windows 7 or later):
	CHECK_HR(hr = m_pGrabberActivate->SetUINT32(MF_SAMPLEGRABBERSINK_IGNORE_CLOCK, TRUE));

	// Create the Media Session.
	CHECK_HR(hr = MFCreateMediaSession(pSessionAttributes, &m_pSession));

	// Create the topology.
	CHECK_HR(hr = MFUtils::CreateTopology(
		m_pSource, 
		m_pCodec->GetMFT(), 
		m_pGrabberActivate, 
		NULL, // no preview
		m_pOutputType, 
		&m_pTopologyPartial));
	// Resolve topology (adds video processors if needed).
	CHECK_HR(hr = MFUtils::ResolveTopology(m_pTopologyPartial, &m_pTopologyFull));

	m_bInitialized = TRUE;

bail:
	SafeRelease(&pSessionAttributes);

	if(FAILED(hr))
	{
		DeInitialize();
	}

	return hr;
}

void* TSK_STDCALL MFCodecTopology::RunSessionThread(void *pArg)
{
	MFCodecTopology *pSelf = (MFCodecTopology *)pArg;
	HRESULT hrStatus = S_OK;
	HRESULT hr = S_OK;
	IMFMediaEvent *pEvent = NULL;
	MediaEventType met;

	TSK_DEBUG_INFO("RunSessionThread (MFCodecTopology) - ENTER");

	while(pSelf->isStarted())
	{
		CHECK_HR(hr = pSelf->m_pSession->GetEvent(0, &pEvent));
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
	TSK_DEBUG_INFO("RunSessionThread (MFCodecTopology) - EXIT");

	return NULL;
}

HRESULT MFCodecTopology::DeInitialize()
{
	Stop();

	SafeRelease(&m_pCodec);
	SafeRelease(&m_pSource);
	SafeRelease(&m_pCodec);
	SafeRelease(&m_pSession);
	SafeRelease(&m_pTopologyFull);
	SafeRelease(&m_pTopologyPartial);
	SafeRelease(&m_pOutputType);
	SafeRelease(&m_pInputType);
	SafeRelease(&m_pGrabberCallback);
	SafeRelease(&m_pGrabberActivate);

	if(m_pTread)
	{
		tsk_thread_join(&m_pTread);
	}

	m_SampleQueue.Clear();

	m_bInitialized = FALSE;

	return S_OK;
}

HRESULT MFCodecTopology::ProcessInput(IMFSample* pSample)
{
	HRESULT hr = S_OK;
	IMFMediaBuffer* pMediaBuffer = NULL;
	BYTE* _pcBufferPtr = NULL;

	if(!pSample)
	{
		CHECK_HR(hr = E_POINTER);
	}

	if(m_pCodec->GetMediaType() != MFCodecMediaType_Video)
	{
		CHECK_HR(hr = E_NOTIMPL);
	}

	if(!m_bStarted)
	{
		CHECK_HR(hr = Start());
	}

	CHECK_HR(hr = pSample->GetBufferByIndex(0, &pMediaBuffer));
	
	DWORD dwDataLength = 0;
	BOOL bLocked = FALSE;
	CHECK_HR(hr = pMediaBuffer->GetCurrentLength(&dwDataLength));
	bLocked = TRUE;
	if(dwDataLength > 0)
	{
		CHECK_HR(hr = pMediaBuffer->Lock(&_pcBufferPtr, NULL, NULL));
		CHECK_HR(hr = m_pSource->CopyVideoBuffer(
			dynamic_cast<MFCodecVideo*>(m_pCodec)->GetWidth(),
			dynamic_cast<MFCodecVideo*>(m_pCodec)->GetHeight(),
			_pcBufferPtr, dwDataLength));
	}

bail:
	if(bLocked)
	{
		pMediaBuffer->Unlock();
	}
	SafeRelease(&pMediaBuffer);
	return hr;
}

HRESULT MFCodecTopology::ProcessOutput(IMFSample **ppSample)
{
	HRESULT hr = S_OK;

	if(!ppSample)
	{
		CHECK_HR(hr = E_POINTER);
	}

	if(!m_SampleQueue.IsEmpty())
	{
		CHECK_HR(hr = m_SampleQueue.Dequeue(ppSample)); // thread-safe
	}

bail:
	return hr;
}

//
//	MFCodecVideoTopology
//


MFCodecVideoTopology::MFCodecVideoTopology(MFCodec* pCodec, HRESULT &hr)
: MFCodecTopology(pCodec, hr)
, m_nWidth(0)
, m_nHeight(0)
{
	assert(pCodec->GetMediaType() == MFCodecMediaType_Video);
}

MFCodecVideoTopology::~MFCodecVideoTopology()
{

}


