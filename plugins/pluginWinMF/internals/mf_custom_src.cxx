/* 
* Copyright (C) Microsoft Corporation. All rights reserved.
* Copyright (C) 2013 Mamadou DIOP
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

// Implementing custom source: http://msdn.microsoft.com/en-us/library/windows/desktop/ms700134(v=vs.85).aspx

#include "mf_custom_src.h"
#include "mf_utils.h"

#include "tsk_debug.h"

#include <assert.h>

//
// Locking:
//     The source and stream objects both have critical sections. If you
//     hold both locks, the source lock must be held FIRST, to avoid
//     deadlocks.
//
// Shutdown:
//     Most methods start by calling CheckShutdown(). This method
//     fails if the source was shut down.
//



template <class T>
T AlignUp(T num, T mult)
{
    assert(num >= 0);
    T tmp = num + mult - 1;
    return tmp - (tmp % mult);
}



// Helper Functions

HRESULT QueueEventWithIUnknown(
    IMFMediaEventGenerator *pMEG,
    MediaEventType meType,
    HRESULT hrStatus,
    IUnknown *pUnk);

LONGLONG BufferSizeFromAudioDuration(const WAVEFORMATEX *pWav, LONGLONG duration);

HRESULT CMFSource_CreateInstance(REFIID iid, void **ppMFT)
{
	return CMFSource::CreateInstance(iid, ppMFT);
}


//-------------------------------------------------------------------
// Name: CreateInstance
// Description: Static method to create an instance of the source.
//
// iid:         IID of the requested interface on the source.
// ppSource:    Receives a ref-counted pointer to the source.
//-------------------------------------------------------------------

HRESULT CMFSource::CreateInstance(REFIID iid, void **ppSource) // Called when source used as plugin
{
	return CreateInstanceEx(iid, ppSource, NULL);
}

HRESULT CMFSource::CreateInstanceEx(REFIID iid, void **ppSource, IMFMediaType *pMediaType) // Called when source directly called
{
	if (ppSource == NULL)
    {
        return E_POINTER;
    }

    HRESULT hr = S_OK;
    CMFSource *pSource = new (std::nothrow) CMFSource(hr, pMediaType); // Created with ref count = 1.
    if (pSource == NULL)
    {
        return E_OUTOFMEMORY;
    }

    if (SUCCEEDED(hr))
    {
        hr = pSource->QueryInterface(iid, ppSource);
		if(SUCCEEDED(hr))
		{
			((CMFSource*)(*ppSource))->AddRef();
		}
    }

    SafeRelease(&pSource);
    return hr;
}


//-------------------------------------------------------------------
// CMFSource constructor.
//
// hr: If the constructor fails, this value is set to a failure code.
//-------------------------------------------------------------------

CMFSource::CMFSource(HRESULT& hr, IMFMediaType *pMediaType)
  : m_nRefCount(1),
    m_pEventQueue(NULL),
    m_pPresentationDescriptor(NULL),
    m_IsShutdown(FALSE),
    m_state(STATE_STOPPED),
    m_pStream(NULL),
	m_pMediaType(NULL)
{
    // Create the media event queue.
    hr = MFCreateEventQueue(&m_pEventQueue);

	if(pMediaType)
	{
		m_pMediaType = pMediaType;
		pMediaType->AddRef();
	}

    InitializeCriticalSection(&m_critSec);
}


//-------------------------------------------------------------------
// CMFSource destructor.
//-------------------------------------------------------------------


CMFSource::~CMFSource()
{
    assert(m_IsShutdown);
    assert(m_nRefCount == 0);
	SafeRelease(&m_pMediaType);

    DeleteCriticalSection(&m_critSec);
}

// IMFCustomSource methods

HRESULT CMFSource::CopyVideoBuffer(UINT32 nWidth, UINT32 nHeight, const void* pBufferPtr, UINT32 nBufferSize)
{
	if(!pBufferPtr)
	{
		TSK_DEBUG_ERROR("Invalid buffer pointer");
		return E_POINTER;
	}

	if(!nWidth || !nHeight || !nBufferSize)
	{
		TSK_DEBUG_ERROR("Invalid parameter");
		return E_INVALIDARG;
	}
	if(m_pStream)
	{
		return m_pStream->CopyVideoBuffer(nWidth, nHeight, pBufferPtr, nBufferSize);
	}
	else
	{
		TSK_DEBUG_ERROR("No stream associated to this source");
		return E_NOT_VALID_STATE;
	}
}

// IUnknown methods

ULONG CMFSource::AddRef()
{
    return InterlockedIncrement(&m_nRefCount);
}

ULONG  CMFSource::Release()
{
    ULONG uCount = InterlockedDecrement(&m_nRefCount);
    if (uCount == 0)
    {
        delete this;
    }
    // For thread safety, return a temporary variable.
    return uCount;
}

HRESULT CMFSource::QueryInterface(REFIID iid, void** ppv)
{
    static const QITAB qit[] =
    {
        QITABENT(CMFSource, IMFMediaEventGenerator),
        QITABENT(CMFSource, IMFMediaSource),
        { 0 }
    };
    return QISearch(this, qit, iid, ppv);
}


// IMFMediaEventGenerator methods
//
// All of the IMFMediaEventGenerator methods do the following:
// 1. Check for shutdown status.
// 2. Call the event generator helper object.

HRESULT CMFSource::BeginGetEvent(IMFAsyncCallback* pCallback, IUnknown* punkState)
{
    HRESULT hr = S_OK;

    EnterCriticalSection(&m_critSec);

    hr = CheckShutdown();

    if (SUCCEEDED(hr))
    {
        hr = m_pEventQueue->BeginGetEvent(pCallback, punkState);
    }

    LeaveCriticalSection(&m_critSec);

    return hr;
}

HRESULT CMFSource::EndGetEvent(IMFAsyncResult* pResult, IMFMediaEvent** ppEvent)
{
    HRESULT hr = S_OK;

    EnterCriticalSection(&m_critSec);

    hr = CheckShutdown();

    if (SUCCEEDED(hr))
    {
        hr = m_pEventQueue->EndGetEvent(pResult, ppEvent);
    }

    LeaveCriticalSection(&m_critSec);

    return hr;
}

HRESULT CMFSource::GetEvent(DWORD dwFlags, IMFMediaEvent** ppEvent)
{
    // NOTE: GetEvent can block indefinitely, so we don't hold the
    //       CMFSource lock. This requires some juggling with the
    //       event queue pointer.

    HRESULT hr = S_OK;

    IMFMediaEventQueue *pQueue = NULL;

    EnterCriticalSection(&m_critSec);

    // Check shutdown
    hr = CheckShutdown();

    if (SUCCEEDED(hr))
    {
        pQueue = m_pEventQueue;
        pQueue->AddRef();
    }

    LeaveCriticalSection(&m_critSec);

    if (SUCCEEDED(hr))
    {
        hr = pQueue->GetEvent(dwFlags, ppEvent);
    }

    SafeRelease(&pQueue);
    return hr;
}

HRESULT CMFSource::QueueEvent(MediaEventType met, REFGUID guidExtendedType, HRESULT hrStatus, const PROPVARIANT* pvValue)
{
    HRESULT hr = S_OK;

    EnterCriticalSection(&m_critSec);

    hr = CheckShutdown();

    if (SUCCEEDED(hr))
    {
        hr = m_pEventQueue->QueueEventParamVar(met, guidExtendedType, hrStatus, pvValue);
    }

    LeaveCriticalSection(&m_critSec);

    return hr;
}


// IMFMediaSource methods


//-------------------------------------------------------------------
// Name: CreatePresentationDescriptor
// Description: Returns a copy of the default presentation descriptor.
//-------------------------------------------------------------------

HRESULT CMFSource::CreatePresentationDescriptor(IMFPresentationDescriptor** ppPresentationDescriptor)
{
    if (ppPresentationDescriptor == NULL)
    {
        return E_POINTER;
    }

    EnterCriticalSection(&m_critSec);

    HRESULT hr = S_OK;

    hr = CheckShutdown();

    if (SUCCEEDED(hr))
    {
        if (m_pPresentationDescriptor == NULL)
        {
            hr = CreatePresentationDescriptor();
        }
    }

    // Clone our default presentation descriptor.
    if (SUCCEEDED(hr))
    {
        hr = m_pPresentationDescriptor->Clone(ppPresentationDescriptor);
    }

    LeaveCriticalSection(&m_critSec);

    return hr;
}


//-------------------------------------------------------------------
// Name: GetCharacteristics
// Description: Returns flags the describe the source.
//-------------------------------------------------------------------

HRESULT CMFSource::GetCharacteristics(DWORD* pdwCharacteristics)
{
    if (pdwCharacteristics == NULL)
    {
        return E_POINTER;
    }

    EnterCriticalSection(&m_critSec);

    HRESULT hr = S_OK;

    hr = CheckShutdown();

    if (SUCCEEDED(hr))
    {
        *pdwCharacteristics =  MFMEDIASOURCE_CAN_PAUSE | MFMEDIASOURCE_IS_LIVE;
    }

    LeaveCriticalSection(&m_critSec);

    return hr;
}

//-------------------------------------------------------------------
// Name: Start
// Description: Switches to running state.
//-------------------------------------------------------------------

HRESULT CMFSource::Start(
    IMFPresentationDescriptor* pPresentationDescriptor,
    const GUID* pguidTimeFormat,
    const PROPVARIANT* pvarStartPosition
    )
{
    HRESULT hr = S_OK;
    LONGLONG llStartOffset = 0;
    BOOL bIsSeek = FALSE;
    BOOL bIsRestartFromCurrentPosition = FALSE;
    BOOL bQueuedStartEvent = FALSE;

    IMFMediaEvent *pEvent = NULL;

    PROPVARIANT var;
    PropVariantInit(&var);

    // Check parameters.
    // Start position and presentation descriptor cannot be NULL.
    if (pvarStartPosition == NULL || pPresentationDescriptor == NULL)
    {
        return E_INVALIDARG;
    }

    // Check the time format. Must be "reference time" units.
    if ((pguidTimeFormat != NULL) && (*pguidTimeFormat != GUID_NULL))
    {
        // Unrecognized time format GUID.
        return MF_E_UNSUPPORTED_TIME_FORMAT;
    }

    EnterCriticalSection(&m_critSec);

    // Fail if the source is shut down.
    CHECK_HR(hr = CheckShutdown());

    // Check the start position.
    if (pvarStartPosition->vt == VT_I8)
    {
        // Start position is given in pvarStartPosition in 100-ns units.
        llStartOffset = pvarStartPosition->hVal.QuadPart;

        if (m_state != STATE_STOPPED)
        {
            // Source is running or paused, so this is a seek.
            bIsSeek = TRUE;
        }
    }
    else if (pvarStartPosition->vt == VT_EMPTY)
    {
        // Start position is "current position".
        // For stopped, that means 0. Otherwise, use the current position.
        if (m_state == STATE_STOPPED)
        {
            llStartOffset = 0;
        }
        else
        {
            llStartOffset = GetCurrentPosition();
            bIsRestartFromCurrentPosition = TRUE;
        }
    }
    else
    {
        // We don't support this time format.
        hr =  MF_E_UNSUPPORTED_TIME_FORMAT;
        goto bail;
    }

    // Validate the caller's presentation descriptor.
    CHECK_HR(hr = ValidatePresentationDescriptor(pPresentationDescriptor));

    // Sends the MENewStream or MEUpdatedStream event.
    CHECK_HR(hr = QueueNewStreamEvent(pPresentationDescriptor));

    // Notify the stream of the new start time.
    CHECK_HR(hr = m_pStream->SetPosition(llStartOffset));

    // Send Started or Seeked events.

    var.vt = VT_I8;
    var.hVal.QuadPart = llStartOffset;

    // Send the source event.
    if (bIsSeek)
    {
        CHECK_HR(hr = QueueEvent(MESourceSeeked, GUID_NULL, hr, &var));
    }
    else
    {
        // For starting, if we are RESTARTING from the current position and our
        // previous state was running/paused, then we need to add the
        // MF_EVENT_SOURCE_ACTUAL_START attribute to the event. This requires
        // creating the event object first.

        // Create the event.
        CHECK_HR(hr = MFCreateMediaEvent(MESourceStarted, GUID_NULL, hr, &var, &pEvent));

        // For restarts, set the actual start time as an attribute.
        if (bIsRestartFromCurrentPosition)
        {
            CHECK_HR(hr = pEvent->SetUINT64(MF_EVENT_SOURCE_ACTUAL_START, llStartOffset));
        }

        // Now  queue the event.
        CHECK_HR(hr = m_pEventQueue->QueueEvent(pEvent));
    }

    bQueuedStartEvent = TRUE;

    // Send the stream event.
    if (m_pStream)
    {
        if (bIsSeek)
        {
            CHECK_HR(hr = m_pStream->QueueEvent(MEStreamSeeked, GUID_NULL, hr, &var));
        }
        else
        {
            CHECK_HR(hr = m_pStream->QueueEvent(MEStreamStarted, GUID_NULL, hr, &var));
        }
    }

    if (bIsSeek)
    {
        // For seek requests, flush any queued samples.
        CHECK_HR(hr = m_pStream->Flush());
    }
    else
    {
        // Otherwise, deliver any queued samples.
        CHECK_HR(hr = m_pStream->DeliverQueuedSamples());
    }

	// Initialize Stream parameters
	CHECK_HR(hr = m_pStream->InitializeParams());

    m_state = STATE_STARTED;

bail:

    // If a failure occurred and we have not sent the
    // MESourceStarted/MESourceSeeked event yet, then it is
    // OK just to return an error code from Start().

    // If a failure occurred and we have already sent the
    // event (with a success code), then we need to raise an
    // MEError event.

    if (FAILED(hr) && bQueuedStartEvent)
    {
        hr = QueueEvent(MEError, GUID_NULL, hr, &var);
    }

    PropVariantClear(&var);
    SafeRelease(&pEvent);

    LeaveCriticalSection(&m_critSec);
	
    return hr;
}


//-------------------------------------------------------------------
// Name: Pause
// Description: Switches to paused state.
//-------------------------------------------------------------------

HRESULT CMFSource::Pause()
{
    EnterCriticalSection(&m_critSec);

    HRESULT hr = S_OK;

    hr = CheckShutdown();

    // Pause is only allowed from started state.
    if (SUCCEEDED(hr))
    {
        if (m_state != STATE_STARTED)
        {
            hr = MF_E_INVALID_STATE_TRANSITION;
        }
    }

    // Send the appropriate events.
    if (SUCCEEDED(hr))
    {
        if (m_pStream)
        {
            hr = m_pStream->QueueEvent(MEStreamPaused, GUID_NULL, S_OK, NULL);
        }
    }

    if (SUCCEEDED(hr))
    {
        hr = QueueEvent(MESourcePaused, GUID_NULL, S_OK, NULL);
    }

    // Update our state.
    if (SUCCEEDED(hr))
    {
        m_state = STATE_PAUSED;
    }

    LeaveCriticalSection(&m_critSec);

    return hr;
}


//-------------------------------------------------------------------
// Name: Stop
// Description: Switches to stopped state.
//-------------------------------------------------------------------

HRESULT CMFSource::Stop()
{
    EnterCriticalSection(&m_critSec);

    HRESULT hr = S_OK;

    hr = CheckShutdown();

    if (SUCCEEDED(hr))
    {
        // Update our state.
        m_state = STATE_STOPPED;

        // Flush all queued samples.
        hr = m_pStream->Flush();
    }

    //
    // Queue events.
    //

    if (SUCCEEDED(hr))
    {
        if (m_pStream)
        {
            hr = m_pStream->QueueEvent(MEStreamStopped, GUID_NULL, S_OK, NULL);
        }
    }
    if (SUCCEEDED(hr))
    {
        hr = QueueEvent(MESourceStopped, GUID_NULL, S_OK, NULL);
    }

    LeaveCriticalSection(&m_critSec);

    return hr;
}


//-------------------------------------------------------------------
// Name: Shutdown
// Description: Releases resources.
//
// The source and stream objects hold reference counts on each other.
// To avoid memory leaks caused by circular ref. counts, the Shutdown
// method releases the pointer to the stream.
//-------------------------------------------------------------------

HRESULT CMFSource::Shutdown()
{
    EnterCriticalSection(&m_critSec);

    HRESULT hr = S_OK;

    hr = CheckShutdown();

    if (SUCCEEDED(hr))
    {
        // Shut down the stream object.
        if (m_pStream)
        {
            (void)m_pStream->Shutdown();
        }

        // Shut down the event queue.
        if (m_pEventQueue)
        {
            (void)m_pEventQueue->Shutdown();
        }

        // Release objects.
        SafeRelease(&m_pStream);
        SafeRelease(&m_pEventQueue);
        SafeRelease(&m_pPresentationDescriptor);

        // Set our shutdown flag.
        m_IsShutdown = TRUE;
    }

    LeaveCriticalSection(&m_critSec);

    return hr;
}

/////////////// Private CMFSource methods

// NOTE: These private methods do not hold the source's critical
// section. The caller must ensure the critical section is held.
// Also, these methods do not check for shut-down.


//-------------------------------------------------------------------
// Name: CreatePresentationDescriptor
// Description: Creates the default presentation descriptor.
//-------------------------------------------------------------------

HRESULT CMFSource::CreatePresentationDescriptor()
{
    HRESULT hr = S_OK;

    IMFStreamDescriptor *pStreamDescriptor = NULL;
    IMFMediaTypeHandler *pHandler = NULL;

    // Create the stream descriptor.
    hr = MFCreateStreamDescriptor(
        0,          // stream identifier
        1,          // Number of media types.
        &m_pMediaType, // Array of media types
        &pStreamDescriptor
        );

    // Set the default media type on the media type handler.
    if (SUCCEEDED(hr))
    {
        hr = pStreamDescriptor->GetMediaTypeHandler(&pHandler);
    }
	
    if (SUCCEEDED(hr))
    {
       hr = pHandler->SetCurrentMediaType(m_pMediaType);
    }

    // Create the presentation descriptor.
    if (SUCCEEDED(hr))
    {
        hr = MFCreatePresentationDescriptor(
            1,                      // Number of stream descriptors
            &pStreamDescriptor,     // Array of stream descriptors
            &m_pPresentationDescriptor
            );
    }
    // Select the first stream
    if (SUCCEEDED(hr))
    {
        hr = m_pPresentationDescriptor->SelectStream(0);
    }

    // Set the file/stream duration as an attribute on the presentation descriptor.
    if (SUCCEEDED(hr))
    {
		hr = m_pPresentationDescriptor->SetUINT64(MF_PD_DURATION, (UINT64)ULLONG_MAX);
    }
    
    SafeRelease(&pStreamDescriptor);
    SafeRelease(&pHandler);
    return hr;
}



//-------------------------------------------------------------------
// Name: ValidatePresentationDescriptor
// Description: Validates the caller's presentation descriptor.
//
// This method is called when Start() is called with a non-NULL
// presentation descriptor. The caller is supposed to give us back
// the same PD that we gave out in CreatePresentationDescriptor().
// This method performs a sanity check on the caller's PD to make
// sure it matches ours.
//
// Note: Because this media source has one stream with single, fixed
//       media type, there is not much for the caller to decide. In
//       a more complicated source, the caller might select different
//       streams, or select from a list of media types.
//-------------------------------------------------------------------

HRESULT CMFSource::ValidatePresentationDescriptor(IMFPresentationDescriptor *pPD)
{
    HRESULT hr;

    assert(pPD != NULL);

    IMFStreamDescriptor *pStreamDescriptor = NULL;
    IMFMediaTypeHandler *pHandler = NULL;
    IMFMediaType        *pMediaType = NULL;
	GUID				majorType;

    DWORD   cStreamDescriptors = 0;
    BOOL    fSelected = FALSE;

    // Make sure there is only one stream.
    hr = pPD->GetStreamDescriptorCount(&cStreamDescriptors);

    if (SUCCEEDED(hr))
    {
        if (cStreamDescriptors != 1)
        {
            hr = MF_E_UNSUPPORTED_REPRESENTATION;
        }
    }

    // Get the stream descriptor.
    if (SUCCEEDED(hr))
    {
        hr = pPD->GetStreamDescriptorByIndex(0, &fSelected, &pStreamDescriptor);
    }

    // Make sure it's selected. (This media source has only one stream, so it
    // is not useful to deselect the only stream.)
    if (SUCCEEDED(hr))
    {
        if (!fSelected)
        {
            hr = MF_E_UNSUPPORTED_REPRESENTATION;
        }
    }

    // Get the media type handler, so that we can get the media type.
    if (SUCCEEDED(hr))
    {
        hr = pStreamDescriptor->GetMediaTypeHandler(&pHandler);
    }

    if (SUCCEEDED(hr))
    {
        hr = pHandler->GetCurrentMediaType(&pMediaType);
    }

	hr = pMediaType->GetMajorType(&majorType);

	if (SUCCEEDED(hr))
	{
		if(majorType == MFMediaType_Video)
		{
			if (SUCCEEDED(hr))
			{
				hr = MFUtils::ValidateVideoFormat(pMediaType);
			}
		}
		else
		{
			WAVEFORMATEX        *pFormat = NULL;
			UINT32  cbWaveFormat = 0;

			if (SUCCEEDED(hr))
			{
				hr = MFCreateWaveFormatExFromMFMediaType(
					pMediaType,
					&pFormat,
					&cbWaveFormat);
			}
			if (SUCCEEDED(hr))
			{
				/*assert(this->WaveFormat() != NULL);

				if (cbWaveFormat < this->WaveFormatSize())
				{
					hr = MF_E_INVALIDMEDIATYPE;
				}*/
			}

			if (SUCCEEDED(hr))
			{
				/*if (memcmp(pFormat, WaveFormat(), WaveFormatSize()) != 0)
				{
					hr = MF_E_INVALIDMEDIATYPE;
				}*/
			}

			CoTaskMemFree(pFormat);
		}
	}

    SafeRelease(&pStreamDescriptor);
    SafeRelease(&pHandler);
    SafeRelease(&pMediaType);

    return hr;
}


