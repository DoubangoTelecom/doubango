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
#include "mf_codec.h"
#include "mf_utils.h"
#include "mf_sample_queue.h"

#include "tinymedia/tmedia_common.h"

#include "tsk_debug.h"

#include <KS.h>
#include <Codecapi.h>
#include <assert.h>
#include <initguid.h>

// NV12 is the only format supported by all HW encoders and decoders
#if !defined(kMFCodecUncompressedFormat)
#	define kMFCodecUncompressedFormat MFVideoFormat_NV12
#endif

// Max frames allowed in the queue
#if !defined(kMFCodecQueuedFramesMax)
#	define kMFCodecQueuedFramesMax (30 << 1)
#endif

// Make sure usable on Win7 SDK targeting Win8 OS
#if !defined(CODECAPI_AVLowLatencyMode)
DEFINE_GUID(CODECAPI_AVLowLatencyMode,
            0x9c27891a, 0xed7a, 0x40e1, 0x88, 0xe8, 0xb2, 0x27, 0x27, 0xa0, 0x24, 0xee);
#endif
#if !defined(CODECAPI_AVDecVideoH264ErrorConcealment)
DEFINE_GUID(CODECAPI_AVDecVideoH264ErrorConcealment,
            0xececace8, 0x3436, 0x462c, 0x92, 0x94, 0xcd, 0x7b, 0xac, 0xd7, 0x58, 0xa9);
#endif

//
//	MFCodec
//

MFCodec::MFCodec(MFCodecId_t eId, MFCodecType_t eType, IMFTransform *pMFT /*= NULL*/)
    : m_nRefCount(1)
    , m_eId(eId)
    , m_eType(eType)
    , m_pMFT(NULL)
    , m_pCodecAPI(NULL)
    , m_pOutputType(NULL)
    , m_pInputType(NULL)
    , m_dwInputID(0)
    , m_dwOutputID(0)
    , m_rtStart(0)
    , m_rtDuration(0)
    , m_pSampleIn(NULL)
    , m_pSampleOut(NULL)
    , m_pEventGenerator(NULL)
    , m_bIsAsync(FALSE)
    , m_bIsFirstFrame(TRUE)
    , m_bIsBundled(FALSE)
    , m_nMETransformNeedInputCount(0)
    , m_nMETransformHaveOutputCount(0)
    , m_pSampleQueueAsyncInput(NULL)
{
    MFUtils::Startup();

    HRESULT hr = S_OK;

    switch(eId) {
    case MFCodecId_H264Base:
    case MFCodecId_H264Main: {
        m_eMediaType = MFCodecMediaType_Video;
        m_guidCompressedFormat = MFVideoFormat_H264;
        break;
    }
    case MFCodecId_AAC: {
        m_eMediaType = MFCodecMediaType_Audio;
        m_guidCompressedFormat = MFAudioFormat_AAC;
        break;
    }
    default: {
        assert(false);
        break;
    }
    }
    CHECK_HR(hr = MFCreateMediaType(&m_pOutputType));
    CHECK_HR(hr = MFCreateMediaType(&m_pInputType));
    if(pMFT) { // up to the caller to make sure all parameters are corrrect
        m_pMFT = pMFT;
        m_pMFT->AddRef();
    }
    else {
        CHECK_HR(hr = MFUtils::GetBestCodec(
                          (m_eType == MFCodecType_Encoder) ? TRUE : FALSE, // Encoder ?
                          (m_eMediaType == MFCodecMediaType_Video) ? MFMediaType_Video : MFMediaType_Audio, // Media Type
                          (m_eType == MFCodecType_Encoder) ? kMFCodecUncompressedFormat : m_guidCompressedFormat/*GUID_NULL*/, // Input
                          (m_eType == MFCodecType_Encoder) ? m_guidCompressedFormat : kMFCodecUncompressedFormat, // Output
                          &m_pMFT));
    }
    hr = m_pMFT->QueryInterface(IID_PPV_ARGS(&m_pCodecAPI));
    if(FAILED(hr) && m_eType == MFCodecType_Encoder) { // Required only for Encoders
        CHECK_HR(hr);
    }


    CHECK_HR(hr = MFUtils::IsAsyncMFT(m_pMFT, &m_bIsAsync));
    if(m_bIsAsync) {
        m_pSampleQueueAsyncInput = new MFSampleQueue();
        if(!m_pSampleQueueAsyncInput) {
            CHECK_HR(hr = E_OUTOFMEMORY);
        }
        CHECK_HR(hr = MFUtils::UnlockAsyncMFT(m_pMFT));
        CHECK_HR(hr = m_pMFT->QueryInterface(IID_PPV_ARGS(&m_pEventGenerator)));
    }

bail:
    if(FAILED(hr)) {
        SafeRelease(&m_pMFT);
        SafeRelease(&m_pCodecAPI);
    }
    if(!IsValid()) {
        TSK_DEBUG_ERROR("Failed to create codec with id = %d", m_eId);
    }
}

