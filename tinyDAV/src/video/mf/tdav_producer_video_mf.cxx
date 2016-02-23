/*Copyright (C) 2013 Doubango Telecom <http://www.doubango.org>
* Copyright (C) Microsoft Corporation. All rights reserved.
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
/**@file tdav_producer_video_mf.cxx
 * @brief Microsoft Media Foundation video producer (source).
 *	http://msdn.microsoft.com/en-us/library/windows/desktop/ms694197(v=vs.85).aspx
 */
#include "tinydav/video/mf/tdav_producer_video_mf.h"

#if HAVE_MF

#include "tsk_semaphore.h"
#include "tsk_thread.h"
#include "tsk_string.h"
#include "tsk_debug.h"

#undef _WIN32_WINNT /* project-level value is 0x0501 (XP) */
#define _WIN32_WINNT _WIN32_WINNT_WIN7 /* must be re-defined before include Media Doundation headers */

#if defined(_MSC_VER)
#	pragma comment(lib, "mfplat.lib")
#	pragma comment(lib, "mf.lib")
#	pragma comment(lib, "mfuuid.lib")
#	pragma comment(lib, "shlwapi.lib")
#endif

#include <new>
#include <mfapi.h>
#include <mfidl.h>
#include <Mferror.h>
#include <shlwapi.h>

#undef SafeRelease
#define SafeRelease(ppT) \
{ \
    if (*ppT) \
    { \
        (*ppT)->Release(); \
        *ppT = NULL; \
    } \
}

#undef CHECK_HR
#define CHECK_HR(x) if (FAILED(x)) { TSK_DEBUG_ERROR("Operation Failed"); goto bail; }


typedef struct VideoSubTypeGuidPair {
    tmedia_chroma_t chroma;
    const GUID fourcc;
}
VideoSubTypeGuidPair;
static const VideoSubTypeGuidPair g_VideoSubTypeGuidPairs[] = {

    { tmedia_chroma_nv12, MFVideoFormat_NV12 }, // 0
    { tmedia_chroma_rgb24, MFVideoFormat_RGB24 },
    { tmedia_chroma_rgb32, MFVideoFormat_RGB32 },
    // to be continued
};

// MFVideoFormat_NV12, MFVideoFormat_YUY2, MFVideoFormat_UYVY,
// MFVideoFormat_RGB32, MFVideoFormat_RGB24, MFVideoFormat_IYUV
#undef DEFAULT_SUBTYPE_INDEX
#define DEFAULT_SUBTYPE_INDEX 0

//
//	DeviceList [Declaration]
//
class DeviceList
{
    UINT32      m_cDevices;
    IMFActivate **m_ppDevices;

public:
    DeviceList() : m_ppDevices(NULL), m_cDevices(0) {

    }
    ~DeviceList() {
        Clear();
    }

    UINT32  Count() const {
        return m_cDevices;
    }

    void    Clear();
    HRESULT EnumerateDevices();
    HRESULT GetDeviceAtIndex(UINT32 index, IMFActivate **ppActivate);
    HRESULT GetDeviceBest(IMFActivate **ppActivate);
    HRESULT GetDeviceName(UINT32 index, WCHAR **ppszName);
};


//
//	SampleGrabberCB [Declaration]
//	http://msdn.microsoft.com/en-us/library/windows/desktop/hh184779(v=vs.85).aspx
//
class SampleGrabberCB : public IMFSampleGrabberSinkCallback
{
    long m_cRef;
    const struct tdav_producer_video_mf_s* m_pWrappedProducer;