//-------------------------------------------------------------------
// Name: QueueNewStreamEvent
// Description:
// Queues an MENewStream or MEUpdatedStream event during Start.
//
// pPD: The presentation descriptor.
//
// Precondition: The presentation descriptor is assumed to be valid.
// Call ValidatePresentationDescriptor before calling this method.
//-------------------------------------------------------------------

HRESULT CMFSource::QueueNewStreamEvent(IMFPresentationDescriptor *pPD)
{
    assert(pPD != NULL);

    HRESULT hr = S_OK;
    IMFStreamDescriptor *pSD = NULL;

    BOOL fSelected = FALSE;

    hr = pPD->GetStreamDescriptorByIndex(0, &fSelected, &pSD);

    if (SUCCEEDED(hr))
    {
        // The stream must be selected, because we don't allow the app
        // to de-select the stream. See ValidatePresentationDescriptor.
        assert(fSelected);

        if (m_pStream)
        {
            // The stream already exists, and is still selected.
            // Send the MEUpdatedStream event.
            hr = QueueEventWithIUnknown(this, MEUpdatedStream, S_OK, m_pStream);
        }
        else
        {
            // The stream does not exist, and is now selected.
            // Create a new stream.

            hr = CreateCMFStreamSource(pSD);

            if (SUCCEEDED(hr))
            {
                // CreateCMFStreamSource creates the stream, so m_pStream is no longer NULL.
                assert(m_pStream != NULL);

                // Send the MENewStream event.
                hr = QueueEventWithIUnknown(this, MENewStream, S_OK, m_pStream);
            }
        }
    }

    SafeRelease(&pSD);
    return hr;
}

