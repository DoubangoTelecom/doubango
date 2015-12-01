/*Copyright (C) 2013 Doubango Telecom <http://www.doubango.org>
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
/**@file tdav_producer_winm.cxx
* @brief Microsoft Windows Media (WinM) video producer.
*
*/
#include "tinydav/video/winm/tdav_producer_winm.h"

#if HAVE_WINM

#include "tsk_string.h"
#include "tsk_debug.h"

#if TDAV_UNDER_WINDOWS_PHONE

#include <windows.h>
#include <implements.h>
#include <Windows.Phone.Media.Capture.h>
#include <Windows.Phone.Media.Capture.Native.h>

using namespace Windows::System::Threading;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Platform;
using namespace Windows::Phone::Media::Capture;
using namespace Windows::Storage::Streams;

struct tdav_producer_winm_s;

namespace Doubango
{
	namespace VoIP
	{
		class CaptureSampleSink :
            public Microsoft::WRL::RuntimeClass<
            Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::RuntimeClassType::ClassicCom>, 
            ICameraCaptureSampleSink>
        {
            DWORD m_dwSampleCount;
			const struct tdav_producer_winm_s* m_pProducer;

        public:

            STDMETHODIMP RuntimeClassInitialize(const struct tdav_producer_winm_s* pProducer)
            {
                m_dwSampleCount = 0;
				m_pProducer = pProducer;
                return S_OK;
            }

            DWORD GetSampleCount()
            {
                return m_dwSampleCount;
            }

            IFACEMETHODIMP_(void)
                OnSampleAvailable(
                ULONGLONG hnsPresentationTime,
                ULONGLONG hnsSampleDuration,
                DWORD cbSample,
                BYTE* pSample)
            {
                m_dwSampleCount++;
				if(m_pProducer && TMEDIA_PRODUCER(m_pProducer)->enc_cb.callback)
				{
					TMEDIA_PRODUCER(m_pProducer)->enc_cb.callback(TMEDIA_PRODUCER(m_pProducer)->enc_cb.callback_data, pSample, cbSample);
				}
            }
        };

		ref class VideoCapturePhone sealed
		{
		public:
			virtual ~VideoCapturePhone();
		internal:
			VideoCapturePhone();

			int Prepare(const struct tdav_producer_winm_s* winm);
			int Start();
			int Pause();
			int Stop();
			void SetCameraLocation(Windows::Phone::Media::Capture::CameraSensorLocation cameraLocation);
			void ToggleCamera();

		private:
			int UnPrepare();
			void ToggleCameraThread(Windows::Foundation::IAsyncAction^ operation);

			tsk_mutex_handle_t* m_hMutex;

			const tdav_producer_winm_s* m_pWrappedPlugin;

			// Has capture started?
			bool m_bStarted, m_bPrepared;

			// Events to signal whether capture has stopped/started
			HANDLE m_hStopCompleted;
			HANDLE m_hStartCompleted;

			IAsyncOperation<AudioVideoCaptureDevice^> ^m_pOpenOperation;

			Windows::Foundation::IAsyncAction^ m_ToggleThread;

			// Native sink and video device
			CaptureSampleSink *m_pVideoSink;
			IAudioVideoCaptureDeviceNative *m_pVideoDevice;

			Windows::Phone::Media::Capture::CameraSensorLocation m_eCameraLocation;

			Windows::Phone::Media::Capture::AudioVideoCaptureDevice ^m_pVideoOnlyDevice;
			Windows::Foundation::IAsyncAction ^m_pVideoCaptureAction;
		};
	}
}

using namespace Doubango::VoIP;

#endif


typedef struct tdav_producer_winm_s
{
	TMEDIA_DECLARE_PRODUCER;
#if TDAV_UNDER_WINDOWS_PHONE
	VideoCapturePhone^ videoCapturePhone;
#endif
}
tdav_producer_winm_t;