    SampleGrabberCB(const struct tdav_producer_video_mf_s* pcWrappedProducer) : m_cRef(1), m_pWrappedProducer(pcWrappedProducer) {}

public:
    static HRESULT CreateInstance(const struct tdav_producer_video_mf_s* pcWrappedProducer, SampleGrabberCB **ppCB);

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


//
//	tdav_producer_video_mf_t
//
typedef struct tdav_producer_video_mf_s {
    TMEDIA_DECLARE_PRODUCER;

    bool bStarted;
    tsk_thread_handle_t* ppTread[1];

    DeviceList* pDeviceList;

    IMFMediaSession *pSession;
    IMFMediaSource *pSource;
    SampleGrabberCB *pCallback;
    IMFActivate *pSinkActivate;
    IMFTopology *pTopology;
    IMFMediaType *pType;
}
tdav_producer_video_mf_t;

//
// Forward declarations for glovbal functions
//
static HRESULT CreateTopology(
    IMFMediaSource *pSource,
    IMFActivate *pSinkActivate,
    IMFTopology **ppTopo);
static HRESULT AddSourceNode(
    IMFTopology *pTopology,
    IMFMediaSource *pSource,
    IMFPresentationDescriptor *pPD,
    IMFStreamDescriptor *pSD,
    IMFTopologyNode **ppNode);
static HRESULT AddOutputNode(
    IMFTopology *pTopology,
    IMFActivate *pActivate,
    DWORD dwId,
    IMFTopologyNode **ppNode);
static HRESULT RunSession(
    IMFMediaSession *pSession,
    IMFTopology *pTopology);
static HRESULT StopSession(
    IMFMediaSession *pSession,
    IMFMediaSource *pSource);
static HRESULT PauseSession(
    IMFMediaSession *pSession);
static void* TSK_STDCALL RunSessionThread(void *pArg);


/* ============ Media Producer Interface ================= */
static int tdav_producer_video_mf_set(tmedia_producer_t *self, const tmedia_param_t* param)
{
    int ret = 0;
    tdav_producer_video_mf_t* pSelf = (tdav_producer_video_mf_t*)self;

    if(!pSelf || !param) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    return ret;
}

static int tdav_producer_video_mf_prepare(tmedia_producer_t* self, const tmedia_codec_t* codec)
{
    tdav_producer_video_mf_t* pSelf = (tdav_producer_video_mf_t*)self;

    if(!pSelf || !codec || !codec->plugin) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

#if 0
    TMEDIA_PRODUCER(producer)->encoder.codec_id = (tmedia_codec_id_t)(tmedia_codec_id_h264_bp | tmedia_codec_id_h264_mp);
#else
    TMEDIA_PRODUCER(pSelf)->video.chroma = g_VideoSubTypeGuidPairs[DEFAULT_SUBTYPE_INDEX].chroma;
#endif

    TMEDIA_PRODUCER(pSelf)->video.fps = TMEDIA_CODEC_VIDEO(codec)->out.fps;
    TMEDIA_PRODUCER(pSelf)->video.width = TMEDIA_CODEC_VIDEO(codec)->out.width;
    TMEDIA_PRODUCER(pSelf)->video.height = TMEDIA_CODEC_VIDEO(codec)->out.height;

    // MFVideoFormat_NV12, MFVideoFormat_YUY2, MFVideoFormat_UYVY,
// MFVideoFormat_RGB32, MFVideoFormat_RGB24, MFVideoFormat_IYUV
#undef DEFAULT_SUBTYPE

    HRESULT hr = S_OK;

    // create device list object
    if(!pSelf->pDeviceList && !(pSelf->pDeviceList = new DeviceList())) {
        TSK_DEBUG_ERROR("Failed to create device list");
        hr = E_OUTOFMEMORY;
        goto  bail;
    }
    // enumerate devices
    hr = pSelf->pDeviceList->EnumerateDevices();
    if(!SUCCEEDED(hr)) {
        goto bail;
    }

    // check if we have at least one MF video source connected to the PC
    if(pSelf->pDeviceList->Count() == 0) {
        TSK_DEBUG_WARN("No MF video source could be found...no video will be sent");
        // do not break the negotiation as one-way video connection is a valid use-case
    }
    else {
        IMFActivate* pActivate = NULL;
        // Get best MF video source
        hr = pSelf->pDeviceList->GetDeviceBest(&pActivate);
        if(!SUCCEEDED(hr) || !pActivate) {
            TSK_DEBUG_ERROR("Failed to get best MF video source");
            if(!pActivate) {
                hr = E_OUTOFMEMORY;
            }
            goto bail;
        }

        // Create the media source for the device.
        hr = pActivate->ActivateObject(
                 __uuidof(IMFMediaSource),
                 (void**)&pSelf->pSource
             );
        SafeRelease(&pActivate);
        if(!SUCCEEDED(hr)) {
            TSK_DEBUG_ERROR("ActivateObject(MF video source) failed");
            goto bail;
        }

        // Configure the media type that the Sample Grabber will receive.
        // Setting the major and subtype is usually enough for the topology loader
        // to resolve the topology.

        CHECK_HR(hr = MFCreateMediaType(&pSelf->pType));
        CHECK_HR(hr = pSelf->pType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video));
#if 0
        CHECK_HR(hr = pSelf->pType->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_H264);
#else
        CHECK_HR(hr = pSelf->pType->SetGUID(MF_MT_SUBTYPE, g_VideoSubTypeGuidPairs[DEFAULT_SUBTYPE_INDEX].fourcc));
#endif
                 CHECK_HR(hr = MFSetAttributeSize(pSelf->pType, MF_MT_FRAME_SIZE, TMEDIA_PRODUCER(pSelf)->video.width, TMEDIA_PRODUCER(pSelf)->video.height));
                 // CHECK_HR(hr = pSelf->pType->SetUINT32(MF_MT_DEFAULT_STRIDE, 1280));
                 CHECK_HR(hr = pSelf->pType->SetUINT32(MF_MT_ALL_SAMPLES_INDEPENDENT, 1));
                 CHECK_HR(hr = pSelf->pType->SetUINT32(MF_MT_FIXED_SIZE_SAMPLES, 1));
                 // CHECK_HR(hr = pSelf->pType->SetUINT32(MF_MT_AVG_BITRATE,    147456000));
                 //CHECK_HR(hr = MFSetAttributeRatio(pSelf->pType, MF_MT_FRAME_RATE, (UINT32)30000, 1001));
                 CHECK_HR(hr = MFSetAttributeRatio(pSelf->pType, MF_MT_PIXEL_ASPECT_RATIO, 1, 1));
                 CHECK_HR(hr = pSelf->pType->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive));