//-------------------------------------------------------------------
// Name: CreateCMFStreamSource
// Description: Creates the source's media stream object.
//-------------------------------------------------------------------

HRESULT CMFSource::CreateCMFStreamSource(IMFStreamDescriptor *pSD)
{
    HRESULT hr = S_OK;
    m_pStream = new (std::nothrow) CMFStreamSource(this, pSD, hr);

    if (m_pStream == NULL)
    {
        hr = E_OUTOFMEMORY;
    }

    if (FAILED(hr))
    {
        SafeRelease(&m_pStream);
    }

    return hr;
}



//-------------------------------------------------------------------
// Name: GetCurrentPosition
// Description: Returns the current playback position.
//-------------------------------------------------------------------

LONGLONG CMFSource::GetCurrentPosition() const
{
    if (m_pStream)
    {
        return m_pStream->GetCurrentPosition();
    }
    else
    {
        // If no stream is selected, we are at time 0 by definition.
        return 0;
    }
}



////////// AUDIO STREAM

//-------------------------------------------------------------------
// CMFStreamSource constructor.
//
// pSource: Parent media source.
// pSD: Stream descriptor that describes this stream.
// hr: If the constructor fails, this value is set to a failure code.
//-------------------------------------------------------------------


CMFStreamSource::CMFStreamSource(CMFSource *pSource,  IMFStreamDescriptor *pSD, HRESULT& hr) :
    m_nRefCount(1),
    m_pEventQueue(NULL),
    m_IsShutdown(FALSE),
    m_rtCurrentPosition(0),
	m_rtDuration(0),
    m_discontinuity(FALSE),
    m_EOS(FALSE),
	m_pMediaBuffer(NULL),
	m_nBufferSize(0)
{
    m_pSource = pSource;
    m_pSource->AddRef();

    m_pStreamDescriptor = pSD;
    m_pStreamDescriptor->AddRef();

    // Create the media event queue.
    CHECK_HR(hr = MFCreateEventQueue(&m_pEventQueue));

	//CHECK_HR(hr = InitializeParams());
	
    InitializeCriticalSection(&m_critSec);

bail:
	return;
}


