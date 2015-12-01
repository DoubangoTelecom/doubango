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
#include "plugin_audio_dsp_mediabuffer.h"

AudioDSPMediaBuffer::AudioDSPMediaBuffer(DWORD cbMaxLength, HRESULT& hr) :
    m_nRefCount(1),
    m_cbMaxLength(cbMaxLength),
    m_cbLength(0),
    m_pbData(NULL)
{
    m_pbData = new BYTE[cbMaxLength];
    if (!m_pbData) 
    {
        hr = E_OUTOFMEMORY;
    }
}

AudioDSPMediaBuffer::~AudioDSPMediaBuffer()
{
    if (m_pbData) 
    {
        delete [] m_pbData;
    }
}


// Function to create a new IMediaBuffer object and return 
// an AddRef'd interface pointer.
HRESULT AudioDSPMediaBuffer::Create(long cbMaxLen, IMediaBuffer **ppBuffer)
{
    HRESULT hr = S_OK;
    AudioDSPMediaBuffer *pBuffer = NULL;

    if (ppBuffer == NULL)
    {
        return E_POINTER;
    }

    pBuffer = new AudioDSPMediaBuffer(cbMaxLen, hr);

    if (pBuffer == NULL)
    {
        hr = E_OUTOFMEMORY;
    }

    if (SUCCEEDED(hr))
    {
       *ppBuffer = pBuffer;
       (*ppBuffer)->AddRef();
    }

    if (pBuffer)
    {
        pBuffer->Release();
    }
    return hr;
}

// IUnknown methods.
STDMETHODIMP AudioDSPMediaBuffer::QueryInterface(REFIID riid, void **ppv)
{
    if (ppv == NULL) 
    {
        return E_POINTER;
    }
    else if (riid == IID_IMediaBuffer || riid == IID_IUnknown) 
    {
        *ppv = static_cast<IMediaBuffer *>(this);
        AddRef();
        return S_OK;
    }
    else
    {
        *ppv = NULL;
        return E_NOINTERFACE;
    }
}

STDMETHODIMP_(ULONG) AudioDSPMediaBuffer::AddRef()
{
    return InterlockedIncrement(&m_nRefCount);
}

STDMETHODIMP_(ULONG) AudioDSPMediaBuffer::Release()
{
    LONG lRef = InterlockedDecrement(&m_nRefCount);
    if (lRef == 0) 
    {
        delete this;
        // m_cRef is no longer valid! Return lRef.
    }
    return lRef;  
}

// IMediaBuffer methods.
STDMETHODIMP AudioDSPMediaBuffer::SetLength(DWORD cbLength)
{
    if (cbLength > m_cbMaxLength) 
    {
        return E_INVALIDARG;
    }
    m_cbLength = cbLength;
    return S_OK;
}

STDMETHODIMP AudioDSPMediaBuffer::GetMaxLength(DWORD *pcbMaxLength)
{
    if (pcbMaxLength == NULL) 
    {
        return E_POINTER;
    }
    *pcbMaxLength = m_cbMaxLength;
    return S_OK;
}

STDMETHODIMP AudioDSPMediaBuffer::GetBufferAndLength(BYTE **ppbBuffer, DWORD *pcbLength)
{
    // Either parameter can be NULL, but not both.
    if (ppbBuffer == NULL && pcbLength == NULL) 
    {
        return E_POINTER;
    }
    if (ppbBuffer) 
    {
        *ppbBuffer = m_pbData;
    }
    if (pcbLength) 
    {
        *pcbLength = m_cbLength;
    }
    return S_OK;
}
