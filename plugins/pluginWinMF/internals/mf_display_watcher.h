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
#ifndef PLUGIN_WIN_MF_DISPLAY_WATCHER_H
#define PLUGIN_WIN_MF_DISPLAY_WATCHER_H

#include "../plugin_win_mf_config.h"

#include <new>
#include <mfapi.h>
#include <mfidl.h>
#include <Mferror.h>
#include <shlwapi.h>
#include <Evr.h>

class DisplayWatcher
{
public:
    DisplayWatcher(HWND hWnd, IMFMediaSink* pMediaSink, HRESULT &hr);
    virtual ~DisplayWatcher();

public:
    HRESULT Start();
    HRESULT SetFullscreen(BOOL bEnabled);
    HRESULT SetHwnd(HWND hWnd);
    HRESULT Stop();

private:
    void UpdatePosition();
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    IMFVideoDisplayControl *m_pDisplayControl;
    HWND m_hWnd;
    WNDPROC m_pWndProc;
    BOOL m_bStarted;
    BOOL m_bFullScreen;
};

#endif /* PLUGIN_WIN_MF_DISPLAY_WATCHER_H */