//-------------------------------------------------------------------
// CMFStreamSource destructor.
//-------------------------------------------------------------------

CMFStreamSource::~CMFStreamSource()
{
    assert(m_IsShutdown);
    assert(m_nRefCount == 0);

	SafeRelease(&m_pMediaBuffer);

    DeleteCriticalSection(&m_critSec);
}


// IMFCustomSource methods

HRESULT CMFStreamSource::CopyVideoBuffer(UINT32 nWidth, UINT32 nHeight, const void* pBufferPtr, UINT32 nBufferSize)
{
	// Buffer pointer and size validity already checked by source (caller)
	if(m_guidMajorType != MFMediaType_Video)
	{
		TSK_DEBUG_ERROR("Calling CopyVideoBuffer on no-video stream");
#if defined(E_ILLEGAL_METHOD_CALL)
		return E_ILLEGAL_METHOD_CALL;
#else
		return _HRESULT_TYPEDEF_(0x8000000EL);
#endif
	}
	if(nWidth != m_structVideoParams.nWidth || nHeight != m_structVideoParams.nHeigh || nBufferSize != m_nBufferSize)
	{
		TSK_DEBUG_ERROR("Invalid argument %u#%u or %u#%u or %u#%u. If the call is from a video consumer then, you can safely ignore this message.", nWidth, m_structVideoParams.nWidth, nHeight, m_structVideoParams.nHeigh, nBufferSize, m_nBufferSize);
#if defined(E_BOUNDS)
		return E_BOUNDS;
#else
		return _HRESULT_TYPEDEF_(0x8000000BL);
#endif
	}

	HRESULT hr = S_OK;
	
	BYTE* pMediaBufferPtr = NULL;
	DWORD cbMaxLength = nBufferSize, cbCurrentLength = nBufferSize;
	CHECK_HR(hr = m_pMediaBuffer->Lock(&pMediaBufferPtr, &cbMaxLength, &cbCurrentLength));
	
	memcpy(pMediaBufferPtr, pBufferPtr, nBufferSize);
	CHECK_HR(hr = m_pMediaBuffer->SetCurrentLength(nBufferSize));
	CHECK_HR(hr = m_pMediaBuffer->Unlock());
	
bail:
	return hr;
}

