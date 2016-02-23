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
#ifndef PLUGIN_AUDIO_DSP_MEDIABUFFER_H
#define PLUGIN_AUDIO_DSP_MEDIABUFFER_H

#include "plugin_audio_dsp_config.h"

#include <dmo.h>

class AudioDSPMediaBuffer : public IMediaBuffer
{
private:
    DWORD        m_cbLength;
    const DWORD  m_cbMaxLength;
    LONG         m_nRefCount;  // Reference count
    BYTE         *m_pbData;

    AudioDSPMediaBuffer(DWORD cbMaxLength, HRESULT& hr);
    ~AudioDSPMediaBuffer();

public:
    static HRESULT Create(long cbMaxLen, IMediaBuffer **ppBuffer);

    // IUnknown methods.
    STDMETHODIMP QueryInterface(REFIID riid, void **ppv);
    STDMETHODIMP_(ULONG) AddRef();
    STDMETHODIMP_(ULONG) Release();

    // IMediaBuffer methods.
    STDMETHODIMP SetLength(DWORD cbLength);
    STDMETHODIMP GetMaxLength(DWORD *pcbMaxLength);
    STDMETHODIMP GetBufferAndLength(BYTE **ppbBuffer, DWORD *pcbLength);
};

#endif /* PLUGIN_AUDIO_DSP_MEDIABUFFER_H */