MFCodec::~MFCodec()
{
    assert(m_nRefCount == 0);

    if(m_bIsAsync && m_pMFT) {
        m_pMFT->ProcessMessage(MFT_MESSAGE_NOTIFY_END_OF_STREAM, NULL);
        m_pMFT->ProcessMessage(MFT_MESSAGE_COMMAND_DRAIN, NULL);
    }

    SafeRelease(&m_pMFT);
    SafeRelease(&m_pCodecAPI);
    SafeRelease(&m_pOutputType);
    SafeRelease(&m_pInputType);
    SafeRelease(&m_pSampleIn);
    SafeRelease(&m_pSampleOut);
    SafeRelease(&m_pEventGenerator);
    SafeRelease(&m_pSampleQueueAsyncInput);
}

ULONG MFCodec::AddRef()
{
    return InterlockedIncrement(&m_nRefCount);
}

ULONG  MFCodec::Release()
{
    ULONG uCount = InterlockedDecrement(&m_nRefCount);
    if (uCount == 0) {
        delete this;
    }
    // For thread safety, return a temporary variable.
    return uCount;
}

HRESULT MFCodec::QueryInterface(REFIID iid, void** ppv)
{
    if(!IsValid()) {
        return E_FAIL;
    }
    return m_pMFT->QueryInterface(iid, ppv);
}

// IMFAsyncCallback
STDMETHODIMP MFCodec::GetParameters(DWORD *pdwFlags, DWORD *pdwQueue)
{
    return E_NOTIMPL;
}

STDMETHODIMP MFCodec::Invoke(IMFAsyncResult *pAsyncResult)
{
    HRESULT hr = S_OK, hrStatus = S_OK;
    IMFMediaEvent* pEvent = NULL;
    MediaEventType meType = MEUnknown;

    CHECK_HR(hr = m_pEventGenerator->EndGetEvent(pAsyncResult, &pEvent));
    CHECK_HR(hr = pEvent->GetType(&meType));
    CHECK_HR(hr = pEvent->GetStatus(&hrStatus));

    if (SUCCEEDED(hrStatus)) {
        switch(meType) {
        case METransformNeedInput: {
            InterlockedIncrement(&m_nMETransformNeedInputCount);
            break;
        }

        case METransformHaveOutput: {
            InterlockedIncrement(&m_nMETransformHaveOutputCount);
            break;
        }
        }
    }

    CHECK_HR(hr = m_pEventGenerator->BeginGetEvent(this, NULL));

bail:
    SafeRelease(&pEvent);
    return hr;
}

