/* Copyright (C) 2015 Mamadou DIOP
* Copyright (C) 2015 Doubango Telecom <http://www.doubango.org>
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
// Microsoft Duplication Desktop producer for Win8+: https://msdn.microsoft.com/en-us/library/windows/desktop/hh404487(v=VS.85).aspx
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved
#include "plugin_win_dd_config.h"

#include "tinymedia/tmedia_defaults.h"
#include "tinymedia/tmedia_producer.h"

#include "tsk_time.h"
#include "tsk_string.h"
#include "tsk_thread.h"
#include "tsk_safeobj.h"
#include "tsk_debug.h"

#include "internals/DisplayManager.h"
#include "internals/DuplicationManager.h"
#include "internals/OutputManager.h"
#include "internals/ThreadManager.h"

#include <Windows.h>

#define DD_DEBUG_INFO(FMT, ...) TSK_DEBUG_INFO("[DESKTOP DUPLICATION] " FMT, ##__VA_ARGS__)
#define DD_DEBUG_WARN(FMT, ...) TSK_DEBUG_WARN("[DESKTOP DUPLICATION] " FMT, ##__VA_ARGS__)
#define DD_DEBUG_ERROR(FMT, ...) TSK_DEBUG_ERROR("[DESKTOP DUPLICATION] " FMT, ##__VA_ARGS__)
#define DD_DEBUG_FATAL(FMT, ...) TSK_DEBUG_FATAL("[DESKTOP DUPLICATION] " FMT, ##__VA_ARGS__)

#define DD_CHECK_HR(x) { HRESULT __hr__ = (x); if (FAILED(__hr__)) { DD_DEBUG_ERROR("Operation Failed (%08x)", __hr__); goto bail; } }

#if !defined(DD_DDPROC_THREAD_TIMEOUT)
#	define DD_DDPROC_THREAD_TIMEOUT 1500
#endif

//
//	plugin_win_dd_producer_t
//
typedef struct plugin_win_dd_producer_s {
    TMEDIA_DECLARE_PRODUCER;

    bool bStarted, bPrepared, bMuted, bWindowHooked, bThreadTerminationDelayed;
    tsk_thread_handle_t* ppTread[1];

    OUTPUTMANAGER *pOutMgr;
    THREADMANAGER *pThreadMgr;

    // Window handles
    HWND hwndPreview;
    WNDPROC wndPreviewProc;
    HWND hwndSrc;

    // Synchronization
    HANDLE hlUnexpectedErrorEvent;
    HANDLE hlExpectedErrorEvent;
    HANDLE hlOcclutionEvent;
    HANDLE hlTerminateThreadsEvent;

    HCURSOR hcCursor;
}
plugin_win_dd_producer_t;


// Forward declarations
static int _plugin_win_dd_producer_unprepare(plugin_win_dd_producer_t* pSelf, bool bCleanup = false);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static HRESULT HookWindow(struct plugin_win_dd_producer_s *pSelf, HWND hWnd);
static HRESULT UnhookWindow(struct plugin_win_dd_producer_s *pSelf);

DWORD WINAPI DDProc(_In_ void* Param);
void DisplayMsg(_In_ LPCWSTR Str, _In_ LPCWSTR Title, HRESULT hr);
_Post_satisfies_(return != DUPL_RETURN_SUCCESS)
DUPL_RETURN ProcessFailure(_In_opt_ ID3D11Device* Device, _In_ LPCWSTR Str, _In_ LPCWSTR Title, HRESULT hr, _In_opt_z_ HRESULT* ExpectedErrors);
static void* TSK_STDCALL DDThread(void *pArg);

//
// Class for progressive waits
//
typedef struct {
    UINT    WaitTime;
    UINT    WaitCount;
} WAIT_BAND;

#define WAIT_BAND_COUNT 3
#define WAIT_BAND_STOP 0

class DYNAMIC_WAIT
{
public:
    DYNAMIC_WAIT();
    ~DYNAMIC_WAIT();

    void Wait();

private:

    static const WAIT_BAND   m_WaitBands[WAIT_BAND_COUNT];

    // Period in seconds that a new wait call is considered part of the same wait sequence
    static const UINT       m_WaitSequenceTimeInSeconds = 2;

    UINT                    m_CurrentWaitBandIdx;
    UINT                    m_WaitCountInCurrentBand;
    LARGE_INTEGER           m_QPCFrequency;
    LARGE_INTEGER           m_LastWakeUpTime;
    BOOL                    m_QPCValid;
};
const WAIT_BAND DYNAMIC_WAIT::m_WaitBands[WAIT_BAND_COUNT] = {
    { 250, 20 },
    { 2000, 60 },
    { 5000, WAIT_BAND_STOP }   // Never move past this band
};

/* ============ Video DD Producer Interface ================= */
static int plugin_win_dd_producer_set(tmedia_producer_t *p_self, const tmedia_param_t* pc_param)
{
    int ret = -1;
    plugin_win_dd_producer_t* p_dd = (plugin_win_dd_producer_t*)p_self;

    if (!p_dd || !pc_param) {
        DD_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if (pc_param->value_type == tmedia_pvt_int64) {
        if (tsk_striequals(pc_param->key, "local-hwnd") || tsk_striequals(pc_param->key, "preview-hwnd")) {
            HWND hwnd = (HWND)*((int64_t*)pc_param->value);
            ret = SUCCEEDED(HookWindow(p_dd, hwnd)) ? 0 : -1;
        }
        else if (tsk_striequals(pc_param->key, "src-hwnd")) {
            p_dd->hwndSrc = (HWND)*((int64_t*)pc_param->value);
            ret = 0;
        }
    }
    else if (pc_param->value_type == tmedia_pvt_int32) {
        if (tsk_striequals(pc_param->key, "mute")) {
            p_dd->bMuted = (TSK_TO_INT32((uint8_t*)pc_param->value) != 0);
            ret = 0;
        }
    }

    return ret;
}

static int plugin_win_dd_producer_prepare(tmedia_producer_t* self, const tmedia_codec_t* codec)
{
    plugin_win_dd_producer_t* pSelf = (plugin_win_dd_producer_t*)self;
    HRESULT hr = S_OK;

    if (!pSelf || !codec && codec->plugin) {
        DD_DEBUG_ERROR("Invalid parameter");
        DD_CHECK_HR(hr = E_UNEXPECTED);
    }

    if (pSelf->bPrepared) {
        DD_DEBUG_WARN("DD video producer already prepared");
        DD_CHECK_HR(hr = E_UNEXPECTED);
    }

    if (pSelf->bThreadTerminationDelayed) {
        DD_DEBUG_INFO("Thread termination was delayed ...cleanup now");
        if (_plugin_win_dd_producer_unprepare(pSelf, true/*cleanup?*/) != 0) {
            DD_CHECK_HR(hr = E_UNEXPECTED);
        }
    }

    TMEDIA_PRODUCER(pSelf)->video.fps = TMEDIA_CODEC_VIDEO(codec)->out.fps;
    TMEDIA_PRODUCER(pSelf)->video.width = TMEDIA_CODEC_VIDEO(codec)->out.width;
    TMEDIA_PRODUCER(pSelf)->video.height = TMEDIA_CODEC_VIDEO(codec)->out.height;
    TMEDIA_PRODUCER(pSelf)->encoder.codec_id = tmedia_codec_id_none; // means RAW frames as input

    DD_DEBUG_INFO("DD video producer: fps=%d, width=%d, height=%d",
                  TMEDIA_PRODUCER(pSelf)->video.fps,
                  TMEDIA_PRODUCER(pSelf)->video.width,
                  TMEDIA_PRODUCER(pSelf)->video.height);

    // Event used by the threads to signal an unexpected error and we want to quit the app
    if (!pSelf->hlUnexpectedErrorEvent && !(pSelf->hlUnexpectedErrorEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr))) {
        ProcessFailure(nullptr, L"UnexpectedErrorEvent creation failed", L"Error", E_UNEXPECTED);
        DD_CHECK_HR(hr = E_UNEXPECTED);
    }

    // Event for when a thread encounters an expected error
    if (!pSelf->hlExpectedErrorEvent && !(pSelf->hlExpectedErrorEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr))) {
        ProcessFailure(nullptr, L"ExpectedErrorEvent creation failed", L"Error", E_UNEXPECTED);
        DD_CHECK_HR(hr = E_UNEXPECTED);
    }

    // Event for Occlution
    if (!pSelf->hlOcclutionEvent && !(pSelf->hlOcclutionEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr))) {
        ProcessFailure(nullptr, L"OcclutionEvent creation failed", L"Error", E_UNEXPECTED);
        DD_CHECK_HR(hr = E_UNEXPECTED);
    }

    // Event to tell spawned threads to quit
    if (!pSelf->hlTerminateThreadsEvent && !(pSelf->hlTerminateThreadsEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr))) {
        ProcessFailure(nullptr, L"TerminateThreadsEvent creation failed", L"Error", E_UNEXPECTED);
        DD_CHECK_HR(hr = E_UNEXPECTED);
    }

    // Load simple cursor
    if (!pSelf->hcCursor && !(pSelf->hcCursor = LoadCursor(nullptr, IDC_ARROW))) {
        ProcessFailure(nullptr, L"Cursor load failed", L"Error", E_UNEXPECTED);
        DD_CHECK_HR(hr = E_UNEXPECTED);
    }

    if (!pSelf->pOutMgr && !(pSelf->pOutMgr = new OUTPUTMANAGER())) {
        ProcessFailure(nullptr, L"Out manager allocation failed", L"Error", E_OUTOFMEMORY);
        DD_CHECK_HR(hr = E_OUTOFMEMORY);
    }

    if (!pSelf->pThreadMgr && !(pSelf->pThreadMgr = new THREADMANAGER())) {
        ProcessFailure(nullptr, L"Thread managed allocation failed", L"Error", E_OUTOFMEMORY);
        DD_CHECK_HR(hr = E_OUTOFMEMORY);
    }