// IUnknown methods

ULONG CMFStreamSource::AddRef()
{
    return InterlockedIncrement(&m_nRefCount);
}

ULONG  CMFStreamSource::Release()
{
    ULONG uCount = InterlockedDecrement(&m_nRefCount);
    if (uCount == 0)
    {
        delete this;
    }
    // For thread safety, return a temporary variable.
    return uCount;
}

HRESULT CMFStreamSource::QueryInterface(REFIID iid, void** ppv)
{
    static const QITAB qit[] =
    {
        QITABENT(CMFStreamSource, IMFMediaEventGenerator),
        QITABENT(CMFStreamSource, IMFMediaStream),
        { 0 }
    };
    return QISearch(this, qit, iid, ppv);
}


// IMFMediaEventGenerator methods
// [See note for CMFSource class]

HRESULT CMFStreamSource::BeginGetEvent(IMFAsyncCallback* pCallback, IUnknown* punkState)
{
    HRESULT hr = S_OK;

    EnterCriticalSection(&m_critSec);

    hr = CheckShutdown();
    if (SUCCEEDED(hr))
    {
        hr = m_pEventQueue->BeginGetEvent(pCallback, punkState);
    }

    LeaveCriticalSection(&m_critSec);
    return hr;
}

HRESULT CMFStreamSource::EndGetEvent(IMFAsyncResult* pResult, IMFMediaEvent** ppEvent)
{
    HRESULT hr = S_OK;

    EnterCriticalSection(&m_critSec);

    hr = CheckShutdown();
    if (SUCCEEDED(hr))
    {
        hr = m_pEventQueue->EndGetEvent(pResult, ppEvent);
    }

    LeaveCriticalSection(&m_critSec);
    return hr;
}