/* ============ Media Producer Interface ================= */
static int tdav_producer_winm_set(tmedia_producer_t *self, const tmedia_param_t* param)
{
	int ret = 0;
	tdav_producer_winm_t* producer = (tdav_producer_winm_t*)self;

	if(!producer || !param){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(param->value_type == tmedia_pvt_int32){
		if(tsk_striequals(param->key, "camera-location")){
			Windows::Phone::Media::Capture::CameraSensorLocation cameraLocation = (Windows::Phone::Media::Capture::CameraSensorLocation)*((int32_t*)param->value);
			if(producer->videoCapturePhone)
			{
				producer->videoCapturePhone->SetCameraLocation(cameraLocation);
				return 0;
			}
		}
	}

	return ret;
}

static int tdav_producer_winm_prepare(tmedia_producer_t* self, const tmedia_codec_t* codec)
{
	tdav_producer_winm_t* producer = (tdav_producer_winm_t*)self;

	if(!producer || !codec && codec->plugin){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	
	if(codec->id == tmedia_codec_id_h264_bp || codec->id == tmedia_codec_id_h264_mp) {
		TMEDIA_PRODUCER(producer)->encoder.codec_id = codec->id;
	}
	else {
		TMEDIA_PRODUCER(producer)->encoder.codec_id = tmedia_codec_id_none;
	}
	TMEDIA_PRODUCER(producer)->video.fps = TMEDIA_CODEC_VIDEO(codec)->out.fps;
	TMEDIA_PRODUCER(producer)->video.width = TMEDIA_CODEC_VIDEO(codec)->out.width;
	TMEDIA_PRODUCER(producer)->video.height = TMEDIA_CODEC_VIDEO(codec)->out.height;


#if TDAV_UNDER_WINDOWS_PHONE
	return producer->videoCapturePhone->Prepare(producer);
#else
	TSK_DEBUG_ERROR("Unexpected code called");
	return -1;
#endif
}

static int tdav_producer_winm_start(tmedia_producer_t* self)
{
	tdav_producer_winm_t* producer = (tdav_producer_winm_t*)self;

	if(!producer){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

#if TDAV_UNDER_WINDOWS_PHONE
	return producer->videoCapturePhone->Start();
#else
	TSK_DEBUG_ERROR("Unexpected code called");
	return -1;
#endif
}

static int tdav_producer_winm_pause(tmedia_producer_t* self)
{
	tdav_producer_winm_t* producer = (tdav_producer_winm_t*)self;

	if(!producer){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

#if TDAV_UNDER_WINDOWS_PHONE
	return producer->videoCapturePhone->Pause();
#else
	TSK_DEBUG_ERROR("Unexpected code called");
	return -1;
#endif
}

static int tdav_producer_winm_stop(tmedia_producer_t* self)
{
	tdav_producer_winm_t* producer = (tdav_producer_winm_t*)self;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

#if TDAV_UNDER_WINDOWS_PHONE
	return producer->videoCapturePhone->Stop();
#else
	TSK_DEBUG_ERROR("Unexpected code called");
	return -1;
#endif
}


#if TDAV_UNDER_WINDOWS_PHONE

VideoCapturePhone::VideoCapturePhone() :
    m_bStarted(false),
	m_bPrepared(false),
    m_pVideoOnlyDevice(nullptr),
    m_pVideoSink(NULL),
    m_pVideoDevice(NULL),
	m_pWrappedPlugin(NULL),
	m_pOpenOperation(nullptr),
    m_eCameraLocation(CameraSensorLocation::Front)
{
	if(!(m_hMutex = tsk_mutex_create())){
		throw ref new Platform::FailureException(L"Failed to create mutex");
	}

    m_hStopCompleted = CreateEventEx(NULL, NULL, CREATE_EVENT_MANUAL_RESET, EVENT_ALL_ACCESS);
    if (!m_hStopCompleted)
    {
        throw ref new Platform::Exception(HRESULT_FROM_WIN32(GetLastError()), L"Could not create shutdown event");
    }

    m_hStartCompleted = CreateEventEx(NULL, NULL, CREATE_EVENT_MANUAL_RESET, EVENT_ALL_ACCESS);
    if (!m_hStartCompleted)
    {
        throw ref new Platform::Exception(HRESULT_FROM_WIN32(GetLastError()), L"Could not create start event");
    }
}

VideoCapturePhone::~VideoCapturePhone()
{
	Stop();

    if(m_ToggleThread)
    {
        m_ToggleThread->Cancel();
        m_ToggleThread->Close();
        m_ToggleThread = nullptr;
    }

	tsk_mutex_destroy(&m_hMutex);
}

int VideoCapturePhone::Prepare(const struct tdav_producer_winm_s* winm)
{
	HRESULT hr = E_FAIL;
	int ret = 0;
	Windows::Foundation::Size dimensionsRequested, dimensionsClosest;
	Collections::IVectorView<Size> ^availableSizes;
	Collections::IIterator<Windows::Foundation::Size> ^availableSizesIterator;
	bool bClosestFound = false;

	#define WINM_SET_ERROR(code) ret = (code); goto bail;

	tsk_mutex_lock(m_hMutex);

	if(m_bPrepared)
	{
		TSK_DEBUG_INFO("#WASAPI: Audio producer already prepared");
		goto bail;
	}

	if(!winm)
	{
		TSK_DEBUG_ERROR("Invalid parameter");
		WINM_SET_ERROR(-1);
	}

	if(m_pVideoCaptureAction || m_pVideoDevice || m_pVideoOnlyDevice || m_pVideoSink || m_pOpenOperation){
		TSK_DEBUG_ERROR("Producer already prepared");
		WINM_SET_ERROR(-2);
	}
	
	dimensionsClosest.Width = dimensionsRequested.Width = (float)TMEDIA_PRODUCER(winm)->video.width;
	dimensionsClosest.Height = dimensionsRequested.Height = (float)TMEDIA_PRODUCER(winm)->video.height;
    availableSizes = AudioVideoCaptureDevice::GetAvailableCaptureResolutions(m_eCameraLocation);
    availableSizesIterator = availableSizes->First();
    
    while(!m_pOpenOperation && availableSizesIterator->HasCurrent)
    {
		TSK_DEBUG_INFO("Camera Supported size: (%f, %f)", availableSizesIterator->Current.Width, availableSizesIterator->Current.Height);
        if(availableSizesIterator->Current.Height == dimensionsRequested.Width && availableSizesIterator->Current.Width == dimensionsRequested.Height)
        {
            m_pOpenOperation = AudioVideoCaptureDevice::OpenForVideoOnlyAsync(m_eCameraLocation, dimensionsRequested);
			TSK_DEBUG_INFO("Camera::Open(%d, %d)", dimensionsRequested.Width, dimensionsRequested.Height);
			break;
        }
		else if(!bClosestFound && (availableSizesIterator->Current.Height <= dimensionsRequested.Height && availableSizesIterator->Current.Width <= dimensionsRequested.Width))
		{
			dimensionsClosest.Height = availableSizesIterator->Current.Height;
			dimensionsClosest.Width = availableSizesIterator->Current.Width;
			bClosestFound = true;
		}
        availableSizesIterator->MoveNext();
    }

	if(!m_pOpenOperation)
	{
		m_pOpenOperation = AudioVideoCaptureDevice::OpenForVideoOnlyAsync(m_eCameraLocation, dimensionsClosest);
		TSK_DEBUG_INFO("Camera::Open(%f, %f)", dimensionsClosest.Width, dimensionsClosest.Height);
	}

bail:
	if(ret != 0){
		UnPrepare();
	}
	if((m_bPrepared = (ret == 0)))
	{
		m_pWrappedPlugin = winm;
	}

	tsk_mutex_unlock(m_hMutex);

	return ret;
}

int VideoCapturePhone::Start()
{
	tsk_mutex_lock(m_hMutex);

	if(m_bStarted)
	{
		TSK_DEBUG_INFO("#WINM: Video producer already started");
		goto bail;
	}
	if(!m_bPrepared)
	{
		TSK_DEBUG_ERROR("#WINM: Video producer not prepared");
		goto bail;
	}

	m_bStarted = true;

	m_pOpenOperation->Completed = ref new AsyncOperationCompletedHandler<AudioVideoCaptureDevice^>([this] (IAsyncOperation<AudioVideoCaptureDevice^> ^operation, Windows::Foundation::AsyncStatus status)
    {
		tsk_mutex_lock(m_hMutex);
		if(m_bStarted)
		{

			if(status == Windows::Foundation::AsyncStatus::Completed)
			{

				TSK_DEBUG_INFO("+[VideoCapturePhone::Prepare] => OpenAsyncOperation started");
            
				auto videoDevice = operation->GetResults();

				m_pVideoOnlyDevice = videoDevice;
				IAudioVideoCaptureDeviceNative *pNativeDevice = NULL; 
				HRESULT hr = reinterpret_cast<IUnknown*>(videoDevice)->QueryInterface(__uuidof(IAudioVideoCaptureDeviceNative), (void**) &pNativeDevice);
            
				if (NULL == pNativeDevice || FAILED(hr))
				{
					throw ref new FailureException("Unable to QI IAudioVideoCaptureDeviceNative");
				}

				// Save off the native device
				m_pVideoDevice = pNativeDevice;

				// Set Fps
				CameraCapturePropertyRange^ cameraCapturePropertyRange = m_pVideoOnlyDevice->GetSupportedPropertyRange(m_eCameraLocation, KnownCameraAudioVideoProperties::VideoFrameRate);
				if(cameraCapturePropertyRange)
				{
					try
					{
						Windows::Foundation::IPropertyValue^ vMin = dynamic_cast<Windows::Foundation::IPropertyValue^>(cameraCapturePropertyRange->Min);
						Windows::Foundation::IPropertyValue^ vMax = dynamic_cast<Windows::Foundation::IPropertyValue^>(cameraCapturePropertyRange->Max);
						UINT32 nFps = TSK_CLAMP(vMin->GetUInt32(), (UINT32)TMEDIA_PRODUCER(m_pWrappedPlugin)->video.fps, vMax->GetUInt32());
						m_pVideoOnlyDevice->SetProperty(KnownCameraAudioVideoProperties::VideoFrameRate, nFps);
					}
					catch(...){ }
				}
				
				// Set Camera Rotation
				try
				{
					m_pVideoOnlyDevice->SetProperty(
							KnownCameraGeneralProperties::EncodeWithOrientation, 
							m_eCameraLocation == Windows::Phone::Media::Capture::CameraSensorLocation::Back ? 90 : -90
						);
				}
				catch(...){  }

				// Create the sink
				MakeAndInitialize<CaptureSampleSink>(&(m_pVideoSink), m_pWrappedPlugin);
				pNativeDevice->SetVideoSampleSink(m_pVideoSink);

				// Use the same encoding format as in VideoMediaStreamSource.cs
				videoDevice->VideoEncodingFormat = CameraCaptureVideoFormat::H264;

				SetEvent(m_hStartCompleted);

				// Start recording to our sink
				m_pVideoCaptureAction = videoDevice->StartRecordingToSinkAsync();
				m_pVideoCaptureAction->Completed = ref new AsyncActionCompletedHandler([this] (IAsyncAction ^asyncInfo, Windows::Foundation::AsyncStatus status)
				{
					if(status == Windows::Foundation::AsyncStatus::Completed)
					{
						TSK_DEBUG_INFO("[VideoCapturePhone::Prepare] => StartRecordingToSinkAsync completed");
					}
					else if(status == Windows::Foundation::AsyncStatus::Error || status == Windows::Foundation::AsyncStatus::Canceled)
					{
						TSK_DEBUG_INFO("[VideoCapturePhone::Prepare] => StartRecordingToSinkAsync did not complete");
					} 
				});

				TSK_DEBUG_INFO("-[VideoCapturePhone::Prepare] => OpenAsyncOperation Completed");
			}
			else if(status == Windows::Foundation::AsyncStatus::Canceled)
			{
				TSK_DEBUG_INFO("[VideoCapturePhone::Prepare] => OpenAsyncOperation Canceled");
			}
			else if(status == Windows::Foundation::AsyncStatus::Error)
			{
				TSK_DEBUG_INFO("[VideoCapturePhone::Prepare] => OpenAsyncOperation encountered an error");
			}
		}

		tsk_mutex_unlock(m_hMutex);
    });
	
bail:
	tsk_mutex_unlock(m_hMutex);

    return (m_bStarted ? 0 : -2);
}

int VideoCapturePhone::Pause()
{
	tsk_mutex_lock(m_hMutex);

	if(m_bStarted)
	{

	}

	tsk_mutex_unlock(m_hMutex);

	return 0;
}

int VideoCapturePhone::Stop()
{
	tsk_mutex_lock(m_hMutex);

	TSK_DEBUG_INFO("+[VideoCapturePhone::Stop] => Trying to stop capture");
    if (m_pVideoOnlyDevice)
    {
        TSK_DEBUG_INFO("Destroying VideoCaptureDevice");

        try
        {
			if(m_bStarted)
			{
				m_pVideoOnlyDevice->StopRecordingAsync()->Completed = ref new AsyncActionCompletedHandler([this] (IAsyncAction ^action, Windows::Foundation::AsyncStatus status){
					if(status == Windows::Foundation::AsyncStatus::Completed)
					{
						TSK_DEBUG_INFO("[VideoCapturePhone::StopRecordingAsync]  Video successfully stopped");
					}
					else
					{
						TSK_DEBUG_INFO("[VideoCapturePhone::StopRecordingAsync] Error occurred while stopping recording");
					}
					m_pVideoCaptureAction = nullptr;
					m_pVideoOnlyDevice = nullptr;
					m_bStarted = false;
					SetEvent(m_hStopCompleted);
				});
			}
        }
        catch(...)
        {
            // A Platform::ObjectDisposedException can be raised if the app has had its access
            // to video revoked (most commonly when the app is going out of the foreground)
            TSK_DEBUG_ERROR("Exception caught while destroying video capture");
            m_pVideoCaptureAction = nullptr;
            m_pVideoOnlyDevice = nullptr;
            m_bStarted = false;
            SetEvent(m_hStopCompleted);
        }

        if (m_pVideoDevice)
        {
            m_pVideoDevice->Release();
            m_pVideoDevice = NULL;
        }

        if (m_pVideoSink)
        {
            m_pVideoSink->Release();
            m_pVideoSink = NULL;
        }
    }
	else
	{
		m_bStarted = false;
	}

	TSK_DEBUG_INFO("-[VideoCapturePhone::Stop] => finished stopping capture\n");

	// will be prepared again before next start()
	UnPrepare();

	tsk_mutex_unlock(m_hMutex);

	return 0;
}

void VideoCapturePhone::SetCameraLocation(Windows::Phone::Media::Capture::CameraSensorLocation cameraLocation)
{
	if(m_eCameraLocation != cameraLocation)
	{
		if(m_bStarted)
		{
			ToggleCamera();
		}
		else
		{
			m_eCameraLocation = cameraLocation;
		}
	}
}

int VideoCapturePhone::UnPrepare()
{
	tsk_mutex_lock(m_hMutex);

	if(m_bStarted)
	{
		ResetEvent(m_hStopCompleted);
		Stop();
		DWORD waitResult = WaitForSingleObjectEx(m_hStopCompleted, 5000, FALSE);
		if(waitResult != WAIT_OBJECT_0)
		{
			TSK_DEBUG_ERROR("Failed to stop video producer");
		}
	}

	if (m_pVideoDevice)
    {
        m_pVideoDevice->Release();
        m_pVideoDevice = NULL;
    }

    if (m_pVideoSink)
    {
        m_pVideoSink->Release();
        m_pVideoSink = NULL;
    }

	m_pOpenOperation = nullptr;

	m_bPrepared = false;

	tsk_mutex_unlock(m_hMutex);

	return 0;
}

void VideoCapturePhone::ToggleCamera()
{
    tsk_mutex_lock(m_hMutex);

    if(m_ToggleThread)
    {
        m_ToggleThread->Cancel();
        m_ToggleThread->Close();
        m_ToggleThread = nullptr;
    }

    m_ToggleThread = ThreadPool::RunAsync(ref new WorkItemHandler(this, &VideoCapturePhone::ToggleCameraThread), WorkItemPriority::High, WorkItemOptions::TimeSliced);

	tsk_mutex_unlock(m_hMutex);
}


void VideoCapturePhone::ToggleCameraThread(Windows::Foundation::IAsyncAction^ operation)
{
    TSK_DEBUG_INFO("+[VideoCapturePhone::ToggleCamera] => Toggling camera");

    ResetEvent(m_hStopCompleted);
    Stop();
    DWORD waitResult = WaitForSingleObjectEx(m_hStopCompleted, INFINITE, FALSE);
    if(waitResult == WAIT_OBJECT_0)
    {
        ResetEvent(m_hStartCompleted);
        if(m_eCameraLocation == Windows::Phone::Media::Capture::CameraSensorLocation::Back)
        {
            m_eCameraLocation = Windows::Phone::Media::Capture::CameraSensorLocation::Front;
        }
        else
        {
            m_eCameraLocation = Windows::Phone::Media::Capture::CameraSensorLocation::Back;
        }
		Prepare(m_pWrappedPlugin);
		Start();
    }
    else
    {
        throw ref new Platform::Exception(HRESULT_FROM_WIN32(waitResult), L"Error waiting for capture to stop when toggling cameras");
    }

    waitResult = WaitForSingleObjectEx(m_hStartCompleted, INFINITE, FALSE);
    if(waitResult == WAIT_OBJECT_0)
    {
        // CameraLocationChanged(newCameraLocation);
    }
    else
    {
        throw ref new Platform::Exception(HRESULT_FROM_WIN32(waitResult), L"Error waiting for capture to start when toggling cameras");
    }
	TSK_DEBUG_INFO("-[VideoCapturePhone::ToggleCamera] => Toggling camera");
}

#endif /* TDAV_UNDER_WINDOWS_PHONE */


//
//	Windows Media video producer object definition
//
/* constructor */
static tsk_object_t* tdav_producer_winm_ctor(tsk_object_t * self, va_list * app)
{
	tdav_producer_winm_t *producer = (tdav_producer_winm_t *)self;
	if(producer){
		/* init base */
		tmedia_producer_init(TMEDIA_PRODUCER(producer));
		TMEDIA_PRODUCER(producer)->video.chroma = tmedia_chroma_yuv420p; // To avoid chroma conversion
		/* init self with default values*/

		TMEDIA_PRODUCER(producer)->video.fps = 15;
		TMEDIA_PRODUCER(producer)->video.width = 352;
		TMEDIA_PRODUCER(producer)->video.height = 288;

#if TDAV_UNDER_WINDOWS_PHONE
		producer->videoCapturePhone = ref new VideoCapturePhone();
#endif
	}
	return self;
}
/* destructor */
static tsk_object_t* tdav_producer_winm_dtor(tsk_object_t * self)
{ 
	tdav_producer_winm_t *producer = (tdav_producer_winm_t *)self;
	if(producer){
		/* stop */
		//if(producer->started){
			tdav_producer_winm_stop((tmedia_producer_t*)self);
		//}

		/* deinit base */
		tmedia_producer_deinit(TMEDIA_PRODUCER(producer));
		/* deinit self */
#if TDAV_UNDER_WINDOWS_PHONE
		if(producer->videoCapturePhone)
		{
			delete producer->videoCapturePhone;
		}
#endif
	}

	return self;
}
/* object definition */
static const tsk_object_def_t tdav_producer_winm_def_s = 
{
	sizeof(tdav_producer_winm_t),
	tdav_producer_winm_ctor, 
	tdav_producer_winm_dtor,
	tsk_null, 
};
/* plugin definition*/
static const tmedia_producer_plugin_def_t tdav_producer_winm_plugin_def_s = 
{
	&tdav_producer_winm_def_s,

	tmedia_video,
	"Microsoft Windows Media producer (Video)",

	tdav_producer_winm_set,
	tdav_producer_winm_prepare,
	tdav_producer_winm_start,
	tdav_producer_winm_pause,
	tdav_producer_winm_stop
};
const tmedia_producer_plugin_def_t *tdav_producer_winm_plugin_def_t = &tdav_producer_winm_plugin_def_s;

#endif /* HAVE_WINM */