                 // Create the sample grabber sink.
                 CHECK_HR(hr = SampleGrabberCB::CreateInstance(pSelf, &pSelf->pCallback));
                 CHECK_HR(hr = MFCreateSampleGrabberSinkActivate(pSelf->pType, pSelf->pCallback, &pSelf->pSinkActivate));

                 // To run as fast as possible, set this attribute (requires Windows 7):
                 CHECK_HR(hr = pSelf->pSinkActivate->SetUINT32(MF_SAMPLEGRABBERSINK_IGNORE_CLOCK, TRUE));

                 // Create the Media Session.
                 CHECK_HR(hr = MFCreateMediaSession(NULL, &pSelf->pSession));

                 // Create the topology.
                 CHECK_HR(hr = CreateTopology(pSelf->pSource, pSelf->pSinkActivate, &pSelf->pTopology));
    }

bail:
    return SUCCEEDED(hr) ? 0 : -1;
}

static int tdav_producer_video_mf_start(tmedia_producer_t* self)
{
    tdav_producer_video_mf_t* pSelf = (tdav_producer_video_mf_t*)self;

    if(!pSelf) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if(pSelf->bStarted) {
        return 0;
    }

    HRESULT hr = S_OK;

    // Run the media session.
    CHECK_HR(hr = RunSession(pSelf->pSession, pSelf->pTopology));

    // Start asynchronous watcher thread
    pSelf->bStarted = true;
    int ret = tsk_thread_create(&pSelf->ppTread[0], RunSessionThread, pSelf);
    if(ret != 0) {
        TSK_DEBUG_ERROR("Failed to create thread");
        hr = E_FAIL;
        pSelf->bStarted = false;
        StopSession(pSelf->pSession, pSelf->pSource);
        goto bail;
    }

bail:
    return SUCCEEDED(hr) ? 0 : -1;
}