HRESULT CMFStreamSource::GetEvent(DWORD dwFlags, IMFMediaEvent** ppEvent)
{
    HRESULT hr = S_OK;

    IMFMediaEventQueue *pQueue = NULL;

    EnterCriticalSection(&m_critSec);

    hr = CheckShutdown();

    if (SUCCEEDED(hr))
    {
        pQueue = m_pEventQueue;
        pQueue->AddRef();
    }

    LeaveCriticalSection(&m_critSec);

    if (SUCCEEDED(hr))
    {
        hr = pQueue->GetEvent(dwFlags, ppEvent);
    }

    SafeRelease(&pQueue);
    return hr;
}

HRESULT CMFStreamSource::QueueEvent(MediaEventType met, REFGUID guidExtendedType, HRESULT hrStatus, const PROPVARIANT* pvValue)
{
    HRESULT hr = S_OK;

    EnterCriticalSection(&m_critSec);

    hr = CheckShutdown();
    if (SUCCEEDED(hr))
    {
        hr = m_pEventQueue->QueueEventParamVar(met, guidExtendedType, hrStatus, pvValue);
    }

    LeaveCriticalSection(&m_critSec);
    return hr;
}


// IMFMediaStream methods.


//-------------------------------------------------------------------
// Name: GetMediaSource
// Description: Returns a pointer to the media source.
//-------------------------------------------------------------------

HRESULT CMFStreamSource::GetMediaSource(IMFMediaSource** ppMediaSource)
{
    if (ppMediaSource == NULL)
    {
        return E_POINTER;
    }

    EnterCriticalSection(&m_critSec);

    HRESULT hr = S_OK;

    // If called after shutdown, them m_pSource is NULL.
    // Otherwise, m_pSource should not be NULL.

    hr = CheckShutdown();

    if (SUCCEEDED(hr))
    {
        if (m_pSource == NULL)
        {
            hr = E_UNEXPECTED;
        }
    }

    if (SUCCEEDED(hr))
    {
        hr = m_pSource->QueryInterface(IID_PPV_ARGS(ppMediaSource));
    }

    LeaveCriticalSection(&m_critSec);
    return hr;
}


//-------------------------------------------------------------------
// Name: GetStreamDescriptor
// Description: Returns the stream descriptor for this stream.
//-------------------------------------------------------------------

HRESULT CMFStreamSource::GetStreamDescriptor(IMFStreamDescriptor** ppStreamDescriptor)
{
    if (ppStreamDescriptor == NULL)
    {
        return E_POINTER;
    }

    if (m_pStreamDescriptor == NULL)
    {
        return E_UNEXPECTED;
    }

    EnterCriticalSection(&m_critSec);

    HRESULT hr = S_OK;

    hr = CheckShutdown();

    if (SUCCEEDED(hr))
    {
        *ppStreamDescriptor = m_pStreamDescriptor;
        (*ppStreamDescriptor)->AddRef();
    }

    LeaveCriticalSection(&m_critSec);

    return hr;
}



//-------------------------------------------------------------------
// Name: RequestSample
// Description: Requests a new sample.
//
// pToken: Token object. Can be NULL.
//-------------------------------------------------------------------

HRESULT CMFStreamSource::RequestSample(IUnknown* pToken)
{
    if (m_pSource == NULL)
    {
        return E_UNEXPECTED;
    }

    HRESULT hr = S_OK;

    IMFMediaSource *pSource = NULL;
    IMFSample *pSample = NULL;  // Sample to deliver.

    EnterCriticalSection(&m_critSec);

    // Check if we are shut down.
    hr = CheckShutdown();

    // Check if we already reached the end of the stream.
    if (SUCCEEDED(hr))
    {
        if (m_EOS)
        {
            hr = MF_E_END_OF_STREAM;
        }
    }

    // Check the source is stopped.
    // GetState does not hold the source's critical section. Safe to call.
    if (SUCCEEDED(hr))
    {
        if (m_pSource->GetState() == CMFSource::STATE_STOPPED)
        {
            hr = MF_E_INVALIDREQUEST;
        }
    }

    if (SUCCEEDED(hr))
    {
        // Create a new audio sample.
        hr = CreateSample(&pSample);
    }

    if (SUCCEEDED(hr))
    {
        // If the caller provided a token, attach it to the sample as
        // an attribute.

        // NOTE: If we processed sample requests asynchronously, we would
        // need to call AddRef on the token and put the token onto a FIFO
        // queue. See documenation for IMFMediaStream::RequestSample.
        if (pToken && pSample)
        {
            hr = pSample->SetUnknown(MFSampleExtension_Token, pToken);
        }
    }

    // If paused, queue the sample for later delivery. Otherwise, deliver the sample now.
    if (SUCCEEDED(hr) && pSample)
    {
        if (m_pSource->GetState() == CMFSource::STATE_PAUSED)
        {
            hr = m_sampleQueue.Queue(pSample);
        }
        else
        {
            hr = DeliverSample(pSample);
        }
    }

    // Cache a pointer to the source, prior to leaving the critical section.
    if (SUCCEEDED(hr))
    {
        pSource = m_pSource;
        pSource->AddRef();
    }

    LeaveCriticalSection(&m_critSec);


    // We only have one stream, so the end of the stream is also the end of the
    // presentation. Therefore, when we reach the end of the stream, we need to
    // queue the end-of-presentation event from the source. Logically we would do
    // this inside the CheckEndOfStream method. However, we cannot hold the
    // source's critical section while holding the stream's critical section, at
    // risk of deadlock.

    if (SUCCEEDED(hr))
    {
        if (m_EOS)
        {
            hr = pSource->QueueEvent(MEEndOfPresentation, GUID_NULL, S_OK, NULL);
        }
    }

    SafeRelease(&pSample);
    SafeRelease(&pSource);
    return hr;
}


