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
#ifndef PLUGIN_WIN_MF_CODEC_TOPOLOGY_H
#define PLUGIN_WIN_MF_CODEC_TOPOLOGY_H

#include "mf_codec.h"
#include "mf_custom_src.h"

#include "tsk_thread.h"

class MFCodecTopologySampleGrabberCB;

class MFCodecTopology : IUnknown
{
    friend class MFCodecTopologySampleGrabberCB;
public:
    MFCodecTopology(MFCodec* pCodec, HRESULT &hr);
    virtual ~MFCodecTopology();

    virtual HRESULT Initialize();
    virtual HRESULT DeInitialize();

    virtual HRESULT ProcessInput(IMFSample* pSample);
    virtual HRESULT ProcessOutput(IMFSample **ppSample);

    // IUnknown
    STDMETHODIMP QueryInterface(REFIID iid, void** ppv);
    STDMETHODIMP_(ULONG) AddRef();
    STDMETHODIMP_(ULONG) Release();

    inline BOOL isStarted() {
        return m_bStarted;
    }
    inline BOOL isInitialized() {
        return m_bInitialized;
    }

private:
    static void* TSK_STDCALL RunSessionThread(void *pArg);

protected:
    HRESULT Start();
    HRESULT Stop();

private:
    long m_nRefCount;

protected:
    BOOL m_bInitialized;
    BOOL m_bStarted;
    MFCodec* m_pCodec;
    CMFSource *m_pSource;
    IMFMediaSession *m_pSession;
    IMFTopology *m_pTopologyFull;
    IMFTopology *m_pTopologyPartial;
    IMFMediaType *m_pOutputType;
    IMFMediaType *m_pInputType;
    MFCodecTopologySampleGrabberCB *m_pGrabberCallback;
    IMFActivate *m_pGrabberActivate;
    tsk_thread_handle_t* m_pTread;
    SampleQueue m_SampleQueue;
};

class MFCodecVideoTopology : public MFCodecTopology
{
public:
    MFCodecVideoTopology(MFCodec* pCodec, HRESULT &hr);
    virtual ~MFCodecVideoTopology();

private:
    UINT32 m_nWidth, m_nHeight;
};


#endif /* PLUGIN_WIN_MF_CODEC_TOPOLOGY_H */