static int tdav_producer_video_mf_pause(tmedia_producer_t* self)
{
    tdav_producer_video_mf_t* pSelf = (tdav_producer_video_mf_t*)self;

    if(!pSelf) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    HRESULT hr = PauseSession(pSelf->pSession);

    return 0;
}

static int tdav_producer_video_mf_stop(tmedia_producer_t* self)
{
    tdav_producer_video_mf_t* pSelf = (tdav_producer_video_mf_t*)self;

    if(!pSelf) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    HRESULT hr = S_OK;

    // for the thread
    pSelf->bStarted = false;
    hr = StopSession(pSelf->pSession, NULL); // stop session to wakeup the asynchronous thread
    if(pSelf->ppTread[0]) {
        tsk_thread_join(&pSelf->ppTread[0]);
    }
    hr = StopSession(NULL, pSelf->pSource); // stop source to release the camera

    return 0;
}

//
//	Windows Media Foundation video producer object definition
//
/* constructor */
static tsk_object_t* tdav_producer_video_mf_ctor(tsk_object_t * self, va_list * app)
{
    tdav_producer_video_mf_t *pSelf = (tdav_producer_video_mf_t *)self;
    if(pSelf) {
        /* init base */
        tmedia_producer_init(TMEDIA_PRODUCER(pSelf));
#if 0 // H.264
        TMEDIA_PRODUCER(pSelf)->video.chroma = tmedia_chroma_yuv420p; // To avoid chroma conversion
#endif
        /* init self with default values*/

        TMEDIA_PRODUCER(pSelf)->video.fps = 15;
        TMEDIA_PRODUCER(pSelf)->video.width = 352;
        TMEDIA_PRODUCER(pSelf)->video.height = 288;

#if TDAV_UNDER_WINDOWS_PHONE
        pSelf->videoCapturePhone = ref new VideoCapturePhone();
#endif
    }
    return self;
}
/* destructor */
static tsk_object_t* tdav_producer_video_mf_dtor(tsk_object_t * self)
{
    tdav_producer_video_mf_t *pSelf = (tdav_producer_video_mf_t *)self;
    if(pSelf) {
        /* stop */
        //if(pSelf->started){
        tdav_producer_video_mf_stop((tmedia_producer_t*)self);
        //}

        /* deinit base */
        tmedia_producer_deinit(TMEDIA_PRODUCER(pSelf));
        /* deinit self */
        if(pSelf->pDeviceList) {
            delete pSelf->pDeviceList, pSelf->pDeviceList = NULL;
        }
        if(pSelf->pSource) {
            pSelf->pSource->Shutdown();
        }
        if(pSelf->pSession) {
            pSelf->pSession->Shutdown();
        }

        SafeRelease(&pSelf->pSession);
        SafeRelease(&pSelf->pSource);
        SafeRelease(&pSelf->pCallback);
        SafeRelease(&pSelf->pSinkActivate);
        SafeRelease(&pSelf->pTopology);
        SafeRelease(&pSelf->pType);
    }

    return self;
}
/* object definition */
static const tsk_object_def_t tdav_producer_video_mf_def_s = {
    sizeof(tdav_producer_video_mf_t),
    tdav_producer_video_mf_ctor,
    tdav_producer_video_mf_dtor,
    tsk_null,
};
/* plugin definition*/
static const tmedia_producer_plugin_def_t tdav_producer_video_mf_plugin_def_s = {
    &tdav_producer_video_mf_def_s,

    tmedia_video,
    "Microsoft Windows Media Foundation producer (Video)",

    tdav_producer_video_mf_set,
    tdav_producer_video_mf_prepare,
    tdav_producer_video_mf_start,
    tdav_producer_video_mf_pause,
    tdav_producer_video_mf_stop
};
const tmedia_producer_plugin_def_t *tdav_producer_video_mf_plugin_def_t = &tdav_producer_video_mf_plugin_def_s;