HRESULT MFCodec::ProcessInput(IMFSample* pSample)
{
    assert(IsReady());

    HRESULT hr = S_OK;

    if(m_bIsFirstFrame) {
        if(m_bIsAsync && !m_bIsBundled) {
            CHECK_HR(hr = m_pMFT->ProcessMessage(MFT_MESSAGE_NOTIFY_START_OF_STREAM, NULL));
            CHECK_HR(hr = m_pEventGenerator->BeginGetEvent(this, NULL));
        }
        m_bIsFirstFrame = FALSE;
    }

    if(m_bIsAsync) {
        if(m_nMETransformNeedInputCount == 1 && m_pSampleQueueAsyncInput->IsEmpty()) {
            InterlockedDecrement(&m_nMETransformNeedInputCount);
            return m_pMFT->ProcessInput(m_dwInputID, pSample, 0);
        }

        if(m_pSampleQueueAsyncInput->Count() > kMFCodecQueuedFramesMax) {
            m_pSampleQueueAsyncInput->Clear();
            CHECK_HR(hr = E_UNEXPECTED);
        }

        // Input sample holds shared memory (also used by other samples)
        IMFSample *pSampleCopy = NULL;
        IMFMediaBuffer *pMediaBuffer = NULL, *pMediaBufferCopy = NULL;
        BYTE *pBufferPtr = NULL, *pBufferPtrCopy = NULL;
        DWORD dwDataLength = 0;
        BOOL bMediaBufferLocked = FALSE, bMediaBufferLockedCopy = FALSE;

        CHECK_HR(hr = pSample->GetBufferByIndex(0, &pMediaBuffer));
        hr = pMediaBuffer->GetCurrentLength(&dwDataLength);
        if(FAILED(hr)) {
            goto endofcopy;
        }
        hr = pMediaBuffer->Lock(&pBufferPtr, NULL, NULL);
        if(FAILED(hr)) {
            goto endofcopy;
        }
        bMediaBufferLocked = TRUE;

        hr = MFUtils::CreateMediaSample(dwDataLength, &pSampleCopy);
        if(FAILED(hr)) {
            goto endofcopy;
        }
        hr = pSampleCopy->GetBufferByIndex(0, &pMediaBufferCopy);
        if(FAILED(hr)) {
            goto endofcopy;
        }
        hr = pMediaBufferCopy->Lock(&pBufferPtrCopy, NULL, NULL);
        if(FAILED(hr)) {
            goto endofcopy;
        }
        bMediaBufferLockedCopy = TRUE;

        memcpy(pBufferPtrCopy, pBufferPtr, dwDataLength);
        hr = pMediaBufferCopy->SetCurrentLength(dwDataLength);
        if(FAILED(hr)) {
            goto endofcopy;
        }

        LONGLONG hnsSampleTime = 0;
        LONGLONG hnsSampleDuration = 0;
        hr = pSample->GetSampleTime(&hnsSampleTime);
        if(SUCCEEDED(hr)) {
            hr = pSampleCopy->SetSampleTime(hnsSampleTime);
        }
        hr = pSample->GetSampleDuration(&hnsSampleDuration);
        if(SUCCEEDED(hr)) {
            hr = pSampleCopy->SetSampleDuration(hnsSampleDuration);
        }

        // EnQueue
        hr = m_pSampleQueueAsyncInput->Queue(pSampleCopy);
endofcopy:
        if(pMediaBuffer && bMediaBufferLocked) {
            pMediaBuffer->Unlock();
        }
        if(pMediaBufferCopy && bMediaBufferLockedCopy) {
            pMediaBufferCopy->Unlock();
        }
        SafeRelease(&pSampleCopy);
        SafeRelease(&pMediaBuffer);
        CHECK_HR(hr);

        while(m_nMETransformNeedInputCount > 0) {
            if(m_pSampleQueueAsyncInput->IsEmpty()) {
                break;
            }
            IMFSample *_pSample = NULL;
            hr = m_pSampleQueueAsyncInput->Dequeue(&_pSample);
            if(SUCCEEDED(hr)) {
                InterlockedDecrement(&m_nMETransformNeedInputCount);
                hr = m_pMFT->ProcessInput(m_dwInputID, _pSample, 0);
            }
            SafeRelease(&_pSample);
            CHECK_HR(hr);
        }
    }
    else {
        CHECK_HR(hr = m_pMFT->ProcessInput(m_dwInputID, pSample, 0));
    }

bail:
    return hr;
}