///// Private CMFStreamSource methods

HRESULT CMFStreamSource::InitializeParams()
{
	HRESULT hr = S_OK;

	IMFMediaTypeHandler *pMediaTypeHandler = NULL;
	IMFMediaType* pMediaType = NULL;

	CHECK_HR(hr = m_pStreamDescriptor->GetMediaTypeHandler(&pMediaTypeHandler));
	CHECK_HR(hr = pMediaTypeHandler->GetCurrentMediaType(&pMediaType));

	GUID majorType, subType;
	pMediaType->GetMajorType(&majorType);
	if(majorType == MFMediaType_Video)
	{
		memset(&m_structVideoParams, 0, sizeof(m_structVideoParams));
		CHECK_HR(hr = MFGetAttributeSize(pMediaType, MF_MT_FRAME_SIZE, &m_structVideoParams.nWidth, &m_structVideoParams.nHeigh));
		CHECK_HR(hr = pMediaType->GetGUID(MF_MT_SUBTYPE, &subType));

		m_guidMajorType = MFMediaType_Video;
		m_guidSubType = subType;

		// Guess video size
		UINT32 nBufferSize;
		if(subType == MFVideoFormat_RGB32)
		{
			nBufferSize = (m_structVideoParams.nWidth * m_structVideoParams.nHeigh << 2);
		}
		else if(subType == MFVideoFormat_RGB24)
		{
			nBufferSize = (m_structVideoParams.nWidth * m_structVideoParams.nHeigh << 2);
		}
		else if(subType == MFVideoFormat_NV12 || subType == MFVideoFormat_I420)
		{
			nBufferSize = (m_structVideoParams.nWidth * m_structVideoParams.nHeigh * 3) >> 1;
		}
		else
		{
			TSK_DEBUG_ERROR("Video subType not supported");
			CHECK_HR(hr = E_NOTIMPL);
		}

		// Allocate media buffer
		SafeRelease(&m_pMediaBuffer);
		CHECK_HR(hr = MFCreateMemoryBuffer(nBufferSize, &m_pMediaBuffer));
		m_nBufferSize = nBufferSize;
		{
			//FIXME: DeliverSample() stops if no data
			BYTE* pBuffer = NULL;
			CHECK_HR(hr = m_pMediaBuffer->Lock(&pBuffer, NULL, NULL));
			memset(pBuffer, 0, nBufferSize);
			CHECK_HR(hr = m_pMediaBuffer->SetCurrentLength(nBufferSize));
			CHECK_HR(hr = m_pMediaBuffer->Unlock());
		}
		
		// Retrieve video Frame rate
		UINT32 unNumerator, unDenominator;
		CHECK_HR(hr = MFGetAttributeRatio(pMediaType, MF_MT_FRAME_RATE, &unNumerator, &unDenominator));
		m_structVideoParams.nFps = (unNumerator / unDenominator);
		
		// Retrieve sample duration based on framerate
		m_rtCurrentPosition = 0;
		CHECK_HR(hr = MFFrameRateToAverageTimePerFrame(m_structVideoParams.nFps, 1, &m_rtDuration));
	}
	else
	{
		TSK_DEBUG_ERROR("Only video media type is supported");
		CHECK_HR(hr = E_NOTIMPL);
	}

bail:
	SafeRelease(&pMediaTypeHandler);
	SafeRelease(&pMediaType);

	return hr;
}

// NOTE: Some of these methods hold the stream's critical section
// because they are called by the media source object.

//-------------------------------------------------------------------
// Name: CreateSample
// Description: Creates a new audio/video sample.
//-------------------------------------------------------------------

HRESULT CMFStreamSource::CreateSample(IMFSample **ppSample)
{
	*ppSample = NULL;

    HRESULT hr = S_OK;

    IMFSample *pSample = NULL;
	DWORD nCurrentLength = 0;

	CHECK_HR(hr = m_pMediaBuffer->GetCurrentLength(&nCurrentLength));

	if(nCurrentLength > 0)
	{
		CHECK_HR(hr = MFCreateSample(&pSample));
		CHECK_HR(hr = pSample->SetSampleTime(m_rtCurrentPosition));
		CHECK_HR(hr = pSample->SetSampleDuration(m_rtDuration));
		m_rtCurrentPosition += m_rtDuration;
		CHECK_HR(hr = pSample->AddBuffer(m_pMediaBuffer));

		if((*ppSample = pSample))
		{
			(*ppSample)->AddRef();
		}
	}
	
bail:
    SafeRelease(&pSample);
    return hr;
}

//-------------------------------------------------------------------
// Name: DeliverSample
// Description: Delivers a sample by sending an MEMediaSample event.
//-------------------------------------------------------------------
HRESULT CMFStreamSource::DeliverSample(IMFSample *pSample)
{
    HRESULT hr = S_OK;

	if(pSample)
	{
		// Send the MEMediaSample event with the new sample.
		hr = QueueEventWithIUnknown(this, MEMediaSample, hr, pSample);
	}

    // See if we reached the end of the stream.
    if (SUCCEEDED(hr))
    {
        hr = CheckEndOfStream();    // This method sends MEEndOfStream if needed.
    }

    return hr;
}

//-------------------------------------------------------------------
// Name: DeliverQueuedSamples
// Description: Delivers any samples waiting in the queue.
//
// Note: If the client requests a sample while the source is paused,
// the sample is queued and delivered on the next non-seeking call
// to Start(). The queue is flushed if the source is seeked or
// stopped.
//-------------------------------------------------------------------