//
//	DeviceList [Implementation]
//
void DeviceList::Clear()
{
    for (UINT32 i = 0; i < m_cDevices; i++) {
        SafeRelease(&m_ppDevices[i]);
    }
    CoTaskMemFree(m_ppDevices);
    m_ppDevices = NULL;

    m_cDevices = 0;
}

HRESULT DeviceList::EnumerateDevices()
{
    HRESULT hr = S_OK;
    IMFAttributes *pAttributes = NULL;

    Clear();

    // Initialize an attribute store. We will use this to
    // specify the enumeration parameters.

    hr = MFCreateAttributes(&pAttributes, 1);

    // Ask for source type = video capture devices
    if (SUCCEEDED(hr)) {
        hr = pAttributes->SetGUID(
                 MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE,
                 MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID
             );
    }

    // Enumerate devices.
    if (SUCCEEDED(hr)) {
        hr = MFEnumDeviceSources(pAttributes, &m_ppDevices, &m_cDevices);
    }

    SafeRelease(&pAttributes);

    return hr;
}


HRESULT DeviceList::GetDeviceAtIndex(UINT32 index, IMFActivate **ppActivate)
{
    if (index >= Count()) {
        return E_INVALIDARG;
    }

    *ppActivate = m_ppDevices[index];
    (*ppActivate)->AddRef();

    return S_OK;
}

HRESULT DeviceList::GetDeviceBest(IMFActivate **ppActivate)
{
    // for now we just get the default video source device (index = 0)
    return GetDeviceAtIndex(0, ppActivate);
}

HRESULT DeviceList::GetDeviceName(UINT32 index, WCHAR **ppszName)
{
    if (index >= Count()) {
        return E_INVALIDARG;
    }

    HRESULT hr = S_OK;

    hr = m_ppDevices[index]->GetAllocatedString(
             MF_DEVSOURCE_ATTRIBUTE_FRIENDLY_NAME,
             ppszName,
             NULL
         );

    return hr;
}


//
// SampleGrabberCB [implementation]
// http://msdn.microsoft.com/en-us/library/windows/desktop/hh184779(v=vs.85).aspx
//

// Create a new instance of the object.
HRESULT SampleGrabberCB::CreateInstance(const struct tdav_producer_video_mf_s* pcWrappedProducer, SampleGrabberCB **ppCB)
{
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
    return S_OK;
}

STDMETHODIMP SampleGrabberCB::OnClockStop(MFTIME hnsSystemTime)
{
    return S_OK;
}

STDMETHODIMP SampleGrabberCB::OnClockPause(MFTIME hnsSystemTime)
{
    return S_OK;
}

STDMETHODIMP SampleGrabberCB::OnClockRestart(MFTIME hnsSystemTime)
{
    return S_OK;
}

STDMETHODIMP SampleGrabberCB::OnClockSetRate(MFTIME hnsSystemTime, float flRate)
{
    return S_OK;
}

// IMFSampleGrabberSink methods.

STDMETHODIMP SampleGrabberCB::OnSetPresentationClock(IMFPresentationClock* pClock)
{
    return S_OK;
}

