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
#ifndef PLUGIN_WIN_MF_SAMPLE_GRABBER_H
#define PLUGIN_WIN_MF_SAMPLE_GRABBER_H

#include "../plugin_win_mf_config.h"

#include <new>
#include <mfapi.h>
#include <mfidl.h>
#include <Mferror.h>
#include <shlwapi.h>

//
//      Sample Grabber callback [Declaration]
//      http://msdn.microsoft.com/en-us/library/windows/desktop/hh184779(v=vs.85).aspx
//
class SampleGrabberCB : public IMFSampleGrabberSinkCallback
{
    bool m_bMuted;
    long m_cRef;
    const struct tmedia_producer_s* m_pWrappedProducer;

    SampleGrabberCB(const struct tmedia_producer_s* pcWrappedProducer) : m_cRef(1), m_bMuted(false), m_pWrappedProducer(pcWrappedProducer) {}

public:
    static HRESULT CreateInstance(const struct tmedia_producer_s* pcWrappedProducer, SampleGrabberCB **ppCB);

    void SetMute(bool bMuted) {
        m_bMuted = bMuted;
    }

    // IUnknown methods
    STDMETHODIMP QueryInterface(REFIID iid, void** ppv);
    STDMETHODIMP_(ULONG) AddRef();
    STDMETHODIMP_(ULONG) Release();

    // IMFClockStateSink methods
    STDMETHODIMP OnClockStart(MFTIME hnsSystemTime, LONGLONG llClockStartOffset);
    STDMETHODIMP OnClockStop(MFTIME hnsSystemTime);
    STDMETHODIMP OnClockPause(MFTIME hnsSystemTime);
    STDMETHODIMP OnClockRestart(MFTIME hnsSystemTime);
    STDMETHODIMP OnClockSetRate(MFTIME hnsSystemTime, float flRate);

    // IMFSampleGrabberSinkCallback methods
    STDMETHODIMP OnSetPresentationClock(IMFPresentationClock* pClock);
    STDMETHODIMP OnProcessSample(REFGUID guidMajorMediaType, DWORD dwSampleFlags,
                                 LONGLONG llSampleTime, LONGLONG llSampleDuration, const BYTE * pSampleBuffer,
                                 DWORD dwSampleSize);
    STDMETHODIMP OnShutdown();
};


#endif /* PLUGIN_WIN_MF_SAMPLE_GRABBER_H */