HRESULT MFCodec::ProcessOutput(IMFSample **ppSample)
{
    assert(IsReady());

    if(m_bIsAsync) {
        if(m_nMETransformHaveOutputCount == 0) {
            return S_OK;
        }
        InterlockedDecrement(&m_nMETransformHaveOutputCount);
    }

    *ppSample = NULL;

    IMFMediaBuffer* pBufferOut = NULL;

    DWORD dwStatus;

    HRESULT hr = S_OK;

    MFT_OUTPUT_STREAM_INFO mftStreamInfo = { 0 };
    MFT_OUTPUT_DATA_BUFFER mftOutputData = { 0 };

    CHECK_HR(hr = m_pMFT->GetOutputStreamInfo(m_dwOutputID, &mftStreamInfo));

    BOOL bOutputStreamProvidesSamples = (mftStreamInfo.dwFlags & MFT_OUTPUT_STREAM_PROVIDES_SAMPLES) == MFT_OUTPUT_STREAM_PROVIDES_SAMPLES;

    if(!bOutputStreamProvidesSamples) {
        if(!m_pSampleOut) {
            CHECK_HR(hr = MFUtils::CreateMediaSample(mftStreamInfo.cbSize, &m_pSampleOut));
            hr = m_pSampleOut->GetBufferByIndex(0, &pBufferOut);
            if(FAILED(hr)) {
                SafeRelease(&m_pSampleOut);
                CHECK_HR(hr);
            }
        }
        else {
            DWORD dwMaxLength = 0;
            CHECK_HR(hr = m_pSampleOut->GetBufferByIndex(0, &pBufferOut));
            CHECK_HR(hr = pBufferOut->GetMaxLength(&dwMaxLength));
            if(dwMaxLength < mftStreamInfo.cbSize) {
                CHECK_HR(hr = m_pSampleOut->RemoveAllBuffers());
                SafeRelease(&pBufferOut);
                CHECK_HR(hr = MFCreateMemoryBuffer(mftStreamInfo.cbSize, &pBufferOut));
                CHECK_HR(hr = m_pSampleOut->AddBuffer(pBufferOut));
            }
        }
    }

    if(pBufferOut) {
        CHECK_HR(hr = pBufferOut->SetCurrentLength(0));
    }

    //Set the output sample
    mftOutputData.pSample = bOutputStreamProvidesSamples ? NULL : m_pSampleOut;
    //Set the output id
    mftOutputData.dwStreamID = m_dwOutputID;

    //Generate the output sample
    hr = m_pMFT->ProcessOutput(0, 1, &mftOutputData, &dwStatus);
    if (hr == MF_E_TRANSFORM_NEED_MORE_INPUT) {
        hr = S_OK;
        goto bail;
    }

    if (FAILED(hr)) {
        goto bail;
    }

    *ppSample = mftOutputData.pSample;
    if(*ppSample) {
        (*ppSample)->AddRef();
    }

bail:
    if(bOutputStreamProvidesSamples) {
        SafeRelease(&mftOutputData.pSample);
    }
    SafeRelease(&pBufferOut);
    return hr;
}

bool MFCodec::IsValid()
{
    return (m_pMFT && (m_eType == MFCodecType_Decoder || m_pCodecAPI));
}

bool MFCodec::IsReady()
{
    return (IsValid() && m_pOutputType && m_pInputType);
}

HRESULT MFCodec::Process(const void* pcInputPtr, UINT32 nInputSize, IMFSample **ppSampleOut)
{
    if(!pcInputPtr || !nInputSize || !ppSampleOut) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return E_INVALIDARG;
    }

    *ppSampleOut = NULL;

    HRESULT hr = S_OK;

    IMFMediaBuffer* pBufferIn = NULL;
    BYTE* pBufferPtr = NULL;
    BOOL bMediaChangeHandled = FALSE; // Endless loop guard

    if(!m_pSampleIn) {
        CHECK_HR(hr = MFUtils::CreateMediaSample(nInputSize, &m_pSampleIn));
        hr = m_pSampleIn->GetBufferByIndex(0, &pBufferIn);
        if(FAILED(hr)) {
            SafeRelease(&m_pSampleIn);
            CHECK_HR(hr);
        }
    }
    else {
        DWORD dwMaxLength = 0;
        CHECK_HR(hr = m_pSampleIn->GetBufferByIndex(0, &pBufferIn));
        CHECK_HR(hr = pBufferIn->GetMaxLength(&dwMaxLength));
        if(dwMaxLength < nInputSize) {
            CHECK_HR(hr = m_pSampleIn->RemoveAllBuffers());
            SafeRelease(&pBufferIn);
            CHECK_HR(hr = MFCreateMemoryBuffer(nInputSize, &pBufferIn));
            CHECK_HR(hr = m_pSampleIn->AddBuffer(pBufferIn));
        }
    }

    CHECK_HR(hr = pBufferIn->Lock(&pBufferPtr, NULL, NULL));
    memcpy(pBufferPtr, pcInputPtr, nInputSize);
    CHECK_HR(hr = pBufferIn->Unlock());
    CHECK_HR(hr = pBufferIn->SetCurrentLength(nInputSize));

    if(m_eType == MFCodecType_Encoder) {
        CHECK_HR(hr = m_pSampleIn->SetSampleDuration(m_rtDuration));
        CHECK_HR(hr = m_pSampleIn->SetSampleTime(m_rtStart)); // FIXME: use clock(), Same for custom source
    }
