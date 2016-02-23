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
#include "mf_sample_grabber.h"

#include "tinymedia/tmedia_producer.h"

#include "tsk_memory.h"
#include "tsk_debug.h"

#include <assert.h>

// Create a new instance of the object.
HRESULT SampleGrabberCB::CreateInstance(const struct tmedia_producer_s* pcWrappedProducer, SampleGrabberCB **ppCB)
{
    assert(pcWrappedProducer);

    *ppCB = new (std::nothrow) SampleGrabberCB(pcWrappedProducer);

    if (ppCB == NULL) {
        return E_OUTOFMEMORY;
    }
    return S_OK;
}

STDMETHODIMP SampleGrabberCB::QueryInterface(REFIID riid, void** ppv)
{
    static const QITAB qit[] = {
        QITABENT(SampleGrabberCB, IMFSampleGrabberSinkCallback),
        QITABENT(SampleGrabberCB, IMFClockStateSink),
        { 0 }
    };
    return QISearch(this, qit, riid, ppv);
}

STDMETHODIMP_(ULONG) SampleGrabberCB::AddRef()
{
    return InterlockedIncrement(&m_cRef);
}

STDMETHODIMP_(ULONG) SampleGrabberCB::Release()
{
    ULONG cRef = InterlockedDecrement(&m_cRef);
    if (cRef == 0) {
        delete this;
    }
    return cRef;

}

// IMFClockStateSink methods.

// In these example, the IMFClockStateSink methods do not perform any actions.
// You can use these methods to track the state of the sample grabber sink.

STDMETHODIMP SampleGrabberCB::OnClockStart(MFTIME hnsSystemTime, LONGLONG llClockStartOffset)
{
    TSK_DEBUG_INFO("SampleGrabberCB::OnClockStart(%lld, %lld)", hnsSystemTime, llClockStartOffset);
    return S_OK;
}

STDMETHODIMP SampleGrabberCB::OnClockStop(MFTIME hnsSystemTime)
{
    TSK_DEBUG_INFO("SampleGrabberCB::OnClockStop(%lld)", hnsSystemTime);
    return S_OK;
}

STDMETHODIMP SampleGrabberCB::OnClockPause(MFTIME hnsSystemTime)
{
    TSK_DEBUG_INFO("SampleGrabberCB::OnClockPause(%lld)", hnsSystemTime);
    return S_OK;
}

STDMETHODIMP SampleGrabberCB::OnClockRestart(MFTIME hnsSystemTime)
{
    TSK_DEBUG_INFO("SampleGrabberCB::OnClockRestart(%lld)", hnsSystemTime);
    return S_OK;
}

STDMETHODIMP SampleGrabberCB::OnClockSetRate(MFTIME hnsSystemTime, float flRate)
{
    TSK_DEBUG_INFO("SampleGrabberCB::OnClockSetRate(%lld, %f)", hnsSystemTime, flRate);
    return S_OK;
}

// IMFSampleGrabberSink methods.

STDMETHODIMP SampleGrabberCB::OnSetPresentationClock(IMFPresentationClock* pClock)
{
    TSK_DEBUG_INFO("SampleGrabberCB::OnSetPresentationClock");
    return S_OK;
}

STDMETHODIMP SampleGrabberCB::OnProcessSample(
    REFGUID guidMajorMediaType, DWORD dwSampleFlags,
    LONGLONG llSampleTime, LONGLONG llSampleDuration, const BYTE * pSampleBuffer,
    DWORD dwSampleSize)
{
    if (m_pWrappedProducer && TMEDIA_PRODUCER(m_pWrappedProducer)->enc_cb.callback) {
#if 1
        if (m_bMuted) {
            // Send zeros. Do not skip sending data to avoid NAT issues and session deconnection.
            // Some TelePresence systems disconnect the session when the remote peer stops sending video data.
            memset((void*)pSampleBuffer, 0, dwSampleSize);
        }
#endif
        TMEDIA_PRODUCER(m_pWrappedProducer)->enc_cb.callback(TMEDIA_PRODUCER(m_pWrappedProducer)->enc_cb.callback_data, pSampleBuffer, dwSampleSize);
    }

    return S_OK;
}

STDMETHODIMP SampleGrabberCB::OnShutdown()
{
    TSK_DEBUG_INFO("SampleGrabberCB::OnShutdown");
    return S_OK;
}