HRESULT CMFStreamSource::DeliverQueuedSamples()
{
    HRESULT hr = S_OK;
    IMFSample *pSample = NULL;

    EnterCriticalSection(&m_critSec);

    // If we already reached the end of the stream, send the MEEndStream
    // event again.
    if (m_EOS)
    {
        hr = QueueEvent(MEEndOfStream, GUID_NULL, S_OK, NULL);
    }

    if (SUCCEEDED(hr))
    {
        // Deliver any queued samples.
        while (!m_sampleQueue.IsEmpty())
        {
            hr = m_sampleQueue.Dequeue(&pSample);
            if (FAILED(hr))
            {
                break;
            }

            hr = DeliverSample(pSample);
            if (FAILED(hr))
            {
                break;
            }

            SafeRelease(&pSample);
        }
    }

    LeaveCriticalSection(&m_critSec);

    // If we reached the end of the stream, send the end-of-presentation event from
    // the media source.
    if (SUCCEEDED(hr))
    {
        if (m_EOS)
        {
            hr = m_pSource->QueueEvent(MEEndOfPresentation, GUID_NULL, S_OK, NULL);
        }
    }

    SafeRelease(&pSample);
    return hr;
}


//-------------------------------------------------------------------
// Name: Flush
// Description: Flushes the sample queue.
//-------------------------------------------------------------------

HRESULT CMFStreamSource::Flush()
{
    EnterCriticalSection(&m_critSec);

    m_sampleQueue.Clear();

    LeaveCriticalSection(&m_critSec);
    return S_OK;
}


//-------------------------------------------------------------------
// Name: Shutdown
// Description: Notifies the stream that the source was shut down.
//-------------------------------------------------------------------

HRESULT CMFStreamSource::Shutdown()
{
    EnterCriticalSection(&m_critSec);

    // Flush queued samples.
    Flush();

    // Shut down the event queue.
    if (m_pEventQueue)
    {
        m_pEventQueue->Shutdown();
    }

    SafeRelease(&m_pEventQueue);
    SafeRelease(&m_pSource);
    SafeRelease(&m_pStreamDescriptor);

    m_IsShutdown = TRUE;

    LeaveCriticalSection(&m_critSec);
    return S_OK;
}

//-------------------------------------------------------------------
// Name: SetPosition
// Description: Updates the new stream position.
//-------------------------------------------------------------------

HRESULT CMFStreamSource::SetPosition(LONGLONG rtNewPosition)
{
    EnterCriticalSection(&m_critSec);

	HRESULT hr = S_OK;

/*
    // Check if the requested position is beyond the end of the stream.
    LONGLONG duration = AudioDurationFromBufferSize(m_pRiff->Format(), m_pRiff->Chunk().DataSize());

    if (rtNewPosition > duration)
    {
        LeaveCriticalSection(&m_critSec);

        return MF_E_INVALIDREQUEST; // Start position is past the end of the presentation.
    }

    if (m_rtCurrentPosition != rtNewPosition)
    {
        LONGLONG offset = BufferSizeFromAudioDuration(m_pRiff->Format(), rtNewPosition);

        // The chunk size is a DWORD. So if our calculations are correct, there is no
        // way that the maximum valid seek position can be larger than a DWORD.
        assert(offset <= MAXDWORD);

        hr = m_pRiff->MoveToChunkOffset((DWORD)offset);

        if (SUCCEEDED(hr))
        {
            m_rtCurrentPosition = rtNewPosition;
            m_discontinuity = TRUE;
            m_EOS = FALSE;
        }
    }
*/
    LeaveCriticalSection(&m_critSec);
    return hr;
}

HRESULT CMFStreamSource::CheckEndOfStream()
{
    HRESULT hr = S_OK;
/*
    if (m_pRiff->BytesRemainingInChunk() < m_pRiff->Format()->nBlockAlign)
    {
        // The remaining data is smaller than the audio block size. (In theory there shouldn't be
        // partial bits of data at the end, so we should reach an even zero bytes, but the file
        // might not be authored correctly.)
        m_EOS = TRUE;

        // Send the end-of-stream event,
        hr = QueueEvent(MEEndOfStream, GUID_NULL, S_OK, NULL);
    }
	*/
    return hr;
}




//-------------------------------------------------------------------
// Name: QueueEventWithIUnknown
// Description: Helper function to queue an event with an IUnknown
//              pointer value.
//
// pMEG:        Media event generator that will queue the event.
// meType:      Media event type.
// hrStatus:    Status code for the event.
// pUnk:        IUnknown pointer value.
//
//-------------------------------------------------------------------


HRESULT QueueEventWithIUnknown(
    IMFMediaEventGenerator *pMEG,
    MediaEventType meType,
    HRESULT hrStatus,
    IUnknown *pUnk)
{

    // Create the PROPVARIANT to hold the IUnknown value.
    PROPVARIANT var;
    var.vt = VT_UNKNOWN;
    var.punkVal = pUnk;
    pUnk->AddRef();

    // Queue the event.
    HRESULT hr = pMEG->QueueEvent(meType, GUID_NULL, hrStatus, &var);

    // Clear the PROPVARIANT.
    PropVariantClear(&var);

    return hr;
}

LONGLONG AudioDurationFromBufferSize(const WAVEFORMATEX *pWav, DWORD cbAudioDataSize)
{
    assert(pWav != NULL);

    if (pWav->nAvgBytesPerSec == 0)
    {
        return 0;
    }
    return (LONGLONG)cbAudioDataSize * 10000000 / pWav->nAvgBytesPerSec;
}

LONGLONG BufferSizeFromAudioDuration(const WAVEFORMATEX *pWav, LONGLONG duration)
{
    LONGLONG cbSize = duration * pWav->nAvgBytesPerSec / 10000000;

    ULONG ulRemainder = (ULONG)(cbSize % pWav->nBlockAlign);

    // Round up to the next block.
    if(ulRemainder)
    {
        cbSize += pWav->nBlockAlign - ulRemainder;
    }

    return cbSize;
}


