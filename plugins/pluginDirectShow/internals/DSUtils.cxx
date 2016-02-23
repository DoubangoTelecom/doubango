/* Copyright (C) 2011-2013 Doubango Telecom <http://www.doubango.org>
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
#include "internals/DSUtils.h"

#if defined (_WIN32_WCE)
#include <atlbase.h>
#include <atlstr.h>
#else
#include <atlconv.h>
#include <d3d9.h>
#endif

#include "tsk_debug.h"

HWND GetMainWindow()
{
    HWND hWnd;
    if (!(hWnd = GetActiveWindow())) {
        if (!(hWnd = GetForegroundWindow())) {
#if !defined(_WIN32_WCE)
            if (!(hWnd = GetConsoleWindow())) {
                return NULL;
            }
#endif
        }
    }
    return hWnd;
}

bool IsMainThread()
{
    HWND hWnd = GetMainWindow();
    if (hWnd) {
        DWORD mainTid = GetWindowThreadProcessId(hWnd, NULL);
        DWORD currentTid = GetCurrentThreadId();
        return (mainTid == currentTid);
    }
    return false;
}

bool IsD3D9Supported()
{
#if defined(_WIN32_WCE)
    return false;
#else
    static bool g_bChecked = false;
    static bool g_bSupported = false;

    if (g_bChecked) {
        return g_bSupported;
    }
    g_bChecked = true;
    HRESULT hr = S_OK;
    IDirect3D9* pD3D = NULL;
    D3DDISPLAYMODE mode = { 0 };
    D3DPRESENT_PARAMETERS pp = {0};
    IDirect3DDevice9* pDevice = NULL;

    if (!(pD3D = Direct3DCreate9(D3D_SDK_VERSION))) {
        hr = E_OUTOFMEMORY;
        goto bail;
    }

    hr = pD3D->GetAdapterDisplayMode(
             D3DADAPTER_DEFAULT,
             &mode
         );
    if (FAILED(hr)) {
        goto bail;
    }

    hr = pD3D->CheckDeviceType(
             D3DADAPTER_DEFAULT,
             D3DDEVTYPE_HAL,
             mode.Format,
             D3DFMT_X8R8G8B8,
             TRUE    // windowed
         );
    if (FAILED(hr)) {
        goto bail;
    }
    pp.BackBufferFormat = D3DFMT_X8R8G8B8;
    pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
    pp.Windowed = TRUE;
    pp.hDeviceWindow = GetDesktopWindow();
    hr = pD3D->CreateDevice(
             D3DADAPTER_DEFAULT,
             D3DDEVTYPE_HAL,
             pp.hDeviceWindow,
             D3DCREATE_HARDWARE_VERTEXPROCESSING,
             &pp,
             &pDevice
         );
    if (FAILED(hr)) {
        goto bail;
    }

    // Everythings is OK
    g_bSupported = true;
    TSK_DEBUG_INFO("D3D9 supported");

bail:
    if (!g_bSupported) {
        TSK_DEBUG_WARN("D3D9 not supported");
    }
    SAFE_RELEASE(pDevice);
    SAFE_RELEASE(pD3D);
    return g_bSupported;
#endif /* _WIN32_WCE */
}

IPin *GetPin(IBaseFilter *filter, PIN_DIRECTION direction)
{
    IEnumPins	*enumPins = NULL;
    IPin		*pin = NULL;

    HRESULT hr = filter->EnumPins(&enumPins);
    if(!enumPins) {
        return NULL;
    }

    for(;;) {
        ULONG fetched = 0;
        PIN_DIRECTION pinDir = PIN_DIRECTION(-1);
        pin = NULL;

        if (FAILED(enumPins->Next(1, &pin, &fetched))) {
            enumPins->Release();
            return NULL;
        }

        if (fetched == 1 && pin) {
            pin->QueryDirection(&pinDir);
            if(pinDir == direction) {
                break;
            }
            pin->Release();
        }
    }

    enumPins->Release();
    return pin;
}

HRESULT ConnectFilters(IGraphBuilder *graphBuilder, IBaseFilter *source, IBaseFilter *destination, AM_MEDIA_TYPE *mediaType)
{
    HRESULT hr;

    IPin *outPin = GetPin(source, PINDIR_OUTPUT);
    IPin *inPin = GetPin(destination, PINDIR_INPUT);

    if (mediaType != NULL) {
        hr = graphBuilder->ConnectDirect(outPin, inPin, mediaType);
    }
    else {
        hr = graphBuilder->Connect(outPin, inPin);
    }

    SAFE_RELEASE(outPin);
    SAFE_RELEASE(inPin);

    return hr;
}

HRESULT DisconnectFilters(IGraphBuilder *graphBuilder, IBaseFilter *source, IBaseFilter *destination)
{
    HRESULT hr;

    IPin *outPin = GetPin(source, PINDIR_OUTPUT);
    IPin *inPin = GetPin(destination, PINDIR_INPUT);

    if (inPin) {
        hr = graphBuilder->Disconnect(inPin);
    }

    if (outPin) {
        hr = graphBuilder->Disconnect(outPin);
    }

    SAFE_RELEASE(outPin);
    SAFE_RELEASE(inPin);

    return hr;
}

bool DisconnectAllFilters(IGraphBuilder *graphBuilder)
{
    IEnumFilters* filterEnum = NULL;
    IBaseFilter* currentFilter = NULL;
    ULONG fetched;
    HRESULT hr;

    hr = graphBuilder->EnumFilters(&filterEnum);
    if (FAILED(hr)) {
        SAFE_RELEASE(filterEnum);
        return false;
    }

    while(filterEnum->Next(1, &currentFilter, &fetched) == S_OK) {
        hr = DisconnectFilters(graphBuilder, currentFilter, currentFilter);
        SAFE_RELEASE(currentFilter);
    }
    SAFE_RELEASE(filterEnum);
    SAFE_RELEASE(currentFilter);
    return true;
}

