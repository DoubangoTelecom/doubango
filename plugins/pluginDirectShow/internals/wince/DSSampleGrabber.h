/* Copyright (C) 2014-2015 Mamadou DIOP
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

#pragma once

#if defined(_WIN32_WCE)

#include "plugin_dshow_config.h"

#include <streams.h>
#include <math.h>

#include <internals/DSUtils.h>
#include <internals/wince/DSISampleGrabberCB.h>
#include "DSSampleGrabberUtils.h"

class DSSampleGrabber : public CTransInPlaceFilter
{
public:
    // instantiation
    DSSampleGrabber( TCHAR *tszName, LPUNKNOWN punk, HRESULT *phr );
    ~DSSampleGrabber(void);

public:
    HRESULT Transform(IMediaSample *pSample);
    HRESULT CheckInputType(const CMediaType* mtIn);

    HRESULT SetFps(int inputFps, int outputFps);

    // DECLARE_IUNKNOWN;
    STDMETHODIMP QueryInterface(REFIID riid, void **ppv) {
        return GetOwner()->QueryInterface(riid,ppv);
    };
    STDMETHODIMP_(ULONG) AddRef() {
        return InterlockedIncrement(&m_cRef);
    };
    STDMETHODIMP_(ULONG) Release() {
        return GetOwner()->Release();
    };

    STDMETHODIMP SetCallback(DSISampleGrabberCB* callback_);
    HRESULT SetSize(int width, int height);

    inline AM_MEDIA_TYPE GetMediaType() {
        return  (AM_MEDIA_TYPE)this->mt;
    }

private:
    int m_progress;
    int m_inputFps, m_outputFps;
    bool m_bProcessFrame;
    REFERENCE_TIME m_rtFrameLength;		// UNITS/fps
    LONGLONG m_iFrameNumber;

    DSISampleGrabberCB* callback;
    CMediaType mt;
    BYTE *m_rgb24;
};

#endif /* _WIN32_WCE */