Label_ProcessInput:
    hr = ProcessInput(m_pSampleIn);
    while(hr == MF_E_NOTACCEPTING) {
        TSK_DEBUG_INFO("MF_E_NOTACCEPTING");
        IMFSample* pSample = NULL;
        hr = ProcessOutput(&pSample);
        if(SUCCEEDED(hr) && pSample) {
            SafeRelease(ppSampleOut);
            *ppSampleOut = pSample, pSample = NULL;

            hr = m_pSampleIn->SetUINT32(MFSampleExtension_Discontinuity, TRUE);
            hr = ProcessInput(m_pSampleIn);
        }
    }
    if(!*ppSampleOut) {
        hr = ProcessOutput(ppSampleOut);
        if(hr == MF_E_TRANSFORM_STREAM_CHANGE) { /* Handling Stream Changes: http://msdn.microsoft.com/en-us/library/windows/desktop/ee663587(v=vs.85).aspx */
            TSK_DEBUG_INFO("[MF Codec] Stream changed");
            if(m_eType == MFCodecType_Decoder) {
                IMFMediaType *pTypeOut = NULL;
                hr = m_pMFT->GetOutputAvailableType(m_dwOutputID, 0, &pTypeOut);
                if(SUCCEEDED(hr)) {
                    UINT32 uWidth = 0, uHeight = 0;
                    hr = MFGetAttributeSize(pTypeOut, MF_MT_FRAME_SIZE, &uWidth, &uHeight);
                    if(SUCCEEDED(hr)) {
                        TSK_DEBUG_INFO("[MF Decoder] New size: width=%u, height=%u", uWidth, uHeight);
                        hr = m_pMFT->SetOutputType(m_dwOutputID, pTypeOut, 0);
                        if(SUCCEEDED(hr)) {
                            SafeRelease(&m_pOutputType);
                            pTypeOut->AddRef();
                            m_pOutputType = pTypeOut;
                            if(m_eMediaType == MFCodecMediaType_Video) {
                                dynamic_cast<MFCodecVideo*>(this)->m_nWidth = uWidth;
                                dynamic_cast<MFCodecVideo*>(this)->m_nHeight = uHeight;
                            }
                        }
                    }
                }
                SafeRelease(&pTypeOut);
                if(SUCCEEDED(hr)) {
                    if(!bMediaChangeHandled) {
                        bMediaChangeHandled = TRUE;
                        goto Label_ProcessInput;
                    }
                }
            }
        }
    }

    m_rtStart += m_rtDuration;

bail:
    SafeRelease(&pBufferIn);
    return hr;
}

enum tmedia_chroma_e MFCodec::GetUncompressedChroma()
{
    if(kMFCodecUncompressedFormat == MFVideoFormat_NV12) {
        return tmedia_chroma_nv12;
    }
    assert(false);
    return tmedia_chroma_none;
}

//
//	MFCodecVideo
//

MFCodecVideo::MFCodecVideo(MFCodecId_t eId, MFCodecType_t eType, IMFTransform *pMFT /*= NULL*/)
    : MFCodec(eId, eType, pMFT)
    , m_nFrameRate(0)
    , m_nWidth(0)
    , m_nHeight(0)
{
    assert(m_eMediaType == MFCodecMediaType_Video);
}