STDMETHODIMP SampleGrabberCB::OnProcessSample(REFGUID guidMajorMediaType, DWORD dwSampleFlags,
        LONGLONG llSampleTime, LONGLONG llSampleDuration, const BYTE * pSampleBuffer,
        DWORD dwSampleSize)
{
    if(m_pWrappedProducer && TMEDIA_PRODUCER(m_pWrappedProducer)->enc_cb.callback) {
        TMEDIA_PRODUCER(m_pWrappedProducer)->enc_cb.callback(TMEDIA_PRODUCER(m_pWrappedProducer)->enc_cb.callback_data, pSampleBuffer, dwSampleSize);
    }

    return S_OK;
}

STDMETHODIMP SampleGrabberCB::OnShutdown()
{
    return S_OK;
}


//
//	Global functions
//

// Create the topology.
static HRESULT CreateTopology(IMFMediaSource *pSource, IMFActivate *pSinkActivate, IMFTopology **ppTopo)
{
    IMFTopology *pTopology = NULL;
    IMFPresentationDescriptor *pPD = NULL;
    IMFStreamDescriptor *pSD = NULL;
    IMFMediaTypeHandler *pHandler = NULL;
    IMFTopologyNode *pNode1 = NULL;
    IMFTopologyNode *pNode2 = NULL;

    HRESULT hr = S_OK;
    DWORD cStreams = 0;

    CHECK_HR(hr = MFCreateTopology(&pTopology));
    CHECK_HR(hr = pSource->CreatePresentationDescriptor(&pPD));
    CHECK_HR(hr = pPD->GetStreamDescriptorCount(&cStreams));

    for (DWORD i = 0; i < cStreams; i++) {
        // In this example, we look for audio streams and connect them to the sink.

        BOOL fSelected = FALSE;
        GUID majorType;

        CHECK_HR(hr = pPD->GetStreamDescriptorByIndex(i, &fSelected, &pSD));
        CHECK_HR(hr = pSD->GetMediaTypeHandler(&pHandler));
        CHECK_HR(hr = pHandler->GetMajorType(&majorType));

        if (majorType == MFMediaType_Video && fSelected) {
            CHECK_HR(hr = AddSourceNode(pTopology, pSource, pPD, pSD, &pNode1));
            CHECK_HR(hr = AddOutputNode(pTopology, pSinkActivate, 0, &pNode2));
            CHECK_HR(hr = pNode1->ConnectOutput(0, pNode2, 0));
            break;
        }
        else {
            CHECK_HR(hr = pPD->DeselectStream(i));
        }
        SafeRelease(&pSD);
        SafeRelease(&pHandler);
    }

    *ppTopo = pTopology;
    (*ppTopo)->AddRef();

bail:
    SafeRelease(&pTopology);
    SafeRelease(&pNode1);
    SafeRelease(&pNode2);
    SafeRelease(&pPD);
    SafeRelease(&pSD);
    SafeRelease(&pHandler);
    return hr;
}

// Add a source node to a topology.
static HRESULT AddSourceNode(
    IMFTopology *pTopology,           // Topology.
    IMFMediaSource *pSource,          // Media source.
    IMFPresentationDescriptor *pPD,   // Presentation descriptor.
    IMFStreamDescriptor *pSD,         // Stream descriptor.
    IMFTopologyNode **ppNode)         // Receives the node pointer.
{
    IMFTopologyNode *pNode = NULL;

    HRESULT hr = S_OK;
    CHECK_HR(hr = MFCreateTopologyNode(MF_TOPOLOGY_SOURCESTREAM_NODE, &pNode));
    CHECK_HR(hr = pNode->SetUnknown(MF_TOPONODE_SOURCE, pSource));
    CHECK_HR(hr = pNode->SetUnknown(MF_TOPONODE_PRESENTATION_DESCRIPTOR, pPD));
    CHECK_HR(hr = pNode->SetUnknown(MF_TOPONODE_STREAM_DESCRIPTOR, pSD));
    CHECK_HR(hr = pTopology->AddNode(pNode));

    // Return the pointer to the caller.
    *ppNode = pNode;
    (*ppNode)->AddRef();

bail:
    SafeRelease(&pNode);
    return hr;
}