bail:
    pSelf->bPrepared = SUCCEEDED(hr);
    return SUCCEEDED(hr) ? 0 : -1;
}

static int plugin_win_dd_producer_start(tmedia_producer_t* self)
{
    plugin_win_dd_producer_t* pSelf = (plugin_win_dd_producer_t*)self;
    HRESULT hr = S_OK;

    if (!pSelf) {
        DD_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if (pSelf->bStarted) {
        DD_DEBUG_INFO("Producer already started");
        goto bail;
    }
    if (!pSelf->bPrepared) {
        DD_DEBUG_ERROR("Producer not prepared");
        DD_CHECK_HR(hr = E_UNEXPECTED);
    }

    DD_CHECK_HR(hr = HookWindow(pSelf, pSelf->hwndPreview));

    // Start asynchronous watcher thread
    pSelf->bStarted = true;
    int ret = tsk_thread_create(&pSelf->ppTread[0], DDThread, pSelf);
    if (ret != 0) {
        TSK_DEBUG_ERROR("Failed to create thread");
        pSelf->bStarted = false;
        if (pSelf->ppTread[0]) {
            tsk_thread_join(&pSelf->ppTread[0]);
        }
        DD_CHECK_HR(hr = E_UNEXPECTED);
    }

bail:
    if (FAILED(hr)) {
        UnhookWindow(pSelf);
        return -1;
    }
    pSelf->bStarted = true;
    return 0;
}

static int plugin_win_dd_producer_pause(tmedia_producer_t* self)
{
    plugin_win_dd_producer_t* pSelf = (plugin_win_dd_producer_t*)self;

    if (!pSelf) {
        DD_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if (!pSelf->bStarted) {
        DD_DEBUG_INFO("MF video producer not started");
    }

    return 0;
}

static int plugin_win_dd_producer_stop(tmedia_producer_t* self)
{
    plugin_win_dd_producer_t* pSelf = (plugin_win_dd_producer_t*)self;

    if (!pSelf) {
        DD_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    pSelf->bStarted = false;

    UnhookWindow(pSelf);

    if (pSelf->hlTerminateThreadsEvent) {
        SetEvent(pSelf->hlTerminateThreadsEvent);
    }
    if (pSelf->ppTread[0]) {
        tsk_thread_join(&pSelf->ppTread[0]);
    }

    // next start() will be called after prepare()
    int ret = _plugin_win_dd_producer_unprepare(pSelf);

    return ret;
}

static int _plugin_win_dd_producer_unprepare(plugin_win_dd_producer_t* pSelf, bool bCleanup /*= false*/)
{
    HRESULT hr = S_OK;

    if (!pSelf) {
        DD_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if (pSelf->bStarted) {
        DD_CHECK_HR(hr = E_UNEXPECTED);
    }

    pSelf->bThreadTerminationDelayed = false;

    // Thread manager must be destroyed before the events and output manager
    if (pSelf->pThreadMgr) {
        // if we are cleaning the producer then all threads must exit only when all threads are destroyed
        // https://code.google.com/p/sincity/issues/detail?id=7
        if (pSelf->pThreadMgr->WaitForThreadTermination(bCleanup ? INFINITE : DD_DDPROC_THREAD_TIMEOUT) == true) {
            delete pSelf->pThreadMgr;
            pSelf->pThreadMgr = nullptr;
        }
        else {
            // Thread wait timedout
            DD_DEBUG_WARN("DDProc thread termination delayed");
            pSelf->bThreadTerminationDelayed = true;
        }
    }

    if (!pSelf->bThreadTerminationDelayed) {
        if (pSelf->hlUnexpectedErrorEvent) {
            CloseHandle(pSelf->hlUnexpectedErrorEvent);
            pSelf->hlUnexpectedErrorEvent = nullptr;
        }
        if (pSelf->hlExpectedErrorEvent) {
            CloseHandle(pSelf->hlExpectedErrorEvent);
            pSelf->hlExpectedErrorEvent = nullptr;
        }
        if (pSelf->hlOcclutionEvent) {
            CloseHandle(pSelf->hlOcclutionEvent);
            pSelf->hlOcclutionEvent = nullptr;
        }
        if (pSelf->hlTerminateThreadsEvent) {
            CloseHandle(pSelf->hlTerminateThreadsEvent);
            pSelf->hlTerminateThreadsEvent = nullptr;
        }

        if (pSelf->hcCursor) {
            DestroyCursor(pSelf->hcCursor);
            pSelf->hcCursor = nullptr;
        }

        if (pSelf->pOutMgr) {
            delete pSelf->pOutMgr;
            pSelf->pOutMgr = nullptr;
        }
    }

    pSelf->bPrepared = false;

bail:
    return 0;
}

static HRESULT HookWindow(struct plugin_win_dd_producer_s *pSelf, HWND hWnd)
{
    HRESULT hr = S_OK;

    DD_CHECK_HR(hr = UnhookWindow(pSelf));

    if ((pSelf->hwndPreview = hWnd)) {
        pSelf->wndPreviewProc = (WNDPROC)SetWindowLongPtr(pSelf->hwndPreview, GWLP_WNDPROC, (LONG_PTR)WndProc);
        if (!pSelf->wndPreviewProc) {
            DD_DEBUG_ERROR("HookWindowLongPtr() failed with errcode=%d", GetLastError());
            DD_CHECK_HR(hr = E_FAIL);
        }
        SetProp(pSelf->hwndPreview, L"Self", pSelf);
        pSelf->bWindowHooked = true;
    }
bail:
    return S_OK;
}

static HRESULT UnhookWindow(struct plugin_win_dd_producer_s *pSelf)
{
    if (pSelf->hwndPreview && pSelf->wndPreviewProc) {
        SetWindowLongPtr(pSelf->hwndPreview, GWLP_WNDPROC, (LONG_PTR)pSelf->wndPreviewProc);
        pSelf->wndPreviewProc = NULL;
    }
    if (pSelf->hwndPreview) {
        ::InvalidateRect(pSelf->hwndPreview, NULL, FALSE);
    }
    pSelf->bWindowHooked = false;
    return S_OK;
}

//
//      Windows Desktop Duplication producer object definition
//
/* constructor */
static tsk_object_t* plugin_win_dd_producer_ctor(tsk_object_t * self, va_list * app)
{
    plugin_win_dd_producer_t *pSelf = (plugin_win_dd_producer_t *)self;
    if (pSelf) {
        /* init base */
        tmedia_producer_init(TMEDIA_PRODUCER(pSelf));

        /* init self with default values*/
        TMEDIA_PRODUCER(pSelf)->encoder.codec_id = tmedia_codec_id_none; // means RAW frames as input
        TMEDIA_PRODUCER(pSelf)->video.chroma = tmedia_chroma_rgb32;
        TMEDIA_PRODUCER(pSelf)->video.fps = 15;
        TMEDIA_PRODUCER(pSelf)->video.width = 352;
        TMEDIA_PRODUCER(pSelf)->video.height = 288;

        DD_DEBUG_INFO("Create Microsoft Desktop Duplication producer");
    }
    return self;
}
/* destructor */
static tsk_object_t* plugin_win_dd_producer_dtor(tsk_object_t * self)
{
    plugin_win_dd_producer_t *pSelf = (plugin_win_dd_producer_t *)self;
    if (pSelf) {
        /* stop */
        if (pSelf->bStarted) {
            plugin_win_dd_producer_stop(TMEDIA_PRODUCER(pSelf));
        }

        /* deinit base */
        tmedia_producer_deinit(TMEDIA_PRODUCER(pSelf));
        /* deinit self */
        _plugin_win_dd_producer_unprepare(pSelf, true/*cleanup*/);

        DD_DEBUG_INFO("*** WinDD producer destroyed ***");
    }

    return self;
}
/* object definition */
static const tsk_object_def_t plugin_win_dd_producer_def_s = {
    sizeof(plugin_win_dd_producer_t),
    plugin_win_dd_producer_ctor,
    plugin_win_dd_producer_dtor,
    tsk_null,
};
/* plugin definition*/
static const tmedia_producer_plugin_def_t plugin_win_dd_producer_plugin_def_s = {
    &plugin_win_dd_producer_def_s,

    tmedia_bfcp_video,
    "Microsoft Windows Desktop Duplication producer (Video)",

    plugin_win_dd_producer_set,
    plugin_win_dd_producer_prepare,
    plugin_win_dd_producer_start,
    plugin_win_dd_producer_pause,
    plugin_win_dd_producer_stop
};
const tmedia_producer_plugin_def_t *plugin_win_dd_producer_plugin_def_t = &plugin_win_dd_producer_plugin_def_s;





// Below are lists of errors expect from Dxgi API calls when a transition event like mode change, PnpStop, PnpStart
// desktop switch, TDR or session disconnect/reconnect. In all these cases we want the application to clean up the threads that process
// the desktop updates and attempt to recreate them.
// If we get an error that is not on the appropriate list then we exit the application

// These are the errors we expect from general Dxgi API due to a transition
HRESULT SystemTransitionsExpectedErrors[] = {
    DXGI_ERROR_DEVICE_REMOVED,
    DXGI_ERROR_ACCESS_LOST,
    static_cast<HRESULT>(WAIT_ABANDONED),
    S_OK                                    // Terminate list with zero valued HRESULT
};

// These are the errors we expect from IDXGIOutput1::DuplicateOutput due to a transition
HRESULT CreateDuplicationExpectedErrors[] = {
    DXGI_ERROR_DEVICE_REMOVED,
    static_cast<HRESULT>(E_ACCESSDENIED),
    DXGI_ERROR_UNSUPPORTED,
    DXGI_ERROR_SESSION_DISCONNECTED,
    S_OK                                    // Terminate list with zero valued HRESULT
};

// These are the errors we expect from IDXGIOutputDuplication methods due to a transition
HRESULT FrameInfoExpectedErrors[] = {
    DXGI_ERROR_DEVICE_REMOVED,
    DXGI_ERROR_ACCESS_LOST,
    S_OK                                    // Terminate list with zero valued HRESULT
};

// These are the errors we expect from IDXGIAdapter::EnumOutputs methods due to outputs becoming stale during a transition
HRESULT EnumOutputsExpectedErrors[] = {
    DXGI_ERROR_NOT_FOUND,
    S_OK                                    // Terminate list with zero valued HRESULT
};



_Post_satisfies_(return != DUPL_RETURN_SUCCESS)
DUPL_RETURN ProcessFailure(_In_opt_ ID3D11Device* Device, _In_ LPCWSTR Str, _In_ LPCWSTR Title, HRESULT hr, _In_opt_z_ HRESULT* ExpectedErrors)
{
    HRESULT TranslatedHr;

    // On an error check if the DX device is lost
    if (Device) {
        HRESULT DeviceRemovedReason = Device->GetDeviceRemovedReason();

        switch (DeviceRemovedReason) {
        case DXGI_ERROR_DEVICE_REMOVED:
        case DXGI_ERROR_DEVICE_RESET:
        case static_cast<HRESULT>(E_OUTOFMEMORY) : {
            // Our device has been stopped due to an external event on the GPU so map them all to
            // device removed and continue processing the condition
            TranslatedHr = DXGI_ERROR_DEVICE_REMOVED;
            break;
        }

        case S_OK: {
            // Device is not removed so use original error
            TranslatedHr = hr;
            break;
        }

        default: {
            // Device is removed but not a error we want to remap
            TranslatedHr = DeviceRemovedReason;
        }
        }
    }
    else {
        TranslatedHr = hr;
    }

    // Check if this error was expected or not
    if (ExpectedErrors) {
        HRESULT* CurrentResult = ExpectedErrors;

        while (*CurrentResult != S_OK) {
            if (*(CurrentResult++) == TranslatedHr) {
                return DUPL_RETURN_ERROR_EXPECTED;
            }
        }
    }

    // Error was not expected so display the message box
    DisplayMsg(Str, Title, TranslatedHr);

    return DUPL_RETURN_ERROR_UNEXPECTED;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case WM_DESTROY: {
        PostQuitMessage(0);
        break;
    }
    case WM_SIZE: {
        // Tell output manager that window size has changed
        plugin_win_dd_producer_t* pSelf = static_cast<plugin_win_dd_producer_t*>(GetProp(hWnd, L"Self"));
        if (pSelf && pSelf->pOutMgr) {
            pSelf->pOutMgr->WindowResize();
        }
        break;
    }
    case OCCLUSION_STATUS_MSG: {
        plugin_win_dd_producer_t* pSelf = static_cast<plugin_win_dd_producer_t*>(GetProp(hWnd, L"Self"));
        if (pSelf && pSelf->hlOcclutionEvent) {
            SetEvent(pSelf->hlOcclutionEvent);
        }
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

//
// Entry point for new duplication threads
//
DWORD WINAPI DDProc(_In_ void* Param)
{
    DD_DEBUG_INFO("DDProc (producer) - ENTER");

    // Classes
    DISPLAYMANAGER DispMgr;
    DUPLICATIONMANAGER DuplMgr;

    // D3D objects
    ID3D11Texture2D* SharedSurf = nullptr;
    IDXGIKeyedMutex* KeyMutex = nullptr;

    // Data passed in from thread creation
    THREAD_DATA* TData = reinterpret_cast<THREAD_DATA*>(Param);

    // Get desktop
    DUPL_RETURN Ret;
    HDESK CurrentDesktop = nullptr;
    CurrentDesktop = OpenInputDesktop(0, FALSE, GENERIC_ALL);
    if (!CurrentDesktop) {
        // We do not have access to the desktop so request a retry
        SetEvent(TData->ExpectedErrorEvent);
        Ret = DUPL_RETURN_ERROR_EXPECTED;
        goto Exit;
    }

    // Attach desktop to this thread
    bool DesktopAttached = SetThreadDesktop(CurrentDesktop) != 0;
    CloseDesktop(CurrentDesktop);
    CurrentDesktop = nullptr;
    if (!DesktopAttached) {
        // We do not have access to the desktop so request a retry
        Ret = DUPL_RETURN_ERROR_EXPECTED;
        goto Exit;
    }

    // New display manager
    DispMgr.InitD3D(&TData->DxRes);

    // FPS manager
    uint64_t TimeNow, TimeLastFrame = 0;
    const uint64_t TimeFrameDuration = 1000 / TData->Producer->video.fps;

    // Obtain handle to sync shared Surface
    HRESULT hr = TData->DxRes.Device->OpenSharedResource(TData->TexSharedHandle, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&SharedSurf));
    if (FAILED(hr)) {
        Ret = ProcessFailure(TData->DxRes.Device, L"Opening shared texture failed", L"Error", hr, SystemTransitionsExpectedErrors);
        goto Exit;
    }

    hr = SharedSurf->QueryInterface(__uuidof(IDXGIKeyedMutex), reinterpret_cast<void**>(&KeyMutex));
    if (FAILED(hr)) {
        Ret = ProcessFailure(nullptr, L"Failed to get keyed mutex interface in spawned thread", L"Error", hr);
        goto Exit;
    }

    // Make duplication manager
    Ret = DuplMgr.InitDupl(TData->DxRes.Device, TData->DxRes.Context ,TData->Output);
    if (Ret != DUPL_RETURN_SUCCESS) {
        goto Exit;
    }

    // Get output description
    DXGI_OUTPUT_DESC DesktopDesc;
    RtlZeroMemory(&DesktopDesc, sizeof(DXGI_OUTPUT_DESC));
    DuplMgr.GetOutputDesc(&DesktopDesc);

    // Main duplication loop
    bool WaitToProcessCurrentFrame = false;
    FRAME_DATA CurrentData;

    while (TData->Producer->is_started &&  (WaitForSingleObjectEx(TData->TerminateThreadsEvent, 0, FALSE) == WAIT_TIMEOUT)) {
        if (!WaitToProcessCurrentFrame) {
            // Get new frame from desktop duplication
            bool TimeOut;
            Ret = DuplMgr.GetFrame(&CurrentData, &TimeOut);
            if (Ret != DUPL_RETURN_SUCCESS) {
                // An error occurred getting the next frame drop out of loop which
                // will check if it was expected or not
                break;
            }

            // Check for timeout
            if (TimeOut) {
                // No new frame at the moment
                continue;
            }
        }

        // We have a new frame so try and process it
        // Try to acquire keyed mutex in order to access shared surface
        hr = KeyMutex->AcquireSync(0, 1000);
        if (hr == static_cast<HRESULT>(WAIT_TIMEOUT)) {
            // Can't use shared surface right now, try again later
            WaitToProcessCurrentFrame = true;
            continue;
        }
        else if (FAILED(hr)) {
            // Generic unknown failure
            Ret = ProcessFailure(TData->DxRes.Device, L"Unexpected error acquiring KeyMutex", L"Error", hr, SystemTransitionsExpectedErrors);
            DuplMgr.DoneWithFrame();
            break;
        }

        // We can now process the current frame
        WaitToProcessCurrentFrame = false;

        // Get mouse info
        Ret = DuplMgr.GetMouse(TData->PtrInfo, &(CurrentData.FrameInfo), TData->OffsetX, TData->OffsetY);
        if (Ret != DUPL_RETURN_SUCCESS) {
            DuplMgr.DoneWithFrame();
            KeyMutex->ReleaseSync(1);
            break;
        }

        // Process new frame
        Ret = DispMgr.ProcessFrame(&CurrentData, SharedSurf, TData->OffsetX, TData->OffsetY, &DesktopDesc);
        if (Ret != DUPL_RETURN_SUCCESS) {
            DuplMgr.DoneWithFrame();
            KeyMutex->ReleaseSync(1);
            break;
        }

        // Release acquired keyed mutex
        hr = KeyMutex->ReleaseSync(1);
        if (FAILED(hr)) {
            Ret = ProcessFailure(TData->DxRes.Device, L"Unexpected error releasing the keyed mutex", L"Error", hr, SystemTransitionsExpectedErrors);
            DuplMgr.DoneWithFrame();
            break;
        }

        // Send Frame Over the Network
        TimeNow = tsk_time_now();
        if ((TimeNow - TimeLastFrame) > TimeFrameDuration) {
            if (!((const plugin_win_dd_producer_t*)TData->Producer)->bMuted) {
                hr = DuplMgr.SendData(const_cast<struct tmedia_producer_s*>(TData->Producer), &CurrentData);
            }
            if (SUCCEEDED(hr)) {
                TimeLastFrame = TimeNow;
            }
        }
#if 0
        else {
            DD_DEBUG_INFO("Skip frame");
        }
#endif

        // Release frame back to desktop duplication
        Ret = DuplMgr.DoneWithFrame();
        if (Ret != DUPL_RETURN_SUCCESS) {
            break;
        }
    }

Exit:
    if (Ret != DUPL_RETURN_SUCCESS) {
        if (Ret == DUPL_RETURN_ERROR_EXPECTED) {
            // The system is in a transition state so request the duplication be restarted
            SetEvent(TData->ExpectedErrorEvent);
        }
        else {
            // Unexpected error so exit the application
            SetEvent(TData->UnexpectedErrorEvent);
        }
    }

    if (SharedSurf) {
        SharedSurf->Release();
        SharedSurf = nullptr;
    }

    if (KeyMutex) {
        KeyMutex->Release();
        KeyMutex = nullptr;
    }

    DD_DEBUG_INFO("DDProc (producer) - EXIT");

    return 0;
}

// Run session async thread
static void* TSK_STDCALL DDThread(void *pArg)
{
    plugin_win_dd_producer_t *pSelf = (plugin_win_dd_producer_t *)pArg;
    HRESULT hr = S_OK;
    INT SingleOutput = -1;

    RECT DeskBounds = {};
    UINT OutputCount = 1;

    bool FirstTime = true;
    bool Occluded = true;
    bool PreviewChanged = false;
    DYNAMIC_WAIT DynamicWait;
    HWND hwndPreview = NULL;

    DD_DEBUG_INFO("DDThread (producer) - ENTER");

    while (pSelf->bStarted) {
        DUPL_RETURN Ret = DUPL_RETURN_SUCCESS;

        // Check if Preview window changed
        PreviewChanged = (hwndPreview != pSelf->hwndPreview);

        if (WaitForSingleObjectEx(pSelf->hlOcclutionEvent, 0, FALSE) == WAIT_OBJECT_0) {
            Occluded = false;
        }
        if (WaitForSingleObjectEx(pSelf->hlUnexpectedErrorEvent, 0, FALSE) == WAIT_OBJECT_0) {
            // Unexpected error occurred so exit the application
            DD_CHECK_HR(hr = E_UNEXPECTED);
        }
        else if (FirstTime || PreviewChanged || WaitForSingleObjectEx(pSelf->hlExpectedErrorEvent, 0, FALSE) == WAIT_OBJECT_0) {
            if (PreviewChanged) {
                hwndPreview = pSelf->hwndPreview;
            }

            if (!FirstTime) {
                // Terminate other threads
                SetEvent(pSelf->hlTerminateThreadsEvent);
                pSelf->pThreadMgr->WaitForThreadTermination();
                ResetEvent(pSelf->hlTerminateThreadsEvent);
                ResetEvent(pSelf->hlExpectedErrorEvent);

                // Clean up
                pSelf->pThreadMgr->Clean();
                pSelf->pOutMgr->CleanRefs();

                // As we have encountered an error due to a system transition we wait before trying again, using this dynamic wait
                // the wait periods will get progressively long to avoid wasting too much system resource if this state lasts a long time
                DynamicWait.Wait();
            }
            else {
                // First time through the loop so nothing to clean up
                FirstTime = false;
            }

            // Re-initialize
            Ret = pSelf->pOutMgr->InitOutput(hwndPreview, SingleOutput, &OutputCount, &DeskBounds);
            if (Ret == DUPL_RETURN_SUCCESS) {
                HANDLE SharedHandle = pSelf->pOutMgr->GetSharedHandle();
                if (SharedHandle) {
                    Ret = pSelf->pThreadMgr->Initialize(SingleOutput, OutputCount, pSelf->hlUnexpectedErrorEvent, pSelf->hlExpectedErrorEvent, pSelf->hlTerminateThreadsEvent, SharedHandle, TMEDIA_PRODUCER(pSelf), &DeskBounds);
                }
                else {
                    DisplayMsg(L"Failed to get handle of shared surface", L"Error", S_OK);
                    Ret = DUPL_RETURN_ERROR_UNEXPECTED;
                }
            }


            // We start off in occluded state and we should immediate get a occlusion status window message
            Occluded = true;
        }
        else {
            // Nothing else to do, so try to present to write out to window if not occluded
            if (!Occluded || !pSelf->bWindowHooked) {
                Ret = pSelf->pOutMgr->UpdateApplicationWindow(pSelf->pThreadMgr->GetPointerInfo(), &Occluded);
            }
        }

        // Check if for errors
        if (Ret != DUPL_RETURN_SUCCESS) {
            if (Ret == DUPL_RETURN_ERROR_EXPECTED) {
                // Some type of system transition is occurring so retry
                SetEvent(pSelf->hlExpectedErrorEvent);
            }
            else {
                // Unexpected error so exit
                DD_CHECK_HR(hr = E_UNEXPECTED);
                break;
            }
        }
    }

bail:

    DD_DEBUG_INFO("DDThread (producer) - BAIL");

#if 0 // Done by unprepare()
    // Make sure all other threads have exited
    if (SetEvent(pSelf->hlTerminateThreadsEvent)) {
        ThreadMgr.WaitForThreadTermination();
    }

    // Clean up
    CloseHandle(pSelf->hlUnexpectedErrorEvent);
    pSelf->hlUnexpectedErrorEvent = NULL;
    CloseHandle(pSelf->hlExpectedErrorEvent);
    pSelf->hlExpectedErrorEvent = NULL;
    CloseHandle(pSelf->hlTerminateThreadsEvent);
    pSelf->hlTerminateThreadsEvent = NULL;
#endif

    DD_DEBUG_INFO("DDThread (producer) - EXIT");

    return NULL;
}

//
// Displays a message
//
void DisplayMsg(_In_ LPCWSTR Str, _In_ LPCWSTR Title, HRESULT hr)
{
    const UINT StringLen = (UINT)(wcslen(Str) + sizeof(" with HRESULT 0x########."));
    wchar_t* OutStr = new wchar_t[StringLen];
    if (!OutStr) {
        return;
    }

    INT LenWritten = swprintf_s(OutStr, StringLen, L"%s with 0x%X.", Str, hr);
    if (LenWritten != -1) {
        DD_DEBUG_ERROR("%ls: %ls", Title, OutStr);
    }

    delete[] OutStr;
}



DYNAMIC_WAIT::DYNAMIC_WAIT() : m_CurrentWaitBandIdx(0), m_WaitCountInCurrentBand(0)
{
    m_QPCValid = QueryPerformanceFrequency(&m_QPCFrequency);
    m_LastWakeUpTime.QuadPart = 0L;
}

DYNAMIC_WAIT::~DYNAMIC_WAIT()
{
}

void DYNAMIC_WAIT::Wait()
{
    LARGE_INTEGER CurrentQPC = { 0 };

    // Is this wait being called with the period that we consider it to be part of the same wait sequence
    QueryPerformanceCounter(&CurrentQPC);
    if (m_QPCValid && (CurrentQPC.QuadPart <= (m_LastWakeUpTime.QuadPart + (m_QPCFrequency.QuadPart * m_WaitSequenceTimeInSeconds)))) {
        // We are still in the same wait sequence, lets check if we should move to the next band
        if ((m_WaitBands[m_CurrentWaitBandIdx].WaitCount != WAIT_BAND_STOP) && (m_WaitCountInCurrentBand > m_WaitBands[m_CurrentWaitBandIdx].WaitCount)) {
            m_CurrentWaitBandIdx++;
            m_WaitCountInCurrentBand = 0;
        }
    }
    else {
        // Either we could not get the current time or we are starting a new wait sequence
        m_WaitCountInCurrentBand = 0;
        m_CurrentWaitBandIdx = 0;
    }

    // Sleep for the required period of time
    Sleep(m_WaitBands[m_CurrentWaitBandIdx].WaitTime);

    // Record the time we woke up so we can detect wait sequences
    QueryPerformanceCounter(&m_LastWakeUpTime);
    m_WaitCountInCurrentBand++;
}