MFCodecVideo::~MFCodecVideo()
{

}

HRESULT MFCodecVideo::Initialize(
    UINT32 nFrameRate,
    UINT32 nWidth,
    UINT32 nHeight,
    UINT32 nOutputBitRateInBps /*= 0*/
)
{
    assert(IsValid());

    HRESULT hr = S_OK;

    VARIANT var = {0};

    // make sure identifiers are zero-based (other layouts not supported yet)
    hr = m_pMFT->GetStreamIDs(1, &m_dwInputID, 1, &m_dwOutputID);
    if (hr == E_NOTIMPL) {
        m_dwInputID = 0;
        m_dwOutputID = 0;
        hr = S_OK;
    }
    else if (FAILED(hr)) {
        TSK_DEBUG_ERROR("The stream identifiers are not zero-based");
        return hr;
    }

    m_rtStart = 0;
    CHECK_HR(hr = MFFrameRateToAverageTimePerFrame(nFrameRate, 1, &m_rtDuration));

    CHECK_HR(hr = m_pOutputType->SetGUID(MF_MT_MAJOR_TYPE, (m_eMediaType == MFCodecMediaType_Video) ? MFMediaType_Video : MFMediaType_Audio));
    CHECK_HR(hr = m_pInputType->SetGUID(MF_MT_MAJOR_TYPE, (m_eMediaType == MFCodecMediaType_Video) ? MFMediaType_Video : MFMediaType_Audio));

    CHECK_HR(hr = m_pOutputType->SetGUID(MF_MT_SUBTYPE, (m_eType == MFCodecType_Encoder) ? m_guidCompressedFormat : kMFCodecUncompressedFormat));
    CHECK_HR(hr = m_pInputType->SetGUID(MF_MT_SUBTYPE, (m_eType == MFCodecType_Encoder) ? kMFCodecUncompressedFormat : m_guidCompressedFormat));

    CHECK_HR(hr = m_pOutputType->SetUINT32(MF_MT_ALL_SAMPLES_INDEPENDENT, (m_eType == MFCodecType_Encoder) ? FALSE : TRUE));
    CHECK_HR(hr = m_pInputType->SetUINT32(MF_MT_ALL_SAMPLES_INDEPENDENT, (m_eType == MFCodecType_Encoder) ? TRUE : FALSE));

    CHECK_HR(hr = m_pOutputType->SetUINT32(MF_MT_FIXED_SIZE_SAMPLES, (m_eType == MFCodecType_Encoder) ? FALSE : TRUE));
    CHECK_HR(hr = m_pInputType->SetUINT32(MF_MT_FIXED_SIZE_SAMPLES, (m_eType == MFCodecType_Encoder) ? TRUE : FALSE));

    // Set bitrate
    // Set (MF_MT_AVG_BITRATE) for MediaType
    // Set (CODECAPI_AVEncCommonMeanBitRate) for H.264
    hr = SetBitRate(nOutputBitRateInBps);

    CHECK_HR(hr = m_pOutputType->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive));
    CHECK_HR(hr = m_pInputType->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive));

    CHECK_HR(hr = MFSetAttributeSize(m_pOutputType, MF_MT_FRAME_SIZE, nWidth, nHeight));
    CHECK_HR(hr = MFSetAttributeSize(m_pInputType, MF_MT_FRAME_SIZE, nWidth, nHeight));

    CHECK_HR(hr = MFSetAttributeRatio(m_pOutputType, MF_MT_FRAME_RATE, nFrameRate, 1));
    CHECK_HR(hr = MFSetAttributeRatio(m_pInputType, MF_MT_FRAME_RATE, nFrameRate, 1));

    CHECK_HR(hr = MFSetAttributeRatio(m_pOutputType, MF_MT_PIXEL_ASPECT_RATIO, 1, 1));
    CHECK_HR(hr = MFSetAttributeRatio(m_pInputType, MF_MT_PIXEL_ASPECT_RATIO, 1, 1));

    // Encoder: Output format must be set before input
    // Decoder: Input format must be set before output
    if(m_eType == MFCodecType_Encoder) {
        CHECK_HR(hr = m_pMFT->SetOutputType(m_dwOutputID, m_pOutputType, 0));
        CHECK_HR(hr = m_pMFT->SetInputType(m_dwInputID, m_pInputType, 0));
    }
    else {
        CHECK_HR(hr = m_pMFT->SetInputType(m_dwInputID, m_pInputType, 0));
        CHECK_HR(hr = m_pMFT->SetOutputType(m_dwOutputID, m_pOutputType, 0));
    }

    if(m_eId == MFCodecId_H264Base || m_eId == MFCodecId_H264Main) {
        if(m_eType == MFCodecType_Decoder) {
            // Only decoder support GetAttributes()
            IMFAttributes* pAttributes = NULL;
            hr = m_pMFT->GetAttributes(&pAttributes);
            if(SUCCEEDED(hr)) {
                // FIXME: Very strange that "CODECAPI_AVLowLatencyMode" only works with "IMFAttributes->" and not "ICodecAPI->SetValue()"
                hr = pAttributes->SetUINT32(CODECAPI_AVLowLatencyMode, TRUE);
            }
            SafeRelease(&pAttributes);
        }
        else {
            var.vt = VT_BOOL;
            var.boolVal = VARIANT_TRUE;
            hr = m_pCodecAPI->SetValue(&CODECAPI_AVLowLatencyMode, &var);

            var.vt = VT_BOOL;
            var.boolVal = VARIANT_TRUE;
            hr = m_pCodecAPI->SetValue(&CODECAPI_AVEncCommonLowLatency, &var); // Correct for the decoder

            // Disable B-Frames
            var.vt = VT_UI4;
            var.ulVal = 0;
            hr = m_pCodecAPI->SetValue(&CODECAPI_AVEncMPVDefaultBPictureCount, &var);

            // Constant bitrate (updated using RTCP)
            var.vt = VT_UI4;
            var.ulVal = eAVEncCommonRateControlMode_CBR;
            hr = m_pCodecAPI->SetValue(&CODECAPI_AVEncCommonRateControlMode, &var);
        }

        hr = S_OK; // Not mandatory features
    }