bool RemoveAllFilters(IGraphBuilder *graphBuilder)
{
    IEnumFilters* filterEnum = NULL;
    IBaseFilter* currentFilter = NULL;
    ULONG fetched;
    HRESULT hr;

    hr = graphBuilder->EnumFilters(&filterEnum);
    if (FAILED(hr)) {
        return false;
    }

    while(filterEnum->Next(1, &currentFilter, &fetched) == S_OK) {
        hr = graphBuilder->RemoveFilter(currentFilter);
        if (FAILED(hr)) {
            SAFE_RELEASE(filterEnum);
            return false;
        }
        SAFE_RELEASE(currentFilter);
        filterEnum->Reset();
    }

    SAFE_RELEASE(filterEnum);
    SAFE_RELEASE(currentFilter);
    return true;
}


#include "internals/DSDisplay.h"
#include "internals/DSGrabber.h"

#define WM_CREATE_DISPLAY_ON_UI_THREAD				(WM_USER + 101)
#define WM_CREATE_GRABBER_ON_UI_THREAD				(WM_CREATE_DISPLAY_ON_UI_THREAD + 1)
#define WM_CREATE_ON_UI_THREAD_TIMEOUT		1000

// C Callback that dispatch event to create display on UI thread
static LRESULT CALLBACK __create__WndProcWindow(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HANDLE* event = reinterpret_cast<HANDLE*>(wParam);
    BOOL* isScreenCast = reinterpret_cast<BOOL*>(GetProp(hWnd, TEXT("screnCast")));

    if(event && lParam) {
        switch(uMsg) {
        case WM_CREATE_DISPLAY_ON_UI_THREAD: {
            HRESULT hr;
            DSDisplay** ppDisplay = reinterpret_cast<DSDisplay**>(lParam);
            *ppDisplay = new DSDisplay(&hr);
            SetEvent(event);
            break;
        }
        case WM_CREATE_GRABBER_ON_UI_THREAD: {
            HRESULT hr;
            DSGrabber** ppGrabber = reinterpret_cast<DSGrabber**>(lParam);
            *ppGrabber = new DSGrabber(&hr, *isScreenCast);
            SetEvent(event);
            break;
        }
        }
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int createOnCurrentThead(HWND hWnd, void** ppRet, BOOL display, BOOL screnCast)
{
    HRESULT hr;
    if(display) {
        *ppRet = new DSDisplay(&hr);
    }
    else {
        *ppRet = new DSGrabber(&hr, screnCast);
    }
    if(FAILED(hr)) {
        TSK_DEBUG_ERROR("Failed to created DirectShow %s", display ? "Display" : "Grabber");
        SAFE_DELETE_PTR(*ppRet);
        return -2;
    }
    return 0;
}

int createOnUIThead(HWND hWnd, void** ppRet, BOOL display, BOOL screnCast)
{
    static BOOL __isScreenCastFalse = FALSE;
    static BOOL __isScreenCastTrue = TRUE;
    if(!ppRet) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if (IsMainThread()) {
        return createOnCurrentThead(hWnd, ppRet, display, screnCast);
    }
    else {
        TSK_DEBUG_INFO("Create DirectShow element on worker thread");
        HANDLE event = NULL;
        int ret = 0;
        DWORD retWait, retryCount = 3;

        if(!hWnd) {
            if (!(hWnd = FindWindow(NULL, TEXT("Boghe - IMS/RCS Client")))) {
                if(!(hWnd = GetMainWindow())) {
                    TSK_DEBUG_ERROR("No Window handle could be used");
                    return -2;
                }
            }
        }
#if defined(_WIN32_WCE)
        WNDPROC wndProc = (WNDPROC) SetWindowLong(hWnd, GWL_WNDPROC, (LONG) __create__WndProcWindow);
#else
        WNDPROC wndProc = (WNDPROC) SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR) __create__WndProcWindow);
#endif
        if (!wndProc) {
            TSK_DEBUG_ERROR("SetWindowLongPtr() failed with errcode=%d", GetLastError());
            return createOnCurrentThead(hWnd, ppRet, display, screnCast);
        }

        if (!(event = CreateEvent(NULL, TRUE, FALSE, NULL))) {
            TSK_DEBUG_ERROR("Failed to create new event");
            ret = -4;
            goto bail;
        }
        SetProp(hWnd, TEXT("screnCast"), screnCast ? &__isScreenCastTrue : &__isScreenCastFalse);
        if (!PostMessage(hWnd, display ? WM_CREATE_DISPLAY_ON_UI_THREAD : WM_CREATE_GRABBER_ON_UI_THREAD, reinterpret_cast<WPARAM>(event), reinterpret_cast<LPARAM>(ppRet))) {
            TSK_DEBUG_ERROR("PostMessageA() failed");
            ret = -5;
            goto bail;
        }

        do {
            retWait = WaitForSingleObject(event, WM_CREATE_ON_UI_THREAD_TIMEOUT);
        }
        while (retryCount-- > 0 && (retWait == WAIT_TIMEOUT));

bail:
        // restore
        if (hWnd && wndProc) {
#if defined(_WIN32_WCE)
            SetWindowLong(hWnd, GWL_WNDPROC, (LONG)wndProc);
#else
            SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)wndProc);
#endif
        }
        if (event) {
            CloseHandle(event);
        }

        return ret;
    }
}