// Add an output node to a topology.
static HRESULT AddOutputNode(
    IMFTopology *pTopology,     // Topology.
    IMFActivate *pActivate,     // Media sink activation object.
    DWORD dwId,                 // Identifier of the stream sink.
    IMFTopologyNode **ppNode)   // Receives the node pointer.
{
    IMFTopologyNode *pNode = NULL;

    HRESULT hr = S_OK;
    CHECK_HR(hr = MFCreateTopologyNode(MF_TOPOLOGY_OUTPUT_NODE, &pNode));
    CHECK_HR(hr = pNode->SetObject(pActivate));
    CHECK_HR(hr = pNode->SetUINT32(MF_TOPONODE_STREAMID, dwId));
    CHECK_HR(hr = pNode->SetUINT32(MF_TOPONODE_NOSHUTDOWN_ON_REMOVE, FALSE));
    CHECK_HR(hr = pTopology->AddNode(pNode));

    // Return the pointer to the caller.
    *ppNode = pNode;
    (*ppNode)->AddRef();

bail:
    SafeRelease(&pNode);
    return hr;
}

// Run session
static HRESULT RunSession(IMFMediaSession *pSession, IMFTopology *pTopology)
{
    IMFMediaEvent *pEvent = NULL;

    PROPVARIANT var;
    PropVariantInit(&var);

    MediaEventType met;
    HRESULT hrStatus = S_OK;
    HRESULT hr = S_OK;
    CHECK_HR(hr = pSession->SetTopology(0, pTopology));
    CHECK_HR(hr = pSession->Start(&GUID_NULL, &var));

    // Check first event
    hr = pSession->GetEvent(MF_EVENT_FLAG_NO_WAIT, &pEvent);
    if(hr == MF_E_NO_EVENTS_AVAILABLE) {
        hr = S_OK;
        goto bail;
    }
    CHECK_HR(hr = pEvent->GetStatus(&hrStatus));
    if (FAILED(hrStatus)) {
        CHECK_HR(hr = pEvent->GetType(&met));
        TSK_DEBUG_ERROR("Session error: 0x%x (event id: %d)\n", hrStatus, met);
        hr = hrStatus;
        goto bail;
    }

bail:
    SafeRelease(&pEvent);
    return hr;
}

// Stop session
static HRESULT StopSession(
    IMFMediaSession *pSession,
    IMFMediaSource *pSource)
{
    // MUST be source then session
    if(pSource) {
        pSource->Shutdown();
    }
    if(pSession) {
        pSession->Shutdown();
    }
    return S_OK;
}

// Pause session
static HRESULT PauseSession(
    IMFMediaSession *pSession)
{
    if(!pSession) {
        return E_INVALIDARG;
    }
    return pSession->Pause();
}

// Run session async thread
static void* TSK_STDCALL RunSessionThread(void *pArg)
{
    tdav_producer_video_mf_t *pSelf = (tdav_producer_video_mf_t *)pArg;
    HRESULT hrStatus = S_OK;
    HRESULT hr = S_OK;
    IMFMediaEvent *pEvent = NULL;
    MediaEventType met;

    TSK_DEBUG_INFO("RunSessionThread - ENTER");

    while(pSelf->bStarted) {
        CHECK_HR(hr = pSelf->pSession->GetEvent(0, &pEvent));
        CHECK_HR(hr = pEvent->GetStatus(&hrStatus));
        CHECK_HR(hr = pEvent->GetType(&met));

        if (FAILED(hrStatus)) {
            TSK_DEBUG_ERROR("Session error: 0x%x (event id: %d)\n", hrStatus, met);
            hr = hrStatus;
            goto bail;
        }
        if (met == MESessionEnded) {
            break;
        }
        SafeRelease(&pEvent);
    }

bail:
    TSK_DEBUG_INFO("RunSessionThread - EXIT");

    return NULL;
}

#endif /*HAVE_MF  */