bail:

    if(SUCCEEDED(hr)) {
        m_nFrameRate = nFrameRate;
        m_nWidth = nWidth;
        m_nHeight = nHeight;
    }

    return hr;
}

HRESULT MFCodecVideo::SetGOPSize(UINT32 nFramesCount)
{
    assert(IsValid());

    HRESULT hr = S_OK;

    if(m_eType == MFCodecType_Encoder && (m_eId == MFCodecId_H264Base || m_eId == MFCodecId_H264Main)) {
        VARIANT var = {0};
        var.vt = VT_UI4;
        var.ullVal = nFramesCount;
        CHECK_HR(hr = m_pCodecAPI->SetValue(&CODECAPI_AVEncMPVGOPSize, &var));
    }

bail:
    return hr;
}

HRESULT MFCodecVideo::SetBitRate(UINT32 nBitRateInBps)
{
    assert(IsValid());

    HRESULT hr = S_OK;

    if(nBitRateInBps > 0 && m_eType == MFCodecType_Encoder) {
        CHECK_HR(hr = m_pOutputType->SetUINT32(MF_MT_AVG_BITRATE, nBitRateInBps));

        if((m_eId == MFCodecId_H264Base || m_eId == MFCodecId_H264Main)) {
            VARIANT var = {0};

            // Set BitRate
            var.vt = VT_UI4;
            var.ullVal = nBitRateInBps;
            CHECK_HR(hr = m_pCodecAPI->SetValue(&CODECAPI_AVEncCommonMeanBitRate, &var));
        }
    }

bail:
    return hr;
}

