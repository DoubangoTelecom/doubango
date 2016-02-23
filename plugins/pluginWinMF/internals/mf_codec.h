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
#ifndef PLUGIN_WIN_MF_CODEC_H
#define PLUGIN_WIN_MF_CODEC_H

#include "../plugin_win_mf_config.h"

#include <new>
#include <mfapi.h>
#include <mfidl.h>
#include <Mferror.h>
#include <shlwapi.h>
#include <strmif.h>

class MFSampleQueue;

typedef enum MFCodecId_e {
    MFCodecId_H264Base,
    MFCodecId_H264Main,
    MFCodecId_AAC
}
MFCodecId_t;

typedef enum MFCodecType_e {
    MFCodecType_Encoder,
    MFCodecType_Decoder
}
MFCodecType_t;

typedef enum MFCodecMediaType_e {
    MFCodecMediaType_Audio,
    MFCodecMediaType_Video
}
MFCodecMediaType_t;

class MFCodec : IMFAsyncCallback
{
protected:
    MFCodec(MFCodecId_t eId, MFCodecType_t eType, IMFTransform *pMFT = NULL);
    virtual ~MFCodec();
    HRESULT ProcessInput(IMFSample* pSample);
    HRESULT ProcessOutput(IMFSample **ppSample);

public:
    virtual bool IsValid();
    virtual bool IsReady();
    virtual HRESULT Process(const void* pcInputPtr, UINT32 nInputSize, IMFSample **ppSampleOut);
    static enum tmedia_chroma_e GetUncompressedChroma();
    inline IMFTransform* GetMFT() {
        return m_pMFT;
    }
    inline MFCodecId_t GetId() {
        return m_eId;
    }
    inline MFCodecType_t GetType() {
        return m_eType;
    }
    inline void setBundled(BOOL bBundled) {
        m_bIsBundled = bBundled;
    }

    // IUnknown
    STDMETHODIMP QueryInterface(REFIID iid, void** ppv);
    STDMETHODIMP_(ULONG) AddRef();
    STDMETHODIMP_(ULONG) Release();

    // IMFAsyncCallback
    STDMETHODIMP GetParameters(DWORD *pdwFlags, DWORD *pdwQueue);
    STDMETHODIMP Invoke(IMFAsyncResult *pAsyncResult);

private:
    long				m_nRefCount;

protected:
    MFCodecId_t			m_eId;			// Codec Id
    MFCodecType_t		m_eType;		// Codec type.
    MFCodecMediaType_t	m_eMediaType;	// Codec Media type.
    DWORD				m_dwInputID;     // Input stream ID.
    DWORD				m_dwOutputID;    // Output stream ID.

    GUID				m_guidCompressedFormat;	// Compressed Media format (e.g. MFVideoFormat_H264)
    IMFTransform		*m_pMFT;         // Pointer to the encoder MFT.
    ICodecAPI			*m_pCodecAPI;	// Pointer to CodecAPI.
    IMFMediaType		*m_pOutputType;  // Output media type of the codec.
    IMFMediaType		*m_pInputType;  // Input media type of the codec.

    LONGLONG m_rtStart;
    UINT64 m_rtDuration;

    IMFSample *m_pSampleIn;
    IMFSample *m_pSampleOut;

    MFSampleQueue *m_pSampleQueueAsyncInput;
    BOOL m_bIsBundled; // Bundled with a producer or cosumer -> do not monitor events
    BOOL m_bIsAsync;
    IMFMediaEventGenerator  *m_pEventGenerator;
    BOOL m_bIsFirstFrame;
    long m_nMETransformNeedInputCount, m_nMETransformHaveOutputCount;
};


class MFCodecVideo : public MFCodec
{
    friend class MFCodec;
protected:
    MFCodecVideo(MFCodecId_t eId, MFCodecType_t eType, IMFTransform *pMFT = NULL);
    virtual ~MFCodecVideo();

public:
    virtual HRESULT Initialize(
        UINT32 nFrameRate,
        UINT32 nWidth,
        UINT32 nHeight,
        UINT32 nOutputBitRateInBps = 0 // Only for encoders
    );
    virtual HRESULT SetGOPSize(UINT32 nFramesCount);
    virtual HRESULT SetBitRate(UINT32 nBitRateInBps);
    virtual HRESULT SetSliceMaxSizeInBytes(UINT32 nSliceMaxSizeInBytes);
    virtual HRESULT RequestKeyFrame();

    virtual HRESULT IsSetSliceMaxSizeInBytesSupported(BOOL &supported);
    virtual inline UINT32 GetFrameRate() {
        return m_nFrameRate;
    }
    virtual inline UINT32 GetWidth() {
        return m_nWidth;
    }
    virtual inline UINT32 GetHeight() {
        return m_nHeight;
    }

protected:
    UINT32 m_nFrameRate;
    UINT32 m_nWidth;
    UINT32 m_nHeight;
};

class MFCodecVideoH264 : public MFCodecVideo
{
protected:
    MFCodecVideoH264(MFCodecId_t eId, MFCodecType_t eType, IMFTransform *pMFT = NULL);

public:
    virtual ~MFCodecVideoH264();
    static MFCodecVideoH264* CreateCodecH264Base(MFCodecType_t eType, IMFTransform *pMFT = NULL);
    static MFCodecVideoH264* CreateCodecH264Main(MFCodecType_t eType, IMFTransform *pMFT = NULL);

protected:

};


#endif /* PLUGIN_WIN_MF_CODEC_H */
