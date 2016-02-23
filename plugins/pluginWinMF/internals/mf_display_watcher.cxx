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
#include "mf_display_watcher.h"
#include "mf_utils.h"

#include "tsk_debug.h"

#include <assert.h>

DisplayWatcher::DisplayWatcher(HWND hWnd, IMFMediaSink* pMediaSink, HRESULT &hr)
    : m_pDisplayControl(NULL)
    , m_hWnd(hWnd)
    , m_pWndProc(NULL)
    , m_bStarted(FALSE)
    , m_bFullScreen(FALSE)
{
    IMFGetService *pService = NULL;

    CHECK_HR(hr = pMediaSink->QueryInterface(__uuidof(IMFGetService), (void**)&pService));
    CHECK_HR(hr = pService->GetService(MR_VIDEO_RENDER_SERVICE, __uuidof(IMFVideoDisplayControl), (void**)&m_pDisplayControl));
    CHECK_HR(hr = m_pDisplayControl->SetAspectRatioMode(MFVideoARMode_PreservePicture));
bail:
    SafeRelease(&pService);
}

DisplayWatcher::~DisplayWatcher()
{
    Stop();

    SafeRelease(&m_pDisplayControl);
}

HRESULT DisplayWatcher::Start()
{
    HRESULT hr = S_OK;
    HWND hWnd = m_hWnd; // save()
    CHECK_HR(hr = Stop());

    if((m_hWnd = hWnd) && m_pDisplayControl) {
        CHECK_HR(hr = m_pDisplayControl->SetVideoWindow(hWnd));

        BOOL ret = SetPropA(m_hWnd, "This", this);
        assert(ret);

#if _M_X64
        m_pWndProc = (WNDPROC)SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)DisplayWatcher::WndProc);
#else
        m_pWndProc = (WNDPROC)SetWindowLongPtr(m_hWnd, GWL_WNDPROC, (LONG)DisplayWatcher::WndProc);
#endif

        UpdatePosition(); // black screen if attached later
    }
    m_bStarted = TRUE;
bail:
    return hr;
}

HRESULT DisplayWatcher::SetFullscreen(BOOL bEnabled)
{
    if(m_pDisplayControl) {
        HRESULT hr =  m_pDisplayControl->SetFullscreen(bEnabled);
        m_bFullScreen = SUCCEEDED(hr);
        return hr;
    }

    return E_FAIL;
}

HRESULT DisplayWatcher::SetHwnd(HWND hWnd)
{
    BOOL bWasStarted = m_bStarted;
    Stop();
    m_hWnd = hWnd;
    if(bWasStarted) {
        return Start();
    }
    return S_OK;
}

HRESULT DisplayWatcher::Stop()
{
    if(m_hWnd && m_pWndProc) {
        // Restore

#if _M_X64
        SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)m_pWndProc);
#else
        SetWindowLongPtr(m_hWnd, GWL_WNDPROC, (LONG)m_pWndProc);
#endif
    }
    m_hWnd = NULL;
    m_pWndProc = NULL;
    m_bStarted = FALSE;
    return S_OK;
}

void DisplayWatcher::UpdatePosition()
{
    if(m_pDisplayControl && m_hWnd) {
        RECT rcDst = { 0, 0, 0, 0 };
        GetClientRect(m_hWnd, &rcDst);
        m_pDisplayControl->SetVideoPosition(NULL, &rcDst);
    }
}

LRESULT CALLBACK DisplayWatcher::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg) {
    case WM_CREATE:
    case WM_SIZE:
    case WM_MOVE: {
        DisplayWatcher* This = dynamic_cast<DisplayWatcher*>((DisplayWatcher*)GetPropA(hWnd, "This"));
        if(This) {
            This->UpdatePosition();
        }
        break;
    }

    case WM_CHAR:
    case WM_KEYUP: {
        DisplayWatcher* This = dynamic_cast<DisplayWatcher*>((DisplayWatcher*)GetPropA(hWnd, "This"));
        if(This) {
            if(This->m_bFullScreen && (wParam == 0x1B || wParam == VK_ESCAPE)) {
                This->SetFullscreen(FALSE);
            }
        }

        break;
    }
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}