HRESULT MFCodecVideo::IsSetSliceMaxSizeInBytesSupported(BOOL &supported)
{
    HRESULT hr = S_OK;
    supported = FALSE;

    if ((m_eId == MFCodecId_H264Base || m_eId == MFCodecId_H264Main)) {
#if defined(CODECAPI_AVEncSliceControlMode) && defined(CODECAPI_AVEncSliceControlSize)
        if (m_pCodecAPI->IsSupported(&CODECAPI_AVEncSliceControlMode) == S_OK && m_pCodecAPI->IsSupported(&CODECAPI_AVEncSliceControlSize) == S_OK) {
            supported = TRUE;
        }
#endif
    }
    return hr;
}

HRESULT MFCodecVideo::SetSliceMaxSizeInBytes(UINT32 nSliceMaxSizeInBytes)
{
    assert(IsValid() && nSliceMaxSizeInBytes > 0);

    HRESULT hr = S_OK;

    if ((m_eId == MFCodecId_H264Base || m_eId == MFCodecId_H264Main)) {
#if defined(CODECAPI_AVEncSliceControlMode) && defined(CODECAPI_AVEncSliceControlSize)
        if (m_pCodecAPI->IsSupported(&CODECAPI_AVEncSliceControlMode) == S_OK && m_pCodecAPI->IsSupported(&CODECAPI_AVEncSliceControlSize) == S_OK) {
            VARIANT var = { 0 };
            var.vt = VT_UI4;

            var.ulVal = 1; // Bits
            CHECK_HR(hr = m_pCodecAPI->SetValue(&CODECAPI_AVEncSliceControlMode, &var));

            var.ulVal = (nSliceMaxSizeInBytes << 3); // From Bytes to Bits
            CHECK_HR(hr = m_pCodecAPI->SetValue(&CODECAPI_AVEncSliceControlSize, &var));
        }
#else
        CHECK_HR(hr = S_OK);
#endif
    }

bail:
    return hr;
}

HRESULT MFCodecVideo::RequestKeyFrame()
{
    assert(IsValid());

    HRESULT hr = S_OK;

    if ((m_eId == MFCodecId_H264Base || m_eId == MFCodecId_H264Main)) {
#if defined(CODECAPI_AVEncVideoForceKeyFrame)
        if (m_pCodecAPI->IsSupported(&CODECAPI_AVEncVideoForceKeyFrame) == S_OK) {
            VARIANT var = { 0 };

            var.vt = VT_UI4;
            var.ulVal = 1;
            CHECK_HR(hr = m_pCodecAPI->SetValue(&CODECAPI_AVEncVideoForceKeyFrame, &var));
        }
#else
        CHECK_HR(hr = S_OK);
#endif
    }

bail:
    return hr;
}

//
//	MFCodecVideo
//
MFCodecVideoH264::MFCodecVideoH264(MFCodecId_t eId, MFCodecType_t eType, IMFTransform *pMFT /*= NULL*/)
    : MFCodecVideo(eId, eType, pMFT)
{
    assert(eId == MFCodecId_H264Base || eId == MFCodecId_H264Main);

    HRESULT hr = S_OK;

    if(m_pOutputType) {
        CHECK_HR(hr = m_pOutputType->SetUINT32(MF_MT_MPEG2_PROFILE, (m_eId == MFCodecId_H264Base) ? eAVEncH264VProfile_Base : eAVEncH264VProfile_Main));
    }

bail:
    assert(SUCCEEDED(hr));
}

MFCodecVideoH264::~MFCodecVideoH264()
{

}

MFCodecVideoH264* MFCodecVideoH264::CreateCodecH264Base(MFCodecType_t eType, IMFTransform *pMFT /*= NULL*/)
{
    MFCodecVideoH264* pCodec = new MFCodecVideoH264(MFCodecId_H264Base, eType, pMFT);
    if(pCodec && !pCodec->IsValid()) {
        SafeRelease(&pCodec);
    }
    return pCodec;
}

MFCodecVideoH264* MFCodecVideoH264::CreateCodecH264Main(MFCodecType_t eType, IMFTransform *pMFT /*= NULL*/)
{
    MFCodecVideoH264* pCodec = new MFCodecVideoH264(MFCodecId_H264Main, eType, pMFT);
    if(pCodec && !pCodec->IsValid()) {
        SafeRelease(&pCodec);
    }
    return pCodec;
}
